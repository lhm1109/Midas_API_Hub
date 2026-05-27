import { createClient } from '@supabase/supabase-js';
import dotenv from 'dotenv';
import fs from 'fs/promises';
import path from 'path';

dotenv.config({ path: path.join(process.cwd(), '.env') });

const APPLY = process.argv.includes('--apply');
const SKIP_ZENDESK = process.argv.includes('--skip-zendesk');
const REPORT_DIR = path.join(process.cwd(), 'reports');
const BACKUP_DIR = 'backups/supabase-data-2026-05-26T08-36-50';

const TARGET_PATHS = [
  '/DESIGN/RC/KDS-41-20-2022/WC-TABLE',
  '/DESIGN/RC/KDS-41-20-2022/WD-TABLE',
];

const REPLACEMENTS = new Map([
  ['Wall Mark', 'WallMark'],
  ['Bar Layer', 'BarLayer'],
]);

const TABLE_CONFIGS = [
  {
    table: 'spec_data',
    columns: ['json_schema', 'json_schema_original', 'json_schema_enhanced', 'specifications'],
  },
  {
    table: 'manual_data',
    columns: [
      'json_schema',
      'json_schema_original',
      'json_schema_enhanced',
      'request_examples',
      'response_examples',
      'examples',
      'specifications',
    ],
  },
  {
    table: 'runner_data',
    columns: ['request_body', 'response_body'],
  },
  {
    table: 'test_cases',
    columns: ['request_body', 'response_body', 'expected_response'],
  },
];

function countAllNeedles(value) {
  const text = typeof value === 'string' ? value : JSON.stringify(value ?? '');
  let count = 0;
  for (const oldValue of REPLACEMENTS.keys()) {
    count += text.split(oldValue).length - 1;
  }
  return count;
}

function renameExactComponentValues(value) {
  if (typeof value === 'string') {
    return REPLACEMENTS.get(value) ?? value;
  }

  if (Array.isArray(value)) {
    return value.map((item) => renameExactComponentValues(item));
  }

  if (!value || typeof value !== 'object') {
    return value;
  }

  const renamed = {};
  for (const [key, child] of Object.entries(value)) {
    const nextKey = REPLACEMENTS.get(key) ?? key;
    if (Object.prototype.hasOwnProperty.call(renamed, nextKey)) {
      throw new Error(`Key collision while renaming ${key} to ${nextKey}`);
    }
    renamed[nextKey] = renameExactComponentValues(child);
  }
  return renamed;
}

function replaceQuotedComponentTokens(text) {
  if (typeof text !== 'string') return text;

  let next = text;
  for (const [oldValue, newValue] of REPLACEMENTS.entries()) {
    const tokenPairs = [
      [`"${oldValue}"`, `"${newValue}"`],
      [`&quot;${oldValue}&quot;`, `&quot;${newValue}&quot;`],
      [`&#34;${oldValue}&#34;`, `&#34;${newValue}&#34;`],
      [`'${oldValue}'`, `'${newValue}'`],
      [`&#39;${oldValue}&#39;`, `&#39;${newValue}&#39;`],
      [`<code>${oldValue}</code>`, `<code>${newValue}</code>`],
    ];

    for (const [from, to] of tokenPairs) {
      next = next.split(from).join(to);
    }
  }
  return next;
}

