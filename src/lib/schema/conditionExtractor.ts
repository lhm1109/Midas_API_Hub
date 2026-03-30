/**
 * Condition Extractor (YAML-based)
 * 
 * Enhanced Schema의 조건부 필드 정보를 YAML 규칙에 따라 추출합니다.
 * 
 * @see schema_definitions/{psdSet}/{schemaType}/table.yaml
 */

import type { EnhancedField } from './schemaCompiler';
import type { TableDefinition } from '../rendering/definitionLoader';

export interface ConditionEntry {
  condition: Record<string, unknown>; // TYPE: "TENSTR" 등
  hint?: string; // 조건별 힌트 (선택)
  groupId?: string; // 조건별 그룹 ID (선택)
}

export interface FieldCondition {
  type: string; // 조건 타입 (예: "x-optional-when")
  label: string; // 표시 레이블
  conditionText: string; // 조건 텍스트 (예: "TYPE is TENSTR")
  color: string; // 표시 색상
  icon: string; // 표시 아이콘
  value: unknown; // 원본 조건 값 (객체 또는 배열)
  // 🔥 배열 형식 지원: 조건별 힌트/그룹 포함
  conditions?: ConditionEntry[];
}

export interface FieldConditionInfo {
  conditionKey: string; // 그룹핑용 키
  conditionInfo: FieldCondition | null;
  group: string | undefined;
}

function getFieldLeafKey(field: EnhancedField): string {
  const rawKey = String(field.key || '');
  const segments = rawKey.split('.');
  return (segments[segments.length - 1] || rawKey).replace(/\[\]/g, '');
}

function serializeConditionValue(value: unknown): string {
  if (typeof value === 'string') return value;
  return JSON.stringify(value);
}

function buildConditionKey(condition: Record<string, unknown>): string {
  return Object.entries(condition)
    .map(([key, value]) => `${key}:${value}`)
    .join('|');
}

function extractDirectSimpleRequiredCondition(field: EnhancedField): {
  axisField: string;
  values: unknown[];
} | null {
  const fieldAny = field as any;
  const requiredWhen = fieldAny['x-required-when'];

  if (!requiredWhen || Array.isArray(requiredWhen) || typeof requiredWhen !== 'object') {
    return null;
  }

  const entries = Object.entries(requiredWhen as Record<string, unknown>);
  if (entries.length !== 1) {
    return null;
  }

  const [axisField, rawValue] = entries[0];
  const values = Array.isArray(rawValue) ? rawValue : [rawValue];

  if (values.length === 0) {
    return null;
  }

  return { axisField, values };
}

function createSyntheticConditionInfo(
  type: 'x-required-when' | 'x-optional-when',
  axisField: string,
  axisValue: unknown
): FieldCondition {
  const marker = DEFAULT_CONDITIONAL_MARKERS[type];
  const condition = { [axisField]: axisValue };

  return {
    type,
    label: marker.label,
    icon: marker.icon,
    color: marker.color,
    value: condition,
    conditionText: formatConditionText(condition),
  };
}

