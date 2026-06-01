import {
  Table,
  TableBody,
  TableCell,
  TableHead,
  TableHeader,
  TableRow,
} from '@/components/ui/table';
import type { RunnerResultTableModel } from './runnerResultTable.logic';

interface RunnerResultTableViewProps {
  model: RunnerResultTableModel;
}

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

export function RunnerResultTableView({ model }: RunnerResultTableViewProps) {
  return (
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
  );
}
