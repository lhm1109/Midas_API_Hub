/**
 * Enhanced Schema Compiler (YAML-based)
 * 
 * Enhanced JSON Schema (x-* extensions)를 처리하여
 * 동적 섹션, 조건부 필드, TYPE별 enum 등을 지원합니다.
 * 
 * 이제 모든 로직이 YAML로 정의되어 있습니다!
 * 새로운 규칙 추가 시 schema-logic.yaml만 수정하면 됩니다.
 * 
 * @see api_docs_zendesk/schema_to_table_algorithm.md
 * @see schema_definitions/enhanced/schema-logic.yaml
 */

import {
  determineSectionName as determineSectionNameDynamic,
  getSectionOrder,
  determineValidationLayers as determineValidationLayersDynamic,
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
}

export interface EnhancedProperty {
  type: string;
  enum?: (string | number)[];
  default?: any;
  minItems?: number;
  maxItems?: number;
  items?: { type: string; properties?: Record<string, any> };
  
  // Object type - nested properties
  properties?: Record<string, EnhancedProperty>;  // 🔥 중첩 객체 지원
  required?: string[];  // 🔥 중첩 객체의 required 필드
  
  // Enhanced extensions
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
}

export interface ConditionalRule {
  if: {
    properties: Record<string, { const?: string; enum?: string[] }>;
  };
  then: {
    required: string[];
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
 * Enhanced JSON Schema를 섹션별 필드 그룹으로 컴파일 (YAML-based, 동기)
 * 
 * YAML 규칙이 초기화되어 있어야 합니다 (initSchemaLogicRules 호출 후 사용)
 */
export function compileEnhancedSchema(schema: EnhancedSchema): SectionGroup[] {
  // 🔥 스키마 유효성 검사
  if (!schema || !schema.properties) {
    console.warn('⚠️ Invalid schema: missing properties', schema);
    return [];
  }
  
  // Phase 1: Extract basic info
  const types = extractTypes(schema);
  const fields = extractFields(schema);
  const conditionalRules = extractConditionalRequired(schema);
  
  // Phase 2: Calculate required status for each field
  const fieldsWithStatus = fields.map(field => {
    const layers = determineValidationLayersDynamic(field);
    return {
      ...field,
      required: calculateRequiredStatus(field, types, schema.required || [], conditionalRules),
      validationLayers: layers as ValidationLayer[],
    };
  });
  
  // Phase 3: Group by sections (YAML-based, 동기)
  const sections = groupFieldsBySectionsDynamic(fieldsWithStatus, types);
  
  // Phase 4: Sort sections (YAML-based, 동기)
  return sortSectionsDynamic(sections);
}

// ============================================================================
// Phase 1: Schema Analysis
// ============================================================================

/**
 * TYPE 필드에서 가능한 타입 값 추출
 */
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
  return schema.allOf || [];
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
      if (matchesCondition(rule.if.properties, type) && 
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
  condition: Record<string, { const?: string; enum?: string[] }>,
  type: string
): boolean {
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
  types: string[]
): Map<string, EnhancedField[]> {
  const sections = new Map<string, EnhancedField[]>();
  
  for (const field of fields) {
    const sectionName = determineSectionNameDynamic(field, types);
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
function sortSectionsDynamic(sections: Map<string, EnhancedField[]>): SectionGroup[] {
  const sectionOrder = getSectionOrder();
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

// Note: determineValidationLayersDynamic is used directly in compileEnhancedSchema (line 125)
// This wrapper function is kept for future use if needed
