/**
 * MFC C++ Dialog Source Code Parser
 * Extracts DDX mappings, conditional logic, and dialog IDs
 */

export interface DDXMapping {
    function: string;      // DDX_Check, DDX_Text, DDX_CBIndex, etc.
    controlId: string;     // IDC_xxx
    variable: string;      // m_xxx member variable
    inferredType: string;  // 'boolean' | 'integer' | 'number' | 'string' | 'object'
}

export interface ConditionalAction {
    controlId: string;
    action: 'enable' | 'disable' | 'show' | 'hide';
    condition?: string;    // Raw condition expression
}

export interface ConditionalBlock {
    triggerVariable: string;   // The variable that drives the condition
    triggerCondition: string;  // Raw condition string
    actions: ConditionalAction[];
}

export interface OnChangeHandler {
    handlerName: string;       // e.g., OnChangeCombType
    triggerControlId?: string; // IDC_xxx if detectable
    triggerVariable?: string;  // m_xxx if detectable
    body: string;              // Raw function body
}

export interface ParsedCppInfo {
    dialogId?: string;           // IDD_xxx
    ddxMappings: DDXMapping[];
    conditionalBlocks: ConditionalBlock[];
    onChangeHandlers: OnChangeHandler[];
    rawDoDataExchange?: string;  // Raw DDX function body for LLM
    rawUpdateLogic?: string;     // Raw update/enable logic for LLM
    controlGroupArrays?: Record<string, string[]>;  // m_aCtrlXxx → [IDC_xxx, ...]
}

/**
 * Extract dialog IDD constant from header file
 * Looks for: enum { IDD = IDD_xxx };
 * or: static const int IDD = IDD_xxx;
 */
