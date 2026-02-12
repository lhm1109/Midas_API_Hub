/**
 * parse_dialog MCP Tool
 *
 * Parses MFC dialog source code (.cpp/.h) and RC resource files to extract:
 *   - Dialog controls and their types
 *   - DDX data bindings (member variable ↔ control)
 *   - Conditional enable/disable/show/hide logic
 *
 * Output is saved to generated_resources/{ClassName}.json
 * The LLM then uses this info to call save_schema with proper conditional logic.
 */

import * as fs from 'fs';
import * as path from 'path';
import { findDialogFiles, readSourceFile, toRelativePath } from '../utils/dialog-finder.js';
import { parseRCFile, findDialogInRC, getMeaningfulControls, RCDialog, RCControl } from '../utils/rc-parser.js';
import { parseCppFile, DDXMapping, ConditionalBlock } from '../utils/cpp-parser.js';
import { buildResourcePath } from '../utils/path-guard.js';
import { toCanonicalJSON } from '../utils/deterministic-json.js';

// ============================================================================
// INTERFACES
// ============================================================================

export interface ParseDialogInput {
    dialogClass: string;        // e.g., "CNodeCreateDlg" or "NodeCreateDlg"
    extraSearchPaths?: string[]; // Additional search paths (relative to project root)
}

export interface EnrichedControl {
    id: string;
    type: string;
    caption?: string;
    labelText?: string;
    x: number;
    y: number;
    w: number;
    h: number;
    // From DDX
    variable?: string;
    inferredType?: string;
    ddxFunction?: string;
}

export interface FieldConditional {
    controlId: string;
    variable?: string;       // DDX member variable name
    action: 'enable' | 'disable' | 'show' | 'hide';
    triggerVariable: string; // What variable triggers this
    condition?: string;      // Raw condition expression
}

export interface DialogResourceInfo {
    dialogClass: string;
    dialogId?: string;
    sourceFile?: string;
    headerFile?: string;
    rcFile?: string;
    caption?: string;
    // All parsed controls (enriched with DDX info)
    controls: EnrichedControl[];
    // Conditional logic
    conditionals: FieldConditional[];
    // Control group arrays: m_aCtrlXxx → [IDC_xxx, ...] (for enable/disable group logic)
    controlGroupArrays?: Record<string, string[]>;
    // Raw data for LLM analysis
    rawDoDataExchange?: string;
    rawUpdateLogic?: string;
    rawOnChangeHandlers?: Array<{ name: string; body: string }>;
    // Parsing metadata
    parsedAt: string;
    warnings: string[];
}

export interface ParseDialogResult {
    ok: boolean;
    dialogClass: string;
    resourceJsonPath?: string;
    info?: DialogResourceInfo;
    error?: string;
    message?: string;
}

// ============================================================================
// MAIN FUNCTION
// ============================================================================

