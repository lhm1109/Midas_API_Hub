/**
 * Layout to Schema Converter
 * 
 * Canvas Layout (DesignedField[]) → Enhanced JSON Schema
 * 디자이너에서 편집한 내용을 스키마로 변환
 */

import type { DesignedField, EnhancedSchemaOutput, EnhancedSchemaProperty } from '../types/designer';

// ============================================================================
// Main Converter
// ============================================================================

/**
 * DesignedField 배열을 Enhanced JSON Schema로 변환
 */
export function layoutToSchema(
    fields: DesignedField[],
    transport?: {
        uri?: string;
        methods?: string[];
        bodyRoot?: string;
    }
): EnhancedSchemaOutput {
    const properties: Record<string, EnhancedSchemaProperty> = {};
    const required: string[] = [];

    fields.forEach(field => {
        properties[field.key] = fieldToProperty(field);

        // Required 처리 (조건부 required 제외)
        if (field.required && !field.xrequiredWhen) {
            required.push(field.key);
        }
    });

    const schema: EnhancedSchemaOutput = {
        type: 'object',
        properties,
    };

    // Required 배열 (비어있지 않은 경우만)
    if (required.length > 0) {
        schema.required = required;
    }

    // x-transport
    if (transport && (transport.uri || transport.methods || transport.bodyRoot)) {
        schema['x-transport'] = {};
        if (transport.uri) schema['x-transport'].uri = transport.uri;
        if (transport.methods) schema['x-transport'].methods = transport.methods;
        if (transport.bodyRoot) schema['x-transport']['body-root'] = transport.bodyRoot;
    }

    // allOf 생성 (조건부 required가 있는 경우)
    const allOfRules = generateAllOfRules(fields);
    if (allOfRules.length > 0) {
        schema.allOf = allOfRules;
    }

    return schema;
}

/**
 * 단일 DesignedField를 EnhancedSchemaProperty로 변환
 */
function fieldToProperty(field: DesignedField): EnhancedSchemaProperty {
    const prop: EnhancedSchemaProperty = {
        type: field.type === 'enum' ? 'string' : field.type,
    };

    // 기본 속성
    if (field.description) prop.description = field.description;
    if (field.default !== undefined) prop.default = field.default;
    if (field.enum && field.enum.length > 0) prop.enum = field.enum;
    if (field.minimum !== undefined) prop.minimum = field.minimum;
    if (field.maximum !== undefined) prop.maximum = field.maximum;
    if (field.minLength !== undefined) prop.minLength = field.minLength;
    if (field.maxLength !== undefined) prop.maxLength = field.maxLength;
    if (field.pattern) prop.pattern = field.pattern;
    if (field.format) prop.format = field.format;

    // Array 속성
    if (field.type === 'array') {
        if (field.items) {
            prop.items = field.items;
        } else {
            prop.items = { type: 'string' };
        }
        if (field.minItems !== undefined) prop.minItems = field.minItems;
        if (field.maxItems !== undefined) prop.maxItems = field.maxItems;
    }

    // x-ui 확장
    if (field.xui && Object.keys(field.xui).some(k => field.xui![k as keyof typeof field.xui])) {
        prop['x-ui'] = {};
        if (field.xui.label) prop['x-ui'].label = field.xui.label;
        if (field.xui.group) prop['x-ui'].group = field.xui.group;
        if (field.xui.hint) prop['x-ui'].hint = field.xui.hint;
        if (field.xui.visibleWhen && Object.keys(field.xui.visibleWhen).length > 0) {
            prop['x-ui'].visibleWhen = field.xui.visibleWhen;
        }
    }

    // x-enum-labels
    if (field.xenumLabels && Object.keys(field.xenumLabels).length > 0) {
        prop['x-enum-labels'] = field.xenumLabels;
    }

    // x-enum-labels-by-type (순수 UI 마커)
    if (field.xenumLabelsByType && Object.keys(field.xenumLabelsByType).length > 0) {
        prop['x-enum-labels-by-type'] = field.xenumLabelsByType;
    }

    // NOTE: x-enum-by-type, x-value-constraint, x-node-count-by-type는 deprecated (shared.yaml SSOT)

    // x-required-when
    if (field.xrequiredWhen && Object.keys(field.xrequiredWhen).length > 0) {
        prop['x-required-when'] = field.xrequiredWhen;
    }

    // 중첩 객체 처리
    if (field.type === 'object' && field.children && field.children.length > 0) {
        prop.properties = {};
        field.children.forEach(child => {
            prop.properties![child.key] = fieldToProperty(child);
        });
    }

    return prop;
}

/**
 * 조건부 required를 allOf 규칙으로 변환
 */
function generateAllOfRules(fields: DesignedField[]): any[] {
    const rules: any[] = [];

    fields.forEach(field => {
        if (field.xrequiredWhen && Object.keys(field.xrequiredWhen).length > 0) {
            Object.entries(field.xrequiredWhen).forEach(([conditionField, conditionValues]) => {
                rules.push({
                    if: {
                        properties: {
                            [conditionField]: { enum: conditionValues },
                        },
                    },
                    then: {
                        required: [field.key],
                    },
                });
            });
        }
    });

    return rules;
}

/**
 * Layout을 JSON 문자열로 변환 (Pretty Print)
 */
export function layoutToSchemaString(
    fields: DesignedField[],
    transport?: {
        uri?: string;
        methods?: string[];
        bodyRoot?: string;
    }
): string {
    const schema = layoutToSchema(fields, transport);
    return JSON.stringify(schema, null, 2);
}
