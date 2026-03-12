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
import { loadSharedRules, getRulesDir, getLastLoadError, loadAccuracyPolicy } from '../utils/rules-loader.js';
import { validateWithAccuracyPolicy } from '../utils/accuracy-validator.js';
import type { AccuracyPolicy } from '../types/accuracy-types.js';
import { removeInternalMarkers } from '../utils/schema-cleaner.js';

export interface SaveSchemaInput {
    schemaName: string;
    schema: Record<string, unknown>;
    description?: string;
    outputDir?: string;
    questions?: Question[];
    skipValidation?: boolean;
    confirmed?: boolean;  // NEW: Set to true to skip review and save directly
    entityType?: 'collection' | 'single';  // NEW: Explicit entity type selection
    wrapWithEntityCollection?: boolean;  // DEPRECATED: use entityType: 'collection'
    wrapWithSimpleObject?: boolean;  // DEPRECATED: use entityType: 'single'
    generateTableSchema?: boolean;
    tableTypeEnums?: string[];
    componentEnums?: string[];
    componentEnumLabelsByType?: Record<string, Record<string, string>>;
    partEnums?: string[];
    additionalTableFields?: string[];
    bodyRoot?: string;
    excludeSchemaField?: boolean;
}


export interface Question {
    field: string;
    question: string;
    context?: string;
    suggestion?: unknown;
}

