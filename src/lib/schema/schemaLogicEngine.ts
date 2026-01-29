/**
 * Schema Logic Engine
 * 
 * YAML로 정의된 스키마 처리 규칙을 읽어서 동적으로 실행합니다.
 * 이제 새로운 섹션 규칙이나 visibility 규칙을 추가할 때 코드 수정 없이
 * YAML 파일만 수정하면 됩니다.
 * 
 * YAML은 앱 시작 시 한 번 로드되어 캐시되므로 동기적으로 사용 가능합니다.
 */

import yaml from 'js-yaml';

// ============================================================================
// Type Definitions
// ============================================================================

export interface SchemaLogicRules {
  platformSkeleton?: PlatformSkeleton;
  schemaStructurePatterns?: SchemaStructurePattern[];
  sectionRules: SectionRule[];
  sectionOrder: string[];
  typeLabels: Record<string, string>;
  visibilityRules: VisibilityRule[];
  requiredCalculationRules: any;
  validationLayerRules: any;
  descriptionBuildingRules: any;
}

export interface PlatformSkeleton {
  entityCollection: {
    description: string;
    type: string;
    patternProperties: {
      pattern: string;
      description: string;
      ref: string;
    };
    additionalProperties: boolean;
    minProperties?: number;
  };
  defaultBodyRoot: string;
  alternativeBodyRoots?: string[];
  rootStructure: {
    type: string;
    additionalProperties: boolean;
  };
}

export interface SchemaStructurePattern {
  name: string;
  description: string;
  enabled: boolean;
  detect: PatternDetectRule[];
  transform: PatternTransform;
}

export interface PatternDetectRule {
  path: string;
  exists?: boolean;
  isArray?: boolean;
  value?: any;
}

export interface PatternTransform {
  action: string;
  wrapperKey?: string;
  wrapperType?: string;
  preserveMetadata?: string[];
  strategy?: string;
  optionSeparator?: string;
}

export interface SectionRule {
  name: string;
  description: string;
  condition: RuleCondition;
  action: RuleAction;
}

export interface RuleCondition {
  type: string;
  types?: string[];
  includes?: string[];
  excludes?: string[];
  fields?: Record<string, any>;
}

export interface RuleAction {
  type: string;
  section?: string;
  separator?: string;
}

export interface VisibilityRule {
  name: string;
  description: string;
  condition: RuleCondition;
  result?: boolean;
  evaluation?: any;
}

// ============================================================================
// Cache & Initialization
// ============================================================================

const cachedRulesMap = new Map<string, SchemaLogicRules>();
const loadingMap = new Map<string, Promise<SchemaLogicRules>>();

/**
 * YAML 규칙 로드 (캐시 지원, 2-Level)
 * @param psdSet - PSD 세트 (Level 1) - 제품의 psd_set에서 가져옴
 * @param schemaType - 스키마 타입 (Level 2) - 'original' | 'enhanced'
 */
export async function initSchemaLogicRules(
  psdSet: string,
  schemaType: string,
  forceReload: boolean = false
): Promise<SchemaLogicRules> {
  const cacheKey = `${psdSet}/${schemaType}`;

  // 🔥 개발 중 캐시 우회: URL에 ?nocache 있으면 강제 리로드
  const urlParams = new URLSearchParams(window.location.search);
  const noCache = urlParams.has('nocache') || forceReload;

  if (cachedRulesMap.has(cacheKey) && !noCache) {
    return cachedRulesMap.get(cacheKey)!;
  }

  // 🔥 캐시 무효화
  if (noCache && cachedRulesMap.has(cacheKey)) {
    console.log(`🔄 Force reloading shared.yaml (schema logic) for ${cacheKey}`);
    cachedRulesMap.delete(cacheKey);
  }

  if (loadingMap.has(cacheKey)) {
    return loadingMap.get(cacheKey)!;
  }

  const loadPromise = (async () => {
    try {
      // 🔥 NEW: shared.yaml에서 schema logic 규칙 로드 (구 schema-logic.yaml 통합)
      const path = `/schema_definitions/${psdSet}/${schemaType}/shared.yaml`;
      const response = await fetch(path);
      if (!response.ok) {
        throw new Error(`Failed to load ${path}: ${response.statusText}`);
      }

      const yamlText = await response.text();
      const rules = yaml.load(yamlText) as SchemaLogicRules;

      cachedRulesMap.set(cacheKey, rules);
      loadingMap.delete(cacheKey);

      console.log(`✅ Loaded schema logic from ${cacheKey}/shared.yaml`);
      return rules;
    } catch (error) {
      console.error(`❌ Failed to load schema logic rules from ${cacheKey}:`, error);
      // Fallback to empty rules
      const fallbackRules: SchemaLogicRules = {
        sectionRules: [],
        sectionOrder: [],
        typeLabels: {},
        visibilityRules: [],
        requiredCalculationRules: {},
        validationLayerRules: {},
        descriptionBuildingRules: {}
      };
      cachedRulesMap.set(cacheKey, fallbackRules);
      loadingMap.delete(cacheKey);
      return fallbackRules;
    }
  })();

  loadingMap.set(cacheKey, loadPromise);
  return loadPromise;
}

