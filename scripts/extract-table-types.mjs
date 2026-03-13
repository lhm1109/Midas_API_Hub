import fs from 'fs';
import path from 'path';
import { fileURLToPath } from 'url';
import { dirname } from 'path';

const __filename = fileURLToPath(import.meta.url);
const __dirname = dirname(__filename);

const POST_DIR = path.join(__dirname, '..', 'api_data_set', 'POST');

/**
 * HTML 파일에서 TABLE_TYPE 추출
 */
function extractTableType(htmlContent) {
  // "TABLE_TYPE": "ELEMENTWEIGHT" 패턴 (HTML 엔티티 포함)
  const patterns = [
    /"TABLE_TYPE"[^"]*"([A-Z_0-9]+)"/g,
    /TABLE_TYPE[^"]*"([A-Z_0-9]+)"/g,
    /Result Table Type[^"]*"([A-Z_0-9]+)"/g
  ];

  for (const pattern of patterns) {
    const matches = [...htmlContent.matchAll(pattern)];
    for (const match of matches) {
      if (match[1] && match[1] !== 'string' && match[1] !== 'String') {
        return match[1];
      }
    }
  }
  return null;
}

/**
 * HTML 파일에서 타이틀 추출
 */
function extractTitle(htmlContent) {
  const titleMatch = htmlContent.match(/<title>([^<]+)<\/title>/);
  if (titleMatch) {
    return titleMatch[1].replace(' - POST', '').trim();
  }
  
  const h1Match = htmlContent.match(/<h1>([^<]+)<\/h1>/);
  if (h1Match) {
    return h1Match[1].trim();
  }
  
  return null;
}

/**
 * POST 폴더 스캔하여 TABLE_TYPE 정보 추출
 */
function extractAllTableTypes() {
  const results = [];
  const folders = fs.readdirSync(POST_DIR);

  console.log(`📂 총 ${folders.length}개의 폴더 발견`);

  for (const folder of folders) {
    const folderPath = path.join(POST_DIR, folder);
    
    let stat;
    try {
      stat = fs.statSync(folderPath);
    } catch (e) {
      continue;
    }
    
    if (!stat.isDirectory()) continue;

    // zendesk 폴더 내 HTML 파일 찾기
    const zendeskPath = path.join(folderPath, 'zendesk');
    if (!fs.existsSync(zendeskPath)) {
      console.log(`⚠️  ${folder}: zendesk 폴더 없음`);
      continue;
    }

    const files = fs.readdirSync(zendeskPath);
    const htmlFile = files.find(f => f.endsWith('.html'));
    
    if (!htmlFile) {
      console.log(`⚠️  ${folder}: HTML 파일 없음`);
      continue;
    }

    const htmlPath = path.join(zendeskPath, htmlFile);
    const content = fs.readFileSync(htmlPath, 'utf-8');
    
    const tableType = extractTableType(content);
    const title = extractTitle(content);
    
    if (tableType && title) {
      console.log(`✅ ${folder}: ${tableType}`);
      results.push({
        folder: folder,
        tableType: tableType,
        title: title,
        description: title
      });
    } else {
      console.log(`❌ ${folder}: TABLE_TYPE 또는 Title 추출 실패`);
    }
  }

  console.log(`\n✅ 총 ${results.length}개의 TABLE_TYPE 추출 완료\n`);
  return results;
}

/**
 * Manager에 이미 등록된 엔드포인트 가져오기
 */
async function getManagerEndpoints() {
  try {
    const response = await fetch('http://localhost:9527/api/manager/tasks');
    if (!response.ok) {
      console.log('⚠️  Manager 서버가 실행되지 않았을 수 있습니다.');
      return [];
    }
    const data = await response.json();
    return data.map(task => ({
      endpoint: task.endPoint?.toLowerCase() || '',
      mode: task.mode?.toLowerCase() || ''
    }));
  } catch (error) {
    console.log('⚠️  Manager 서버 연결 실패:', error.message);
    return [];
  }
}

/**
 * CSV 파일 생성
 */
async function generateCSV() {
  console.log('📂 POST 폴더 스캔 중...\n');
  
  const tableTypes = extractAllTableTypes();
  console.log(`✅ 총 ${tableTypes.length}개의 TABLE_TYPE 발견\n`);

  console.log('🔍 Manager 데이터베이스 확인 중...\n');
  const managerEndpoints = await getManagerEndpoints();
  
  // POST/TABLE이 이미 등록되어 있는지 확인
  const postTableEntry = managerEndpoints.find(e => 
    e.endpoint.includes('/post/table')
  );

  if (postTableEntry) {
    console.log('✅ POST/TABLE이 Manager에 등록되어 있습니다.');
    console.log(`   현재 Mode: ${postTableEntry.mode || '(비어있음)'}\n`);
  } else {
    console.log('❌ POST/TABLE이 Manager에 등록되지 않았습니다.\n');
  }

  // CSV 라인 생성
  const csvLines = ['Tag,Path,Mode,Methods,Description'];
  
  for (const item of tableTypes) {
    const line = [
      'POST',
      '/POST/TABLE',
      item.tableType,
      'POST',
      `"${item.description}"`
    ].join(',');
    csvLines.push(line);
  }

  // 기존 missing-endpoints.csv 읽기
  const missingCsvPath = path.join(__dirname, '..', 'missing-endpoints.csv');
  let existingLines = [];
  
  if (fs.existsSync(missingCsvPath)) {
    const existingContent = fs.readFileSync(missingCsvPath, 'utf-8');
    existingLines = existingContent.split('\n').filter(line => line.trim());
  }

  // 새 CSV 파일 생성 (table-types.csv)
  const tableTypesCsvPath = path.join(__dirname, '..', 'table-types.csv');
  fs.writeFileSync(tableTypesCsvPath, csvLines.join('\n'), 'utf-8');
  
  console.log(`\n📄 생성된 파일: table-types.csv`);
  console.log(`   총 ${tableTypes.length}개의 TABLE_TYPE 레코드\n`);

  // 샘플 출력
  console.log('📋 샘플 데이터 (처음 10개):\n');
  console.log('Tag | Path | Mode | Methods | Description');
  console.log('-'.repeat(80));
  
  tableTypes.slice(0, 10).forEach(item => {
    console.log(`POST | /POST/TABLE | ${item.tableType} | POST | ${item.description}`);
  });

  if (tableTypes.length > 10) {
    console.log(`... 그 외 ${tableTypes.length - 10}개 항목\n`);
  }

  return tableTypes;
}

// 실행
generateCSV().catch(console.error);
