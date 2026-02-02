# MCP API Schema Server

> **AI가 생성한 JSON Schema를 저장** - YAML 규칙 기반 자동 검증/변환

---

## 아키텍처

```
┌─────────────────────────────────────────────────────────────────┐
│  AI                                                             │
│  └─ UI 캡쳐 분석 → 스키마 생성 → save_schema 호출              │
└─────────────────────┬───────────────────────────────────────────┘
                      │
┌─────────────────────▼───────────────────────────────────────────┐
│  MCP Server                                                     │
│  ├─ rules-loader.ts    ← YAML 규칙 읽기                        │
│  ├─ schema-validator.ts ← 검증 & 변환                          │
│  └─ save.ts            ← 스키마 저장                           │
└─────────────────────┬───────────────────────────────────────────┘
                      │ 읽기
┌─────────────────────▼───────────────────────────────────────────┐
│  schema_definitions/civil_gen_definition/enhanced/              │
│  ├─ mcp-generation-rules.yaml  ← 생성 규칙 (enum 형식 등)      │
│  └─ mcp-validation-rules.yaml  ← 검증 규칙 (필수값, 타입 등)   │
└─────────────────────────────────────────────────────────────────┘
```

---

## YAML 규칙 파일

### 📝 `mcp-generation-rules.yaml` - 생성 규칙

| 섹션 | 설명 |
|-----|------|
| `enumRules` | enum 생성 형식 (정수는 oneOf, 문자열은 enum) |
| `fieldNaming` | 접두사 타입 추론 (i→integer, b→boolean) |
| `xuiRules` | x-ui 필수/선택 속성 |

### ✅ `mcp-validation-rules.yaml` - 검증 규칙

| 섹션 | 설명 |
|-----|------|
| `enumValidation` | 정수 enum → oneOf 자동 변환 |
| `requiredProperties` | x-ui.label, x-ui.group 필수 |
| `typeValidation` | 접두사-타입 일치 검사 |
| `structureValidation` | $schema, title 필수 |

---

## 자동 변환 예시

### enum → oneOf 변환

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
  ]
}
```

---

## 워크플로우

```
1. 사용자 → UI 캡쳐 제공
2. AI → 분석 후 스키마 생성
3. AI → 불확실한 항목이 있으면 questions와 함께 save_schema 호출
4. MCP → pending_questions 상태 반환 (저장 안 함)
5. AI → 사용자에게 질문
6. 사용자 → 답변 제공
7. AI → 완성된 스키마로 save_schema 호출
8. MCP → YAML 규칙 읽기 → 검증 → 자동 변환 → 저장
```

---

## 규칙 수정 가이드

### enum 형식 변경하기

`mcp-validation-rules.yaml` 수정:

```yaml
enumValidation:
  integerEnumMustUseOneOf:
    enabled: true   # false로 바꾸면 변환 안 함
    fix:
      fallbackLabel: "Option {value}"  # 라벨 없을 때 포맷
```

### 필수 속성 변경하기

```yaml
requiredProperties:
  xuiLabel:
    enabled: true   # x-ui.label 필수 여부
  xuiGroup:
    enabled: true   # x-ui.group 필수 여부
