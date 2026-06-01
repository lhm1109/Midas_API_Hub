import { describe, expect, it } from 'vitest';
import { generatePydanticCode } from './pydantic-generator.js';

describe('generatePydanticCode', () => {
  it('adds endpoint SDK helpers to generated modules', () => {
    const schema = {
      type: 'object',
      required: ['Argument'],
      properties: {
        Argument: {
          type: 'object',
          required: ['TABLE_TYPE'],
          properties: {
            TABLE_TYPE: {
              type: 'string',
              enum: ['PLATESTRESSL'],
            },
          },
        },
      },
    };

    const result = generatePydanticCode({
      schema,
      endpoint: {
        id: 'post-table-plate-stress-local-analysis-result-table',
        name: 'Plate Stress Local',
        method: 'POST',
        path: '/post/TABLE',
      },
    });

    expect(result.code).toContain('ENDPOINT_ID = "post-table-plate-stress-local-analysis-result-table"');
    expect(result.code).toContain('DEFAULT_METHOD = "POST"');
    expect(result.code).toContain('from generated_pydantic.client import endpoint as _endpoint');
    expect(result.code).toContain('build_payload = _command.build_payload');
    expect(result.code).toContain('run = _command.run');
    expect(result.code).toContain('__all__ = ["PlateStressLocalRequest"]');
  });
});
