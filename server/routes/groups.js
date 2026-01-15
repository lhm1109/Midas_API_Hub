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
 * 새 그룹 생성
 */
router.post('/', async (req, res) => {
  try {
    const { id, product_id, name, description } = req.body;
    const now = new Date().toISOString();

    if (!id || !product_id || !name) {
      return res.status(400).json({ error: 'id, product_id, and name are required' });
    }

    // 해당 제품의 현재 최대 order_index 조회
    const { data: maxOrderData } = await supabase
      .from('groups')
      .select('order_index')
      .eq('product_id', product_id)
      .order('order_index', { ascending: false })
      .limit(1)
      .single();

    const nextOrderIndex = (maxOrderData?.order_index ?? -1) + 1;

    const { data, error } = await supabase
      .from('groups')
      .insert({
        id,
        product_id,
        name,
        description: description || null,
        order_index: nextOrderIndex,
        created_at: now,
        updated_at: now,
      })
      .select()
      .single();

    if (error) throw error;

    console.log('✅ Group created:', id);
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
      console.log('⚠️ Group not found in groups table, treating as legacy group:', id);
      
      // ID에서 product와 group_name 추출 (예: civil-nx_Gen → product: civil-nx, group_name: Gen)
      const parts = id.split('_');
      if (parts.length < 2) {
        return res.status(400).json({ error: 'Invalid group ID format', id });
      }
      
      const product = parts[0];
      const groupName = parts.slice(1).join('_');
      
      console.log('📦 Legacy group:', { product, groupName });
      
      // 해당 엔드포인트들 조회
      const { data: endpoints, error: endpointsError } = await supabase
        .from('endpoints')
        .select('id, name')
        .eq('product', product)
        .eq('group_name', groupName);

      if (endpointsError) throw endpointsError;

      console.log(`🔍 Found ${endpoints?.length || 0} legacy endpoints:`, endpoints?.map(e => e.name));

      // 엔드포인트들 삭제
      if (endpoints && endpoints.length > 0) {
        const { error: deleteError } = await supabase
          .from('endpoints')
          .delete()
          .eq('product', product)
          .eq('group_name', groupName);

        if (deleteError) {
          console.error('❌ Delete endpoints error:', deleteError);
          throw deleteError;
        }

        console.log(`✅ Deleted ${endpoints.length} legacy endpoints`);
      }

      return res.json({ 
        message: 'Legacy group deleted successfully (endpoints only)', 
        deletedEndpoints: endpoints?.length || 0,
        legacy: true
      });
    }

    if (groupError) {
      throw groupError;
    }

    console.log('📦 Found group:', group);

    // 2. 해당 그룹의 엔드포인트들 조회
    const { data: endpoints, error: endpointsError } = await supabase
      .from('endpoints')
      .select('id, name')
      .eq('product', group.product_id)
      .eq('group_name', group.name);

    if (endpointsError) throw endpointsError;

    console.log(`🔍 Found ${endpoints?.length || 0} endpoints in this group:`, endpoints?.map(e => e.name));

    // 3. 그룹의 엔드포인트들 삭제
    if (endpoints && endpoints.length > 0) {
      const { error: deleteEndpointsError } = await supabase
        .from('endpoints')
        .delete()
        .eq('product', group.product_id)
        .eq('group_name', group.name);

      if (deleteEndpointsError) {
        console.error('❌ Delete endpoints error:', deleteEndpointsError);
        throw deleteEndpointsError;
      }

      console.log(`✅ Deleted ${endpoints.length} endpoints`);
    }

    // 4. 그룹 삭제
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

