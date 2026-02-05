# MCP API Schema Server

> **AI 기반 JSON Schema 자동 생성** - YAML 규칙 기반 검증/변환/최적화

---

## 🎯 핵심 기능

1. **AI → Schema 생성** - UI 캡처 분석 후 스키마 자동 생성
2. **YAML 규칙 기반 검증** - shared.yaml SSOT 규칙 준수 확인
3. **자동 변환** - enum → oneOf, 필드명 타입 검증, 조건부 로직 최적화
4. **Accuracy Policy** - 불확실한 항목 자동 감지 및 질문 생성
5. **Table Schema** - 테이블 결과 스키마 자동 생성

---

## 📐 아키텍처

```
┌─────────────────────────────────────────────────────────────────┐
│  AI (Gemini/Claude)                                             │
│  └─ UI 캡쳐 분석 → 스키마 생성 → save_schema 호출              │
└─────────────────────┬───────────────────────────────────────────┘
                      │
┌─────────────────────▼───────────────────────────────────────────┐
│  MCP Server (mcp-server/src)                                    │
│  ├─ tools/save.ts          ← 스키마 저장 도구                 │
│  ├─ utils/                                                      │
│  │  ├─ schema-validator.ts ← 검증 엔진                        │
│  │  ├─ schema-transformer.ts ← 변환 엔진                      │
│  │  └─ rule-loader.ts      ← YAML 규칙 로더                  │
│  └─ resources/rules.ts     ← MCP Resources (규칙 제공)        │
└─────────────────────┬───────────────────────────────────────────┘
                      │ 읽기
┌─────────────────────▼───────────────────────────────────────────┐
│  schema_definitions/civil_gen_definition/enhanced/              │
│  ├─ shared.yaml           ← 🔥 SSOT v1.5 (테이블/섹션/마커)   │
│  ├─ promptRules.yaml      ← AI 생성 가이드라인                │
│  ├─ accuracyPolicy.yaml   ← 정확도 정책 (자동 질문)           │
│  ├─ mcp.yaml              ← 필드 약어 사전                    │
│  └─ ui.yaml               ← UI 컴포넌트 규칙                  │
└─────────────────────────────────────────────────────────────────┘
```

---

## 📦 YAML 규칙 파일 체계

### � `shared.yaml` - SSOT v1.5 (Single Source of Truth)

**역할:** 프로젝트 전체의 단일 진실 소스

| 섹션 | 설명 |
|-----|------|
| `tableSchemaRegistry` | 테이블 스키마 공통 필드 정의 (TABLE_NAME, TABLE_TYPE, COMPONENTS 등) |
| `sectionRegistry` | UI 섹션 정의 (SECTION_GENERAL, SECTION_INPUT, SECTION_OUTPUT 등) |
| `markers` | 특수 마커 ({{USER_INPUT}}, {{TABLE_TYPE}}, {{ELEMENT_TYPE}} 등) |
| `commonFields` | 재사용 가능한 공통 필드 템플릿 |

**검증 기능:**
- `x-ui.sectionId`가 `sectionRegistry`에 존재하는지 확인
- `{{마커}}` 문법 검증
- 테이블 스키마 필수 필드 확인

### 📋 `promptRules.yaml` - AI 생성 가이드라인

**역할:** AI가 스키마 생성 시 따라야 할 규칙

| 섹션 | 설명 |
|-----|------|
| `fieldNamingRules` | 필드명 규칙 (Hungarian Notation: i=integer, b=boolean, s=string 등) |
| `enumRules` | enum 형식 (정수는 oneOf, 문자열은 enum) |
| `conditionalLogic` | allOf/if/then 조건부 로직 패턴 |
| `criticalWarnings` | 절대 해서는 안 될 것들 (SELECT_TYPE, Element Type 등) |

### 🎯 `accuracyPolicy.yaml` - 정확도 정책

**역할:** 불확실한 항목 자동 감지 및 질문 생성

