/**
 * Groups API Routes
 */
import express from 'express';
import { createClient } from '@supabase/supabase-js';

const router = express.Router();

const supabase = createClient(
  process.env.SUPABASE_URL,
  process.env.SUPABASE_SERVICE_KEY
);

/**
 * GET /api/groups
 * 모든 그룹 조회 (옵션: product_id로 필터링)
 */
router.get('/', async (req, res) => {
  try {
    const { product_id } = req.query;

    let query = supabase
      .from('groups')
      .select('*')
      .order('order_index', { ascending: true })
      .order('created_at', { ascending: true });

    if (product_id) {
      query = query.eq('product_id', product_id);
    }

    const { data: groups, error } = await query;

    if (error) throw error;

    res.json(groups || []);
  } catch (error) {
    console.error('Get groups error:', error);
    res.status(500).json({ error: error.message });
  }
});

/**
 * GET /api/groups/:id
 * 특정 그룹 조회
 */
router.get('/:id', async (req, res) => {
  try {
    const { id } = req.params;
    const { data: group, error } = await supabase
      .from('groups')
      .select('*')
      .eq('id', id)
      .single();

    if (error) {
      if (error.code === 'PGRST116') {
        return res.status(404).json({ error: 'Group not found' });
      }
      throw error;
    }

    res.json(group);
  } catch (error) {
    console.error('Get group error:', error);
    res.status(500).json({ error: error.message });
  }
});

/**
 * POST /api/groups
 * 새 그룹 생성 (계층 구조 지원 - 최대 5단계)
 */
router.post('/', async (req, res) => {
  try {
    const { id, product_id, name, description, parent_group_id } = req.body;
    const now = new Date().toISOString();

    if (!id || !product_id || !name) {
      return res.status(400).json({ error: 'id, product_id, and name are required' });
    }

    // 부모 그룹의 depth 조회 (있는 경우)
    let depth = 1;
    if (parent_group_id) {
      const { data: parentGroup, error: parentError } = await supabase
        .from('groups')
        .select('depth')
        .eq('id', parent_group_id)
        .single();

      if (parentError) {
        console.error('❌ Parent group lookup error:', parentError);
        return res.status(400).json({ error: 'Parent group not found' });
      }

      depth = (parentGroup.depth || 1) + 1;

      // 최대 5단계 제한
      if (depth > 5) {
        return res.status(400).json({ error: 'Maximum group depth (5) exceeded' });
      }
    }

    // 해당 제품/부모 그룹의 현재 최대 order_index 조회
    let query = supabase
      .from('groups')
      .select('order_index')
      .eq('product_id', product_id)
      .order('order_index', { ascending: false })
      .limit(1);

    if (parent_group_id) {
      query = query.eq('parent_group_id', parent_group_id);
    } else {
      query = query.is('parent_group_id', null);
    }

    const { data: maxOrderData } = await query.single();
    const nextOrderIndex = (maxOrderData?.order_index ?? -1) + 1;

    const { data, error } = await supabase
      .from('groups')
      .insert({
        id,
        product_id,
        name,
        description: description || null,
        parent_group_id: parent_group_id || null,
        depth,
        order_index: nextOrderIndex,
        created_at: now,
        updated_at: now,
      })
      .select()
      .single();

    if (error) throw error;

    console.log('✅ Group created:', id, 'depth:', depth);
    res.status(201).json({ group: data, message: 'Group created successfully' });
  } catch (error) {
    console.error('❌ Create group error:', error);
    res.status(500).json({ error: error.message });
  }
});

/**
 * PUT /api/groups/reorder
 * 그룹 순서 변경
 */
router.put('/reorder', async (req, res) => {
  try {
    const { groups } = req.body;

    console.log('🔄 Reorder groups request:', { groupsCount: groups?.length, groups });

    if (!Array.isArray(groups)) {
      console.error('❌ Invalid request: groups is not an array');
      return res.status(400).json({ error: 'groups array is required' });
    }

    if (groups.length === 0) {
      console.log('⚠️ No groups to reorder');
      return res.json({ message: 'No groups to reorder', count: 0 });
    }

    let successCount = 0;
    for (const group of groups) {
      console.log(`  Updating ${group.id} to order_index: ${group.order_index}`);

      const { data, error } = await supabase
        .from('groups')
        .update({
          order_index: group.order_index,
          updated_at: new Date().toISOString()
        })
        .eq('id', group.id)
        .select();

      if (error) {
        console.error('❌ Reorder error for', group.id, ':', error);
        return res.status(500).json({
          error: error.message,
          details: error,
          group: group.id
        });
      }

      console.log(`  ✅ Updated ${group.id}:`, data);
      successCount++;
    }

    console.log(`✅ Successfully reordered ${successCount} groups`);
    res.json({
      message: 'Groups reordered successfully',
      count: successCount
    });
  } catch (error) {
    console.error('❌ Reorder groups error:', error);
    res.status(500).json({
      error: error.message,
      stack: error.stack
    });
  }
});

