/**
 * Schema to Builder UI Adapter
 * 
 * JSON Schema를 BuilderTab의 동적 폼으로 변환합니다.
 * 
 * Features:
 * - visibleWhen 조건부 필드 표시
 * - TYPE별 동적 enum 값 및 레이블
 * - TYPE별 동적 required 상태
 * - 섹션별 그룹화
 */

import { 
  compileEnhancedSchema, 
  type EnhancedSchema, 
  type EnhancedField, 
  type SectionGroup 
} from './schemaCompiler';

// ============================================================================
// UI Builder Field Type (from legacy uiSchemaAdapters)
// ============================================================================

export interface UIBuilderField {
  name: string;
  type: 'string' | 'number' | 'integer' | 'boolean' | 'enum' | 'array' | 'object';
  description?: string;
  required: boolean;
  default?: any;
  enum?: any[];
  placeholder?: string;
  items?: { type: string };
  children?: UIBuilderField[];
  oneOfOptions?: string[];
  optionIndex?: number;
  visible?: boolean;
  valueConstraint?: string;
}

// Re-export for convenience
export type { EnhancedSchema, EnhancedField, SectionGroup };

// ============================================================================
// Builder UI Types
// ============================================================================

export interface BuilderSection {
  name: string;
  fields: UIBuilderField[];
}

export interface BuilderFormState {
  sections: BuilderSection[];
  values: Record<string, any>;
  errors: Record<string, string>;
}

// ============================================================================
// Trigger Fields Extraction (Auto-detection)
// ============================================================================

/**
 * 스키마에서 visibleWhen 조건에 사용된 모든 트리거 필드를 자동으로 추출
 * 
 * @param schema JSON Schema
 * @param psdSet PSD 세트
 * @param schemaType 스키마 타입
 * @returns 트리거 필드 이름 배열 (중복 제거됨)
 * 
 * @example
 * // SKEW 엔티티의 경우
 * extractTriggerFields(schema) // => ['iMETHOD']
 * 
 * // ELEM 엔티티의 경우
 * extractTriggerFields(schema) // => ['TYPE', 'STYPE']
 */
export function extractTriggerFields(
  schema: EnhancedSchema,
  psdSet: string,
  schemaType: string
): string[] {
  const triggerFields = new Set<string>();
  
  // 스키마 컴파일하여 모든 필드 추출
  const sections = compileEnhancedSchema(schema, psdSet, schemaType);
  
  for (const section of sections) {
    for (const field of section.fields) {
      // x-ui.visibleWhen 조건 확인
      const visibleWhen = field.ui?.visibleWhen;
      if (visibleWhen && typeof visibleWhen === 'object') {
        // visibleWhen의 모든 키를 트리거 필드로 추가
        for (const key of Object.keys(visibleWhen)) {
          triggerFields.add(key);
        }
      }
      
      // 중첩 필드도 재귀적으로 확인
      if (field.children && Array.isArray(field.children)) {
        for (const child of field.children) {
          const childVisibleWhen = (child as any).ui?.visibleWhen;
          if (childVisibleWhen && typeof childVisibleWhen === 'object') {
            for (const key of Object.keys(childVisibleWhen)) {
              triggerFields.add(key);
            }
          }
        }
      }
    }
  }
  
  return Array.from(triggerFields);
}

// ============================================================================
// Main Adapter
// ============================================================================

/**
 * Schema를 Builder UI로 변환
 * 
 * @param schema JSON Schema
 * @param currentValues 현재 폼 값 (visibleWhen 평가에 사용)
 * @returns Builder 섹션 리스트
 */
