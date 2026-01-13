import express from 'express';
import supabase from '../database.js';

const router = express.Router();

/**
 * GET /api/debug/supabase
 * Supabase PostgreSQL 상태 조회
 */
router.get('/supabase', async (req, res) => {
  try {
    // Products
    const { data: products, error: productsError } = await supabase
      .from('products')
      .select('*')
      .order('order_index', { ascending: true });
    
    if (productsError) throw productsError;

    // Groups
    const { data: groups, error: groupsError } = await supabase
      .from('groups')
      .select('*')
      .order('product_id', { ascending: true })
      .order('order_index', { ascending: true });
    
    if (groupsError) throw groupsError;

    // Endpoints
    const { data: endpoints, error: endpointsError } = await supabase
      .from('endpoints')
      .select('*')
      .order('product', { ascending: true })
      .order('group_name', { ascending: true })
      .order('order_index', { ascending: true });
    
    if (endpointsError) throw endpointsError;

    // Versions
    const { data: versions, error: versionsError } = await supabase
      .from('versions')
      .select('*')
      .order('created_at', { ascending: false })
      .limit(50);
    
    if (versionsError) throw versionsError;

    // Endpoint Locks (optional - may not exist)
    let endpointLocks = [];
    try {
      const { data, error } = await supabase
        .from('endpoint_locks')
        .select('*')
        .order('locked_at', { ascending: false });
      
      if (!error) {
        endpointLocks = data || [];
      }
    } catch (err) {
      console.warn('⚠️ endpoint_locks table not found (optional)');
    }

    // Version Locks (optional - may not exist)
    let versionLocks = [];
    try {
      const { data, error } = await supabase
        .from('version_locks')
        .select('*')
        .order('locked_at', { ascending: false });
      
      if (!error) {
        versionLocks = data || [];
      }
    } catch (err) {
      console.warn('⚠️ version_locks table not found (optional)');
    }

    res.json({
      products: products || [],
      groups: groups || [],
      endpoints: endpoints || [],
      versions: versions || [],
      locks: [
        ...endpointLocks.map(lock => ({ ...lock, type: 'endpoint' })),
        ...versionLocks.map(lock => ({ ...lock, type: 'version' }))
      ],
      timestamp: new Date().toISOString(),
    });
  } catch (error) {
    console.error('❌ Supabase debug error:', error);
    res.status(500).json({ error: error.message });
  }
});

/**
 * GET /api/debug/database
 * 로컬 데이터베이스 상태 조회 (기존)
 */
router.get('/database', async (req, res) => {
  try {
    const { data: endpoints, error: endpointsError } = await supabase
      .from('endpoints')
      .select('*');
    
    const { data: versions, error: versionsError } = await supabase
      .from('versions')
      .select('*');
    
    const { data: manualData, error: manualError } = await supabase
      .from('manual_data')
      .select('*');
    
    const { data: specData, error: specError } = await supabase
      .from('spec_data')
      .select('*');
    
    const { data: builderData, error: builderError } = await supabase
      .from('builder_data')
      .select('*');
    
    const { data: runnerData, error: runnerError } = await supabase
      .from('runner_data')
      .select('*');
    
    const { data: testCases, error: testCasesError } = await supabase
      .from('test_cases')
      .select('*');
    
    res.json({
      endpoints: endpoints || [],
      versions: versions || [],
      manualData: manualData || [],
      specData: specData || [],
      builderData: builderData || [],
      runnerData: runnerData || [],
      testCases: testCases || [],
    });
  } catch (error) {
    console.error('❌ Database debug error:', error);
    res.status(500).json({ error: error.message });
  }
});

/**
 * DELETE /api/debug/database/clear
 * 데이터베이스 초기화 (위험)
 */
router.delete('/database/clear', async (req, res) => {
  try {
    console.warn('⚠️ Clearing database...');
    
    // 순서대로 삭제 (foreign key 제약 때문)
    await supabase.from('test_cases').delete().neq('id', '');
    await supabase.from('runner_data').delete().neq('version_id', '');
    await supabase.from('builder_data').delete().neq('version_id', '');
    await supabase.from('spec_data').delete().neq('version_id', '');
    await supabase.from('manual_data').delete().neq('version_id', '');
    await supabase.from('versions').delete().neq('id', '');
    await supabase.from('endpoints').delete().neq('id', '');
    await supabase.from('groups').delete().neq('id', '');
    await supabase.from('products').delete().neq('id', '');
    await supabase.from('endpoint_locks').delete().neq('endpoint_id', '');
    await supabase.from('version_locks').delete().neq('version_id', '');
    
    res.json({ message: 'Database cleared successfully' });
  } catch (error) {
    console.error('❌ Clear database error:', error);
    res.status(500).json({ error: error.message });
  }
});

/**
 * GET /api/debug/database/export
 * 전체 데이터베이스 Export (JSON)
 */
