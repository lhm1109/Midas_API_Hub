/**
 * Enhanced Schema → Original Schema 컨버터
 * 
 * Enhanced 스키마에서 x-* 확장 마커를 모두 제거하여
 * 표준 JSON Schema Draft-07 호환 Original 스키마로 변환합니다.
 * 
 * @module schemaConverter
 */

/**
 * Enhanced 스키마에서 사용되는 x-* 확장 마커 목록
 * shared.yaml의 markerRegistry에서 정의된 모든 마커
 */
export const ENHANCED_MARKERS = [
    'x-ui',
    'x-transport',
    'x-enum-by-type',
    'x-node-count-by-type',
    'x-value-constraint',
    'x-required-by-type',
    'x-required-when',
    'x-optional-when',
    'x-enum-labels',
    'x-enum-labels-by-type',
] as const;

/**
 * x-* 키 패턴 (모든 x- 접두사 키 매칭)
 */
const X_PREFIX_PATTERN = /^x-/;

/**
 * Enhanced Schema를 Original Schema로 변환
 * 
 * 변환 규칙:
 * 1. 모든 x-* 키를 재귀적으로 제거
 * 2. description, title, $schema 등 표준 속성은 유지
 * 3. allOf, oneOf, anyOf 등 조건부 스키마 구조는 유지
 * 
 * @param enhancedSchema - Enhanced 스키마 객체
 * @returns Original 스키마 객체 (x-* 마커 제거됨)
 */
export function convertEnhancedToOriginal<T extends object>(enhancedSchema: T): T {
    // Deep clone to avoid mutating original
    const original = JSON.parse(JSON.stringify(enhancedSchema)) as T;

    // Recursively remove x-* keys
    removeEnhancedMarkers(original);

    return original;
}

/**
 * 객체에서 x-* 키를 재귀적으로 제거
 * 
 * @param obj - 처리할 객체
 */
function removeEnhancedMarkers(obj: unknown): void {
    if (!obj || typeof obj !== 'object') return;

    if (Array.isArray(obj)) {
        obj.forEach(item => removeEnhancedMarkers(item));
        return;
    }

    const record = obj as Record<string, unknown>;

    // Find and remove all x-* keys
    const keysToRemove = Object.keys(record).filter(key => X_PREFIX_PATTERN.test(key));
    keysToRemove.forEach(key => delete record[key]);

    // Recursively process remaining values
    Object.values(record).forEach(value => {
        if (value && typeof value === 'object') {
            removeEnhancedMarkers(value);
        }
    });
}

/**
 * 스키마가 Enhanced 스키마인지 확인
 * 
 * @param schema - 확인할 스키마
 * @returns Enhanced 마커가 포함되어 있으면 true
 */
export function isEnhancedSchema(schema: unknown): boolean {
    if (!schema || typeof schema !== 'object') return false;

    const checkForMarkers = (obj: unknown): boolean => {
        if (!obj || typeof obj !== 'object') return false;

        if (Array.isArray(obj)) {
            return obj.some(item => checkForMarkers(item));
        }

        const record = obj as Record<string, unknown>;

        // Check if any key starts with x-
        if (Object.keys(record).some(key => X_PREFIX_PATTERN.test(key))) {
            return true;
        }

        // Recursively check values
        return Object.values(record).some(value => checkForMarkers(value));
    };

    return checkForMarkers(schema);
}

/**
 * 변환 결과 메타데이터
 */
export interface ConversionResult<T> {
    /** 변환된 Original 스키마 */
    schema: T;
    /** 변환 과정에서 제거된 x-* 키 개수 */
    removedMarkersCount: number;
    /** 제거된 마커 키 목록 (중복 제거) */
    removedMarkerKeys: string[];
    /** 변환 성공 여부 */
    success: boolean;
}

/**
 * Enhanced Schema를 Original Schema로 변환 (상세 결과 포함)
 * 
 * @param enhancedSchema - Enhanced 스키마 객체
 * @returns 변환 결과 (메타데이터 포함)
 */
export function convertWithDetails<T extends object>(enhancedSchema: T): ConversionResult<T> {
    const original = JSON.parse(JSON.stringify(enhancedSchema)) as T;
    const removedKeys = new Set<string>();
    let removedCount = 0;

    const removeWithTracking = (obj: unknown): void => {
        if (!obj || typeof obj !== 'object') return;

        if (Array.isArray(obj)) {
            obj.forEach(item => removeWithTracking(item));
            return;
        }

        const record = obj as Record<string, unknown>;

        Object.keys(record).forEach(key => {
            if (X_PREFIX_PATTERN.test(key)) {
                removedKeys.add(key);
                removedCount++;
                delete record[key];
            }
        });

        Object.values(record).forEach(value => {
            if (value && typeof value === 'object') {
                removeWithTracking(value);
            }
        });
    };

    removeWithTracking(original);

    return {
        schema: original,
        removedMarkersCount: removedCount,
        removedMarkerKeys: Array.from(removedKeys).sort(),
        success: true,
    };
}
