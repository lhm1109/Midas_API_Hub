import { describe, expect, it } from 'vitest';
import type { EnhancedField } from './schemaCompiler';
import { collectFieldConditionInfo, groupFieldsByCondition } from './conditionExtractor';

describe('groupFieldsByCondition', () => {
  it('groups injected required fields only for matching controller values unless visibility markers exist', () => {
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
    ]);
  });

  it('skips synthetic optional groups for controller values excluded by visibility rules', () => {
    const fields: EnhancedField[] = [
      {
        key: 'TYPE',
        type: 'string',
        enum: ['INTERIOR', 'EDGE', 'CORNER'],
        required: { '*': 'required' },
        section: '',
        validationLayers: [],
      },
      {
        key: 'CRITICAL_SECTION',
        type: 'array',
        required: { '*': 'conditional' },
        section: '',
        validationLayers: [],
        'x-required-when': { TYPE: ['EDGE', 'CORNER'] },
        'x-optional-when': { TYPE: ['EDGE', 'CORNER'] },
        _injectedRequiredWhen: true,
      },
    ];

    const fieldInfoMap = collectFieldConditionInfo(fields, []);
    const { fieldGroups } = groupFieldsByCondition(fields, fieldInfoMap);

    const groupTexts = Array.from(fieldGroups.values()).map(
      (entries) => entries[0]?.conditionInfo.conditionText
    );

    expect(groupTexts).toEqual(['"TYPE" is EDGE', '"TYPE" is CORNER']);
  });

  it('does not synthesize optional groups for nested allOf-required fields like XI', () => {
    const fields: EnhancedField[] = [
      {
        key: 'RESULT_COMMON.LONG_TERM.TYPE',
        type: 'string',
        enum: ['5Y_MORE', '12M', '6M', '3M', 'USER'],
        required: { '*': 'required' },
        section: '',
        validationLayers: [],
      },
      {
        key: 'RESULT_COMMON.LONG_TERM.XI',
        type: 'number',
        required: { '*': 'conditional' },
        section: '',
        validationLayers: [],
        'x-required-when': { TYPE: 'USER' },
        _injectedRequiredWhen: true,
      },
    ];

    const fieldInfoMap = collectFieldConditionInfo(fields, []);
    const { fieldGroups } = groupFieldsByCondition(fields, fieldInfoMap);

    const groupTexts = Array.from(fieldGroups.values()).map(
      (entries) => entries[0]?.conditionInfo.conditionText
    );

    expect(groupTexts).toEqual(['"TYPE" is USER']);
  });
});
