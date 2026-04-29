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
import type { FieldRuntimeState, FieldRuntimeStateMap } from '@/lib/schema/fieldRuntimeState';
import type { ValidationOneOfInfo } from '@/lib/schema/validationOneOf';

function resolveNestedFieldKey(parentFieldName: string, childFieldName: string): string {
    if (!childFieldName) return childFieldName;
    if (childFieldName === parentFieldName || childFieldName.startsWith(`${parentFieldName}.`)) {
        return childFieldName;
    }
    return `${parentFieldName}.${childFieldName}`;
}

function findFieldByPath(fieldPath: string, schemaFields: UIBuilderField[]): UIBuilderField | undefined {
    for (const field of schemaFields) {
        if (field.name === fieldPath) {
            return field;
        }

        if (field.children && field.children.length > 0) {
            const matchedChild = findFieldByPath(fieldPath, field.children);
            if (matchedChild) {
                return matchedChild;
            }
        }
    }

    return undefined;
}

function getFieldLeafName(fieldPath: string): string {
    return fieldPath.replace(/\[\]/g, '').split('.').filter(Boolean).pop() || fieldPath;
}

function cloneValue<T>(value: T): T {
    if (value === undefined || value === null) {
        return value;
    }

    try {
        return JSON.parse(JSON.stringify(value));
    } catch {
        return value;
    }
}

function isPlainObject(value: any): value is Record<string, any> {
    return !!value && typeof value === 'object' && !Array.isArray(value);
}

function isMatchingFieldPath(key: string, fieldPath: string): boolean {
    return (
        key === fieldPath ||
        key.startsWith(`${fieldPath}.`) ||
        key.startsWith(`${fieldPath}[`) ||
        key.startsWith(`${fieldPath}._`)
    );
}

function findTopLevelFieldByParticipantKey(
    schemaFields: UIBuilderField[],
    participantKey: string
): UIBuilderField | undefined {
    return schemaFields.find((field) => {
        if (field.name.startsWith('__section_')) {
            return false;
        }

        return getFieldLeafName(field.name) === participantKey;
    });
}

function clearFieldState(target: Record<string, any>, field: UIBuilderField): void {
    delete target[field.name];
    delete target[`${field.name}._enabled`];
    delete target[`${field.name}.__selectedOption`];

    field.children?.forEach((child) => {
        clearFieldState(target, child);
    });
}

export function hasMeaningfulFieldValue(value: any): boolean {
    if (value === undefined || value === null) return false;

    if (typeof value === 'string') {
        const trimmed = value.trim();
        if (!trimmed) return false;

        try {
            const parsed = JSON.parse(trimmed);
            if (Array.isArray(parsed)) return parsed.length > 0;
            if (parsed && typeof parsed === 'object') return Object.keys(parsed).length > 0;
        } catch {
            return true;
        }

        return true;
    }

    if (Array.isArray(value)) return value.length > 0;
    if (typeof value === 'object') return Object.keys(value).length > 0;

    return true;
}

function hasDataForField(sourceData: Record<string, any>, field: UIBuilderField): boolean {
    if (hasMeaningfulFieldValue(sourceData[field.name])) {
        return true;
    }

    if (sourceData[`${field.name}._enabled`] === true) {
        return true;
    }

    return Object.entries(sourceData).some(([key, value]) => {
        if (!isMatchingFieldPath(key, field.name) || key === field.name) {
            return false;
        }

        if (key.endsWith('._enabled')) {
            return value === true;
        }

        return hasMeaningfulFieldValue(value);
    });
}

export function shouldPreserveObjectValue(field?: UIBuilderField): boolean {
    if (!field || field.type !== 'object') {
        return false;
    }

    return field.isKeyedObject === true || !field.children || field.children.length === 0;
}

export function normalizeBuilderFieldPath(path: string): string {
    return path
        .replace(/\[\]/g, '')
        .split('.')
        .map((part) => part.trim())
        .filter(Boolean)
        .join('.');
}

