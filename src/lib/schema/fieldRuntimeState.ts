/**
 * Field Runtime State Engine
 * 
 * 스키마의 의미(visible, required, enabled)를 한 곳에서 계산하여
 * Spec Tab, Builder UI, JSON Builder가 모두 참조하는 Single Source of Truth
 */

import type { EnhancedField, SectionGroup, VariantAxis } from './schemaCompiler';

/**
 * 필드의 런타임 상태
 */
export interface FieldRuntimeState {
  /** 현재 보이는지 여부 (x-ui.visibleWhen 평가 결과) */
  visible: boolean;

  /** 필수 상태 모드 (Spec 표시용) */
  required: 'required' | 'optional' | 'conditional' | 'n/a';

  /** 현재 시점에 required인지 (Builder validation/JSON 포함 판단용) */
  requiredNow: boolean;

  /** 활성화 여부 (사용자 입력 가능 여부) */
  enabled: boolean;
}

/**
 * 전체 필드들의 런타임 상태 맵
 */
export type FieldRuntimeStateMap = Record<string, FieldRuntimeState>;

/**
 * 타입 정규화: 문자열 "3"과 숫자 3을 올바르게 비교하기 위해
 */
function normalizeValue(value: any, expectedValue: any): any {
  // 기대값이 number인데 실제값이 string이면 → number로 변환
  if (typeof expectedValue === 'number' && typeof value === 'string') {
    const parsed = Number(value);
    return isNaN(parsed) ? value : parsed;
  }

  // 기대값이 string인데 실제값이 number면 → string으로 변환
  if (typeof expectedValue === 'string' && typeof value === 'number') {
    return String(value);
  }

  // 배열인 경우: 배열의 첫 번째 요소 타입을 기준으로 정규화
  if (Array.isArray(expectedValue) && expectedValue.length > 0) {
    const firstExpected = expectedValue[0];
    if (typeof firstExpected === 'number' && typeof value === 'string') {
      const parsed = Number(value);
      return isNaN(parsed) ? value : parsed;
    }
    if (typeof firstExpected === 'string' && typeof value === 'number') {
      return String(value);
    }
  }

  return value;
}

/**
 * visibleWhen 조건 평가 (타입 정규화 적용)
 */
function evaluateVisibleWhen(
  visibleWhen: Record<string, any> | undefined,
  formValues: Record<string, any>
): boolean {
  if (!visibleWhen) return true;

  for (const [key, expectedValue] of Object.entries(visibleWhen)) {
    const actualValue = formValues[key];

    // 🔥 타입 정규화: "3" vs 3 문제 해결
    const normalizedValue = normalizeValue(actualValue, expectedValue);

    if (Array.isArray(expectedValue)) {
      // 예: { "iMETHOD": [2, 4] } → iMETHOD가 2 또는 4일 때 표시
      if (!expectedValue.includes(normalizedValue)) {
        return false;
      }
    } else {
      // 예: { "iMETHOD": 1 } → iMETHOD가 1일 때만 표시
      if (normalizedValue !== expectedValue) {
        return false;
      }
    }
  }

  return true;
}

/**
 * x-required-when 조건 평가 (타입 정규화 적용)
 */
function evaluateRequiredWhen(
  requiredWhen: Record<string, any> | undefined,
  formValues: Record<string, any>
): boolean {
  if (!requiredWhen) return false;

  for (const [key, expectedValue] of Object.entries(requiredWhen)) {
    const actualValue = formValues[key];

    // 🔥 타입 정규화
    const normalizedValue = normalizeValue(actualValue, expectedValue);

    if (Array.isArray(expectedValue)) {
      if (!expectedValue.includes(normalizedValue)) {
        return false;
      }
    } else {
      if (normalizedValue !== expectedValue) {
        return false;
      }
    }
  }

  return true;
}

