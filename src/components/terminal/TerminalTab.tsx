/**
 * TerminalTab - 통합 터미널 관리 탭
 * 왼쪽: 서버 목록 + 터미널
 * 오른쪽: MCP Inspector 웹뷰 (실행 중일 때)
 */

import React, { useState, useEffect, useCallback, useRef } from 'react';
import { Terminal as TerminalIcon, Globe, RefreshCw, GripHorizontal, GripVertical, ExternalLink, PlayCircle, StopCircle } from 'lucide-react';
import { Button } from '@/components/ui/button';
import { ServerCard } from './ServerCard';
import { TerminalEmulator } from './TerminalEmulator';
import { PREDEFINED_SERVERS, ServerConfig } from './serverConfig';
import { terminalAPI, isElectron, TerminalInfo } from './types';

interface TerminalSession {
    id: number;
    serverId: string;
    name: string;
}

export const TerminalTab: React.FC = () => {
    const [sessions, setSessions] = useState<TerminalSession[]>([]);
    const [selectedServerId, setSelectedServerId] = useState<string | null>(null);
    const [notElectron, setNotElectron] = useState(false);
    const [webviewKey, setWebviewKey] = useState(0);
    const [serverListHeight, setServerListHeight] = useState(280);
    const [leftPanelWidth, setLeftPanelWidth] = useState(380);
    const initialized = useRef(false);
    const isDraggingH = useRef(false);
    const isDraggingV = useRef(false);

    // MCP Inspector가 실행 중인지 확인
    const mcpServer = PREDEFINED_SERVERS.find(s => s.id === 'mcp');
    const isMcpRunning = mcpServer && sessions.some(s => s.serverId === 'mcp');

    // 수평 드래그 리사이즈 (서버 목록 높이)
    const handleMouseDownH = useCallback((e: React.MouseEvent) => {
        e.preventDefault();
        isDraggingH.current = true;
        const startY = e.clientY;
        const startHeight = serverListHeight;

        const handleMouseMove = (moveEvent: MouseEvent) => {
            if (!isDraggingH.current) return;
            const delta = moveEvent.clientY - startY;
            const newHeight = Math.max(80, Math.min(300, startHeight + delta));
            setServerListHeight(newHeight);
        };

        const handleMouseUp = () => {
            isDraggingH.current = false;
            document.removeEventListener('mousemove', handleMouseMove);
            document.removeEventListener('mouseup', handleMouseUp);
        };

        document.addEventListener('mousemove', handleMouseMove);
        document.addEventListener('mouseup', handleMouseUp);
    }, [serverListHeight]);

    // 수직 드래그 리사이즈 (왼쪽 패널 너비)
    const handleMouseDownV = useCallback((e: React.MouseEvent) => {
        e.preventDefault();
        isDraggingV.current = true;
        const startX = e.clientX;
        const startWidth = leftPanelWidth;

        const handleMouseMove = (moveEvent: MouseEvent) => {
            if (!isDraggingV.current) return;
            const delta = moveEvent.clientX - startX;
            const newWidth = Math.max(280, Math.min(600, startWidth + delta));
            setLeftPanelWidth(newWidth);
        };

        const handleMouseUp = () => {
            isDraggingV.current = false;
            document.removeEventListener('mousemove', handleMouseMove);
            document.removeEventListener('mouseup', handleMouseUp);
        };

        document.addEventListener('mousemove', handleMouseMove);
        document.addEventListener('mouseup', handleMouseUp);
    }, [leftPanelWidth]);

    // 컴포넌트 마운트 시 main process에서 세션 상태 동기화
    useEffect(() => {
        if (!isElectron()) {
            setNotElectron(true);
            return;
        }

        const api = terminalAPI();
        if (!api) return;

        if (initialized.current) return;
        initialized.current = true;

        const syncSessions = async () => {
            const result = await api.syncState();
            if (result.success && result.data) {
                const syncedSessions: TerminalSession[] = result.data
                    .filter((t: TerminalInfo) => t.serverId)
                    .map((t: TerminalInfo) => ({
                        id: t.id,
                        serverId: t.serverId!,
                        name: t.name,
                    }));
                setSessions(syncedSessions);

                if (syncedSessions.length > 0 && !selectedServerId) {
                    setSelectedServerId(syncedSessions[0].serverId);
                }
            }
        };
        syncSessions();

        api.onExit((payload) => {
            setSessions((prev) => prev.filter((s) => s.id !== payload.id));
        });

        return () => {
            const api = terminalAPI();
            if (api) {
                api.removeListeners();
            }
        };
    }, [selectedServerId]);

    const startServer = useCallback(async (server: ServerConfig) => {
        const api = terminalAPI();
        if (!api) return;

        const existing = sessions.find((s) => s.serverId === server.id);
        if (existing) {
            setSelectedServerId(server.id);
            return;
        }

        if (server.port) {
            console.log(`Killing existing process on port ${server.port}...`);
            await api.killPort(server.port);
            await new Promise((resolve) => setTimeout(resolve, 500));
        }

        const result = await api.create({
            name: server.name,
            cwd: server.cwd,
        });

        if (result.success && result.id) {
            await api.setServerId(result.id, server.id);
            await api.write(result.id, server.command + '\r');

            const newSession: TerminalSession = {
                id: result.id,
                serverId: server.id,
                name: server.name,
            };

            setSessions((prev) => [...prev, newSession]);
            setSelectedServerId(server.id);
        }
    }, [sessions]);

    const stopServer = useCallback(async (serverId: string) => {
        const api = terminalAPI();
        if (!api) return;

        const session = sessions.find((s) => s.serverId === serverId);
        if (session) {
            await api.kill(session.id);
            setSessions((prev) => prev.filter((s) => s.id !== session.id));
            if (selectedServerId === serverId) {
                setSelectedServerId(null);
            }
        }
    }, [sessions, selectedServerId]);

    const isServerRunning = (serverId: string) => {
        return sessions.some((s) => s.serverId === serverId);
    };

    const getTerminalId = (serverId: string) => {
        const session = sessions.find((s) => s.serverId === serverId);
        return session?.id ?? null;
    };

    const handleRefreshWebview = () => {
        setWebviewKey((prev) => prev + 1);
    };

    const handleOpenExternal = () => {
        if (mcpServer?.webviewUrl) {
            window.open(mcpServer.webviewUrl, '_blank');
        }
    };

    if (notElectron) {
        return (
            <div className="flex-1 flex items-center justify-center p-8 text-center">
                <div>
                    <TerminalIcon className="h-16 w-16 text-zinc-600 mb-4 mx-auto" />
                    <h2 className="text-xl font-semibold text-zinc-400 mb-2">
                        Terminal not available
                    </h2>
                    <p className="text-sm text-zinc-500 max-w-md">
                        Terminal features are only available in the Electron desktop app.
                        <br />
                        Run <code className="bg-zinc-800 px-1 rounded">npm run electron:dev</code> to use terminals.
                    </p>
                </div>
            </div>
        );
    }

    return (
        <div className="flex-1 flex h-full overflow-hidden">
            {/* 왼쪽 패널: 서버 목록 + 터미널 */}
            <div
                className="flex flex-col bg-zinc-900 border-r border-zinc-800"
                style={{ width: leftPanelWidth }}
            >
                {/* Header */}
                <div className="flex items-center justify-between px-4 py-3 border-b border-zinc-800 shrink-0">
                    <div className="flex items-center gap-2">
                        <TerminalIcon className="h-5 w-5 text-cyan-400" />
                        <h2 className="font-semibold text-zinc-100">Servers</h2>
                        <span className="px-2 py-0.5 text-xs bg-zinc-800 text-zinc-400 rounded-full">
                            {sessions.length} running
                        </span>
                    </div>
                    <div className="flex items-center gap-1">
                        <Button
                            variant="ghost"
                            size="sm"
                            className="h-7 px-2 text-xs text-green-400 hover:text-green-300 hover:bg-green-900/30"
                            onClick={() => {
                                PREDEFINED_SERVERS.forEach(server => {
                                    if (!isServerRunning(server.id)) {
                                        startServer(server);
                                    }
                                });
                            }}
                            title="Start All Servers"
                        >
                            <PlayCircle className="h-4 w-4 mr-1" />
                            All
                        </Button>
                        <Button
                            variant="ghost"
                            size="sm"
                            className="h-7 px-2 text-xs text-red-400 hover:text-red-300 hover:bg-red-900/30"
                            onClick={() => {
                                PREDEFINED_SERVERS.forEach(server => {
                                    if (isServerRunning(server.id)) {
                                        stopServer(server.id);
                                    }
                                });
                            }}
                            title="Stop All Servers"
                        >
                            <StopCircle className="h-4 w-4 mr-1" />
                            All
                        </Button>
                    </div>
                </div>

                {/* Server List */}
                <div
                    className="overflow-y-auto shrink-0"
                    style={{ height: serverListHeight }}
                >
                    <div className="p-3 space-y-2">
                        {PREDEFINED_SERVERS.map((server) => (
                            <ServerCard
                                key={server.id}
                                server={server}
                                isRunning={isServerRunning(server.id)}
                                onStart={() => startServer(server)}
                                onStop={() => stopServer(server.id)}
                                onSelect={() => setSelectedServerId(server.id)}
                                isSelected={selectedServerId === server.id}
                            />
                        ))}
                    </div>
                </div>

                {/* Resize Handle H */}
                <div
                    className="h-2 bg-zinc-800 hover:bg-zinc-600 cursor-ns-resize flex items-center justify-center shrink-0 transition-colors"
                    onMouseDown={handleMouseDownH}
                >
                    <GripHorizontal className="h-3 w-3 text-zinc-500" />
                </div>

                {/* Terminal View */}
                <div className="flex-1 min-h-0 overflow-hidden">
                    {selectedServerId && getTerminalId(selectedServerId) ? (
                        <TerminalEmulator
                            key={getTerminalId(selectedServerId)!}
                            terminalId={getTerminalId(selectedServerId)!}
                            className="h-full"
                        />
                    ) : (
                        <div className="flex items-center justify-center h-full text-zinc-500">
                            <div className="text-center">
                                <TerminalIcon className="h-12 w-12 mx-auto mb-2 opacity-30" />
                                <p className="text-sm">Select a server to view console</p>
                            </div>
                        </div>
                    )}
                </div>
            </div>

            {/* Resize Handle V */}
            <div
                className="w-2 bg-zinc-800 hover:bg-zinc-600 cursor-ew-resize flex items-center justify-center shrink-0 transition-colors"
                onMouseDown={handleMouseDownV}
            >
                <GripVertical className="h-4 w-4 text-zinc-500" />
            </div>

            {/* 오른쪽 영역: MCP Inspector 웹뷰 */}
            <div className="flex-1 flex flex-col bg-zinc-950 overflow-hidden">
                {isMcpRunning && mcpServer?.webviewUrl ? (
                    <>
                        {/* Header */}
                        <div className="flex items-center justify-between px-4 py-3 border-b border-zinc-800 shrink-0">
                            <div className="flex items-center gap-2">
                                <Globe className="h-5 w-5 text-purple-400" />
                                <h2 className="font-semibold text-zinc-100">MCP Inspector</h2>
                                <span className="px-2 py-0.5 text-xs bg-purple-900/30 text-purple-400 rounded-full">
                                    {mcpServer.webviewUrl}
                                </span>
                            </div>
                            <div className="flex items-center gap-1">
                                <Button
                                    variant="ghost"
                                    size="icon"
                                    className="h-8 w-8 text-zinc-400 hover:text-zinc-100"
                                    onClick={handleRefreshWebview}
                                    title="Refresh"
                                >
                                    <RefreshCw className="h-4 w-4" />
                                </Button>
                                <Button
                                    variant="ghost"
                                    size="icon"
                                    className="h-8 w-8 text-zinc-400 hover:text-zinc-100"
                                    onClick={handleOpenExternal}
                                    title="Open in browser"
                                >
                                    <ExternalLink className="h-4 w-4" />
                                </Button>
                            </div>
                        </div>
                        {/* Iframe */}
                        <iframe
                            key={webviewKey}
                            src={mcpServer.webviewUrl}
                            className="flex-1 w-full border-0"
                            title="MCP Inspector"
                        />
                    </>
                ) : (
                    <div className="flex items-center justify-center h-full text-zinc-500">
                        <div className="text-center">
                            <Globe className="h-16 w-16 mx-auto mb-4 opacity-30" />
                            <h3 className="text-lg font-semibold text-zinc-400 mb-2">MCP Inspector</h3>
                            <p className="text-sm text-zinc-500 max-w-md">
                                Start the MCP Inspector server to view it here.
                            </p>
                        </div>
                    </div>
                )}
            </div>
        </div>
    );
};

export default TerminalTab;
