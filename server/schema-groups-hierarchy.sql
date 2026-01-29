-- ============================================================================
-- Groups 테이블 계층 구조 지원 (5단계까지)
-- ============================================================================

-- 1. parent_group_id 컬럼 추가 (자기 참조 외래키)
DO $$
BEGIN
  IF NOT EXISTS (
    SELECT 1 FROM information_schema.columns 
    WHERE table_name = 'groups' AND column_name = 'parent_group_id'
  ) THEN
    ALTER TABLE groups ADD COLUMN parent_group_id TEXT REFERENCES groups(id) ON DELETE CASCADE;
  END IF;
END $$;

-- 2. depth 컬럼 추가 (계층 깊이, 1부터 시작)
DO $$
BEGIN
  IF NOT EXISTS (
    SELECT 1 FROM information_schema.columns 
    WHERE table_name = 'groups' AND column_name = 'depth'
  ) THEN
    ALTER TABLE groups ADD COLUMN depth INTEGER DEFAULT 1;
  END IF;
END $$;

-- 3. 인덱스 생성
CREATE INDEX IF NOT EXISTS idx_groups_parent ON groups(parent_group_id);
CREATE INDEX IF NOT EXISTS idx_groups_depth ON groups(depth);

-- 4. 기존 그룹 depth 업데이트 (모두 1단계)
UPDATE groups SET depth = 1 WHERE depth IS NULL;

-- 5. 최대 depth 제한을 위한 CHECK 제약조건 (옵션)
-- ALTER TABLE groups ADD CONSTRAINT check_max_depth CHECK (depth <= 5);

-- 설명
COMMENT ON COLUMN groups.parent_group_id IS '상위 그룹 ID (NULL이면 최상위 그룹)';
COMMENT ON COLUMN groups.depth IS '그룹 계층 깊이 (1부터 시작, 최대 5)';
