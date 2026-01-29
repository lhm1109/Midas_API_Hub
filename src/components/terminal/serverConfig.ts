/**
 * Server configuration for terminal management
 */

export interface ServerConfig {
    id: string;
    name: string;
    command: string;
    cwd: string;
    icon: string;
    description?: string;
    webviewUrl?: string; // 웹뷰로 표시할 URL (있으면 터미널 대신 iframe 표시)
    port?: number; // 서버 포트 (충돌 방지를 위해 사용)
}

export const PREDEFINED_SERVERS: ServerConfig[] = [
    {
        id: 'express',
        name: 'Express API Server',
        command: 'npm run server',
        cwd: 'c:/MIDAS/code/apiverification',
        icon: '🚀',
        description: 'Backend API server (port 3001)',
        port: 3001,
    },
    {
        id: 'vite',
        name: 'Vite Dev Server',
        command: 'npm run dev',
        cwd: 'c:/MIDAS/code/apiverification',
        icon: '⚡',
        description: 'Frontend dev server (port 5178)',
        port: 5178,
    },
    {
        id: 'mcp',
        name: 'MCP Inspector',
        command: 'npx @modelcontextprotocol/inspector node dist/index.js',
        cwd: 'c:/MIDAS/code/apiverification/mcp-server',
        icon: '🔌',
        description: 'MCP Schema Server inspector',
        webviewUrl: 'http://localhost:6274',
        port: 6274,
    },
    {
        id: 'design-registry',
        name: 'Design Meta Registry',
        command: 'npm run dev',
        cwd: 'c:/MIDAS/code/apiverification/design-meta-registry',
        icon: '📦',
        description: 'Design metadata registry',
        port: 5173,
    },
];
