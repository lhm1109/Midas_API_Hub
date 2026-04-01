import { resolve } from 'node:path';
import { beforeAll, describe, expect, it, vi } from 'vitest';
import type { EnhancedField } from './schemaCompiler';
import { applyConditionalRequiredToField, compileSchema, flattenComposedObjectSchema } from './schemaCompiler';
import { initSchemaLogicRules } from './schemaLogicEngine';

describe('applyConditionalRequiredToField', () => {
  it('marks only newly injected allOf-required fields as _injectedRequiredWhen', () => {
    const standardCodeField: EnhancedField = {
      key: 'CONCRETE.STANDARD_CODE',
      type: 'string',
      required: {},
      section: '',
      validationLayers: [],
      'x-required-when': { CODE: 'Standard' },
    };

    applyConditionalRequiredToField(
      standardCodeField,
      {
        STANDARD_CODE: { CODE: 'Standard' },
      },
      'STANDARD_CODE',
      'CONCRETE.STANDARD_CODE'
    );

    expect((standardCodeField as any)._injectedRequiredWhen).not.toBe(true);
    expect((standardCodeField as any)['x-required-when']).toEqual({ CODE: 'Standard' });

    const fcField: EnhancedField = {
      key: 'CONCRETE.FC',
      type: 'number',
      required: {},
      section: '',
      validationLayers: [],
    };

    applyConditionalRequiredToField(
      fcField,
      {
        FC: { CODE: 'None' },
      },
      'FC',
      'CONCRETE.FC'
    );

    expect((fcField as any)._injectedRequiredWhen).toBe(true);
    expect((fcField as any)['x-required-when']).toEqual({ CODE: 'None' });
  });
});

describe('flattenComposedObjectSchema', () => {
  it('hoists object fragments in allOf into direct properties and preserves conditional rules', () => {
    const schema = {
      type: 'object',
      required: ['bLAST_FINAL'],
      allOf: [
        {
          type: 'object',
          properties: {
            bLAST_FINAL: {
              type: 'boolean',
            },
            FINAL_STAGE: {
              type: 'string',
            },
          },
          allOf: [
            {
              if: {
                properties: {
                  bLAST_FINAL: {
                    const: false,
                  },
                },
                required: ['bLAST_FINAL'],
              },
              then: {
                required: ['FINAL_STAGE'],
              },
            },
          ],
        },
        {
          type: 'object',
          properties: {
            ANAL_TYPE: {
              type: 'object',
              properties: {
                iINC_NLA: {
                  type: 'integer',
                },
              },
            },
          },
        },
        {
          if: {
            properties: {
              ANAL_TYPE: {
                required: ['iINC_NLA'],
              },
            },
            required: ['ANAL_TYPE'],
          },
          then: {
            not: {
              required: ['TIME_DEP_CONTROL'],
            },
          },
        },
      ],
    };

    const flattened = flattenComposedObjectSchema(schema);

    expect(flattened.properties?.bLAST_FINAL?.type).toBe('boolean');
    expect(flattened.properties?.FINAL_STAGE?.type).toBe('string');
    expect(flattened.properties?.ANAL_TYPE?.type).toBe('object');
    expect(flattened.required).toContain('bLAST_FINAL');
    expect(flattened.allOf).toHaveLength(2);
    expect(flattened.allOf?.[0]?.then?.required).toEqual(['FINAL_STAGE']);
    expect(flattened.allOf?.[1]?.then?.not?.required).toEqual(['TIME_DEP_CONTROL']);
  });
});

