import Database from 'better-sqlite3';
import { createClient } from '@supabase/supabase-js';
import { fileURLToPath } from 'url';
import { dirname, join } from 'path';

const __filename = fileURLToPath(import.meta.url);
const __dirname = dirname(__filename);

// Supabase 클라이언트 초기화
const supabaseUrl = process.env.SUPABASE_URL;
const supabaseKey = process.env.SUPABASE_SERVICE_ROLE_KEY;

if (!supabaseUrl || !supabaseKey) {
  console.error('❌ SUPABASE_URL and SUPABASE_SERVICE_ROLE_KEY must be set in environment variables');
  process.exit(1);
}

const supabase = createClient(supabaseUrl, supabaseKey);

// SQLite DB 연결
const sqliteDb = new Database(join(__dirname, 'api-verification.db'));

console.log('🚀 Starting migration from SQLite to Supabase...\n');

async function migrateTable(tableName, transform = (row) => row) {
  console.log(`📊 Migrating table: ${tableName}`);
  
  try {
    // SQLite에서 데이터 읽기
    const rows = sqliteDb.prepare(`SELECT * FROM ${tableName}`).all();
    console.log(`   Found ${rows.length} rows`);
    
    if (rows.length === 0) {
      console.log(`   ⏭️  Skipping (no data)\n`);
      return;
    }
    
    // 데이터 변환
    const transformedRows = rows.map(transform);
    
    // Supabase에 삽입 (배치 처리)
    const batchSize = 100;
    for (let i = 0; i < transformedRows.length; i += batchSize) {
      const batch = transformedRows.slice(i, i + batchSize);
      const { error } = await supabase
        .from(tableName)
        .insert(batch);
      
      if (error) {
        console.error(`   ❌ Error inserting batch ${i}-${i + batch.length}:`, error);
      } else {
        console.log(`   ✅ Inserted ${batch.length} rows`);
      }
    }
    
    console.log(`   ✅ Migration complete\n`);
  } catch (error) {
    console.error(`   ❌ Error migrating ${tableName}:`, error.message, '\n');
  }
}

async function migrate() {
  try {
    // 1. Endpoints
    await migrateTable('endpoints', (row) => ({
      ...row,
      created_at: new Date(row.created_at).toISOString(),
      updated_at: new Date(row.updated_at).toISOString(),
    }));
    
    // 2. Versions
    await migrateTable('versions', (row) => ({
      ...row,
      created_at: new Date(row.created_at).toISOString(),
      updated_at: new Date(row.updated_at).toISOString(),
    }));
    
    // 3. Manual Data
    await migrateTable('manual_data');
    
    // 4. Spec Data
    await migrateTable('spec_data');
    
    // 5. Builder Data
    await migrateTable('builder_data');
    
    // 6. Runner Data
    await migrateTable('runner_data');
    
    // 7. Test Cases
    await migrateTable('test_cases', (row) => ({
      ...row,
      created_at: new Date(row.created_at).toISOString(),
      updated_at: new Date(row.updated_at).toISOString(),
    }));
    
    // 8. Attachments (메타데이터만, 실제 파일은 Storage로 업로드 필요)
    await migrateTable('attachments', (row) => ({
      ...row,
      uploaded_at: new Date(row.uploaded_at).toISOString(),
    }));
    
    console.log('✅ All tables migrated successfully!');
    console.log('\n📝 Next steps:');
    console.log('   1. Upload files from server/uploads/ to Supabase Storage');
    console.log('   2. Update file_path in attachments table to point to Storage URLs');
    console.log('   3. Test the application with Supabase');
    
  } catch (error) {
    console.error('❌ Migration failed:', error);
  } finally {
    sqliteDb.close();
  }
}

migrate();