export async function parseDialog(input: ParseDialogInput): Promise<ParseDialogResult> {
    const { dialogClass } = input;
    const warnings: string[] = [];

    try {
        // 1. Find source files
        console.error(`[parse_dialog] Searching for: ${dialogClass}`);
        const fileSet = await findDialogFiles(dialogClass, input.extraSearchPaths);

        if (!fileSet.cppFile && !fileSet.hFile) {
            return {
                ok: false,
                dialogClass,
                error: `Source files not found for: ${dialogClass}`,
                message: `Could not find .cpp/.h files for "${dialogClass}" in ui_src/. ` +
                    `Make sure the file exists in ui_src/wg_cmd/ or provide extraSearchPaths.`,
            };
        }

        console.error(`[parse_dialog] Found files:`);
        if (fileSet.cppFile) console.error(`  cpp: ${fileSet.cppFile}`);
        if (fileSet.hFile)   console.error(`  h:   ${fileSet.hFile}`);
        if (fileSet.rcFile)  console.error(`  rc:  ${fileSet.rcFile}`);
        if (fileSet.dialogId) console.error(`  IDD: ${fileSet.dialogId}`);

        // 2. Read source files
        const cppContent = fileSet.cppFile ? readSourceFile(fileSet.cppFile) : '';
        const hContent = fileSet.hFile ? readSourceFile(fileSet.hFile) : '';

        // 3. Parse C++ source
        const cppInfo = parseCppFile(cppContent, hContent);
        if (!cppInfo.dialogId && fileSet.dialogId) {
            cppInfo.dialogId = fileSet.dialogId;
        }

        // 4. Parse RC file
        let rcDialog: RCDialog | null = null;
        if (fileSet.rcFile && cppInfo.dialogId) {
            const rcContent = readSourceFile(fileSet.rcFile);
            rcDialog = findDialogInRC(rcContent, cppInfo.dialogId);
            if (!rcDialog) {
                warnings.push(`Dialog ID "${cppInfo.dialogId}" not found in RC file: ${toRelativePath(fileSet.rcFile)}`);
            }
        } else if (!fileSet.rcFile) {
            warnings.push('RC file not found. Control layout will be missing.');
        } else if (!cppInfo.dialogId) {
            warnings.push('Dialog ID (IDD_xxx) not found in header. Cannot extract RC controls.');
        }

        // 5. Build enriched control list
        const rcControls = rcDialog ? getMeaningfulControls(rcDialog) : [];
        const enrichedControls = enrichControls(rcControls, cppInfo.ddxMappings);

        // 6. Build conditional list
        const conditionals = buildConditionals(cppInfo.conditionalBlocks, cppInfo.ddxMappings);

        // 7. Assemble result
        const info: DialogResourceInfo = {
            dialogClass,
            dialogId: cppInfo.dialogId,
            sourceFile: fileSet.cppFile ? toRelativePath(fileSet.cppFile) : undefined,
            headerFile: fileSet.hFile ? toRelativePath(fileSet.hFile) : undefined,
            rcFile: fileSet.rcFile ? toRelativePath(fileSet.rcFile) : undefined,
            caption: rcDialog?.caption,
            controls: enrichedControls,
            conditionals,
            controlGroupArrays: cppInfo.controlGroupArrays,
            rawDoDataExchange: cppInfo.rawDoDataExchange,
            rawUpdateLogic: cppInfo.rawUpdateLogic,
            rawOnChangeHandlers: cppInfo.onChangeHandlers.map(h => ({
                name: h.handlerName,
                body: h.body,
            })),
            parsedAt: new Date().toISOString(),
            warnings,
        };

        // 8. Save to generated_resources/{ClassName}.json
        const resourcePath = buildResourcePath(dialogClass);
        const json = toCanonicalJSON(info as unknown as Record<string, unknown>);
        fs.writeFileSync(resourcePath, json, 'utf-8');
        console.error(`[parse_dialog] Saved resource JSON: ${resourcePath}`);

        return {
            ok: true,
            dialogClass,
            resourceJsonPath: resourcePath,
            info,
            message: buildSummaryMessage(info),
        };

    } catch (error) {
        const message = error instanceof Error ? error.message : String(error);
        console.error(`[parse_dialog] Error:`, message);
        return {
            ok: false,
            dialogClass,
            error: message,
        };
    }
}

// ============================================================================
// HELPERS
// ============================================================================

/**
 * Merge RC controls with DDX mappings to produce enriched control list
 */
function enrichControls(rcControls: RCControl[], ddxMappings: DDXMapping[]): EnrichedControl[] {
    // Build DDX lookup map: controlId → DDXMapping
    const ddxMap = new Map<string, DDXMapping>();
    for (const ddx of ddxMappings) {
        // Prefer non-Control (DDX_Control is just binding, not data)
        if (!ddxMap.has(ddx.controlId) || ddx.function !== 'DDX_Control') {
            ddxMap.set(ddx.controlId, ddx);
        }
    }

    // Enrich RC controls
    const enriched: EnrichedControl[] = rcControls.map(ctrl => {
        const ddx = ddxMap.get(ctrl.id);
        return {
            id: ctrl.id,
            type: ctrl.type,
            caption: ctrl.caption,
            labelText: ctrl.labelText,
            x: ctrl.x,
            y: ctrl.y,
            w: ctrl.w,
            h: ctrl.h,
            variable: ddx?.variable,
            inferredType: ddx?.inferredType,
            ddxFunction: ddx?.function,
        };
    });

    // Add DDX-only controls (not in RC but have DDX mappings)
    for (const ddx of ddxMappings) {
        if (ddx.function === 'DDX_Control') continue; // skip bare control bindings
        const alreadyInRC = enriched.some(c => c.id === ddx.controlId);
        if (!alreadyInRC) {
            enriched.push({
                id: ddx.controlId,
                type: 'UNKNOWN',   // not in RC
                x: 0, y: 0, w: 0, h: 0,
                variable: ddx.variable,
                inferredType: ddx.inferredType,
                ddxFunction: ddx.function,
            });
        }
    }

    return enriched;
}

/**
 * Convert ConditionalBlock + DDX info into flat FieldConditional list
 */
