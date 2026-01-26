/**
 * Designed Field
 * 
 * 캔버스에 배치된 개별 필드 컴포넌트
 * useDraggable 사용하여 행 간 이동 가능
 */

import { useDraggable } from '@dnd-kit/core';
import { CSS } from '@dnd-kit/utilities';
import { GripVertical, Trash2, ChevronDown, ChevronRight } from 'lucide-react';
import { useState, useEffect } from 'react';
import { useDesignerStore } from '../stores/designerStore';
import { FieldPreview } from './FieldPreview';
import type { DesignedField as DesignedFieldType } from '../types/designer';

// ============================================================================
// Designed Field
// ============================================================================

interface DesignedFieldProps {
    field: DesignedFieldType;
    depth?: number;
}

export function DesignedField({ field, depth = 0 }: DesignedFieldProps) {
    const { selectedFieldId, selectField, removeField } = useDesignerStore();
    const [isExpanded, setIsExpanded] = useState(true);

    const isSelected = selectedFieldId === field.id;
    const hasChildren = field.children && field.children.length > 0;

    // useDraggable - 전체 카드를 드래그 가능
    const {
        attributes,
        listeners,
        setNodeRef,
        transform,
        isDragging,
    } = useDraggable({
        id: field.id,
        data: {
            type: 'field',
            field,
        },
    });

    // 디버깅: useDraggable 반환값 확인
    useEffect(() => {
        console.log('🔧 useDraggable setup:', {
            fieldId: field.id,
            hasListeners: !!listeners,
            hasAttributes: !!attributes,
            hasSetNodeRef: !!setNodeRef,
            listenerKeys: listeners ? Object.keys(listeners) : [],
            listeners: listeners, // 전체 객체 출력
        });
    }, [field.id, listeners, attributes, setNodeRef]);

    // 디버깅: 드래그 상태 확인 (렌더링 시)
    useEffect(() => {
        if (isDragging) {
            console.log('🟠 Field is being dragged:', field.id);
        }
    }, [isDragging, field.id]);

    const style = {
        transform: CSS.Translate.toString(transform),
        marginLeft: depth * 16,
    };

    const handleClick = (e: React.MouseEvent) => {
        // 드래그 중이 아닐 때만 선택
        if (!isDragging) {
            e.stopPropagation();
            selectField(field.id);
        }
    };

    const handleDelete = (e: React.MouseEvent) => {
        e.stopPropagation();
        e.preventDefault();
        removeField(field.id);
    };

    const handleToggleExpand = (e: React.MouseEvent) => {
        e.stopPropagation();
        setIsExpanded(!isExpanded);
    };

    return (
        <div 
            style={style}
            className="relative"
        >
            {/* Draggable Field Card */}
            <div
                onClick={handleClick}
                className={`
                    group relative bg-zinc-800/80 border-2 rounded-lg p-3 
                    transition-all duration-150 
                    cursor-pointer
                    ${isDragging ? 'shadow-2xl shadow-cyan-500/50 ring-4 ring-cyan-500 scale-110 opacity-50' : ''}
                    ${isSelected
                        ? 'border-cyan-500 ring-2 ring-cyan-500/50 shadow-lg shadow-cyan-500/20'
                        : 'border-zinc-700 hover:border-zinc-600 hover:shadow-md'
                    }
                `}
            >
                {/* Drag Handle Icon - setNodeRef, listeners, attributes 모두 여기에! */}
                <div 
                    ref={setNodeRef}
                    {...listeners}
                    {...attributes}
                    className="absolute top-2 left-2 text-zinc-500 hover:text-cyan-400 transition-colors z-20 cursor-grab active:cursor-grabbing p-2 rounded hover:bg-zinc-700/50"
                    title="Drag to move"
                >
                    <GripVertical size={16} />
                </div>

                {/* Delete Button */}
                <button
                    onClick={handleDelete}
                    onMouseDown={(e) => e.stopPropagation()}
                    className="absolute top-2 right-2 opacity-0 group-hover:opacity-100 transition-opacity hover:bg-zinc-700 p-1 rounded bg-zinc-800/90 text-zinc-400 hover:text-red-400 z-30"
                >
                    <Trash2 size={14} />
                </button>

                {/* Expand/Collapse for nested fields */}
                {hasChildren && (
                    <button
                        onClick={handleToggleExpand}
                        onMouseDown={(e) => e.stopPropagation()}
                        className="absolute top-2 left-8 opacity-0 group-hover:opacity-100 transition-opacity hover:bg-zinc-700 p-1 rounded bg-zinc-800/90 text-zinc-400 hover:text-cyan-400 z-30"
                    >
                        {isExpanded ? <ChevronDown size={14} /> : <ChevronRight size={14} />}
                    </button>
                )}

                {/* Required Badge */}
                {field.required && (
                    <div className="absolute top-2 left-8 bg-red-500/20 text-red-400 text-xs px-1.5 py-0.5 rounded pointer-events-none">
                        Required
                    </div>
                )}

                {/* UI Preview - Main content */}
                <div className="ml-6">
                    <FieldPreview field={field} />
                </div>

                {/* Field Key Badge */}
                <div className="absolute bottom-2 right-2 opacity-0 group-hover:opacity-60 transition-opacity pointer-events-none">
                    <span className="text-[10px] font-mono text-zinc-500 bg-zinc-900/50 px-1.5 py-0.5 rounded">
                        {field.key}
                    </span>
                </div>
            </div>

            {/* Children (Nested Fields) */}
            {hasChildren && isExpanded && (
                <div className="mt-2 ml-4 pl-3 border-l-2 border-zinc-700 space-y-2">
                    {field.children!.map(child => (
                        <DesignedField key={child.id} field={child} depth={depth + 1} />
                    ))}
                </div>
            )}
        </div>
    );
}
