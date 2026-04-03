/**
 * @file builder.logic.test.ts
 * @description 단위 테스트 - builder.logic.ts 순수 함수들
 * 
 * ⚠️ 원칙:
 * - 입력 → 출력 비교만 수행
 * - 스냅샷 테스트 아님
 */

import { describe, it, expect } from 'vitest';
import {
    getDefaultValue,
    isTriggerField,
    isAlwaysRequired,
    shouldInitializeField,
    initializeFieldValue,
    buildInitialDynamicFormData,
    flattenObjectToDotNotationWithSchema,
    buildRootValidationOneOfOptionLabels,
    inferRootValidationOneOfSelection,
    getRootValidationOneOfOptionIndexForFieldKey,
    applyRootValidationOneOfSelection,
    isRootValidationOneOfFieldVisible,
    shouldIncludeFieldForRootValidationOneOf,
} from './builder.logic';
import type { UIBuilderField } from '@/lib/schema';
import type { ValidationOneOfInfo } from '@/lib/schema/validationOneOf';

// ============================================================================
// Test: getDefaultValue
// ============================================================================
describe('getDefaultValue', () => {
    it('should return explicit default value', () => {
        const field: UIBuilderField = {
            name: 'TEST_FIELD',
            type: 'string',
            default: 'my_default',
        };
        expect(getDefaultValue(field)).toBe('my_default');
    });

    it('should return first enum value if no default', () => {
        const field: UIBuilderField = {
            name: 'ENUM_FIELD',
            type: 'enum',
            enum: ['OPTION_A', 'OPTION_B', 'OPTION_C'],
        };
        expect(getDefaultValue(field)).toBe('OPTION_A');
    });

    it('should return empty array for array type', () => {
        const field: UIBuilderField = {
            name: 'ARRAY_FIELD',
            type: 'array',
        };
        expect(getDefaultValue(field)).toEqual([]);
    });

    it('should return false for boolean type', () => {
        const field: UIBuilderField = {
            name: 'BOOL_FIELD',
            type: 'boolean',
        };
        expect(getDefaultValue(field)).toBe(false);
    });

    it('should return empty object for keyed object field', () => {
        const field: UIBuilderField = {
            name: 'SELECTED_MEMBERS',
            type: 'object',
            isKeyedObject: true,
        };
        expect(getDefaultValue(field)).toEqual({});
    });

    it('should return null for required field (forceValue=true)', () => {
        const field: UIBuilderField = {
            name: 'REQUIRED_FIELD',
            type: 'string',
            required: true,
        };
        expect(getDefaultValue(field, true)).toBe(null);
    });

    it('should return empty string for optional field', () => {
        const field: UIBuilderField = {
            name: 'OPTIONAL_FIELD',
            type: 'string',
        };
        expect(getDefaultValue(field)).toBe('');
    });
});

// ============================================================================
// Test: isTriggerField
// ============================================================================
describe('isTriggerField', () => {
    it('should return true for enum field without conditional visibility', () => {
        const field: UIBuilderField = {
            name: 'TYPE',
            type: 'enum',
            enum: ['TYPE_A', 'TYPE_B'],
        };
        expect(isTriggerField(field)).toBe(true);
    });

    it('should return false for enum field with x-required-when', () => {
        const field = {
            name: 'CONDITIONAL_ENUM',
            type: 'enum',
            enum: ['OPT_1', 'OPT_2'],
            'x-required-when': { TYPE: 'TYPE_A' },
        } as unknown as UIBuilderField;
        expect(isTriggerField(field)).toBe(false);
    });

    it('should return false for non-enum field', () => {
        const field: UIBuilderField = {
            name: 'TEXT_FIELD',
            type: 'string',
        };
        expect(isTriggerField(field)).toBe(false);
    });
});

// ============================================================================
// Test: isAlwaysRequired
// ============================================================================
describe('isAlwaysRequired', () => {
    it('should return true for required=true', () => {
        const field: UIBuilderField = {
            name: 'REQ_FIELD',
            type: 'string',
            required: true,
        };
        expect(isAlwaysRequired(field)).toBe(true);
    });

    it('should return true for required["*"]="required"', () => {
        const field = {
            name: 'STAR_REQ_FIELD',
            type: 'string',
            required: { '*': 'required' },
        } as unknown as UIBuilderField;
        expect(isAlwaysRequired(field)).toBe(true);
    });

    it('should return false for optional field', () => {
        const field: UIBuilderField = {
            name: 'OPT_FIELD',
            type: 'string',
        };
        expect(isAlwaysRequired(field)).toBe(false);
    });
});