function buildConditionals(
    blocks: ConditionalBlock[],
    ddxMappings: DDXMapping[]
): FieldConditional[] {
    // Build variable → controlId lookup
    const varToControl = new Map<string, string>();
    for (const ddx of ddxMappings) {
        if (ddx.variable && ddx.controlId) {
            varToControl.set(ddx.variable, ddx.controlId);
        }
    }

    const result: FieldConditional[] = [];

    for (const block of blocks) {
        for (const action of block.actions) {
            // Look up variable for the affected control
            const variable = [...varToControl.entries()]
                .find(([, ctrlId]) => ctrlId === action.controlId)?.[0];

            result.push({
                controlId: action.controlId,
                variable,
                action: action.action,
                triggerVariable: block.triggerVariable,
                condition: action.condition,
            });
        }
    }

    return result;
}

/**
 * Build a human-readable summary message for the LLM
 */
function buildSummaryMessage(info: DialogResourceInfo): string {
    const lines: string[] = [];

    lines.push(`✅ Dialog parsed: ${info.dialogClass}`);
    if (info.dialogId) lines.push(`   IDD: ${info.dialogId}`);
    if (info.caption) lines.push(`   Caption: "${info.caption}"`);
    lines.push('');

    lines.push(`📋 Controls (${info.controls.length} total):`);
    const inputControls = info.controls.filter(c =>
        !['LTEXT', 'RTEXT', 'CTEXT', 'GROUPBOX', 'PUSHBUTTON', 'DEFPUSHBUTTON', 'SPINCONTROL'].includes(c.type)
    );
    for (const ctrl of inputControls) {
        const label = ctrl.labelText ? `"${ctrl.labelText}"` : ctrl.caption ? `"${ctrl.caption}"` : '';
        const varInfo = ctrl.variable ? ` → ${ctrl.variable} (${ctrl.inferredType})` : '';
        lines.push(`   ${ctrl.id} [${ctrl.type}] ${label}${varInfo}`);
    }
    lines.push('');

    if (info.conditionals.length > 0) {
        lines.push(`⚡ Conditional Logic (${info.conditionals.length} rules):`);
        for (const cond of info.conditionals) {
            const condStr = cond.condition ? ` when (${cond.condition})` : '';
            lines.push(`   ${cond.controlId}: ${cond.action.toUpperCase()}${condStr} [trigger: ${cond.triggerVariable}]`);
        }
        lines.push('');
    }

    if (info.warnings.length > 0) {
        lines.push(`⚠️ Warnings:`);
        for (const w of info.warnings) lines.push(`   - ${w}`);
        lines.push('');
    }

    lines.push(`💾 Resource JSON saved to: generated_resources/${info.dialogClass}.json`);
    lines.push('');
    lines.push('📌 Next: Use the control/DDX/conditional info above to call save_schema.');

    return lines.join('\n');
}

// ============================================================================
// MCP TOOL DEFINITION
// ============================================================================

export const parseDialogTool = {
    name: 'parse_dialog',
    description: `Parse MFC dialog source code to extract controls, DDX bindings, and conditional logic for schema generation.

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
📖 PURPOSE
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

Analyzes C++ MFC dialog source files and RC resource files to extract:
  1. Controls: EDITTEXT, COMBOBOX, CHECKBOX, RADIOBUTTON etc. with labels
  2. DDX Mappings: Member variables bound to controls (type inference)
  3. Conditional Logic: EnableWindow / ShowWindow / CtrlEnableDisable patterns

Results are saved to generated_resources/{ClassName}.json

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
🔧 WORKFLOW
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

1. parse_dialog({ dialogClass: "CNodeCreateDlg" })
   → Finds NodeCreateDlg.cpp / NodeCreateDlg.h in ui_src/wg_cmd/
   → Finds IDD_xxx in header
   → Finds RC file (wg_treemenu/*.rc) with IDD_xxx block
   → Parses controls, DDX, conditional logic
   → Saves generated_resources/CNodeCreateDlg.json
   → Returns structured analysis

2. Analyze the returned controls and conditionals
   → x-optional-when from EnableWindow/ShowWindow conditions
   → if/then from DDX type info and conditions

3. Call save_schema with the generated schema

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
📂 Search Paths
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

Source (.cpp/.h): ui_src/wg_cmd/, ui_src/wg_treemenu/
RC files: ui_src/wg_treemenu/, ui_src/wg_cmd/`,

    inputSchema: {
        type: 'object' as const,
        properties: {
            dialogClass: {
                type: 'string',
                description: 'MFC dialog class name. e.g., "CNodeCreateDlg" or "NodeCreateDlg"',
            },
            extraSearchPaths: {
                type: 'array',
                items: { type: 'string' },
                description: 'Additional paths to search for source files (relative to project root)',
            },
        },
        required: ['dialogClass'],
    },
};