| 패턴 | 트리거 조건 | 액션 |
|-----|-----------|------|
| `dropdown_single_option` | enum이 1개만 보임 | "모든 옵션 알려주세요" 질문 생성 |
| `missing_table_headers` | COMPONENTS enum 누락 | "테이블 컬럼 헤더 추출하세요" 질문 생성 |
| `user_custom_value` | "User", "Custom" 키워드 발견 | "User 선택 시 값 형식?" 질문 생성 |
| `complex_conditional` | 3개 이상 중첩 조건 | "조건 로직 검증 필요" 질문 생성 |

### 📖 `mcp.yaml` - 필드 약어 사전

**역할:** 필드명 약어 표준 정의

```yaml
abbreviations:
  COMB: "Combined"
  ST: "Shear Torsion"
  FLEX: "Flexural"
  CHK: "Check"
  # ... 200+ 약어
```

### 🎨 `ui.yaml` - UI 컴포넌트 규칙

**역할:** 조건부 표시 로직 및 UI 상태 정의

| 섹션 | 설명 |
|-----|------|
| `conditionalDisplay` | x-required-when, x-optional-when 규칙 |
| `uiStates` | hidden, disabled, required 상태 정의 |

---

## 🔧 자동 변환 예시

### 1. enum → oneOf 변환 (정수)

**입력 (AI 생성):**
```json
"iTENDON_TYPE": {
  "type": "integer",
  "enum": [0, 1, 2],
  "x-ui": {
    "options": [
      { "value": 0, "label": "Low Relaxation Tendons" },
      { "value": 1, "label": "Stress Relieved Tendons" }
    ]
  }
}
```

**출력 (MCP 변환 후):**
```json
"iTENDON_TYPE": {
  "type": "integer",
  "oneOf": [
    { "const": 0, "title": "Low Relaxation Tendons" },
    { "const": 1, "title": "Stress Relieved Tendons" },
    { "const": 2, "title": "Option 2" }  // 라벨 없으면 자동 생성
  ],
  "x-ui": {
    "sectionId": "SECTION_GENERAL"  // group → sectionId 변환
  }
}
```

### 2. Table Schema 생성

**입력:**
```javascript
{
  schemaName: "STRESS_CHECK_RESULTS",
  schema: { /* ... */ },
  generateTableSchema: true,
  tableTypeEnums: ["STRESSCS", "STRESSCONC"],
  componentEnums: ["Elem", "Part", "Girder/Slab", "CHK", "FT", "FB"],
  componentEnumLabelsByType: {
    "STRESSCS": {
      "FT": "Stress at Top (N/mm²)",
      "FB": "Stress at Bottom (N/mm²)"
    }
  }
}
```

**출력:**
자동으로 TABLE_NAME, TABLE_TYPE, EXPORT_PATH, UNIT, STYLES, COMPONENTS 필드 추가

---

## 📝 워크플로우

### 일반 스키마 생성

```
1. 사용자 → UI 캡쳐 제공
2. AI → 분석 후 스키마 생성
3. AI → save_schema 호출 (schema + questions)
4. MCP → shared.yaml 규칙 검증
5. MCP → 자동 변환 (enum→oneOf, group→sectionId 등)
6. MCP → accuracyPolicy 검사
   ├─ 질문이 있으면 → pending_questions 반환 (저장 안 함)
   └─ 질문 없으면 → saved 반환 (저장 완료)
7. AI → 사용자에게 질문 (pending인 경우)
8. 사용자 → 답변
9. AI → 완성된 스키마로 save_schema 재호출
```

### Table Schema 생성

```
1. 사용자 → 테이블 결과 이미지 제공
2. AI → 테이블 헤더 추출 (Elem, Part, CHK, FT, FB, FMAX 등)
3. AI → save_schema 호출:
   - generateTableSchema: true
   - componentEnums: ["Elem", "Part", "CHK", ...]
   - componentEnumLabelsByType: { "STRESSCS": { "FT": "...", ... } }
4. MCP → tableSchemaRegistry에서 필드 자동 추가
5. MCP → COMPONENTS enum 검증 (누락 시 critical 질문 생성)
6. MCP → 저장 또는 pending_questions 반환
```

