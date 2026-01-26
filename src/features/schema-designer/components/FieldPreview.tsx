/**
 * Field Preview Component
 * 
 * 필드 타입별 실제 UI 미리보기
 */

import { Input } from '@/components/ui/input';
import { Label } from '@/components/ui/label';
import { Checkbox } from '@/components/ui/checkbox';
import type { DesignedField } from '../types/designer';

interface FieldPreviewProps {
    field: DesignedField;
}

export function FieldPreview({ field }: FieldPreviewProps) {
    const label = field.xui?.label || field.key;
    const hint = field.xui?.hint;
    const widget = field.xui?.widget;

    // String with enum = Dropdown
    if (field.type === 'string' && field.enum) {
        // Radio buttons
        if (widget === 'radio') {
            return (
                <div className="space-y-2">
                    <Label className="text-sm text-zinc-300">{label}</Label>
                    {field.enum.map((val) => (
                        <div key={String(val)} className="flex items-center gap-2">
                            <div className="w-4 h-4 rounded-full border-2 border-zinc-500 bg-zinc-700" />
                            <span className="text-sm text-zinc-300">
                                {field.xenumLabels?.[String(val)] || String(val)}
                            </span>
                        </div>
                    ))}
                    {hint && <p className="text-xs text-zinc-500 mt-1">{hint}</p>}
                </div>
            );
        }

        // Dropdown (default)
        return (
            <div className="space-y-1">
                <Label className="text-sm text-zinc-300">{label}</Label>
                <div className="w-full px-3 py-2 bg-zinc-700 border border-zinc-600 rounded text-sm text-zinc-400 flex items-center justify-between">
                    <span>Select...</span>
                    <span className="text-xs">▼</span>
                </div>
                {hint && <p className="text-xs text-zinc-500">{hint}</p>}
            </div>
        );
    }

    // Enum type = Dropdown or Radio
    if (field.type === 'enum') {
        // Radio buttons
        if (widget === 'radio') {
            return (
                <div className="space-y-2">
                    <Label className="text-sm text-zinc-300">{label}</Label>
                    {field.enum?.map((val) => (
                        <div key={String(val)} className="flex items-center gap-2">
                            <div className="w-4 h-4 rounded-full border-2 border-zinc-500 bg-zinc-700" />
                            <span className="text-sm text-zinc-300">
                                {field.xenumLabels?.[String(val)] || String(val)}
                            </span>
                        </div>
                    ))}
                    {hint && <p className="text-xs text-zinc-500 mt-1">{hint}</p>}
                </div>
            );
        }

        // Dropdown (default)
        return (
            <div className="space-y-1">
                <Label className="text-sm text-zinc-300">{label}</Label>
                <div className="w-full px-3 py-2 bg-zinc-700 border border-zinc-600 rounded text-sm text-zinc-400 flex items-center justify-between">
                    <span>Select...</span>
                    <span className="text-xs">▼</span>
                </div>
                {hint && <p className="text-xs text-zinc-500">{hint}</p>}
            </div>
        );
    }

    // String type
    if (field.type === 'string') {
        // Textarea
        if (widget === 'textarea') {
            return (
                <div className="space-y-1">
                    <Label className="text-sm text-zinc-300">{label}</Label>
                    <div className="w-full px-3 py-2 bg-zinc-700 border border-zinc-600 rounded text-sm text-zinc-500 min-h-[80px]">
                        {hint || `Enter ${label}...`}
                    </div>
                    {hint && <p className="text-xs text-zinc-500">{hint}</p>}
                </div>
            );
        }

        // Date picker
        if (widget === 'date') {
            return (
                <div className="space-y-1">
                    <Label className="text-sm text-zinc-300">{label}</Label>
                    <div className="w-full px-3 py-2 bg-zinc-700 border border-zinc-600 rounded text-sm text-zinc-400 flex items-center justify-between">
                        <span>Select date...</span>
                        <span className="text-xs">📅</span>
                    </div>
                    {hint && <p className="text-xs text-zinc-500">{hint}</p>}
                </div>
            );
        }

        // Text input (default)
        return (
            <div className="space-y-1">
                <Label className="text-sm text-zinc-300">{label}</Label>
                <Input
                    type="text"
                    placeholder={hint || `Enter ${label}...`}
                    className="bg-zinc-700 border-zinc-600 text-zinc-300 placeholder:text-zinc-500"
                    disabled
                />
                {hint && <p className="text-xs text-zinc-500">{hint}</p>}
            </div>
        );
    }

    // Number / Integer
    if (field.type === 'number' || field.type === 'integer') {
        return (
            <div className="space-y-1">
                <Label className="text-sm text-zinc-300">{label}</Label>
                <Input
                    type="number"
                    placeholder={hint || '0'}
                    className="bg-zinc-700 border-zinc-600 text-zinc-300 placeholder:text-zinc-500"
                    disabled
                />
                {hint && <p className="text-xs text-zinc-500">{hint}</p>}
            </div>
        );
    }

    // Boolean
    if (field.type === 'boolean') {
        return (
            <div className="flex items-center gap-2 py-2">
                <Checkbox className="border-zinc-500 data-[state=checked]:bg-cyan-600" disabled />
                <Label className="text-sm text-zinc-300">{label}</Label>
            </div>
        );
    }

    // Array
    if (field.type === 'array') {
        return (
            <div className="space-y-1">
                <Label className="text-sm text-zinc-300">{label}</Label>
                <div className="w-full px-3 py-2 bg-zinc-700 border border-zinc-600 rounded text-sm text-zinc-500">
                    <div className="flex items-center gap-2">
                        <span>Array of {field.items?.type || 'items'}</span>
                        <button className="ml-auto text-xs text-cyan-400">+ Add</button>
                    </div>
                </div>
                {hint && <p className="text-xs text-zinc-500">{hint}</p>}
            </div>
        );
    }

    // Object
    if (field.type === 'object') {
        return (
            <div className="space-y-1">
                <Label className="text-sm text-zinc-300">{label}</Label>
                <div className="w-full px-3 py-2 bg-zinc-700 border border-zinc-600 rounded text-sm text-zinc-500">
                    <span>Object with {field.children?.length || 0} properties</span>
                </div>
                {hint && <p className="text-xs text-zinc-500">{hint}</p>}
            </div>
        );
    }

    // Default fallback
    return (
        <div className="space-y-1">
            <Label className="text-sm text-zinc-300">{label}</Label>
            <div className="text-xs text-zinc-500 py-2">
                {field.type} field
            </div>
        </div>
    );
}
