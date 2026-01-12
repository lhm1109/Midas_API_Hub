/**
 * Schema Compiler - JSON Schema를 정규화된 Canonical AST로 변환
 * 
 * 목적:
 * - allOf, if/then 같은 조건부 로직을 평탄화
 * - 중첩 구조(Argument, nested objects)를 정규화
 * - UI가 직접 JSON Schema를 해석하지 않도록 추상화
 */

export type CanonicalType = 
  | 'string' 
  | 'number' 
  | 'integer' 
  | 'boolean' 
  | 'array' 
  | 'object'
  | 'array[string]'
  | 'array[number]'
  | 'array[object]';

export interface CanonicalCondition {
  when: string;           // "ITERATION_METHOD = NEWTON"
  field: string;          // "ITERATION_METHOD"
  value: any;             // "NEWTON"
  thenRequired?: string[]; // 조건 충족 시 required 필드
  displayName?: string;   // UI 표시용 이름
}

export interface CanonicalField {
  path: string;              // "Argument.TABLE_NAME" or "TABLE_NAME"
  name: string;              // "TABLE_NAME"
  displayName?: string;      // UI에 표시할 이름
  type: CanonicalType;
  required: boolean;
  default?: any;
  enum?: any[];
  minimum?: number;
  maximum?: number;
  description?: string;
  
  // 조건부 required
  conditions?: CanonicalCondition[];
  
  // 중첩 구조
  children?: CanonicalField[];
  
  // 메타데이터
  section?: string;          // 섹션 헤더 (조건부 그룹) - 별도 행으로 렌더링
  sectionHeader?: string;    // 필드의 섹션 헤더 (필드 위에 렌더링)
  level?: number;            // 중첩 레벨 (0=root, 1=child)
}

/**
 * JSON Schema를 Canonical Fields로 컴파일
 */
export function compileSchema(schema: any): CanonicalField[] {
  if (!schema) return [];
  
  const fields: CanonicalField[] = [];
  
  // 🔥 Step 1: 루트 키 찾기 (TABLE, NLCT 등)
  let rootSchema = schema;
  const rootKey = Object.keys(schema).find(key => key !== '$schema' && key !== 'title');
  
  if (rootKey && schema[rootKey].properties) {
    rootSchema = schema[rootKey];
  }
  
  // 🔥 Step 2: Argument 레벨 unwrap
  let properties = rootSchema.properties || {};
  let baseRequired = rootSchema.required || [];
  let allOfConditions = rootSchema.allOf || [];
  let oneOfConditions = rootSchema.oneOf || [];
  
  if (properties.Argument && properties.Argument.properties) {
    const argSchema = properties.Argument;
    properties = argSchema.properties;
    baseRequired = argSchema.required || [];
    allOfConditions = argSchema.allOf || [];
    oneOfConditions = argSchema.oneOf || [];
  }
  
  // 🔥 Step 3: allOf 조건 파싱
  const conditions = parseAllOfConditions(allOfConditions);
  const conditionalRequiredMap = buildConditionalRequiredMap(conditions);
  
  // 🔥 Step 3.5: oneOf 조건 파싱 (상호 배타적 선택)
  const oneOfParsed = parseOneOfConditions(oneOfConditions);
  const oneOfRequiredMap = buildOneOfRequiredMap(oneOfParsed);
  
  // 조건 통합
  Object.assign(conditionalRequiredMap, oneOfRequiredMap);
  
  // 🔥 Step 4: 필드 분류 (common vs conditional)
  const { commonFields, conditionalFieldGroups } = classifyFields(
    properties, 
    baseRequired, 
    conditionalRequiredMap
  );
  
  // 🔥 Step 5: Common 필드 변환
  let fieldNo = 1;
  commonFields.forEach(key => {
    const prop = properties[key];
    
    const field = convertPropertyToCanonical(key, prop, baseRequired.includes(key), fieldNo++);
    
    // 🔥 x-section-header가 있으면 필드에 sectionHeader 속성 추가 (별도 행 생성하지 않음)
    if (prop['x-section-header']) {
      field.sectionHeader = prop['x-section-header'];
    }
    
    fields.push(field);
  });
  
  // 🔥 Step 6: Conditional 필드를 섹션별로 그룹화
  Object.entries(conditionalFieldGroups).forEach(([conditionValue, fieldKeys]) => {
    if (fieldKeys.length === 0) return;
    
    // 섹션 헤더
    const condition = conditions.find(c => c.value === conditionValue);
    if (condition) {
      fields.push({
        path: `__section_${conditionValue}`,
        name: `__section_${conditionValue}`,
        type: 'string',
        required: false,
        section: condition.displayName || `${condition.field}: ${conditionValue}`,
        level: 0,
      });
    }
    
    // 조건부 필드들
    const methodRequired = conditionalRequiredMap[conditionValue] || [];
    fieldKeys.forEach(key => {
      const prop = properties[key];
      const field = convertPropertyToCanonical(
        key, 
        prop, 
        methodRequired.includes(key), 
        fieldNo++,
        condition
      );
      fields.push(field);
    });
  });
  
  return fields;
}

