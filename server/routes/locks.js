import express from 'express';
import supabase from '../database.js';

const router = express.Router();

// ============================================================================
// 📌 Supabase용 편집 잠금 API
// ============================================================================

// 만료된 잠금 자동 정리
async function cleanupExpiredLocks() {
  const now = new Date().toISOString();
  
  await Promise.all([
    supabase.from('endpoint_locks').delete().lt('expires_at', now),
    supabase.from('version_locks').delete().lt('expires_at', now)
  ]);
}

// 1분마다 자동 정리
setInterval(cleanupExpiredLocks, 60 * 1000);

// ============================================================================
// 🔒 엔드포인트 잠금
// ============================================================================

// 잠금 획득
router.post('/endpoint/:endpointId/lock', async (req, res) => {
  try {
    const { endpointId } = req.params;
    const { userId } = req.body;
    
    if (!userId) {
      return res.status(400).json({ error: 'userId is required' });
    }
    
    await cleanupExpiredLocks();
    
    // 이미 잠겨있는지 확인
    const { data: existingLock, error: checkError } = await supabase
      .from('endpoint_locks')
      .select('*')
      .eq('endpoint_id', endpointId)
      .single();
    
    if (checkError && checkError.code !== 'PGRST116') {
      throw checkError;
    }
    
    if (existingLock) {
      if (existingLock.locked_by === userId) {
        // 같은 사용자면 갱신
        const expiresAt = new Date(Date.now() + 5 * 60 * 1000).toISOString(); // 5분
        const now = new Date().toISOString();
        
        const { error: updateError } = await supabase
          .from('endpoint_locks')
          .update({ 
            last_activity: now, 
            expires_at: expiresAt 
          })
          .eq('endpoint_id', endpointId);
        
        if (updateError) throw updateError;
        
        return res.json({ 
          success: true, 
          message: 'Lock refreshed',
          lock: { endpointId, lockedBy: userId, expiresAt }
        });
      } else {
        // 다른 사용자가 잠금 중
        return res.status(423).json({ 
          error: 'Endpoint is locked by another user',
          lockedBy: existingLock.locked_by,
          expiresAt: existingLock.expires_at
        });
      }
    }
    
    // 새 잠금 생성
    const now = new Date().toISOString();
    const expiresAt = new Date(Date.now() + 5 * 60 * 1000).toISOString();
    
    const { error: insertError } = await supabase
      .from('endpoint_locks')
      .insert({
        endpoint_id: endpointId,
        locked_by: userId,
        locked_at: now,
        expires_at: expiresAt,
        last_activity: now
      });
    
    if (insertError) throw insertError;
    
    res.json({ 
      success: true,
      lock: { endpointId, lockedBy: userId, expiresAt }
    });
    
  } catch (error) {
    console.error('Lock error:', error);
    res.status(500).json({ error: 'Failed to acquire lock', details: error.message });
  }
});

// 잠금 해제
router.delete('/endpoint/:endpointId/lock', async (req, res) => {
  try {
    const { endpointId } = req.params;
    const { userId } = req.body;
    
    const { data: existingLock, error: checkError } = await supabase
      .from('endpoint_locks')
      .select('*')
      .eq('endpoint_id', endpointId)
      .single();
    
    if (checkError) {
      if (checkError.code === 'PGRST116') {
        return res.json({ success: true, message: 'No lock exists' });
      }
      throw checkError;
    }
    
    if (existingLock.locked_by !== userId) {
      return res.status(403).json({ 
        error: 'Cannot unlock - locked by another user',
        lockedBy: existingLock.locked_by
      });
    }
    
    const { error: deleteError } = await supabase
      .from('endpoint_locks')
      .delete()
      .eq('endpoint_id', endpointId);
    
    if (deleteError) throw deleteError;
    
    res.json({ success: true });
    
  } catch (error) {
    console.error('Unlock error:', error);
    res.status(500).json({ error: 'Failed to release lock', details: error.message });
  }
});

// 잠금 상태 확인
router.get('/endpoint/:endpointId/lock', async (req, res) => {
  try {
    const { endpointId } = req.params;
    
    await cleanupExpiredLocks();
    
    const { data: lock, error } = await supabase
      .from('endpoint_locks')
      .select('*')
      .eq('endpoint_id', endpointId)
      .single();
    
    if (error) {
      if (error.code === 'PGRST116') {
        return res.json({ locked: false });
      }
      throw error;
    }
    
    res.json({ 
      locked: true,
      lockedBy: lock.locked_by,
      lockedAt: lock.locked_at,
      expiresAt: lock.expires_at
    });
    
  } catch (error) {
    console.error('Check lock error:', error);
    res.status(500).json({ error: 'Failed to check lock status', details: error.message });
  }
});

