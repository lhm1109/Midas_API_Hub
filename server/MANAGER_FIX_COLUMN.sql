-- 컬럼명 수정: end_point -> endPoint

-- 기존 테이블 삭제 (데이터가 삭제됩니다!)
DROP TABLE IF EXISTS manager_tasks;

-- 새로운 스키마로 재생성
CREATE TABLE manager_tasks (
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
  issue TEXT,
  status TEXT,
  charge TEXT,
  remark TEXT,
  created_at TIMESTAMPTZ DEFAULT NOW(),
  updated_at TIMESTAMPTZ DEFAULT NOW()
);

-- 인덱스 재생성
CREATE INDEX idx_manager_tasks_created_at ON manager_tasks(created_at);
CREATE INDEX idx_manager_tasks_status ON manager_tasks(status);
