# AI Workspace Architecture

> Nimbalyst Agent Mode 구조 기반 (docs/REFERENCE/nimbalyst 분석)  
> 세부 구현 전 확정된 아키텍처 문서

---

## 레이아웃 구조

```
┌─────────────────────────────────────────────────────────────────┐
│ GlobalSidebar (56px) │         AI Workspace View                │
│                      │                                          │
│  [AI] ← 첫번째탭     │  ┌──────────┬──────────────┬──────────┐ │
│  [Terminal]          │  │ Session  │   Chat Area  │ Artifact │ │
│  [Manager]           │  │ Sidebar  │              │  Panel   │ │
│  ...                 │  │          │  [Messages]  │          │ │
│                      │  │ Sess 1   │              │ models.py│ │
│                      │  │ Sess 2   │              │ test.json│ │
│                      │  │ + New    │  [Input]     │ [Monaco] │ │
│                      │  └──────────┴──────────────┴──────────┘ │
└─────────────────────────────────────────────────────────────────┘

패널 너비:
- SessionSidebar:  240px (default), 160–400px, collapsible
- ChatArea:        flex-1 (나머지 공간)
- ArtifactPanel:   320px (default), 200–600px, collapsible
```

---

## 컴포넌트 계층 구조

```
src/features/ai-chat/
  index.ts                          ← barrel export
  AIWorkspaceView.tsx               ← 루트 (GlobalSidebar에서 렌더)

  layout/
    ResizableLayout.tsx             ← 3패널 수평 분할 (Nimbalyst ResizablePanel 기반)
    ResizableDivider.tsx            ← 드래그 핸들 (커스텀, 라이브러리 없음)

  store/
    useAIWorkspaceStore.ts          ← 패널 너비/가시성/레이아웃 상태 (Zustand)
    useSessionStore.ts              ← 세션 목록 + 활성 세션 + 메시지 (Zustand)
    useArtifactStore.ts             ← 생성된 파일 목록 + 활성 파일 (Zustand)

  panels/
    SessionSidebar.tsx              ← 좌측 패널
    ChatArea.tsx                    ← 중앙 패널
    ArtifactPanel.tsx               ← 우측 패널

  components/
    SessionList.tsx                 ← 세션 목록
    SessionItem.tsx                 ← 세션 아이템 (이름, 시간, 메시지 수)
    ChatHeader.tsx                  ← 채팅 헤더 (세션명, 모델 배지)
    MessageBubble.tsx               ← 말풍선 (user/assistant)
    MarkdownRenderer.tsx            ← 마크다운+코드 렌더러 (Nimbalyst 기반)
    ChatInput.tsx                   ← 입력창 (자동 높이)
    ArtifactViewer.tsx              ← 생성 파일 뷰어 (Monaco 에디터)
    ArtifactFileTab.tsx             ← 파일 탭 (models.py, test.json...)
    EmptyChat.tsx                   ← 빈 상태 + 제안 칩
```

---

## Store 인터페이스 (타입만 확정)

### useAIWorkspaceStore
```typescript
interface AIWorkspaceState {
  // 패널 레이아웃
  sessionSidebarWidth: number        // px, default 240
  sessionSidebarCollapsed: boolean   // default false
  artifactPanelWidth: number         // px, default 320
  artifactPanelVisible: boolean      // default false (파일 생성 시 자동 open)

  // 액션
  setSessionSidebarWidth(w: number): void
  setSessionSidebarCollapsed(v: boolean): void
  setArtifactPanelWidth(w: number): void
  setArtifactPanelVisible(v: boolean): void
}
```

### useSessionStore
```typescript
interface Session {
  id: string
  title: string
  createdAt: number
  updatedAt: number
  messageCount: number
  model: string
}

interface ChatMessage {
  id: string
  sessionId: string
  role: 'user' | 'assistant'
  content: string
  isStreaming?: boolean
  createdAt: number
}

interface SessionState {
  sessions: Session[]
  activeSessionId: string | null
  messages: ChatMessage[]            // 활성 세션의 메시지들
  isLoading: boolean
  error: string | null

  // 액션
  createSession(): Promise<string>
  selectSession(id: string): Promise<void>
  deleteSession(id: string): Promise<void>
  sendMessage(content: string, systemPrompt: string): Promise<void>
  stopStreaming(): void
}
```

