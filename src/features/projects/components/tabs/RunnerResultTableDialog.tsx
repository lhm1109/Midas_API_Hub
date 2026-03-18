import { useEffect, useRef, useState } from 'react';
import {
  Dialog,
  DialogContent,
  DialogDescription,
  DialogHeader,
  DialogTitle,
} from '@/components/ui/dialog';
import {
  Table,
  TableBody,
  TableCell,
  TableHead,
  TableHeader,
  TableRow,
} from '@/components/ui/table';
import type { RunnerResultTableModel } from './runnerResultTable.logic';

interface RunnerResultTableDialogProps {
  model: RunnerResultTableModel | null;
  open: boolean;
  onOpenChange: (open: boolean) => void;
}

type DialogSize = {
  width: number;
  height: number;
};

const DIALOG_MARGIN = 12;
const MIN_DIALOG_WIDTH = 720;
const MIN_DIALOG_HEIGHT = 420;

const getCellTone = (value: string) => {
  const normalized = value.trim().toUpperCase();
  if (normalized === 'OK') return 'text-emerald-300 font-medium';
  if (normalized === 'NG' || normalized === 'FAIL' || normalized === 'ERROR') return 'text-red-300 font-medium';
  if (normalized === '-') return 'text-zinc-500';
  return 'text-zinc-200';
};

const getAlignClass = (align: 'left' | 'center' | 'right') => {
  if (align === 'right') return 'text-right';
  if (align === 'center') return 'text-center';
  return 'text-left';
};

