# 🚀 Supabase 마이그레이션 가이드

## 📋 목차
1. [Supabase 프로젝트 생성](#1-supabase-프로젝트-생성)
2. [데이터베이스 스키마 적용](#2-데이터베이스-스키마-적용)
3. [환경 변수 설정](#3-환경-변수-설정)
4. [데이터 마이그레이션](#4-데이터-마이그레이션)
5. [실시간 구독 설정](#5-실시간-구독-설정)
6. [파일 저장소 설정](#6-파일-저장소-설정)

---

## 1. Supabase 프로젝트 생성

### 1.1 회원가입 및 프로젝트 생성
1. https://supabase.com 접속
2. **Start your project** 클릭
3. 프로젝트 이름: `midas-api-verification`
4. Database Password 설정 (강력한 비밀번호 권장)
5. Region: **Northeast Asia (Seoul)** 선택
6. **Create new project** 클릭

### 1.2 프로젝트 정보 확인
프로젝트 생성 후 다음 정보를 확인하세요:
- **Project URL**: `https://xxxxx.supabase.co`
- **API Keys**:
  - `anon` key (공개 키)
  - `service_role` key (비공개 키, 서버 전용)

---

## 2. 데이터베이스 스키마 적용

### 2.1 SQL Editor에서 스키마 생성
1. Supabase 대시보드에서 **SQL Editor** 클릭
2. **New query** 클릭
3. `server/schema-with-locks.sql` 파일의 내용을 복사
4. SQL Editor에 붙여넣기
5. **Run** 버튼 클릭

### 2.2 RLS (Row Level Security) 설정 (선택사항)
```sql
-- 모든 테이블에 대한 읽기 허용 (개발 단계)
ALTER TABLE endpoints ENABLE ROW LEVEL SECURITY;
CREATE POLICY "Allow public read" ON endpoints FOR SELECT USING (true);

-- 쓰기는 인증된 사용자만
CREATE POLICY "Allow authenticated write" ON endpoints 
  FOR ALL USING (auth.role() = 'authenticated');
```

---

## 3. 환경 변수 설정

### 3.1 `.env` 파일 생성
프로젝트 루트에 `.env` 파일을 생성하고 다음 내용 추가:

```env
# Supabase 설정
VITE_SUPABASE_URL=https://xxxxx.supabase.co
VITE_SUPABASE_ANON_KEY=your_anon_key_here

# 서버용 (service_role key)
SUPABASE_URL=https://xxxxx.supabase.co
SUPABASE_SERVICE_ROLE_KEY=your_service_role_key_here

# 기존 설정
VITE_API_URL=http://localhost:9527
```

### 3.2 `.gitignore`에 추가
```
.env
.env.local
```

---

## 4. 데이터 마이그레이션

### 4.1 Supabase 클라이언트 설치
```bash
npm install @supabase/supabase-js
```

### 4.2 마이그레이션 스크립트 실행
```bash
node server/migrate-to-supabase.js
```

이 스크립트는:
- ✅ SQLite DB의 모든 데이터를 읽음
- ✅ Supabase로 데이터 전송
- ✅ 첨부 파일을 Supabase Storage로 업로드

---

## 5. 실시간 구독 설정

### 5.1 Realtime 활성화
Supabase 대시보드에서:
1. **Database** → **Replication** 메뉴
2. **endpoint_locks** 테이블 찾기
3. **Enable** 버튼 클릭
4. **version_locks** 테이블도 동일하게 활성화

### 5.2 프론트엔드에서 실시간 구독
```typescript
import { createClient } from '@supabase/supabase-js'

const supabase = createClient(
  import.meta.env.VITE_SUPABASE_URL,
  import.meta.env.VITE_SUPABASE_ANON_KEY
)

// 엔드포인트 잠금 상태 구독
const subscription = supabase
  .channel('endpoint-locks')
  .on(
    'postgres_changes',
    {
      event: '*',
      schema: 'public',
      table: 'endpoint_locks'
    },
    (payload) => {
      console.log('Lock status changed:', payload)
      // UI 업데이트
    }
  )
  .subscribe()
```

---

## 6. 파일 저장소 설정

### 6.1 Storage Bucket 생성
1. **Storage** 메뉴 클릭
2. **Create a new bucket** 클릭
3. Bucket 이름: `attachments`
4. **Public bucket** 체크 해제 (비공개)
5. **Create bucket** 클릭

### 6.2 Storage 정책 설정
```sql
-- 인증된 사용자만 업로드 가능
CREATE POLICY "Allow authenticated upload" ON storage.objects
  FOR INSERT WITH CHECK (
    bucket_id = 'attachments' AND 
    auth.role() = 'authenticated'
  );

-- 모든 사용자가 다운로드 가능
CREATE POLICY "Allow public download" ON storage.objects
  FOR SELECT USING (bucket_id = 'attachments');
```

---

## 7. 편집 잠금 시스템 동작 원리

### 7.1 잠금 획득
```typescript
// 사용자가 엔드포인트 편집 시작
POST /api/locks/endpoint/:endpointId/lock
{
  "userId": "user@example.com"
}

// 성공 응답 (200)
{
  "success": true,
  "lock": {
    "endpointId": "db/beam",
    "lockedBy": "user@example.com",
    "expiresAt": "2026-01-12T16:40:00Z"
  }
}

// 다른 사용자가 잠금 시도 시 실패 (423 Locked)
{
  "error": "Endpoint is locked by another user",
  "lockedBy": "user@example.com",
  "expiresAt": "2026-01-12T16:40:00Z"
}
```

### 7.2 자동 잠금 해제
- ⏰ **5분 후 자동 해제** (inactivity timeout)
- 🔄 사용자가 계속 편집 중이면 자동 갱신
- 💾 사용자가 저장 또는 취소 시 수동 해제

### 7.3 UI 표시
```typescript
// 다른 사용자가 편집 중일 때
<Alert variant="warning">
  ⚠️ {lockedBy}님이 현재 편집 중입니다.
  {expiresAt}까지 잠금 상태입니다.
</Alert>

// 읽기 전용 모드로 표시
<Input disabled readOnly />
```

---

## 8. 테스트

### 8.1 서버 재시작
```bash
npm run dev:all
```

### 8.2 잠금 테스트
```bash
# 잠금 획득
curl -X POST http://localhost:9527/api/locks/endpoint/db%2Fbeam/lock \
  -H "Content-Type: application/json" \
  -d '{"userId":"user1@test.com"}'

# 잠금 상태 확인
curl http://localhost:9527/api/locks/endpoint/db%2Fbeam/lock

# 잠금 해제
curl -X DELETE http://localhost:9527/api/locks/endpoint/db%2Fbeam/lock \
  -H "Content-Type: application/json" \
  -d '{"userId":"user1@test.com"}'
```

---

## 9. Supabase vs SQLite 비교

| 기능 | SQLite (현재) | Supabase (이전 후) |
|------|--------------|-------------------|
| 동시 접속 | ❌ 제한적 | ✅ 무제한 |
| 실시간 업데이트 | ❌ 없음 | ✅ Realtime 지원 |
| 파일 저장소 | 📁 로컬 폴더 | ☁️ Cloud Storage |
| 백업 | 수동 파일 복사 | ✅ 자동 백업 |
| 확장성 | ❌ 단일 서버 | ✅ Auto-scaling |
| 비용 | 🆓 무료 | 🆓 무료 (500MB까지) |

---

## 10. 다음 단계

✅ **현재 완료**:
- 편집 잠금 API 구현 (SQLite 기반)
- 스키마 설계 완료

🔄 **진행 중**:
- Supabase 프로젝트 생성
- 데이터 마이그레이션

📝 **향후 작업**:
- 프론트엔드 잠금 UI 구현
- 실시간 알림 추가
- 사용자 인증 시스템 연동

---

## 💡 도움말

### 문제 해결
1. **연결 실패**: 방화벽 설정 확인
2. **권한 오류**: Supabase 대시보드에서 RLS 정책 확인
3. **속도 느림**: Region을 Seoul로 설정했는지 확인

### 추가 리소스
- [Supabase 공식 문서](https://supabase.com/docs)
- [Realtime 가이드](https://supabase.com/docs/guides/realtime)
- [Storage 가이드](https://supabase.com/docs/guides/storage)

