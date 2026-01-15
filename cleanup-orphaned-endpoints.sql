-- ============================================================================
-- 고아 엔드포인트 정리 (Orphaned Endpoints Cleanup)
-- ============================================================================
-- 
-- 문제: products 테이블에 없는 제품의 엔드포인트들이 남아있음
-- 해결: 해당 엔드포인트들을 삭제
--

-- 1. 고아 엔드포인트 확인 (실행 전 확인)
SELECT 
  e.id,
  e.name,
  e.product,
  e.group_name,
  e.path
FROM endpoints e
WHERE e.product NOT IN (SELECT id FROM products)
  AND e.product IS NOT NULL
ORDER BY e.product, e.group_name;

-- 2. gen-nx 제품의 모든 엔드포인트 삭제
DELETE FROM endpoints 
WHERE product = 'gen-nx';

-- 3. 모든 고아 엔드포인트 삭제 (조심!)
-- 아래 주석을 해제하면 products 테이블에 없는 모든 제품의 엔드포인트를 삭제합니다
-- DELETE FROM endpoints 
-- WHERE product NOT IN (SELECT id FROM products)
--   AND product IS NOT NULL;

-- 4. 확인: 남은 엔드포인트 목록
SELECT 
  p.name as product_name,
  e.product,
  e.group_name,
  COUNT(*) as endpoint_count
FROM endpoints e
LEFT JOIN products p ON p.id = e.product
GROUP BY p.name, e.product, e.group_name
ORDER BY p.name, e.group_name;