---

## 🛠️ MCP 도구 (Tools)

### `save_schema`

**파라미터:**

| 이름 | 타입 | 필수 | 설명 |
|-----|-----|-----|------|
| `schemaName` | string | ✅ | 파일명 (확장자 제외) |
| `schema` | object | ✅ | JSON Schema 객체 |
| `outputDir` | string | ❌ | 서브디렉토리 (기본: "default") |
| `description` | string | ❌ | 스키마 설명 |
| `questions` | array | ❌ | 불확실한 항목 질문 목록 |
| `skipValidation` | boolean | ❌ | 검증 건너뛰기 (디버깅용) |
| `excludeSchemaField` | boolean | ❌ | $schema 필드 제외 (기본: true) |
| **테이블 스키마 전용** | | | |
| `generateTableSchema` | boolean | ❌ | 테이블 스키마 생성 |
| `tableTypeEnums` | array | ❌ | TABLE_TYPE enum 값 |
| `componentEnums` | array | ❌ | COMPONENTS enum 값 (테이블 헤더) |
| `componentEnumLabelsByType` | object | ❌ | TABLE_TYPE별 컬럼 라벨 |
| `additionalTableFields` | array | ❌ | 추가 테이블 필드 (NODE_ELEMS, PARTS 등) |
| `wrapWithSimpleObject` | boolean | ❌ | Argument 래퍼 추가 |
| `wrapWithEntityCollection` | boolean | ❌ | Assign 래퍼 추가 |

**응답 예시:**

```json
{
  "ok": true,
  "status": "saved",  // 또는 "pending_questions"
  "schemaPath": "generated_schemas/default/PSC_DESIGN_CODE.json",
  "validation": {
    "errors": [],
    "warnings": [
      {
        "field": "sDesignCode",
        "message": "x-ui.group → x-ui.sectionId 자동 변환",
        "fixApplied": true
      }
    ]
  },
  "questions": [  // pending_questions인 경우
    {
      "field": "componentEnums",
      "question": "Please extract ALL table column headers visible in the image",
      "context": "COMPONENTS field detected but enum values are missing"
    }
  ]
}
```

---

## � MCP Resources

### `rules/shared.yaml`
- SSOT v1.5 규칙 전체
- tableSchemaRegistry, sectionRegistry, markers

### `rules/promptRules.yaml`
- AI 생성 가이드라인
- criticalWarnings (절대 하지 말아야 할 것)

### `rules/accuracyPolicy.yaml`
- 자동 질문 생성 패턴
- Evidence 요구사항

### `rules/mcp.yaml`
- 필드 약어 사전
- abbreviations 맵핑

---

## 🔧 MCP 서버 설정

### 1. 빌드

```bash
cd mcp-server
npm install
npm run build
```

### 2. Gemini Desktop 설정

`~/.gemini/settings.json`:

```json
{
  "mcpServers": {
    "api-schema": {
      "command": "node",
      "args": ["./mcp-server/dist/index.js"],
      "cwd": "C:/MIDAS/code/apiverification",
      "env": {
        "APIVERIFICATION_ROOT": "."
      }
    }
  }
}
```

### 3. VS Code + Continue 설정

`.continue/config.json`:

```json
{
  "experimental": {
    "modelContextProtocolServers": [
      {
        "transport": {
          "type": "stdio",
          "command": "node",
          "args": ["C:/MIDAS/code/apiverification/mcp-server/dist/index.js"],
          "env": {
            "APIVERIFICATION_ROOT": "C:/MIDAS/code/apiverification"
          }
        }
      }
    ]
  }
}
```

---

## 📁 디렉토리 구조