export interface SaveSchemaResult {
    ok: boolean;
    status: 'saved' | 'pending_questions' | 'pending_review';  // NEW: pending_review
    schemaPath?: string;
    metaPath?: string;
    outputHash?: string;
    pendingQuestions?: Question[];
    processedSchema?: Record<string, unknown>;  // NEW: Schema with auto-additions
    autoAddedFields?: string[];  // NEW: List of auto-added fields
    message?: string;  // NEW: Message for LLM
    validation?: {
        errors: Array<{ field: string; message: string; fixApplied?: boolean }>;
        warnings: Array<{ field: string; message: string }>;
        evidenceCoverage?: number;
        autoQuestionsGenerated?: number;
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
        // 1. Clean input schema defaults FIRST
        let schema = cleanInputSchemaDefaults(input.schema);

        // 2. Validate schema if not skippedut
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
            componentEnums: input.componentEnums,
            componentEnumLabelsByType: input.componentEnumLabelsByType,
            additionalTableFields: input.additionalTableFields,
        }, null, 2));

        // 3. Generate Table schema ONLY if explicitly requested by LLM
        // ⚠️ NO AUTO-DETECTION - LLM must read promptRules.yaml and decide
        // LLM workflow:
        //   1. Read shared.yaml (tableSchemaRegistry)
        //   2. Read promptRules.yaml (tableDetectionRules)
        //   3. Decide if generateTableSchema: true/false
        //   4. Call save_schema with explicit flag
        const isTableSchema = input.generateTableSchema === true;

        // 3. Generate Table schema from registry ONLY if LLM explicitly requested
        if (isTableSchema) {
            console.error('[DEBUG] Generating Table schema from registry...');
            schema = generateTableSchemaFromRegistry(schema, input.tableTypeEnums, input.componentEnums, input.componentEnumLabelsByType, input.partEnums, input.additionalTableFields);

            // 🔥 CRITICAL: Final cleanup after table schema generation
            // Remove defaults from required fields (TABLE_TYPE, etc.)
            console.error('[DEBUG] Final cleanup: removing defaults from required fields...');
            schema = cleanInputSchemaDefaults(schema);
        }

        // 4. Validate and transform based on SSOT rules
        let validationResult = null;

        if (!input.skipValidation) {
            validationResult = validateAndTransform(schema);
            schema = validationResult.transformed as Record<string, unknown>;
        }

        // 4a. Accuracy policy validation (evidence-based)
        const accuracyPolicy = loadAccuracyPolicy() as AccuracyPolicy | null;
        let accuracyResult = null;
        const autoGeneratedQuestions: SaveSchemaInput['questions'] = [];

        if (accuracyPolicy && !input.skipValidation) {
            console.error('[DEBUG] Running accuracy policy validation...');
            accuracyResult = validateWithAccuracyPolicy(schema, accuracyPolicy);

            // Convert auto-generated questions to Question format
            for (const autoQ of accuracyResult.autoQuestions) {
                autoGeneratedQuestions.push({
                    field: autoQ.field,
                    question: autoQ.question,
                    context: autoQ.context,
                    suggestion: undefined
                });
            }

            // If there are auto-questions or evidence errors, return them
            if (autoGeneratedQuestions.length > 0 || accuracyResult.errors.length > 0) {
                console.error(`[DEBUG] Accuracy validation: ${accuracyResult.errors.length} errors, ${autoGeneratedQuestions.length} auto-questions`);
                console.error('[DEBUG] Evidence coverage:', (accuracyResult.evidenceCoverage * 100).toFixed(0) + '%');
            }
        }

        // Return auto-generated questions if present (before wrapping)
        // 🚨 DISABLED: Questions no longer block saving - treat as warnings only
        if (autoGeneratedQuestions.length > 0) {
            console.error(`[WARNING] ${autoGeneratedQuestions.length} auto-questions generated (not blocking save):`);
            console.error(JSON.stringify(autoGeneratedQuestions, null, 2));
            // Continue with save instead of blocking
            /* ORIGINAL BLOCKING CODE:
            return {
                ok: true,
                status: 'pending_questions',
                pendingQuestions: autoGeneratedQuestions,
                validation: accuracyResult ? {
                    errors: [],
                    warnings: [],
                    evidenceCoverage: accuracyResult.evidenceCoverage,
                    autoQuestionsGenerated: autoGeneratedQuestions.length
                } : undefined
            };
            */
        }

        // 4. Determine entity type with priority: explicit entityType > legacy options > auto-detect
        let effectiveEntityType: 'collection' | 'single' | null = null;

        if (input.entityType) {
            // Priority 1: Explicit entityType parameter
            effectiveEntityType = input.entityType;
            console.error(`[WRAPPER] Using explicit entityType: ${effectiveEntityType}`);
        } else if (input.wrapWithEntityCollection) {
            // Priority 2: Legacy wrapWithEntityCollection
            effectiveEntityType = 'collection';
            console.error('[WRAPPER] Using legacy wrapWithEntityCollection → collection');
        } else if (input.wrapWithSimpleObject) {
            // Priority 3: Legacy wrapWithSimpleObject
            effectiveEntityType = 'single';
            console.error('[WRAPPER] Using legacy wrapWithSimpleObject → single');
        } else {
            // Priority 4: Auto-detect from schemaName
            effectiveEntityType = detectEntityType(input.schemaName);
            console.error(`[WRAPPER] Auto-detected entityType from name "${input.schemaName}": ${effectiveEntityType}`);
        }

        // Determine bodyRoot default based on effective entity type (unless explicitly provided)
        let resolvedBodyRoot = input.bodyRoot || (effectiveEntityType === 'single' ? 'Argument' : 'Assign');
        // If single entityType but bodyRoot defaults to Assign, override to Argument
        if (effectiveEntityType === 'single' && resolvedBodyRoot === 'Assign') {
            console.error('[WRAPPER] Overriding bodyRoot Assign -> Argument for single entityType');
            resolvedBodyRoot = 'Argument';
        }

        // If schema already has a wrapper, do NOT wrap again
        const existingWrapper =
            schema?.type === 'object' &&
            Array.isArray(schema.required) &&
            schema.properties &&
            (schema.required.includes('Assign') || schema.required.includes('Argument'))
                ? (schema.required.includes('Assign') ? 'Assign' : 'Argument')
                : null;

        if (existingWrapper) {
            if (effectiveEntityType === 'collection' && existingWrapper !== 'Assign') {
                console.error(`[WRAPPER] Warning: schema already wrapped with ${existingWrapper} but entityType=collection requested. Skipping wrap.`);
            } else if (effectiveEntityType === 'single' && existingWrapper !== 'Argument') {
                console.error(`[WRAPPER] Warning: schema already wrapped with ${existingWrapper} but entityType=single requested. Skipping wrap.`);
            } else {
                console.error(`[WRAPPER] Schema already wrapped with ${existingWrapper}. Skipping wrap.`);
            }
            effectiveEntityType = null;
        }

        // If schema is a components-only bundle (OpenAPI-style), do NOT wrap
        const hasComponentsSchemas =
            schema &&
            typeof schema === 'object' &&
            (schema as any).components &&
            (schema as any).components.schemas;
        if (hasComponentsSchemas) {
            console.error('[WRAPPER] Schema contains components.schemas; skipping wrapper.');
            effectiveEntityType = null;
        }

        // Apply wrapping based on entityType
        if (effectiveEntityType === 'collection') {
            schema = wrapWithEntityCollectionStructure(schema, resolvedBodyRoot);
        } else if (effectiveEntityType === 'single') {
            schema = wrapWithSimpleObjectStructure(schema, resolvedBodyRoot);
        }

        // 5. Clean up internal markers (x-evidence)
        // Evidence is for validation only, not for final schema
        schema = removeInternalMarkers(schema);

        // ⚠️ IMPORTANT: Return processed schema to LLM for review
        // LLM should show user what was auto-generated (TABLE_NAME, TABLE_TYPE, etc.)
        // Then LLM calls save_schema again with confirmed: true to actually save
        // ✅ FIX 3: Remove pending_review workflow - always save directly
        // LLM should read rules first, generate complete schema, then save
        // No intermediate confirmation needed

        // 6. Handle $schema field
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

