/**
 * Property Panel
 * 
 * 선택된 필드의 속성 편집 (x-ui, x-enum 등)
 */

import { useDesignerStore, selectSelectedField } from '../stores/designerStore';
import { Input } from '@/components/ui/input';
import { Label } from '@/components/ui/label';
import { Checkbox } from '@/components/ui/checkbox';
import { Button } from '@/components/ui/button';
import { Separator } from '@/components/ui/separator';
import { ScrollArea } from '@/components/ui/scroll-area';
import { Plus, Trash2 } from 'lucide-react';
import { useState } from 'react';
import type { DesignedField } from '../types/designer';

// ============================================================================
// No Selection State
// ============================================================================

function NoSelectionState() {
    return (
        <div className="flex-1 flex flex-col items-center justify-center text-center p-6">
            <div className="text-3xl mb-3">👆</div>
            <h3 className="text-sm font-medium text-zinc-400 mb-1">
                No Field Selected
            </h3>
            <p className="text-xs text-zinc-600">
                Click on a field in the canvas to edit its properties
            </p>
        </div>
    );
}

// ============================================================================
// Section Header
// ============================================================================

function SectionHeader({ icon, title }: { icon: string; title: string }) {
    return (
        <div className="flex items-center gap-2 py-2">
            <span className="text-sm">{icon}</span>
            <span className="text-xs font-semibold text-zinc-400 uppercase tracking-wider">
                {title}
            </span>
        </div>
    );
}

// ============================================================================
// Enum Editor
// ============================================================================

interface EnumEditorProps {
    values: (string | number)[];
    labels?: Record<string, string>;
    onChange: (values: (string | number)[], labels?: Record<string, string>) => void;
}

function EnumEditor({ values, labels = {}, onChange }: EnumEditorProps) {
    const [newValue, setNewValue] = useState('');

    const handleAddValue = () => {
        if (newValue.trim()) {
            onChange([...values, newValue.trim()], labels);
            setNewValue('');
        }
    };

    const handleRemoveValue = (index: number) => {
        const newValues = values.filter((_, i) => i !== index);
        const newLabels = { ...labels };
        delete newLabels[String(values[index])];
        onChange(newValues, newLabels);
    };

    const handleLabelChange = (value: string | number, label: string) => {
        onChange(values, { ...labels, [String(value)]: label });
    };

    return (
        <div className="space-y-2">
            {values.map((value, index) => (
                <div key={index} className="flex items-center gap-2">
                    <Input
                        value={String(value)}
                        onChange={(e) => {
                            const newValues = [...values];
                            newValues[index] = e.target.value;
                            onChange(newValues, labels);
                        }}
                        className="flex-1 h-8 text-xs bg-zinc-800 border-zinc-700"
                        placeholder="Value"
                    />
                    <Input
                        value={labels[String(value)] || ''}
                        onChange={(e) => handleLabelChange(value, e.target.value)}
                        className="flex-1 h-8 text-xs bg-zinc-800 border-zinc-700"
                        placeholder="Label (optional)"
                    />
                    <Button
                        variant="ghost"
                        size="icon"
                        className="h-8 w-8 text-zinc-500 hover:text-red-400"
                        onClick={() => handleRemoveValue(index)}
                    >
                        <Trash2 size={14} />
                    </Button>
                </div>
            ))}
            <div className="flex items-center gap-2">
                <Input
                    value={newValue}
                    onChange={(e) => setNewValue(e.target.value)}
                    onKeyDown={(e) => e.key === 'Enter' && handleAddValue()}
                    className="flex-1 h-8 text-xs bg-zinc-800 border-zinc-700"
                    placeholder="Add new value..."
                />
                <Button
                    variant="ghost"
                    size="icon"
                    className="h-8 w-8 text-zinc-500 hover:text-cyan-400"
                    onClick={handleAddValue}
                >
                    <Plus size={14} />
                </Button>
            </div>
        </div>
    );
}