// ============================================================================
// Test: shouldInitializeField
// ============================================================================
describe('shouldInitializeField', () => {
    it('should return true for trigger field', () => {
        const field: UIBuilderField = {
            name: 'TYPE',
            type: 'enum',
            enum: ['A', 'B'],
        };
        expect(shouldInitializeField(field)).toBe(true);
    });

    it('should return true for always required field', () => {
        const field: UIBuilderField = {
            name: 'NAME',
            type: 'string',
            required: true,
        };
        expect(shouldInitializeField(field)).toBe(true);
    });

    it('should return false for optional non-enum field', () => {
        const field: UIBuilderField = {
            name: 'DESCRIPTION',
            type: 'string',
        };
        expect(shouldInitializeField(field)).toBe(false);
    });
});

// ============================================================================
// Test: initializeFieldValue
// ============================================================================
describe('initializeFieldValue', () => {
    it('should set simple field value', () => {
        const field: UIBuilderField = {
            name: 'SIMPLE',
            type: 'string',
            default: 'test',
        };
        const data: Record<string, any> = {};
        initializeFieldValue(field, data);
        expect(data['SIMPLE']).toBe('test');
    });

    it('should set object field with _enabled and children', () => {
        const field: UIBuilderField = {
            name: 'PARENT',
            type: 'object',
            children: [
                { name: 'CHILD_A', type: 'string', default: 'a' },
                { name: 'CHILD_B', type: 'number', default: 10 },
            ],
        };
        const data: Record<string, any> = {};
        initializeFieldValue(field, data);
        expect(data['PARENT._enabled']).toBe(false);
        expect(data['PARENT.CHILD_A']).toBe('a');
        expect(data['PARENT.CHILD_B']).toBe(10);
    });

    it('should keep keyed object field as direct object value', () => {
        const field: UIBuilderField = {
            name: 'SELECTED_MEMBERS',
            type: 'object',
            isKeyedObject: true,
            children: [
                { name: 'ELEM_LIST', type: 'array', items: { type: 'integer' } },
            ],
        };
        const data: Record<string, any> = {};
        initializeFieldValue(field, data);
        expect(data['SELECTED_MEMBERS']).toEqual({});
        expect(data['SELECTED_MEMBERS._enabled']).toBeUndefined();
    });

    it('should set array field to empty array', () => {
        const field: UIBuilderField = {
            name: 'ITEMS',
            type: 'array',
            items: { type: 'string' },
        };
        const data: Record<string, any> = {};
        initializeFieldValue(field, data);
        expect(data['ITEMS']).toEqual([]);
    });
});

// ============================================================================
// Test: buildInitialDynamicFormData
// ============================================================================
describe('buildInitialDynamicFormData', () => {
    it('should only initialize trigger and required fields', () => {
        const schemaFields: UIBuilderField[] = [
            { name: 'TYPE', type: 'enum', enum: ['A', 'B'] }, // Trigger
            { name: 'NAME', type: 'string', required: true }, // Required
            { name: 'DESC', type: 'string' }, // Optional - should NOT be initialized
        ];

        const result = buildInitialDynamicFormData(schemaFields, {});

        expect(result['TYPE']).toBe('A');
        expect(result['NAME']).toBe('');
        expect('DESC' in result).toBe(false);
    });

    it('should preserve existing data', () => {
        const schemaFields: UIBuilderField[] = [
            { name: 'TYPE', type: 'enum', enum: ['A', 'B'] },
        ];
        const existingData = { EXISTING_KEY: 'existing_value' };

        const result = buildInitialDynamicFormData(schemaFields, existingData);

        expect(result['TYPE']).toBe('A');
        expect(result['EXISTING_KEY']).toBe('existing_value');
    });

    it('should not initialize conditional enum fields', () => {
        const schemaFields = [
            {
                name: 'CONDITIONAL',
                type: 'enum',
                enum: ['X', 'Y'],
                'x-required-when': { TYPE: 'A' } // Conditional
            },
        ] as unknown as UIBuilderField[];

        const result = buildInitialDynamicFormData(schemaFields, {});

        expect('CONDITIONAL' in result).toBe(false);
    });
});

