import { describe, expect, it } from 'vitest';
import { runPydanticEndpoint } from './pydantic-runner.js';

function response(body, init = {}) {
  return {
    ok: init.ok ?? true,
    status: init.status ?? 200,
    statusText: init.statusText ?? 'OK',
    text: async () => body,
  };
}

describe('runPydanticEndpoint', () => {
  it('does not call the API when the saved model is missing', async () => {
    let fetchCalled = false;
    const result = await runPydanticEndpoint({
      endpoint: { id: 'missing/model', method: 'POST' },
      method: 'POST',
      url: 'https://example.test/api',
      requestBody: '{}',
      fetchImpl: async () => {
        fetchCalled = true;
        return response('{}');
      },
      runPythonValidation: async () => ({
        status: 'missing_model',
        valid: false,
        message: 'Save .py first.',
      }),
    });

    expect(fetchCalled).toBe(false);
    expect(result.stage).toBe('request_validation');
    expect(result.requestValidation.status).toBe('missing_model');
  });

  it('does not call the API when request validation fails', async () => {
    let fetchCalled = false;
    const result = await runPydanticEndpoint({
      endpoint: { id: 'endpoint', method: 'POST' },
      method: 'POST',
      url: 'https://example.test/api',
      requestBody: '{"BAD":true}',
      fetchImpl: async () => {
        fetchCalled = true;
        return response('{}');
      },
      runPythonValidation: async () => ({
        status: 'invalid',
        valid: false,
        modelName: 'EndpointRequest',
        errors: [{ loc: ['Argument'], msg: 'Field required' }],
      }),
    });

    expect(fetchCalled).toBe(false);
    expect(result.ok).toBe(false);
    expect(result.requestValidation.status).toBe('invalid');
  });

  it('returns response validation errors after the API call', async () => {
    const phases = [];
    const result = await runPydanticEndpoint({
      endpoint: { id: 'endpoint', method: 'POST' },
      method: 'POST',
      url: 'https://example.test/api',
      headers: { 'MAPI-Key': 'secret' },
      requestBody: '{"Argument":{}}',
      fetchImpl: async (_url, options) => {
        expect(options.headers['MAPI-Key']).toBe('secret');
        expect(JSON.parse(options.body)).toEqual({ Argument: {} });
        return response('{"BAD":true}');
      },
      runPythonValidation: async ({ phase, payload }) => {
        phases.push(phase);
        if (phase === 'request') {
          return { status: 'valid', valid: true, modelName: 'EndpointRequest', payload };
        }
        return {
          status: 'invalid',
          valid: false,
          modelName: 'EndpointResponse',
          errors: [{ loc: ['OK'], msg: 'Field required' }],
        };
      },
    });

    expect(phases).toEqual(['request', 'response']);
    expect(result.ok).toBe(false);
    expect(result.http?.status).toBe(200);
    expect(result.responseValidation?.status).toBe('invalid');
  });

  it('returns ok when request, API, and response validation pass', async () => {
    const result = await runPydanticEndpoint({
      endpoint: { id: 'endpoint', method: 'POST' },
      method: 'POST',
      url: 'https://example.test/api',
      requestBody: '{"Argument":{}}',
      fetchImpl: async () => response('{"OK":true}'),
      runPythonValidation: async ({ phase, payload }) => ({
        status: 'valid',
        valid: true,
        modelName: phase === 'request' ? 'EndpointRequest' : 'EndpointResponse',
        payload,
      }),
    });

    expect(result.ok).toBe(true);
    expect(result.requestValidation.status).toBe('valid');
    expect(result.responseValidation?.status).toBe('valid');
    expect(result.responseBody).toBe(JSON.stringify({ OK: true }, null, 2));
  });
});