// ============================================================================
// 🔒 버전 잠금 (선택적 - 세밀한 제어 필요 시)
// ============================================================================

// 잠금 획득
router.post('/version/:versionId/lock', async (req, res) => {
  try {
    const { versionId } = req.params;
    const { userId } = req.body;
    
    if (!userId) {
      return res.status(400).json({ error: 'userId is required' });
    }
    
    await cleanupExpiredLocks();
    
    // 이미 잠겨있는지 확인
    const { data: existingLock, error: checkError } = await supabase
      .from('version_locks')
      .select('*')
      .eq('version_id', versionId)
      .single();
    
    if (checkError && checkError.code !== 'PGRST116') {
      throw checkError;
    }
    
    if (existingLock) {
      if (existingLock.locked_by === userId) {
        // 같은 사용자면 갱신
        const expiresAt = new Date(Date.now() + 5 * 60 * 1000).toISOString();
        const now = new Date().toISOString();
        
        const { error: updateError } = await supabase
          .from('version_locks')
          .update({ 
            last_activity: now, 
            expires_at: expiresAt 
          })
          .eq('version_id', versionId);
        
        if (updateError) throw updateError;
        
        return res.json({ 
          success: true, 
          message: 'Lock refreshed',
          lock: { versionId, lockedBy: userId, expiresAt }
        });
      } else {
        // 다른 사용자가 잠금 중
        return res.status(423).json({ 
          error: 'Version is locked by another user',
          lockedBy: existingLock.locked_by,
          expiresAt: existingLock.expires_at
        });
      }
    }
    
    // 새 잠금 생성
    const now = new Date().toISOString();
    const expiresAt = new Date(Date.now() + 5 * 60 * 1000).toISOString();
    
    const { error: insertError } = await supabase
      .from('version_locks')
      .insert({
        version_id: versionId,
        locked_by: userId,
        locked_at: now,
        expires_at: expiresAt,
        last_activity: now
      });
    
    if (insertError) throw insertError;
    
    res.json({ 
      success: true,
      lock: { versionId, lockedBy: userId, expiresAt }
    });
    
  } catch (error) {
    console.error('Lock error:', error);
    res.status(500).json({ error: 'Failed to acquire lock', details: error.message });
  }
});

// 잠금 해제
router.delete('/version/:versionId/lock', async (req, res) => {
  try {
    const { versionId } = req.params;
    const { userId } = req.body;
    
    const { data: existingLock, error: checkError } = await supabase
      .from('version_locks')
      .select('*')
      .eq('version_id', versionId)
      .single();
    
    if (checkError) {
      if (checkError.code === 'PGRST116') {
        return res.json({ success: true, message: 'No lock exists' });
      }
      throw checkError;
    }
    
    if (existingLock.locked_by !== userId) {
      return res.status(403).json({ 
        error: 'Cannot unlock - locked by another user',
        lockedBy: existingLock.locked_by
      });
    }
    
    const { error: deleteError } = await supabase
      .from('version_locks')
      .delete()
      .eq('version_id', versionId);
    
    if (deleteError) throw deleteError;
    
    res.json({ success: true });
    
  } catch (error) {
    console.error('Unlock error:', error);
    res.status(500).json({ error: 'Failed to release lock', details: error.message });
  }
});

// 잠금 상태 확인
router.get('/version/:versionId/lock', async (req, res) => {
  try {
    const { versionId } = req.params;
    
    await cleanupExpiredLocks();
    
    const { data: lock, error } = await supabase
      .from('version_locks')
      .select('*')
      .eq('version_id', versionId)
      .single();
    
    if (error) {
      if (error.code === 'PGRST116') {
        return res.json({ locked: false });
      }
      throw error;
    }
    
    res.json({ 
      locked: true,
      lockedBy: lock.locked_by,
      lockedAt: lock.locked_at,
      expiresAt: lock.expires_at
    });
    
  } catch (error) {
    console.error('Check lock error:', error);
    res.status(500).json({ error: 'Failed to check lock status', details: error.message });
  }
});

// ============================================================================
// 📊 잠금 목록 조회 (관리용)
// ============================================================================

router.get('/all', async (req, res) => {
  try {
    await cleanupExpiredLocks();
    
    const [endpointLocksResult, versionLocksResult] = await Promise.all([
      supabase.from('endpoint_locks').select('*').order('locked_at', { ascending: false }),
      supabase.from('version_locks').select('*').order('locked_at', { ascending: false })
    ]);
    
    res.json({
      endpointLocks: endpointLocksResult.data || [],
      versionLocks: versionLocksResult.data || []
    });
    
  } catch (error) {
    console.error('Get all locks error:', error);
    res.status(500).json({ error: 'Failed to get locks', details: error.message });
  }
});

export default router;