/**
 * 동기적으로 규칙 가져오기 (초기화 후 사용)
 * 
 * 우선순위:
 * 1. schema['x-schema-rules'] (스키마 내부 정의) ← 최우선
 * 2. YAML 파일 (플랫폼 공통 fallback)
 * 
 * @param psdSet - PSD 세트 (Level 1) - 제품의 psd_set에서 가져옴
 * @param schemaType - 스키마 타입 (Level 2) - 'original' | 'enhanced'
 * @param schema - 선택적: 스키마 자체 (x-schema-rules 추출용)
 */
export function getSchemaLogicRules(
  psdSet: string,
  schemaType: string,
  schema?: any
): SchemaLogicRules {
  const cacheKey = `${psdSet}/${schemaType}`;
  const baseRules = cachedRulesMap.get(cacheKey);
  if (!baseRules) {
    throw new Error(`Schema logic rules not initialized for ${cacheKey}. Call initSchemaLogicRules() first.`);
  }

  // 🎯 스키마 자체에 x-schema-rules가 있으면 병합 (스키마 우선)
  if (schema?.['x-schema-rules']) {
    const schemaRules = schema['x-schema-rules'];
    console.log('📋 Using schema-specific rules from x-schema-rules', schemaRules);

    // 🔥 방어: 각 필드가 올바른 타입인지 확인
    const mergedRules: SchemaLogicRules = {
      ...baseRules,
      // platformSkeleton과 schemaStructurePatterns는 항상 YAML에서
      platformSkeleton: baseRules.platformSkeleton,
      schemaStructurePatterns: baseRules.schemaStructurePatterns,
      // 나머지는 스키마 우선, 없으면 base 사용
      sectionRules: Array.isArray(schemaRules.sectionRules)
        ? schemaRules.sectionRules
        : baseRules.sectionRules,
      sectionOrder: Array.isArray(schemaRules.sectionOrder)
        ? schemaRules.sectionOrder
        : baseRules.sectionOrder,
      typeLabels: schemaRules.typeLabels || baseRules.typeLabels,
      visibilityRules: Array.isArray(schemaRules.visibilityRules)
        ? schemaRules.visibilityRules
        : baseRules.visibilityRules,
      requiredCalculationRules: schemaRules.requiredCalculationRules || baseRules.requiredCalculationRules,
      validationLayerRules: schemaRules.validationLayerRules || baseRules.validationLayerRules,
      descriptionBuildingRules: schemaRules.descriptionBuildingRules || baseRules.descriptionBuildingRules
    };

    return mergedRules;
  }

  return baseRules;
}

/**
 * 캐시 초기화
 */
export function clearSchemaLogicCache(): void {
  cachedRulesMap.clear();
  loadingMap.clear();
}

// ============================================================================
// Section Determination (동기)
// ============================================================================

export interface FieldContext {
  ui?: {
    group?: string;
    visibleWhen?: {
      TYPE?: string | string[];
      [key: string]: any;
    };
  };
}

/**
 * 필드의 섹션 이름 결정 (YAML 규칙 기반, 동기)
 */
export function determineSectionName(
  field: FieldContext,
  _allTypes: string[],
  psdSet: string,
  schemaType: string,
  schema?: any
): string {
  const rules = getSchemaLogicRules(psdSet, schemaType, schema);

  // 🔥 방어: sectionRules가 배열인지 확인
  if (!Array.isArray(rules.sectionRules)) {
    console.error('❌ sectionRules is not an array:', rules.sectionRules);
    return 'General';
  }

  // 규칙을 순서대로 평가
  for (const rule of rules.sectionRules) {
    if (evaluateSectionCondition(rule.condition, field)) {
      return executeSectionAction(rule.action, field, rules.typeLabels);
    }
  }

  // Fallback (shouldn't reach here if YAML has "always" rule)
  return 'Advanced';
}

