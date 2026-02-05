# Bat Rack

> [!CAUTION]
> **⚠️ 모든 문서들은 바이브코딩 중 참고하기 위한 문서여서 정확하지 않을 수 있습니다.**


> **통합 API 개발 및 프로젝트 관리 플랫폼**  
> API 설계, 개발, 테스트, 문서화를 하나의 데스크톱 애플리케이션에서

<p align="center">
  <img src="https://img.shields.io/badge/Electron-28-blue?logo=electron" alt="Electron"/>
  <img src="https://img.shields.io/badge/React-18-blue?logo=react" alt="React"/>
  <img src="https://img.shields.io/badge/TypeScript-5-blue?logo=typescript" alt="TypeScript"/>
  <img src="https://img.shields.io/badge/Supabase-PostgreSQL-green?logo=supabase" alt="Supabase"/>
  <img src="https://img.shields.io/badge/TailwindCSS-4-blue?logo=tailwindcss" alt="Tailwind"/>
</p>

---

## 📖 개요

**Bat Rack**은 MIDAS IT의 API 개발 워크플로우를 통합 관리하는 Electron 기반 데스크톱 애플리케이션입니다.

프로젝트 계획부터 API 설계, 개발 진행 관리, 테스트, 문서화까지 전체 라이프사이클을 지원합니다.

### ✨ 주요 특징

- 🗂️ **프로젝트 중심 관리** - 제품/그룹/엔드포인트 계층 구조
- 📊 **칸반 보드 방식** - API 개발 파이프라인 시각화 (Plan → Dev → V&V → Doc → Deploy)
- 🧪 **통합 테스트 러너** - 버전별 테스트 케이스 관리 및 실행
- 📚 **실시간 매뉴얼** - Zendesk 문서 자동 연동
- 🖥️ **내장 터미널** - MCP 서버 연동으로 스키마 생성 자동화
- 📤 **CSV/Excel 지원** - 대량 데이터 import/export

---

## 🎯 주요 기능

### 📂 Projects (프로젝트 관리)

**제품 → 그룹 → 엔드포인트 계층 구조**로 API 프로젝트를 체계적으로 관리합니다.

- ✅ 드래그 앤 드롭으로 직관적인 구조 편집
- ✅ 그룹 계층 구조 지원 (최대 3단계 서브그룹)
- ✅ 엔드포인트별 메타데이터 관리
- ✅ Manager 탭과 양방향 연동

### 👤 Manager (개발 파이프라인)

**API 개발 진행 상황**을 테이블 뷰로 추적합니다.

- ✅ 파이프라인 단계: `Plan` → `Dev` → `V&V` → `Doc` → `Deploy` → `Issue`
- ✅ 상태별 아이콘 (empty, progress, done, warning)
- ✅ CSV/Excel 일괄 import/export
- ✅ 대소문자 구분 없는 컬럼 매핑
- ✅ 실시간 검색 및 필터링

### 🧪 Runner (테스트 실행)

**API 테스트 케이스**를 버전별로 관리하고 실행합니다.

- ✅ Request Body 자동 로드
- ✅ 테스트 케이스별 응답 저장
- ✅ Manual 탭으로 케이스 전송
- ✅ 히스토리 관리

### 📚 Manual (API 문서)

**Zendesk 매뉴얼**과 **API 스키마**를 통합하여 표시합니다.

- ✅ Request/Response 예시
- ✅ 파라미터 자동 문서화
- ✅ Code snippet 복사
- ✅ 실시간 동기화

### 🖥️ Terminal (통합 터미널)

**MCP (Model Context Protocol) 서버** 연동으로 스키마 생성을 자동화합니다.

- ✅ 내장 xterm.js 터미널
- ✅ AI 기반 스키마 생성
- ✅ 검증 및 변환 파이프라인
- ✅ YAML 기반 SSOT 관리

---

## 🚀 빠른 시작

### 사전 요구사항

- Node.js 18+ (권장: v20 LTS)
- npm 9+
- Supabase 계정

### 설치

```bash
# 1. 저장소 클론
git clone <repository-url>
cd apiverification

# 2. 의존성 설치
npm install

# 3. 환경 변수 설정
cp .env.example .env
# .env 파일을 열어 Supabase 정보 입력

# 4. 데이터베이스 스키마 생성
# Supabase SQL Editor에서 server/schema-*.sql 파일 실행
# (자세한 내용은 docs/SETUP.md 참고)

# 5. 앱 실행
npm run electron:dev
```

> 📖 **자세한 설치 가이드**: [docs/SETUP.md](./docs/SETUP.md)

---

## 🛠️ 기술 스택