export function adaptSchemaToBuilder(
  schema: EnhancedSchema,
  currentValues: Record<string, any> = {},
  psdSet: string,
  schemaType: string
): BuilderSection[] {
  const sections = compileEnhancedSchema(schema, psdSet, schemaType);
  
  // 🔥 schema.properties가 없을 수 있음 (단순 엔티티 정의)
  // TYPE 필드를 sections에서 찾아서 기본값 추출
  let typeDefaultValue = '';
  for (const section of sections) {
    const typeField = section.fields.find(f => f.key === 'TYPE');
    if (typeField?.default) {
      typeDefaultValue = typeField.default;
      break;
    }
  }
  
  const currentType = currentValues['TYPE'] || typeDefaultValue || '';
  
  return sections.map(section => ({
    name: section.name,
    fields: section.fields
      .map(field => adaptFieldToBuilder(field, currentType, currentValues))
      .filter(field => field.visible)
  }));
}

/**
 * Schema를 평탄화된 UIBuilderField 배열로 변환
 * (BuilderTab과 호환되도록)
 * 
 * @param schema JSON Schema
 * @param currentValues 현재 폼 값 (visibleWhen 평가에 사용)
 * @returns UIBuilderField 배열
 */
export function schemaToBuilderFields(
  schema: EnhancedSchema,
  currentValues: Record<string, any> = {},
  psdSet: string,
  schemaType: string
): UIBuilderField[] {
  // 🔥 inject-entity-collection 변환 후 구조 감지
  // properties가 $ref만 있고, $defs/entity가 있으면 entity 필드만 사용
  const schemaAny = schema as any;
  const hasEntityDef = schemaAny.$defs?.entity;
  const propsKeys = Object.keys(schema.properties || {});
  const hasOnlyRefProps = propsKeys.length === 1 && 
    schemaAny.properties?.[propsKeys[0]]?.$ref;
  
  console.log('🔍 Builder schemaToBuilderFields:', {
    hasEntityDef,
    propsKeys,
    hasOnlyRefProps,
    firstProp: schemaAny.properties?.[propsKeys[0]]
  });
  
  if (hasEntityDef && hasOnlyRefProps) {
    console.log('🔍 Builder: Detected entity-collection structure, using $defs/entity directly');
    console.log('🔍 Builder: Entity properties:', Object.keys(schemaAny.$defs.entity.properties || {}));
    // $defs/entity를 직접 스키마로 사용
    const entitySchema = {
      ...schema,
      properties: schemaAny.$defs.entity.properties,
      required: schemaAny.$defs.entity.required || []
    };
    const sections = adaptSchemaToBuilder(entitySchema as EnhancedSchema, currentValues, psdSet, schemaType);
    const fields: UIBuilderField[] = [];
    
    for (const section of sections) {
      if (section.name) {
        fields.push({
          name: `__section_${section.name}__`,
          type: 'object',
          description: section.name,
          required: false,
        } as UIBuilderField & { sectionHeader?: string });
      }
      fields.push(...section.fields);
    }
    
    return fields;
  }
  
  // 일반 스키마 처리
  const sections = adaptSchemaToBuilder(schema, currentValues, psdSet, schemaType);
  const fields: UIBuilderField[] = [];
  
  // 섹션을 평탄화하되, 섹션 헤더는 유지
  for (const section of sections) {
    // 섹션 헤더를 sectionHeader 필드로 추가 (BuilderTab이 인식할 수 있도록)
    if (section.name) {
      fields.push({
        name: `__section_${section.name}__`,
        type: 'object',
        description: section.name,
        required: false,
      } as UIBuilderField & { sectionHeader?: string });
    }
    
    // 섹션의 필드들 추가
    fields.push(...section.fields);
  }
  
  return fields;
}

// Legacy alias for backward compatibility
export const enhancedSchemaToBuilderFields = schemaToBuilderFields;

/**
 * Enhanced Field를 UIBuilderField로 변환
 */
