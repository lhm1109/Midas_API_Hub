import { useState, useEffect, useCallback, useRef, useMemo, memo } from 'react';
import { Search, ChevronRight, ChevronDown, FileText, FolderClosed, FolderOpen, Plus, Pencil, Trash2, MoreVertical, GripVertical, Copy, ChevronLeft, Link2 } from 'lucide-react';
import { Input } from '@/components/ui/input';
import { Button } from '@/components/ui/button';
import {
  Dialog,
  DialogContent,
  DialogDescription,
  DialogFooter,
  DialogHeader,
  DialogTitle,
} from '@/components/ui/dialog';
import { ScrollArea } from '@/components/ui/scroll-area';
import {
  Tooltip,
  TooltipContent,
  TooltipProvider,
  TooltipTrigger,
} from '@/components/ui/tooltip';
import {
  DropdownMenu,
  DropdownMenuContent,
  DropdownMenuItem,
  DropdownMenuTrigger,
} from '@/components/ui/dropdown-menu';
import {
  DndContext,
  KeyboardSensor,
  PointerSensor,
  useSensor,
  useSensors,
  DragEndEvent,
  DragOverEvent,
  useDroppable,
  closestCorners,  // ✅ closestCenter 대신 closestCorners 사용
} from '@dnd-kit/core';
import {
  arrayMove,
  SortableContext,
  sortableKeyboardCoordinates,
  useSortable,
  verticalListSortingStrategy,
} from '@dnd-kit/sortable';
import { CSS } from '@dnd-kit/utilities';
import { EndpointDialog } from './EndpointDialog';
import { ProductGroupDialog } from './ProductGroupDialog';
import { apiClient } from '@/lib/api-client';
import { useAppStore } from '@/store/useAppStore';
import type { ApiEndpoint, ApiProduct, ApiGroup } from '@/types';
import { DndId, parseDndId } from '../utils/dndIdUtils';

interface SortableEndpointItemProps {
  sortableId: string;  // ✅ DnD용 ID (e.g. "e:xyz789")
  endpoint: ApiEndpoint;
  isSelected: boolean;
  isLinked?: boolean;  // Manager 탭과 연결 여부
  onSelect: (endpoint: ApiEndpoint) => void;
  onEdit: (endpoint: ApiEndpoint) => void;
  onDelete: (endpoint: ApiEndpoint) => void;
  onDuplicate: (endpoint: ApiEndpoint) => void;
  getStatusIndicator: (endpointId: string) => JSX.Element | null;
}

// ⚡ React.memo로 최적화: props가 같으면 리렌더링 방지
const SortableEndpointItem = memo(function SortableEndpointItem({
  sortableId,  // ✅ 추가
  endpoint,
  isSelected,
  isLinked,
  onSelect,
  onEdit,
  onDelete,
  onDuplicate,
  getStatusIndicator,
}: SortableEndpointItemProps) {
  const {
    attributes,
    listeners,
    setNodeRef,
    transform,
    transition,
    isDragging,
  } = useSortable({ id: sortableId });  // ✅ sortableId 사용

  // 🔥 성능 최적화: GPU 가속 및 will-change 사용
  const style = {
    transform: CSS.Transform.toString(transform),
    transition: isDragging ? 'none' : transition, // 드래그 중 transition 비활성화
    opacity: isDragging ? 0.5 : 1,
    willChange: isDragging ? 'transform' : 'auto', // GPU 가속
  };

  return (
    <div
      ref={setNodeRef}
      style={style}
      className={`group flex items-center gap-1 rounded text-sm ${isSelected
        ? 'bg-blue-600 text-white'
        : 'text-zinc-300 hover:bg-zinc-800'
        }`}
    >
      {/* Drag Handle */}
      <button
        {...attributes}
        {...listeners}
        className="p-0.5 cursor-grab active:cursor-grabbing hover:bg-zinc-700/50 rounded opacity-0 group-hover:opacity-100 transition-opacity"
        onClick={(e) => e.stopPropagation()}
      >
        <GripVertical className="w-3 h-3" />
      </button>

      <button
        onClick={() => onSelect(endpoint)}
        className="flex-1 flex items-center gap-2 px-2 py-1 rounded text-xs"
      >
        <FileText className="w-3 h-3" />
        <span className="flex-1 text-left">{endpoint.name}</span>
        {isLinked && (
          <Link2 className="w-3 h-3 text-cyan-400" />
        )}
        {getStatusIndicator(endpoint.id)}
      </button>

      {/* Actions Menu */}
      <DropdownMenu>
        <DropdownMenuTrigger asChild>
          <button
            className={`p-0.5 rounded hover:bg-zinc-700/50 opacity-0 group-hover:opacity-100 transition-opacity ${isSelected ? 'opacity-100' : ''
              }`}
            onClick={(e) => e.stopPropagation()}
          >
            <MoreVertical className="w-3 h-3" />
          </button>
        </DropdownMenuTrigger>
        <DropdownMenuContent align="end" className="w-40">
          <DropdownMenuItem
            onClick={(e) => {
              e.stopPropagation();
              onEdit(endpoint);
            }}
          >
            <Pencil className="w-4 h-4 mr-2" />
            Edit
          </DropdownMenuItem>
          <DropdownMenuItem
            onClick={(e) => {
              e.stopPropagation();
              onDuplicate(endpoint);
            }}
          >
            <Copy className="w-4 h-4 mr-2" />
            Duplicate
          </DropdownMenuItem>
          <DropdownMenuItem
            onClick={(e) => {
              e.stopPropagation();
              onDelete(endpoint);
            }}
            className="text-red-400 focus:text-red-300"
          >
            <Trash2 className="w-4 h-4 mr-2" />
            Delete
          </DropdownMenuItem>
        </DropdownMenuContent>
      </DropdownMenu>
    </div>
  );
});

// Sortable 제품 컴포넌트
interface SortableProductItemProps {
  product: ApiProduct;
  isExpanded: boolean;
  onToggle: () => void;
  onAddGroup: () => void;
  onDelete: () => void;
  children: React.ReactNode;
}

