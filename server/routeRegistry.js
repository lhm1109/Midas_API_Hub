/**
 * 라우터 레지스트리 - 자동 문서화 시스템
 * 
 * 사용법:
 * 1. createRoute()로 라우터와 문서를 함께 정의
 * 2. 서버 시작 시 자동으로 DB에 등록
 */

import supabase from './database.js';

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
export async function syncRoutesToDatabase() {
  console.log('🔄 Syncing routes to database...');
  
  const now = new Date().toISOString();
  let inserted = 0;
  let updated = 0;
  let withDocs = 0;

  for (const route of routes) {
    try {
      // 1. Endpoint 저장/업데이트
      const { data: existing, error: checkError } = await supabase
        .from('endpoints')
        .select('id')
        .eq('id', route.id)
        .single();
      
      if (checkError && checkError.code !== 'PGRST116') {
        throw checkError;
      }
      
      if (existing) {
        const { error: updateError } = await supabase
          .from('endpoints')
          .update({
            name: route.name,
            method: route.method,
            path: route.path,
            product: route.product,
            group_name: route.group_name,
            description: route.description || null,
            status: route.status,
            updated_at: now
          })
          .eq('id', route.id);
        
        if (updateError) throw updateError;
        updated++;
      } else {
        const { error: insertError } = await supabase
          .from('endpoints')
          .insert({
            id: route.id,
            name: route.name,
            method: route.method,
            path: route.path,
            product: route.product,
            group_name: route.group_name,
            description: route.description || null,
            status: route.status,
            created_at: now,
            updated_at: now
          });
        
        if (insertError) throw insertError;
        inserted++;
      }

      // 2. 문서가 있으면 기본 버전 생성
      if (route.docs && Object.keys(route.docs).length > 0) {
        const versionId = `v_auto_${route.id.replace(/\//g, '_')}`;
        const { data: existingVersion, error: versionCheckError } = await supabase
          .from('versions')
          .select('id')
          .eq('id', versionId)
          .single();
        
        if (versionCheckError && versionCheckError.code !== 'PGRST116') {
          throw versionCheckError;
        }
        
        if (!existingVersion) {
          // 버전 생성
          const { error: versionInsertError } = await supabase
            .from('versions')
            .insert({
              id: versionId,
              version: '1.0',
              endpoint_id: route.id,
              created_at: now,
              updated_at: now,
              author: 'System',
              change_log: 'Auto-generated from route definition'
            });
          
          if (versionInsertError) throw versionInsertError;

          // Manual 데이터 생성
          if (route.docs.schema || route.docs.htmlContent || route.docs.examples) {
            const { error: manualInsertError } = await supabase
              .from('manual_data')
              .insert({
                version_id: versionId,
                title: route.docs.title || route.name,
                category: route.group_name,
                input_uri: route.path,
                active_methods: route.method,
                json_schema: route.docs.schema ? JSON.stringify(route.docs.schema) : '{}',
                json_schema_original: route.docs.schemaOriginal ? JSON.stringify(route.docs.schemaOriginal) : null,
                json_schema_enhanced: route.docs.schemaEnhanced ? JSON.stringify(route.docs.schemaEnhanced) : null,
                examples: route.docs.examples ? JSON.stringify(route.docs.examples) : '[]',
                specifications: route.docs.specifications || route.description || null,
                html_content: route.docs.htmlContent || null
              });
            
            if (manualInsertError) throw manualInsertError;
            withDocs++;
          }
        }
      }
    } catch (error) {
      console.error(`❌ Failed to sync route ${route.id}:`, error.message);
    }
  }

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





