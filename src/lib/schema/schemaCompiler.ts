/**
 * Schema Compiler (YAML-based, Unified)
 * 
 * JSON Schema (with x-* extensions)를 처리하여
 * 동적 섹션, 조건부 필드, TYPE별 enum 등을 지원합니다.
 * 
 * Original / Enhanced 구분은 YAML 기반으로 이루어집니다!
 * - Original: original/schema-logic.yaml (x-* 비활성, 단순 규칙)
 * - Enhanced: enhanced/schema-logic.yaml (x-* 활성, 복잡 규칙)
 * 
 * @see api_docs_zendesk/schema_to_table_algorithm.md
 * @see schema_definitions/{psdSet}/{schemaType}/schema-logic.yaml
 */

import {
  determineSectionName as determineSectionNameDynamic,
  getSectionOrder,
  determineValidationLayers as determineValidationLayersDynamic,
  getSchemaStructurePatterns,
  getPlatformSkeleton,
} from './schemaLogicEngine';

// ============================================================================
// Type Definitions
// ============================================================================

export interface EnhancedSchema {
  $schema?: string;
  title: string;
  type: string;
  required: string[];
  properties: Record<string, EnhancedProperty>;
  allOf?: ConditionalRule[];
  oneOf?: any[];
  description?: string;
  'x-transport'?: {
    uri: string;
    methods: string[];
    'body-root': string;
  };
  'x-exclusive-keys'?: string[];
  [key: string]: any;
}

export interface EnhancedProperty {
  type: string;
  enum?: (string | number)[];
  default?: any;
  minItems?: number;
  maxItems?: number;
  items?: { type: string; properties?: Record<string, any> };
  description?: string;
  
  // Object type - nested properties
  properties?: Record<string, EnhancedProperty>;  // 🔥 중첩 객체 지원
  required?: string[];  // 🔥 중첩 객체의 required 필드
  oneOf?: any[];  // 🔥 oneOf 지원
  
  // Enhanced extensions (YAML로 활성화 제어)
  'x-enum-labels'?: Record<string, string>;
  'x-enum-by-type'?: Record<string, (string | number)[]>;
  'x-enum-labels-by-type'?: Record<string, Record<string, string>>;
  'x-node-count-by-type'?: Record<string, number | number[]>;
  'x-value-constraint'?: Record<string, string>;
  'x-ui'?: {
    label?: string;
    group?: string;
    hint?: string;
    visibleWhen?: {
      TYPE?: string | string[];
      STYPE?: number;
      [key: string]: any;
    };
  };
  'x-transport'?: {
    uri: string;
    methods: string[];
    'body-root': string;
  };
  'x-exclusive-keys'?: string[];
  [key: string]: any;
}

export interface ConditionalRule {
  if?: {
    properties?: Record<string, { const?: string; enum?: string[] }>;
  };
  then?: {
    required?: string[];
    properties?: any;
  };
}

export interface EnhancedField {
  key: string;
  type: string;
  default?: any;
  required: RequiredStatus;
  enum?: (string | number)[];
  enumLabels?: Record<string, string>;
  enumByType?: Record<string, (string | number)[]>;
  enumLabelsByType?: Record<string, Record<string, string>>;
  nodeCountByType?: Record<string, number | number[]>;
  valueConstraint?: Record<string, string>;
  minItems?: number;
  maxItems?: number;
  items?: { type: string };
  ui?: {
    label?: string;
    group?: string;
    hint?: string;
    visibleWhen?: any;
  };
  section: string;
  validationLayers: ValidationLayer[];
  children?: EnhancedField[];  // 🔥 중첩 필드 지원
}

export type RequiredStatus = Record<string, 'required' | 'optional' | 'n/a'>;

export type ValidationLayer = 'STD' | 'APP';

export interface SectionGroup {
  name: string;
  fields: EnhancedField[];
  order: number;
}

// ============================================================================
// Main Compiler Function
// ============================================================================

