-- ============================================================================
-- Product Schema Definition (PSD) 추가
-- Products 테이블에 schema_definition 컬럼 추가
-- ============================================================================

-- Products 테이블에 schema_definition 컬럼 추가
DO $$ 
BEGIN
  IF NOT EXISTS (
    SELECT 1 FROM information_schema.columns 
    WHERE table_name = 'products' AND column_name = 'schema_definition'
  ) THEN
    ALTER TABLE products 
    ADD COLUMN schema_definition TEXT DEFAULT 'enhanced';
    
    COMMENT ON COLUMN products.schema_definition IS 'Schema definition set name (folder name in schema_definitions/)';
  END IF;
END $$;

-- 기존 제품들의 schema_definition 기본값 설정
UPDATE products 
SET schema_definition = 'enhanced' 
WHERE schema_definition IS NULL;

-- 인덱스 생성 (선택적 조회를 위해)
CREATE INDEX IF NOT EXISTS idx_products_schema_definition ON products(schema_definition);

