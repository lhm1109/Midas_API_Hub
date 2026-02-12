# 🚀 Supabase 설정 가이드

## 📝 개요

이 애플리케이션을 Supabase로 마이그레이션하여 팀원들과 실시간으로 데이터를 공유할 수 있습니다.

---

## 🔧 설정 단계

### **1단계: Supabase API 키 확인**

1. https://vqdtbhvoytalprawwxfe.supabase.co 접속
2. **Settings** → **API** 메뉴로 이동
3. **Project API keys** 섹션에서 다음 키들을 복사:
   - `anon public` 키 (나중에 프론트엔드에서 사용)
   - `service_role` 키 (⚠️ 비밀 키 - 서버에서만 사용)

---

### **2단계: .env 파일 생성**

프로젝트 루트에 `.env` 파일 생성:

**Git Bash / PowerShell:**
```bash
cd /c/MIDAS/code/apiverification
echo "SUPABASE_URL=https://vqdtbhvoytalprawwxfe.supabase.co" > .env
echo "SUPABASE_SERVICE_KEY=여기에_service_role_키_붙여넣기" >> .env
echo "USER_ID=user_lhm1109" >> .env
```

**또는 직접 파일 생성:**
`.env` 파일을 만들고 아래 내용 입력:
```
SUPABASE_URL=https://vqdtbhvoytalprawwxfe.supabase.co
SUPABASE_SERVICE_KEY=여기에_service_role_키_붙여넣기
USER_ID=user_lhm1109
```

---

### **3단계: Supabase에 스키마 적용**

1. Supabase 대시보드에서 **SQL Editor** 메뉴로 이동
2. **New Query** 클릭
3. `server/schema-with-locks.sql` 파일의 내용을 복사해서 붙여넣기
4. **Run** 클릭하여 실행

**또는 터미널에서:**
```bash
# Supabase CLI 설치 (선택사항)
npm install -g supabase
supabase db push
```

---

### **4단계: Supabase Storage Bucket 생성**

파일 첨부 기능을 위한 스토리지 버킷 생성:

1. Supabase 대시보드에서 **Storage** 메뉴로 이동
2. **New bucket** 클릭
3. 설정:
   - **Name**: `api-verification`
   - **Public bucket**: ❌ 체크 해제 (Private로 설정)
   - **File size limit**: 100 MB (또는 원하는 크기)
4. **Create bucket** 클릭

---

### **5단계: Storage 정책 설정 (중요!)**

파일 업로드/다운로드가 작동하려면 Storage 정책이 필요합니다:

1. **Storage** → **Policies** → **api-verification bucket**
2. 아래 정책들을 추가:

**업로드 정책:**
```sql
CREATE POLICY "Allow authenticated uploads"
ON storage.objects FOR INSERT
TO authenticated
WITH CHECK (bucket_id = 'api-verification');
```

**다운로드 정책:**
```sql
CREATE POLICY "Allow authenticated downloads"
ON storage.objects FOR SELECT
TO authenticated
USING (bucket_id = 'api-verification');
```

**삭제 정책:**
```sql
CREATE POLICY "Allow authenticated deletes"
ON storage.objects FOR DELETE
TO authenticated
USING (bucket_id = 'api-verification');
```

**또는 간단하게 (개발 중):**
```sql
-- 모든 인증된 사용자에게 전체 권한 부여
CREATE POLICY "Allow all for authenticated users"
ON storage.objects
TO authenticated
USING (bucket_id = 'api-verification')
WITH CHECK (bucket_id = 'api-verification');
```

---

### **6단계: 기존 데이터 마이그레이션 (선택사항)**

기존 SQLite 데이터가 있다면:

```bash
cd /c/MIDAS/code/apiverification
node server/migrate-sqlite-to-supabase.js
```

**주의:** 
- 첨부파일은 메타데이터만 마이그레이션됩니다
- 실제 파일은 `server/uploads`에서 Supabase Storage로 수동 업로드 필요

---

### **7단계: 서버 재시작**

```bash
cd /c/MIDAS/code/apiverification
npm run dev:all
```

**성공 메시지 확인:**
```
✅ Supabase connection successful
📡 Connected to: https://vqdtbhvoytalprawwxfe.supabase.co
```

---

## ✅ 확인 사항

### **연결 테스트**

브라우저에서 확인:
- http://localhost:5178 (프론트엔드)
- http://localhost:9527/health (백엔드 헬스체크)

### **기능 테스트**

1. ✅ 엔드포인트 목록 로드
2. ✅ 버전 생성/로드/저장
3. ✅ 파일 첨부 업로드/다운로드
4. ✅ 편집 잠금 (다른 사용자가 편집 중일 때 알림)

---

## 🔒 보안 고려사항

### **환경 변수 보호**

`.env` 파일은 Git에 커밋하지 마세요:
```bash
# .gitignore에 이미 포함되어 있음
.env
.env.local
.env.production
```

### **Service Role Key**

- ⚠️ **절대 프론트엔드에 노출하지 마세요**
- 서버 코드에서만 사용
- Git에 커밋하지 마세요

### **팀원 설정**

각 팀원은:
1. 자신의 `.env` 파일 생성
2. 동일한 Supabase 프로젝트 사용
3. `USER_ID`를 고유하게 설정 (예: `user_john`, `user_jane`)

---

## 🌐 실시간 협업

### **편집 잠금 시스템**

- 한 사용자가 엔드포인트를 편집 중이면 다른 사용자는 읽기 전용
- 5분간 활동이 없으면 자동 잠금 해제
- 화면에 누가 편집 중인지 표시

### **실시간 알림 (Realtime 구독)**

Supabase Realtime이 활성화되어 있으므로:
- 다른 사용자의 편집 시작/종료가 즉시 반영
- 별도의 폴링 불필요

---

## 🐛 트러블슈팅

### **"Failed to connect to Supabase" 오류**

1. `.env` 파일이 올바른 위치에 있는지 확인
2. `SUPABASE_SERVICE_KEY`가 올바른지 확인
3. 네트워크 연결 확인
4. Supabase 프로젝트가 활성화되어 있는지 확인

### **파일 업로드 실패**

1. Storage bucket `api-verification`이 생성되었는지 확인
2. Storage 정책이 올바르게 설정되었는지 확인
3. 파일 크기가 100MB 이하인지 확인

### **편집 잠금이 작동하지 않음**

1. `endpoint_locks` 테이블이 생성되었는지 확인
2. Realtime이 활성화되었는지 확인:
   ```sql
   ALTER PUBLICATION supabase_realtime ADD TABLE endpoint_locks;
   ```

---

## 📚 추가 리소스

- [Supabase 공식 문서](https://supabase.com/docs)
- [Supabase Storage 가이드](https://supabase.com/docs/guides/storage)
- [PostgreSQL 문서](https://www.postgresql.org/docs/)

---

## 💡 다음 단계

1. ✅ Supabase 설정 완료
2. ✅ 기존 데이터 마이그레이션
3. 🔄 팀원들과 공유
4. 🎉 실시간 협업 시작!

---

**문제가 있으면 언제든 질문해주세요!** 🙋‍♂️


