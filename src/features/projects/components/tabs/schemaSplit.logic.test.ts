import { describe, expect, it } from 'vitest';
import { computeSplitFromParsed } from './schemaSplit.logic';

const collectRefs = (value: any): string[] => {
  const refs: string[] = [];

  const visit = (node: any) => {
    if (!node || typeof node !== 'object') return;
    if (Array.isArray(node)) {
      node.forEach(visit);
      return;
    }
    if (typeof node.$ref === 'string') {
      refs.push(node.$ref);
    }
    Object.values(node).forEach(visit);
  };

  visit(value);
  return refs;
};

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

  it('dereferences nested child refs and exposes nested slices for map-body request/response splits', () => {
    const schema = {
      components: {
        schemas: {
          DTO_STCT_M1_REQUEST_MAP_BODY: {
            type: 'object',
            properties: {
              Assign: {
                type: 'object',
                additionalProperties: {
                  $ref: '#/components/schemas/DTO_STCT_M1_ITEM',
                },
              },
            },
          },
          DTO_STCT_M1_RESPONSE_MAP_BODY: {
            type: 'object',
            properties: {
              'STCT-M1': {
                type: 'object',
                additionalProperties: {
                  $ref: '#/components/schemas/DTO_STCT_M1_ITEM',
                },
              },
            },
          },
          DTO_STCT_M1_ITEM: {
            type: 'object',
            allOf: [
              { $ref: '#/components/schemas/DTO_STCT_M1_UI_STAGE_HEADER' },
              { $ref: '#/components/schemas/DTO_STCT_M1_UI_NONLINEAR_CONTROL' },
            ],
          },
          DTO_STCT_M1_UI_STAGE_HEADER: {
            type: 'object',
            properties: {
              bLAST_FINAL: {
                type: 'boolean',
              },
              RESTART_CS_ANAL: {
                $ref: '#/components/schemas/DTO_STCT_M1_RESTART_CS_ANAL',
              },
            },
          },
          DTO_STCT_M1_RESTART_CS_ANAL: {
            type: 'object',
            properties: {
              OPT_USE: {
                type: 'boolean',
              },
            },
          },
          DTO_STCT_M1_UI_NONLINEAR_CONTROL: {
            type: 'object',
            properties: {
              NONL_CONTROL: {
                type: 'object',
                properties: {
                  ADVANCED: {
                    $ref: '#/components/schemas/DTO_STCT_M1_UI_NONLINEAR_CONTROL_ADVANCED',
                  },
                  DISP: {
                    $ref: '#/components/schemas/DTO_STCT_M1_UI_NORM_ITEM',
                  },
                },
              },
            },
          },
          DTO_STCT_M1_UI_NONLINEAR_CONTROL_ADVANCED: {
            type: 'object',
            properties: {
              LINE_SEARCH: {
                $ref: '#/components/schemas/DTO_STCT_M1_UI_NONLINEAR_CONTROL_ADVANCED_LINE_SEARCH',
              },
            },
          },
          DTO_STCT_M1_UI_NONLINEAR_CONTROL_ADVANCED_LINE_SEARCH: {
            type: 'object',
            properties: {
              OPT_USE: {
                type: 'boolean',
              },
            },
          },
          DTO_STCT_M1_UI_NORM_ITEM: {
            type: 'object',
            properties: {
              VALUE: {
                type: 'number',
              },
            },
          },
        },
      },
    };

    const result = computeSplitFromParsed(schema);

    expect(collectRefs(result.requestSchema)).toEqual([]);
    expect(collectRefs(result.responseSchema)).toEqual([]);

    const requestRefNames = result.requestSlices
      .map((slice) => slice.refName)
      .filter((value): value is string => typeof value === 'string');

    expect(requestRefNames).toContain('DTO_STCT_M1_ITEM');
    expect(requestRefNames).toContain('DTO_STCT_M1_UI_STAGE_HEADER');
    expect(requestRefNames).toContain('DTO_STCT_M1_RESTART_CS_ANAL');
    expect(requestRefNames).toContain('DTO_STCT_M1_UI_NONLINEAR_CONTROL');
    expect(requestRefNames).toContain('DTO_STCT_M1_UI_NONLINEAR_CONTROL_ADVANCED');
    expect(requestRefNames).toContain('DTO_STCT_M1_UI_NONLINEAR_CONTROL_ADVANCED_LINE_SEARCH');
    expect(requestRefNames).toContain('DTO_STCT_M1_UI_NORM_ITEM');

    const advancedSlice = result.requestSlices.find(
      (slice) => slice.refName === 'DTO_STCT_M1_UI_NONLINEAR_CONTROL_ADVANCED'
    );
    expect(advancedSlice).toBeDefined();
    expect(collectRefs(advancedSlice?.schema)).toEqual([]);

    const restartSlice = result.requestSlices.find(
      (slice) => slice.refName === 'DTO_STCT_M1_RESTART_CS_ANAL'
    );
    expect(restartSlice?.schema?.properties?.OPT_USE?.type).toBe('boolean');

    expect(result.responseSlices.some((slice) => slice.refName === 'DTO_STCT_M1_UI_NORM_ITEM')).toBe(true);
  });

  it('splits namespaced argument and response schemas that share an item ref', () => {
    const schema = {
      components: {
        schemas: {
          AASHTO_LRFD24: {
            DTO_LCOM_CONC_ARGUMENT: {
              type: 'object',
              additionalProperties: false,
              required: ['Argument'],
              properties: {
                Argument: {
                  $ref: '#/components/schemas/AASHTO_LRFD24/DTO_LCOM_CONC_ITEM',
                },
              },
            },
            DTO_LCOM_CONC_RESPONSE: {
              type: 'object',
              additionalProperties: false,
              required: ['LCOM-CONC'],
              properties: {
                'LCOM-CONC': {
                  $ref: '#/components/schemas/AASHTO_LRFD24/DTO_LCOM_CONC_ITEM',
                },
              },
            },
            DTO_LCOM_CONC_ITEM: {
              type: 'object',
              additionalProperties: false,
              required: ['DGNCODE', 'LOAD_CASE_TYPE'],
              properties: {
                DGNCODE: {
                  type: 'string',
                  enum: ['AASHTO_LRFD24'],
                },
                LOAD_CASE_TYPE: {
                  type: 'string',
                  enum: ['ST_ONLY', 'CS_ONLY', 'ST_CS'],
                },
              },
            },
          },
        },
      },
    };

    const result = computeSplitFromParsed(schema);

    expect(result.requestKey).toBe('AASHTO_LRFD24/DTO_LCOM_CONC_ARGUMENT');
    expect(result.responseKey).toBe('AASHTO_LRFD24/DTO_LCOM_CONC_RESPONSE');
    expect(collectRefs(result.requestSchema)).toEqual([]);
    expect(collectRefs(result.responseSchema)).toEqual([]);
    expect(result.requestSchema.properties.Argument.properties.DGNCODE.enum).toEqual(['AASHTO_LRFD24']);
    expect(result.responseSchema.properties['LCOM-CONC'].properties.LOAD_CASE_TYPE.enum).toEqual([
      'ST_ONLY',
      'CS_ONLY',
      'ST_CS',
    ]);
    expect(result.requestSlices.some((slice) => slice.refName === 'AASHTO_LRFD24/DTO_LCOM_CONC_ITEM')).toBe(true);
    expect(result.responseSlices.some((slice) => slice.refName === 'AASHTO_LRFD24/DTO_LCOM_CONC_ITEM')).toBe(true);
  });
});
