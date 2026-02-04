/**
 * Type-Dependent Metadata Handler
 * 
 * ⚠️ DEPRECATED: shared.yaml SSOT에 따라 x-*-by-type 마커들은 제거됨
 * - x-enum-by-type → allOf[].if.then.properties.*.enum
 * - x-node-count-by-type → allOf[].if.then.properties.NODE.minItems/maxItems
 * - x-required-by-type → allOf[].if.then.required
 * - x-value-constraint → allOf[].if.then (설명은 스키마 description에)
 * 
 * 이 파일은 레거시 스키마 호환을 위해 최소한의 stub만 유지합니다.
 */

import type { EnhancedField } from './schemaCompiler';

export type TypeDependentMetadataType =
  | 'enum'
  | 'enumLabels';

/**
 * TYPE 값에 따라 메타데이터 값 가져오기
 * ⚠️ 순수 UI 메타데이터(enumLabels)만 지원
 */
export function getTypeDependentValue(
  field: EnhancedField,
  type: string,
  metadataType: TypeDependentMetadataType
): any {
  const fieldAny = field as any;

  switch (metadataType) {
    case 'enumLabels':
      // x-enum-labels-by-type에서 해당 TYPE의 enum labels 가져오기 (순수 UI 마커)
      const enumLabelsByType = fieldAny.enumLabelsByType || fieldAny['x-enum-labels-by-type'];
      return enumLabelsByType?.[type] || fieldAny.enumLabels || fieldAny['x-enum-labels'];

    default:
      return undefined;
  }
}

/**
 * 필드가 TYPE에 의존하는 메타데이터를 가지고 있는지 확인
 * ⚠️ 순수 UI 메타데이터(enumLabels)만 체크
 */
export function hasTypeDependentMetadata(field: EnhancedField): boolean {
  const fieldAny = field as any;

  return !!(
    fieldAny.enumLabelsByType ||
    fieldAny['x-enum-labels-by-type']
  );
}

/**
 * 필드의 모든 TYPE별 메타데이터 타입 목록 가져오기
 * ⚠️ 순수 UI 메타데이터만
 */
export function getTypeDependentMetadataTypes(field: EnhancedField): TypeDependentMetadataType[] {
  const types: TypeDependentMetadataType[] = [];
  const fieldAny = field as any;

  if (fieldAny.enumLabelsByType || fieldAny['x-enum-labels-by-type']) {
    types.push('enumLabels');
  }

  return types;
}
