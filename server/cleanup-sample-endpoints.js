/**
 * 샘플 엔드포인트 정리 스크립트
 * 
 * Supabase DB에서 더 이상 사용하지 않는 샘플 엔드포인트들을 삭제합니다.
 * 
 * 실행 방법:
 * node server/cleanup-sample-endpoints.js
 */

import { createClient } from '@supabase/supabase-js';
import dotenv from 'dotenv';

dotenv.config();

const supabaseUrl = process.env.SUPABASE_URL;
const supabaseKey = process.env.SUPABASE_SERVICE_KEY;

if (!supabaseUrl || !supabaseKey) {
  console.error('❌ Supabase credentials not found in .env file');
  process.exit(1);
}

const supabase = createClient(supabaseUrl, supabaseKey, {
  auth: {
    autoRefreshToken: false,
    persistSession: false
  }
});

// 삭제할 샘플 엔드포인트 ID 목록
const SAMPLE_ENDPOINT_IDS = [
  'db/node',
  'db/beam',
  'db/nlct',
  'db/mvct',
  'db/elem',
  'gen/project',
  'gen/material',
  'post/table',
  'analysis/static',
  'analysis/dynamic'
];

async function cleanupSampleEndpoints() {
  console.log('🧹 샘플 엔드포인트 정리를 시작합니다...\n');
  
  let totalDeleted = 0;
  let errors = 0;

  for (const endpointId of SAMPLE_ENDPOINT_IDS) {
    try {
      console.log(`🔍 처리 중: ${endpointId}`);
      
      // 1. 해당 엔드포인트의 버전 조회
      const { data: versions, error: versionsError } = await supabase
        .from('versions')
        .select('id')
        .eq('endpoint_id', endpointId);
      
      if (versionsError) throw versionsError;
      
      if (versions && versions.length > 0) {
        console.log(`   📦 버전 ${versions.length}개 발견`);
        
        // 2. 각 버전의 관련 데이터 삭제
        for (const version of versions) {
          // manual_data 삭제
          const { error: manualError } = await supabase
            .from('manual_data')
            .delete()
            .eq('version_id', version.id);
          
          if (manualError && manualError.code !== 'PGRST116') {
            console.warn(`   ⚠️  manual_data 삭제 실패: ${manualError.message}`);
          }
          
          // spec_data 삭제
          const { error: specError } = await supabase
            .from('spec_data')
            .delete()
            .eq('version_id', version.id);
          
          if (specError && specError.code !== 'PGRST116') {
            console.warn(`   ⚠️  spec_data 삭제 실패: ${specError.message}`);
          }
          
          // builder_data 삭제
          const { error: builderError } = await supabase
            .from('builder_data')
            .delete()
            .eq('version_id', version.id);
          
          if (builderError && builderError.code !== 'PGRST116') {
            console.warn(`   ⚠️  builder_data 삭제 실패: ${builderError.message}`);
          }
          
          // runner_data 삭제
          const { error: runnerError } = await supabase
            .from('runner_data')
            .delete()
            .eq('version_id', version.id);
          
          if (runnerError && runnerError.code !== 'PGRST116') {
            console.warn(`   ⚠️  runner_data 삭제 실패: ${runnerError.message}`);
          }
          
          // test_cases 삭제
          const { error: testError } = await supabase
            .from('test_cases')
            .delete()
            .eq('version_id', version.id);
          
          if (testError && testError.code !== 'PGRST116') {
            console.warn(`   ⚠️  test_cases 삭제 실패: ${testError.message}`);
          }
        }
        
        // 3. 버전 삭제
        const { error: deleteVersionError } = await supabase
          .from('versions')
          .delete()
          .eq('endpoint_id', endpointId);
        
        if (deleteVersionError) {
          console.warn(`   ⚠️  versions 삭제 실패: ${deleteVersionError.message}`);
        } else {
          console.log(`   ✅ 버전 데이터 삭제 완료`);
        }
      }
      
      // 4. 엔드포인트 삭제
      const { error: deleteEndpointError } = await supabase
        .from('endpoints')
        .delete()
        .eq('id', endpointId);
      
      if (deleteEndpointError) {
        console.error(`   ❌ 엔드포인트 삭제 실패: ${deleteEndpointError.message}`);
        errors++;
      } else {
        console.log(`   ✅ 엔드포인트 삭제 완료\n`);
        totalDeleted++;
      }
      
    } catch (error) {
      console.error(`   ❌ 오류 발생: ${error.message}\n`);
      errors++;
    }
  }
  
  console.log('\n' + '='.repeat(60));
  console.log(`✨ 정리 완료!`);
  console.log(`   🗑️  삭제된 엔드포인트: ${totalDeleted}개`);
  console.log(`   ❌ 오류 발생: ${errors}개`);
  console.log('='.repeat(60) + '\n');
  
  if (totalDeleted > 0) {
    console.log('💡 다음 단계:');
    console.log('   1. 서버를 재시작하세요: npm run server');
    console.log('   2. 브라우저에서 엔드포인트 목록을 확인하세요');
    console.log('   3. 더 이상 샘플 엔드포인트가 나타나지 않아야 합니다\n');
  }
}

// 실행
cleanupSampleEndpoints()
  .then(() => {
    console.log('👋 스크립트 종료');
    process.exit(0);
  })
  .catch(error => {
    console.error('💥 치명적 오류:', error);
    process.exit(1);
  });

