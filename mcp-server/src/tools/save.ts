/**
 * save_schema MCP Tool
 * Saves AI-generated schemas with SSOT-based validation/transformation
 */

import * as fs from 'fs';
import { buildWritePath } from '../utils/path-guard.js';
import { toCanonicalJSON } from '../utils/deterministic-json.js';
import { sha256 } from '../utils/hash.js';
import { RULES_VERSION, JSON_SCHEMA_VERSION } from '../rules/transform.js';
import { validateAndTransform } from '../utils/schema-validator.js';
import { loadSharedRules, getRulesDir, getLastLoadError } from '../utils/rules-loader.js';

export interface SaveSchemaInput {
    schemaName: string;
    outputDir?: string;
    schema: Record<string, unknown>;
    description?: string;
    questions?: Question[];
    skipValidation?: boolean;  // Skip validation (for debugging)
    wrapWithEntityCollection?: boolean;  // Wrap schema with Assign → additionalProperties structure
    wrapWithSimpleObject?: boolean;  // Wrap with simple Argument object (no additionalProperties)
    bodyRoot?: string;  // Body root key (default: "Assign")
    excludeSchemaField?: boolean;  // Exclude $schema field from output
    generateTableSchema?: boolean;  // Generate Table API schema using tableSchemaRegistry
    tableTypeEnums?: string[];  // TABLE_TYPE enum values (required when generateTableSchema is true)
    additionalTableFields?: string[];  // Additional fields from tableSchemaRegistry to include
}


export interface Question {
    field: string;
    question: string;
    context?: string;
    suggestion?: unknown;
}

export interface SaveSchemaResult {
    ok: boolean;
    status: 'saved' | 'pending_questions';
    schemaPath?: string;
    metaPath?: string;
    outputHash?: string;
    pendingQuestions?: Question[];
    validation?: {
        errors: Array<{ field: string; message: string; fixApplied?: boolean }>;
        warnings: Array<{ field: string; message: string }>;
    };
    stats?: {
        fieldCount: number;
        requiredCount: number;
        debug?: unknown;
    };
    error?: string;
}

/**
 * Execute save_schema tool
 */