function transformTextColumn(value) {
  if (typeof value !== 'string' || !value) {
    return value;
  }

  try {
    const parsed = JSON.parse(value);
    const renamed = renameExactComponentValues(parsed);
    if (JSON.stringify(parsed) === JSON.stringify(renamed)) {
      return value;
    }
    const prettyPrinted = /^\s*[\[{]\s*\r?\n/.test(value);
    return JSON.stringify(renamed, null, prettyPrinted ? 2 : 0);
  } catch {
    return replaceQuotedComponentTokens(value);
  }
}

function buildUpdateForRow(row, columns) {
  const update = {};
  const changes = [];

  for (const column of columns) {
    if (!Object.prototype.hasOwnProperty.call(row, column)) continue;
    const before = row[column];
    const after = transformTextColumn(before);
    if (after !== before) {
      update[column] = after;
      changes.push({
        column,
        oldTokenCountBefore: countAllNeedles(before),
        oldTokenCountAfter: countAllNeedles(after),
      });
    }
  }

  return { update, changes };
}

function asTrimmedString(value) {
  return typeof value === 'string' ? value.trim() : '';
}

function normalizeZendeskLocale(locale = 'en-us') {
  return String(locale || 'en-us').trim().replace(/_/g, '-').toLowerCase() || 'en-us';
}

function parseZendeskSubdomainFromBaseUrl(baseUrl) {
  const trimmed = asTrimmedString(baseUrl);
  if (!trimmed) return '';
  try {
    const parsed = new URL(trimmed);
    const match = parsed.hostname.toLowerCase().match(/^([a-z0-9-]+)\.zendesk\.com$/i);
    return match ? match[1] : '';
  } catch {
    return '';
  }
}

function getZendeskConfig() {
  const baseUrl = asTrimmedString(process.env.ZENDESK_BASE_URL);
  const subdomain = asTrimmedString(process.env.ZENDESK_SUBDOMAIN) || parseZendeskSubdomainFromBaseUrl(baseUrl);
  const email = asTrimmedString(process.env.ZENDESK_EMAIL);
  const apiToken = asTrimmedString(process.env.ZENDESK_API_TOKEN);
  const password = asTrimmedString(process.env.ZENDESK_PASSWORD);
  const locale = normalizeZendeskLocale(process.env.ZENDESK_DEFAULT_LOCALE || 'en-us');
  const secret = apiToken || password;
  const missing = [];
  if (!subdomain) missing.push('ZENDESK_SUBDOMAIN or ZENDESK_BASE_URL');
  if (!email) missing.push('ZENDESK_EMAIL');
  if (!secret) missing.push('ZENDESK_API_TOKEN or ZENDESK_PASSWORD');
  return { subdomain, email, apiToken, password, locale, missing };
}

async function zendeskRequest(config, requestPath, method = 'GET', payload = undefined) {
  const username = config.apiToken ? `${config.email}/token` : config.email;
  const secret = config.apiToken || config.password;
  const auth = Buffer.from(`${username}:${secret}`).toString('base64');
  const response = await fetch(`https://${config.subdomain}.zendesk.com${requestPath}`, {
    method,
    headers: {
      Authorization: `Basic ${auth}`,
      'Content-Type': 'application/json',
    },
    body: payload === undefined ? undefined : JSON.stringify(payload),
  });

  const body = await response.text();
  let data = body;
  if (body) {
    try {
      data = JSON.parse(body);
    } catch {
      // Keep plain text.
    }
  }

  if (!response.ok) {
    const detail = typeof data === 'string' ? data : JSON.stringify(data);
    throw new Error(`HTTP ${response.status}: ${detail}`);
  }
  return data;
}

async function fetchTargetVersions(supabase) {
  const { data: endpoints, error: endpointError } = await supabase
    .from('endpoints')
    .select('id,name,path')
    .in('path', TARGET_PATHS);

  if (endpointError) throw endpointError;

  const endpointIds = (endpoints || []).map((endpoint) => endpoint.id);
  const { data: versions, error: versionError } = await supabase
    .from('versions')
    .select('id,version,endpoint_id')
    .in('endpoint_id', endpointIds);

  if (versionError) throw versionError;

  const endpointById = new Map((endpoints || []).map((endpoint) => [endpoint.id, endpoint]));
  return (versions || []).map((version) => ({
    ...version,
    endpoint: endpointById.get(version.endpoint_id),
  }));
}

async function updateSupabaseTables(supabase, targetVersions) {
  const targetVersionIds = targetVersions.map((version) => version.id);
  const versionById = new Map(targetVersions.map((version) => [version.id, version]));
  const results = [];

  for (const config of TABLE_CONFIGS) {
    const { data: rows, error } = await supabase
      .from(config.table)
      .select('*')
      .in('version_id', targetVersionIds);

    if (error) {
      results.push({ table: config.table, status: 'failed', error: error.message });
      continue;
    }

    for (const row of rows || []) {
      const { update, changes } = buildUpdateForRow(row, config.columns);
      const version = versionById.get(row.version_id);
      const result = {
        table: config.table,
        versionId: row.version_id,
        endpointPath: version?.endpoint?.path || '',
        endpointName: version?.endpoint?.name || '',
        status: changes.length ? (APPLY ? 'updated' : 'dry-run') : 'unchanged',
        changes,
      };

      if (changes.length && APPLY) {
        const { error: updateError } = await supabase
          .from(config.table)
          .update(update)
          .eq('version_id', row.version_id);

        if (updateError) {
          result.status = 'failed';
          result.error = updateError.message;
        }
      }

      results.push(result);
    }
  }

  return results;
}

async function updateZendeskArticles(supabase, targetVersions) {
  if (SKIP_ZENDESK) {
    return [{ status: 'skipped', reason: '--skip-zendesk' }];
  }

  const config = getZendeskConfig();
  if (config.missing.length) {
    return [{ status: 'failed', error: `Missing ${config.missing.join(', ')}` }];
  }

  const targetVersionIds = targetVersions.map((version) => version.id);
  const versionById = new Map(targetVersions.map((version) => [version.id, version]));
  const { data: manualRows, error } = await supabase
    .from('manual_data')
    .select('version_id,title,article_id,url')
    .in('version_id', targetVersionIds)
    .not('article_id', 'is', null);

  if (error) return [{ status: 'failed', error: error.message }];

  const results = [];
  const seenArticleIds = new Set();
  for (const row of manualRows || []) {
    const articleId = String(row.article_id || '').trim();
    if (!articleId || seenArticleIds.has(articleId)) continue;
    seenArticleIds.add(articleId);

    const locale = normalizeZendeskLocale(config.locale);
    const version = versionById.get(row.version_id);
    const result = {
      articleId,
      locale,
      versionId: row.version_id,
      endpointPath: version?.endpoint?.path || '',
      title: row.title || '',
      status: APPLY ? 'updated' : 'dry-run',
      oldTokenCountBefore: 0,
      oldTokenCountAfter: 0,
    };

    try {
      const translationData = await zendeskRequest(
        config,
        `/api/v2/help_center/articles/${encodeURIComponent(articleId)}/translations/${encodeURIComponent(locale)}.json`
      );
      const translation = translationData?.translation || {};
      const beforeBody = String(translation.body || '');
      const afterBody = replaceQuotedComponentTokens(beforeBody);
      result.oldTokenCountBefore = countAllNeedles(beforeBody);
      result.oldTokenCountAfter = countAllNeedles(afterBody);

      if (afterBody === beforeBody) {
        result.status = 'unchanged';
      } else if (APPLY) {
        await zendeskRequest(
          config,
          `/api/v2/help_center/articles/${encodeURIComponent(articleId)}/translations/${encodeURIComponent(locale)}.json`,
          'PUT',
          { translation: { body: afterBody, title: translation.title || row.title || undefined } }
        );
      }
    } catch (error) {
      result.status = 'failed';
      result.error = error instanceof Error ? error.message : String(error);
    }

    results.push(result);
  }

  return results;
}

function formatReport({ mode, backupDir, targetVersions, supabaseResults, zendeskResults }) {
  const lines = [];
  lines.push('# Design Table Component Key Update Report');
  lines.push('');
  lines.push(`- Mode: ${mode}`);
  lines.push(`- Backup: ${backupDir}`);
  lines.push(`- Created: ${new Date().toISOString()}`);
  lines.push(`- Target paths: ${TARGET_PATHS.join(', ')}`);
  lines.push('');
  lines.push('## Target Versions');
  for (const version of targetVersions) {
    lines.push(`- ${version.endpoint?.path || ''} | ${version.endpoint?.name || ''} | ${version.id}`);
  }
  lines.push('');
  lines.push('## Supabase Results');
  for (const result of supabaseResults) {
    const changed = result.changes?.length
      ? result.changes.map((change) => `${change.column} ${change.oldTokenCountBefore}->${change.oldTokenCountAfter}`).join(', ')
      : 'no changes';
    lines.push(`- ${result.status}: ${result.table} | ${result.endpointPath} | ${result.versionId} | ${changed}${result.error ? ` | ${result.error}` : ''}`);
  }
  lines.push('');
  lines.push('## Zendesk Results');
  for (const result of zendeskResults) {
    lines.push(`- ${result.status}: article ${result.articleId || '-'} | ${result.endpointPath || '-'} | old tokens ${result.oldTokenCountBefore ?? '-'}->${result.oldTokenCountAfter ?? '-'}${result.error ? ` | ${result.error}` : ''}${result.reason ? ` | ${result.reason}` : ''}`);
  }
  lines.push('');
  lines.push('## Replacement Rule');
  for (const [oldValue, newValue] of REPLACEMENTS.entries()) {
    lines.push(`- "${oldValue}" -> "${newValue}"`);
  }
  lines.push('');
  return lines.join('\n');
}

async function main() {
  const supabaseUrl = process.env.SUPABASE_URL;
  const supabaseKey = process.env.SUPABASE_SERVICE_KEY;
  if (!supabaseUrl || !supabaseKey) {
    throw new Error('Missing SUPABASE_URL or SUPABASE_SERVICE_KEY');
  }

  const supabase = createClient(supabaseUrl, supabaseKey, {
    auth: { autoRefreshToken: false, persistSession: false },
  });

  const targetVersions = await fetchTargetVersions(supabase);
  const supabaseResults = await updateSupabaseTables(supabase, targetVersions);
  const zendeskResults = await updateZendeskArticles(supabase, targetVersions);

  await fs.mkdir(REPORT_DIR, { recursive: true });
  const stamp = new Date().toISOString().replace(/[:.]/g, '-').slice(0, 19);
  const reportPath = path.join(REPORT_DIR, `design-table-component-key-update-${stamp}.md`);
  const report = formatReport({
    mode: APPLY ? 'apply' : 'dry-run',
    backupDir: BACKUP_DIR,
    targetVersions,
    supabaseResults,
    zendeskResults,
  });
  await fs.writeFile(reportPath, report, 'utf8');

  console.log(report);
  console.log(`Report: ${reportPath}`);
}

main().catch((error) => {
  console.error(error instanceof Error ? error.message : String(error));
  process.exit(1);
});
