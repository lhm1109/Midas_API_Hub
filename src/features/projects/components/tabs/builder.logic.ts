/**
 * @file builder.logic.ts
 * @description BuilderTab 순수 함수 추출 - PR#1
 *
 * ⚠️ 원칙:
 * - 호출 시그니처와 동작은 BuilderTab.tsx와 100% 동일
 * - 조건/순서/분기 기준 변경 금지
 * - 복붙 수준의 이동만 수행
 */

import type { UIBuilderField } from '@/lib/schema';

function resolveNestedFieldKey(parentFieldName: string, childFieldName: string): string {
    if (!childFieldName) return childFieldName;
    if (childFieldName === parentFieldName || childFieldName.startsWith(`${parentFieldName}.`)) {
        return childFieldName;
    }
    return `${parentFieldName}.${childFieldName}`;
}

function findFieldByPath(fieldPath: string, schemaFields: UIBuilderField[]): UIBuilderField | undefined {
    return schemaFields.find((field) => field.name === fieldPath);
}

export function shouldPreserveObjectValue(field?: UIBuilderField): boolean {
    if (!field || field.type !== 'object') {
        return false;
    }

    return field.isKeyedObject === true || !field.children || field.children.length === 0;
}

// ============================================================================
// getDefaultValue: 필드의 기본값 반환
// 원본 위치: BuilderTab.tsx 라인 289-313
// ============================================================================
/**
 * 필드의 기본값을 반환합니다.
 * - forceValue=true: Required 필드로 간주, null 반환 (key는 존재해야 함)
 * - forceValue=false: Optional 필드로 간주, '' 반환 (나중에 필터링됨)
 */
export function getDefaultValue(field: UIBuilderField, forceValue: boolean = false): any {
    // 1. 명시적 default 값이 있으면 사용
    if (field.default !== undefined && field.default !== null) {
        return field.default;
    }

    // 2. enum 값이 있으면 첫 번째 값 사용 (type과 무관하게)
    // 🔥 FIX: integer/number 타입도 enum을 가질 수 있음 (드롭다운으로 표시)
    if (field.enum && field.enum.length > 0) {
        return field.enum[0];
    }

    // 3. 타입별 초기값
    if (field.type === 'array') return [];  // 배열은 빈 배열
    if (field.type === 'boolean') return false;  // boolean은 false
    if (field.type === 'object' && field.isKeyedObject) return {};  // keyed object는 빈 object

    // 4. 🔥 FIX: Required 필드는 null 반환 (forceValue=true일 때)
    // forceValue=true: Required 필드로 간주, JSON에 key가 포함되어야 함
    if (forceValue) {
        return null;  // Required 필드는 null로 표시 (key는 존재)
    }

    // 5. Optional 필드는 빈 문자열 (나중에 필터링됨)
    return '';
}

// ============================================================================
// isTriggerField: 필드가 Trigger 필드인지 판별
// 원본 위치: BuilderTab.tsx 여러 곳에서 반복되는 조건 (라인 353, 392, 434, 584 등)
// ============================================================================
/**
 * Trigger 필드 판별: enum이 있고 조건부 visibility가 없는 필드
 * - x-required-when이 있으면 조건부 → Trigger 아님
 */
export function isTriggerField(field: UIBuilderField): boolean {
    // 🔥 FIX: x-required-when이 있는 enum 필드는 조건부 → Trigger가 아님
    const xRequiredWhen = (field as any)['x-required-when'];
    const hasConditionalVisibility = xRequiredWhen && typeof xRequiredWhen === 'object';

    // ✅ Trigger 필드: enum이 있고 조건부 visibility가 없는 필드
    return !!(field.enum && Array.isArray(field.enum) && field.enum.length > 0 && !hasConditionalVisibility);
}

// ============================================================================
// isAlwaysRequired: 필드가 항상 Required인지 판별
// 원본 위치: BuilderTab.tsx 여러 곳에서 반복되는 조건 (라인 356-359, 395-398 등)
// ============================================================================
/**
 * Always Required 필드 판별
 * - required === true
 * - 또는 required['*'] === 'required'
 */
export function isAlwaysRequired(field: UIBuilderField): boolean {
    return (
        field.required === true ||
        (typeof field.required === 'object' &&
            (field.required as any)['*'] === 'required')
    );
}

