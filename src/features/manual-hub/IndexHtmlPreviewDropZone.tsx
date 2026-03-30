import type { ReactNode } from 'react';
import { useDroppable } from '@dnd-kit/core';
import { ScrollArea } from '@/components/ui/scroll-area';

function previewInsertLabel(insertIndex: number, rowCount: number): string {
  if (rowCount === 0) return '목록에 추가';
  if (insertIndex === 0) return '맨 위';
  if (insertIndex === rowCount) return '맨 아래';
  return `${insertIndex + 1}번 행 앞`;
}

function PreviewInsertChip(props: { insertIndex: number; dragActive: boolean; rowCount: number }) {
  const { insertIndex, dragActive, rowCount } = props;
  const { setNodeRef, isOver } = useDroppable({
    id: `mh-insert-preview:${insertIndex}`,
    disabled: !dragActive,
  });

  return (
    <button
      type="button"
      ref={setNodeRef}
      className={`rounded border px-2 py-1 text-left text-[10px] leading-tight transition-colors ${
        dragActive && isOver
          ? 'border-blue-500 bg-blue-950/60 text-blue-100'
          : dragActive
            ? 'border-zinc-600 bg-zinc-900 text-zinc-300 hover:bg-zinc-800'
            : 'border-transparent bg-zinc-900/50 text-zinc-500'
      }`}
    >
      {previewInsertLabel(insertIndex, rowCount)}
    </button>
  );
}

type IndexHtmlPreviewDropZoneProps = {
  children: ReactNode;
  dragActive: boolean;
  rowCount: number;
};

/**
 * Local preview / Split(로컬). iframe 위 오버레이 대신 하단에 삽입 위치 칩을 둡니다.
 * (대형 HTML에 테이블이 많아 DOM 행 수와 목록이 일치하지 않는 경우가 많음)
 */
export function IndexHtmlPreviewDropZone({ children, dragActive, rowCount }: IndexHtmlPreviewDropZoneProps) {
  const slotCount = Math.max(1, rowCount + 1);

  return (
    <div className="relative flex min-h-0 min-w-0 flex-1 flex-col">
      <div className="min-h-0 flex-1">{children}</div>
      {dragActive && (
        <div className="flex-shrink-0 border-t border-zinc-800 bg-zinc-900/95 px-2 py-2">
          <p className="mb-1.5 text-[10px] text-zinc-500">
            인덱스 목록 삽입 위치 (아래 칩에 놓기). 화면 속 특정 테이블과는 별개이며, Index table 탭 순서와 동일합니다.
          </p>
          <ScrollArea className="max-h-28 w-full">
            <div className="flex flex-wrap gap-1 pr-2 pb-1">
              {Array.from({ length: slotCount }, (_, i) => (
                <PreviewInsertChip key={i} insertIndex={i} dragActive={dragActive} rowCount={rowCount} />
              ))}
            </div>
          </ScrollArea>
        </div>
      )}
    </div>
  );
}
