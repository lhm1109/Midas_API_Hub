/**
 * Row Drop Zone
 * 
 * 행 사이 드롭 영역 - 필드를 새 행으로 분리
 */

import { useDroppable } from '@dnd-kit/core';

interface RowDropZoneProps {
    afterRowId?: string;  // undefined = 맨 앞 또는 맨 뒤
    position: 'before' | 'after';
}

export function RowDropZone({ afterRowId, position }: RowDropZoneProps) {
    const dropId = afterRowId
        ? `row-drop-${position}-${afterRowId}`
        : `row-drop-${position}-canvas`;

    const { setNodeRef, isOver } = useDroppable({
        id: dropId,
        data: {
            type: 'row-drop-zone',
            afterRowId: position === 'after' ? afterRowId : undefined,
            beforeRowId: position === 'before' ? afterRowId : undefined,
        },
    });

    // Debug: isOver 상태 변경 로깅
    if (isOver) {
        console.log('🎯 Row drop zone is over:', dropId);
    }

    return (
        <div
            ref={setNodeRef}
            className={`
                transition-all duration-200 ease-out rounded
                ${isOver
                    ? 'h-16 my-2 bg-gradient-to-b from-cyan-500/20 to-cyan-500/5 border-2 border-dashed border-cyan-500 flex items-center justify-center'
                    : 'h-3 my-0.5 bg-zinc-700/20 hover:bg-cyan-500/20 hover:h-6 cursor-pointer'
                }
            `}
        >
            {isOver && (
                <div className="flex items-center gap-2">
                    <span className="text-lg text-cyan-400">+</span>
                    <span className="text-xs text-cyan-400 font-medium">New Row</span>
                </div>
            )}
        </div>
    );
}
