/**
 * Schema Builder Page
 * 
 * 독립 Schema Builder 페이지 - 2패널 레이아웃 (Builder | JSON Viewer)
 */

import { useState, useEffect, useCallback } from 'react';
import { ResizablePanelGroup, ResizablePanel, ResizableHandle } from '@/components/ui/resizable';
import { Button } from '@/components/ui/button';
import { CodeEditor } from '@/components/common';
import { Copy, Download, Upload, Trash2 } from 'lucide-react';
import { toast } from 'sonner';
import { DndContext, DragEndEvent, PointerSensor, useSensor, useSensors } from '@dnd-kit/core';
import { useDesignerStore } from '../../schema-designer/stores/designerStore';
import { ComponentPalette } from '../../schema-designer/components/ComponentPalette';
import { CanvasArea } from '../../schema-designer/components/CanvasArea';
import { PropertyPanel } from '../../schema-designer/components/PropertyPanel';
import { layoutToSchemaString } from '../../schema-designer/lib/layoutToSchema';
import { parseSchemaToLayout } from '../../schema-designer/lib/schemaToLayout';
import type { DesignedField, FieldType } from '../../schema-designer/types/designer';

// ============================================================================
// Helper Functions
// ============================================================================

function generateId(): string {
    return `field_${Date.now()}_${Math.random().toString(36).substr(2, 9)}`;
}

function createDefaultField(type: FieldType | 'section'): DesignedField {
    const id = generateId();
    const baseKey = type === 'section' ? 'Section' : type.toUpperCase();

    const field: DesignedField = {
        id,
        key: `${baseKey}_${id.slice(-4)}`,
        type: type === 'section' ? 'string' : type === 'enum' ? 'string' : type,
    };

    if (type === 'section') {
        field.xui = { group: 'New Section' };
    }

    if (type === 'enum') {
        field.type = 'enum';
        field.enum = ['Option1', 'Option2', 'Option3'];
        field.xenumLabels = {
            'Option1': 'Option 1',
            'Option2': 'Option 2',
            'Option3': 'Option 3',
        };
    }

    if (type === 'object') {
        field.children = [];
    }

    if (type === 'array') {
        field.items = { type: 'string' };
    }

    return field;
}

// ============================================================================
// Schema Builder Page
// ============================================================================

