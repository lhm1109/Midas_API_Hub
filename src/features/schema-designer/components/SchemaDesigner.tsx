/**
 * Schema Designer
 * 
 * 메인 3패널 레이아웃 - Visual JSON Schema Builder
 * Row-based Grid Layout System
 */

import { useEffect, useCallback, useState } from 'react';
import {
    DndContext,
    DragEndEvent,
    DragStartEvent,
    PointerSensor,
    useSensor,
    useSensors,
    closestCenter,
    DragOverlay,
} from '@dnd-kit/core';
import { ResizablePanelGroup, ResizablePanel, ResizableHandle } from '@/components/ui/resizable';
import { useDesignerStore } from '../stores/designerStore';
import { ComponentPalette } from './ComponentPalette';
import { CanvasArea } from './CanvasArea';
import { PropertyPanel } from './PropertyPanel';
import { schemaToLayout } from '../lib/schemaToLayout';
import { layoutToSchemaString } from '../lib/layoutToSchema';
import type { DesignedField, FieldType } from '../types/designer';

// ============================================================================
// Types
// ============================================================================

interface SchemaDesignerProps {
    initialSchema?: any;
    psdSet?: string;
    onSchemaChange?: (schemaJson: string) => void;
}

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

    // Section은 x-ui.group으로 처리
    if (type === 'section') {
        field.xui = { group: 'New Section' };
    }

    // Enum은 기본값 제공
    if (type === 'enum') {
        field.type = 'enum';
        field.enum = ['Option1', 'Option2', 'Option3'];
        field.xenumLabels = {
            'Option1': 'Option 1',
            'Option2': 'Option 2',
            'Option3': 'Option 3',
        };
    }

    // Object는 children 초기화
    if (type === 'object') {
        field.children = [];
    }

    // Array는 items 초기화
    if (type === 'array') {
        field.items = { type: 'string' };
    }

    return field;
}

// ============================================================================
// Schema Designer
// ============================================================================

