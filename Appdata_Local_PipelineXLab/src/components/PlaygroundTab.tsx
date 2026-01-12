import { useState, useEffect } from 'react';
import {
    Plus, Play, Save, Settings, Eye, Zap, Box, Type, Hash,
    ToggleLeft, List, File, ChevronDown, Copy, TestTube, Code,
    FileJson, Download, Check, RefreshCw, X, ArrowRight
} from 'lucide-react';

type FieldType = 'text' | 'number' | 'boolean' | 'select' | 'file' | 'list';

interface ComponentField {
    id: string;
    name: string;
    type: FieldType;
    label: string;
    defaultValue?: any;
    required?: boolean;
}

interface ComponentDef {
    name: string;
    id: string;
    version: string;
    description: string;
    category: string;
    author: string;
    inputs: ComponentField[];
    outputs: ComponentField[];
    pythonCode: string;
}

const defaultPythonCode = `# PipelineXLab Component
# 아래 run() 함수에 로직을 작성하세요

def run(inputs: dict) -> dict:
    """
    컴포넌트 메인 함수
    
    Args:
        inputs: 입력 값 딕셔너리 (정의한 input 필드들)
        
    Returns:
        출력 값 딕셔너리 (정의한 output 필드들)
    """
    # 예시: 입력값 처리
    result = inputs.get('input_data', '')
    
    # 로직 작성
    processed = f"Processed: {result}"
    
    # 출력 반환
    return {
        'output_result': processed
    }
`;

const categories = ['Utility', 'Data Processing', 'Visualization', 'AI/ML', 'Analysis', 'Structural', 'Custom'];

