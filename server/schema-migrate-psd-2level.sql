-- PSD 2-Level Structure Migration
-- Products 테이블에 psd_set, schema_type 필드 추가

-- 1. 기존 schema_definition 필드를 psd_set으로 변경
ALTER TABLE products 
RENAME COLUMN schema_definition TO psd_set;

-- 2. schema_type 필드 추가 (기본값: enhanced)
ALTER TABLE products 
ADD COLUMN schema_type TEXT DEFAULT 'enhanced';

-- 3. 기존 데이터 마이그레이션
-- schema_definition이 'enhanced', 'original', 'manual'이었던 경우
-- → psd_set='default', schema_type='enhanced/original/manual'로 변경

-- psd_set을 'default'로 설정 (기존 모든 제품)
UPDATE products 
SET psd_set = 'default' 
WHERE psd_set IN ('enhanced', 'original', 'manual') OR psd_set IS NULL;

-- schema_type은 이미 enhanced가 기본값이므로 별도 업데이트 불필요

-- 4. 인덱스 추가
CREATE INDEX IF NOT EXISTS idx_products_psd_set ON products(psd_set);
CREATE INDEX IF NOT EXISTS idx_products_schema_type ON products(schema_type);

-- 5. 확인
SELECT id, name, psd_set, schema_type FROM products;



