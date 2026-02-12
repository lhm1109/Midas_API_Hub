/**
 * Dialog File Finder
 * Searches ui_src/ for dialog .cpp/.h files and RC resource files
 */

import * as fs from 'fs';
import * as path from 'path';

// PROJECT_ROOT: same logic as path-guard.ts
const PROJECT_ROOT = process.env.APIVERIFICATION_ROOT
    || process.cwd().replace(/[\\/]mcp-server([\\/]dist)?$/, '');

export const UI_SRC_DIR = path.join(PROJECT_ROOT, 'ui_src');

// Known directories to search (in priority order)
const CPP_SEARCH_DIRS = [
    path.join(UI_SRC_DIR, 'wg_cmd'),
    path.join(UI_SRC_DIR, 'wg_treemenu'),
    path.join(UI_SRC_DIR, 'db_control'),
    UI_SRC_DIR,
];

const RC_SEARCH_DIRS = [
    path.join(UI_SRC_DIR, 'wg_treemenu'),
    path.join(UI_SRC_DIR, 'wg_cmd'),
    UI_SRC_DIR,
];

export interface DialogFileSet {
    cppFile?: string;    // Absolute path to .cpp
    hFile?: string;      // Absolute path to .h
    rcFile?: string;     // Absolute path to .rc
    dialogId?: string;   // IDD_xxx found in header
}

/**
 * Find all source files for a dialog class
 *
 * @param dialogClass - e.g. "CNodeCreateDlg" or "NodeCreateDlg"
 * @param extraSearchPaths - Additional directories to search
 */
export async function findDialogFiles(
    dialogClass: string,
    extraSearchPaths?: string[]
): Promise<DialogFileSet> {
    // Normalize: strip leading C if it's a typical MFC class name
    const baseName = stripClassPrefix(dialogClass);

    // Candidates to search for
    const cppCandidates = buildCandidates(baseName, '.cpp');
    const hCandidates = buildCandidates(baseName, '.h');

    const searchDirs = [...CPP_SEARCH_DIRS];
    if (extraSearchPaths) {
        searchDirs.push(...extraSearchPaths.map(p => path.isAbsolute(p) ? p : path.join(PROJECT_ROOT, p)));
    }

    // Find .cpp and .h files
    const cppFile = findFileInDirs(cppCandidates, searchDirs);
    const hFile = findFileInDirs(hCandidates, searchDirs);

    const result: DialogFileSet = { cppFile, hFile };

    // Extract dialog ID from header
    if (hFile) {
        const headerContent = fs.readFileSync(hFile, 'latin1');
        result.dialogId = extractDialogIdFromHeader(headerContent) ?? undefined;
    }

    // Find RC file containing the dialog
    if (result.dialogId) {
        const rcFile = await findRCFileForDialog(result.dialogId, extraSearchPaths);
        if (rcFile) result.rcFile = rcFile;
    }

    return result;
}

/**
 * Find an RC file that contains the given dialog ID
 */
export async function findRCFileForDialog(
    dialogId: string,
    extraSearchPaths?: string[]
): Promise<string | null> {
    const rcDirs = [...RC_SEARCH_DIRS];
    if (extraSearchPaths) {
        rcDirs.push(...extraSearchPaths.map(p => path.isAbsolute(p) ? p : path.join(PROJECT_ROOT, p)));
    }

    // Collect all .rc files
    const rcFiles: string[] = [];
    for (const dir of rcDirs) {
        if (!fs.existsSync(dir)) continue;
        collectFiles(dir, '.rc', rcFiles, 2); // depth limit 2 for performance
    }

    // Search each RC file for the dialog ID
    for (const rcFile of rcFiles) {
        try {
            const content = fs.readFileSync(rcFile, 'latin1');
            // Quick check: does this file contain the dialog ID?
            if (content.includes(dialogId)) {
                return rcFile;
            }
        } catch {
            // skip unreadable files
        }
    }

    return null;
}

/**
 * Read file contents safely (handles Windows-1252/CP1252 encoding common in MFC)
 */
export function readSourceFile(filePath: string): string {
    // Try UTF-8 first, fall back to latin1 (covers CP1252)
    try {
        return fs.readFileSync(filePath, 'utf8');
    } catch {
        return fs.readFileSync(filePath, 'latin1');
    }
}

/**
 * Search for a file matching any of the candidates in any of the directories
 */
function findFileInDirs(candidates: string[], dirs: string[]): string | undefined {
    for (const dir of dirs) {
        if (!fs.existsSync(dir)) continue;
        for (const candidate of candidates) {
            const fullPath = path.join(dir, candidate);
            if (fs.existsSync(fullPath)) {
                return fullPath;
            }
        }
    }
    return undefined;
}

/**
 * Build list of candidate filenames for a base name
 * e.g., "NodeCreateDlg" → ["NodeCreateDlg.cpp", "CNodeCreateDlg.cpp"]
 */
function buildCandidates(baseName: string, ext: string): string[] {
    const candidates: string[] = [];

    // Exact match
    candidates.push(`${baseName}${ext}`);

    // With C prefix
    if (!baseName.startsWith('C')) {
        candidates.push(`C${baseName}${ext}`);
    }

    // Without C prefix (if it starts with C)
    if (baseName.startsWith('C') && baseName.length > 1) {
        candidates.push(`${baseName.slice(1)}${ext}`);
    }

    return candidates;
}

/**
 * Strip MFC class prefix (C) to get base name
 * CNodeCreateDlg → NodeCreateDlg
 * NodeCreateDlg → NodeCreateDlg (unchanged)
 */
function stripClassPrefix(className: string): string {
    // If starts with C followed by uppercase, it's likely MFC prefix
    if (/^C[A-Z]/.test(className)) {
        return className.slice(1);
    }
    return className;
}

/**
 * Extract IDD_xxx from header content
 */
function extractDialogIdFromHeader(headerContent: string): string | null {
    // enum { IDD = IDD_xxx }
    const enumMatch = headerContent.match(/enum\s*\{[^}]*\bIDD\s*=\s*(IDD_\w+)[^}]*\}/);
    if (enumMatch) return enumMatch[1];

    // IDD = IDD_xxx
    const assignMatch = headerContent.match(/\bIDD\s*=\s*(IDD_\w+)/);
    if (assignMatch) return assignMatch[1];

    return null;
}

/**
 * Recursively collect files with a given extension up to maxDepth
 */
function collectFiles(dir: string, ext: string, result: string[], maxDepth: number, currentDepth = 0): void {
    if (currentDepth > maxDepth) return;
    if (!fs.existsSync(dir)) return;

    let entries: fs.Dirent[];
    try {
        entries = fs.readdirSync(dir, { withFileTypes: true });
    } catch {
        return;
    }

    for (const entry of entries) {
        const fullPath = path.join(dir, entry.name);
        if (entry.isDirectory()) {
            collectFiles(fullPath, ext, result, maxDepth, currentDepth + 1);
        } else if (entry.isFile() && entry.name.toLowerCase().endsWith(ext)) {
            result.push(fullPath);
        }
    }
}

/**
 * Get relative path from project root (for display)
 */
export function toRelativePath(absolutePath: string): string {
    return path.relative(PROJECT_ROOT, absolutePath).replace(/\\/g, '/');
}