export function PlaygroundTab() {
    const [component, setComponent] = useState<ComponentDef>({
        name: 'My Component',
        id: 'my-component',
        version: '0.1.0',
        description: '',
        category: 'Utility',
        author: '',
        inputs: [{ id: '1', name: 'input_data', type: 'text', label: 'Input Data', required: true }],
        outputs: [{ id: '2', name: 'output_result', type: 'text', label: 'Output Result', required: false }],
        pythonCode: defaultPythonCode,
    });

    const [testInputs, setTestInputs] = useState<Record<string, any>>({});
    const [testOutput, setTestOutput] = useState<any>(null);
    const [isRunning, setIsRunning] = useState(false);
    const [activeTab, setActiveTab] = useState<'code' | 'test' | 'manifest'>('code');
    const [copied, setCopied] = useState(false);

    // Auto-generate component ID from name
    useEffect(() => {
        const id = component.name.toLowerCase().replace(/\s+/g, '-').replace(/[^a-z0-9-]/g, '');
        setComponent(prev => ({ ...prev, id }));
    }, [component.name]);

    const addField = (section: 'inputs' | 'outputs') => {
        const count = component[section].length + 1;
        const prefix = section === 'inputs' ? 'input' : 'output';
        const newField: ComponentField = {
            id: `${section}-${Date.now()}`,
            name: `${prefix}_${count}`,
            type: 'text',
            label: `${section === 'inputs' ? 'Input' : 'Output'} ${count}`,
            required: false,
        };
        setComponent(prev => ({ ...prev, [section]: [...prev[section], newField] }));
    };

    const updateField = (section: 'inputs' | 'outputs', id: string, updates: Partial<ComponentField>) => {
        setComponent(prev => ({
            ...prev,
            [section]: prev[section].map(f => f.id === id ? { ...f, ...updates } : f),
        }));
    };

    const removeField = (section: 'inputs' | 'outputs', id: string) => {
        setComponent(prev => ({
            ...prev,
            [section]: prev[section].filter(f => f.id !== id),
        }));
    };

    // Generate manifest JSON
    const generateManifest = () => {
        return {
            "$schema": "https://pipelinexlab.com/schemas/component.json",
            id: component.id,
            name: component.name,
            version: component.version,
            description: component.description,
            author: {
                name: component.author || "Unknown",
            },
            category: component.category,
            inputs: component.inputs.map(i => ({
                name: i.name,
                type: i.type,
                label: i.label,
                required: i.required,
                default: i.defaultValue,
            })),
            outputs: component.outputs.map(o => ({
                name: o.name,
                type: o.type,
                label: o.label,
            })),
            entry: {
                main: "main.py",
                function: "run",
            },
        };
    };

    const runTest = () => {
        setIsRunning(true);
        setTimeout(() => {
            // 시뮬레이션: 실제로는 백엔드에서 Python 실행
            const mockOutput: Record<string, any> = {};
            component.outputs.forEach(o => {
                if (o.type === 'text') mockOutput[o.name] = `Processed: ${testInputs[component.inputs[0]?.name] || 'test'}`;
                else if (o.type === 'number') mockOutput[o.name] = 42;
                else mockOutput[o.name] = 'output';
            });
            setTestOutput({ success: true, data: mockOutput, time: `${Math.random() * 500 + 100 | 0}ms` });
            setIsRunning(false);
        }, 1000);
    };

    const copyManifest = () => {
        navigator.clipboard.writeText(JSON.stringify(generateManifest(), null, 2));
        setCopied(true);
        setTimeout(() => setCopied(false), 2000);
    };

    const downloadComponent = () => {
        alert(`컴포넌트 패키지 다운로드:\n- component.json\n- main.py\n\nAppData/Local/PipelineXLab/components/${component.id}/ 에 저장됩니다.`);
    };

    return (
        <div className="flex h-full bg-background">
            {/* Left - Component Setup */}
            <div className="w-80 bg-surface-1 border-r border-surface-2 flex flex-col overflow-hidden">
                <div className="p-3 border-b border-surface-2">
                    <div className="flex items-center gap-2">
                        <TestTube className="w-4 h-4 text-purple-400" />
                        <span className="text-xs text-foreground font-medium">Component Builder</span>
                    </div>
                </div>

                <div className="flex-1 overflow-y-auto p-3 space-y-4">
                    {/* Basic Info */}
                    <div className="space-y-2">
                        <input
                            value={component.name}
                            onChange={(e) => setComponent(p => ({ ...p, name: e.target.value }))}
                            className="w-full px-3 py-2 bg-input border border-border rounded text-sm text-foreground"
                            placeholder="Component Name"
                        />
                        <div className="flex gap-2">
                            <input
                                value={component.version}
                                onChange={(e) => setComponent(p => ({ ...p, version: e.target.value }))}
                                className="w-20 px-2 py-1.5 bg-input border border-border rounded text-[11px] text-foreground"
                                placeholder="0.1.0"
                            />
                            <select
                                value={component.category}
                                onChange={(e) => setComponent(p => ({ ...p, category: e.target.value }))}
                                className="flex-1 px-2 py-1.5 bg-input border border-border rounded text-[11px] text-foreground"
                            >
                                {categories.map(c => <option key={c}>{c}</option>)}
                            </select>
                        </div>
                        <textarea
                            value={component.description}
                            onChange={(e) => setComponent(p => ({ ...p, description: e.target.value }))}
                            className="w-full px-3 py-2 bg-input border border-border rounded text-[11px] text-foreground resize-none"
                            rows={2}
                            placeholder="Description..."
                        />
                    </div>

                    {/* Inputs */}
                    <div>
                        <div className="flex items-center justify-between mb-2">
                            <span className="text-[10px] text-blue-400 uppercase tracking-wide font-medium">Inputs</span>
                            <button onClick={() => addField('inputs')} className="text-[10px] text-blue-400 hover:underline">+ Add</button>
                        </div>
                        {component.inputs.map(f => (
                            <div key={f.id} className="flex items-center gap-1.5 mb-1.5 p-2 bg-muted rounded border border-surface-2">
                                <select
                                    value={f.type}
                                    onChange={(e) => updateField('inputs', f.id, { type: e.target.value as FieldType })}
                                    className="w-16 px-1 py-0.5 bg-transparent border border-border rounded text-[9px] text-muted-foreground"
                                >
                                    <option value="text">text</option>
                                    <option value="number">num</option>
                                    <option value="boolean">bool</option>
                                    <option value="file">file</option>
                                </select>
                                <input
                                    value={f.name}
                                    onChange={(e) => updateField('inputs', f.id, { name: e.target.value })}
                                    className="flex-1 px-2 py-0.5 bg-transparent border border-border rounded text-[10px] text-foreground font-mono"
                                />
                                <button onClick={() => removeField('inputs', f.id)} className="text-slate-600 hover:text-red-400">
                                    <X className="w-3 h-3" />
                                </button>
                            </div>
                        ))}
                    </div>

                    {/* Outputs */}
                    <div>
                        <div className="flex items-center justify-between mb-2">
                            <span className="text-[10px] text-green-400 uppercase tracking-wide font-medium">Outputs</span>
                            <button onClick={() => addField('outputs')} className="text-[10px] text-green-400 hover:underline">+ Add</button>
                        </div>
                        {component.outputs.map(f => (
                            <div key={f.id} className="flex items-center gap-1.5 mb-1.5 p-2 bg-muted rounded border border-surface-2">
                                <select
                                    value={f.type}
                                    onChange={(e) => updateField('outputs', f.id, { type: e.target.value as FieldType })}
                                    className="w-16 px-1 py-0.5 bg-transparent border border-border rounded text-[9px] text-muted-foreground"
                                >
                                    <option value="text">text</option>
                                    <option value="number">num</option>
                                    <option value="boolean">bool</option>
                                    <option value="file">file</option>
                                </select>
                                <input
                                    value={f.name}
                                    onChange={(e) => updateField('outputs', f.id, { name: e.target.value })}
                                    className="flex-1 px-2 py-0.5 bg-transparent border border-border rounded text-[10px] text-foreground font-mono"
                                />
                                <button onClick={() => removeField('outputs', f.id)} className="text-slate-600 hover:text-red-400">
                                    <X className="w-3 h-3" />
                                </button>
                            </div>
                        ))}
                    </div>

                    {/* Node Preview */}
                    <div className="p-3 bg-muted rounded-lg border border-surface-2">
                        <div className="text-[9px] text-slate-600 mb-2">Preview</div>
                        <div className="flex items-center justify-between">
                            <div className="space-y-1">
                                {component.inputs.map(i => (
                                    <div key={i.id} className="flex items-center gap-1">
                                        <div className="w-2 h-2 rounded-full bg-blue-500" />
                                        <span className="text-[8px] text-slate-500">{i.name}</span>
                                    </div>
                                ))}
                            </div>
                            <div className="px-3 py-2 bg-muted rounded text-[9px] text-foreground">{component.name}</div>
                            <div className="space-y-1">
                                {component.outputs.map(o => (
                                    <div key={o.id} className="flex items-center gap-1">
                                        <span className="text-[8px] text-slate-500">{o.name}</span>
                                        <div className="w-2 h-2 rounded-full bg-green-500" />
                                    </div>
                                ))}
                            </div>
                        </div>
                    </div>
                </div>

                {/* Actions */}
                <div className="p-3 border-t border-surface-2 space-y-2">
                    <button onClick={downloadComponent} className="w-full flex items-center justify-center gap-2 py-2 bg-purple-600 hover:bg-purple-700 text-white text-xs rounded">
                        <Download className="w-4 h-4" /> Register & Save
                    </button>
                </div>
            </div>

            {/* Right - Code/Test/Manifest */}
            <div className="flex-1 flex flex-col">
                {/* Tabs */}
                <div className="flex border-b border-surface-2 bg-muted">
                    {[
                        { id: 'code', label: 'Python Code', icon: Code },
                        { id: 'test', label: 'Test', icon: Play },
                        { id: 'manifest', label: 'Manifest', icon: FileJson },
                    ].map(tab => (
                        <button
                            key={tab.id}
                            onClick={() => setActiveTab(tab.id as any)}
                            className={`flex items-center gap-2 px-4 py-3 text-xs border-b-2 ${activeTab === tab.id ? 'border-blue-500 text-blue-400' : 'border-transparent text-slate-500'
                                }`}
                        >
                            <tab.icon className="w-4 h-4" /> {tab.label}
                        </button>
                    ))}
                </div>

                {/* Code Editor */}
                {activeTab === 'code' && (
                    <div className="flex-1 flex flex-col">
                        <div className="px-4 py-2 bg-muted border-b border-surface-2 flex items-center justify-between">
                            <span className="text-[11px] text-slate-500">main.py</span>
                            <span className="text-[9px] text-slate-600">Python 3.9+</span>
                        </div>
                        <textarea
                            value={component.pythonCode}
                            onChange={(e) => setComponent(p => ({ ...p, pythonCode: e.target.value }))}
                            className="flex-1 bg-editor text-foreground font-mono text-[12px] p-4 resize-none focus:outline-none leading-relaxed"
                            spellCheck={false}
                        />
                    </div>
                )}

                {/* Test Panel */}
                {activeTab === 'test' && (
                    <div className="flex-1 flex">
                        <div className="w-1/2 p-4 border-r border-surface-2 overflow-y-auto">
                            <h3 className="text-xs text-foreground mb-3">Test Inputs</h3>
                            {component.inputs.map(input => (
                                <div key={input.id} className="mb-3">
                                    <label className="block text-[10px] text-slate-500 mb-1">{input.name}</label>
                                    <input
                                        type={input.type === 'number' ? 'number' : 'text'}
                                        value={testInputs[input.name] || ''}
                                        onChange={(e) => setTestInputs(p => ({ ...p, [input.name]: e.target.value }))}
                                        className="w-full px-3 py-2 bg-input border border-border rounded text-sm text-foreground"
                                    />
                                </div>
                            ))}
                            <button
                                onClick={runTest}
                                disabled={isRunning}
                                className="w-full mt-2 flex items-center justify-center gap-2 py-2.5 bg-blue-600 hover:bg-blue-700 text-white text-xs rounded"
                            >
                                {isRunning ? <RefreshCw className="w-4 h-4 animate-spin" /> : <Play className="w-4 h-4" />}
                                {isRunning ? 'Running...' : 'Run Test'}
                            </button>
                        </div>
                        <div className="w-1/2 p-4 bg-muted overflow-y-auto">
                            <h3 className="text-xs text-foreground mb-3">Output</h3>
                            {testOutput ? (
                                <div className="space-y-2">
                                    <div className="flex items-center gap-2 text-[10px]">
                                        <Check className="w-3 h-3 text-green-400" />
                                        <span className="text-green-400">Success</span>
                                        <span className="text-slate-600">•</span>
                                        <span className="text-slate-500">{testOutput.time}</span>
                                    </div>
                                    <pre className="p-3 bg-editor rounded text-[11px] text-foreground font-mono overflow-x-auto">
                                        {JSON.stringify(testOutput.data, null, 2)}
                                    </pre>
                                </div>
                            ) : (
                                <div className="text-muted-foreground text-sm text-center py-8">Run test to see output</div>
                            )}
                        </div>
                    </div>
                )}

                {/* Manifest Preview */}
                {activeTab === 'manifest' && (
                    <div className="flex-1 flex flex-col">
                        <div className="px-4 py-2 bg-muted border-b border-surface-2 flex items-center justify-between">
                            <span className="text-[11px] text-slate-500">component.json (Auto-generated)</span>
                            <button
                                onClick={copyManifest}
                                className="flex items-center gap-1 px-2 py-1 bg-muted hover:bg-accent text-foreground text-[10px] rounded"
                            >
                                {copied ? <Check className="w-3 h-3 text-green-400" /> : <Copy className="w-3 h-3" />}
                                {copied ? 'Copied!' : 'Copy'}
                            </button>
                        </div>
                        <pre className="flex-1 bg-editor text-foreground font-mono text-[11px] p-4 overflow-auto">
                            {JSON.stringify(generateManifest(), null, 2)}
                        </pre>
                    </div>
                )}
            </div>
        </div >
    );
}
