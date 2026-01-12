import db from './database.js';

const versionId = 'v_1768196909962_k32c55w7t';

console.log(`🔍 Checking version: ${versionId}\n`);

const manualData = db.prepare('SELECT * FROM manual_data WHERE version_id = ?').get(versionId);

if (manualData) {
  console.log('📋 Manual Data Fields:');
  console.log('- json_schema type:', typeof manualData.json_schema);
  console.log('- json_schema length:', manualData.json_schema?.length || 0);
  console.log('- json_schema first 200 chars:', String(manualData.json_schema).substring(0, 200));
  console.log('\n- json_schema_original type:', typeof manualData.json_schema_original);
  console.log('- json_schema_original length:', manualData.json_schema_original?.length || 0);
  console.log('- json_schema_original first 200 chars:', String(manualData.json_schema_original).substring(0, 200));
  console.log('\n- json_schema_enhanced type:', typeof manualData.json_schema_enhanced);
  console.log('- json_schema_enhanced:', manualData.json_schema_enhanced);
  
  console.log('\n- specifications type:', typeof manualData.specifications);
  console.log('- specifications length:', manualData.specifications?.length || 0);
  console.log('- specifications first 200 chars:', String(manualData.specifications).substring(0, 200));
  
  // HTML 태그 체크
  const hasHTMLInSchema = manualData.json_schema?.includes('<br>') || manualData.json_schema?.includes('<span>');
  const hasHTMLInOriginal = manualData.json_schema_original?.includes('<br>') || manualData.json_schema_original?.includes('<span>');
  const hasHTMLInSpecs = manualData.specifications?.includes('<table>') || manualData.specifications?.includes('<br>');
  
  console.log('\n🔍 HTML Detection:');
  console.log('- json_schema has HTML:', hasHTMLInSchema);
  console.log('- json_schema_original has HTML:', hasHTMLInOriginal);
  console.log('- specifications has HTML:', hasHTMLInSpecs);
  
  // JSON 파싱 테스트
  console.log('\n🧪 JSON Parse Test:');
  try {
    JSON.parse(manualData.json_schema);
    console.log('✅ json_schema is valid JSON');
  } catch (e) {
    console.log('❌ json_schema is NOT valid JSON:', e.message);
  }
  
  try {
    if (manualData.json_schema_original) {
      JSON.parse(manualData.json_schema_original);
      console.log('✅ json_schema_original is valid JSON');
    }
  } catch (e) {
    console.log('❌ json_schema_original is NOT valid JSON:', e.message);
  }
} else {
  console.log('❌ No manual_data found for this version');
}


