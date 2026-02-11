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

            // 1. Validate x-* markers against markerRegistry — strip unknown ones
            for (const key of Object.keys(field)) {
                if (key.startsWith('x-') && validMarkerKeys.length > 0 && !validMarkerKeys.includes(key)) {
                    delete field[key];
                    warnings.push({
                        field: fieldName,
                        message: `Removed unknown x-* marker: "${key}". Valid markers: ${validMarkerKeys.join(', ')}`
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

            // 5b. Normalize x-optional-when / x-required-when:
            // Single-condition array with { condition: {...} } → plain object
            // e.g. [{ condition: { iMASS_TYPE: 1 }, hint: "..." }] → { iMASS_TYPE: 1 }
            for (const markerKey of ['x-optional-when', 'x-required-when'] as const) {
                const val = field[markerKey];
                if (Array.isArray(val) && val.length === 1) {
                    const item = val[0] as Record<string, unknown>;
                    if (item && typeof item === 'object' && 'condition' in item && typeof item['condition'] === 'object') {
                        field[markerKey] = item['condition'];
                        warnings.push({
                            field: fieldName,
                            message: `Normalized ${markerKey}: single-item array with condition → plain object`,
                        });
                    }
                }
            }

            // 5d. Strip unknown x-ui properties (only allow SSOT-defined keys)
            // shared.yaml markerRegistry MARKER_UI defines: label, groupId, hint, groups, component
            {
                const xui = field['x-ui'] as Record<string, unknown> | undefined;
                if (xui && typeof xui === 'object') {
                    // shared.yaml MARKER_UI schema에 정의된 키만 허용
                    const ALLOWED_XUI_KEYS = new Set(['label', 'groupId', 'hint', 'groups', 'component']);
                    const unknownXuiKeys = Object.keys(xui).filter(k => !ALLOWED_XUI_KEYS.has(k));
                    if (unknownXuiKeys.length > 0) {
                        for (const k of unknownXuiKeys) {
                            delete xui[k];
                        }
                        warnings.push({
                            field: fieldName,
                            message: `Removed unknown x-ui properties: ${unknownXuiKeys.join(', ')} (not in shared.yaml markerRegistry)`,
                        });
                    }
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

    // 10. Deep pass: strip unknown x-* markers and x-ui sub-properties from ALL nesting levels
    // Needed for entity collection schemas where fields are nested in patternProperties["^[0-9]+$"].properties.*
    const ALLOWED_XUI_KEYS_DEEP = new Set(['label', 'groupId', 'hint', 'groups', 'component']);
    stripUnknownMarkersDeep(transformed, validMarkerKeys, ALLOWED_XUI_KEYS_DEEP);

    return {
        valid: errors.filter(e => !e.fixApplied).length === 0,
        errors,
        warnings,
        transformed,
    };
}

/**
 * Recursively strip unknown x-* markers and unknown x-ui sub-properties from all levels.
 * Handles entity collection wrappers (patternProperties) and simple object wrappers alike.
 * Mutates the object in place.
 */
function stripUnknownMarkersDeep(
    obj: unknown,
    validMarkerKeys: string[],
    allowedXuiKeys: Set<string>,
    visited = new Set<unknown>()
): void {
    if (!obj || typeof obj !== 'object' || visited.has(obj)) return;
    visited.add(obj);

    if (Array.isArray(obj)) {
        for (const item of obj) {
            stripUnknownMarkersDeep(item, validMarkerKeys, allowedXuiKeys, visited);
        }
        return;
    }

    const o = obj as Record<string, unknown>;

    // Strip unknown x-* markers at this level
    if (validMarkerKeys.length > 0) {
        for (const key of Object.keys(o)) {
            if (key.startsWith('x-') && !validMarkerKeys.includes(key)) {
                delete o[key];
            }
        }
    }

    // Strip unknown x-ui sub-properties at this level
    if (o['x-ui'] && typeof o['x-ui'] === 'object' && !Array.isArray(o['x-ui'])) {
        const xui = o['x-ui'] as Record<string, unknown>;
        for (const k of Object.keys(xui)) {
            if (!allowedXuiKeys.has(k)) {
                delete xui[k];
            }
        }
    }

    // Recurse into all child values
    for (const val of Object.values(o)) {
        stripUnknownMarkersDeep(val, validMarkerKeys, allowedXuiKeys, visited);
    }
}
