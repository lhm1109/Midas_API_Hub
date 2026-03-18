import { describe, expect, it } from 'vitest';
import { computeSplitFromParsed } from './schemaSplit.logic';

describe('computeSplitFromParsed', () => {
  it('extracts per-ref slices from map body item allOf blocks', () => {
    const schema = {
      components: {
        schemas: {
          DTO_STCT_REQUEST_MAP_BODY: {
            type: 'object',
            properties: {
              Assign: {
                type: 'object',
                additionalProperties: {
                  $ref: '#/components/schemas/DTO_STCT_ITEM',
                },
              },
            },
          },
          DTO_STCT_RESPONSE_MAP_BODY: {
            type: 'object',
            properties: {
              STCT: {
                type: 'object',
                additionalProperties: {
                  $ref: '#/components/schemas/DTO_STCT_ITEM',
                },
              },
            },
          },
          DTO_STCT_ITEM: {
            type: 'object',
            allOf: [
              { $ref: '#/components/schemas/DTO_STCT_UI_STAGE_HEADER' },
              { $ref: '#/components/schemas/DTO_STCT_UI_ANALYSIS_SELECTOR' },
              {
                description: 'Inline nonlinear rule block',
                if: {
                  properties: {
                    iINC_NLA: { const: 0 },
                  },
                },
                then: {
                  not: {
                    required: ['bSAVE_OCS'],
                  },
                },
              },
            ],
          },
          DTO_STCT_UI_STAGE_HEADER: {
            type: 'object',
            properties: {
              bLAST_FINAL: {
                type: 'boolean',
              },
            },
          },
          DTO_STCT_UI_ANALYSIS_SELECTOR: {
            type: 'object',
            properties: {
              iINC_NLA: {
                type: 'integer',
                enum: [0, 1, 2, 3],
              },
            },
          },
        },
      },
    };

    const result = computeSplitFromParsed(schema);

    expect(result.requestKey).toBe('DTO_STCT_REQUEST_MAP_BODY');
    expect(result.responseKey).toBe('DTO_STCT_RESPONSE_MAP_BODY');
    expect(result.requestSlices[0].label).toBe('Full');

    const requestRefNames = result.requestSlices
      .map((slice) => slice.refName)
      .filter((value): value is string => typeof value === 'string');

    expect(requestRefNames).toContain('DTO_STCT_ITEM');
    expect(requestRefNames).toContain('DTO_STCT_UI_STAGE_HEADER');
    expect(requestRefNames).toContain('DTO_STCT_UI_ANALYSIS_SELECTOR');

    const inlineRule = result.requestSlices.find((slice) => slice.kind === 'inline');
    expect(inlineRule).toBeDefined();
    expect(inlineRule?.description).toBe('Inline nonlinear rule block');

    const entrySlice = result.requestSlices.find((slice) => slice.kind === 'entry');
    expect(entrySlice?.schema?.allOf).toHaveLength(3);

    const stageHeaderSlice = result.requestSlices.find((slice) => slice.refName === 'DTO_STCT_UI_STAGE_HEADER');
    expect(stageHeaderSlice?.schema?.properties?.bLAST_FINAL?.type).toBe('boolean');

    expect(result.responseSlices.some((slice) => slice.refName === 'DTO_STCT_ITEM')).toBe(true);
  });
});
