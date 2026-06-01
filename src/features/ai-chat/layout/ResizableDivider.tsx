import { useRef, useCallback } from 'react';

// Nimbalyst ResizablePanel의 드래그 핸들 패턴 기반
// 라이브러리 없음, 커스텀 드래그 핸들러

interface ResizableDividerProps {
  direction: 'horizontal' | 'vertical';
  onResize: (delta: number) => void;
  onResizeEnd?: () => void;
}

export function ResizableDivider({ direction, onResize, onResizeEnd }: ResizableDividerProps) {
  const startPosRef = useRef<number>(0);
  const isDraggingRef = useRef(false);

  const handleMouseDown = useCallback((e: React.MouseEvent) => {
    e.preventDefault();
    isDraggingRef.current = true;
    startPosRef.current = direction === 'horizontal' ? e.clientX : e.clientY;

    document.body.style.cursor = direction === 'horizontal' ? 'ew-resize' : 'ns-resize';
    document.body.style.userSelect = 'none';

    const onMove = (moveEvent: MouseEvent) => {
      if (!isDraggingRef.current) return;
      const pos = direction === 'horizontal' ? moveEvent.clientX : moveEvent.clientY;
      const delta = pos - startPosRef.current;
      startPosRef.current = pos;
      onResize(delta);
    };

    const onUp = () => {
      isDraggingRef.current = false;
      document.body.style.cursor = '';
      document.body.style.userSelect = '';
      document.removeEventListener('mousemove', onMove);
      document.removeEventListener('mouseup', onUp);
      onResizeEnd?.();
    };

    document.addEventListener('mousemove', onMove);
    document.addEventListener('mouseup', onUp);
  }, [direction, onResize, onResizeEnd]);

  return (
    <div
      onMouseDown={handleMouseDown}
      className={`
        group relative flex-shrink-0 bg-zinc-800
        hover:bg-blue-500/50 active:bg-blue-500
        transition-colors duration-150
        ${direction === 'horizontal'
          ? 'w-[3px] cursor-ew-resize h-full'
          : 'h-[3px] cursor-ns-resize w-full'
        }
      `}
    >
      {/* 드래그 영역 확장 (시각적 너비보다 넓게) */}
      <div className={`
        absolute inset-0
        ${direction === 'horizontal' ? '-left-1 -right-1' : '-top-1 -bottom-1'}
      `} />
    </div>
  );
}
