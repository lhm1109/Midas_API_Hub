/**
 * Condition Extractor (YAML-based)
 * 
 * Enhanced Schema의 조건부 필드 정보를 YAML 규칙에 따라 추출합니다.
 * 
 * @see schema_definitions/{psdSet}/{schemaType}/table.yaml
 */

import type { EnhancedField } from './schemaCompiler';
import type { TableDefinition } from '../rendering/definitionLoader';

export interface FieldCondition {
  type: string; // 조건 타입 (예: "x-ui.visibleWhen")
  label: string; // 표시 레이블
  conditionText: string; // 조건 텍스트 (예: "TYPE is TENSTR")
  color: string; // 표시 색상
  icon: string; // 표시 아이콘
  value: any; // 원본 조건 값
}

export interface FieldConditionInfo {
  conditionKey: string; // 그룹핑용 키
  conditionInfo: FieldCondition | null;
  group: string | undefined;
}

/**
 * 조건 마커 스타일 기본값 (YAML에서 오버라이드 가능)
 * @see schema_definitions/{psdSet}/{schemaType}/ui.yaml - enhancedFeatures.conditionalMarkers
 */
const DEFAULT_CONDITIONAL_MARKERS: Record<string, { label: string; icon: string; color: string }> = {
  'x-required-when': {
    label: 'Required When',
    icon: '●',
    color: '#ff7e7e',  // 빨간색 계열 (필수)
  },
  'x-optional-when': {
    label: 'Optional When',
    icon: '○',
    color: '#7eb8ff',  // 파란색 계열 (선택)
  },
};

/**
 * 필드에서 조건 정보 추출
 * 
 * @param field - Enhanced Schema 필드
 * @param conditionalRules - YAML에서 정의된 조건 규칙
 * @param markerStyles - YAML에서 정의된 마커 스타일 (ui.yaml - enhancedFeatures.conditionalMarkers)
 */
export function extractFieldConditions(
  field: EnhancedField,
  conditionalRules: NonNullable<TableDefinition['schemaExtensions']>['conditional'] = [],
  markerStyles?: Record<string, { label?: string; icon?: string; color?: string }>
): FieldCondition | null {
  const fieldAny = field as any;

  // 🔥 마커 스타일 병합 (YAML 우선, 기본값 폴백)
  const getMarkerStyle = (markerKey: string) => {
    const yamlStyle = markerStyles?.[markerKey];
    const defaultStyle = DEFAULT_CONDITIONAL_MARKERS[markerKey];
    return {
      label: yamlStyle?.label || defaultStyle?.label || markerKey,
      icon: yamlStyle?.icon || defaultStyle?.icon || '•',
      color: yamlStyle?.color || defaultStyle?.color || '#4c9aff',
    };
  };

  // 🔥 1. x-required-when 처리 (조건부 필수 = 표시 + required)
  const requiredWhen = fieldAny['x-required-when'];
  if (requiredWhen && typeof requiredWhen === 'object' && Object.keys(requiredWhen).length > 0) {
    const style = getMarkerStyle('x-required-when');
    return {
      type: 'x-required-when',
      label: style.label,
      conditionText: formatConditionText(requiredWhen),
      color: style.color,
      icon: style.icon,
      value: requiredWhen,
    };
  }

  // 🔥 2. x-optional-when 처리 (조건부 선택 = 표시 + optional)
  const optionalWhen = fieldAny['x-optional-when'];
  if (optionalWhen && typeof optionalWhen === 'object' && Object.keys(optionalWhen).length > 0) {
    const style = getMarkerStyle('x-optional-when');
    return {
      type: 'x-optional-when',
      label: style.label,
      conditionText: formatConditionText(optionalWhen),
      color: style.color,
      icon: style.icon,
      value: optionalWhen,
    };
  }

  // 🔥 3. YAML 규칙 기반 조건 처리 (기존 로직)
  if (conditionalRules && conditionalRules.length > 0) {
    for (const rule of conditionalRules) {
      if (!rule.displayInTable) continue;

      const value = getNestedValue(fieldAny, rule.key);

      // 조건 값이 있으면 조건 정보 생성
      if (value && typeof value === 'object' && Object.keys(value).length > 0) {
        return {
          type: rule.key,
          label: rule.displayLabel || rule.key,
          conditionText: formatConditionText(value),
          color: rule.displayColor || '#4c9aff',
          icon: rule.displayIcon || '•',
          value: value,
        };
      }
    }
  }

  return null;
}

