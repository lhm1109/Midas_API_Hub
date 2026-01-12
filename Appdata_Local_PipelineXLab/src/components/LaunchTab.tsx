import { useState, useEffect, useRef } from 'react';
import {
    Rocket, Square, RefreshCw, ExternalLink, Terminal,
    ChevronLeft, ChevronRight, GripVertical,
    Play, Pause, Trash2, Copy, ChevronDown, Settings,
    Globe, Server, Database, Loader2, CheckCircle, XCircle, AlertTriangle
} from 'lucide-react';
// ... (interfaces and constants remain same) - replaced with actual code

interface LogEntry {
    id: number;
    type: 'info' | 'success' | 'error' | 'warning' | 'debug';
    source: 'frontend' | 'backend' | 'system' | 'database';
    message: string;
    timestamp: string;
}

interface ServiceStatus {
    id: string;
    name: string;
    icon: React.ElementType;
    port: number;
    status: 'stopped' | 'starting' | 'running' | 'error' | 'crashed';
    url?: string;
    pid?: number;
}

const initialServices: ServiceStatus[] = [
    { id: 'frontend', name: 'Frontend', icon: Globe, port: 5173, status: 'stopped', url: 'http://localhost:5173' },
    { id: 'backend', name: 'Backend API', icon: Server, port: 8000, status: 'stopped', url: 'http://localhost:8000' },
    { id: 'database', name: 'Database', icon: Database, port: 5432, status: 'stopped' },
];

