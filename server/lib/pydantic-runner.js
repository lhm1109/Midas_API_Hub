import { spawn } from 'child_process';
import path from 'path';

const PYTHON_VALIDATOR = String.raw`
import json
import sys
from pathlib import Path

def write_result(value):
    sys.stdout.write(json.dumps(value, ensure_ascii=False))

try:
    payload = json.load(sys.stdin)
    endpoint_id = payload.get("endpointId") or ""
    phase = payload.get("phase") or "request"
    data = payload.get("payload")
    output_root = Path(payload.get("outputRoot") or "generated_pydantic").resolve()
    sys.path.insert(0, str(output_root.parent))

    from pydantic import ValidationError
    from generated_pydantic import get_models

    try:
        models = get_models(endpoint_id)
    except KeyError:
        write_result({
            "status": "missing_model",
            "valid": False,
            "message": "Save .py first. No generated_pydantic model is registered for this endpoint.",
        })
        raise SystemExit(0)

    suffix = "Response" if phase == "response" else "Request"
    model_name = next((name for name in models if name.endswith(suffix)), None)
    if not model_name:
        write_result({
            "status": "skipped",
            "valid": True,
            "message": f"No {suffix} model is registered for this endpoint.",
            "payload": data,
        })
        raise SystemExit(0)

    model_cls = models[model_name]
    try:
        model = model_cls.model_validate(data)
        write_result({
            "status": "valid",
            "valid": True,
            "modelName": model_name,
                "payload": model.model_dump(by_alias=True, mode="json", exclude_none=True),
        })
    except ValidationError as error:
        write_result({
            "status": "invalid",
            "valid": False,
            "modelName": model_name,
            "errors": error.errors(),
            "message": str(error),
        })
except SystemExit:
    raise
except Exception as error:
    write_result({
        "status": "error",
        "valid": False,
        "message": str(error),
    })
`;

const DEFAULT_TIMEOUT_MS = 30000;

function parseJsonBody(value, fieldName) {
  if (value === undefined || value === null || value === '') return {};
  if (typeof value === 'string') {
    try {
      return JSON.parse(value);
    } catch (error) {
      throw new Error(`${fieldName} is not valid JSON: ${error.message}`);
    }
  }
  if (typeof value === 'object') return value;
  throw new Error(`${fieldName} must be a JSON object or JSON string`);
}

function formatResponseBody(text) {
  try {
    return {
      text: JSON.stringify(JSON.parse(text), null, 2),
      json: JSON.parse(text),
      isJson: true,
    };
  } catch {
    return {
      text,
      json: null,
      isJson: false,
    };
  }
}

function runPythonValidationProcess({
  endpointId,
  phase,
  payload,
  outputRoot,
  pythonCommand = process.env.PYTHON || process.env.PYTHON_BIN || 'python',
  timeoutMs = DEFAULT_TIMEOUT_MS,
}) {
  return new Promise((resolve) => {
    const child = spawn(pythonCommand, ['-c', PYTHON_VALIDATOR], {
      cwd: process.cwd(),
      stdio: ['pipe', 'pipe', 'pipe'],
      windowsHide: true,
    });

    let stdout = '';
    let stderr = '';
    const timeout = setTimeout(() => {
      child.kill();
      resolve({
        status: 'error',
        valid: false,
        message: `Python validation timed out after ${timeoutMs}ms.`,
      });
    }, timeoutMs);

    child.stdout.on('data', (chunk) => {
      stdout += chunk.toString();
    });
    child.stderr.on('data', (chunk) => {
      stderr += chunk.toString();
    });
    child.on('error', (error) => {
      clearTimeout(timeout);
      resolve({
        status: 'error',
        valid: false,
        message: `Failed to start Python: ${error.message}`,
      });
    });
    child.on('close', () => {
      clearTimeout(timeout);
      try {
        resolve(JSON.parse(stdout || '{}'));
      } catch {
        resolve({
          status: 'error',
          valid: false,
          message: stderr || stdout || 'Python validation did not return JSON.',
        });
      }
    });

    child.stdin.end(JSON.stringify({ endpointId, phase, payload, outputRoot }));
  });
}

export async function runPydanticEndpoint({
  endpoint,
  method,
  url,
  headers = {},
  requestBody,
  outputRoot = path.join(process.cwd(), 'generated_pydantic'),
  fetchImpl = globalThis.fetch,
  runPythonValidation = runPythonValidationProcess,
}) {
  const endpointId = endpoint?.id || '';
  const httpMethod = String(method || endpoint?.method || 'POST').split(',')[0].trim().toUpperCase() || 'POST';
  const targetUrl = String(url || '').trim();
  if (!targetUrl) {
    throw new Error('url is required');
  }

  const parsedRequest = parseJsonBody(requestBody, 'requestBody');
  const requestValidation = await runPythonValidation({
    endpointId,
    phase: 'request',
    payload: parsedRequest,
    outputRoot,
  });

  if (!requestValidation?.valid) {
    return {
      ok: false,
      stage: 'request_validation',
      requestValidation,
      responseValidation: null,
      http: null,
      responseBody: '',
      responseIsJson: false,
    };
  }

  const startedAt = Date.now();
  const response = await fetchImpl(targetUrl, {
    method: httpMethod,
    headers,
    body: httpMethod === 'GET' ? undefined : JSON.stringify(requestValidation.payload ?? parsedRequest),
  });
  const responseText = await response.text();
  const elapsedMs = Date.now() - startedAt;
  const formatted = formatResponseBody(responseText);

  let responseValidation = {
    status: 'skipped',
    valid: true,
    message: formatted.isJson ? 'No response validation was run.' : 'Response is not JSON.',
    payload: formatted.json,
  };

  if (formatted.isJson) {
    responseValidation = await runPythonValidation({
      endpointId,
      phase: 'response',
      payload: formatted.json,
      outputRoot,
    });
  }

  return {
    ok: Boolean(response.ok && requestValidation.valid && responseValidation?.valid),
    stage: 'complete',
    requestValidation,
    responseValidation,
    http: {
      ok: response.ok,
      status: response.status,
      statusText: response.statusText,
      time: elapsedMs,
    },
    responseBody: formatted.text,
    responseIsJson: formatted.isJson,
  };
}

export const internals = {
  parseJsonBody,
  formatResponseBody,
  runPythonValidationProcess,
};
