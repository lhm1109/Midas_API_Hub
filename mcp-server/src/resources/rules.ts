/**
 * MCP Resources - YAML 규칙 파일을 Resource로 노출
 * AI가 read_resource 호출로 규칙을 직접 읽을 수 있음
 */

import * as fs from 'fs';
import * as path from 'path';
import { fileURLToPath } from 'url';

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

export interface MCPResource {
    uri: string;
    name: string;
    description: string;
    mimeType: string;
}

export interface ResourceContent {
    uri: string;
    mimeType: string;
    text: string;
}

// 규칙 파일 경로들
const RULES_BASE_PATH = path.resolve(__dirname, '../../../schema_definitions/civil_gen_definition/enhanced');

/**
 * 사용 가능한 리소스 목록
 */
export const allResources: MCPResource[] = [
    {
        uri: 'mcp://rules/shared.yaml',
        name: 'Shared Rules (SSOT)',
        description: '공통 규칙 - 마커 레지스트리, 테이블 스키마 레지스트리, 조건 레지스트리, integrityRules',
        mimeType: 'application/yaml',
    },
    {
        uri: 'mcp://rules/mcp.yaml',
        name: 'MCP Rules',
        description: 'MCP 서버 전용 규칙 - 보안, IO, 결정론, 검증 파이프라인',
        mimeType: 'application/yaml',
    },
    {
        uri: 'mcp://rules/ui.yaml',
        name: 'UI Rules',
        description: 'UI 렌더링 규칙 - 필드 타입, 레이아웃, 상태 표시',
        mimeType: 'application/yaml',
    },
    {
        uri: 'mcp://rules/accuracyPolicy.yaml',
        name: 'Accuracy Policy',
        description: 'Evidence-based 생성 규칙 - 근거 요구사항, 자동 질문 패턴, 품질 관문',
        mimeType: 'application/yaml',
    },
    {
        uri: 'mcp://rules/promptRules.yaml',
        name: 'Prompt Rules (LLM Guidelines)',
        description: 'LLM 스키마 생성 가이드라인 - 필드 네이밍, 패턴, critical warnings, questions policy',
        mimeType: 'application/yaml',
    },
];

/**
 * 리소스 내용 읽기
 */
export function readResource(uri: string): ResourceContent | null {
    const resource = allResources.find((r) => r.uri === uri);
    if (!resource) {
        return null;
    }

    // URI에서 파일명 추출
    const filename = uri.replace('mcp://rules/', '');
    const filePath = path.join(RULES_BASE_PATH, filename);

    try {
        const text = fs.readFileSync(filePath, 'utf-8');
        return {
            uri: resource.uri,
            mimeType: resource.mimeType,
            text,
        };
    } catch (error) {
        console.error(`Failed to read resource: ${uri}`, error);
        return null;
    }
}
