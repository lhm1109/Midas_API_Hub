/**
 * Database API Routes
 * PostgreSQL 직접 연결로 SQL/마이그레이션 실행
 */
import express from 'express';
import pg from 'pg';
import fs from 'fs';
import path from 'path';
import { fileURLToPath } from 'url';

const { Pool } = pg;
const router = express.Router();

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

// PostgreSQL 풀 생성 함수
function createPool(connectionString) {
    if (!connectionString) {
        throw new Error('Database connection string is required');
    }
    return new Pool({
        connectionString,
        ssl: { rejectUnauthorized: false } // Supabase는 SSL 필요
    });
}

// Supabase URL에서 Connection String 생성
function getConnectionString(supabaseUrl, supabaseServiceKey) {
    if (!supabaseUrl || !supabaseServiceKey) {
        throw new Error('Supabase URL and Service Key are required');
    }

    // supabaseUrl 형식: https://xxx.supabase.co
    const projectRef = supabaseUrl.replace('https://', '').replace('.supabase.co', '');

    // Supabase 직접 연결 문자열
    // 형식: postgresql://postgres.[project-ref]:[password]@aws-0-[region].pooler.supabase.com:6543/postgres
    // 참고: Service Key는 DB 비밀번호가 아님. 실제로는 DB 비밀번호가 필요함

    // 대안: Transaction Pooler 사용
    const connectionString = `postgresql://postgres.${projectRef}:${supabaseServiceKey}@aws-0-ap-northeast-2.pooler.supabase.com:6543/postgres`;

    return connectionString;
}

/**
 * POST /api/database/query
 * SQL 쿼리 직접 실행
 */
router.post('/query', async (req, res) => {
    let pool = null;
    try {
        const { sql, supabaseUrl, supabaseServiceKey, dbPassword } = req.body;

        if (!sql) {
            return res.status(400).json({ error: 'SQL query is required' });
        }

        if (!dbPassword) {
            return res.status(400).json({
                error: 'Database password is required',
                hint: 'Settings에서 Supabase DB Password를 입력해주세요. (Project Settings > Database > Connection string에서 확인)'
            });
        }

        // Supabase Session Pooler 연결 (IPv4 호환)
        const projectRef = supabaseUrl.replace('https://', '').replace('.supabase.co', '');
        const connectionString = `postgresql://postgres.${projectRef}:${dbPassword}@aws-1-ap-south-1.pooler.supabase.com:5432/postgres`;

        console.log('📝 Executing SQL query:', sql.substring(0, 100) + '...');

        pool = createPool(connectionString);
        const result = await pool.query(sql);

        console.log('✅ SQL executed successfully, rows:', result.rowCount);
        res.json({
            success: true,
            data: result.rows,
            rowCount: result.rowCount,
            message: `Query executed successfully (${result.rowCount} rows affected)`
        });
    } catch (error) {
        console.error('❌ SQL execution error:', error);
        res.status(400).json({
            success: false,
            error: error.message,
            hint: error.code === 'ENOTFOUND'
                ? 'Connection failed. Check your Supabase URL and network connection.'
                : error.code === '28P01'
                    ? 'Authentication failed. Check your database password.'
                    : 'SQL execution failed. Check your query syntax.'
        });
    } finally {
        if (pool) {
            await pool.end();
        }
    }
});

/**
 * GET /api/database/migrations
 * 사용 가능한 마이그레이션 파일 목록 조회
 */
router.get('/migrations', async (req, res) => {
    try {
        const serverDir = path.resolve(__dirname, '..');
        const files = fs.readdirSync(serverDir)
            .filter(file => file.endsWith('.sql') && file.startsWith('schema-'))
            .map(file => ({
                name: file,
                path: path.join(serverDir, file),
                content: fs.readFileSync(path.join(serverDir, file), 'utf-8')
            }));

        console.log(`📂 Found ${files.length} migration files`);
        res.json({ files });
    } catch (error) {
        console.error('❌ Get migrations error:', error);
        res.status(500).json({ error: error.message });
    }
});

/**
 * POST /api/database/migrate
 * 마이그레이션 파일 실행 (PostgreSQL 직접 연결)
 */