describe('compileSchema map-wrapper oneOf', () => {
  beforeAll(async () => {
    vi.stubGlobal('window', {
      location: { search: '' },
    });

    vi.stubGlobal('fetch', async (input: string | URL) => {
      const normalizedPath = String(input).replace(/^\//, '');
      const filePath = resolve(process.cwd(), normalizedPath);
      const body = await (await import('node:fs/promises')).readFile(filePath, 'utf-8');
      return new Response(body, { status: 200 });
    });

    await initSchemaLogicRules('civil_gen_definition', 'enhanced');
  });

  it('compiles Assign.additionalProperties.oneOf schemas into non-empty sections', () => {
    const schema = {
      type: 'object',
      additionalProperties: false,
      required: ['Assign'],
      properties: {
        Assign: {
          type: 'object',
          description: 'Plastic material keyed map',
          additionalProperties: {
            oneOf: [
              {
                type: 'object',
                required: ['NAME', 'MODEL_TYPE', 'TRESCA'],
                properties: {
                  NAME: {
                    type: 'string',
                    description: 'Name',
                  },
                  MODEL_TYPE: {
                    allOf: [
                      {
                        type: 'integer',
                        enum: [0, 1],
                      },
                      {
                        const: 0,
                      },
                    ],
                    description: 'Model Type (Tresca:0)',
                  },
                  TRESCA: {
                    type: 'object',
                    description: 'TRESCA Model',
                    properties: {
                      INIT_YIELD_STRESS: {
                        type: 'number',
                        description: 'Initial yield stress',
                      },
                    },
                    required: ['INIT_YIELD_STRESS'],
                  },
                },
              },
              {
                type: 'object',
                required: ['NAME', 'MODEL_TYPE', 'VMISES'],
                properties: {
                  NAME: {
                    type: 'string',
                    description: 'Name',
                  },
                  MODEL_TYPE: {
                    allOf: [
                      {
                        type: 'integer',
                        enum: [0, 1],
                      },
                      {
                        const: 1,
                      },
                    ],
                    description: 'Model Type (Von Mises:1)',
                  },
                  VMISES: {
                    type: 'object',
                    description: 'Von Mises Model',
                    properties: {
                      INIT_YIELD_STRESS: {
                        type: 'number',
                        description: 'Initial yield stress',
                      },
                    },
                    required: ['INIT_YIELD_STRESS'],
                  },
                },
              },
            ],
          },
        },
      },
    };

    const sections = compileSchema(schema as any, 'civil_gen_definition', 'enhanced');
    const compiled = JSON.stringify(sections);
    const trescaSectionCount = (compiled.match(/"section":"TRESCA Model"/g) || []).length;
    const vmisesSectionCount = (compiled.match(/"section":"Von Mises Model"/g) || []).length;

    expect(sections.length).toBeGreaterThan(0);
    expect(compiled).toContain('TRESCA Model');
    expect(compiled).toContain('Von Mises Model');
    expect(compiled).toContain('INIT_YIELD_STRESS');
    expect(trescaSectionCount).toBe(1);
    expect(vmisesSectionCount).toBe(1);
  });

  it('preserves deep nested object children without introducing array markers', () => {
    const schema = {
      type: 'object',
      required: ['EXPORT_PATH', 'RESULT_GRAPHIC'],
      properties: {
        EXPORT_PATH: {
          type: 'string',
          description: 'path',
        },
        RESULT_GRAPHIC: {
          type: 'object',
          properties: {
            TYPE_OF_DISPLAY: {
              type: 'object',
              properties: {
                REINFORCEMENT: {
                  type: 'object',
                  properties: {
                    DISPLAY_MEMBERS: {
                      type: 'object',
                      properties: {
                        BEAM: { type: 'boolean', default: true },
                        COLUMN: { type: 'boolean', default: true },
                        BRACE: { type: 'boolean', default: true },
                        WALL: { type: 'boolean', default: true },
                      },
                    },
                  },
                },
              },
            },
          },
        },
      },
    };

    const sections = compileSchema(schema as any, 'civil_gen_definition', 'enhanced');
    const compiled = JSON.stringify(sections);
    const resultGraphic = sections[0]?.fields.find((field) => field.key === 'RESULT_GRAPHIC');
    const displayMembers =
      resultGraphic?.children?.[0]?.children?.[0]?.children?.[0];

    expect(compiled).toContain('DISPLAY_MEMBERS.BEAM');
    expect(compiled).toContain('DISPLAY_MEMBERS.WALL');
    expect(compiled).not.toContain('RESULT_GRAPHIC[].TYPE_OF_DISPLAY');
    expect(displayMembers?.key).toBe('RESULT_GRAPHIC.TYPE_OF_DISPLAY.REINFORCEMENT.DISPLAY_MEMBERS');
    expect(displayMembers?.children?.map((child) => child.key)).toEqual([
      'RESULT_GRAPHIC.TYPE_OF_DISPLAY.REINFORCEMENT.DISPLAY_MEMBERS.BEAM',
      'RESULT_GRAPHIC.TYPE_OF_DISPLAY.REINFORCEMENT.DISPLAY_MEMBERS.COLUMN',
      'RESULT_GRAPHIC.TYPE_OF_DISPLAY.REINFORCEMENT.DISPLAY_MEMBERS.BRACE',
      'RESULT_GRAPHIC.TYPE_OF_DISPLAY.REINFORCEMENT.DISPLAY_MEMBERS.WALL',
    ]);
  });
});
