import db from './database.js';

console.log('🧹 Cleaning up HTML-formatted versions...\n');

// 모든 버전의 manual_data 확인
const versions = db.prepare('SELECT version_id FROM manual_data').all();

let cleanedCount = 0;

versions.forEach(({ version_id }) => {
  const data = db.prepare('SELECT json_schema FROM manual_data WHERE version_id = ?').get(version_id);
  
  if (data && data.json_schema && data.json_schema.includes('<br>')) {
    console.log(`🗑️  Deleting version ${version_id} (contains HTML)`);
    
    db.prepare('DELETE FROM manual_data WHERE version_id = ?').run(version_id);
    db.prepare('DELETE FROM spec_data WHERE version_id = ?').run(version_id);
    db.prepare('DELETE FROM builder_data WHERE version_id = ?').run(version_id);
    db.prepare('DELETE FROM runner_data WHERE version_id = ?').run(version_id);
    db.prepare('DELETE FROM test_cases WHERE version_id = ?').run(version_id);
    db.prepare('DELETE FROM versions WHERE id = ?').run(version_id);
    
    cleanedCount++;
  }
});

console.log(`\n✅ Cleaned ${cleanedCount} versions with HTML formatting`);




