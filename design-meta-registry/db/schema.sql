-- Generic Entity-Relation Registry - Schema
-- 범용 오브젝트 및 관계 정의 시스템

-- ============================================================================
-- 엔티티 정의 (오브젝트 타입)
-- ============================================================================

-- 사용자 정의 엔티티 타입
CREATE TABLE IF NOT EXISTS entity_types (
  id TEXT PRIMARY KEY,          -- 'design-code', 'parameter', 'step'
  name TEXT NOT NULL,           -- 'Design Code', 'Parameter'
  description TEXT,
  icon TEXT,                    -- 이모지 또는 아이콘 이름
  color TEXT,                   -- hex color
  created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- 엔티티의 필드 정의
CREATE TABLE IF NOT EXISTS entity_fields (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  entity_type_id TEXT NOT NULL,
  field_name TEXT NOT NULL,     -- 'name', 'region', 'year'
  field_type TEXT NOT NULL,     -- 'text', 'number', 'enum', 'boolean', 'date'
  is_required INTEGER DEFAULT 0,
  is_primary INTEGER DEFAULT 0, -- 표시용 주요 필드
  enum_values TEXT,             -- JSON array for enum type
  default_value TEXT,
  display_order INTEGER DEFAULT 0,
  FOREIGN KEY (entity_type_id) REFERENCES entity_types(id) ON DELETE CASCADE,
  UNIQUE(entity_type_id, field_name)
);

-- ============================================================================
-- 엔티티 데이터 (실제 인스턴스)
-- ============================================================================

-- 엔티티 인스턴스
CREATE TABLE IF NOT EXISTS entities (
  id TEXT PRIMARY KEY,          -- UUID or custom ID
  entity_type_id TEXT NOT NULL,
  created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (entity_type_id) REFERENCES entity_types(id) ON DELETE CASCADE
);

-- 엔티티 데이터 (Key-Value 형태)
CREATE TABLE IF NOT EXISTS entity_data (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  entity_id TEXT NOT NULL,
  field_name TEXT NOT NULL,
  field_value TEXT,
  FOREIGN KEY (entity_id) REFERENCES entities(id) ON DELETE CASCADE,
  UNIQUE(entity_id, field_name)
);

-- ============================================================================
-- 관계 정의
-- ============================================================================

-- 엔티티 타입 간 관계 정의
CREATE TABLE IF NOT EXISTS relation_types (
  id TEXT PRIMARY KEY,          -- 'code-has-parameters'
  name TEXT NOT NULL,           -- 'Code → Parameters'
  source_type_id TEXT NOT NULL, -- 'design-code'
  target_type_id TEXT NOT NULL, -- 'parameter'
  relation_kind TEXT DEFAULT 'many-to-many', -- 'one-to-one', 'one-to-many', 'many-to-many'
  description TEXT,
  created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (source_type_id) REFERENCES entity_types(id) ON DELETE CASCADE,
  FOREIGN KEY (target_type_id) REFERENCES entity_types(id) ON DELETE CASCADE
);

-- 실제 관계 인스턴스
CREATE TABLE IF NOT EXISTS relations (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  relation_type_id TEXT NOT NULL,
  source_entity_id TEXT NOT NULL,
  target_entity_id TEXT NOT NULL,
  data TEXT,                    -- JSON for additional relation data
  created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (relation_type_id) REFERENCES relation_types(id) ON DELETE CASCADE,
  FOREIGN KEY (source_entity_id) REFERENCES entities(id) ON DELETE CASCADE,
  FOREIGN KEY (target_entity_id) REFERENCES entities(id) ON DELETE CASCADE,
  UNIQUE(relation_type_id, source_entity_id, target_entity_id)
);

-- ============================================================================
-- 인덱스
-- ============================================================================

CREATE INDEX IF NOT EXISTS idx_entities_type ON entities(entity_type_id);
CREATE INDEX IF NOT EXISTS idx_entity_data_entity ON entity_data(entity_id);
CREATE INDEX IF NOT EXISTS idx_relations_type ON relations(relation_type_id);
CREATE INDEX IF NOT EXISTS idx_relations_source ON relations(source_entity_id);
CREATE INDEX IF NOT EXISTS idx_relations_target ON relations(target_entity_id);
