import { useState, useEffect, useCallback, useRef, useMemo, memo } from 'react';
import { Search, ChevronRight, ChevronDown, FileText, FolderClosed, FolderOpen, Plus, Pencil, Trash2, MoreVertical, GripVertical, Copy, ChevronLeft } from 'lucide-react';
import { Input } from '@/components/ui/input';
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
  pointerWithin,
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
import type { ApiEndpoint, ApiProduct } from '@/types';

interface SortableEndpointItemProps {
  endpoint: ApiEndpoint;
  isSelected: boolean;
  onSelect: (endpoint: ApiEndpoint) => void;
  onEdit: (endpoint: ApiEndpoint) => void;
  onDelete: (endpoint: ApiEndpoint) => void;
  onDuplicate: (endpoint: ApiEndpoint) => void;
  getStatusIndicator: (endpointId: string) => JSX.Element | null;
}

// ⚡ React.memo로 최적화: props가 같으면 리렌더링 방지
const SortableEndpointItem = memo(function SortableEndpointItem({
  endpoint,
  isSelected,
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
  } = useSortable({ id: endpoint.id });

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
      className={`group flex items-center gap-1 rounded text-sm ${
        isSelected
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
        {getStatusIndicator(endpoint.id)}
      </button>

      {/* Actions Menu */}
      <DropdownMenu>
        <DropdownMenuTrigger asChild>
          <button
            className={`p-0.5 rounded hover:bg-zinc-700/50 opacity-0 group-hover:opacity-100 transition-opacity ${
              isSelected ? 'opacity-100' : ''
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
  groupId: string;
  productId: string;
  groupName: string;
  isExpanded: boolean;
  onToggle: () => void;
  onAddEndpoint: () => void;
  onDelete: () => void;
  children: React.ReactNode;
}

// ⚡ React.memo로 최적화: props가 같으면 리렌더링 방지
const SortableGroupItem = memo(function SortableGroupItem({
  groupId,
  productId: _productId,
  groupName,
  isExpanded,
  onToggle,
  onAddEndpoint,
  onDelete,
  children,
}: SortableGroupItemProps) {
  const {
    attributes,
    listeners,
    setNodeRef,
    transform,
    transition,
    isDragging,
  } = useSortable({ id: groupId });

  // 🔥 성능 최적화: GPU 가속 및 will-change 사용
  const style = {
    transform: CSS.Transform.toString(transform),
    transition: isDragging ? 'none' : transition, // 드래그 중 transition 비활성화
    opacity: isDragging ? 0.5 : 1,
    willChange: isDragging ? 'transform' : 'auto', // GPU 가속
  };

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
          <FolderClosed className="w-3 h-3 text-yellow-400" />
          <span className="flex-1 text-left">{groupName}</span>
        </button>

        {/* Add Endpoint Button */}
        <button
          onClick={(e) => {
            e.stopPropagation();
            onAddEndpoint();
          }}
          className="p-1 rounded hover:bg-zinc-700/50 opacity-0 group-hover:opacity-100 transition-opacity"
          title="엔드포인트 추가"
        >
          <Plus className="w-3 h-3" />
        </button>

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
      className={`ml-4 space-y-1 min-h-[40px] rounded transition-colors ${
        isOver ? 'bg-blue-900/20 ring-2 ring-blue-500' : ''
      }`}
    >
      {children}
    </div>
  );
}

interface APIListPanelProps {
  products: ApiProduct[];
  selectedEndpoint: string | null;
  onEndpointSelect: (endpoint: ApiEndpoint) => void;
  onEndpointsChange?: () => void;
  onToggleCollapse?: () => void; // 🔥 접기/펴기 콜백
}

export function APIListPanel({ products, selectedEndpoint, onEndpointSelect, onEndpointsChange, onToggleCollapse }: APIListPanelProps) {
  const [searchTerm, setSearchTerm] = useState('');
  
  // 🔥 엔드포인트별 잠금 상태 관리
  const [endpointLocks, setEndpointLocks] = useState<Record<string, { locked: boolean; lockedBy?: string }>>({});
  const { currentUserId } = useAppStore();
  
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
  const [activeDroppableId, setActiveDroppableId] = useState<string | null>(null);
  const [isRefreshingLock, setIsRefreshingLock] = useState(false);

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
      products.forEach(product => {
        product.groups.forEach(group => {
          group.endpoints.forEach(endpoint => {
            allEndpoints.push(endpoint.id);
          });
        });
      });

      // 병렬로 모든 엔드포인트 상태 확인
      await Promise.all(allEndpoints.map(endpointId => checkLockStatus(endpointId)));
    };

    // 초기 로드 시 한 번만 체크 (주기적 체크는 제거 - 버전 로드 시에만 체크)
    checkAllLocks();
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

  const handleGroupDragEnd = async (event: DragEndEvent) => {
    const { active, over } = event;

    if (!over || active.id === over.id) {
      return;
    }

    const activeId = active.id as string;
    const overId = over.id as string;

    // 그룹 ID 형식: {productId}___group___{groupName}
    const parseGroupId = (groupId: string) => {
      const parts = groupId.split('___group___');
      if (parts.length !== 2) return null;
      return { productId: parts[0], groupName: parts[1] };
    };

    const activeGroup = parseGroupId(activeId);
    const overGroup = parseGroupId(overId);

    if (!activeGroup || !overGroup) {
      console.error('Invalid group ID format:', { activeId, overId });
      return;
    }

    // 같은 제품 내에서만 그룹 순서 변경 가능
    if (activeGroup.productId !== overGroup.productId) {
      console.log('⚠️ Groups must be in the same product');
      return;
    }

    // 제품 찾기
    const product = products.find((p) => p.id === activeGroup.productId);
    if (!product) {
      console.error('Product not found:', activeGroup.productId);
      return;
    }

    const oldIndex = product.groups.findIndex((g) => g.name === activeGroup.groupName);
    const newIndex = product.groups.findIndex((g) => g.name === overGroup.groupName);

    if (oldIndex === -1 || newIndex === -1) {
      return;
    }

    console.log('🔄 Reorder groups:', {
      product: product.id,
      from: oldIndex,
      to: newIndex,
    });

    // 순서 변경
    const reorderedGroups = arrayMove(product.groups, oldIndex, newIndex);

    // order_index 업데이트
    const updates = reorderedGroups.map((group, index) => ({
      id: group.id,
      order_index: index,
    }));

    try {
      const result = await apiClient.reorderGroups(updates);
      if (result.error) {
        throw new Error(result.error);
      }

      console.log('✅ Groups reordered successfully');

      // UI 업데이트
      if (onEndpointsChange) {
        onEndpointsChange();
      }
    } catch (error) {
      console.error('Failed to reorder groups:', error);
      alert(`❌ Reorder failed: ${error instanceof Error ? error.message : 'Unknown error'}`);
    }
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

  const handleDragEnd = async (event: DragEndEvent) => {
    const { active, over } = event;

    if (!over) {
      return;
    }

    const activeId = active.id as string;

    // 제품 드래그인지 확인
    const isProductDrag = products.some((p) => p.id === activeId);
    if (isProductDrag) {
      await handleProductDragEnd(event);
      return;
    }

    // 그룹 드래그인지 확인 (형식: {productId}___group___{groupName})
    if (activeId.includes('___group___')) {
      await handleGroupDragEnd(event);
      return;
    }

    // 엔드포인트 드래그
    const draggedEndpointId = activeId;
    
    // 드래그된 엔드포인트 찾기
    let draggedEndpoint: ApiEndpoint | null = null;
    let sourceProduct: string = '';
    let sourceGroup: string = '';
    
    for (const product of products) {
      for (const group of product.groups) {
        const endpoint = group.endpoints.find((e) => e.id === draggedEndpointId);
        if (endpoint) {
          draggedEndpoint = endpoint;
          sourceProduct = product.id;
          sourceGroup = group.name;
          break;
        }
      }
      if (draggedEndpoint) break;
    }

    if (!draggedEndpoint) {
      console.error('Dragged endpoint not found:', draggedEndpointId);
      return;
    }

    console.log('🔄 Drag end:', {
      from: `${sourceProduct}/${sourceGroup}/${draggedEndpoint.name}`,
      to: over.id,
    });

    // over가 그룹인지 엔드포인트인지 확인
    const overIdStr = String(over.id);
    
    // 그룹으로 드롭된 경우 (droppable-{product.id}___{group.name} 형식)
    if (overIdStr.startsWith('droppable-')) {
      const dropId = overIdStr.replace('droppable-', '');
      const parts = dropId.split('___');
      
      if (parts.length !== 2) {
        console.error('❌ Invalid droppable ID format:', overIdStr);
        alert(`Invalid drop target: ${overIdStr}`);
        return;
      }
      
      const targetProduct = parts[0];
      const targetGroup = parts[1];
      
      console.log('📍 Drop to group:', { targetProduct, targetGroup });
      
      // 같은 그룹이면 무시
      if (targetProduct === sourceProduct && targetGroup === sourceGroup) {
        return;
      }
      
      // 다른 그룹으로 이동
      try {
        const result = await apiClient.moveEndpoint(
          draggedEndpointId,
          targetProduct,
          targetGroup,
          0 // 맨 위로 이동
        );
        
        if (result.error) {
          throw new Error(result.error);
        }
        
        alert(`✅ Endpoint moved to ${targetGroup} in ${targetProduct}`);
        if (onEndpointsChange) {
          onEndpointsChange();
        }
      } catch (error) {
        console.error('Failed to move endpoint:', error);
        alert(`❌ Move failed: ${error instanceof Error ? error.message : 'Unknown error'}`);
      }
      return;
    }
    
    // 엔드포인트로 드롭된 경우 (같은 그룹 내 순서 변경 또는 다른 그룹으로 이동)
    const targetEndpointId = String(over.id);
    
    console.log('📍 Drop to endpoint:', targetEndpointId);
    
    // 대상 엔드포인트 찾기
    let targetProduct: string = '';
    let targetGroup: string = '';
    let targetGroupEndpoints: ApiEndpoint[] = [];
    
    for (const product of products) {
      for (const group of product.groups) {
        if (group.endpoints.some((e) => e.id === targetEndpointId)) {
          targetProduct = product.id;
          targetGroup = group.name;
          targetGroupEndpoints = group.endpoints;
          break;
        }
      }
      if (targetProduct) break;
    }
    
    if (!targetProduct) {
      console.error('❌ Target endpoint not found:', targetEndpointId);
      return;
    }
    
    console.log('📍 Target location:', { targetProduct, targetGroup });
    
    // 다른 그룹으로 이동하는 경우
    if (targetProduct !== sourceProduct || targetGroup !== sourceGroup) {
      const targetIndex = targetGroupEndpoints.findIndex((e) => e.id === targetEndpointId);
      
      console.log('🔀 Moving to different group at index:', targetIndex);
      
      try {
        const result = await apiClient.moveEndpoint(
          draggedEndpointId,
          targetProduct,
          targetGroup,
          targetIndex
        );
        
        if (result.error) {
          throw new Error(result.error);
        }
        
        alert(`✅ Endpoint moved to ${targetGroup} in ${targetProduct}`);
        if (onEndpointsChange) {
          onEndpointsChange();
        }
      } catch (error) {
        console.error('Failed to move endpoint:', error);
        alert(`❌ Move failed: ${error instanceof Error ? error.message : 'Unknown error'}`);
      }
      return;
    }
    
    // 같은 그룹 내 순서 변경
    console.log('🔄 Reordering within same group');
    
    if (draggedEndpointId === targetEndpointId) {
      console.log('⚠️ Same endpoint, no action');
      return;
    }
    
    const oldIndex = targetGroupEndpoints.findIndex((e) => e.id === draggedEndpointId);
    const newIndex = targetGroupEndpoints.findIndex((e) => e.id === targetEndpointId);
    
    if (oldIndex === -1 || newIndex === -1) {
      console.error('❌ Index not found:', { oldIndex, newIndex });
      return;
    }
    
    console.log('📊 Reorder:', { from: oldIndex, to: newIndex });
    
    // 순서 변경
    const reorderedEndpoints = arrayMove(targetGroupEndpoints, oldIndex, newIndex);
    
    // order_index 업데이트
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
      
      // UI 업데이트
      if (onEndpointsChange) {
        onEndpointsChange();
      }
    } catch (error) {
      console.error('Failed to reorder endpoints:', error);
      alert(`❌ Reorder failed: ${error instanceof Error ? error.message : 'Unknown error'}`);
    }
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
      <ScrollArea className="flex-1">
        <DndContext
          sensors={sensors}
          collisionDetection={pointerWithin}
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
                {/* Groups */}
                {expandedProducts.has(product.id) && (
                  <SortableContext
                    items={product.groups.map((g) => g.id)}
                    strategy={verticalListSortingStrategy}
                  >
                    <div className="ml-4 space-y-1">
                      {product.groups.map((group) => {
                        return (
                          <SortableGroupItem
                            key={group.id}
                            groupId={group.id}
                            productId={product.id}
                            groupName={group.name}
                            isExpanded={expandedGroups.has(group.id)}
                            onToggle={() => toggleGroup(group.id)}
                            onAddEndpoint={() => handleAddEndpoint(product.id, group.id)}
                            onDelete={() => handleDeleteGroup(group.id, group.name, group.endpoints)}
                          >

                      {/* Endpoints */}
                      {expandedGroups.has(group.id) && (
                        <DroppableGroup
                          id={`droppable-${product.id}___${group.name}`}
                          isOver={activeDroppableId === `droppable-${product.id}___${group.name}`}
                        >
                          {/* Endpoints List with Drag & Drop */}
                          <SortableContext
                            items={group.endpoints
                              .filter((endpoint) =>
                                endpoint.name.toLowerCase().includes(searchTerm.toLowerCase())
                              )
                              .map((e) => e.id)}
                            strategy={verticalListSortingStrategy}
                          >
                            {group.endpoints.length === 0 ? (
                              <div className="px-2 py-3 text-xs text-zinc-500 text-center">
                                엔드포인트 없음
                              </div>
                            ) : (
                              group.endpoints
                                .filter((endpoint) =>
                                  endpoint.name.toLowerCase().includes(searchTerm.toLowerCase())
                                )
                                .map((endpoint) => (
                                  <SortableEndpointItem
                                    key={endpoint.id}
                                    endpoint={endpoint}
                                    isSelected={selectedEndpoint === endpoint.id}
                                    onSelect={onEndpointSelect}
                                    onEdit={(e) => handleEditEndpoint(e, product.id, group.id)}
                                    onDelete={handleDeleteEndpoint}
                                    onDuplicate={handleDuplicateEndpoint}
                                    getStatusIndicator={getStatusIndicator}
                                  />
                                ))
                            )}
                          </SortableContext>
                        </DroppableGroup>
                      )}
                          </SortableGroupItem>
                        );
                      })}
                    </div>
                  </SortableContext>
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
        onSuccess={handleDialogSuccess}
      />
    </div>
  );
}
