# PipelineXLab AI 시스템 기획서

## 1. 개요

PipelineXLab Flow Editor 내에 AI 기능을 노드 형태로 제공합니다.
사용자가 직접 AI를 조작하고 워크플로우 자동화에 활용할 수 있는 환경을 구축합니다.

---

## 2. 핵심 설계 원칙

### 🔴 가장 중요한 원칙: 책임 경계 분리

> **"AI Agent는 값을 만든다. AI Orchestrator는 구조를 바꾼다."**
>
> 이 경계가 무너지면 시스템이 반드시 터진다.

**왜 이 분리가 필수인가?**

| 구분 | AI Agent / GraphRAG | AI Orchestrator |
|------|---------------------|-----------------|
| **역할** | 값 생성 (계산기/검색기) | 구조 변경 (설계자) |
| **입력** | 데이터 (숫자, 텍스트, JSON, 질문) | Flow 전체 상태 |
| **출력** | 값 (코드, 텍스트, JSON, context_bundle) | FlowPatch (구조 변경 명세) |
| **실행 시점** | Flow 실행 중 | Flow 설계 단계 |
| **컨텍스트** | 연결된 input만 봄 (국소적) | Flow 전체를 봄 (전역적) |
| **실패 비용** | 낮음 (다시 실행하면 됨) | 매우 높음 (Flow가 망가짐) |

> **GraphRAG Query는 Agent 계열이다.** (값 = 컨텍스트 생성)
> 따라서 Flow 구조 변경 권한이 없고, 결과는 항상 `context_bundle`로 노드 출력/아티팩트로 저장된다.

### Agent가 Node를 직접 만들면 안 되는 5가지 이유

**1️⃣ 책임 추적 불가**
- "누가 이 연결을 만들었지?" 에 답이 안 나옴
- Undo / Preview / Audit 모두 애매해짐

**2️⃣ 국소 컨텍스트로 전역 판단**
- Agent는 자신에게 연결된 input만 봄
- 이 상태로 Node를 만들면: 중복 생성, 역할 겹침, 실행 순서 꼬임

**3️⃣ 트랜잭션 불가능**
- "미리보기" 전에 이미 Flow가 변함
- 중간 실패 시 롤백 경로가 없음

**4️⃣ 권한 모델 붕괴**
- "이 Agent는 노드 생성 금지" 같은 정책 불가
- 권한 체크 포인트가 분산됨

**5️⃣ 실패 비용 차이** (가장 중요)
- Agent 실패: 결과가 틀림 → 다시 실행
- Orchestrator 실패: Flow가 망가짐 → 프로젝트 복구 필요

### 올바른 모델: FlowPatch

**❌ 위험한 모델**
```
AI Agent → Node 직접 생성 → Flow에 바로 반영
```

**✅ 안전한 모델**
```
AI Agent → 값 출력 (코드, 텍스트, JSON)
          ↓
AI Orchestrator → FlowPatch 생성 (preview only)
          ↓
사용자 승인 → 실제 Node/Edge 반영
```

**FlowPatch 예시**:
```json
{
  "action": "create_node",
  "node_type": "earthquake_load",
  "settings": { "code": "KDS_41_17" },
  "connect_to": ["structural_model"],
  "position": { "x": 400, "y": 200 }
}
```

### AI Agent를 노드로 만드는 이유

> "Node를 못 만들 거면, 왜 Agent를 노드로 만드냐?"

**Agent가 노드인 이유는 "구조 변경" 때문이 아니다.**
**"Flow 안에서 실행·조합·재사용되게 하기 위해서"다.**

| 노드가 아닐 때 | 노드일 때 |
|--------------|---------|
| 입력→출력이 그래프에 안 남음 | 데이터 흐름에 참여 |
| 실행 순서 제어 불가 | 다른 노드와 연결 가능 |
| 재실행/캐시 불가 | 입력이 같으면 재현 가능 |
| 결과가 자산으로 안 남음 | Flow 저장 시 함께 저장 |

**비유**: Python 노드는 Node를 만들 수 있냐? ❌
하지만 노드로 존재한다. 왜? **값을 계산해서 다음 단계로 넘기니까.**

AI Agent도 똑같다. 단지 계산 방식이 LLM일 뿐.

