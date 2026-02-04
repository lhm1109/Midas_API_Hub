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
import { schemaCompileCache, generateSchemaHash } from '../cache/schemaCache';
import { expandFieldsByArrayGroupId } from './conditionExtractor';

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

  // Enhanced extensions (YAML로 활성화 제어) - 순수 UI 마커만
  'x-enum-labels'?: Record<string, string>;
  'x-enum-labels-by-type'?: Record<string, Record<string, string>>;
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
  description?: string;
  default?: any;
  required: RequiredStatus;

  // 필수 필드들만 명시
  section: string;
  validationLayers: ValidationLayer[];
  children?: EnhancedField[];

  // 🎯 NEW: 런타임 트리거 필드 (visibleWhen, x-required-when에서 자동 추출)
  runtimeTriggers?: string[];

  // 🔥 하드코딩 제거: 모든 확장 필드를 동적으로 저장
  [key: string]: any;  // x-* 필드들을 동적으로 저장
}

export type RequiredStatus = Record<string, 'required' | 'optional' | 'n/a' | 'conditional'>;

export type ValidationLayer = 'STD' | 'APP';

/**
 * 🎯 VariantAxis: 스키마의 조건부 분기를 결정하는 축
 * 
 * 예시:
 * - SKEW 엔티티: { field: 'iMETHOD', values: [1,2,3,4] }
 * - ELEM 엔티티: { field: 'TYPE', values: ['BEAM','TRUSS',...] }
 * - LOAD 엔티티: { field: 'LOAD_TYPE', values: [1,2,3] }
 * 
 * 자동 추론 규칙:
 * 1. enum을 가진 필드
 * 2. 다른 필드들의 visibleWhen/x-required-when에서 반복 사용됨
 * 3. allOf.if 조건에 등장
 */
export interface VariantAxis {
  field: string;
  type: 'enum' | 'integer' | 'string' | 'number';
  values: any[];
  labels?: Record<string, string>; // x-enum-labels
}

/**
 * 🎯 Compiled Schema Context: 스키마 컴파일 결과 + 메타데이터
 */
export interface CompiledSchemaContext {
  sections: SectionGroup[];
  variantAxes: VariantAxis[];
}

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
 * ⚡ 캐싱 적용: 같은 스키마는 재컴파일하지 않음
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
  // ⚡ 캐시 체크
  const cacheKey = generateSchemaHash(schema, psdSet, schemaType);
  const cached = schemaCompileCache.get(cacheKey);

  if (cached) {
    console.log('✅ Using cached schema compilation (key:', cacheKey.substring(0, 80) + '...)');
    return cached;
  }

  console.log('🔄 Compiling schema (cache miss)');
  // 🔥 YAML 기반 스키마 구조 패턴 감지 및 변환
  const transformedSchema = applySchemaStructurePatterns(schema, psdSet, schemaType);

  // 🔥 스키마 유효성 검사
  if (!transformedSchema || !transformedSchema.properties) {
    console.warn('⚠️ Invalid schema: missing properties', transformedSchema);
    return [];
  }

  // Phase 1: Extract basic info
  const types = extractTypes(transformedSchema);
  const rawFields = extractFields(transformedSchema);
  const conditionalRules = extractConditionalRequired(transformedSchema);

  // 🔥 Phase 1.5: Expand fields with array groupId
  // x-optional-when 배열에 groupId가 있으면 각 조건별로 필드 인스턴스 생성
  const fields = expandFieldsByArrayGroupId(rawFields, []);

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
  const result = sortSectionsDynamic(sections, psdSet, schemaType, transformedSchema);

  // ⚡ 결과를 캐시에 저장
  schemaCompileCache.set(cacheKey, result);

  return result;
}

// Legacy alias for backward compatibility (with required parameters)
export const compileEnhancedSchema = (
  schema: EnhancedSchema,
  psdSet: string,
  schemaType: string
): SectionGroup[] => {
  return compileSchema(schema, psdSet, schemaType);
};

