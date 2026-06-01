/**
 * import_zendesk_doc MCP Tool
 *
 * Imports an old Zendesk DOC folder into Supabase as an endpoint version.
 * The actual importer lives in scripts/import-zendesk-doc-to-supabase.mjs so
 * CLI and MCP behavior stay identical.
 */

import * as path from 'path';
import { execFile } from 'child_process';
import { promisify } from 'util';

const execFileAsync = promisify(execFile);
const PROJECT_ROOT = process.env.APIVERIFICATION_ROOT
  || process.cwd().replace(/[\\/]mcp-server([\\/]dist)?$/, '');

export interface ImportZendeskDocInput {
  sourceDir: string;
  productId?: string;
  groupId?: string;
  endpointName?: string;
  endpointPath?: string;
  method?: string;
  versionName?: string;
  author?: string;
  dryRun?: boolean;
  skipBackup?: boolean;
}

export interface ImportZendeskDocResult {
  ok: boolean;
  result?: unknown;
  stdout?: string;
  stderr?: string;
  error?: string;
}

function pushArg(args: string[], flag: string, value?: string) {
  if (value === undefined || value === null || value === '') return;
  args.push(flag, value);
}

export async function importZendeskDoc(input: ImportZendeskDocInput): Promise<ImportZendeskDocResult> {
  if (!input.sourceDir || typeof input.sourceDir !== 'string') {
    throw new Error('sourceDir is required');
  }

  const scriptPath = path.join(PROJECT_ROOT, 'scripts', 'import-zendesk-doc-to-supabase.mjs');
  const args = [scriptPath];
  pushArg(args, '--source', input.sourceDir);
  pushArg(args, '--product', input.productId);
  pushArg(args, '--group-id', input.groupId);
  pushArg(args, '--endpoint-name', input.endpointName);
  pushArg(args, '--path', input.endpointPath);
  pushArg(args, '--method', input.method);
  pushArg(args, '--version', input.versionName);
  pushArg(args, '--author', input.author);
  if (input.dryRun) args.push('--dry-run');
  if (input.skipBackup) args.push('--skip-backup');

  try {
    const { stdout, stderr } = await execFileAsync(process.execPath, args, {
      cwd: PROJECT_ROOT,
      maxBuffer: 1024 * 1024 * 20,
    });
    const jsonStart = stdout.lastIndexOf('\n{');
    const jsonText = jsonStart >= 0 ? stdout.slice(jsonStart + 1) : stdout;
    let result: unknown = undefined;
    try {
      result = JSON.parse(jsonText);
    } catch {
      result = undefined;
    }
    return {
      ok: true,
      result,
      stdout,
      stderr,
    };
  } catch (error: any) {
    return {
      ok: false,
      stdout: error?.stdout,
      stderr: error?.stderr,
      error: error?.message || String(error),
    };
  }
}

export const importZendeskDocTool = {
  name: 'import_zendesk_doc',
  description: `Import an old Zendesk DOC folder into Supabase as an endpoint with spec/manual data.

This tool reads a folder like api_data_set/DOC/002_Open_Project, extracts the Zendesk HTML manual,
builds the current Argument-style schema, creates/updates the endpoint, creates/updates a version,
and stores manual/spec/builder/runner data. It runs a Supabase backup by default before writes.`,
  inputSchema: {
    type: 'object' as const,
    properties: {
      sourceDir: {
        type: 'string',
        description: 'Old manual folder path, e.g. api_data_set/DOC/002_Open_Project',
      },
      productId: {
        type: 'string',
        description: 'Target product id. Default: verification',
      },
      groupId: {
        type: 'string',
        description: 'Target group id. Default: <productId>_project',
      },
      endpointName: {
        type: 'string',
        description: 'Endpoint display name. Default: Zendesk article title',
      },
      endpointPath: {
        type: 'string',
        description: 'Endpoint path. Default: parsed Input URI',
      },
      method: {
        type: 'string',
        description: 'Active method. Default: parsed Active Methods or POST',
      },
      versionName: {
        type: 'string',
        description: 'Version name. Default: LATEST API MANUAL',
      },
      author: {
        type: 'string',
        description: 'Version author. Default: Codex',
      },
      dryRun: {
        type: 'boolean',
        description: 'Parse and preview without writing to Supabase.',
      },
      skipBackup: {
        type: 'boolean',
        description: 'Skip automatic Supabase backup before writes.',
      },
    },
    required: ['sourceDir'],
  },
};