**예시: AI Agent 데이터 흐름**
```
[Number] ──┐
           ├──▶ [AI Agent] ──▶ [Python] ──▶ [Report]
[Table] ───┘
           │         │            │            │
         입력값    코드 생성    코드 실행    결과 표시
                 (값 출력)   (AI가 만든 코드)
```
- AI Agent는 Number와 Table의 값을 받아서 **코드를 생성**
- 생성된 코드는 Python 노드로 전달되어 **실행**
- **Node를 만드는 것이 아니라, 값(코드)을 다음 노드로 넘기는 것**

---

## 3. AI 노드 종류

### 3.1 AI Agent 노드 (개별 작업용)

**역할**: 하나의 AI 작업 수행 (코드 생성, 텍스트 생성, 데이터 변환 등)

**노드 외형**:
```
┌─────────────────────────────────────┐
│ 🤖 AI Agent                         │
├─────────────────────────────────────┤
│ ● input_context                     │
│ ● input_prompt                      │
│─────────────────────────────────────│
│  Model: GPT-4                       │
│  Output: Code                       │
│  [⚙️ Settings]                      │
│─────────────────────────────────────│
│                      output_result ●│
└─────────────────────────────────────┘
```

**모달 설정 UI**:
```
┌─────────────── AI Agent Settings ───────────────┐
│                                                  │
│  Model:      [GPT-4 ▼]                          │
│  Output:     ○ Text  ● Code  ○ JSON (Validated) │
│  Temperature: ═══════●═══ 0.7                   │
│                                                  │
│  System Prompt:                                  │
│  ┌──────────────────────────────────────────┐   │
│  │ 구조공학 전문가로서 답변하세요.         │   │
│  │ 단위는 SI 단위를 사용합니다.            │   │
│  └──────────────────────────────────────────┘   │
│                                                  │
│  Input Context:                                  │
│  ☑ input_0 (number)                             │
│  ☑ input_1 (text)                               │
│  ☐ input_2 (json)                               │
│                                                  │
│              [Cancel]  [Save]                    │
└──────────────────────────────────────────────────┘
```

**설정 옵션 상세**:
| 옵션 | 설명 | 기본값 |
|------|------|--------|
| Model | 사용할 LLM 모델 | GPT-4 |
| Output | 출력 형식 (Text/Code/JSON) | Text |
| Temperature | 창의성 조절 (0=정확, 1=창의적) | 0.7 |
| System Prompt | AI 역할 및 규칙 정의 | 빈 값 |
| Input Context | 컨텍스트로 전달할 input 선택 | 전체 선택 |

**입출력 정의**:
| 포트 | 타입 | 설명 |
|------|------|------|
| input_context | any | 다른 노드의 출력값 (컨텍스트) |
| input_prompt | text | 사용자 프롬프트 |
| output_result | 설정에 따라 다름 | AI 생성 결과 |

---

### 3.2 AI Orchestrator 노드 (워크플로우 제어용)

**역할**: 현재 워크플로우 전체를 분석하고 노드/연결을 조작

**노드 외형**:
```
┌─────────────────────────────────────┐
│ 🎭 AI Orchestrator                  │
├─────────────────────────────────────┤
│ ● input_command                     │
│─────────────────────────────────────│
│  Scope: Entire Flow                 │
│  Mode: Preview First                │
│  [⚙️ Settings] [▶️ Execute]         │
│─────────────────────────────────────│
│                       output_log ●  │
│                    output_status ●  │
└─────────────────────────────────────┘
```

**모달 설정 UI**:
```
┌────────────── AI Orchestrator Settings ──────────────┐
│                                                       │
│  Control Scope:                                       │
│  ● Entire Flow (현재 워크플로우 전체)                 │
│  ○ Selected Nodes Only (선택된 노드만)                │
│  ○ Connected Nodes (연결된 노드 그래프)               │
│                                                       │
│  Allowed Actions:                                     │
│  ☑ Create nodes (노드 생성)                           │
│  ☑ Modify node settings (노드 설정 수정)              │
│  ☑ Create/Delete connections (연결 조작)              │
│  ☐ Delete nodes (노드 삭제 - 위험)                    │
│                                                       │
│  Execution Mode:                                      │
│  ○ Auto-execute (자동 적용)                           │
│  ● Preview first (미리보기 후 승인)                   │
│                                                       │
│  Safety Options:                                      │
│  ☑ Backup before changes (변경 전 백업)               │
│  ☑ Undo available (실행취소 가능)                     │
│                                                       │
│                [Cancel]  [Save]                       │
└───────────────────────────────────────────────────────┘
```