export function extractDialogId(headerContent: string): string | null {
    // Pattern 1: enum { IDD = IDD_xxx }
    const enumMatch = headerContent.match(/enum\s*\{[^}]*\bIDD\s*=\s*(IDD_\w+)[^}]*\}/);
    if (enumMatch) return enumMatch[1];

    // Pattern 2: IDD = IDD_xxx anywhere
    const assignMatch = headerContent.match(/\bIDD\s*=\s*(IDD_\w+)/);
    if (assignMatch) return assignMatch[1];

    // Pattern 3: #define for dialog ID
    const defineMatch = headerContent.match(/#define\s+IDD_\w+\s+\d+/);
    if (defineMatch) {
        const idMatch = defineMatch[0].match(/#define\s+(IDD_\w+)/);
        if (idMatch) return idMatch[1];
    }

    return null;
}

/**
 * Extract DDX mappings from DoDataExchange function
 */
export function extractDDXMappings(cppContent: string): DDXMapping[] {
    const mappings: DDXMapping[] = [];

    // Extract DoDataExchange body
    const body = extractFunctionBody(cppContent, 'DoDataExchange');
    if (!body) return mappings;

    // Match DDX_xxx(pDX, IDC_xxx, m_xxx) patterns
    // Also handles DDX_xxx(pDX, IDC_xxx, m_xxx, extra)
    const ddxPattern = /\bDDX_(\w+)\s*\(\s*pDX\s*,\s*(IDC_\w+|IDOK|IDCANCEL|\w+)\s*,\s*([\w.->]+)/g;
    let match: RegExpExecArray | null;

    while ((match = ddxPattern.exec(body)) !== null) {
        const funcSuffix = match[1];
        const controlId = match[2];
        const variable = match[3].replace(/^m_/, 'm_'); // normalize

        // Infer type from DDX function name and variable name
        const inferredType = inferTypeFromDDX(funcSuffix, variable);

        mappings.push({
            function: `DDX_${funcSuffix}`,
            controlId,
            variable,
            inferredType,
        });
    }

    return mappings;
}

/**
 * Extract control group arrays: m_aCtrlXxx.Add(IDC_xxx)
 * Returns a map of group variable → list of control IDs
 */
export function extractControlGroupArrays(content: string): Map<string, string[]> {
    const groups = new Map<string, string[]>();
    // Pattern: m_aCtrlXxx.Add(IDC_xxx) or m_aCtrlXxx.Add( IDC_xxx )
    const addPattern = /\b(m_a\w+)\s*\.\s*Add\s*\(\s*(IDC_\w+)\s*\)/g;
    let m: RegExpExecArray | null;
    while ((m = addPattern.exec(content)) !== null) {
        const groupVar = m[1];
        const controlId = m[2];
        if (!groups.has(groupVar)) groups.set(groupVar, []);
        groups.get(groupVar)!.push(controlId);
    }
    return groups;
}

/**
 * Find the nearest enclosing if-condition before a given position in the body.
 * Handles one level of nested parens: if(x) and if((x && y)).
 */
function findNearestIfCondition(body: string, position: number): string | null {
    const textBefore = body.slice(0, position);
    // Match: if( ... ) where ... can contain one level of nested parens
    const ifPattern = /\bif\s*\(\s*((?:[^()]*|\([^()]*\))*)\s*\)/g;
    let lastCondition: string | null = null;
    let m: RegExpExecArray | null;
    while ((m = ifPattern.exec(textBefore)) !== null) {
        lastCondition = m[1].trim();
    }
    return lastCondition;
}

/**
 * Extract conditional enable/disable/show/hide logic from C++ source
 * Looks for:
 *   GetDlgItem(IDC_xxx)->EnableWindow(expr)
 *   GetDlgItem(IDC_xxx)->ShowWindow(SW_SHOW/SW_HIDE)
 *   CtrlEnableDisable(IDC_xxx, expr)  (custom helper)
 *   CDlgUtil::CtrlEnableDisable(this, m_aCtrlXxx, expr)  (group expand)
 */
export function extractConditionalLogic(cppContent: string): ConditionalBlock[] {
    const blocks: ConditionalBlock[] = [];

    // Build control group map first (m_aCtrlXxx → [IDC_xxx, ...])
    const controlGroupMap = extractControlGroupArrays(cppContent);

    // Find likely "update control" functions
    const updateFuncNames = findUpdateFunctionNames(cppContent);

    // Collect all function bodies that contain control enable/show logic
    const targetFunctions = [
        'OnInitDialog',
        'OnOK',
        'UpdateControls',
        'UpdateData',
        'EnableControls',
        'SetControlState',
        'CtrlManager',       // MFC custom: manages control enable/disable state
        'InitDialogData',    // MFC custom: sets initial data
        ...updateFuncNames,
    ];

    const allActions: Array<{ funcName: string; action: ConditionalAction; context: string }> = [];

    for (const funcName of targetFunctions) {
        const body = extractFunctionBody(cppContent, funcName);
        if (!body) continue;

        const actions = extractControlActions(body, controlGroupMap);
        for (const action of actions) {
            allActions.push({ funcName, action, context: body });
        }
    }

    // Also look for OnChangeXxx, OnSelchangeXxx patterns
    const onChangePattern = /\bOn(?:Change|Selchange|SelChange|BnClicked|BnClick|Clicked)\w*\s*\([^)]*\)\s*\{/g;
    let fnMatch: RegExpExecArray | null;

    while ((fnMatch = onChangePattern.exec(cppContent)) !== null) {
        const startIdx = fnMatch.index + fnMatch[0].length - 1; // start of {
        const body = extractBodyFromBrace(cppContent, startIdx);
        if (!body) continue;

        const actions = extractControlActions(body, controlGroupMap);
        if (actions.length > 0) {
            const funcName = fnMatch[0].match(/\b(On\w+)\s*\(/)![1];
            for (const action of actions) {
                allActions.push({ funcName, action, context: body });
            }
        }
    }

    // Group actions by trigger variable (from context)
    const triggerMap = new Map<string, ConditionalAction[]>();

    for (const { action, context } of allActions) {
        // Try to find what variable drives this condition
        const trigger = findTriggerVariable(action.condition ?? '', context);
        const key = trigger ?? '__unknown__';

        if (!triggerMap.has(key)) {
            triggerMap.set(key, []);
        }
        triggerMap.get(key)!.push(action);
    }

    for (const [trigger, actions] of triggerMap.entries()) {
        // Deduplicate actions by controlId
        const seen = new Set<string>();
        const unique = actions.filter(a => {
            const k = `${a.controlId}:${a.action}`;
            if (seen.has(k)) return false;
            seen.add(k);
            return true;
        });

        blocks.push({
            triggerVariable: trigger,
            triggerCondition: trigger !== '__unknown__' ? trigger : 'unknown',
            actions: unique,
        });
    }

    return blocks;
}

/**
 * Extract OnChange/OnSelchange handler names and their bodies
 */
export function extractOnChangeHandlers(cppContent: string): OnChangeHandler[] {
    const handlers: OnChangeHandler[] = [];

    // Match OnChange/OnSelchange handlers
    const pattern = /\b(On(?:Change|Selchange|SelChange|BnClicked|BnClick|Clicked)(\w*?))\s*\(\s*\)\s*\{/g;
    let match: RegExpExecArray | null;

    while ((match = pattern.exec(cppContent)) !== null) {
        const handlerName = match[1];
        const suffix = match[2];
        const startIdx = match.index + match[0].length - 1;
        const body = extractBodyFromBrace(cppContent, startIdx);

        if (body) {
            const handler: OnChangeHandler = {
                handlerName,
                body: body.trim(),
            };

            // Try to associate with a control ID from handler name (e.g., OnSelchangeComboType → IDC_COMBO_TYPE)
            const controlGuess = guessControlIdFromHandlerName(suffix);
            if (controlGuess) {
                handler.triggerControlId = controlGuess;
            }

            handlers.push(handler);
        }
    }

    return handlers;
}

/**
 * Full parse of a .cpp file
 */
export function parseCppFile(cppContent: string, headerContent?: string): ParsedCppInfo {
    const result: ParsedCppInfo = {
        ddxMappings: [],
        conditionalBlocks: [],
        onChangeHandlers: [],
    };

    // Extract dialog ID from header
    if (headerContent) {
        result.dialogId = extractDialogId(headerContent) ?? undefined;
    }

    // Extract DDX mappings
    result.ddxMappings = extractDDXMappings(cppContent);

    // Extract control group arrays (m_aCtrlXxx → [IDC_xxx, ...])
    const groupMap = extractControlGroupArrays(cppContent);
    if (groupMap.size > 0) {
        result.controlGroupArrays = Object.fromEntries(groupMap);
    }

    // Extract conditional logic
    result.conditionalBlocks = extractConditionalLogic(cppContent);

    // Extract OnChange handlers
    result.onChangeHandlers = extractOnChangeHandlers(cppContent);

    // Save raw DoDataExchange for LLM reference
    const ddxBody = extractFunctionBody(cppContent, 'DoDataExchange');
    if (ddxBody) result.rawDoDataExchange = ddxBody.trim();

    // Save raw update logic (include CtrlManager and other control-state functions)
    const updateFuncs = findUpdateFunctionNames(cppContent);
    const allUpdateBodies: string[] = [];
    for (const fn of ['OnInitDialog', 'UpdateControls', 'EnableControls', 'CtrlManager', ...updateFuncs]) {
        const body = extractFunctionBody(cppContent, fn);
        if (body) allUpdateBodies.push(`// ${fn}:\n${body.trim()}`);
    }
    if (allUpdateBodies.length > 0) {
        result.rawUpdateLogic = allUpdateBodies.join('\n\n');
    }

    return result;
}

// ============================================================================
// HELPERS
// ============================================================================

/**
 * Extract function body using brace matching
 * Finds: functionName(...) { ... }
 */
function extractFunctionBody(content: string, functionName: string): string | null {
    // Use [^{;]* instead of [^{]* to stop at ';' — prevents matching function CALLS
    // (calls end with ';', definitions end with '{')
    // e.g. "CtrlManager();" in OnInitDialog body won't match, but "CtrlManager() {" will
    const pattern = new RegExp(`\\b${escapeRegex(functionName)}\\s*\\([^)]*\\)[^{;]*\\{`);
    const match = pattern.exec(content);
    if (!match) return null;

    const startBrace = content.indexOf('{', match.index + match[0].length - 1);
    if (startBrace === -1) return null;

    return extractBodyFromBrace(content, startBrace);
}

/**
 * Extract body from a position of opening brace
 */
function extractBodyFromBrace(content: string, bracePos: number): string | null {
    let depth = 0;
    let i = bracePos;

    while (i < content.length) {
        const ch = content[i];
        if (ch === '{') depth++;
        else if (ch === '}') {
            depth--;
            if (depth === 0) {
                return content.slice(bracePos + 1, i);
            }
        }
        i++;
    }

    return null;
}

/**
 * Extract control enable/show actions from a code block
 */
function extractControlActions(body: string, controlGroupMap?: Map<string, string[]>): ConditionalAction[] {
    const actions: ConditionalAction[] = [];

    // Pattern: GetDlgItem(IDC_xxx)->EnableWindow(expr)
    const enablePattern = /GetDlgItem\s*\(\s*(IDC_\w+|\w+)\s*\)\s*->\s*EnableWindow\s*\(\s*([^)]+)\)/g;
    let m: RegExpExecArray | null;

    while ((m = enablePattern.exec(body)) !== null) {
        const controlId = m[1];
        const expr = m[2].trim();

        // Determine if enabling or disabling based on expression
        // EnableWindow(FALSE) → disable, EnableWindow(!xxx) → conditional
        const isDisable = expr === 'FALSE' || expr === '0' || expr === 'false';
        const isEnable = expr === 'TRUE' || expr === '1' || expr === 'true';

        actions.push({
            controlId,
            action: isDisable ? 'disable' : 'enable',
            condition: isDisable || isEnable ? undefined : expr,
        });
    }

    // Pattern: GetDlgItem(IDC_xxx)->ShowWindow(SW_SHOW/SW_HIDE)
    const showPattern = /GetDlgItem\s*\(\s*(IDC_\w+|\w+)\s*\)\s*->\s*ShowWindow\s*\(\s*([^)]+)\)/g;

    while ((m = showPattern.exec(body)) !== null) {
        const controlId = m[1];
        const expr = m[2].trim();

        const isHide = expr.includes('SW_HIDE') || expr === '0' || expr === 'false';
        const isShow = expr.includes('SW_SHOW') || expr === '1';

        actions.push({
            controlId,
            action: isHide ? 'hide' : 'show',
            condition: !isHide && !isShow ? expr : undefined,
        });
    }

    // Pattern: CtrlEnableDisable(IDC_xxx, expr) - custom MFC helper (simple 2-arg form)
    // Only match IDC_xxx as first arg to avoid false match on CDlgUtil::CtrlEnableDisable(this, ...)
    const ctrlPattern = /CtrlEnableDisable\s*\(\s*(IDC_\w+)\s*,\s*([^)]+)\)/g;

    while ((m = ctrlPattern.exec(body)) !== null) {
        const controlId = m[1];
        const expr = m[2].trim();

        const isDisable = expr === 'FALSE' || expr === '0' || expr === 'false';

        actions.push({
            controlId,
            action: isDisable ? 'disable' : 'enable',
            condition: isDisable ? undefined : expr,
        });
    }

    // Pattern: m_ctrlXxx.EnableWindow(expr)
    const memberPattern = /\bm_(\w+)\s*\.\s*EnableWindow\s*\(\s*([^)]+)\)/g;

    while ((m = memberPattern.exec(body)) !== null) {
        const varSuffix = m[1];
        const expr = m[2].trim();
        const isDisable = expr === 'FALSE' || expr === '0' || expr === 'false';

        // Try to reconstruct IDC name from variable
        const controlId = `m_${varSuffix}`;

        actions.push({
            controlId,
            action: isDisable ? 'disable' : 'enable',
            condition: isDisable ? undefined : expr,
        });
    }

    // Pattern: CDlgUtil::CtrlEnableDisable(this, m_aCtrlXxx, TRUE/FALSE)
    // Expands group variable to individual control IDs using controlGroupMap
    const cdlgUtilPattern = /(?:\w+::\s*)?CtrlEnableDisable\s*\(\s*this\s*,\s*(m_\w+)\s*,\s*([^)]+)\)/g;

    while ((m = cdlgUtilPattern.exec(body)) !== null) {
        const groupVar = m[1];
        const expr = m[2].trim();
        const callPosition = m.index;

        const isDisable = expr === 'FALSE' || expr === '0' || expr === 'false';
        const isEnable  = expr === 'TRUE'  || expr === '1' || expr === 'true';

        // Find the enclosing if-condition for context (to identify trigger variable)
        const enclosingCond = findNearestIfCondition(body, callPosition);

        // Expand group → individual control IDs
        const controlIds = controlGroupMap?.get(groupVar) ?? [groupVar];

        for (const controlId of controlIds) {
            actions.push({
                controlId,
                action: isDisable ? 'disable' : 'enable',
                condition: enclosingCond ?? (isDisable || isEnable ? undefined : expr),
            });
        }
    }

    return actions;
}

