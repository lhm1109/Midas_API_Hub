import express from 'express';
import supabase from '../database.js';

const router = express.Router();

/**
 * GET /api/endpoints
 * 모든 엔드포인트 목록 조회 (계층 구조)
 */
router.get('/', async (req, res) => {
  try {
    const { data: endpoints, error } = await supabase
      .from('endpoints')
      .select('*')
      .order('product', { ascending: true })
      .order('group_name', { ascending: true })
      .order('order_index', { ascending: true })
      .order('name', { ascending: true });
    
    if (error) throw error;
    res.json(endpoints || []);
  } catch (error) {
    console.error('Get endpoints error:', error);
    res.status(500).json({ error: error.message });
  }
});

/**
 * GET /api/endpoints/tree
 * 트리 구조로 엔드포인트 조회 (빈 제품/그룹도 포함)
 */
router.get('/tree', async (req, res) => {
  try {
    // 1. Products 테이블에서 모든 제품 가져오기
    const { data: products, error: productsError } = await supabase
      .from('products')
      .select('*')
      .order('order_index', { ascending: true });
    
    if (productsError) throw productsError;

    // 2. Groups 테이블에서 모든 그룹 가져오기
    const { data: groups, error: groupsError } = await supabase
      .from('groups')
      .select('*')
      .order('product_id', { ascending: true })
      .order('order_index', { ascending: true });
    
    if (groupsError) throw groupsError;

    // 3. Endpoints 테이블에서 모든 엔드포인트 가져오기
    const { data: endpoints, error: endpointsError } = await supabase
      .from('endpoints')
      .select('*')
      .order('product', { ascending: true })
      .order('group_name', { ascending: true })
      .order('order_index', { ascending: true })
      .order('name', { ascending: true });
    
    if (endpointsError) throw endpointsError;
    
    // 4. 트리 구조로 변환
    const tree = {};
    
    // 4-1. Products 먼저 생성
    (products || []).forEach(product => {
      tree[product.id] = {
        id: product.id,
        name: product.name,
        groups: {}
      };
    });
    
    // 4-2. Groups 추가
    (groups || []).forEach(group => {
      if (tree[group.product_id]) {
        tree[group.product_id].groups[group.name] = {
          id: group.id,
          name: group.name,
          endpoints: []
        };
      }
    });
    
    // 4-3. Endpoints 추가 (products 테이블에 있는 것만)
    (endpoints || []).forEach(endpoint => {
      // 🔥 Product가 products 테이블에 없으면 무시 (고아 엔드포인트)
      if (!tree[endpoint.product]) {
        console.log(`⚠️ Orphaned endpoint detected: ${endpoint.name} (product: ${endpoint.product})`);
        return; // Skip this endpoint
      }
      
      // Group이 없으면 생성 (레거시 데이터 대응)
      if (!tree[endpoint.product].groups[endpoint.group_name]) {
        console.log(`⚠️ Creating missing group: ${endpoint.group_name} for product: ${endpoint.product}`);
        tree[endpoint.product].groups[endpoint.group_name] = {
          id: `${endpoint.product}_${endpoint.group_name}`,
          name: endpoint.group_name,
          endpoints: []
        };
      }
      
      // Endpoint 추가
      tree[endpoint.product].groups[endpoint.group_name].endpoints.push({
        id: endpoint.id,
        name: endpoint.name,
        method: endpoint.method,
        path: endpoint.path,
        status: endpoint.status,
        description: endpoint.description,
        order_index: endpoint.order_index
      });
    });
    
    // 5. Object를 Array로 변환
    const result = Object.values(tree).map(product => ({
      ...product,
      groups: Object.values(product.groups)
    }));
    
    res.json(result);
  } catch (error) {
    console.error('Get endpoints tree error:', error);
    res.status(500).json({ error: error.message });
  }
});

/**
 * GET /api/endpoints/:id
 * 특정 엔드포인트 조회
 */
router.get('/:id', async (req, res) => {
  try {
    const { id } = req.params;
    const { data: endpoint, error } = await supabase
      .from('endpoints')
      .select('*')
      .eq('id', id)
      .single();
    
    if (error) {
      if (error.code === 'PGRST116') {
        return res.status(404).json({ message: 'Endpoint not found' });
      }
      throw error;
    }
    
    res.json(endpoint);
  } catch (error) {
    console.error('Get endpoint error:', error);
    res.status(500).json({ error: error.message });
  }
});