export function normalizeJsonPreviewFieldPath(fieldPath: string, wrapperKeys: string[]): string {
    const wrapperKeySet = new Set(wrapperKeys.filter(Boolean));

    return fieldPath
        .replace(/\[\]/g, '')
        .split('.')
        .map((part) => part.trim())
        .filter((part) =>
            Boolean(part) &&
            !wrapperKeySet.has(part) &&
            !/^\d+$/.test(part) &&
            !part.startsWith('__section_')
        )
        .join('.');
}

export function buildNormalizedFieldLookup(schemaFields: UIBuilderField[]): Map<string, UIBuilderField> {
    const lookup = new Map<string, UIBuilderField>();

    const visit = (fields: UIBuilderField[]) => {
        fields.forEach((field) => {
            lookup.set(normalizeBuilderFieldPath(field.name), field);

            if (field.children && field.children.length > 0) {
                visit(field.children);
            }
        });
    };

    visit(schemaFields);
    return lookup;
}

export function buildNormalizedRuntimeStateLookup(runtimeStates: FieldRuntimeStateMap): Map<string, FieldRuntimeState> {
    const lookup = new Map<string, FieldRuntimeState>();

    Object.entries(runtimeStates).forEach(([fieldKey, runtimeState]) => {
        lookup.set(normalizeBuilderFieldPath(fieldKey), runtimeState);
    });

    return lookup;
}

