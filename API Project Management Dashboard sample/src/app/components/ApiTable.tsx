import { useState, useMemo } from "react";
import {
  Table,
  TableBody,
  TableCell,
  TableHead,
  TableHeader,
  TableRow,
} from "@/app/components/ui/table";
import { Button } from "@/app/components/ui/button";
import {
  DropdownMenu,
  DropdownMenuCheckboxItem,
  DropdownMenuContent,
  DropdownMenuTrigger,
  DropdownMenuSeparator,
  DropdownMenuLabel,
  DropdownMenuItem,
} from "@/app/components/ui/dropdown-menu";
import { Input } from "@/app/components/ui/input";
import {
  Popover,
  PopoverContent,
  PopoverTrigger,
} from "@/app/components/ui/popover";
import { Badge } from "@/app/components/ui/badge";
import { StatusIcon, StatusBadge } from "@/app/components/StatusIcon";
import { ApiTask } from "@/app/data/sampleData";
import {
  ChevronUp,
  ChevronDown,
  Settings2,
  ExternalLink,
  Edit,
  Trash2,
  Plus,
  Filter,
  X,
  Search,
  CheckSquare,
} from "lucide-react";
import { cn } from "@/app/components/ui/utils";

interface Column {
  id: string;
  label: string;
  visible: boolean;
}

interface ApiTableProps {
  tasks: ApiTask[];
  columns: Column[];
  onColumnVisibilityChange: (columnId: string, visible: boolean) => void;
  onTaskEdit: (task: ApiTask) => void;
  onTaskDelete: (taskId: string) => void;
  onAddTask: () => void;
  onAddColumn: () => void;
}

type SortDirection = "asc" | "desc" | null;

interface FilterConfig {
  columnId: string;
  columnLabel: string;
  selectedValues: string[]; // 체크된 항목들 (이 항목들만 표시)
}

// 컬럼 그룹 정의
const COLUMN_GROUPS = {
  productRibbon: {
    label: "Product Ribbon",
    columns: ["tab", "group", "sub1"],
  },
  api: {
    label: "API",
    columns: ["sub2", "sub3", "seg1", "seg2", "endPoint"],
  },
  pipeline: {
    label: "Pipeline",
    columns: ["dev", "vv", "doc", "issue", "status", "charge", "remark"],
  },
};

// 기본 컬럼 ID 목록
const DEFAULT_COLUMN_IDS = [
  ...COLUMN_GROUPS.productRibbon.columns,
  ...COLUMN_GROUPS.api.columns,
  ...COLUMN_GROUPS.pipeline.columns,
];

