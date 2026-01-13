-- ============================================================================
-- Products & Groups 테이블 추가
-- ============================================================================

-- Products 테이블
CREATE TABLE IF NOT EXISTS products (
  id TEXT PRIMARY KEY,
  name TEXT NOT NULL,
  description TEXT,
  order_index INTEGER DEFAULT 0,
  created_at TIMESTAMP NOT NULL DEFAULT NOW(),
  updated_at TIMESTAMP NOT NULL DEFAULT NOW()
);

-- Groups 테이블
CREATE TABLE IF NOT EXISTS groups (
  id TEXT PRIMARY KEY,
  product_id TEXT NOT NULL REFERENCES products(id) ON DELETE CASCADE,
  name TEXT NOT NULL,
  description TEXT,
  order_index INTEGER DEFAULT 0,
  created_at TIMESTAMP NOT NULL DEFAULT NOW(),
  updated_at TIMESTAMP NOT NULL DEFAULT NOW()
);

-- Endpoints 테이블에 product_id, group_id 컬럼 추가 (기존 product, group_name은 유지)
DO $$ 
BEGIN
  -- product_id 추가
  IF NOT EXISTS (
    SELECT 1 FROM information_schema.columns 
    WHERE table_name = 'endpoints' AND column_name = 'product_id'
  ) THEN
    ALTER TABLE endpoints ADD COLUMN product_id TEXT REFERENCES products(id) ON DELETE CASCADE;
  END IF;
  
  -- group_id 추가
  IF NOT EXISTS (
    SELECT 1 FROM information_schema.columns 
    WHERE table_name = 'endpoints' AND column_name = 'group_id'
  ) THEN
    ALTER TABLE endpoints ADD COLUMN group_id TEXT REFERENCES groups(id) ON DELETE CASCADE;
  END IF;
END $$;

-- 인덱스 생성
CREATE INDEX IF NOT EXISTS idx_products_order ON products(order_index);
CREATE INDEX IF NOT EXISTS idx_groups_product ON groups(product_id);
CREATE INDEX IF NOT EXISTS idx_groups_order ON groups(order_index);
CREATE INDEX IF NOT EXISTS idx_endpoints_product ON endpoints(product_id);
CREATE INDEX IF NOT EXISTS idx_endpoints_group ON endpoints(group_id);

-- 기존 데이터 마이그레이션 (endpoints 테이블의 product, group_name을 기반으로)
-- 1. 기존 product를 products 테이블에 삽입
INSERT INTO products (id, name, order_index, created_at, updated_at)
SELECT DISTINCT 
  product as id,
  product as name,
  0 as order_index,
  MIN(created_at) as created_at,
  MAX(updated_at) as updated_at
FROM endpoints
WHERE product IS NOT NULL 
  AND product != ''
  AND NOT EXISTS (SELECT 1 FROM products WHERE products.id = endpoints.product)
GROUP BY product;

-- 2. 기존 group_name을 groups 테이블에 삽입
INSERT INTO groups (id, product_id, name, order_index, created_at, updated_at)
SELECT DISTINCT
  product || '_' || group_name as id,
  product as product_id,
  group_name as name,
  0 as order_index,
  MIN(created_at) as created_at,
  MAX(updated_at) as updated_at
FROM endpoints
WHERE product IS NOT NULL 
  AND group_name IS NOT NULL
  AND product != ''
  AND group_name != ''
  AND NOT EXISTS (SELECT 1 FROM groups WHERE groups.id = endpoints.product || '_' || endpoints.group_name)
GROUP BY product, group_name;

-- 3. endpoints의 product_id, group_id 업데이트
UPDATE endpoints
SET 
  product_id = product,
  group_id = product || '_' || group_name
WHERE product IS NOT NULL 
  AND group_name IS NOT NULL
  AND (product_id IS NULL OR group_id IS NULL);

-- placeholder 엔드포인트 삭제 (더 이상 필요 없음)
DELETE FROM endpoints WHERE name = '_placeholder';

-- 테이블 설명
COMMENT ON TABLE products IS 'API 제품 목록';
COMMENT ON TABLE groups IS 'API 그룹 목록 (각 제품 내)';
COMMENT ON TABLE endpoints IS 'API 엔드포인트 목록';