```
C:/MIDAS/code/apiverification/
├── generated_schemas/              ← 📌 생성된 스키마 저장 위치
│   ├── default/                    ← 기본 outputDir
│   │   ├── PSC_DESIGN_CODE.json
│   │   └── STRESS_CHECK.json
│   └── custom_dir/                 ← 커스텀 outputDir
│
├── schema_definitions/
│   └── civil_gen_definition/
│       └── enhanced/
│           ├── shared.yaml         ← SSOT v1.5 (테이블/섹션/마커)
│           ├── promptRules.yaml    ← AI 생성 규칙
│           ├── accuracyPolicy.yaml ← 정확도 정책
│           ├── mcp.yaml            ← 약어 사전
│           └── ui.yaml             ← UI 규칙
│
└── mcp-server/
    ├── src/
    │   ├── index.ts               ← MCP 서버 엔트리포인트
    │   ├── tools/save.ts          ← save_schema 도구
    │   ├── resources/rules.ts     ← MCP Resources
    │   └── utils/
    │       ├── schema-validator.ts
    │       ├── schema-transformer.ts
    │       └── rule-loader.ts
    └── dist/                       ← 빌드 결과
```

---

## 🔍 MCP Inspector

앱 내장 Terminal 탭에서 MCP Inspector 실행 가능:

```bash
# 또는 수동 실행
npx @modelcontextprotocol/inspector node dist/index.js
```

---

## 🎯 Best Practices

### 1. 테이블 스키마 생성 시

✅ **DO:**
- `componentEnums`에 **모든** 테이블 헤더 명시
- `componentEnumLabelsByType`로 각 헤더에 대한 의미 제공
- `generateTableSchema: true` 설정

❌ **DON'T:**
- `componentEnums`를 빈 배열로 남겨두지 말 것
- `SELECT_TYPE`, `Element Type` 같은 UI 전용 필드 포함하지 말 것

### 2. enum 값 정의 시

✅ **DO:**
- 이미지에서 보이는 모든 옵션 추출
- 불확실하면 `questions` 파라미터 사용

❌ **DON'T:**
- 보이는 옵션이 1개뿐이라고 그것만 enum으로 만들지 말 것
- 추측하지 말고 질문할 것

### 3. 조건부 로직 사용 시

✅ **DO:**
- `allOf + if/then` 패턴 사용
- `x-required-when`, `x-optional-when` 사용

❌ **DON'T:**
- 하드코딩된 조건 사용하지 말 것
- 너무 복잡한 중첩 피할 것

---

## 🚨 자주 발생하는 오류

### 1. `sectionId not found in sectionRegistry`

**원인:** `x-ui.group` 대신 `x-ui.sectionId` 사용해야 함

**해결:**
```json
// ❌ 잘못된 예
"x-ui": {
  "group": "General"  // 레거시
}

// ✅ 올바른 예
"x-ui": {
  "sectionId": "SECTION_GENERAL"  // SSOT v1.5
}
```

### 2. `COMPONENTS enum is empty`

**원인:** 테이블 스키마인데 `componentEnums`를 제공하지 않음

**해결:**
```javascript
{
  generateTableSchema: true,
  componentEnums: ["Elem", "Part", "CHK", "FT", "FB"]  // 필수!
}
```

### 3. `Integer enum must use oneOf`

**원인:** 정수 enum을 `enum` 형식으로 사용

**해결:** 자동 변환되므로 그대로 저장하면 됨 (또는 직접 oneOf 사용)

---

## 📚 참고 자료

- [MCP Protocol Specification](https://modelcontextprotocol.io/)
- [JSON Schema Specification](https://json-schema.org/)
- [SSOT v1.5 Documentation](../schema_definitions/civil_gen_definition/enhanced/shared.yaml)
- [Accuracy Policy Guide](../schema_definitions/civil_gen_definition/enhanced/accuracyPolicy.yaml)

---

## 🤝 기여 방법

1. 규칙 수정: `schema_definitions/civil_gen_definition/enhanced/*.yaml` 편집
2. MCP 서버 수정: `mcp-server/src/` 편집 후 빌드
3. 테스트: MCP Inspector로 검증

---

**Last Updated:** 2026-02-05
**Version:** 2.0.0 (SSOT v1.5 기반)
