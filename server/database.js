import Database from 'better-sqlite3';
import { fileURLToPath } from 'url';
import { dirname, join } from 'path';

const __filename = fileURLToPath(import.meta.url);
const __dirname = dirname(__filename);

// SQLite 데이터베이스 초기화
const db = new Database(join(__dirname, 'api-verification.db'));

export function initDb() {
  // 테이블 생성
  db.exec(`
    -- 엔드포인트 정의 테이블 (계층 구조 지원)
    CREATE TABLE IF NOT EXISTS endpoints (
      id TEXT PRIMARY KEY,              -- 예: "db/nlct", "gen/project"
      name TEXT NOT NULL,               -- 표시 이름: "NLCT", "Project"
      method TEXT NOT NULL,             -- HTTP 메서드: GET, POST, etc.
      path TEXT NOT NULL,               -- API 경로: "/db/nlct"
      product TEXT NOT NULL,            -- 제품: "civil-nx", "gen-nx"
      group_name TEXT NOT NULL,         -- 그룹: "DB", "Gen"
      description TEXT,                 -- 설명
      status TEXT,                      -- 상태: "active", "deprecated"
      created_at TEXT NOT NULL,
      updated_at TEXT NOT NULL
    );

    CREATE TABLE IF NOT EXISTS versions (
      id TEXT PRIMARY KEY,
      version TEXT NOT NULL,
      endpoint_id TEXT NOT NULL,        -- 예: "db/nlct" (계층 구조)
      created_at TEXT NOT NULL,
      updated_at TEXT NOT NULL,
      author TEXT,
      change_log TEXT,
      FOREIGN KEY (endpoint_id) REFERENCES endpoints(id)
    );

    CREATE TABLE IF NOT EXISTS manual_data (
      version_id TEXT PRIMARY KEY,
      title TEXT,
      category TEXT,
      input_uri TEXT,
      active_methods TEXT,
      json_schema TEXT,
      json_schema_original TEXT,
      json_schema_enhanced TEXT,
      examples TEXT,
      request_examples TEXT,
      response_examples TEXT,
      specifications TEXT,
      html_content TEXT,
      article_id TEXT,
      section_id TEXT,
      author_id TEXT,
      url TEXT,
      FOREIGN KEY (version_id) REFERENCES versions(id) ON DELETE CASCADE
    );

    CREATE TABLE IF NOT EXISTS spec_data (
      version_id TEXT PRIMARY KEY,
      json_schema TEXT,
      json_schema_original TEXT,
      json_schema_enhanced TEXT,
      specifications TEXT,
      FOREIGN KEY (version_id) REFERENCES versions(id) ON DELETE CASCADE
    );

    CREATE TABLE IF NOT EXISTS builder_data (
      version_id TEXT PRIMARY KEY,
      form_data TEXT,
      FOREIGN KEY (version_id) REFERENCES versions(id) ON DELETE CASCADE
    );

    CREATE TABLE IF NOT EXISTS runner_data (
      version_id TEXT PRIMARY KEY,
      request_body TEXT,
      response_body TEXT,
      FOREIGN KEY (version_id) REFERENCES versions(id) ON DELETE CASCADE
    );

    CREATE TABLE IF NOT EXISTS test_cases (
      id TEXT PRIMARY KEY,
      version_id TEXT NOT NULL,
      name TEXT NOT NULL,
      description TEXT,
      request_body TEXT,
      created_at TEXT NOT NULL,
      updated_at TEXT NOT NULL,
      FOREIGN KEY (version_id) REFERENCES versions(id) ON DELETE CASCADE
    );

    CREATE TABLE IF NOT EXISTS attachments (
      id TEXT PRIMARY KEY,
      version_id TEXT NOT NULL,
      file_name TEXT NOT NULL,
      file_size INTEGER NOT NULL,
      file_path TEXT NOT NULL,
      mime_type TEXT,
      uploaded_at TEXT NOT NULL,
      FOREIGN KEY (version_id) REFERENCES versions(id) ON DELETE CASCADE
    );

    CREATE INDEX IF NOT EXISTS idx_versions_endpoint ON versions(endpoint_id);
    CREATE INDEX IF NOT EXISTS idx_test_cases_version ON test_cases(version_id);
    CREATE INDEX IF NOT EXISTS idx_attachments_version ON attachments(version_id);
  `);

  // 🔄 Migration: Add request_examples and response_examples columns if they don't exist
  try {
    const columns = db.pragma("table_info('manual_data')");
    const hasRequestExamples = columns.some(col => col.name === 'request_examples');
    const hasResponseExamples = columns.some(col => col.name === 'response_examples');
    
    if (!hasRequestExamples) {
      db.exec(`ALTER TABLE manual_data ADD COLUMN request_examples TEXT`);
      console.log('✅ Added request_examples column to manual_data table');
    }
    
    if (!hasResponseExamples) {
      db.exec(`ALTER TABLE manual_data ADD COLUMN response_examples TEXT`);
      console.log('✅ Added response_examples column to manual_data table');
    }
  } catch (error) {
    console.error('⚠️ Migration error:', error.message);
  }

  console.log('✅ Database initialized successfully');
}

export default db;
