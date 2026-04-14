import { resolve } from 'node:path';
import { beforeAll, describe, expect, it, vi } from 'vitest';

import { initSchemaLogicRules } from './schemaLogicEngine';
import { generateHTMLDocument } from './tableGenerator';

describe('generateHTMLDocument legacy rowspan', () => {
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

  it('matches the rendered descendant row count for NONL_CONTROL in stct schema', () => {
    const schema = {
      type: 'object',
      required: ['Assign'],
      additionalProperties: false,
      properties: {
        Assign: {
          type: 'object',
          patternProperties: {
            '^[0-9]+$': {
              title: 'Rowspan Regression',
              type: 'object',
              additionalProperties: false,
              properties: {
                NONL_CONTROL: {
                  type: 'object',
                  description: 'Nonlinear control block',
                  additionalProperties: false,
                  properties: {
                    ADVANCED: {
                      type: 'object',
                      description: 'Advanced settings',
                      additionalProperties: false,
                      properties: {
                        STIFF_UPD_SCHEME: {
                          type: 'integer',
                          description: 'Stiffness update scheme',
                          default: 0,
                        },
                        ITER_BEF_UPDATE: {
                          type: 'integer',
                          description: 'Iterations before stiffness update',
                          'x-required-when': {
                            STIFF_UPD_SCHEME: 0,
                          },
                          'x-optional-when': {
                            STIFF_UPD_SCHEME: 0,
                          },
                        },
                      },
                    },
                    DISP: {
                      type: 'object',
                      description: 'Considering Displacement Norm',
                      additionalProperties: false,
                      properties: {
                        OPT_USE: {
                          type: 'boolean',
                          description: 'Norm option on/off',
                          default: false,
                        },
                        VALUE: {
                          type: 'number',
                          description: 'Norm tolerance value',
                          'x-required-when': {
                            OPT_USE: true,
                          },
                          'x-optional-when': {
                            OPT_USE: true,
                          },
                        },
                      },
                    },
                    LOAD: {
                      type: 'object',
                      description: 'Considering Load Norm',
                      additionalProperties: false,
                      properties: {
                        OPT_USE: {
                          type: 'boolean',
                          description: 'Norm option on/off',
                          default: false,
                        },
                        VALUE: {
                          type: 'number',
                          description: 'Norm tolerance value',
                          'x-required-when': {
                            OPT_USE: true,
                          },
                          'x-optional-when': {
                            OPT_USE: true,
                          },
                        },
                      },
                    },
                    WORK: {
                      type: 'object',
                      description: 'Considering Work Norm',
                      additionalProperties: false,
                      properties: {
                        OPT_USE: {
                          type: 'boolean',
                          description: 'Norm option on/off',
                          default: false,
                        },
                        VALUE: {
                          type: 'number',
                          description: 'Norm tolerance value',
                          'x-required-when': {
                            OPT_USE: true,
                          },
                          'x-optional-when': {
                            OPT_USE: true,
                          },
                        },
                      },
                    },
                  },
                },
                FINAL_STAGE: {
                  type: 'string',
                  description: 'Construction stage name',
                },
              },
            },
          },
        },
      },
    };

    const html = generateHTMLDocument(schema as any, 'civil_gen_definition', 'enhanced');

    const row1Match = /<tr>\s*<td style="[^"]*" rowspan="(\d+)">\s*<p style="text-align: center;">1<\/p>/i.exec(html);
    expect(row1Match).toBeTruthy();

    const row1Start = row1Match!.index ?? 0;
    const after1 = html.slice(row1Start);
    const nextTopLevelRow2 = /<tr>\s*<td style="[^"]*">\s*<p style="text-align: center;">2<\/p>/i.exec(after1);

    expect(nextTopLevelRow2).toBeTruthy();

    const row1Block = after1.slice(0, nextTopLevelRow2!.index);
    const renderedRowsFor1 = (row1Block.match(/<tr\b/gi) || []).length;

    expect(Number(row1Match![1])).toBe(renderedRowsFor1);
    expect(html).toContain('1.4');
    expect(html.indexOf('1.4')).toBeLessThan(html.indexOf('>2</p>'));
  });

  it('renders map-wrapper oneOf entities from additionalProperties into specification rows', () => {
    const schema = {
      type: 'object',
      required: ['Assign'],
      additionalProperties: false,
      properties: {
        Assign: {
          type: 'object',
          description: 'Plastic material keyed map',
          minProperties: 1,
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

    const html = generateHTMLDocument(schema as any, 'civil_gen_definition', 'enhanced');
    const trescaHeaderCount = (html.match(/<span style="color: #4c9aff;">TRESCA Model<\/span>/g) || []).length;
    const vmisesHeaderCount = (html.match(/<span style="color: #4c9aff;">Von Mises Model<\/span>/g) || []).length;

    expect(html).toContain('TRESCA Model');
    expect(html).toContain('Von Mises Model');
    expect(html).toContain('INIT_YIELD_STRESS');
    expect(html).toContain('"MODEL_TYPE"');
    expect(trescaHeaderCount).toBe(1);
    expect(vmisesHeaderCount).toBe(1);
    expect(html.indexOf('<span style="color: #4c9aff;">TRESCA Model</span>')).toBeLessThan(html.indexOf('>1.1<'));
    expect(html.indexOf('<span style="color: #4c9aff;">Von Mises Model</span>')).toBeLessThan(html.indexOf('>1.4<'));
  });

  it('unwraps Argument body wrappers for item rows and keeps deep nested members in HTML', () => {
    const schema = {
      type: 'object',
      required: ['Argument'],
      properties: {
        Argument: {
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
        },
      },
    };

    const html = generateHTMLDocument(schema as any, 'civil_gen_definition', 'enhanced');
    const itemTableIndex = html.indexOf('Item (Value Object Schema)');
    const exportIndex = html.indexOf('"EXPORT_PATH"', itemTableIndex);
    const argumentIndex = html.indexOf('"Argument"', itemTableIndex);

    expect(exportIndex).toBeGreaterThan(itemTableIndex);
    expect(argumentIndex === -1 || argumentIndex > exportIndex).toBe(true);
    expect(html).toContain('2.1.1.1.1');
    expect(html).toContain('"BEAM"');
    expect(html).toContain('"WALL"');
  });

  it('renders validation-only oneOf guidance for mutually exclusive object keys', () => {
    const schema = {
      type: 'object',
      required: ['Argument'],
      properties: {
        Argument: {
          type: 'object',
          required: ['PERFORM_TYPE'],
          oneOf: [
            { required: ['ELEMS'] },
            { required: ['SECTIONS'] },
          ],
          properties: {
            PERFORM_TYPE: {
              type: 'string',
              description: 'Perform target',
              oneOf: [
                { title: 'All Elements', const: 'ALL' },
                { title: 'By Element No.', const: 'ELEMS' },
                { title: 'By Section No.', const: 'SECTIONS' },
              ],
            },
            ELEMS: {
              type: 'object',
              description: 'Element numbers',
              oneOf: [
                { required: ['KEYS'] },
                { required: ['TO'] },
                { required: ['STRUCTURE_GROUP_NAME'] },
              ],
              properties: {
                KEYS: {
                  type: 'array',
                  items: { type: 'integer' },
                },
                TO: {
                  type: 'string',
                },
                STRUCTURE_GROUP_NAME: {
                  type: 'string',
                },
              },
            },
            SECTIONS: {
              type: 'array',
              items: { type: 'integer' },
            },
          },
        },
      },
    };

    const html = generateHTMLDocument(schema as any, 'civil_gen_definition', 'enhanced');

    expect(html).toContain('Choose exactly one of the following keys: &quot;ELEMS&quot; or &quot;SECTIONS&quot;.');
    expect(html).toContain('Choose exactly one of the following keys: &quot;KEYS&quot;, &quot;TO&quot;, or &quot;STRUCTURE_GROUP_NAME&quot;.');
    expect(html.indexOf('Choose exactly one of the following keys: &quot;ELEMS&quot; or &quot;SECTIONS&quot;.')).toBeLessThan(
      html.indexOf('<p style="text-align: center;">"ELEMS"</p>')
    );
  });

  it('renders nested validation-only oneOf guidance inside map-wrapper array item objects', () => {
    const schema = {
      type: 'object',
      required: ['Assign'],
      properties: {
        Assign: {
          type: 'object',
          patternProperties: {
            '^[0-9]+$': {
              type: 'object',
              required: ['ITEMS'],
              properties: {
                ITEMS: {
                  type: 'array',
                  items: {
                    type: 'object',
                    required: ['CREATE_SUB_SECTION', 'MAIN_BAR'],
                    properties: {
                      CREATE_SUB_SECTION: {
                        type: 'boolean',
                        default: false,
                      },
                      ELEMS: {
                        type: 'object',
                        description: 'Element List',
                        properties: {
                          KEYS: {
                            type: 'array',
                            items: { type: 'integer' },
                          },
                          TO: {
                            type: 'string',
                          },
                          STRUCTURE_GROUP_NAME: {
                            type: 'string',
                          },
                        },
                        oneOf: [
                          { required: ['KEYS'] },
                          { required: ['TO'] },
                          { required: ['STRUCTURE_GROUP_NAME'] },
                        ],
                      },
                      MAIN_BAR: {
                        type: 'object',
                        required: ['NAME'],
                        properties: {
                          NAME: {
                            type: 'string',
                          },
                        },
                      },
                    },
                    allOf: [
                      {
                        if: {
                          properties: {
                            CREATE_SUB_SECTION: { const: true },
                          },
                          required: ['CREATE_SUB_SECTION'],
                        },
                        then: {
                          required: ['ELEMS'],
                        },
                      },
                    ],
                  },
                },
              },
            },
          },
        },
      },
    };

    const html = generateHTMLDocument(schema as any, 'civil_gen_definition', 'enhanced');
    const elemsIndex = html.indexOf('<p style="text-align: center;">"ELEMS"</p>');
    const oneOfIndex = html.indexOf('Choose exactly one of the following keys: &quot;KEYS&quot;, &quot;TO&quot;, or &quot;STRUCTURE_GROUP_NAME&quot;.');
    const keysIndex = html.indexOf('<p style="text-align: center;">"KEYS"</p>');

    expect(elemsIndex).toBeGreaterThan(-1);
    expect(oneOfIndex).toBeGreaterThan(elemsIndex);
    expect(keysIndex).toBeGreaterThan(oneOfIndex);
  });
});
