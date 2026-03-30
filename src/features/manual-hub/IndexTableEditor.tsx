import { useMemo } from 'react';
import { useDroppable } from '@dnd-kit/core';
import { SortableContext, useSortable, verticalListSortingStrategy } from '@dnd-kit/sortable';
import { CSS } from '@dnd-kit/utilities';
import { Button } from '@/components/ui/button';
import { Input } from '@/components/ui/input';
import { ScrollArea } from '@/components/ui/scroll-area';
import { GripVertical, Trash2 } from 'lucide-react';
import type { ApiEndpoint } from '@/types';
import type { IndexTableRow } from './manualHubStorage';

type IndexTableEditorProps = {
  rows: IndexTableRow[];
  endpointsById: Map<string, ApiEndpoint>;
  /** 왼쪽에서 엔드포인트 드래그 중이면 행 사이 드롭 줄을 강조 */
  endpointDragActive: boolean;
  onRowChange: (id: string, patch: Partial<Pick<IndexTableRow, 'detailLabel' | 'detailHref'>>) => void;
  onRemoveRow: (id: string) => void;
  onApplyToHtml: () => void;
};

function InsertDropRow(props: { index: number; endpointDragActive: boolean }) {
  const { index, endpointDragActive } = props;
  const { setNodeRef, isOver } = useDroppable({
    id: `mh-insert-table:${index}`,
  });

  return (
    <tr ref={setNodeRef} className="border-0">
      <td colSpan={4} className="p-0">
        <div
          className={`w-full transition-colors ${
            endpointDragActive
              ? isOver
                ? 'min-h-[14px] bg-blue-600/45'
                : 'min-h-[10px] bg-zinc-800/50'
              : 'min-h-[3px] bg-transparent'
          }`}
          aria-hidden
        />
      </td>
    </tr>
  );
}

function SortableRow(props: {
  row: IndexTableRow;
  index: number;
  endpoint?: ApiEndpoint;
  onRowChange: IndexTableEditorProps['onRowChange'];
  onRemoveRow: IndexTableEditorProps['onRemoveRow'];
}) {
  const { row, index, endpoint, onRowChange, onRemoveRow } = props;
  const { attributes, listeners, setNodeRef, transform, transition, isDragging } = useSortable({
    id: `mh-row:${row.id}`,
  });
  const style = {
    transform: CSS.Transform.toString(transform),
    transition,
    opacity: isDragging ? 0.6 : 1,
  };

  return (
    <tr ref={setNodeRef} style={style} className="border-b border-zinc-800 bg-zinc-950/80">
      <td className="p-1 align-middle w-10">
        <div className="flex items-center justify-center gap-0.5">
          <button
            type="button"
            className="cursor-grab active:cursor-grabbing p-0.5 rounded hover:bg-zinc-800 text-zinc-500"
            aria-label="Reorder row"
            {...attributes}
            {...listeners}
          >
            <GripVertical className="w-3.5 h-3.5" />
          </button>
          <span className="text-xs text-zinc-400 w-5 text-center">{index + 1}</span>
        </div>
      </td>
      <td className="p-1 align-top">
        <div className="text-[11px] font-mono text-zinc-300 break-all px-1 py-0.5">
          {endpoint?.path ?? '—'}
        </div>
        <div className="text-[10px] text-zinc-500 truncate px-1">{endpoint?.name ?? row.endpointId}</div>
      </td>
      <td className="p-1 align-top min-w-[180px]">
        <Input
          value={row.detailLabel}
          onChange={(e) => onRowChange(row.id, { detailLabel: e.target.value })}
          className="h-7 text-[11px] bg-zinc-900 border-zinc-700 mb-1"
          placeholder="Details 링크 텍스트"
        />
        <Input
          value={row.detailHref}
          onChange={(e) => onRowChange(row.id, { detailHref: e.target.value })}
          className="h-7 text-[11px] bg-zinc-900 border-zinc-700 font-mono"
          placeholder="https://… 또는 /hc/…"
        />
      </td>
      <td className="p-1 w-10 align-middle">
        <Button
          type="button"
          variant="ghost"
          size="sm"
          className="h-7 w-7 p-0 text-zinc-500 hover:text-red-400"
          onClick={() => onRemoveRow(row.id)}
          aria-label="Remove row"
        >
          <Trash2 className="w-3.5 h-3.5" />
        </Button>
      </td>
    </tr>
  );
}

export function IndexTableEditor({
  rows,
  endpointsById,
  endpointDragActive,
  onRowChange,
  onRemoveRow,
  onApplyToHtml,
}: IndexTableEditorProps) {
  const sortableIds = useMemo(() => rows.map((r) => `mh-row:${r.id}`), [rows]);

  return (
    <div className="flex flex-col h-full min-h-0 bg-zinc-950">
      <div className="flex-shrink-0 px-3 py-2 border-b border-zinc-800 flex flex-wrap items-center justify-between gap-2">
        <div>
          <h3 className="text-sm font-medium text-zinc-200">Index 테이블</h3>
          <p className="text-[10px] text-zinc-500 mt-0.5">
            왼쪽 ⋮⋮을 행 사이 회색 줄(또는 Local preview의 위·아래 구간)에 놓으면 그 위치에 삽입·이동됩니다. 이미 있는 엔드포인트는 같은 동작으로 순서만 바뀝니다.
          </p>
        </div>
        <Button type="button" size="sm" className="h-8 text-xs bg-emerald-700 hover:bg-emerald-600" onClick={onApplyToHtml}>
          HTML에 테이블 반영
        </Button>
      </div>
      <ScrollArea className="flex-1 min-h-0">
        <div className="p-3">
          <div className="rounded-lg border border-zinc-800 overflow-hidden">
            <table className="w-full text-left text-xs">
              <thead>
                <tr className="bg-zinc-900 text-zinc-400 border-b border-zinc-800">
                  <th className="p-2 w-14 font-medium">No.</th>
                  <th className="p-2 font-medium">End Point</th>
                  <th className="p-2 font-medium">Details</th>
                  <th className="p-2 w-10" />
                </tr>
              </thead>
              <tbody>
                <InsertDropRow index={0} endpointDragActive={endpointDragActive} />
                {rows.length === 0 ? (
                  <tr>
                    <td colSpan={4} className="p-4 text-center text-[10px] text-zinc-500">
                      행이 없습니다. 왼쪽 ⋮⋮을 드래그하거나 위 줄에 놓으세요.
                    </td>
                  </tr>
                ) : (
                  <SortableContext items={sortableIds} strategy={verticalListSortingStrategy}>
                    {rows.flatMap((row, idx) => [
                      <SortableRow
                        key={row.id}
                        row={row}
                        index={idx}
                        endpoint={endpointsById.get(row.endpointId)}
                        onRowChange={onRowChange}
                        onRemoveRow={onRemoveRow}
                      />,
                      <InsertDropRow
                        key={`mh-ins-${row.id}`}
                        index={idx + 1}
                        endpointDragActive={endpointDragActive}
                      />,
                    ])}
                  </SortableContext>
                )}
              </tbody>
            </table>
          </div>
        </div>
      </ScrollArea>
    </div>
  );
}
