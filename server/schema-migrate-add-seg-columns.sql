-- seg3, seg4 컬럼 추가 마이그레이션
-- 실행 방법: Supabase Dashboard > SQL Editor에서 실행

-- 1. manager_tasks 테이블에 seg3, seg4 컬럼 추가
ALTER TABLE manager_tasks ADD COLUMN IF NOT EXISTS seg3 TEXT;
ALTER TABLE manager_tasks ADD COLUMN IF NOT EXISTS seg4 TEXT;

-- 2. manager_columns 테이블에 seg3, seg4 컬럼 설정 추가
INSERT INTO manager_columns (id, label, visible, order_index) VALUES
  ('seg3', 'seg3', true, 8),
  ('seg4', 'seg4', true, 9)
ON CONFLICT (id) DO NOTHING;

-- 3. 기존 컬럼들의 order_index 조정 (seg3, seg4가 추가되어 2씩 증가)
UPDATE manager_columns SET order_index = 10 WHERE id = 'endPoint';
UPDATE manager_columns SET order_index = 11 WHERE id = 'mode';
UPDATE manager_columns SET order_index = 12 WHERE id = 'plan';
UPDATE manager_columns SET order_index = 13 WHERE id = 'dev';
UPDATE manager_columns SET order_index = 14 WHERE id = 'vv';
UPDATE manager_columns SET order_index = 15 WHERE id = 'doc';
UPDATE manager_columns SET order_index = 16 WHERE id = 'deploy';
UPDATE manager_columns SET order_index = 17 WHERE id = 'issue';
UPDATE manager_columns SET order_index = 18 WHERE id = 'status';
UPDATE manager_columns SET order_index = 19 WHERE id = 'charge';
UPDATE manager_columns SET order_index = 20 WHERE id = 'remark';

-- 완료 메시지
SELECT 'seg3, seg4 컬럼 마이그레이션 완료!' as message;
