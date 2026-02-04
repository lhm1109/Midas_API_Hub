/**
 * 결정론적 JSON 출력
 * - 스키마 핵심 키는 의미있는 순서 유지
 * - 나머지 키는 알파벳 정렬
 * - 배열 순서 유지
 * - 2 spaces 들여쓰기
 * - LF 줄바꿈
 */

// JSON Schema 키 우선순위 (낮은 숫자가 먼저 출력)
const SCHEMA_KEY_PRIORITY: Record<string, number> = {
    '$schema': 0,
    'title': 1,
    'type': 2,
    'description': 3,
    'required': 4,
    'additionalProperties': 5,
    'properties': 6,
    'items': 7,
    'enum': 8,
    'const': 9,
    'default': 10,
    'minimum': 11,
    'maximum': 12,
    'minItems': 13,
    'maxItems': 14,
    'minProperties': 15,
    'maxProperties': 16,
    'oneOf': 17,
    'anyOf': 18,
    'allOf': 19,
    'if': 20,
    'then': 21,
    'else': 22,
    // x-* markers come last
    'x-ui': 100,
    'x-transport': 101,
    'x-enum-labels': 102,
    'x-enum-labels-by-type': 103,
    'x-required-when': 104,
    'x-optional-when': 105,
};

/**
 * 키 정렬 비교 함수
 * - SCHEMA_KEY_PRIORITY에 정의된 키는 우선순위대로
 * - 나머지는 알파벳순
 */
function compareKeys(a: string, b: string): number {
    const priorityA = SCHEMA_KEY_PRIORITY[a] ?? 50;  // 정의 안된건 중간 우선순위
    const priorityB = SCHEMA_KEY_PRIORITY[b] ?? 50;

    if (priorityA !== priorityB) {
        return priorityA - priorityB;
    }
    // 같은 우선순위면 알파벳순
    return a.localeCompare(b);
}

/**
 * 객체 키를 의미있는 순서로 정렬 (재귀적)
 * 배열은 순서 유지
 * properties, patternProperties 내부 필드는 x-ui.order 기준 정렬
 */
function sortKeysDeep(obj: unknown, preserveOrder: boolean = false): unknown {
    if (Array.isArray(obj)) {
        // 배열은 순서 유지, 내부 객체만 정렬
        return obj.map(item => sortKeysDeep(item, false));
    }
    if (obj !== null && typeof obj === 'object') {
        const typedObj = obj as Record<string, unknown>;

        if (preserveOrder) {
            // 🔥 properties/patternProperties 내부: x-ui.order 기준 정렬
            const keys = Object.keys(typedObj);

            // x-ui.order 값으로 정렬
            keys.sort((a, b) => {
                const fieldA = typedObj[a] as Record<string, unknown> | null;
                const fieldB = typedObj[b] as Record<string, unknown> | null;

                const orderA = (fieldA?.['x-ui'] as Record<string, unknown>)?.order as number | undefined;
                const orderB = (fieldB?.['x-ui'] as Record<string, unknown>)?.order as number | undefined;

                // order가 있으면 그것으로 정렬, 없으면 원래 순서 유지 (stable sort)
                if (orderA !== undefined && orderB !== undefined) {
                    return orderA - orderB;
                }
                if (orderA !== undefined) return -1;  // order 있는게 먼저
                if (orderB !== undefined) return 1;

                // 🔥 둘 다 order가 없으면 원래 순서 유지 (알파벳 정렬 X)
                // Table 스키마 등에서 정의된 순서가 중요함
                return 0;
            });

            return keys.reduce((acc, key) => {
                acc[key] = sortKeysDeep(typedObj[key], false);
                return acc;
            }, {} as Record<string, unknown>);
        }

        // 스키마 키 정렬
        return Object.keys(typedObj)
            .sort(compareKeys)
            .reduce((acc, key) => {
                // properties와 patternProperties 내부는 x-ui.order 기준 정렬
                const shouldPreserve = key === 'properties' || key === 'patternProperties';
                acc[key] = sortKeysDeep(typedObj[key], shouldPreserve);
                return acc;
            }, {} as Record<string, unknown>);
    }
    return obj;
}

/**
 * 결정론적 JSON 문자열 생성
 * - 스키마 키 우선순위 정렬
 * - 2 spaces
 * - LF 줄바꿈
 */
export function toCanonicalJSON(obj: unknown): string {
    const sorted = sortKeysDeep(obj);
    return JSON.stringify(sorted, null, 2).replace(/\r\n/g, '\n') + '\n';
}

