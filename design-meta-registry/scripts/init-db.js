// DB 초기화 스크립트
import Database from 'better-sqlite3';
import fs from 'fs';
import path from 'path';
import { fileURLToPath } from 'url';

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

const dbPath = path.join(__dirname, '../db/registry.db');
const schemaPath = path.join(__dirname, '../db/schema.sql');
const seedPath = path.join(__dirname, '../db/seed.sql');

// 기존 DB 삭제
if (fs.existsSync(dbPath)) {
    fs.unlinkSync(dbPath);
    console.log('🗑️  Removed existing database');
}

// DB 생성
const db = new Database(dbPath);
console.log('📁 Created database:', dbPath);

// 스키마 실행
const schema = fs.readFileSync(schemaPath, 'utf-8');
db.exec(schema);
console.log('✅ Schema created');

// 시드 데이터 실행
const seed = fs.readFileSync(seedPath, 'utf-8');
db.exec(seed);
console.log('✅ Seed data inserted');

// 결과 확인
const tables = db.prepare("SELECT name FROM sqlite_master WHERE type='table'").all();
console.log('\n📊 Tables created:');
tables.forEach(t => {
    const count = db.prepare(`SELECT COUNT(*) as cnt FROM ${t.name}`).get();
    console.log(`   - ${t.name}: ${count.cnt} rows`);
});

db.close();
console.log('\n🎉 Database initialization complete!');
