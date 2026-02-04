/**
 * YAML to JSON Schema Transformation Rules (Deterministic)
 * Based on shared.yaml SSOT
 */

import { loadSharedRules, loadMarkerRegistry, loadTypeInferenceRegistry } from '../utils/rules-loader.js';

export const RULES_VERSION = '2.0.0';  // Upgraded to SSOT-based
export const JSON_SCHEMA_VERSION = 'http://json-schema.org/draft-07/schema#';

/**
 * Determine wrapper from URI pattern (based on shared.yaml wrapperRegistry)
 */
export function getWrapperForUri(uri: string): 'Assign' | 'Argument' | null {
    const sharedRules = loadSharedRules();
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    const wrapperRegistry = (sharedRules as any)?.['wrapperRegistry'] as Array<{
        id: string;
        pattern: string;
        wrapper: string | null;
        priority: number;
    }> | undefined;

    if (!wrapperRegistry) {
        // Fallback to hardcoded rules
        const WRAPPER_RULES = [
            { pattern: /^\/db\//, wrapper: 'Assign' as const },
            { pattern: /^\/post\//, wrapper: 'Argument' as const },
            { pattern: /^\/doc\//, wrapper: 'Argument' as const },
        ];
        for (const rule of WRAPPER_RULES) {
            if (rule.pattern.test(uri)) {
                return rule.wrapper;
            }
        }
        return null;
    }

    // Sort by priority DESC (SSOT policy)
    const sorted = [...wrapperRegistry].sort((a, b) => (b.priority || 0) - (a.priority || 0));

    for (const rule of sorted) {
        const regex = new RegExp(rule.pattern);
        if (regex.test(uri)) {
            return rule.wrapper as 'Assign' | 'Argument' | null;
        }
    }
    return null;
}

/**
 * Get enhanced schema marker list (based on markerRegistry SSOT)
 */
export function getEnhancedMarkers(): string[] {
    const markers = loadMarkerRegistry();
    if (!markers) {
        // Fallback
        return [
            'x-ui',
            'x-transport',
            'x-enum-labels',
            'x-enum-labels-by-type',
            'x-required-when',
            'x-optional-when',
        ];
    }
    return markers.map(m => m.key);
}

/**
 * Count enhanced schema markers
 */
export function countEnhancedMarkers(content: string): number {
    const markers = getEnhancedMarkers();
    let count = 0;
    for (const marker of markers) {
        const regex = new RegExp(marker.replace(/[.*+?^${}()|[\]\\]/g, '\\$&'), 'g');
        const matches = content.match(regex);
        if (matches) {
            count += matches.length;
        }
    }
    return count;
}

/**
 * Infer type from field name (based on typeInferenceRegistry SSOT)
 */
export function inferTypeFromFieldName(fieldName: string): string | null {
    const typeRegistry = loadTypeInferenceRegistry();

    if (!typeRegistry) {
        // Fallback to hardcoded rules
        const TYPE_INFERENCE_PREFIX: Record<string, string> = {
            'i': 'integer',
            'b': 'boolean',
            'd': 'number',
            's': 'string',
            'n': 'number',
        };
        if (fieldName.length < 2) return null;
        const prefix = fieldName[0].toLowerCase();
        return TYPE_INFERENCE_PREFIX[prefix] || null;
    }

    for (const rule of typeRegistry) {
        if (fieldName.startsWith(rule.prefix)) {
            return rule.type;
        }
    }
    return null;
}