/**
 * 🎯 Schema를 컴파일하고 VariantAxes를 자동 추론하여 CompiledSchemaContext 반환
 * 
 * @param schema - JSON Schema
 * @param psdSet - PSD 세트 (Level 1)
 * @param schemaType - 스키마 타입 (Level 2)
 * @returns CompiledSchemaContext (sections + variantAxes)
 */
export function compileSchemaWithContext(
  schema: EnhancedSchema,
  psdSet: string,
  schemaType: string
): CompiledSchemaContext {
  const sections = compileSchema(schema, psdSet, schemaType);
  const variantAxes = inferVariantAxes(schema, sections);

  return {
    sections,
    variantAxes,
  };
}

/**
 * 🎯 VariantAxis 자동 추론
 * 
 * 규칙:
 * 1. enum을 가진 필드
 * 2. 다른 필드들의 visibleWhen/x-required-when에서 반복 사용됨
 * 3. allOf.if 조건에 등장
 * 
 * @param schema - JSON Schema
 * @param sections - 컴파일된 섹션들
 * @returns VariantAxis 배열
 */
function inferVariantAxes(
  schema: EnhancedSchema,
  sections: SectionGroup[]
): VariantAxis[] {
  const axes: VariantAxis[] = [];
  const candidateFields = new Map<string, { type: string; values: any[]; labels?: Record<string, string>; refCount: number }>();

  // 🔥 Step 1: enum을 가진 필드를 후보로 수집
  for (const section of sections) {
    for (const field of section.fields) {
      if (field.enum && Array.isArray(field.enum) && field.enum.length > 0) {
        candidateFields.set(field.key, {
          type: field.type,
          values: field.enum,
          labels: (field as any)['x-enum-labels'],
          refCount: 0,
        });
      }
    }
  }

  // 🔥 Step 2: visibleWhen / x-required-when에서 사용 횟수 카운트
  for (const section of sections) {
    for (const field of section.fields) {
      // visibleWhen 확인
      const visibleWhen = field.ui?.visibleWhen;
      if (visibleWhen && typeof visibleWhen === 'object') {
        for (const key of Object.keys(visibleWhen)) {
          if (candidateFields.has(key)) {
            const candidate = candidateFields.get(key)!;
            candidate.refCount++;
          }
        }
      }

      // x-required-when 확인
      const requiredWhen = (field as any)['x-required-when'];
      if (requiredWhen && typeof requiredWhen === 'object') {
        for (const key of Object.keys(requiredWhen)) {
          if (candidateFields.has(key)) {
            const candidate = candidateFields.get(key)!;
            candidate.refCount++;
          }
        }
      }

      // 중첩 필드도 확인
      if (field.children && Array.isArray(field.children)) {
        for (const child of field.children) {
          const childVisibleWhen = (child as any).ui?.visibleWhen;
          if (childVisibleWhen && typeof childVisibleWhen === 'object') {
            for (const key of Object.keys(childVisibleWhen)) {
              if (candidateFields.has(key)) {
                const candidate = candidateFields.get(key)!;
                candidate.refCount++;
              }
            }
          }

          const childRequiredWhen = (child as any)['x-required-when'];
          if (childRequiredWhen && typeof childRequiredWhen === 'object') {
            for (const key of Object.keys(childRequiredWhen)) {
              if (candidateFields.has(key)) {
                const candidate = candidateFields.get(key)!;
                candidate.refCount++;
              }
            }
          }
        }
      }
    }
  }

  // 🔥 Step 3: allOf.if 조건 확인
  if (schema.allOf && Array.isArray(schema.allOf)) {
    for (const rule of schema.allOf) {
      if (rule.if?.properties) {
        for (const key of Object.keys(rule.if.properties)) {
          if (candidateFields.has(key)) {
            const candidate = candidateFields.get(key)!;
            candidate.refCount += 2; // allOf는 가중치 높게
          }
        }
      }
    }
  }

  // 🔥 Step 4: refCount > 0인 필드를 VariantAxis로 추가
  for (const [field, candidate] of candidateFields.entries()) {
    if (candidate.refCount > 0) {
      axes.push({
        field,
        type: candidate.type === 'integer' || candidate.type === 'number' ? candidate.type : 'enum',
        values: candidate.values,
        labels: candidate.labels,
      });
    }
  }

  return axes;
}

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
  // 🔥 방어 코드: schema가 유효한지 확인
  if (!schema || typeof schema !== 'object') {
    console.warn('⚠️ applySchemaStructurePatterns: Invalid schema', schema);
    return schema || {} as EnhancedSchema;
  }

  const patterns = getSchemaStructurePatterns(psdSet, schemaType);

  console.log(`🔍 applySchemaStructurePatterns: ${patterns?.length || 0} patterns loaded for ${psdSet}/${schemaType}`);
  console.log('🔍 Schema keys:', Object.keys(schema));
  console.log('🔍 Schema has properties.Assign?', !!(schema as any).properties?.Assign);
  console.log('🔍 Schema properties.Assign.additionalProperties?', !!(schema as any).properties?.Assign?.additionalProperties);

  // 🔥 방어: patterns가 배열인지 확인
  if (!patterns || !Array.isArray(patterns) || patterns.length === 0) {
    console.warn('⚠️ No patterns loaded or patterns is not an array, returning schema as-is. patterns:', patterns);
    return schema; // No patterns defined, return as-is
  }

  // 각 패턴을 순서대로 확인
  for (const pattern of patterns) {
    if (!pattern.enabled) {
      console.log(`⏭️ Pattern "${pattern.name}" is disabled, skipping`);
      continue;
    }

    console.log(`🔍 Testing pattern: "${pattern.name}" with detect rules:`, JSON.stringify(pattern.detect, null, 2));

    // 패턴 감지
    if (matchesPattern(schema, pattern.detect)) {
      console.log(`✅ Pattern matched: ${pattern.name} - applying transform: ${pattern.transform?.action}`);

      // 패턴에 따라 변환 (psdSet, schemaType 전달)
      return transformSchema(schema, pattern.transform, psdSet, schemaType);
    } else {
      console.log(`❌ Pattern "${pattern.name}" did not match`);
    }
  }

  console.warn('⚠️ No matching pattern found, returning schema as-is');
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

    // 🔥 NEW: Assign/Argument + additionalProperties 패턴 처리
    case 'unwrap-wrapper-with-additionalProperties':
      return unwrapWrapperWithAdditionalProperties(schema, transform);

    default:
      console.warn(`⚠️ Unknown transform action: ${action}`);
      return schema;
  }
}