// ============================================================================
// HELPER FUNCTIONS
// ============================================================================

/**
 * Auto-detect if schema is table-related based on name patterns
 */
function detectTableSchema(schemaName: string): boolean {
    const tableKeywords = [
        'activation',  // records_activation, beam_activation
        'records',     // records table
        'filter',      // result filtering
        'table',       // explicit table
        'result',      // result table
        'output'       // output table
        // ❌ 'dialog' removed - too broad, catches settings dialogs
    ];

    const lowerName = schemaName.toLowerCase();
    return tableKeywords.some(keyword => lowerName.includes(keyword));
}

/**
 * Auto-detect entity type based on schema name patterns
 * @returns 'collection' for entity assignment APIs (default), 'single' for table/result/graphic outputs
 * 
 * RULE SUMMARY:
 * - Collection (Assign + patternProperties): 부재/요소에 설정을 할당하는 API (기본값)
 *   Examples: set_concrete_design_code, set_beam_properties, assign_material
 * 
 * - Single (Argument): 테이블, 그래픽, 결과 출력 등 단일 데이터 조회/표시
 *   Examples: get_table_force, get_result_displacement, show_graphic_moment
 */
function detectEntityType(schemaName: string): 'collection' | 'single' {
    const lowerName = schemaName.toLowerCase();

    // Single Object patterns (Argument wrapper - for output/display/table/result APIs)
    // These return single data sets, not ID-keyed entity collections
    const singlePatterns = [
        // Output/Display APIs
        'table',        // Table data (get_table_force, table_beam_design)
        'grid',         // Grid display
        'result',       // Result output (get_result_displacement, result_stress)
        'output',       // General output data
        'report',       // Report generation
        
        // Graphics/Visualization  
        'graphic',      // Graphic display (show_graphic_moment)
        'chart',        // Chart display
        'plot',         // Plot display
        'view',         // View display data
        'diagram',      // Diagram display (moment_diagram)
        
        // Global Settings (not entity-specific)
        'settings',     // Global settings
        'config',       // Configuration
        'preference',   // Preferences
    ];

    // Check single patterns
    if (singlePatterns.some(pattern => lowerName.includes(pattern))) {
        const matchedPattern = singlePatterns.find(p => lowerName.includes(p));
        console.error(`[WRAPPER] Auto-detected as 'single' (matched pattern: ${matchedPattern})`);
        return 'single';
    }

    // Default to 'collection' (most Midas APIs assign settings to entities)
    // Format: { "Assign": { "1": {...}, "2": {...} } }
    // Examples: set_*, assign_*, define_* APIs for beams, columns, walls, etc.
    console.error(`[WRAPPER] Auto-detected as 'collection' (default - entity assignment API)`);
    return 'collection';
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
 * Clean input schema defaults:
 * 1. Remove defaults from required fields (must be user-provided)
 * 2. Remove multi-value array defaults (should be single value or empty)
 */
function cleanInputSchemaDefaults(
    schema: Record<string, unknown>
): Record<string, unknown> {
    const cleaned = JSON.parse(JSON.stringify(schema));
    const properties = (cleaned.properties as Record<string, any>) || {};
    const required = (cleaned.required as string[]) || [];

    // Clean direct properties
    for (const [fieldName, fieldDef] of Object.entries(properties)) {
        // Remove default if field is required
        if (required.includes(fieldName) && fieldDef.default !== undefined) {
            console.error(`[CLEAN] Removing default from required field: ${fieldName}`);
            delete fieldDef.default;
        }

        // Remove multi-value array defaults
        if (Array.isArray(fieldDef.default) && fieldDef.default.length > 1) {
            console.error(`[CLEAN] Removing multi-value array default from: ${fieldName}`);
            delete fieldDef.default;
        }

        // Clean nested Argument wrapper
        if (fieldName === 'Argument' && fieldDef.properties) {
            const argRequired = (fieldDef.required as string[]) || [];
            for (const [argField, argDef] of Object.entries(fieldDef.properties)) {
                const typedArgDef = argDef as any;
                if (argRequired.includes(argField) && typedArgDef.default !== undefined) {
                    console.error(`[CLEAN] Removing default from required Argument field: ${argField}`);
                    delete typedArgDef.default;
                }
                if (Array.isArray(typedArgDef.default) && typedArgDef.default.length > 1) {
                    console.error(`[CLEAN] Removing multi-value array default from Argument: ${argField}`);
                    delete typedArgDef.default;
                }
            }
        }
    }

    return cleaned;
}


/**
 * Remove units from enum labels
 * Strips patterns like (N/mm²), (kN), (mm), etc. from label strings
 */
function removeUnitsFromLabels(
    labelsByType: Record<string, Record<string, string>>
): Record<string, Record<string, string>> {
    const cleaned: Record<string, Record<string, string>> = {};

    for (const [type, labels] of Object.entries(labelsByType)) {
        cleaned[type] = {};
        for (const [key, label] of Object.entries(labels)) {
            // Remove units in parentheses: (N/mm²), (kN), (mm), etc.
            // Pattern: \s*\([^)]+\)\s*$ matches whitespace + (anything) at end of string
            cleaned[type][key] = label.replace(/\s*\([^)]+\)\s*$/g, '').trim();
        }
    }

    return cleaned;
}


