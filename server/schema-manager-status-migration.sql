-- Manager 테이블 상태 값 마이그레이션
-- wip → progress, 대문자 → 소문자 변환
-- 실행일: 2026-02-05
-- 목적: 상태 값 소문자 통일 (empty, progress, done, warning)

-- =====================================================
-- 1. Progress 컬럼들 (plan, dev, vv, doc, deploy, issue) 마이그레이션
-- =====================================================

-- wip → progress 변환
UPDATE manager_tasks 
SET plan = 'progress' 
WHERE plan = 'wip';

UPDATE manager_tasks 
SET dev = 'progress' 
WHERE dev = 'wip';

UPDATE manager_tasks 
SET vv = 'progress' 
WHERE vv = 'wip';

UPDATE manager_tasks 
SET doc = 'progress' 
WHERE doc = 'wip';

UPDATE manager_tasks 
SET deploy = 'progress' 
WHERE deploy = 'wip';

UPDATE manager_tasks 
SET issue = 'progress' 
WHERE issue = 'wip';

-- na 제거 (비어있음으로 처리)
UPDATE manager_tasks 
SET plan = 'empty' 
WHERE plan = 'na';

UPDATE manager_tasks 
SET dev = 'empty' 
WHERE dev = 'na';

UPDATE manager_tasks 
SET vv = 'empty' 
WHERE vv = 'na';

UPDATE manager_tasks 
SET doc = 'empty' 
WHERE doc = 'na';

UPDATE manager_tasks 
SET deploy = 'empty' 
WHERE deploy = 'na';

UPDATE manager_tasks 
SET issue = 'empty' 
WHERE issue = 'na';

-- =====================================================
-- 2. Status 컬럼 마이그레이션 (대문자 → 소문자)
-- =====================================================

-- In Progress → progress
UPDATE manager_tasks 
SET status = 'progress' 
WHERE status = 'In Progress' OR status = 'in progress' OR status = 'IN PROGRESS';

-- Done → done
UPDATE manager_tasks 
SET status = 'done' 
WHERE status = 'Done' OR status = 'DONE';

-- Cancel → cancel
UPDATE manager_tasks 
SET status = 'Cancel' 
WHERE status = 'cancel';

UPDATE manager_tasks 
SET status = 'cancel' 
WHERE status = 'Cancel' OR status = 'CANCEL';

-- Working → working
UPDATE manager_tasks 
SET status = 'working' 
WHERE status = 'Working' OR status = 'WORKING';

-- =====================================================
-- 3. 확인 쿼리
-- =====================================================

-- Progress 컬럼 상태 확인
SELECT 
  COUNT(*) as total,
  SUM(CASE WHEN plan IN ('empty', 'progress', 'done', 'warning') THEN 1 ELSE 0 END) as plan_valid,
  SUM(CASE WHEN dev IN ('empty', 'progress', 'done', 'warning') THEN 1 ELSE 0 END) as dev_valid,
  SUM(CASE WHEN vv IN ('empty', 'progress', 'done', 'warning') THEN 1 ELSE 0 END) as vv_valid,
  SUM(CASE WHEN doc IN ('empty', 'progress', 'done', 'warning') THEN 1 ELSE 0 END) as doc_valid,
  SUM(CASE WHEN deploy IN ('empty', 'progress', 'done', 'warning') THEN 1 ELSE 0 END) as deploy_valid,
  SUM(CASE WHEN issue IN ('empty', 'progress', 'done', 'warning') THEN 1 ELSE 0 END) as issue_valid
FROM manager_tasks;

-- Status 컬럼 분포 확인
SELECT status, COUNT(*) as count
FROM manager_tasks
GROUP BY status
ORDER BY count DESC;