// ============================================================================
// VisibleWhen Editor
// ============================================================================

interface VisibleWhenEditorProps {
    value?: Record<string, any[]>;
    availableFields: DesignedField[];
    onChange: (value: Record<string, any[]>) => void;
}

function VisibleWhenEditor({ value = {}, availableFields, onChange }: VisibleWhenEditorProps) {
    const [selectedField, setSelectedField] = useState('');
    const [conditionValues, setConditionValues] = useState('');

    const handleAddCondition = () => {
        if (selectedField && conditionValues.trim()) {
            const values = conditionValues.split(',').map(v => v.trim());
            onChange({ ...value, [selectedField]: values });
            setSelectedField('');
            setConditionValues('');
        }
    };

    const handleRemoveCondition = (fieldKey: string) => {
        const newValue = { ...value };
        delete newValue[fieldKey];
        onChange(newValue);
    };

    return (
        <div className="space-y-2">
            {Object.entries(value).map(([fieldKey, vals]) => (
                <div key={fieldKey} className="flex items-center gap-2 bg-zinc-800 p-2 rounded">
                    <span className="text-xs text-zinc-400">{fieldKey} =</span>
                    <span className="text-xs text-cyan-400">[{vals.join(', ')}]</span>
                    <Button
                        variant="ghost"
                        size="icon"
                        className="h-6 w-6 ml-auto text-zinc-500 hover:text-red-400"
                        onClick={() => handleRemoveCondition(fieldKey)}
                    >
                        <Trash2 size={12} />
                    </Button>
                </div>
            ))}
            <div className="flex flex-col gap-2">
                <select
                    value={selectedField}
                    onChange={(e) => setSelectedField(e.target.value)}
                    className="h-8 text-xs bg-zinc-800 border border-zinc-700 rounded px-2 text-zinc-200"
                >
                    <option value="">Select field...</option>
                    {availableFields
                        .filter(f => f.enum && f.enum.length > 0)
                        .map(f => (
                            <option key={f.id} value={f.key}>{f.key}</option>
                        ))}
                </select>
                <div className="flex gap-2">
                    <Input
                        value={conditionValues}
                        onChange={(e) => setConditionValues(e.target.value)}
                        className="flex-1 h-8 text-xs bg-zinc-800 border-zinc-700"
                        placeholder="Values (comma separated)"
                    />
                    <Button
                        variant="ghost"
                        size="icon"
                        className="h-8 w-8 text-zinc-500 hover:text-cyan-400"
                        onClick={handleAddCondition}
                    >
                        <Plus size={14} />
                    </Button>
                </div>
            </div>
        </div>
    );
}

// ============================================================================
// Property Panel
// ============================================================================

