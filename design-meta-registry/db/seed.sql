-- Generic Entity-Relation Registry - Sample Data

-- ============================================================================
-- 엔티티 타입 예시
-- ============================================================================

INSERT INTO entity_types (id, name, description, icon, color) VALUES
('design-code', 'Design Code', '설계 코드/기준', '📋', '#3b82f6'),
('parameter', 'Parameter', '설계 파라미터', '📝', '#22c55e'),
('step', 'Step', '설계 단계', '🔷', '#f59e0b'),
('product', 'Product', '제품', '📦', '#8b5cf6');

-- ============================================================================
-- 필드 정의 예시
-- ============================================================================

-- Design Code 필드
INSERT INTO entity_fields (entity_type_id, field_name, field_type, is_required, is_primary, display_order) VALUES
('design-code', 'name', 'text', 1, 1, 1),
('design-code', 'region', 'text', 0, 0, 2),
('design-code', 'year', 'text', 0, 0, 3),
('design-code', 'full_name', 'text', 0, 0, 4);

-- Parameter 필드
INSERT INTO entity_fields (entity_type_id, field_name, field_type, is_required, is_primary, display_order) VALUES
('parameter', 'name', 'text', 1, 1, 1),
('parameter', 'category', 'enum', 0, 0, 2),
('parameter', 'description', 'text', 0, 0, 3);

-- Step 필드
INSERT INTO entity_fields (entity_type_id, field_name, field_type, is_required, is_primary, display_order) VALUES
('step', 'name', 'text', 1, 1, 1),
('step', 'category', 'enum', 0, 0, 2),
('step', 'order', 'number', 0, 0, 3);

-- ============================================================================
-- 관계 정의 예시
-- ============================================================================

INSERT INTO relation_types (id, name, source_type_id, target_type_id, relation_kind, description) VALUES
('code-parameters', 'Code → Parameters', 'design-code', 'parameter', 'many-to-many', '코드가 지원하는 파라미터'),
('code-steps', 'Code → Steps', 'design-code', 'step', 'many-to-many', '코드의 설계 단계'),
('product-codes', 'Product → Codes', 'product', 'design-code', 'one-to-many', '제품이 지원하는 코드');

-- ============================================================================
-- 샘플 엔티티 데이터
-- ============================================================================

-- Design Codes
INSERT INTO entities (id, entity_type_id) VALUES
('kds-24-14-31', 'design-code'),
('aisc-360', 'design-code'),
('japan-road-h24', 'design-code');

INSERT INTO entity_data (entity_id, field_name, field_value) VALUES
('kds-24-14-31', 'name', 'KDS 24 14 31 : 2018'),
('kds-24-14-31', 'region', 'KR'),
('kds-24-14-31', 'year', '2018'),
('aisc-360', 'name', 'AISC 360-22'),
('aisc-360', 'region', 'US'),
('aisc-360', 'year', '2022'),
('japan-road-h24', 'name', 'Japan Road H24'),
('japan-road-h24', 'region', 'JP'),
('japan-road-h24', 'year', '2012');

-- Parameters
INSERT INTO entities (id, entity_type_id) VALUES
('frame-definition', 'parameter'),
('load-contribution', 'parameter'),
('unbraced-length', 'parameter'),
('effective-k', 'parameter'),
('strength-reduction', 'parameter');

INSERT INTO entity_data (entity_id, field_name, field_value) VALUES
('frame-definition', 'name', 'Definition of Frame'),
('frame-definition', 'category', 'input'),
('load-contribution', 'name', 'Load Contribution'),
('load-contribution', 'category', 'input'),
('unbraced-length', 'name', 'Unbraced Length'),
('unbraced-length', 'category', 'stability'),
('effective-k', 'name', 'Effective Length Factor (K)'),
('effective-k', 'category', 'stability'),
('strength-reduction', 'name', 'Strength Reduction Factors'),
('strength-reduction', 'category', 'strength');

-- ============================================================================
-- 샘플 관계 데이터
-- ============================================================================

-- KDS가 지원하는 파라미터들
INSERT INTO relations (relation_type_id, source_entity_id, target_entity_id) VALUES
('code-parameters', 'kds-24-14-31', 'frame-definition'),
('code-parameters', 'kds-24-14-31', 'load-contribution'),
('code-parameters', 'kds-24-14-31', 'unbraced-length'),
('code-parameters', 'kds-24-14-31', 'effective-k'),
('code-parameters', 'kds-24-14-31', 'strength-reduction'),
('code-parameters', 'aisc-360', 'frame-definition'),
('code-parameters', 'aisc-360', 'load-contribution'),
('code-parameters', 'aisc-360', 'unbraced-length'),
('code-parameters', 'japan-road-h24', 'frame-definition'),
('code-parameters', 'japan-road-h24', 'load-contribution');
