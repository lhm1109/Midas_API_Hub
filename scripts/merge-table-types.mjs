import fs from 'fs';
import path from 'path';
import { fileURLToPath } from 'url';
import { dirname } from 'path';

const __filename = fileURLToPath(import.meta.url);
const __dirname = dirname(__filename);

/**
 * missing-endpoints.csv에 TABLE_TYPE 정보 추가
 */
function mergeTableTypes() {
  try {
    const tableTypesCsvPath = path.join(__dirname, '..', 'table-types.csv');
    const missingCsvPath = path.join(__dirname, '..', 'missing-endpoints.csv');
    const outputCsvPath = path.join(__dirname, '..', 'missing-endpoints-with-modes.csv');

    console.log('📂 파일 경로:');
    console.log(`   table-types: ${tableTypesCsvPath}`);
    console.log(`   missing: ${missingCsvPath}`);
    console.log(`   output: ${outputCsvPath}\n`);

    // table-types.csv 읽기
    const tableTypesContent = fs.readFileSync(tableTypesCsvPath, 'utf-8');
    const tableTypesLines = tableTypesContent.split('\n').filter(line => line.trim());
    
    // missing-endpoints.csv 읽기
    const missingContent = fs.readFileSync(missingCsvPath, 'utf-8');
    const missingLines = missingContent.split('\n').filter(line => line.trim());

    console.log(`📊 파일 정보:`);
    console.log(`   table-types.csv: ${tableTypesLines.length - 1}개 레코드`);
    console.log(`   missing-endpoints.csv: ${missingLines.length - 1}개 레코드\n`);

    // 새 헤더 (Mode 컬럼 추가)
    const newHeader = 'Tag,Path,Mode,Methods,Description';
    const outputLines = [newHeader];

    // 기존 missing-endpoints 변환 (Mode 컬럼 빈 값으로)
    console.log('🔄 기존 missing-endpoints 변환 중...');
    for (let i = 1; i < missingLines.length; i++) {
      const line = missingLines[i];
      const parts = line.split(',');
      
      if (parts.length >= 3) {
        const tag = parts[0];
        const path = parts[1];
        const methods = parts[2];
        const description = parts.slice(3).join(','); // description에 쉼표가 있을 수 있음
        
        outputLines.push(`${tag},${path},,${methods},${description}`);
      }
    }
    console.log(`   ✅ ${missingLines.length - 1}개 변환 완료\n`);

    // table-types 추가 (헤더 제외)
    console.log('➕ TABLE_TYPE 정보 추가 중...');
    for (let i = 1; i < tableTypesLines.length; i++) {
      outputLines.push(tableTypesLines[i]);
    }
    console.log(`   ✅ ${tableTypesLines.length - 1}개 추가 완료\n`);

    // 파일 저장
    fs.writeFileSync(outputCsvPath, outputLines.join('\n'), 'utf-8');
    
    console.log(`✅ 통합 완료!`);
    console.log(`   출력 파일: missing-endpoints-with-modes.csv`);
    console.log(`   총 레코드: ${outputLines.length - 1}개\n`);
    
    // 통계
    const modeCount = outputLines.filter(line => {
      const parts = line.split(',');
      return parts.length >= 3 && parts[2].trim() !== '' && parts[2] !== 'Mode';
    }).length;
    
    const noModeCount = outputLines.length - 1 - modeCount;
    
    console.log(`📈 통계:`);
    console.log(`   Mode 있음: ${modeCount}개 (POST/TABLE TABLE_TYPES)`);
    console.log(`   Mode 없음: ${noModeCount}개 (기존 누락 엔드포인트)\n`);
    
    // 샘플 출력
    console.log(`📋 샘플 (POST/TABLE TABLE_TYPES 처음 5개):\n`);
    let sampleCount = 0;
    for (let i = 1; i < outputLines.length && sampleCount < 5; i++) {
      const line = outputLines[i];
      const parts = line.split(',');
      if (parts[2] && parts[2].trim() !== '') {
        console.log(`   ${line}`);
        sampleCount++;
      }
    }
  } catch (error) {
    console.error('❌ 오류 발생:', error.message);
    console.error(error.stack);
  }
}

// 실행
mergeTableTypes();