/**
 * 🔥 NEW: Assign/Argument + additionalProperties/patternProperties 패턴 언래핑
 * 
 * additionalProperties 패턴:
 * {
 *   "properties": {
 *     "Assign": {
 *       "type": "object",
 *       "additionalProperties": { ...entitySchema }
 *     }
 *   }
 * }
 * 
 * patternProperties 패턴:
 * {
 *   "properties": {
 *     "Assign": {
 *       "type": "object",
 *       "additionalProperties": false,
 *       "patternProperties": {
 *         "^[0-9]+$": { ...entitySchema }
 *       }
 *     }
 *   }
 * }
 * 
 * 결과:
 * {
 *   "title": "Assign",
 *   "properties": {...},  // entity 내용
 *   "required": [...],    // entity 내용
 *   "allOf": [...]        // entity 내용
 * }
 */
function unwrapWrapperWithAdditionalProperties(schema: any, _transform: any): EnhancedSchema {
  // 1. wrapper key 찾기 (Assign, Argument 등)
  const wrapperKeys = ['Assign', 'Argument'];

  if (!schema.properties) {
    console.warn('⚠️ unwrapWrapperWithAdditionalProperties: No properties found');
    return schema as EnhancedSchema;
  }

  let wrapperKey: string | undefined;
  let entitySchema: any;

  // 먼저 additionalProperties 패턴 확인
  for (const key of wrapperKeys) {
    if (schema.properties[key]?.additionalProperties &&
      typeof schema.properties[key].additionalProperties === 'object' &&
      schema.properties[key].additionalProperties.type === 'object') {
      wrapperKey = key;
      entitySchema = schema.properties[key].additionalProperties;
      break;
    }
  }

  // additionalProperties가 없으면 patternProperties 확인
  if (!entitySchema) {
    for (const key of wrapperKeys) {
      const patternProps = schema.properties[key]?.patternProperties;
      if (patternProps) {
        // 숫자 ID 패턴 찾기 (^[0-9]+$ 또는 유사 패턴)
        const numericPatterns = ['^[0-9]+$', '^\\d+$', '^[1-9][0-9]*$'];
        for (const pattern of numericPatterns) {
          if (patternProps[pattern]?.type === 'object') {
            wrapperKey = key;
            entitySchema = patternProps[pattern];
            console.log(`✅ Found entity in patternProperties["${pattern}"]`);
            break;
          }
        }
        if (entitySchema) break;
      }
    }
  }

  // 동적으로 wrapper key 찾기
  if (!entitySchema) {
    for (const key of Object.keys(schema.properties)) {
      // additionalProperties 확인
      if (schema.properties[key]?.additionalProperties?.type === 'object') {
        wrapperKey = key;
        entitySchema = schema.properties[key].additionalProperties;
        break;
      }
      // patternProperties 확인
      const patternProps = schema.properties[key]?.patternProperties;
      if (patternProps) {
        const patternKey = Object.keys(patternProps).find(p =>
          patternProps[p]?.type === 'object'
        );
        if (patternKey) {
          wrapperKey = key;
          entitySchema = patternProps[patternKey];
          break;
        }
      }
    }
  }

  // 🔥 NEW: Argument wrapper with direct properties (Table schema pattern)
  // Pattern: { properties: { Argument: { type: 'object', properties: {...} } } }
  // This is different from Entity Collection pattern (additionalProperties/patternProperties)
  if (!entitySchema) {
    for (const key of wrapperKeys) {
      const wrapperObj = schema.properties[key];
      // 🔥 FIX: Check if wrapperObj exists before accessing its properties
      if (!wrapperObj) continue;

      // Check if it's a simple wrapper with direct properties
      // additionalProperties can be missing or explicitly false (both are valid for Table schemas)
      const hasMapPattern = wrapperObj.additionalProperties && typeof wrapperObj.additionalProperties === 'object';
      if (wrapperObj?.type === 'object' &&
        wrapperObj.properties &&
        !hasMapPattern &&
        !wrapperObj.patternProperties) {
        wrapperKey = key;
        entitySchema = wrapperObj;  // Use the entire Argument object as the entity
        console.log(`✅ Found simple wrapper pattern: "${key}" with direct properties`);
        break;
      }
    }
  }

  if (!wrapperKey || !entitySchema) {
    console.warn('⚠️ unwrapWrapperWithAdditionalProperties: No wrapper key with additionalProperties/patternProperties/properties found');
    return schema as EnhancedSchema;
  }

  if (!entitySchema || typeof entitySchema !== 'object') {
    console.warn('⚠️ unwrapWrapperWithAdditionalProperties: Invalid entity schema');
    return schema as EnhancedSchema;
  }

  console.log(`✅ unwrapWrapperWithAdditionalProperties: Extracting entity from "${wrapperKey}"`);

  // 3. 새로운 스키마 구성 (엔티티 스키마를 최상위로)
  const result: EnhancedSchema = {
    ...entitySchema,
    title: wrapperKey,
  };

  // 4. $schema 보존
  if (schema.$schema) {
    result.$schema = schema.$schema;
  }

  return result;
}