router.post('/migrate', async (req, res) => {
    let pool = null;
    try {
        const { fileName, sql, supabaseUrl, dbPassword } = req.body;

        if (!sql) {
            return res.status(400).json({ error: 'SQL content is required' });
        }

        if (!dbPassword) {
            return res.status(400).json({
                error: 'Database password is required',
                hint: 'Settings에서 Supabase DB Password를 입력해주세요.'
            });
        }

        console.log(`🔄 Running migration: ${fileName || 'custom'}`);
        console.log(`📝 SQL preview: ${sql.substring(0, 200)}...`);

        // Supabase Session Pooler 연결 (IPv4 호환)
        const projectRef = supabaseUrl.replace('https://', '').replace('.supabase.co', '');
        const connectionString = `postgresql://postgres.${projectRef}:${dbPassword}@aws-1-ap-south-1.pooler.supabase.com:5432/postgres`;

        pool = createPool(connectionString);
        const result = await pool.query(sql);

        console.log('✅ Migration executed successfully');
        res.json({
            success: true,
            message: `Migration "${fileName}" executed successfully`,
            rowCount: result.rowCount
        });
    } catch (error) {
        console.error('❌ Migration error:', error);
        res.status(400).json({
            success: false,
            error: error.message,
            hint: 'Migration failed. Check your SQL syntax and database connection.'
        });
    } finally {
        if (pool) {
            await pool.end();
        }
    }
});

/**
 * POST /api/database/test-connection
 * PostgreSQL 연결 테스트
 */
router.post('/test-connection', async (req, res) => {
    let pool = null;
    try {
        const { supabaseUrl, dbPassword } = req.body;

        if (!supabaseUrl || !dbPassword) {
            return res.status(400).json({
                success: false,
                error: 'Supabase URL and DB Password are required'
            });
        }

        // Supabase Session Pooler 연결 (IPv4 호환)
        const projectRef = supabaseUrl.replace('https://', '').replace('.supabase.co', '');
        const connectionString = `postgresql://postgres.${projectRef}:${dbPassword}@aws-1-ap-south-1.pooler.supabase.com:5432/postgres`;

        pool = createPool(connectionString);
        const result = await pool.query('SELECT NOW()');

        console.log('✅ PostgreSQL connection successful:', result.rows[0].now);
        res.json({
            success: true,
            message: 'Connection successful',
            serverTime: result.rows[0].now,
            projectRef
        });
    } catch (error) {
        console.error('❌ Connection test error:', error);
        res.status(400).json({
            success: false,
            error: error.message,
            hint: error.code === '28P01'
                ? 'Authentication failed. Check your database password (not Service Key!).'
                : 'Connection failed. Check your Supabase URL.'
        });
    } finally {
        if (pool) {
            await pool.end();
        }
    }
});

/**
 * GET /api/database/tables
 * 테이블 목록 조회
 */
router.get('/tables', async (req, res) => {
    let pool = null;
    try {
        const { supabaseUrl, dbPassword } = req.query;

        if (!supabaseUrl || !dbPassword) {
            // 기본 테이블 목록 반환
            return res.json({
                tables: [
                    { name: 'products', type: 'TABLE' },
                    { name: 'groups', type: 'TABLE' },
                    { name: 'endpoints', type: 'TABLE' },
                    { name: 'versions', type: 'TABLE' },
                    { name: 'locks', type: 'TABLE' },
                ],
                source: 'predefined'
            });
        }

        const projectRef = supabaseUrl.replace('https://', '').replace('.supabase.co', '');
        const connectionString = `postgresql://postgres.${projectRef}:${dbPassword}@aws-1-ap-south-1.pooler.supabase.com:5432/postgres`;

        pool = createPool(connectionString);
        const result = await pool.query(`
      SELECT table_name, table_type 
      FROM information_schema.tables 
      WHERE table_schema = 'public'
      ORDER BY table_name
    `);

        res.json({
            tables: result.rows.map(t => ({ name: t.table_name, type: t.table_type })),
            source: 'database'
        });
    } catch (error) {
        console.error('❌ Get tables error:', error);
        res.status(500).json({ error: error.message });
    } finally {
        if (pool) {
            await pool.end();
        }
    }
});

export default router;