/**
 * 중첩된 객체에서 경로로 값 추출
 * 예: "x-ui.visibleWhen" -> field['x-ui']?.['visibleWhen']
 */
function getNestedValue(obj: any, path: string): any {
  if (!obj || !path) return undefined;

  const parts = path.split('.');
  let value: any = obj;

  for (const part of parts) {
    if (!value) break;

    // x- prefix 처리
    if (part.startsWith('x-')) {
      if (part === 'x-ui') {
        // x-ui는 ui로도 변환될 수 있음
        value = value['ui'] || value['x-ui'];
      } else {
        value = value[part];
      }
    } else {
      value = value[part];
    }
  }

  return value;
}

/**
 * 조건 객체를 텍스트로 변환
 * 예: { TYPE: "TENSTR", STYPE: 3 } -> "TYPE" is TENSTR, "STYPE" is 3
 */
function formatConditionText(condition: Record<string, any>): string {
  return Object.entries(condition)
    .map(([k, v]) => `"${k}" is ${v}`)
    .join(', ');
}

/**
 * 필드 그룹의 조건 정보를 수집
 * 
 * @param fields - 섹션의 모든 필드
 * @param conditionalRules - YAML에서 정의된 조건 규칙
 * @returns 필드별 조건 정보 맵
 */
export function collectFieldConditionInfo(
  fields: EnhancedField[],
  conditionalRules: NonNullable<TableDefinition['schemaExtensions']>['conditional'] = []
): Map<EnhancedField, FieldConditionInfo> {
  const fieldInfoMap = new Map<EnhancedField, FieldConditionInfo>();
  const groupToConditionMap = new Map<string, { conditionKey: string; conditionInfo: FieldCondition }>();

  // 1단계: 모든 필드의 조건과 그룹 정보 수집
  for (const field of fields) {
    const fieldAny = field as any;
    const group = fieldAny.ui?.group || fieldAny['x-ui']?.group;
    const condition = extractFieldConditions(field, conditionalRules);

    let conditionKey = '';
    if (condition) {
      // 조건 키 생성 (그룹핑용)
      conditionKey = Object.entries(condition.value)
        .map(([k, v]) => `${k}:${v}`)
        .join('|');
    }

    fieldInfoMap.set(field, {
      conditionKey,
      conditionInfo: condition,
      group,
    });

    // 그룹별 조건 매핑 생성 (같은 그룹의 필드들이 같은 조건을 공유)
    if (group && condition) {
      groupToConditionMap.set(group, { conditionKey, conditionInfo: condition });
    }
  }

  // 2단계: 그룹 조건을 조건이 없는 필드에도 적용
  for (const field of fields) {
    const info = fieldInfoMap.get(field)!;

    // 조건이 없지만 같은 그룹에 조건이 있는 필드가 있으면 그 조건 사용
    if (!info.conditionKey && info.group) {
      const groupCondition = groupToConditionMap.get(info.group);
      if (groupCondition) {
        fieldInfoMap.set(field, {
          conditionKey: groupCondition.conditionKey,
          conditionInfo: groupCondition.conditionInfo,
          group: info.group,
        });
      }
    }
  }

  return fieldInfoMap;
}

/**
 * 필드들을 조건별로 그룹핑
 */
export function groupFieldsByCondition(
  fields: EnhancedField[],
  fieldInfoMap: Map<EnhancedField, FieldConditionInfo>
): {
  fieldGroups: Map<string, Array<{ field: EnhancedField; conditionInfo: FieldCondition }>>;
  noConditionFields: Array<{ field: EnhancedField; conditionInfo: null }>;
} {
  const fieldGroups = new Map<string, Array<{ field: EnhancedField; conditionInfo: FieldCondition }>>();
  const noConditionFields: Array<{ field: EnhancedField; conditionInfo: null }> = [];

  for (const field of fields) {
    const info = fieldInfoMap.get(field)!;

    if (info.conditionKey && info.conditionInfo) {
      if (!fieldGroups.has(info.conditionKey)) {
        fieldGroups.set(info.conditionKey, []);
      }
      fieldGroups.get(info.conditionKey)!.push({
        field,
        conditionInfo: info.conditionInfo,
      });
    } else {
      noConditionFields.push({ field, conditionInfo: null });
    }
  }

  return { fieldGroups, noConditionFields };
}
