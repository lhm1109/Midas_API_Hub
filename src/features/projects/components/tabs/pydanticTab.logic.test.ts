import { describe, expect, it } from 'vitest';
import { buildMinimalPayload, buildPydanticHeaders, buildPythonUsageExample, selectSpecSchema } from './pydanticTab.logic';

describe('selectSpecSchema', () => {
  it('accepts enhanced schemas stored as JSON strings', () => {
    const schema = { type: 'object', properties: { Argument: { type: 'object' } } };
    const selected = selectSpecSchema({
      jsonSchemaEnhanced: JSON.stringify(schema),
    });

    expect(selected.source).toBe('Enhanced Spec');
    expect(selected.schema).toEqual(schema);
  });

  it('accepts enhanced request/response bundles stored as objects', () => {
    const bundle = {
      request: { type: 'object', properties: { Argument: { type: 'object' } } },
      response: { type: 'object', properties: { OK: { type: 'boolean' } } },
    };
    const selected = selectSpecSchema({
      jsonSchemaEnhanced: bundle,
    });

    expect(selected.source).toBe('Enhanced Spec');
    expect(selected.schema).toBe(bundle);
  });

  it('falls back to original schema when enhanced/base schemas are empty', () => {
    const original = { type: 'object', properties: { VALUE: { type: 'number' } } };
    const selected = selectSpecSchema({
      jsonSchema: '{}',
      jsonSchemaOriginal: original,
    });

    expect(selected.source).toBe('Original Spec');
    expect(selected.schema).toBe(original);
  });
});

describe('buildPydanticHeaders', () => {
  it('merges common headers and MAPI-Key', () => {
    const result = buildPydanticHeaders({
      commonHeaders: JSON.stringify({ 'X-Custom': '1' }),
      mapiKey: 'secret',
    });

    expect(result.error).toBeNull();
    expect(result.headers).toMatchObject({
      Accept: 'application/json',
      'Content-Type': 'application/json',
      'X-Custom': '1',
      'MAPI-Key': 'secret',
    });
  });
});

describe('buildPythonUsageExample', () => {
  it('builds executable library-oriented Python code without embedding API secrets', () => {
    const code = buildPythonUsageExample({
      id: 'ope/analstatus/analysis-status',
      method: 'POST',
      path: '/OPE/ANALSTATUS',
    });

    expect(code).toContain('from generated_pydantic import ope_analstatus_analysis_status');
    expect(code).toContain('result = ope_analstatus_analysis_status(');
    expect(code).toContain('PYDANTIC_API_URL');
    expect(code).not.toContain('model_validate');
    expect(code).not.toContain('RequestModel');
    expect(code).not.toContain('"Argument"');
    expect(code).not.toContain('payload =');
    expect(code).not.toContain('MAPI-Key');
  });

  it('embeds a minimal valid payload from required schema fields', () => {
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
              enum: ['REACTIONG', 'REACTIONL'],
            },
          },
        },
      },
    };

    const code = buildPythonUsageExample({
      id: 'post-table-reaction-analysis-result-table',
      method: 'POST',
    }, schema);

    expect(code).toContain('table_type="REACTIONG"');
    expect(code).not.toContain('"Argument": {}');
  });

  it('omits single literal endpoint constants from SDK-style calls', () => {
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

    const code = buildPythonUsageExample({
      id: 'post-table-plate-stress-local-analysis-result-table',
      method: 'POST',
      productId: 'verification',
    }, schema);

    expect(code).toContain('from generated_pydantic import post_table_plate_stress_local_analysis_result_table');
    expect(code).not.toContain('table_type=');
    expect(code).not.toContain('PLATESTRESSL');
  });
});

describe('buildMinimalPayload', () => {
  it('uses request schema from enhanced bundles', () => {
    const payload = buildMinimalPayload({
      request: {
        type: 'object',
        required: ['Argument'],
        properties: {
          Argument: {
            type: 'object',
            required: ['TABLE_TYPE'],
            properties: {
              TABLE_TYPE: {
                type: 'string',
                enum: ['MEMB', 'PROP'],
              },
            },
          },
        },
      },
      response: {
        type: 'object',
      },
    });

    expect(payload).toEqual({
      Argument: {
        TABLE_TYPE: 'MEMB',
      },
    });
  });
});
