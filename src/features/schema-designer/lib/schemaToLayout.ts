/**
 * Schema to Layout Converter
 * 
 * Enhanced JSON Schema → Canvas Layout (DesignedField[])
 * 기존 스키마를 디자이너에서 열 때 사용
 */

import type { DesignedField, FieldType } from '../types/designer';

// ============================================================================
// Types
// ============================================================================

interface EnhancedProperty {
    type?: string;
    enum?: (string | number)[];
    default?: any;
    minimum?: number;
    maximum?: number;
    minLength?: number;
    maxLength?: number;
    pattern?: string;
    format?: string;
    description?: string;
    items?: any;
    minItems?: number;
    maxItems?: number;
    properties?: Record<string, EnhancedProperty>;
    required?: string[];
    'x-ui'?: {
        label?: string;
        group?: string;
        hint?: string;
        visibleWhen?: Record<string, any[]>;
    };
    'x-enum-labels'?: Record<string, string>;
    'x-enum-labels-by-type'?: Record<string, Record<string, string>>;
    // NOTE: x-enum-by-type, x-value-constraint, x-node-count-by-type deprecated (shared.yaml SSOT)
    'x-required-when'?: Record<string, any[]>;
}

interface EnhancedSchema {
    type?: string;
    title?: string;
    description?: string;
    properties?: Record<string, EnhancedProperty>;
    required?: string[];
    'x-transport'?: {
        uri?: string;
        methods?: string[];
        'body-root'?: string;
    };
    'x-exclusive-keys'?: string[];
    allOf?: any[];
}

// ============================================================================
// Helper Functions
// ============================================================================

function generateId(): string {
    return `field_${Date.now()}_${Math.random().toString(36).substr(2, 9)}`;
}

function determineFieldType(prop: EnhancedProperty): FieldType {
    if (prop.enum && prop.enum.length > 0) {
        return 'enum';
    }

    const type = prop.type || 'string';

    switch (type) {
        case 'string':
            return 'string';
        case 'number':
            return 'number';
        case 'integer':
            return 'integer';
        case 'boolean':
            return 'boolean';
        case 'array':
            return 'array';
        case 'object':
            return 'object';
        default:
            return 'string';
    }
}

// ============================================================================
// Main Converter
// ============================================================================

/**
 * Enhanced JSON Schema를 DesignedField 배열로 변환
 */
export function schemaToLayout(schema: EnhancedSchema): {
    fields: DesignedField[];
    transport?: {
        uri?: string;
        methods?: string[];
        bodyRoot?: string;
    };
} {
    const fields: DesignedField[] = [];
    const requiredFields = new Set(schema.required || []);

    if (!schema.properties) {
        return { fields };
    }

    // 각 property를 DesignedField로 변환
    Object.entries(schema.properties).forEach(([key, prop]) => {
        const field = propertyToField(key, prop, requiredFields.has(key));
        fields.push(field);
    });

    // Transport 정보 추출
    const transport = schema['x-transport'] ? {
        uri: schema['x-transport'].uri,
        methods: schema['x-transport'].methods,
        bodyRoot: schema['x-transport']['body-root'],
    } : undefined;

    return { fields, transport };
}

/**
 * 단일 property를 DesignedField로 변환
 */
function propertyToField(
    key: string,
    prop: EnhancedProperty,
    isRequired: boolean
): DesignedField {
    const type = determineFieldType(prop);

    const field: DesignedField = {
        id: generateId(),
        key,
        type,
        required: isRequired,
    };

    // 기본 속성
    if (prop.description) field.description = prop.description;
    if (prop.default !== undefined) field.default = prop.default;
    if (prop.enum) field.enum = prop.enum;
    if (prop.minimum !== undefined) field.minimum = prop.minimum;
    if (prop.maximum !== undefined) field.maximum = prop.maximum;
    if (prop.minLength !== undefined) field.minLength = prop.minLength;
    if (prop.maxLength !== undefined) field.maxLength = prop.maxLength;
    if (prop.pattern) field.pattern = prop.pattern;
    if (prop.format) field.format = prop.format;

    // Array 속성
    if (prop.items) field.items = prop.items;
    if (prop.minItems !== undefined) field.minItems = prop.minItems;
    if (prop.maxItems !== undefined) field.maxItems = prop.maxItems;

    // x-ui 확장
    if (prop['x-ui']) {
        field.xui = {
            label: prop['x-ui'].label,
            group: prop['x-ui'].group,
            hint: prop['x-ui'].hint,
            visibleWhen: prop['x-ui'].visibleWhen,
        };
    }

    // x-enum 확장 (순수 UI 마커만)
    if (prop['x-enum-labels']) {
        field.xenumLabels = prop['x-enum-labels'];
    }
    if (prop['x-enum-labels-by-type']) {
        field.xenumLabelsByType = prop['x-enum-labels-by-type'];
    }

    // NOTE: x-enum-by-type, x-value-constraint, x-node-count-by-type deprecated (shared.yaml SSOT)

    // x-required-when
    if (prop['x-required-when']) {
        field.xrequiredWhen = prop['x-required-when'];
    }

    // 중첩 객체 처리
    if (type === 'object' && prop.properties) {
        const nestedRequired = new Set(prop.required || []);
        field.children = Object.entries(prop.properties).map(([childKey, childProp]) =>
            propertyToField(childKey, childProp, nestedRequired.has(childKey))
        );
    }

    return field;
}

/**
 * JSON 문자열을 파싱하여 Layout으로 변환
 */
export function parseSchemaToLayout(schemaJson: string): {
    fields: DesignedField[];
    transport?: {
        uri?: string;
        methods?: string[];
        bodyRoot?: string;
    };
} | null {
    try {
        const schema = JSON.parse(schemaJson);
        return schemaToLayout(schema);
    } catch (error) {
        console.error('Failed to parse schema:', error);
        return null;
    }
}