/**
 * 섹션 규칙 조건 평가
 */
function evaluateSectionCondition(condition: RuleCondition, field: FieldContext): boolean {
  // 🔥 condition이 없거나 undefined면 false 반환
  if (!condition || !condition.type) {
    console.warn('⚠️ Invalid condition:', condition);
    return false;
  }

  const { type } = condition;
  const visibleWhen = field.ui?.visibleWhen;

  switch (type) {
    case 'always':
      // 항상 매칭 (Original 스키마 기본값)
      return true;

    case 'has-explicit-group':
      return !!field.ui?.group;

    case 'no-visible-when':
      return !visibleWhen;

    case 'visible-types-include-all': {
      if (!visibleWhen?.TYPE) return false;
      const visibleTypes = Array.isArray(visibleWhen.TYPE) ? visibleWhen.TYPE : [visibleWhen.TYPE];
      return condition.types?.every(t => visibleTypes.includes(t)) ?? false;
    }

    case 'visible-types-match': {
      if (!visibleWhen?.TYPE) return false;
      const visibleTypes = Array.isArray(visibleWhen.TYPE) ? visibleWhen.TYPE : [visibleWhen.TYPE];

      // Check includes
      if (condition.includes) {
        const hasIncludes = condition.includes.every(t => visibleTypes.includes(t));
        if (!hasIncludes) return false;
      }

      // Check excludes
      if (condition.excludes) {
        const hasExcludes = condition.excludes.some(t => visibleTypes.includes(t));
        if (hasExcludes) return false;
      }

      // Check field conditions
      if (condition.fields) {
        for (const [key, value] of Object.entries(condition.fields)) {
          if (visibleWhen[key] !== value) {
            return false;
          }
        }
      }

      return true;
    }

    case 'visible-types-include-any': {
      if (!visibleWhen?.TYPE) return false;
      const visibleTypes = Array.isArray(visibleWhen.TYPE) ? visibleWhen.TYPE : [visibleWhen.TYPE];
      return condition.types?.some(t => visibleTypes.includes(t)) ?? false;
    }

    case 'visible-types-exact': {
      if (!visibleWhen?.TYPE) return false;
      const visibleTypes = Array.isArray(visibleWhen.TYPE) ? visibleWhen.TYPE : [visibleWhen.TYPE];
      return (
        visibleTypes.length === condition.types?.length &&
        condition.types.every(t => visibleTypes.includes(t))
      );
    }

    case 'visible-types-multiple': {
      if (!visibleWhen?.TYPE) return false;
      const visibleTypes = Array.isArray(visibleWhen.TYPE) ? visibleWhen.TYPE : [visibleWhen.TYPE];
      return visibleTypes.length > 1;
    }

    default:
      return false;
  }
}

/**
 * 섹션 액션 실행
 */
function executeSectionAction(
  action: RuleAction,
  field: FieldContext,
  typeLabels: Record<string, string>
): string {
  const { type } = action;

  switch (type) {
    case 'use-explicit-group':
      return field.ui?.group || 'Advanced';

    case 'assign-section':
      return action.section || 'Advanced';

    case 'join-type-labels': {
      const visibleWhen = field.ui?.visibleWhen;
      if (!visibleWhen?.TYPE) return 'Advanced';

      const visibleTypes = Array.isArray(visibleWhen.TYPE) ? visibleWhen.TYPE : [visibleWhen.TYPE];
      const labels = visibleTypes.map(t => typeLabels[t] || t);
      return labels.join(action.separator || ', ');
    }

    default:
      return 'Advanced';
  }
}

// ============================================================================
// Section Sorting (동기)
// ============================================================================

/**
 * 섹션 정렬 순서 가져오기 (YAML 규칙 기반, 동기)
 */
export function getSectionOrder(
  psdSet: string,
  schemaType: string,
  schema?: any
): string[] {
  const rules = getSchemaLogicRules(psdSet, schemaType, schema);
  return rules.sectionOrder || [];
}

/**
 * Schema Structure Patterns 가져오기 (YAML 규칙 기반, 동기)
 */
export function getSchemaStructurePatterns(
  psdSet: string,
  schemaType: string
): SchemaStructurePattern[] {
  const rules = getSchemaLogicRules(psdSet, schemaType);
  return rules.schemaStructurePatterns || [];
}

/**
 * Platform Skeleton 가져오기 (YAML 규칙 기반, 동기)
 */