function normalizeArrayToken(token: string): string {
    return token.trim().replace(/^["']|["']$/g, '');
}

function parseLooseArrayString(raw: string): string[] | null {
    const trimmed = raw.trim();
    if (!trimmed.startsWith('[') || !trimmed.endsWith(']')) {
        return null;
    }

    const inner = trimmed.slice(1, -1).trim();
    if (!inner) return [];

    return inner
        .split(',')
        .map(normalizeArrayToken)
        .filter(Boolean);
}

function castScalarByType(value: any, type?: string): any {
    if (typeof value !== 'string') return value;
    const trimmed = value.trim();

    if (type === 'integer') {
        const parsed = Number.parseInt(trimmed, 10);
        return Number.isNaN(parsed) ? value : parsed;
    }

    if (type === 'number') {
        const parsed = Number.parseFloat(trimmed);
        return Number.isNaN(parsed) ? value : parsed;
    }

    if (type === 'boolean') {
        if (trimmed.toLowerCase() === 'true') return true;
        if (trimmed.toLowerCase() === 'false') return false;
    }

    return value;
}

function normalizeStringValue(value: any): any {
    if (typeof value !== 'string') return value;

    const trimmed = value.trim();
    if (trimmed.length < 2) return value;

    if (trimmed.startsWith('"') && trimmed.endsWith('"')) {
        try {
            const parsed = JSON.parse(trimmed);
            if (typeof parsed === 'string') {
                return parsed;
            }
        } catch {
            return value;
        }
    }

    if (trimmed.startsWith("'") && trimmed.endsWith("'")) {
        return trimmed.slice(1, -1);
    }

    return value;
}

function coerceArrayValue(value: any, itemType?: string): any {
    if (Array.isArray(value)) {
        return value.map((item) => castScalarByType(item, itemType));
    }

    if (typeof value !== 'string') return value;
    const trimmed = value.trim();
    if (!trimmed) return [];

    try {
        const parsed = JSON.parse(trimmed);
        if (Array.isArray(parsed)) {
            return parsed.map((item) => castScalarByType(item, itemType));
        }
    } catch {
        // no-op: fallback below
    }

    const looseParsed = parseLooseArrayString(trimmed);
    if (looseParsed) {
        return looseParsed.map((item) => castScalarByType(item, itemType));
    }

    return [castScalarByType(normalizeArrayToken(trimmed), itemType)];
}

function findObjectChildField(children: UIBuilderField[] | undefined, key: string): UIBuilderField | undefined {
    if (!children || children.length === 0) {
        return undefined;
    }

    const normalizedKey = normalizeBuilderFieldPath(key);
    return children.find((child) => {
        if (child.name === key) return true;
        if (getFieldLeafName(child.name) === key) return true;
        return normalizeBuilderFieldPath(child.name) === normalizedKey;
    });
}

function coerceObjectValue(value: any, field: UIBuilderField): any {
    if (!isPlainObject(value) || !field.children || field.children.length === 0) {
        return value;
    }

    const coerceEntryObject = (entryValue: any): any => {
        if (!isPlainObject(entryValue)) {
            return entryValue;
        }

        const coercedEntry: Record<string, any> = {};
        Object.entries(entryValue).forEach(([entryKey, childValue]) => {
            const childField = findObjectChildField(field.children, entryKey);
            coercedEntry[entryKey] = coerceValueForBuilderField(childValue, childField);
        });
        return coercedEntry;
    };

    if (field.isKeyedObject) {
        const coercedObject: Record<string, any> = {};
        Object.entries(value).forEach(([entryKey, entryValue]) => {
            coercedObject[entryKey] = coerceEntryObject(entryValue);
        });
        return coercedObject;
    }

    return coerceEntryObject(value);
}

export function coerceValueForBuilderField(value: any, field?: UIBuilderField): any {
    if (!field) return value;

    if (field.type === 'array') {
        return coerceArrayValue(value, field.items?.type);
    }

    if (field.type === 'object') {
        return coerceObjectValue(value, field);
    }

    if (field.type === 'number' || field.type === 'integer' || field.type === 'boolean') {
        return castScalarByType(value, field.type);
    }

    if (field.type === 'string') {
        return normalizeStringValue(value);
    }

    return value;
}

export function resolveBuilderFieldByPath(path: string, schemaFields: UIBuilderField[]): UIBuilderField | undefined {
    const normalizedPath = normalizeBuilderFieldPath(path);

    const visit = (fields: UIBuilderField[]): UIBuilderField | undefined => {
        for (const field of fields) {
            if (normalizeBuilderFieldPath(field.name) === normalizedPath) {
                return field;
            }

            if (field.children && field.children.length > 0) {
                const matchedChild = visit(field.children);
                if (matchedChild) {
                    return matchedChild;
                }
            }
        }

        return undefined;
    };

    return visit(schemaFields);
}

function collectObjectDefaultValues(
    field: UIBuilderField,
    target: Record<string, any>,
    sourceDefault: any = field.default
): void {
    if (!field.children || field.children.length === 0) {
        return;
    }

    if (!isPlainObject(sourceDefault)) {
        return;
    }

    field.children.forEach((child) => {
        if (child.name.startsWith('__section_')) {
            return;
        }

        const childPath = resolveNestedFieldKey(field.name, child.name);
        const childLeafName = getFieldLeafName(child.name);
        if (!Object.prototype.hasOwnProperty.call(sourceDefault, childLeafName)) {
            return;
        }

        const childDefault = sourceDefault[childLeafName];

        if (child.type === 'object' && child.children && child.children.length > 0) {
            if (isPlainObject(childDefault)) {
                target[`${childPath}._enabled`] = true;
                collectObjectDefaultValues(child, target, childDefault);
            }
            return;
        }

        if (childDefault !== undefined) {
            target[childPath] = cloneValue(childDefault);
        }
    });
}

export function applyEnabledObjectDefaults(
    previousState: Record<string, any>,
    key: string,
    value: any,
    schemaFields: UIBuilderField[]
): Record<string, any> {
    const updatedState = { ...previousState, [key]: value };

    if (!key.endsWith('._enabled')) {
        return updatedState;
    }

    const fieldPath = key.slice(0, -'._enabled'.length);
    const field = resolveBuilderFieldByPath(fieldPath, schemaFields);
    if (!field || field.type !== 'object' || !field.children || field.children.length === 0) {
        return updatedState;
    }

    if (value === false) {
        field.children.forEach((child) => clearFieldState(updatedState, child));
        return updatedState;
    }

    if (value !== true) {
        return updatedState;
    }

    const defaults: Record<string, any> = {};
    collectObjectDefaultValues(field, defaults);

    Object.entries(defaults).forEach(([defaultKey, defaultValue]) => {
        if (
            updatedState[defaultKey] === undefined ||
            updatedState[defaultKey] === null ||
            updatedState[defaultKey] === ''
        ) {
            updatedState[defaultKey] = defaultValue;
        }
    });

    return updatedState;
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

export function buildRootValidationOneOfOptionLabels(
    info: ValidationOneOfInfo | null | undefined,
    schemaFields: UIBuilderField[]
): string[] {
    if (!info) {
        return [];
    }

    return info.optionKeyGroups.map((group, index) => {
        const labels = group
            .map((participantKey) => {
                const field = findTopLevelFieldByParticipantKey(schemaFields, participantKey);
                return field?.description || participantKey;
            })
            .filter(Boolean);

        if (labels.length === 0) {
            return `Option ${index + 1}`;
        }

        return labels.join(' + ');
    });
}

export function inferRootValidationOneOfSelection(
    info: ValidationOneOfInfo | null | undefined,
    schemaFields: UIBuilderField[],
    sourceData: Record<string, any>
): number {
    if (!info || info.optionKeyGroups.length === 0) {
        return 0;
    }

    const matchedIndex = info.optionKeyGroups.findIndex((group) =>
        group.some((participantKey) => {
            const field = findTopLevelFieldByParticipantKey(schemaFields, participantKey);
            return field ? hasDataForField(sourceData, field) : false;
        })
    );

    return matchedIndex >= 0 ? matchedIndex : 0;
}

export function getRootValidationOneOfOptionIndexForFieldKey(
    key: string,
    info: ValidationOneOfInfo | null | undefined,
    schemaFields: UIBuilderField[]
): number {
    if (!info) {
        return -1;
    }

    return info.optionKeyGroups.findIndex((group) =>
        group.some((participantKey) => {
            const field = findTopLevelFieldByParticipantKey(schemaFields, participantKey);
            return field ? isMatchingFieldPath(key, field.name) : false;
        })
    );
}

export function isRootValidationOneOfFieldVisible(
    field: UIBuilderField,
    info: ValidationOneOfInfo | null | undefined,
    selectedOptionIndex: number
): boolean {
    if (!info) {
        return true;
    }

    const fieldLeaf = getFieldLeafName(field.name);
    if (!info.participantKeys.includes(fieldLeaf)) {
        return true;
    }

    const selectedGroup = info.optionKeyGroups[selectedOptionIndex] || info.optionKeyGroups[0] || [];
    return selectedGroup.includes(fieldLeaf);
}

export function applyRootValidationOneOfSelection(
    previousState: Record<string, any>,
    selectedOptionIndex: number,
    schemaFields: UIBuilderField[],
    info: ValidationOneOfInfo | null | undefined
): Record<string, any> {
    if (!info || info.optionKeyGroups.length === 0) {
        return previousState;
    }

    const selectedGroup = info.optionKeyGroups[selectedOptionIndex] || info.optionKeyGroups[0];
    const updatedState: Record<string, any> = {
        ...previousState,
        '__root__.__selectedOption': selectedOptionIndex,
    };

    info.participantKeys.forEach((participantKey) => {
        const field = findTopLevelFieldByParticipantKey(schemaFields, participantKey);
        if (!field) {
            return;
        }

        if (selectedGroup.includes(participantKey)) {
            if (field.type === 'object' && field.children && !field.isKeyedObject) {
                updatedState[`${field.name}._enabled`] = true;
            }
            return;
        }

        clearFieldState(updatedState, field);
    });

    return updatedState;
}

export function shouldIncludeFieldForRootValidationOneOf(
    key: string,
    info: ValidationOneOfInfo | null | undefined,
    schemaFields: UIBuilderField[],
    selectedOptionIndex: number
): boolean {
    if (!info) {
        return true;
    }

    const selectedGroup = info.optionKeyGroups[selectedOptionIndex] || info.optionKeyGroups[0] || [];

    for (const participantKey of info.participantKeys) {
        const field = findTopLevelFieldByParticipantKey(schemaFields, participantKey);
        if (!field || !isMatchingFieldPath(key, field.name)) {
            continue;
        }

        return selectedGroup.includes(participantKey);
    }

    return true;
}

// ============================================================================
// 타입 정의
// ============================================================================
export interface InitializationResult {
    data: Record<string, any>;
    triggersInitialized: string[];
    requiredFieldsInitialized: string[];
}
