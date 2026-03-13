import fs from 'fs';
import path from 'path';
import { fileURLToPath } from 'url';
import { dirname } from 'path';

const __filename = fileURLToPath(import.meta.url);
const __dirname = dirname(__filename);

/**
 * OpenAPI JSON에서 모든 엔드포인트 추출
 */
function extractAllEndpoints(jsonPath) {
  console.log('📂 OpenAPI JSON 파일 읽는 중...\n');
  console.log(`   경로: ${jsonPath}\n`);
  
  if (!fs.existsSync(jsonPath)) {
    console.error('❌ 파일을 찾을 수 없습니다:', jsonPath);
    process.exit(1);
  }
  
  const content = fs.readFileSync(jsonPath, 'utf-8');
  console.log(`✅ 파일 읽기 완료 (${(content.length / 1024 / 1024).toFixed(2)} MB)\n`);
  
  console.log('⏳ JSON 파싱 중...');
  const openapi = JSON.parse(content);
  console.log('✅ JSON 파싱 완료\n');
  
  const endpoints = [];
  const paths = openapi.paths || {};
  
  console.log(`🔍 총 ${Object.keys(paths).length}개의 경로 발견\n`);
  
  for (const [pathUrl, pathItem] of Object.entries(paths)) {
    const methods = [];
    const tags = new Set();
    let description = '';
    let operationId = '';
    
    // 각 HTTP 메서드 확인
    for (const method of ['get', 'post', 'put', 'delete', 'patch', 'options', 'head']) {
      if (pathItem[method]) {
        methods.push(method.toUpperCase());
        
        const operation = pathItem[method];
        
        // tags 수집 (첫 번째 tag 사용)
        if (operation.tags && operation.tags.length > 0) {
          tags.add(operation.tags[0]);
        }
        
        // description 수집 (우선순위: summary > description > operationId)
        if (!description) {
          if (operation.summary) {
            description = operation.summary;
          } else if (operation.description) {
            description = operation.description;
          } else if (operation.operationId) {
            operationId = operation.operationId;
          }
        }
      }
    }
    
    if (methods.length > 0) {
      // tag는 첫 번째 것만 사용
      const tag = Array.from(tags)[0] || '';
      
      // description이 없으면 operationId를 변환
      if (!description && operationId) {
        // get_requestinfo_post_table_typelist -> Get Requestinfo Post Table Typelist
        description = operationId
          .split('_')
          .map(word => word.charAt(0).toUpperCase() + word.slice(1))
          .join(' ');
      }
      
      endpoints.push({
        tag: tag,
        path: pathUrl,
        methods: methods,
        description: description || ''
      });
    }
  }
  
  console.log(`✅ 총 ${endpoints.length}개의 엔드포인트 추출 완료\n`);
  return endpoints;
}

/**
 * CSV 파일 생성
 */
function generateCSV() {
  const jsonPath = path.join(__dirname, '..', 'api_data_set', 'openapi 1.json');
  const outputPath = path.join(__dirname, '..', 'all-endpoints.csv');
  
  console.log('📊 OpenAPI 엔드포인트 전체 목록 추출\n');
  console.log('=' .repeat(60) + '\n');
  
  const endpoints = extractAllEndpoints(jsonPath);
  
  // CSV 생성
  const csvLines = ['Tag,Path,Mode,Methods,Description'];
  
  for (const endpoint of endpoints) {
    const methodsStr = endpoint.methods.join(', ');
    const desc = endpoint.description.replace(/"/g, '""');
    // Methods에 쉼표가 있으므로 반드시 따옴표로 감쌈
    csvLines.push(`${endpoint.tag},${endpoint.path},,"${methodsStr}","${desc}"`);
  }
  
  fs.writeFileSync(outputPath, csvLines.join('\n'), 'utf-8');
  
  console.log('📄 생성된 파일: all-endpoints.csv');
  console.log(`   총 ${endpoints.length}개 레코드\n`);
  
  // 통계
  const tagStats = {};
  for (const endpoint of endpoints) {
    tagStats[endpoint.tag] = (tagStats[endpoint.tag] || 0) + 1;
  }
  
  console.log('📈 태그별 통계:\n');
  const sortedTags = Object.entries(tagStats)
    .sort((a, b) => b[1] - a[1])
    .slice(0, 10);
  
  for (const [tag, count] of sortedTags) {
    console.log(`   ${tag.padEnd(25)} : ${count}개`);
  }
  
  // 샘플 출력
  console.log('\n📋 샘플 데이터 (처음 10개):\n');
  console.log('Tag | Path | Methods | Description');
  console.log('-'.repeat(80));
  
  endpoints.slice(0, 10).forEach(ep => {
    const desc = ep.description.substring(0, 40);
    console.log(`${ep.tag.padEnd(15)} | ${ep.path.substring(0, 25).padEnd(25)} | ${ep.methods.join(',').padEnd(8)} | ${desc}`);
  });
  
  if (endpoints.length > 10) {
    console.log(`\n... 그 외 ${endpoints.length - 10}개 항목\n`);
  }
}

// 실행
generateCSV();