**설정 옵션 상세**:
| 옵션 | 설명 | 기본값 |
|------|------|--------|
| Control Scope | 제어 범위 | Entire Flow |
| Create nodes | 노드 생성 허용 | ☑ |
| Modify settings | 노드 설정 수정 허용 | ☑ |
| Connections | 연결 생성/삭제 허용 | ☑ |
| Delete nodes | 노드 삭제 허용 (위험) | ☐ |
| Execution Mode | 자동 적용 또는 미리보기 | Preview |
| Backup | 변경 전 자동 백업 | ☑ |

**입출력 정의**:
| 포트 | 타입 | 설명 |
|------|------|------|
| input_command | text | 사용자 명령 프롬프트 |
| output_log | text | 실행 로그 |
| output_status | json | 변경 사항 요약 |

> ⚠️ **중요**: AI Orchestrator는 Flow 실행(run)과 분리된 **설계 트랜잭션**으로 동작하며,
> 실행 그래프에는 직접 포함되지 않는다.

**실행 흐름**:
1. 사용자가 프롬프트 입력 (예: "지진하중 분석 추가")
2. Orchestrator가 LangGraph로 계획 수립
3. Preview 모드면 미리보기 표시
4. 사용자 승인 시 PydanticAI 에이전트들이 노드 생성/연결

---

### 3.3 AI Budget Guard 노드 (비용/실행 통제용)

**역할**: AI 실행 비용, 토큰 사용량, 접근 권한을 제어하는 제동장치

**왜 필요한가**: AI Orchestrator가 강력한 만큼, 엔터프라이즈 환경에서는 통제 장치가 필수

**노드 외형**:
```
┌─────────────────────────────────────┐
│ 🛡️ AI Budget Guard                 │
├─────────────────────────────────────┤
│  Token Limit: 100,000 / month       │
│  Execution Limit: 500 / day         │
│  Current Usage: 23,450 tokens       │
│  [⚙️ Settings]                      │
├─────────────────────────────────────┤
│  Status: ● Active                   │
│                       output_log ●  │
└─────────────────────────────────────┘
```

**모달 설정 UI**:
```
┌────────────── AI Budget Guard Settings ──────────────┐
│                                                       │
│  Token Limits:                                        │
│  ├─ Monthly limit:     [100,000    ] tokens          │
│  ├─ Daily limit:       [10,000     ] tokens          │
│  └─ Per-execution:     [2,000      ] tokens          │
│                                                       │
│  Execution Limits:                                    │
│  ├─ Daily executions:  [500        ] times           │
│  └─ Concurrent:        [5          ] parallel        │
│                                                       │
│  Cost Limits:                                         │
│  ├─ Monthly budget:    [$50        ] USD             │
│  └─ Alert threshold:   [80         ] %               │
│                                                       │
│  Access Control:                                      │
│  ☑ Allow AI Agent nodes                              │
│  ☑ Allow AI Orchestrator                             │
│  ☐ Allow node deletion (위험)                         │
│  ☐ Allow external API calls                           │
│                                                       │
│  Allowed Node Types:                                  │
│  ☑ Python    ☑ Number    ☑ Display                  │
│  ☑ Formula   ☐ API       ☐ MIDAS                    │
│                                                       │
│  On Limit Reached:                                    │
│  ● Block execution                                    │
│  ○ Warn and continue                                  │
│  ○ Notify admin only                                  │
│                                                       │
│                [Cancel]  [Save]                       │
└───────────────────────────────────────────────────────┘
```

**설정 옵션 상세**:
| 옵션 | 설명 | 기본값 |
|------|------|--------|
| Token Limits | 월간/일간/단일 실행 토큰 한도 | 100k/10k/2k |
| Execution Limits | 일간 실행 횟수, 동시 실행 수 | 500/5 |
| Cost Limits | 월간 비용 한도, 알림 임계값 | $50/80% |
| Access Control | AI가 접근 가능한 기능 제한 | Agent/Orchestrator만 |
| Allowed Node Types | AI가 생성/수정 가능한 노드 제한 | 안전한 노드만 |
| On Limit Reached | 한도 도달 시 동작 | Block |

**입출력 정의**:
| 포트 | 타입 | 설명 |
|------|------|------|
| output_log | text | 사용량 로그 |
| output_alert | event | 한도 도달 알림 |

