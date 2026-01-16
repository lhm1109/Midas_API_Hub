/**
 * Type-Dependent Metadata Handler
 * 
 * Enhanced Schema의 x-*-by-type 메타데이터를 통합 처리합니다.
 * 
 * @see api_docs_zendesk/make_enhanced_schema.md
 */

import type { EnhancedField } from './schemaCompiler';

export type TypeDependentMetadataType = 
  | 'enum' 
  | 'enumLabels' 
  | 'required' 
  | 'valueConstraint' 
  | 'nodeCount';

/**
 * TYPE 값에 따라 메타데이터 값 가져오기
 */
export function getTypeDependentValue(
  field: EnhancedField,
  type: string,
  metadataType: TypeDependentMetadataType
): any {
  const fieldAny = field as any;

  switch (metadataType) {
    case 'enum':
      // x-enum-by-type에서 해당 TYPE의 enum 값 가져오기
      const enumByType = fieldAny.enumByType || fieldAny['x-enum-by-type'];
      return enumByType?.[type] || field.enum;

    case 'enumLabels':
      // x-enum-labels-by-type에서 해당 TYPE의 enum labels 가져오기
      const enumLabelsByType = fieldAny.enumLabelsByType || fieldAny['x-enum-labels-by-type'];
      return enumLabelsByType?.[type] || fieldAny.enumLabels || fieldAny['x-enum-labels'];

    case 'required':
      // x-required-by-type에서 해당 TYPE의 required 상태 가져오기
      const requiredByType = fieldAny.requiredByType || fieldAny['x-required-by-type'];
      if (requiredByType?.[type] !== undefined) {
        return requiredByType[type];
      }
      // fallback: required 객체에서 TYPE별 상태 확인
      return field.required?.[type] || field.required?.['*'] || 'optional';

    case 'valueConstraint':
      // x-value-constraint에서 해당 TYPE의 제약 조건 가져오기
      const valueConstraint = fieldAny.valueConstraint || fieldAny['x-value-constraint'];
      return valueConstraint?.[type];

    case 'nodeCount':
      // x-node-count-by-type에서 해당 TYPE의 노드 개수 가져오기
      const nodeCountByType = fieldAny.nodeCountByType || fieldAny['x-node-count-by-type'];
      return nodeCountByType?.[type];

    default:
      return undefined;
  }
}

/**
 * 필드가 TYPE에 의존하는 메타데이터를 가지고 있는지 확인
 */
export function hasTypeDependentMetadata(field: EnhancedField): boolean {
  const fieldAny = field as any;
  
  return !!(
    fieldAny.enumByType ||
    fieldAny['x-enum-by-type'] ||
    fieldAny.enumLabelsByType ||
    fieldAny['x-enum-labels-by-type'] ||
    fieldAny.requiredByType ||
    fieldAny['x-required-by-type'] ||
    fieldAny.valueConstraint ||
    fieldAny['x-value-constraint'] ||
    fieldAny.nodeCountByType ||
    fieldAny['x-node-count-by-type']
  );
}

/**
 * 필드의 모든 TYPE별 메타데이터 타입 목록 가져오기
 */
export function getTypeDependentMetadataTypes(field: EnhancedField): TypeDependentMetadataType[] {
  const types: TypeDependentMetadataType[] = [];
  const fieldAny = field as any;

  if (fieldAny.enumByType || fieldAny['x-enum-by-type']) {
    types.push('enum');
  }
  if (fieldAny.enumLabelsByType || fieldAny['x-enum-labels-by-type']) {
    types.push('enumLabels');
  }
  if (fieldAny.requiredByType || fieldAny['x-required-by-type']) {
    types.push('required');
  }
  if (fieldAny.valueConstraint || fieldAny['x-value-constraint']) {
    types.push('valueConstraint');
  }
  if (fieldAny.nodeCountByType || fieldAny['x-node-count-by-type']) {
    types.push('nodeCount');
  }

  return types;
}
