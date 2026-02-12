# 🚀 배포 가이드 (Deployment Guide)

## 📋 배포 아키텍처

```
┌─────────────────────────────────────────────────────┐
│                   사용자 (Users)                     │
└──────────────────┬──────────────────────────────────┘
                   │
        ┌──────────┴──────────┐
        │                     │
        ▼                     ▼
┌───────────────┐     ┌───────────────┐
│   Frontend    │     │   Backend     │
│   (Vercel)    │────▶│  (Railway)    │
│   React/Vite  │     │  Node.js/API  │
└───────────────┘     └───────┬───────┘
                              │
                              ▼
                      ┌───────────────┐
                      │   Supabase    │
                      │  PostgreSQL   │
                      │   + Storage   │
                      └───────────────┘
```

---

## 🎯 배포 체크리스트

- [ ] Supabase 프로젝트 생성
- [ ] 데이터베이스 스키마 적용
- [ ] 데이터 마이그레이션
- [ ] 백엔드 배포 (Railway)
- [ ] 프론트엔드 배포 (Vercel)
- [ ] 환경 변수 설정
- [ ] 도메인 연결 (선택)

---

## 1️⃣ Supabase 설정

### 1.1 프로젝트 생성
1. https://supabase.com 접속 → 로그인
2. **New Project** 클릭
3. 설정:
   - Name: `midas-api-verification`
   - Database Password: 강력한 비밀번호 생성
   - Region: **Northeast Asia (Seoul)**
4. **Create new project** (약 2분 소요)

### 1.2 데이터베이스 스키마 적용
1. Supabase Dashboard → **SQL Editor**
2. **New query** 클릭
3. `server/schema-with-locks.sql` 내용 복사 & 붙여넣기
4. **Run** 클릭

### 1.3 API Keys 복사
1. Project Settings → **API**
2. 복사:
   - **Project URL**: `https://xxxxx.supabase.co`
   - **anon public** key: `eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9...`
   - **service_role** key (비밀): `eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9...`

### 1.4 Storage Bucket 생성
1. **Storage** → **New bucket**
2. Name: `attachments`
3. **Public bucket** 체크 해제 (비공개)
4. **Create bucket**

---

## 2️⃣ 데이터 마이그레이션

### 2.1 Supabase 클라이언트 설치
```bash
npm install @supabase/supabase-js
```

### 2.2 환경 변수 설정
`.env` 파일 생성:
```env
SUPABASE_URL=https://xxxxx.supabase.co
SUPABASE_SERVICE_ROLE_KEY=eyJhbGciOiJIUzI1...
```

### 2.3 마이그레이션 실행
```bash
node server/migrate-to-supabase.js
```

---

## 3️⃣ 백엔드 배포 (Railway)

### 3.1 Railway 프로젝트 생성
1. https://railway.app → 로그인 (GitHub)
2. **New Project** → **Deploy from GitHub repo**
3. `Midas_API_Hub` 선택

### 3.2 환경 변수 설정
Railway Dashboard → **Variables**:
```env
SUPABASE_URL=https://xxxxx.supabase.co
SUPABASE_SERVICE_ROLE_KEY=eyJhbGciOiJIUzI1...
PORT=9527
NODE_ENV=production
```

### 3.3 Start Command 설정
Railway Dashboard → **Settings** → **Start Command**:
```bash
node server/server.js
```

### 3.4 Root Directory 설정 (필요시)
만약 서버가 루트에 없다면:
- **Settings** → **Root Directory**: `/server`

### 3.5 배포 URL 확인
배포 완료 후 URL 복사:
```
https://midas-api-backend.up.railway.app
```

---

## 4️⃣ 프론트엔드 배포 (Vercel)

### 4.1 Vercel 프로젝트 생성
1. https://vercel.com → 로그인 (GitHub)
2. **Add New** → **Project**
3. Import `Midas_API_Hub`

### 4.2 빌드 설정
- **Framework Preset**: Vite
- **Build Command**: `npm run build`
- **Output Directory**: `dist`
- **Install Command**: `npm install`

### 4.3 환경 변수 설정
**Settings** → **Environment Variables**:
```env
VITE_API_URL=https://midas-api-backend.up.railway.app
VITE_SUPABASE_URL=https://xxxxx.supabase.co
VITE_SUPABASE_ANON_KEY=eyJhbGciOiJIUzI1...
```

### 4.4 배포
- **Deploy** 클릭
- 배포 완료 후 URL:
  ```
  https://midas-api-hub.vercel.app
  ```

