/**
 * missing-endpoints → manager_tasks xlsx 적합한 위치에 행 삽입
 * 1. 이전 스크립트가 하단에 추가한 rows 제거 (Order > 900)
 * 2. 각 엔드포인트를 알파벳 순 기준으로 같은 seg1 그룹 내 적절한 위치에 삽입
 * 3. 삽입된 행: 옅은 빨간색 배경 (#FFE0E0)
 */

import fs from 'fs';
import path from 'path';
import { fileURLToPath } from 'url';
import ExcelJS from 'exceljs';

const __dirname = path.dirname(fileURLToPath(import.meta.url));
const ROOT = path.join(__dirname, '..');
const CSV_PATH  = path.join(ROOT, 'missing-endpoints.csv');
const XLSX_PATH = path.join(ROOT, 'manager_tasks_2026-03-12.xlsx');

const LIGHT_RED = { type: 'pattern', pattern: 'solid', fgColor: { argb: 'FFFFE0E0' } };

// ── CSV 파싱 ─────────────────────────────────────────────────
function parseCSVLine(line) {
  const cols = [];
  let inQ = false, cur = '';
  for (let i = 0; i < line.length; i++) {
    const c = line[i];
    if (c === '"') {
      if (inQ && line[i + 1] === '"') { cur += '"'; i++; }
      else inQ = !inQ;
    } else if (c === ',' && !inQ) {
      cols.push(cur); cur = '';
    } else { cur += c; }
  }
  cols.push(cur);
  return cols;
}

function loadMissingEndpoints() {
  const raw = fs.readFileSync(CSV_PATH, 'utf-8');
  const lines = raw.split(/\r?\n/).filter(l => l.trim());
  const entries = [];
  for (let i = 1; i < lines.length; i++) {
    const cols = parseCSVLine(lines[i]);
    const pathUrl = (cols[1] || '').trim();
    if (!pathUrl) continue;
    // /{id} 제외
    if (pathUrl.toLowerCase().endsWith('/{id}')) continue;
    entries.push({
      tag:     (cols[0] || '').trim(),
      path:    pathUrl,
      mode:    (cols[2] || '').trim(),
      methods: (cols[3] || '').trim(),
      desc:    (cols[4] || '').trim(),
    });
  }
  return entries;
}