/**
 * 필드의 required 상태 계산 (2축: required 모드 + requiredNow)
 * 
 * 규칙 우선순위:
 * 1. JSON Schema native (allOf + if/then) - 가장 강한 근거
 * 2. x-required-when - UI 표시/도움용 규칙
 * 3. required 배열 - 기본 required
 * 
 * @param field - 필드 정보
 * @param formValues - 현재 폼 값들
 * @param isVisible - 현재 visible 상태
 * @returns { required: 상태 모드, requiredNow: 현재 required 여부 }
 */
function calculateFieldRequired(
  field: EnhancedField,
  formValues: Record<string, any>,
  isVisible: boolean
): { required: 'required' | 'optional' | 'conditional' | 'n/a'; requiredNow: boolean } {
  // 🔥 보이지 않으면 N/A
  if (!isVisible) {
    return { required: 'n/a', requiredNow: false };
  }

  // 🔥 우선순위 1: JSON Schema native conditional required
  // field.required가 Record<string, string>이고 TYPE별로 다른 경우
  // → schemaCompiler가 이미 allOf + if/then을 파싱해서 만든 결과
  if (field.required && typeof field.required === 'object' && !('*' in field.required)) {
    // 예: { "BEAM": "required", "TRUSS": "optional" }
    // → TYPE 기반 조건부

    // 🎯 트리거 필드는 스키마에서 자동 추출 (하드코딩 제거)
    let triggerValue: any = undefined;

    if (field.runtimeTriggers && field.runtimeTriggers.length > 0) {
      // 스키마가 명시한 트리거 필드 사용
      const triggerKey = field.runtimeTriggers[0];
      triggerValue = formValues[triggerKey];
    } else {
      // Fallback: required 객체의 키 중 하나가 formValues에 있는지 확인
      // (예: TYPE, iMETHOD, MODE, STYPE 등)
      const requiredKeys = Object.keys(field.required);
      for (const possibleTrigger of Object.keys(formValues)) {
        if (requiredKeys.some(k => k !== '*')) {
          triggerValue = formValues[possibleTrigger];
          break;
        }
      }
    }

    if (triggerValue !== undefined) {
      // 타입 정규화
      const normalizedType = normalizeValue(triggerValue, Object.keys(field.required)[0]);
      const statusForType = field.required[normalizedType];

      if (statusForType) {
        const isRequiredForType = statusForType === 'required';

        // 다른 타입에서는 다를 수 있으므로 conditional
        const hasMultipleStatuses = new Set(Object.values(field.required)).size > 1;

        return {
          required: hasMultipleStatuses ? 'conditional' : (isRequiredForType ? 'required' : 'optional'),
          requiredNow: isRequiredForType
        };
      }
    }

    // 🔥 트리거 값이 없으면 조건을 판단할 수 없으므로 requiredNow는 false
    // ⚠️ 중요: requiredNow=true는 "현재 시점에 required"를 의미
    //         트리거가 없으면 조건 미충족 상태이므로 false가 정답
    return {
      required: 'conditional',
      requiredNow: false
    };
  }

  // 🔥 우선순위 2: x-required-when (UI 전용 규칙)
  const xRequiredWhen = (field as any)['x-required-when'];
  if (xRequiredWhen && typeof xRequiredWhen === 'object') {
    const isCurrentlyRequired = evaluateRequiredWhen(xRequiredWhen, formValues);
    return {
      required: 'conditional',
      requiredNow: isCurrentlyRequired
    };
  }

  // 🔥 우선순위 3: 기본 required 체크
  if (field.required) {
    // field.required가 boolean인 경우
    if (typeof field.required === 'boolean') {
      return {
        required: field.required ? 'required' : 'optional',
        requiredNow: field.required
      };
    }

    // field.required가 Record<string, string>이고 '*' 키가 있는 경우
    if (typeof field.required === 'object' && field.required['*']) {
      const status = field.required['*'];
      if (status === 'required') {
        return { required: 'required', requiredNow: true };
      }
      if (status === 'conditional') {
        // 🎯 conditional이면 x-required-when을 다시 확인 (우선순위 2에서 놓친 경우)
        const xRequiredWhen = (field as any)['x-required-when'];
        if (xRequiredWhen && typeof xRequiredWhen === 'object') {
          const isCurrentlyRequired = evaluateRequiredWhen(xRequiredWhen, formValues);
          return {
            required: 'conditional',
            requiredNow: isCurrentlyRequired
          };
        }
        // x-required-when이 없으면 조건을 알 수 없으므로 requiredNow는 false
        return { required: 'conditional', requiredNow: false };
      }
      if (status === 'n/a') {
        return { required: 'n/a', requiredNow: false };
      }
    }
  }

  return { required: 'optional', requiredNow: false };
}