export function SchemaBuilderPage() {
    const {
        fields,
        transport,
        addField,
        setFields,
        updateTransport,
        loadConfig,
        reorderFields,
        reset,
    } = useDesignerStore();

    const [jsonText, setJsonText] = useState('');
    const [syncDirection, setSyncDirection] = useState<'builder' | 'json'>('builder');

    // Load config on mount
    useEffect(() => {
        loadConfig();
    }, [loadConfig]);

    // Builder → JSON (양방향 동기화)
    useEffect(() => {
        if (syncDirection === 'builder') {
            const schema = layoutToSchemaString(fields, transport);
            setJsonText(schema);
        }
    }, [fields, transport, syncDirection]);

    // JSON → Builder (양방향 동기화)
    const handleJsonChange = useCallback((newJson: string) => {
        setSyncDirection('json');
        setJsonText(newJson);

        try {
            const parsed = parseSchemaToLayout(newJson);
            if (parsed) {
                setFields(parsed.fields);
                if (parsed.transport) {
                    updateTransport(parsed.transport);
                }
                setSyncDirection('builder');
            }
        } catch (error) {
            console.error('Invalid JSON:', error);
        }
    }, [setFields, updateTransport]);

    // DnD Sensors
    const sensors = useSensors(
        useSensor(PointerSensor, {
            activationConstraint: {
                distance: 5,
            },
        })
    );

    // Handle drag end
    const handleDragEnd = useCallback((event: DragEndEvent) => {
        const { active, over } = event;

        if (!over) return;

        const activeData = active.data.current;
        const overData = over.data.current;

        // Palette → Canvas (new field)
        if (activeData?.type === 'palette-item' && overData?.type === 'canvas') {
            const fieldType = activeData.fieldType as FieldType | 'section';
            const newField = createDefaultField(fieldType);
            addField(newField);
            setSyncDirection('builder');
            return;
        }

        // Reorder within canvas
        if (activeData?.type === 'field' && active.id !== over.id) {
            const oldIndex = fields.findIndex((f: DesignedField) => f.id === active.id);
            const newIndex = fields.findIndex((f: DesignedField) => f.id === over.id);

            if (oldIndex !== -1 && newIndex !== -1) {
                reorderFields(oldIndex, newIndex);
                setSyncDirection('builder');
            }
        }
    }, [fields, addField, reorderFields]);

    // Copy JSON to clipboard
    const handleCopyJson = () => {
        navigator.clipboard.writeText(jsonText);
        toast.success('JSON copied to clipboard');
    };

    // Download JSON
    const handleDownloadJson = () => {
        const blob = new Blob([jsonText], { type: 'application/json' });
        const url = URL.createObjectURL(blob);
        const a = document.createElement('a');
        a.href = url;
        a.download = 'schema.json';
        a.click();
        URL.revokeObjectURL(url);
        toast.success('Schema downloaded');
    };

    // Upload JSON
    const handleUploadJson = () => {
        const input = document.createElement('input');
        input.type = 'file';
        input.accept = '.json';
        input.onchange = (e) => {
            const file = (e.target as HTMLInputElement).files?.[0];
            if (file) {
                const reader = new FileReader();
                reader.onload = (e) => {
                    const content = e.target?.result as string;
                    handleJsonChange(content);
                    toast.success('Schema loaded');
                };
                reader.readAsText(file);
            }
        };
        input.click();
    };

    // Clear all
    const handleClear = () => {
        if (confirm('Clear all fields? This cannot be undone.')) {
            reset();
            setJsonText('');
            toast.success('Schema cleared');
        }
    };

    return (
        <div className="h-full w-full flex flex-col bg-zinc-950">
            {/* Header */}
            <div className="flex-shrink-0 bg-zinc-900 border-b border-zinc-800 px-4 py-3">
                <div className="flex items-center justify-between">
                    <div>
                        <h1 className="text-lg font-semibold text-zinc-200 flex items-center gap-2">
                            <span>🎨</span>
                            Schema Builder
                        </h1>
                        <p className="text-xs text-zinc-500 mt-0.5">
                            Build JSON Schemas visually with drag & drop
                        </p>
                    </div>

                    <div className="flex items-center gap-2">
                        <Button
                            variant="outline"
                            size="sm"
                            onClick={handleCopyJson}
                            className="h-8 px-3 text-xs"
                        >
                            <Copy size={14} className="mr-1" />
                            Copy JSON
                        </Button>
                        <Button
                            variant="outline"
                            size="sm"
                            onClick={handleDownloadJson}
                            className="h-8 px-3 text-xs"
                        >
                            <Download size={14} className="mr-1" />
                            Download
                        </Button>
                        <Button
                            variant="outline"
                            size="sm"
                            onClick={handleUploadJson}
                            className="h-8 px-3 text-xs"
                        >
                            <Upload size={14} className="mr-1" />
                            Upload
                        </Button>
                        <Button
                            variant="outline"
                            size="sm"
                            onClick={handleClear}
                            className="h-8 px-3 text-xs text-red-400 hover:text-red-300"
                        >
                            <Trash2 size={14} className="mr-1" />
                            Clear
                        </Button>
                    </div>
                </div>
            </div>

            {/* Main Content: 2-Panel Layout */}
            <ResizablePanelGroup direction="horizontal" className="flex-1">
                {/* Left Panel: Visual Builder */}
                <ResizablePanel defaultSize={50} minSize={30}>
                    <DndContext
                        sensors={sensors}
                        onDragEnd={handleDragEnd}
                    >
                        <div className="h-full flex flex-col bg-zinc-950">
                            <ResizablePanelGroup direction="horizontal" className="flex-1">
                                {/* Component Palette */}
                                <ResizablePanel defaultSize={25} minSize={20} maxSize={35}>
                                    <ComponentPalette />
                                </ResizablePanel>

                                <ResizableHandle className="w-1 bg-zinc-800 hover:bg-cyan-600 transition-colors" />

                                {/* Canvas */}
                                <ResizablePanel defaultSize={50} minSize={40}>
                                    <CanvasArea />
                                </ResizablePanel>

                                <ResizableHandle className="w-1 bg-zinc-800 hover:bg-cyan-600 transition-colors" />

                                {/* Property Panel */}
                                <ResizablePanel defaultSize={25} minSize={20} maxSize={35}>
                                    <PropertyPanel />
                                </ResizablePanel>
                            </ResizablePanelGroup>
                        </div>
                    </DndContext>
                </ResizablePanel>

                <ResizableHandle className="w-2 bg-zinc-800 hover:bg-cyan-600 transition-colors" />

                {/* Right Panel: JSON Viewer */}
                <ResizablePanel defaultSize={50} minSize={30}>
                    <div className="h-full flex flex-col bg-zinc-950">
                        <div className="p-4 border-b border-zinc-800 bg-zinc-900 flex-shrink-0">
                            <h3 className="text-sm font-medium text-zinc-200">JSON Schema</h3>
                            <p className="text-xs text-zinc-500 mt-0.5">
                                Edit JSON directly - changes sync to builder
                            </p>
                        </div>

                        <div className="flex-1 relative">
                            <CodeEditor
                                value={jsonText}
                                onChange={(value) => handleJsonChange(value || '')}
                                language="json"
                                minimap={true}
                            />
                        </div>
                    </div>
                </ResizablePanel>
            </ResizablePanelGroup>
        </div>
    );
}
