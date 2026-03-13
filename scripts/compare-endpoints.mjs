#!/usr/bin/env node
/**
 * JSON 파일의 엔드포인트와 매니저 DB의 엔드포인트를 비교하여 누락된 항목을 찾습니다.
 */

import fs from 'fs';
import path from 'path';
import { fileURLToPath } from 'url';

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

// JSON 파일 경로
const jsonFilePath = path.join(__dirname, '../api_data_set/openapi 1.json');
const managerApiUrl = 'http://localhost:9527/api/manager/tasks';

async function extractEndpointsFromJson() {
  try {
    const jsonContent = fs.readFileSync(jsonFilePath, 'utf-8');
    const apiData = JSON.parse(jsonContent);
    
    const endpoints = [];
    
    if (apiData.paths) {
      for (const [path, methods] of Object.entries(apiData.paths)) {
        for (const [method, details] of Object.entries(methods)) {
          if (method === 'parameters' || method === 'description') continue;
          
          const tags = details.tags || [];
          const tag = tags[0] || 'UNKNOWN';
          
          endpoints.push({
            path: path,
            method: method.toUpperCase(),
            tag: tag,
            operationId: details.operationId || '',
            description: details.summary || details.description || ''
          });
        }
      }
    }
    
    return endpoints;
  } catch (error) {
    console.error('JSON 파일 읽기 오류:', error);
    return [];
  }
}

async function getManagerEndpoints() {
  try {
    const response = await fetch(managerApiUrl);
    if (!response.ok) {
      throw new Error(`HTTP error! status: ${response.status}`);
    }
    const tasks = await response.json();
    
    // endPoint 필드만 추출 (중복 제거)
    const endpoints = [...new Set(
      tasks
        .map(task => task.endPoint)
        .filter(ep => ep && ep.trim() !== '')
    )];
    
    return {
      tasks,
      endpoints
    };
  } catch (error) {
    console.error('매니저 데이터 가져오기 오류:', error);
    console.log('\n⚠️  서버가 실행 중인지 확인하세요: http://localhost:9527');
    return { tasks: [], endpoints: [] };
  }
}

function compareEndpoints(jsonEndpoints, managerEndpoints) {
  // 대소문자 구분 없이 비교하기 위해 소문자로 변환
  const managerSet = new Set(managerEndpoints.map(ep => ep.toLowerCase()));
  
  // 베이스 경로 세트 생성 (/{id} 제거)
  const managerBaseSet = new Set();
  for (const ep of managerEndpoints) {
    const lower = ep.toLowerCase();
    managerBaseSet.add(lower);
    // {id} 패턴이 있으면 베이스 경로도 추가
    if (lower.includes('{') || lower.includes('/{id}')) {
      const base = lower.replace(/\/\{[^}]+\}.*$/, '');
      managerBaseSet.add(base);
    }
  }
  
  const missing = [];
  
  for (const ep of jsonEndpoints) {
    const fullPath = ep.path.toLowerCase();
    
    // 직접 매치 확인
    if (managerSet.has(fullPath)) {
      continue;
    }
    
    // /{id} 패턴이 있는 경우, 베이스 경로가 매니저에 있는지 확인
    if (fullPath.includes('{')) {
      const basePath = fullPath.replace(/\/\{[^}]+\}.*$/, '');
      if (managerBaseSet.has(basePath)) {
        continue; // 베이스 경로가 있으면 스킵
      }
    }
    
    missing.push(ep);
  }
  
  return missing;
}

function groupByTag(endpoints) {
  const grouped = {};
  for (const ep of endpoints) {
    if (!grouped[ep.tag]) {
      grouped[ep.tag] = [];
    }
    grouped[ep.tag].push(ep);
  }
  return grouped;
}

