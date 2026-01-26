-- mode 컬럼 추가 마이그레이션

-- 옵션 1: 기존 데이터 유지하면서 컬럼 추가
ALTER TABLE manager_tasks ADD COLUMN IF NOT EXISTS mode TEXT;

-- 기존 데이터에 기본값 설정 (선택사항)
UPDATE manager_tasks SET mode = '' WHERE mode IS NULL;

-- manager_columns 테이블에 mode 컬럼 설정 추가
INSERT INTO manager_columns (id, label, visible, order_index) 
VALUES ('mode', 'mode', true, 9)
ON CONFLICT (id) DO UPDATE SET 
  label = 'mode',
  visible = true,
  order_index = 9;

-- 기존 컬럼들의 order_index 조정 (mode 이후 컬럼들)
UPDATE manager_columns SET order_index = 10 WHERE id = 'dev';
UPDATE manager_columns SET order_index = 11 WHERE id = 'vv';
UPDATE manager_columns SET order_index = 12 WHERE id = 'doc';
UPDATE manager_columns SET order_index = 13 WHERE id = 'issue';
UPDATE manager_columns SET order_index = 14 WHERE id = 'status';
UPDATE manager_columns SET order_index = 15 WHERE id = 'charge';
UPDATE manager_columns SET order_index = 16 WHERE id = 'remark';
