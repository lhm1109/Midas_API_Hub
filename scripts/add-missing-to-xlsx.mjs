/**
 * missing-endpoints.csv → manager_tasks xlsx 에 추가
 * - /{id} 항목 제외 (중복)
 * - 추가된 행은 옅은 빨간색 배경 (FFFFE0E0)
 */

import fs from 'fs';
import path from 'path';
import { fileURLToPath } from 'url';
import ExcelJS from 'exceljs';

const __dirname = path.dirname(fileURLToPath(import.meta.url));
const ROOT = path.join(__dirname, '..');

const CSV_PATH   = path.join(ROOT, 'missing-endpoints.csv');
const XLSX_PATH  = path.join(ROOT, 'manager_tasks_2026-03-12.xlsx');

// 옅은 빨간색 배경 (#FFE0E0)
const LIGHT_RED = { type: 'pattern', pattern: 'solid', fgColor: { argb: 'FFFFE0E0' } };

// ── CSV 파싱 ──────────────────────────────────────────────
function parseCSVLine(line) {
  const cols = [];
  let inQ = false, cur = '';
  for (let i = 0; i < line.length; i++) {
    const c = line[i];
    if (c === '"') {
      if (inQ && line[i+1] === '"') { cur += '"'; i++; }
      else inQ = !inQ;
    } else if (c === ',' && !inQ) {
      cols.push(cur); cur = '';
    } else {
      cur += c;
    }
  }
  cols.push(cur);
  return cols;
}

// ── CSV 읽기 및 {id} 제외 ─────────────────────────────────
function loadMissingEndpoints() {
  const raw = fs.readFileSync(CSV_PATH, 'utf-8');
  const lines = raw.split(/\r?\n/).filter(l => l.trim());
  // 헤더 제외, /{id} 제외
  const entries = [];
  for (let i = 1; i < lines.length; i++) {
    const cols = parseCSVLine(lines[i]);
    const pathUrl = (cols[1] || '').trim();
    if (pathUrl.endsWith('/{id}') || pathUrl.endsWith('/{ID}')) continue;
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

// ── 경로를 세그먼트로 분리 (/DB/DCTL → ['DB','DCTL','','']) ──
function splitPath(urlPath) {
  // leading slash 제거 후 분리
  const parts = urlPath.replace(/^\//, '').split('/');
  return [
    parts[0] || '',
    parts[1] || '',
    parts[2] || '',
    parts[3] || '',
  ];
}

// ── 메인 ────────────────────────────────────────────────
async function main() {
  console.log('📂 xlsx 파일 읽는 중...');
  const wb = new ExcelJS.Workbook();
  await wb.xlsx.readFile(XLSX_PATH);

  const ws = wb.getWorksheet('Tasks');
  if (!ws) { console.error('Tasks 시트를 찾을 수 없습니다'); process.exit(1); }

  // 헤더 행 위치 찾기 (Order 컬럼이 있는 행)
  let headerRowNum = -1;
  ws.eachRow((row, rowNum) => {
    if (headerRowNum === -1 && row.getCell(1).value === 'Order') {
      headerRowNum = rowNum;
    }
  });
  console.log(`헤더 행: ${headerRowNum}`);

  // 현재 최대 Order 파악
  let maxOrder = 0;
  ws.eachRow((row, rowNum) => {
    if (rowNum <= headerRowNum) return;
    const v = row.getCell(1).value;
    if (typeof v === 'number' && v > maxOrder) maxOrder = v;
  });
  console.log(`현재 최대 Order: ${maxOrder}`);

  // 기존 EndPoint 목록 (중복 방지)
  const existingEndpoints = new Set();
  ws.eachRow((row, rowNum) => {
    if (rowNum <= headerRowNum) return;
    const ep = row.getCell(12).value;
    if (ep) existingEndpoints.add(String(ep).trim().toLowerCase());
  });
  console.log(`기존 EndPoint 수: ${existingEndpoints.size}`);

  // 헤더 컬럼 순서 파악 (A=1 기준)
  // Order(1), Product(2), Tab(3), Group(4), sub1(5), sub2(6), sub3(7),
  // seg1(8), seg2(9), seg3(10), seg4(11), EndPoint(12), mode(13),
  // Plan(14), Dev(15), V&V(16), doc(17), Deploy(18), Issue(19),
  // status(20), charge(21), remark(22), ?(23), 개발비고(24)

  // missing 엔드포인트 로드
  const entries = loadMissingEndpoints();
  console.log(`📋 missing 엔드포인트 (/{id} 제외): ${entries.length}개`);

  // 마지막 데이터 행 번호 찾기
  let lastDataRow = headerRowNum;
  ws.eachRow((row, rowNum) => {
    if (rowNum > headerRowNum && row.getCell(1).value !== null && row.getCell(1).value !== undefined && row.getCell(1).value !== '') {
      lastDataRow = rowNum;
    }
  });
  console.log(`마지막 데이터 행: ${lastDataRow}`);

  let addedCount = 0;
  let skippedCount = 0;
  let currentOrder = maxOrder;
  let currentRowNum = lastDataRow + 1;

  for (const entry of entries) {
    // 중복 체크
    if (existingEndpoints.has(entry.path.toLowerCase())) {
      skippedCount++;
      continue;
    }

    currentOrder++;
    const [seg1, seg2, seg3, seg4] = splitPath(entry.path);

    const row = ws.getRow(currentRowNum);

    // 값 설정
    row.getCell(1).value  = currentOrder;      // Order
    row.getCell(2).value  = '';                 // Product
    row.getCell(3).value  = entry.tag;          // Tab
    row.getCell(4).value  = entry.tag;          // Group
    row.getCell(5).value  = entry.desc || '';   // sub1 (description)
    row.getCell(6).value  = '';                 // sub2
    row.getCell(7).value  = '';                 // sub3
    row.getCell(8).value  = seg1;               // seg1
    row.getCell(9).value  = seg2;               // seg2
    row.getCell(10).value = seg3;               // seg3
    row.getCell(11).value = seg4;               // seg4
    row.getCell(12).value = entry.path;         // End Point
    row.getCell(13).value = entry.mode || '';   // mode
    // Plan~Deploy 비워둠
    row.getCell(22).value = entry.methods;      // remark에 메서드 기록

    // 옅은 빨간색 배경 적용
    for (let col = 1; col <= 24; col++) {
      row.getCell(col).fill = LIGHT_RED;
    }

    row.commit();
    existingEndpoints.add(entry.path.toLowerCase());
    addedCount++;
    currentRowNum++;
  }

  console.log(`\n✅ 추가: ${addedCount}개 / 스킵(기존): ${skippedCount}개`);

  // 저장
  await wb.xlsx.writeFile(XLSX_PATH);
  console.log(`💾 저장 완료: ${XLSX_PATH}`);
}

main().catch(err => { console.error('❌ 오류:', err); process.exit(1); });