/**
 * JSON Schema를 섹션별 필드 그룹으로 컴파일 (YAML-based, 동기)
 * 
 * Original / Enhanced 구분은 psdSet/schemaType으로 결정됩니다!
 * YAML 규칙이 초기화되어 있어야 합니다 (initSchemaLogicRules 호출 후 사용)
 * 
 * @param schema - JSON Schema (with or without x-* extensions)
 * @param psdSet - PSD 세트 (Level 1) - 제품의 psd_set에서 가져옴
 * @param schemaType - 스키마 타입 (Level 2) - 'original' | 'enhanced'
 */
export function compileSchema(
  schema: EnhancedSchema,
  psdSet: string,
  schemaType: string
): SectionGroup[] {
  // 🔥 YAML 기반 스키마 구조 패턴 감지 및 변환
  const transformedSchema = applySchemaStructurePatterns(schema, psdSet, schemaType);
  
  // 🔥 스키마 유효성 검사
  if (!transformedSchema || !transformedSchema.properties) {
    console.warn('⚠️ Invalid schema: missing properties', transformedSchema);
    return [];
  }
  
  // Phase 1: Extract basic info
  const types = extractTypes(transformedSchema);
  const fields = extractFields(transformedSchema);
  const conditionalRules = extractConditionalRequired(transformedSchema);
  
  // Phase 2: Calculate required status for each field
  const fieldsWithStatus = fields.map(field => {
    const layers = determineValidationLayersDynamic(field, psdSet, schemaType);
    return {
      ...field,
      required: calculateRequiredStatus(field, types, transformedSchema.required || [], conditionalRules),
      validationLayers: layers as ValidationLayer[],
    };
  });
  
  // Phase 3: Group by sections (YAML-based, 동기)
  const sections = groupFieldsBySectionsDynamic(fieldsWithStatus, types, psdSet, schemaType, transformedSchema);
  
  // Phase 4: Sort sections (YAML-based, 동기)
  return sortSectionsDynamic(sections, psdSet, schemaType, transformedSchema);
}

// Legacy alias for backward compatibility (with required parameters)
export const compileEnhancedSchema = (
  schema: EnhancedSchema,
  psdSet: string,
  schemaType: string
): SectionGroup[] => {
  return compileSchema(schema, psdSet, schemaType);
};

// ============================================================================
// Phase 0: Schema Structure Pattern Detection (YAML-based)
// ============================================================================

/**
 * YAML 규칙 기반 스키마 구조 패턴 감지 및 변환
 */
function applySchemaStructurePatterns(
  schema: EnhancedSchema,
  psdSet: string,
  schemaType: string
): EnhancedSchema {
  const patterns = getSchemaStructurePatterns(psdSet, schemaType);
  
  if (!patterns || patterns.length === 0) {
    return schema; // No patterns defined, return as-is
  }
  
  // 각 패턴을 순서대로 확인
  for (const pattern of patterns) {
    if (!pattern.enabled) continue;
    
    // 패턴 감지
    if (matchesPattern(schema, pattern.detect)) {
      console.log(`🔄 Applying schema pattern: ${pattern.name}`);
      
      // 패턴에 따라 변환 (psdSet, schemaType 전달)
      return transformSchema(schema, pattern.transform, psdSet, schemaType);
    }
  }
  
  return schema; // No matching pattern
}

/**
 * 스키마가 패턴 감지 조건과 일치하는지 확인
 */
function matchesPattern(schema: any, detectRules: any[]): boolean {
  if (!detectRules || detectRules.length === 0) return false;
  
  return detectRules.every(rule => {
    const { path, exists, isArray, value } = rule;
    
    // path로 값 접근
    const pathValue = getValueByPath(schema, path);
    
    // exists 체크
    if (exists !== undefined) {
      if (exists && pathValue === undefined) return false;
      if (!exists && pathValue !== undefined) return false;
    }
    
    // isArray 체크
    if (isArray !== undefined) {
      if (isArray && !Array.isArray(pathValue)) return false;
      if (!isArray && Array.isArray(pathValue)) return false;
    }
    
    // value 체크
    if (value !== undefined && pathValue !== value) {
      return false;
    }
    
    return true;
  });
}

/**
 * path 문자열로 객체 값에 접근
 */
