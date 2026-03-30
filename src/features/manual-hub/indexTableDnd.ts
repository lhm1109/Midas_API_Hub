import type { IndexTableRow } from './manualHubStorage';

/** insertIndex: 0..rows.length (행 앞에 삽입, length면 맨 끝). 같은 endpointId가 있으면 그 행을 해당 위치로 옮깁니다. */
export function applyEndpointToRowsAt(
  rows: IndexTableRow[],
  endpointId: string,
  insertIndex: number,
  newRow: IndexTableRow
): IndexTableRow[] {
  const clamped = Math.max(0, Math.min(insertIndex, rows.length));
  const next = [...rows];
  const from = next.findIndex((r) => r.endpointId === endpointId);
  if (from >= 0) {
    const [row] = next.splice(from, 1);
    let to = clamped;
    if (from < clamped) to = clamped - 1;
    to = Math.max(0, Math.min(to, next.length));
    next.splice(to, 0, row);
    return next;
  }
  next.splice(clamped, 0, newRow);
  return next;
}

export function parseInsertDropTarget(overId: string): { kind: 'table' | 'preview'; index: number } | null {
  const t = /^mh-insert-table:(\d+)$/.exec(overId);
  if (t) return { kind: 'table', index: parseInt(t[1], 10) };
  const p = /^mh-insert-preview:(\d+)$/.exec(overId);
  if (p) return { kind: 'preview', index: parseInt(p[1], 10) };
  return null;
}
