-- Manager plan 컬럼 추가 마이그레이션
-- 실행 시간: 2026-01-28

-- 1. plan 컬럼 추가 (dev 앞에 위치)
ALTER TABLE manager_tasks ADD COLUMN IF NOT EXISTS plan TEXT;

-- 2. manager_columns에 plan 컬럼 추가
INSERT INTO manager_columns (id, label, visible, order_index) 
VALUES ('plan', 'Plan', true, 9)
ON CONFLICT (id) DO NOTHING;

-- 3. 기존 컬럼들의 order_index 업데이트 (plan이 dev 앞에 오도록)
UPDATE manager_columns SET order_index = 10 WHERE id = 'dev';
UPDATE manager_columns SET order_index = 11 WHERE id = 'vv';
UPDATE manager_columns SET order_index = 12 WHERE id = 'doc';
UPDATE manager_columns SET order_index = 13 WHERE id = 'issue';
UPDATE manager_columns SET order_index = 14 WHERE id = 'status';
UPDATE manager_columns SET order_index = 15 WHERE id = 'charge';
UPDATE manager_columns SET order_index = 16 WHERE id = 'remark';

-- 4. status 값 통일: 'Working' -> 'In Progress'
UPDATE manager_tasks SET status = 'In Progress' WHERE status = 'Working';

-- 5. 상태값 통일: green/yellow/red를 done/wip/warning으로 변환
-- dev 컬럼
UPDATE manager_tasks SET dev = 'done' WHERE dev = 'green';
UPDATE manager_tasks SET dev = 'wip' WHERE dev = 'yellow';
UPDATE manager_tasks SET dev = 'warning' WHERE dev = 'red';

-- vv 컬럼
UPDATE manager_tasks SET vv = 'done' WHERE vv = 'green';
UPDATE manager_tasks SET vv = 'wip' WHERE vv = 'yellow';
UPDATE manager_tasks SET vv = 'warning' WHERE vv = 'red';

-- doc 컬럼
UPDATE manager_tasks SET doc = 'done' WHERE doc = 'green';
UPDATE manager_tasks SET doc = 'wip' WHERE doc = 'yellow';
UPDATE manager_tasks SET doc = 'warning' WHERE doc = 'red';

-- issue 컬럼
UPDATE manager_tasks SET issue = 'done' WHERE issue = 'green';
UPDATE manager_tasks SET issue = 'wip' WHERE issue = 'yellow';
UPDATE manager_tasks SET issue = 'warning' WHERE issue = 'red';

-- plan 컬럼에 대해서도 동일하게 (나중에 추가될 데이터를 위해)
-- 기본값은 empty로 설정
UPDATE manager_tasks SET plan = 'empty' WHERE plan IS NULL;
