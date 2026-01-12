import express from 'express';
import db from '../database.js';

const router = express.Router();

/**
 * GET /api/debug/database
 * 데이터베이스의 모든 테이블 내용 조회
 */
router.get('/database', (req, res) => {
  try {
    // 모든 테이블의 데이터 조회
    const endpoints = db.prepare('SELECT * FROM endpoints').all();
    const versions = db.prepare('SELECT * FROM versions').all();
    const manualData = db.prepare('SELECT * FROM manual_data').all();
    const specData = db.prepare('SELECT * FROM spec_data').all();
    const builderData = db.prepare('SELECT * FROM builder_data').all();
    const runnerData = db.prepare('SELECT * FROM runner_data').all();
    const testCases = db.prepare('SELECT * FROM test_cases').all();

    res.json({
      endpoints,
      versions,
      manualData,
      specData,
      builderData,
      runnerData,
      testCases,
      summary: {
        totalEndpoints: endpoints.length,
        totalVersions: versions.length,
        totalManualData: manualData.length,
        totalSpecData: specData.length,
        totalBuilderData: builderData.length,
        totalRunnerData: runnerData.length,
        totalTestCases: testCases.length,
      },
    });
  } catch (error) {
    console.error('Database query error:', error);
    res.status(500).json({ error: error.message });
  }
});

/**
 * DELETE /api/debug/database/clear
 * 데이터베이스의 모든 데이터 삭제 (테이블 구조는 유지)
 */
router.delete('/database/clear', (req, res) => {
  try {
    // 모든 테이블의 데이터 삭제
    db.prepare('DELETE FROM test_cases').run();
    db.prepare('DELETE FROM runner_data').run();
    db.prepare('DELETE FROM builder_data').run();
    db.prepare('DELETE FROM spec_data').run();
    db.prepare('DELETE FROM manual_data').run();
    db.prepare('DELETE FROM versions').run();

    console.log('✅ Database cleared successfully');
    
    res.json({ 
      message: 'Database cleared successfully',
      clearedTables: [
        'versions',
        'manual_data',
        'spec_data',
        'builder_data',
        'runner_data',
        'test_cases',
      ],
    });
  } catch (error) {
    console.error('Database clear error:', error);
    res.status(500).json({ error: error.message });
  }
});

/**
 * GET /api/debug/database/stats
 * 데이터베이스 통계 조회
 */
router.get('/database/stats', (req, res) => {
  try {
    const stats = {
      versions: db.prepare('SELECT COUNT(*) as count FROM versions').get(),
      manualData: db.prepare('SELECT COUNT(*) as count FROM manual_data').get(),
      specData: db.prepare('SELECT COUNT(*) as count FROM spec_data').get(),
      builderData: db.prepare('SELECT COUNT(*) as count FROM builder_data').get(),
      runnerData: db.prepare('SELECT COUNT(*) as count FROM runner_data').get(),
      testCases: db.prepare('SELECT COUNT(*) as count FROM test_cases').get(),
    };

    res.json(stats);
  } catch (error) {
    console.error('Database stats error:', error);
    res.status(500).json({ error: error.message });
  }
});

/**
 * GET /api/debug/database/table/:tableName
 * 특정 테이블의 데이터 조회
 */
router.get('/database/table/:tableName', (req, res) => {
  try {
    const { tableName } = req.params;
    const validTables = ['versions', 'manual_data', 'spec_data', 'builder_data', 'runner_data', 'test_cases'];
    
    if (!validTables.includes(tableName)) {
      return res.status(400).json({ error: 'Invalid table name' });
    }

    const data = db.prepare(`SELECT * FROM ${tableName}`).all();
    res.json({ table: tableName, data, count: data.length });
  } catch (error) {
    console.error('Table query error:', error);
    res.status(500).json({ error: error.message });
  }
});

/**
 * POST /api/debug/database/migrate-endpoint-ids
 * 기존 endpoint_id를 새로운 계층 구조로 마이그레이션
 */
router.post('/database/migrate-endpoint-ids', (req, res) => {
  try {
    // 기존 버전들의 endpoint_id 확인
    const versions = db.prepare('SELECT id, endpoint_id FROM versions').all();
    
    const migrations = {
      'nlct': 'db/nlct',
      'node': 'db/node',
      'beam': 'db/beam',
      'mvct': 'db/mvct',
      'project': 'gen/project',
      'material': 'gen/material',
      'static': 'analysis/static',
      'dynamic': 'analysis/dynamic',
    };
    
    let updated = 0;
    const updateStmt = db.prepare('UPDATE versions SET endpoint_id = ? WHERE id = ?');
    
    versions.forEach(version => {
      const oldId = version.endpoint_id;
      const newId = migrations[oldId];
      
      if (newId && newId !== oldId) {
        updateStmt.run(newId, version.id);
        updated++;
        console.log(`  Migrated: ${oldId} → ${newId} (version: ${version.id})`);
      }
    });
    
    res.json({ 
      message: 'Migration completed', 
      updated,
      migrations: Object.entries(migrations).map(([old, newId]) => ({ old, new: newId }))
    });
  } catch (error) {
    console.error('Migration error:', error);
    res.status(500).json({ error: error.message });
  }
});

export default router;