describe('flattenObjectToDotNotationWithSchema', () => {
    it('preserves nested keyed object values instead of flattening entry keys', () => {
        const schemaFields: UIBuilderField[] = [
            { name: 'SELECT_ALL', type: 'boolean' },
            { name: 'SELECTED_MEMBERS', type: 'object', isKeyedObject: true },
        ];

        const target: Record<string, any> = {};
        flattenObjectToDotNotationWithSchema(
            {
                SELECT_ALL: false,
                SELECTED_MEMBERS: {
                    '250': { ELEM_LIST: [484, 485] },
                    '131': { ELEM_LIST: [196, 201] },
                },
            },
            target,
            schemaFields
        );

        expect(target).toEqual({
            SELECT_ALL: false,
            SELECTED_MEMBERS: {
                '250': { ELEM_LIST: [484, 485] },
                '131': { ELEM_LIST: [196, 201] },
            },
        });
    });

    it('restores nested object branches with _enabled flags and leaf values', () => {
        const schemaFields: UIBuilderField[] = [
            {
                name: 'RESULT_GRAPHIC',
                type: 'object',
                children: [
                    { name: 'RESULT_GRAPHIC.CURRENT_MODE', type: 'string' },
                    {
                        name: 'RESULT_GRAPHIC.LOAD_CASE_COMB',
                        type: 'object',
                        children: [
                            { name: 'RESULT_GRAPHIC.LOAD_CASE_COMB.TYPE', type: 'string' },
                            { name: 'RESULT_GRAPHIC.LOAD_CASE_COMB.NAME', type: 'string' },
                        ],
                    },
                    {
                        name: 'RESULT_GRAPHIC.TYPE_OF_DISPLAY',
                        type: 'object',
                        children: [
                            {
                                name: 'RESULT_GRAPHIC.TYPE_OF_DISPLAY.REINFORCEMENT',
                                type: 'object',
                                children: [
                                    {
                                        name: 'RESULT_GRAPHIC.TYPE_OF_DISPLAY.REINFORCEMENT.DISPLAY_MEMBERS',
                                        type: 'object',
                                        children: [
                                            { name: 'RESULT_GRAPHIC.TYPE_OF_DISPLAY.REINFORCEMENT.DISPLAY_MEMBERS.BEAM', type: 'boolean' },
                                            { name: 'RESULT_GRAPHIC.TYPE_OF_DISPLAY.REINFORCEMENT.DISPLAY_MEMBERS.COLUMN', type: 'boolean' },
                                            { name: 'RESULT_GRAPHIC.TYPE_OF_DISPLAY.REINFORCEMENT.DISPLAY_MEMBERS.BRACE', type: 'boolean' },
                                            { name: 'RESULT_GRAPHIC.TYPE_OF_DISPLAY.REINFORCEMENT.DISPLAY_MEMBERS.WALL', type: 'boolean' },
                                        ],
                                    },
                                ],
                            },
                        ],
                    },
                ],
            },
        ];

        const target: Record<string, any> = {};
        flattenObjectToDotNotationWithSchema(
            {
                RESULT_GRAPHIC: {
                    CURRENT_MODE: 'INFLL_DESIGN_RC',
                    LOAD_CASE_COMB: {
                        TYPE: 'CBS',
                        NAME: 'gLCB183',
                    },
                    TYPE_OF_DISPLAY: {
                        REINFORCEMENT: {
                            DISPLAY_MEMBERS: {
                                BEAM: true,
                                COLUMN: true,
                                BRACE: true,
                                WALL: true,
                            },
                        },
                    },
                },
            },
            target,
            schemaFields
        );

        expect(target).toEqual({
            'RESULT_GRAPHIC._enabled': true,
            'RESULT_GRAPHIC.CURRENT_MODE': 'INFLL_DESIGN_RC',
            'RESULT_GRAPHIC.LOAD_CASE_COMB._enabled': true,
            'RESULT_GRAPHIC.LOAD_CASE_COMB.TYPE': 'CBS',
            'RESULT_GRAPHIC.LOAD_CASE_COMB.NAME': 'gLCB183',
            'RESULT_GRAPHIC.TYPE_OF_DISPLAY._enabled': true,
            'RESULT_GRAPHIC.TYPE_OF_DISPLAY.REINFORCEMENT._enabled': true,
            'RESULT_GRAPHIC.TYPE_OF_DISPLAY.REINFORCEMENT.DISPLAY_MEMBERS._enabled': true,
            'RESULT_GRAPHIC.TYPE_OF_DISPLAY.REINFORCEMENT.DISPLAY_MEMBERS.BEAM': true,
            'RESULT_GRAPHIC.TYPE_OF_DISPLAY.REINFORCEMENT.DISPLAY_MEMBERS.COLUMN': true,
            'RESULT_GRAPHIC.TYPE_OF_DISPLAY.REINFORCEMENT.DISPLAY_MEMBERS.BRACE': true,
            'RESULT_GRAPHIC.TYPE_OF_DISPLAY.REINFORCEMENT.DISPLAY_MEMBERS.WALL': true,
        });
    });

    it('preserves nested keyed objects when the keyed schema exists below the top level', () => {
        const schemaFields: UIBuilderField[] = [
            {
                name: 'RESULT_GRAPHIC',
                type: 'object',
                children: [
                    {
                        name: 'RESULT_GRAPHIC.ACTIVE',
                        type: 'object',
                        isKeyedObject: true,
                    },
                ],
            },
        ];

        const target: Record<string, any> = {};
        flattenObjectToDotNotationWithSchema(
            {
                RESULT_GRAPHIC: {
                    ACTIVE: {
                        NODE: { VISIBLE: true },
                    },
                },
            },
            target,
            schemaFields
        );

        expect(target).toEqual({
            'RESULT_GRAPHIC._enabled': true,
            'RESULT_GRAPHIC.ACTIVE': {
                NODE: { VISIBLE: true },
            },
        });
    });
});

