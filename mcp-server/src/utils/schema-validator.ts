/**
 * Schema Validator
 * Validates and transforms schemas based on shared.yaml SSOT
 */

import { loadSharedRules, loadMarkerRegistry, getValidMarkerKeys, loadTypeInferenceRegistry } from '../utils/rules-loader.js';

export interface ValidationResult {
    valid: boolean;
    errors: ValidationError[];
    warnings: ValidationWarning[];
    transformed: Record<string, unknown>;
}

export interface ValidationError {
    field: string;
    message: string;
    fixable: boolean;
    fixApplied?: boolean;
}

export interface ValidationWarning {
    field: string;
    message: string;
}

/**
 * Validate and transform schema
 */
export function validateAndTransform(
    schema: Record<string, unknown>
): ValidationResult {
    const sharedRules = loadSharedRules();
    const validMarkerKeys = getValidMarkerKeys();
    const typeInferenceRegistry = loadTypeInferenceRegistry();

    const errors: ValidationError[] = [];
    const warnings: ValidationWarning[] = [];

    // Deep copy for transformation
    const transformed = JSON.parse(JSON.stringify(schema)) as Record<string, unknown>;

    const properties = transformed['properties'] as Record<string, unknown> | undefined;

    if (properties) {
        for (const [fieldName, fieldDef] of Object.entries(properties)) {
            const field = fieldDef as Record<string, unknown>;

            // 1. Validate x-* markers against markerRegistry
            for (const key of Object.keys(field)) {
                if (key.startsWith('x-') && validMarkerKeys.length > 0 && !validMarkerKeys.includes(key)) {
                    warnings.push({
                        field: fieldName,
                        message: `Unknown x-* marker: "${key}". Valid markers: ${validMarkerKeys.join(', ')}`
                    });
                }
            }

            // 2. Auto-convert integer enum to oneOf
            const enumValues = field['enum'] as unknown[] | undefined;
            const fieldType = field['type'] as string | undefined;

            if (enumValues && (fieldType === 'integer' || fieldType === 'number') && !field['oneOf']) {
                const fallbackLabel = 'Option {value}';

                // Get labels from x-ui.options
                const xui = field['x-ui'] as Record<string, unknown> | undefined;
                const options = xui?.['options'] as Array<{ value: number; label: string }> | undefined;

                // Convert to oneOf
                const oneOf = enumValues.map(value => {
                    const option = options?.find(o => o.value === value);
                    const title = option?.label || fallbackLabel.replace('{value}', String(value));
                    return { const: value, title };
                });

                field['oneOf'] = oneOf;
                delete field['enum'];

                // Remove x-ui.options (merged into oneOf)
                if (xui?.['options']) {
                    delete xui['options'];
                }

                errors.push({
                    field: fieldName,
                    message: 'Auto-converted integer enum to oneOf format',
                    fixable: true,
                    fixApplied: true,
                });
            }

            // 3. Auto-generate x-ui.label
            {
                const xui = field['x-ui'] as Record<string, unknown> | undefined;
                if (!xui?.['label'] && !field['title']) {
                    // Generate from field name
                    // Fixed: Handle TABLE_NAME correctly -> "Table Name" (not "T A B L E  N A M E")
                    let label = fieldName
                        .replace(/^[ibds]/, '');  // Remove type prefix (iID -> ID, sNAME -> NAME)

                    // Split by underscore or camelCase
                    label = label
                        .replace(/_/g, ' ')           // TABLE_NAME -> TABLE NAME
                        .replace(/([a-z])([A-Z])/g, '$1 $2')  // camelCase -> camel Case (only lowercase-to-uppercase)
                        .toLowerCase()                // TABLE NAME -> table name
                        .split(' ')                   // ["table", "name"]
                        .map(word => word.charAt(0).toUpperCase() + word.slice(1))  // Title case
                        .join(' ')                    // "Table Name"
                        .trim();

                    if (!xui) {
                        field['x-ui'] = { label };
                    } else {
                        xui['label'] = label;
                    }

                    warnings.push({
                        field: fieldName,
                        message: `Auto-generated x-ui.label: "${label}"`,
                    });
                }
            }

            // 4. Convert x-uiRules.visibleWhen to x-optional-when
            const xuiRules = field['x-uiRules'] as Record<string, unknown> | undefined;
            if (xuiRules?.['visibleWhen']) {
                field['x-optional-when'] = xuiRules['visibleWhen'];
                delete xuiRules['visibleWhen'];

                if (Object.keys(xuiRules).length === 0) {
                    delete field['x-uiRules'];
                }

                errors.push({
                    field: fieldName,
                    message: 'Auto-converted x-uiRules.visibleWhen to x-optional-when (deprecated pattern)',
                    fixable: true,
                    fixApplied: true,
                });
            }

            // 5. Convert x-ui.visibleWhen to x-optional-when
            {
                const xui = field['x-ui'] as Record<string, unknown> | undefined;
                if (xui?.['visibleWhen']) {
                    field['x-optional-when'] = xui['visibleWhen'];
                    delete xui['visibleWhen'];

                    errors.push({
                        field: fieldName,
                        message: 'Auto-converted x-ui.visibleWhen to x-optional-when (forbidden pattern)',
                        fixable: true,
                        fixApplied: true,
                    });
                }
            }

            // 6. Validate prefix-type consistency
            if (typeInferenceRegistry) {
                for (const rule of typeInferenceRegistry) {
                    if (fieldName.startsWith(rule.prefix)) {
                        const fieldType = field['type'] as string | undefined;
                        if (fieldType && fieldType !== rule.type) {
                            warnings.push({
                                field: fieldName,
                                message: `Prefix '${rule.prefix}' expects type '${rule.type}', but got '${fieldType}'`,
                            });
                        }
                        break;
                    }
                }
            }
        }
    }

    // 9. Add $schema
    if (!transformed['$schema']) {
        transformed['$schema'] = 'http://json-schema.org/draft-07/schema#';
    }

    return {
        valid: errors.filter(e => !e.fixApplied).length === 0,
        errors,
        warnings,
        transformed,
    };
}