function getValueByPath(obj: any, path: string): any {
  if (!obj || !path) return undefined;
  
  // "properties.*.oneOf" 같은 와일드카드 지원
  if (path.includes('*')) {
    const parts = path.split('.');
    let current = obj;
    
    for (const part of parts) {
      if (part === '*') {
        // 와일드카드: 모든 키를 순회하며 하나라도 존재하면 true
        if (typeof current !== 'object') return undefined;
        const keys = Object.keys(current);
        for (const key of keys) {
          const result = getValueByPath(current[key], parts.slice(parts.indexOf('*') + 1).join('.'));
          if (result !== undefined) return result;
        }
        return undefined;
      }
      
      current = current[part];
      if (current === undefined) return undefined;
    }
    
    return current;
  }
  
  // 일반 path
  return path.split('.').reduce((acc, part) => acc?.[part], obj);
}

/**
 * 패턴 변환 규칙 적용
 */
function transformSchema(schema: EnhancedSchema, transform: any, psdSet: string, schemaType: string): EnhancedSchema {
  const { action } = transform;
  
  switch (action) {
    case 'unwrap-root-key':
      return unwrapRootKey(schema, transform);
    
    case 'inject-entity-collection-simple':
      return injectEntityCollectionSimple(schema, transform, psdSet, schemaType);
    
    case 'inject-entity-collection':
      return injectEntityCollection(schema, transform, psdSet, schemaType);
    
    case 'wrap-in-virtual-object':
      return wrapInVirtualObject(schema, transform);
    
    case 'merge-properties-with-oneof':
      return mergePropertiesWithOneOf(schema, transform);
    
    case 'merge-conditional-properties':
      // allOf + properties 병합 (향후 구현)
      return schema;
    
    case 'process-as-exclusive-options':
      // 이미 extractFields에서 처리됨
      return schema;
    
    default:
      console.warn(`⚠️ Unknown transform action: ${action}`);
      return schema;
  }
}

/**
 * 단순 엔티티 정의 → 플랫폼 골격 자동 조립
 * 
 * 입력 (개발자 정의):
 * {
 *   "ELEM": {
 *     "type": "object",
 *     "properties": {...}
 *   }
 * }
 * 
 * 출력 (시스템 조립):
 * {
 *   "type": "object",
 *   "title": "ELEM",
 *   "properties": {
 *     "Assign": { "$ref": "#/$defs/entity-collection" }
 *   },
 *   "$defs": {
 *     "entity": {...},  // ELEM의 내용
 *     "entity-collection": {
 *       "type": "object",
 *       "patternProperties": {
 *         "^[0-9]+$": { "$ref": "#/$defs/entity" }
 *       }
 *     }
 *   }
 * }
 */
function injectEntityCollectionSimple(schema: any, _transform: any, psdSet: string, schemaType: string): EnhancedSchema {
  // 🎯 YAML에서 플랫폼 골격 정의 로드
  const skeleton = getPlatformSkeleton(psdSet, schemaType);
  
  // 1. 최상위 키가 하나만 있는지 확인 (엔티티 이름)
  const keys = Object.keys(schema);
  if (keys.length !== 1) {
    console.warn('⚠️ inject-entity-collection-simple: Expected single entity key, found:', keys);
    return schema;
  }
  
  const entityName = keys[0];
  const entityDef = schema[entityName];
  
  // 2. entity 정의가 유효한지 확인
  if (!entityDef || entityDef.type !== 'object') {
    console.warn('⚠️ inject-entity-collection-simple: Invalid entity definition');
    return schema;
  }
  
  // 3. body-root 결정 (YAML 기본값 사용)
  const bodyRoot = skeleton.defaultBodyRoot;
  
  // 4. entity-collection 정의 생성 (YAML 정의 기반)
  const entityCollection: any = {
    description: skeleton.entityCollection.description,
    type: skeleton.entityCollection.type,
    patternProperties: {
      [skeleton.entityCollection.patternProperties.pattern]: {
        $ref: skeleton.entityCollection.patternProperties.ref
      }
    },
    additionalProperties: skeleton.entityCollection.additionalProperties
  };
  
  // minProperties가 있으면 추가
  if (skeleton.entityCollection.minProperties !== undefined) {
    entityCollection.minProperties = skeleton.entityCollection.minProperties;
  }
  
  // 5. 최종 스키마 조립 (YAML 정의 기반)
  const assembledSchema: EnhancedSchema = {
    ...skeleton.rootStructure,
    title: entityName,
    properties: {
      [bodyRoot]: {
        $ref: '#/$defs/entity-collection'
      }
    },
    $defs: {
      entity: entityDef,
      'entity-collection': entityCollection
    },
    'x-transport': {
      uri: '',
      methods: [],
      'body-root': bodyRoot
    }
  };
  
  console.log(`✅ Injected entity collection (YAML-driven): "${entityName}" → "${bodyRoot}" → entity-collection → entity`);
  console.log(`📋 Platform skeleton from: ${psdSet}/${schemaType}/schema-logic.yaml`);
  
  return assembledSchema;
}

