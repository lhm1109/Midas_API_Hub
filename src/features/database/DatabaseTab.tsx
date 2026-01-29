/**
 * DatabaseTab - Supabase 데이터베이스 관리 탭
 * SQL 쿼리 실행, 마이그레이션 관리, 테이블 조회
 */

import { useState, useEffect } from 'react';
import { Database, Play, FileText, RefreshCw, CheckCircle2, XCircle, Copy, ExternalLink } from 'lucide-react';
import { Button } from '@/components/ui/button';
import { ScrollArea } from '@/components/ui/scroll-area';

interface MigrationFile {
    name: string;
    path: string;
    content: string;
}

interface QueryResult {
    success: boolean;
    message?: string;
    error?: string;
    data?: any;
    sql?: string;
    instruction?: string;
}

interface SupabaseSettings {
    supabaseUrl?: string;
    supabaseServiceKey?: string;
    supabaseDbPassword?: string;
}

interface DatabaseTabProps {
    settings?: SupabaseSettings;
}

export function DatabaseTab({ settings: propSettings }: DatabaseTabProps) {
    const [sql, setSql] = useState('');
    const [result, setResult] = useState<QueryResult | null>(null);
    const [loading, setLoading] = useState(false);
    const [migrations, setMigrations] = useState<MigrationFile[]>([]);
    const [selectedMigration, setSelectedMigration] = useState<string | null>(null);
    const [connectionStatus, setConnectionStatus] = useState<'unknown' | 'connected' | 'error'>('unknown');

    // props에서 settings 사용
    const settings = propSettings || {};

    // 🔥 디버깅
    console.log('DatabaseTab settings:', settings);
    console.log('supabaseUrl:', settings?.supabaseUrl);
    console.log('supabaseDbPassword:', settings?.supabaseDbPassword);

    // Supabase 설정 확인 (URL + DB Password 필요)
    const hasSupabaseConfig = Boolean(settings?.supabaseUrl && settings?.supabaseDbPassword);

    // 마이그레이션 파일 목록 로드
    useEffect(() => {
        loadMigrations();
    }, []);

    // 연결 테스트
    useEffect(() => {
        if (hasSupabaseConfig) {
            testConnection();
        }
    }, [settings?.supabaseUrl, settings?.supabaseDbPassword]);

    const loadMigrations = async () => {
        try {
            const response = await fetch('http://localhost:9527/api/database/migrations');
            const data = await response.json();
            setMigrations(data.files || []);
        } catch (error) {
            console.error('Failed to load migrations:', error);
        }
    };

    const testConnection = async () => {
        try {
            const response = await fetch('http://localhost:9527/api/database/test-connection', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({
                    supabaseUrl: settings?.supabaseUrl,
                    dbPassword: settings?.supabaseDbPassword,
                }),
            });
            const data = await response.json();
            setConnectionStatus(data.success ? 'connected' : 'error');
        } catch (error) {
            setConnectionStatus('error');
        }
    };

    const executeQuery = async () => {
        if (!sql.trim()) {
            setResult({ success: false, error: 'SQL query is required' });
            return;
        }

        setLoading(true);
        try {
            const response = await fetch('http://localhost:9527/api/database/query', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({
                    sql,
                    supabaseUrl: settings?.supabaseUrl,
                    dbPassword: settings?.supabaseDbPassword,
                }),
            });
            const data = await response.json();
            setResult(data);
        } catch (error) {
            setResult({ success: false, error: error instanceof Error ? error.message : 'Unknown error' });
        } finally {
            setLoading(false);
        }
    };

    const runMigration = async (migration: MigrationFile) => {
        setLoading(true);
        try {
            const response = await fetch('http://localhost:9527/api/database/migrate', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({
                    fileName: migration.name,
                    sql: migration.content,
                    supabaseUrl: settings?.supabaseUrl,
                    dbPassword: settings?.supabaseDbPassword,
                }),
            });
            const data = await response.json();
            setResult(data);
        } catch (error) {
            setResult({ success: false, error: error instanceof Error ? error.message : 'Unknown error' });
        } finally {
            setLoading(false);
        }
    };

    const copyToClipboard = (text: string) => {
        navigator.clipboard.writeText(text);
        alert('✅ Copied to clipboard!');
    };

    const openSupabaseDashboard = () => {
        if (settings?.supabaseUrl) {
            // supabaseUrl 형식: https://xxx.supabase.co
            const projectRef = settings.supabaseUrl.split('https://')[1]?.split('.supabase.co')[0];
            if (projectRef) {
                window.open(`https://supabase.com/dashboard/project/${projectRef}/sql`, '_blank');
            }
        }
    };

    return (
        <div className="h-full flex flex-col bg-zinc-900">
            {/* Header */}
            <div className="p-4 border-b border-zinc-800 flex items-center justify-between">
                <div className="flex items-center gap-3">
                    <Database className="w-5 h-5 text-green-400" />
                    <h2 className="text-lg font-semibold text-white">Database Manager</h2>
                    {connectionStatus === 'connected' && (
                        <span className="flex items-center gap-1 text-xs text-green-400 bg-green-900/30 px-2 py-1 rounded">
                            <CheckCircle2 className="w-3 h-3" />
                            Connected
                        </span>
                    )}
                    {connectionStatus === 'error' && (
                        <span className="flex items-center gap-1 text-xs text-red-400 bg-red-900/30 px-2 py-1 rounded">
                            <XCircle className="w-3 h-3" />
                            Connection Error
                        </span>
                    )}
                </div>
                <div className="flex items-center gap-2">
                    <Button
                        variant="outline"
                        size="sm"
                        onClick={testConnection}
                        disabled={!hasSupabaseConfig}
                    >
                        <RefreshCw className="w-4 h-4 mr-1" />
                        Test Connection
                    </Button>
                    <Button
                        variant="outline"
                        size="sm"
                        onClick={openSupabaseDashboard}
                        disabled={!settings?.supabaseUrl}
                    >
                        <ExternalLink className="w-4 h-4 mr-1" />
                        Supabase Dashboard
                    </Button>
                </div>
            </div>

            {/* Config Warning */}
            {!hasSupabaseConfig && (
                <div className="p-4 bg-yellow-900/30 border-b border-yellow-700/30">
                    <p className="text-yellow-400 text-sm">
                        ⚠️ Supabase 설정이 필요합니다. Settings에서 <strong>Supabase URL</strong>과 <strong>Database Password</strong>를 설정해주세요.
                    </p>
                </div>
            )}

            <div className="flex-1 flex overflow-hidden">
                {/* Left: Migrations */}
                <div className="w-64 border-r border-zinc-800 flex flex-col">
                    <div className="p-3 border-b border-zinc-800">
                        <h3 className="text-sm font-semibold text-zinc-300 flex items-center gap-2">
                            <FileText className="w-4 h-4" />
                            Migration Files
                        </h3>
                    </div>
                    <ScrollArea className="flex-1">
                        <div className="p-2 space-y-1">
                            {migrations.map((migration) => (
                                <button
                                    key={migration.name}
                                    onClick={() => {
                                        setSelectedMigration(migration.name);
                                        setSql(migration.content);
                                    }}
                                    className={`w-full text-left px-3 py-2 rounded text-sm transition-colors ${selectedMigration === migration.name
                                        ? 'bg-green-900/30 text-green-400 border border-green-700/50'
                                        : 'text-zinc-400 hover:bg-zinc-800 hover:text-zinc-200'
                                        }`}
                                >
                                    <div className="truncate">{migration.name}</div>
                                </button>
                            ))}
                        </div>
                    </ScrollArea>
                </div>

                {/* Right: SQL Editor & Result */}
                <div className="flex-1 flex flex-col">
                    {/* SQL Editor */}
                    <div className="flex-1 flex flex-col border-b border-zinc-800">
                        <div className="p-3 border-b border-zinc-800 flex items-center justify-between">
                            <h3 className="text-sm font-semibold text-zinc-300">SQL Editor</h3>
                            <div className="flex items-center gap-2">
                                {selectedMigration && (
                                    <Button
                                        variant="outline"
                                        size="sm"
                                        onClick={() => {
                                            const migration = migrations.find(m => m.name === selectedMigration);
                                            if (migration) runMigration(migration);
                                        }}
                                        disabled={loading || !hasSupabaseConfig}
                                    >
                                        <Play className="w-4 h-4 mr-1 text-orange-400" />
                                        Run Migration
                                    </Button>
                                )}
                                <Button
                                    variant="default"
                                    size="sm"
                                    onClick={executeQuery}
                                    disabled={loading || !hasSupabaseConfig}
                                >
                                    <Play className="w-4 h-4 mr-1" />
                                    {loading ? 'Executing...' : 'Execute'}
                                </Button>
                            </div>
                        </div>
                        <textarea
                            value={sql}
                            onChange={(e) => setSql(e.target.value)}
                            className="flex-1 bg-zinc-950 text-zinc-100 font-mono text-sm p-4 resize-none focus:outline-none"
                            placeholder="-- Enter SQL query here...
SELECT * FROM products LIMIT 10;"
                            spellCheck={false}
                        />
                    </div>

                    {/* Result */}
                    <div className="flex-1 flex flex-col min-h-[200px]">
                        <div className="p-3 border-b border-zinc-800 flex items-center justify-between">
                            <h3 className="text-sm font-semibold text-zinc-300">Result</h3>
                            {result?.sql && (
                                <Button
                                    variant="ghost"
                                    size="sm"
                                    onClick={() => copyToClipboard(result.sql!)}
                                >
                                    <Copy className="w-4 h-4 mr-1" />
                                    Copy SQL
                                </Button>
                            )}
                        </div>
                        <ScrollArea className="flex-1 h-full">
                            <div className="p-4">
                                {result ? (
                                    <div className="space-y-3">
                                        {result.success ? (
                                            <div className="flex items-center gap-2 text-green-400">
                                                <CheckCircle2 className="w-4 h-4" />
                                                <span>{result.message}</span>
                                            </div>
                                        ) : (
                                            <div className="flex items-center gap-2 text-red-400">
                                                <XCircle className="w-4 h-4" />
                                                <span>{result.error}</span>
                                            </div>
                                        )}

                                        {result.instruction && (
                                            <div className="bg-blue-900/20 border border-blue-700/30 rounded p-3 text-sm text-blue-300">
                                                <p className="font-semibold mb-2">📋 Instructions:</p>
                                                <p>{result.instruction}</p>
                                                <Button
                                                    variant="outline"
                                                    size="sm"
                                                    className="mt-3"
                                                    onClick={openSupabaseDashboard}
                                                >
                                                    <ExternalLink className="w-4 h-4 mr-1" />
                                                    Open SQL Editor
                                                </Button>
                                            </div>
                                        )}

                                        {result.data && (
                                            <pre className="bg-zinc-950 p-3 rounded text-xs text-zinc-300 overflow-auto max-h-[400px]">
                                                {JSON.stringify(result.data, null, 2)}
                                            </pre>
                                        )}
                                    </div>
                                ) : (
                                    <p className="text-zinc-500 text-sm">Execute a query to see results</p>
                                )}
                            </div>
                        </ScrollArea>
                    </div>
                </div>
            </div>
        </div>
    );
}
