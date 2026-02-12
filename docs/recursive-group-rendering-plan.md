# Recursive Group Rendering Implementation Plan (v3.1 - Final)

## 📋 Overview

현재 `APIListPanel.tsx`에서 그룹을 렌더링할 때 **1단계 그룹만 표시**됩니다.
서버에서 `subgroups` 배열로 계층 구조를 전달하지만, 프론트엔드에서 재귀적으로 렌더링하지 않아 **하위 그룹이 표시되지 않습니다**.

이 문서는 **안정적인 재귀 렌더링 + 확장/축소 + DnD**를 구현하기 위한 최종 계획입니다.

---

## ⚠️ 핵심 설계 원칙

### 1. useCallback 재귀 함수 피하기
```tsx
// ❌ 잘못된 패턴
const renderGroup = useCallback(() => { ... renderGroup(...) }, [deps])

// ✅ 올바른 패턴 - 순수 함수로 분리
function renderGroupTree(group: ApiGroup, productId: string, deps: Deps): JSX.Element
```

### 2. DnD 컨텍스트는 "부모별 컨테이너"로 분리
```tsx
// ❌ 잘못된 패턴 - 전체 flatten
<SortableContext items={getAllGroupIds(product.groups)}>

// ✅ 올바른 패턴 - 부모별 DroppableGroup + SortableContext 중첩
<DroppableGroup id={containerIdForParent}>
  <SortableContext items={directChildrenIds}>
```

### 3. ID 규칙 - group.id 기반 안정적 ID
```tsx
// ❌ 잘못된 ID
id={`droppable-${productId}___${group.name}`}

// ✅ 안정적인 ID 규칙 (prefix 기반)
// g:  → 그룹 아이템
// e:  → 엔드포인트 아이템
// grp: → 그룹 컨테이너
// ep:  → 엔드포인트 컨테이너
```

### 4. sortableId와 실제 ID 분리
```tsx
// SortableGroupItem, SortableEndpointItem 모두:
interface Props {
  sortableId: string;  // DnD용 (e.g. "g:abc123", "e:xyz789")
  groupId: string;     // 또는 endpointId - API 호출용 실제 ID
}
```

### 5. 필터 결과 단일화
```tsx
// ❌ 잘못된 패턴 - items와 render에서 별도 필터링
const items = group.endpoints.filter(...).map(...)
{group.endpoints.filter(...).map(...)}  // 미스매치 가능

// ✅ 올바른 패턴 - 한 번만 필터링
const filteredEndpoints = group.endpoints.filter(...);
const items = filteredEndpoints.map(ep => DndId.endpointItem(ep.id));
{filteredEndpoints.map(ep => ...)}
```

### 6. 빈 그룹에도 DroppableGroup 항상 렌더 (⚠️ 중요)
```tsx
// ❌ 잘못된 패턴 - 빈 그룹이면 드롭 불가
{subgroupItems.length > 0 && (
  <DroppableGroup ...>...</DroppableGroup>
)}

// ✅ 올바른 패턴 - 항상 렌더, 빈 경우 placeholder
<DroppableGroup id={...}>
  <SortableContext items={subgroupItems}>
    {subgroupItems.length === 0 ? (
      <div className="py-2 text-xs text-zinc-500 text-center">하위 그룹을 여기에 드롭</div>
    ) : (
      filteredSubgroups.map(sg => renderGroupTree(sg, productId, deps))
    )}
  </SortableContext>
</DroppableGroup>
```

---

## 📝 구현 계획

### Phase 1: 타입 정의 수정

**파일:** `src/types/index.ts`

```typescript
export interface ApiGroup {
  id: string;
  name: string;
  depth: number;
  parent_group_id?: string | null;
  order_index?: number;
  subgroups: ApiGroup[];  // ✅ 재귀 타입
  endpoints: ApiEndpoint[];
}
```

---

### Phase 2: DnD ID 유틸리티 (생성 + 파싱)

**파일:** `src/features/projects/utils/dndIdUtils.ts` (새 파일)

