/**
 * YAML → JSON Schema 변환 규칙 (결정론적)
 */

export const RULES_VERSION = '1.0.0';
export const JSON_SCHEMA_VERSION = 'http://json-schema.org/draft-07/schema#';

/**
 * URI 패턴 → Wrapper 결정
 */
export const WRAPPER_RULES = [
    { pattern: /^\/db\//, wrapper: 'Assign' as const },
    { pattern: /^\/post\//, wrapper: 'Argument' as const },
    { pattern: /^\/doc\//, wrapper: 'Argument' as const },
];

/**
 * Enhanced Schema 마커
 */
export const ENHANCED_MARKERS = [
    'x-ui',
    'x-transport',
    'x-enum-by-type',
    'x-node-count-by-type',
    'x-value-constraint',
];

/**
 * 필드 타입 추론 규칙 (접두사 기반)
 */
export const TYPE_INFERENCE_PREFIX: Record<string, string> = {
    'i': 'integer',   // iMETHOD → integer
    'b': 'boolean',   // bUSE → boolean
    'd': 'number',    // dVALUE → number
    's': 'string',    // sTEXT → string
    'n': 'number',    // nCOUNT → number
};

/**
 * URI 패턴에 따른 wrapper 결정
 */
export function getWrapperForUri(uri: string): 'Assign' | 'Argument' | null {
    for (const rule of WRAPPER_RULES) {
        if (rule.pattern.test(uri)) {
            return rule.wrapper;
        }
    }
    return null;
}

/**
 * Enhanced Schema 마커 개수 카운트
 */
export function countEnhancedMarkers(content: string): number {
    let count = 0;
    for (const marker of ENHANCED_MARKERS) {
        const regex = new RegExp(marker, 'g');
        const matches = content.match(regex);
        if (matches) {
            count += matches.length;
        }
    }
    return count;
}

/**
 * 필드명에서 타입 추론
 */
export function inferTypeFromFieldName(fieldName: string): string | null {
    if (fieldName.length < 2) return null;
    const prefix = fieldName[0].toLowerCase();
    return TYPE_INFERENCE_PREFIX[prefix] || null;
}