---

## 5️⃣ 코드 수정 (Supabase 연동)

### 5.1 Supabase 클라이언트 생성
`src/lib/supabase.ts` 파일 생성:
```typescript
import { createClient } from '@supabase/supabase-js';

const supabaseUrl = import.meta.env.VITE_SUPABASE_URL;
const supabaseKey = import.meta.env.VITE_SUPABASE_ANON_KEY;

if (!supabaseUrl || !supabaseKey) {
  throw new Error('Supabase credentials are missing');
}

export const supabase = createClient(supabaseUrl, supabaseKey);
```

### 5.2 API Client 수정
`src/lib/api-client.ts`:
```typescript
const baseURL = import.meta.env.VITE_API_URL || 'http://localhost:9527';
```

### 5.3 실시간 구독 추가
`src/store/useAppStore.ts`에 Realtime 구독 추가:
```typescript
import { supabase } from '@/lib/supabase';

// 엔드포인트 잠금 실시간 구독
supabase
  .channel('endpoint-locks')
  .on('postgres_changes', {
    event: '*',
    schema: 'public',
    table: 'endpoint_locks'
  }, (payload) => {
    // 잠금 상태 업데이트
    checkEndpointLock(payload.new.endpoint_id);
  })
  .subscribe();
```

---

## 6️⃣ Git에 푸시 & 자동 배포

### 6.1 변경사항 커밋
```bash
git add .
git commit -m "feat: Add Supabase integration and deployment config"
git push origin main
```

### 6.2 자동 배포
- **Vercel**: GitHub push 시 자동 배포
- **Railway**: GitHub push 시 자동 배포

---

## 7️⃣ CORS 설정 (백엔드)

`server/server.js`에 CORS 설정:
```javascript
app.use(cors({
  origin: [
    'http://localhost:5178',
    'https://midas-api-hub.vercel.app',
    'https://midas-api-hub-*.vercel.app' // Preview deployments
  ],
  credentials: true
}));
```

---

## 8️⃣ 비용

| 서비스 | 무료 플랜 | 제한 |
|--------|----------|------|
| **Supabase** | ✅ 무료 | 500MB DB, 1GB Storage, 2GB 대역폭/월 |
| **Railway** | ✅ $5 크레딧/월 | ~500시간 실행 가능 |
| **Vercel** | ✅ 무료 | 무제한 배포, 100GB 대역폭/월 |

💡 **개발/팀 사용에는 무료 플랜으로 충분합니다!**

---

## 9️⃣ 테스트

### 배포 후 테스트 체크리스트:
- [ ] 프론트엔드 로드 확인
- [ ] 백엔드 API 호출 확인
- [ ] 데이터베이스 연결 확인
- [ ] 파일 업로드/다운로드 확인
- [ ] 편집 잠금 시스템 동작 확인
- [ ] 실시간 동기화 확인

### 테스트 명령어:
```bash
# API Health Check
curl https://midas-api-backend.up.railway.app/health

# 데이터베이스 연결 테스트
curl https://midas-api-backend.up.railway.app/api/endpoints/tree
```

---

## 🔧 트러블슈팅

### 문제 1: CORS 에러
- **원인**: 백엔드가 프론트엔드 도메인을 허용하지 않음
- **해결**: `server/server.js`의 CORS 설정에 Vercel URL 추가

### 문제 2: 환경 변수 안 읽힘
- **원인**: `.env` 파일이 배포되지 않음
- **해결**: Railway/Vercel Dashboard에서 환경 변수 설정

### 문제 3: 데이터베이스 연결 실패
- **원인**: Supabase credentials 오류
- **해결**: API Keys 재확인, service_role key 사용 확인

### 문제 4: 파일 업로드 실패
- **원인**: Storage bucket이 비공개
- **해결**: Supabase Storage 정책 설정 확인

---

## 📚 참고 문서

- [Supabase 공식 문서](https://supabase.com/docs)
- [Railway 배포 가이드](https://docs.railway.app/)
- [Vercel 배포 가이드](https://vercel.com/docs)
- [SUPABASE_SETUP.md](./SUPABASE_SETUP.md) - 자세한 Supabase 설정

---

## 🎉 완료!

배포가 완료되면:
- **프론트엔드**: https://midas-api-hub.vercel.app
- **백엔드**: https://midas-api-backend.up.railway.app
- **데이터베이스**: Supabase Dashboard에서 관리

이제 팀원들과 **실시간으로 협업**할 수 있습니다! 🚀