```typescript
// ==========================================
// DnD ID 생성 유틸리티
// ==========================================
export const DndId = {
  // 그룹 정렬 컨테이너 (해당 그룹의 직계 subgroups용)
  groupContainer: (productId: string, parentGroupId: string) => 
    `grp:${productId}:${parentGroupId}`,
  
  // 루트 그룹 컨테이너 (product 직속 groups용)
  rootGroupContainer: (productId: string) => 
    `grp:${productId}:root`,
  
  // 엔드포인트 정렬 컨테이너
  endpointContainer: (productId: string, groupId: string) => 
    `ep:${productId}:${groupId}`,
  
  // 그룹 아이템
  groupItem: (groupId: string) => `g:${groupId}`,
  
  // 엔드포인트 아이템
  endpointItem: (endpointId: string) => `e:${endpointId}`,
};

// ==========================================
// DnD ID 파싱 유틸리티 (onDragEnd에서 사용)
// ==========================================
type DndItemType = 'group' | 'endpoint' | 'groupContainer' | 'endpointContainer' | 'unknown';

interface ParsedDndId {
  type: DndItemType;
  itemId: string;            // 아이템인 경우: 실제 ID (group.id 또는 endpoint.id)
  productId?: string;        // 컨테이너인 경우
  containerParentId?: string; // 컨테이너인 경우 (e.g. "root" or parentGroup.id)
}

export function parseDndId(dndId: string): ParsedDndId {
  // 그룹 아이템: g:${groupId}
  if (dndId.startsWith('g:')) {
    return { type: 'group', itemId: dndId.slice(2) };
  }
  
  // 엔드포인트 아이템: e:${endpointId}
  if (dndId.startsWith('e:')) {
    return { type: 'endpoint', itemId: dndId.slice(2) };
  }
  
  // 그룹 컨테이너: grp:${productId}:${parentGroupId}
  if (dndId.startsWith('grp:')) {
    const parts = dndId.slice(4).split(':');
    // ⚠️ 형식 검증: 정확히 2개 파트 필요
    if (parts.length !== 2) {
      console.warn(`Invalid groupContainer ID format: ${dndId}`);
      return { type: 'unknown', itemId: dndId };
    }
    return { 
      type: 'groupContainer', 
      itemId: '',  // 컨테이너는 itemId 없음
      productId: parts[0],
      containerParentId: parts[1]  // "root" 또는 실제 parentGroup.id
    };
  }
  
  // 엔드포인트 컨테이너: ep:${productId}:${groupId}
  if (dndId.startsWith('ep:')) {
    const parts = dndId.slice(3).split(':');
    // ⚠️ 형식 검증: 정확히 2개 파트 필요
    if (parts.length !== 2) {
      console.warn(`Invalid endpointContainer ID format: ${dndId}`);
      return { type: 'unknown', itemId: dndId };
    }
    return { 
      type: 'endpointContainer', 
      itemId: '',
      productId: parts[0],
      containerParentId: parts[1]  // 소속 그룹 ID
    };
  }
  
  return { type: 'unknown', itemId: dndId };
}

// ==========================================
// DnD 이벤트 판별 헬퍼
// ==========================================

/**
 * Reorder vs Move 판별
 * 
 * ⚠️ 주의: 단순히 "같은 타입"만으로 reorder 판단하면 안 됨!
 * - over가 아이템(g:xxx)인 경우: 같은 컨테이너 내 reorder 가능성
 * - over가 컨테이너(grp:xxx)인 경우: 다른 부모로 move
 * 
 * 정확한 판별을 위해서는 source container와 target container를 비교해야 함.
 * dnd-kit의 active.data.current.sortable.containerId 사용 권장.
 */
export interface DndAction {
  actionType: 'reorder' | 'move' | 'unknown';
  activeType: 'group' | 'endpoint';
  activeId: string;
  
  // reorder인 경우
  overId?: string;
  
  // move인 경우
  targetContainerProductId?: string;
  targetContainerParentId?: string;
}

export function determineDndAction(
  activeId: string, 
  overId: string,
  activeContainerId?: string,  // active.data.current.sortable?.containerId
  overContainerId?: string     // over.data.current.sortable?.containerId
): DndAction {
  const activeParsed = parseDndId(activeId);
  const overParsed = parseDndId(overId);
  
  // 기본 결과
  const baseAction: DndAction = {
    actionType: 'unknown',
    activeType: activeParsed.type as 'group' | 'endpoint',
    activeId: activeParsed.itemId,
  };
  
  // 아이템 타입이 아니면 무시
  if (activeParsed.type !== 'group' && activeParsed.type !== 'endpoint') {
    return baseAction;
  }
  
  // over가 컨테이너인 경우 → move
  if (overParsed.type === 'groupContainer' || overParsed.type === 'endpointContainer') {
    return {
      ...baseAction,
      actionType: 'move',
      targetContainerProductId: overParsed.productId,
      targetContainerParentId: overParsed.containerParentId,
    };
  }
  
  // over가 같은 타입의 아이템인 경우
  if (
    (activeParsed.type === 'group' && overParsed.type === 'group') ||
    (activeParsed.type === 'endpoint' && overParsed.type === 'endpoint')
  ) {
    // ⚠️ 정확한 reorder/move 판별: 컨테이너 ID 비교
    if (activeContainerId && overContainerId) {
      if (activeContainerId === overContainerId) {
        return { ...baseAction, actionType: 'reorder', overId: overParsed.itemId };
      } else {
        // 다른 컨테이너 → move (over 위치로)
        const containerParsed = parseDndId(overContainerId);
        return {
          ...baseAction,
          actionType: 'move',
          targetContainerProductId: containerParsed.productId,
          targetContainerParentId: containerParsed.containerParentId,
        };
      }
    }
    
    // containerId 정보 없으면 일단 reorder로 처리
    return { ...baseAction, actionType: 'reorder', overId: overParsed.itemId };
  }
  
  return baseAction;
}
```

