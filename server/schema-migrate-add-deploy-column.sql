-- Deploy 컬럼 추가 마이그레이션
-- 기존 데이터베이스에 deploy 컬럼을 추가하는 스크립트
-- Supabase SQL Editor에서 실행하세요

-- 1. manager_tasks 테이블에 deploy 컬럼 추가
ALTER TABLE manager_tasks ADD COLUMN IF NOT EXISTS deploy TEXT DEFAULT 'empty';

-- 2. manager_columns 테이블에 deploy 컬럼 정보 추가
INSERT INTO manager_columns (id, label, visible, order_index)
VALUES ('deploy', 'Deploy', true, 14)
ON CONFLICT (id) DO NOTHING;

-- 3. mode, plan 컬럼이 없을 경우 추가 (이전 마이그레이션 누락 시)
ALTER TABLE manager_tasks ADD COLUMN IF NOT EXISTS mode TEXT;
ALTER TABLE manager_tasks ADD COLUMN IF NOT EXISTS plan TEXT DEFAULT 'empty';

-- 4. mode, plan 컬럼 메타데이터 추가
INSERT INTO manager_columns (id, label, visible, order_index)
VALUES 
  ('mode', 'mode', true, 9),
  ('plan', 'Plan', true, 10)
ON CONFLICT (id) DO NOTHING;

-- 5. 기존 order_index 조정 (전체 컬럼 순서 재정렬)
UPDATE manager_columns SET order_index = 0 WHERE id = 'product';
UPDATE manager_columns SET order_index = 1 WHERE id = 'tab';
UPDATE manager_columns SET order_index = 2 WHERE id = 'group';
UPDATE manager_columns SET order_index = 3 WHERE id = 'sub1';
UPDATE manager_columns SET order_index = 4 WHERE id = 'sub2';
UPDATE manager_columns SET order_index = 5 WHERE id = 'sub3';
UPDATE manager_columns SET order_index = 6 WHERE id = 'seg1';
UPDATE manager_columns SET order_index = 7 WHERE id = 'seg2';
UPDATE manager_columns SET order_index = 8 WHERE id = 'endPoint';
UPDATE manager_columns SET order_index = 9 WHERE id = 'mode';
UPDATE manager_columns SET order_index = 10 WHERE id = 'plan';
UPDATE manager_columns SET order_index = 11 WHERE id = 'dev';
UPDATE manager_columns SET order_index = 12 WHERE id = 'vv';
UPDATE manager_columns SET order_index = 13 WHERE id = 'doc';
UPDATE manager_columns SET order_index = 14 WHERE id = 'deploy';
UPDATE manager_columns SET order_index = 15 WHERE id = 'issue';
UPDATE manager_columns SET order_index = 16 WHERE id = 'status';
UPDATE manager_columns SET order_index = 17 WHERE id = 'charge';
UPDATE manager_columns SET order_index = 18 WHERE id = 'remark';

-- 마이그레이션 완료 확인
SELECT id, label, order_index FROM manager_columns ORDER BY order_index;
