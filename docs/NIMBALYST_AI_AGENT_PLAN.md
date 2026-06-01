# Nimbalyst AI Agent Integration Plan

> **레퍼런스**: `docs/REFERENCE/nimbalyst/` (MIT 라이선스 — 복사/수정 자유)  
> **전략**: Nimbalyst Chat UI 소스를 이 앱에 이식. Nimbalyst 앱 설치 불필요. FastAPI/pydantic-ai 불필요.

---

## 개요

Nimbalyst의 AI Chat UI 컴포넌트를 이 앱(Electron + React)에 이식하여, 왼쪽 사이드바 첫 번째 탭에 AI Chat 패널을 추가한다.  
Supabase spec_data를 컨텍스트로 주입하여 API 스키마 분석, Pydantic 모델 생성, 경계조건 생성 등을 자연어로 요청할 수 있다.

---

## UI 배치

```
왼쪽 사이드바 (GlobalSidebar.tsx)
  [AI]        ← 새로 추가 (첫 번째 탭, Bot 아이콘)
  ─────────
  [Terminal]
  [Manager]
  [Projects]
  ...
```

---

## Nimbalyst Chat UI 이식 계획

### 라이선스
MIT — 자유롭게 복사/수정/재배포 가능.

### 호환성
- Nimbalyst: Tailwind CSS, 커스텀 컴포넌트 (shadcn/Radix 없음)
- 이 앱: Tailwind CSS → **스타일 호환**
- Electron IPC → Express REST API로 교체하면 동작

### 이식 대상 컴포넌트

| 컴포넌트 | 원본 경로 | 재사용 % | 변경 내용 |
|----------|-----------|---------|-----------|
| `MarkdownRenderer` | `runtime/src/ui/AgentTranscript/components/MarkdownRenderer.tsx` | 90% | 거의 그대로 |
| `MessageSegment` | `...AgentTranscript/components/MessageSegment.tsx` | 85% | 첨부파일 IPC 제거 |
| `AIInput` | `electron/src/renderer/components/UnifiedAI/AIInput.tsx` | 70% | 음성/파일멘션 제거, 단순화 |
| `ChatSidebar` | `...components/ChatSidebar/ChatSidebar.tsx` | 65% | Electron IPC → REST |
| `SessionHistory` | `...AgenticCoding/SessionHistory.tsx` | 40% | worktree 로직 제거, 재작성 |

### 핵심 변환 패턴

```typescript
// 원본: Electron IPC
window.electronAPI.invoke('sessions:list', workspacePath)
window.electronAPI.invoke('sessions:create', { ... })

// 이식 후: Express REST
fetch('/api/ai/sessions')
fetch('/api/ai/sessions', { method: 'POST', body: JSON.stringify({ ... }) })
```

### 추가 패키지

```bash
npm install react-markdown remark-gfm react-syntax-highlighter virtua jotai
npm install -D @types/react-syntax-highlighter
```

### 구현 순서

**Phase 1: UI 기반 (이식)**
- [ ] `GlobalSidebar`에 AI 탭 추가 (`Bot` 아이콘, 첫 번째)
- [ ] `src/features/ai-chat/` 디렉토리 생성
- [ ] `MarkdownRenderer` 이식 (90% 그대로)
- [ ] `MessageSegment` 이식 (말풍선)
- [ ] `AIInput` 단순화 이식 (텍스트 입력 + 전송)
- [ ] `ChatPanel` 조립 (세션 없는 단순 버전)

**Phase 2: 백엔드 연결**
- [ ] `server/routes/ai-chat.js` — OpenAI 스트리밍 프록시
- [ ] `server/routes/ai-sessions.js` — 세션 CRUD (Supabase 저장)
- [ ] Supabase `ai_sessions`, `ai_messages` 테이블 추가

**Phase 3: 컨텍스트 주입**
- [ ] 현재 열린 endpoint의 spec_data를 시스템 프롬프트에 자동 주입
- [ ] "이 API" 멘션 시 해당 스키마 포함
- [ ] 경계조건 생성 / Pydantic 모델 생성 슬래시 커맨드

