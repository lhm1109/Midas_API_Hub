import fs from 'fs';
import path from 'path';
import { fileURLToPath } from 'url';

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);
const ROOT = path.join(__dirname, '..');
const API_DATA = path.join(ROOT, 'api_data_set');

/**
 * HTML에서 title(h1) 추출
 */
function extractTitle(html) {
  const m = html.match(/<h1[^>]*>\s*([^<]+)\s*<\/h1>/);
  return m ? m[1].trim() : null;
}

/**
 * HTML에서 Input URI 추출 → /DB/NODE 형태
 * 패턴: {base url} + db/NODE  or  {base url} + post/TABLE  등
 */
function extractInputURI(html) {
  // Input URI 섹션의 th 텍스트에서 추출
  // 예: {base url} + db/NODE  → /DB/NODE
  // 예: {base url} + ope/ANALSTATUS → /OPE/ANALSTATUS
  const uriPatterns = [
    /\{base url\}\s*\+\s*([\w\/\-\.\{\}]+)/gi,
    /base url.*?\+\s*([\w\/\-\.\{\}]+)/gi,
    /Input URI[\s\S]{0,500}?<th[^>]*>[\s\S]*?\{base url\}\s*\+\s*([\w\/\-\.\{\}]+)/gi,
  ];

  for (const pattern of uriPatterns) {
    const matches = [...html.matchAll(pattern)];
    for (const m of matches) {
      let raw = m[1].replace(/&nbsp;/g, '').replace(/\s+/g, '').trim();
      // db/NODE → /DB/NODE
      if (!raw.startsWith('/')) raw = '/' + raw;
      raw = raw.toUpperCase();
      // 유효한 경로인지 확인
      if (/^\/(DB|OPE|DOC|VIEW|POST|CONFIG|INFO)\//i.test(raw)) {
        return raw;
      }
    }
  }
  return null;
}

/**
 * api_data_set 하위 폴더를 재귀 스캔하여 HTML 파일 수집
 */
function* walkHtmlFiles(dir) {
  for (const entry of fs.readdirSync(dir, { withFileTypes: true })) {
    const full = path.join(dir, entry.name);
    if (entry.isDirectory()) {
      yield* walkHtmlFiles(full);
    } else if (entry.name.endsWith('.html')) {
      yield full;
    }
  }
}

/**
 * 모든 HTML 파일에서 경로 → 설명 맵 생성
 */
function buildDescMap() {
  const map = new Map(); // normalized path (uppercase) → description

  const categories = ['DB', 'OPE', 'DOC', 'VIEW', 'POST'];
  let total = 0;
  let found = 0;

  for (const cat of categories) {
    const catDir = path.join(API_DATA, cat);
    if (!fs.existsSync(catDir)) continue;

    for (const htmlFile of walkHtmlFiles(catDir)) {
      total++;
      const html = fs.readFileSync(htmlFile, 'utf-8');
      const title = extractTitle(html);
      const uri = extractInputURI(html);

      if (title && uri) {
        // base path 저장 (이미 uppercase)
        map.set(uri, title);
        // {id} variant - uppercase 상태의 /{ID} 로 저장
        if (!uri.includes('{')) {
          map.set(uri + '/{ID}', title);
        }
        // /INFO/DB/XXXX 역매핑
        if (uri.startsWith('/DB/') && !uri.includes('{')) {
          const infoPath = '/INFO' + uri;
          map.set(infoPath, title);
        }
        found++;
      }
    }
  }

  // CONFIG 엔드포인트 수동 추가 (HTML 없음)
  map.set('/CONFIG/PROJECT', 'Project Configuration');
  map.set('/CONFIG/VER', 'API Version Information');

  console.log(`🔍 HTML 파일 스캔: ${total}개 처리, ${found}개 경로-설명 매핑 + INFO/DB 역매핑 완료\n`);
  return map;
}

/**
 * CSV 파일 파싱 (쉼표가 포함된 따옴표 필드 처리)
 */
