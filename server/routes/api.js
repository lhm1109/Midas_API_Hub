/**
 * API 라우트 정의 - 자동 문서화
 * 
 * 이 파일에서 정의된 모든 라우트는 자동으로:
 * 1. DB의 endpoints 테이블에 등록
 * 2. 문서가 포함되면 versions + manual_data 테이블에 저장
 * 3. API Docs 탭에서 확인 가능
 */

import express from 'express';
import { registerRoutes } from '../routeRegistry.js';

const router = express.Router();

// ============================================================================
// Civil NX - DB Group
// ============================================================================

const civilDbRoutes = [
  {
    id: 'db/node',
    name: 'Node',
    method: 'POST',
    path: '/db/node',
    product: 'civil-nx',
    group: 'DB',
    description: 'Create or update node data in the structural model',
    
    handler: async (req, res) => {
      try {
        // 실제 비즈니스 로직
        const { id, x, y, z } = req.body;
        
        // TODO: 실제 구현
        res.json({
          success: true,
          message: 'Node created',
          data: { id, x, y, z }
        });
      } catch (error) {
        res.status(500).json({ error: error.message });
      }
    },
    
    docs: {
      title: 'Node API',
      schema: {
        type: 'object',
        properties: {
          id: { type: 'string', description: 'Node ID' },
          x: { type: 'number', description: 'X coordinate' },
          y: { type: 'number', description: 'Y coordinate' },
          z: { type: 'number', description: 'Z coordinate' },
        },
        required: ['id', 'x', 'y', 'z']
      },
      examples: [
        {
          name: 'Create a node at origin',
          description: 'Creates a node at coordinates (0, 0, 0)',
          request: {
            id: 'N1',
            x: 0,
            y: 0,
            z: 0
          },
          response: {
            success: true,
            message: 'Node created',
            data: { id: 'N1', x: 0, y: 0, z: 0 }
          }
        },
        {
          name: 'Create a node at custom position',
          description: 'Creates a node at specified coordinates',
          request: {
            id: 'N2',
            x: 10.5,
            y: 20.3,
            z: 5.0
          },
          response: {
            success: true,
            message: 'Node created',
            data: { id: 'N2', x: 10.5, y: 20.3, z: 5.0 }
          }
        }
      ],
      htmlContent: `
        <div class="api-manual">
          <h1>Node API</h1>
          <p>이 API는 구조 모델에 노드를 생성하거나 업데이트합니다.</p>
          
          <h2>사용 방법</h2>
          <pre><code>POST /api/civil/db/node
Content-Type: application/json

{
  "id": "N1",
  "x": 0,
  "y": 0,
  "z": 0
}</code></pre>

          <h2>주의사항</h2>
          <ul>
            <li>Node ID는 고유해야 합니다</li>
            <li>좌표는 모델의 전역 좌표계를 기준으로 합니다</li>
            <li>중복된 ID로 요청 시 기존 노드가 업데이트됩니다</li>
          </ul>
        </div>
      `,
      specifications: 'Creates or updates a node in the structural model with specified 3D coordinates.'
    }
  },

  {
    id: 'db/beam',
    name: 'Beam',
    method: 'POST',
    path: '/db/beam',
    product: 'civil-nx',
    group: 'DB',
    description: 'Create beam elements between two nodes',
    
    handler: async (req, res) => {
      try {
        const { id, nodeI, nodeJ, section } = req.body;
        
        res.json({
          success: true,
          message: 'Beam created',
          data: { id, nodeI, nodeJ, section }
        });
      } catch (error) {
        res.status(500).json({ error: error.message });
      }
    },
    
    docs: {
      title: 'Beam Element API',
      schema: {
        type: 'object',
        properties: {
          id: { type: 'string', description: 'Beam ID' },
          nodeI: { type: 'string', description: 'Start node ID' },
          nodeJ: { type: 'string', description: 'End node ID' },
          section: { type: 'string', description: 'Section property ID' },
        },
        required: ['id', 'nodeI', 'nodeJ']
      },
      examples: [
        {
          name: 'Create a simple beam',
          request: {
            id: 'B1',
            nodeI: 'N1',
            nodeJ: 'N2',
            section: 'SEC1'
          },
          response: {
            success: true,
            message: 'Beam created',
            data: { id: 'B1', nodeI: 'N1', nodeJ: 'N2', section: 'SEC1' }
          }
        }
      ],
      htmlContent: `
        <div class="api-manual">
          <h1>Beam Element API</h1>
          <p>두 노드 사이에 보 요소를 생성합니다.</p>
          
          <h2>필수 조건</h2>
          <ul>
            <li>시작 노드(nodeI)와 끝 노드(nodeJ)가 미리 생성되어 있어야 합니다</li>
            <li>Section 속성이 정의되어 있어야 합니다</li>
          </ul>
        </div>
      `
    }
  },

  {
    id: 'db/nlct',
    name: 'NLCT',
    method: 'POST',
    path: '/db/nlct',
    product: 'civil-nx',
    group: 'DB',
    description: 'Create Non-Linear Constraint Table',
    
    handler: async (req, res) => {
      try {
        const { name, values } = req.body;
        
        res.json({
          success: true,
          message: 'NLCT created',
          data: { name, values }
        });
      } catch (error) {
        res.status(500).json({ error: error.message });
      }
    },
    
    docs: {
      title: 'Non-Linear Constraint Table (NLCT)',
      schema: {
        type: 'object',
        properties: {
          name: { type: 'string', description: 'Table name' },
          values: { 
            type: 'array',
            items: { type: 'number' },
            description: 'Constraint values'
          },
        },
        required: ['name', 'values']
      },
      examples: [
        {
          name: 'Create NLCT with simple values',
          request: {
            name: 'NLCT1',
            values: [1, 2, 3, 4, 5]
          },
          response: {
            success: true,
            message: 'NLCT created',
            data: { name: 'NLCT1', values: [1, 2, 3, 4, 5] }
          }
        }
      ]
    }
  },

  {
    id: 'db/mvct',
    name: 'MVCT',
    method: 'POST',
    path: '/db/mvct',
    product: 'civil-nx',
    group: 'DB',
    description: 'Create Multi-Variable Constraint Table',
    
    handler: async (req, res) => {
      try {
        const { name, variables } = req.body;
        
        res.json({
          success: true,
          message: 'MVCT created',
          data: { name, variables }
        });
      } catch (error) {
        res.status(500).json({ error: error.message });
      }
    },
    
    docs: {
      title: 'Multi-Variable Constraint Table (MVCT)',
      schema: {
        type: 'object',
        properties: {
          name: { type: 'string' },
          variables: { type: 'array' },
        }
      }
    }
  },
];

