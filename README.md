# API 검증 시스템

API 엔드포인트를 체계적으로 관리하고 테스트하는 Electron + React 하이브리드 애플리케이션입니다.

## 주요 기능

### 🏠 홈 (대시보드)
- 전체 API 현황 통계
- 테스트 완료/대기 상태
- 최근 테스트 기록
- 성공률 차트

### 🔌 API 관리
- **왼쪽 패널**: 카테고리별 API 목록 (DB, DOC, OPE, VIEW, POST)
- **오른쪽 패널**: 선택된 API 상세 정보
  - 📁 제품 샘플 파일
  - 📚 Zendesk 매뉴얼
  - 🔧 API 구조 (스키마, 파라미터)
  - 🧪 실시간 테스트

### 🔄 프리셋 (자동화)
- React Flow 기반 비주얼 워크플로우
- 여러 API를 순차적으로 테스트
- 조건부 실행 및 딜레이 설정
- 프리셋 저장 및 재사용

### 💾 로컬 데이터베이스
- SQLite 기반 경량 DB
- 프리셋 저장 및 관리
- 테스트 결과 히스토리
- 통계 데이터 집계

## 기술 스택

- **Frontend**: React + TypeScript + Vite
- **Desktop**: Electron
- **상태 관리**: Zustand
- **스타일**: Tailwind CSS + 중앙 집중식 CSS
- **워크플로우**: React Flow
- **데이터베이스**: better-sqlite3
- **폰트**: Inter

## 설치 및 실행

```bash
# 의존성 설치
npm install

# 개발 모드 실행
npm run dev

# 프로덕션 빌드
npm run build

# Electron 앱 패키징
npm run electron:build
```

## 개발 서버

- **Vite Dev Server**: http://localhost:5178
- **Electron**: 자동으로 dev 서버에 연결

## 프로젝트 구조

```
src/
├── components/
│   ├── layout/          # 레이아웃 컴포넌트
│   │   ├── MainSidebar.tsx    # 메인 네비게이션 (홈/API/프리셋)
│   │   ├── Header.tsx         # 상단 헤더
│   │   ├── TabBar.tsx         # 탭 바
│   │   └── ContentPanel.tsx   # 컨텐츠 패널
│   ├── pages/           # 페이지 컴포넌트
│   │   ├── HomePage.tsx       # 대시보드
│   │   ├── ApiPage.tsx        # API 관리 페이지
│   │   └── PresetPage.tsx     # 프리셋 플로우 페이지
│   ├── tabs/            # 탭 컨텐츠
│   └── settings/        # 설정 모달
├── styles/
│   ├── index.css        # 전역 스타일 + Tailwind
│   └── components.css   # 컴포넌트별 스타일 (BEM)
├── utils/
│   ├── database.ts      # DB API 래퍼
│   ├── apiClient.ts     # API 클라이언트
│   └── apiSettings.ts   # API 설정 관리
├── store/               # Zustand 스토어
├── types/               # TypeScript 타입 정의
└── data/                # 정적 데이터

electron/
├── main.js              # Electron 메인 프로세스
├── preload.js           # Preload 스크립트
└── database.js          # SQLite DB 핸들러

api_data_set/            # API별 데이터 폴더
└── [CATEGORY]/
    └── [NUMBER]_[NAME]/
        ├── product_samples/
        ├── zendesk/
        ├── api_schema/
        └── test_results/
```

## API 설정

1. 우측 상단 "설정" 버튼 클릭
2. Base URL 입력 (예: `http://localhost:3000`)
3. MAPI-Key 입력
4. "연결 테스트" 버튼으로 확인
5. 저장하면 모든 API 요청에 자동으로 적용

## 데이터베이스

애플리케이션 데이터는 다음 위치에 저장됩니다:
- **Windows**: `%APPDATA%/api-verification/api-verification.db`
- **macOS**: `~/Library/Application Support/api-verification/api-verification.db`
- **Linux**: `~/.config/api-verification/api-verification.db`

### 테이블 구조

- `presets`: 저장된 워크플로우 프리셋
- `test_results`: API 테스트 결과 기록
- `api_test_history`: API별 테스트 통계

## 스타일 가이드

모든 스타일은 `src/styles/components.css`에서 중앙 집중식으로 관리됩니다.
- BEM 명명 규칙 사용
- Tailwind 유틸리티 클래스는 최소화
- CSS 변수로 색상/간격/폰트 크기 통일

## 라이선스

MIT