/**
 * PUT /api/groups/:id/move
 * ?? ?? (parent_group_id ?? + depth ???)
 */
router.put('/:id/move', async (req, res) => {
  try {
    const { id } = req.params;
    const { parent_group_id, order_index } = req.body;
    const now = new Date().toISOString();

    // 1. ?? ?? ??
    const { data: group, error: groupError } = await supabase
      .from('groups')
      .select('id, product_id, parent_group_id, depth')
      .eq('id', id)
      .single();

    if (groupError) {
      if (groupError.code === 'PGRST116') {
        return res.status(404).json({ error: 'Group not found' });
      }
      throw groupError;
    }

    const targetParentId = parent_group_id || null;
    const movingSameParent = (group.parent_group_id || null) === targetParentId;
    const hasOrderIndex = Number.isInteger(order_index);

    // 2. ?? ?? + order_index ???? ?? ??
    if (movingSameParent && !hasOrderIndex) {
      return res.json({ message: 'No changes', group });
    }

    // 3. ?? ? ?? ?? ??
    const { data: allGroups, error: allGroupsError } = await supabase
      .from('groups')
      .select('id, parent_group_id, depth, order_index, product_id, created_at')
      .eq('product_id', group.product_id);

    if (allGroupsError) throw allGroupsError;

    const groupsData = allGroups || [];
    const groupMap = new Map(groupsData.map(g => [g.id, g]));
    const parentMap = new Map(groupsData.map(g => [g.id, g.parent_group_id || null]));

    // 4. ?? ?? ?? + ?? ?? ??
    if (targetParentId) {
      const parentGroup = groupMap.get(targetParentId);
      if (!parentGroup) {
        return res.status(400).json({ error: 'Parent group not found' });
      }
      if (parentGroup.product_id !== group.product_id) {
        return res.status(400).json({ error: 'Cross-product group move is not allowed' });
      }

      let current = targetParentId;
      while (current) {
        if (current === id) {
          return res.status(400).json({ error: 'Cannot move group into its own subtree' });
        }
        current = parentMap.get(current) || null;
      }
    }

    // 5. children map ??
    const childrenMap = new Map();
    for (const g of groupsData) {
      const parentId = g.parent_group_id || null;
      if (!childrenMap.has(parentId)) {
        childrenMap.set(parentId, []);
      }
      childrenMap.get(parentId).push(g.id);
    }

    // 6. ?? ? depth ?? + ?? depth(5) ??
    const parentDepth = targetParentId ? (groupMap.get(targetParentId)?.depth || 1) : 0;
    const newDepth = parentDepth + 1;

    let maxRelativeDepth = 0;
    const depthStack = [{ id, rel: 0 }];
    while (depthStack.length > 0) {
      const { id: currentId, rel } = depthStack.pop();
      maxRelativeDepth = Math.max(maxRelativeDepth, rel);
      const children = childrenMap.get(currentId) || [];
      for (const childId of children) {
        depthStack.push({ id: childId, rel: rel + 1 });
      }
    }

    if (newDepth + maxRelativeDepth > 5) {
      return res.status(400).json({ error: 'Maximum group depth (5) exceeded' });
    }

    // 7. depth ???? ?
    const updatesById = new Map();
    const ensureUpdate = (groupId) => {
      if (!updatesById.has(groupId)) {
        updatesById.set(groupId, { updated_at: now });
      }
      return updatesById.get(groupId);
    };

    const stack = [{ id, rel: 0 }];
    while (stack.length > 0) {
      const { id: currentId, rel } = stack.pop();
      const update = ensureUpdate(currentId);
      update.depth = newDepth + rel;

      const children = childrenMap.get(currentId) || [];
      for (const childId of children) {
        stack.push({ id: childId, rel: rel + 1 });
      }
    }

    // 8. order_index ???
    const sortByOrder = (a, b) => {
      const orderA = a.order_index ?? 0;
      const orderB = b.order_index ?? 0;
      if (orderA !== orderB) return orderA - orderB;
      return new Date(a.created_at).getTime() - new Date(b.created_at).getTime();
    };

    const getSiblings = (parentId) =>
      groupsData
        .filter(g => (g.parent_group_id || null) === parentId && g.id !== id)
        .sort(sortByOrder);

    const oldParentId = group.parent_group_id || null;
    const oldSiblings = movingSameParent ? [] : getSiblings(oldParentId);
    const newSiblings = getSiblings(targetParentId);

    const insertIndex = hasOrderIndex
      ? Math.max(0, Math.min(order_index, newSiblings.length))
      : newSiblings.length;

    newSiblings.splice(insertIndex, 0, group);

    const applyOrder = (siblings) => {
      siblings.forEach((g, index) => {
        const update = ensureUpdate(g.id);
        update.order_index = index;
      });
    };

    if (!movingSameParent) {
      applyOrder(oldSiblings);
    }
    applyOrder(newSiblings);

    // 9. parent_group_id ????
    const mainUpdate = ensureUpdate(id);
    mainUpdate.parent_group_id = targetParentId;

    // 10. DB ????
    let successCount = 0;
    for (const [groupId, update] of updatesById.entries()) {
      const { error } = await supabase
        .from('groups')
        .update(update)
        .eq('id', groupId);

      if (error) {
        console.error('? Move group update error:', groupId, error);
        return res.status(500).json({ error: error.message, group: groupId });
      }
      successCount++;
    }

    res.json({ message: 'Group moved successfully', count: successCount });
  } catch (error) {
    console.error('? Move group error:', error);
    res.status(500).json({ error: error.message });
  }
});

