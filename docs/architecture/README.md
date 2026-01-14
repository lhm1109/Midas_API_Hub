# API Verification System - Architecture

**Version 3.0 - 완전 동적 시스템**

---

## 📋 목차

1. [시스템 개요](#시스템-개요)
2. [전체 아키텍처](#전체-아키텍처)
3. [핵심 컴포넌트](#핵심-컴포넌트)
4. [데이터 플로우](#데이터-플로우)
5. [V3 완전 동적 시스템](#v3-완전-동적-시스템)
6. [폴더 구조](#폴더-구조)

---

## 시스템 개요

### 목적
API 문서 기반의 요청/응답 검증 및 테스트 케이스 관리 시스템

### 핵심 기능
- 📝 **JSON Schema 기반 UI 자동 생성** (Builder, Table, HTML)
- 🔄 **동적 동작 로직** (YAML 기반 Behavior Engine)
- 🧪 **테스트 케이스 관리** (저장, 로드, 실행)
- 📊 **버전 관리** (API 스펙 히스토리)
- 🔒 **분산 락** (동시 편집 방지)
- 🌐 **실시간 API 테스트**

### 기술 스택
- **Frontend**: React 18 + TypeScript + Vite
- **State**: Zustand
- **UI**: shadcn/ui + Tailwind CSS
- **Backend**: Node.js + Express
- **Database**: Supabase (PostgreSQL)
- **Realtime**: Supabase Realtime
- **Schema**: JSON Schema Draft-07
- **Config**: YAML (js-yaml)

---

## 전체 아키텍처

```
┌─────────────────────────────────────────────────────────────────┐
│                         Frontend (React)                         │
├─────────────────────────────────────────────────────────────────┤
│                                                                   │
│  ┌──────────────────┐  ┌──────────────────┐  ┌───────────────┐ │
│  │  ProjectsView    │  │  APIListPanel    │  │ SettingsModal │ │
│  │  - Version Tab   │  │  - Product Tree  │  │ - Schema Def  │ │
│  │  - Manual Tab    │  │  - Lock Status   │  │ - Assign Wrap │ │
│  │  - Spec Tab      │  │  - Search        │  │               │ │
│  │  - Builder Tab   │  └──────────────────┘  └───────────────┘ │
│  │  - Runner Tab    │                                           │
│  └──────────────────┘                                           │
│                                                                   │
│  ┌─────────────────────────── Core Layers ────────────────────┐ │
│  │                                                              │ │
│  │  🎨 Rendering Layer                                         │ │
│  │  ├─ DynamicSchemaRenderer   (YAML-based UI rendering)     │ │
│  │  ├─ DynamicTableRenderer    (Table generation)             │ │
│  │  └─ DynamicExtensionRenderer (x-* field rendering)         │ │
│  │                                                              │ │
│  │  🧠 Schema Processing Layer                                 │ │
│  │  ├─ schemaCompiler          (JSON Schema → AST)            │ │
│  │  ├─ enhancedSchemaCompiler  (x-* extensions parsing)       │ │
│  │  ├─ dynamicSchemaCompiler   (YAML-driven parsing)          │ │
│  │  └─ enhancedTableGenerator  (HTML table generation)        │ │
│  │                                                              │ │
│  │  ⚡ Behavior Engine (V3)                                    │ │
│  │  ├─ ExpressionEvaluator     (DSL execution)                │ │
│  │  ├─ ActionExecutors         (Built-in actions)             │ │
│  │  └─ BehaviorEngine          (Trigger & execution)          │ │
│  │                                                              │ │
│  │  💾 State Management                                        │ │
│  │  └─ useAppStore (Zustand)                                  │ │
│  │     ├─ settings                                             │ │
│  │     ├─ projects / endpoints                                │ │
│  │     ├─ versions / testCases                                │ │
│  │     └─ locks                                                │ │
│  │                                                              │ │
│  └──────────────────────────────────────────────────────────────┘ │
│                                                                   │
└───────────────────────────┬───────────────────────────────────────┘
                            │
                            │ HTTP / WebSocket
                            │
┌───────────────────────────▼───────────────────────────────────────┐
│                      Backend (Node.js)                            │
├───────────────────────────────────────────────────────────────────┤
│                                                                   │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐          │
│  │   Projects   │  │   Endpoints  │  │    Locks     │          │
│  │   Routes     │  │   Routes     │  │   Routes     │          │
│  └──────────────┘  └──────────────┘  └──────────────┘          │
│                                                                   │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐          │
│  │   Versions   │  │  Test Cases  │  │    Proxy     │          │
│  │   Routes     │  │   Routes     │  │   (CORS)     │          │
│  └──────────────┘  └──────────────┘  └──────────────┘          │
│                                                                   │
└───────────────────────────┬───────────────────────────────────────┘
                            │
                            │ PostgreSQL + Realtime
                            │
┌───────────────────────────▼───────────────────────────────────────┐
│                      Supabase (Database)                          │
├───────────────────────────────────────────────────────────────────┤
│                                                                   │
│  Tables:                                                          │
│  ├─ projects                                                      │
│  ├─ api_endpoints                                                 │
│  ├─ api_versions                                                  │
│  ├─ test_cases                                                    │
│  ├─ endpoint_locks     (5분 TTL, Realtime)                       │
│  └─ version_locks      (5분 TTL, Realtime)                       │
│                                                                   │
└───────────────────────────────────────────────────────────────────┘

┌───────────────────────────────────────────────────────────────────┐
│                  Configuration (YAML Files)                       │
├───────────────────────────────────────────────────────────────────┤
│                                                                   │
│  schema_definitions/                                              │
│  ├─ x-extensions-v3.yaml      🔥 V3: Behavior + Actions          │
│  ├─ enhanced/                                                     │
│  │  ├─ ui-rules.yaml          UI rendering rules                 │
│  │  ├─ builder.yaml           Builder-specific rules             │
│  │  ├─ table.yaml             Table-specific rules               │
│  │  └─ html-template.yaml     HTML generation template           │
│  └─ original/                                                     │
│     ├─ ui-rules.yaml                                              │
│     ├─ builder.yaml                                               │
│     └─ table.yaml                                                 │
│                                                                   │
└───────────────────────────────────────────────────────────────────┘
```

---

## 핵심 컴포넌트

### 1. Frontend Components

#### ProjectsView
- **역할**: 메인 워크스페이스, 탭 관리
- **탭**:
  - Version: 버전 히스토리 관리
  - Manual: 수동 문서 작성
  - Spec: 스펙 테이블 보기
  - Builder: 폼 기반 요청 빌더
  - Runner: API 테스트 실행

#### APIListPanel
- **역할**: API 엔드포인트 트리 표시
- **기능**:
  - Product > Group > Endpoint 계층 구조
  - 검색 (제품/그룹/엔드포인트명)
  - 락 상태 표시 (🟢 편집 가능 / 🔴 다른 사용자 편집 중)
  - 수동 락 상태 새로고침

#### BuilderTab
- **역할**: JSON Schema 기반 동적 폼 생성
- **기능**:
  - 스키마 기반 자동 폼 생성
  - Assign wrapper 지원 (다중 인스턴스)
  - 테스트 케이스 저장/로드
  - JSON Preview (View/Edit)
- **렌더링**: `DynamicSchemaRenderer` 사용

#### RunnerTab
- **역할**: API 요청 실행 및 응답 확인
- **기능**:
  - HTTP 메서드 선택 (GET, POST, PUT, DELETE)
  - Request/Response 표시
  - 자동 엔드포인트 상태 업데이트

---

### 2. Schema Processing Layer

#### schemaCompiler.ts
```typescript
// JSON Schema → Canonical AST
compileSchema(schema: JSONSchema): CanonicalField[]
```
- 표준 JSON Schema 파싱
- oneOf, allOf, if-then-else 처리
- 타입 정규화

#### enhancedSchemaCompiler.ts
```typescript
// Enhanced Schema → AST with x-* extensions
compileEnhancedSchema(schema: EnhancedSchema): EnhancedField[]
```
- x-ui, x-section-header, x-enum-by-type 등 파싱
- 섹션 블록 생성
- UI 메타데이터 추출

#### dynamicSchemaCompiler.ts
```typescript
// YAML-driven schema parsing
loadExtensionDefinitions(): Promise<ExtensionDefinitions>
parseSchemaExtensions(schema, definitions): ParsedExtensions
```
- x-extensions-v3.yaml 로드
- 동적으로 모든 x-* 필드 파싱
- 코드 수정 없이 새 필드 지원

---

### 3. Rendering Layer

#### DynamicSchemaRenderer
```typescript
// YAML 기반 동적 UI 렌더링
<DynamicSchemaRenderer
  schemaFields={fields}
  formData={data}
  settings={settings}
/>
```
- YAML 정의 로드 (builder.yaml, ui-rules.yaml)
- 필드 타입별 컴포넌트 매핑
- 조건부 렌더링 (visibleWhen)

#### DynamicTableRenderer
```typescript
// YAML 기반 테이블 렌더링
<DynamicTableRenderer
  fields={fields}
  tableRules={rules}
/>
```
- table.yaml 기반 렌더링
- 섹션 헤더, 그룹화
- enum 값 표시

#### DynamicExtensionRenderer
```typescript
// x-* 필드 동적 렌더링
<DynamicExtensionRenderer
  field={field}
  context="builder" | "table" | "html"
/>
```
- x-extensions-v3.yaml 기반
- 컨텍스트별 렌더링 (Builder, Table, HTML)

---

### 4. Behavior Engine (V3) 🔥

#### 아키텍처

```
┌─────────────────────────────────────────────────────────────┐
│                    Behavior Engine V3                        │
├─────────────────────────────────────────────────────────────┤
│                                                              │
│  ┌──────────────────────────────────────────────────────┐  │
│  │  1. YAML Definitions (x-extensions-v3.yaml)          │  │
│  │     - Extensions (x-enum-by-type, x-auto-calculate)  │  │
│  │     - Behaviors (trigger, when, actions)             │  │
│  │     - Expression Language (DSL)                      │  │
│  └──────────────────────────────────────────────────────┘  │
│                          ↓                                   │
│  ┌──────────────────────────────────────────────────────┐  │
│  │  2. Behavior Engine                                  │  │
│  │     - loadDefinitions()                              │  │
│  │     - executeTrigger(trigger, context)               │  │
│  │     - findAppliedExtensions()                        │  │
│  └──────────────────────────────────────────────────────┘  │
│                          ↓                                   │
│  ┌──────────────────────────────────────────────────────┐  │
│  │  3. Expression Evaluator                             │  │
│  │     - evaluate(expression, context)                  │  │
│  │     - extractVariables()                             │  │
│  │     - Support: self.value, TYPE.value, conditions    │  │
│  └──────────────────────────────────────────────────────┘  │
│                          ↓                                   │
│  ┌──────────────────────────────────────────────────────┐  │
│  │  4. Action Executors                                 │  │
│  │     ├─ update-enum      (Enum 자동 업데이트)        │  │
│  │     ├─ clear-value      (조건부 값 초기화)          │  │
│  │     ├─ validate         (검증 규칙 실행)            │  │
│  │     ├─ calculate        (수식 자동 계산)            │  │
│  │     ├─ format           (값 포맷팅)                 │  │
│  │     ├─ show-hint        (힌트 표시)                 │  │
│  │     └─ show-message     (메시지 표시)               │  │
│  └──────────────────────────────────────────────────────┘  │
│                                                              │
└─────────────────────────────────────────────────────────────┘
```

#### 실행 흐름

```
사용자 입력 (TYPE 필드 변경)
    ↓
onFieldChange() 호출
    ↓
Behavior Engine.executeTrigger('dependency-change')
    ↓
findAppliedExtensions() → ['x-enum-by-type']
    ↓
YAML에서 behaviors 로드
    ↓
when 조건 체크 (field: "TYPE", changed: true)
    ↓
actions 순차 실행:
    1. update-enum
       - Expression: "self.enumByType[TYPE.value]"
       - Evaluator가 실제 값으로 평가
       - STYPE.enum = [1, 2, 3]
    2. clear-value
       - Condition: "!self.enum.includes(self.value)"
       - 기존 값이 새 enum에 없으면 초기화
    ↓
UI 자동 업데이트 ✨
```

#### Expression Language (DSL)

```yaml
# 변수 접근
self.value                      # 현재 필드의 값
TYPE.value                      # TYPE 필드의 값
self.enumByType                 # 현재 필드의 x-enum-by-type 속성

# 조건식
TYPE.value === 'BEAM'
self.value > 0
!self.enum.includes(self.value)
TYPE.value === 'TENSTR' && self.value < 0

# 계산식
WIDTH.value * HEIGHT.value
PRICE.value + TAX.value
round(WIDTH.value * 2.54, 2)
```

---

### 5. State Management (Zustand)

#### useAppStore

```typescript
interface AppStore {
  // Settings
  settings: Settings;
  updateSettings: (settings: Settings) => void;
  
  // Projects & Endpoints
  projects: Project[];
  selectedProject: Project | null;
  selectedEndpoint: ApiEndpoint | null;
  
  // Versions
  versions: Version[];
  currentVersion: Version | null;
  saveCurrentVersion: () => Promise<void>;
  
  // Test Cases
  testCases: TestCase[];
  addTestCase: (name: string, body: any) => void;
  
  // Locks
  endpointLock: EndpointLock | null;
  versionLock: VersionLock | null;
  acquireLock: (type, id) => Promise<boolean>;
  releaseLock: (type, id) => Promise<void>;
  
  // Data for each tab
  manualData: ManualData;
  specData: SpecData;
  builderData: BuilderData;
  runnerData: RunnerData;
}
```

#### State Flow

```
User Action
    ↓
Component Event Handler
    ↓
useAppStore Action
    ↓
API Client (if needed)
    ↓
Backend API
    ↓
Supabase Database
    ↓
Response
    ↓
Store Update
    ↓
Component Re-render
```

---

### 6. Backend API Routes

#### Endpoints

```
POST   /api/projects                    # 프로젝트 생성
GET    /api/projects                    # 프로젝트 목록
PUT    /api/projects/:id                # 프로젝트 수정
DELETE /api/projects/:id                # 프로젝트 삭제

POST   /api/endpoints                   # 엔드포인트 생성
GET    /api/endpoints                   # 엔드포인트 목록
GET    /api/endpoints/:id               # 엔드포인트 조회
PUT    /api/endpoints/:id               # 엔드포인트 수정
DELETE /api/endpoints/:id               # 엔드포인트 삭제

POST   /api/versions                    # 버전 생성
GET    /api/versions/endpoint/:id       # 엔드포인트 버전 목록
PUT    /api/versions/:id                # 버전 수정

POST   /api/test-cases                  # 테스트 케이스 생성
GET    /api/test-cases/endpoint/:id     # 테스트 케이스 목록
PUT    /api/test-cases/:id              # 테스트 케이스 수정
DELETE /api/test-cases/:id              # 테스트 케이스 삭제

POST   /api/locks/endpoint/:id/lock     # 엔드포인트 락 획득
POST   /api/locks/endpoint/:id/unlock   # 엔드포인트 락 해제
GET    /api/locks/endpoint/:id/status   # 엔드포인트 락 상태

POST   /api/locks/version/:id/lock      # 버전 락 획득
POST   /api/locks/version/:id/unlock    # 버전 락 해제

POST   /api/proxy                       # CORS 프록시
```

---

### 7. Database Schema (Supabase)

```sql
-- Projects
CREATE TABLE projects (
  id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
  name TEXT NOT NULL,
  description TEXT,
  created_at TIMESTAMPTZ DEFAULT NOW()
);

-- API Endpoints
CREATE TABLE api_endpoints (
  id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
  project_id UUID REFERENCES projects(id) ON DELETE CASCADE,
  product TEXT NOT NULL,
  api_group TEXT NOT NULL,
  endpoint_name TEXT NOT NULL,
  http_method TEXT NOT NULL,
  base_url TEXT,
  path TEXT,
  status TEXT DEFAULT 'none',
  status_message TEXT,
  created_at TIMESTAMPTZ DEFAULT NOW()
);

-- API Versions
CREATE TABLE api_versions (
  id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
  endpoint_id UUID REFERENCES api_endpoints(id) ON DELETE CASCADE,
  version_number INTEGER NOT NULL,
  request_schema JSONB,
  response_schema JSONB,
  examples JSONB,
  manual_content TEXT,
  created_at TIMESTAMPTZ DEFAULT NOW(),
  created_by TEXT
);

-- Test Cases
CREATE TABLE test_cases (
  id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
  endpoint_id UUID REFERENCES api_endpoints(id) ON DELETE CASCADE,
  name TEXT NOT NULL,
  request_body JSONB NOT NULL,
  created_at TIMESTAMPTZ DEFAULT NOW()
);

-- Endpoint Locks (5분 TTL)
CREATE TABLE endpoint_locks (
  endpoint_id TEXT PRIMARY KEY,
  user_id TEXT NOT NULL,
  locked_at TIMESTAMPTZ DEFAULT NOW()
);

-- Version Locks (5분 TTL)
CREATE TABLE version_locks (
  version_id UUID PRIMARY KEY,
  user_id TEXT NOT NULL,
  locked_at TIMESTAMPTZ DEFAULT NOW()
);
```

#### Realtime Configuration

```sql
-- Realtime publication
CREATE PUBLICATION supabase_realtime FOR TABLE
  endpoint_locks,
  version_locks;
```

---

## 데이터 플로우

### 1. 엔드포인트 선택 플로우

```
User clicks endpoint in APIListPanel
    ↓
ProjectsView.handleSelectEndpoint()
    ↓
useAppStore.setSelectedEndpoint()
    ↓
acquireLock(endpoint.id)
    ↓
Backend: POST /api/locks/endpoint/:id/lock
    ↓
Supabase: INSERT INTO endpoint_locks
    ↓
Lock acquired (5분 TTL)
    ↓
Load versions: GET /api/versions/endpoint/:id
    ↓
Load test cases: GET /api/test-cases/endpoint/:id
    ↓
Store updated
    ↓
All tabs receive new data
    ↓
Tabs re-render with new endpoint data
```

### 2. Builder Tab 폼 렌더링 플로우

```
BuilderTab mounted
    ↓
Load YAML definitions
  - settings.schemaDefinition (auto/original/enhanced)
  - builder.yaml, ui-rules.yaml
    ↓
Compile schema
  - If enhanced → enhancedSchemaCompiler
  - If original → schemaCompiler
    ↓
Parse x-* extensions
  - dynamicSchemaCompiler.parseSchemaExtensions()
  - Based on x-extensions-v3.yaml
    ↓
<DynamicSchemaRenderer>
  ↓
  For each field:
    - Check field type
    - Apply YAML rendering rules
    - Render appropriate component (Input, Select, etc.)
    - Attach behavior handlers
    ↓
User interacts with field
    ↓
onFieldChange()
    ↓
Behavior Engine.executeTrigger()
  ↓
  Execute YAML-defined actions
    - update-enum
    - validate
    - calculate
    - etc.
    ↓
  Update formData
    ↓
Component re-renders with new data ✨
```

### 3. API 테스트 실행 플로우

```
User fills form in BuilderTab
    ↓
Clicks "Send" in RunnerTab
    ↓
handleSend()
    ↓
Build request body from builderData
  - Apply Assign wrapper (if enabled)
  - Convert dot notation to nested object
    ↓
Send request via apiClient.proxy()
    ↓
Backend: POST /api/proxy
  ↓
  Forward to target API
  ↓
  Receive response
    ↓
Return response to frontend
    ↓
Display response in RunnerTab
    ↓
Auto-update endpoint status
  - 2xx → status: 'success', message: 'OK'
  - 4xx/5xx → status: 'error', message: error details
    ↓
Backend: PUT /api/endpoints/:id
    ↓
Supabase: UPDATE api_endpoints
    ↓
APIListPanel updates status indicator
  🟢 Success / 🔴 Error
```

### 4. 버전 저장 플로우

```
User clicks "Save Version" in main bar
    ↓
ProjectsView.handleSaveVersion()
    ↓
Gather all tab data:
  - manualData.content
  - specData.tableSchema
  - builderData.schema
  - runnerData.examples
    ↓
useAppStore.saveCurrentVersion()
    ↓
Backend: POST /api/versions
    ↓
Supabase: INSERT INTO api_versions
    ↓
Success: toast.success('Version saved!')
Error: toast.error('Failed to save')
    ↓
Reload versions list
    ↓
VersionTab updates with new version
```

---

## V3 완전 동적 시스템

### 핵심 개념

**"코드 수정 없이 YAML만 수정하여 모든 것을 제어"**

### V2 vs V3 비교

#### V2 (부분적 동적)
```yaml
# YAML: 선언만 가능
x-enum-by-type:
  renderIn: ["builder"]
```

```typescript
// 코드: 로직 하드코딩
if (field['x-enum-by-type']) {
  const enumValues = field['x-enum-by-type'][formValues.TYPE];
  updateEnum(enumValues);  // ← 로직 변경 시 코드 수정 필요!
}
```

#### V3 (완전 동적) ✨
```yaml
# YAML: 동작 로직까지 정의!
extensions:
  - name: "x-enum-by-type"
    behaviors:
      - trigger: "dependency-change"
        when:
          field: "TYPE"
        actions:
          - type: "update-enum"
            source: "self.enumByType[TYPE.value]"  # DSL 표현식
          - type: "clear-value"
            condition: "!self.enum.includes(self.value)"
```

```typescript
// 코드: 범용 엔진만 (수정 불필요!)
await engine.executeTrigger('dependency-change', context);
```

### 새 x-* 필드 추가 방법

#### 1단계: x-extensions-v3.yaml 수정
```yaml
extensions:
  - name: "x-custom-validation"
    internalName: "customValidation"
    type: "object"
    behaviors:
      - trigger: "value-change"
        actions:
          - type: "validate"
            rules:
              - condition: "self.value < MIN.value"
                error: "Value must be greater than MIN"
```

#### 2단계: Schema에 사용
```json
{
  "MY_FIELD": {
    "type": "number",
    "x-custom-validation": {
      "minField": "MIN"
    }
  }
}
```

#### 3단계: 끝! 🎉
새로고침만 하면 자동으로 작동합니다!

### V3 시스템의 장점

✅ **확장성**: 새 필드 추가 시 코드 수정 불필요  
✅ **유지보수성**: YAML 수정만으로 동작 변경  
✅ **협업성**: 비개발자도 YAML 수정 가능  
✅ **플러그인**: 커스텀 액션 추가 지원  
✅ **테스트**: 동작 로직이 선언적이라 테스트 용이  

---

## 폴더 구조

```
c:/MIDAS/code/apiverification/
├── src/
│   ├── components/
│   │   ├── common/
│   │   │   ├── Header.tsx
│   │   │   └── SettingsModal.tsx           # 전역 설정
│   │   └── ui/                              # shadcn/ui components
│   │
│   ├── features/
│   │   └── projects/
│   │       └── components/
│   │           ├── ProjectsView.tsx         # 메인 워크스페이스
│   │           ├── APIListPanel.tsx         # API 트리 + 락 상태
│   │           ├── EndpointDialog.tsx       # 엔드포인트 생성/수정
│   │           └── tabs/
│   │               ├── VersionTab.tsx       # 버전 히스토리
│   │               ├── ManualTab.tsx        # 문서 작성
│   │               ├── SpecTab.tsx          # 스펙 테이블
│   │               ├── BuilderTab.tsx       # 폼 빌더
│   │               ├── RunnerTab.tsx        # API 테스트
│   │               └── builder/
│   │                   └── BehaviorEngineExample.tsx  # V3 데모
│   │
│   ├── lib/
│   │   ├── api/
│   │   │   └── client.ts                    # API 클라이언트
│   │   │
│   │   ├── schema/
│   │   │   ├── schemaCompiler.ts            # 기본 컴파일러
│   │   │   ├── enhancedSchemaCompiler.ts    # Enhanced 컴파일러
│   │   │   ├── dynamicSchemaCompiler.ts     # YAML-driven 컴파일러
│   │   │   ├── enhancedTableGenerator.ts    # HTML 테이블 생성
│   │   │   ├── enhancedBuilderAdapter.ts    # Builder 어댑터
│   │   │   └── uiSchemaAdapters.ts          # UI 어댑터
│   │   │
│   │   ├── rendering/
│   │   │   ├── definitionLoader.ts          # YAML 로더
│   │   │   ├── dynamicRenderer.tsx          # 동적 렌더러
│   │   │   ├── dynamicTableRenderer.tsx     # 테이블 렌더러
│   │   │   ├── dynamicExtensionRenderer.tsx # x-* 렌더러
│   │   │   └── tableToHTML.ts               # HTML 생성
│   │   │
│   │   ├── engine/
│   │   │   ├── behaviorEngine.ts            # 🔥 V3 Behavior Engine
│   │   │   └── BEHAVIOR_ENGINE_README.md    # 🔥 V3 문서
│   │   │
│   │   └── utils/
│   │       └── htmlFormatter.ts             # HTML 유틸
│   │
│   ├── store/
│   │   └── useAppStore.ts                   # Zustand 스토어
│   │
│   ├── types/
│   │   └── index.ts                         # TypeScript 타입
│   │
│   └── App.tsx                              # 메인 앱
│
├── server/
│   ├── routes/
│   │   ├── projects.js                      # 프로젝트 API
│   │   ├── endpoints.js                     # 엔드포인트 API
│   │   ├── versions.js                      # 버전 API
│   │   ├── testCases.js                     # 테스트 케이스 API
│   │   ├── locks.js                         # 락 API
│   │   └── proxy.js                         # CORS 프록시
│   │
│   ├── schema-with-locks.sql                # DB 스키마
│   └── server.js                            # Express 서버
│
├── schema_definitions/                      # 🔥 YAML 설정
│   ├── x-extensions-v3.yaml                 # 🔥 V3: Behavior 정의
│   │
│   ├── enhanced/                            # Enhanced Schema
│   │   ├── ui-rules.yaml                    # UI 렌더링 규칙
│   │   ├── builder.yaml                     # Builder 규칙
│   │   ├── table.yaml                       # Table 규칙
│   │   └── html-template.yaml               # HTML 템플릿
│   │
│   ├── original/                            # Original Schema
│   │   ├── ui-rules.yaml
│   │   ├── builder.yaml
│   │   └── table.yaml
│   │
│   └── README.md                            # 설정 가이드
│
├── api_docs_zendesk/                        # API 문서
│   ├── DB/
│   │   ├── 015_Node.html
│   │   ├── 015_Node_enhanced.json
│   │   ├── 016_Element_enhanced.json
│   │   └── ...
│   ├── make_enhanced_schema.md              # Schema 규칙
│   └── schema_to_table_algorithm.md         # Table 알고리즘
│
└── doc/
    └── architecture/                        # 아키텍처 문서
        ├── README.md                        # 👈 현재 파일
        ├── v3-behavior-engine.md            # V3 상세 문서
        └── data-flow.md                     # 데이터 플로우
```

---

## 참고 문서

- **V3 Behavior Engine**: `src/lib/engine/BEHAVIOR_ENGINE_README.md`
- **Schema 규칙**: `api_docs_zendesk/make_enhanced_schema.md`
- **YAML 설정**: `schema_definitions/README.md`
- **Table 알고리즘**: `api_docs_zendesk/schema_to_table_algorithm.md`

---

## 변경 이력

### Version 3.0 (2026-01-14)
- 🔥 **완전 동적 시스템 구현**
  - Behavior Engine V3
  - Expression Language (DSL)
  - YAML 기반 동작 로직 정의
- 동적 x-* extension 파싱
- 코드 수정 없는 확장성

### Version 2.0 (2026-01-13)
- YAML 기반 UI 렌더링 시스템
- 동적 스키마 렌더러
- Enhanced Schema 지원

### Version 1.0 (Initial)
- 기본 API 검증 시스템
- 하드코딩된 UI 로직
- 표준 JSON Schema 지원

---

**Last Updated**: 2026-01-14  
**Architecture Version**: 3.0