---

### 3.4 GraphRAG Query 노드 (컨텍스트 생성용)

> **한 줄 정의**: Flow에서 GraphRAG를 "근거 컨텍스트 생성(Context Producer)" 노드로 제공한다.

**핵심 원칙**:
- GraphRAG 노드는 **답을 만들지 않는다. 컨텍스트만 만든다.**
- **경계 원칙 준수**: GraphRAG는 "값(컨텍스트)"을 만든다 → Agent와 동일한 책임 등급
- Flow 구조 변경은 **Orchestrator(FlowPatch)만** 수행한다

#### 3.4.1 책임/역할

입력 질문과 KB 참조(`kb_ref`)를 기반으로:
- 근거 원문(`chunks`)
- 출처 메타(`citations`)
- 관계 서브그래프(`entity_subgraph`)
- 요약/통계(`summary`/`stats`)

를 생성해 **`context_bundle`(ResultPack)** 로 출력한다.

#### 3.4.2 노드 외형

```
┌─────────────────────────────────────┐
│ 📚 GraphRAG Query                   │
├─────────────────────────────────────┤
│ ● input_question                    │
│ ● input_kb_ref                      │
│─────────────────────────────────────│
│  KB: project_docs @ v2.1           │
│  Mode: auto                         │
│  TopK: 10  Hop: 2                  │
│  [⚙️ Settings] [🔍 Inspector]       │
│─────────────────────────────────────│
│               output_context_bundle●│
└─────────────────────────────────────┘
```

#### 3.4.3 입력/출력 정의 (Contract)

**입력(Input)**:
| 포트/설정 | 타입 | 필수 | 설명 |
|----------|------|------|------|
| question | string | ✅ | 검색 질문 |
| kb_ref | { index_id, version? } | ✅ | Knowledge Base 참조 |
| mode | 'auto' \| 'local' \| 'global' | - | 검색 모드 (기본: auto) |
| filters | { scope?, tags?, doc_ids? } | - | 검색 범위 필터 |
| params | { top_k?, community_limit?, hop? } | - | 검색 파라미터 |
| budget | { max_tokens? } | - | 실행 예산(토큰) |

**출력(Output)** - `output_context_bundle`:
```json
{
  "query": "지진하중 계산 방법",
  "mode": "local",
  "summary": "관련 문서 8개에서 15개 청크 검색됨",
  "chunks": [
    { "doc_id": "doc_001", "text": "...", "score": 0.92, "locator": "p.12", "span": [120, 450] }
  ],
  "citations": [
    { "doc_id": "doc_001", "locator": "p.12-13", "url": "...", "hash": "abc123" }
  ],
  "entity_subgraph": {
    "nodes": [{"id": "지진하중", "type": "concept"}],
    "edges": [{"source": "지진하중", "target": "KDS 41", "relation": "defined_in"}]
  },
  "kb": { "index_id": "project_docs", "index_version": "v2.1" },
  "stats": { "latency_ms": 342, "token_usage": 1250, "cost": 0.003, "cache_hit": false }
}
```

#### 3.4.4 캐시 정책 (필수)

- **캐시 키**: `index_id + index_version + mode + normalized(question) + filters + params`
- **원칙**: 캐시는 GraphRAGQueryNode에만 적용한다. (후속 노드는 projection이므로 캐시 불필요)
- **이유**:
  - 인덱스 업데이트 후에도 구버전 결과가 재사용되는 사고 방지
  - 동일 질문 반복 실행 비용 절감

#### 3.4.5 실패/폴백 정책

| 실패 유형 | 대응 |
|----------|------|
| KB 없음/버전 불일치 | error + Inspector에서 확인 |
| 검색 결과 0 | 빈 context_bundle 반환 + ⚠️ 근거부족 배지 |
| GraphRAG 파이프라인 오류 | VectorRAGFallback=true면 벡터검색으로 degrade, 아니면 실패 처리 |

#### 3.4.6 권장 연결 패턴

```
[Question] ──┐
             ├──▶ [GraphRAG Query] ──▶ [AI Agent] ──▶ [Report]
[KB Ref] ────┘         │                    │
                  context_bundle      답변 생성
                    (근거)          (근거 기반)
```

- **장점**:
  - UI에서 "이 에이전트가 어떤 KB를 쓰는지" 명확
  - 디버깅/재현 쉬움
  - 캐시/로그가 노드 단위로 정리됨

