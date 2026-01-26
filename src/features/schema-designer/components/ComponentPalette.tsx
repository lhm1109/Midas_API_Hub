/**
 * Component Palette
 * 
 * 드래그 가능한 위젯 목록 (YAML fieldTypeMapping 기반)
 */

import { useDraggable } from '@dnd-kit/core';
import { CSS } from '@dnd-kit/utilities';
import { getWidgetPalette } from '../lib/yamlRulesLoader';
import type { WidgetPaletteItem } from '../types/designer';

// ============================================================================
// Draggable Item
// ============================================================================

interface DraggablePaletteItemProps {
    item: WidgetPaletteItem;
}

function DraggablePaletteItem({ item }: DraggablePaletteItemProps) {
    const { attributes, listeners, setNodeRef, transform, isDragging } = useDraggable({
        id: `palette-${item.type}`,
        data: {
            type: 'palette-item',
            fieldType: item.type,
            item,
        },
    });

    const style = {
        transform: CSS.Translate.toString(transform),
        opacity: isDragging ? 0.5 : 1,
    };

    return (
        <div
            ref={setNodeRef}
            style={style}
            {...listeners}
            {...attributes}
            className={`
        flex items-center gap-2 px-3 py-2 rounded-md cursor-grab
        bg-zinc-800 border border-zinc-700
        hover:bg-zinc-700 hover:border-zinc-600
        transition-colors duration-150
        ${isDragging ? 'ring-2 ring-cyan-500' : ''}
      `}
        >
            <span className="text-lg">{item.icon}</span>
            <span className="text-sm text-zinc-200">{item.label}</span>
        </div>
    );
}

// ============================================================================
// Component Palette
// ============================================================================

export function ComponentPalette() {
    const palette = getWidgetPalette();

    // 그룹핑: 기본 타입, 컨테이너, 레이아웃
    const basicTypes = palette.filter(p =>
        ['string', 'number', 'integer', 'boolean', 'enum'].includes(p.type)
    );
    const containerTypes = palette.filter(p =>
        ['object', 'array'].includes(p.type)
    );
    const layoutTypes = palette.filter(p =>
        p.type === 'section'
    );

    return (
        <div className="h-full flex flex-col bg-zinc-900 border-r border-zinc-700">
            {/* Header */}
            <div className="px-4 py-3 border-b border-zinc-700">
                <h3 className="text-sm font-semibold text-zinc-200 flex items-center gap-2">
                    <span>🎨</span>
                    Components
                </h3>
                <p className="text-xs text-zinc-500 mt-1">Drag to canvas</p>
            </div>

            {/* Palette Items */}
            <div className="flex-1 overflow-y-auto p-3 space-y-4">
                {/* Basic Types */}
                <div>
                    <div className="text-xs font-medium text-zinc-500 uppercase tracking-wider mb-2">
                        Basic Fields
                    </div>
                    <div className="space-y-1.5">
                        {basicTypes.map(item => (
                            <DraggablePaletteItem key={item.type} item={item} />
                        ))}
                    </div>
                </div>

                {/* Containers */}
                <div>
                    <div className="text-xs font-medium text-zinc-500 uppercase tracking-wider mb-2">
                        Containers
                    </div>
                    <div className="space-y-1.5">
                        {containerTypes.map(item => (
                            <DraggablePaletteItem key={item.type} item={item} />
                        ))}
                    </div>
                </div>

                {/* Layout */}
                <div>
                    <div className="text-xs font-medium text-zinc-500 uppercase tracking-wider mb-2">
                        Layout
                    </div>
                    <div className="space-y-1.5">
                        {layoutTypes.map(item => (
                            <DraggablePaletteItem key={item.type} item={item} />
                        ))}
                    </div>
                </div>
            </div>

            {/* Help */}
            <div className="px-4 py-3 border-t border-zinc-700 bg-zinc-800/50">
                <p className="text-xs text-zinc-500">
                    💡 Drag widgets to the canvas to build your schema
                </p>
            </div>
        </div>
    );
}
