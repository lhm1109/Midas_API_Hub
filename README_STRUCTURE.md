# 프로젝트 구조 (확장성 있는 아키텍처)

## 📁 디렉토리 구조

```
src/
├── App.tsx                      # 메인 애플리케이션 컴포넌트
├── main.tsx                     # 진입점
│
├── features/                    # 기능별 모듈 (Feature-based)
│   │
│   ├── projects/               # Projects 탭 (왼쪽 사이드바)
│   │   ├── components/
│   │   │   ├── ProjectsView.tsx     # 메인 워크스페이스
│   │   │   ├── APIListPanel.tsx     # API 목록 패널
│   │   │   ├── index.ts
│   │   │   └── tabs/                # Projects 안의 하위 탭들
│   │   │       ├── VersionTab.tsx
│   │   │       ├── ManualTab.tsx
│   │   │       ├── SpecTab.tsx
│   │   │       ├── BuilderTab.tsx
│   │   │       ├── RunnerTab.tsx
│   │   │       ├── CompareVersionsDialog.tsx
│   │   │       └── index.ts
│   │   ├── hooks/
│   │   └── types/
│   │
│   └── history/                # History 탭 (왼쪽 사이드바)
│       ├── components/
│       │   ├── HistoryView.tsx
│       │   └── index.ts
│       ├── hooks/
│       └── types/
│
├── components/                  # 공통 컴포넌트
│   ├── ui/                     # shadcn/ui 컴포넌트
│   │   ├── button.tsx
│   │   ├── input.tsx
│   │   └── ...
│   │
│   ├── common/                 # 재사용 가능한 공통 컴포넌트
│   │   ├── SettingsModal.tsx
│   │   ├── figma/
│   │   └── index.ts
│   │
│   └── layouts/                # 레이아웃 컴포넌트
│       ├── GlobalSidebar.tsx   # 왼쪽 최상위 사이드바
│       └── index.ts
│
├── store/                       # 전역 상태 관리 (Zustand)
│   └── useAppStore.ts
│
├── hooks/                       # 공통 커스텀 훅
│   ├── useDebounce.ts
│   ├── useLocalStorage.ts
│   └── index.ts
│
├── lib/                         # 외부 라이브러리 래퍼
│   └── api-client.ts           # API 클라이언트
│
├── utils/                       # 유틸리티 함수
│   ├── format.ts               # 포맷팅 함수
│   ├── validation.ts           # 검증 함수
│   └── index.ts
│
├── types/                       # 공통 타입 정의
│   └── index.ts
│
├── config/                      # 설정 파일
│   └── constants.ts            # 상수
│
├── data/                        # 정적 데이터
│   └── apiSpecs.ts
│
├── styles/                      # 스타일 파일
│   ├── index.css
│   ├── tailwind.css
│   ├── theme.css
│   └── fonts.css
│
└── services/                    # (deprecated, lib으로 이동)
    └── api.ts

server/                          # 백엔드 서버
├── server.js                   # Express 서버
├── database.js                 # SQLite 데이터베이스
├── routes/                     # API 라우트
│   ├── versions.js
│   └── data.js
└── README.md
```

## 🎯 설계 원칙

### 1. Feature-based Architecture (탭 기반 구조)
- **왼쪽 사이드바의 각 탭이 하나의 feature**
  - `features/projects/`: Projects 탭 전체
  - `features/history/`: History 탭 전체
- 각 feature는 독립적인 모듈로 구성
- 하위 탭들은 `tabs/` 폴더에 구성

### 2. 계층 구조
```
왼쪽 사이드바 (GlobalSidebar)
├─ Projects 탭 → features/projects/
│  ├─ APIListPanel
│  └─ ProjectsView
│     └─ tabs/ (Version, Manual, Spec, Builder, Runner)
└─ History 탭 → features/history/
   └─ HistoryView
```

