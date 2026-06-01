import { createClient } from '@supabase/supabase-js';
import dotenv from 'dotenv';
import fs from 'fs/promises';
import path from 'path';
import { fileURLToPath } from 'url';
import { spawnSync } from 'child_process';

const __dirname = path.dirname(fileURLToPath(import.meta.url));
const projectRoot = path.resolve(__dirname, '..');
dotenv.config({ path: path.join(projectRoot, '.env') });

function parseArgs(argv) {
  const args = {};
  for (let i = 0; i < argv.length; i += 1) {
    const token = argv[i];
    if (!token.startsWith('--')) continue;
    const key = token.slice(2);
    const next = argv[i + 1];
    if (!next || next.startsWith('--')) {
      args[key] = true;
      continue;
    }
    args[key] = next;
    i += 1;
  }
  return args;
}

function usage() {
  return [
    'Usage:',
    '  node scripts/import-zendesk-doc-to-supabase.mjs --source <doc-folder> --product <product-id> --group-id <group-id>',
    '',
    'Optional:',
    '  --endpoint-name "Open Project"',
    '  --path /doc/OPEN',
    '  --method POST',
    '  --version "LATEST API MANUAL"',
    '  --author Codex',
    '  --dry-run',
    '  --skip-backup',
  ].join('\n');
}