**Phase 4: 세션 관리**
- [ ] `SessionHistory` 단순화 버전 (worktree 없이)
- [ ] 대화 히스토리 Supabase 저장/불러오기

---

## Nimbalyst Extension SDK AI 분석 결과

레퍼런스 분석 파일:
- `packages/extension-sdk/src/types/extension.ts` (lines 815–965) — AI API 전체 시그니처
- `packages/extensions/automations/src/index.tsx` — 실제 사용 패턴
- `packages/runtime/src/extensions/ExtensionAIToolsBridge.ts` — 툴 브릿지
- `docs/EXTENSION_ARCHITECTURE.md` — EditorHost 계약

### Extension SDK가 제공하는 AI API

```typescript
context.services.ai = {
  // 세션 기반 프롬프트 (히스토리 자동 저장)
  sendPrompt(options: {
    prompt: string;
    sessionName?: string;
    provider?: 'claude-code' | 'claude' | 'openai';
    model?: string;
  }): Promise<{ sessionId: string; response: string }>;

  // 비상태 단발 완성 (구조화 출력 지원)
  chatCompletion(options: ChatCompletionOptions): Promise<ChatCompletionResult>;

  // 스트리밍
  chatCompletionStream(options: ChatCompletionStreamOptions): Promise<ChatCompletionStreamHandle>;

  // 커스텀 툴 등록 (Nimbalyst Chat에서 자동 사용 가능)
  registerTool(tool: ExtensionAITool): Disposable;

  // 글로벌 컨텍스트 주입
  registerContextProvider(provider: ExtensionContextProvider): Disposable;

  listModels(): Promise<ExtensionAIModel[]>;
}
```

### 구조화 출력 (JSON Schema 강제)

```typescript
// GPT-4o의 structured output과 동일
chatCompletion({
  responseFormat: {
    type: 'json_schema',
    schema: { ... },
    strict: true   // 응답이 반드시 스키마를 따름
  }
})
```

---

## pydantic-ai vs Nimbalyst Extension SDK 비교

| 항목 | pydantic-ai | Nimbalyst Extension SDK |
|------|-------------|------------------------|
| LLM 호출 | ✅ | ✅ (Claude/OpenAI/LM Studio) |
| 스트리밍 | ✅ | ✅ `chatCompletionStream()` |
| 구조화 JSON 출력 | ✅ | ✅ `json_schema` strict |
| 자동 툴 오케스트레이션 루프 | ✅ 자동 | ⚠️ 수동 구현 필요 |
| 결과를 파일/Artifact로 저장 | ❌ 별도 처리 | ✅ `filesystem.writeFile()` → 자동 Artifact |
| Workspace 세션 연동 | ❌ | ✅ Session Manager 자동 연동 |
| 멀티 프로바이더 전환 | 코드 수정 필요 | ✅ UI에서 전환 |
| 별도 서버 필요 | ✅ FastAPI 필요 | ❌ 불필요 |

**결론**: 단순 `스키마 → 모델 생성` 파이프라인은 Nimbalyst SDK만으로 충분.  
자동 멀티스텝 루프가 필요하면 Extension handler 안에서 수동 구현.

---

## 수정된 아키텍처

```
[Nimbalyst Chat]
    ↓ "이 API 스키마로 Pydantic 모델 만들어줘"
    ↓ generate_pydantic_model tool 자동 호출 (Extension 등록)

[Nimbalyst Extension Backend] (utility-process)
    ↓ 1. Supabase REST API → endpoint + spec_data 조회
    ↓ 2. context.services.ai.chatCompletion() 호출
           model: 'openai:gpt-4o'
           responseFormat: json_schema (strict)
    ↓ 3. [선택] Python 검증: 기존 Express /pydantic/execute 재사용
    ↓ 4. context.services.filesystem.writeFile() × 3

[Nimbalyst Workspace]
    models.py          ← Artifact 자동 생성
    test_body.json     ← Artifact 자동 생성
    schema.json        ← Artifact 자동 생성
```