---

### Phase 3: SortableGroupItem props 수정

**파일:** `src/features/projects/components/APIListPanel.tsx`

```tsx
// SortableGroupItemProps 수정
interface SortableGroupItemProps {
  sortableId: string;     // ✅ DnD용 ID (e.g. "g:abc123")
  groupId: string;        // ✅ 실제 group.id (API 호출용)
  productId: string;
  groupName: string;
  depth: number;
  isExpanded: boolean;
  onToggle: () => void;
  onAddEndpoint: () => void;
  onAddSubgroup: () => void;
  onDelete: () => void;
  children: React.ReactNode;
}

const SortableGroupItem = memo(function SortableGroupItem({
  sortableId,  // ✅ 변경됨
  groupId,     // ✅ 새로 추가
  productId,
  groupName,
  depth,
  isExpanded,
  onToggle,
  onAddEndpoint,
  onAddSubgroup,
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
  } = useSortable({ id: sortableId });  // ✅ sortableId 사용
  
  // ... 나머지 동일
});
```

---

### Phase 4: SortableEndpointItem props 수정

**파일:** `src/features/projects/components/APIListPanel.tsx`

```tsx
// SortableEndpointItemProps 수정
interface SortableEndpointItemProps {
  sortableId: string;     // ✅ DnD용 ID (e.g. "e:xyz789")
  endpoint: ApiEndpoint;
  isSelected: boolean;
  onSelect: (endpoint: ApiEndpoint) => void;
  onEdit: (endpoint: ApiEndpoint) => void;
  onDelete: (endpoint: ApiEndpoint) => void;
  onDuplicate: (endpoint: ApiEndpoint) => void;
  getStatusIndicator: (endpointId: string) => JSX.Element | null;
}

const SortableEndpointItem = memo(function SortableEndpointItem({
  sortableId,  // ✅ 새로 추가
  endpoint,
  // ...
}: SortableEndpointItemProps) {
  const {
    attributes,
    listeners,
    setNodeRef,
    transform,
    transition,
    isDragging,
  } = useSortable({ id: sortableId });  // ✅ sortableId 사용
  
  // ... 나머지 동일
});
```

---

### Phase 5: 순수 렌더 함수 생성 (재귀)