---

### 3.5 RAG Inspector 모달 (GraphRAG 노드 전용)

> **목적**: "RAG가 잘 됐는지"를 시각적으로 검증하고, 튜닝(TopK/mode/filters)을 감이 아니라 **근거 기반**으로 한다.
>
> **원칙**: Inspector는 실행을 다시 돌리지 않고, **Run에 저장된 아티팩트(artifact)만 렌더링**한다.

#### 3.5.1 모달 진입

- GraphRAG 노드의 **[🔍 Inspector]** 버튼 클릭 시 오픈
- 선택된 Run(또는 마지막 성공 Run)의 `rag_result.json` 로드

#### 3.5.2 헤더 (고정)

```
┌───────────────────────────────────────────────────────────────┐
│ 📊 RAG Inspector                                              │
├───────────────────────────────────────────────────────────────┤
│ KB: project_docs @ v2.1    Mode: auto    TopK: 10  Hop: 2    │
│ Latency: 342ms  Tokens: 1,250  Cost: $0.003  Cache: ❌        │
│                                                               │
│ Status: ✅ OK  |  ⚠️ 근거부족  |  ❌ 불일치                    │
├───────────────────────────────────────────────────────────────┤
│ [Overview] [Chunks] [Citations] [Graph]                       │
└───────────────────────────────────────────────────────────────┘
```

**Status 배지**:
- ✅ OK
- ⚠️ 근거부족
- ❌ 불일치 (옵션: Answer가 used_citations를 제공할 때)

#### 3.5.3 탭 구성 (4탭 고정)

| 탭 | 내용 |
|----|------|
| **Overview** | 요약 + 근거 수치 + Red Flags |
| **Chunks** | 점수/문서/스니펫 리스트 + 프리뷰(하이라이트), Copy context / Open source |
| **Citations** | doc/page/span/url 테이블 + Export |
| **Graph** | entity_subgraph 뷰어 + hop/edge 필터, 노드 클릭 시 관련 chunks/citations로 jump |

#### 3.5.4 레드 플래그 룰 (MVP)

| 배지 | 조건 |
|------|------|
| ⚠️ 근거부족 | `citations = 0` 또는 `chunks < 3` 또는 `top_score < threshold` |
| ❌ 불일치 | (옵션) Answer 노드가 `used_citations` 리턴 시, `used_citations = 0` 또는 커버리지 낮음 |

#### 3.5.5 저장/재현 (Artifact)

**Run에 저장 (필수)**:
- `rag_result.json` : context_bundle
- `rag_trace.json` : params, score 분포, cache_hit, latency

**옵션**:
- `rag_eval.json` : 정량 평가 (추후)

---

### 3.6 AI 노드 공통 모달 설정 (Retry/Repair 정책)

> **원칙**: Retry/Repair는 플랫폼 공통 런타임 기능으로 제공하되, 각 AI 노드(Agent/GraphRAG/Orchestrator)는 모달에서 노드별 정책을 설정/오버라이드할 수 있다.
>
> 이는 Flow의 타입/스키마 의존성을 깨지 않기 위한 **필수 안전장치**이며, 실패 시 자동 리페어/모델 승급/폴백을 **정책 기반**으로 수행한다.

#### 3.6.1 Retry 정책

| 설정 | 옵션 | 기본값 |
|------|------|--------|
| Max Attempts | 1~5 | 2 |
| Backoff | None / Fixed / Exponential | Fixed |

**Retry On** (체크박스):
- ✅ Schema validation failed
- ✅ Empty/low-evidence context (RAG)
- ☐ Tool call error
- ☐ Timeout

#### 3.6.2 Repair 정책 (핵심)

| 설정 | 옵션 | 기본값 |
|------|------|--------|
| Repair Mode | Off / Auto Repair / Strict (JSON only) | Auto Repair |
| Repair Strategy | Schema Repair / Content Repair / Both | Both |

**Repair Strategy 상세**:
- **Schema Repair**: 형식/필드 맞추기 (JSON 구조 수정)
- **Content Repair**: 값 누락/단위/형식 보정

**Repair Prompt Template**:
- 기본 템플릿 제공
- "Advanced"에서 커스텀 가능
- (권장) 기본은 잠그고, 옵션으로만 수정 허용

#### 3.6.3 Model Escalation

