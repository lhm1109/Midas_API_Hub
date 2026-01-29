/**
 * DnD ID 유틸리티
 * 
 * ID 체계:
 * - g:${groupId}           → 그룹 아이템
 * - e:${endpointId}        → 엔드포인트 아이템
 * - grp:${productId}:${parentId} → 그룹 컨테이너
 * - ep:${productId}:${groupId}   → 엔드포인트 컨테이너
 */

// ==========================================
// DnD ID 생성 유틸리티
// ==========================================
export const DndId = {
    /** 그룹 정렬 컨테이너 (해당 그룹의 직계 subgroups용) */
    groupContainer: (productId: string, parentGroupId: string) =>
        `grp:${productId}:${parentGroupId}`,

    /** 루트 그룹 컨테이너 (product 직속 groups용) */
    rootGroupContainer: (productId: string) =>
        `grp:${productId}:root`,

    /** 엔드포인트 정렬 컨테이너 */
    endpointContainer: (productId: string, groupId: string) =>
        `ep:${productId}:${groupId}`,

    /** 그룹 아이템 */
    groupItem: (groupId: string) => `g:${groupId}`,

    /** 엔드포인트 아이템 */
    endpointItem: (endpointId: string) => `e:${endpointId}`,
};

// ==========================================
// DnD ID 파싱 유틸리티
// ==========================================
export type DndItemType = 'group' | 'endpoint' | 'groupContainer' | 'endpointContainer' | 'unknown';

export interface ParsedDndId {
    type: DndItemType;
    itemId: string;              // 아이템인 경우: 실제 ID
    productId?: string;          // 컨테이너인 경우
    containerParentId?: string;  // 컨테이너인 경우 (e.g. "root" or parentGroup.id)
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
            itemId: '',
            productId: parts[0],
            containerParentId: parts[1]
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
            containerParentId: parts[1]
        };
    }

    return { type: 'unknown', itemId: dndId };
}

// ==========================================
// DnD 액션 판별
// ==========================================
export type DndActionType = 'reorder' | 'move' | 'unknown';

export interface DndAction {
    actionType: DndActionType;
    activeType: 'group' | 'endpoint' | 'unknown';
    activeId: string;

    // reorder인 경우
    overId?: string;

    // move인 경우
    targetContainerProductId?: string;
    targetContainerParentId?: string;
}

/**
 * DnD 액션 판별
 * 
 * @param activeId - 드래그 중인 아이템 ID
 * @param overId - 드롭 대상 ID
 * @param activeContainerId - active.data.current.sortable?.containerId
 * @param overContainerId - over.data.current.sortable?.containerId
 * @param parentLookup - groupId/endpointId -> parentId 맵 (cross-container 판별용)
 */
export function determineDndAction(
    activeId: string,
    overId: string,
    activeContainerId?: string,
    overContainerId?: string,
    parentLookup?: { groupParentMap: Map<string, string>; endpointGroupMap: Map<string, string> }
): DndAction {
    const activeParsed = parseDndId(activeId);
    const overParsed = parseDndId(overId);

    // 기본 결과
    const baseAction: DndAction = {
        actionType: 'unknown',
        activeType: (activeParsed.type === 'group' || activeParsed.type === 'endpoint')
            ? activeParsed.type
            : 'unknown',
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
                // 다른 컨테이너 → move
                const containerParsed = parseDndId(overContainerId);
                return {
                    ...baseAction,
                    actionType: 'move',
                    targetContainerProductId: containerParsed.productId,
                    targetContainerParentId: containerParsed.containerParentId,
                };
            }
        }

        // containerId 정보 없으면 parentLookup으로 판별 시도
        if (parentLookup) {
            const { groupParentMap, endpointGroupMap } = parentLookup;

            if (activeParsed.type === 'group') {
                const activeParent = groupParentMap.get(activeParsed.itemId);
                const overParent = groupParentMap.get(overParsed.itemId);

                if (activeParent === overParent) {
                    return { ...baseAction, actionType: 'reorder', overId: overParsed.itemId };
                } else {
                    return {
                        ...baseAction,
                        actionType: 'move',
                        targetContainerParentId: overParent ?? 'root',
                    };
                }
            }

            if (activeParsed.type === 'endpoint') {
                const activeGroup = endpointGroupMap.get(activeParsed.itemId);
                const overGroup = endpointGroupMap.get(overParsed.itemId);

                if (activeGroup === overGroup) {
                    return { ...baseAction, actionType: 'reorder', overId: overParsed.itemId };
                } else {
                    return {
                        ...baseAction,
                        actionType: 'move',
                        targetContainerParentId: overGroup,
                    };
                }
            }
        }

        // 최종 fallback: reorder로 처리
        return { ...baseAction, actionType: 'reorder', overId: overParsed.itemId };
    }

    return baseAction;
}

// ==========================================
// 부모 Lookup 맵 생성 헬퍼
// ==========================================
import type { ApiGroup, ApiProduct } from '@/types';

export interface ParentLookup {
    groupParentMap: Map<string, string>;      // groupId -> parentGroupId (root면 'root')
    endpointGroupMap: Map<string, string>;    // endpointId -> groupId
}

export function buildParentLookup(products: ApiProduct[]): ParentLookup {
    const groupParentMap = new Map<string, string>();
    const endpointGroupMap = new Map<string, string>();

    function traverseGroup(group: ApiGroup, parentId: string) {
        groupParentMap.set(group.id, parentId);

        // Endpoints
        for (const ep of group.endpoints) {
            endpointGroupMap.set(ep.id, group.id);
        }

        // Subgroups 재귀
        for (const sg of group.subgroups) {
            traverseGroup(sg, group.id);
        }
    }

    for (const product of products) {
        for (const group of product.groups) {
            traverseGroup(group, 'root');
        }
    }

    return { groupParentMap, endpointGroupMap };
}