export function PropertyPanel() {
    const store = useDesignerStore();
    const selectedField = selectSelectedField(store);
    const { fields, updateField } = store;

    if (!selectedField) {
        return (
            <div className="h-full flex flex-col bg-zinc-900 border-l border-zinc-700">
                <div className="px-4 py-3 border-b border-zinc-700">
                    <h3 className="text-sm font-semibold text-zinc-200 flex items-center gap-2">
                        <span>⚙️</span>
                        Properties
                    </h3>
                </div>
                <NoSelectionState />
            </div>
        );
    }

    const update = (updates: Partial<DesignedField>) => {
        updateField(selectedField.id, updates);
    };

    const updateXUI = (xuiUpdates: Partial<NonNullable<DesignedField['xui']>>) => {
        update({
            xui: { ...selectedField.xui, ...xuiUpdates },
        });
    };

    return (
        <div className="h-full flex flex-col bg-zinc-900 border-l border-zinc-700">
            {/* Header */}
            <div className="px-4 py-3 border-b border-zinc-700">
                <h3 className="text-sm font-semibold text-zinc-200 flex items-center gap-2">
                    <span>⚙️</span>
                    Properties
                </h3>
                <p className="text-xs text-zinc-500 mt-0.5">
                    Editing: <span className="text-cyan-400">{selectedField.key}</span>
                </p>
            </div>

            {/* Content */}
            <ScrollArea className="flex-1">
                <div className="p-4 space-y-4">
                    {/* Basic Info */}
                    <div>
                        <SectionHeader icon="📋" title="Basic" />
                        <div className="space-y-3">
                            <div>
                                <Label className="text-xs text-zinc-400">Key (Property Name)</Label>
                                <Input
                                    value={selectedField.key}
                                    onChange={(e) => update({ key: e.target.value })}
                                    className="h-8 mt-1 text-sm bg-zinc-800 border-zinc-700"
                                />
                            </div>
                            <div className="flex items-center gap-2">
                                <Checkbox
                                    id="required"
                                    checked={selectedField.required || false}
                                    onCheckedChange={(checked) => update({ required: !!checked })}
                                />
                                <Label htmlFor="required" className="text-xs text-zinc-300">
                                    Required
                                </Label>
                            </div>
                            <div>
                                <Label className="text-xs text-zinc-400">Default Value</Label>
                                <Input
                                    value={selectedField.default ?? ''}
                                    onChange={(e) => update({ default: e.target.value || undefined })}
                                    className="h-8 mt-1 text-sm bg-zinc-800 border-zinc-700"
                                    placeholder="No default"
                                />
                            </div>
                        </div>
                    </div>

                    <Separator className="bg-zinc-700" />

                    {/* x-ui Section */}
                    <div>
                        <SectionHeader icon="🎨" title="x-ui (Display)" />
                        <div className="space-y-3">
                            <div>
                                <Label className="text-xs text-zinc-400">Label</Label>
                                <Input
                                    value={selectedField.xui?.label || ''}
                                    onChange={(e) => updateXUI({ label: e.target.value || undefined })}
                                    className="h-8 mt-1 text-sm bg-zinc-800 border-zinc-700"
                                    placeholder="Display label"
                                />
                            </div>
                            <div>
                                <Label className="text-xs text-zinc-400">Group (Section)</Label>
                                <Input
                                    value={selectedField.xui?.group || ''}
                                    onChange={(e) => updateXUI({ group: e.target.value || undefined })}
                                    className="h-8 mt-1 text-sm bg-zinc-800 border-zinc-700"
                                    placeholder="e.g., General, Advanced"
                                />
                            </div>
                            <div>
                                <Label className="text-xs text-zinc-400">Hint</Label>
                                <Input
                                    value={selectedField.xui?.hint || ''}
                                    onChange={(e) => updateXUI({ hint: e.target.value || undefined })}
                                    className="h-8 mt-1 text-sm bg-zinc-800 border-zinc-700"
                                    placeholder="Help text"
                                />
                            </div>
                            <div>
                                <Label className="text-xs text-zinc-400">Layout Position</Label>
                                <div className="mt-1 p-2 bg-zinc-800 rounded border border-zinc-700">
                                    <div className="text-xs text-zinc-300">
                                        {selectedField.layout ? (
                                            <>
                                                <span className="text-zinc-500">Row:</span>{' '}
                                                <span className="font-mono text-cyan-400">
                                                    {selectedField.layout.rowId?.slice(-4) || 'N/A'}
                                                </span>
                                                <span className="mx-2 text-zinc-600">|</span>
                                                <span className="text-zinc-500">Column:</span>{' '}
                                                <span className="font-mono text-cyan-400">
                                                    {(selectedField.layout.columnIndex ?? 0) + 1}
                                                </span>
                                            </>
                                        ) : (
                                            <span className="text-zinc-500">Not positioned</span>
                                        )}
                                    </div>
                                </div>
                                <p className="text-[10px] text-zinc-600 mt-1">
                                    Drag fields to reorder rows or columns
                                </p>
                            </div>
                        </div>
                    </div>

                    <Separator className="bg-zinc-700" />

                    {/* Enum Section (for enum type) */}
                    {(selectedField.type === 'enum' || selectedField.type === 'string') && (
                        <>
                            <div>
                                <SectionHeader icon="📋" title="Enum Values" />
                                <EnumEditor
                                    values={selectedField.enum || []}
                                    labels={selectedField.xenumLabels}
                                    onChange={(values, labels) => {
                                        update({
                                            enum: values.length > 0 ? values : undefined,
                                            xenumLabels: labels && Object.keys(labels).length > 0 ? labels : undefined,
                                            type: values.length > 0 ? 'enum' : 'string',
                                        });
                                    }}
                                />
                            </div>
                            <Separator className="bg-zinc-700" />
                        </>
                    )}

                    {/* Validation Section */}
                    {(selectedField.type === 'string' || selectedField.type === 'enum') && (
                        <>
                            <div>
                                <SectionHeader icon="✅" title="Validation (String)" />
                                <div className="space-y-3">
                                    <div className="grid grid-cols-2 gap-2">
                                        <div>
                                            <Label className="text-xs text-zinc-400">Min Length</Label>
                                            <Input
                                                type="number"
                                                value={selectedField.minLength ?? ''}
                                                onChange={(e) => update({
                                                    minLength: e.target.value ? Number(e.target.value) : undefined
                                                })}
                                                className="h-8 mt-1 text-sm bg-zinc-800 border-zinc-700"
                                            />
                                        </div>
                                        <div>
                                            <Label className="text-xs text-zinc-400">Max Length</Label>
                                            <Input
                                                type="number"
                                                value={selectedField.maxLength ?? ''}
                                                onChange={(e) => update({
                                                    maxLength: e.target.value ? Number(e.target.value) : undefined
                                                })}
                                                className="h-8 mt-1 text-sm bg-zinc-800 border-zinc-700"
                                            />
                                        </div>
                                    </div>
                                    <div>
                                        <Label className="text-xs text-zinc-400">Pattern (Regex)</Label>
                                        <Input
                                            value={selectedField.pattern || ''}
                                            onChange={(e) => update({ pattern: e.target.value || undefined })}
                                            className="h-8 mt-1 text-sm bg-zinc-800 border-zinc-700 font-mono"
                                            placeholder="^[a-zA-Z]+$"
                                        />
                                    </div>
                                </div>
                            </div>
                            <Separator className="bg-zinc-700" />
                        </>
                    )}

                    {(selectedField.type === 'number' || selectedField.type === 'integer') && (
                        <>
                            <div>
                                <SectionHeader icon="✅" title="Validation (Number)" />
                                <div className="grid grid-cols-2 gap-2">
                                    <div>
                                        <Label className="text-xs text-zinc-400">Minimum</Label>
                                        <Input
                                            type="number"
                                            value={selectedField.minimum ?? ''}
                                            onChange={(e) => update({
                                                minimum: e.target.value ? Number(e.target.value) : undefined
                                            })}
                                            className="h-8 mt-1 text-sm bg-zinc-800 border-zinc-700"
                                        />
                                    </div>
                                    <div>
                                        <Label className="text-xs text-zinc-400">Maximum</Label>
                                        <Input
                                            type="number"
                                            value={selectedField.maximum ?? ''}
                                            onChange={(e) => update({
                                                maximum: e.target.value ? Number(e.target.value) : undefined
                                            })}
                                            className="h-8 mt-1 text-sm bg-zinc-800 border-zinc-700"
                                        />
                                    </div>
                                </div>
                            </div>
                            <Separator className="bg-zinc-700" />
                        </>
                    )}

                    {/* visibleWhen Section */}
                    <div>
                        <SectionHeader icon="👁️" title="visibleWhen" />
                        <VisibleWhenEditor
                            value={selectedField.xui?.visibleWhen}
                            availableFields={fields.filter(f => f.id !== selectedField.id)}
                            onChange={(visibleWhen) => updateXUI({ visibleWhen })}
                        />
                    </div>
                </div>
            </ScrollArea>
        </div>
    );
}