export function SchemaDesigner({ initialSchema, psdSet, onSchemaChange }: SchemaDesignerProps) {
    const {
        fields,
        rows,
        transport,
        addField,
        setFields,
        updateTransport,
        loadConfig,
        mergeToRow,
        separateToNewRow,
        reorderColumnsInRow,
    } = useDesignerStore();

    // Drag overlay state
    const [activeId, setActiveId] = useState<string | null>(null);

    // Load config on mount
    useEffect(() => {
        console.log('🎨 SchemaDesigner mounted, loading config for psdSet:', psdSet);
        loadConfig(psdSet);
    }, [psdSet, loadConfig]);

    // Debug: Log rows and fields
    useEffect(() => {
        console.log('📊 Current state:', {
            rowsCount: rows.length,
            fieldsCount: fields.length,
            rows: rows.map(r => ({ id: r.id, fieldIds: r.fieldIds })),
        });
    }, [rows, fields]);

    // Load initial schema
    useEffect(() => {
        if (initialSchema) {
            try {
                const schema = typeof initialSchema === 'string'
                    ? JSON.parse(initialSchema)
                    : initialSchema;

                const result = schemaToLayout(schema);
                setFields(result.fields);
                if (result.transport) {
                    updateTransport(result.transport);
                }
            } catch (error) {
                console.error('Failed to load initial schema:', error);
            }
        }
    }, [initialSchema, setFields, updateTransport]);

    // Emit schema changes
    useEffect(() => {
        if (onSchemaChange && fields.length > 0) {
            const schemaJson = layoutToSchemaString(fields, transport);
            onSchemaChange(schemaJson);
        }
    }, [fields, transport, onSchemaChange]);

    // DnD Sensors - activationConstraint 제거 (즉시 드래그 가능)
    const sensors = useSensors(
        useSensor(PointerSensor, {
            // activationConstraint 제거하여 즉시 드래그 가능하도록
        })
    );

    // Debug: DndContext mounted
    useEffect(() => {
        console.log('🎯 DndContext ready with sensors:', sensors.length, 'sensors');
        console.log('🎯 Sensors config:', sensors);
    }, [sensors]);

    // Handle drag end - useCallback으로 재설정
    const handleDragEnd = useCallback((event: DragEndEvent) => {
        const { active, over } = event;

        console.log('🔴 ========== DRAG END ==========');
        console.log('🔹 Active:', active?.id, active?.data?.current);
        console.log('🔹 Over:', over?.id, over?.data?.current);

        // Reset active state
        setActiveId(null);

        if (!over) {
            console.log('⚠️ No drop target (over is null)');
            return;
        }

        const activeData = active.data.current;
        const overData = over.data.current;

        console.log('🔹 Processing drag:', { activeType: activeData?.type, overType: overData?.type });

        // ================================================================
        // 1. Palette → Canvas (새 필드 → 새 행으로 추가)
        // ================================================================
        if (activeData?.type === 'palette-item' && overData?.type === 'canvas') {
            const fieldType = activeData.fieldType as FieldType | 'section';
            const newField = createDefaultField(fieldType);
            addField(newField);  // addField now automatically creates a new row
            return;
        }

        // ================================================================
        // 2. Palette → Row Drop Zone (새 필드 → 특정 위치에 새 행)
        // ================================================================
        if (activeData?.type === 'palette-item' && overData?.type === 'row-drop-zone') {
            const fieldType = activeData.fieldType as FieldType | 'section';
            const newField = createDefaultField(fieldType);
            addField(newField);
            // After adding, move to correct position
            if (overData.afterRowId) {
                separateToNewRow(newField.id, overData.afterRowId);
            }
            return;
        }

        // ================================================================
        // 3. Palette → Column Drop Zone (새 필드 → 기존 행에 컬럼으로)
        // ================================================================
        if (activeData?.type === 'palette-item' && overData?.type === 'column-drop-zone') {
            const fieldType = activeData.fieldType as FieldType | 'section';
            const newField = createDefaultField(fieldType);
            addField(newField);
            // Merge to the target row
            mergeToRow(newField.id, overData.rowId, overData.columnIndex);
            return;
        }

        // ================================================================
        // 4. Field → Row Drop Zone (기존 필드 → 새 행으로 분리)
        // ================================================================
        if (activeData?.type === 'field' && overData?.type === 'row-drop-zone') {
            const fieldId = active.id as string;
            separateToNewRow(fieldId, overData.afterRowId);
            return;
        }

        // ================================================================
        // 5. Field → Column Drop Zone (기존 필드 → 다른 행으로 합치기)
        // ================================================================
        if (activeData?.type === 'field' && overData?.type === 'column-drop-zone') {
            const fieldId = active.id as string;
            const { rowId, columnIndex } = overData;

            // Find current row of the field
            const currentRow = rows.find(r => r.fieldIds.includes(fieldId));

            if (currentRow && currentRow.id === rowId) {
                // Same row - reorder within row
                const currentIndex = currentRow.fieldIds.indexOf(fieldId);
                if (currentIndex !== columnIndex && currentIndex !== columnIndex - 1) {
                    // Adjust index if moving forward
                    const adjustedIndex = currentIndex < columnIndex ? columnIndex - 1 : columnIndex;
                    reorderColumnsInRow(rowId, currentIndex, adjustedIndex);
                }
            } else {
                // Different row - merge to target row
                mergeToRow(fieldId, rowId, columnIndex);
            }
            return;
        }

        // ================================================================
        // 6. Field → Field (같은 행 내 순서 변경)
        // ================================================================
        if (activeData?.type === 'field' && active.id !== over.id) {
            const fieldId = active.id as string;
            const targetFieldId = over.id as string;

            // Find rows
            const sourceRow = rows.find(r => r.fieldIds.includes(fieldId));
            const targetRow = rows.find(r => r.fieldIds.includes(targetFieldId));

            if (sourceRow && targetRow && sourceRow.id === targetRow.id) {
                // Same row - reorder
                const sourceIndex = sourceRow.fieldIds.indexOf(fieldId);
                const targetIndex = sourceRow.fieldIds.indexOf(targetFieldId);
                reorderColumnsInRow(sourceRow.id, sourceIndex, targetIndex);
            } else if (sourceRow && targetRow) {
                // Different rows - merge to target row at target field's position
                const targetIndex = targetRow.fieldIds.indexOf(targetFieldId);
                mergeToRow(fieldId, targetRow.id, targetIndex);
            }
        }
    }, [rows, addField, mergeToRow, separateToNewRow, reorderColumnsInRow]);

    // 드래그 이벤트 핸들러
    const handleDragStart = useCallback((event: DragStartEvent) => {
        console.log('🟢 ========== DRAG START ==========');
        console.log('🔹 Active ID:', event.active?.id);
        console.log('🔹 Active Data:', event.active?.data?.current);
        console.log('🔹 Active Type:', typeof event.active?.id);
        console.log('🔹 Event:', event);
        setActiveId(event.active.id as string);
    }, []);

    const handleDragMove = useCallback((event: any) => {
        // 너무 많은 로그를 방지하기 위해 over가 변경될 때만 로깅
        if (event.over?.id) {
            console.log('🔵 DRAG MOVE over:', event.over.id);
        }
    }, []);

    const handleDragCancel = useCallback(() => {
        console.log('❌ DRAG CANCELLED');
        setActiveId(null);
    }, []);

    // 이벤트 핸들러 등록 확인
    useEffect(() => {
        console.log('✅ DndContext handlers registered:', {
            handleDragStart: !!handleDragStart,
            handleDragMove: !!handleDragMove,
            handleDragEnd: !!handleDragEnd,
            handleDragCancel: !!handleDragCancel,
        });
    }, [handleDragStart, handleDragMove, handleDragEnd, handleDragCancel]);

    // 🔍 직접 테스트용 핸들러 (인라인)
    console.log('🎪 SchemaDesigner render - handlers:', {
        handleDragStart: typeof handleDragStart,
        handleDragMove: typeof handleDragMove,
        handleDragEnd: typeof handleDragEnd,
        handleDragCancel: typeof handleDragCancel,
    });

    return (
        <DndContext
            sensors={sensors}
            onDragStart={(e) => {
                console.log('🟢🟢🟢 INLINE DRAG START:', e.active.id);
                handleDragStart(e);
            }}
            onDragMove={(e) => {
                if (e.over?.id) {
                    console.log('🔵🔵🔵 INLINE DRAG MOVE:', e.over.id);
                }
                handleDragMove(e);
            }}
            onDragEnd={(e) => {
                console.log('🔴🔴🔴 INLINE DRAG END:', e.active.id, '->', e.over?.id);
                handleDragEnd(e);
            }}
            onDragCancel={() => {
                console.log('❌❌❌ INLINE DRAG CANCEL');
                handleDragCancel();
            }}
            collisionDetection={closestCenter}
        >
            <div className="h-full w-full flex flex-col bg-zinc-950">
                {/* Main Content */}
                <ResizablePanelGroup direction="horizontal" className="flex-1">
                    {/* Left Panel: Component Palette */}
                    <ResizablePanel defaultSize={18} minSize={15} maxSize={25}>
                        <ComponentPalette />
                    </ResizablePanel>

                    <ResizableHandle className="w-1 bg-zinc-800 hover:bg-cyan-600 transition-colors" />

                    {/* Center Panel: Canvas */}
                    <ResizablePanel defaultSize={52} minSize={40}>
                        <CanvasArea />
                    </ResizablePanel>

                    <ResizableHandle className="w-1 bg-zinc-800 hover:bg-cyan-600 transition-colors" />

                    {/* Right Panel: Properties */}
                    <ResizablePanel defaultSize={30} minSize={20} maxSize={40}>
                        <PropertyPanel />
                    </ResizablePanel>
                </ResizablePanelGroup>
            </div>

            {/* Drag Overlay - 드래그 중인 요소 표시 */}
            <DragOverlay>
                {activeId ? (
                    <div className="bg-zinc-800 border-2 border-cyan-500 rounded-lg p-3 shadow-2xl opacity-80">
                        <span className="text-sm text-zinc-200">Dragging: {activeId}</span>
                    </div>
                ) : null}
            </DragOverlay>
        </DndContext>
    );
}
