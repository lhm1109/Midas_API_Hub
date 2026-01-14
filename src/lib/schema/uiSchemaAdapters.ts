/**
 * UI Schema Adapters - Canonical Field를 UI별 스키마로 변환
 * 
 * Table, Builder, Form 모두 이 레이어를 소비
 */

import type { CanonicalField, CanonicalType } from './schemaCompiler';

/**
 * Table Row Schema (Manual Tab용)
 */
export interface UITableRow {
  no: number | string;       // 1, 2, (1), (2) - children은 괄호
  name: string;              // "TABLE_NAME"
  description: string;       // 설명
  type: string;              // "String", "Integer", "Array[Object]"
  default: string;           // "-" or "\"value\"" or "0"
  required: string;          // "Required" or "Optional"
  options?: string[];        // enum 값들 (포맷팅된)
  children?: UITableRow[];   // 중첩 필드
  section?: string;          // 섹션 헤더
}

/**
 * Builder Card Schema (Builder Tab용)
 */
export interface UIBuilderField {
  name: string;
  parentName?: string;
  type: 'string' | 'number' | 'integer' | 'boolean' | 'array' | 'object' | 'enum';
  description: string;
  required: boolean;
  default?: any;
  enum?: any[];
  minimum?: number;
  maximum?: number;
  items?: any;
  placeholder?: string;
  children?: UIBuilderField[];  // 🔥 중첩 필드 지원
  oneOfOptions?: string[];      // 🔥 oneOf 옵션 제목 (Method 1, Method 2, ...)
  optionIndex?: number;         // 🔥 oneOf의 경우 어느 옵션에 속하는지 (0, 1, 2, ...)
}

/**
 * Canonical Fields → Table Schema
 */
export function canonicalToTableSchema(fields: CanonicalField[]): UITableRow[] {
  const rows: UITableRow[] = [];
  let rowNo = 1;
  
  fields.forEach(field => {
    // 🔥 section: oneOf 같은 조건부 그룹 (번호 리셋)
    if (field.section) {
      rows.push({
        no: '',
        name: '',
        description: '',
        type: '',
        default: '',
        required: '',
        section: field.section,
      });
      rowNo = 1; // 🔥 oneOf 섹션마다 번호 리셋
      return;
    }
    
    // 🔥 sectionHeader: 시각적 구분자 (번호 유지)
    if (field.sectionHeader) {
      rows.push({
        no: '',
        name: '',
        description: '',
        type: '',
        default: '',
        required: '',
        section: field.sectionHeader,
      });
      // 번호는 리셋하지 않음
    }
    
    // 일반 필드
    const row = fieldToTableRow(field, rowNo++);
    rows.push(row);
  });
  
  return rows;
}

/**
 * CanonicalField → UITableRow 변환
 */
function fieldToTableRow(field: CanonicalField, no: number): UITableRow {
  const row: UITableRow = {
    no,
    name: field.name,
    description: field.displayName || field.description || field.name,
    type: formatTypeForTable(field.type),
    default: formatDefaultForTable(field.default),
    required: field.required ? 'Required' : 'Optional',
  };
  
  // Enum options
  if (field.enum && field.enum.length > 0) {
    row.options = field.enum.map(opt => {
      if (typeof opt === 'string') {
        return ` • "${opt}"`;
      } else if (typeof opt === 'number') {
        return ` • ${opt}`;
      } else {
        return ` • ${JSON.stringify(opt)}`;
      }
    });
  }
  
  // Children (nested fields)
  if (field.children && field.children.length > 0) {
    let childNo = 1; // 🔥 자식 번호를 별도로 관리
    row.children = [];
    
    field.children.forEach((child) => {
      // 🔥 섹션 헤더는 특별 처리 (oneOf 등 - 번호 리셋)
      if (child.section) {
        childNo = 1; // 🔥 oneOf 섹션 헤더마다 번호 리셋
        row.children!.push({
          no: '',
          name: '',
          description: '',
          type: '',
          default: '',
          required: '',
          section: child.section,
        });
        return;
      }
      
      // 🔥 sectionHeader가 있으면 헤더 행을 먼저 추가 (번호는 리셋하지 않음)
      if (child.sectionHeader) {
        row.children!.push({
          no: '',
          name: '',
          description: '',
          type: '',
          default: '',
          required: '',
          section: child.sectionHeader,
        });
      }
      
      const childRow = fieldToTableRow(child, childNo);
      childRow.no = `(${childNo++})`; // 🔥 실제 필드만 번호 증가
      // 🔥 중첩 필드의 name은 부모 prefix 제거 (UNIT.FORCE → FORCE)
      if (childRow.name.includes('.')) {
        const parts = childRow.name.split('.');
        childRow.name = parts[parts.length - 1];
      }
      row.children!.push(childRow);
    });
  }
  
  return row;
}