### Frontend
- **React 18** - UI 라이브러리
- **TypeScript 5** - 타입 안정성
- **Vite 6** - 빌드 도구
- **Tailwind CSS 4** - 스타일링
- **Radix UI** - 접근성 높은 컴포넌트
- **Zustand** - 상태 관리
- **dnd-kit** - 드래그 앤 드롭

### Backend
- **Express.js** - REST API 서버
- **Supabase** - PostgreSQL 데이터베이스
- **node-pty** - 터미널 에뮬레이터

### Desktop
- **Electron 28** - 크로스 플랫폼 데스크톱
- **Monaco Editor** - 코드 에디터
- **xterm.js** - 터미널 UI

### 기타
- **xlsx** - Excel/CSV 처리
- **js-yaml** - YAML 파싱
- **react-flow** - 플로우차트 (향후 확장용)

---

## 📁 프로젝트 구조

```
bat-rack/
├── src/                    # React 프론트엔드
│   ├── features/           # 기능별 모듈
│   │   ├── projects/       # Projects 탭
│   │   ├── manager/        # Manager 탭
│   │   ├── runner/         # Runner 탭
│   │   ├── manual/         # Manual 탭
│   │   └── terminal/       # Terminal 탭
│   ├── components/ui/      # 재사용 UI 컴포넌트
│   ├── lib/                # 유틸리티
│   └── store/              # Zustand 스토어
├── server/                 # Express 백엔드
│   ├── routes/             # API 라우트
│   ├── schema-*.sql        # DB 스키마
│   └── server.js           # 서버 진입점
├── electron/               # Electron 메인 프로세스
├── mcp-server/             # MCP 서버 (스키마 생성)
├── schema_definitions/     # JSON 스키마 정의
└── docs/                   # 문서
```

---

## 📚 문서

- [📖 설치 가이드](./docs/SETUP.md) - 처음 설정하는 방법
- [🗄️ Manager 설정](./server/MANAGER_SETUP.md) - Manager 탭 상세 가이드
- [🔧 MCP 서버](./mcp-server/README.md) - 터미널 스키마 생성 가이드
- [📝 스키마 정의](./schema_definitions/README.md) - JSON 스키마 작성 가이드

---

## 🎨 스크린샷

<!-- TODO: 스크린샷 추가 -->

### Projects 탭
계층 구조로 API 프로젝트를 관리합니다.

### Manager 탭
개발 파이프라인을 테이블 뷰로 추적합니다.

### Runner 탭
테스트 케이스를 실행하고 응답을 저장합니다.

---

## 🔧 개발

### 개발 모드

```bash
# Electron 앱 개발 모드
npm run electron:dev

# 백엔드만 실행
npm run server

# 프론트엔드만 실행
npm run dev
```

### 빌드

```bash
# 프로덕션 빌드
npm run build

# Electron 앱 패키징
npm run electron:build
```

### 스크립트

| 명령어 | 설명 |
|--------|------|
| `npm run dev` | Vite 개발 서버 실행 |
| `npm run server` | Express 서버 실행 |
| `npm run electron:dev` | 전체 앱 개발 모드 |
| `npm run electron:build` | 앱 패키징 |
| `npm run validate:shared` | YAML 스키마 검증 |

---

## 🐛 트러블슈팅

### 일반적인 문제

**1. Electron 앱이 시작되지 않음**
```bash
# 캐시 삭제
rm -rf node_modules/.vite
npm run electron:dev
```

**2. Supabase 연결 오류**
```bash
# .env 파일 확인
cat .env
# SUPABASE_URL과 SUPABASE_SERVICE_KEY가 올바른지 확인
```

**3. 네이티브 모듈 빌드 실패**
```bash
# Windows
npm install --global windows-build-tools

# macOS
xcode-select --install

# Linux
sudo apt-get install build-essential
```

> 📖 **더 많은 해결 방법**: [docs/SETUP.md#트러블슈팅](./docs/SETUP.md#-트러블슈팅)

---

## 📊 Manager 탭 상태 값

### Progress 컬럼 (Plan, Dev, V&V, Doc, Deploy, Issue)

| 값 | 표시 | 의미 |
|---|---|---|
| `empty` | ⭕ 빈 원 | 시작 안함 |
| `progress` | 🔄 회전 | 작업 중 |
| `done` | ✅ 체크 | 완료 |
| `warning` | ⚠️ 경고 | 문제 발생 |

### Status 컬럼

| 값 | 색상 | 의미 |
|---|---|---|
| `cancel` | 빨간색 | 취소됨 |
| `done` | 녹색 | 완료 |
| `progress` | 노란색 | 진행 중 |
| `working` | 파란색 | 작업 중 |
| `none` | - | 없음 |

---