**파일:** `src/features/projects/components/APIListPanel.tsx`

```tsx
import { DndId } from '../utils/dndIdUtils';

// 의존성 인터페이스
interface RenderGroupDeps {
  expandedGroups: Set<string>;
  toggleGroup: (id: string) => void;
  handleAddEndpoint: (productId: string, groupId: string) => void;
  handleAddSubgroup: (productId: string, groupId: string) => void;
  handleDeleteGroup: (groupId: string, name: string, endpoints: any[]) => void;
  selectedEndpoint: string | null;
  onEndpointSelect: (endpoint: ApiEndpoint) => void;
  handleEditEndpoint: (e: ApiEndpoint, productId: string, groupId: string) => void;
  handleDeleteEndpoint: (e: ApiEndpoint) => void;
  handleDuplicateEndpoint: (e: ApiEndpoint) => void;
  getStatusIndicator: (id: string) => JSX.Element | null;
  searchTerm: string;
  activeDroppableId: string | null;
}

// 컴포넌트 외부에 순수 함수로 정의
function renderGroupTree(
  group: ApiGroup,
  productId: string,
  deps: RenderGroupDeps
): JSX.Element {
  const isExpanded = deps.expandedGroups.has(group.id);
  
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
      sortableId={DndId.groupItem(group.id)}  // ✅ DnD용 ID
      groupId={group.id}                       // ✅ 실제 ID
      productId={productId}
      groupName={group.name}
      depth={group.depth}
      isExpanded={isExpanded}
      onToggle={() => deps.toggleGroup(group.id)}
      onAddEndpoint={() => deps.handleAddEndpoint(productId, group.id)}
      onAddSubgroup={() => deps.handleAddSubgroup(productId, group.id)}
      onDelete={() => deps.handleDeleteGroup(group.id, group.name, group.endpoints)}
    >
      {isExpanded && (
        <>
          {/* ✅ 하위 그룹 - 항상 DroppableGroup 렌더 (빈 경우도) */}
          <DroppableGroup 
            id={DndId.groupContainer(productId, group.id)}
            isOver={deps.activeDroppableId === DndId.groupContainer(productId, group.id)}
          >
            <SortableContext 
              items={subgroupItems} 
              strategy={verticalListSortingStrategy}
            >
              <div className="ml-4 space-y-1">
                {subgroupItems.length === 0 ? (
                  // ✅ 빈 그룹 placeholder (드롭 가능하게)
                  <div className="py-2 px-3 text-xs text-zinc-600 text-center border border-dashed border-zinc-700 rounded">
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
              items={endpointItems} 
              strategy={verticalListSortingStrategy}
            >
              <div className="ml-6">
                {filteredEndpoints.length === 0 ? (
                  <div className="px-2 py-3 text-xs text-zinc-500 text-center">
                    엔드포인트 없음
                  </div>
                ) : (
                  filteredEndpoints.map(ep => (
                    <SortableEndpointItem
                      key={ep.id}
                      sortableId={DndId.endpointItem(ep.id)}  // ✅ DnD용 ID
                      endpoint={ep}
                      isSelected={deps.selectedEndpoint === ep.id}
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
```

---

### Phase 6: 기존 렌더링 코드 교체 + onDragOver 추가

**파일:** `src/features/projects/components/APIListPanel.tsx` (1237-1302줄 대체)

```tsx
// ✅ onDragOver에서 activeDroppableId 갱신 (isOver 표시용)
const handleDragOver = useCallback((event: DragOverEvent) => {
  const { over } = event;
  setActiveDroppableId(over?.id as string ?? null);
}, []);

// DndContext에 onDragOver 추가
<DndContext
  sensors={sensors}
  collisionDetection={closestCenter}
  onDragStart={handleDragStart}
  onDragOver={handleDragOver}  // ✅ 추가
  onDragEnd={handleDragEnd}
>
  ...
</DndContext>

{/* 루트 그룹 정렬 컨테이너 */}
{expandedProducts.has(product.id) && (
  <DroppableGroup 
    id={DndId.rootGroupContainer(product.id)}
    isOver={activeDroppableId === DndId.rootGroupContainer(product.id)}
  >
    <SortableContext 
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
            selectedEndpoint,
            onEndpointSelect,
            handleEditEndpoint,
            handleDeleteEndpoint,
            handleDuplicateEndpoint,
            getStatusIndicator,
            searchTerm,
            activeDroppableId,
          })
        )}
      </div>
    </SortableContext>
  </DroppableGroup>
)}
```

