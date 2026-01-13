# 🎯 구조 재설계 완료 (v2.0)

## ✅ 문제 해결

### 이전 문제점
- 왼쪽 사이드바의 Projects/History가 features로 구조화되지 않음
- Version/Manual/Spec 등이 별도 features로 분리되어 있었지만, 실제로는 Projects의 하위 탭
- MainWorkspace와 APIListPanel이 layouts에 있었지만, Projects 기능의 일부

### 새로운 구조 (v2.0)

```
src/features/
├── projects/              # 왼쪽 사이드바 - Projects 탭
│   ├── components/
│   │   ├── ProjectsView.tsx      (이전 MainWorkspace)
│   │   ├── APIListPanel.tsx
│   │   ├── index.ts
│   │   └── tabs/                 # Projects 안의 하위 탭들
│   │       ├── VersionTab.tsx
│   │       ├── ManualTab.tsx
│   │       ├── SpecTab.tsx
│   │       ├── BuilderTab.tsx
│   │       ├── RunnerTab.tsx
│   │       ├── CompareVersionsDialog.tsx
│   │       └── index.ts
│   ├── hooks/
│   └── types/
│
└── history/               # 왼쪽 사이드바 - History 탭
    ├── components/
    │   ├── HistoryView.tsx
    │   └── index.ts
    ├── hooks/
    └── types/
```

## 📊 계층 구조

```
App.tsx
└── GlobalSidebar (왼쪽 사이드바)
    ├── Projects 버튼 → features/projects/
    │   ├── APIListPanel (API 목록)
    │   └── ProjectsView (메인 워크스페이스)
    │       └── Tabs (하위 탭들)
    │           ├── VersionTab
    │           ├── ManualTab
    │           ├── SpecTab
    │           ├── BuilderTab
    │           └── RunnerTab
    │
    └── History 버튼 → features/history/
        └── HistoryView
```

## 🎯 핵심 원칙

> **"왼쪽 사이드바의 각 탭이 하나의 Feature"**

1. **GlobalSidebar**: 탭 버튼만 담당 (`components/layouts/`)
2. **각 탭의 모든 내용**: 해당 feature 폴더에 포함
3. **하위 탭들**: 상위 feature의 `components/tabs/`에 구조화

## 🔄 주요 변경사항

| 이전 위치 | 새 위치 | 이유 |
|---------|--------|------|
| `layouts/MainWorkspace.tsx` | `projects/components/ProjectsView.tsx` | Projects 탭의 메인 뷰 |
| `layouts/APIListPanel.tsx` | `projects/components/APIListPanel.tsx` | Projects 탭의 일부 |
| `common/HistoryView.tsx` | `history/components/HistoryView.tsx` | History 탭의 메인 뷰 |
| `features/versions/` | `projects/components/tabs/VersionTab.tsx` | Projects의 하위 탭 |
| `features/manual/` | `projects/components/tabs/ManualTab.tsx` | Projects의 하위 탭 |
| `features/spec/` | `projects/components/tabs/SpecTab.tsx` | Projects의 하위 탭 |
| `features/builder/` | `projects/components/tabs/BuilderTab.tsx` | Projects의 하위 탭 |
| `features/runner/` | `projects/components/tabs/RunnerTab.tsx` | Projects의 하위 탭 |

## 📝 Import 패턴

```typescript
// App.tsx에서
import { GlobalSidebar } from '@/components/layouts';
import { ProjectsView, APIListPanel } from '@/features/projects/components';
import { HistoryView } from '@/features/history/components';

// ProjectsView.tsx에서
import { VersionTab, ManualTab, SpecTab, BuilderTab, RunnerTab } from './tabs';
```

## ✨ 장점

1. **명확한 계층 구조**: 사이드바 탭 → feature → 하위 탭
2. **논리적 그룹화**: 관련된 기능들이 한 곳에
3. **쉬운 확장**: 새 사이드바 탭 추가가 명확
4. **유지보수성**: 기능별로 독립적인 모듈

---

**재구조화 완료**: 2026-01-08  
**버전**: v2.0