export async function saveSchema(input: SaveSchemaInput): Promise<SaveSchemaResult> {
    try {
        // 1. Validate input
        if (!input.schemaName) {
            throw new Error('schemaName is required');
        }
        if (!input.schema || typeof input.schema !== 'object') {
            throw new Error('schema is required and must be an object');
        }

        // 2. Return questions if present
        if (input.questions && input.questions.length > 0) {
            return {
                ok: true,
                status: 'pending_questions',
                pendingQuestions: input.questions,
            };
        }

        const outputDir = input.outputDir || 'default';

        // DEBUG: Log input parameters (use stderr to avoid interfering with MCP)
        console.error('[DEBUG] save_schema input:', JSON.stringify({
            generateTableSchema: input.generateTableSchema,
            tableTypeEnums: input.tableTypeEnums,
            additionalTableFields: input.additionalTableFields,
        }, null, 2));

        // 3. Generate Table schema from registry if requested
        let schema = { ...input.schema };
        if (input.generateTableSchema) {
            console.error('[DEBUG] Generating Table schema from registry...');
            schema = generateTableSchemaFromRegistry(schema, input.tableTypeEnums, input.additionalTableFields);
        }

        // 4. Validate and transform based on SSOT rules
        let validationResult = null;

        if (!input.skipValidation) {
            validationResult = validateAndTransform(schema);
            schema = validationResult.transformed as Record<string, unknown>;
        }

        // 4. Wrap with entity collection structure if requested
        if (input.wrapWithEntityCollection) {
            const bodyRoot = input.bodyRoot || 'Assign';
            schema = wrapWithEntityCollectionStructure(schema, bodyRoot);
        }

        // 4b. Wrap with simple object structure if requested (for POST table schemas)
        if (input.wrapWithSimpleObject) {
            const bodyRoot = input.bodyRoot || 'Argument';
            schema = wrapWithSimpleObjectStructure(schema, bodyRoot);
        }

        // 5. Handle $schema field
        // Default: exclude $schema for OpenAPI 3.1 compatibility
        // Only add if explicitly requested (!excludeSchemaField)
        if (input.excludeSchemaField === false && !schema['$schema']) {
            schema['$schema'] = JSON_SCHEMA_VERSION;
        }
        // Remove $schema if present (default behavior)
        if (input.excludeSchemaField !== false && schema['$schema']) {
            delete schema['$schema'];
        }

        // 5. Build write path
        const paths = buildWritePath(outputDir, input.schemaName);

        // 6. Collect stats
        const properties = schema['properties'] as Record<string, unknown> | undefined;
        const required = schema['required'] as string[] | undefined;
        const fieldCount = properties ? Object.keys(properties).length : 0;
        const requiredCount = required ? required.length : 0;

        // 7. Generate deterministic JSON
        const schemaJson = toCanonicalJSON(schema);
        const outputHash = sha256(schemaJson);

        // 8. Create metadata
        const meta = {
            rulesVersion: RULES_VERSION,
            description: input.description || null,
            outputHash,
            generatedAt: new Date().toISOString(),
            stats: { fieldCount, requiredCount },
            validation: validationResult ? {
                errorsFixed: validationResult.errors.filter(e => e.fixApplied).length,
                warnings: validationResult.warnings.length,
            } : null,
        };
        const metaJson = toCanonicalJSON(meta);

        // 9. Write files
        fs.writeFileSync(paths.schemaPath, schemaJson, 'utf-8');
        fs.writeFileSync(paths.metaPath, metaJson, 'utf-8');

        return {
            ok: true,
            status: 'saved',
            schemaPath: paths.schemaPath,
            metaPath: paths.metaPath,
            outputHash,
            validation: validationResult ? {
                errors: validationResult.errors,
                warnings: validationResult.warnings,
            } : undefined,
            stats: {
                fieldCount,
                requiredCount,
                // DEBUG INFO
                debug: {
                    generateTableSchemaRequested: input.generateTableSchema || false,
                    tableTypeEnums: input.tableTypeEnums || [],
                    additionalTableFields: input.additionalTableFields || [],
                }
            },
        };

    } catch (error) {
        const message = error instanceof Error ? error.message : String(error);
        return {
            ok: false,
            status: 'saved',
            error: message,
        };
    }
}

/**
 * Wrap schema with entity collection structure (for DB entity APIs):
 * {
 *   type: object, additionalProperties: false, required: [Assign],
 *   properties: {
 *     Assign: {
 *       type: object, minProperties: 1, additionalProperties: false,
 *       patternProperties: { "^[0-9]+$": { ...entitySchema } }
 *     }
 *   }
 * }
 */
function wrapWithEntityCollectionStructure(
    schema: Record<string, unknown>,
    bodyRoot: string
): Record<string, unknown> {
    // Extract entity properties (remove $schema, title from top level)
    const { $schema, title, type, ...entityDef } = schema as any;

    // Build the entity schema (the item inside patternProperties)
    const entitySchema: Record<string, unknown> = {
        type: 'object',
        title: title,
        additionalProperties: false,
        ...entityDef,
    };

    const wrappedSchema: Record<string, unknown> = {
        type: 'object',
        additionalProperties: false,
        required: [bodyRoot],
        properties: {
            [bodyRoot]: {
                type: 'object',
                description: `Keyed object (dictionary). Each property name is an ID string (e.g., "1").`,
                minProperties: 1,
                additionalProperties: false,
                patternProperties: {
                    '^[0-9]+$': entitySchema,
                },
            },
        },
    };

    return wrappedSchema;
}


/**
 * Wrap schema with simple object structure (for POST table schemas):
 * {
 *   title, type: object, additionalProperties: false, required: [bodyRoot],
 *   properties: {
 *     Argument: {
 *       type: object, additionalProperties: false, required: [...],
 *       properties: {...}
 *     }
 *   }
 * }
 */