/**
 * 조건 마커 스타일 기본값 (YAML에서 오버라이드 가능)
 * @see schema_definitions/{psdSet}/{schemaType}/ui.yaml - legacyMarkers (deprecated)
 * @see 새 스키마: allOf if/then + x-uiRules.visibleWhen 사용 권장
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
  // NOTE: ui.visibleWhen은 사용하지 않음 - x-optional-when으로 대체
};

/**
 * 필드에서 조건 정보 추출
 * 
 * 지원하는 조건 마커:
 * - x-required-when: 조건부 필수 (조건 만족 시 필수)
 * - x-optional-when: 조건부 선택 (조건 만족 시 선택적으로 표시)
 * 
 * @param field - Enhanced Schema 필드
 * @param conditionalRules - YAML에서 정의된 조건 규칙
 * @param markerStyles - YAML에서 정의된 마커 스타일 (ui.yaml - legacyMarkers)
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

  /**
   * 🔥 조건 마커 파싱 (객체 또는 배열 형식 지원)
   * 
   * 객체 형식: { TYPE: "WALL" }
   * 배열 형식: [{ condition: { TYPE: "TENSTR" }, hint: "음수 입력 권장" }, ...]
   */
  const parseConditionalMarker = (
    value: unknown,
    markerType: string,
    getStyle: typeof getMarkerStyle
  ): FieldCondition | null => {
    if (!value) return null;

    const style = getStyle(markerType);

    // 🔥 배열 형식 처리
    if (Array.isArray(value)) {
      if (value.length === 0) return null;

      // ConditionEntry 배열로 정규화
      const conditions: ConditionEntry[] = value.map((item) => {
        if (typeof item === 'object' && item !== null) {
          // { condition: {...}, hint: "..." } 형식
          if ('condition' in item && typeof item.condition === 'object') {
            return {
              condition: item.condition as Record<string, unknown>,
              hint: item.hint as string | undefined,
              groupId: item.groupId as string | undefined,
            };
          }
          // { TYPE: "WALL" } 형식 (레거시 호환)
          return {
            condition: item as Record<string, unknown>,
          };
        }
        return { condition: {} };
      }).filter(entry => Object.keys(entry.condition).length > 0);

      if (conditions.length === 0) return null;

      // 모든 조건을 텍스트로 결합
      const conditionTexts = conditions.map(c => formatConditionText(c.condition));
      const conditionText = conditionTexts.join(' | ');

      return {
        type: markerType,
        label: style.label,
        conditionText,
        color: style.color,
        icon: style.icon,
        value,
        conditions, // 🔥 조건별 힌트/그룹 정보 포함
      };
    }

    // 🔥 객체 형식 처리 (기존 방식)
    if (typeof value === 'object' && value !== null && Object.keys(value).length > 0) {
      return {
        type: markerType,
        label: style.label,
        conditionText: formatConditionText(value as Record<string, unknown>),
        color: style.color,
        icon: style.icon,
        value,
        // 객체 형식은 단일 조건이므로 conditions 미사용 (undefined)
      };
    }

    return null;
  };

  // 🔥 1. x-required-when 처리 (조건부 필수 = 표시 + required)
  const requiredWhen = fieldAny['x-required-when'];
  if (requiredWhen) {
    const result = parseConditionalMarker(requiredWhen, 'x-required-when', getMarkerStyle);
    if (result) return result;
  }

  // 🔥 2. x-optional-when 처리 (조건부 선택 = 표시 + optional)
  // 객체 또는 배열 형식 모두 지원
  const optionalWhen = fieldAny['x-optional-when'];
  if (optionalWhen) {
    const result = parseConditionalMarker(optionalWhen, 'x-optional-when', getMarkerStyle);
    if (result) return result;
  }

  // NOTE: ui.visibleWhen은 더 이상 지원하지 않음
  // 조건부 표시가 필요한 경우 x-optional-when 사용

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
      if (condition.value && typeof condition.value === 'object' && !Array.isArray(condition.value)) {
        conditionKey = Object.entries(condition.value as Record<string, unknown>)
          .map(([k, v]) => `${k}:${v}`)
          .join('|');
      }

      if (!conditionKey && condition.conditionText) {
        conditionKey = condition.conditionText;
      }
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
  const handledFields = new Set<EnhancedField>();

  const supplementalGroups = new Map<string, Array<{ field: EnhancedField; conditionInfo: FieldCondition }>>();
  const expandableAxes = new Map<string, Array<{ field: EnhancedField; values: unknown[] }>>();

  for (const field of fields) {
    if (!(field as any)._injectedRequiredWhen) {
      continue;
    }

    const simpleCondition = extractDirectSimpleRequiredCondition(field);
    if (!simpleCondition) {
      continue;
    }

    if (!expandableAxes.has(simpleCondition.axisField)) {
      expandableAxes.set(simpleCondition.axisField, []);
    }

    expandableAxes.get(simpleCondition.axisField)!.push({
      field,
      values: simpleCondition.values,
    });
  }

  for (const [axisField, members] of expandableAxes.entries()) {
    const axisValueMap = new Map<string, unknown>();

    const controllerField = fields.find((field) => {
      const fieldAny = field as any;
      return getFieldLeafKey(field) === axisField && Array.isArray(fieldAny.enum) && fieldAny.enum.length > 0;
    });

    const controllerValues = Array.isArray((controllerField as any)?.enum) ? (controllerField as any).enum : [];
    controllerValues.forEach((value: unknown) => {
      axisValueMap.set(serializeConditionValue(value), value);
    });

    members.forEach(({ values }) => {
      values.forEach((value) => {
        axisValueMap.set(serializeConditionValue(value), value);
      });
    });

    const axisValues = Array.from(axisValueMap.values());
    if (axisValues.length < 2) {
      continue;
    }

    members.forEach(({ field }) => handledFields.add(field));

    axisValues.forEach((axisValue) => {
      const axisValueKey = serializeConditionValue(axisValue);
      const condition = { [axisField]: axisValue };
      const conditionGroupKey = buildConditionKey(condition);

      if (!supplementalGroups.has(conditionGroupKey)) {
        supplementalGroups.set(conditionGroupKey, []);
      }

      members.forEach(({ field, values }) => {
        const isRequiredForValue = values.some((value) => serializeConditionValue(value) === axisValueKey);
        supplementalGroups.get(conditionGroupKey)!.push({
          field,
          conditionInfo: createSyntheticConditionInfo(
            isRequiredForValue ? 'x-required-when' : 'x-optional-when',
            axisField,
            axisValue
          ),
        });
      });
    });
  }

  for (const field of fields) {
    if (handledFields.has(field)) {
      continue;
    }

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

  for (const [conditionKey, entries] of supplementalGroups.entries()) {
    if (!fieldGroups.has(conditionKey)) {
      fieldGroups.set(conditionKey, []);
    }
    fieldGroups.get(conditionKey)!.push(...entries);
  }

  return { fieldGroups, noConditionFields };
}

