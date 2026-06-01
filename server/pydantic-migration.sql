-- ============================================================================
-- Pydantic 코드 저장 마이그레이션
-- spec_data 테이블에 pydantic_code, pydantic_generated_at 컬럼 추가
--
-- 실행 방법:
--   Supabase Dashboard → SQL Editor → 이 파일 내용 붙여넣기 → Run
--
-- 안전성:
--   - IF NOT EXISTS / ADD COLUMN IF NOT EXISTS 로 중복 실행 무해
--   - 기존 컬럼(json_schema, json_schema_enhanced 등) 및 데이터 일절 변경 없음
-- ============================================================================

ALTER TABLE spec_data
  ADD COLUMN IF NOT EXISTS pydantic_code         TEXT,
  ADD COLUMN IF NOT EXISTS pydantic_generated_at TIMESTAMPTZ;