**제거된 레이어**:
- ~~FastAPI 서버~~ → Nimbalyst Extension utility-process로 대체
- ~~pydantic-ai~~ → `chatCompletion()` + 수동 검증 루프로 대체

---

## 구현 단위

### 패키지 구조

```
nimbalyst-api-ext/               ← 새로 만들 Nimbalyst Extension
  package.json                   (depends on @nimbalyst/extension-sdk)
  manifest.ts                    permissions: ai, filesystem, configuration
  src/
    index.ts                     activate() + aiTools 등록
    tools/
      generate-model.ts          핵심: schema → models.py
      fetch-schema.ts            Supabase에서 spec_data 조회
      validate-model.ts          Express /pydantic/execute 호출
    panel/
      ApiAgentPanel.tsx          endpoint 선택 + 실행 UI (선택)
    supabase.ts                  Supabase REST 클라이언트
```

---

## AI Tool 코드 패턴 (레퍼런스 기반)

```typescript
// src/tools/generate-model.ts
import type { ExtensionAITool, AIToolContext } from '@nimbalyst/extension-sdk';

export const generatePydanticModelTool: ExtensionAITool = {
  name: 'generate_pydantic_model',
  description: 'API 스키마에서 Pydantic v2 모델(models.py), test_body.json, schema.json 생성',
  scope: 'global',
  inputSchema: {
    type: 'object',
    properties: {
      endpoint_id: {
        type: 'string',
        description: 'Supabase endpoint ID (예: civil-nxgen_get_building_info)',
      },
      schema: {
        type: 'object',
        description: '직접 스키마 전달 시 (endpoint_id 없을 때)',
      },
    },
  },
  handler: async (args, toolCtx: AIToolContext) => {
    const { ai, filesystem } = toolCtx.extensionContext.services;
    if (!ai || !filesystem) return { success: false, error: 'Services unavailable' };

    // 1. Supabase에서 스키마 조회
    const schema = args.schema ?? (await fetchSchemaFromSupabase(args.endpoint_id as string));
    if (!schema) return { success: false, error: `스키마 없음: ${args.endpoint_id}` };

    // 2. GPT-4o로 모델 생성 (구조화 출력)
    const result = await ai.chatCompletion({
      model: 'openai:gpt-4o',
      systemPrompt: PYDANTIC_SYSTEM_PROMPT,
      messages: [{ role: 'user', content: `스키마:\n${JSON.stringify(schema, null, 2)}` }],
      responseFormat: {
        type: 'json_schema',
        strict: true,
        schema: {
          type: 'object',
          properties: {
            models_py: { type: 'string' },
            test_body: { type: 'object' },
            schema_json: { type: 'object' },
          },
          required: ['models_py', 'test_body', 'schema_json'],
        },
      },
      maxTokens: 4096,
    });

    const output = JSON.parse(result.content);

    // 3. [선택] 검증: Express /pydantic/execute 호출
    const validation = await validatePythonCode(output.models_py);
    if (!validation.success) {
      // 재시도 1회 (에러 메시지 피드백)
      const retry = await ai.chatCompletion({
        model: 'openai:gpt-4o',
        messages: [
          { role: 'user', content: `스키마:\n${JSON.stringify(schema, null, 2)}` },
          { role: 'assistant', content: result.content },
          { role: 'user', content: `다음 오류를 수정해줘:\n${validation.error}` },
        ],
        responseFormat: result /* same format */,
      });
      Object.assign(output, JSON.parse(retry.content));
    }

    // 4. Artifact 저장 (Nimbalyst Workspace에 파일로 표시됨)
    const baseName = (args.endpoint_id as string) ?? 'generated';
    await filesystem.writeFile(`generated_pydantic/${baseName}/models.py`, output.models_py);
    await filesystem.writeFile(`generated_pydantic/${baseName}/test_body.json`, JSON.stringify(output.test_body, null, 2));
    await filesystem.writeFile(`generated_pydantic/${baseName}/schema.json`, JSON.stringify(output.schema_json, null, 2));

    return {
      success: true,
      message: `3개 파일 생성 완료: ${baseName}`,
      data: { files: ['models.py', 'test_body.json', 'schema.json'] },
    };
  },
};

const PYDANTIC_SYSTEM_PROMPT = `
너는 API 스키마를 Pydantic v2 모델로 변환하는 Python 전문가다.
- BaseModel 사용, Field(alias=...) 또는 serialization_alias 사용
- 필수/선택 필드 명확히 구분
- 모든 응답은 JSON 형식으로만 반환 (models_py, test_body, schema_json)
`.trim();
```