export function getPlatformSkeleton(
  psdSet: string,
  schemaType: string
): PlatformSkeleton {
  const rules = getSchemaLogicRules(psdSet, schemaType);

  // 기본값 제공 (YAML에 없을 경우)
  const defaultSkeleton: PlatformSkeleton = {
    entityCollection: {
      description: "Platform Standard Collection Map",
      type: "object",
      patternProperties: {
        pattern: "^[0-9]+$",
        description: "Entity ID (numeric)",
        ref: "#/$defs/entity"
      },
      additionalProperties: false,
      minProperties: 0
    },
    defaultBodyRoot: "Assign",
    alternativeBodyRoots: ["Argument", "Assign"],
    rootStructure: {
      type: "object",
      additionalProperties: false
    }
  };

  return rules.platformSkeleton || defaultSkeleton;
}

/**
 * Type Label 가져오기 (YAML 규칙 기반, 동기)
 * @deprecated Use getSchemaLogicRules(psdSet, schemaType).typeLabels instead
 */
export function getTypeLabel(
  type: string,
  psdSet: string,
  schemaType: string
): string {
  const rules = getSchemaLogicRules(psdSet, schemaType);
  return rules.typeLabels[type] || type;
}

// ============================================================================
// Visibility Evaluation (동기)
// ============================================================================

/**
 * visibleWhen 조건 평가 (YAML 규칙 기반, 동기)
 */
export function evaluateVisibility(
  visibleWhen: any | undefined,
  currentType: string,
  currentValues: Record<string, any>
): boolean {
  // visibleWhen이 없으면 항상 표시
  if (!visibleWhen) return true;

  // TYPE 조건 확인
  if (visibleWhen.TYPE) {
    const allowedTypes = Array.isArray(visibleWhen.TYPE)
      ? visibleWhen.TYPE
      : [visibleWhen.TYPE];

    if (!allowedTypes.includes(currentType)) {
      return false;
    }
  }

  // 다른 필드 조건 확인
  for (const [key, expectedValue] of Object.entries(visibleWhen)) {
    if (key === 'TYPE') continue;

    const currentValue = currentValues[key];

    if (Array.isArray(expectedValue)) {
      if (!expectedValue.includes(currentValue)) {
        return false;
      }
    } else {
      if (currentValue !== expectedValue) {
        return false;
      }
    }
  }

  return true;
}

// ============================================================================
// Validation Layer Determination (동기)
// ============================================================================

export interface FieldForValidation {
  enum?: any[];
  minItems?: number;
  maxItems?: number;
  type?: string;
  enumByType?: any;
  nodeCountByType?: any;
  valueConstraint?: any;
  [key: string]: any;
}

/**
 * Validation Layer 결정 (YAML 규칙 기반, 동기)
 */
export function determineValidationLayers(
  field: FieldForValidation,
  psdSet: string,
  schemaType: string,
  schema?: any
): string[] {
  const rules = getSchemaLogicRules(psdSet, schemaType, schema);
  const layers: string[] = [];

  // STD layer check
  const stdTriggers = rules.validationLayerRules?.STD?.triggers || [];
  for (const trigger of stdTriggers) {
    if (checkTrigger(trigger, field)) {
      layers.push('STD');
      break;
    }
  }

  // APP layer check
  const appTriggers = rules.validationLayerRules?.APP?.triggers || [];
  for (const trigger of appTriggers) {
    if (checkTrigger(trigger, field)) {
      layers.push('APP');
      break;
    }
  }

  return layers;
}

function checkTrigger(trigger: string, field: FieldForValidation): boolean {
  switch (trigger) {
    case 'has-enum':
      return !!field.enum;
    case 'has-minItems':
      return field.minItems !== undefined;
    case 'has-maxItems':
      return field.maxItems !== undefined;
    case 'has-type':
      return !!field.type;
    case 'has-enumByType':
      return !!field.enumByType;
    case 'has-nodeCountByType':
      return !!field.nodeCountByType;
    case 'has-valueConstraint':
      return !!field.valueConstraint;
    case 'has-custom-x-field':
      return Object.keys(field).some(k => k.startsWith('x-') && !['x-ui', 'x-transport'].includes(k));
    default:
      return false;
  }
}

// ============================================================================
// Export All
// ============================================================================

export default {
  initSchemaLogicRules,
  getSchemaLogicRules,
  clearSchemaLogicCache,
  determineSectionName,
  getSectionOrder,
  getTypeLabel,
  evaluateVisibility,
  determineValidationLayers,
};

