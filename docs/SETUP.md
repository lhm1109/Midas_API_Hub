# Bat Rack 설치 가이드

> **Bat Rack** - API Development & Server Management Platform  
> MIDAS IT API 개발, 검증 및 프로젝트 관리를 위한 통합 Electron 데스크톱 애플리케이션

---

## 📋 목차

1. [시스템 요구사항](#-시스템-요구사항)
2. [사전 준비](#-사전-준비)
3. [설치 단계](#-설치-단계)
4. [데이터베이스 설정](#-데이터베이스-설정-supabase)
5. [환경 변수 설정](#-환경-변수-설정)
6. [앱 실행](#-앱-실행)
7. [첫 실행 후 설정](#-첫-실행-후-설정)
8. [트러블슈팅](#-트러블슈팅)
9. [개발 가이드](#-개발-가이드)

---

## 🖥️ 시스템 요구사항

### 최소 요구사항
- **OS**: Windows 10+, macOS 10.15+, Linux (Ubuntu 20.04+)
- **Node.js**: v18.0.0 이상
- **npm**: v9.0.0 이상
- **RAM**: 4GB 이상 권장
- **디스크 공간**: 1GB 이상

### 권장 사항
- **Node.js**: v20.x LTS
- **RAM**: 8GB 이상
- **인터넷 연결**: Supabase 연결 필요

---

## 🔧 사전 준비

### 1. Node.js 설치

Node.js가 설치되어 있지 않다면:

**Windows:**
```bash
# https://nodejs.org 에서 LTS 버전 다운로드 및 설치
```

**macOS (Homebrew 사용):**
```bash
brew install node@20
```

**Linux (Ubuntu/Debian):**
```bash
curl -fsSL https://deb.nodesource.com/setup_20.x | sudo -E bash -
sudo apt-get install -y nodejs
```

### 2. Git 설치

프로젝트를 클론하기 위해 Git이 필요합니다:
```bash
# Windows: https://git-scm.com/download/win
# macOS: brew install git
# Linux: sudo apt-get install git
```

### 3. Supabase 계정

프로젝트는 Supabase를 데이터베이스로 사용합니다:
1. [Supabase](https://supabase.com)에 계정 생성
2. 새 프로젝트 생성
3. Project Settings → API → Project URL 및 Service Role Key 복사

---

## 📥 설치 단계

### 1. 저장소 클론

```bash
# 저장소 클론
git clone <repository-url>
cd apiverification

# 또는 특정 브랜치 클론
git clone -b <branch-name> <repository-url>
cd apiverification
```

### 2. 의존성 설치

```bash
# npm 의존성 설치
npm install

# 설치 완료 확인
npm list --depth=0
```

> **참고**: `better-sqlite3`, `node-pty` 등 네이티브 모듈이 포함되어 있어 설치 시간이 다소 소요될 수 있습니다.

### 3. 환경 변수 파일 생성

```bash
# .env.example을 복사하여 .env 파일 생성
cp .env.example .env  # macOS/Linux
copy .env.example .env  # Windows
```

---

## 🗄️ 데이터베이스 설정 (Supabase)

### 1. Supabase 프로젝트 생성

1. [Supabase Dashboard](https://app.supabase.com)에 로그인
2. "New Project" 클릭
3. 프로젝트 이름, 데이터베이스 비밀번호 입력
4. 리전 선택 (가장 가까운 리전 권장)
5. 프로젝트 생성 완료까지 대기 (약 2분 소요)

### 2. API 키 복사

1. 프로젝트 대시보드에서 **Settings** → **API** 이동
2. 다음 정보 복사:
   - **Project URL**: `https://xxxxx.supabase.co`
   - **Service Role Key**: `eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9...`

### 3. 데이터베이스 스키마 생성

Supabase SQL Editor에서 다음 SQL 파일들을 **순차적으로** 실행하세요:

#### 3.1. 기본 테이블 생성

```sql
-- 1. Products & Groups 테이블
-- 파일: server/schema-products-groups.sql
-- Supabase SQL Editor에 붙여넣기 후 실행
```

```sql
-- 2. Manager 테이블
-- 파일: server/schema-manager.sql
-- Supabase SQL Editor에 붙여넣기 후 실행
```

#### 3.2. 필수 마이그레이션

```sql
-- 3. Products 컬럼 추가
-- 파일: server/schema-fix-products-columns.sql
-- 실행하여 psd_set, schema_type 컬럼 추가
```

```sql
-- 4. Manager Deploy 컬럼 추가
-- 파일: server/schema-migrate-add-deploy-column.sql
-- 실행하여 deploy 컬럼 추가
```

```sql
-- 5. Manager 상태 값 마이그레이션 (데이터가 있는 경우에만)
-- 파일: server/schema-manager-status-migration.sql
-- wip → progress, 대문자 → 소문자 변환
```

```sql
-- 6. Test Case Response 테이블
-- 파일: server/schema-add_test_case_response.sql
-- Runner 탭 테스트 케이스 응답 저장용
```

#### 3.3. 선택적 마이그레이션

```sql
-- Linked Endpoint ID 추가 (프로젝트 연결 기능 사용 시)
-- 파일: server/schema-add-linked-endpoint-id.sql
```

### 4. 데이터베이스 검증

다음 쿼리로 테이블이 정상적으로 생성되었는지 확인:

```sql
-- 모든 테이블 확인
SELECT table_name 
FROM information_schema.tables 
WHERE table_schema = 'public'
ORDER BY table_name;

-- 예상 결과: products, groups, manager_tasks, manager_columns, test_case_responses 등
```

---

## 🔐 환경 변수 설정

`.env` 파일을 열고 다음 값들을 설정하세요:

```env
# Supabase Configuration
SUPABASE_URL=https://vqdtbhvoytalprawwxfe.supabase.co
SUPABASE_SERVICE_KEY=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.your-actual-key-here

# User ID (for locking mechanism)
# 각 개발자마다 고유한 ID 설정
USER_ID=user_yourname
```

### 환경 변수 설명

| 변수 | 설명 | 예시 |
|------|------|------|
| `SUPABASE_URL` | Supabase 프로젝트 URL | `https://xxxxx.supabase.co` |
| `SUPABASE_SERVICE_KEY` | Supabase Service Role Key (admin 권한) | `eyJhbGciOiJIUzI1...` |
| `USER_ID` | 사용자 식별자 (편집 잠금용) | `user_john`, `user_alice` |

> ⚠️ **중요**: `SUPABASE_SERVICE_KEY`는 **절대 커밋하지 마세요**. `.gitignore`에 `.env`가 포함되어 있는지 확인하세요.

---

## 🚀 앱 실행

### 개발 모드 실행

```bash
# Electron 앱 개발 모드 (권장)
npm run electron:dev

# 이 명령은 다음을 동시에 실행합니다:
# - Express 백엔드 서버 (포트 9527)
# - Vite 개발 서버 (포트 5178)
# - Electron 앱
```

### 개별 실행 (디버깅용)

```bash
# 1. 백엔드 서버만 실행
npm run server

# 2. (다른 터미널에서) 프론트엔드만 실행
npm run dev

# 3. (다른 터미널에서) Electron만 실행
npm run electron
```

### 프로덕션 빌드

```bash
# 1. 프로덕션 빌드
npm run build

# 2. Electron 앱 패키징
npm run electron:build

# 빌드된 실행 파일 위치:
# Windows: release/Bat Rack-1.0.0-portable.exe
# macOS: release/Bat Rack-1.0.0.dmg
# Linux: release/Bat Rack-1.0.0.AppImage
```

---

## ⚙️ 첫 실행 후 설정

### 1. API 서버 설정

앱 실행 후:

1. 우측 상단 **⚙️ 설정** 버튼 클릭
2. **Base URL** 입력:
   ```
   http://localhost:9527
   ```
3. **MAPI-Key** 입력 (테스트용):
   ```
   your-mapi-key-here
   ```
4. **연결 테스트** 버튼 클릭하여 확인
5. **저장** 클릭

### 2. 탭 살펴보기

| 탭 | 설명 |
|---|---|
| **🏠 Home** | 대시보드 및 통계 |
| **📂 Projects** | API 프로젝트 관리 (제품/그룹/엔드포인트) |
| **👤 Manager** | API 개발 파이프라인 관리 (칸반 보드) |
| **🧪 Runner** | API 테스트 케이스 실행 |
| **📚 Manual** | API 매뉴얼 및 문서 |
| **🖥️ Terminal** | 내장 터미널 (MCP 서버 연동) |

### 3. 프로젝트 추가

**Projects 탭에서:**

1. **➕ 제품 추가** 클릭
2. 제품명, 설명 입력
3. **그룹 추가** 클릭하여 API 그룹 생성
4. **엔드포인트 추가** 클릭하여 API 추가

---

## 🐛 트러블슈팅

### 1. `npm install` 실패

**문제:** 네이티브 모듈 빌드 실패
```
gyp ERR! build error
```

**해결:**
```bash
# Windows: Visual Studio Build Tools 설치
npm install --global windows-build-tools

# macOS: Xcode Command Line Tools 설치
xcode-select --install

# Linux: 빌드 도구 설치
sudo apt-get install build-essential
```

### 2. Electron 앱이 시작되지 않음

**문제:** Vite 서버 연결 실패

**해결:**
```bash
# 1. 포트 5178이 사용 가능한지 확인
netstat -ano | findstr :5178  # Windows
lsof -i :5178                 # macOS/Linux

# 2. 캐시 삭제 후 재시작
rm -rf node_modules/.vite
npm run electron:dev
```

### 3. Supabase 연결 오류

**문제:** `Error: Invalid Supabase URL`

**해결:**
1. `.env` 파일이 프로젝트 루트에 있는지 확인
2. 환경 변수가 올바른지 확인:
   ```bash
   # .env 파일 확인
   cat .env  # macOS/Linux
   type .env  # Windows
   ```
3. 서버 재시작:
   ```bash
   # Ctrl+C로 종료 후
   npm run electron:dev
   ```

### 4. "Module not found" 에러

**문제:** TypeScript 경로 별칭 오류

**해결:**
```bash
# TypeScript 재컴파일
npx tsc --build --clean
npx tsc

# 또는 모듈 재설치
rm -rf node_modules package-lock.json
npm install
```

### 5. 데이터베이스 테이블 없음

**문제:** `relation "products" does not exist`

**해결:**
1. Supabase SQL Editor로 이동
2. [데이터베이스 설정](#-데이터베이스-설정-supabase) 섹션의 SQL 파일들을 순차 실행
3. 모든 테이블이 생성되었는지 확인

### 6. Excel/CSV Import 실패

**문제:** Excel 파일 가져오기 시 오류

**해결:**
1. 파일 형식이 `.xlsx`, `.xls`, `.xlsm`, `.csv` 중 하나인지 확인
2. 첫 번째 행이 헤더인지 확인
3. 파일이 손상되지 않았는지 확인

---

## 👨‍💻 개발 가이드

### 프로젝트 구조

```
bat-rack/
├── src/                          # 프론트엔드 소스
│   ├── components/ui/            # 재사용 가능한 UI 컴포넌트 (Shadcn)
│   ├── features/                 # 기능별 모듈
│   │   ├── projects/             # Projects 탭
│   │   ├── manager/              # Manager 탭
│   │   ├── runner/               # Runner 탭
│   │   └── manual/               # Manual 탭
│   ├── lib/                      # 유틸리티 및 라이브러리
│   ├── store/                    # Zustand 상태 관리
│   └── types/                    # TypeScript 타입 정의
├── server/                       # Express 백엔드
│   ├── routes/                   # API 라우트
│   ├── schema-*.sql              # 데이터베이스 스키마
│   └── server.js                 # Express 서버 진입점
├── electron/                     # Electron 메인/프리로드
│   ├── main.cjs                  # Electron 메인 프로세스
│   └── preload.cjs               # Preload 스크립트
├── mcp-server/                   # MCP 서버 (터미널 통합)
│   ├── src/                      
│   └── package.json
├── schema_definitions/           # JSON 스키마 정의
│   └── civil_gen_definition/
│       └── enhanced/
├── generated_schemas/            # 생성된 스키마 파일
└── docs/                         # 문서
```

### 주요 기술 스택

| 영역 | 기술 |
|------|------|
| **Frontend** | React 18 + TypeScript + Vite |
| **Desktop** | Electron 28 |
| **UI Components** | Radix UI + Tailwind CSS 4 |
| **State Management** | Zustand |
| **Backend** | Express.js |
| **Database** | Supabase (PostgreSQL) |
| **Code Editor** | Monaco Editor |
| **Drag & Drop** | dnd-kit |
| **Excel/CSV** | xlsx library |
| **Terminal** | xterm.js + node-pty |

### 개발 워크플로우

1. **브랜치 생성**
   ```bash
   git checkout -b feature/your-feature-name
   ```

2. **코드 작성 및 테스트**
   ```bash
   npm run electron:dev
   ```

3. **커밋**
   ```bash
   git add .
   git commit -m "feat: add your feature description"
   ```

4. **푸시 및 PR**
   ```bash
   git push origin feature/your-feature-name
   ```

### 유용한 스크립트

```bash
# YAML 스키마 검증
npm run validate:shared

# 데이터베이스 마이그레이션 (Supabase로 이전)
npm run migrate

# 프리뷰 모드 (빌드 확인)
npm run preview
```

### 환경별 설정

개발/테스트/프로덕션 환경별로 다른 `.env` 파일을 사용할 수 있습니다:

```bash
.env              # 기본 (gitignore됨)
.env.example      # 예시 템플릿
.testenv          # 테스트 환경 (선택사항)
```

---

## 📚 추가 문서

- [API 문서](./API.md) - REST API 엔드포인트 명세
- [Manager 설정 가이드](../server/MANAGER_SETUP.md) - Manager 탭 상세 설정
- [Schema 정의 가이드](../schema_definitions/README.md) - JSON 스키마 작성 방법
- [MCP 서버 가이드](../mcp-server/README.md) - 터미널 통합 및 MCP 도구

---

## 🙋 도움말

### 문제 발생 시

1. **로그 확인**
   - Electron 개발자 도구: `Ctrl+Shift+I` (Windows/Linux) 또는 `Cmd+Option+I` (macOS)
   - 서버 로그: 터미널 출력 확인

2. **이슈 제기**
   - GitHub Issues에 다음 정보와 함께 제기:
     - OS 및 버전
     - Node.js 버전 (`node --version`)
     - npm 버전 (`npm --version`)
     - 에러 로그 전문
     - 재현 단계

3. **커뮤니티**
   - 팀 Slack 채널
   - 개발팀 문의: dev@midasit.com

---

## ✅ 설치 체크리스트

설치가 완료되었다면 다음 항목들을 확인하세요:

- [ ] Node.js v18+ 설치 확인 (`node --version`)
- [ ] npm 의존성 설치 완료 (`npm install`)
- [ ] `.env` 파일 생성 및 Supabase 키 설정
- [ ] Supabase 데이터베이스 스키마 생성 완료
- [ ] `npm run electron:dev` 실행 성공
- [ ] Electron 앱이 정상적으로 열림
- [ ] Projects 탭에서 제품 추가 가능
- [ ] Manager 탭에서 작업 추가 가능
- [ ] Terminal 탭에서 명령 실행 가능

모든 항목이 체크되었다면 설치 완료입니다! 🎉

---

**마지막 업데이트**: 2026-02-05  
**버전**: 1.0.0  
**작성자**: MIDAS IT Development Team
