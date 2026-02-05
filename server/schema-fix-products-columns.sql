-- ============================================================================
-- Fix Products Table - Add psd_set and schema_type columns
-- ============================================================================

-- Add psd_set column
DO $$ 
BEGIN
  IF NOT EXISTS (
    SELECT 1 FROM information_schema.columns 
    WHERE table_name = 'products' AND column_name = 'psd_set'
  ) THEN
    ALTER TABLE products 
    ADD COLUMN psd_set TEXT DEFAULT 'default';
    
    COMMENT ON COLUMN products.psd_set IS 'Product Schema Definition set name';
  END IF;
END $$;

-- Add schema_type column
DO $$ 
BEGIN
  IF NOT EXISTS (
    SELECT 1 FROM information_schema.columns 
    WHERE table_name = 'products' AND column_name = 'schema_type'
  ) THEN
    ALTER TABLE products 
    ADD COLUMN schema_type TEXT DEFAULT 'enhanced';
    
    COMMENT ON COLUMN products.schema_type IS 'Schema type (enhanced, basic, etc.)';
  END IF;
END $$;

-- Update existing products with default values
UPDATE products 
SET psd_set = 'default', schema_type = 'enhanced'
WHERE psd_set IS NULL OR schema_type IS NULL;

-- Create indexes for better query performance
CREATE INDEX IF NOT EXISTS idx_products_psd_set ON products(psd_set);
CREATE INDEX IF NOT EXISTS idx_products_schema_type ON products(schema_type);