/**
 * PUT /api/groups/:id
 * 그룹 수정
 */
router.put('/:id', async (req, res) => {
  try {
    const { id } = req.params;
    const { name, description } = req.body;

    const { data, error } = await supabase
      .from('groups')
      .update({
        name,
        description: description || null,
        updated_at: new Date().toISOString(),
      })
      .eq('id', id)
      .select()
      .single();

    if (error) {
      if (error.code === 'PGRST116') {
        return res.status(404).json({ error: 'Group not found' });
      }
      throw error;
    }

    res.json({ group: data, message: 'Group updated successfully' });
  } catch (error) {
    console.error('Update group error:', error);
    res.status(500).json({ error: error.message });
  }
});

/**
 * DELETE /api/groups/:id
 * 그룹 삭제 (CASCADE로 하위 엔드포인트도 모두 삭제)
 */
router.delete('/:id', async (req, res) => {
  try {
    const { id } = req.params;

    console.log('🗑️ Delete group request:', { id });

    // 1. 먼저 그룹 정보 조회
    const { data: group, error: groupError } = await supabase
      .from('groups')
      .select('*')
      .eq('id', id)
      .single();

    // 🔥 groups 테이블에 없는 경우 (동적 그룹/레거시 그룹)
    if (groupError && groupError.code === 'PGRST116') {
      console.log('⚠️ Group not found in groups table, checking exact legacy group_id match:', id);

      // 안전한 fallback: endpoint.group_id가 정확히 일치하는 경우에만 삭제 허용
      const { data: legacyEndpoints, error: legacyEndpointsError } = await supabase
        .from('endpoints')
        .select('id, name')
        .eq('group_id', id);

      if (legacyEndpointsError) throw legacyEndpointsError;

      console.log(
        `🔍 Found ${legacyEndpoints?.length || 0} legacy endpoints for exact group_id=${id}:`,
        legacyEndpoints?.map(e => e.name)
      );

      if (legacyEndpoints && legacyEndpoints.length > 0) {
        const { error: deleteError } = await supabase
          .from('endpoints')
          .delete()
          .eq('group_id', id);

        if (deleteError) {
          console.error('❌ Delete endpoints error:', deleteError);
          throw deleteError;
        }

        console.log(`✅ Safely deleted ${legacyEndpoints.length} legacy endpoints by exact group_id`);

        return res.json({
          message: 'Legacy group deleted safely by exact group_id',
          deletedEndpoints: legacyEndpoints.length,
          legacy: true
        });
      }

      console.warn('🛑 Blocked unsafe legacy group delete with no exact group_id match:', id);
      return res.status(409).json({
        error: 'Legacy group deletion blocked for safety. This folder is not backed by the groups table, and deleting by name could remove other folders with the same name.',
        groupId: id,
        safeDeleteRequired: true
      });
    }

    if (groupError) {
      throw groupError;
    }

    console.log('📦 Found group:', group);

    // 2–3. 이 그룹에만 연결된 엔드포인트 삭제 (group_id 기준)
    // 동일 product 내에서 group.name이 같은 다른 폴더의 엔드포인트가 지워지지 않도록 함
    const { data: endpoints, error: endpointsError } = await supabase
      .from('endpoints')
      .select('id, name')
      .eq('group_id', id);

    if (endpointsError) throw endpointsError;

    console.log(`🔍 Found ${endpoints?.length || 0} endpoints for group_id=${id}:`, endpoints?.map(e => e.name));

    if (endpoints && endpoints.length > 0) {
      const { error: deleteEndpointsError } = await supabase
        .from('endpoints')
        .delete()
        .eq('group_id', id);

      if (deleteEndpointsError) {
        console.error('❌ Delete endpoints error:', deleteEndpointsError);
        throw deleteEndpointsError;
      }

      console.log(`✅ Deleted ${endpoints.length} endpoints`);
    }

    // 4. 그룹 삭제 (하위 그룹·해당 엔드포인트는 DB FK CASCADE로 정리)
    const { error } = await supabase
      .from('groups')
      .delete()
      .eq('id', id);

    if (error) {
      console.error('❌ Delete group error:', error);
      throw error;
    }

    console.log('✅ Group deleted:', id);
    res.json({
      message: 'Group deleted successfully',
      deletedEndpoints: endpoints?.length || 0
    });
  } catch (error) {
    console.error('❌ Delete group error:', error);
    res.status(500).json({ error: error.message });
  }
});

export default router;