---

### Phase 7: onDragEnd 핸들러 수정

**파일:** `src/features/projects/components/APIListPanel.tsx`

```tsx
import { parseDndId, determineDndAction } from '../utils/dndIdUtils';

const handleDragEnd = useCallback((event: DragEndEvent) => {
  const { active, over } = event;
  setActiveDroppableId(null);
  
  if (!over || active.id === over.id) return;
  
  const activeId = active.id as string;
  const overId = over.id as string;
  
  // ✅ 컨테이너 ID 추출 (정확한 reorder/move 판별용)
  const activeContainerId = active.data.current?.sortable?.containerId as string | undefined;
  const overContainerId = over.data.current?.sortable?.containerId as string | undefined;
  
  // ✅ 액션 판별
  const action = determineDndAction(activeId, overId, activeContainerId, overContainerId);
  
  console.log('DnD Action:', action);
  
  if (action.actionType === 'reorder') {
    // 같은 컨테이너 내 순서 변경
    console.log(`Reorder ${action.activeType}:`, action.activeId, 'relative to:', action.overId);
    // TODO: Phase 8 reorder API 호출
  } else if (action.actionType === 'move') {
    // 다른 컨테이너로 이동
    console.log(`Move ${action.activeType}:`, action.activeId, 
      'to container:', action.targetContainerProductId, action.targetContainerParentId);
    // TODO: Phase 8 move API 호출
  }
}, []);
```

---

### Phase 8: 서버 API 추가 (DnD 저장용)

**파일:** `server/routes/groups.js` (새로 생성)

```javascript
import express from 'express';
import supabase from '../database.js';

const router = express.Router();

/**
 * 그룹 순서 변경 (같은 부모 내에서)
 * 
 * ⚠️ 성능/안정성 개선 권장:
 * - 현재: N번 개별 UPDATE → 느리고 레이스 위험
 * - 권장: Supabase RPC(Stored Procedure)로 한 번에 처리
 * - 최소: Promise.all + 트랜잭션
 */
router.patch('/reorder', async (req, res) => {
  const { parentGroupId, productId, orderedGroupIds } = req.body;
  
  try {
    // ⚠️ 개선 필요: 현재는 순차 업데이트 (느림)
    // TODO: RPC 또는 Promise.all로 변경
    const updates = orderedGroupIds.map((id, index) => 
      supabase
        .from('groups')
        .update({ order_index: index })
        .eq('id', id)
        .eq('product_id', productId)  // 안전 조건 추가
    );
    
    await Promise.all(updates);
    res.json({ success: true });
  } catch (error) {
    res.status(500).json({ error: error.message });
  }
});

/**
 * 그룹 이동 (다른 부모로)
 * 
 * ⚠️ depth 재계산 주의:
 * - 이동한 그룹뿐 아니라 모든 하위 그룹(subtree)의 depth도 갱신 필요
 * - 현재: 이동 그룹만 갱신 → 자식 depth가 틀어짐
 * - 권장: subtree 전체 depth 재계산 또는 move 후 전체 re-fetch
 */
router.patch('/move', async (req, res) => {
  const { groupId, newParentId, newIndex, productId } = req.body;
  
  try {
    // 1. 새 depth 계산
    let newDepth = 1;
    if (newParentId && newParentId !== 'root') {
      const { data: parent } = await supabase
        .from('groups')
        .select('depth')
        .eq('id', newParentId)
        .single();
      newDepth = (parent?.depth ?? 0) + 1;
    }
    
    // 2. 이동 그룹 업데이트
    await supabase
      .from('groups')
      .update({ 
        parent_group_id: newParentId === 'root' ? null : newParentId,
        depth: newDepth,
        order_index: newIndex 
      })
      .eq('id', groupId);
    
    // 3. ⚠️ 하위 그룹(subtree) depth 재계산
    // 재귀적으로 자식들의 depth를 갱신해야 함
    await updateSubtreeDepth(groupId, newDepth);
    
    // 4. 이동 후 전체 트리 재조회하여 반환 (프론트에서 동기화)
    const { data: updatedGroups } = await supabase
      .from('groups')
      .select('*')
      .eq('product_id', productId)
      .order('depth')
      .order('order_index');
    
    res.json({ success: true, groups: updatedGroups });
  } catch (error) {
    res.status(500).json({ error: error.message });
  }
});

/**
 * ⚠️ Subtree depth 재계산 헬퍼
 * 이동한 그룹의 모든 자식들의 depth를 재귀적으로 갱신
 */
async function updateSubtreeDepth(parentId, parentDepth) {
  // 직계 자식 조회
  const { data: children } = await supabase
    .from('groups')
    .select('id')
    .eq('parent_group_id', parentId);
  
  if (!children || children.length === 0) return;
  
  const childDepth = parentDepth + 1;
  
  // 자식들 depth 업데이트
  await supabase
    .from('groups')
    .update({ depth: childDepth })
    .eq('parent_group_id', parentId);
  
  // 각 자식의 하위도 재귀 처리
  for (const child of children) {
    await updateSubtreeDepth(child.id, childDepth);
  }
}

export default router;
```

