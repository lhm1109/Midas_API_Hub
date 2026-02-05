import { useState, useMemo } from 'react';
import {
  Table,
  TableBody,
  TableCell,
  TableHead,
  TableHeader,
  TableRow,
} from '@/components/ui/table';
import { Button } from '@/components/ui/button';
import {
  DropdownMenu,
  DropdownMenuCheckboxItem,
  DropdownMenuContent,
  DropdownMenuTrigger,
} from '@/components/ui/dropdown-menu';
import {
  ChevronUp,
  ChevronDown,
  Settings2,
  ExternalLink,
  Edit,
  Trash2,
  Plus,
} from 'lucide-react';
import { cn } from '@/components/ui/utils';
import { ApiTask, Column } from '../../../types/manager';
import { StatusIcon, StatusBadge } from './StatusIcon';

interface ApiTableProps {
  tasks: ApiTask[];
  columns: Column[];
  searchTerm?: string;
  onColumnVisibilityChange: (columnId: string, visible: boolean) => void;
  onTaskEdit: (task: ApiTask) => void;
  onTaskDelete: (taskId: string) => void;
  onAddTask: () => void;
}

type SortDirection = 'asc' | 'desc' | null;

// 컬럼 그룹 정의
const COLUMN_GROUPS = {
  productRibbon: {
    label: 'Product Ribbon',
    columns: ['tab', 'group', 'sub1'],
  },
  api: {
    label: 'API',
    columns: ['sub2', 'sub3', 'seg1', 'seg2', 'endPoint'],
  },
  pipeline: {
    label: 'Pipeline',
    columns: ['dev', 'vv', 'doc', 'issue', 'status', 'charge', 'remark'],
  },
};