/**
 * oneOf 조건들을 파싱 (상호 배타적 선택)
 */
function parseOneOfConditions(oneOfConditions: any[]): CanonicalCondition[] {
  const conditions: CanonicalCondition[] = [];
  
  oneOfConditions.forEach((option: any, index: number) => {
    const optionProps = option.properties || {};
    const requiredFields = option.required || [];
    
    // oneOf의 각 옵션을 조건으로 변환
    requiredFields.forEach((fieldName: string) => {
      const prop = optionProps[fieldName];
      
      // x-section-header를 displayName으로 사용
      const displayName = prop?.['x-section-header'] || `Option ${index + 1}: ${fieldName}`;
      
      conditions.push({
        when: `oneOf_option_${index}`,
        field: '_oneOf',
        value: `option_${index}_${fieldName}`,
        thenRequired: [fieldName],
        displayName,
      });
    });
  });
  
  return conditions;
}

/**
 * oneOf 조건별 required 필드 매핑 생성
 */
function buildOneOfRequiredMap(conditions: CanonicalCondition[]): Record<string, string[]> {
  const map: Record<string, string[]> = {};
  
  conditions.forEach(condition => {
    if (condition.thenRequired) {
      map[condition.value] = condition.thenRequired;
    }
  });
  
  return map;
}

/**
 * allOf 조건들을 파싱
 */
function parseAllOfConditions(allOfConditions: any[]): CanonicalCondition[] {
  const conditions: CanonicalCondition[] = [];
  
  allOfConditions.forEach((condition: any) => {
    if (condition.if && condition.then) {
      const conditionProps = condition.if.properties || {};
      const conditionKey = Object.keys(conditionProps)[0];
      const conditionValue = conditionProps[conditionKey]?.const;
      
      if (conditionValue && condition.then.required) {
        conditions.push({
          when: `${conditionKey} = ${conditionValue}`,
          field: conditionKey,
          value: conditionValue,
          thenRequired: condition.then.required,
        });
      }
    }
  });
  
  return conditions;
}

/**
 * 조건별 required 필드 매핑 생성
 */
function buildConditionalRequiredMap(conditions: CanonicalCondition[]): Record<string, string[]> {
  const map: Record<string, string[]> = {};
  
  conditions.forEach(condition => {
    if (condition.thenRequired) {
      map[condition.value] = condition.thenRequired;
    }
  });
  
  return map;
}

/**
 * 필드를 common vs conditional로 분류
 */
function classifyFields(
  properties: any,
  baseRequired: string[],
  conditionalRequiredMap: Record<string, string[]>
) {
  const commonFields: string[] = [];
  const conditionalFieldGroups: Record<string, string[]> = {};
  
  // 초기화
  Object.keys(conditionalRequiredMap).forEach(method => {
    conditionalFieldGroups[method] = [];
  });
  
  Object.keys(properties).forEach(key => {
    let isConditional = false;
    
    // 조건부 required 체크
    for (const [method, requiredFields] of Object.entries(conditionalRequiredMap)) {
      if (requiredFields.includes(key) && !baseRequired.includes(key)) {
        conditionalFieldGroups[method].push(key);
        isConditional = true;
        break;
      }
    }
    
    if (!isConditional) {
      commonFields.push(key);
    }
  });
  
  return { commonFields, conditionalFieldGroups };
}