**파일:** `server/routes/endpoints.js` 에 추가

```javascript
// 엔드포인트 순서 변경
router.patch('/reorder', async (req, res) => {
  const { groupId, orderedEndpointIds } = req.body;
  
  try {
    const updates = orderedEndpointIds.map((id, index) => 
      supabase
        .from('endpoints')
        .update({ order_index: index })
        .eq('id', id)
        .eq('group_id', groupId)  // 안전 조건
    );
    
    await Promise.all(updates);
    res.json({ success: true });
  } catch (error) {
    res.status(500).json({ error: error.message });
  }
});

// 엔드포인트 이동 (다른 그룹으로)
router.patch('/move', async (req, res) => {
  const { endpointId, newGroupId, newIndex } = req.body;
  
  try {
    await supabase
      .from('endpoints')
      .update({ 
        group_id: newGroupId,
        order_index: newIndex 
      })
      .eq('id', endpointId);
    
    res.json({ success: true });
  } catch (error) {
    res.status(500).json({ error: error.message });
  }
});
```

---

## 📂 수정 파일 목록

| Phase | 파일 | 변경 내용 |
|-------|------|----------|
| 1 | `src/types/index.ts` | `ApiGroup`에 `subgroups` 타입 추가 |
| 2 | `src/features/projects/utils/dndIdUtils.ts` | **새 파일** - ID 생성/파싱 유틸 |
| 3 | `src/features/projects/components/APIListPanel.tsx` | `SortableGroupItem` props 수정 |
| 4 | `src/features/projects/components/APIListPanel.tsx` | `SortableEndpointItem` props 수정 |
| 5 | `src/features/projects/components/APIListPanel.tsx` | `renderGroupTree` 순수 함수 추가 |
| 6 | `src/features/projects/components/APIListPanel.tsx` | 기존 렌더링 코드 교체 + `onDragOver` |
| 7 | `src/features/projects/components/APIListPanel.tsx` | `onDragEnd` 핸들러 수정 |
| 8 | `server/routes/groups.js` | **새 파일** - reorder/move API |
| 8 | `server/routes/endpoints.js` | reorder/move API 추가 |

---

## 📊 DnD 컨텍스트 구조 (최종)