const serviceIcons: Record<string, React.ElementType> = {
    frontend: Globe,
    backend: Server,
    database: Database,
    proxy: Server,
};
export function LaunchTab() {
    const [services, setServices] = useState<ServiceStatus[]>(initialServices);
    const [logs, setLogs] = useState<LogEntry[]>([]);
    const [activeLogFilter, setActiveLogFilter] = useState<string>('all');
    const [autoScroll, setAutoScroll] = useState(true);
    const [showDebug, setShowDebug] = useState(false);
    const logsEndRef = useRef<HTMLDivElement>(null);
    const serviceScrollRef = useRef<HTMLDivElement>(null);
    const logIdRef = useRef(0);

    const scrollServices = (direction: 'left' | 'right') => {
        if (serviceScrollRef.current) {
            const scrollAmount = 300; // Approx card width + gap
            serviceScrollRef.current.scrollBy({
                left: direction === 'left' ? -scrollAmount : scrollAmount,
                behavior: 'smooth'
            });
        }
    };


    const isAnyRunning = services.some(s => s.status === 'running' || s.status === 'starting');
    const allRunning = services.every(s => s.status === 'running');

    // ✅ Electron API 사용 여부 확인
    const isElectron = typeof window !== 'undefined' && (window as any).electronAPI;

    // ✅ Electron 모드: 실제 서비스 관리
    useEffect(() => {
        if (!isElectron) {
            return;
        }

        console.log('🔌 Electron Mode Detected. Loading services...');
        loadServices();

        const cleanupStatus = (window as any).electronAPI.onServiceStatus((data: any) => {
            console.log('📡 Service Status:', data);
            setServices(prev => {
                const index = prev.findIndex(s => s.id === data.id);
                if (index >= 0) {
                    const updated = [...prev];
                    updated[index] = {
                        ...updated[index],
                        status: data.status,
                        pid: data.pid,
                    };
                    return updated;
                }
                return prev;
            });
        });

        const cleanupLog = (window as any).electronAPI.onServiceLog((data: any) => {
            console.log('📝 Service Log:', data);
            addLog(data.type, data.source, data.message);
        });

        return () => {
            cleanupStatus();
            cleanupLog();
        };
    }, [isElectron]);

    useEffect(() => {
        if (autoScroll && logsEndRef.current) {
            logsEndRef.current.scrollIntoView({ behavior: 'smooth' });
        }
    }, [logs, autoScroll]);

    // ✅ Frontend 실행 시 자동으로 화면 전환 (Debug -> App)
    useEffect(() => {
        const frontend = services.find(s => s.id === 'frontend');
        if (frontend?.status === 'running') {
            setShowDebug(false);
        }
    }, [services]);

    const loadServices = async () => {
        if (!isElectron) return;

        try {
            const serviceList = await (window as any).electronAPI.getAllServices();
            setServices(serviceList.map((s: any) => ({
                ...s,
                icon: serviceIcons[s.id] || Server,
                url: s.id === 'frontend' ? 'http://localhost:5173' :
                    s.id === 'backend' ? 'http://localhost:8001' :
                        s.id === 'proxy' ? 'http://localhost:8000' : undefined,
            })));
        } catch (error) {
            console.error('Failed to load services:', error);
        }
    };

    const addLog = (type: LogEntry['type'], source: LogEntry['source'], message: string) => {
        const newLog: LogEntry = {
            id: logIdRef.current++,
            type,
            source,
            message,
            timestamp: new Date().toLocaleTimeString('ko-KR', { hour12: false }),
        };
        setLogs(prev => [...prev, newLog]);
    };

    // 브라우저 모드 시뮬레이션 함수
    const simulateStartup = () => {
        // 시스템 시작 로그
        addLog('info', 'system', '🎭 시뮬레이션 모드 (실제 서비스는 시작되지 않습니다)');
        addLog('info', 'system', '💡 실제 서비스 관리는 Electron 앱에서만 가능합니다');
        addLog('info', 'system', '$ Starting PipelineXLab services...');

        // Frontend 시작
        setServices(prev => prev.map(s => s.id === 'frontend' ? { ...s, status: 'starting' } : s));
        addLog('info', 'frontend', 'Starting development server...');

        setTimeout(() => {
            addLog('info', 'frontend', 'Compiling TypeScript...');
        }, 300);

        setTimeout(() => {
            addLog('success', 'frontend', 'VITE v5.4.21  ready in 219 ms');
            addLog('info', 'frontend', '➜  Local:   http://localhost:5173/');
            setServices(prev => prev.map(s => s.id === 'frontend' ? { ...s, status: 'running' } : s));
        }, 800);

        // Backend 시작
        setTimeout(() => {
            setServices(prev => prev.map(s => s.id === 'backend' ? { ...s, status: 'starting' } : s));
            addLog('info', 'backend', 'Starting FastAPI server...');
        }, 200);

        setTimeout(() => {
            addLog('info', 'backend', 'Loading component registry...');
        }, 500);

        setTimeout(() => {
            addLog('debug', 'backend', 'Registered 24 components');
            addLog('debug', 'backend', 'WebSocket server initialized');
        }, 700);

        setTimeout(() => {
            addLog('success', 'backend', 'Uvicorn running on http://0.0.0.0:8001');
            addLog('info', 'backend', 'API docs: http://localhost:8001/docs');
            setServices(prev => prev.map(s => s.id === 'backend' ? { ...s, status: 'running' } : s));
        }, 1000);

        // Database 연결
        setTimeout(() => {
            setServices(prev => prev.map(s => s.id === 'database' ? { ...s, status: 'starting' } : s));
            addLog('info', 'database', 'Connecting to PostgreSQL...');
        }, 400);

        setTimeout(() => {
            addLog('warning', 'database', 'Connection retry (1/3)...');
        }, 600);

        setTimeout(() => {
            addLog('success', 'database', 'Connected to database: pipelinexlab');
            addLog('debug', 'database', 'Connection pool: 5/20');
            setServices(prev => prev.map(s => s.id === 'database' ? { ...s, status: 'running' } : s));
        }, 900);

        // 완료
        setTimeout(() => {
            addLog('success', 'system', '✓ 시뮬레이션 완료 (실제로는 실행되지 않음)');
        }, 1200);
    };

    const handleLaunch = async () => {
        if (!isElectron) {
            // 브라우저 모드: 시뮬레이션
            simulateStartup();
            return;
        }

        // Electron 모드: 실제 실행
        try {
            const result = await (window as any).electronAPI.launchAll();
            if (!result.success) {
                addLog('error', 'system', result.error || 'Failed to launch services');
            }
        } catch (error: any) {
            addLog('error', 'system', `Launch failed: ${error.message}`);
        }
    };

    const handleStop = async () => {
        if (!isElectron) {
            // 브라우저 모드: 시뮬레이션
            addLog('info', 'system', '$ Stopping all services...');
            setServices(prev => prev.map(s => ({ ...s, status: 'stopped' })));
            addLog('info', 'frontend', 'Server stopped');
            addLog('info', 'backend', 'Shutting down...');
            addLog('info', 'database', 'Connection closed');
            addLog('success', 'system', '✓ All services stopped (simulation)');
            return;
        }

        try {
            await (window as any).electronAPI.stopAll();
        } catch (error: any) {
            addLog('error', 'system', `Stop failed: ${error.message}`);
        }
    };

    const handleStartService = async (id: string, name: string) => {
        if (!isElectron) {
            addLog('info', 'system', `Starting ${name} (simulation)...`);
            setServices(prev => prev.map(s => s.id === id ? { ...s, status: 'starting' } : s));
            setTimeout(() => {
                setServices(prev => prev.map(s => s.id === id ? { ...s, status: 'running' } : s));
                addLog('success', 'system', `${name} started (simulation)`);
            }, 1000);
            return;
        }

        try {
            addLog('info', 'system', `Starting ${name}...`);
            await (window as any).electronAPI.startService(id);
        } catch (error: any) {
            addLog('error', 'system', `Failed to start ${name}: ${error.message}`);
        }
    };

    const handleStopService = async (id: string, name: string) => {
        if (!isElectron) {
            addLog('info', 'system', `Stopping ${name} (simulation)...`);
            setServices(prev => prev.map(s => s.id === id ? { ...s, status: 'stopped' } : s));
            addLog('success', 'system', `${name} stopped (simulation)`);
            return;
        }

        try {
            addLog('info', 'system', `Stopping ${name}...`);
            await (window as any).electronAPI.stopService(id);
        } catch (error: any) {
            addLog('error', 'system', `Failed to stop ${name}: ${error.message}`);
        }
    };

    const handleRestartService = async (id: string, name: string) => {
        if (!isElectron) {
            addLog('info', 'system', `Restarting ${name} (simulation)...`);
            setServices(prev => prev.map(s => s.id === id ? { ...s, status: 'starting' } : s));
            setTimeout(() => {
                setServices(prev => prev.map(s => s.id === id ? { ...s, status: 'running' } : s));
                addLog('success', 'system', `${name} restarted (simulation)`);
            }, 1500);
            return;
        }

        try {
            addLog('info', 'system', `Restarting ${name}...`);
            // ✅ Use dedicated restartService API that properly waits for port release
            const result = await (window as any).electronAPI.restartService(id);
            if (!result.success) {
                addLog('error', 'system', `Failed to restart ${name}: ${result.error || 'Unknown error'}`);
            }
        } catch (error: any) {
            addLog('error', 'system', `Failed to restart ${name}: ${error.message}`);
        }
    };

    const handleOpenExternal = (url: string) => {
        if (isElectron) {
            (window as any).electronAPI.openExternal(url);
        } else {
            window.open(url, '_blank');
        }
    };

    const clearLogs = () => {
        setLogs([]);
        logIdRef.current = 0;
    };

    const copyLogs = () => {
        const text = filteredLogs.map(l => `[${l.timestamp}] [${l.source.toUpperCase()}] ${l.message}`).join('\n');
        navigator.clipboard.writeText(text);
    };

    const getLogColor = (type: LogEntry['type']) => {
        switch (type) {
            case 'success': return 'text-green-400';
            case 'error': return 'text-red-400';
            case 'warning': return 'text-yellow-400';
            case 'debug': return 'text-purple-400';
            default: return 'text-blue-400';
        }
    };

    const getSourceColor = (source: LogEntry['source']) => {
        switch (source) {
            case 'frontend': return 'text-cyan-500';
            case 'backend': return 'text-orange-500';
            case 'database': return 'text-pink-500';
            default: return 'text-slate-500';
        }
    };

    const getStatusIcon = (status: ServiceStatus['status']) => {
        switch (status) {
            case 'running': return <CheckCircle className="w-3 h-3 text-green-400" />;
            case 'starting': return <Loader2 className="w-3 h-3 text-yellow-400 animate-spin" />;
            case 'crashed': return <AlertTriangle className="w-3 h-3 text-orange-400" />;
            case 'error': return <XCircle className="w-3 h-3 text-red-400" />;
            default: return <div className="w-3 h-3 rounded-full bg-slate-600" />;
        }
    };

    const logFilters = [
        { id: 'all', label: 'All' },
        { id: 'frontend', label: 'Frontend' },
        { id: 'backend', label: 'Backend' },
        { id: 'database', label: 'Database' },
        { id: 'system', label: 'System' },
    ];

    const filteredLogs = activeLogFilter === 'all'
        ? logs
        : logs.filter(log => log.source === activeLogFilter);

    return (
        <div className="flex flex-col h-full bg-background">
            {/* Header Toolbar */}
            <div className="px-4 py-2 bg-surface-1 border-b border-surface-2 flex items-center justify-between shrink-0">
                <div className="flex items-center gap-4">
                    <div className="flex items-center gap-2">
                        <Rocket className="w-4 h-4 text-blue-500" />
                        <span className="text-xs text-foreground font-medium">
                            Launch Console {!isElectron && '(시뮬레이션 모드)'}
                        </span>
                    </div>
                </div>

                <div className="flex items-center gap-2">
                    {/* Control Buttons Moved to Header */}
                    {!isAnyRunning ? (
                        <button
                            onClick={handleLaunch}
                            className="flex items-center gap-1.5 px-3 py-1.5 bg-blue-600 hover:bg-blue-700 text-white text-xs rounded transition-colors shadow-sm"
                        >
                            <Play className="w-3.5 h-3.5 fill-current" />
                            Launch All
                        </button>
                    ) : (
                        <button
                            onClick={handleStop}
                            className="flex items-center gap-1.5 px-3 py-1.5 bg-red-600 hover:bg-red-700 text-white text-xs rounded transition-colors shadow-sm"
                        >
                            <Square className="w-3.5 h-3.5 fill-current" />
                            Stop All
                        </button>
                    )}
                    <button
                        onClick={isElectron ? loadServices : () => window.location.reload()}
                        className="flex items-center gap-1.5 px-3 py-1.5 bg-surface-2 hover:bg-surface-3 text-foreground text-xs rounded border border-surface-3 transition-colors"
                    >
                        <RefreshCw className="w-3.5 h-3.5" />
                        {isElectron ? 'Refresh' : 'Restart'}
                    </button>

                    <div className="w-px h-4 bg-surface-3 mx-1" />

                    {!isElectron && (
                        <span className="px-2 py-1 text-[10px] bg-yellow-600/20 text-yellow-400 rounded">
                            브라우저 모드
                        </span>
                    )}
                    <button
                        onClick={() => setShowDebug(!showDebug)}
                        className={`px-3 py-1.5 text-xs rounded border transition-colors ${showDebug
                            ? 'bg-blue-500/10 text-blue-500 border-blue-500/20'
                            : 'bg-surface-2 text-muted-foreground border-surface-3 hover:bg-surface-3'
                            }`}
                    >
                        Debug
                    </button>
                    <button className="p-1.5 text-muted-foreground hover:text-foreground hover:bg-accent rounded transition-colors">
                        <Settings className="w-4 h-4" />
                    </button>
                </div>
            </div>

            {/* Main Content */}
            <div className="flex-1 flex flex-col overflow-hidden p-4 gap-4">
                {/* Services Carousel */}
                {/* Services Banner Slider */}
                <div className="relative group shrink-0">
                    <button
                        onClick={() => scrollServices('left')}
                        className="absolute left-0 top-1/2 -translate-y-1/2 -ml-2 z-10 p-1.5 rounded-full bg-background border shadow-md text-muted-foreground hover:text-foreground opacity-0 group-hover:opacity-100 transition-opacity disabled:opacity-0"
                    >
                        <ChevronLeft className="w-4 h-4" />
                    </button>

                    <div
                        ref={serviceScrollRef}
                        className="flex gap-4 overflow-x-hidden scroll-smooth pb-1"
                        style={{ scrollbarWidth: 'none', msOverflowStyle: 'none' }}
                    >
                        {services.map((service) => {
                            const Icon = service.icon;
                            return (
                                <div
                                    key={service.id}
                                    draggable
                                    className={`flex-shrink-0 w-64 p-3 rounded-xl border transition-all cursor-grab active:cursor-grabbing
                                        backdrop-blur-sm shadow-lg hover:shadow-xl hover:scale-[1.02] transform
                                        ${service.status === 'running'
                                            ? 'bg-gradient-to-br from-green-500/10 to-green-600/5 border-green-500/40 shadow-green-500/10'
                                            : service.status === 'starting'
                                                ? 'bg-gradient-to-br from-yellow-500/10 to-orange-500/5 border-yellow-500/40 shadow-yellow-500/10'
                                                : 'bg-gradient-to-br from-slate-800/80 to-slate-900/80 border-slate-700/50 hover:border-slate-600/60'
                                        }`}
                                >
                                    {/* Drag Handle + Header */}
                                    <div className="flex items-center justify-between mb-3">
                                        <div className="flex items-center gap-2">
                                            <GripVertical className="w-3 h-3 text-slate-500 opacity-50 hover:opacity-100 transition-opacity" />
                                            <div className={`p-2 rounded-lg ${service.status === 'running'
                                                ? 'bg-green-500/20 text-green-400 shadow-inner'
                                                : service.status === 'starting'
                                                    ? 'bg-yellow-500/20 text-yellow-400 shadow-inner'
                                                    : 'bg-slate-700/50 text-slate-400'
                                                }`}>
                                                <Icon className="w-4 h-4" />
                                            </div>
                                            <div>
                                                <div className="text-sm font-semibold text-foreground">{service.name}</div>
                                                <div className="text-[10px] text-muted-foreground font-mono">:{service.port}</div>
                                            </div>
                                        </div>
                                        {getStatusIcon(service.status)}
                                    </div>

                                    {/* Controls */}
                                    <div className="flex items-center justify-between pt-2 border-t border-white/5 mt-2">
                                        <div className="flex items-center gap-1.5">
                                            {/* Individual Service Control */}
                                            {service.status === 'running' ? (
                                                <>
                                                    <button
                                                        onClick={() => handleStopService(service.id, service.name)}
                                                        className="p-1.5 rounded-lg bg-red-500/15 text-red-400 hover:bg-red-500/25 transition-all hover:scale-105"
                                                        title={`Stop ${service.name}`}
                                                    >
                                                        <Square className="w-3 h-3 fill-current" />
                                                    </button>
                                                    <button
                                                        onClick={() => handleRestartService(service.id, service.name)}
                                                        className="p-1.5 rounded-lg bg-orange-500/15 text-orange-400 hover:bg-orange-500/25 transition-all hover:scale-105"
                                                        title={`Restart ${service.name}`}
                                                    >
                                                        <RefreshCw className="w-3 h-3" />
                                                    </button>
                                                </>
                                            ) : (
                                                <button
                                                    onClick={() => handleStartService(service.id, service.name)}
                                                    className={`p-1.5 rounded-lg transition-all hover:scale-105 ${service.status === 'starting'
                                                        ? 'bg-yellow-500/15 text-yellow-400 cursor-wait'
                                                        : 'bg-blue-500/15 text-blue-400 hover:bg-blue-500/25'
                                                        }`}
                                                    disabled={service.status === 'starting'}
                                                    title={`Start ${service.name}`}
                                                >
                                                    {service.status === 'starting' ? (
                                                        <Loader2 className="w-3 h-3 animate-spin" />
                                                    ) : (
                                                        <Play className="w-3 h-3 fill-current" />
                                                    )}
                                                </button>
                                            )}

                                            <div className="text-[9px] text-slate-500 font-mono ml-1">
                                                {service.pid ? `PID:${service.pid}` : '—'}
                                            </div>
                                        </div>

                                        {service.status === 'running' && service.url && (
                                            <button
                                                onClick={() => handleOpenExternal(service.url!)}
                                                className="text-[10px] bg-blue-500/15 text-blue-400 hover:bg-blue-500/25 px-2.5 py-1 rounded-lg flex items-center gap-1 transition-all hover:scale-105 font-medium"
                                            >
                                                Open <ExternalLink className="w-2.5 h-2.5" />
                                            </button>
                                        )}
                                    </div>
                                </div>
                            );
                        })}
                    </div>

                    <button
                        onClick={() => scrollServices('right')}
                        className="absolute right-0 top-1/2 -translate-y-1/2 -mr-2 z-10 p-1.5 rounded-full bg-background border shadow-md text-muted-foreground hover:text-foreground opacity-0 group-hover:opacity-100 transition-opacity"
                    >
                        <ChevronRight className="w-4 h-4" />
                    </button>
                </div>

                {/* Content Area: Debug Console OR Flow Editor */}
                <div className="flex-1 flex flex-col min-h-0 bg-surface-1 border border-surface-2 rounded-lg overflow-hidden">
                    {showDebug ? (
                        <>
                            {/* Console Header */}
                            <div className="px-3 py-2 bg-muted border-b border-surface-2 flex items-center justify-between">
                                <div className="flex items-center gap-2">
                                    <Terminal className="w-3.5 h-3.5 text-muted-foreground" />
                                    <span className="text-[11px] text-muted-foreground font-medium">Debug Console</span>
                                    <span className="text-[9px] text-muted-foreground bg-muted px-1.5 py-0.5 rounded">
                                        {logs.length} logs
                                    </span>
                                </div>
                                <div className="flex items-center gap-1">
                                    <button
                                        onClick={copyLogs}
                                        className="p-1 text-slate-500 hover:text-slate-300 rounded transition-colors"
                                        title="Copy logs"
                                    >
                                        <Copy className="w-3 h-3" />
                                    </button>
                                    <button
                                        onClick={clearLogs}
                                        className="p-1 text-slate-500 hover:text-slate-300 rounded transition-colors"
                                        title="Clear logs"
                                    >
                                        <Trash2 className="w-3 h-3" />
                                    </button>
                                </div>
                            </div>

                            {/* Log Filters */}
                            <div className="px-3 py-1.5 bg-muted/50 border-b border-surface-2 flex items-center gap-1 overflow-x-auto">
                                {logFilters.map((filter) => (
                                    <button
                                        key={filter.id}
                                        onClick={() => setActiveLogFilter(filter.id)}
                                        className={`px-2 py-0.5 text-[10px] rounded whitespace-nowrap transition-colors ${activeLogFilter === filter.id
                                            ? 'bg-blue-500/20 text-blue-400'
                                            : 'text-slate-500 hover:text-slate-300'
                                            }`}
                                    >
                                        {filter.label}
                                    </button>
                                ))}
                                <div className="flex-1" />
                                <button
                                    onClick={() => setAutoScroll(!autoScroll)}
                                    className={`text-[9px] ${autoScroll ? 'text-blue-400' : 'text-slate-500'}`}
                                >
                                    Auto-scroll: {autoScroll ? 'ON' : 'OFF'}
                                </button>
                            </div>

                            {/* Log Content */}
                            <div className="flex-1 p-3 font-mono text-[11px] overflow-y-auto bg-editor">
                                {filteredLogs.length === 0 ? (
                                    <div className="text-muted-foreground text-center py-8">
                                        <div className="mb-2">
                                            No logs yet. Click "Launch All" to start {isElectron ? 'services' : 'simulation'}.
                                        </div>
                                        {!isElectron && (
                                            <div className="text-[10px] text-yellow-400/70 mt-2">
                                                💡 실제 서비스 관리는 <code className="px-1 bg-yellow-500/20 rounded">npm run electron:dev</code>로 실행하세요
                                            </div>
                                        )}
                                    </div>
                                ) : (
                                    filteredLogs.map((log) => (
                                        <div key={log.id} className="flex gap-2 mb-0.5 hover:bg-accent px-1 -mx-1 rounded">
                                            <span className="text-slate-600 flex-shrink-0">[{log.timestamp}]</span>
                                            <span className={`flex-shrink-0 w-20 ${getSourceColor(log.source)}`}>
                                                [{log.source.toUpperCase()}]
                                            </span>
                                            <span className={getLogColor(log.type)}>{log.message}</span>
                                        </div>
                                    ))
                                )}
                                <div ref={logsEndRef} />
                            </div>
                        </>
                    ) : (
                        /* Flow Editor Iframe */
                        services.find(s => s.id === 'frontend')?.status === 'running' ? (
                            <div className="w-full h-full flex items-center justify-center overflow-hidden">
                                <iframe
                                    key="flow-editor-iframe"
                                    src="http://localhost:5173"
                                    title="Flow Editor"
                                    style={{
                                        width: '100%',
                                        height: '100%',
                                        transform: 'scale(1)',
                                        transformOrigin: 'center center',
                                        border: 'none',
                                        backgroundColor: 'white',
                                    }}
                                />
                            </div>
                        ) : (
                            <div className="flex-1 flex flex-col items-center justify-center text-muted-foreground">
                                <Rocket className="w-12 h-12 text-slate-700 mb-4" />
                                <div className="text-sm font-medium">Ready to Launch</div>
                                <div className="text-xs text-slate-500 mt-2">
                                    Click "Launch All" to start the Flow Editor
                                </div>
                            </div>
                        )
                    )}
                </div>
            </div>

            {/* Status Bar */}
            <div className="px-4 py-1.5 bg-surface-1 border-t border-surface-2 flex items-center justify-between text-[10px]">
                <div className="flex items-center gap-3">
                    {services.map((service) => (
                        <div key={service.id} className="flex items-center gap-1.5">
                            <div className={`w-1.5 h-1.5 rounded-full ${service.status === 'running' ? 'bg-green-400' :
                                service.status === 'starting' ? 'bg-yellow-400 animate-pulse' :
                                    service.status === 'crashed' ? 'bg-orange-400' :
                                        'bg-slate-600'
                                }`} />
                            <span className="text-muted-foreground">{service.name}</span>
                        </div>
                    ))}
                </div>
                {allRunning && (
                    <span className="text-green-400 flex items-center gap-1">
                        <CheckCircle className="w-3 h-3" />
                        {isElectron ? 'All Systems Running' : 'Simulation Running'}
                    </span>
                )}
            </div>
        </div>
    );
}