/**
 * POST /api/endpoints
 * 새 엔드포인트 생성
 */
router.post('/', async (req, res) => {
  try {
    const { id, name, method, path, product, group_name, description, status, statusMessage } = req.body;
    const now = new Date().toISOString();
    
    // product_id, group_id 생성
    const product_id = product;
    const group_id = `${product}_${group_name}`;
    
    const { data, error } = await supabase
      .from('endpoints')
      .insert({
        id,
        name,
        method,
        path,
        product,
        product_id,    // ✅ 추가
        group_name,
        group_id,      // ✅ 추가
        description: description || null,
        status: status || 'active',
        status_message: statusMessage || null, // 🔥 상태 메시지 추가
        created_at: now,
        updated_at: now
      })
      .select()
      .single();
    
    if (error) throw error;
    
    res.status(201).json({ id, message: 'Endpoint created' });
  } catch (error) {
    console.error('Create endpoint error:', error);
    res.status(500).json({ error: error.message });
  }
});

/**
 * PUT /api/endpoints/reorder
 * 엔드포인트 순서 변경
 * ⚠️ 주의: /:id 라우트보다 앞에 정의해야 합니다!
 */
router.put('/reorder', async (req, res) => {
  try {
    const { endpoints } = req.body; // [{ id, order_index }, ...]
    
    console.log('🔄 Reorder request received:', { 
      endpointsCount: endpoints?.length,
      endpoints: endpoints
    });
    
    if (!Array.isArray(endpoints)) {
      console.error('❌ Invalid request: endpoints is not an array');
      return res.status(400).json({ error: 'endpoints array is required' });
    }
    
    if (endpoints.length === 0) {
      console.log('⚠️ No endpoints to reorder');
      return res.json({ message: 'No endpoints to reorder', count: 0 });
    }
    
    // 각 엔드포인트의 순서 업데이트
    let successCount = 0;
    for (const endpoint of endpoints) {
      console.log(`  Updating ${endpoint.id} to order_index: ${endpoint.order_index}`);
      
      const { data, error } = await supabase
        .from('endpoints')
        .update({ order_index: endpoint.order_index })
        .eq('id', endpoint.id)
        .select();
      
      if (error) {
        console.error('❌ Reorder error for', endpoint.id, ':', error);
        return res.status(500).json({ 
          error: error.message, 
          details: error,
          endpoint: endpoint.id 
        });
      }
      
      console.log(`  ✅ Updated ${endpoint.id}:`, data);
      successCount++;
    }
    
    console.log(`✅ Successfully reordered ${successCount} endpoints`);
    res.json({ 
      message: 'Endpoints reordered successfully', 
      count: successCount 
    });
  } catch (error) {
    console.error('❌ Reorder endpoints error:', error);
    res.status(500).json({ 
      error: error.message,
      stack: error.stack
    });
  }
});

/**
 * POST /api/endpoints/:id/duplicate
 * 엔드포인트 복제
 */
router.post('/:id/duplicate', async (req, res) => {
  try {
    const { id } = req.params;
    
    console.log('🔄 Duplicate endpoint request:', { id });
    
    // 원본 엔드포인트 조회
    const { data: originalEndpoint, error: fetchError } = await supabase
      .from('endpoints')
      .select('*')
      .eq('id', id)
      .single();
    
    if (fetchError) {
      if (fetchError.code === 'PGRST116') {
        return res.status(404).json({ error: 'Endpoint not found' });
      }
      throw fetchError;
    }
    
    // 새로운 ID와 이름 생성
    const timestamp = Date.now();
    const newId = `${originalEndpoint.id}_copy_${timestamp}`;
    const newName = `Copy of ${originalEndpoint.name}`;
    const now = new Date().toISOString();
    
    // 엔드포인트 복제
    const { data: newEndpoint, error: insertError } = await supabase
      .from('endpoints')
      .insert({
        id: newId,
        name: newName,
        method: originalEndpoint.method,
        path: originalEndpoint.path,
        product: originalEndpoint.product,
        group_name: originalEndpoint.group_name,
        description: originalEndpoint.description,
        status: originalEndpoint.status,
        order_index: (originalEndpoint.order_index || 0) + 1,
        created_at: now,
        updated_at: now,
      })
      .select()
      .single();
    
    if (insertError) throw insertError;
    
    console.log('✅ Endpoint duplicated:', { originalId: id, newId, newName });
    res.status(201).json({ endpoint: newEndpoint, message: 'Endpoint duplicated successfully' });
  } catch (error) {
    console.error('❌ Duplicate endpoint error:', error);
    res.status(500).json({ error: error.message });
  }
});