function adaptFieldToBuilder(
  field: EnhancedField,
  currentType: string,
  currentValues: Record<string, any>
): UIBuilderField & { visible: boolean; valueConstraint?: string } {
  const visible = evaluateVisibility(field, currentType, currentValues);
  
  // 🔥 Required 상태 결정: TYPE별 또는 전체 required
  let required = false;
  if (typeof field.required === 'boolean') {
    // Simple boolean required
    required = field.required;
  } else if (typeof field.required === 'object' && field.required !== null) {
    // TYPE-dependent required (Enhanced Schema)
    if (currentType && field.required[currentType]) {
      required = field.required[currentType] === 'required';
    } else {
      // currentType이 없거나, 해당 TYPE의 정보가 없으면 기본값 확인
      const requiredValues = Object.values(field.required);
      required = requiredValues.some(v => v === 'required');
    }
  }
  
  // Map Enhanced Field type to UIBuilderField type
  let uiType: UIBuilderField['type'] = 'string';
  if (field.enum || field.enumByType) {
    uiType = 'enum';
  } else if (field.type === 'number' || field.type === 'integer') {
    uiType = field.type;
  } else if (field.type === 'boolean') {
    uiType = 'boolean';
  } else if (field.type === 'array') {
    uiType = 'array';
  } else if (field.type === 'object') {
    uiType = 'object';
  }
  
  const builderField: UIBuilderField & { visible: boolean; valueConstraint?: string } = {
    name: field.key,
    type: uiType,
    description: field.ui?.label || field.key,
    required,
    default: field.default,
    enum: buildEnumArray(field, currentType),
    placeholder: buildHint(field, currentType),
    items: field.type === 'array' ? { type: field.items?.type || 'any' } : undefined,
    visible,
    valueConstraint: buildValueConstraint(field, currentType)
  };
  
  // 🔥 중첩 객체의 children 전달
  if (field.children && field.children.length > 0) {
    // 🔥 oneOf 패턴 감지: 첫 번째 child가 section-header면 oneOf
    const hasOneOf = field.children.some(child => child.type === 'section-header');
    
    if (hasOneOf) {
      // oneOf의 경우: 섹션 헤더를 제외하고 실제 필드만 children으로 전달
      // UI에서는 라디오 버튼으로 섹션 선택 가능하게 함
      let currentOptionIndex = -1;
      const childrenWithOptionIndex: any[] = [];
      
      field.children.forEach(child => {
        if (child.type === 'section-header') {
          currentOptionIndex++;
        } else {
          const adaptedChild = adaptFieldToBuilder(child, currentType, currentValues) as any;
          adaptedChild.optionIndex = currentOptionIndex; // 🔥 옵션 인덱스 추가
          childrenWithOptionIndex.push(adaptedChild);
        }
      });
      
      builderField.children = childrenWithOptionIndex;
      
      // oneOf 옵션 정보 추출 (섹션 제목들)
      const oneOfOptions: string[] = [];
      field.children.forEach(child => {
        if (child.type === 'section-header' && child.section) {
          oneOfOptions.push(child.section);
        }
      });
      builderField.oneOfOptions = oneOfOptions;
    } else {
      // 일반 중첩 객체
      builderField.children = field.children.map(child => 
        adaptFieldToBuilder(child, currentType, currentValues)
      );
    }
  }
  
  return builderField;
}

// ============================================================================
// Visibility Logic
// ============================================================================

/**
 * visibleWhen 조건 평가
 */