### useArtifactStore
```typescript
interface Artifact {
  id: string
  sessionId: string
  filename: string                   // 'models.py', 'test_body.json', ...
  language: string                   // 'python', 'json', ...
  content: string
  createdAt: number
}

interface ArtifactState {
  artifacts: Artifact[]              // 활성 세션의 artifacts
  activeArtifactId: string | null

  // 액션
  addArtifact(a: Omit<Artifact, 'id' | 'createdAt'>): void
  selectArtifact(id: string): void
  clearArtifacts(): void
}
```

---

## 리사이즈 패턴 (Nimbalyst 동일)

라이브러리 없음. 커스텀 드래그 핸들러:

```typescript
// ResizableDivider.tsx 패턴
onMouseDown: (e) => {
  startX = e.clientX
  startWidth = currentWidth
  document.addEventListener('mousemove', onMove)
  document.addEventListener('mouseup', onUp)
  document.body.style.cursor = 'ew-resize'
  document.body.style.userSelect = 'none'
}

onMove: (e) => {
  const delta = e.clientX - startX
  const newWidth = clamp(startWidth + delta, minWidth, maxWidth)
  setWidth(newWidth)  // 로컬 상태만 업데이트 (퍼포먼스)
}

onUp: () => {
  persistWidth(currentWidth)  // Zustand에 저장
  cleanup()
}
```

---

## 데이터 레이어

### Express API 엔드포인트
```
POST   /api/ai-chat/sessions           세션 생성
GET    /api/ai-chat/sessions           세션 목록
DELETE /api/ai-chat/sessions/:id       세션 삭제
PATCH  /api/ai-chat/sessions/:id       세션 수정 (제목 등)

GET    /api/ai-chat/sessions/:id/messages    메시지 목록
POST   /api/ai-chat/stream                   스트리밍 응답 (기존)
```

### Supabase 테이블 (마이그레이션 필요)
```sql
CREATE TABLE ai_sessions (
  id TEXT PRIMARY KEY,
  title TEXT NOT NULL,
  model TEXT DEFAULT 'gpt-4o',
  created_at TIMESTAMPTZ DEFAULT NOW(),
  updated_at TIMESTAMPTZ DEFAULT NOW()
);

CREATE TABLE ai_messages (
  id TEXT PRIMARY KEY,
  session_id TEXT REFERENCES ai_sessions(id) ON DELETE CASCADE,
  role TEXT NOT NULL,      -- 'user' | 'assistant'
  content TEXT NOT NULL,
  created_at TIMESTAMPTZ DEFAULT NOW()
);

CREATE TABLE ai_artifacts (
  id TEXT PRIMARY KEY,
  session_id TEXT REFERENCES ai_sessions(id) ON DELETE CASCADE,
  filename TEXT NOT NULL,
  language TEXT,
  content TEXT NOT NULL,
  created_at TIMESTAMPTZ DEFAULT NOW()
);
```

---

## 구현 단계

### Step 1: 스켈레톤 (지금)
- [ ] 폴더 구조 생성
- [ ] 빈 컴포넌트 파일들 (placeholder UI만)
- [ ] Store 타입 + 기본 상태
- [ ] ResizableLayout 동작 확인

### Step 2: 데이터 레이어
- [ ] Supabase 마이그레이션 SQL
- [ ] Express 세션 API
- [ ] Store 비동기 액션 연결

### Step 3: 세션 사이드바
- [ ] SessionList, SessionItem
- [ ] 세션 생성/삭제/선택
- [ ] 세션 제목 자동 생성 (첫 메시지 기반)

### Step 4: 채팅 영역
- [ ] MessageBubble + MarkdownRenderer
- [ ] ChatInput (스트리밍)
- [ ] 세션별 히스토리 복원

### Step 5: Artifact 패널
- [ ] ArtifactViewer (Monaco)
- [ ] 파일 탭 전환
- [ ] AI 응답에서 코드 블록 자동 추출 → Artifact 저장
- [ ] 패널 자동 열기 (파일 생성 시)

---

## 레퍼런스 파일

| 패턴 | Nimbalyst 소스 |
|------|---------------|
| 3패널 레이아웃 | `AgentMode/AgentMode.tsx` |
| ResizablePanel | `AgenticCoding/ResizablePanel.tsx` |
| 세션 목록 | `AgenticCoding/SessionHistory.tsx` |
| 세션 아이템 | `AgenticCoding/SessionListItem.tsx` |
| 채팅 패널 | `ChatSidebar/ChatSidebar.tsx` |
| 파일 사이드바 | `AgentMode/FilesEditedSidebar.tsx` |
| 마크다운 렌더러 | `runtime/src/ui/AgentTranscript/components/MarkdownRenderer.tsx` |
