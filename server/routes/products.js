/**
 * Products API Routes
 */
import express from 'express';
import { createClient } from '@supabase/supabase-js';

const router = express.Router();

const supabase = createClient(
  process.env.SUPABASE_URL,
  process.env.SUPABASE_SERVICE_KEY
);

/**
 * GET /api/products
 * 모든 제품 조회 (순서대로)
 */
router.get('/', async (req, res) => {
  try {
    const { data: products, error } = await supabase
      .from('products')
      .select('*')
      .order('order_index', { ascending: true })
      .order('created_at', { ascending: true });

    if (error) throw error;

    res.json(products || []);
  } catch (error) {
    console.error('Get products error:', error);
    res.status(500).json({ error: error.message });
  }
});

/**
 * GET /api/products/:id
 * 특정 제품 조회
 */
router.get('/:id', async (req, res) => {
  try {
    const { id } = req.params;
    const { data: product, error } = await supabase
      .from('products')
      .select('*')
      .eq('id', id)
      .single();

    if (error) {
      if (error.code === 'PGRST116') {
        return res.status(404).json({ error: 'Product not found' });
      }
      throw error;
    }

    res.json(product);
  } catch (error) {
    console.error('Get product error:', error);
    res.status(500).json({ error: error.message });
  }
});

/**
 * POST /api/products
 * 새 제품 생성
 */
router.post('/', async (req, res) => {
  try {
    const { id, name, description, psd_set, schema_type } = req.body;
    const now = new Date().toISOString();

    if (!id || !name) {
      return res.status(400).json({ error: 'id and name are required' });
    }

    // 현재 최대 order_index 조회
    const { data: maxOrderData } = await supabase
      .from('products')
      .select('order_index')
      .order('order_index', { ascending: false })
      .limit(1)
      .single();

    const nextOrderIndex = (maxOrderData?.order_index ?? -1) + 1;

    const { data, error } = await supabase
      .from('products')
      .insert({
        id,
        name,
        description: description || null,
        psd_set: psd_set || 'default',
        schema_type: schema_type || 'enhanced',
        order_index: nextOrderIndex,
        created_at: now,
        updated_at: now,
      })
      .select()
      .single();

    if (error) throw error;

    console.log('✅ Product created:', id);
    res.status(201).json({ product: data, message: 'Product created successfully' });
  } catch (error) {
    console.error('❌ Create product error:', error);
    res.status(500).json({ error: error.message });
  }
});

/**
 * PUT /api/products/reorder
 * 제품 순서 변경
 */
router.put('/reorder', async (req, res) => {
  try {
    const { products } = req.body;

    console.log('🔄 Reorder products request:', { productsCount: products?.length, products });

    if (!Array.isArray(products)) {
      console.error('❌ Invalid request: products is not an array');
      return res.status(400).json({ error: 'products array is required' });
    }

    if (products.length === 0) {
      console.log('⚠️ No products to reorder');
      return res.json({ message: 'No products to reorder', count: 0 });
    }

    let successCount = 0;
    for (const product of products) {
      console.log(`  Updating ${product.id} to order_index: ${product.order_index}`);

      const { data, error } = await supabase
        .from('products')
        .update({ 
          order_index: product.order_index,
          updated_at: new Date().toISOString()
        })
        .eq('id', product.id)
        .select();

      if (error) {
        console.error('❌ Reorder error for', product.id, ':', error);
        return res.status(500).json({
          error: error.message,
          details: error,
          product: product.id
        });
      }

      console.log(`  ✅ Updated ${product.id}:`, data);
      successCount++;
    }

    console.log(`✅ Successfully reordered ${successCount} products`);
    res.json({
      message: 'Products reordered successfully',
      count: successCount
    });
  } catch (error) {
    console.error('❌ Reorder products error:', error);
    res.status(500).json({
      error: error.message,
      stack: error.stack
    });
  }
});

/**
 * PUT /api/products/:id
 * 제품 수정
 */