function parseCSVLine(line) {
  const result = [];
  let current = '';
  let inQuotes = false;

  for (let i = 0; i < line.length; i++) {
    const ch = line[i];
    if (ch === '"') {
      if (inQuotes && line[i + 1] === '"') {
        current += '"';
        i++;
      } else {
        inQuotes = !inQuotes;
      }
    } else if (ch === ',' && !inQuotes) {
      result.push(current);
      current = '';
    } else {
      current += ch;
    }
  }
  result.push(current);
  return result;
}

function formatCSVField(val) {
  if (val === undefined || val === null) return '';
  const s = String(val);
  if (s.includes(',') || s.includes('"') || s.includes('\n')) {
    return '"' + s.replace(/"/g, '""') + '"';
  }
  return s;
}

function formatCSVLine(cols) {
  return cols.map(formatCSVField).join(',');
}

/**
 * CSV 파일에 Description 채워넣기
 * @param forceOverwrite  true면 기존 설명도 HTML 설명으로 덮어씀
 */
function enrichCSV(csvPath, descMap, outputPath, forceOverwrite = false) {
  const lines = fs.readFileSync(csvPath, 'utf-8').split('\n').filter(l => l.trim());
  const header = lines[0];
  const result = [header];

  let filled = 0;
  let overwritten = 0;
  let alreadyHad = 0;
  let notFound = 0;

  for (let i = 1; i < lines.length; i++) {
    const cols = parseCSVLine(lines[i]);
    const tag = cols[0] || '';
    const pathUrl = cols[1] || '';
    const mode = cols[2] || '';
    const methods = cols[3] || '';
    let desc = cols[4] || '';

    const normalized = pathUrl.toUpperCase();
    const mapped = descMap.get(normalized);

    if (mapped) {
      if (!desc.trim()) {
        desc = mapped;
        filled++;
      } else if (forceOverwrite) {
        desc = mapped;
        overwritten++;
      } else {
        alreadyHad++;
      }
    } else {
      if (desc.trim()) alreadyHad++;
      else notFound++;
    }

    result.push(formatCSVLine([tag, pathUrl, mode, methods, desc]));
  }

  fs.writeFileSync(outputPath, result.join('\n'), 'utf-8');

  console.log(`   ✅ 새로 채움:    ${filled}개`);
  if (overwritten > 0) console.log(`   🔄 덮어씀:      ${overwritten}개`);
  console.log(`   이미 있음:    ${alreadyHad}개`);
  console.log(`   못찾음:       ${notFound}개`);
  console.log(`   총 레코드:    ${result.length - 1}개\n`);
}

// ─── 실행 ───────────────────────────────────────────────────
console.log('=' .repeat(60));
console.log('  API 엔드포인트 Description 보강 스크립트');
console.log('=' .repeat(60) + '\n');

console.log('📚 api_data_set HTML 파일 스캔 중...\n');
const descMap = buildDescMap();

// 디버그: 매핑 샘플 출력
console.log('📋 매핑 샘플 (처음 10개):');
let count = 0;
for (const [k, v] of descMap) {
  if (count++ >= 10) break;
  console.log(`   ${k.padEnd(30)} → ${v}`);
}
console.log('');

// all-endpoints.csv: HTML 설명으로 덮어씀 (operationId 기반 설명 교체)
console.log('📄 all-endpoints.csv 업데이트 중... (HTML 설명으로 교체)');
enrichCSV(
  path.join(ROOT, 'all-endpoints.csv'),
  descMap,
  path.join(ROOT, 'all-endpoints-new.csv'),
  true  // forceOverwrite
);

// missing-endpoints.csv: 빈 항목만 채움
console.log('📄 missing-endpoints.csv 업데이트 중...');
enrichCSV(
  path.join(ROOT, 'missing-endpoints.csv'),
  descMap,
  path.join(ROOT, 'missing-endpoints-new.csv'),
  false
);

console.log('✅ 완료! (파일명: all-endpoints-new.csv, missing-endpoints-new.csv)');