function generateAddReport(missingEndpoints, managerData) {
  console.log('\n' + '='.repeat(80));
  console.log('📊 API 엔드포인트 비교 분석 결과');
  console.log('='.repeat(80));
  
  console.log(`\n📁 JSON 파일 전체 엔드포인트: ${missingEndpoints.length + managerData.endpoints.length}개`);
  console.log(`✅ 매니저에 등록된 엔드포인트: ${managerData.endpoints.length}개`);
  console.log(`❌ 누락된 엔드포인트: ${missingEndpoints.length}개`);
  
  if (missingEndpoints.length === 0) {
    console.log('\n✨ 모든 엔드포인트가 매니저에 등록되어 있습니다!');
    return;
  }
  
  // 엔드포인트별로 그룹화하여 CRUD 메서드를 컬럼으로 정리
  const endpointMap = new Map();
  
  for (const ep of missingEndpoints) {
    const key = `${ep.tag}|||${ep.path}`;
    if (!endpointMap.has(key)) {
      endpointMap.set(key, {
        tag: ep.tag,
        path: ep.path,
        methods: {},
        description: ep.description
      });
    }
    endpointMap.get(key).methods[ep.method] = true;
  }
  
  // 정렬된 엔드포인트 목록
  const groupedEndpoints = Array.from(endpointMap.values()).sort((a, b) => {
    if (a.tag !== b.tag) return a.tag.localeCompare(b.tag);
    return a.path.localeCompare(b.path);
  });

  console.log('\n' + '-'.repeat(80));
  console.log('📋 태그별 누락된 엔드포인트');
  console.log('-'.repeat(80));
  
  const groupedByTag = {};
  for (const ep of groupedEndpoints) {
    if (!groupedByTag[ep.tag]) {
      groupedByTag[ep.tag] = [];
    }
    groupedByTag[ep.tag].push(ep);
  }
  
  for (const [tag, endpoints] of Object.entries(groupedByTag).sort()) {
    console.log(`\n🏷️  ${tag} (${endpoints.length}개)`);
    console.log('─'.repeat(80));
    
    for (const ep of endpoints) {
      const methods = Object.keys(ep.methods).sort().join(', ');
      console.log(`  ${ep.path.padEnd(40)} [${methods}]`);
      if (ep.description) {
        console.log(`    └─ ${ep.description}`);
      }
    }
  }

  // CSV 형식 출력
  console.log('\n' + '='.repeat(80));
  console.log('📄 CSV 형식 (Excel로 가져오기)');
  console.log('='.repeat(80));
  console.log('Tag,Path,Methods,Description');
  
  for (const ep of groupedEndpoints) {
    const escapedDesc = (ep.description || '').replace(/"/g, '""');
    const methods = Object.keys(ep.methods).sort().join(', ');
    console.log(`${ep.tag},${ep.path},"${methods}","${escapedDesc}"`);
  }
  
  // CSV 파일로 저장
  const csvContent = [
    'Tag,Path,Methods,Description',
    ...groupedEndpoints.map(ep => {
      const escapedDesc = (ep.description || '').replace(/"/g, '""');
      const methods = Object.keys(ep.methods).sort().join(', ');
      return `${ep.tag},${ep.path},"${methods}","${escapedDesc}"`;
    })
  ].join('\n');
  
  const outputPath = path.join(__dirname, '../missing-endpoints.csv');
  fs.writeFileSync(outputPath, csvContent, 'utf-8');
  console.log(`\n💾 결과가 저장되었습니다: ${outputPath}`);
  
  // 매니저에 추가할 데이터 템플릿 생성
  console.log('\n' + '='.repeat(80));
  console.log('📝 매니저 추가용 템플릿 (상위 10개)');
  console.log('='.repeat(80));
  console.log('Product,Tab,Group,sub1,sub2,sub3,seg1,seg2,seg3,seg4,End Point,mode,Methods');
  
  const topTen = groupedEndpoints.slice(0, 10);
  for (const ep of topTen) {
    // 경로를 분석하여 자동으로 seg 값 채우기
    const pathParts = ep.path.split('/').filter(p => p);
    const seg1 = pathParts[0] || '';
    const seg2 = pathParts[1] || '';
    const seg3 = pathParts[2] || '';
    const seg4 = pathParts[3] || '';
    
    const methods = Object.keys(ep.methods).sort().join(', ');
    
    console.log(`Civil,${ep.tag},,,,,${seg1},${seg2},${seg3},${seg4},${ep.path},,"${methods}"`);
  }
  
  if (groupedEndpoints.length > 10) {
    console.log(`... (나머지 ${groupedEndpoints.length - 10}개는 CSV 파일 참조)`);
  }
}

async function main() {
  console.log('🔍 엔드포인트 비교 분석 시작...\n');
  
  // 1. JSON에서 엔드포인트 추출
  console.log('📖 JSON 파일에서 엔드포인트 추출 중...');
  const jsonEndpoints = await extractEndpointsFromJson();
  console.log(`   ✓ ${jsonEndpoints.length}개 엔드포인트 발견`);
  
  // 2. 매니저에서 엔드포인트 가져오기
  console.log('\n🔌 매니저 데이터 조회 중...');
  const managerData = await getManagerEndpoints();
  console.log(`   ✓ ${managerData.endpoints.length}개 엔드포인트 등록됨`);
  
  // 3. 비교
  console.log('\n⚖️  비교 분석 중...');
  const missingEndpoints = compareEndpoints(jsonEndpoints, managerData.endpoints);
  
  // 4. 리포트 생성
  generateAddReport(missingEndpoints, managerData);
  
  console.log('\n✅ 분석 완료!\n');
}

main().catch(console.error);
