-- ============================================================================
-- Migration: Add response columns to test_cases table
-- Date: 2026-02-03
-- Description: Test Case에 Response 저장을 위한 컬럼 추가
-- ============================================================================

-- 1. response_body 컬럼 추가 (JSON 형태의 응답 본문)
ALTER TABLE test_cases ADD COLUMN IF NOT EXISTS response_body TEXT;

-- 2. response_status 컬럼 추가 (HTTP 상태 코드)
ALTER TABLE test_cases ADD COLUMN IF NOT EXISTS response_status INTEGER;

-- 3. response_time 컬럼 추가 (응답 시간 ms)
ALTER TABLE test_cases ADD COLUMN IF NOT EXISTS response_time INTEGER;

-- 확인
-- SELECT column_name, data_type FROM information_schema.columns WHERE table_name = 'test_cases';