// ============================================================================
// Civil NX - Gen Group
// ============================================================================

const civilGenRoutes = [
  {
    id: 'gen/project',
    name: 'Project',
    method: 'GET',
    path: '/gen/project',
    product: 'civil-nx',
    group: 'Gen',
    description: 'Get project information',
    
    handler: async (req, res) => {
      try {
        res.json({
          success: true,
          data: {
            name: 'Sample Project',
            version: '1.0',
            created: new Date().toISOString()
          }
        });
      } catch (error) {
        res.status(500).json({ error: error.message });
      }
    },
    
    docs: {
      title: 'Project Information API',
      schema: {
        type: 'object',
        properties: {
          name: { type: 'string' },
          version: { type: 'string' },
          created: { type: 'string', format: 'date-time' }
        }
      },
      examples: [
        {
          name: 'Get project info',
          request: {},
          response: {
            success: true,
            data: {
              name: 'Sample Project',
              version: '1.0',
              created: '2026-01-08T10:00:00Z'
            }
          }
        }
      ]
    }
  },

  {
    id: 'gen/material',
    name: 'Material',
    method: 'POST',
    path: '/gen/material',
    product: 'civil-nx',
    group: 'Gen',
    description: 'Create material properties',
    
    handler: async (req, res) => {
      try {
        const { id, type, properties } = req.body;
        
        res.json({
          success: true,
          message: 'Material created',
          data: { id, type, properties }
        });
      } catch (error) {
        res.status(500).json({ error: error.message });
      }
    },
    
    docs: {
      title: 'Material Properties API',
      schema: {
        type: 'object',
        properties: {
          id: { type: 'string' },
          type: { type: 'string', enum: ['steel', 'concrete', 'timber'] },
          properties: { type: 'object' }
        }
      }
    }
  },
];

// ============================================================================
// Gen NX - Analysis Group
// ============================================================================

const genAnalysisRoutes = [
  {
    id: 'analysis/static',
    name: 'Static Analysis',
    method: 'POST',
    path: '/analysis/static',
    product: 'gen-nx',
    group: 'Analysis',
    description: 'Run static structural analysis',
    
    handler: async (req, res) => {
      try {
        const { modelId, options } = req.body;
        
        res.json({
          success: true,
          message: 'Analysis completed',
          results: {
            displacement: [],
            stress: [],
            reaction: []
          }
        });
      } catch (error) {
        res.status(500).json({ error: error.message });
      }
    },
    
    docs: {
      title: 'Static Analysis API',
      htmlContent: `
        <h1>Static Analysis</h1>
        <p>정적 구조 해석을 수행합니다.</p>
      `
    }
  },

  {
    id: 'analysis/dynamic',
    name: 'Dynamic Analysis',
    method: 'POST',
    path: '/analysis/dynamic',
    product: 'gen-nx',
    group: 'Analysis',
    description: 'Run dynamic structural analysis',
    
    handler: async (req, res) => {
      try {
        const { modelId, timeSteps } = req.body;
        
        res.json({
          success: true,
          message: 'Dynamic analysis completed'
        });
      } catch (error) {
        res.status(500).json({ error: error.message });
      }
    },
    
    docs: {
      title: 'Dynamic Analysis API'
    }
  },
];

// ============================================================================
// 라우터에 등록
// ============================================================================

registerRoutes(router, [...civilDbRoutes, ...civilGenRoutes, ...genAnalysisRoutes]);

export default router;

