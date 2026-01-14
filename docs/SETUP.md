# 시스템 프레임워크 구축 완료 ✅

## 📦 설치된 구조

```
api-verification-platform/
├── electron/              # Electron 메인 프로세스
│   ├── main.js           # Electron 진입점 (파일 시스템 접근)
│   └── preload.js        # IPC 브릿지 (보안)
├── src/
│   ├── components/
│   │   ├── layout/      # 레이아웃 컴포넌트
│   │   │   ├── MainLayout.tsx    # 메인 레이아웃
│   │   │   ├── Sidebar.tsx       # 왼쪽 사이드바
│   │   │   ├── ContentArea.tsx  # 중앙 영역
│   │   │   ├── TabBar.tsx        # 상단 탭 바
│   │   │   └── ContentPanel.tsx # 컨텐츠 패널
│   │   └── tabs/        # 탭 컴포넌트
│   │       ├── SampleFilesTab.tsx
│   │       ├── ZendeskManualTab.tsx
│   │       ├── ApiStructureTab.tsx
│   │       └── LiveTestTab.tsx
│   ├── store/
│   │   └── useAppStore.ts        # Zustand 상태 관리
│   ├── styles/
│   │   ├── global.css            # 전역 CSS 변수
│   │   ├── App.css
│   │   └── [Component].css      # 컴포넌트별 CSS
│   ├── types/
│   │   └── electron.d.ts         # Electron API 타입
│   ├── App.tsx
│   └── main.tsx
├── package.json
├── tsconfig.json
├── vite.config.ts
└── index.html
```

## ✅ 구현된 기능

### 1. 레이아웃 구조
- ✅ **왼쪽 사이드바**: 메뉴 네비게이션 (API 목록, 테스트 실행, 결과 분석 등)
- ✅ **상단 탭**: 4개 메인 탭 (제품 샘플 파일, Zendesk 매뉴얼, API 구조, 실시간 테스트)
- ✅ **중앙 컨텐츠**: 탭별 컨텐츠 표시 영역
- ✅ **오른쪽 사이드바**: 추후 사용 (토글 가능)

### 2. 성능 최적화
- ✅ **useShallow**: 모든 Zustand 사용 부분에 적용
  - `Sidebar.tsx`
  - `TabBar.tsx`
  - `ContentPanel.tsx`

### 3. 스타일링
- ✅ **CSS 변수**: `global.css`에 모든 색상, 간격, 폰트 정의
- ✅ **하드코딩 금지**: 모든 스타일은 CSS 변수 사용
- ✅ **Inter 폰트**: Google Fonts에서 로드

### 4. Electron 통합
- ✅ **파일 시스템 접근**: IPC를 통한 안전한 파일 읽기/쓰기
- ✅ **로컬 권한**: Electron을 통한 로컬 파일 접근 가능
- ✅ **하이브리드 모드**: 웹 개발 + Electron 데스크톱 앱

## 🚀 실행 방법

### 개발 모드 (웹 브라우저)
```bash
npm run dev
```
→ http://localhost:5178 에서 확인

### 개발 모드 (Electron 앱)
```bash
npm run electron:dev
```
→ Electron 데스크톱 앱으로 실행

### 프로덕션 빌드
```bash
npm run build        # 웹 빌드
npm run electron:build  # Electron 앱 빌드
```

## 🎨 UI 구조

```
┌─────────────────────────────────────────────────────────────┐
│  [왼쪽 사이드바]  [상단 탭]  [중앙 컨텐츠]  [오른쪽 사이드바] │
│                                                              │
│  • API 목록      📁 제품 샘플 파일                          │
│  • 테스트 실행   📚 Zendesk 매뉴얼                          │
│  • 결과 분석     🔧 API 구조                                │
│  • 설정          🧪 실시간 테스트                           │
└─────────────────────────────────────────────────────────────┘
```

## 📝 다음 단계

### 1. 탭별 기능 구현
- [ ] **제품 샘플 파일 탭**: 파일 업로드, 파싱, 분석
- [ ] **Zendesk 매뉴얼 탭**: 문서 뷰어, 불일치 검출
- [ ] **API 구조 탭**: GET/POST/PUT/DELETE 메서드별 표시
- [ ] **실시간 테스트 탭**: 파라미터 토글, API 테스트

### 2. Electron API 활용
- [ ] 파일 선택 다이얼로그
- [ ] 파일 읽기/쓰기
- [ ] 디렉토리 탐색

### 3. 상태 관리 확장
- [ ] API 목록 상태
- [ ] 테스트 결과 상태
- [ ] 파일 관리 상태

## 🛠 기술 스택

- **Frontend**: React 18 + TypeScript
- **Desktop**: Electron 28
- **State**: Zustand 4.4.7 (useShallow 최적화)
- **Build**: Vite 5
- **Styling**: CSS Modules (변수 기반)
- **Font**: Inter

## 📄 참고

- 모든 스타일은 `src/styles/global.css`의 CSS 변수 사용
- 성능 최적화를 위해 `useShallow` 필수 사용
- Electron API는 `window.electronAPI`를 통해 접근