```

---

## MCP 도구

### `save_schema`

| 파라미터 | 타입 | 설명 |
|---------|-----|------|
| schemaName | string | 필수. 파일명 |
| outputDir | string | 서브디렉토리 (기본: default) |
| schema | object | 필수. JSON Schema |
| description | string | 설명 (선택) |
| questions | array | 불확실한 항목 질문 목록 |
| skipValidation | boolean | 검증 건너뛰기 (디버깅용) |

### 응답

```json
{
  "ok": true,
  "status": "saved",
  "schemaPath": "generated_schemas/civil_gen/PSC_DESIGN_CODE.json",
  "validation": {
    "errors": [{ "field": "iTENDON_TYPE", "message": "...", "fixApplied": true }],
    "warnings": [{ "field": "DCODE", "message": "x-ui.label 자동 생성" }]
  }
}
```

---

## 🔧 MCP 서버 설정

### 1. 빌드

```bash
cd mcp-server
npm install
npm run build
```

### 2. Gemini CLI 설정

`~/.gemini/settings.json` 파일에 다음 내용을 추가:

#### 옵션 A: 절대경로 사용

```json
{
  "mcpServers": {
    "api-schema": {
      "command": "node",
      "args": [
        "C:/MIDAS/code/apiverification/mcp-server/dist/index.js"
      ],
      "env": {
        "APIVERIFICATION_ROOT": "C:/MIDAS/code/apiverification"
      }
    }
  }
}
```

#### 옵션 B: 상대경로 사용 (cwd 설정)

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

> **💡 설정 설명:**
> - `cwd`: MCP 서버가 실행될 작업 디렉토리 (한 번만 절대경로 설정)
> - `args`: `cwd` 기준 상대경로로 스크립트 지정
> - `APIVERIFICATION_ROOT`: `.`으로 설정하면 `cwd`를 기준으로 동작


### 3. VS Code + Continue 설정

`.continue/config.json` 파일에 추가:

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

## 📁 스키마 저장 경로

### 디렉토리 구조

```
C:/MIDAS/code/apiverification/
├── generated_schemas/              ← 📌 스키마 저장 위치
│   ├── default/                    ← 기본 outputDir
│   │   ├── PSC_DESIGN_CODE.json   ← 스키마 파일
│   │   └── PSC_DESIGN_CODE.meta.json  ← 메타데이터
│   └── {outputDir}/                ← 커스텀 outputDir
│
├── schema_definitions/
│   └── civil_gen_definition/
│       └── enhanced/
│           ├── shared.yaml         ← SSOT v1.5 규칙
│           ├── mcp-generation-rules.yaml
│           └── mcp-validation-rules.yaml
│
└── mcp-server/
    └── dist/index.js               ← MCP 서버 엔트리포인트
```

### 저장되는 파일

| 파일 | 설명 |
|-----|------|
| `{schemaName}.json` | 생성된 JSON Schema (검증/변환 후) |
| `{schemaName}.meta.json` | 메타데이터 (해시, 타임스탬프, 검증 결과) |

### 메타데이터 예시

```json
{
  "createdAt": "2026-02-02T09:00:00.000Z",
  "updatedAt": "2026-02-02T09:00:00.000Z",
  "description": "PSC Design Code 설정 스키마",
  "inputHash": "sha256:abc123...",
  "outputHash": "sha256:def456...",
  "fieldCount": 17,
  "requiredCount": 2
}
```

---

## 🔍 MCP Inspector로 테스트

앱 내 **Terminal 탭**에서 MCP Inspector를 실행하면 내장 웹뷰에서 테스트 가능:

```bash
# 수동 실행 시
npx @modelcontextprotocol/inspector node dist/index.js
```

또는 앱의 **Terminal 탭 → MCP Inspector → Start** 클릭

---

## 🔥 v1.5 SSOT 검증

MCP 서버는 `shared.yaml`의 SSOT 규칙을 참조하여 검증:

| 검증 항목 | 설명 |
|---------|------|
| `sectionId` 무결성 | `x-ui.sectionId`가 `sectionRegistry`에 존재하는지 |
| `group → sectionId` 마이그레이션 | 레거시 `x-ui.group`을 `sectionId`로 자동 변환 |
| `enum → oneOf` 변환 | 정수 enum을 `oneOf + const` 형식으로 변환 |

### 에러 예시

```json
{
  "validation": {
    "errors": [
      {
        "field": "sDesignCode",
        "message": "x-ui.sectionId \"SECTION_INVALID\"가 sectionRegistry에 없음. 유효값: SECTION_GENERAL, SECTION_INPUT, SECTION_OUTPUT",
        "fixable": true
      }
    ]
  }
}
```