/**
 * 명시적 엔티티 정의 → 플랫폼 골격 자동 조립
 * 
 * 입력 (개발자 정의):
 * {
 *   "x-transport": { "body-root": "Assign" },
 *   "$defs": {
 *     "entity": { "type": "object", "properties": {...} }
 *   }
 * }
 * 
 * 출력 (시스템 조립):
 * {
 *   "type": "object",
 *   "properties": {
 *     "Assign": { "$ref": "#/$defs/entity-collection" }
 *   },
 *   "$defs": {
 *     "entity": {...},
 *     "entity-collection": {
 *       "type": "object",
 *       "patternProperties": {
 *         "^[0-9]+$": { "$ref": "#/$defs/entity" }
 *       },
 *       "additionalProperties": false
 *     }
 *   }
 * }
 */
function injectEntityCollection(schema: any, _transform: any, psdSet: string, schemaType: string): EnhancedSchema {
  // 🎯 YAML에서 플랫폼 골격 정의 로드
  const skeleton = getPlatformSkeleton(psdSet, schemaType);
  
  // 1. body-root 추출
  const bodyRoot = schema['x-transport']?.['body-root'];
  if (!bodyRoot) {
    console.warn('⚠️ inject-entity-collection: x-transport.body-root not found');
    return schema;
  }
  
  // 2. body-root 유효성 검사 (YAML 정의 기반)
  const validBodyRoots = [skeleton.defaultBodyRoot, ...(skeleton.alternativeBodyRoots || [])];
  if (!validBodyRoots.includes(bodyRoot)) {
    console.warn(`⚠️ inject-entity-collection: Invalid body-root "${bodyRoot}". Valid options:`, validBodyRoots);
  }
  
  // 3. entity 정의 확인
  if (!schema.$defs?.entity) {
    console.warn('⚠️ inject-entity-collection: $defs.entity not found');
    return schema;
  }
  
  // 4. entity-collection 정의 생성 (YAML 정의 기반)
  const entityCollection: any = {
    description: skeleton.entityCollection.description,
    type: skeleton.entityCollection.type,
    patternProperties: {
      [skeleton.entityCollection.patternProperties.pattern]: {
        $ref: skeleton.entityCollection.patternProperties.ref
      }
    },
    additionalProperties: skeleton.entityCollection.additionalProperties
  };
  
  // minProperties가 있으면 추가
  if (skeleton.entityCollection.minProperties !== undefined) {
    entityCollection.minProperties = skeleton.entityCollection.minProperties;
  }
  
  // 5. 최종 스키마 조립 (YAML 정의 기반)
  const assembledSchema: EnhancedSchema = {
    ...schema,
    ...skeleton.rootStructure,
    properties: {
      [bodyRoot]: {
        $ref: '#/$defs/entity-collection'
      }
    },
    $defs: {
      ...schema.$defs,
      'entity-collection': entityCollection
    }
  };
  
  // 6. title 설정 (없으면 body-root 사용)
  if (!assembledSchema.title) {
    assembledSchema.title = bodyRoot;
  }
  
  console.log(`✅ Injected entity collection: "${bodyRoot}" → entity-collection → entity`);
  
  return assembledSchema;
}

/**
 * 최상위 wrapper key 제거
 * 예: { "TABLE": { $schema, type, properties } } → { $schema, type, properties, title: "TABLE" }
 */