describe('root validation-only oneOf helpers', () => {
    const rootOneOfInfo: ValidationOneOfInfo = {
        optionKeyGroups: [['ELEMS'], ['SECTIONS']],
        participantKeys: ['ELEMS', 'SECTIONS'],
        description: 'Choose exactly one of the following keys: "ELEMS" or "SECTIONS".',
    };

    const schemaFields: UIBuilderField[] = [
        {
            name: 'PERFORM_TYPE',
            type: 'enum',
            description: 'Perform Target',
            enum: ['ALL', 'ELEMS', 'SECTIONS'],
        },
        {
            name: 'ELEMS',
            type: 'object',
            description: 'Element No.',
            children: [
                { name: 'ELEMS.KEYS', type: 'array', items: { type: 'integer' } },
            ],
        },
        {
            name: 'SECTIONS',
            type: 'array',
            description: 'Section No.',
            items: { type: 'integer' },
        },
    ];

    it('builds user-facing option labels from top-level fields', () => {
        expect(buildRootValidationOneOfOptionLabels(rootOneOfInfo, schemaFields)).toEqual([
            'Element No.',
            'Section No.',
        ]);
    });

    it('infers the selected root option from existing form data', () => {
        expect(inferRootValidationOneOfSelection(rootOneOfInfo, schemaFields, {
            'SECTIONS': [10, 20],
        })).toBe(1);

        expect(inferRootValidationOneOfSelection(rootOneOfInfo, schemaFields, {
            'ELEMS._enabled': true,
            'ELEMS.KEYS': [1, 2, 3],
        })).toBe(0);
    });

    it('matches nested field keys back to the correct root option', () => {
        expect(getRootValidationOneOfOptionIndexForFieldKey('ELEMS.KEYS', rootOneOfInfo, schemaFields)).toBe(0);
        expect(getRootValidationOneOfOptionIndexForFieldKey('SECTIONS', rootOneOfInfo, schemaFields)).toBe(1);
        expect(getRootValidationOneOfOptionIndexForFieldKey('PERFORM_TYPE', rootOneOfInfo, schemaFields)).toBe(-1);
    });

    it('clears unselected branches and enables the selected object branch', () => {
        const nextState = applyRootValidationOneOfSelection(
            {
                PERFORM_TYPE: 'ELEMS',
                'ELEMS._enabled': true,
                'ELEMS.KEYS': [1, 2, 3],
                SECTIONS: [4, 5],
            },
            1,
            schemaFields,
            rootOneOfInfo
        );

        expect(nextState['__root__.__selectedOption']).toBe(1);
        expect(nextState['SECTIONS']).toEqual([4, 5]);
        expect(nextState['ELEMS._enabled']).toBeUndefined();
        expect(nextState['ELEMS.KEYS']).toBeUndefined();
    });

    it('filters UI visibility and JSON inclusion by the selected root branch', () => {
        expect(isRootValidationOneOfFieldVisible(schemaFields[1], rootOneOfInfo, 0)).toBe(true);
        expect(isRootValidationOneOfFieldVisible(schemaFields[2], rootOneOfInfo, 0)).toBe(false);
        expect(shouldIncludeFieldForRootValidationOneOf('ELEMS.KEYS', rootOneOfInfo, schemaFields, 1)).toBe(false);
        expect(shouldIncludeFieldForRootValidationOneOf('SECTIONS', rootOneOfInfo, schemaFields, 1)).toBe(true);
        expect(shouldIncludeFieldForRootValidationOneOf('PERFORM_TYPE', rootOneOfInfo, schemaFields, 1)).toBe(true);
    });
});