// ⚡ React.memo로 최적화: props가 같으면 리렌더링 방지
const SortableProductItem = memo(function SortableProductItem({
  product,
  isExpanded,
  onToggle,
  onAddGroup,
  onDelete,
  children,
}: SortableProductItemProps) {
  const {
    attributes,
    listeners,
    setNodeRef,
    transform,
    transition,
    isDragging,
  } = useSortable({ id: product.id });

  // 🔥 성능 최적화: GPU 가속 및 will-change 사용
  const style = {
    transform: CSS.Transform.toString(transform),
    transition: isDragging ? 'none' : transition, // 드래그 중 transition 비활성화
    opacity: isDragging ? 0.5 : 1,
    willChange: isDragging ? 'transform' : 'auto', // GPU 가속
  };

  return (
    <div ref={setNodeRef} style={style} className="mb-2">
      <div className="flex items-center gap-1 group">
        {/* Drag Handle */}
        <button
          {...attributes}
          {...listeners}
          className="p-1 cursor-grab active:cursor-grabbing hover:bg-zinc-700/50 rounded opacity-0 group-hover:opacity-100 transition-opacity"
          onClick={(e) => e.stopPropagation()}
        >
          <GripVertical className="w-4 h-4" />
        </button>

        <button
          onClick={onToggle}
          className="flex-1 flex items-center gap-2 px-2 py-1.5 hover:bg-zinc-800 rounded text-sm text-zinc-300"
        >
          {isExpanded ? (
            <ChevronDown className="w-4 h-4" />
          ) : (
            <ChevronRight className="w-4 h-4" />
          )}
          {isExpanded ? (
            <FolderOpen className="w-4 h-4 text-blue-400" />
          ) : (
            <FolderClosed className="w-4 h-4 text-blue-400" />
          )}
          <span className="flex-1 text-left">{product.name}</span>
        </button>

        {/* Add Group Button */}
        <button
          onClick={(e) => {
            e.stopPropagation();
            onAddGroup();
          }}
          className="p-1 rounded hover:bg-zinc-700/50 opacity-0 group-hover:opacity-100 transition-opacity"
          title="그룹 추가"
        >
          <Plus className="w-4 h-4" />
        </button>

        {/* Product Actions Menu */}
        <DropdownMenu>
          <DropdownMenuTrigger asChild>
            <button
              className="p-1 rounded hover:bg-zinc-700/50 opacity-0 group-hover:opacity-100 transition-opacity"
              onClick={(e) => e.stopPropagation()}
            >
              <MoreVertical className="w-4 h-4" />
            </button>
          </DropdownMenuTrigger>
          <DropdownMenuContent align="end" className="w-40">
            <DropdownMenuItem
              onClick={(e) => {
                e.stopPropagation();
                onDelete();
              }}
              className="text-red-400 focus:text-red-300"
            >
              <Trash2 className="w-4 h-4 mr-2" />
              제품 삭제
            </DropdownMenuItem>
          </DropdownMenuContent>
        </DropdownMenu>
      </div>

      {children}
    </div>
  );
});

// Sortable 그룹 컴포넌트
interface SortableGroupItemProps {
  sortableId: string;  // ✅ DnD용 ID (e.g. "g:abc123")
  groupId: string;     // ✅ 실제 group.id (API 호출용)
  productId: string;
  groupName: string;
  depth: number;
  isExpanded: boolean;
  onToggle: () => void;
  onAddEndpoint: () => void;
  onAddSubgroup: () => void;
  onDelete: () => void;
  onRename: () => void;  // 이름 변경
  children: React.ReactNode;
}

// ⚡ React.memo로 최적화: props가 같으면 리렌더링 방지
const SortableGroupItem = memo(function SortableGroupItem({
  sortableId,
  groupId: _groupId,  // 추후 API 호출용
  productId: _productId,
  groupName,
  depth,
  isExpanded,
  onToggle,
  onAddEndpoint,
  onAddSubgroup,
  onDelete,
  onRename,
  children,
}: SortableGroupItemProps) {
  const {
    attributes,
    listeners,
    setNodeRef,
    transform,
    transition,
    isDragging,
  } = useSortable({ id: sortableId });  // ✅ sortableId 사용

  // 🔥 성능 최적화: GPU 가속 및 will-change 사용
  const style = {
    transform: CSS.Transform.toString(transform),
    transition: isDragging ? 'none' : transition, // 드래그 중 transition 비활성화
    opacity: isDragging ? 0.5 : 1,
    willChange: isDragging ? 'transform' : 'auto', // GPU 가속
  };

  // depth에 따른 색상 배열
  const depthColors = ['text-yellow-400', 'text-orange-400', 'text-pink-400', 'text-purple-400', 'text-cyan-400'];
  const folderColor = depthColors[(depth - 1) % depthColors.length];

  return (
    <div ref={setNodeRef} style={style} className="mb-1">
      <div className="flex items-center gap-1 group">
        {/* Drag Handle */}
        <button
          {...attributes}
          {...listeners}
          className="p-1 cursor-grab active:cursor-grabbing hover:bg-zinc-700/50 rounded opacity-0 group-hover:opacity-100 transition-opacity"
          onClick={(e) => e.stopPropagation()}
        >
          <GripVertical className="w-3 h-3" />
        </button>

        <button
          onClick={onToggle}
          className="flex-1 flex items-center gap-2 px-2 py-1 hover:bg-zinc-800 rounded text-sm text-zinc-300"
        >
          {isExpanded ? (
            <ChevronDown className="w-3 h-3" />
          ) : (
            <ChevronRight className="w-3 h-3" />
          )}
          <FolderClosed className={`w-3 h-3 ${folderColor}`} />
          <span className="flex-1 text-left">{groupName}</span>
          {depth > 1 && (
            <span className="text-xs text-zinc-500">L{depth}</span>
          )}
        </button>

        {/* Add Dropdown Menu */}
        <DropdownMenu>
          <DropdownMenuTrigger asChild>
            <button
              className="p-1 rounded hover:bg-zinc-700/50 opacity-0 group-hover:opacity-100 transition-opacity"
              onClick={(e) => e.stopPropagation()}
              title="추가"
            >
              <Plus className="w-3 h-3" />
            </button>
          </DropdownMenuTrigger>
          <DropdownMenuContent align="end" className="w-44">
            <DropdownMenuItem
              onClick={(e) => {
                e.stopPropagation();
                onAddEndpoint();
              }}
            >
              <FileText className="w-4 h-4 mr-2" />
              엔드포인트 추가
            </DropdownMenuItem>
            {depth < 5 && (
              <DropdownMenuItem
                onClick={(e) => {
                  e.stopPropagation();
                  onAddSubgroup();
                }}
              >
                <FolderClosed className="w-4 h-4 mr-2" />
                하위 그룹 추가
              </DropdownMenuItem>
            )}
          </DropdownMenuContent>
        </DropdownMenu>

        {/* Group Actions Menu */}
        <DropdownMenu>
          <DropdownMenuTrigger asChild>
            <button
              className="p-1 rounded hover:bg-zinc-700/50 opacity-0 group-hover:opacity-100 transition-opacity"
              onClick={(e) => e.stopPropagation()}
            >
              <MoreVertical className="w-3 h-3" />
            </button>
          </DropdownMenuTrigger>
          <DropdownMenuContent align="end" className="w-40">
            <DropdownMenuItem
              onClick={(e) => {
                e.stopPropagation();
                onRename();
              }}
              className="text-zinc-200 hover:bg-zinc-700 focus:bg-zinc-700 focus:text-zinc-100"
            >
              <Pencil className="w-4 h-4 mr-2" />
              이름 변경
            </DropdownMenuItem>
            <DropdownMenuItem
              onClick={(e) => {
                e.stopPropagation();
                onDelete();
              }}
              className="text-red-400 focus:text-red-300"
            >
              <Trash2 className="w-4 h-4 mr-2" />
              그룹 삭제
            </DropdownMenuItem>
          </DropdownMenuContent>
        </DropdownMenu>
      </div>

      {children}
    </div>
  );
});

// Droppable 그룹 컴포넌트
interface DroppableGroupProps {
  id: string;
  children: React.ReactNode;
  isOver: boolean;
}

function DroppableGroup({ id, children, isOver }: DroppableGroupProps) {
  const { setNodeRef } = useDroppable({
    id,
  });

  return (
    <div
      ref={setNodeRef}
      className={`space-y-1 min-h-[32px] rounded transition-colors ${isOver ? 'bg-blue-900/20 ring-2 ring-blue-500' : ''
        }`}
    >
      {children}
    </div>
  );
}

