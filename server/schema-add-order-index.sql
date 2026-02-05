-- Manager Tasks 테이블에 order_index 컬럼 추가
-- 명확한 순서 관리를 위한 마이그레이션
-- 실행일: 2026-02-05

-- =====================================================
-- 1. order_index 컬럼 추가
-- =====================================================

-- order_index 컬럼 추가 (기본값 0)
ALTER TABLE manager_tasks 
ADD COLUMN IF NOT EXISTS order_index INTEGER DEFAULT 0;

-- =====================================================
-- 2. 기존 데이터에 순서 할당 (created_at 기준)
-- =====================================================

-- created_at 순서대로 order_index 할당
WITH ordered_tasks AS (
  SELECT id, ROW_NUMBER() OVER (ORDER BY created_at ASC) - 1 AS new_order
  FROM manager_tasks
)
UPDATE manager_tasks
SET order_index = ordered_tasks.new_order
FROM ordered_tasks
WHERE manager_tasks.id = ordered_tasks.id;

-- =====================================================
-- 3. 인덱스 생성
-- =====================================================

CREATE INDEX IF NOT EXISTS idx_manager_tasks_order ON manager_tasks(order_index);

-- =====================================================
-- 4. 확인 쿼리
-- =====================================================

-- order_index 확인
SELECT id, product, tab, "group", order_index, created_at
FROM manager_tasks
ORDER BY order_index ASC
LIMIT 20;

-- 통계 확인
SELECT 
  COUNT(*) as total_tasks,
  MIN(order_index) as min_order,
  MAX(order_index) as max_order,
  COUNT(DISTINCT order_index) as unique_orders
FROM manager_tasks;