function wrapWithSimpleObjectStructure(
    schema: Record<string, unknown>,
    bodyRoot: string
): Record<string, unknown> {
    // Extract properties from original schema
    const { $schema, title, type, properties, required, additionalProperties, ...rest } = schema as any;

    // Build the inner Argument object
    const innerObject: Record<string, unknown> = {
        type: 'object',
        additionalProperties: false,
    };

    // Add required if present in original schema
    if (required && Array.isArray(required) && required.length > 0) {
        innerObject['required'] = required;
    }

    // Add properties
    if (properties) {
        innerObject['properties'] = properties;
    }

    // Spread any remaining properties (like allOf, oneOf, etc.)
    Object.assign(innerObject, rest);

    const wrappedSchema: Record<string, unknown> = {
        title: title,
        type: 'object',
        additionalProperties: false,
        required: [bodyRoot],
        properties: {
            [bodyRoot]: innerObject,
        },
    };

    return wrappedSchema;
}


/**
 * Generate Table schema using tableSchemaRegistry from shared.yaml
 * Automatically includes standard Table API fields (UNIT, STYLES, COMPONENTS, etc.)
 */
function generateTableSchemaFromRegistry(
    inputSchema: Record<string, unknown>,
    tableTypeEnums?: string[],
    additionalFields?: string[]
): Record<string, unknown> {
    const sharedRules = loadSharedRules();
    const tableRegistry = (sharedRules as any)?.tableSchemaRegistry;

    // DEBUG: Add info about what we found
    const debugInfo = {
        rulesDir: getRulesDir(),
        loadError: getLastLoadError(),
        sharedRulesLoaded: !!sharedRules,
        sharedRulesKeys: sharedRules ? Object.keys(sharedRules as object) : [],
        tableRegistryFound: !!tableRegistry,
        tableRegistryKeys: tableRegistry ? Object.keys(tableRegistry) : [],
    };
    console.error('[DEBUG] generateTableSchemaFromRegistry:', JSON.stringify(debugInfo, null, 2));

    if (!tableRegistry || !tableRegistry.commonFields) {
        console.error('⚠️ tableSchemaRegistry not found in shared.yaml, using input schema as-is');
        // Return with debug info attached
        return {
            ...inputSchema,
            'x-debug': debugInfo
        };
    }

    const commonFields = tableRegistry.commonFields;
    const baseFields = tableRegistry.baseFields || ['TABLE_NAME', 'TABLE_TYPE', 'EXPORT_PATH', 'UNIT', 'STYLES', 'COMPONENTS'];

    // Start with input schema properties
    const inputProperties = (inputSchema.properties as Record<string, unknown>) || {};
    const inputRequired = (inputSchema.required as string[]) || [];

    // Build properties from registry
    const properties: Record<string, unknown> = {};
    const required: string[] = [];

    // Add base fields from registry
    for (const fieldName of baseFields) {
        const fieldDef = commonFields[fieldName];
        if (fieldDef) {
            const fieldSchema: Record<string, unknown> = {
                type: fieldDef.type,
                description: fieldDef.description,
            };

            // Add default if present
            if (fieldDef.default !== undefined) {
                fieldSchema.default = fieldDef.default;
            }

            // Special handling for TABLE_TYPE - use provided enums
            if (fieldName === 'TABLE_TYPE' && tableTypeEnums && tableTypeEnums.length > 0) {
                fieldSchema.enum = tableTypeEnums;
            }

            // Add nested properties if present (for UNIT, STYLES, NODE_ELEMS)
            if (fieldDef.properties) {
                fieldSchema.properties = fieldDef.properties;
            }

            // Add array items if present
            if (fieldDef.items) {
                fieldSchema.items = fieldDef.items;
            }

            // Add constraints
            if (fieldDef.minimum !== undefined) fieldSchema.minimum = fieldDef.minimum;
            if (fieldDef.maximum !== undefined) fieldSchema.maximum = fieldDef.maximum;
            if (fieldDef.enum) fieldSchema.enum = fieldDef.enum;

            // Add x-ui from registry
            if (fieldDef['x-ui']) fieldSchema['x-ui'] = fieldDef['x-ui'];

            properties[fieldName] = fieldSchema;

            // Mark as required if specified
            if (fieldDef.required) {
                required.push(fieldName);
            }
        }
    }

    // Add additional fields from registry if specified
    if (additionalFields && additionalFields.length > 0) {
        for (const fieldName of additionalFields) {
            const fieldDef = commonFields[fieldName];
            if (fieldDef && !properties[fieldName]) {
                const fieldSchema: Record<string, unknown> = {
                    type: fieldDef.type,
                    description: fieldDef.description,
                };
                if (fieldDef.default !== undefined) fieldSchema.default = fieldDef.default;
                if (fieldDef.properties) fieldSchema.properties = fieldDef.properties;
                if (fieldDef.items) fieldSchema.items = fieldDef.items;
                if (fieldDef.enum) fieldSchema.enum = fieldDef.enum;
                if (fieldDef['x-ui']) fieldSchema['x-ui'] = fieldDef['x-ui'];

                properties[fieldName] = fieldSchema;
            }
        }
    }

    // Merge with input properties (input takes precedence for overrides)
    for (const [key, value] of Object.entries(inputProperties)) {
        properties[key] = value;
    }

    // Merge required arrays
    for (const req of inputRequired) {
        if (!required.includes(req)) {
            required.push(req);
        }
    }

    // Build final schema
    const result: Record<string, unknown> = {
        ...inputSchema,
        type: 'object',
        properties,
    };

    if (required.length > 0) {
        result.required = required;
    }

    return result;
}