/**
 * 🎯 메인 함수: 모든 필드의 런타임 상태 계산
 * 
 * @param sections - compileEnhancedSchema 결과
 * @param formValues - 현재 폼 값들 (tempFormValuesForSchema)
 * @param variantAxes - VariantAxis 배열 (optional, 자동 추론 결과)
 * @returns 필드명 → 런타임 상태 맵
 */
export function calculateFieldRuntimeStates(
  sections: SectionGroup[],
  formValues: Record<string, any>,
  _variantAxes?: VariantAxis[]
): FieldRuntimeStateMap {
  const stateMap: FieldRuntimeStateMap = {};

  // 🔥 Step 0: 그룹별 허용 TYPE 사전 계산
  // 그룹 내 필드들의 x-required-by-type, x-enum-by-type, x-value-constraint에서 허용 TYPE 수집
  const groupAllowedTypes = new Map<string, Set<string>>();

  for (const section of sections) {
    for (const field of section.fields) {
      const groupName = field.ui?.group;
      if (!groupName) continue;

      // 그룹 이름이 "common"을 포함하면 무시 (항상 표시)
      if (groupName.toLowerCase().includes('common')) continue;

      // 이 필드에서 허용 TYPE 수집
      const allowedTypesForField = new Set<string>();

      // x-required-by-type에서 수집
      const xRequiredByType = (field as any)['x-required-by-type'];
      if (xRequiredByType && typeof xRequiredByType === 'object') {
        for (const [typeKey, status] of Object.entries(xRequiredByType)) {
          if (status !== false) { // true 또는 다른 값이면 허용
            allowedTypesForField.add(typeKey);
          }
        }
      }

      // x-enum-by-type에서 수집
      const xEnumByType = (field as any)['x-enum-by-type'];
      if (xEnumByType && typeof xEnumByType === 'object') {
        for (const typeKey of Object.keys(xEnumByType)) {
          allowedTypesForField.add(typeKey);
        }
      }

      // x-value-constraint에서 수집
      const xValueConstraint = (field as any)['x-value-constraint'];
      if (xValueConstraint && typeof xValueConstraint === 'object') {
        for (const typeKey of Object.keys(xValueConstraint)) {
          allowedTypesForField.add(typeKey);
        }
      }

      // x-required-when에서 수집 (TYPE 조건이 있는 경우)
      const xRequiredWhen = (field as any)['x-required-when'];
      if (xRequiredWhen && typeof xRequiredWhen === 'object' && xRequiredWhen.TYPE) {
        const typeValue = xRequiredWhen.TYPE;
        if (Array.isArray(typeValue)) {
          typeValue.forEach(t => allowedTypesForField.add(t));
        } else {
          allowedTypesForField.add(typeValue);
        }
      }

      // 그룹에 허용 TYPE 병합
      if (allowedTypesForField.size > 0) {
        if (!groupAllowedTypes.has(groupName)) {
          groupAllowedTypes.set(groupName, new Set());
        }
        const existingSet = groupAllowedTypes.get(groupName)!;
        allowedTypesForField.forEach(t => existingSet.add(t));
      }
    }
  }

  for (const section of sections) {
    for (const field of section.fields) {
      // 🔥 1. visible 계산 (visibleWhen + x-required-when + x-* + 그룹 기반)
      let visible = evaluateVisibleWhen(field.ui?.visibleWhen, formValues);

      // 🔥 1.1: x-required-when이 있으면 조건이 맞지 않으면 숨김
      // 예: WALL_ID는 x-required-when: { TYPE: "WALL" } → TYPE=BEAM이면 숨김
      const xRequiredWhen = (field as any)['x-required-when'];
      if (visible && xRequiredWhen && typeof xRequiredWhen === 'object') {
        const conditionMet = evaluateRequiredWhen(xRequiredWhen, formValues);
        if (!conditionMet) {
          visible = false;
        }
      }

      // 🔥 1.2: x-required-by-type 확인
      // 필드가 특정 TYPE에서만 관련이 있으면, 다른 TYPE에서는 숨김
      // 예: STYPE는 x-required-by-type: { TENSTR: true, BEAM: undefined } → BEAM에서 숨김
      const xRequiredByType = (field as any)['x-required-by-type'];
      if (visible && xRequiredByType && typeof xRequiredByType === 'object') {
        const currentType = formValues['TYPE'];
        if (currentType !== undefined) {
          const typeStatus = xRequiredByType[currentType];
          // typeStatus가 undefined이면 이 TYPE에는 해당 필드가 관련 없음 → 숨김
          // typeStatus가 false이면 명시적으로 불필요 → 숨김
          if (typeStatus === undefined || typeStatus === false) {
            visible = false;
          }
        }
      }

      // 🔥 1.3: x-enum-by-type 확인
      // 필드가 특정 TYPE에서만 enum을 가지면, 다른 TYPE에서는 숨김
      // 예: STYPE는 x-enum-by-type: { TENSTR: [1,2,3], ... } → BEAM에서 숨김
      const xEnumByType = (field as any)['x-enum-by-type'];
      if (visible && xEnumByType && typeof xEnumByType === 'object') {
        const currentType = formValues['TYPE'];
        if (currentType !== undefined) {
          const enumForType = xEnumByType[currentType];
          // 이 TYPE에 대한 enum이 없으면 숨김
          if (enumForType === undefined) {
            visible = false;
          }
        }
      }

      // 🔥 1.4: x-value-constraint 확인 (선택적)
      // 특정 TYPE에서만 value constraint가 있으면, 다른 TYPE에서는 숨김
      const xValueConstraint = (field as any)['x-value-constraint'];
      if (visible && xValueConstraint && typeof xValueConstraint === 'object') {
        const currentType = formValues['TYPE'];
        if (currentType !== undefined) {
          const constraintForType = xValueConstraint[currentType];
          // 이 TYPE에 대한 constraint가 없으면 숨김 (다른 TYPE 전용 필드)
          if (constraintForType === undefined && Object.keys(xValueConstraint).length > 0) {
            visible = false;
          }
        }
      }
      // 🔥 1.5: 그룹별 허용 TYPE 기반 visibility (가장 중요!)
      // 그룹 내 다른 필드들에서 수집한 TYPE 제약을 이 필드에도 적용
      const uiGroup = field.ui?.group;
      if (visible && uiGroup && typeof uiGroup === 'string') {
        const groupLower = uiGroup.toLowerCase();

        // "common" 키워드가 포함되면 항상 표시
        if (!groupLower.includes('common')) {
          const allowedTypes = groupAllowedTypes.get(uiGroup);
          if (allowedTypes && allowedTypes.size > 0) {
            const currentType = formValues['TYPE'];
            if (currentType !== undefined && !allowedTypes.has(currentType)) {
              // 현재 TYPE이 그룹의 허용 TYPE 목록에 없음 → 숨김
              visible = false;
            }
          }
        }
      }

      // 🔥 2. required 계산 (2축: required 모드 + requiredNow)
      const { required, requiredNow } = calculateFieldRequired(field, formValues, visible);

      // 🔥 3. enabled 계산
      // ⚠️ 현재는 visible과 동일하지만, 추후 readOnly/disabled 개념 추가 시 분리 가능
      // 예: const enabled = visible && !field.readOnly;
      const enabled = visible;

      // 🔥 4. 상태 저장
      stateMap[field.key] = {
        visible,
        required,
        requiredNow,
        enabled,
      };

      // 🔥 5. 자식 필드들도 처리 (중첩 필드)
      if (field.children && Array.isArray(field.children)) {
        for (const child of field.children) {
          let childVisible = evaluateVisibleWhen(child.ui?.visibleWhen, formValues);

          // 🔥 5.1: x-required-when 기반 visibility
          const childXRequiredWhen = (child as any)['x-required-when'];
          if (childVisible && childXRequiredWhen && typeof childXRequiredWhen === 'object') {
            const conditionMet = evaluateRequiredWhen(childXRequiredWhen, formValues);
            if (!conditionMet) {
              childVisible = false;
            }
          }

          // 🔥 5.2: x-required-by-type 기반 visibility
          const childXRequiredByType = (child as any)['x-required-by-type'];
          if (childVisible && childXRequiredByType && typeof childXRequiredByType === 'object') {
            const currentType = formValues['TYPE'];
            if (currentType !== undefined) {
              const typeStatus = childXRequiredByType[currentType];
              if (typeStatus === undefined || typeStatus === false) {
                childVisible = false;
              }
            }
          }

          // 🔥 5.3: x-enum-by-type 기반 visibility
          const childXEnumByType = (child as any)['x-enum-by-type'];
          if (childVisible && childXEnumByType && typeof childXEnumByType === 'object') {
            const currentType = formValues['TYPE'];
            if (currentType !== undefined) {
              const enumForType = childXEnumByType[currentType];
              if (enumForType === undefined) {
                childVisible = false;
              }
            }
          }

          const { required: childRequired, requiredNow: childRequiredNow } = calculateFieldRequired(child, formValues, childVisible);

          stateMap[child.key] = {
            visible: childVisible,
            required: childRequired,
            requiredNow: childRequiredNow,
            enabled: childVisible,
          };
        }
      }
    }
  }

  return stateMap;
}