function unwrapRootKey(schema: any, transform: any): EnhancedSchema {
  const { extractTitle, preserveMetadata } = transform;
  
  // 최상위 키가 하나만 있는지 확인
  const keys = Object.keys(schema);
  if (keys.length !== 1) {
    console.warn('⚠️ unwrap-root-key: Expected single root key, found:', keys);
    return schema;
  }
  
  const wrapperKey = keys[0];
  const innerSchema = schema[wrapperKey];
  
  // innerSchema가 유효한 JSON Schema인지 확인
  if (!innerSchema || typeof innerSchema !== 'object') {
    console.warn('⚠️ unwrap-root-key: Invalid inner schema');
    return schema;
  }
  
  // Unwrap: innerSchema를 최상위로 이동
  const unwrappedSchema: EnhancedSchema = {
    ...innerSchema,
  };
  
  // wrapper key를 title로 사용 (extractTitle이 true이고 title이 없으면)
  if (extractTitle && !unwrappedSchema.title) {
    unwrappedSchema.title = wrapperKey;
  }
  
  // 메타데이터 보존
  if (preserveMetadata) {
    preserveMetadata.forEach((metaKey: string) => {
      if (schema[metaKey] && !unwrappedSchema[metaKey]) {
        unwrappedSchema[metaKey] = schema[metaKey];
      }
    });
  }
  
  console.log(`✅ Unwrapped root key: "${wrapperKey}" → title: "${unwrappedSchema.title}"`);
  
  return unwrappedSchema;
}

/**
 * 최상위 oneOf를 가상 래퍼 객체로 감싸기
 */
function wrapInVirtualObject(schema: EnhancedSchema, transform: any): EnhancedSchema {
  const { wrapperKey, wrapperType, preserveMetadata } = transform;
  
  // 래퍼 키 결정 (템플릿 지원)
  const key = wrapperKey.replace('{title}', schema.title || 'Options');
  
  // 래퍼 스키마 생성
  const wrappedSchema: EnhancedSchema = {
    type: 'object',
    title: schema.title,
    required: [],
    properties: {
      [key]: {
        type: wrapperType,
        ...schema,
      }
    },
  };
  
  // 메타데이터 보존
  if (preserveMetadata) {
    preserveMetadata.forEach((metaKey: string) => {
      if (schema[metaKey]) {
        wrappedSchema[metaKey] = schema[metaKey];
      }
    });
  }
  
  return wrappedSchema;
}

/**
 * 최상위 properties + oneOf 병합
 * 예: { properties: { X_RATIO, X_LENGTH }, oneOf: [...] }
 * → oneOf에 정의된 필드만 유지하고 가상 객체로 래핑
 */
function mergePropertiesWithOneOf(schema: EnhancedSchema, transform: any): EnhancedSchema {
  const { wrapperKey, wrapperType } = transform;
  
  // oneOf에서 사용되는 필드명 수집
  const oneOfFields = new Set<string>();
  if (schema.oneOf && Array.isArray(schema.oneOf)) {
    schema.oneOf.forEach((option: any) => {
      if (option.properties) {
        Object.keys(option.properties).forEach(key => oneOfFields.add(key));
      }
      if (option.required && Array.isArray(option.required)) {
        option.required.forEach((key: string) => oneOfFields.add(key));
      }
    });
  }
  
  // oneOf에 없는 properties 필드는 제거 (oneOf가 우선)
  const filteredProperties: Record<string, any> = {};
  if (schema.properties) {
    for (const [key, prop] of Object.entries(schema.properties)) {
      if (oneOfFields.has(key)) {
        filteredProperties[key] = prop;
      }
    }
  }
  
  // 래퍼 키 결정
  const key = wrapperKey.replace('{title}', schema.title || 'Options');
  
  // oneOf를 가진 가상 객체로 래핑
  const wrappedSchema: EnhancedSchema = {
    type: 'object',
    title: schema.title,
    required: [],
    properties: {
      [key]: {
        type: wrapperType,
        description: schema.description,
        oneOf: schema.oneOf,
        'x-ui': schema['x-ui'],
        'x-exclusive-keys': schema['x-exclusive-keys'],
      }
    },
    'x-transport': schema['x-transport'],
  };
  
  return wrappedSchema;
}