// ==========================================
// renderGroupTree - 재귀적 그룹 렌더링 순수 함수
// ==========================================
interface RenderGroupDeps {
  expandedGroups: Set<string>;
  toggleGroup: (id: string) => void;
  handleAddEndpoint: (productId: string, groupId: string) => void;
  handleAddSubgroup: (productId: string, groupId: string) => void;
  handleDeleteGroup: (groupId: string, name: string, endpoints: ApiEndpoint[]) => void;
  handleRenameGroup: (groupId: string, currentName: string) => void;  // 추가
  selectedEndpoint: string | null;
  onEndpointSelect: (endpoint: ApiEndpoint) => void;
  handleEditEndpoint: (e: ApiEndpoint, productId: string, groupId: string) => void;
  handleDeleteEndpoint: (e: ApiEndpoint) => void;
  handleDuplicateEndpoint: (e: ApiEndpoint) => void;
  getStatusIndicator: (id: string) => JSX.Element | null;
  searchTerm: string;
  activeDroppableId: string | null;
  linkedEndpointIds?: Set<string>;  // Manager 탭과 연결된 엔드포인트 ID
}

function renderGroupTree(
  group: ApiGroup,
  productId: string,
  deps: RenderGroupDeps
): JSX.Element {
  const isExpanded = deps.expandedGroups.has(group.id);

  // ✅ 검색 중이면 자동 확장 (검색 결과 보여주기)
  const hasMatchingEndpoints = (group.endpoints ?? []).some(ep =>
    ep.name.toLowerCase().includes(deps.searchTerm.toLowerCase())
  );
  const shouldExpand = isExpanded || !!(deps.searchTerm && hasMatchingEndpoints);

  // ✅ 필터 결과 단일화 - 한 번만 계산
  const filteredSubgroups = group.subgroups ?? [];
  const filteredEndpoints = (group.endpoints ?? []).filter(ep =>
    ep.name.toLowerCase().includes(deps.searchTerm.toLowerCase())
  );

  // ✅ items는 필터된 결과로부터 생성
  const subgroupItems = filteredSubgroups.map(sg => DndId.groupItem(sg.id));
  const endpointItems = filteredEndpoints.map(ep => DndId.endpointItem(ep.id));

  return (
    <SortableGroupItem
      key={group.id}
      sortableId={DndId.groupItem(group.id)}
      groupId={group.id}
      productId={productId}
      groupName={group.name}
      depth={group.depth}
      isExpanded={shouldExpand}
      onToggle={() => deps.toggleGroup(group.id)}
      onAddEndpoint={() => deps.handleAddEndpoint(productId, group.id)}
      onAddSubgroup={() => deps.handleAddSubgroup(productId, group.id)}
      onDelete={() => deps.handleDeleteGroup(group.id, group.name, group.endpoints)}
      onRename={() => deps.handleRenameGroup(group.id, group.name)}
    >
      {shouldExpand && (
        <>
          {/* ✅ 하위 그룹 - 항상 DroppableGroup 렌더 (빈 경우도) */}
          <DroppableGroup
            id={DndId.groupContainer(productId, group.id)}
            isOver={deps.activeDroppableId === DndId.groupContainer(productId, group.id)}
          >
            <SortableContext
              id={DndId.groupContainer(productId, group.id)}
              items={subgroupItems}
              strategy={verticalListSortingStrategy}
            >
              <div className="ml-4 space-y-1">
                {subgroupItems.length === 0 ? (
                  // ✅ 빈 그룹 placeholder (드롭 가능하게)
                  <div className="py-2 px-3 text-xs text-zinc-600 text-center border border-dashed border-zinc-700 rounded opacity-50 hover:opacity-100 transition-opacity">
                    하위 그룹을 여기에 드롭
                  </div>
                ) : (
                  filteredSubgroups.map(sg =>
                    renderGroupTree(sg, productId, deps)
                  )
                )}
              </div>
            </SortableContext>
          </DroppableGroup>

          {/* ✅ 엔드포인트 */}
          <DroppableGroup
            id={DndId.endpointContainer(productId, group.id)}
            isOver={deps.activeDroppableId === DndId.endpointContainer(productId, group.id)}
          >
            <SortableContext
              id={DndId.endpointContainer(productId, group.id)}
              items={endpointItems}
              strategy={verticalListSortingStrategy}
            >
              <div className="ml-6">
                {filteredEndpoints.length === 0 ? (
                  <div className="px-2 py-3 text-xs text-zinc-500 text-center">
                    {deps.searchTerm ? '검색 결과 없음' : '엔드포인트 없음'}
                  </div>
                ) : (
                  filteredEndpoints.map(ep => (
                    <SortableEndpointItem
                      key={ep.id}
                      sortableId={DndId.endpointItem(ep.id)}
                      endpoint={ep}
                      isSelected={deps.selectedEndpoint === ep.id}
                      isLinked={deps.linkedEndpointIds?.has(ep.id)}
                      onSelect={deps.onEndpointSelect}
                      onEdit={(e) => deps.handleEditEndpoint(e, productId, group.id)}
                      onDelete={deps.handleDeleteEndpoint}
                      onDuplicate={deps.handleDuplicateEndpoint}
                      getStatusIndicator={deps.getStatusIndicator}
                    />
                  ))
                )}
              </div>
            </SortableContext>
          </DroppableGroup>
        </>
      )}
    </SortableGroupItem>
  );
}

interface APIListPanelProps {
  products: ApiProduct[];
  selectedEndpoint: string | null;
  onEndpointSelect: (endpoint: ApiEndpoint) => void;
  onEndpointsChange?: () => void;
  onToggleCollapse?: () => void; // 🔥 접기/펴기 콜백
  linkedEndpointIds?: Set<string>;  // Manager 탭과 연결된 엔드포인트 ID 목록
}