---

## Supabase 데이터 활용 (변경 없음)

| 테이블 | 활용 |
|--------|------|
| `endpoints` | endpoint 목록 조회 |
| `spec_data.json_schema_enhanced` | Agent 주 입력 스키마 |
| `spec_data.pydantic_code` | 기존 생성 코드 캐시 참고 |
| `manual_data` | 추가 컨텍스트 (필요 시) |

---

## 검증 (기존 Express 재사용)

pydantic-ai 없이도 검증은 **기존 Express 서버**로 가능:

```typescript
// src/tools/validate-model.ts
async function validatePythonCode(code: string) {
  const res = await fetch('http://localhost:3001/api/pydantic/execute', {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({ code, endpoint: { id: 'validation_test' } }),
  });
  const data = await res.json();
  return { success: !data.stderr, error: data.stderr };
}
```

---

## 인증: OpenAI

Nimbalyst Extension에서 `model: 'openai:gpt-4o'`를 쓰면 Nimbalyst 앱의 OpenAI API 키 설정을 그대로 사용한다. 별도 키 관리 불필요.

```
Nimbalyst 앱 Settings → AI Providers → OpenAI API Key 입력
  → Extension이 자동으로 해당 키로 호출
```

**Azure OpenAI AAD (OAuth)**: Nimbalyst provider를 `azure-openai`로 설정하면 Azure AD 토큰 인증 사용 가능. 코드 변경 없이 `model: 'azure-openai:gpt-4o'`로만 전환.

---

## 구현 순서

### Phase 1: Extension 기본 골격
- [ ] `nimbalyst-api-ext/` 패키지 초기화
- [ ] `@nimbalyst/extension-sdk` 설치
- [ ] manifest 작성 (permissions: ai, filesystem)
- [ ] `activate()` + `aiTools` export 기본 구조

### Phase 2: Supabase 연결
- [ ] `src/supabase.ts` — utility-process에서 SUPABASE_URL + SERVICE_KEY 사용
- [ ] `fetch-schema.ts` — endpoint_id → spec_data 조회

### Phase 3: 핵심 AI Tool
- [ ] `generate-model.ts` — chatCompletion + json_schema strict
- [ ] `validate-model.ts` — Express /pydantic/execute 재사용
- [ ] 재시도 루프 (최대 2회)

### Phase 4: Artifact 저장
- [ ] `filesystem.writeFile()` × 3 (models.py, test_body.json, schema.json)
- [ ] Nimbalyst Workspace에서 열리는지 검증

### Phase 5: 고도화 (선택)
- [ ] `ApiAgentPanel.tsx` — endpoint 검색 + 배치 생성 UI
- [ ] 스트리밍 응답 (`chatCompletionStream`)
- [ ] Azure OpenAI AAD 전환

---

---

## AI 판단이 필요한 시나리오별 구현 전략

### 시나리오 A: "경계조건 생성" — AI가 무엇을 만들지 판단

**판단이 일어나는 위치**: LLM 응답 내부 (툴 호출 없음)  
**pydantic-ai 필요**: ❌

스키마를 받은 LLM이 응답 안에서 스스로 추론한다:
- integer 필드 → min/max/zero/negative 케이스
- enum 필드 → 각 값 + 잘못된 값
- required 필드 → null/missing 케이스
- 중첩 객체 → depth별 케이스

`chatCompletion()` + `json_schema strict`로 구조화된 테스트케이스를 반환받으면 충분하다.