| 설정 | 옵션 | 기본값 |
|------|------|--------|
| Escalate on Repair | Off / On | On |
| Primary Model | 모델 선택 | gpt-4o-mini |
| Fallback Model | 모델 선택 | gpt-4.1 |

**동작**: 1차 모델 실패 시 → Fallback 모델로 자동 승급

#### 3.6.4 Stop/Degrade 정책

| 설정 | 옵션 | 기본값 |
|------|------|--------|
| On Failure | Fail / Degrade / Return empty + warning | Fail |

**Degrade 옵션**:
- GraphRAG → VectorRAG fallback
- Agent → Partial result 반환

#### 3.6.5 노드 타입별 프리셋

모달에서 **[프리셋]** 버튼으로 빠르게 세팅:

**AI Agent (Text/Code)**
| 항목 | 값 |
|------|---|
| Attempts | 2 |
| Repair | Auto (Content + Schema) |
| Escalation | On (optional) |
| On Failure | Fail |

**AI Agent (JSON Validated)**
| 항목 | 값 |
|------|---|
| Attempts | 3 |
| Repair | Strict JSON + Schema Repair 우선 |
| Escalation | On |
| On Failure | Fail (downstream 보호) |

**GraphRAG Query**
| 항목 | 값 |
|------|---|
| Attempts | 2 |
| Retry On | Empty/low evidence |
| Repair 전략 | top_k 증가 / mode auto→global / filters 완화 |
| On Failure | Degrade (VectorRAG) or Warn |

**Orchestrator (Preview only)**
| 항목 | 값 |
|------|---|
| Attempts | 1~2 |
| Repair | FlowPatch schema strict |
| Escalation | On |
| On Failure | Preview 중단 + 사용자 확인 (자동 적용 금지) |

#### 3.6.6 저장/재현

**노드 config에 저장**:
```json
{
  "retry_policy": { "max_attempts": 2, "backoff": "fixed", ... },
  "repair_policy": { "mode": "auto", "strategy": "both", ... }
}
```

**실행 결과 artifact에 포함** (Inspector 디버깅용):
- `attempts`: 시도 횟수
- `error_history`: 각 시도별 에러
- `repair_prompt_used`: 사용된 리페어 프롬프트 (요약본)
- `repaired`: true/false

---

## 4. 시스템 구조

```
┌─────────────────────────────────────────────────────────────┐
│                      Flow Editor                             │
│                                                              │
│   ┌────────────────┐  ┌────────────────┐  ┌────────────────┐│
│   │  AI Agent 노드  │  │ AI Orchestrator │  │ GraphRAG Query ││
│   │  (단일 작업)    │  │  (플로우 제어)  │  │ (컨텍스트 생성)││
│   │                │  │                │  │                ││
│   │  모달: 설정    │  │  모달: 설정    │  │ 모달: Inspector││
│   └───────┬────────┘  └───────┬────────┘  └───────┬────────┘│
│           │                   │                   │          │
└───────────┼───────────────────┼───────────────────┼──────────┘
            │                   │                   │
            ▼                   ▼                   ▼
┌─────────────────────────────────────────────────────────────┐
│                      Backend API                             │
│                                                              │
│   /api/ai/agent/              /api/ai/orchestrator/         │
│   ├── generate-text           ├── analyze-flow              │
│   ├── generate-code           ├── plan-changes              │
│   └── generate-json           ├── preview                   │
│                               └── execute                   │
│                                                              │
│   /api/rag/                   /api/ai/common/               │
│   ├── query (컨텍스트 생성)    ├── models (모델 목록)         │
│   ├── index (인덱싱/버전)      └── node-catalog (노드 스키마) │
│   └── artifacts/{run_id}                                    │
│       (Inspector용 결과/트레이스)                            │
└──────────────────────────────┬──────────────────────────────┘
                               │
                               ▼
┌─────────────────────────────────────────────────────────────┐
│                      LLM Provider / KB                       │
│   OpenAI / Claude / Ollama (로컬) + GraphRAG Index           │
└─────────────────────────────────────────────────────────────┘
```

---

## 5. 기술 스택

### 5.1 오케스트레이터: LangGraph

**선택 이유**:
- 무료, 오픈소스
- Python 친화적 (FastAPI 백엔드와 통합 용이)
- Temporal보다 가벼움
- 그래프 기반으로 PipelineXLab 노드 구조와 개념 일치
- 상태 관리 및 사이클 지원