export function APIListPanel({ products, selectedEndpoint, onEndpointSelect, onEndpointsChange, onToggleCollapse, linkedEndpointIds }: APIListPanelProps) {
  const [searchTerm, setSearchTerm] = useState('');

  // 🔥 엔드포인트별 잠금 상태 관리
  const [endpointLocks, setEndpointLocks] = useState<Record<string, { locked: boolean; lockedBy?: string }>>({});
  const { currentUserId, releaseEndpointLock } = useAppStore();

  // 🔓 엔드포인트 변경 시 이전 락 해제
  const prevSelectedEndpointRef = useRef<string | null>(null);
  useEffect(() => {
    const prevEndpoint = prevSelectedEndpointRef.current;
    if (prevEndpoint && prevEndpoint !== selectedEndpoint) {
      // 이전 엔드포인트의 락 해제
      releaseEndpointLock(prevEndpoint).catch(console.error);
    }
    prevSelectedEndpointRef.current = selectedEndpoint;
  }, [selectedEndpoint, releaseEndpointLock]);

  // localStorage에서 확장 상태 로드
  const [expandedProducts, setExpandedProducts] = useState<Set<string>>(() => {
    try {
      const saved = localStorage.getItem('expandedProducts');
      return saved ? new Set(JSON.parse(saved)) : new Set(['civil-nx']);
    } catch {
      return new Set(['civil-nx']);
    }
  });

  const [expandedGroups, setExpandedGroups] = useState<Set<string>>(() => {
    try {
      const saved = localStorage.getItem('expandedGroups');
      return saved ? new Set(JSON.parse(saved)) : new Set(['db']);
    } catch {
      return new Set(['db']);
    }
  });

  const [dialogOpen, setDialogOpen] = useState(false);
  const [editingEndpoint, setEditingEndpoint] = useState<ApiEndpoint | null>(null);
  const [dialogProductId, setDialogProductId] = useState<string>('');
  const [dialogGroupId, setDialogGroupId] = useState<string>('');
  const [productGroupDialogOpen, setProductGroupDialogOpen] = useState(false);
  const [productGroupDialogType, setProductGroupDialogType] = useState<'product' | 'group'>('product');
  const [productGroupDialogProductId, setProductGroupDialogProductId] = useState<string>('');
  const [productGroupDialogParentGroupId, setProductGroupDialogParentGroupId] = useState<string | null>(null);
  const [activeDroppableId, setActiveDroppableId] = useState<string | null>(null);
  const [isRefreshingLock, setIsRefreshingLock] = useState(false);

  // Rename Dialog 상태
  const [renameDialogOpen, setRenameDialogOpen] = useState(false);
  const [renameGroupId, setRenameGroupId] = useState<string | null>(null);
  const [renameGroupName, setRenameGroupName] = useState('');

  // 🔥 현재 선택된 엔드포인트의 잠금 상태 확인 함수 (안정화)
  const checkLockStatusRef = useRef<Record<string, number>>({});

  const checkLockStatus = useCallback(async (endpointId: string) => {
    if (!endpointId) {
      return;
    }

    // 중복 호출 방지: 같은 엔드포인트를 1초 이내에 다시 호출하지 않음
    const now = Date.now();
    const lastCheck = checkLockStatusRef.current[endpointId];
    if (lastCheck && now - lastCheck < 1000) {
      return;
    }
    checkLockStatusRef.current[endpointId] = now;

    try {
      const response = await fetch(`http://localhost:9527/api/locks/endpoint/${encodeURIComponent(endpointId)}/lock`);
      if (response.ok) {
        const data = await response.json();
        setEndpointLocks((prev) => ({
          ...prev,
          [endpointId]: {
            locked: data.locked && data.lockedBy !== currentUserId,
            lockedBy: data.lockedBy,
          }
        }));
      } else {
        setEndpointLocks((prev) => ({
          ...prev,
          [endpointId]: { locked: false }
        }));
      }
    } catch (error) {
      setEndpointLocks((prev) => ({
        ...prev,
        [endpointId]: { locked: false }
      }));
    }
  }, [currentUserId]); // selectedEndpoint 제거 - 함수 파라미터로만 사용

  // 🔥 모든 엔드포인트의 초기 상태 확인 (한 번만, products가 실제로 변경될 때만)
  // ⚡ 최적화: JSON.stringify 대신 ID 배열 비교
  const productsRef = useRef<string>('');
  const productsIdsHash = useMemo(() => {
    const ids: string[] = [];
    products.forEach(p => {
      ids.push(p.id);
      p.groups.forEach(g => {
        ids.push(g.id);
        g.endpoints.forEach(e => ids.push(e.id));
      });
    });
    return ids.join('|');
  }, [products]);

  useEffect(() => {
    // products가 실제로 변경되었을 때만 실행
    if (productsRef.current === productsIdsHash) {
      return;
    }
    productsRef.current = productsIdsHash;

    const checkAllLocks = async () => {
      const allEndpoints: string[] = [];

      // 재귀적으로 모든 그룹의 엔드포인트 수집
      const collectEndpoints = (groups: typeof products[0]['groups']) => {
        groups.forEach(group => {
          group.endpoints.forEach(endpoint => {
            allEndpoints.push(endpoint.id);
          });
          // 하위 그룹도 재귀적으로 순회
          if (group.subgroups && group.subgroups.length > 0) {
            collectEndpoints(group.subgroups);
          }
        });
      };

      products.forEach(product => {
        collectEndpoints(product.groups);
      });

      // 병렬로 모든 엔드포인트 상태 확인
      await Promise.all(allEndpoints.map(endpointId => checkLockStatus(endpointId)));
    };

    // 초기 로드 시 한 번만 체크 (주기적 체크는 제거 - 버전 로드 시에만 체크)
    checkAllLocks();

    // 🔄 lock-status-changed 이벤트 구독 (VersionTab에서 Load 시 발생)
    const handleLockStatusChanged = () => {
      checkAllLocks();
    };
    window.addEventListener('lock-status-changed', handleLockStatusChanged);
    return () => {
      window.removeEventListener('lock-status-changed', handleLockStatusChanged);
    };
  }, [products, checkLockStatus]);

  // 🔥 선택된 엔드포인트는 즉시 확인
  const selectedEndpointRef = useRef<string | null>(null);
  useEffect(() => {
    if (!selectedEndpoint) {
      selectedEndpointRef.current = null;
      return;
    }

    // 선택된 엔드포인트가 변경되었을 때 즉시 확인
    if (selectedEndpointRef.current !== selectedEndpoint) {
      selectedEndpointRef.current = selectedEndpoint;
      checkLockStatus(selectedEndpoint);
    }
  }, [selectedEndpoint, checkLockStatus]);

  // 확장 상태가 변경될 때마다 localStorage에 저장
  useEffect(() => {
    localStorage.setItem('expandedProducts', JSON.stringify(Array.from(expandedProducts)));
  }, [expandedProducts]);

  useEffect(() => {
    localStorage.setItem('expandedGroups', JSON.stringify(Array.from(expandedGroups)));
  }, [expandedGroups]);

  // 🔥 성능 최적화: 드래그 센서 설정
  const sensors = useSensors(
    useSensor(PointerSensor, {
      activationConstraint: {
        distance: 5, // 8 -> 5로 줄여서 더 빠른 반응
        delay: 0, // 지연 없음
        tolerance: 5,
      },
    }),
    useSensor(KeyboardSensor, {
      coordinateGetter: sortableKeyboardCoordinates,
    })
  );

  const handleDragOver = (event: DragOverEvent) => {
    const { over } = event;
    setActiveDroppableId(over ? String(over.id) : null);
  };

  const handleProductDragEnd = async (event: DragEndEvent) => {
    const { active, over } = event;

    if (!over || active.id === over.id) {
      return;
    }

    const oldIndex = products.findIndex((p) => p.id === active.id);
    const newIndex = products.findIndex((p) => p.id === over.id);

    if (oldIndex === -1 || newIndex === -1) {
      return;
    }

    console.log('🔄 Reorder products:', { from: oldIndex, to: newIndex });

    // 순서 변경
    const reorderedProducts = arrayMove(products, oldIndex, newIndex);

    // order_index 업데이트
    const updates = reorderedProducts.map((product, index) => ({
      id: product.id,
      order_index: index,
    }));

    try {
      const result = await apiClient.reorderProducts(updates);
      if (result.error) {
        throw new Error(result.error);
      }

      console.log('✅ Products reordered successfully');

      // UI 업데이트
      if (onEndpointsChange) {
        onEndpointsChange();
      }
    } catch (error) {
      console.error('Failed to reorder products:', error);
      alert(`❌ Reorder failed: ${error instanceof Error ? error.message : 'Unknown error'}`);
    }
  };

  // ✅ 재귀적으로 그룹에서 엔드포인트 찾기 (하위 그룹 포함)
  const findEndpointInGroups = (groups: ApiGroup[], endpointId: string): { endpoint: ApiEndpoint; group: ApiGroup; productId: string } | null => {
    for (const group of groups) {
      // 현재 그룹의 엔드포인트에서 찾기
      const endpoint = group.endpoints.find((e) => e.id === endpointId);
      if (endpoint) {
        return { endpoint, group, productId: '' }; // productId는 상위에서 설정
      }
      // 하위 그룹에서 재귀적으로 찾기
      if (group.subgroups && group.subgroups.length > 0) {
        const result = findEndpointInGroups(group.subgroups, endpointId);
        if (result) return result;
      }
    }
    return null;
  };

  // ✅ 재귀적으로 그룹 ID로 그룹 찾기
  const findGroupById = (groups: ApiGroup[], groupId: string): ApiGroup | null => {
    for (const group of groups) {
      if (group.id === groupId) return group;
      if (group.subgroups && group.subgroups.length > 0) {
        const result = findGroupById(group.subgroups, groupId);
        if (result) return result;
      }
    }
    return null;
  };

  // ???ш??곸쑝濡?洹몃৯怨?遺紐? ?뺣낫 李얘린
  const findGroupWithParent = (
    groups: ApiGroup[],
    groupId: string,
    parentId: string | null
  ): { group: ApiGroup; parentId: string | null } | null => {
    for (const group of groups) {
      if (group.id === groupId) {
        return { group, parentId };
      }
      if (group.subgroups && group.subgroups.length > 0) {
        const result = findGroupWithParent(group.subgroups, groupId, group.id);
        if (result) return result;
      }
    }
    return null;
  };

  // ???ш??곸쑝濡?洹몃９ ?먯떇/??손?뺤씤 (自??ڷ? 諛⑹?)
  const isDescendantGroup = (parent: ApiGroup, candidateId: string): boolean => {
    for (const sg of parent.subgroups ?? []) {
      if (sg.id === candidateId) return true;
      if (isDescendantGroup(sg, candidateId)) return true;
    }
    return false;
  };

  const handleGroupDragEnd = async (event: DragEndEvent) => {
    const { active, over } = event;

    if (!over || active.id === over.id) {
      return;
    }

    const activeId = String(active.id);
    const overId = String(over.id);

    // ???????ID ???: g:{groupId}
    const activeParsed = parseDndId(activeId);
    const overParsed = parseDndId(overId);

    if (activeParsed.type !== 'group') {
      console.error('Invalid group drag IDs:', { activeId, overId });
      return;
    }

    // active group & parent/product info
    let activeInfo: { group: ApiGroup; parentId: string | null; product: ApiProduct } | null = null;
    let overInfo: { group: ApiGroup; parentId: string | null; product: ApiProduct } | null = null;

    for (const product of products) {
      if (!activeInfo) {
        const foundActive = findGroupWithParent(product.groups, activeParsed.itemId, null);
        if (foundActive) {
          activeInfo = { ...foundActive, product };
        }
      }
      if (overParsed.type === 'group' && !overInfo) {
        const foundOver = findGroupWithParent(product.groups, overParsed.itemId, null);
        if (foundOver) {
          overInfo = { ...foundOver, product };
        }
      }
    }

    if (!activeInfo) {
      console.error('Active group not found:', activeId);
      return;
    }

    // === Move to subgroup container (become child) ===
    if (overParsed.type === 'groupContainer') {
      const targetProductId = overParsed.productId;
      const targetParentId = overParsed.containerParentId === 'root' ? null : overParsed.containerParentId;

      if (!targetProductId) {
        console.error('Invalid group container target:', overId);
        return;
      }

      if (targetProductId !== activeInfo.product.id) {
        console.log('??? Cross-product group move not supported');
        return;
      }

      if (targetParentId === activeInfo.group.id) {
        alert('??Group cannot be moved into itself.');
        return;
      }

      if (targetParentId && isDescendantGroup(activeInfo.group, targetParentId)) {
        alert('??Cannot move a group into its own subgroup.');
        return;
      }

      if (targetParentId === activeInfo.parentId) {
        console.log('??? Same parent container, no move needed');
        return;
      }

      try {
        const result = await apiClient.moveGroup(activeInfo.group.id, targetParentId);
        if (result.error) {
          throw new Error(result.error);
        }

        console.log('??Group moved successfully');
        if (onEndpointsChange) {
          onEndpointsChange();
        }
      } catch (error) {
        console.error('Failed to move group:', error);
        alert(`??Move failed: ${error instanceof Error ? error.message : 'Unknown error'}`);
      }
      return;
    }

    // === Reorder within same parent ===
    if (overParsed.type === 'group') {
      if (!overInfo) {
        console.error('Over group not found:', overId);
        return;
      }

      if (activeInfo.product.id !== overInfo.product.id) {
        console.log('??? Cross-product group reorder not supported');
        return;
      }

      if (activeInfo.parentId !== overInfo.parentId) {
        console.log('??? Different parents - drop into subgroup container to move');
        return;
      }

      const getGroupSiblings = (product: ApiProduct, parentId: string | null) => {
        if (!parentId) return product.groups;
        const parentGroup = findGroupById(product.groups, parentId);
        return parentGroup?.subgroups ?? [];
      };

      const siblings = getGroupSiblings(activeInfo.product, activeInfo.parentId);
      const oldIndex = siblings.findIndex((g) => g.id === activeInfo.group.id);
      const newIndex = siblings.findIndex((g) => g.id === overInfo.group.id);

      if (oldIndex === -1 || newIndex === -1) {
        console.error('??Index not found for group reorder:', { oldIndex, newIndex });
        return;
      }

      console.log('?? Reorder groups:', {
        product: activeInfo.product.id,
        parent: activeInfo.parentId || 'root',
        from: oldIndex,
        to: newIndex,
      });

      const reorderedGroups = arrayMove(siblings, oldIndex, newIndex);
      const updates = reorderedGroups.map((group, index) => ({
        id: group.id,
        order_index: index,
      }));

      try {
        const result = await apiClient.reorderGroups(updates);
        if (result.error) {
          throw new Error(result.error);
        }

        console.log('??Groups reordered successfully');
        if (onEndpointsChange) {
          onEndpointsChange();
        }
      } catch (error) {
        console.error('Failed to reorder groups:', error);
        alert(`??Reorder failed: ${error instanceof Error ? error.message : 'Unknown error'}`);
      }
      return;
    }

    console.log('??? Unsupported group drop target:', overParsed.type);
  };

  const toggleProduct = (productId: string) => {
    const newExpanded = new Set(expandedProducts);
    if (newExpanded.has(productId)) {
      newExpanded.delete(productId);
    } else {
      newExpanded.add(productId);
    }
    setExpandedProducts(newExpanded);
  };

  const toggleGroup = (groupId: string) => {
    const newExpanded = new Set(expandedGroups);
    if (newExpanded.has(groupId)) {
      newExpanded.delete(groupId);
    } else {
      newExpanded.add(groupId);
    }
    setExpandedGroups(newExpanded);
  };

  const getStatusIndicator = (endpointId: string) => {
    const lockInfo = endpointLocks[endpointId];

    // 상태 정보가 없으면 초록색 (편집 가능)으로 표시
    if (!lockInfo) {
      return (
        <span className="w-2 h-2 rounded-full bg-green-500 cursor-help" title="✅ 편집 가능" />
      );
    }

    const isLocked = lockInfo.locked;
    const statusColor = isLocked ? 'bg-red-500' : 'bg-green-500';
    const tooltipText = isLocked
      ? `🔒 편집 중\n다른 사용자(${lockInfo.lockedBy || 'Unknown'})가 편집하고 있습니다.\n읽기 전용 모드입니다.`
      : '✅ 편집 가능\n이 엔드포인트를 편집할 수 있습니다.';

    return (
      <TooltipProvider>
        <Tooltip delayDuration={200}>
          <TooltipTrigger asChild>
            <span className={`w-2 h-2 rounded-full ${statusColor} cursor-help`} />
          </TooltipTrigger>
          <TooltipContent side="right" className="max-w-xs">
            <div className="space-y-1">
              <p className="font-semibold text-xs">
                {isLocked ? '🔴 편집 중' : '🟢 편집 가능'}
              </p>
              <p className="text-xs text-zinc-300 whitespace-pre-line">{tooltipText}</p>
            </div>
          </TooltipContent>
        </Tooltip>
      </TooltipProvider>
    );
  };

  const handleAddEndpoint = (productId: string, groupId: string) => {
    setEditingEndpoint(null);
    setDialogProductId(productId);
    setDialogGroupId(groupId);
    setDialogOpen(true);
  };

  const handleEditEndpoint = (endpoint: ApiEndpoint, productId: string, groupId: string) => {
    setEditingEndpoint(endpoint);
    setDialogProductId(productId);
    setDialogGroupId(groupId);
    setDialogOpen(true);
  };

  const handleDeleteEndpoint = async (endpoint: ApiEndpoint) => {
    if (!confirm(`Delete endpoint "${endpoint.name}"?\n\n⚠️ All related versions and data will also be deleted.`)) {
      return;
    }

    console.log('🗑️ Deleting endpoint:', endpoint.id, endpoint.name);

    try {
      const result = await apiClient.deleteEndpoint(endpoint.id);

      console.log('📦 Delete result:', result);

      if (result.error) {
        throw new Error(result.error);
      }

      console.log('✅ Endpoint deleted successfully');
      alert('✅ Endpoint deleted successfully.');

      // 🔥 강제로 데이터 새로고침
      if (onEndpointsChange) {
        console.log('🔄 Calling onEndpointsChange to refresh list...');
        await onEndpointsChange();
      }
    } catch (error) {
      console.error('❌ Failed to delete endpoint:', error);
      alert(`❌ Delete failed: ${error instanceof Error ? error.message : 'Unknown error'}`);
    }
  };

  const handleDuplicateEndpoint = async (endpoint: ApiEndpoint) => {
    try {
      const result = await apiClient.duplicateEndpoint(endpoint.id);
      if (result.error) {
        throw new Error(result.error);
      }
      alert(`✅ Endpoint duplicated: ${result.data?.endpoint?.name}`);
      if (onEndpointsChange) {
        onEndpointsChange();
      }
    } catch (error) {
      console.error('Failed to duplicate endpoint:', error);
      alert(`❌ Duplicate failed: ${error instanceof Error ? error.message : 'Unknown error'}`);
    }
  };

  // 그룹 이름 변경 다이얼로그 열기
  const handleRenameGroup = (groupId: string, currentName: string) => {
    setRenameGroupId(groupId);
    setRenameGroupName(currentName);
    setRenameDialogOpen(true);
  };

  // 그룹 이름 변경 확정
  const confirmRenameGroup = async () => {
    if (!renameGroupId || !renameGroupName.trim()) {
      return;
    }

    try {
      const result = await apiClient.renameGroup(renameGroupId, renameGroupName.trim());
      if (result.error) {
        throw new Error(result.error);
      }

      console.log('✅ Group renamed successfully:', renameGroupId, renameGroupName);

      // UI 업데이트
      if (onEndpointsChange) {
        await onEndpointsChange();
      }
    } catch (error) {
      console.error('❌ Failed to rename group:', error);
      alert(`❌ Rename failed: ${error instanceof Error ? error.message : 'Unknown error'}`);
    } finally {
      setRenameDialogOpen(false);
      setRenameGroupId(null);
      setRenameGroupName('');
    }
  };

  const handleDragEnd = async (event: DragEndEvent) => {
    const { active, over } = event;

    if (!over) {
      return;
    }

    const activeId = String(active.id);
    const overId = String(over.id);

    // ✅ 새로운 ID 체계 파싱
    const activeParsed = parseDndId(activeId);
    const overParsed = parseDndId(overId);

    console.log('🔄 Drag end:', { activeId, overId, activeParsed, overParsed });

    // 제품 드래그인지 확인 (제품은 아직 prefix 없음)
    const isProductDrag = products.some((p) => p.id === activeId);
    if (isProductDrag) {
      await handleProductDragEnd(event);
      return;
    }

    // ✅ 그룹 드래그 (g: prefix)
    if (activeParsed.type === 'group') {
      await handleGroupDragEnd(event);
      return;
    }

    // ✅ 엔드포인트 드래그 (e: prefix)
    if (activeParsed.type !== 'endpoint') {
      console.log('⚠️ Unknown drag type:', activeParsed.type);
      return;
    }

    const draggedEndpointId = activeParsed.itemId;

    // 드래그된 엔드포인트 찾기 (재귀적)
    let draggedEndpoint: ApiEndpoint | null = null;
    let sourceProductId: string = '';
    let sourceGroup: ApiGroup | null = null;

    for (const product of products) {
      const result = findEndpointInGroups(product.groups, draggedEndpointId);
      if (result) {
        draggedEndpoint = result.endpoint;
        sourceGroup = result.group;
        sourceProductId = product.id;
        break;
      }
    }

    if (!draggedEndpoint || !sourceGroup) {
      console.error('Dragged endpoint not found:', draggedEndpointId);
      return;
    }

    console.log('🔄 Drag from:', {
      product: sourceProductId,
      group: sourceGroup.name,
      endpoint: draggedEndpoint.name,
    });

    // ✅ over가 엔드포인트 컨테이너인 경우 (ep:productId:groupId) - 그룹으로 이동
    if (overParsed.type === 'endpointContainer') {
      const targetGroupId = overParsed.containerParentId;

      if (!targetGroupId) {
        console.error('Invalid endpoint container:', overId);
        return;
      }

      // 같은 그룹이면 무시
      if (targetGroupId === sourceGroup.id) {
        console.log('⚠️ Same group, no move needed');
        return;
      }

      // 대상 그룹 찾기
      let targetGroup: ApiGroup | null = null;
      let targetProductId: string = '';

      for (const product of products) {
        const found = findGroupById(product.groups, targetGroupId);
        if (found) {
          targetGroup = found;
          targetProductId = product.id;
          break;
        }
      }

      if (!targetGroup) {
        console.error('Target group not found:', targetGroupId);
        return;
      }

      console.log('📍 Move to group:', { targetProductId, targetGroup: targetGroup.name });

      try {
        // 새로운 move API 호출 (group_id 사용)
        const result = await apiClient.moveEndpointToGroup(
          draggedEndpointId,
          targetGroupId,
          0
        );

        if (result.error) {
          throw new Error(result.error);
        }

        console.log('✅ Endpoint moved successfully');
        if (onEndpointsChange) {
          onEndpointsChange();
        }
      } catch (error) {
        console.error('Failed to move endpoint:', error);
        alert(`❌ Move failed: ${error instanceof Error ? error.message : 'Unknown error'}`);
      }
      return;
    }

    // ✅ over가 엔드포인트인 경우 (e:endpointId) - 재정렬 또는 이동
    if (overParsed.type === 'endpoint') {
      const targetEndpointId = overParsed.itemId;

      if (draggedEndpointId === targetEndpointId) {
        console.log('⚠️ Same endpoint, no action');
        return;
      }

      // 대상 엔드포인트 찾기 (재귀적)
      let targetGroup: ApiGroup | null = null;

      for (const product of products) {
        const result = findEndpointInGroups(product.groups, targetEndpointId);
        if (result) {
          targetGroup = result.group;
          // _targetProductId = product.id; // Not currently used
          break;
        }
      }

      if (!targetGroup) {
        console.error('Target endpoint not found:', targetEndpointId);
        return;
      }

      // 같은 그룹 내 재정렬
      if (sourceGroup.id === targetGroup.id) {
        console.log('� Reordering within same group');

        const targetGroupEndpoints = targetGroup.endpoints;
        const oldIndex = targetGroupEndpoints.findIndex((e) => e.id === draggedEndpointId);
        const newIndex = targetGroupEndpoints.findIndex((e) => e.id === targetEndpointId);

        if (oldIndex === -1 || newIndex === -1) {
          console.error('❌ Index not found:', { oldIndex, newIndex });
          return;
        }

        console.log('📊 Reorder:', { from: oldIndex, to: newIndex });

        const reorderedEndpoints = arrayMove(targetGroupEndpoints, oldIndex, newIndex);
        const updates = reorderedEndpoints.map((endpoint, index) => ({
          id: endpoint.id,
          order_index: index,
        }));

        try {
          const result = await apiClient.reorderEndpoints(updates);
          if (result.error) {
            throw new Error(result.error);
          }

          console.log('✅ Reorder successful');
          if (onEndpointsChange) {
            onEndpointsChange();
          }
        } catch (error) {
          console.error('Failed to reorder endpoints:', error);
          alert(`❌ Reorder failed: ${error instanceof Error ? error.message : 'Unknown error'}`);
        }
        return;
      }

      // 다른 그룹으로 이동
      console.log('� Moving to different group');
      const targetIndex = targetGroup.endpoints.findIndex((e) => e.id === targetEndpointId);

      try {
        const result = await apiClient.moveEndpointToGroup(
          draggedEndpointId,
          targetGroup.id,
          targetIndex
        );

        if (result.error) {
          throw new Error(result.error);
        }

        console.log('✅ Endpoint moved successfully');
        if (onEndpointsChange) {
          onEndpointsChange();
        }
      } catch (error) {
        console.error('Failed to move endpoint:', error);
        alert(`❌ Move failed: ${error instanceof Error ? error.message : 'Unknown error'}`);
      }
      return;
    }

    console.log('⚠️ Unhandled drop target type:', overParsed.type);
  };

  const handleDialogSuccess = () => {
    if (onEndpointsChange) {
      onEndpointsChange();
    }
  };

  const handleAddProduct = () => {
    setProductGroupDialogType('product');
    setProductGroupDialogProductId('');
    setProductGroupDialogOpen(true);
  };

  const handleAddGroup = (productId: string) => {
    setProductGroupDialogType('group');
    setProductGroupDialogProductId(productId);
    setProductGroupDialogParentGroupId(null);
    setProductGroupDialogOpen(true);
  };

  const handleAddSubgroup = (productId: string, parentGroupId: string) => {
    setProductGroupDialogType('group');
    setProductGroupDialogProductId(productId);
    setProductGroupDialogParentGroupId(parentGroupId);
    setProductGroupDialogOpen(true);
  };

  const handleDeleteProduct = async (product: ApiProduct) => {
    const endpointCount = product.groups.reduce((sum, group) => sum + group.endpoints.length, 0);

    const message = endpointCount > 0
      ? `제품 "${product.name}"과 관련된 모든 데이터를 삭제하시겠습니까?\n\n` +
      `- 그룹: ${product.groups.length}개\n` +
      `- 엔드포인트: ${endpointCount}개\n` +
      `- 모든 버전 및 작업 데이터\n\n` +
      `⚠️ 이 작업은 되돌릴 수 없습니다.`
      : `빈 제품 "${product.name}"을 삭제하시겠습니까?`;

    if (!confirm(message)) {
      return;
    }

    console.log('🗑️ Deleting product:', product.id, product.name);

    try {
      // products 테이블에서 삭제 (CASCADE로 자동으로 관련 데이터 삭제)
      const result = await apiClient.deleteProduct(product.id);

      console.log('📦 Delete result:', result);

      if (result.error) {
        throw new Error(result.error);
      }

      console.log('✅ Product deleted successfully');
      alert('✅ 제품이 성공적으로 삭제되었습니다.');

      // 🔥 강제로 데이터 새로고침
      if (onEndpointsChange) {
        console.log('🔄 Calling onEndpointsChange to refresh list...');
        await onEndpointsChange();
      }
    } catch (error) {
      console.error('❌ Failed to delete product:', error);
      alert(`❌ Delete failed: ${error instanceof Error ? error.message : 'Unknown error'}`);
    }
  };

  const handleDeleteGroup = async (groupId: string, groupName: string, endpoints: ApiEndpoint[]) => {
    const message = endpoints.length > 0
      ? `그룹 "${groupName}"과 관련된 모든 데이터를 삭제하시겠습니까?\n\n` +
      `- 엔드포인트: ${endpoints.length}개\n` +
      `- 모든 버전 및 작업 데이터\n\n` +
      `⚠️ 이 작업은 되돌릴 수 없습니다.`
      : `빈 그룹 "${groupName}"을 삭제하시겠습니까?`;

    if (!confirm(message)) {
      return;
    }

    console.log('🗑️ Deleting group:', { groupId, groupName });

    try {
      // groups 테이블에서 삭제 (CASCADE로 자동으로 관련 데이터 삭제)
      const result = await apiClient.deleteGroup(groupId);

      console.log('📦 Delete result:', result);

      if (result.error) {
        throw new Error(result.error);
      }

      console.log('✅ Group deleted successfully');
      alert('✅ 그룹이 성공적으로 삭제되었습니다.');

      // 🔥 강제로 데이터 새로고침
      if (onEndpointsChange) {
        console.log('🔄 Calling onEndpointsChange to refresh list...');
        await onEndpointsChange();
      }
    } catch (error) {
      console.error('❌ Failed to delete group:', error);
      alert(`❌ Delete failed: ${error instanceof Error ? error.message : 'Unknown error'}`);
    }
  };

  return (
    <div className="h-full bg-zinc-900 flex flex-col">
      {/* Search */}
      <div className="p-3 border-b border-zinc-800">
        <div className="flex items-center gap-2">
          <div className="relative flex-1">
            <Search className="absolute left-2 top-1/2 -translate-y-1/2 w-4 h-4 text-zinc-500" />
            <Input
              type="text"
              placeholder="Search API"
              value={searchTerm}
              onChange={(e) => setSearchTerm(e.target.value)}
              className="pl-8 bg-zinc-800 border-zinc-700 text-sm h-8"
            />
          </div>
          {/* 🔄 전체 목록 새로고침 버튼 */}
          <button
            onClick={() => {
              console.log('🔄 Manual refresh triggered');
              if (onEndpointsChange) {
                onEndpointsChange();
              }
            }}
            className="h-8 w-8 flex items-center justify-center rounded border border-zinc-700 hover:bg-zinc-800 transition-colors"
            title="목록 새로고침"
          >
            <svg className="w-4 h-4 text-zinc-400" fill="none" stroke="currentColor" viewBox="0 0 24 24">
              <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={2} d="M4 4v5h.582m15.356 2A8.001 8.001 0 004.582 9m0 0H9m11 11v-5h-.581m0 0a8.003 8.003 0 01-15.357-2m15.357 2H15" />
            </svg>
          </button>

          {/* 🔄 잠금 상태 새로고침 버튼 */}
          <button
            onClick={() => {
              if (!selectedEndpoint) return;
              setIsRefreshingLock(true);
              checkLockStatus(selectedEndpoint).finally(() => setIsRefreshingLock(false));
            }}
            disabled={isRefreshingLock || !selectedEndpoint}
            className="h-8 w-8 flex items-center justify-center rounded border border-zinc-700 hover:bg-zinc-800 disabled:opacity-50 disabled:cursor-not-allowed transition-colors flex-shrink-0"
            title="잠금 상태 새로고침 (5분마다 자동)"
          >
            {isRefreshingLock ? (
              <div className="w-4 h-4 border-2 border-zinc-600 border-t-blue-500 rounded-full animate-spin" />
            ) : (
              <svg className="w-4 h-4 text-zinc-400" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={2} d="M12 15v2m-6 4h12a2 2 0 002-2v-6a2 2 0 00-2-2H6a2 2 0 00-2 2v6a2 2 0 002 2zm10-10V7a4 4 0 00-8 0v4h8z" />
              </svg>
            )}
          </button>

          {/* 🔥 접기 버튼 */}
          {onToggleCollapse && (
            <button
              onClick={onToggleCollapse}
              className="h-8 w-8 flex items-center justify-center rounded border border-zinc-700 hover:bg-zinc-800 transition-colors flex-shrink-0"
              title="패널 접기"
            >
              <ChevronLeft className="w-4 h-4 text-zinc-400" />
            </button>
          )}
        </div>
      </div>

      {/* API Tree */}
      <ScrollArea className="flex-1 overflow-hidden">
        <DndContext
          sensors={sensors}
          collisionDetection={closestCorners}
          onDragEnd={handleDragEnd}
          onDragOver={handleDragOver}
        >
          <div className="p-2">
            {/* Add Product Button */}
            <button
              onClick={handleAddProduct}
              className="w-full mb-2 flex items-center gap-2 px-2 py-1.5 rounded text-sm text-zinc-400 hover:bg-zinc-800 hover:text-zinc-200 transition-colors border border-dashed border-zinc-700 hover:border-zinc-600"
            >
              <Plus className="w-4 h-4" />
              <span>제품 추가</span>
            </button>

            {/* Products List with Drag & Drop */}
            <SortableContext
              items={products.map((p) => p.id)}
              strategy={verticalListSortingStrategy}
            >
              {products.map((product) => (
                <SortableProductItem
                  key={product.id}
                  product={product}
                  isExpanded={expandedProducts.has(product.id)}
                  onToggle={() => toggleProduct(product.id)}
                  onAddGroup={() => handleAddGroup(product.id)}
                  onDelete={() => handleDeleteProduct(product)}
                >
                  {/* ✅ 루트 그룹 DroppableGroup + renderGroupTree 사용 */}
                  {expandedProducts.has(product.id) && (
                    <DroppableGroup
                      id={DndId.rootGroupContainer(product.id)}
                      isOver={activeDroppableId === DndId.rootGroupContainer(product.id)}
                    >
                      <SortableContext
                        id={DndId.rootGroupContainer(product.id)}
                        items={product.groups.map(g => DndId.groupItem(g.id))}
                        strategy={verticalListSortingStrategy}
                      >
                        <div className="ml-4 space-y-1">
                          {product.groups.map((group) =>
                            renderGroupTree(group, product.id, {
                              expandedGroups,
                              toggleGroup,
                              handleAddEndpoint,
                              handleAddSubgroup,
                              handleDeleteGroup,
                              handleRenameGroup,
                              selectedEndpoint,
                              onEndpointSelect,
                              handleEditEndpoint,
                              handleDeleteEndpoint,
                              handleDuplicateEndpoint,
                              getStatusIndicator,
                              searchTerm,
                              activeDroppableId,
                              linkedEndpointIds,
                            })
                          )}
                        </div>
                      </SortableContext>
                    </DroppableGroup>
                  )}
                </SortableProductItem>
              ))}
            </SortableContext>
          </div>
        </DndContext>
      </ScrollArea>

      {/* Endpoint Dialog */}
      <EndpointDialog
        open={dialogOpen}
        onOpenChange={setDialogOpen}
        endpoint={editingEndpoint}
        productId={dialogProductId}
        groupId={dialogGroupId}
        onSuccess={handleDialogSuccess}
      />

      {/* Product/Group Dialog */}
      <ProductGroupDialog
        open={productGroupDialogOpen}
        onOpenChange={setProductGroupDialogOpen}
        type={productGroupDialogType}
        productId={productGroupDialogProductId}
        parentGroupId={productGroupDialogParentGroupId}
        onSuccess={handleDialogSuccess}
      />

      {/* Rename Group Dialog */}
      <Dialog open={renameDialogOpen} onOpenChange={setRenameDialogOpen}>
        <DialogContent className="sm:max-w-[400px] bg-zinc-900 border-zinc-700">
          <DialogHeader>
            <DialogTitle className="text-zinc-100">그룹 이름 변경</DialogTitle>
            <DialogDescription className="text-zinc-400">
              새로운 그룹 이름을 입력하세요.
            </DialogDescription>
          </DialogHeader>
          <div className="py-4">
            <Input
              value={renameGroupName}
              onChange={(e) => setRenameGroupName(e.target.value)}
              placeholder="그룹 이름"
              className="bg-zinc-800 border-zinc-600 text-zinc-100"
              onKeyDown={(e) => {
                if (e.key === 'Enter') {
                  confirmRenameGroup();
                }
              }}
              autoFocus
            />
          </div>
          <DialogFooter>
            <Button
              variant="outline"
              onClick={() => setRenameDialogOpen(false)}
              className="bg-zinc-800 border-zinc-600 text-zinc-300 hover:bg-zinc-700"
            >
              취소
            </Button>
            <Button
              onClick={confirmRenameGroup}
              disabled={!renameGroupName.trim()}
              className="bg-blue-600 hover:bg-blue-500 text-white"
            >
              변경
            </Button>
          </DialogFooter>
        </DialogContent>
      </Dialog>
    </div>
  );
}
