/**
 * Canvas Area
 * 
 * Row-based Grid Layout System
 * 행 기반 드래그 앤 드롭 레이아웃
 */

import { useDroppable } from '@dnd-kit/core';
import { useDesignerStore } from '../stores/designerStore';
import { DesignedField } from './DesignedField';
import { RowDropZone } from './RowDropZone';
import { ColumnDropZone } from './ColumnDropZone';
import type { DesignerRow, DesignedField as DesignedFieldType } from '../types/designer';

// ============================================================================
// Empty State
// ============================================================================

function EmptyState() {
    return (
        <div className="flex-1 flex flex-col items-center justify-center text-center p-8">
            <div className="text-4xl mb-4">📋</div>
            <h3 className="text-lg font-medium text-zinc-300 mb-2">
                Start Building Your Schema
            </h3>
            <p className="text-sm text-zinc-500 max-w-xs">
                Drag components from the left panel and drop them here to create your JSON Schema structure
            </p>
            <div className="mt-6 flex flex-col gap-2 text-xs text-zinc-600">
                <div className="flex items-center gap-2">
                    <span>📝</span>
                    <span>Drag Text, Number, Select fields</span>
                </div>
                <div className="flex items-center gap-2">
                    <span>📁</span>
                    <span>Use Object for nested structures</span>
                </div>
                <div className="flex items-center gap-2">
                    <span>🔄</span>
                    <span>Drag to row gap = new row, to field side = same row</span>
                </div>
            </div>
        </div>
    );
}

// ============================================================================
// Row Container
// ============================================================================

interface RowContainerProps {
    row: DesignerRow;
    rowIndex: number;
    fields: DesignedFieldType[];
}

function RowContainer({ row, rowIndex, fields }: RowContainerProps) {
    const rowFields = row.fieldIds
        .map(id => fields.find(f => f.id === id))
        .filter((f): f is DesignedFieldType => f !== null);

    const columnCount = rowFields.length;

    // Calculate flex basis based on column count for equal distribution
    const getColumnStyle = () => {
        if (columnCount <= 0) return {};
        return { flex: `1 1 ${100 / columnCount}%`, maxWidth: `${100 / columnCount}%` };
    };

    return (
        <div className="group/row relative flex items-stretch gap-0">
            {/* Row Index Badge - 바로 왼쪽에 배치 */}
            <div className="flex-shrink-0 flex items-center pr-2">
                <span className="text-[11px] text-zinc-500 font-mono bg-zinc-800 px-1.5 py-1 rounded opacity-50 group-hover/row:opacity-100 transition-opacity">
                    {rowIndex + 1}
                </span>
            </div>

            {/* Row Container with Horizontal Layout */}
            <div className="flex-1 flex items-stretch rounded-lg border-2 border-transparent group-hover/row:border-zinc-700 transition-all bg-zinc-800/30 group-hover/row:bg-zinc-800/50">
                {/* First column drop zone */}
                <ColumnDropZone rowId={row.id} columnIndex={0} />

                {rowFields.map((field, colIndex) => (
                    <div
                        key={field.id}
                        className="flex items-stretch"
                        style={getColumnStyle()}
                    >
                        {/* Field - 마진으로 간격 */}
                        <div className="flex-1 min-w-0 my-1 mx-1">
                            <DesignedField field={field} />
                        </div>

                        {/* Column drop zone after each field */}
                        <ColumnDropZone rowId={row.id} columnIndex={colIndex + 1} />
                    </div>
                ))}

                {/* Column count indicator */}
                {columnCount > 1 && (
                    <div className="absolute -right-2 top-1/2 -translate-y-1/2 opacity-0 group-hover/row:opacity-70 transition-opacity">
                        <span className="text-[9px] text-cyan-400 font-mono bg-zinc-800 px-1 py-0.5 rounded">
                            {columnCount}col
                        </span>
                    </div>
                )}
            </div>
        </div>
    );
}

// ============================================================================
// Canvas Area
// ============================================================================

export function CanvasArea() {
    const { fields, rows, selectField } = useDesignerStore();

    const { setNodeRef, isOver } = useDroppable({
        id: 'canvas-drop-zone',
        data: {
            type: 'canvas',
        },
    });

    const handleCanvasClick = () => {
        selectField(null);
    };

    const isEmpty = rows.length === 0;

    return (
        <div className="flex-1 flex flex-col bg-zinc-900/50 h-full">
            {/* Header */}
            <div className="px-4 py-3 border-b border-zinc-700 bg-zinc-900">
                <div className="flex items-center justify-between">
                    <h3 className="text-sm font-semibold text-zinc-200 flex items-center gap-2">
                        <span>🎯</span>
                        Canvas
                        {rows.length > 0 && (
                            <span className="text-xs text-zinc-500 font-normal">
                                ({rows.length} rows, {fields.length} fields)
                            </span>
                        )}
                    </h3>
                </div>
                {rows.length > 0 && (
                    <div className="text-[10px] text-zinc-600 mt-1">
                        💡 Drag to <span className="text-cyan-500">row gap</span> = new row |
                        Drag to <span className="text-cyan-500">field side</span> = add column
                    </div>
                )}
            </div>

            {/* Canvas Drop Zone */}
            <div
                ref={setNodeRef}
                onClick={handleCanvasClick}
                className={`
                    flex-1 overflow-y-auto p-4
                    transition-colors duration-200
                    ${isOver ? 'bg-cyan-900/10 ring-2 ring-cyan-600 ring-inset' : ''}
                    ${isEmpty ? 'flex' : ''}
                `}
            >
                {isEmpty ? (
                    <EmptyState />
                ) : (
                    <div className="space-y-0">
                        {rows.map((row, rowIndex) => (
                            <div key={row.id}>
                                {/* Row drop zone before each row (for inserting new row above) */}
                                <RowDropZone position="before" afterRowId={rowIndex > 0 ? rows[rowIndex - 1].id : undefined} />

                                {/* Row */}
                                <RowContainer
                                    row={row}
                                    rowIndex={rowIndex}
                                    fields={fields}
                                />
                            </div>
                        ))}

                        {/* Final row drop zone (add at bottom) */}
                        <RowDropZone position="after" afterRowId={rows[rows.length - 1]?.id} />
                    </div>
                )}

                {/* Drop Indicator for empty canvas */}
                {isOver && isEmpty && (
                    <div className="mt-2 h-16 border-2 border-dashed border-cyan-600 rounded-lg flex items-center justify-center">
                        <span className="text-sm text-cyan-400">Drop here to add first field</span>
                    </div>
                )}
            </div>
        </div>
    );
}
