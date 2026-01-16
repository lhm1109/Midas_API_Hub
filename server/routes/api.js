/**
 * API 라우트 정의 - 자동 문서화
 * 
 * 이 파일에서 정의된 모든 라우트는 자동으로:
 * 1. DB의 endpoints 테이블에 등록
 * 2. 문서가 포함되면 versions + manual_data 테이블에 저장
 * 3. API Docs 탭에서 확인 가능
 * 
 * 사용법:
 * - 새로운 API 엔드포인트를 추가하려면 아래 배열에 라우트 정의를 추가하세요
 * - 서버 재시작 시 자동으로 DB에 동기화됩니다
 */

import express from 'express';
import { registerRoutes } from '../routeRegistry.js';

const router = express.Router();

// ============================================================================
// API 라우트 정의
// ============================================================================

// 여기에 새로운 API 엔드포인트를 추가하세요
// 예시:
// const myRoutes = [
//   {
//     id: 'example/hello',
//     name: 'Hello World',
//     method: 'POST',
//     path: '/example/hello',
//     product: 'my-product',
//     group: 'Example',
//     description: 'Example API endpoint',
//     
//     handler: async (req, res) => {
//       try {
//         res.json({ message: 'Hello World' });
//       } catch (error) {
//         res.status(500).json({ error: error.message });
//       }
//     },
//     
//     docs: {
//       title: 'Hello World API',
//       schema: { type: 'object', properties: {} },
//       examples: [],
//       htmlContent: '<h1>Hello World</h1>'
//     }
//   }
// ];

const allRoutes = [
  // 여기에 라우트 배열을 추가하세요
];

// ============================================================================
// 라우터에 등록
// ============================================================================

registerRoutes(router, allRoutes);

export default router;