function splitPath(urlPath) {
  const parts = urlPath.replace(/^\//, '').split('/');
  return [parts[0]||'', parts[1]||'', parts[2]||'', parts[3]||''];
}

// ── 삽입 위치 결정 ────────────────────────────────────────────
// existingEntries: [{path, rowNum, tag}] (rowNum은 현재 실제 sheet row)
function findInsertAfterRow(missingPath, missingTag, existingEntries) {
  const seg1 = missingPath.split('/')[1] || '';
  const missingUpper = missingPath.toUpperCase();

  // 1차: 같은 seg1 그룹 (대소문자 무시)
  let group = existingEntries.filter(e =>
    (e.path.split('/')[1] || '').toUpperCase() === seg1.toUpperCase()
  );

  // seg1 그룹이 없으면 2차: 같은 Tag
  if (group.length === 0) {
    group = existingEntries.filter(e => e.tag === missingTag);
  }

  if (group.length === 0) return -1; // 찾지 못함 → 맨 끝에 추가

  // 그룹 내 마지막 row (알파벳 기준 포지션 찾기)
  // 그룹을 rowNum 순으로 정렬
  group.sort((a, b) => a.rowNum - b.rowNum);

  // path <= missingPath 인 마지막 항목 → insert after
  let bestRowNum = group[0].rowNum - 1; // 그룹 시작 전 (최악의 경우 그룹 앞)
  for (const e of group) {
    if (e.path.toUpperCase() <= missingUpper) {
      bestRowNum = e.rowNum;
    }
  }
  return bestRowNum;
}

// ── 메인 ─────────────────────────────────────────────────────
async function main() {
  console.log('📂 xlsx 읽는 중...');
  const wb = new ExcelJS.Workbook();
  await wb.xlsx.readFile(XLSX_PATH);
  const ws = wb.getWorksheet('Tasks');
  if (!ws) { console.error('Tasks 시트 없음'); process.exit(1); }

  // ─ Step 1: 이전에 추가된 하단 행 제거 (Order > 900) ─
  const rowsToDelete = [];
  ws.eachRow((row, rowNum) => {
    if (rowNum <= 2) return;
    const v = row.getCell(1).value;
    if (typeof v === 'number' && v > 900) rowsToDelete.push(rowNum);
    // Order가 없더라도 light-red fill인 행 (이전 추가분 일부)
    else {
      const fill = row.getCell(1).fill;
      if (fill && fill.fgColor && fill.fgColor.argb === 'FFFFE0E0') {
        rowsToDelete.push(rowNum);
      }
    }
  });
  rowsToDelete.sort((a, b) => b - a);
  for (const r of rowsToDelete) ws.spliceRows(r, 1);
  console.log(`🗑️  이전 추가 행 ${rowsToDelete.length}개 제거`);

  // ─ Step 2: 현재 엔드포인트 맵 구성 ─
  const existingEntries = [];
  ws.eachRow((row, rowNum) => {
    if (rowNum <= 2) return;
    const ep = row.getCell(12).value;
    const tag = row.getCell(3).value;
    if (ep && String(ep).trim()) {
      existingEntries.push({
        path:   String(ep).trim(),
        rowNum,
        tag:    tag ? String(tag).trim() : '',
      });
    }
  });
  console.log(`기존 EndPoint 행: ${existingEntries.length}개`);

  // ─ Step 3: missing 엔드포인트 로드 ─
  const missing = loadMissingEndpoints();
  const existingSet = new Set(existingEntries.map(e => e.path.toLowerCase()));

  // ─ Step 4: 삽입 계획 수립 ─
  // insertPlan: Map<afterRowNum, entry[]>
  const insertPlan = new Map();
  let skipCount = 0;

  for (const entry of missing) {
    if (existingSet.has(entry.path.toLowerCase())) { skipCount++; continue; }

    const afterRow = findInsertAfterRow(entry.path, entry.tag, existingEntries);
    const key = afterRow === -1 ? 99999 : afterRow; // -1이면 맨 끝
    if (!insertPlan.has(key)) insertPlan.set(key, []);
    insertPlan.get(key).push(entry);
  }

  console.log(`스킵(기존): ${skipCount}개 / 삽입 예정: ${[...insertPlan.values()].flat().length}개`);

  // ─ Step 5: 하단→상단 순으로 삽입 ─
  const sortedPositions = [...insertPlan.keys()].sort((a, b) => b - a);
  let totalAdded = 0;

  for (const afterRowNum of sortedPositions) {
    const entries = insertPlan.get(afterRowNum);
    // 각 position에서 알파벳 순 정렬
    entries.sort((a, b) => a.path.localeCompare(b.path));

    // spliceRows로 한 번에 삽입
    let lastRowNum = 2;
    ws.eachRow((r, n) => { lastRowNum = Math.max(lastRowNum, n); });
    const insertAt = afterRowNum === 99999
      ? lastRowNum + 1
      : afterRowNum + 1;

    const rowArrays = entries.map(entry => {
      const [seg1, seg2, seg3, seg4] = splitPath(entry.path);
      const arr = Array(28).fill(null);
      arr[0]  = null;          // Order (비워둠 - 나중에 수동 채움)
      arr[1]  = null;          // Product
      arr[2]  = entry.tag;     // Tab
      arr[3]  = entry.tag;     // Group
      arr[4]  = entry.desc;    // sub1 (description)
      arr[5]  = null;
      arr[6]  = null;
      arr[7]  = seg1;          // seg1
      arr[8]  = seg2;          // seg2
      arr[9]  = seg3;          // seg3
      arr[10] = seg4;          // seg4
      arr[11] = entry.path;    // EndPoint
      arr[12] = entry.mode;    // mode
      arr[21] = entry.methods; // remark (col 22, idx 21)
      return arr;
    });

    ws.spliceRows(insertAt, 0, ...rowArrays);

    // 삽입된 행에 빨간색 배경 적용
    for (let i = 0; i < entries.length; i++) {
      const row = ws.getRow(insertAt + i);
      for (let col = 1; col <= 24; col++) {
        const cell = row.getCell(col);
        cell.fill = LIGHT_RED;
      }
      row.commit();
    }

    totalAdded += entries.length;
  }

  console.log(`\n✅ 총 ${totalAdded}개 행 삽입 완료`);

  // ─ 저장 ─
  await wb.xlsx.writeFile(XLSX_PATH);
  console.log(`💾 저장: ${XLSX_PATH}`);

  // ─ 검증: MVLD 근처 확인 ─
  console.log('\n📋 검증 - MVLD 근처 행:');
  const wb2 = new ExcelJS.Workbook();
  await wb2.xlsx.readFile(XLSX_PATH);
  const ws2 = wb2.getWorksheet('Tasks');
  ws2.eachRow((row, rowNum) => {
    const ep = row.getCell(12).value;
    if (ep && String(ep).toUpperCase().includes('MVLD')) {
      const fill = row.getCell(1).fill;
      const isNew = fill && fill.fgColor && fill.fgColor.argb === 'FFFFE0E0' ? '[NEW]' : '';
      console.log(`  Row${rowNum} EP:${ep} ${isNew}`);
    }
  });
}

main().catch(err => { console.error('❌', err); process.exit(1); });