/**
 * 최상위 wrapper key 제거 (예: { 'Argument': { type, properties } })
 * 
 * 입력:
 * {
 *   "$schema": "...",
 *   "Argument": {
 *     "type": "object",
 *     "properties": { ... }
 *   }
 * }
 * 
 * 출력:
 * {
 *   "$schema": "...",
 *   "type": "object",
 *   "title": "Argument",
 *   "properties": { ... }
 * }
 */
function unwrapRootKey(schema: any, transform: any): EnhancedSchema {
  const keys = Object.keys(schema);

  // $schema 키 제외하고 실제 wrapper key 찾기
  const wrapperKey = keys.find(k => k !== '$schema' && k !== 'description' && k !== 'x-ui' && k !== 'x-transport');

  if (!wrapperKey) {
    console.warn('⚠️ unwrapRootKey: No wrapper key found', keys);
    return schema as EnhancedSchema;
  }

  const innerSchema = schema[wrapperKey];

  // 내부 스키마가 유효한지 확인
  if (!innerSchema || typeof innerSchema !== 'object') {
    console.warn('⚠️ unwrapRootKey: Invalid inner schema for key:', wrapperKey);
    return schema as EnhancedSchema;
  }

  console.log(`✅ unwrapRootKey: Unwrapping "${wrapperKey}"`, innerSchema);

  // 메타데이터 보존 (transform.preserveMetadata에 정의된 키들)
  const preserveKeys = transform.preserveMetadata || ['description', 'x-ui', 'x-transport'];
  const preservedMetadata: Record<string, any> = {};
  for (const key of preserveKeys) {
    if (schema[key] !== undefined) {
      preservedMetadata[key] = schema[key];
    }
  }

  // 새로운 스키마 구성
  const result: EnhancedSchema = {
    ...innerSchema,
    ...preservedMetadata,
  };

  // $schema 보존
  if (schema.$schema) {
    result.$schema = schema.$schema;
  }

  // wrapper key를 title로 사용 (extractTitle: true인 경우)
  if (transform.extractTitle && !result.title) {
    result.title = wrapperKey;
  }

  return result;
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
  const assembledSchema = {
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
  } as unknown as EnhancedSchema;

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
        ...schema,
        type: wrapperType, // schema의 type을 wrapperType으로 덮어씀
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
 * 🎯 Helper: 필드에서 런타임 트리거 필드 추출
 * visibleWhen, x-required-when에서 사용되는 필드명들을 추출
 */
function extractRuntimeTriggers(prop: EnhancedProperty): string[] {
  const triggers = new Set<string>();

  // 1. x-uiRules.visibleWhen에서 추출 (새 철학)
  const xUiRules = (prop as any)['x-uiRules'];
  if (xUiRules?.visibleWhen && typeof xUiRules.visibleWhen === 'object') {
    for (const key of Object.keys(xUiRules.visibleWhen)) {
      triggers.add(key);
    }
  }

  // 2. x-ui.visibleWhen에서 추출 (레거시 호환)
  const xUi = (prop as any)['x-ui'];
  if (xUi?.visibleWhen && typeof xUi.visibleWhen === 'object') {
    for (const key of Object.keys(xUi.visibleWhen)) {
      triggers.add(key);
    }
  }

  // 3. x-required-when에서 추출 (레거시)
  const xRequiredWhen = (prop as any)['x-required-when'];
  if (xRequiredWhen && typeof xRequiredWhen === 'object') {
    for (const key of Object.keys(xRequiredWhen)) {
      triggers.add(key);
    }
  }

  return Array.from(triggers);
}

/**
 * 모든 필드 추출 (중첩 객체 포함)
 */
function extractFields(schema: EnhancedSchema): EnhancedField[] {
  const fields: EnhancedField[] = [];

  // 🔥 $defs/entity가 있으면 entity의 properties를 사용 (inject-entity-collection 변환 후)
  const schemaAny = schema as any;
  const entityDef = schemaAny.$defs?.entity;
  const propsSource: Record<string, EnhancedProperty> = (entityDef?.properties || schema.properties) as any;

  console.log('🔍 extractFields - has $defs.entity:', !!entityDef);
  console.log('🔍 extractFields - propsSource keys:', propsSource ? Object.keys(propsSource) : 'none');

  // 🔥 properties가 없으면 빈 배열 반환
  if (!propsSource) {
    return [];
  }

  // 🎯 allOf → x-required-when 정규화 맵 생성
  const conditionalRequiredMap = normalizeConditionalRequired(schema);

  for (const [key, prop] of Object.entries(propsSource)) {

    // 🔥 기본 필드 구조
    const field: EnhancedField = {
      key,
      type: prop.type,
      description: prop.description,
      default: prop.default,
      required: {},
      section: '',
      validationLayers: [],
      runtimeTriggers: extractRuntimeTriggers(prop), // 🎯 NEW: 트리거 필드 자동 추출
    };

    // 🔥 동적으로 모든 속성 복사 (x-*, enum, items 등)
    for (const [propKey, propValue] of Object.entries(prop)) {
      if (propKey === 'type' || propKey === 'description' || propKey === 'default') {
        continue; // 이미 처리됨
      }

      // x-ui는 ui로 변환
      if (propKey === 'x-ui') {
        field.ui = propValue as any;
      }
      // x-로 시작하는 필드는 그대로 유지
      else if (propKey.startsWith('x-')) {
        field[propKey] = propValue;
        // 🔥 디버그: x-optional-when 복사 확인
        if (propKey === 'x-optional-when') {
          console.log(`🔍 Copied x-optional-when for ${key}:`, propValue);
        }
      }
      // 그 외 표준 JSON Schema 필드들 (enum, items, minItems, maxItems 등)
      else {
        field[propKey] = propValue;
      }
    }

    // 🔥 oneOf → enum 변환 (const 값들을 enum으로 추출)
    if (prop.oneOf && Array.isArray(prop.oneOf) && !field.enum) {
      const enumValues: any[] = [];
      const enumLabels: Record<string, string> = {};

      for (const option of prop.oneOf) {
        if (option.const !== undefined) {
          enumValues.push(option.const);
          if (option.title) {
            enumLabels[String(option.const)] = option.title;
          }
        }
      }

      if (enumValues.length > 0) {
        field.enum = enumValues;
        if (Object.keys(enumLabels).length > 0) {
          field['x-enum-labels'] = enumLabels;
        }
        console.log(`✅ Converted oneOf → enum for ${key}:`, enumValues);
      }
    }


    // 🔥 x-uiRules.visibleWhen → x-optional-when 변환 (ui.visibleWhen은 사용하지 않음)
    // 조건부 표시 필드는 x-optional-when으로 통일
    const xUiRules = (prop as any)['x-uiRules'];
    if (xUiRules?.visibleWhen && !field['x-optional-when'] && !field['x-required-when']) {
      field['x-optional-when'] = xUiRules.visibleWhen;
      console.log(`✅ Converted x-uiRules.visibleWhen → x-optional-when for ${key}:`, xUiRules.visibleWhen);
    }

    // 🎯 allOf에서 추출한 조건부 required 주입
    if (conditionalRequiredMap[key]) {
      field['x-required-when'] = {
        ...(field['x-required-when'] ?? {}),
        ...conditionalRequiredMap[key],
      };
      console.log(`✅ Injected x-required-when for ${key}:`, field['x-required-when']);
    }

    // 🔥 Object 타입 - 중첩 필드 추출
    if (prop.type === 'object' && prop.properties) {
      field.children = [];
      const objRequired = (prop.required as string[]) || [];

      for (const [childKey, childProp] of Object.entries(prop.properties)) {
        const childField: EnhancedField = {
          key: `${key}.${childKey}`,
          type: (childProp as any).type,
          default: (childProp as any).default,
          required: objRequired.includes(childKey) ? { '*': 'required' } : { '*': 'optional' },
          section: '',
          validationLayers: [],
        };

        // 🔥 자식 필드도 동적으로 모든 속성 복사
        for (const [cpKey, cpValue] of Object.entries(childProp as any)) {
          if (cpKey === 'type' || cpKey === 'default') continue;

          if (cpKey === 'x-ui') {
            childField.ui = cpValue;
          } else if (cpKey.startsWith('x-')) {
            childField[cpKey] = cpValue;
          } else {
            childField[cpKey] = cpValue;
          }
        }

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
            type: (childProp as any).type,
            default: (childProp as any).default,
            required: optionRequired.includes(childKey) ? { '*': 'required' } : { '*': 'optional' },
            section: optionTitle,
            validationLayers: [],
          };

          // 🔥 동적으로 모든 속성 복사
          for (const [cpKey, cpValue] of Object.entries(childProp as any)) {
            if (cpKey === 'type' || cpKey === 'default') continue;

            if (cpKey === 'x-ui') {
              childField.ui = cpValue;
            } else if (cpKey.startsWith('x-')) {
              childField[cpKey] = cpValue;
            } else {
              childField[cpKey] = cpValue;
            }
          }

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
  const rules = schema.allOf.filter(rule =>
    rule &&
    typeof rule === 'object' &&
    'if' in rule &&
    'then' in rule
  );

  console.log('🔍 extractConditionalRequired:', {
    hasAllOf: !!schema.allOf,
    allOfLength: schema.allOf?.length,
    rulesExtracted: rules.length,
    rules: rules.map(r => ({
      ifCondition: r.if,
      thenRequired: r.then?.required
    }))
  });

  return rules;
}

/**
 * 🎯 allOf → x-required-when 정규화
 * 
 * JSON Schema의 allOf + if/then required를 x-required-when 형태로 변환
 * 
 * 예시:
 * allOf: [{ 
 *   if: { properties: { iMETHOD: { const: 4 } } },
 *   then: { required: ["REFTYPE", "G_DIR"] }
 * }]
 * 
 * →
 * {
 *   "REFTYPE": { "iMETHOD": 4 },
 *   "G_DIR": { "iMETHOD": 4 }
 * }
 * 
 * @param schema - JSON Schema
 * @returns 필드명 → 조건 맵
 */
function normalizeConditionalRequired(
  schema: EnhancedSchema
): Record<string, Record<string, any>> {
  const map: Record<string, Record<string, any>> = {};

  if (!schema.allOf || !Array.isArray(schema.allOf)) {
    return map;
  }

  for (const rule of schema.allOf) {
    const condProps = rule.if?.properties;
    const requiredFields = rule.then?.required;

    if (!condProps || !requiredFields || !Array.isArray(requiredFields)) {
      continue;
    }

    // 🔥 조건 축 필드 추출 (예: iMETHOD, TYPE, MODE 등)
    // allOf의 if.properties에서 첫 번째 조건을 가져옴
    const entries = Object.entries(condProps);
    if (entries.length === 0) continue;

    const [axisField, axisCond] = entries[0];
    const axisValue = (axisCond as any).const ?? (axisCond as any).enum;

    // 🔥 각 required 필드에 x-required-when 주입
    for (const fieldName of requiredFields) {
      if (!map[fieldName]) {
        map[fieldName] = {};
      }
      map[fieldName][axisField] = axisValue;
    }
  }

  console.log('🎯 normalizeConditionalRequired:', map);

  return map;
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

  // 🔥 TYPE 필드가 없는 스키마 (e.g., SKEW with iMETHOD)
  // → 트리거 필드 기반 조건부 required 확인
  if (types.length === 0) {
    console.log(`🔍 calculateRequiredStatus for ${field.key}:`, {
      types: types,
      conditionalRulesCount: conditionalRules.length,
      baseRequired: baseRequired,
      fieldKey: field.key,
      hasXRequiredWhen: !!(field as any)['x-required-when']
    });

    // ✅ Step 1: x-required-when 확인 (allOf → x-required-when 정규화 완료)
    // 이제 allOf의 조건도 x-required-when으로 변환되어 있음
    const xRequiredWhen = (field as any)['x-required-when'];
    if (xRequiredWhen && typeof xRequiredWhen === 'object') {
      // x-required-when이 있으면 조건부 required
      // 예: { "iMETHOD": 4 } → iMETHOD가 4일 때만 required
      console.log(`✅ ${field.key} is conditionally required via x-required-when:`, xRequiredWhen);
      return { '*': 'conditional' };
    }

    // ✅ Step 2: 기본 required 체크
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

  // 2. x-required-when 체크 (개별 필드 레벨, TYPE이 있는 경우)
  const xRequiredWhen = (field as any)['x-required-when'];
  if (xRequiredWhen && typeof xRequiredWhen === 'object') {
    // x-required-when의 조건을 확인
    // 예: { "TYPE": "1" } → TYPE이 "1"일 때만 required
    types.forEach(type => {
      status[type] = 'optional'; // 기본값

      // TYPE 조건 확인
      if (xRequiredWhen.TYPE) {
        const requiredForTypes = Array.isArray(xRequiredWhen.TYPE)
          ? xRequiredWhen.TYPE
          : [xRequiredWhen.TYPE];

        if (requiredForTypes.includes(type)) {
          status[type] = 'required';
        }
      }
    });

    // 적어도 하나의 TYPE에서 required이면 conditional로 표시
    const hasRequired = Object.values(status).some(s => s === 'required');
    const hasOptional = Object.values(status).some(s => s === 'optional');
    if (hasRequired && hasOptional) {
      // 일부 TYPE에서만 required → 모든 TYPE을 conditional로 변경
      types.forEach(type => {
        if (status[type] !== 'n/a') {
          status[type] = 'conditional';
        }
      });
    }
  } else {
    // 3. For each TYPE, check allOf conditions (x-required-when이 없는 경우)
    types.forEach(type => {
      status[type] = 'optional'; // default

      // Check conditional required from allOf
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
    });
  }

  // 4. Check visibleWhen (determines N/A)
  types.forEach(type => {
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
/**
 * 🎯 동적 조건 매칭
 * 
 * TYPE뿐만 아니라 iMETHOD, MODE, LOAD_TYPE 등 모든 VariantAxis 지원
 * 
 * @param condition - 조건 객체 (예: { TYPE: { const: "1" } } 또는 { iMETHOD: { const: 4 } })
 * @param axisValue - 비교할 값 (예: "1", 4)
 * @returns 조건이 일치하면 true
 */
function matchesCondition(
  condition: Record<string, { const?: any; enum?: any[] }> | undefined,
  axisValue: string | number
): boolean {
  if (!condition) return false;

  // 🔥 조건의 첫 번째 속성을 동적으로 가져옴 (TYPE, iMETHOD, MODE 등)
  const entries = Object.entries(condition);
  if (entries.length === 0) return false;

  const [_axisField, axisCond] = entries[0];

  // const 값과 비교
  if (axisCond.const !== undefined) {
    // 타입 정규화 (문자열 "4" vs 숫자 4)
    return String(axisCond.const) === String(axisValue);
  }

  // enum 배열과 비교
  if (axisCond.enum && Array.isArray(axisCond.enum)) {
    return axisCond.enum.some(v => String(v) === String(axisValue));
  }

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
    const sectionName = determineSectionNameDynamic(field as any, types, psdSet, schemaType, schema);
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