### 3. Separation of Concerns
- **features/**: 사이드바 탭별 기능 모듈
- **components/**: 재사용 가능한 순수 UI 컴포넌트
- **lib/**: 외부 라이브러리 통합 및 래퍼
- **utils/**: 순수 함수 유틸리티

### 3. 중앙화된 관리
- **types/**: 모든 공통 타입을 한 곳에서 관리
- **config/**: 설정과 상수를 중앙 집중화
- **store/**: 전역 상태를 Zustand로 관리

### 4. Import 경로 규칙
```typescript
// ✅ Good - Alias 사용
import { Button } from '@/components/ui/button';
import { useAppStore } from '@/store/useAppStore';
import type { ApiEndpoint } from '@/types';

// ❌ Bad - 상대 경로
import { Button } from '../../../components/ui/button';
```

## 📦 새 기능 추가 방법

### 예시 1: 왼쪽 사이드바에 새 탭 추가 (Analytics)

1. **Feature 디렉토리 생성**
```bash
mkdir -p src/features/analytics/{components,hooks,types}
```

2. **컴포넌트 작성**
```typescript
// src/features/analytics/components/AnalyticsView.tsx
export function AnalyticsView() {
  return <div>Analytics Content</div>;
}
```

3. **Export 추가**
```typescript
// src/features/analytics/components/index.ts
export { AnalyticsView } from './AnalyticsView';
```

4. **App.tsx에 추가**
```typescript
import { AnalyticsView } from '@/features/analytics/components';

// GlobalSidebar에 'analytics' 버튼 추가
// activeView 타입에 'analytics' 추가
const [activeView, setActiveView] = useState<'projects' | 'history' | 'analytics'>('projects');
```

### 예시 2: Projects 안에 새 하위 탭 추가

1. **탭 컴포넌트 작성**
```typescript
// src/features/projects/components/tabs/TestTab.tsx
export function TestTab() {
  return <div>Test Tab Content</div>;
}
```

2. **tabs/index.ts에 export 추가**
```typescript
export { TestTab } from './TestTab';
```

3. **ProjectsView.tsx의 Tabs에 추가**
```typescript
import { TestTab } from './tabs';

// TabsList에 TabsTrigger 추가
// TabsContent에 TestTab 추가
```

## 🔧 유지보수 가이드

### 컴포넌트 이동
- UI 컴포넌트: `components/ui/`
- 공통 컴포넌트: `components/common/`
- 레이아웃: `components/layouts/` (GlobalSidebar만)
- **왼쪽 사이드바 탭**: `features/[tab-name]/components/`
- **Projects 하위 탭**: `features/projects/components/tabs/`

### 타입 관리
- 공통 타입: `types/index.ts`
- 기능별 타입: `features/[feature]/types/`

### 상태 관리
- 전역 상태: `store/useAppStore.ts`
- 로컬 상태: 각 컴포넌트의 useState
- 서버 상태: React Query (향후 도입 고려)

## 🚀 확장 가능성

이 구조는 다음과 같은 확장을 쉽게 지원합니다:

1. **왼쪽 사이드바에 새 탭 추가**: `features/` 폴더에 새 디렉토리
2. **Projects 안에 새 하위 탭 추가**: `features/projects/components/tabs/`에 추가
3. **공통 로직 추출**: `hooks/` 폴더에 커스텀 훅 추가
4. **타입 안정성**: `types/` 폴더에서 중앙 관리
5. **테스트 추가**: 각 feature/component 옆에 `.test.tsx` 파일
6. **스토리북 통합**: 각 컴포넌트 옆에 `.stories.tsx` 파일

## 📌 구조의 핵심 원칙

> **"왼쪽 사이드바의 각 탭 = 하나의 Feature"**

- GlobalSidebar는 탭 버튼만 담당
- 각 탭의 모든 내용은 해당 feature에 포함
- Projects 안의 하위 탭들도 `projects/components/tabs/`에 구조화

