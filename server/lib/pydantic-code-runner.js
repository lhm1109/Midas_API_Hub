import { spawn } from 'child_process';
import path from 'path';

const DEFAULT_TIMEOUT_MS = 30000;

function parseStdout(stdout) {
  const trimmed = stdout.trim();
  if (!trimmed) return null;
  try {
    return JSON.parse(trimmed);
  } catch {
    return null;
  }
}

export function runPydanticCode({
  code,
  endpoint,
  method,
  url,
  headers = {},
  outputRoot = path.join(process.cwd(), 'generated_pydantic'),
  pythonCommand = process.env.PYTHON || process.env.PYTHON_BIN || 'python',
  timeoutMs = DEFAULT_TIMEOUT_MS,
}) {
  return new Promise((resolve) => {
    const startedAt = Date.now();
    const outputRootParent = path.dirname(outputRoot);
    const env = {
      ...process.env,
      PYDANTIC_OUTPUT_ROOT: outputRoot,
      PYDANTIC_ENDPOINT_ID: endpoint?.id || '',
      PYDANTIC_API_METHOD: String(method || endpoint?.method || 'POST').split(',')[0].trim().toUpperCase(),
      PYDANTIC_API_URL: String(url || ''),
      PYDANTIC_HEADERS: JSON.stringify(headers || {}),
      API_URL: String(url || ''),
      MAPI_KEY: headers?.['MAPI-Key'] || headers?.['mapi-key'] || '',
      PYTHONPATH: process.env.PYTHONPATH
        ? `${outputRootParent}${path.delimiter}${process.env.PYTHONPATH}`
        : outputRootParent,
    };

    const child = spawn(pythonCommand, ['-c', String(code || '')], {
      cwd: process.cwd(),
      env,
      stdio: ['ignore', 'pipe', 'pipe'],
      windowsHide: true,
    });

    let stdout = '';
    let stderr = '';
    let settled = false;

    const finish = (result) => {
      if (settled) return;
      settled = true;
      clearTimeout(timeout);
      resolve({
        ...result,
        time: Date.now() - startedAt,
      });
    };

    const timeout = setTimeout(() => {
      child.kill();
      finish({
        ok: false,
        exitCode: null,
        signal: 'timeout',
        stdout,
        stderr: `Python execution timed out after ${timeoutMs}ms.`,
        parsedStdout: parseStdout(stdout),
      });
    }, timeoutMs);

    child.stdout.on('data', (chunk) => {
      stdout += chunk.toString();
    });
    child.stderr.on('data', (chunk) => {
      stderr += chunk.toString();
    });
    child.on('error', (error) => {
      finish({
        ok: false,
        exitCode: null,
        signal: null,
        stdout,
        stderr: `Failed to start Python: ${error.message}`,
        parsedStdout: parseStdout(stdout),
      });
    });
    child.on('close', (exitCode, signal) => {
      finish({
        ok: exitCode === 0,
        exitCode,
        signal,
        stdout,
        stderr,
        parsedStdout: parseStdout(stdout),
      });
    });
  });
}

export const internals = {
  parseStdout,
};
