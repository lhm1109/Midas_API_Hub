#!/usr/bin/env node
/**
 * MCP API Schema Server
 * AI 생성 스키마 저장 MCP 서버
 */

import { Server } from '@modelcontextprotocol/sdk/server/index.js';
import { StdioServerTransport } from '@modelcontextprotocol/sdk/server/stdio.js';
import {
    CallToolRequestSchema,
    ListToolsRequestSchema,
} from '@modelcontextprotocol/sdk/types.js';
import { saveSchema, saveSchemaTool, SaveSchemaInput } from './tools/save.js';

const server = new Server(
    {
        name: 'mcp-api-schema-server',
        version: '1.0.0',
    },
    {
        capabilities: {
            tools: {},
        },
    }
);

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

// 서버 시작
async function main() {
    const transport = new StdioServerTransport();
    await server.connect(transport);
    console.error('MCP API Schema Server running on stdio');
}

main().catch((error) => {
    console.error('Fatal error:', error);
    process.exit(1);
});