function decodeHtml(value) {
  return String(value || '')
    .replace(/&nbsp;/g, ' ')
    .replace(/&#160;/g, ' ')
    .replace(/&quot;/g, '"')
    .replace(/&#39;/g, "'")
    .replace(/&lt;/g, '<')
    .replace(/&gt;/g, '>')
    .replace(/&amp;/g, '&')
    .replace(/혻/g, ' ');
}

function htmlToText(value) {
  return decodeHtml(value)
    .replace(/<br\s*\/?>/gi, '\n')
    .replace(/<[^>]+>/g, '')
    .replace(/\u00a0/g, ' ')
    .replace(/[ \t]+\n/g, '\n')
    .replace(/\n{3,}/g, '\n\n')
    .trim();
}

function stripTags(value) {
  return htmlToText(value).replace(/\s+/g, ' ').trim();
}

function extractFirst(html, regex, fallback = '') {
  const match = html.match(regex);
  return match ? stripTags(match[1]) : fallback;
}

function normalizePath(rawPath) {
  const trimmed = String(rawPath || '').trim();
  if (!trimmed) return '';
  return trimmed.startsWith('/') ? trimmed : `/${trimmed}`;
}

function slugify(value, fallback = '') {
  const slug = String(value || '')
    .trim()
    .toLowerCase()
    .replace(/[\\/]+/g, '-')
    .replace(/\s+/g, '-')
    .replace(/[^a-z0-9-]/g, '-')
    .replace(/-+/g, '-')
    .replace(/^-|-$/g, '');
  return slug || fallback;
}

function endpointIdFromPath(endpointPath, name, groupId, method) {
  const pathSegments = normalizePath(endpointPath)
    .replace(/^\//, '')
    .split('/')
    .map((segment) => slugify(segment))
    .filter(Boolean);
  const fallbackGroup = typeof groupId === 'string'
    ? groupId.split('_').pop() || groupId
    : 'endpoint';
  const segments = pathSegments.length > 0 ? pathSegments : [slugify(fallbackGroup, 'endpoint')];
  const lastSegment = segments[segments.length - 1];
  const nameSlug = slugify(name);
  const methodSlug = slugify(String(method || '').replace(/,/g, '-'));
  if (nameSlug && nameSlug !== lastSegment) {
    segments.push(nameSlug);
  } else if (!nameSlug && methodSlug && methodSlug !== lastSegment) {
    segments.push(methodSlug);
  }
  return segments.join('/');
}

async function findZendeskHtml(sourceDir) {
  const zendeskDir = path.join(sourceDir, 'zendesk');
  const entries = await fs.readdir(zendeskDir, { withFileTypes: true });
  const html = entries.find((entry) => entry.isFile() && entry.name.toLowerCase().endsWith('.html'));
  if (!html) throw new Error(`No zendesk HTML found in ${zendeskDir}`);
  return path.join(zendeskDir, html.name);
}

function extractCopyTarget(html, id) {
  const pattern = new RegExp(`<div\\s+id=["']${id}["'][^>]*>([\\s\\S]*?)<\\/div>`, 'i');
  const match = html.match(pattern);
  return match ? htmlToText(match[1]) : '';
}

function parseJsonFromCopyTarget(text) {
  const normalized = text
    .replace(/\r/g, '')
    .replace(/^[^\{]*/, '')
    .trim();
  return JSON.parse(normalized);
}

function parseMetadata(html) {
  const title = extractFirst(html, /<h1[^>]*>([\s\S]*?)<\/h1>/i, 'Open Project');
  const category = extractFirst(html, /<span[^>]*class=["']label["'][^>]*>\s*Category:\s*<\/span>([\s\S]*?)<\/div>/i, 'DOC');
  const articleId = extractFirst(html, /<span[^>]*class=["']label["'][^>]*>\s*Article ID:\s*<\/span>([\s\S]*?)<\/div>/i);
  const authorId = extractFirst(html, /<span[^>]*class=["']label["'][^>]*>\s*Author ID:\s*<\/span>([\s\S]*?)<\/div>/i);
  const sectionId = extractFirst(html, /<span[^>]*class=["']label["'][^>]*>\s*Section ID:\s*<\/span>([\s\S]*?)<\/div>/i);
  const url = extractFirst(html, /<span[^>]*class=["']label["'][^>]*>\s*URL:\s*<\/span>([\s\S]*?)<\/div>/i);
  const inputUri = extractFirst(html, /<h3[^>]*>\s*<strong>\s*Input URI\s*<\/strong>\s*<\/h3>[\s\S]*?<strong>\s*\{base url\}\s*\+\s*([^<]+?)\s*<\/strong>/i);
  const activeMethods = extractFirst(html, /<h3[^>]*>\s*<strong>\s*Active Methods\s*<\/strong>\s*<\/h3>[\s\S]*?<strong>\s*([^<]+?)\s*<\/strong>/i, 'POST');
  return {
    title,
    category,
    articleId,
    authorId,
    sectionId,
    url,
    inputUri: normalizePath(inputUri),
    activeMethods,
  };
}

function buildCurrentSchema(oldSchema, example) {
  const argumentSchema = oldSchema?.properties?.Argument || {};
  const type = argumentSchema.type || 'string';
  const placeholder = typeof example?.Argument === 'string' ? example.Argument : undefined;
  return {
    type: 'object',
    required: ['Argument'],
    additionalProperties: false,
    properties: {
      Argument: {
        type,
        description: 'Opened File Path',
        'x-ui': {
          label: 'Opened File Path',
          ...(placeholder ? { placeholder } : {}),
          order: 1,
        },
      },
    },
  };
}

function buildSpecifications() {
  return [
    '| No. | Description | Key | Value Type | Default | Required |',
    '| --- | --- | --- | --- | --- | --- |',
    '| 1 | Opened File Path | Argument | string | - | Required |',
  ].join('\n');
}

async function getNextEndpointOrderIndex(supabase, groupId) {
  const { data, error } = await supabase
    .from('endpoints')
    .select('order_index')
    .eq('group_id', groupId)
    .order('order_index', { ascending: false })
    .limit(1);
  if (error) throw error;
  const lastOrder = Array.isArray(data) && data.length > 0 ? Number(data[0].order_index || 0) : 0;
  return lastOrder + 1;
}

async function getOrCreateVersion(supabase, endpointId, versionName, author, changeLog) {
  const { data: existing, error: existingError } = await supabase
    .from('versions')
    .select('*')
    .eq('endpoint_id', endpointId)
    .eq('version', versionName)
    .maybeSingle();
  if (existingError) throw existingError;
  if (existing) return { version: existing, created: false };

  const now = new Date().toISOString();
  const version = {
    id: `v_${Date.now()}_${Math.random().toString(36).slice(2, 11)}`,
    version: versionName,
    endpoint_id: endpointId,
    created_at: now,
    updated_at: now,
    author,
    change_log: changeLog,
  };
  const { data, error } = await supabase
    .from('versions')
    .insert(version)
    .select()
    .single();
  if (error) throw error;
  return { version: data, created: true };
}

async function upsertVersionData(supabase, versionId, rows) {
  const { manual, spec, builder, runner } = rows;
  const operations = [
    supabase.from('manual_data').upsert({ version_id: versionId, ...manual }, { onConflict: 'version_id' }),
    supabase.from('spec_data').upsert({ version_id: versionId, ...spec }, { onConflict: 'version_id' }),
    supabase.from('builder_data').upsert({ version_id: versionId, ...builder }, { onConflict: 'version_id' }),
    supabase.from('runner_data').upsert({ version_id: versionId, ...runner }, { onConflict: 'version_id' }),
  ];
  const results = await Promise.all(operations);
  for (const result of results) {
    if (result.error) throw result.error;
  }
}

async function main() {
  const args = parseArgs(process.argv.slice(2));
  if (!args.source) {
    console.error(usage());
    process.exit(1);
  }

  const supabaseUrl = process.env.SUPABASE_URL;
  const supabaseKey = process.env.SUPABASE_SERVICE_KEY;
  if (!supabaseUrl || !supabaseKey) {
    throw new Error('SUPABASE_URL and SUPABASE_SERVICE_KEY are required in .env');
  }

  const sourceDir = path.resolve(projectRoot, String(args.source));
  const htmlPath = await findZendeskHtml(sourceDir);
  const html = await fs.readFile(htmlPath, 'utf8');
  const metadata = parseMetadata(html);
  const oldSchemaText = extractCopyTarget(html, 'copyTarget1');
  const exampleText = extractCopyTarget(html, 'copyTarget2');
  const oldSchema = parseJsonFromCopyTarget(oldSchemaText);
  const example = parseJsonFromCopyTarget(exampleText);
  const schema = buildCurrentSchema(oldSchema, example);
  const schemaPretty = JSON.stringify(schema, null, 2);
  const enhancedPretty = JSON.stringify({ request: schema, response: {} }, null, 2);
  const specifications = buildSpecifications();

  const productId = String(args.product || 'verification');
  const groupId = String(args['group-id'] || `${productId}_project`);
  const endpointName = String(args['endpoint-name'] || metadata.title);
  const endpointPath = normalizePath(args.path || metadata.inputUri);
  const method = String(args.method || metadata.activeMethods || 'POST').trim();
  const endpointId = String(args['endpoint-id'] || endpointIdFromPath(endpointPath, endpointName, groupId, method));
  const versionName = String(args.version || 'LATEST API MANUAL');
  const author = String(args.author || 'Codex');
  const now = new Date().toISOString();

  const summary = {
    sourceDir,
    htmlPath,
    productId,
    groupId,
    endpointId,
    endpointName,
    endpointPath,
    method,
    versionName,
    schema,
    example,
  };

  if (args['dry-run']) {
    console.log(JSON.stringify({ dryRun: true, ...summary }, null, 2));
    return;
  }

  if (!args['skip-backup']) {
    const backup = spawnSync(process.execPath, [path.join(projectRoot, 'scripts', 'backup-supabase.mjs')], {
      cwd: projectRoot,
      stdio: 'inherit',
    });
    if (backup.status !== 0) {
      throw new Error('Supabase backup failed; import aborted.');
    }
  }

  const supabase = createClient(supabaseUrl, supabaseKey, {
    auth: { autoRefreshToken: false, persistSession: false },
  });

  const { data: product, error: productError } = await supabase
    .from('products')
    .select('id')
    .eq('id', productId)
    .maybeSingle();
  if (productError) throw productError;
  if (!product) throw new Error(`Product not found: ${productId}`);

  const { data: group, error: groupError } = await supabase
    .from('groups')
    .select('*')
    .eq('id', groupId)
    .maybeSingle();
  if (groupError) throw groupError;
  if (!group) throw new Error(`Group not found: ${groupId}`);

  const { data: existingEndpoint, error: existingEndpointError } = await supabase
    .from('endpoints')
    .select('created_at, order_index')
    .eq('id', endpointId)
    .maybeSingle();
  if (existingEndpointError) throw existingEndpointError;
  const orderIndex = existingEndpoint?.order_index ?? await getNextEndpointOrderIndex(supabase, groupId);
  const endpointRow = {
    id: endpointId,
    name: endpointName,
    method,
    path: endpointPath,
    product: productId,
    product_id: productId,
    group_name: group.name,
    group_id: groupId,
    description: `Imported from old Zendesk manual: ${metadata.title}`,
    status: 'success',
    order_index: orderIndex,
    created_at: existingEndpoint?.created_at || now,
    updated_at: now,
  };

  const { error: endpointError } = await supabase
    .from('endpoints')
    .upsert(endpointRow, { onConflict: 'id' });
  if (endpointError) throw endpointError;

  const { version, created } = await getOrCreateVersion(
    supabase,
    endpointId,
    versionName,
    author,
    `Imported from ${path.relative(projectRoot, htmlPath).replace(/\\/g, '/')}`,
  );

  const manual = {
    title: metadata.title,
    category: metadata.category,
    input_uri: endpointPath,
    active_methods: method,
    json_schema: schemaPretty,
    json_schema_original: schemaPretty,
    json_schema_enhanced: enhancedPretty,
    examples: JSON.stringify([{ title: metadata.title, code: JSON.stringify(example, null, 2) }]),
    request_examples: JSON.stringify([{ title: metadata.title, code: JSON.stringify(example, null, 2) }]),
    response_examples: JSON.stringify([]),
    specifications,
    html_content: html,
    article_id: metadata.articleId || null,
    section_id: metadata.sectionId || null,
    author_id: metadata.authorId || null,
    url: metadata.url || null,
  };
  const spec = {
    json_schema: schemaPretty,
    json_schema_original: schemaPretty,
    json_schema_enhanced: enhancedPretty,
    specifications,
  };
  const builder = {
    form_data: JSON.stringify({}),
  };
  const runner = {
    request_body: JSON.stringify(example, null, 2),
    response_body: '',
  };

  await upsertVersionData(supabase, version.id, { manual, spec, builder, runner });

  const { error: versionUpdateError } = await supabase
    .from('versions')
    .update({ updated_at: now })
    .eq('id', version.id);
  if (versionUpdateError) throw versionUpdateError;

  console.log(JSON.stringify({
    ok: true,
    endpointId,
    versionId: version.id,
    versionCreated: created,
    endpointPath,
    method,
    productId,
    groupId,
  }, null, 2));
}

main().catch((error) => {
  console.error(error);
  process.exit(1);
});