**역할**:
- 여러 AI Agent 노드의 실행 순서 조율
- 상태 공유 및 전달
- 조건부 분기 처리
- 오류 발생 시 재시도/대체 경로

### 5.2 개별 에이전트 (AI Agent 노드): PydanticAI

**선택 이유**:
- 형식 준수 능력 최상 (Structured Output)
- Pydantic 기반 데이터 검증
- 엔지니어링 데이터의 타입 안전성 보장
- 노드 스키마 검증과 자연스럽게 연동

**역할**:
- 개별 AI 작업 실행 (코드 생성, 텍스트 생성 등)
- 출력 형식 강제 (JSON 스키마 준수)
- 입력값 검증

### 5.3 기술 스택 구조

```
┌─────────────────────────────────────────────────────────────┐
│                      AI Orchestrator 노드                    │
│                            │                                 │
│                    ┌───────▼───────┐                        │
│                    │   LangGraph   │ ← 에이전트 조율         │
│                    │  StateGraph   │                        │
│                    └───────┬───────┘                        │
│         ┌──────────────────┼──────────────────┐             │
│         ▼                  ▼                  ▼             │
│   ┌───────────┐     ┌───────────┐     ┌───────────┐        │
│   │ PydanticAI│     │ PydanticAI│     │ PydanticAI│        │
│   │  Agent 1  │     │  Agent 2  │     │  Agent 3  │        │
│   └───────────┘     └───────────┘     └───────────┘        │
│         │                  │                  │             │
│         ▼                  ▼                  ▼             │
│   [검증된 출력]      [검증된 출력]      [검증된 출력]        │
└─────────────────────────────────────────────────────────────┘
```

### 5.4 API Key 관리 (Settings에서 중앙 관리)

**위치**: Launcher > Settings > AI Configuration

```
┌────────────────── AI Configuration ──────────────────┐
│                                                       │
│  API Keys (한 곳에서 관리)                            │
│  ──────────────────────────────────────────────────  │
│                                                       │
│  OpenAI API Key:                                      │
│  ┌─────────────────────────────────────┐ [👁️] [Test] │
│  │ sk-xxxx...xxxx                      │             │
│  └─────────────────────────────────────┘             │
│                                                       │
│  Anthropic API Key (Claude):                          │
│  ┌─────────────────────────────────────┐ [👁️] [Test] │
│  │ (not set)                           │             │
│  └─────────────────────────────────────┘             │
│                                                       │
│  Ollama URL (로컬):                                   │
│  ┌─────────────────────────────────────┐     [Test]  │
│  │ http://localhost:11434              │             │
│  └─────────────────────────────────────┘             │
│                                                       │
│  Default Model:                                       │
│  [GPT-4 ▼]                                           │
│                                                       │
│  Usage Limit (월간):                                  │
│  [$50    ▼]  Current: $12.35                         │
│                                                       │
│                [Cancel]  [Save]                       │
└───────────────────────────────────────────────────────┘
```

**저장 위치**:
- 개발 모드: `launcher-settings.json` (프로젝트 루트)
- 프로덕션: `%APPDATA%/PipelineXLab/settings.json`

**보안**:
- API Key는 암호화 저장 (electron safeStorage)
- 로그에 API Key 노출 금지
- UI에서 마스킹 처리 (sk-xxxx...xxxx)

**연동 흐름**:
1. Settings에서 API Key 설정
2. AI 노드 실행 시 Settings에서 Key 조회
3. Backend가 해당 Key로 LLM API 호출

---

## 6. 사용자 시나리오

### 시나리오 A: 코드 생성

1. 사용자가 Number 노드 두 개 배치 (폭, 높이)
2. AI Agent 노드 배치, Number 노드들과 연결
3. AI Agent 모달에서 "출력: 코드" 선택
4. 프롬프트: "단면2차모멘트 계산 코드"
5. AI가 Python 코드 생성 → Python 노드로 전달

### 시나리오 B: 플로우 자동 확장

1. 기본 구조해석 플로우 작성 완료
2. AI Orchestrator 노드 배치
3. 프롬프트: "지진하중 분석 추가"
4. AI가 계획 수립 → 미리보기 표시
5. 사용자 승인 → 노드들 자동 생성 및 연결

### 시나리오 C: 설명 문서 생성