/**
 * Property를 Canonical Field로 변환
 */
function convertPropertyToCanonical(
  key: string,
  prop: any,
  isRequired: boolean,
  _no: number,
  condition?: CanonicalCondition
): CanonicalField {
  // Type 정규화
  let type: CanonicalType = normalizeType(prop.type, prop.items);
  
  const field: CanonicalField = {
    path: key,
    name: key,
    displayName: prop.description || key,
    type,
    required: isRequired,
    description: prop.description,
    level: 0,
  };
  
  // Default value
  if (prop.default !== undefined) {
    field.default = prop.default;
  }
  
  // Enum
  if (prop.enum && Array.isArray(prop.enum)) {
    field.enum = prop.enum;
  }
  
  // Number constraints
  if (prop.minimum !== undefined) field.minimum = prop.minimum;
  if (prop.maximum !== undefined) field.maximum = prop.maximum;
  
  // 조건 정보
  if (condition) {
    field.conditions = [condition];
  }
  
  // 🔥 Object 타입 - 중첩 필드 재귀 처리
  if (prop.type === 'object' && prop.properties) {
    const objRequired = prop.required || [];
    field.children = [];
    
    Object.keys(prop.properties).forEach((childKey, idx) => {
      const childProp = prop.properties[childKey];
      
      const childField = convertPropertyToCanonical(
        `${key}.${childKey}`,
        childProp,
        objRequired.includes(childKey),
        idx + 1
      );
      childField.level = 1;
      
      // 🔥 x-section-header가 있으면 섹션 헤더 정보를 필드에 추가
      if (childProp['x-section-header']) {
        childField.sectionHeader = childProp['x-section-header'];
      }
      
      field.children!.push(childField);
    });
  }
  
  // 🔥 Object 타입 with oneOf - 상호 배타적 선택
  if (prop.type === 'object' && prop.oneOf && Array.isArray(prop.oneOf)) {
    field.children = [];
    
    prop.oneOf.forEach((option: any, optionIndex: number) => {
      const optionProps = option.properties || {};
      const optionRequired = option.required || [];
      
      Object.keys(optionProps).forEach((childKey, idx) => {
        const childProp = optionProps[childKey];
        
        const childField = convertPropertyToCanonical(
          `${key}.${childKey}`,
          childProp,
          optionRequired.includes(childKey),
          idx + 1
        );
        childField.level = 1;
        
        // 🔥 x-section-header가 있으면 섹션 헤더 정보를 필드에 추가
        if (childProp['x-section-header']) {
          childField.sectionHeader = childProp['x-section-header'];
        }
        
        // oneOf 조건 정보 추가
        childField.conditions = [{
          when: `oneOf_option_${optionIndex}`,
          field: key,
          value: childKey,
          thenRequired: [childKey],
          displayName: childProp['x-section-header'] || `Option ${optionIndex + 1}`,
        }];
        
        field.children!.push(childField);
      });
    });
  }
  
  // 🔥 Array[Object] 타입 - items 재귀 처리
  if (prop.type === 'array' && prop.items?.type === 'object' && prop.items.properties) {
    const itemRequired = prop.items.required || [];
    field.children = [];
    
    Object.keys(prop.items.properties).forEach((childKey, idx) => {
      const childProp = prop.items.properties[childKey];
      const childField = convertPropertyToCanonical(
        `${key}[].${childKey}`,
        childProp,
        itemRequired.includes(childKey),
        idx + 1
      );
      childField.level = 1;
      field.children!.push(childField);
    });
  }
  
  return field;
}

/**
 * Type 정규화
 */
function normalizeType(type: string | undefined, items?: any): CanonicalType {
  if (!type) return 'string';
  
  switch (type) {
    case 'integer':
      return 'integer';
    case 'number':
      return 'number';
    case 'boolean':
      return 'boolean';
    case 'object':
      return 'object';
    case 'array':
      if (items?.type === 'object') return 'array[object]';
      if (items?.type === 'string') return 'array[string]';
      if (items?.type === 'number' || items?.type === 'integer') return 'array[number]';
      return 'array';
    case 'string':
    default:
      return 'string';
  }
}