// ============================================================================
// shouldInitializeField: 초기화 대상 필드인지 판별
// 원본 위치: 7곳에서 반복되는 (isTriggerField || isAlwaysRequired) 조건
// ============================================================================
/**
 * 초기화 대상 필드 판별: Trigger 또는 Always Required
 */
export function shouldInitializeField(field: UIBuilderField): boolean {
    return isTriggerField(field) || isAlwaysRequired(field);
}

// ============================================================================
// initializeFieldValue: 필드 하나에 대한 초기값 설정
// 원본 위치: BuilderTab.tsx 라인 362-372, 401-411 등 반복 패턴
// ============================================================================
/**
 * 필드 하나에 대한 초기값을 data 객체에 설정합니다.
 * ⚠️ 이 함수는 data 객체를 직접 수정합니다 (mutates in-place)
 */
export function initializeFieldValue(
    field: UIBuilderField,
    data: Record<string, any>,
    getDefaultValueFn: (field: UIBuilderField, forceValue?: boolean) => any = getDefaultValue
): void {
    if (field.type === 'array' && field.items) {
        data[field.name] = getDefaultValueFn(field);
    } else if (field.type === 'object' && field.isKeyedObject) {
        data[field.name] = getDefaultValueFn(field);
    } else if (field.type === 'object' && field.children) {
        data[`${field.name}._enabled`] = false;
        field.children.forEach(child => {
            data[resolveNestedFieldKey(field.name, child.name)] = getDefaultValueFn(child);
        });
    } else {
        data[field.name] = getDefaultValueFn(field);
    }
}

// ============================================================================
// buildInitialDynamicFormData: schemaFields에서 초기 폼 데이터 생성
// 원본 위치: BuilderTab.tsx 라인 341-380, 382-419 등
// ============================================================================
/**
 * schemaFields에서 초기 dynamicFormData를 생성합니다.
 * - Trigger + Required 필드만 초기화
 * - Optional/Conditional 필드는 key 자체를 만들지 않음
 *
 * @param schemaFields - UIBuilderField 배열
 * @param existingData - 기존 데이터 (있으면 유지)
 * @returns 초기화된 폼 데이터 객체
 */
export function buildInitialDynamicFormData(
    schemaFields: UIBuilderField[],
    existingData: Record<string, any> = {}
): Record<string, any> {
    const initialData: Record<string, any> = { ...existingData };

    schemaFields.forEach(field => {
        // 🎯 Trigger 또는 Always Required만 초기화
        if (shouldInitializeField(field)) {
            initializeFieldValue(field, initialData, getDefaultValue);
        }
        // 🔥 Optional/Conditional 필드는 key 자체를 만들지 않음
    });

    return initialData;
}

// ============================================================================
// buildAssignInstanceInitialData: Assign 인스턴스 초기 데이터 생성
// 원본 위치: BuilderTab.tsx 라인 383-419
// ============================================================================
/**
 * 새 Assign 인스턴스의 초기 데이터를 생성합니다.
 */
export function buildAssignInstanceInitialData(
    schemaFields: UIBuilderField[]
): Record<string, any> {
    return buildInitialDynamicFormData(schemaFields, {});
}

export function flattenObjectToDotNotationWithSchema(
    obj: any,
    target: Record<string, any>,
    schemaFields: UIBuilderField[],
    prefix = ''
): void {
    Object.keys(obj).forEach((key) => {
        const value = obj[key];
        const newKey = prefix ? `${prefix}.${key}` : key;
        const matchedField = findFieldByPath(newKey, schemaFields);

        if (value !== null && typeof value === 'object' && !Array.isArray(value)) {
            if (shouldPreserveObjectValue(matchedField)) {
                target[newKey] = value;
                return;
            }

            target[`${newKey}._enabled`] = true;
            flattenObjectToDotNotationWithSchema(value, target, schemaFields, newKey);
            return;
        }

        target[newKey] = value;
    });
}

// ============================================================================
// 타입 정의
// ============================================================================
export interface InitializationResult {
    data: Record<string, any>;
    triggersInitialized: string[];
    requiredFieldsInitialized: string[];
}