/**
 * Find function names that look like they update/enable controls
 */
function findUpdateFunctionNames(content: string): string[] {
    const names: string[] = [];
    const pattern = /\b((?:Update|Enable|Refresh|Set|Init|Control|Ctrl|Manager)\w*)\s*\(\s*\)/g;
    let match: RegExpExecArray | null;

    const seen = new Set<string>();
    while ((match = pattern.exec(content)) !== null) {
        const name = match[1];
        if (!seen.has(name) && name.length > 6) {
            // Check if this is actually a function definition (has { somewhere nearby)
            const bodyCheck = content.slice(match.index, match.index + 200);
            if (bodyCheck.includes('{')) {
                seen.add(name);
                names.push(name);
            }
        }
    }

    return names;
}

/**
 * Try to find what variable triggers a condition expression
 */
function findTriggerVariable(condition: string, context: string): string | null {
    if (!condition) return null;

    // Direct variable reference: m_xxx, m_bXxx
    const varMatch = condition.match(/\bm_(\w+)/);
    if (varMatch) return `m_${varMatch[1]}`;

    return null;
}

/**
 * Infer JSON Schema type from DDX function name and variable name
 */
function inferTypeFromDDX(ddxFuncSuffix: string, variable: string): string {
    // DDX function hints
    switch (ddxFuncSuffix.toLowerCase()) {
        case 'check': return 'boolean';
        case 'radio': return 'integer';
        case 'cbindex': return 'integer';
        case 'cbstring': return 'string';
        case 'text': break; // fall through to variable check
        case 'int': return 'integer';
        case 'uint': return 'integer';
        case 'long': return 'integer';
        case 'ulong': return 'integer';
        case 'float': return 'number';
        case 'double': return 'number';
        case 'scroll': return 'integer';
        case 'slider': return 'integer';
        case 'listindex': return 'integer';
        case 'control': return 'object'; // CWnd binding
    }

    // Variable name prefix hints
    if (variable.startsWith('m_b') || variable.startsWith('m_B')) return 'boolean';
    if (variable.startsWith('m_i') || variable.startsWith('m_n') || variable.startsWith('m_N')) return 'integer';
    if (variable.startsWith('m_d') || variable.startsWith('m_f') || variable.startsWith('m_D') || variable.startsWith('m_F')) return 'number';
    if (variable.startsWith('m_s') || variable.startsWith('m_str') || variable.startsWith('m_S')) return 'string';
    if (variable.startsWith('m_ctrl') || variable.startsWith('m_Ctrl') || variable.startsWith('m_wnd')) return 'object';

    return 'string'; // default
}

/**
 * Try to guess IDC_xxx control ID from handler name suffix
 * e.g., "ComboType" → "IDC_COMBO_TYPE"
 */
function guessControlIdFromHandlerName(suffix: string): string | null {
    if (!suffix) return null;

    // Convert PascalCase to IDC_UPPER_SNAKE
    const snake = suffix
        .replace(/([a-z])([A-Z])/g, '$1_$2')
        .toUpperCase();

    return `IDC_${snake}`;
}

function escapeRegex(str: string): string {
    return str.replace(/[.*+?^${}()|[\]\\]/g, '\\$&');
}
