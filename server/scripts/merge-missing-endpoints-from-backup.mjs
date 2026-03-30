/**
 * Debug export JSON(또는 동일 형식)에서 현재 DB에 없는 endpoints 행만 삽입합니다.
 * 사용: node server/scripts/merge-missing-endpoints-from-backup.mjs <backup.json>
 * server/.env에 SUPABASE_URL, SUPABASE_SERVICE_KEY 필요
 */
import dotenv from 'dotenv';
import { createClient } from '@supabase/supabase-js';
import { readFileSync } from 'fs';
import { fileURLToPath } from 'url';
import { dirname, join } from 'path';

const __dirname = dirname(fileURLToPath(import.meta.url));
dotenv.config({ path: join(__dirname, '..', '.env') });

const supabaseUrl = process.env.SUPABASE_URL;
const supabaseKey = process.env.SUPABASE_SERVICE_KEY;

if (!supabaseUrl || !supabaseKey) {
  console.error('SUPABASE_URL and SUPABASE_SERVICE_KEY are required (server/.env)');
  process.exit(1);
}

const backupPath = process.argv[2];
if (!backupPath) {
  console.error('Usage: node server/scripts/merge-missing-endpoints-from-backup.mjs <path-to-backup.json>');
  process.exit(1);
}

const supabase = createClient(supabaseUrl, supabaseKey, {
  auth: { autoRefreshToken: false, persistSession: false },
});

const raw = readFileSync(backupPath, 'utf8');
const backup = JSON.parse(raw);
const endpoints = backup.tables?.endpoints;

if (!Array.isArray(endpoints)) {
  console.error('Invalid backup: tables.endpoints must be an array');
  process.exit(1);
}

const { data: existingRows, error: selErr } = await supabase.from('endpoints').select('id');
if (selErr) {
  console.error(selErr);
  process.exit(1);
}

const existing = new Set((existingRows || []).map((r) => r.id));
const toInsert = endpoints.filter((e) => e && e.id && !existing.has(e.id));

if (toInsert.length === 0) {
  console.log('No missing endpoints to insert.');
  process.exit(0);
}

const chunkSize = 80;
let inserted = 0;
for (let i = 0; i < toInsert.length; i += chunkSize) {
  const chunk = toInsert.slice(i, i + chunkSize);
  const { error } = await supabase.from('endpoints').upsert(chunk, { onConflict: 'id' });
  if (error) {
    console.error('Upsert failed at offset', i, error);
    process.exit(1);
  }
  inserted += chunk.length;
  console.log(`Inserted/updated chunk ${inserted}/${toInsert.length}`);
}

console.log(`Done. Restored ${inserted} endpoint row(s) from backup.`);
