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
} from './builder.logic';
import type { UIBuilderField } from '@/lib/schema';

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
