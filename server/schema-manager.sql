-- Manager 기능을 위한 테이블 스키마
-- 프로젝트 관리 작업(tasks)과 컬럼 설정을 저장

-- manager_tasks 테이블: API 작업 관리
CREATE TABLE IF NOT EXISTS manager_tasks (
  id TEXT PRIMARY KEY,
  product TEXT,
  tab TEXT,
  "group" TEXT,
  sub1 TEXT,
  sub2 TEXT,
  sub3 TEXT,
  seg1 TEXT,
  seg2 TEXT,
  "endPoint" TEXT,
  mode TEXT,
  dev TEXT,
  vv TEXT,
  doc TEXT,
  deploy TEXT,
  issue TEXT,
  status TEXT,
  charge TEXT,
  remark TEXT,
  created_at TIMESTAMPTZ DEFAULT NOW(),
  updated_at TIMESTAMPTZ DEFAULT NOW()
);

-- manager_columns 테이블: 컬럼 설정 (표시/숨김, 순서 등)
CREATE TABLE IF NOT EXISTS manager_columns (
  id TEXT PRIMARY KEY,
  label TEXT NOT NULL,
  visible BOOLEAN DEFAULT TRUE,
  order_index INTEGER DEFAULT 0,
  updated_at TIMESTAMPTZ DEFAULT NOW()
);

-- 인덱스 생성
CREATE INDEX IF NOT EXISTS idx_manager_tasks_created_at ON manager_tasks(created_at);
CREATE INDEX IF NOT EXISTS idx_manager_tasks_status ON manager_tasks(status);
CREATE INDEX IF NOT EXISTS idx_manager_columns_order ON manager_columns(order_index);

-- RLS (Row Level Security) 활성화 (선택사항)
-- ALTER TABLE manager_tasks ENABLE ROW LEVEL SECURITY;
-- ALTER TABLE manager_columns ENABLE ROW LEVEL SECURITY;

-- 모든 사용자가 읽기/쓰기 가능하도록 설정 (선택사항)
-- CREATE POLICY "Enable all access for manager_tasks" ON manager_tasks FOR ALL USING (true);
-- CREATE POLICY "Enable all access for manager_columns" ON manager_columns FOR ALL USING (true);

-- 기본 컬럼 데이터 삽입
INSERT INTO manager_columns (id, label, visible, order_index) VALUES
  ('product', 'Product', true, 0),
  ('tab', 'Tab', true, 1),
  ('group', 'Group', true, 2),
  ('sub1', 'sub1', true, 3),
  ('sub2', 'sub2', true, 4),
  ('sub3', 'sub3', true, 5),
  ('seg1', 'seg1', true, 6),
  ('seg2', 'seg2', true, 7),
  ('endPoint', 'End Point', true, 8),
  ('mode', 'mode', true, 9),
  ('plan', 'Plan', true, 10),
  ('dev', 'Dev', true, 11),
  ('vv', 'V&V', true, 12),
  ('doc', 'doc.', true, 13),
  ('deploy', 'Deploy', true, 14),
  ('issue', 'Issue', true, 15),
  ('status', 'status', true, 16),
  ('charge', 'charge', true, 17),
  ('remark', 'remark', true, 18)
ON CONFLICT (id) DO NOTHING;