```
<DndContext onDragOver={...} onDragEnd={...}>
  <SortableContext items={productSortableIds}>
    
    <Product id="civil-nx">
      <DroppableGroup id="grp:civil-nx:root">          ← 루트 그룹 드롭 영역
        <SortableContext items={["g:PSC_Design", ...]}>
          
          <SortableGroupItem sortableId="g:PSC_Design">
            
            <DroppableGroup id="grp:civil-nx:PSC_Design">   ← subgroups 드롭 영역 (항상 렌더)
              <SortableContext items={["g:option", "g:test"]}>
                
                <SortableGroupItem sortableId="g:option">
                  <DroppableGroup id="grp:civil-nx:option">  ← 빈 경우도 placeholder
                    <SortableContext items={[]}>
                      <Placeholder>하위 그룹을 여기에 드롭</Placeholder>
                    </SortableContext>
                  </DroppableGroup>
                  <DroppableGroup id="ep:civil-nx:option">
                    <SortableContext items={["e:ep1"]}>
                      <SortableEndpointItem sortableId="e:ep1"/>
                    </SortableContext>
                  </DroppableGroup>
                </SortableGroupItem>
                
              </SortableContext>
            </DroppableGroup>
            
            <DroppableGroup id="ep:civil-nx:PSC_Design">    ← endpoints 드롭 영역
              <SortableContext items={["e:ep2", "e:ep3"]}>
                ...
              </SortableContext>
            </DroppableGroup>
            
          </SortableGroupItem>
          
        </SortableContext>
      </DroppableGroup>
    </Product>
    
  </SortableContext>
</DndContext>
```

---

## 📊 ID 체계 요약

| 용도 | 접두사 | 예시 | 파싱 결과 |
|------|--------|------|-----------|
| 그룹 아이템 | `g:` | `g:PSC_Design` | `{ type: 'group', itemId: 'PSC_Design' }` |
| 엔드포인트 아이템 | `e:` | `e:abc123` | `{ type: 'endpoint', itemId: 'abc123' }` |
| 루트 그룹 컨테이너 | `grp:` | `grp:civil-nx:root` | `{ type: 'groupContainer', productId: 'civil-nx', containerParentId: 'root' }` |
| 그룹 컨테이너 | `grp:` | `grp:civil-nx:PSC_Design` | `{ type: 'groupContainer', productId: 'civil-nx', containerParentId: 'PSC_Design' }` |
| 엔드포인트 컨테이너 | `ep:` | `ep:civil-nx:PSC_Design` | `{ type: 'endpointContainer', productId: 'civil-nx', containerParentId: 'PSC_Design' }` |

---

## ⏱️ 예상 소요 시간

| Phase | 작업 | 시간 |
|-------|------|------|
| 1 | 타입 정의 수정 | 5분 |
| 2 | DnD ID 유틸리티 (파싱 개선 포함) | 20분 |
| 3 | SortableGroupItem 수정 | 15분 |
| 4 | SortableEndpointItem 수정 | 10분 |
| 5 | renderGroupTree 함수 (빈 그룹 처리 포함) | 35분 |
| 6 | 기존 코드 교체 + onDragOver | 20분 |
| 7 | onDragEnd 핸들러 (determineDndAction 사용) | 25분 |
| 8 | 서버 API (subtree depth 재계산 포함) | 50분 |
| - | 테스트 | 30분 |

**총 예상 시간: 약 3.5시간**

---

## ⚠️ 주의사항

1. **sortableId와 items 매칭**: `SortableContext.items`에 들어간 값과 `useSortable({ id })`가 **정확히 동일**해야 함
2. **필터 결과 단일화**: items와 렌더링에서 같은 필터된 배열 사용
3. **빈 그룹에도 DroppableGroup**: subgroups가 0개여도 DroppableGroup 렌더해야 드롭 가능
4. **onDragOver 필수**: `activeDroppableId`를 갱신해야 `isOver` 표시 동작
5. **reorder/move 판별**: `active.data.current.sortable.containerId` 비교 필요
6. **move 후 depth 전파**: subtree 전체 depth 재계산 필수
7. **서버 reorder 성능**: `Promise.all` 또는 RPC 사용 권장 (순차 업데이트는 느림/레이스 위험)
8. **move 후 동기화**: 서버에서 updated groups 반환 → 프론트에서 상태 갱신 (또는 re-fetch)
9. **성능**: 깊은 중첩 시 React.memo 적극 활용