export function ApiTable({
  tasks,
  columns,
  searchTerm = '',
  onColumnVisibilityChange,
  onTaskEdit,
  onTaskDelete,
  onAddTask,
}: ApiTableProps) {
  const [sortColumn, setSortColumn] = useState<string | null>(null);
  const [sortDirection, setSortDirection] = useState<SortDirection>(null);

  const handleSort = (columnId: string) => {
    if (sortColumn === columnId) {
      // 같은 컬럼 클릭: asc -> desc -> null
      if (sortDirection === 'asc') {
        setSortDirection('desc');
      } else if (sortDirection === 'desc') {
        setSortColumn(null);
        setSortDirection(null);
      }
    } else {
      setSortColumn(columnId);
      setSortDirection('asc');
    }
  };

  const sortedTasks = useMemo(() => {
    if (!sortColumn || !sortDirection) return tasks;

    return [...tasks].sort((a, b) => {
      const aValue = a[sortColumn];
      const bValue = b[sortColumn];

      if (aValue === bValue) return 0;

      const comparison = aValue < bValue ? -1 : 1;
      return sortDirection === 'asc' ? comparison : -comparison;
    });
  }, [tasks, sortColumn, sortDirection]);

  // Filter tasks based on search term
  const filteredTasks = useMemo(() => {
    console.log('🔍 Search Term:', searchTerm);
    console.log('📊 Total Tasks:', sortedTasks.length);

    if (!searchTerm || searchTerm.trim() === '') {
      return sortedTasks;
    }

    const lowerSearchTerm = searchTerm.toLowerCase();
    const filtered = sortedTasks.filter((task) => {
      // Search across all task fields
      return Object.values(task).some((value) => {
        if (value === null || value === undefined) return false;
        return String(value).toLowerCase().includes(lowerSearchTerm);
      });
    });

    console.log('✅ Filtered Tasks:', filtered.length);
    return filtered;
  }, [sortedTasks, searchTerm]);

  const visibleColumns = columns.filter((col) => col.visible);

  const renderCellContent = (task: ApiTask, columnId: string) => {
    const value = task[columnId];

    // 상태 아이콘 컬럼
    if (columnId === 'dev' || columnId === 'vv' || columnId === 'doc' || columnId === 'issue') {
      return <StatusIcon status={value as any} />;
    }

    // 상태 뱃지 컬럼
    if (columnId === 'status') {
      return value ? <StatusBadge status={value as string} /> : null;
    }

    // 엔드포인트 컬럼 (클릭 가능)
    if (columnId === 'endPoint' && value) {
      return (
        <button
          onClick={() => console.log('Navigate to endpoint:', value)}
          className="flex items-center gap-1 text-blue-400 hover:text-blue-300 hover:underline"
        >
          {value}
          <ExternalLink className="w-3 h-3" />
        </button>
      );
    }

    return value || '-';
  };

  // 각 그룹별 보이는 컬럼 개수 계산
  const getVisibleColumnsForGroup = (groupColumns: string[]) => {
    return visibleColumns.filter((col) => groupColumns.includes(col.id));
  };

  // 컬럼이 어느 그룹에 속하는지 확인
  const getColumnGroup = (columnId: string) => {
    if (COLUMN_GROUPS.productRibbon.columns.includes(columnId)) {
      return 'productRibbon';
    }
    if (COLUMN_GROUPS.api.columns.includes(columnId)) {
      return 'api';
    }
    if (COLUMN_GROUPS.pipeline.columns.includes(columnId)) {
      return 'pipeline';
    }
    return null;
  };

  // 그룹 경계에 있는지 확인 (오른쪽 테두리 적용용)
  const isGroupBoundary = (columnId: string) => {
    const visibleColumnIds = visibleColumns.map((col) => col.id);
    const currentIndex = visibleColumnIds.indexOf(columnId);

    if (currentIndex === -1 || currentIndex === visibleColumnIds.length - 1) {
      return false;
    }

    const currentGroup = getColumnGroup(columnId);
    const nextGroup = getColumnGroup(visibleColumnIds[currentIndex + 1]);

    return currentGroup !== nextGroup;
  };

  return (
    <div className="space-y-4">
      {/* 상단 액션 버튼 */}
      <div className="flex items-center justify-between">
        <div className="flex items-center gap-2">
          <Button onClick={onAddTask} size="sm" className="bg-blue-600 hover:bg-blue-700">
            <Plus className="w-4 h-4 mr-2" />
            행 추가
          </Button>
        </div>

        <DropdownMenu>
          <DropdownMenuTrigger asChild>
            <Button variant="outline" size="sm" className="border-zinc-700 text-zinc-300 hover:bg-zinc-800">
              <Settings2 className="w-4 h-4 mr-2" />
              컬럼 설정
            </Button>
          </DropdownMenuTrigger>
          <DropdownMenuContent align="end" className="w-48 bg-zinc-900 border-zinc-700">
            {columns.map((column) => (
              <DropdownMenuCheckboxItem
                key={column.id}
                checked={column.visible}
                onCheckedChange={(checked) =>
                  onColumnVisibilityChange(column.id, checked)
                }
                className="text-zinc-300"
              >
                {column.label}
              </DropdownMenuCheckboxItem>
            ))}
          </DropdownMenuContent>
        </DropdownMenu>
      </div>

      <div className="rounded-md border border-zinc-800 bg-zinc-950 overflow-x-auto">
        <Table>
          <TableHeader>
            {/* 첫 번째 헤더 행 - 카테고리 그룹 */}
            <TableRow className="bg-zinc-900 border-zinc-800 hover:bg-zinc-900">
              {Object.entries(COLUMN_GROUPS).map(([groupKey, group]) => {
                const visibleGroupColumns = getVisibleColumnsForGroup(group.columns);
                if (visibleGroupColumns.length === 0) return null;

                return (
                  <TableHead
                    key={groupKey}
                    colSpan={visibleGroupColumns.length}
                    className="text-center font-semibold border-r border-zinc-700 text-zinc-300"
                  >
                    {group.label}
                  </TableHead>
                );
              })}
              <TableHead className="w-[100px] border-l border-zinc-700 text-zinc-300" rowSpan={2}>
                작업
              </TableHead>
            </TableRow>
            {/* 두 번째 헤더 행 - 개별 컬럼 */}
            <TableRow className="bg-zinc-900 border-zinc-800 hover:bg-zinc-900">
              {visibleColumns.map((column, index) => {
                const isLastColumn = index === visibleColumns.length - 1;
                const isLastInGroup = isGroupBoundary(column.id);

                return (
                  <TableHead
                    key={column.id}
                    className={cn(
                      'cursor-pointer select-none hover:bg-zinc-800 text-zinc-300',
                      sortColumn === column.id && 'bg-zinc-800',
                      (isLastInGroup || isLastColumn) && 'border-r border-zinc-700'
                    )}
                    onClick={() => handleSort(column.id)}
                  >
                    <div className="flex items-center gap-1">
                      {column.label}
                      {sortColumn === column.id && (
                        <>
                          {sortDirection === 'asc' && <ChevronUp className="w-4 h-4" />}
                          {sortDirection === 'desc' && <ChevronDown className="w-4 h-4" />}
                        </>
                      )}
                    </div>
                  </TableHead>
                );
              })}
            </TableRow>
          </TableHeader>
          <TableBody>
            {filteredTasks.map((task) => (
              <TableRow key={task.id} className="hover:bg-zinc-900 border-zinc-800">
                {visibleColumns.map((column, index) => {
                  const isLastColumn = index === visibleColumns.length - 1;
                  const isLastInGroup = isGroupBoundary(column.id);

                  return (
                    <TableCell
                      key={column.id}
                      className={cn(
                        'text-zinc-300',
                        (isLastInGroup || isLastColumn) && 'border-r border-zinc-800'
                      )}
                    >
                      {renderCellContent(task, column.id)}
                    </TableCell>
                  );
                })}
                <TableCell className="border-l border-zinc-800">
                  <div className="flex items-center gap-1">
                    <Button
                      variant="ghost"
                      size="sm"
                      onClick={() => onTaskEdit(task)}
                      className="text-zinc-400 hover:text-zinc-200 hover:bg-zinc-800"
                    >
                      <Edit className="w-4 h-4" />
                    </Button>
                    <Button
                      variant="ghost"
                      size="sm"
                      onClick={() => onTaskDelete(task.id)}
                      className="text-red-400 hover:text-red-300 hover:bg-zinc-800"
                    >
                      <Trash2 className="w-4 h-4" />
                    </Button>
                  </div>
                </TableCell>
              </TableRow>
            ))}
          </TableBody>
        </Table>
      </div>

      {filteredTasks.length === 0 && (
        <div className="text-center py-12 text-zinc-500">
          {searchTerm ? '검색 결과가 없습니다' : '데이터가 없습니다'}
        </div>
      )}
    </div>
  );
}
