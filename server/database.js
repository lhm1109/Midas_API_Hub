import { createClient } from '@supabase/supabase-js';
import dotenv from 'dotenv';

dotenv.config();

// Supabase 클라이언트 초기화
const supabaseUrl = process.env.SUPABASE_URL;
const supabaseKey = process.env.SUPABASE_SERVICE_KEY;

if (!supabaseUrl || !supabaseKey) {
  console.error('❌ Supabase credentials not found in .env file');
  console.error('Please create a .env file with:');
  console.error('  SUPABASE_URL=your-project-url');
  console.error('  SUPABASE_SERVICE_KEY=your-service-role-key');
  process.exit(1);
}

const supabase = createClient(supabaseUrl, supabaseKey, {
  auth: {
    autoRefreshToken: false,
    persistSession: false
  }
});

// Supabase에서는 SQL 스키마를 미리 적용해야 하므로 initDb는 연결 테스트만 수행
export async function initDb() {
  try {
    // 연결 테스트: endpoints 테이블 쿼리
    const { data, error } = await supabase
      .from('endpoints')
      .select('id')
      .limit(1);
    
    if (error && error.code !== 'PGRST116') { // PGRST116 = no rows returned (정상)
      throw error;
    }
    
    console.log('✅ Supabase connection successful');
    console.log(`📡 Connected to: ${supabaseUrl}`);
  } catch (error) {
    console.error('❌ Supabase connection failed:', error.message);
    console.error('Please check your .env configuration');
    process.exit(1);
  }
}

// Supabase 클라이언트를 기본 export
export default supabase;
