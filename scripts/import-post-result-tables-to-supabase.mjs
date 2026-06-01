import { createClient } from '@supabase/supabase-js';
import dotenv from 'dotenv';
import fs from 'fs/promises';
import path from 'path';
import { fileURLToPath } from 'url';
import { spawnSync } from 'child_process';

const __dirname = path.dirname(fileURLToPath(import.meta.url));
const projectRoot = path.resolve(__dirname, '..');
dotenv.config({ path: path.join(projectRoot, '.env') });

const SOURCE_ROOT = path.join(projectRoot, 'api_data_set', 'POST');
const PRODUCT_ID = 'verification';
const GROUP_ID = 'verification_result_table';
const GROUP_NAME = 'Result Table';
const VERSION_NAME = 'LATEST API MANUAL';
const AUTHOR = 'Codex';

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

function decodeHtml(value) {
  return String(value || '')
    .replaceAll('&nbsp;', ' ')
    .replaceAll('&#160;', ' ')
    .replaceAll('&quot;', '"')
    .replaceAll('&#39;', "'")
    .replaceAll('&lt;', '<')
    .replaceAll('&gt;', '>')
    .replaceAll('&amp;', '&')
    .replace(/[\u00a0\uD63B\uFFFD]/g, ' ');
}

function htmlToText(value) {
  return decodeHtml(value)
    .replace(/<br\s*\/?\s*>/gi, '\n')
    .replace(/<[^>]*>/g, '')
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

function slugify(value, fallback = 'endpoint') {
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

function parseMetadata(html, fallbackTitle) {
  const title = extractFirst(html, /<h1[^>]*>([\s\S]*?)<\/h1>/i, fallbackTitle);
  const category = extractFirst(html, /<span[^>]*class=["']label["'][^>]*>\s*Category:\s*<\/span>([\s\S]*?)<\/div>/i, 'POST');
  const articleId = extractFirst(html, /<span[^>]*class=["']label["'][^>]*>\s*Article ID:\s*<\/span>([\s\S]*?)<\/div>/i);
  const authorId = extractFirst(html, /<span[^>]*class=["']label["'][^>]*>\s*Author ID:\s*<\/span>([\s\S]*?)<\/div>/i);
  const sectionId = extractFirst(html, /<span[^>]*class=["']label["'][^>]*>\s*Section ID:\s*<\/span>([\s\S]*?)<\/div>/i);
  const url = extractFirst(html, /<span[^>]*class=["']label["'][^>]*>\s*URL:\s*<\/span>([\s\S]*?)<\/div>/i);
  const inputUri = extractFirst(html, /<h3[^>]*>\s*<strong>\s*Input URI\s*<\/strong>\s*<\/h3>[\s\S]*?<strong>\s*\{base url\}\s*\+\s*([^<]+?)\s*<\/strong>/i, 'POST/TABLE');
  const activeMethods = extractFirst(html, /<h3[^>]*>\s*<strong>\s*Active Methods\s*<\/strong>\s*<\/h3>[\s\S]*?<strong>\s*([^<]+?)\s*<\/strong>/i, 'POST');
  return {
    title,
    category,
    articleId,
    authorId,
    sectionId,
    url,
    inputUri: normalizePath(inputUri),
    activeMethods: activeMethods.toUpperCase(),
  };
}

function extractJsonCandidates(html) {
  const text = htmlToText(html);
  const candidates = [];
  const parseJson = (raw) => {
    try {
      return JSON.parse(raw);
    } catch {
      return JSON.parse(raw.replace(/\\(?!["\\/bfnrtu])/g, '\\\\'));
    }
  };
  for (let start = 0; start < text.length; start += 1) {
    if (text[start] !== '{') continue;
    let depth = 0;
    let inString = false;
    let escaped = false;
    for (let i = start; i < text.length; i += 1) {
      const ch = text[i];
      if (inString) {
        if (escaped) {
          escaped = false;
        } else if (ch === '\\') {
          escaped = true;
        } else if (ch === '"') {
          inString = false;
        }
        continue;
      }
      if (ch === '"') {
        inString = true;
      } else if (ch === '{') {
        depth += 1;
      } else if (ch === '}') {
        depth -= 1;
        if (depth === 0) {
          const raw = text.slice(start, i + 1);
          try {
            const obj = parseJson(raw);
            candidates.push({ raw, obj });
            start = i;
          } catch {
            // Ignore malformed snippets; old Zendesk pages sometimes contain partial examples.
          }
          break;
        }
      }
    }
  }
  const seen = new Set();
  return candidates.filter((candidate) => {
    const key = JSON.stringify(candidate.obj);
    if (seen.has(key)) return false;
    seen.add(key);
    return true;
  });
}

function findSchemaCandidate(candidates) {
  for (const candidate of candidates) {
    if (candidate.obj?.TABLE && typeof candidate.obj.TABLE === 'object') return candidate.obj.TABLE;
    if (candidate.obj?.PM && typeof candidate.obj.PM === 'object') return candidate.obj.PM;
  }
  for (const candidate of candidates) {
    if (candidate.obj?.properties?.Argument || candidate.obj?.properties?.argument) return candidate.obj;
  }
  return null;
}

function findRequestCandidates(candidates) {
  return candidates
    .map((candidate) => candidate.obj)
    .filter((obj) => obj && typeof obj === 'object' && obj.Argument && typeof obj.Argument === 'object' && !obj.properties);
}

function findResponseCandidates(candidates) {
  return candidates
    .map((candidate) => candidate.obj)
    .filter((obj) => obj && typeof obj === 'object' && !obj.Argument && !obj.properties && !obj.TABLE && !obj.PM);
}

function walk(node, visitor, trail = []) {
  visitor(node, trail);
  if (Array.isArray(node)) {
    node.forEach((item, index) => walk(item, visitor, trail.concat(index)));
    return;
  }
  if (node && typeof node === 'object') {
    Object.entries(node).forEach(([key, value]) => walk(value, visitor, trail.concat(key)));
  }
}

function collectResponseHeads(responseCandidates) {
  const heads = [];
  for (const response of responseCandidates) {
    walk(response, (node, trail) => {
      if (!Array.isArray(node) || trail[trail.length - 1] !== 'HEAD') return;
      const labels = node.filter((item) => typeof item === 'string');
      if (labels.length === 0) return;
      const tableName = typeof trail[trail.length - 2] === 'string' ? trail[trail.length - 2] : '';
      heads.push({ tableName, labels });
    });
  }
  return heads;
}

function labelFromKey(key) {
  const common = {
    TABLE_NAME: 'Table Name',
    TABLE_TYPE: 'Table Type',
    EXPORT_PATH: 'Export Path',
    NODE_ELEMS: 'Node/Element Selection',
    TENDON_NO: 'Tendon Selection',
    LOAD_CASE_NAMES: 'Load Case Names',
    MODAL_MODES: 'Modal Modes',
    PARTS: 'Parts',
    COMPONENTS: 'Components',
    UNIT: 'Unit Settings',
    STYLES: 'Number Format',
  };
  if (common[key]) return common[key];
  return String(key)
    .toLowerCase()
    .split('_')
    .filter(Boolean)
    .map((part) => part.charAt(0).toUpperCase() + part.slice(1))
    .join(' ');
}

function labelFromTableType(type) {
  return String(type || '')
    .toLowerCase()
    .split('_')
    .filter(Boolean)
    .map((part) => part.charAt(0).toUpperCase() + part.slice(1))
    .join(' ');
}

function normalizeComponentKey(label) {
  const greekMap = {
    '\u03b1': 'alpha',
    '\u0391': 'Alpha',
    '\u03b2': 'beta',
    '\u0392': 'Beta',
    '\u03b3': 'gamma',
    '\u0393': 'Gamma',
    '\u03b4': 'delta',
    '\u0394': 'Delta',
    '\u03b5': 'epsilon',
    '\u0395': 'Epsilon',
    '\u03b8': 'theta',
    '\u0398': 'Theta',
    '\u03c1': 'rho',
    '\u03a1': 'Rho',
    '\u03c3': 'sigma',
    '\u03a3': 'Sigma',
    '\u03c4': 'tau',
    '\u03a4': 'Tau',
    '\u03c6': 'phi',
    '\u03a6': 'Phi',
  };
  let value = String(label || '').replace(/[\u03b1\u0391\u03b2\u0392\u03b3\u0393\u03b4\u0394\u03b5\u0395\u03b8\u0398\u03c1\u03a1\u03c3\u03a3\u03c4\u03a4\u03c6\u03a6]/g, (char) => greekMap[char] || char);
  if (/\s/.test(value)) {
    value = value
      .split(/\s+/)
      .filter(Boolean)
      .map((part) => part.charAt(0).toUpperCase() + part.slice(1))
      .join('');
  }
  return value;
}

function normalizeComponentValue(label) {
  const greekMap = {
    'φ': 'Phi',
    'Φ': 'Phi',
    'θ': 'Theta',
    'Θ': 'Theta',
    'ρ': 'Rho',
    'Ρ': 'Rho',
    'δ': 'Delta',
    'Δ': 'Delta',
    'σ': 'Sigma',
    'Σ': 'Sigma',
    'τ': 'Tau',
    'Τ': 'Tau',
    'ε': 'Epsilon',
    'Ε': 'Epsilon',
  };
  let value = String(label || '').replace(/[φΦθΘρΡδΔσΣτΤεΕ]/g, (char) => greekMap[char] || char);
  if (/\s/.test(value)) {
    value = value
      .split(/\s+/)
      .filter(Boolean)
      .map((part) => part.charAt(0).toUpperCase() + part.slice(1))
      .join('');
  }
  return value;
}

function dedupe(values) {
  const seen = new Set();
  const out = [];
  for (const value of values) {
    if (value === undefined || value === null || value === '') continue;
    const key = String(value);
    if (seen.has(key)) continue;
    seen.add(key);
    out.push(key);
  }
  return out;
}

function inferSchemaFromValue(value) {
  if (Array.isArray(value)) {
    const item = value.find((entry) => entry !== null && entry !== undefined);
    return {
      type: 'array',
      items: item === undefined ? { type: 'string' } : inferSchemaFromValue(item),
    };
  }
  if (value && typeof value === 'object') {
    const properties = {};
    for (const [key, child] of Object.entries(value)) {
      properties[key] = inferSchemaFromValue(child);
    }
    return { type: 'object', additionalProperties: false, properties };
  }
  if (typeof value === 'number') return { type: Number.isInteger(value) ? 'integer' : 'number' };
  if (typeof value === 'boolean') return { type: 'boolean' };
  return { type: 'string' };
}

function mergePropertyKeys(schemaCandidate, requestCandidates) {
  const keys = [];
  const argSchema = schemaCandidate?.properties?.Argument || schemaCandidate?.properties?.argument || {};
  if (argSchema?.properties) keys.push(...Object.keys(argSchema.properties));
  for (const request of requestCandidates) {
    keys.push(...Object.keys(request.Argument || {}));
  }
  return dedupe(keys);
}

function findSchemaProperty(schemaCandidate, key) {
  const argSchema = schemaCandidate?.properties?.Argument || schemaCandidate?.properties?.argument || {};
  return argSchema?.properties?.[key] || null;
}

function buildSelectionObject(key, schemaProp, requestCandidates) {
  const sample = requestCandidates.map((request) => request.Argument?.[key]).find((value) => value && typeof value === 'object' && !Array.isArray(value));
  const sourceProps = schemaProp?.properties || (sample ? Object.fromEntries(Object.keys(sample).map((childKey) => [childKey, inferSchemaFromValue(sample[childKey])])) : {});
  const properties = {};
  const childKeys = dedupe([...Object.keys(sourceProps), 'KEYS', 'TO', 'STRUCTURE_GROUP_NAME'].filter((childKey) => childKey in sourceProps || ['KEYS', 'TO', 'STRUCTURE_GROUP_NAME'].includes(childKey)));
  for (const childKey of childKeys) {
    if (childKey === 'KEYS') {
      properties.KEYS = {
        type: 'array',
        description: 'Specify Each ID',
        items: { type: 'integer' },
      };
    } else if (childKey === 'TO') {
      properties.TO = {
        type: 'string',
        description: "Specify ID Range (e.g., '1to160')",
      };
    } else if (childKey === 'STRUCTURE_GROUP_NAME') {
      properties.STRUCTURE_GROUP_NAME = {
        type: 'string',
        description: 'Specify Structure Group Name',
      };
    } else {
      properties[childKey] = { ...sourceProps[childKey], description: sourceProps[childKey]?.description || labelFromKey(childKey) };
    }
  }
  return {
    type: 'object',
    description: schemaProp?.description || labelFromKey(key),
    properties,
    'x-ui': {
      hint: 'Use only ONE of the available selection methods.',
      label: labelFromKey(key),
    },
  };
}

function collectTableTypes(schemaCandidate, requestCandidates, responseHeads) {
  const values = [];
  const schemaProp = findSchemaProperty(schemaCandidate, 'TABLE_TYPE');
  if (Array.isArray(schemaProp?.enum)) values.push(...schemaProp.enum);
  for (const request of requestCandidates) {
    if (typeof request.Argument?.TABLE_TYPE === 'string') values.push(request.Argument.TABLE_TYPE);
  }
  if (values.length === 0) {
    for (const head of responseHeads) {
      if (/^[A-Z0-9_]+$/.test(head.tableName)) values.push(head.tableName);
    }
  }
  return dedupe(values);
}

function collectTableTypeLabels(tableTypes, requestCandidates, responseHeads, title) {
  const labels = {};
  const nameToType = {};
  const genericNames = new Set(['example', 'string', 'table', 'empty']);
  for (const request of requestCandidates) {
    const type = request.Argument?.TABLE_TYPE;
    const tableName = request.Argument?.TABLE_NAME;
    if (typeof type === 'string' && typeof tableName === 'string' && tableName && !genericNames.has(tableName.trim().toLowerCase())) {
      labels[type] = tableName;
      nameToType[tableName] = type;
    }
  }
  for (const head of responseHeads) {
    if (head.tableName && nameToType[head.tableName]) {
      labels[nameToType[head.tableName]] ||= head.tableName;
    }
  }
  for (const type of tableTypes) {
    labels[type] ||= tableTypes.length === 1 ? title : labelFromTableType(type);
  }
  return { labels, nameToType };
}

function collectComponents(tableTypes, requestCandidates, responseHeads, nameToType) {
  const byType = {};
  const globalLabels = {};
  const add = (type, labels) => {
    const normalized = [];
    for (const label of labels) {
      if (!label || label === 'Index') continue;
      const value = normalizeComponentKey(label);
      normalized.push(value);
      globalLabels[value] ||= label;
    }
    if (!type) {
      for (const value of normalized) globalLabels[value] ||= value;
      return;
    }
    byType[type] = dedupe([...(byType[type] || []), ...normalized]);
  };
  for (const request of requestCandidates) {
    const type = request.Argument?.TABLE_TYPE;
    const components = request.Argument?.COMPONENTS;
    if (Array.isArray(components)) add(type, components.filter((item) => typeof item === 'string'));
  }
  responseHeads.forEach((head, index) => {
    const type = nameToType[head.tableName] || (tableTypes.includes(head.tableName) ? head.tableName : tableTypes.length === 1 ? tableTypes[0] : '');
    if (type) {
      if (!byType[type]?.length) add(type, head.labels);
      return;
    }
    if (responseHeads.length === tableTypes.length && tableTypes[index]) {
      if (!byType[tableTypes[index]]?.length) add(tableTypes[index], head.labels);
      return;
    }
    add('', head.labels);
  });
  const union = dedupe([...Object.values(byType).flat(), ...Object.keys(globalLabels)]);
  return { union, labels: globalLabels, byType };
}

function buildProperty(key, schemaProp, requestCandidates, context) {
  if (key === 'TABLE_NAME') {
    return {
      type: 'string',
      description: 'Response Table Title',
      default: '',
      'x-ui': { hint: 'Output table title in response', label: 'Table Name' },
    };
  }
  if (key === 'TABLE_TYPE') {
    const property = {
      type: 'string',
      description: `${context.title} - Result Table Type`,
      enum: context.tableTypes,
      'x-ui': { label: 'Table Type' },
    };
    if (Object.keys(context.tableTypeLabels).length > 0) property['x-enum-labels'] = context.tableTypeLabels;
    return property;
  }
  if (key === 'EXPORT_PATH') {
    return {
      type: 'string',
      description: 'Result Table Save Path',
      'x-ui': { hint: 'Optional: Save results to file path', label: 'Export Path' },
    };
  }
  if (key === 'UNIT') {
    return {
      type: 'object',
      description: 'Response Unit Setting',
      properties: {
        FORCE: { type: 'string', description: 'Force unit' },
        DIST: { type: 'string', description: 'Length/Distance unit' },
        HEAT: { type: 'string', description: 'Heat unit' },
        TEMP: { type: 'string', description: 'Temperature unit' },
      },
      default: 'System',
      'x-ui': { label: 'Unit Settings' },
    };
  }
  if (key === 'STYLES') {
    return {
      type: 'object',
      description: 'Response Number Format',
      properties: {
        FORMAT: {
          type: 'string',
          description: 'Number format',
          enum: ['Default', 'Fixed', 'Scientific', 'General'],
        },
        PLACE: {
          type: 'integer',
          description: 'Digit place',
          minimum: 0,
          maximum: 15,
        },
      },
      default: 'System',
      'x-ui': { label: 'Number Format' },
    };
  }
  if (key === 'COMPONENTS') {
    const property = {
      type: 'array',
      description: 'Components of Result Table',
      items: { type: 'string', enum: context.components.union },
      'x-ui': { label: 'Components' },
    };
    if (Object.keys(context.components.labels).length > 0) property['x-enum-labels'] = context.components.labels;
    const labelsByType = {};
    for (const [type, values] of Object.entries(context.components.byType)) {
      labelsByType[type] = Object.fromEntries(values.map((value) => [value, context.components.labels[value] || value]));
    }
    if (Object.keys(labelsByType).length > 1) property['x-enum-labels-by-type'] = labelsByType;
    return property;
  }
  if (schemaProp?.type === 'object' || requestCandidates.some((request) => request.Argument?.[key] && typeof request.Argument[key] === 'object' && !Array.isArray(request.Argument[key]))) {
    return buildSelectionObject(key, schemaProp, requestCandidates);
  }
  const sample = requestCandidates.map((request) => request.Argument?.[key]).find((value) => value !== undefined);
  const inferred = schemaProp || inferSchemaFromValue(sample);
  const property = {
    ...inferred,
    description: inferred.description || labelFromKey(key),
    'x-ui': { label: labelFromKey(key) },
  };
  if (Array.isArray(property.items?.enum)) property.items.enum = dedupe(property.items.enum.map(normalizeComponentKey));
  return property;
}

function cleanSchemaObject(node) {
  if (!node || typeof node !== 'object') return node;
  if (Array.isArray(node)) return node.map(cleanSchemaObject);
  const next = {};
  for (const [key, value] of Object.entries(node)) {
    if (key === '$schema' || key === 'title') continue;
    next[key] = cleanSchemaObject(value);
  }
  if (next.type === 'object') {
    next.additionalProperties = false;
    next.properties ||= {};
  }
  return next;
}

function buildCurrentSchema({ metadata, schemaCandidate, requestCandidates, responseHeads }) {
  const tableTypes = collectTableTypes(schemaCandidate, requestCandidates, responseHeads);
  const { labels: tableTypeLabels, nameToType } = collectTableTypeLabels(tableTypes, requestCandidates, responseHeads, metadata.title);
  const components = collectComponents(tableTypes, requestCandidates, responseHeads, nameToType);
  const keys = mergePropertyKeys(schemaCandidate, requestCandidates);
  const finalKeys = dedupe([
    'TABLE_NAME',
    'TABLE_TYPE',
    'EXPORT_PATH',
    'UNIT',
    'STYLES',
    ...keys.filter((key) => !['TABLE_NAME', 'TABLE_TYPE', 'EXPORT_PATH', 'UNIT', 'STYLES', 'COMPONENTS'].includes(key)),
    ...(components.union.length > 0 ? ['COMPONENTS'] : []),
  ]).filter((key) => key !== 'argument' && key !== 'Argument');
  const properties = {};
  const context = { title: metadata.title, tableTypes, tableTypeLabels, components };
  for (const key of finalKeys) {
    const prop = buildProperty(key, findSchemaProperty(schemaCandidate, key), requestCandidates, context);
    if (key === 'TABLE_TYPE' && tableTypes.length === 0) continue;
    if (key === 'COMPONENTS' && components.union.length === 0) continue;
    properties[key] = prop;
  }
  const argumentRequired = properties.TABLE_TYPE ? ['TABLE_TYPE'] : [];
  return cleanSchemaObject({
    type: 'object',
    required: ['Argument'],
    additionalProperties: false,
    properties: {
      Argument: {
        type: 'object',
        required: argumentRequired,
        additionalProperties: false,
        properties,
      },
    },
  });
}

function buildSpecifications(schema) {
  const lines = [
    '| No. | Description | Key | Value Type | Default | Required |',
    '| --- | --- | --- | --- | --- | --- |',
    '| 1 | Request body wrapper object for Result Table API. | Argument | object | - | Required |',
  ];
  const props = schema.properties.Argument.properties;
  const required = new Set(schema.properties.Argument.required || []);
  let index = 1;
  for (const [key, prop] of Object.entries(props)) {
    lines.push(`| 1.${index++} | ${prop.description || labelFromKey(key)} | ${key} | ${prop.type || '-'} | ${prop.default ?? '-'} | ${required.has(key) ? 'Required' : 'Optional'} |`);
  }
  return lines.join('\n');
}

async function findHtmlFiles() {
  const entries = await fs.readdir(SOURCE_ROOT, { withFileTypes: true });
  const result = [];
  for (const entry of entries) {
    if (!entry.isDirectory()) continue;
    const folder = path.join(SOURCE_ROOT, entry.name);
    const zendeskDir = path.join(folder, 'zendesk');
    const files = await fs.readdir(zendeskDir);
    const htmlFile = files.find((file) => file.toLowerCase().endsWith('.html'));
    if (htmlFile) result.push({ folderName: entry.name, folder, htmlPath: path.join(zendeskDir, htmlFile) });
  }
  return result.sort((a, b) => a.folderName.localeCompare(b.folderName));
}

async function getNextGroupOrderIndex(supabase, productId) {
  const { data, error } = await supabase
    .from('groups')
    .select('order_index')
    .eq('product_id', productId)
    .order('order_index', { ascending: false })
    .limit(1);
  if (error) throw error;
  return (data?.[0]?.order_index ?? -1) + 1;
}

async function getNextEndpointOrderIndex(supabase, groupId) {
  const { data, error } = await supabase
    .from('endpoints')
    .select('order_index')
    .eq('group_id', groupId)
    .order('order_index', { ascending: false })
    .limit(1);
  if (error) throw error;
  return (data?.[0]?.order_index ?? -1) + 1;
}

async function getOrCreateVersion(supabase, endpointId, versionName, now, sourcePath) {
  const { data: existing, error: existingError } = await supabase
    .from('versions')
    .select('*')
    .eq('endpoint_id', endpointId)
    .eq('version', versionName)
    .maybeSingle();
  if (existingError) throw existingError;
  if (existing) return { version: existing, created: false };
  const row = {
    id: `v_${Date.now()}_${Math.random().toString(36).slice(2, 11)}`,
    version: versionName,
    endpoint_id: endpointId,
    created_at: now,
    updated_at: now,
    author: AUTHOR,
    change_log: `Imported from ${sourcePath}`,
  };
  const { data, error } = await supabase.from('versions').insert(row).select().single();
  if (error) throw error;
  return { version: data, created: true };
}

function makeEndpointId(endpointPath, title) {
  const normalizedPath = normalizePath(endpointPath || '/POST/TABLE').replace(/^\//, '').replace(/\//g, '-');
  return `${slugify(normalizedPath)}-${slugify(title)}`;
}

function firstRequestExample(requestCandidates) {
  return requestCandidates[0] || { Argument: {} };
}

function firstResponseExample(responseCandidates) {
  return responseCandidates[0] || {};
}

async function analyzeFile(file) {
  const html = await fs.readFile(file.htmlPath, 'utf8');
  const metadata = parseMetadata(html, file.folderName.replace(/^\d+_/, '').replace(/_/g, ' '));
  const candidates = extractJsonCandidates(html);
  const schemaCandidate = findSchemaCandidate(candidates);
  const requestCandidates = findRequestCandidates(candidates);
  const responseCandidates = findResponseCandidates(candidates);
  const responseHeads = collectResponseHeads(responseCandidates);
  const schema = buildCurrentSchema({ metadata, schemaCandidate, requestCandidates, responseHeads });
  const endpointPath = normalizePath(metadata.inputUri || '/POST/TABLE');
  return {
    ...file,
    html,
    metadata,
    candidates,
    schemaCandidate,
    requestCandidates,
    responseCandidates,
    responseHeads,
    schema,
    endpointPath,
    endpointId: makeEndpointId(endpointPath, metadata.title),
  };
}

async function ensureResultTableGroup(supabase, now) {
  const { data: existing, error: existingError } = await supabase
    .from('groups')
    .select('*')
    .eq('id', GROUP_ID)
    .maybeSingle();
  if (existingError) throw existingError;
  if (existing) return existing;
  const orderIndex = await getNextGroupOrderIndex(supabase, PRODUCT_ID);
  const row = {
    id: GROUP_ID,
    product_id: PRODUCT_ID,
    name: GROUP_NAME,
    description: 'Imported POST result table APIs',
    order_index: orderIndex,
    parent_group_id: null,
    depth: 1,
    created_at: now,
    updated_at: now,
  };
  const { data, error } = await supabase.from('groups').insert(row).select().single();
  if (error) throw error;
  return data;
}

async function importAnalysis(supabase, analysis, group, orderIndex, now) {
  const sourcePath = path.relative(projectRoot, analysis.htmlPath).replace(/\\/g, '/');
  const schemaPretty = JSON.stringify(analysis.schema, null, 2);
  const enhancedPretty = JSON.stringify({ request: analysis.schema, response: {} }, null, 2);
  const oldSchemaPretty = analysis.schemaCandidate ? JSON.stringify(analysis.schemaCandidate, null, 2) : schemaPretty;
  const requestExample = firstRequestExample(analysis.requestCandidates);
  const responseExample = firstResponseExample(analysis.responseCandidates);
  const endpointRow = {
    id: analysis.endpointId,
    name: analysis.metadata.title,
    method: analysis.metadata.activeMethods || 'POST',
    path: analysis.endpointPath,
    product: PRODUCT_ID,
    product_id: PRODUCT_ID,
    group_name: group.name,
    group_id: group.id,
    description: `Imported from old Zendesk manual: ${analysis.metadata.title}`,
    status: 'success',
    order_index: orderIndex,
    created_at: now,
    updated_at: now,
  };
  const { error: endpointError } = await supabase.from('endpoints').upsert(endpointRow, { onConflict: 'id' });
  if (endpointError) throw endpointError;
  const { version, created } = await getOrCreateVersion(supabase, analysis.endpointId, VERSION_NAME, now, sourcePath);
  const manual = {
    version_id: version.id,
    title: analysis.metadata.title,
    category: analysis.metadata.category,
    input_uri: analysis.endpointPath,
    active_methods: endpointRow.method,
    json_schema: oldSchemaPretty,
    json_schema_original: oldSchemaPretty,
    json_schema_enhanced: oldSchemaPretty,
    examples: JSON.stringify(analysis.requestCandidates.map((example, index) => ({ title: `${analysis.metadata.title} ${index + 1}`, code: JSON.stringify(example, null, 2) }))),
    request_examples: JSON.stringify(analysis.requestCandidates.map((example, index) => ({ title: `${analysis.metadata.title} ${index + 1}`, code: JSON.stringify(example, null, 2) }))),
    response_examples: JSON.stringify(analysis.responseCandidates.map((example, index) => ({ title: `${analysis.metadata.title} ${index + 1}`, code: JSON.stringify(example, null, 2) }))),
    specifications: '',
    html_content: analysis.html,
    article_id: analysis.metadata.articleId || null,
    section_id: analysis.metadata.sectionId || null,
    author_id: analysis.metadata.authorId || null,
    url: analysis.metadata.url || null,
  };
  const spec = {
    version_id: version.id,
    json_schema: schemaPretty,
    json_schema_original: schemaPretty,
    json_schema_enhanced: enhancedPretty,
    specifications: buildSpecifications(analysis.schema),
  };
  const builder = {
    version_id: version.id,
    form_data: JSON.stringify({}),
  };
  const runner = {
    version_id: version.id,
    request_body: JSON.stringify(requestExample, null, 2),
    response_body: Object.keys(responseExample).length ? JSON.stringify(responseExample, null, 2) : '',
  };
  const results = await Promise.all([
    supabase.from('manual_data').upsert(manual, { onConflict: 'version_id' }),
    supabase.from('spec_data').upsert(spec, { onConflict: 'version_id' }),
    supabase.from('builder_data').upsert(builder, { onConflict: 'version_id' }),
    supabase.from('runner_data').upsert(runner, { onConflict: 'version_id' }),
    supabase.from('versions').update({ updated_at: now }).eq('id', version.id),
  ]);
  for (const result of results) {
    if (result.error) throw result.error;
  }
  return { versionId: version.id, versionCreated: created };
}

async function main() {
  const args = parseArgs(process.argv.slice(2));
  const supabaseUrl = process.env.SUPABASE_URL;
  const supabaseKey = process.env.SUPABASE_SERVICE_KEY;
  if (!supabaseUrl || !supabaseKey) throw new Error('SUPABASE_URL and SUPABASE_SERVICE_KEY are required in .env');

  const files = await findHtmlFiles();
  const analyses = [];
  for (const file of files) {
    if (file.folderName.startsWith('001_')) continue;
    analyses.push(await analyzeFile(file));
  }
  const summary = {
    total: analyses.length,
    withTableType: analyses.filter((item) => item.schema.properties.Argument.properties.TABLE_TYPE).length,
    withComponents: analyses.filter((item) => item.schema.properties.Argument.properties.COMPONENTS).length,
    noComponents: analyses
      .filter((item) => !item.schema.properties.Argument.properties.COMPONENTS)
      .map((item) => item.folderName),
    emptyArgument: analyses
      .filter((item) => Object.keys(item.schema.properties.Argument.properties).length === 0)
      .map((item) => item.folderName),
  };
  if (args['dry-run']) {
    console.log(JSON.stringify({ dryRun: true, summary, sample: analyses.slice(1, 4).map((item) => ({
      endpointId: item.endpointId,
      title: item.metadata.title,
      path: item.endpointPath,
      schema: item.schema,
    })) }, null, 2));
    return;
  }
  if (!args['skip-backup']) {
    const backup = spawnSync(process.execPath, [path.join(projectRoot, 'scripts', 'backup-supabase.mjs')], {
      cwd: projectRoot,
      stdio: 'inherit',
    });
    if (backup.status !== 0) throw new Error('Supabase backup failed; import aborted.');
  }
  const supabase = createClient(supabaseUrl, supabaseKey, {
    auth: { autoRefreshToken: false, persistSession: false },
  });
  const now = new Date().toISOString();
  const group = await ensureResultTableGroup(supabase, now);
  let orderIndex = await getNextEndpointOrderIndex(supabase, group.id);
  const imported = [];
  const failed = [];
  for (const analysis of analyses) {
    try {
      const result = await importAnalysis(supabase, analysis, group, orderIndex, now);
      imported.push({
        endpointId: analysis.endpointId,
        title: analysis.metadata.title,
        versionId: result.versionId,
        components: analysis.schema.properties.Argument.properties.COMPONENTS?.items?.enum?.length || 0,
      });
      orderIndex += 1;
    } catch (error) {
      failed.push({ folderName: analysis.folderName, title: analysis.metadata.title, error: error.message });
    }
  }
  console.log(JSON.stringify({
    ok: failed.length === 0,
    groupId: group.id,
    summary,
    importedCount: imported.length,
    failedCount: failed.length,
    failed,
    imported: imported.slice(0, 20),
  }, null, 2));
  if (failed.length > 0) process.exitCode = 1;
}

main().catch((error) => {
  console.error(error);
  process.exit(1);
});
