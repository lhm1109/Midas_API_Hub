import { createClient } from '@supabase/supabase-js';
import dotenv from 'dotenv';
import path from 'path';
import { fileURLToPath } from 'url';
import { generatePydanticCode, savePydanticModule } from '../server/lib/pydantic-generator.js';

const __dirname = path.dirname(fileURLToPath(import.meta.url));
const projectRoot = path.resolve(__dirname, '..');
dotenv.config({ path: path.join(projectRoot, '.env') });

const outputRoot = path.join(projectRoot, 'generated_pydantic');

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

function safeParseJson(value) {
  if (!value) return null;
  try {
    return JSON.parse(value);
  } catch {
    return null;
  }
}

function pickSchema(specData) {
  const enhanced = safeParseJson(specData?.json_schema_enhanced);
  if (enhanced && typeof enhanced === 'object' && Object.keys(enhanced).length > 0) return enhanced;
  const current = safeParseJson(specData?.json_schema);
  if (current && typeof current === 'object' && Object.keys(current).length > 0) return current;
  const original = safeParseJson(specData?.json_schema_original);
  if (original && typeof original === 'object' && Object.keys(original).length > 0) return original;
  return null;
}

async function fetchAll(supabase, table, select = '*') {
  const rows = [];
  const pageSize = 1000;
  for (let from = 0; ; from += pageSize) {
    const to = from + pageSize - 1;
    const { data, error } = await supabase.from(table).select(select).range(from, to);
    if (error) throw error;
    rows.push(...(data || []));
    if (!data || data.length < pageSize) break;
  }
  return rows;
}

function pickLatestVersion(versions) {
  return [...versions].sort((a, b) => {
    const aTime = Date.parse(a.updated_at || a.created_at || '') || 0;
    const bTime = Date.parse(b.updated_at || b.created_at || '') || 0;
    return bTime - aTime;
  })[0];
}

async function main() {
  const args = parseArgs(process.argv.slice(2));
  const supabaseUrl = process.env.SUPABASE_URL;
  const supabaseKey = process.env.SUPABASE_SERVICE_KEY;
  if (!supabaseUrl || !supabaseKey) {
    throw new Error('SUPABASE_URL and SUPABASE_SERVICE_KEY are required in .env');
  }

  const supabase = createClient(supabaseUrl, supabaseKey, {
    auth: { autoRefreshToken: false, persistSession: false },
  });

  const [endpoints, versions, specRows] = await Promise.all([
    fetchAll(supabase, 'endpoints'),
    fetchAll(supabase, 'versions'),
    fetchAll(supabase, 'spec_data'),
  ]);

  const versionsByEndpoint = new Map();
  for (const version of versions) {
    const list = versionsByEndpoint.get(version.endpoint_id) || [];
    list.push(version);
    versionsByEndpoint.set(version.endpoint_id, list);
  }
  const specByVersion = new Map(specRows.map((row) => [row.version_id, row]));

  const exported = [];
  const skipped = [];

  for (const endpoint of endpoints) {
    const latestVersion = pickLatestVersion(versionsByEndpoint.get(endpoint.id) || []);
    if (!latestVersion) {
      skipped.push({ endpointId: endpoint.id, reason: 'no version' });
      continue;
    }
    const schema = pickSchema(specByVersion.get(latestVersion.id));
    if (!schema) {
      skipped.push({ endpointId: endpoint.id, reason: 'no spec schema' });
      continue;
    }

    const endpointPayload = {
      id: endpoint.id,
      name: endpoint.name,
      method: endpoint.method,
      path: endpoint.path,
      productId: endpoint.product_id || endpoint.product,
      product: endpoint.product,
    };

    try {
      if (args['dry-run']) {
        const result = generatePydanticCode({ schema, endpoint: endpointPayload });
        exported.push({ endpointId: endpoint.id, classes: result.rootClasses });
      } else {
        const result = await savePydanticModule({ schema, endpoint: endpointPayload, outputRoot });
        exported.push({ endpointId: endpoint.id, filePath: result.filePath, classes: result.rootClasses });
      }
    } catch (error) {
      skipped.push({ endpointId: endpoint.id, reason: error instanceof Error ? error.message : String(error) });
    }
  }

  console.log(JSON.stringify({
    dryRun: !!args['dry-run'],
    outputRoot,
    exportedCount: exported.length,
    skippedCount: skipped.length,
    skipped,
    sample: exported.slice(0, 20),
  }, null, 2));

  if (skipped.some((item) => !['no version', 'no spec schema'].includes(item.reason))) {
    process.exitCode = 1;
  }
}

main().catch((error) => {
  console.error(error);
  process.exit(1);
});
