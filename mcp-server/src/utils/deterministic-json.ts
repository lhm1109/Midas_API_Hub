/**
 * 결정론적 JSON 출력
 * - 키 알파벳 정렬 (재귀적)
 * - 배열 순서 유지
 * - 2 spaces 들여쓰기
 * - LF 줄바꿈
 */

/**
 * 객체 키를 알파벳 순으로 정렬 (재귀적)
 * 배열은 순서 유지
 */
function sortKeysDeep(obj: unknown): unknown {
    if (Array.isArray(obj)) {
        // 배열은 순서 유지, 내부 객체만 정렬
        return obj.map(sortKeysDeep);
    }
    if (obj !== null && typeof obj === 'object') {
        return Object.keys(obj as Record<string, unknown>)
            .sort()
            .reduce((acc, key) => {
                acc[key] = sortKeysDeep((obj as Record<string, unknown>)[key]);
                return acc;
            }, {} as Record<string, unknown>);
    }
    return obj;
}

/**
 * 결정론적 JSON 문자열 생성
 * - 키 정렬
 * - 2 spaces
 * - LF 줄바꿈
 */
export function toCanonicalJSON(obj: unknown): string {
    const sorted = sortKeysDeep(obj);
    return JSON.stringify(sorted, null, 2).replace(/\r\n/g, '\n') + '\n';
}