1. 복잡한 Python 노드 선택
2. AI Agent 노드 연결
3. 프롬프트: "이 코드를 Markdown으로 설명"
4. 결과 → MarkdownViewer 노드로 표시

---

## 7. 안전 장치

**왜 필요한가**: AI가 잘못된 결과를 생성하면 플로우가 망가질 수 있음

### 6.1 노드 생성 검증
- AI가 생성한 노드 JSON을 스키마와 비교
- 스키마 불일치 시 생성 거부, 오류 메시지 표시

### 6.2 미리보기 모드
- Orchestrator가 변경을 적용하기 전 시각적 미리보기
- 사용자가 확인 후 승인해야 실제 적용

### 6.3 실행취소 지원
- 모든 AI 작업은 되돌리기 가능
- 변경 전 자동 백업

### 6.4 권한 제한
- 기본값: 노드 삭제 비활성화
- 사용자가 명시적으로 허용해야 삭제 가능

---

## 8. 개발 단계

### Stage 1: 기반 구축 (1주)
- 백엔드 AI API 라우터 생성
- LLM Provider 추상화 (OpenAI, Claude, Ollama 지원)
- 노드 스키마 카탈로그 생성

### Stage 2: AI Agent 노드 (2주)
- AI Agent 컴포넌트 개발
- 모달 UI (설정 옵션)
- 텍스트/코드 생성 기능

### Stage 2.5: GraphRAG Query 노드 + Inspector (2주)
- GraphRAGQueryNode executor + 캐시 키 + artifact 저장
- `/api/rag/query`, `/api/rag/index`, `/api/rag/artifacts` 엔드포인트
- RAG Inspector 모달(4탭) 구현
- 레드 플래그 배지(⚠️ 근거부족) 구현
- (옵션) Vector fallback

### Stage 3: AI Orchestrator 노드 (2주)
- 워크플로우 분석 기능
- 변경 계획 생성 및 미리보기
- 노드 생성/연결 자동화

### Stage 4: 안전 및 최적화 (1주)
- 스키마 검증 강화
- 실행취소 기능
- 오류 처리 개선

---

## 9. 예상 결과물

| 항목 | 설명 |
|------|------|
| AI Agent 노드 | 단일 AI 작업 수행 노드 |
| AI Orchestrator 노드 | 워크플로우 제어 노드 |
| 백엔드 AI API | /api/ai/ 엔드포인트 |
| 모달 설정 UI | 각 노드별 옵션 화면 |

---

## 10. 핵심 원칙

1. **노드로 통합**: AI는 별도 기능이 아닌 플로우의 일부
2. **사용자 주도**: 사용자가 AI 동작을 직접 설정
3. **안전 우선**: 검증 없이 적용하지 않음
4. **점진적 구현**: 한 번에 다 만들지 않고 단계별 개발
5. **비용 통제**: Budget Guard로 엔터프라이즈 수준 관리

### 🔹 핵심 원칙: AI 결과는 항상 Node로 귀결된다

> **"AI가 만든 결과는 항상 Node 출력으로 남긴다."**
>
> 로그나 텍스트가 아니라, **재실행 가능한 노드 결과**로 남긴다.

**이것이 PipelineXLab이 ChatGPT와 다른 핵심이다:**

| ChatGPT | PipelineXLab |
|---------|--------------|
| 텍스트 응답 → 복사/붙여넣기 | AI 결과 → 노드로 저장 |
| 대화 기록에만 존재 | 플로우에 영구 보존 |
| 재실행 불가 | 언제든 재실행 가능 |
| 컨텍스트 손실 | 노드 연결로 컨텍스트 유지 |

**구현 원칙**:
- AI Agent의 모든 출력은 `output_result` 포트를 통해 다음 노드로 전달
- AI가 코드를 생성하면 → Python 노드에 저장
- AI가 값을 계산하면 → Display 노드에 저장
- AI가 플로우를 수정하면 → 실제 노드로 생성/수정

> **단, Agent는 값을 노드로 전달할 뿐, 노드 생성은 Orchestrator를 통해서만 이루어진다.**

**결과**:
- 사용자는 AI 결과를 잃어버리지 않음
- 플로우를 저장하면 AI가 만든 모든 것이 함께 저장됨
- 언제든 플로우를 열어 AI 결과를 재활용 가능

---

*작성일: 2026-01-04*
*버전: 3.0*