function evaluateVisibility(
  field: EnhancedField,
  currentType: string,
  currentValues: Record<string, any>
): boolean {
  const visibleWhen = field.ui?.visibleWhen;
  if (!visibleWhen) return true;
  
  // Check TYPE condition
  if (visibleWhen.TYPE) {
    const allowedTypes = Array.isArray(visibleWhen.TYPE) 
      ? visibleWhen.TYPE 
      : [visibleWhen.TYPE];
    
    if (!allowedTypes.includes(currentType)) {
      return false;
    }
  }
  
  // Check other field conditions
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
// Enum Building
// ============================================================================

/**
 * Enum 값을 배열로 변환 (UIBuilderField.enum 형식)
 */
function buildEnumArray(
  field: EnhancedField,
  currentType: string
): any[] | undefined {
  // Type별 enum (우선순위 높음)
  if (field.enumByType && currentType) {
    const values = field.enumByType[currentType];
    if (values) {
      return values;
    }
  }
  
  // Standard enum
  if (field.enum) {
    return field.enum;
  }
  
  return undefined;
}


// ============================================================================
// Hint Building
// ============================================================================

/**
 * 힌트 메시지 생성 (설명 + 제약사항)
 */
function buildHint(field: EnhancedField, currentType: string): string | undefined {
  const hints: string[] = [];
  
  // UI hint
  if (field.ui?.hint) {
    hints.push(field.ui.hint);
  }
  
  // Array constraints
  if (field.type === 'array') {
    if (field.nodeCountByType && currentType) {
      const count = field.nodeCountByType[currentType];
      if (count !== undefined) {
        const countStr = Array.isArray(count) ? count.join(' or ') : count;
        hints.push(`${currentType}: ${countStr} nodes`);
      }
    } else if (field.minItems !== undefined || field.maxItems !== undefined) {
      hints.push(`Items: ${field.minItems || 0} ~ ${field.maxItems || '∞'}`);
    }
  }
  
  return hints.length > 0 ? hints.join(' | ') : undefined;
}

/**
 * Value constraint 메시지 생성
 */
function buildValueConstraint(field: EnhancedField, currentType: string): string | undefined {
  if (!field.valueConstraint) return undefined;
  
  // Type별 constraint 확인
  if (currentType && field.valueConstraint[currentType]) {
    return field.valueConstraint[currentType];
  }
  
  // 전체 constraint 문자열 반환
  return Object.entries(field.valueConstraint)
    .map(([type, constraint]) => `${type}: ${constraint}`)
    .join(' | ');
}


// ============================================================================
// Form State Helpers
// ============================================================================

/**
 * 초기 폼 상태 생성
 */
export function createInitialFormState(
  schema: EnhancedSchema,
  psdSet: string = 'civil_gen_definition',
  schemaType: string = 'enhanced'
): BuilderFormState {
  const sections = compileEnhancedSchema(schema, psdSet, schemaType);
  const values: Record<string, any> = {};
  
  // Collect default values
  for (const section of sections) {
    for (const field of section.fields) {
      if (field.default !== undefined) {
        values[field.key] = field.default;
      }
    }
  }
  
  // Build builder sections with initial visibility
  const builderSections = adaptSchemaToBuilder(schema, values, psdSet, schemaType);
  
  return {
    sections: builderSections,
    values,
    errors: {}
  };
}

/**
 * 값 변경 핸들러 (visibility 재계산)
 */
export function handleValueChange(
  schema: EnhancedSchema,
  currentState: BuilderFormState,
  key: string,
  value: any,
  psdSet: string = 'civil_gen_definition',
  schemaType: string = 'enhanced'
): BuilderFormState {
  const newValues = { ...currentState.values, [key]: value };
  const newSections = adaptSchemaToBuilder(schema, newValues, psdSet, schemaType);
  
  return {
    sections: newSections,
    values: newValues,
    errors: validateFormState(schema, newValues)
  };
}

// ============================================================================
// Validation
// ============================================================================

/**
 * 폼 상태 검증 (Standard + APP 레이어)
 */
export function validateFormState(
  schema: EnhancedSchema,
  values: Record<string, any>,
  psdSet: string = 'civil_gen_definition',
  schemaType: string = 'enhanced'
): Record<string, string> {
  const errors: Record<string, string> = {};
  const sections = compileEnhancedSchema(schema, psdSet, schemaType);
  const currentType = values['TYPE'] || '';
  
  for (const section of sections) {
    for (const field of section.fields) {
      // Check visibility first
      const visible = evaluateVisibility(field, currentType, values);
      if (!visible) continue;
      
      const value = values[field.key];
      const required = field.required[currentType] === 'required';
      
      // Required check
      if (required && (value === undefined || value === null || value === '')) {
        errors[field.key] = `${field.ui?.label || field.key} is required`;
        continue;
      }
      
      // Type check
      if (value !== undefined && value !== null && value !== '') {
        const typeError = validateType(field, value);
        if (typeError) {
          errors[field.key] = typeError;
          continue;
        }
      }
      
      // Enum check (Standard validation)
      if (field.enum && !field.enum.includes(value)) {
        errors[field.key] = `Invalid value. Must be one of: ${field.enum.join(', ')}`;
        continue;
      }
      
      // Enum by type check (APP validation)
      if (field.enumByType && currentType) {
        const allowedValues = field.enumByType[currentType];
        if (allowedValues && !allowedValues.includes(value)) {
          errors[field.key] = `Invalid value for ${currentType}. Must be one of: ${allowedValues.join(', ')}`;
          continue;
        }
      }
      
      // Array constraints
      if (field.type === 'array' && Array.isArray(value)) {
        const arrayError = validateArray(field, value, currentType);
        if (arrayError) {
          errors[field.key] = arrayError;
        }
      }
      
      // Value constraints (APP validation - warning only)
      if (field.valueConstraint && currentType && field.valueConstraint[currentType]) {
        // Don't block submission, just show warning in UI
        // This is handled by the valueConstraint field in BuilderField
      }
    }
  }
  
  return errors;
}

function validateType(field: EnhancedField, value: any): string | null {
  switch (field.type) {
    case 'number':
    case 'integer':
      if (typeof value !== 'number' && isNaN(Number(value))) {
        return `${field.ui?.label || field.key} must be a number`;
      }
      break;
    case 'boolean':
      if (typeof value !== 'boolean') {
        return `${field.ui?.label || field.key} must be a boolean`;
      }
      break;
    case 'array':
      if (!Array.isArray(value)) {
        return `${field.ui?.label || field.key} must be an array`;
      }
      break;
  }
  return null;
}

function validateArray(
  field: EnhancedField,
  value: any[],
  currentType: string
): string | null {
  // Node count by type (APP validation)
  if (field.nodeCountByType && currentType) {
    const expectedCount = field.nodeCountByType[currentType];
    if (expectedCount !== undefined) {
      if (Array.isArray(expectedCount)) {
        if (!expectedCount.includes(value.length)) {
          return `${field.ui?.label || field.key} must have ${expectedCount.join(' or ')} items for ${currentType}`;
        }
      } else {
        if (value.length !== expectedCount) {
          return `${field.ui?.label || field.key} must have exactly ${expectedCount} items for ${currentType}`;
        }
      }
    }
  }
  
  // Standard minItems/maxItems validation
  if (field.minItems !== undefined && value.length < field.minItems) {
    return `${field.ui?.label || field.key} must have at least ${field.minItems} items`;
  }
  
  if (field.maxItems !== undefined && value.length > field.maxItems) {
    return `${field.ui?.label || field.key} must have at most ${field.maxItems} items`;
  }
  
  return null;
}

// ============================================================================
// Export Helpers
// ============================================================================

/**
 * Builder 폼에서 Clean JSON 생성
 */
export function buildCleanJSON(
  schema: EnhancedSchema,
  values: Record<string, any>,
  psdSet: string = 'civil_gen_definition',
  schemaType: string = 'enhanced'
): Record<string, any> {
  const sections = compileEnhancedSchema(schema, psdSet, schemaType);
  const currentType = values['TYPE'] || '';
  const result: Record<string, any> = {};
  
  for (const section of sections) {
    for (const field of section.fields) {
      // Check visibility
      const visible = evaluateVisibility(field, currentType, values);
      if (!visible) continue;
      
      const value = values[field.key];
      
      // Skip undefined/null/empty values (unless it's a valid falsy value like 0 or false)
      if (value === undefined || value === null) continue;
      if (value === '' && field.type !== 'string') continue;
      
      // Include value
      result[field.key] = value;
    }
  }
  
  return result;
}

