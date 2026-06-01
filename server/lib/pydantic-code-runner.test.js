import { describe, expect, it } from 'vitest';
import { runPydanticCode } from './pydantic-code-runner.js';

describe('runPydanticCode', () => {
  it('executes Python code with endpoint/API environment variables', async () => {
    const result = await runPydanticCode({
      code: [
        'import json',
        'import os',
        'print(json.dumps({',
        '  "endpoint": os.environ["PYDANTIC_ENDPOINT_ID"],',
        '  "url": os.environ["PYDANTIC_API_URL"],',
        '  "method": os.environ["PYDANTIC_API_METHOD"],',
        '  "mapi": os.environ["MAPI_KEY"],',
        '}))',
      ].join('\n'),
      endpoint: { id: 'sample/endpoint', method: 'POST' },
      method: 'PUT',
      url: 'https://example.test/api',
      headers: { 'MAPI-Key': 'secret' },
    });

    expect(result.ok).toBe(true);
    expect(result.parsedStdout).toEqual({
      endpoint: 'sample/endpoint',
      url: 'https://example.test/api',
      method: 'PUT',
      mapi: 'secret',
    });
  });

  it('returns stderr and non-zero exit code for Python failures', async () => {
    const result = await runPydanticCode({
      code: 'raise RuntimeError("boom")',
      endpoint: { id: 'sample/endpoint', method: 'POST' },
      method: 'POST',
      url: 'https://example.test/api',
    });

    expect(result.ok).toBe(false);
    expect(result.exitCode).not.toBe(0);
    expect(result.stderr).toContain('RuntimeError: boom');
  });
});
