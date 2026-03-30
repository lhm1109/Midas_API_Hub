import { describe, expect, it } from 'vitest';
import type { EnhancedField } from './schemaCompiler';
import { collectFieldConditionInfo, groupFieldsByCondition } from './conditionExtractor';

describe('groupFieldsByCondition', () => {
  it('groups simple x-required-when fields by condition value and keeps per-row required status', () => {
    const fields: EnhancedField[] = [
      {
        key: 'CONCRETE.CODE',
        type: 'string',
        enum: ['None', 'Standard'],
        required: { '*': 'required' },
        section: '',
        validationLayers: [],
      },
      {
        key: 'CONCRETE.STANDARD_CODE',
        type: 'string',
        required: { '*': 'conditional' },
        section: '',
        validationLayers: [],
        'x-required-when': { CODE: 'Standard' },
      },
      {
        key: 'CONCRETE.GRADE',
        type: 'string',
        required: { '*': 'conditional' },
        section: '',
        validationLayers: [],
        'x-required-when': { CODE: 'Standard' },
      },
      {
        key: 'CONCRETE.NAME',
        type: 'string',
        required: { '*': 'conditional' },
        section: '',
        validationLayers: [],
        'x-required-when': { CODE: 'None' },
      },
      {
        key: 'CONCRETE.FC',
        type: 'number',
        required: { '*': 'conditional' },
        section: '',
        validationLayers: [],
        'x-required-when': { CODE: 'None' },
        _injectedRequiredWhen: true,
      },
    ];

    const fieldInfoMap = collectFieldConditionInfo(fields, []);
    const { fieldGroups, noConditionFields } = groupFieldsByCondition(fields, fieldInfoMap);

    expect(noConditionFields.map(({ field }) => field.key)).toContain('CONCRETE.CODE');

    const groups = Array.from(fieldGroups.values()).map((entries) => ({
      type: entries[0]?.conditionInfo.type,
      conditionText: entries[0]?.conditionInfo.conditionText,
      fields: entries.map((entry) => ({
        key: entry.field.key,
        type: entry.conditionInfo.type,
      })),
    }));

    expect(groups).toHaveLength(2);

    const noneGroup = groups.find((group) => group.conditionText === '"CODE" is None');
    expect(noneGroup?.fields).toEqual([
      { key: 'CONCRETE.NAME', type: 'x-required-when' },
      { key: 'CONCRETE.FC', type: 'x-required-when' },
    ]);

    const standardGroup = groups.find((group) => group.conditionText === '"CODE" is Standard');
    expect(standardGroup?.fields).toEqual([
      { key: 'CONCRETE.STANDARD_CODE', type: 'x-required-when' },
      { key: 'CONCRETE.GRADE', type: 'x-required-when' },
      { key: 'CONCRETE.FC', type: 'x-optional-when' },
    ]);
  });
});
