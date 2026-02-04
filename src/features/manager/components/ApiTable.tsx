import { useState, useMemo, useRef, useCallback } from 'react';
import { useVirtualizer } from '@tanstack/react-virtual';
import { Button } from '@/components/ui/button';
import { Input } from '@/components/ui/input';
import { Badge } from '@/components/ui/badge';
import {
  Tooltip,
  TooltipContent,
  TooltipProvider,
  TooltipTrigger,
} from '@/components/ui/tooltip';
import {
  DropdownMenu,
  DropdownMenuCheckboxItem,
  DropdownMenuContent,
  DropdownMenuTrigger,
  DropdownMenuSeparator,
  DropdownMenuLabel,
  DropdownMenuItem,
} from '@/components/ui/dropdown-menu';
import {
  ChevronUp,
  ChevronDown,
  Settings2,
  Edit,
  Trash2,
  Plus,
  Filter,
  X,
  Search,
  Link2,
} from 'lucide-react';
import { cn } from '@/components/ui/utils';
import { ApiTask, Column } from '../types/manager';
import { StatusIcon, StatusBadge } from './StatusIcon';
import { CategoryBadge } from './CategoryBadge';

interface ApiTableProps {
  tasks: ApiTask[];
  columns: Column[];
  onColumnVisibilityChange: (columnId: string, visible: boolean) => void;
  onTaskEdit: (task: ApiTask) => void;
  onTaskDelete: (taskId: string) => void;
  onAddTask: () => void;
  onTaskUpdate?: (task: ApiTask) => Promise<ApiTask>; // 인라인 편집용
  onNavigateToEndpoint?: (endpointId: string) => void; // 프로젝트 엔드포인트로 이동
}

// 상태 컬럼의 가능한 값들 (인라인 편집용 - 향후 사용)
// const _STATUS_OPTIONS = ['empty', 'wip', 'done', 'warning', 'na'] as const;

type SortDirection = 'asc' | 'desc' | null;

interface FilterConfig {
  columnId: string;
  columnLabel: string;
  selectedValues: string[];
}

// 컬럼 그룹 정의
const COLUMN_GROUPS = {
  productRibbon: {
    label: 'Product Ribbon',
    columns: ['product', 'tab', 'group', 'sub1', 'sub2', 'sub3'],
  },
  api: {
    label: 'API',
    columns: ['seg1', 'seg2', 'endPoint', 'mode'],
  },
  pipeline: {
    label: 'Pipeline',
    columns: ['plan', 'dev', 'vv', 'doc', 'deploy', 'issue', 'status', 'charge', 'remark'],  // deploy 추가
  },
};

