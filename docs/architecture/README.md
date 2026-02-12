# API Verification System - Architecture v4.0

**2026-01 최신 업데이트 반영**

---

## 📋 목차

1. [시스템 개요](#시스템-개요)
2. [핵심 레이어 아키텍처](#핵심-레이어-아키텍처)
3. [Schema-Driven Development (SDD)](#schema-driven-development)
4. [Field Runtime State System](#field-runtime-state-system)
5. [데이터 플로우](#데이터-플로우)
6. [폴더 구조](#폴더-구조)
7. [API 엔드포인트](#api-엔드포인트)
8. [변경 이력](#변경-이력)

---

## 시스템 개요

### 목적
MIDAS API 문서 기반의 요청/응답 검증 및 테스트 케이스 자동 생성 시스템

### 핵심 기능
| 기능 | 설명 |
|------|------|
| 📝 **JSON Schema 기반 UI 자동 생성** | Builder Tab에서 스키마 기반 동적 폼 렌더링 |
| 🎯 **Field Runtime State** | 조건부 Visibility, Required, Enabled 실시간 계산 |
| 🧪 **테스트 케이스 자동 생성** | LLM + YAML Rules 기반 테스트 케이스 생성 |
| 📊 **버전 관리** | API 스펙 히스토리, 버전별 비교 |
| 🔒 **분산 Lock** | 동시 편집 방지 (5분 TTL) |
| 🌐 **실시간 API 테스트** | Runner Tab에서 즉시 테스트 |

### 기술 스택
```
Frontend: React 18 + TypeScript + Vite
State:    Zustand
UI:       shadcn/ui + Tailwind CSS
Backend:  Node.js + Express
Database: Supabase (PostgreSQL)
Schema:   JSON Schema Draft-07 + x-* Extensions
Config:   YAML (schema-validation-rules.yaml)
```

---

## 핵심 레이어 아키텍처

```
┌─────────────────────────────── Frontend ───────────────────────────────┐
│                                                                         │
│  ┌──────────────┐  ┌───────────────┐  ┌────────────────┐ ┌──────────┐ │
│  │  Version Tab │  │  Builder Tab  │  │   Spec Tab     │ │Runner Tab│ │
│  │  - History   │  │  - Dynamic UI │  │  - Table View  │ │- Execute │ │
│  │  - Compare   │  │  - JSON Edit  │  │  - HTML Export │ │- Results │ │
│  └──────────────┘  └───────────────┘  └────────────────┘ └──────────┘ │
│         │                  │                   │                │      │
│  ┌──────┴──────────────────┴───────────────────┴────────────────┘      │
│  │                                                                      │
│  │  🎨 Rendering Layer (src/lib/rendering)                             │
│  │  ├─ DynamicSchemaRenderer    Form rendering (YAML-based)           │
│  │  ├─ DynamicTableRenderer     Table rendering                       │
│  │  └─ definitionLoader         YAML definition loader                │
│  │                                                                      │
│  │  🧠 Schema Processing Layer (src/lib/schema)                        │
│  │  ├─ schemaCompiler           JSON Schema → EnhancedField[]         │
│  │  ├─ fieldRuntimeState ★      Visibility/Required/Enabled 계산       │
│  │  ├─ schemaLogicEngine        VariantAxis 추론, 조건 평가            │
│  │  ├─ tableGenerator           HTML 테이블 생성                        │
│  │  └─ builderAdapter           Builder 전용 스키마 변환               │
│  │                                                                      │
│  │  💾 State Management (src/store)                                    │
│  │  └─ useAppStore (Zustand)                                           │
│  │     ├─ versions, currentVersionId                                   │
│  │     ├─ manualData, specData, builderData, runnerData                │
│  │     ├─ endpointLock, currentUserId                                  │
│  │     └─ acquireEndpointLock, releaseEndpointLock                     │
│  │                                                                      │
│  └──────────────────────────────────────────────────────────────────────┘
│                              │                                          │
└──────────────────────────────┼──────────────────────────────────────────┘
                               │ HTTP
┌──────────────────────────────▼──────────────────────────────────────────┐
│                         Backend (server/)                                │
│  ├─ routes/endpoints.js      CRUD + Status update                       │
│  ├─ routes/versions.js       Create, Load, Export/Import                │
│  ├─ routes/locks.js          Endpoint/Version Lock (5min TTL)           │
│  ├─ routes/attachments.js    File upload/download                       │
│  └─ routes/proxy.js          CORS proxy for API testing                 │
└──────────────────────────────┼──────────────────────────────────────────┘
                               │ PostgreSQL
┌──────────────────────────────▼──────────────────────────────────────────┐
│                        Supabase Database                                 │
│  ├─ projects, products, api_groups, api_endpoints                       │
│  ├─ api_versions, attachments                                           │
│  └─ endpoint_locks, version_locks (5min TTL, auto-cleanup)              │
└─────────────────────────────────────────────────────────────────────────┘
```

---

## Schema-Driven Development

### x-* Extension 아키텍처

JSON Schema의 표준 속성 외에 커스텀 `x-*` 확장을 통해 동적 동작을 정의합니다.

```json
{
  "STYPE": {
    "type": "integer",
    "description": "Element subtype",
    "x-enum-by-type": {
      "TENSTR": [1, 2, 3],
      "COMPTR": [1, 2],
      "PLATE": [1, 2, 3, 4]
    },
    "x-required-by-type": {
      "TENSTR": true,
      "BEAM": false
    },
    "x-ui": {
      "label": "Element Subtype",
      "group": "Common Keys and Solid"
    }
  }
}
```

### 지원하는 x-* Extensions

| Extension | 용도 | 파싱 위치 |
|-----------|------|-----------|
| `x-ui` | UI 메타데이터 (label, group) | schemaCompiler |
| `x-enum-by-type` | TYPE별 다른 enum 값 | fieldRuntimeState |
| `x-required-by-type` | TYPE별 Required 상태 | fieldRuntimeState |
| `x-required-when` | 조건부 Required (예: TYPE=WALL) | fieldRuntimeState |
| `x-value-constraint` | TYPE별 값 제약 | fieldRuntimeState |
| `x-node-count-by-type` | TYPE별 노드 개수 | tableGenerator |
| `x-enum-labels` | Enum 값의 레이블 | tableGenerator |

---

## Field Runtime State System ★

> **핵심 모듈**: `src/lib/schema/fieldRuntimeState.ts`

### 아키텍처

```
┌─────────────────────────────────────────────────────────────────────┐
│              calculateFieldRuntimeStates(sections, formValues)       │
├─────────────────────────────────────────────────────────────────────┤
│                                                                      │
│  Step 0: 그룹별 허용 TYPE 사전 계산                                   │
│  ┌────────────────────────────────────────────────────────────────┐ │
│  │  "Tension / Compression" 그룹:                                  │ │
│  │  ├─ TENS: x-value-constraint에 TENSTR, COMPTR                  │ │
│  │  └─ 최종 허용 TYPE: {TENSTR, COMPTR}                            │ │
│  └────────────────────────────────────────────────────────────────┘ │
│                              ↓                                       │
│  Step 1: 필드별 Visibility 계산                                      │
│  ┌────────────────────────────────────────────────────────────────┐ │
│  │  1.1 x-ui.visibleWhen 평가                                       │ │
│  │  1.2 x-required-when 조건 평가                                   │ │
│  │  1.3 x-required-by-type 체크                                     │ │
│  │  1.4 x-enum-by-type 체크                                         │ │
│  │  1.5 그룹별 허용 TYPE 체크 ★                                      │ │
│  └────────────────────────────────────────────────────────────────┘ │
│                              ↓                                       │
│  Step 2: Required 상태 계산                                          │
│  ┌────────────────────────────────────────────────────────────────┐ │
│  │  - required 모드 (always/conditional/optional)                  │ │
│  │  - requiredNow (현재 조건에서 실제 required 여부)                │ │
│  └────────────────────────────────────────────────────────────────┘ │
│                              ↓                                       │
│  Output: FieldRuntimeStateMap                                        │
│  { STYPE: { visible: false, required: 'conditional', requiredNow: false } }
│                                                                      │
└─────────────────────────────────────────────────────────────────────┘
```

### 단일 소스 원칙 (Single Source of Truth)

```typescript
// ❌ 이전: 여러 곳에서 visibility 계산
if (field['x-required-when']) { /* 로직 A */ }
if (field.ui?.visibleWhen) { /* 로직 B */ }

// ✅ 현재: fieldRuntimeState가 유일한 소스
const runtimeStates = calculateFieldRuntimeStates(sections, formValues, variantAxes);
const isVisible = runtimeStates[fieldName].visible;
```

### 그룹 기반 Visibility

```
TYPE=BEAM 선택
 → "Tension / Compression" 그룹 허용 TYPE: {TENSTR, COMPTR}
 → BEAM ∉ {TENSTR, COMPTR}
 → 그룹 전체 숨김 (ANGLE, TENS, T_LIMIT, T_bLMT 모두!)
 → 섹션 타이틀 "Tension / Compression"도 자동 숨김
```

---

## 데이터 플로우

### Builder Tab 렌더링 플로우

```
1. 엔드포인트 선택 → 버전 로드
2. specData.jsonSchemaEnhanced 로드
3. compileEnhancedSchema() → SectionGroup[] 생성
4. inferVariantAxes() → VariantAxis[] 추론
5. <DynamicSchemaRenderer> 마운트
6. calculateFieldRuntimeStates() 호출
7. 필드별 visible/required/enabled 상태 적용
8. 사용자 입력 → formValues 업데이트
9. useEffect → fieldRuntimeStates 재계산
10. UI 자동 업데이트 ✨
```

### JSON Preview 생성 플로우

```
1. dynamicFormData 변경 감지
2. buildCleanJSON() 호출
   - Required 필드: null도 포함
   - Optional 필드: 값이 있을 때만 포함
3. wrapWithAssign() (Assign 모드 시)
4. JSON 문자열로 변환
5. View/Edit 모드로 표시
```

### 잠금(Lock) 시스템 플로우

```
엔드포인트 선택
 → checkEndpointLock(endpointId)
 → 다른 사용자 잠금 시 Dialog 표시
 → "Load Anyway" → Read-only 모드
 → 잠금 없음 → acquireEndpointLock()
 → 5분 TTL, 1분마다 자동 정리
```

---

## 폴더 구조

```
apiverification/
├── src/
│   ├── features/projects/components/
│   │   ├── tabs/
│   │   │   ├── BuilderTab.tsx       ★ 동적 폼 빌더
│   │   │   ├── VersionTab.tsx        버전 관리 + Lock
│   │   │   ├── SpecTab.tsx           스펙 테이블
│   │   │   └── RunnerTab.tsx         API 테스트
│   │   ├── APIListPanel.tsx          API 트리 + Lock 상태
│   │   └── ProjectsView.tsx          메인 워크스페이스
│   │
│   ├── lib/
│   │   ├── schema/
│   │   │   ├── schemaCompiler.ts     ★ JSON Schema 파싱
│   │   │   ├── fieldRuntimeState.ts  ★ Runtime State 계산
│   │   │   ├── schemaLogicEngine.ts  ★ VariantAxis 추론
│   │   │   ├── tableGenerator.ts       HTML 테이블 생성
│   │   │   └── builderAdapter.ts       Builder 어댑터
│   │   │
│   │   └── rendering/
│   │       ├── dynamicRenderer.tsx   ★ 동적 폼 렌더링
│   │       └── dynamicTableRenderer.tsx
│   │
│   └── store/
│       └── useAppStore.ts            ★ Zustand 스토어
│
├── server/
│   ├── routes/
│   │   ├── endpoints.js               엔드포인트 CRUD
│   │   ├── versions.js                버전 관리
│   │   ├── locks.js                  ★ Lock 시스템
│   │   └── proxy.js                   CORS 프록시
│   └── database.js                    Supabase 연결
│
├── schema_definitions/
│   └── civil_gen_definition/
│       └── schema-validation-rules.yaml  ★ 테스트 생성 규칙
│
└── docs/architecture/
    ├── README.md                       (이 파일)
    ├── auto-test-generation.md         테스트 자동 생성
    ├── data-flow.md                    상세 데이터 플로우
    └── v3-behavior-engine.md           V3 엔진 (레거시)
```

---

## API 엔드포인트

### Endpoints
```
GET    /api/endpoints              목록 조회
POST   /api/endpoints              생성
PUT    /api/endpoints/:id          수정
DELETE /api/endpoints/:id          삭제
```

### Versions
```
GET    /api/versions?endpointId=   목록 조회
GET    /api/versions/:id           단일 조회
POST   /api/versions               생성
PATCH  /api/versions/:id           수정
GET    /api/versions/:id/export    JSON 내보내기
POST   /api/versions/import        JSON 가져오기
```

### Locks
```
GET    /api/locks/endpoint/:id/lock    상태 확인
POST   /api/locks/endpoint/:id/lock    잠금 획득
DELETE /api/locks/endpoint/:id/lock    잠금 해제
GET    /api/locks/all                  전체 Lock 목록 (디버그)
```

### Proxy
```
POST   /api/proxy                   CORS 프록시 요청
```

---

## 변경 이력

### v4.0 (2026-01-21) ★ Current
- **Field Runtime State System** 도입
  - 그룹별 허용 TYPE 사전 계산
  - `x-required-by-type`, `x-enum-by-type` 기반 visibility
  - 섹션 타이틀 자동 숨김
- **Lock 시스템 버그 수정**
  - 사용자 ID 영속 저장
- **Builder Tab 개선**
  - 조건부 Trigger 필드 로직

### v3.0 (2026-01-14)
- Behavior Engine V3
- YAML 기반 동작 로직
- Expression Language (DSL)

### v2.0 (2026-01-13)
- YAML 기반 UI 렌더링
- Enhanced Schema 지원

### v1.0 (Initial)
- 기본 API 검증 시스템

---

**Last Updated**: 2026-01-21  
**Architecture Version**: 4.0
