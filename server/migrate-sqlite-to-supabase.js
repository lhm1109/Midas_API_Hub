import Database from 'better-sqlite3';
import { createClient } from '@supabase/supabase-js';
import dotenv from 'dotenv';
import { join, dirname } from 'path';
import { fileURLToPath } from 'url';

dotenv.config();

const __filename = fileURLToPath(import.meta.url);
const __dirname = dirname(__filename);

// SQLite 데이터베이스 연결
const sqliteDb = new Database(join(__dirname, 'api-verification.db'));

// Supabase 클라이언트 초기화
const supabaseUrl = process.env.SUPABASE_URL;
const supabaseKey = process.env.SUPABASE_SERVICE_KEY;

if (!supabaseUrl || !supabaseKey) {
  console.error('❌ Supabase credentials not found in .env file');
  process.exit(1);
}

const supabase = createClient(supabaseUrl, supabaseKey);

async function migrateTable(tableName, transform = (row) => row) {
  console.log(`\n📋 Migrating table: ${tableName}`);
  
  try {
    // SQLite에서 데이터 읽기
    const rows = sqliteDb.prepare(`SELECT * FROM ${tableName}`).all();
    console.log(`  Found ${rows.length} rows in SQLite`);
    
    if (rows.length === 0) {
      console.log(`  ⏭️  Skipping empty table`);
      return;
    }
    
    // 데이터 변환
    const transformedRows = rows.map(transform);
    
    // Supabase에 배치 삽입 (upsert)
    const { data, error } = await supabase
      .from(tableName)
      .upsert(transformedRows, { onConflict: 'id' });
    
    if (error) {
      console.error(`  ❌ Error migrating ${tableName}:`, error.message);
      throw error;
    }
    
    console.log(`  ✅ Successfully migrated ${rows.length} rows`);
  } catch (error) {
    console.error(`  ❌ Failed to migrate ${tableName}:`, error);
    throw error;
  }
}

async function migrate() {
  console.log('🚀 Starting SQLite to Supabase migration...\n');
  console.log(`📡 Target: ${supabaseUrl}`);
  
  try {
    // 1. Endpoints
    await migrateTable('endpoints');
    
    // 2. Versions
    await migrateTable('versions');
    
    // 3. Manual Data
    await migrateTable('manual_data', (row) => ({
      version_id: row.version_id,
      title: row.title,
      category: row.category,
      input_uri: row.input_uri,
      active_methods: row.active_methods,
      json_schema: row.json_schema,
      json_schema_original: row.json_schema_original,
      json_schema_enhanced: row.json_schema_enhanced,
      examples: row.examples,
      request_examples: row.request_examples,
      response_examples: row.response_examples,
      specifications: row.specifications,
      html_content: row.html_content,
      article_id: row.article_id,
      section_id: row.section_id,
      author_id: row.author_id,
      url: row.url
    }));
    
    // 4. Spec Data
    await migrateTable('spec_data', (row) => ({
      version_id: row.version_id,
      json_schema: row.json_schema,
      json_schema_original: row.json_schema_original,
      json_schema_enhanced: row.json_schema_enhanced,
      specifications: row.specifications
    }));
    
    // 5. Builder Data
    await migrateTable('builder_data', (row) => ({
      version_id: row.version_id,
      form_data: row.form_data
    }));
    
    // 6. Runner Data
    await migrateTable('runner_data', (row) => ({
      version_id: row.version_id,
      request_body: row.request_body,
      response_body: row.response_body
    }));
    
    // 7. Test Cases
    await migrateTable('test_cases');
    
    // 8. Attachments (메타데이터만 - 파일은 별도 처리 필요)
    console.log(`\n📋 Migrating table: attachments`);
    console.log(`  ⚠️  Note: This only migrates metadata. Files need to be uploaded to Supabase Storage manually.`);
    await migrateTable('attachments', (row) => ({
      id: row.id,
      version_id: row.version_id,
      file_name: row.file_name,
      file_size: row.file_size,
      file_path: row.file_path,
      mime_type: row.mime_type,
      uploaded_at: row.uploaded_at
    }));
    
    console.log('\n✅ Migration completed successfully!');
    console.log('\n⚠️  Important:');
    console.log('  1. Attachment files need to be uploaded to Supabase Storage manually');
    console.log('  2. Create a bucket named "api-verification" in Supabase Storage');
    console.log('  3. Upload files from server/uploads to the bucket');
    console.log('\n📖 Next steps:');
    console.log('  1. Stop the server');
    console.log('  2. Update .env with Supabase credentials');
    console.log('  3. Restart the server: npm run dev:all');
    
  } catch (error) {
    console.error('\n❌ Migration failed:', error);
    process.exit(1);
  } finally {
    sqliteDb.close();
  }
}

// 실행
migrate();