// ============================================================================
// Phase 1: Schema Analysis
// ============================================================================
function extractTypes(schema: EnhancedSchema): string[] {
  // 🔥 schema.properties가 없으면 빈 배열 반환
  if (!schema || !schema.properties) {
    return [];
  }
  
  const typeProperty = schema.properties.TYPE;
  if (!typeProperty || !typeProperty.enum) {
    return [];
  }
  return typeProperty.enum as string[];
}

/**
 * 모든 필드 추출 (중첩 객체 포함)
 */
function extractFields(schema: EnhancedSchema): EnhancedField[] {
  const fields: EnhancedField[] = [];
  
  // 🔥 schema.properties가 없으면 빈 배열 반환
  if (!schema || !schema.properties) {
    return [];
  }
  
  for (const [key, prop] of Object.entries(schema.properties)) {
    const field: EnhancedField = {
      key,
      type: prop.type,
      default: prop.default,
      required: {}, // Will be calculated later
      enum: prop.enum,
      enumLabels: prop['x-enum-labels'],
      enumByType: prop['x-enum-by-type'],
      enumLabelsByType: prop['x-enum-labels-by-type'],
      nodeCountByType: prop['x-node-count-by-type'],
      valueConstraint: prop['x-value-constraint'],
      minItems: prop.minItems,
      maxItems: prop.maxItems,
      items: prop.items,
      ui: prop['x-ui'],
      section: '', // Will be determined later
      validationLayers: [],
    };
    
    // 🔥 Object 타입 - 중첩 필드 추출
    if (prop.type === 'object' && prop.properties) {
      field.children = [];
      const objRequired = (prop.required as string[]) || [];
      
      for (const [childKey, childProp] of Object.entries(prop.properties)) {
        const childField: EnhancedField = {
          key: `${key}.${childKey}`,
          type: childProp.type,
          default: childProp.default,
          required: objRequired.includes(childKey) ? { '*': 'required' } : { '*': 'optional' },
          enum: childProp.enum,
          enumLabels: childProp['x-enum-labels'],
          ui: childProp['x-ui'],
          section: '',
          validationLayers: [],
        };
        field.children.push(childField);
      }
    }
    
    // 🔥 Object 타입 with oneOf - 상호 배타적 선택 (예: Method 1, 2, 3 중 선택)
  if (prop.type === 'object' && prop.oneOf && Array.isArray(prop.oneOf)) {
    field.children = [];
    
      // oneOf의 각 옵션을 섹션 헤더로 표시
    prop.oneOf.forEach((option: any, optionIndex: number) => {
        const optionTitle = option.title || `Option ${optionIndex + 1}`;
      const optionProps = option.properties || {};
      const optionRequired = option.required || [];
        
        // 섹션 헤더 추가 (옵션 제목)
        if (Object.keys(optionProps).length > 0) {
          // 섹션 헤더는 특별한 필드로 표시
          field.children!.push({
            key: `${key}.__section_${optionIndex}`,
            type: 'section-header' as any,
            required: {},
            section: optionTitle,
            validationLayers: [],
            ui: { label: optionTitle, group: key },
          });
        }
        
        // 옵션의 각 필드 추가
        for (const [childKey, childProp] of Object.entries(optionProps)) {
          const childField: EnhancedField = {
            key: `${key}.${childKey}`,
            type: childProp.type,
            default: childProp.default,
            required: optionRequired.includes(childKey) ? { '*': 'required' } : { '*': 'optional' },
            enum: childProp.enum,
            enumLabels: childProp['x-enum-labels'],
            ui: childProp['x-ui'],
            section: optionTitle,
            validationLayers: [],
          };
        field.children!.push(childField);
        }
      });
    }
    
    fields.push(field);
  }
  
  return fields;
}

/**
 * allOf에서 조건부 required 규칙 추출
 */
function extractConditionalRequired(schema: EnhancedSchema): ConditionalRule[] {
  if (!schema.allOf || !Array.isArray(schema.allOf)) {
    return [];
  }
  
  // 🔥 allOf 항목 중 if-then 구조를 가진 것만 필터링
  return schema.allOf.filter(rule => 
    rule && 
    typeof rule === 'object' && 
    'if' in rule && 
    'then' in rule
  );
}

