-- ============================================================================
-- Supabase용 스키마 (편집 잠금 시스템 포함)
-- ============================================================================

-- 📌 편집 잠금 테이블 (새로 추가)
CREATE TABLE IF NOT EXISTS endpoint_locks (
  endpoint_id TEXT PRIMARY KEY,         -- 잠긴 엔드포인트 ID
  locked_by TEXT NOT NULL,              -- 잠금한 사용자 (이메일 또는 ID)
  locked_at TIMESTAMP NOT NULL DEFAULT NOW(),
  expires_at TIMESTAMP NOT NULL,        -- 자동 해제 시간 (5분 후)
  last_activity TIMESTAMP NOT NULL DEFAULT NOW()
);

-- 📌 버전 편집 잠금 테이블 (버전별 세밀한 제어)
CREATE TABLE IF NOT EXISTS version_locks (
  version_id TEXT PRIMARY KEY,          -- 잠긴 버전 ID
  locked_by TEXT NOT NULL,              -- 잠금한 사용자
  locked_at TIMESTAMP NOT NULL DEFAULT NOW(),
  expires_at TIMESTAMP NOT NULL,
  last_activity TIMESTAMP NOT NULL DEFAULT NOW(),
  FOREIGN KEY (version_id) REFERENCES versions(id) ON DELETE CASCADE
);

-- 📌 사용자 활동 로그 (누가 언제 무엇을 편집했는지)
CREATE TABLE IF NOT EXISTS edit_history (
  id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
  endpoint_id TEXT,
  version_id TEXT,
  user_id TEXT NOT NULL,
  action TEXT NOT NULL,                 -- 'lock', 'unlock', 'edit', 'save'
  created_at TIMESTAMP NOT NULL DEFAULT NOW()
);

-- 인덱스
CREATE INDEX IF NOT EXISTS idx_endpoint_locks_expires ON endpoint_locks(expires_at);
CREATE INDEX IF NOT EXISTS idx_version_locks_expires ON version_locks(expires_at);
CREATE INDEX IF NOT EXISTS idx_edit_history_endpoint ON edit_history(endpoint_id);
CREATE INDEX IF NOT EXISTS idx_edit_history_user ON edit_history(user_id);

-- ============================================================================
-- 기존 테이블들 (Supabase용으로 수정)
-- ============================================================================

CREATE TABLE IF NOT EXISTS endpoints (
  id TEXT PRIMARY KEY,
  name TEXT NOT NULL,
  method TEXT NOT NULL,
  path TEXT NOT NULL,
  product TEXT NOT NULL,
  group_name TEXT NOT NULL,
  description TEXT,
  status TEXT,
  created_at TIMESTAMP NOT NULL DEFAULT NOW(),
  updated_at TIMESTAMP NOT NULL DEFAULT NOW()
);

CREATE TABLE IF NOT EXISTS versions (
  id TEXT PRIMARY KEY,
  version TEXT NOT NULL,
  endpoint_id TEXT NOT NULL,
  created_at TIMESTAMP NOT NULL DEFAULT NOW(),
  updated_at TIMESTAMP NOT NULL DEFAULT NOW(),
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
  response_body TEXT,        -- 🔥 Response 저장
  response_status INTEGER,   -- 🔥 HTTP 상태 코드
  response_time INTEGER,     -- 🔥 응답 시간 (ms)
  created_at TIMESTAMP NOT NULL DEFAULT NOW(),
  updated_at TIMESTAMP NOT NULL DEFAULT NOW(),
  FOREIGN KEY (version_id) REFERENCES versions(id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS attachments (
  id TEXT PRIMARY KEY,
  version_id TEXT NOT NULL,
  file_name TEXT NOT NULL,
  file_size INTEGER NOT NULL,
  file_path TEXT NOT NULL,
  mime_type TEXT,
  uploaded_at TIMESTAMP NOT NULL DEFAULT NOW(),
  FOREIGN KEY (version_id) REFERENCES versions(id) ON DELETE CASCADE
);

-- 인덱스
CREATE INDEX IF NOT EXISTS idx_versions_endpoint ON versions(endpoint_id);
CREATE INDEX IF NOT EXISTS idx_test_cases_version ON test_cases(version_id);
CREATE INDEX IF NOT EXISTS idx_attachments_version ON attachments(version_id);

-- ============================================================================
-- Supabase Realtime 활성화
-- ============================================================================

-- 편집 잠금 테이블에 대한 실시간 구독 활성화
ALTER PUBLICATION supabase_realtime ADD TABLE endpoint_locks;
ALTER PUBLICATION supabase_realtime ADD TABLE version_locks;

-- ============================================================================
-- 자동 잠금 해제 함수 (만료된 잠금 자동 삭제)
-- ============================================================================

CREATE OR REPLACE FUNCTION cleanup_expired_locks()
RETURNS void AS $$
BEGIN
  DELETE FROM endpoint_locks WHERE expires_at < NOW();
  DELETE FROM version_locks WHERE expires_at < NOW();
END;
$$ LANGUAGE plpgsql;

-- 1분마다 자동 실행 (pg_cron 확장 필요)
-- SELECT cron.schedule('cleanup-locks', '* * * * *', 'SELECT cleanup_expired_locks()');

