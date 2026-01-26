/**
 * Column Drop Zone
 * 
 * 컬럼 사이 드롭 영역 - 같은 행 내 컬럼화 또는 순서 변경
 */

import { useDroppable } from '@dnd-kit/core';

interface ColumnDropZoneProps {
    rowId: string;
    columnIndex: number;
}

export function ColumnDropZone({ rowId, columnIndex }: ColumnDropZoneProps) {
    const dropId = `col-drop-${rowId}-${columnIndex}`;
    const { setNodeRef, isOver } = useDroppable({
        id: dropId,
        data: {
            type: 'column-drop-zone',
            rowId,
            columnIndex,
        },
    });

    // Debug: isOver 상태 변경 로깅
    if (isOver) {
        console.log('🎯 Column drop zone is over:', dropId);
    }

    return (
        <div
            ref={setNodeRef}
            className={`
                flex-shrink-0 transition-all duration-200 ease-out
                flex items-center justify-center
                ${isOver
                    ? 'w-20 bg-gradient-to-r from-cyan-500/30 to-cyan-500/10 border-2 border-dashed border-cyan-500 rounded-lg mx-1'
                    : 'w-6 hover:w-10 mx-0.5 rounded cursor-pointer'
                }
            `}
            style={{ minHeight: '60px' }}
        >
            {isOver ? (
                <div className="flex flex-col items-center">
                    <span className="text-lg text-cyan-400">+</span>
                    <span className="text-[9px] text-cyan-400 font-medium">Column</span>
                </div>
            ) : (
                <div className="flex items-center justify-center opacity-0 hover:opacity-40 transition-opacity">
                    <span className="text-xs text-zinc-600">⋮</span>
                </div>
            )}
        </div>
    );
}
