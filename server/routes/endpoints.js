import express from 'express';
import db from '../database.js';

const router = express.Router();

/**
 * GET /api/endpoints
 * 모든 엔드포인트 목록 조회 (계층 구조)
 */
router.get('/', (req, res) => {
  try {
    const endpoints = db.prepare('SELECT * FROM endpoints ORDER BY product, group_name, name').all();
    res.json(endpoints);
  } catch (error) {
    console.error('Get endpoints error:', error);
    res.status(500).json({ error: error.message });
  }
});

/**
 * GET /api/endpoints/tree
 * 트리 구조로 엔드포인트 조회
 */
router.get('/tree', (req, res) => {
  try {
    const endpoints = db.prepare('SELECT * FROM endpoints ORDER BY product, group_name, name').all();
    
    // 트리 구조로 변환
    const tree = {};
    
    endpoints.forEach(endpoint => {
      // Product 레벨
      if (!tree[endpoint.product]) {
        tree[endpoint.product] = {
          id: endpoint.product,
          name: endpoint.product,
          groups: {}
        };
      }
      
      // Group 레벨
      if (!tree[endpoint.product].groups[endpoint.group_name]) {
        tree[endpoint.product].groups[endpoint.group_name] = {
          id: endpoint.group_name.toLowerCase(),
          name: endpoint.group_name,
          endpoints: []
        };
      }
      
      // Endpoint 레벨
      tree[endpoint.product].groups[endpoint.group_name].endpoints.push({
        id: endpoint.id,
        name: endpoint.name,
        method: endpoint.method,
        path: endpoint.path,
        status: endpoint.status,
        description: endpoint.description
      });
    });
    
    // Object를 Array로 변환
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
router.get('/:id', (req, res) => {
  try {
    const { id } = req.params;
    const endpoint = db.prepare('SELECT * FROM endpoints WHERE id = ?').get(id);
    
    if (endpoint) {
      res.json(endpoint);
    } else {
      res.status(404).json({ message: 'Endpoint not found' });
    }
  } catch (error) {
    console.error('Get endpoint error:', error);
    res.status(500).json({ error: error.message });
  }
});

/**
 * POST /api/endpoints
 * 새 엔드포인트 생성
 */
router.post('/', (req, res) => {
  try {
    const { id, name, method, path, product, group_name, description, status } = req.body;
    const now = new Date().toISOString();
    
    const info = db.prepare(`
      INSERT INTO endpoints (id, name, method, path, product, group_name, description, status, created_at, updated_at)
      VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
    `).run(id, name, method, path, product, group_name, description || null, status || 'active', now, now);
    
    res.status(201).json({ id, message: 'Endpoint created' });
  } catch (error) {
    console.error('Create endpoint error:', error);
    res.status(500).json({ error: error.message });
  }
});

/**
 * PUT /api/endpoints/:id
 * 엔드포인트 수정
 */
router.put('/:id', (req, res) => {
  try {
    const { id } = req.params;
    const { name, method, path, product, group_name, description, status } = req.body;
    const now = new Date().toISOString();
    
    const info = db.prepare(`
      UPDATE endpoints 
      SET name = ?, method = ?, path = ?, product = ?, group_name = ?, description = ?, status = ?, updated_at = ?
      WHERE id = ?
    `).run(name, method, path, product, group_name, description, status, now, id);
    
    if (info.changes > 0) {
      res.json({ message: 'Endpoint updated', changes: info.changes });
    } else {
      res.status(404).json({ message: 'Endpoint not found' });
    }
  } catch (error) {
    console.error('Update endpoint error:', error);
    res.status(500).json({ error: error.message });
  }
});

/**
 * DELETE /api/endpoints/:id
 * 엔드포인트 삭제
 */
router.delete('/:id', (req, res) => {
  try {
    const { id } = req.params;
    
    const info = db.prepare('DELETE FROM endpoints WHERE id = ?').run(id);
    
    if (info.changes > 0) {
      res.json({ message: 'Endpoint deleted', changes: info.changes });
    } else {
      res.status(404).json({ message: 'Endpoint not found' });
    }
  } catch (error) {
    console.error('Delete endpoint error:', error);
    res.status(500).json({ error: error.message });
  }
});

/**
 * POST /api/endpoints/seed
 * 초기 데이터 시딩
 */
router.post('/seed', (req, res) => {
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
    
    const stmt = db.prepare(`
      INSERT OR REPLACE INTO endpoints (id, name, method, path, product, group_name, description, status, created_at, updated_at)
      VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
    `);
    
    let inserted = 0;
    defaultEndpoints.forEach(endpoint => {
      stmt.run(
        endpoint.id,
        endpoint.name,
        endpoint.method,
        endpoint.path,
        endpoint.product,
        endpoint.group_name,
        null,
        'active',
        now,
        now
      );
      inserted++;
    });
    
    res.json({ message: 'Endpoints seeded successfully', count: inserted });
  } catch (error) {
    console.error('Seed endpoints error:', error);
    res.status(500).json({ error: error.message });
  }
});

export default router;




