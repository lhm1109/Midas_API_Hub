/**
 * Supabase Postgres 논리 백업: 서비스 롤로 공개 테이블 전체를 JSON으로 저장합니다.
 * Storage(api-verification 버킷) 바이너리는 포함하지 않습니다. DATABASE_URL이 있으면 pg_dump를 권장합니다.
 */
import { createClient } from '@supabase/supabase-js';
import dotenv from 'dotenv';
import fs from 'fs/promises';
import path from 'path';
import { fileURLToPath } from 'url';

const __dirname = path.dirname(fileURLToPath(import.meta.url));
dotenv.config({ path: path.join(__dirname, '..', '.env') });

const TABLES = [
  'products',
  'groups',
  'endpoints',
  'versions',
  'manual_data',
  'spec_data',
  'builder_data',
  'runner_data',
  'test_cases',
  'attachments',
  'endpoint_locks',
  'version_locks',
  'edit_history',
  'manager_columns',
  'manager_tasks',
];

const PAGE = 1000;

async function fetchAllRows(supabase, table) {
  const rows = [];
  let from = 0;
  for (;;) {
    const { data, error } = await supabase
      .from(table)
      .select('*')
      .range(from, from + PAGE - 1);
    if (error) throw new Error(`${table}: ${error.message}`);
    if (!data?.length) break;
    rows.push(...data);
    if (data.length < PAGE) break;
    from += PAGE;
  }
  return rows;
}

async function main() {
  const supabaseUrl = process.env.SUPABASE_URL;
  const supabaseKey = process.env.SUPABASE_SERVICE_KEY;
  if (!supabaseUrl || !supabaseKey) {
    console.error('SUPABASE_URL 및 SUPABASE_SERVICE_KEY가 .env에 필요합니다.');
    process.exit(1);
  }

  const supabase = createClient(supabaseUrl, supabaseKey, {
    auth: { autoRefreshToken: false, persistSession: false },
  });

  const stamp = new Date()
    .toISOString()
    .replace(/[:.]/g, '-')
    .slice(0, 19);
  const outDir = path.join(__dirname, '..', 'backups', `supabase-data-${stamp}`);
  await fs.mkdir(outDir, { recursive: true });

  const manifest = {
    createdAt: new Date().toISOString(),
    supabaseUrl,
    tables: {},
    note: 'Storage bucket files are not included. For SQL dump set DATABASE_URL and use pg_dump.',
  };

  for (const table of TABLES) {
    try {
      const rows = await fetchAllRows(supabase, table);
      manifest.tables[table] = { rowCount: rows.length, file: `${table}.json` };
      await fs.writeFile(
        path.join(outDir, `${table}.json`),
        JSON.stringify(rows, null, 2),
        'utf8',
      );
      console.log(`OK ${table}: ${rows.length} rows`);
    } catch (e) {
      const msg = e instanceof Error ? e.message : String(e);
      if (/does not exist|schema cache|not find|Could not find/i.test(msg)) {
        manifest.tables[table] = { skipped: true, reason: msg };
        console.warn(`Skip ${table}: ${msg}`);
      } else {
        manifest.tables[table] = { error: msg };
        console.error(`FAIL ${table}: ${msg}`);
      }
    }
  }

  await fs.writeFile(
    path.join(outDir, 'manifest.json'),
    JSON.stringify(manifest, null, 2),
    'utf8',
  );
  console.log(`\nBackup directory: ${outDir}`);
}

main().catch((err) => {
  console.error(err);
  process.exit(1);
});