/**
 * PUT /api/endpoints/:id/move
 * 엔드포인트를 다른 그룹/제품으로 이동
 */
router.put('/:id/move', async (req, res) => {
  try {
    const { id } = req.params;
    const { product, group_name, order_index } = req.body;
    
    console.log('🔄 Move endpoint request:', { id, product, group_name, order_index });
    
    if (!product || !group_name) {
      return res.status(400).json({ error: 'product and group_name are required' });
    }
    
    // product_id, group_id 생성
    const product_id = product;
    const group_id = `${product}_${group_name}`;
    
    const { data, error } = await supabase
      .from('endpoints')
      .update({
        product,
        product_id,    // ✅ product_id 추가
        group_name,
        group_id,      // ✅ group_id 추가
        order_index: order_index ?? 0,
        updated_at: new Date().toISOString(),
      })
      .eq('id', id)
      .select()
      .single();
    
    if (error) {
      if (error.code === 'PGRST116') {
        return res.status(404).json({ error: 'Endpoint not found' });
      }
      throw error;
    }
    
    console.log('✅ Endpoint moved:', { id, product, product_id, group_name, group_id });
    res.json({ endpoint: data, message: 'Endpoint moved successfully' });
  } catch (error) {
    console.error('❌ Move endpoint error:', error);
    res.status(500).json({ error: error.message });
  }
});

/**
 * PUT /api/endpoints/:id
 * 엔드포인트 수정
 */
router.put('/:id', async (req, res) => {
  try {
    const { id } = req.params;
    const { name, method, path, product, group_name, description, status, statusMessage } = req.body;
    const now = new Date().toISOString();
    
    // product_id, group_id 생성
    const product_id = product;
    const group_id = `${product}_${group_name}`;
    
    const { data, error } = await supabase
      .from('endpoints')
      .update({
        name,
        method,
        path,
        product,
        product_id,    // ✅ 추가
        group_name,
        group_id,      // ✅ 추가
        description,
        status,
        status_message: statusMessage, // 🔥 상태 메시지 추가
        updated_at: now
      })
      .eq('id', id)
      .select();
    
    if (error) throw error;
    
    if (data && data.length > 0) {
      res.json({ message: 'Endpoint updated', changes: data.length });
    } else {
      res.status(404).json({ message: 'Endpoint not found' });
    }
  } catch (error) {
    console.error('Update endpoint error:', error);
    res.status(500).json({ error: error.message });
  }
});

/**
 * DELETE /api/endpoints/orphaned
 * 고아 엔드포인트 삭제 (products 테이블에 없는 제품의 엔드포인트)
 * ⚠️ 주의: /:id 라우트보다 먼저 정의되어야 함!
 */
router.delete('/orphaned', async (req, res) => {
  try {
    console.log('🧹 Cleaning up orphaned endpoints...');
    
    // 1. Products 테이블에서 모든 제품 ID 가져오기
    const { data: products, error: productsError } = await supabase
      .from('products')
      .select('id');
    
    if (productsError) throw productsError;
    
    const validProductIds = new Set(products.map(p => p.id));
    console.log('✅ Valid product IDs:', Array.from(validProductIds));
    
    // 2. Endpoints 테이블에서 모든 엔드포인트 가져오기
    const { data: endpoints, error: endpointsError } = await supabase
      .from('endpoints')
      .select('id, name, product');
    
    if (endpointsError) throw endpointsError;
    
    // 3. 고아 엔드포인트 찾기
    const orphanedEndpoints = endpoints.filter(e => !validProductIds.has(e.product));
    
    if (orphanedEndpoints.length === 0) {
      console.log('✅ No orphaned endpoints found');
      return res.json({ message: 'No orphaned endpoints found', deleted: [] });
    }
    
    console.log('🗑️ Found orphaned endpoints:', orphanedEndpoints.map(e => ({ id: e.id, name: e.name, product: e.product })));
    
    // 4. 고아 엔드포인트 삭제
    const orphanedIds = orphanedEndpoints.map(e => e.id);
    const { data: deleted, error: deleteError } = await supabase
      .from('endpoints')
      .delete()
      .in('id', orphanedIds)
      .select();
    
    if (deleteError) throw deleteError;
    
    console.log('✅ Deleted orphaned endpoints:', deleted);
    res.json({ 
      message: `Deleted ${deleted.length} orphaned endpoints`, 
      deleted: deleted.map(e => ({ id: e.id, name: e.name, product: e.product }))
    });
  } catch (error) {
    console.error('❌ Clean up orphaned endpoints error:', error);
    res.status(500).json({ error: error.message });
  }
});