/**
 * Generate Table schema using tableSchemaRegistry from shared.yaml
 * Automatically includes standard Table API fields (UNIT, STYLES, COMPONENTS, etc.)
 */
function generateTableSchemaFromRegistry(
    inputSchema: Record<string, unknown>,
    tableTypeEnums?: string[],
    componentEnums?: string[],
    componentEnumLabelsByType?: Record<string, Record<string, string>>,
    partEnums?: string[],
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
    const tableSpecificFields = tableRegistry.tableSpecificFields || {};
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

            // ✅ FIX 1: Only add default if NOT required (required fields must be user-provided)
            const isRequired = fieldDef.required === true;
            if (fieldDef.default !== undefined && !isRequired) {
                // ✅ FIX 2: For arrays, ensure default has only 0 or 1 element
                if (Array.isArray(fieldDef.default)) {
                    if (fieldDef.default.length === 0) {
                        fieldSchema.default = [];
                    } else if (fieldDef.default.length === 1) {
                        fieldSchema.default = fieldDef.default;
                    }
                    // Skip if multiple values - no default
                } else {
                    fieldSchema.default = fieldDef.default;
                }
            }

            // Special handling for TABLE_TYPE - use provided enums
            if (fieldName === 'TABLE_TYPE' && tableTypeEnums && tableTypeEnums.length > 0) {
                fieldSchema.enum = tableTypeEnums;
            }

            // Special handling for COMPONENTS - use provided enums and labels by type
            if (fieldName === 'COMPONENTS' && componentEnums && componentEnums.length > 0) {
                fieldSchema.items = {
                    type: 'string',
                    enum: componentEnums
                };
                // Add x-enum-labels-by-type if provided (with units removed)
                if (componentEnumLabelsByType && Object.keys(componentEnumLabelsByType).length > 0) {
                    fieldSchema['x-enum-labels-by-type'] = removeUnitsFromLabels(componentEnumLabelsByType);
                }
            }

            // Add nested properties if present (for UNIT, STYLES, NODE_ELEMS)
            if (fieldDef.properties) {
                fieldSchema.properties = fieldDef.properties;
            }

            // Add array items if present (but not for COMPONENTS if we already set it)
            if (fieldDef.items && !(fieldName === 'COMPONENTS' && componentEnums && componentEnums.length > 0)) {
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
            if (isRequired) {
                required.push(fieldName);
            }
        }
    }

    // Add additional fields from registry if specified (search both commonFields and tableSpecificFields)
    if (additionalFields && additionalFields.length > 0) {
        for (const fieldName of additionalFields) {
            // Look in commonFields first, then tableSpecificFields
            const fieldDef = commonFields[fieldName] || tableSpecificFields[fieldName];
            if (fieldDef && !properties[fieldName]) {
                const fieldSchema: Record<string, unknown> = {
                    type: fieldDef.type,
                    description: fieldDef.description,
                };
                if (fieldDef.default !== undefined) fieldSchema.default = fieldDef.default;
                if (fieldDef.properties) fieldSchema.properties = fieldDef.properties;

                // Special handling for PARTS - use provided partEnums if available
                if (fieldName === 'PARTS' && fieldDef.items) {
                    fieldSchema.items = { ...fieldDef.items };
                    if (partEnums && partEnums.length > 0) {
                        (fieldSchema.items as Record<string, unknown>).enum = partEnums;
                    }
                } else if (fieldDef.items) {
                    fieldSchema.items = fieldDef.items;
                }

                if (fieldDef.enum) fieldSchema.enum = fieldDef.enum;
                if (fieldDef.oneOf) fieldSchema.oneOf = fieldDef.oneOf;
                if (fieldDef['x-ui']) fieldSchema['x-ui'] = fieldDef['x-ui'];
                if (fieldDef['x-optional-when']) fieldSchema['x-optional-when'] = fieldDef['x-optional-when'];

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

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
📖 MUST READ FIRST: Schema Generation Guidelines
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

🚨 **REQUIRED WORKFLOW:**
   1. read_resource('mcp://rules/shared.yaml')     // Table registry, common fields
   2. read_resource('mcp://rules/promptRules.yaml') // LLM guidelines, critical warnings
   3. Generate schema following the rules
   4. Call save_schema with complete schema

⚠️ **DO NOT skip step 1 & 2** - All field naming rules, critical warnings, and
   patterns are defined in these external files (NOT in this description).

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
🔥 Quick Reference (Full details in promptRules.yaml)
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

**Table Schemas:**
✅ ALWAYS: componentEnums + generateTableSchema: true + wrapWithSimpleObject: true
❌ NEVER include: SELECT_TYPE, Element Type dropdowns (UI-only controls)

**Field Naming:**
✅ Use: bCOMB_ST, iFLEX_CHK, sDESIGN_CD (abbreviated, max 15-20 chars)
❌ Avoid: bCombinedShearTorsion, bFlexuralStrengthCheck (too long)

**When Uncertain:**
📋 Use 'questions' parameter instead of guessing
Example: questions: [{ field: "sDESIGN_CD", question: "What are all dropdown options?" }]

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
📂 Related Resources (via read_resource)
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

- mcp://rules/shared.yaml        → Table registry, markers, common fields
- mcp://rules/promptRules.yaml   → LLM guidelines, critical warnings, patterns
- mcp://rules/mcp.yaml           → Field abbreviations dictionary
- mcp://rules/accuracyPolicy.yaml → Auto-question patterns

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━`,
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
            entityType: {
                type: 'string',
                enum: ['collection', 'single'],
                description: `Entity type for wrapper structure:
- 'collection': Entity Collection (ELEM, NODE, MATL) → patternProperties wrapper
- 'single': Single Object (Settings, Config) → direct properties wrapper
If not specified, auto-detected from schemaName patterns.`,
            },
            wrapWithEntityCollection: {
                type: 'boolean',
                description: '[DEPRECATED - use entityType: "collection"] Wrap schema with Assign → patternProperties structure (for entity collection APIs)',
                default: false,
            },
            wrapWithSimpleObject: {
                type: 'boolean',
                description: '[DEPRECATED - use entityType: "single"] Wrap schema with simple Assign/Argument object structure.',
                default: false,
            },
            generateTableSchema: {
                type: 'boolean',
                description: 'Generate Table API schema using tableSchemaRegistry. REQUIRED for ALL table-related schemas (result tables, dialogs, activation, filtering). Automatically includes TABLE_NAME, TABLE_TYPE, EXPORT_PATH, UNIT, STYLES, COMPONENTS. Set to true for any schema related to table results or table filtering.',
                default: false,
            },
            tableTypeEnums: {
                type: 'array',
                items: { type: 'string' },
                description: 'TABLE_TYPE enum values. Required when generateTableSchema is true.',
            },
            componentEnums: {
                type: 'array',
                items: { type: 'string' },
                description: 'COMPONENTS array item enum values. CRITICAL: When user provides a table image, YOU MUST read the column headers from the table and provide them here. Extract ALL visible column names (e.g., ["Elem", "Part", "Girder/Slab", "Comp./Tens.", "Stage", "CHK", "FT", "FB", "FTL", "FBL", "FTR", "FBR", "FMAX", "ALW"]). DO NOT leave this empty if generateTableSchema is true.',
            },
            componentEnumLabelsByType: {
                type: 'object',
                description: 'x-enum-labels-by-type for COMPONENTS. Maps TABLE_TYPE to enum labels. CRITICAL: When you provide componentEnums, YOU SHOULD also provide meaningful labels for each component. Example: {"STRESSCS": {"FT": "Stress at Top (N/mm²)", "FB": "Stress at Bottom (N/mm²)", "Elem": "Element Number", "Part": "Part Number", "CHK": "Check Result"}}. This helps users understand what each component column represents.',
            },
            partEnums: {
                type: 'array',
                items: { type: 'string' },
                description: 'PARTS array enum values (e.g., ["PartI", "PartJ", "Part 1/4", "Part 2/4", "Part 3/4"]). Used when additionalTableFields includes "PARTS". This overrides the default PARTS enum from shared.yaml.',
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