export function ApiTable({
  tasks,
  columns,
  onColumnVisibilityChange,
  onTaskEdit,
  onTaskDelete,
  onAddTask,
  onTaskUpdate: _onTaskUpdate,
  onNavigateToEndpoint,
}: ApiTableProps) {
  const [sortColumn, setSortColumn] = useState<string | null>(null);
  const [sortDirection, setSortDirection] = useState<SortDirection>(null);
  const [filters, setFilters] = useState<FilterConfig[]>(() => {
    try {
      const saved = localStorage.getItem('manager-table-filters');
      return saved ? JSON.parse(saved) : [];
    } catch {
      return [];
    }
  });

  // 인라인 편집 상태
  const [_editingCell, _setEditingCell] = useState<{ taskId: string; columnId: string } | null>(null);
  const [_editValue, _setEditValue] = useState<string>('');
  // const _editInputRef = useRef<HTMLInputElement>(null);

  // 컬럼 너비 상태 (드래그 리사이즈용)
  const [columnWidths, setColumnWidths] = useState<Record<string, number>>({});
  const resizingRef = useRef<{ columnId: string; startX: number; startWidth: number } | null>(null);

  // 기본 컬럼 너비 계산
  const getDefaultColumnWidth = useCallback((columnId: string): number => {
    const isStatusCol = ['dev', 'vv', 'doc', 'deploy', 'issue', 'status', 'seg1'].includes(columnId);
    const isProductRibbonCol = ['product', 'tab', 'group', 'sub1', 'sub2', 'sub3'].includes(columnId);
    const isEndpointCol = columnId === 'endPoint';
    return isStatusCol ? 64 : isProductRibbonCol ? 100 : isEndpointCol ? 150 : 80;
  }, []);

  // 현재 컬럼 너비 (사용자 조절값 또는 기본값)
  const getColumnWidth = useCallback((columnId: string): number => {
    return columnWidths[columnId] || getDefaultColumnWidth(columnId);
  }, [columnWidths, getDefaultColumnWidth]);

  // 리사이즈 핸들러
  const handleResizeStart = useCallback((e: React.MouseEvent, columnId: string) => {
    e.preventDefault();
    e.stopPropagation();
    const startWidth = getColumnWidth(columnId);
    resizingRef.current = { columnId, startX: e.clientX, startWidth };

    const handleMouseMove = (moveEvent: MouseEvent) => {
      if (!resizingRef.current) return;
      const delta = moveEvent.clientX - resizingRef.current.startX;
      const newWidth = Math.max(50, resizingRef.current.startWidth + delta); // 최소 50px
      setColumnWidths(prev => ({ ...prev, [resizingRef.current!.columnId]: newWidth }));
    };

    const handleMouseUp = () => {
      resizingRef.current = null;
      document.removeEventListener('mousemove', handleMouseMove);
      document.removeEventListener('mouseup', handleMouseUp);
    };

    document.addEventListener('mousemove', handleMouseMove);
    document.addEventListener('mouseup', handleMouseUp);
  }, [getColumnWidth]);

  const handleSort = (columnId: string) => {
    if (sortColumn === columnId) {
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
      const aValue = a[sortColumn] ?? '';
      const bValue = b[sortColumn] ?? '';

      if (aValue === bValue) return 0;

      const comparison = aValue < bValue ? -1 : 1;
      return sortDirection === 'asc' ? comparison : -comparison;
    });
  }, [tasks, sortColumn, sortDirection]);

  // 필터 관리
  const handleFilterChange = (columnId: string, selectedValues: string[]) => {
    setFilters((prevFilters) => {
      const existingFilterIndex = prevFilters.findIndex(
        (filter) => filter.columnId === columnId
      );
      let newFilters;
      if (existingFilterIndex > -1) {
        newFilters = [...prevFilters];
        newFilters[existingFilterIndex] = {
          columnId,
          columnLabel: columns.find((col) => col.id === columnId)?.label || '',
          selectedValues,
        };
      } else {
        newFilters = [
          ...prevFilters,
          {
            columnId,
            columnLabel: columns.find((col) => col.id === columnId)?.label || '',
            selectedValues,
          },
        ];
      }
      localStorage.setItem('manager-table-filters', JSON.stringify(newFilters));
      return newFilters;
    });
  };

  const clearFilter = (columnId: string) => {
    setFilters((prevFilters) => {
      const newFilters = prevFilters.filter((filter) => filter.columnId !== columnId);
      localStorage.setItem('manager-table-filters', JSON.stringify(newFilters));
      return newFilters;
    });
  };

  const clearAllFilters = () => {
    setFilters([]);
    localStorage.removeItem('manager-table-filters');
  };

  // 모든 컬럼의 unique values를 한 번에 계산하여 캐싱
  const uniqueValuesMap = useMemo(() => {
    const map: Record<string, string[]> = {};
    columns.forEach(col => {
      const values = tasks.map(task => task[col.id]).filter(Boolean);
      map[col.id] = Array.from(new Set(values)) as string[];
    });
    return map;
  }, [tasks, columns]);

  const getUniqueValues = (columnId: string): string[] => {
    return uniqueValuesMap[columnId] || [];
  };

  const isStatusColumn = (columnId: string) => {
    return ['dev', 'vv', 'doc', 'deploy', 'issue'].includes(columnId);
  };

  const filteredTasks = useMemo(() => {
    return sortedTasks.filter((task) => {
      return filters.every((filter) => {
        if (filter.selectedValues.length > 0) {
          return filter.selectedValues.includes(task[filter.columnId] ?? '');
        }
        return true;
      });
    });
  }, [sortedTasks, filters]);

  const getFilterDisplayText = (filter: FilterConfig): string => {
    const uniqueValues = getUniqueValues(filter.columnId);
    const excludedValues = uniqueValues.filter(
      (v) => !filter.selectedValues.includes(v)
    );

    if (filter.selectedValues.length === 0) {
      return '모두 제외';
    }

    if (excludedValues.length === 0) {
      return '전체';
    }

    if (excludedValues.length > 3) {
      return `${excludedValues.slice(0, 3).join(', ')} 외 ${excludedValues.length - 3
        }개 제외`;
    }

    return `${excludedValues.join(', ')} 제외`;
  };

  const visibleColumns = useMemo(() =>
    columns.filter((col) => col.visible),
    [columns]
  );

  const renderCellContent = (task: ApiTask, columnId: string) => {
    const value = task[columnId];

    // plan, dev, vv, doc, deploy, issue는 상태 아이콘으로 표시
    if (columnId === 'plan' || columnId === 'dev' || columnId === 'vv' || columnId === 'doc' || columnId === 'deploy' || columnId === 'issue') {
      return (
        <div className="flex justify-center">
          <StatusIcon status={value as any} />
        </div>
      );
    }

    if (columnId === 'status') {
      return value ? (
        <div className="flex justify-center">
          <StatusBadge status={value as string} />
        </div>
      ) : null;
    }

    if (columnId === 'seg1') {
      return value ? (
        <div className="flex justify-center">
          <CategoryBadge category={value as string} />
        </div>
      ) : null;
    }

    // endPoint 컬럼: linkedEndpointId가 있으면 링크 아이콘 표시 + 툴팁
    if (columnId === 'endPoint') {
      return (
        <Tooltip delayDuration={300}>
          <TooltipTrigger asChild>
            <div className="flex items-center gap-1 justify-center cursor-default">
              <span className="truncate">{value || ''}</span>
              {task.linkedEndpointId && onNavigateToEndpoint && (
                <button
                  onClick={(e) => {
                    e.stopPropagation();
                    onNavigateToEndpoint(task.linkedEndpointId!);
                  }}
                  className="flex-shrink-0 p-0.5 rounded hover:bg-cyan-600/20 transition-colors"
                  title="프로젝트 탭에서 열기"
                >
                  <Link2 className="w-3.5 h-3.5 text-cyan-400" />
                </button>
              )}
            </div>
          </TooltipTrigger>
          {value && (
            <TooltipContent side="top" className="bg-zinc-800 text-zinc-100 border-zinc-700">
              <p className="max-w-xs break-words">{value}</p>
            </TooltipContent>
          )}
        </Tooltip>
      );
    }

    // Product Ribbon 컬럼들은 툴팁과 말줄임표 적용
    if (['product', 'tab', 'group', 'sub1', 'sub2', 'sub3'].includes(columnId)) {
      if (!value) return '';

      // sub2, sub3는 왼쪽 정렬, 나머지는 가운데 정렬
      const isLeftAligned = ['sub2', 'sub3'].includes(columnId);

      return (
        <Tooltip delayDuration={300}>
          <TooltipTrigger asChild>
            <div className={cn("truncate cursor-default", isLeftAligned && "text-left")}>
              {value}
            </div>
          </TooltipTrigger>
          <TooltipContent side="top" className="bg-zinc-800 text-zinc-100 border-zinc-700">
            <p className="max-w-xs break-words">{value}</p>
          </TooltipContent>
        </Tooltip>
      );
    }

    return value || '';
  };

  const getVisibleColumnsForGroup = (groupColumns: string[]) => {
    return visibleColumns.filter((col) => groupColumns.includes(col.id));
  };

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

  // 가상화를 위한 컨테이너 ref
  const tableContainerRef = useRef<HTMLDivElement>(null);

  // 행 가상화 설정
  const rowVirtualizer = useVirtualizer({
    count: filteredTasks.length,
    getScrollElement: () => tableContainerRef.current,
    estimateSize: () => 41, // 각 행의 예상 높이 (px)
    overscan: 10, // 스크롤 시 추가로 렌더링할 행 수
  });

  return (
    <TooltipProvider>
      <div className="flex flex-col h-full gap-4">
        {/* 상단 액션 버튼 */}
        <div className="flex items-center justify-between">
          <div className="flex items-center gap-2">
            <Button onClick={onAddTask} size="sm" className="bg-blue-600 hover:bg-blue-700 text-white">
              <Plus className="w-4 h-4 mr-2" />
              행 추가
            </Button>
          </div>

          <DropdownMenu>
            <DropdownMenuTrigger className="inline-flex items-center justify-center gap-2 rounded-md text-sm font-medium transition-colors focus-visible:outline-none focus-visible:ring-2 focus-visible:ring-ring disabled:pointer-events-none disabled:opacity-50 border border-zinc-700 bg-transparent hover:bg-zinc-800 text-zinc-300 hover:text-zinc-100 h-8 px-3">
              <Settings2 className="w-4 h-4" />
              컬럼 설정
            </DropdownMenuTrigger>
            <DropdownMenuContent align="end" className="w-48 bg-zinc-900 border-zinc-700 text-zinc-100">
              {columns.map((column) => (
                <DropdownMenuCheckboxItem
                  key={column.id}
                  checked={column.visible}
                  onCheckedChange={(checked) =>
                    onColumnVisibilityChange(column.id, checked)
                  }
                  className="text-zinc-200 hover:bg-zinc-800 focus:bg-zinc-800"
                >
                  {column.label}
                </DropdownMenuCheckboxItem>
              ))}
            </DropdownMenuContent>
          </DropdownMenu>
        </div>

        {/* 필터 영역 */}
        <div className="bg-zinc-900 border border-zinc-800 rounded-lg p-4">
          <div className="flex items-center gap-3 flex-wrap">
            {/* 전역 검색 */}
            <div className="relative flex-1 min-w-[200px] max-w-[300px]">
              <Search className="absolute left-3 top-1/2 transform -translate-y-1/2 w-4 h-4 text-zinc-400" />
              <Input
                placeholder="전체 검색..."
                className="pl-9 h-9 bg-zinc-950 border-zinc-700 text-zinc-100 placeholder:text-zinc-500"
              />
            </div>

            {/* 필터 버튼 */}
            <DropdownMenu>
              <DropdownMenuTrigger className="inline-flex items-center justify-center gap-2 rounded-md text-sm font-medium transition-colors focus-visible:outline-none focus-visible:ring-2 focus-visible:ring-ring disabled:pointer-events-none disabled:opacity-50 bg-cyan-600 hover:bg-cyan-700 text-white h-8 px-3">
                <Filter className="w-4 h-4" />
                Filters
              </DropdownMenuTrigger>
              <DropdownMenuContent align="start" className="w-64 bg-zinc-900 border-zinc-700 text-zinc-100">
                <DropdownMenuLabel className="text-zinc-200">필터 관리</DropdownMenuLabel>
                <DropdownMenuSeparator className="bg-zinc-700" />
                {filters.length > 0 && (
                  <>
                    <DropdownMenuItem
                      onClick={clearAllFilters}
                      className="text-red-400 hover:text-red-300 hover:bg-zinc-800 focus:text-red-300 focus:bg-zinc-800"
                    >
                      <X className="w-4 h-4 mr-2" />
                      모든 필터 지우기
                    </DropdownMenuItem>
                    <DropdownMenuSeparator className="bg-zinc-700" />
                  </>
                )}
                <div className="px-2 py-1 text-xs text-zinc-400">
                  컬럼을 선택하여 필터를 추가하세요
                </div>
              </DropdownMenuContent>
            </DropdownMenu>

            {/* 개별 컬럼 필터 드롭다운 */}
            {visibleColumns.map((column) => {
              const activeFilter = filters.find((f) => f.columnId === column.id);
              const uniqueValues = getUniqueValues(column.id);
              const isStatus = isStatusColumn(column.id);
              const isProductRibbon = ['product', 'tab', 'group', 'sub1', 'sub2', 'sub3'].includes(column.id);

              // Status와 Product Ribbon 컬럼은 항상 표시, 그 외는 20개 미만일 때만 표시
              if (!isStatus && !isProductRibbon && uniqueValues.length >= 20) {
                return null;
              }

              const selectedValues = activeFilter?.selectedValues || uniqueValues;

              return (
                <DropdownMenu key={column.id}>
                  <DropdownMenuTrigger
                    className={cn(
                      'inline-flex items-center justify-center gap-2 rounded-md text-sm font-medium transition-colors focus-visible:outline-none focus-visible:ring-2 focus-visible:ring-ring disabled:pointer-events-none disabled:opacity-50 border h-8 px-3',
                      'border-zinc-700 text-zinc-100 bg-zinc-800 hover:bg-zinc-700 hover:text-white',
                      activeFilter &&
                      activeFilter.selectedValues.length < uniqueValues.length &&
                      'border-cyan-500 bg-cyan-900 text-cyan-100'
                    )}
                  >
                    {column.label}
                    <ChevronDown className="w-3 h-3 opacity-70" />
                  </DropdownMenuTrigger>
                  <DropdownMenuContent align="start" className="w-72 bg-zinc-900 border-zinc-700 text-zinc-100">
                    <div className="p-3 space-y-3">
                      <div className="flex items-center justify-between">
                        <h5 className="font-medium text-sm text-zinc-200">{column.label}</h5>
                        {activeFilter && (
                          <Button
                            variant="ghost"
                            size="sm"
                            onClick={() => clearFilter(column.id)}
                            className="h-7 px-2 text-xs text-zinc-400 hover:text-zinc-200 hover:bg-zinc-800"
                          >
                            초기화
                          </Button>
                        )}
                      </div>

                      {/* 체크박스 옵션 */}
                      <div className="max-h-64 overflow-y-auto space-y-1 border border-zinc-700 rounded p-2 bg-zinc-950">
                        {uniqueValues.map((value) => {
                          const isChecked = selectedValues.includes(value);
                          return (
                            <label
                              key={value}
                              className="flex items-center gap-2 px-2 py-1.5 hover:bg-zinc-800 rounded cursor-pointer text-zinc-200"
                            >
                              <input
                                type="checkbox"
                                checked={isChecked}
                                onChange={(e) => {
                                  const newValues = e.target.checked
                                    ? [...selectedValues, value]
                                    : selectedValues.filter((v) => v !== value);
                                  handleFilterChange(column.id, newValues);
                                }}
                                className="rounded border-zinc-600 bg-zinc-800 checked:bg-cyan-600"
                              />
                              <span className="text-sm text-zinc-200">
                                {value || '(비어있음)'}
                              </span>
                            </label>
                          );
                        })}
                      </div>

                      {/* 빠른 선택 */}
                      <div className="flex gap-3 pt-2 border-t border-zinc-700">
                        <button
                          onClick={() => handleFilterChange(column.id, uniqueValues)}
                          className="text-xs text-cyan-400 hover:text-cyan-300 hover:underline"
                        >
                          전체 선택
                        </button>
                        <button
                          onClick={() => handleFilterChange(column.id, [])}
                          className="text-xs text-cyan-400 hover:text-cyan-300 hover:underline"
                        >
                          선택 해제
                        </button>
                      </div>
                    </div>
                  </DropdownMenuContent>
                </DropdownMenu>
              );
            })}
          </div>

          {/* 활성 필터 칩 표시 */}
          {filters.length > 0 && (
            <div className="flex items-center gap-2 flex-wrap mt-3 pt-3 border-t border-zinc-800">
              <span className="text-xs text-zinc-500 font-medium">활성 필터:</span>
              {filters
                .filter((filter) => {
                  const uniqueValues = getUniqueValues(filter.columnId);
                  const excludedValues = uniqueValues.filter(
                    (v) => !filter.selectedValues.includes(v)
                  );
                  return excludedValues.length > 0 || filter.selectedValues.length === 0;
                })
                .map((filter) => {
                  const displayText = getFilterDisplayText(filter);

                  return (
                    <Badge
                      key={filter.columnId}
                      variant="secondary"
                      className="px-2 py-1 gap-2 bg-zinc-800 border border-zinc-700 text-zinc-300"
                    >
                      <span className="text-xs">
                        <span className="font-medium">{filter.columnLabel}</span>: {displayText}
                      </span>
                      <button
                        onClick={() => clearFilter(filter.columnId)}
                        className="hover:bg-zinc-700 rounded-full p-0.5"
                      >
                        <X className="w-3 h-3" />
                      </button>
                    </Badge>
                  );
                })}
            </div>
          )}
        </div>

        <div
          ref={tableContainerRef}
          className="flex-1 min-h-0 rounded-md border border-zinc-800 bg-zinc-950 overflow-auto relative"
        >
          {/* 헤더 - sticky로 고정 */}
          <div className="sticky top-0 z-20 bg-zinc-800">
            {/* 그룹 헤더 행 */}
            <div className="flex border-b border-zinc-700">
              {Object.entries(COLUMN_GROUPS).map(([groupKey, group]) => {
                const visibleGroupColumns = getVisibleColumnsForGroup(group.columns);
                if (visibleGroupColumns.length === 0) return null;

                // 그룹의 총 너비 계산 (사용자 조절값 반영)
                const groupWidth = visibleGroupColumns.reduce((sum, col) => {
                  return sum + getColumnWidth(col.id);
                }, 0);

                return (
                  <div
                    key={groupKey}
                    className="text-center font-semibold border-r border-zinc-600 text-zinc-100 bg-zinc-800 h-10 flex items-center justify-center whitespace-nowrap flex-shrink-0"
                    style={{ width: `${groupWidth}px` }}
                  >
                    {group.label}
                  </div>
                );
              })}
              <div
                className="border-l border-zinc-600 text-zinc-100 text-center bg-zinc-800 h-10 flex items-center justify-center whitespace-nowrap flex-shrink-0"
                style={{ width: '100px' }}
              >
                작업
              </div>
            </div>

            {/* 컬럼 헤더 행 */}
            <div className="flex border-b border-zinc-700">
              {visibleColumns.map((column, index) => {
                const isLastColumn = index === visibleColumns.length - 1;
                const isLastInGroup = isGroupBoundary(column.id);
                const colWidth = getColumnWidth(column.id);

                return (
                  <div
                    key={column.id}
                    className={cn(
                      'relative select-none text-zinc-100 font-medium text-center bg-zinc-800 h-10 px-2 flex items-center justify-center whitespace-nowrap overflow-hidden text-ellipsis flex-shrink-0 group',
                      sortColumn === column.id && 'bg-zinc-700 text-white',
                      (isLastInGroup || isLastColumn) && 'border-r border-zinc-600'
                    )}
                    style={{ width: `${colWidth}px` }}
                  >
                    <div
                      className="flex items-center justify-center gap-1 cursor-pointer hover:bg-zinc-700 flex-1 h-full"
                      onClick={() => handleSort(column.id)}
                    >
                      <span className="truncate">{column.label}</span>
                      {sortColumn === column.id && (
                        <>
                          {sortDirection === 'asc' && <ChevronUp className="w-4 h-4 flex-shrink-0" />}
                          {sortDirection === 'desc' && <ChevronDown className="w-4 h-4 flex-shrink-0" />}
                        </>
                      )}
                    </div>
                    {/* 리사이즈 핸들 */}
                    <div
                      className="absolute right-0 top-0 h-full w-2 cursor-col-resize hover:bg-cyan-500 opacity-0 group-hover:opacity-100 transition-opacity"
                      onMouseDown={(e) => handleResizeStart(e, column.id)}
                    />
                  </div>
                );
              })}
              <div
                className="bg-zinc-800 h-10 flex-shrink-0"
                style={{ width: '100px' }}
              />
            </div>
          </div>

          {/* 가상화된 테이블 본문 */}
          <div
            style={{
              height: `${rowVirtualizer.getTotalSize()}px`,
              position: 'relative',
              width: '100%',
            }}
          >
            {rowVirtualizer.getVirtualItems().map((virtualRow) => {
              const task = filteredTasks[virtualRow.index];
              return (
                <div
                  key={task.id}
                  className="flex items-center hover:bg-zinc-900 border-b border-zinc-800 transition-colors"
                  style={{
                    position: 'absolute',
                    top: 0,
                    left: 0,
                    width: '100%',
                    height: `${virtualRow.size}px`,
                    transform: `translateY(${virtualRow.start}px)`,
                  }}
                >
                  {visibleColumns.map((column, index) => {
                    const isLastColumn = index === visibleColumns.length - 1;
                    const isLastInGroup = isGroupBoundary(column.id);
                    const colWidth = getColumnWidth(column.id);

                    return (
                      <div
                        key={column.id}
                        className={cn(
                          'text-zinc-300 text-center p-2 flex-shrink-0 overflow-hidden text-ellipsis whitespace-nowrap',
                          (isLastInGroup || isLastColumn) && 'border-r border-zinc-800'
                        )}
                        style={{ width: `${colWidth}px` }}
                      >
                        {renderCellContent(task, column.id)}
                      </div>
                    );
                  })}
                  <div
                    className="border-l border-zinc-800 text-center p-2 flex-shrink-0 flex items-center justify-center gap-1"
                    style={{ width: '100px' }}
                  >
                    <Button
                      variant="ghost"
                      size="sm"
                      onClick={() => onTaskEdit(task)}
                      className="text-zinc-400 hover:text-zinc-200 hover:bg-zinc-800 h-7 w-7 p-0"
                    >
                      <Edit className="w-4 h-4" />
                    </Button>
                    <Button
                      variant="ghost"
                      size="sm"
                      onClick={() => onTaskDelete(task.id)}
                      className="text-red-400 hover:text-red-300 hover:bg-zinc-800 h-7 w-7 p-0"
                    >
                      <Trash2 className="w-4 h-4" />
                    </Button>
                  </div>
                </div>
              );
            })}
          </div>
        </div>

        {filteredTasks.length === 0 && (
          <div className="text-center py-12 text-zinc-500">
            {filters.length > 0 ? '필터 조건에 맞는 데이터가 없습니다' : '데이터가 없습니다'}
          </div>
        )}
      </div>
    </TooltipProvider>
  );
}