// ============================================================================
// Phase 2: Required Status Calculation
// ============================================================================

/**
 * 각 TYPE별 필드의 required 상태 계산
 */
function calculateRequiredStatus(
  field: EnhancedField,
  types: string[],
  baseRequired: string[],
  conditionalRules: ConditionalRule[]
): RequiredStatus {
  const status: RequiredStatus = {};
  
  // 🔥 TYPE 필드가 없는 스키마 (e.g., BeamForceTable)
  // → 타입별 조건 없이 단순히 required 배열만 확인
  if (types.length === 0) {
    if (baseRequired.includes(field.key)) {
      return { '*': 'required' };
    } else {
      return { '*': 'optional' };
    }
  }
  
  // 1. Base required check (TYPE 필드가 있는 경우)
  if (baseRequired.includes(field.key)) {
    types.forEach(type => {
      status[type] = 'required';
    });
    return status;
  }
  
  // 2. For each TYPE, check conditions
  types.forEach(type => {
    status[type] = 'optional'; // default
    
    // Check conditional required
    for (const rule of conditionalRules) {
      // 🔥 rule.then과 rule.then.required가 존재하는지 확인
      if (matchesCondition(rule.if?.properties, type) && 
          rule.then?.required && 
          Array.isArray(rule.then.required) &&
          rule.then.required.includes(field.key)) {
        status[type] = 'required';
        break;
      }
    }
    
    // Check visibleWhen (determines N/A)
    if (field.ui?.visibleWhen) {
      if (!isVisible(field.ui.visibleWhen, type)) {
        status[type] = 'n/a';
      }
    }
  });
  
  return status;
}

/**
 * 조건이 특정 TYPE과 매칭되는지 확인
 */
function matchesCondition(
  condition: Record<string, { const?: string; enum?: string[] }> | undefined,
  type: string
): boolean {
  if (!condition) return false;
  if (condition.TYPE?.const === type) return true;
  if (condition.TYPE?.enum?.includes(type)) return true;
  return false;
}

/**
 * visibleWhen 조건에 따라 필드가 해당 TYPE에서 보이는지 확인
 */
function isVisible(visibleWhen: any, type: string): boolean {
  if (!visibleWhen.TYPE) return true;
  
  if (Array.isArray(visibleWhen.TYPE)) {
    return visibleWhen.TYPE.includes(type);
  }
  
  return visibleWhen.TYPE === type;
}

// ============================================================================
// Phase 3: Section Grouping (YAML-based, 동기)
// ============================================================================

/**
 * 필드들을 섹션별로 그룹화 (YAML 규칙 기반, 동기)
 */
function groupFieldsBySectionsDynamic(
  fields: EnhancedField[],
  types: string[],
  psdSet: string,
  schemaType: string,
  schema?: any
): Map<string, EnhancedField[]> {
  const sections = new Map<string, EnhancedField[]>();
  
  for (const field of fields) {
    const sectionName = determineSectionNameDynamic(field, types, psdSet, schemaType, schema);
    field.section = sectionName;
    
    if (!sections.has(sectionName)) {
      sections.set(sectionName, []);
    }
    sections.get(sectionName)!.push(field);
  }
  
  return sections;
}

// ============================================================================
// Phase 4: Section Sorting (YAML-based, 동기)
// ============================================================================

/**
 * 섹션을 정해진 순서로 정렬 (YAML 규칙 기반, 동기)
 */
function sortSectionsDynamic(
  sections: Map<string, EnhancedField[]>,
  psdSet: string,
  schemaType: string,
  schema?: any
): SectionGroup[] {
  const sectionOrder = getSectionOrder(psdSet, schemaType, schema);
  const result: SectionGroup[] = [];
  
  for (const [name, fields] of sections) {
    const order = sectionOrder.indexOf(name);
    result.push({
      name,
      fields,
      order: order === -1 ? 999 : order,
    });
  }
  
  return result.sort((a, b) => a.order - b.order);
}

// ============================================================================
// Validation Layers (YAML-based, 동기)
// ============================================================================

// Note: determineValidationLayersDynamic is used directly in compileSchema (line 147)
// This wrapper function is kept for future use if needed
