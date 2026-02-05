#!/usr/bin/env node
/**
 * MCP API Schema Server (Enhanced)
 * AI 생성 스키마 저장 MCP 서버 - Prompts, Resources 지원
 */

import { Server } from '@modelcontextprotocol/sdk/server/index.js';
import { StdioServerTransport } from '@modelcontextprotocol/sdk/server/stdio.js';
import {
    CallToolRequestSchema,
    ListToolsRequestSchema,
    ListPromptsRequestSchema,
    GetPromptRequestSchema,
    ListResourcesRequestSchema,
    ReadResourceRequestSchema,
} from '@modelcontextprotocol/sdk/types.js';
import { saveSchema, saveSchemaTool, SaveSchemaInput } from './tools/save.js';
import { allPrompts, getSchemaRulesMessages } from './prompts/schema-rules.js';
import { allResources, readResource } from './resources/rules.js';

const server = new Server(
    {
        name: 'mcp-api-schema-server',
        version: '2.0.0',  // 버전 업
    },
    {
        capabilities: {
            tools: {},
            prompts: { listChanged: false },
            resources: { listChanged: false },
        },
    }
);

// ============================================================================
// TOOLS - 도구 처리
// ============================================================================

// 도구 목록 반환
server.setRequestHandler(ListToolsRequestSchema, async () => {
    return {
        tools: [saveSchemaTool],
    };
});

// 도구 실행
server.setRequestHandler(CallToolRequestSchema, async (request) => {
    const { name, arguments: args } = request.params;

    if (name === 'save_schema') {
        const input = args as unknown as SaveSchemaInput;
        const result = await saveSchema(input);

        return {
            content: [
                {
                    type: 'text',
                    text: JSON.stringify(result, null, 2),
                },
            ],
        };
    }

    throw new Error(`Unknown tool: ${name}`);
});

// ============================================================================
// PROMPTS - 프롬프트 처리
// ============================================================================

// 프롬프트 목록 반환
server.setRequestHandler(ListPromptsRequestSchema, async () => {
    return {
        prompts: allPrompts,
    };
});

// 프롬프트 내용 반환
server.setRequestHandler(GetPromptRequestSchema, async (request) => {
    const { name, arguments: args } = request.params;

    if (name === 'schema_rules') {
        const section = args?.['section'] as string | undefined;
        const messages = getSchemaRulesMessages(section);

        return {
            description: '스키마 생성 규칙',
            messages,
        };
    }

    throw new Error(`Unknown prompt: ${name}`);
});

// ============================================================================
// RESOURCES - 리소스 처리
// ============================================================================

// 리소스 목록 반환
server.setRequestHandler(ListResourcesRequestSchema, async () => {
    return {
        resources: allResources,
    };
});

// 리소스 내용 반환
server.setRequestHandler(ReadResourceRequestSchema, async (request) => {
    const { uri } = request.params;

    const content = readResource(uri);
    if (!content) {
        throw new Error(`Unknown resource: ${uri}`);
    }

    return {
        contents: [content],
    };
});

// ============================================================================
// SERVER START
// ============================================================================

async function main() {
    const transport = new StdioServerTransport();
    await server.connect(transport);
    console.error('MCP API Schema Server v2.0 running on stdio');
    console.error('  - Tools: save_schema');
    console.error('  - Prompts: schema_rules');
    console.error('  - Resources: shared.yaml, mcp.yaml, ui.yaml, accuracyPolicy.yaml');
}

main().catch((error) => {
    console.error('Fatal error:', error);
    process.exit(1);
});