/**
 * Type 포맷팅 (Table용)
 */
function formatTypeForTable(type: CanonicalType): string {
  switch (type) {
    case 'string': return 'String';
    case 'number': return 'Number';
    case 'integer': return 'Integer';
    case 'boolean': return 'Boolean';
    case 'object': return 'Object';
    case 'array': return 'Array';
    case 'array[string]': return 'Array[String]';
    case 'array[number]': return 'Array[Number]';
    case 'array[object]': return 'Array[Object]';
    default: return 'String';
  }
}

/**
 * Default value 포맷팅 (Table용)
 */
function formatDefaultForTable(defaultValue: any): string {
  if (defaultValue === undefined || defaultValue === null) return '-';
  
  if (typeof defaultValue === 'string') {
    return defaultValue === '' ? '""' : `"${defaultValue}"`;
  } else if (typeof defaultValue === 'boolean') {
    return defaultValue.toString();
  } else if (Array.isArray(defaultValue)) {
    return JSON.stringify(defaultValue);
  } else if (typeof defaultValue === 'object') {
    return JSON.stringify(defaultValue);
  } else {
    return String(defaultValue);
  }
}

/**
 * Canonical Fields → Builder Schema
 */
export function canonicalToBuilderSchema(fields: CanonicalField[]): UIBuilderField[] {
  const builderFields: UIBuilderField[] = [];
  
  fields.forEach(field => {
    // 섹션 헤더는 스킵 (Builder에서는 시각적으로 표현 안 함)
    if (field.section) return;
    
    const builderField = fieldToBuilderField(field);
    
    // 🔥 중첩 필드를 계층 구조로 유지 (평탄화 제거)
    if (field.children && field.children.length > 0) {
      builderField.children = field.children
        .filter(child => !child.section) // 섹션 헤더 제외
        .map(child => {
          const childField = fieldToBuilderField(child);
          // 자식 필드의 name에서 부모 prefix 제거
          if (childField.name.includes('.')) {
            const parts = childField.name.split('.');
            childField.name = parts[parts.length - 1];
          }
          childField.parentName = field.name;
          return childField;
        });
    }
    
    builderFields.push(builderField);
  });
  
  return builderFields;
}

/**
 * CanonicalField → UIBuilderField 변환
 */
function fieldToBuilderField(field: CanonicalField): UIBuilderField {
  let type: UIBuilderField['type'] = 'string';
  
  // Type 변환
  if (field.enum && field.enum.length > 0) {
    type = 'enum';
  } else {
    switch (field.type) {
      case 'integer':
      case 'number':
        type = 'number';
        break;
      case 'boolean':
        type = 'boolean';
        break;
      case 'array':
      case 'array[string]':
      case 'array[number]':
      case 'array[object]':
        type = 'array';
        break;
      case 'object':
        type = 'object';
        break;
      default:
        type = 'string';
    }
  }
  
  const builderField: UIBuilderField = {
    name: field.name,
    type,
    description: field.displayName || field.description || field.name,
    required: field.required,
    default: field.default,
    placeholder: `Enter ${field.name}`,
  };
  
  if (field.enum) builderField.enum = field.enum;
  if (field.minimum !== undefined) builderField.minimum = field.minimum;
  if (field.maximum !== undefined) builderField.maximum = field.maximum;
  
  // Array items 정보
  if (field.type.startsWith('array')) {
    builderField.items = {
      type: field.type.includes('[object]') ? 'object' : 
            field.type.includes('[string]') ? 'string' : 
            field.type.includes('[number]') ? 'number' : 'string'
    };
  }
  
  return builderField;
}

/**
 * Index 파일에서 export
 */
export * from './schemaRegistry';
export * from './schemaCompiler';