router.put('/:id', async (req, res) => {
  try {
    const { id } = req.params;
    const { name, description, psd_set, schema_type } = req.body;

    const updateData = {
      updated_at: new Date().toISOString(),
    };
    
    if (name !== undefined) updateData.name = name;
    if (description !== undefined) updateData.description = description || null;
    if (psd_set !== undefined) updateData.psd_set = psd_set;
    if (schema_type !== undefined) updateData.schema_type = schema_type;

    const { data, error } = await supabase
      .from('products')
      .update(updateData)
      .eq('id', id)
      .select()
      .single();

    if (error) {
      if (error.code === 'PGRST116') {
        return res.status(404).json({ error: 'Product not found' });
      }
      throw error;
    }

    res.json({ product: data, message: 'Product updated successfully' });
  } catch (error) {
    console.error('Update product error:', error);
    res.status(500).json({ error: error.message });
  }
});

/**
 * DELETE /api/products/:id
 * 제품 삭제 (CASCADE로 하위 그룹, 엔드포인트도 모두 삭제)
 */
router.delete('/:id', async (req, res) => {
  try {
    const { id } = req.params;

    console.log('🗑️ Delete product request:', { id });

    // 먼저 제품이 존재하는지 확인
    const { data: existing, error: checkError } = await supabase
      .from('products')
      .select('*')
      .eq('id', id)
      .single();

    if (checkError) {
      console.error('❌ Check error:', checkError);
      if (checkError.code === 'PGRST116') {
        // 🔥 products 테이블에 없는 레거시 제품인 경우
        console.log('⚠️ Product not found in products table, treating as legacy product:', id);
        
        // 레거시 엔드포인트들 조회
        const { data: legacyEndpoints, error: legacyError } = await supabase
          .from('endpoints')
          .select('id, name')
          .eq('product', id);
        
        if (legacyError) throw legacyError;
        
        console.log(`🔍 Found ${legacyEndpoints?.length || 0} legacy endpoints for product ${id}`);
        
        // 레거시 엔드포인트들 삭제
        if (legacyEndpoints && legacyEndpoints.length > 0) {
          const { error: deleteError } = await supabase
            .from('endpoints')
            .delete()
            .eq('product', id);
          
          if (deleteError) {
            console.error('❌ Delete legacy endpoints error:', deleteError);
            throw deleteError;
          }
          
          console.log(`✅ Deleted ${legacyEndpoints.length} legacy endpoints`);
        }
        
        return res.json({ 
          message: 'Legacy product deleted successfully (endpoints only)', 
          deletedEndpoints: legacyEndpoints?.length || 0,
          legacy: true
        });
      }
      throw checkError;
    }

    console.log('📦 Found product:', existing);

    // 🔥 1. 먼저 레거시 엔드포인트 삭제 (product 컬럼 사용)
    const { data: legacyEndpoints } = await supabase
      .from('endpoints')
      .select('id, name')
      .eq('product', id);
    
    if (legacyEndpoints && legacyEndpoints.length > 0) {
      console.log(`🔍 Found ${legacyEndpoints.length} legacy endpoints (using product column)`);
      
      const { error: deleteLegacyError } = await supabase
        .from('endpoints')
        .delete()
        .eq('product', id);
      
      if (deleteLegacyError) {
        console.error('❌ Delete legacy endpoints error:', deleteLegacyError);
      } else {
        console.log(`✅ Deleted ${legacyEndpoints.length} legacy endpoints`);
      }
    }

    // 🔥 2. 그룹 삭제 (CASCADE로 product_id 기반 엔드포인트도 삭제)
    const { data: groups } = await supabase
      .from('groups')
      .select('id, name')
      .eq('product_id', id);
    
    if (groups && groups.length > 0) {
      console.log(`🔍 Found ${groups.length} groups to delete`);
      
      const { error: deleteGroupsError } = await supabase
        .from('groups')
        .delete()
        .eq('product_id', id);
      
      if (deleteGroupsError) {
        console.error('❌ Delete groups error:', deleteGroupsError);
      } else {
        console.log(`✅ Deleted ${groups.length} groups`);
      }
    }

    // 🔥 3. 제품 삭제
    const { data: deleted, error } = await supabase
      .from('products')
      .delete()
      .eq('id', id)
      .select();

    if (error) {
      console.error('❌ Delete error:', error);
      throw error;
    }

    console.log('✅ Product deleted:', deleted);
    res.json({ 
      message: 'Product deleted successfully', 
      deleted,
      deletedLegacyEndpoints: legacyEndpoints?.length || 0,
      deletedGroups: groups?.length || 0
    });
  } catch (error) {
    console.error('❌ Delete product error:', error);
    res.status(500).json({ error: error.message, details: error });
  }
});

export default router;