/**
 * DELETE /api/endpoints/:id
 * 엔드포인트 삭제
 */
router.delete('/:id', async (req, res) => {
  try {
    const { id } = req.params;
    console.log('🗑️ Attempting to delete endpoint:', id);
    
    // 먼저 엔드포인트가 존재하는지 확인
    const { data: existing, error: checkError } = await supabase
      .from('endpoints')
      .select('*')
      .eq('id', id)
      .single();
    
    if (checkError) {
      if (checkError.code === 'PGRST116') {
        console.log('❌ Endpoint not found:', id);
        return res.status(404).json({ message: 'Endpoint not found', id });
      }
      throw checkError;
    }
    
    console.log('✅ Endpoint found:', existing);
    
    // 삭제 시도
    const { data, error } = await supabase
      .from('endpoints')
      .delete()
      .eq('id', id)
      .select();
    
    if (error) {
      console.error('❌ Delete error:', error);
      throw error;
    }
    
    console.log('✅ Deleted successfully:', data);
    res.json({ message: 'Endpoint deleted successfully', deleted: data });
  } catch (error) {
    console.error('❌ Delete endpoint error:', error);
    res.status(500).json({ error: error.message, details: error });
  }
});

/**
 * POST /api/endpoints/seed
 * 초기 데이터 시딩
 */
router.post('/seed', async (req, res) => {
  try {
    const now = new Date().toISOString();
    
    const defaultEndpoints = [
      // Civil NX - DB Group
      { id: 'db/node', name: 'Node', method: 'POST', path: '/db/node', product: 'civil-nx', group_name: 'DB' },
      { id: 'db/beam', name: 'Beam', method: 'POST', path: '/db/beam', product: 'civil-nx', group_name: 'DB' },
      { id: 'db/nlct', name: 'NLCT', method: 'POST', path: '/db/nlct', product: 'civil-nx', group_name: 'DB' },
      { id: 'db/mvct', name: 'MVCT', method: 'POST', path: '/db/mvct', product: 'civil-nx', group_name: 'DB' },
      
      // Civil NX - Gen Group
      { id: 'gen/project', name: 'Project', method: 'GET', path: '/gen/project', product: 'civil-nx', group_name: 'Gen' },
      { id: 'gen/material', name: 'Material', method: 'POST', path: '/gen/material', product: 'civil-nx', group_name: 'Gen' },
      
      // Gen NX - Analysis Group
      { id: 'analysis/static', name: 'Static', method: 'POST', path: '/analysis/static', product: 'gen-nx', group_name: 'Analysis' },
      { id: 'analysis/dynamic', name: 'Dynamic', method: 'POST', path: '/analysis/dynamic', product: 'gen-nx', group_name: 'Analysis' },
    ];
    
    const insertData = defaultEndpoints.map(endpoint => ({
      id: endpoint.id,
      name: endpoint.name,
      method: endpoint.method,
      path: endpoint.path,
      product: endpoint.product,
      group_name: endpoint.group_name,
      description: null,
      status: 'active',
      created_at: now,
      updated_at: now
    }));
    
    // Upsert (INSERT or UPDATE)
    const { data, error } = await supabase
      .from('endpoints')
      .upsert(insertData, { onConflict: 'id' })
      .select();
    
    if (error) throw error;
    
    res.json({ message: 'Endpoints seeded successfully', count: data ? data.length : 0 });
  } catch (error) {
    console.error('Seed endpoints error:', error);
    res.status(500).json({ error: error.message });
  }
});

export default router;