/**
 * MCP Tool Definition
 */
export const saveSchemaTool = {
    name: 'save_schema',
    description: `Save AI-generated JSON Schema. Auto-validation/transformation based on shared.yaml SSOT.

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
📋 Schema Generation Rules (MUST FOLLOW - shared.yaml SSOT)
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

【Core Principles】
- Validation logic: Use standard JSON Schema keywords (allOf, if/then, required, enum)
- UI markers: x-* prefix (removing them won't affect validation)

【Field Naming Rules (MUST FOLLOW)】
🚨 DO NOT use long descriptive names like "bCombinedShearTorsion"
✅ Use abbreviated/concise keys with Hungarian notation prefix:
   - Prefix: i=integer, b=boolean, d=number, s=string, n=number
   - Use UPPERCASE abbreviations after prefix
   - Max 15-20 characters recommended

Examples:
   ❌ bCombinedShearTorsion → ✅ bCOMB_ST
   ❌ bFlexuralStrengthCheck → ✅ bFLEX_CHK  
   ❌ bPrincipalStressMaxShear → ✅ bPRIN_SHEAR
   ❌ sDesignCode → ✅ sDESIGN_CD
   ❌ iTendonType → ✅ iTENDON
   ❌ dExposureUserValue → ✅ dEXP_USER

【oneOf Pattern (RadioGroup/Dropdown with integer values)】
🎯 Use oneOf with const+title for integer/string options:

Example - RadioGroup with integer values:
  "iCONST_TYPE": {
    "type": "integer",
    "default": 1,
    "oneOf": [
      { "const": 0, "title": "Segmental" },
      { "const": 1, "title": "Non-Segmental" }
    ],
    "x-ui": { "component": "RadioGroup", "label": "Construction Type" }
  }

Example - Dropdown with string values:
  "sDESIGN_CD": {
    "type": "string",
    "default": "AASHTO-LRFD20",
    "oneOf": [
      { "const": "AASHTO-LRFD20", "title": "AASHTO-LRFD20" },
      { "const": "AASHTO-LRFD17", "title": "AASHTO-LRFD17" }
    ],
    "x-ui": { "component": "Dropdown", "label": "Design Code" }
  }

【Field Ordering (IMPORTANT)】
📌 Properties are rendered in the order they appear in the schema.
📌 Use x-ui.order for explicit ordering if needed.
📌 Group related fields together logically.

Recommended field order:
1. Primary selection fields (Design Code, Type selectors)
2. Input parameters (grouped by category)
3. Output/calculation options (checkboxes)
4. Conditional fields last

【Section Grouping with x-ui.groupId】
📌 Use x-ui.groupId to organize fields into visual sections:

Example:
  "iTENDON": {
    "type": "integer",
    "x-ui": { "label": "Tendon Type", "groupId": "inputParams" }
  },
  "bFLEX_CHK": {
    "type": "boolean",
    "x-ui": { "label": "Flexural Strength Check", "groupId": "outputParams" }
  }

【Valid x-* Markers (markerRegistry SSOT)】
✅ x-ui: UI metadata (label, groupId, hint, groups, component, order)
✅ x-transport: API transport info
✅ x-enum-labels: Enum value labels (UI only) - for simple enum arrays
✅ x-enum-labels-by-type: Enum labels by TYPE
✅ x-required-when: Show as required in UI when condition is met
✅ x-optional-when: Show as optional in UI when condition is met

【Conditional Fields (x-optional-when)】
Example - Field visible only when another field has specific value:
  "dEXP_USER": {
    "type": "number",
    "x-optional-when": { "iEXP_FACT": 2 },
    "x-ui": { "label": "User Value" }
  }

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
⚠️ MANDATORY: When to Use 'questions' Parameter (MUST ASK)
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

🚨 DO NOT generate schema if ANY of these are uncertain:

1. **Dropdown/Enum options incomplete**
   - If only ONE option is visible (e.g., "AASHTO-LRFD20" in dropdown)
   - You MUST ask: "What are all available options for this dropdown?"
   
2. **Field type/enum values unclear**
   - Ask before assuming string/integer/boolean

3. **Conditional logic complex**
   - Ask to clarify dependencies between fields

4. **Default values uncertain**
   - Ask what the default should be

📋 How to use 'questions' parameter:
  {
    "schemaName": "psc_design_code",
    "schema": {},  // Empty or partial schema
    "questions": [
      {
        "field": "sDesignCode",
        "question": "What are all available Design Code options for the dropdown?",
        "context": "Image shows only 'AASHTO-LRFD20' selected",
        "suggestion": ["AASHTO-LRFD20", "AASHTO-LRFD17", "Eurocode2"]
      }
    ]
  }

When questions are provided, tool returns 'pending_questions' status instead of saving.

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━`,
    inputSchema: {
        type: 'object' as const,
        properties: {
            schemaName: {
                type: 'string',
                description: 'Schema filename (without extension)',
            },
            outputDir: {
                type: 'string',
                description: 'Output subdirectory. Default: default',
                default: 'default',
            },
            schema: {
                type: 'object',
                description: 'AI-generated JSON Schema object',
            },
            description: {
                type: 'string',
                description: 'Schema description (optional)',
            },
            questions: {
                type: 'array',
                description: 'List of questions for uncertain fields',
                items: {
                    type: 'object',
                    properties: {
                        field: { type: 'string' },
                        question: { type: 'string' },
                        context: { type: 'string' },
                        suggestion: {},
                    },
                    required: ['field', 'question'],
                },
            },
            skipValidation: {
                type: 'boolean',
                description: 'Skip validation (for debugging)',
                default: false,
            },
            wrapWithEntityCollection: {
                type: 'boolean',
                description: 'Wrap schema with Assign → additionalProperties structure (for entity collection APIs)',
                default: false,
            },
            wrapWithSimpleObject: {
                type: 'boolean',
                description: 'Wrap schema with simple Argument object structure (for POST table schemas). No additionalProperties, just direct properties under Argument.',
                default: false,
            },
            generateTableSchema: {
                type: 'boolean',
                description: 'Generate Table API schema using tableSchemaRegistry from shared.yaml. Automatically includes standard fields (TABLE_NAME, TABLE_TYPE, UNIT, STYLES, COMPONENTS, etc.)',
                default: false,
            },
            tableTypeEnums: {
                type: 'array',
                items: { type: 'string' },
                description: 'TABLE_TYPE enum values. Required when generateTableSchema is true.',
            },
            additionalTableFields: {
                type: 'array',
                items: { type: 'string' },
                description: 'Additional fields from tableSchemaRegistry to include (e.g., NODE_ELEMS, LOAD_CASE_NAMES, OPT_CS, STAGE_STEP)',
            },
            bodyRoot: {
                type: 'string',
                description: 'Body root key when using wrapWithEntityCollection or wrapWithSimpleObject (default: "Assign" or "Argument")',
                default: 'Assign',
            },
            excludeSchemaField: {
                type: 'boolean',
                description: 'Exclude $schema field from output JSON. Default: true (OpenAPI 3.1 compatible). Set to false to include $schema.',
                default: true,
            },
        },
        required: ['schemaName', 'schema'],
    },
};
