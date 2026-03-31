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
});
