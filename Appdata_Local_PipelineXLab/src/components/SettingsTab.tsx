import { useState, useEffect } from 'react';
import { Sun, Moon, Monitor, FolderOpen, Bell, Shield, HardDrive, Container, Play, Check, X, Loader2, Cpu, Key, Cloud, Server } from 'lucide-react';

type Theme = 'dark' | 'light' | 'system';

// Check if running in Electron
const isElectron = typeof window !== 'undefined' && (window as any).electronAPI;

interface Settings {
    dockerPath: string;
    autoStartDocker: boolean;
    theme: string;
    componentsPath: string;
    // AI Configuration
    openaiApiKey?: string;
    anthropicApiKey?: string;
    ollamaUrl?: string;
    defaultModel?: string;
    monthlyBudget?: number;
}

export default function SettingsTab() {
    const [theme, setTheme] = useState<Theme>(() => {
        return (localStorage.getItem('pxlab_theme') as Theme) || 'dark';
    });
    const [notifications, setNotifications] = useState(true);
    const [autoUpdate, setAutoUpdate] = useState(true);

    // Docker settings
    const [dockerPath, setDockerPath] = useState('C:\\Program Files\\Docker\\Docker\\Docker Desktop.exe');
    const [autoStartDocker, setAutoStartDocker] = useState(true);
    const [dockerStatus, setDockerStatus] = useState<'unknown' | 'checking' | 'running' | 'stopped'>('unknown');
    const [startingDocker, setStartingDocker] = useState(false);

    // ✅ AI Configuration states
    const [openaiApiKey, setOpenaiApiKey] = useState('');
    const [anthropicApiKey, setAnthropicApiKey] = useState('');
    const [ollamaUrl, setOllamaUrl] = useState('http://localhost:11434');
    const [defaultModel, setDefaultModel] = useState('gemma3:4b');
    const [monthlyBudget, setMonthlyBudget] = useState(50);
    const [showOpenaiKey, setShowOpenaiKey] = useState(false);
    const [showAnthropicKey, setShowAnthropicKey] = useState(false);
    const [aiSettingsSaved, setAiSettingsSaved] = useState(false);

    // Load settings from Electron
    useEffect(() => {
        loadSettings();
        checkDockerStatus();
    }, []);

    const loadSettings = async () => {
        if (isElectron) {
            try {
                const settings = await (window as any).electronAPI.getSettings();
                setDockerPath(settings.dockerPath || 'C:\\Program Files\\Docker\\Docker\\Docker Desktop.exe');
                setAutoStartDocker(settings.autoStartDocker ?? true);
                
                // ✅ Load AI settings
                setOpenaiApiKey(settings.openaiApiKey || '');
                setAnthropicApiKey(settings.anthropicApiKey || '');
                setOllamaUrl(settings.ollamaUrl || 'http://localhost:11434');
                setDefaultModel(settings.defaultModel || 'gemma3:4b');
                setMonthlyBudget(settings.monthlyBudget || 50);
            } catch (error) {
                console.error('Failed to load settings:', error);
            }
        }
    };

    const checkDockerStatus = async () => {
        if (isElectron) {
            setDockerStatus('checking');
            try {
                const result = await (window as any).electronAPI.checkDocker();
                setDockerStatus(result.running ? 'running' : 'stopped');
            } catch {
                setDockerStatus('stopped');
            }
        }
    };

    const handleStartDocker = async () => {
        if (isElectron) {
            setStartingDocker(true);
            try {
                const result = await (window as any).electronAPI.startDocker();
                if (result.success) {
                    setDockerStatus('running');
                } else {
                    alert(`Failed to start Docker: ${result.error}`);
                }
            } catch (error: any) {
                alert(`Error: ${error.message}`);
            } finally {
                setStartingDocker(false);
            }
        }
    };

    const handleBrowseDockerPath = async () => {
        if (isElectron) {
            const result = await (window as any).electronAPI.browseForPath('Select Docker Desktop executable');
            if (result.success && result.path) {
                setDockerPath(result.path);
                await (window as any).electronAPI.setSetting('dockerPath', result.path);
            }
        }
    };

    const handleAutoStartDockerChange = async (value: boolean) => {
        setAutoStartDocker(value);
        if (isElectron) {
            await (window as any).electronAPI.setSetting('autoStartDocker', value);
        }
    };

    useEffect(() => {
        localStorage.setItem('pxlab_theme', theme);

        // 테마 적용
        const root = document.documentElement;
        if (theme === 'dark') {
            root.classList.add('dark');
            root.classList.remove('light');
        } else if (theme === 'light') {
            root.classList.remove('dark');
            root.classList.add('light');
        } else {
            // system
            const prefersDark = window.matchMedia('(prefers-color-scheme: dark)').matches;
            root.classList.toggle('dark', prefersDark);
            root.classList.toggle('light', !prefersDark);
        }
    }, [theme]);

    const themeOptions: { value: Theme; label: string; icon: React.ElementType }[] = [
        { value: 'dark', label: 'Dark', icon: Moon },
        { value: 'light', label: 'Light', icon: Sun },
        { value: 'system', label: 'System', icon: Monitor },
    ];

    return (
        <div className="h-full bg-background overflow-y-auto">
            <div className="max-w-2xl mx-auto p-8">
                <h1 className="text-xl font-semibold text-slate-200 mb-6">Settings</h1>

                {/* Docker Section */}
                <section className="mb-8">
                    <h2 className="text-sm font-medium text-slate-400 uppercase tracking-wider mb-4">Docker</h2>
                    <div className="bg-surface-1 rounded-xl border border-surface-2 divide-y divide-surface-2">
                        {/* Docker Status */}
                        <div className="flex items-center justify-between p-4">
                            <div className="flex items-center gap-3">
                                <Container className="w-5 h-5 text-slate-500" />
                                <div>
                                    <h3 className="text-sm text-slate-200">Docker Status</h3>
                                    <div className="flex items-center gap-2 mt-0.5">
                                        {dockerStatus === 'checking' && (
                                            <span className="flex items-center gap-1 text-xs text-slate-500">
                                                <Loader2 className="w-3 h-3 animate-spin" /> Checking...
                                            </span>
                                        )}
                                        {dockerStatus === 'running' && (
                                            <span className="flex items-center gap-1 text-xs text-green-400">
                                                <Check className="w-3 h-3" /> Running
                                            </span>
                                        )}
                                        {dockerStatus === 'stopped' && (
                                            <span className="flex items-center gap-1 text-xs text-red-400">
                                                <X className="w-3 h-3" /> Not running
                                            </span>
                                        )}
                                        {dockerStatus === 'unknown' && (
                                            <span className="text-xs text-slate-500">Unknown</span>
                                        )}
                                    </div>
                                </div>
                            </div>
                            <div className="flex items-center gap-2">
                                <button
                                    onClick={checkDockerStatus}
                                    className="px-3 py-1.5 text-xs text-muted-foreground bg-surface-2 hover:bg-surface-3 rounded-lg transition-colors"
                                >
                                    Refresh
                                </button>
                                {dockerStatus === 'stopped' && (
                                    <button
                                        onClick={handleStartDocker}
                                        disabled={startingDocker}
                                        className="px-3 py-1.5 text-xs text-white bg-blue-600 hover:bg-blue-700 rounded-lg transition-colors flex items-center gap-1.5 disabled:opacity-50"
                                    >
                                        {startingDocker ? (
                                            <><Loader2 className="w-3 h-3 animate-spin" /> Starting...</>
                                        ) : (
                                            <><Play className="w-3 h-3" /> Start Docker</>
                                        )}
                                    </button>
                                )}
                            </div>
                        </div>

                        {/* Docker Path */}
                        <div className="p-4">
                            <div className="flex items-center gap-3 mb-2">
                                <FolderOpen className="w-5 h-5 text-slate-500" />
                                <div className="flex-1">
                                    <h3 className="text-sm text-slate-200">Docker Desktop Path</h3>
                                    <p className="text-xs text-slate-500 mt-0.5">Path to Docker Desktop executable</p>
                                </div>
                            </div>
                            <div className="flex gap-2 mt-2">
                                <input
                                    type="text"
                                    value={dockerPath}
                                    onChange={(e) => setDockerPath(e.target.value)}
                                    className="flex-1 px-3 py-1.5 text-xs text-foreground bg-surface-2 border border-surface-3 rounded-lg focus:outline-none focus:border-blue-500"
                                />
                                <button
                                    onClick={handleBrowseDockerPath}
                                    className="px-3 py-1.5 text-xs text-muted-foreground bg-surface-2 hover:bg-surface-3 rounded-lg transition-colors"
                                >
                                    Browse
                                </button>
                            </div>
                        </div>

                        {/* Auto-start Docker */}
                        <div className="flex items-center justify-between p-4">
                            <div className="flex items-center gap-3">
                                <Play className="w-5 h-5 text-slate-500" />
                                <div>
                                    <h3 className="text-sm text-slate-200">Auto-start Docker</h3>
                                    <p className="text-xs text-slate-500">Automatically start Docker when launching services</p>
                                </div>
                            </div>
                            <button
                                onClick={() => handleAutoStartDockerChange(!autoStartDocker)}
                                className={`w-11 h-6 rounded-full transition-colors ${autoStartDocker ? 'bg-blue-600' : 'bg-slate-700'}`}
                            >
                                <div className={`w-5 h-5 bg-white rounded-full shadow transition-transform ${autoStartDocker ? 'translate-x-5' : 'translate-x-0.5'}`} />
                            </button>
                        </div>
                    </div>
                </section>

                {/* ✅ AI Configuration Section */}
                <section className="mb-8">
                    <h2 className="text-sm font-medium text-slate-400 uppercase tracking-wider mb-4">AI Configuration</h2>
                    <div className="bg-surface-1 rounded-xl border border-surface-2 divide-y divide-surface-2">
                        {/* OpenAI API Key */}
                        <div className="p-4">
                            <div className="flex items-center gap-3 mb-2">
                                <Key className="w-5 h-5 text-slate-500" />
                                <div className="flex-1">
                                    <h3 className="text-sm text-slate-200">OpenAI API Key</h3>
                                    <p className="text-xs text-slate-500 mt-0.5">API key for GPT-4, GPT-4o models</p>
                                </div>
                            </div>
                            <div className="flex gap-2 mt-2">
                                <input
                                    type={showOpenaiKey ? "text" : "password"}
                                    value={openaiApiKey}
                                    onChange={(e) => setOpenaiApiKey(e.target.value)}
                                    placeholder="sk-..."
                                    className="flex-1 px-3 py-1.5 text-xs text-foreground bg-surface-2 border border-surface-3 rounded-lg focus:outline-none focus:border-blue-500 font-mono"
                                />
                                <button
                                    onClick={() => setShowOpenaiKey(!showOpenaiKey)}
                                    className="px-3 py-1.5 text-xs text-muted-foreground bg-surface-2 hover:bg-surface-3 rounded-lg transition-colors"
                                >
                                    {showOpenaiKey ? 'Hide' : 'Show'}
                                </button>
                            </div>
                        </div>

                        {/* Anthropic API Key */}
                        <div className="p-4">
                            <div className="flex items-center gap-3 mb-2">
                                <Key className="w-5 h-5 text-slate-500" />
                                <div className="flex-1">
                                    <h3 className="text-sm text-slate-200">Anthropic API Key</h3>
                                    <p className="text-xs text-slate-500 mt-0.5">API key for Claude models</p>
                                </div>
                            </div>
                            <div className="flex gap-2 mt-2">
                                <input
                                    type={showAnthropicKey ? "text" : "password"}
                                    value={anthropicApiKey}
                                    onChange={(e) => setAnthropicApiKey(e.target.value)}
                                    placeholder="sk-ant-..."
                                    className="flex-1 px-3 py-1.5 text-xs text-foreground bg-surface-2 border border-surface-3 rounded-lg focus:outline-none focus:border-blue-500 font-mono"
                                />
                                <button
                                    onClick={() => setShowAnthropicKey(!showAnthropicKey)}
                                    className="px-3 py-1.5 text-xs text-muted-foreground bg-surface-2 hover:bg-surface-3 rounded-lg transition-colors"
                                >
                                    {showAnthropicKey ? 'Hide' : 'Show'}
                                </button>
                            </div>
                        </div>

                        {/* Ollama URL */}
                        <div className="p-4">
                            <div className="flex items-center gap-3 mb-2">
                                <Server className="w-5 h-5 text-slate-500" />
                                <div className="flex-1">
                                    <h3 className="text-sm text-slate-200">Ollama URL</h3>
                                    <p className="text-xs text-slate-500 mt-0.5">Local Ollama server URL (for free models)</p>
                                </div>
                            </div>
                            <input
                                type="text"
                                value={ollamaUrl}
                                onChange={(e) => setOllamaUrl(e.target.value)}
                                placeholder="http://localhost:11434"
                                className="w-full px-3 py-1.5 text-xs text-foreground bg-surface-2 border border-surface-3 rounded-lg focus:outline-none focus:border-blue-500 font-mono mt-2"
                            />
                        </div>

                        {/* Default Model */}
                        <div className="p-4">
                            <div className="flex items-center gap-3 mb-2">
                                <Cpu className="w-5 h-5 text-slate-500" />
                                <div className="flex-1">
                                    <h3 className="text-sm text-slate-200">Default Model</h3>
                                    <p className="text-xs text-slate-500 mt-0.5">Default LLM model for AI nodes</p>
                                </div>
                            </div>
                            <select
                                value={defaultModel}
                                onChange={(e) => setDefaultModel(e.target.value)}
                                className="w-full px-3 py-1.5 text-xs text-foreground bg-surface-2 border border-surface-3 rounded-lg focus:outline-none focus:border-blue-500 mt-2"
                            >
                                <optgroup label="Local (Ollama)">
                                    <option value="gemma3:4b">Gemma 3 4B (Recommended)</option>
                                    <option value="llama3:8b">Llama 3 8B</option>
                                    <option value="llama3:70b">Llama 3 70B</option>
                                    <option value="mistral:7b">Mistral 7B</option>
                                    <option value="codellama:13b">Code Llama 13B</option>
                                </optgroup>
                                <optgroup label="OpenAI">
                                    <option value="gpt-4o-mini">GPT-4o Mini</option>
                                    <option value="gpt-4o">GPT-4o</option>
                                    <option value="gpt-4-turbo">GPT-4 Turbo</option>
                                </optgroup>
                                <optgroup label="Anthropic">
                                    <option value="claude-3-5-sonnet-20241022">Claude 3.5 Sonnet</option>
                                    <option value="claude-3-opus-20240229">Claude 3 Opus</option>
                                </optgroup>
                            </select>
                        </div>

                        {/* Monthly Budget */}
                        <div className="p-4">
                            <div className="flex items-center gap-3 mb-2">
                                <Cloud className="w-5 h-5 text-slate-500" />
                                <div className="flex-1">
                                    <h3 className="text-sm text-slate-200">Monthly Budget (USD)</h3>
                                    <p className="text-xs text-slate-500 mt-0.5">Monthly spending limit for cloud models</p>
                                </div>
                            </div>
                            <div className="flex items-center gap-3 mt-2">
                                <input
                                    type="range"
                                    min="10"
                                    max="500"
                                    step="10"
                                    value={monthlyBudget}
                                    onChange={(e) => setMonthlyBudget(Number(e.target.value))}
                                    className="flex-1 h-2 bg-surface-2 rounded-lg appearance-none cursor-pointer accent-blue-600"
                                />
                                <span className="text-sm text-slate-200 font-mono min-w-[60px] text-right">
                                    ${monthlyBudget}
                                </span>
                            </div>
                        </div>

                        {/* Save Button */}
                        <div className="p-4 bg-surface-2/50">
                            <button
                                onClick={async () => {
                                    if (isElectron) {
                                        await (window as any).electronAPI.setSetting('openaiApiKey', openaiApiKey);
                                        await (window as any).electronAPI.setSetting('anthropicApiKey', anthropicApiKey);
                                        await (window as any).electronAPI.setSetting('ollamaUrl', ollamaUrl);
                                        await (window as any).electronAPI.setSetting('defaultModel', defaultModel);
                                        await (window as any).electronAPI.setSetting('monthlyBudget', monthlyBudget);
                                        setAiSettingsSaved(true);
                                        setTimeout(() => setAiSettingsSaved(false), 2000);
                                    }
                                }}
                                className={`w-full px-4 py-2 text-sm font-medium rounded-lg transition-all ${
                                    aiSettingsSaved 
                                        ? 'bg-green-600 text-white' 
                                        : 'bg-blue-600 text-white hover:bg-blue-700'
                                }`}
                            >
                                {aiSettingsSaved ? (
                                    <span className="flex items-center justify-center gap-2">
                                        <Check className="w-4 h-4" /> Saved!
                                    </span>
                                ) : (
                                    'Save AI Configuration'
                                )}
                            </button>
                            <p className="text-xs text-slate-500 mt-2 text-center">
                                💡 API keys are encrypted using electron safeStorage
                            </p>
                        </div>
                    </div>
                </section>

                {/* Appearance Section */}
                <section className="mb-8">
                    <h2 className="text-sm font-medium text-slate-400 uppercase tracking-wider mb-4">Appearance</h2>
                    <div className="bg-surface-1 rounded-xl border border-surface-2 p-4">
                        <div className="flex items-center justify-between mb-4">
                            <div>
                                <h3 className="text-sm text-slate-200 font-medium">Theme</h3>
                                <p className="text-xs text-slate-500 mt-0.5">Select your preferred color theme</p>
                            </div>
                        </div>
                        <div className="flex gap-2">
                            {themeOptions.map((option) => {
                                const Icon = option.icon;
                                const isActive = theme === option.value;
                                return (
                                    <button
                                        key={option.value}
                                        onClick={() => setTheme(option.value)}
                                        className={`flex-1 flex items-center justify-center gap-2 py-3 rounded-lg border transition-all ${isActive
                                            ? 'bg-blue-600 border-blue-500 text-white'
                                            : 'bg-surface-2 border-surface-3 text-muted-foreground hover:border-foreground'
                                            }`}
                                    >
                                        <Icon className="w-4 h-4" />
                                        <span className="text-sm">{option.label}</span>
                                    </button>
                                );
                            })}
                        </div>
                    </div>
                </section>

                {/* Notifications Section */}
                <section className="mb-8">
                    <h2 className="text-sm font-medium text-slate-400 uppercase tracking-wider mb-4">Notifications</h2>
                    <div className="bg-surface-1 rounded-xl border border-surface-2 divide-y divide-surface-2">
                        <div className="flex items-center justify-between p-4">
                            <div className="flex items-center gap-3">
                                <Bell className="w-5 h-5 text-slate-500" />
                                <div>
                                    <h3 className="text-sm text-slate-200">Push Notifications</h3>
                                    <p className="text-xs text-slate-500">Receive updates about new features</p>
                                </div>
                            </div>
                            <button
                                onClick={() => setNotifications(!notifications)}
                                className={`w-11 h-6 rounded-full transition-colors ${notifications ? 'bg-blue-600' : 'bg-slate-700'}`}
                            >
                                <div className={`w-5 h-5 bg-white rounded-full shadow transition-transform ${notifications ? 'translate-x-5' : 'translate-x-0.5'}`} />
                            </button>
                        </div>
                        <div className="flex items-center justify-between p-4">
                            <div className="flex items-center gap-3">
                                <Shield className="w-5 h-5 text-slate-500" />
                                <div>
                                    <h3 className="text-sm text-slate-200">Auto Update</h3>
                                    <p className="text-xs text-slate-500">Automatically download updates</p>
                                </div>
                            </div>
                            <button
                                onClick={() => setAutoUpdate(!autoUpdate)}
                                className={`w-11 h-6 rounded-full transition-colors ${autoUpdate ? 'bg-blue-600' : 'bg-slate-700'}`}
                            >
                                <div className={`w-5 h-5 bg-white rounded-full shadow transition-transform ${autoUpdate ? 'translate-x-5' : 'translate-x-0.5'}`} />
                            </button>
                        </div>
                    </div>
                </section>

                {/* Storage Section */}
                <section className="mb-8">
                    <h2 className="text-sm font-medium text-slate-400 uppercase tracking-wider mb-4">Storage</h2>
                    <div className="bg-surface-1 rounded-xl border border-surface-2 p-4">
                        <div className="flex items-center gap-3 mb-4">
                            <HardDrive className="w-5 h-5 text-slate-500" />
                            <div className="flex-1">
                                <h3 className="text-sm text-slate-200">Components Location</h3>
                                <p className="text-xs text-slate-500 font-mono mt-1">%APPDATA%\Local\PipelineXLab\components</p>
                            </div>
                            <button className="px-3 py-1.5 text-xs text-muted-foreground bg-surface-2 hover:bg-surface-3 rounded-lg transition-colors">
                                Change
                            </button>
                        </div>
                        <div className="flex items-center justify-between text-xs">
                            <span className="text-slate-500">Storage used</span>
                            <span className="text-slate-300">1.2 GB / 10 GB</span>
                        </div>
                        <div className="mt-2 h-2 bg-surface-2 rounded-full overflow-hidden">
                            <div className="h-full w-[12%] bg-blue-600 rounded-full" />
                        </div>
                    </div>
                </section>

                {/* About Section */}
                <section>
                    <h2 className="text-sm font-medium text-slate-400 uppercase tracking-wider mb-4">About</h2>
                    <div className="bg-surface-1 rounded-xl border border-surface-2 p-4">
                        <div className="flex items-center justify-between mb-3">
                            <span className="text-sm text-slate-400">Version</span>
                            <span className="text-sm text-slate-200">1.0.0</span>
                        </div>
                        <div className="flex items-center justify-between">
                            <span className="text-sm text-slate-400">Build</span>
                            <span className="text-sm text-slate-200">2025.12.30</span>
                        </div>
                    </div>
                </section>
            </div>
        </div>
    );
}