/**
 * 🔥 NEW: 배열 groupId가 있는 필드를 조건별로 확장
 * 
 * x-optional-when: [
 *   { condition: { TYPE: "TENSTR" }, groupId: "TENSTR", hint: "..." },
 *   { condition: { TYPE: "COMPTR" }, groupId: "COMPTR", hint: "..." }
 * ]
 * 
 * → TENSTR 그룹에 TENSTR 힌트가 있는 필드
 * → COMPTR 그룹에 COMPTR 힌트가 있는 필드
 */
export function expandFieldsByArrayGroupId(
  fields: EnhancedField[],
  conditionalRules: NonNullable<TableDefinition['schemaExtensions']>['conditional'] = []
): EnhancedField[] {
  const expandedFields: EnhancedField[] = [];

  for (const field of fields) {
    const condition = extractFieldConditions(field, conditionalRules);

    // 배열 형식이고 groupId가 있는지 확인
    if (condition?.conditions && condition.conditions.some(c => c.groupId)) {
      // 각 조건별로 별도의 필드 인스턴스 생성
      for (const entry of condition.conditions) {
        if (entry.groupId) {
          // 필드 복제하고 조건/그룹 정보 덮어쓰기
          const clonedField = {
            ...field,
            // 🔥 확장 필드임을 표시
            _expandedFromArray: true,
            _arrayConditionEntry: entry,
          } as EnhancedField;

          // x-ui에 groupId 주입
          const originalXui = (field as any)['x-ui'] || {};
          (clonedField as any)['x-ui'] = {
            ...originalXui,
            groupId: entry.groupId
          };

          // 조건을 단일 조건으로 변경 (해당 그룹의 조건만)
          (clonedField as any)['x-optional-when'] = entry.condition;

          // 힌트가 있으면 _conditionalHint에 저장
          if (entry.hint) {
            (clonedField as any)._conditionalHint = entry.hint;
          }

          expandedFields.push(clonedField);
        }
      }
    } else {
      // groupId가 없으면 원본 필드 그대로 추가
      expandedFields.push(field);
    }
  }

  return expandedFields;
}
