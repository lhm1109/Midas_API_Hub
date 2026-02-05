/**
 * YAML Rules Loader
 * Loads rules from shared.yaml for MCP generation/validation (SSOT)
 */

import * as fs from 'fs';
import * as path from 'path';
import yaml from 'js-yaml';

// Default rules directory path
const RULES_DIR = path.resolve(
    process.env.APIVERIFICATION_ROOT || process.cwd().replace(/[\\/]mcp-server([\\/]dist)?$/, ''),
    'schema_definitions/civil_gen_definition/enhanced'
);

// ============================================================================
// MARKER REGISTRY TYPES
// ============================================================================

export interface MarkerDefinition {
    id: string;
    key: string;
    description: string;
    pureUI: boolean;
    autoConvertTo?: string;
    schema?: Record<string, unknown>;
    example?: string;
}

// ============================================================================
// SHARED RULES TYPES (SSOT)
// ============================================================================

export interface SharedRules {
    versioning: { rulesSpecVersion: string };
    markerRegistry: MarkerDefinition[];
    conditionRegistry: Array<{
        type: string;
        requiredParams: string[];
        description?: string;
    }>;
    integrityRules: {
        requireMarkerIdInRegistry: boolean;
        requireConditionTypeInRegistry: boolean;
    };
    wrapperRegistryPolicy: {
        sort: string;
        match: string;
    };
    typeInferenceRegistry?: Array<{
        id: string;
        prefix: string;
        type: string;
        example?: string;
    }>;
    componentRegistry?: Record<string, {
        component: string;
        props: Record<string, unknown>;
    }>;
}

// ============================================================================
// LOADERS
// ============================================================================

let lastLoadError: string | null = null;

/**
 * Get the last load error (for debugging)
 */
export function getLastLoadError(): string | null {
    return lastLoadError;
}

/**
 * Load SSOT rules from shared.yaml
 */
export function loadSharedRules(): SharedRules | null {
    const filePath = path.join(RULES_DIR, 'shared.yaml');
    try {
        lastLoadError = null;
        const content = fs.readFileSync(filePath, 'utf-8');
        return yaml.load(content) as SharedRules;
    } catch (error) {
        lastLoadError = error instanceof Error ? error.message : String(error);
        console.error(`Failed to load shared rules: ${filePath}`, error);
        return null;
    }
}

/**
 * Load markerRegistry (for x-* marker validation)
 */
export function loadMarkerRegistry(): MarkerDefinition[] | null {
    const sharedRules = loadSharedRules();
    return sharedRules?.markerRegistry || null;
}

/**
 * Get list of valid x-* marker keys
 */
export function getValidMarkerKeys(): string[] {
    const markers = loadMarkerRegistry();
    if (!markers) return [];
    return markers.map(m => m.key);
}

/**
 * Load typeInferenceRegistry (prefix-type mapping)
 */
export function loadTypeInferenceRegistry(): SharedRules['typeInferenceRegistry'] | null {
    const sharedRules = loadSharedRules();
    return sharedRules?.typeInferenceRegistry || null;
}

/**
 * Get rules directory path
 */
export function getRulesDir(): string {
    return RULES_DIR;
}

/**
 * Load accuracyPolicy.yaml for evidence-based validation
 */
export function loadAccuracyPolicy(): any | null {
    const filePath = path.join(RULES_DIR, 'accuracyPolicy.yaml');
    try {
        lastLoadError = null;
        const content = fs.readFileSync(filePath, 'utf-8');
        return yaml.load(content);
    } catch (error) {
        lastLoadError = error instanceof Error ? error.message : String(error);
        console.error(`Failed to load accuracy policy: ${filePath}`, error);
        return null;
    }
}