```typescript
// 응답 포맷 예시
responseFormat: {
  type: 'json_schema',
  strict: true,
  schema: {
    type: 'object',
    properties: {
      boundary_conditions: {
        type: 'array',
        items: {
          type: 'object',
          properties: {
            field: { type: 'string' },
            condition_type: { type: 'string' },  // 'min', 'max', 'null', 'invalid_enum', ...
            test_value: {},
            expected_behavior: { type: 'string' },
          },
          required: ['field', 'condition_type', 'test_value'],
        },
      },
    },
    required: ['boundary_conditions'],
  },
}
```

---

### 시나리오 B: "376개 중 필요한 것만 골라서 전달" — 컨텍스트 선택 문제

전체 spec_data를 LLM에 던지면 컨텍스트 초과 + 노이즈. 세 가지 방법:

#### 방법 1: 2단계 LLM 호출 (즉시 구현 가능)
```
1차 chatCompletion:
  입력: endpoint 이름 + 그룹 목록 (텍스트, 가벼움)
  프롬프트: "사용자가 '하중 관련 경계조건 만들어줘' 라고 함. 관련 endpoint_id를 골라줘"
  출력: string[] (관련 endpoint_id 목록)

→ Supabase에서 해당 endpoint들만 spec_data fetch

2차 chatCompletion:
  입력: 선별된 스키마만 전달
  출력: 경계조건 / 모델 / 테스트케이스
```

#### 방법 2: Supabase pgvector 벡터 검색 (정확도 최고)
```
사전 작업: endpoint 이름+설명을 OpenAI 임베딩 → Supabase pgvector 저장
실시간:    사용자 입력을 임베딩 → 코사인 유사도 검색 → 상위 N개 endpoint 반환
```
> Supabase pgvector 확장 활성화 필요. 가장 정확하나 사전 세팅 필요.

#### 방법 3: Extension Panel UI에서 사용자가 선택
endpoint 검색/선택 UI → 선택된 것만 AI에 전달. 가장 확실하고 오해 없음.

**권장**: Phase 1은 방법 3(UI 선택), Phase 2는 방법 1(2단계 호출) 순으로 구현.

---

### 시나리오 C: 열린 탐색 — pydantic-ai가 필요한 시점

```
사용자: "이 API 시스템에서 문제가 될 수 있는 부분 찾아줘"

pydantic-ai agent가 스스로 결정:
  → fetch_endpoint_list() 호출
  → 결과 분석 후 "오류율 높은 패턴 있음" → get_test_cases() 호출
  → 결과 분석 후 "이 케이스 누락" → generate_missing_cases() 호출
  → ...  (중간 결과에 따라 다음 툴이 달라짐)
```

이처럼 **중간 결과에 따라 다음 단계가 바뀌는 적응형 탐색**이 필요할 때 pydantic-ai를 추가한다.

---

### 요약 판단표

| 사용자 요청 | 구현 방식 | pydantic-ai |
|-------------|-----------|-------------|
| "경계조건 생성해줘" | chatCompletion + json_schema | ❌ 불필요 |
| "이 API 모델 만들어줘" | chatCompletion + json_schema | ❌ 불필요 |
| "관련 API 찾아서 모델 만들어줘" | 2단계 호출 or pgvector | ❌ 불필요 |
| "이 시스템 문제점 찾아줘" | agentic loop 필요 | ✅ 이때 추가 |
| "최적화 방안 제안해줘" | agentic loop 필요 | ✅ 이때 추가 |

---

## 레퍼런스 파일 (분석 완료)

| 파일 | 내용 |
|------|------|
| `extension-sdk/src/types/extension.ts:815` | AI API 전체 타입 정의 |
| `extensions/automations/src/index.tsx` | 실제 sendPrompt 사용 패턴 |
| `runtime/src/extensions/ExtensionAIToolsBridge.ts` | 툴 브릿지 구현 |
| `docs/EXTENSION_ARCHITECTURE.md` | EditorHost 계약, 에디터 타입 |
| `docs/TRANSCRIPT_ARCHITECTURE.md` | 세션/메시지 아키텍처 |
| `CLAUDE.md` | Extension 개발 규칙 |
