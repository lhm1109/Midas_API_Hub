import { describe, expect, it } from 'vitest';
import type { EnhancedField } from './schemaCompiler';
import { applyConditionalRequiredToField, flattenComposedObjectSchema } from './schemaCompiler';

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
