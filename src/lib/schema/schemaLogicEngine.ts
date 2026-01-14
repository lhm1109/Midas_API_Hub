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
  sectionRules: SectionRule[];
  sectionOrder: string[];
  typeLabels: Record<string, string>;
  visibilityRules: VisibilityRule[];
  requiredCalculationRules: any;
  validationLayerRules: any;
  descriptionBuildingRules: any;
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

let cachedRules: SchemaLogicRules | null = null;
let isLoading = false;
let loadPromise: Promise<SchemaLogicRules> | null = null;

/**
 * YAML 규칙 로드 (캐시 지원, 동기 접근 가능)
 */
export async function initSchemaLogicRules(): Promise<SchemaLogicRules> {
  if (cachedRules) {
    return cachedRules;
  }
  
  if (isLoading && loadPromise) {
    return loadPromise;
  }
  
  isLoading = true;
  loadPromise = (async () => {
    try {
      const response = await fetch('/schema_definitions/enhanced/schema-logic.yaml');
      if (!response.ok) {
        throw new Error(`Failed to load schema-logic.yaml: ${response.statusText}`);
      }
      
      const yamlText = await response.text();
      cachedRules = yaml.load(yamlText) as SchemaLogicRules;
      
      return cachedRules;
    } catch (error) {
      console.error('Failed to load schema logic rules:', error);
      // Fallback to empty rules
      cachedRules = {
        sectionRules: [],
        sectionOrder: [],
        typeLabels: {},
        visibilityRules: [],
        requiredCalculationRules: {},
        validationLayerRules: {},
        descriptionBuildingRules: {}
      };
      return cachedRules;
    } finally {
      isLoading = false;
    }
  })();
  
  return loadPromise;
}

/**
 * 동기적으로 규칙 가져오기 (초기화 후 사용)
 */
export function getSchemaLogicRules(): SchemaLogicRules {
  if (!cachedRules) {
    throw new Error('Schema logic rules not initialized. Call initSchemaLogicRules() first.');
  }
  return cachedRules;
}

/**
 * 캐시 초기화
 */
export function clearSchemaLogicCache(): void {
  cachedRules = null;
  isLoading = false;
  loadPromise = null;
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
  _allTypes: string[]
): string {
  const rules = getSchemaLogicRules();
  
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
  const { type } = condition;
  const visibleWhen = field.ui?.visibleWhen;
  
  switch (type) {
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
    
    case 'always':
      return true;
    
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
export function getSectionOrder(): string[] {
  const rules = getSchemaLogicRules();
  return rules.sectionOrder || [];
}

/**
 * Type Label 가져오기 (YAML 규칙 기반, 동기)
 */
export function getTypeLabel(type: string): string {
  const rules = getSchemaLogicRules();
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
  field: FieldForValidation
): string[] {
  const rules = getSchemaLogicRules();
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