router.get('/database/export', async (req, res) => {
  try {
    console.log('📦 Exporting database...');
    
    // 모든 테이블 데이터 가져오기
    const { data: products } = await supabase.from('products').select('*');
    const { data: groups } = await supabase.from('groups').select('*');
    const { data: endpoints } = await supabase.from('endpoints').select('*');
    const { data: versions } = await supabase.from('versions').select('*');
    const { data: manualData } = await supabase.from('manual_data').select('*');
    const { data: specData } = await supabase.from('spec_data').select('*');
    const { data: builderData } = await supabase.from('builder_data').select('*');
    const { data: runnerData } = await supabase.from('runner_data').select('*');
    const { data: testCases } = await supabase.from('test_cases').select('*');
    const { data: attachments } = await supabase.from('attachments').select('*');
    
    const backup = {
      version: '1.0',
      exportedAt: new Date().toISOString(),
      tables: {
        products: products || [],
        groups: groups || [],
        endpoints: endpoints || [],
        versions: versions || [],
        manual_data: manualData || [],
        spec_data: specData || [],
        builder_data: builderData || [],
        runner_data: runnerData || [],
        test_cases: testCases || [],
        attachments: attachments || [],
      },
      stats: {
        products: products?.length || 0,
        groups: groups?.length || 0,
        endpoints: endpoints?.length || 0,
        versions: versions?.length || 0,
        testCases: testCases?.length || 0,
      }
    };
    
    console.log('✅ Database exported:', backup.stats);
    
    res.setHeader('Content-Type', 'application/json');
    res.setHeader('Content-Disposition', `attachment; filename="supabase-backup-${new Date().toISOString().split('T')[0]}.json"`);
    res.json(backup);
  } catch (error) {
    console.error('❌ Export database error:', error);
    res.status(500).json({ error: error.message });
  }
});

/**
 * POST /api/debug/database/import
 * 전체 데이터베이스 Import (덮어쓰기)
 */
router.post('/database/import', async (req, res) => {
  try {
    const backup = req.body;
    
    console.log('📥 Importing database...');
    console.log('Backup version:', backup.version);
    console.log('Backup stats:', backup.stats);
    
    if (!backup.tables) {
      throw new Error('Invalid backup format: missing tables');
    }
    
    // 1. 기존 데이터 전체 삭제 (순서 중요!)
    console.log('🗑️ Clearing existing data...');
    await supabase.from('test_cases').delete().neq('id', '');
    await supabase.from('attachments').delete().neq('id', '');
    await supabase.from('runner_data').delete().neq('version_id', '');
    await supabase.from('builder_data').delete().neq('version_id', '');
    await supabase.from('spec_data').delete().neq('version_id', '');
    await supabase.from('manual_data').delete().neq('version_id', '');
    await supabase.from('versions').delete().neq('id', '');
    await supabase.from('endpoints').delete().neq('id', '');
    await supabase.from('groups').delete().neq('id', '');
    await supabase.from('products').delete().neq('id', '');
    
    // 2. 백업 데이터 삽입 (UPSERT 방식)
    console.log('📝 Upserting backup data...');
    
    if (backup.tables.products?.length > 0) {
      const { error } = await supabase
        .from('products')
        .upsert(backup.tables.products, { onConflict: 'id' });
      if (error) throw error;
      console.log(`  ✅ Products: ${backup.tables.products.length}`);
    }
    
    if (backup.tables.groups?.length > 0) {
      const { error } = await supabase
        .from('groups')
        .upsert(backup.tables.groups, { onConflict: 'id' });
      if (error) throw error;
      console.log(`  ✅ Groups: ${backup.tables.groups.length}`);
    }
    
    if (backup.tables.endpoints?.length > 0) {
      const { error } = await supabase
        .from('endpoints')
        .upsert(backup.tables.endpoints, { onConflict: 'id' });
      if (error) throw error;
      console.log(`  ✅ Endpoints: ${backup.tables.endpoints.length}`);
    }
    
    if (backup.tables.versions?.length > 0) {
      const { error } = await supabase
        .from('versions')
        .upsert(backup.tables.versions, { onConflict: 'id' });
      if (error) throw error;
      console.log(`  ✅ Versions: ${backup.tables.versions.length}`);
    }
    
    if (backup.tables.manual_data?.length > 0) {
      const { error } = await supabase
        .from('manual_data')
        .upsert(backup.tables.manual_data, { onConflict: 'version_id' });
      if (error) throw error;
      console.log(`  ✅ Manual Data: ${backup.tables.manual_data.length}`);
    }
    
    if (backup.tables.spec_data?.length > 0) {
      const { error } = await supabase
        .from('spec_data')
        .upsert(backup.tables.spec_data, { onConflict: 'version_id' });
      if (error) throw error;
      console.log(`  ✅ Spec Data: ${backup.tables.spec_data.length}`);
    }
    
    if (backup.tables.builder_data?.length > 0) {
      const { error } = await supabase
        .from('builder_data')
        .upsert(backup.tables.builder_data, { onConflict: 'version_id' });
      if (error) throw error;
      console.log(`  ✅ Builder Data: ${backup.tables.builder_data.length}`);
    }
    
    if (backup.tables.runner_data?.length > 0) {
      const { error } = await supabase
        .from('runner_data')
        .upsert(backup.tables.runner_data, { onConflict: 'version_id' });
      if (error) throw error;
      console.log(`  ✅ Runner Data: ${backup.tables.runner_data.length}`);
    }
    
    if (backup.tables.test_cases?.length > 0) {
      const { error } = await supabase
        .from('test_cases')
        .upsert(backup.tables.test_cases, { onConflict: 'id' });
      if (error) throw error;
      console.log(`  ✅ Test Cases: ${backup.tables.test_cases.length}`);
    }
    
    if (backup.tables.attachments?.length > 0) {
      const { error } = await supabase
        .from('attachments')
        .upsert(backup.tables.attachments, { onConflict: 'id' });
      if (error) throw error;
      console.log(`  ✅ Attachments: ${backup.tables.attachments.length}`);
    }
    
    console.log('✅ Database import complete!');
    
    res.json({ 
      message: 'Database imported successfully',
      stats: backup.stats
    });
  } catch (error) {
    console.error('❌ Import database error:', error);
    res.status(500).json({ error: error.message });
  }
});

export default router;