export default function ApiTable({
  tasks,
  columns,
  onColumnVisibilityChange,
  onTaskEdit,
  onTaskDelete,
  onAddTask,
  onAddColumn,
}: ApiTableProps) {
  const [sortColumn, setSortColumn] = useState<string | null>(null);
  const [sortDirection, setSortDirection] = useState<SortDirection>(null);
  const [filters, setFilters] = useState<FilterConfig[]>([]);

  // 커스텀 컬럼 필터링
  const customColumns = useMemo(() => {
    return columns.filter((col) => !DEFAULT_COLUMN_IDS.includes(col.id));
  }, [columns]);

  const handleSort = (columnId: string) => {
    if (sortColumn === columnId) {
      // 같은 컬럼 클릭: asc -> desc -> null
      if (sortDirection === "asc") {
        setSortDirection("desc");
      } else if (sortDirection === "desc") {
        setSortColumn(null);
        setSortDirection(null);
      }
    } else {
      setSortColumn(columnId);
      setSortDirection("asc");
    }
  };

  const sortedTasks = useMemo(() => {
    if (!sortColumn || !sortDirection) return tasks;

    return [...tasks].sort((a, b) => {
      const aValue = a[sortColumn];
      const bValue = b[sortColumn];

      if (aValue === bValue) return 0;

      const comparison = aValue < bValue ? -1 : 1;
      return sortDirection === "asc" ? comparison : -comparison;
    });
  }, [tasks, sortColumn, sortDirection]);

  const visibleColumns = columns.filter((col) => col.visible);

  const handleEndPointClick = (endPoint: string) => {
    if (endPoint) {
      // 실제로는 프로젝트의 엔드포인트로 이동
      console.log("Navigate to endpoint:", endPoint);
    }
  };

  const handleManualClick = () => {
    // 젠데스크 링크로 이동
    window.open("https://support.zendesk.com", "_blank");
  };

  const renderCellContent = (task: ApiTask, columnId: string) => {
    const value = task[columnId];

    // 상태 아이콘 컬럼
    if (columnId === "dev" || columnId === "vv" || columnId === "doc" || columnId === "issue") {
      return <StatusIcon status={value as any} />;
    }

    // 상태 뱃지 컬럼
    if (columnId === "status") {
      return value ? <StatusBadge status={value as string} /> : null;
    }

    // 엔드포인트 컬럼 (클릭 가능)
    if (columnId === "endPoint" && value) {
      return (
        <button
          onClick={() => handleEndPointClick(value as string)}
          className="flex items-center gap-1 text-blue-600 hover:text-blue-800 hover:underline"
        >
          {value}
          <ExternalLink className="w-3 h-3" />
        </button>
      );
    }

    return value || "-";
  };

  // 각 그룹별 보이는 컬럼 개수 계산
  const getVisibleColumnsForGroup = (groupColumns: string[]) => {
    return visibleColumns.filter((col) => groupColumns.includes(col.id));
  };

  // 컬럼이 어느 그룹에 속하는지 확인
  const getColumnGroup = (columnId: string) => {
    if (COLUMN_GROUPS.productRibbon.columns.includes(columnId)) {
      return "productRibbon";
    }
    if (COLUMN_GROUPS.api.columns.includes(columnId)) {
      return "api";
    }
    if (COLUMN_GROUPS.pipeline.columns.includes(columnId)) {
      return "pipeline";
    }
    return null;
  };

  // 그룹 경계에 있는지 확인 (오른쪽 테두리 적용용)
  const isGroupBoundary = (columnId: string) => {
    const visibleColumnIds = visibleColumns.map(col => col.id);
    const currentIndex = visibleColumnIds.indexOf(columnId);
    
    if (currentIndex === -1 || currentIndex === visibleColumnIds.length - 1) {
      return false;
    }

    const currentGroup = getColumnGroup(columnId);
    const nextGroup = getColumnGroup(visibleColumnIds[currentIndex + 1]);
    
    return currentGroup !== nextGroup;
  };

  const handleFilterChange = (columnId: string, selectedValues: string[]) => {
    setFilters((prevFilters) => {
      const existingFilterIndex = prevFilters.findIndex(filter => filter.columnId === columnId);
      if (existingFilterIndex > -1) {
        const newFilters = [...prevFilters];
        newFilters[existingFilterIndex] = {
          columnId,
          columnLabel: columns.find(col => col.id === columnId)?.label || "",
          selectedValues,
        };
        return newFilters;
      } else {
        return [...prevFilters, {
          columnId,
          columnLabel: columns.find(col => col.id === columnId)?.label || "",
          selectedValues,
        }];
      }
    });
  };

  const clearFilter = (columnId: string) => {
    setFilters((prevFilters) => prevFilters.filter(filter => filter.columnId !== columnId));
  };

  const clearAllFilters = () => {
    setFilters([]);
  };

  // 컬럼별 고유 값 추출 (필터 옵션용)
  const getUniqueValues = (columnId: string): string[] => {
    const values = tasks.map((task) => task[columnId]).filter(Boolean);
    return Array.from(new Set(values)) as string[];
  };

  // 상태 컬럼 여부 확인
  const isStatusColumn = (columnId: string) => {
    return ["dev", "vv", "doc", "issue"].includes(columnId);
  };

  const filteredTasks = useMemo(() => {
    return sortedTasks.filter((task) => {
      return filters.every((filter) => {
        const taskValue = String(task[filter.columnId] || "").toLowerCase();
        
        if (filter.selectedValues.length > 0) {
          // 선택된 값들만 포함
          return filter.selectedValues.includes(task[filter.columnId]);
        }
        
        return true;
      });
    });
  }, [sortedTasks, filters]);

  // 필터 UI 렌더링 (외부 관리용)
  const renderFilterUI = (filter: FilterConfig | undefined, columnId: string, column: Column) => {
    const isStatus = isStatusColumn(columnId);
    const uniqueValues = getUniqueValues(columnId);
    const currentFilter = filter || {
      columnId,
      columnLabel: column.label,
      selectedValues: [],
    };

    if (isStatus || uniqueValues.length < 20) {
      // 체크박스 필터
      return (
        <div className="space-y-3">
          {/* 체크박스 옵션 */}
          <div className="space-y-1 max-h-48 overflow-y-auto border rounded-md p-2">
            {uniqueValues.map((value) => (
              <label
                key={value}
                className="flex items-center gap-2 px-2 py-1.5 hover:bg-gray-100 rounded cursor-pointer"
              >
                <input
                  type="checkbox"
                  checked={currentFilter.selectedValues.includes(value)}
                  onChange={(e) => {
                    const newValues = e.target.checked
                      ? [...currentFilter.selectedValues, value]
                      : currentFilter.selectedValues.filter((v) => v !== value);
                    handleFilterChange(columnId, newValues);
                  }}
                  className="rounded"
                />
                <span className="text-sm">{value || "(비어있음)"}</span>
              </label>
            ))}
          </div>

          {/* 빠른 선택 */}
          {uniqueValues.length > 2 && (
            <div className="flex gap-2 text-xs">
              <button
                onClick={() => handleFilterChange(columnId, uniqueValues)}
                className="text-blue-600 hover:underline"
              >
                전체 선택
              </button>
              <button
                onClick={() => handleFilterChange(columnId, [])}
                className="text-blue-600 hover:underline"
              >
                선택 해제
              </button>
            </div>
          )}
        </div>
      );
    } else {
      // 텍스트 검색 필터
      return (
        <div className="space-y-2">
          <div className="relative">
            <Search className="absolute left-3 top-1/2 transform -translate-y-1/2 w-4 h-4 text-gray-400" />
            <Input
              placeholder="검색어를 입력하세요..."
              value={currentFilter.searchTerm}
              onChange={(e) => handleFilterChange(columnId, "search", e.target.value)}
              className="h-9 pl-9"
            />
          </div>
          <p className="text-xs text-gray-500">
            입력한 텍스트를 포함하는 항목만 표시됩니다
          </p>
        </div>
      );
    }
  };

  // 필터 표시 텍스트 생성
  const getFilterDisplayText = (filter: FilterConfig): string => {
    const uniqueValues = getUniqueValues(filter.columnId);
    const excludedValues = uniqueValues.filter(v => !filter.selectedValues.includes(v));
    
    if (filter.selectedValues.length === 0) {
      return "모두 제외";
    }
    
    if (excludedValues.length === 0) {
      return "전체";
    }
    
    // 제외된 항목 표시
    if (excludedValues.length > 3) {
      return `${excludedValues.slice(0, 3).join(", ")} 외 ${excludedValues.length - 3}개 제외`;
    }
    
    return `${excludedValues.join(", ")} 제외`;
  };

  const addNewFilter = (columnId: string) => {
    const column = columns.find(col => col.id === columnId);
    if (!column) return;

    const isStatus = isStatusColumn(columnId);
    const uniqueValues = getUniqueValues(columnId);
    const type = (isStatus || uniqueValues.length < 20) ? "include" : "search";

    handleFilterChange(columnId, type === "search" ? "" : []);
  };

  return (
    <div className="space-y-4">
      {/* 상단 액션 버튼 */}
      <div className="flex items-center justify-between">
        <div className="flex items-center gap-2">
          <Button onClick={onAddTask} size="sm">
            <Plus className="w-4 h-4 mr-2" />
            행 추가
          </Button>
          <Button onClick={onAddColumn} variant="outline" size="sm">
            <Plus className="w-4 h-4 mr-2" />
            컬럼 추가
          </Button>
          <Button onClick={handleManualClick} variant="outline" size="sm">
            <ExternalLink className="w-4 h-4 mr-2" />
            매뉴얼
          </Button>
        </div>

        <DropdownMenu>
          <DropdownMenuTrigger asChild>
            <Button variant="outline" size="sm">
              <Settings2 className="w-4 h-4 mr-2" />
              컬럼 설정
            </Button>
          </DropdownMenuTrigger>
          <DropdownMenuContent align="end" className="w-48">
            {columns.map((column) => (
              <DropdownMenuCheckboxItem
                key={column.id}
                checked={column.visible}
                onCheckedChange={(checked) =>
                  onColumnVisibilityChange(column.id, checked)
                }
              >
                {column.label}
              </DropdownMenuCheckboxItem>
            ))}
          </DropdownMenuContent>
        </DropdownMenu>
      </div>

      {/* 필터 영역 - 가로 배치 */}
      <div className="bg-gray-50 border rounded-lg p-4">
        <div className="flex items-center gap-3 flex-wrap">
          {/* 전역 검색 */}
          <div className="relative flex-1 min-w-[200px] max-w-[300px]">
            <Search className="absolute left-3 top-1/2 transform -translate-y-1/2 w-4 h-4 text-gray-400" />
            <Input
              placeholder="전체 검색..."
              className="pl-9 h-9 bg-white"
            />
          </div>

          {/* 필터 버튼 */}
          <DropdownMenu>
            <DropdownMenuTrigger asChild>
              <Button 
                variant="default" 
                size="sm"
                className="bg-cyan-500 hover:bg-cyan-600 text-white"
              >
                <Filter className="w-4 h-4 mr-2" />
                Filters
              </Button>
            </DropdownMenuTrigger>
            <DropdownMenuContent align="start" className="w-64">
              <DropdownMenuLabel>필터 관리</DropdownMenuLabel>
              <DropdownMenuSeparator />
              {filters.length > 0 && (
                <>
                  <DropdownMenuItem onClick={clearAllFilters} className="text-red-600">
                    <X className="w-4 h-4 mr-2" />
                    모든 필터 지우기
                  </DropdownMenuItem>
                  <DropdownMenuSeparator />
                </>
              )}
              <div className="px-2 py-1 text-xs text-gray-500">
                컬럼을 선택하여 필터를 추가하세요
              </div>
            </DropdownMenuContent>
          </DropdownMenu>

          {/* 개별 컬럼 필터 드롭다운 */}
          {columns.slice(0, 8).map((column) => {
            const activeFilter = filters.find(f => f.columnId === column.id);
            const uniqueValues = getUniqueValues(column.id);
            const isStatus = isStatusColumn(column.id);
            
            // 옵션이 많으면 검색 필터로
            if (!isStatus && uniqueValues.length >= 20) {
              return null; // 검색 필터는 드롭다운에서 제외
            }

            // 필터가 없으면 모든 값이 선택된 것으로 간주
            const selectedValues = activeFilter?.selectedValues || uniqueValues;

            return (
              <DropdownMenu key={column.id}>
                <DropdownMenuTrigger asChild>
                  <Button 
                    variant="outline" 
                    size="sm"
                    className={cn(
                      "gap-2",
                      activeFilter && activeFilter.selectedValues.length < uniqueValues.length && "border-cyan-500 bg-cyan-50"
                    )}
                  >
                    {column.label}
                    <ChevronDown className="w-3 h-3 opacity-50" />
                  </Button>
                </DropdownMenuTrigger>
                <DropdownMenuContent align="start" className="w-72">
                  <div className="p-3 space-y-3">
                    <div className="flex items-center justify-between">
                      <h5 className="font-medium text-sm">{column.label}</h5>
                      {activeFilter && (
                        <Button
                          variant="ghost"
                          size="sm"
                          onClick={() => clearFilter(column.id)}
                          className="h-7 px-2 text-xs"
                        >
                          초기화
                        </Button>
                      )}
                    </div>

                    {/* 체크박스 옵션 */}
                    <div className="max-h-64 overflow-y-auto space-y-1 border rounded p-2 bg-white">
                      {uniqueValues.map((value) => {
                        const isChecked = selectedValues.includes(value);
                        return (
                          <label
                            key={value}
                            className="flex items-center gap-2 px-2 py-1.5 hover:bg-gray-50 rounded cursor-pointer"
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
                              className="rounded border-gray-300"
                            />
                            <span className="text-sm">{value || "(비어있음)"}</span>
                          </label>
                        );
                      })}
                    </div>

                    {/* 빠른 선택 */}
                    <div className="flex gap-3 pt-2 border-t">
                      <button
                        onClick={() => handleFilterChange(column.id, uniqueValues)}
                        className="text-xs text-cyan-600 hover:underline"
                      >
                        전체 선택
                      </button>
                      <button
                        onClick={() => handleFilterChange(column.id, [])}
                        className="text-xs text-cyan-600 hover:underline"
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
          <div className="flex items-center gap-2 flex-wrap mt-3 pt-3 border-t">
            <span className="text-xs text-gray-500 font-medium">활성 필터:</span>
            {filters.filter((filter) => {
              // "전체" 상태인 필터는 표시하지 않음
              const uniqueValues = getUniqueValues(filter.columnId);
              const excludedValues = uniqueValues.filter(v => !filter.selectedValues.includes(v));
              return excludedValues.length > 0 || filter.selectedValues.length === 0;
            }).map((filter) => {
              const displayText = getFilterDisplayText(filter);
              
              return (
                <Badge
                  key={filter.columnId}
                  variant="secondary"
                  className="px-2 py-1 gap-2 bg-white border border-gray-200"
                >
                  <span className="text-xs">
                    <span className="font-medium">{filter.columnLabel}</span>
                    : {displayText}
                  </span>
                  <button
                    onClick={() => clearFilter(filter.columnId)}
                    className="hover:bg-gray-200 rounded-full p-0.5"
                  >
                    <X className="w-3 h-3" />
                  </button>
                </Badge>
              );
            })}
          </div>
        )}
      </div>

      <div className="rounded-md border bg-white overflow-x-auto">
        <Table>
          <TableHeader>
            {/* 첫 번째 헤더 행 - 카테고리 그룹 */}
            <TableRow className="bg-gray-100">
              {Object.entries(COLUMN_GROUPS).map(([groupKey, group]) => {
                const visibleGroupColumns = getVisibleColumnsForGroup(group.columns);
                if (visibleGroupColumns.length === 0) return null;
                
                return (
                  <TableHead
                    key={groupKey}
                    colSpan={visibleGroupColumns.length}
                    className="text-center font-semibold border-r border-gray-300"
                  >
                    {group.label}
                  </TableHead>
                );
              })}
              {/* 커스텀 컬럼 그룹 */}
              {customColumns.filter(col => col.visible).length > 0 && (
                <TableHead
                  colSpan={customColumns.filter(col => col.visible).length}
                  className="text-center font-semibold border-r border-gray-300"
                >
                  Custom
                </TableHead>
              )}
              <TableHead className="w-[100px] border-l border-gray-300" rowSpan={2}>
                작업
              </TableHead>
            </TableRow>
            {/* 두 번째 헤더 행 - 개별 컬럼 */}
            <TableRow className="bg-gray-50">
              {visibleColumns.map((column, index) => {
                const isLastColumn = index === visibleColumns.length - 1;
                const isLastInGroup = isGroupBoundary(column.id);
                
                return (
                  <TableHead
                    key={column.id}
                    className={cn(
                      "cursor-pointer select-none hover:bg-gray-100",
                      sortColumn === column.id && "bg-blue-50",
                      (isLastInGroup || isLastColumn) && "border-r border-gray-300"
                    )}
                    onClick={() => handleSort(column.id)}
                  >
                    <div className="flex items-center gap-1">
                      {column.label}
                      {sortColumn === column.id && (
                        <>
                          {sortDirection === "asc" && (
                            <ChevronUp className="w-4 h-4" />
                          )}
                          {sortDirection === "desc" && (
                            <ChevronDown className="w-4 h-4" />
                          )}
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
              <TableRow key={task.id} className="hover:bg-gray-50">
                {visibleColumns.map((column, index) => {
                  const isLastColumn = index === visibleColumns.length - 1;
                  const isLastInGroup = isGroupBoundary(column.id);
                  
                  return (
                    <TableCell 
                      key={column.id} 
                      className={cn(
                        (isLastInGroup || isLastColumn) && "border-r border-gray-300"
                      )}
                    >
                      {renderCellContent(task, column.id)}
                    </TableCell>
                  );
                })}
                <TableCell className="border-l border-gray-300">
                  <div className="flex items-center gap-1">
                    <Button
                      variant="ghost"
                      size="sm"
                      onClick={() => onTaskEdit(task)}
                    >
                      <Edit className="w-4 h-4" />
                    </Button>
                    <Button
                      variant="ghost"
                      size="sm"
                      onClick={() => onTaskDelete(task.id)}
                    >
                      <Trash2 className="w-4 h-4 text-red-500" />
                    </Button>
                  </div>
                </TableCell>
              </TableRow>
            ))}
          </TableBody>
        </Table>
      </div>

      {filteredTasks.length === 0 && (
        <div className="text-center py-12 text-gray-500">
          데이터가 없습니다
        </div>
      )}
    </div>
  );
}