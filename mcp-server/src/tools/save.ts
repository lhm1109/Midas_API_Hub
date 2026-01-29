/**
 * save_schema MCP Tool
 * AI가 생성한 스키마를 저장 (YAML 규칙 기반 검증/변환)
 */

import * as fs from 'fs';
import { buildWritePath } from '../utils/path-guard.js';
import { toCanonicalJSON } from '../utils/deterministic-json.js';
import { sha256 } from '../utils/hash.js';
import { RULES_VERSION, JSON_SCHEMA_VERSION } from '../rules/transform.js';
import { validateAndTransform } from '../utils/schema-validator.js';

export interface SaveSchemaInput {
    schemaName: string;
    outputDir?: string;
    schema: Record<string, unknown>;
    description?: string;
    questions?: Question[];
    // 새 옵션
    skipValidation?: boolean;  // 검증 건너뛰기 (디버깅용)
}

export interface Question {
    field: string;
    question: string;
    context?: string;
    suggestion?: unknown;
}

export interface SaveSchemaResult {
    ok: boolean;
    status: 'saved' | 'pending_questions';
    schemaPath?: string;
    metaPath?: string;
    outputHash?: string;
    pendingQuestions?: Question[];
    // 검증 결과 추가
    validation?: {
        errors: Array<{ field: string; message: string; fixApplied?: boolean }>;
        warnings: Array<{ field: string; message: string }>;
    };
    stats?: {
        fieldCount: number;
        requiredCount: number;
    };
    error?: string;
}

/**
 * save_schema 도구 실행
 */
export async function saveSchema(input: SaveSchemaInput): Promise<SaveSchemaResult> {
    try {
        // 1. 입력 검증
        if (!input.schemaName) {
            throw new Error('schemaName is required');
        }
        if (!input.schema || typeof input.schema !== 'object') {
            throw new Error('schema is required and must be an object');
        }

        // 2. 질문이 있으면 저장하지 않고 질문 반환
        if (input.questions && input.questions.length > 0) {
            return {
                ok: true,
                status: 'pending_questions',
                pendingQuestions: input.questions,
            };
        }

        const outputDir = input.outputDir || 'default';

        // 3. YAML 규칙 기반 검증 및 변환
        let schema = { ...input.schema };
        let validationResult = null;

        if (!input.skipValidation) {
            validationResult = validateAndTransform(input.schema);
            schema = validationResult.transformed as Record<string, unknown>;
        }

        // 4. $schema 추가 (없으면)
        if (!schema['$schema']) {
            schema['$schema'] = JSON_SCHEMA_VERSION;
        }

        // 5. 쓰기 경로 빌드
        const paths = buildWritePath(outputDir, input.schemaName);

        // 6. 통계 수집
        const properties = schema['properties'] as Record<string, unknown> | undefined;
        const required = schema['required'] as string[] | undefined;
        const fieldCount = properties ? Object.keys(properties).length : 0;
        const requiredCount = required ? required.length : 0;

        // 7. 결정론적 JSON 생성
        const schemaJson = toCanonicalJSON(schema);
        const outputHash = sha256(schemaJson);

        // 8. 메타데이터 생성
        const meta = {
            rulesVersion: RULES_VERSION,
            jsonSchemaVersion: 'draft-07',
            description: input.description || null,
            outputHash,
            generatedAt: new Date().toISOString(),
            stats: { fieldCount, requiredCount },
            validation: validationResult ? {
                errorsFixed: validationResult.errors.filter(e => e.fixApplied).length,
                warnings: validationResult.warnings.length,
            } : null,
        };
        const metaJson = toCanonicalJSON(meta);

        // 9. 파일 저장
        fs.writeFileSync(paths.schemaPath, schemaJson, 'utf-8');
        fs.writeFileSync(paths.metaPath, metaJson, 'utf-8');

        return {
            ok: true,
            status: 'saved',
            schemaPath: paths.schemaPath,
            metaPath: paths.metaPath,
            outputHash,
            validation: validationResult ? {
                errors: validationResult.errors,
                warnings: validationResult.warnings,
            } : undefined,
            stats: { fieldCount, requiredCount },
        };

    } catch (error) {
        const message = error instanceof Error ? error.message : String(error);
        return {
            ok: false,
            status: 'saved',
            error: message,
        };
    }
}

/**
 * MCP Tool Definition
 */
export const saveSchemaTool = {
    name: 'save_schema',
    description: 'AI가 생성한 JSON Schema를 저장. YAML 규칙 기반 자동 검증/변환 (enum→oneOf 등). 불확실한 항목은 questions로 질문.',
    inputSchema: {
        type: 'object' as const,
        properties: {
            schemaName: {
                type: 'string',
                description: '스키마 파일명 (확장자 없이)',
            },
            outputDir: {
                type: 'string',
                description: '출력 서브디렉토리. 기본값: default',
                default: 'default',
            },
            schema: {
                type: 'object',
                description: 'AI가 생성한 JSON Schema 객체',
            },
            description: {
                type: 'string',
                description: '스키마 설명 (선택)',
            },
            questions: {
                type: 'array',
                description: '불확실한 항목에 대한 질문 목록',
                items: {
                    type: 'object',
                    properties: {
                        field: { type: 'string' },
                        question: { type: 'string' },
                        context: { type: 'string' },
                        suggestion: {},
                    },
                    required: ['field', 'question'],
                },
            },
            skipValidation: {
                type: 'boolean',
                description: '검증 건너뛰기 (디버깅용)',
                default: false,
            },
        },
        required: ['schemaName', 'schema'],
    },
};