/**
 * 🎯 Helper: 필드가 JSON에 포함되어야 하는지 판단
 * 
 * @param _fieldName - 필드명 (예약)
 * @param value - 필드 값
 * @param runtimeState - 필드 런타임 상태
 * @returns JSON에 포함할지 여부
 */
/**
 * 🎯 JSON 포함 여부 결정 (Single Source of Truth: runtimeState)
 * 
 * Rule: JSON 포함 여부는 runtimeState 단일 기준으로 판단
 * 
 * include if:
 *   runtimeState.visible === true AND
 *   (
 *     runtimeState.requiredNow === true
 *     OR
 *     (value !== undefined && value !== '' && value !== null)
 *   )
 * 
 * ⚠️ 중요: 0, false는 정상적인 값 (구조 인덱스, 방향, flag 등)
 * 
 * @param _fieldName - 필드명 (예약)
 * @param value - 필드 값
 * @param runtimeState - 필드 런타임 상태 (Single Source of Truth)
 * @returns JSON에 포함할지 여부
 */
export function shouldIncludeInJSON(
  _fieldName: string,
  value: any,
  runtimeState?: FieldRuntimeState
): boolean {
  // 🔥 Fallback: runtimeState가 없으면 의미있는 값이 있을 때만 포함
  if (!runtimeState) {
    return value !== undefined && value !== '' && value !== null;
  }

  // 🔥 Rule 1: 보이지 않으면 무조건 제외
  if (!runtimeState.visible) return false;

  // 🔥 Rule 2: Required면 값 없어도 포함 (key 존재가 의미)
  if (runtimeState.requiredNow) {
    return true;
  }

  // 🔥 Rule 3: Optional이면 의미있는 값만 포함 (값 존재가 의미)
  // ⚠️ 0, false는 정상 값이므로 제외하지 않음
  return value !== undefined && value !== '' && value !== null;
}