export function RunnerResultTableDialog({
  model,
  open,
  onOpenChange,
}: RunnerResultTableDialogProps) {
  const [dialogSize, setDialogSize] = useState<DialogSize | null>(null);
  const resizeStateRef = useRef<{
    startX: number;
    startY: number;
    startWidth: number;
    startHeight: number;
  } | null>(null);

  useEffect(() => {
    if (!open || typeof window === 'undefined') return;

    const maxWidth = window.innerWidth - DIALOG_MARGIN;
    const maxHeight = window.innerHeight - DIALOG_MARGIN;
    setDialogSize((current) => current || {
      width: Math.max(MIN_DIALOG_WIDTH, Math.min(maxWidth, Math.floor(window.innerWidth * 0.9))),
      height: Math.max(MIN_DIALOG_HEIGHT, Math.min(maxHeight, Math.floor(window.innerHeight * 0.85))),
    });
  }, [open]);

  useEffect(() => {
    if (!open || typeof window === 'undefined') return;

    const handleWindowResize = () => {
      const maxWidth = window.innerWidth - DIALOG_MARGIN;
      const maxHeight = window.innerHeight - DIALOG_MARGIN;

      setDialogSize((current) => {
        if (!current) {
          return {
            width: Math.max(MIN_DIALOG_WIDTH, Math.min(maxWidth, Math.floor(window.innerWidth * 0.9))),
            height: Math.max(MIN_DIALOG_HEIGHT, Math.min(maxHeight, Math.floor(window.innerHeight * 0.85))),
          };
        }

        return {
          width: Math.min(current.width, maxWidth),
          height: Math.min(current.height, maxHeight),
        };
      });
    };

    window.addEventListener('resize', handleWindowResize);
    return () => window.removeEventListener('resize', handleWindowResize);
  }, [open]);

  useEffect(() => {
    if (!open) return;

    const handleMouseMove = (event: MouseEvent) => {
      const resizeState = resizeStateRef.current;
      if (!resizeState || typeof window === 'undefined') return;

      const maxWidth = window.innerWidth - DIALOG_MARGIN;
      const maxHeight = window.innerHeight - DIALOG_MARGIN;
      const nextWidth = Math.max(
        MIN_DIALOG_WIDTH,
        Math.min(maxWidth, resizeState.startWidth + (event.clientX - resizeState.startX))
      );
      const nextHeight = Math.max(
        MIN_DIALOG_HEIGHT,
        Math.min(maxHeight, resizeState.startHeight + (event.clientY - resizeState.startY))
      );

      setDialogSize({
        width: nextWidth,
        height: nextHeight,
      });
    };

    const handleMouseUp = () => {
      resizeStateRef.current = null;
      document.body.style.userSelect = '';
      document.body.style.cursor = '';
    };

    window.addEventListener('mousemove', handleMouseMove);
    window.addEventListener('mouseup', handleMouseUp);

    return () => {
      window.removeEventListener('mousemove', handleMouseMove);
      window.removeEventListener('mouseup', handleMouseUp);
      document.body.style.userSelect = '';
      document.body.style.cursor = '';
    };
  }, [open]);

  if (!model) return null;

  const handleResizeStart = (event: React.MouseEvent<HTMLButtonElement>) => {
    event.preventDefault();
    event.stopPropagation();

    resizeStateRef.current = {
      startX: event.clientX,
      startY: event.clientY,
      startWidth: dialogSize?.width || MIN_DIALOG_WIDTH,
      startHeight: dialogSize?.height || MIN_DIALOG_HEIGHT,
    };

    document.body.style.userSelect = 'none';
    document.body.style.cursor = 'nwse-resize';
  };

  return (
    <Dialog open={open} onOpenChange={onOpenChange}>
      <DialogContent
        className="!w-auto !max-w-none sm:!max-w-none bg-zinc-950 border-zinc-800 p-0 gap-0 overflow-hidden"
        style={{
          width: dialogSize ? `${dialogSize.width}px` : '90vw',
          height: dialogSize ? `${dialogSize.height}px` : '85vh',
          maxWidth: `calc(100vw - ${DIALOG_MARGIN}px)`,
          maxHeight: `calc(100vh - ${DIALOG_MARGIN}px)`,
          minWidth: `${MIN_DIALOG_WIDTH}px`,
          minHeight: `${MIN_DIALOG_HEIGHT}px`,
        }}
      >
        <div className="flex h-full min-h-0 min-w-0 flex-col">
          <DialogHeader className="px-6 py-5 border-b border-zinc-800 bg-zinc-900 pr-14 flex-shrink-0">
            <div className="flex items-start justify-between gap-4">
              <div>
                <DialogTitle className="text-zinc-100">{model.title}</DialogTitle>
                <DialogDescription className="text-zinc-400 mt-1">
                  {model.description || 'JSON table preview'}
                </DialogDescription>
              </div>
              <div className="flex flex-wrap justify-end gap-2">
                {model.meta.map((item) => (
                  <div
                    key={item.key}
                    className="rounded-md border border-zinc-700 bg-zinc-950 px-2.5 py-1 text-[11px] text-zinc-300"
                  >
                    <span className="text-zinc-500 mr-1">{item.key}</span>
                    <span className="font-medium text-zinc-100">{item.value}</span>
                  </div>
                ))}
              </div>
            </div>
          </DialogHeader>

          <div className="px-6 py-3 border-b border-zinc-800 bg-zinc-900/60 flex-shrink-0 flex items-center justify-between gap-3 text-xs">
            <div className="flex items-center gap-3 min-w-0">
              <span className="text-zinc-500 truncate">Detected at {model.path}</span>
              <span className="text-zinc-700 hidden sm:inline">|</span>
              <span className="text-zinc-600 hidden sm:inline">Bottom-right corner: drag to resize</span>
            </div>
            <span className="text-zinc-400 flex-shrink-0">
              {model.rows.length} rows · {model.columns.length} columns
            </span>
          </div>

          <div className="flex-1 min-h-0 min-w-0 overflow-auto overscroll-contain">
            <div className="min-h-full min-w-max">
              <Table className="min-w-max border-separate border-spacing-0">
              <TableHeader className="sticky top-0 z-10 bg-zinc-900">
                <TableRow className="border-b border-zinc-700 hover:bg-transparent">
                  {model.columns.map((column) => (
                    <TableHead
                      key={column.key}
                      className={[
                        'sticky top-0 border-b border-r border-zinc-800 bg-zinc-900 px-3 py-2 text-[11px] font-semibold tracking-[0.16em] uppercase text-zinc-300',
                        getAlignClass(column.align),
                      ].join(' ')}
                    >
                      {column.header}
                    </TableHead>
                  ))}
                </TableRow>
              </TableHeader>

              <TableBody>
                {model.rows.map((row, rowIndex) => (
                  <TableRow
                    key={row.id}
                    className={rowIndex % 2 === 0 ? 'bg-zinc-950 hover:bg-zinc-900/80' : 'bg-zinc-900/40 hover:bg-zinc-900/80'}
                  >
                    {row.values.map((value, columnIndex) => {
                      const column = model.columns[columnIndex];
                      return (
                        <TableCell
                          key={`${row.id}-${column.key}`}
                          className={[
                            'border-r border-b border-zinc-800 px-3 py-2 font-mono text-xs',
                            getAlignClass(column.align),
                            getCellTone(value),
                          ].join(' ')}
                        >
                          {value || '-'}
                        </TableCell>
                      );
                    })}
                  </TableRow>
                ))}
              </TableBody>
              </Table>
            </div>
          </div>

          <button
            type="button"
            aria-label="Resize dialog"
            onMouseDown={handleResizeStart}
            className="absolute right-0 bottom-0 z-20 h-5 w-5 cursor-nwse-resize bg-transparent"
          >
            <span className="absolute right-1 bottom-1 h-3 w-3 border-r-2 border-b-2 border-zinc-500" />
          </button>
        </div>
      </DialogContent>
    </Dialog>
  );
}
