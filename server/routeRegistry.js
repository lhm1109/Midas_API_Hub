/**
 * 라우터 레지스트리 - 자동 문서화 시스템
 * 
 * 사용법:
 * 1. createRoute()로 라우터와 문서를 함께 정의
 * 2. 서버 시작 시 자동으로 DB에 등록
 */

import db from './database.js';

// 등록된 모든 라우트
const routes = [];

/**
 * 라우트 생성 및 등록
 * @param {Object} config - 라우트 설정
 * @returns {Object} - Express Router 미들웨어
 */
export function createRoute(config) {
  const {
    id,           // 예: 'db/nlct'
    name,         // 예: 'NLCT'
    method,       // 예: 'POST'
    path,         // 예: '/db/nlct'
    product,      // 예: 'civil-nx'
    group,        // 예: 'DB'
    description,  // 설명
    handler,      // 실제 라우트 핸들러 함수
    
    // 문서 정보
    docs = {}     // { title, schema, examples, htmlContent, specifications }
  } = config;

  // 라우트 정보 저장
  routes.push({
    id,
    name,
    method: method.toUpperCase(),
    path,
    product: product || 'default',
    group_name: group || 'Default',
    description,
    status: 'active',
    docs,
  });

  // Express 미들웨어 반환
  return {
    method: method.toLowerCase(),
    path,
    handler,
  };
}

/**
 * 여러 라우트를 Express 라우터에 등록
 */
export function registerRoutes(router, routeConfigs) {
  routeConfigs.forEach(config => {
    const route = createRoute(config);
    router[route.method](route.path, route.handler);
  });
  return router;
}

/**
 * 등록된 모든 라우트를 DB에 저장
 */
export function syncRoutesToDatabase() {
  console.log('🔄 Syncing routes to database...');
  
  const now = new Date().toISOString();
  let inserted = 0;
  let updated = 0;
  let withDocs = 0;

  routes.forEach(route => {
    try {
      // 1. Endpoint 저장/업데이트
      const existing = db.prepare('SELECT id FROM endpoints WHERE id = ?').get(route.id);
      
      if (existing) {
        db.prepare(`
          UPDATE endpoints 
          SET name=?, method=?, path=?, product=?, group_name=?, description=?, status=?, updated_at=?
          WHERE id=?
        `).run(
          route.name,
          route.method,
          route.path,
          route.product,
          route.group_name,
          route.description || null,
          route.status,
          now,
          route.id
        );
        updated++;
      } else {
        db.prepare(`
          INSERT INTO endpoints (id, name, method, path, product, group_name, description, status, created_at, updated_at)
          VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
        `).run(
          route.id,
          route.name,
          route.method,
          route.path,
          route.product,
          route.group_name,
          route.description || null,
          route.status,
          now,
          now
        );
        inserted++;
      }

      // 2. 문서가 있으면 기본 버전 생성
      if (route.docs && Object.keys(route.docs).length > 0) {
        const versionId = `v_auto_${route.id.replace(/\//g, '_')}`;
        const existingVersion = db.prepare('SELECT id FROM versions WHERE id = ?').get(versionId);
        
        if (!existingVersion) {
          // 버전 생성
          db.prepare(`
            INSERT INTO versions (id, version, endpoint_id, created_at, updated_at, author, change_log)
            VALUES (?, ?, ?, ?, ?, ?, ?)
          `).run(
            versionId,
            '1.0',
            route.id,
            now,
            now,
            'System',
            'Auto-generated from route definition'
          );

          // Manual 데이터 생성
          if (route.docs.schema || route.docs.htmlContent || route.docs.examples) {
            db.prepare(`
              INSERT INTO manual_data 
              (version_id, title, category, input_uri, active_methods, json_schema, json_schema_original, json_schema_enhanced, examples, specifications, html_content)
              VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
            `).run(
              versionId,
              route.docs.title || route.name,
              route.group_name,
              route.path,
              route.method,
              route.docs.schema ? JSON.stringify(route.docs.schema) : '{}',
              route.docs.schemaOriginal ? JSON.stringify(route.docs.schemaOriginal) : null,
              route.docs.schemaEnhanced ? JSON.stringify(route.docs.schemaEnhanced) : null,
              route.docs.examples ? JSON.stringify(route.docs.examples) : '[]',
              route.docs.specifications || route.description || null,
              route.docs.htmlContent || null
            );
            withDocs++;
          }
        }
      }
    } catch (error) {
      console.error(`❌ Failed to sync route ${route.id}:`, error.message);
    }
  });

  console.log(`✅ Route sync complete:`);
  console.log(`   📌 ${inserted} endpoints inserted`);
  console.log(`   🔄 ${updated} endpoints updated`);
  console.log(`   📖 ${withDocs} with documentation`);
  console.log(`   📊 Total routes: ${routes.length}`);
}

/**
 * 등록된 모든 라우트 정보 반환
 */
export function getRegisteredRoutes() {
  return routes;
}

/**
 * 라우트 정보를 문서 형태로 출력
 */
export function printRouteMap() {
  console.log('\n📋 Registered API Routes:');
  console.log('━'.repeat(80));
  
  const grouped = {};
  routes.forEach(route => {
    const key = `${route.product}/${route.group_name}`;
    if (!grouped[key]) grouped[key] = [];
    grouped[key].push(route);
  });

  Object.entries(grouped).forEach(([group, routeList]) => {
    console.log(`\n📦 ${group}`);
    routeList.forEach(route => {
      const methodColor = {
        'GET': '\x1b[32m',    // green
        'POST': '\x1b[34m',   // blue
        'PUT': '\x1b[33m',    // yellow
        'DELETE': '\x1b[31m', // red
      }[route.method] || '\x1b[37m';
      
      console.log(`  ${methodColor}${route.method.padEnd(7)}\x1b[0m ${route.path.padEnd(30)} → ${route.name}`);
    });
  });
  
  console.log('\n' + '━'.repeat(80) + '\n');
}

export default {
  createRoute,
  registerRoutes,
  syncRoutesToDatabase,
  getRegisteredRoutes,
  printRouteMap,
};





