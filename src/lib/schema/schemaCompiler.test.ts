import { describe, expect, it } from 'vitest';
import type { EnhancedField } from './schemaCompiler';
import { applyConditionalRequiredToField } from './schemaCompiler';

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
