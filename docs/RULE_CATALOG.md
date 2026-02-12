# Rule Catalog - Enhanced Schema System

코드에서 사용 가능한 모든 규칙들의 카탈로그입니다.

## 목차

- [1. 스키마 구조 규칙 (schema-logic.yaml)](#1-스키마-구조-규칙-schema-logicyaml)
- [2. UI 렌더링 규칙 (ui-rules.yaml)](#2-ui-렌더링-규칙-ui-rulesyaml)
- [3. 테이블 렌더링 규칙 (table.yaml)](#3-테이블-렌더링-규칙-tableyaml)
- [4. 빌더 폼 규칙 (builder.yaml)](#4-빌더-폼-규칙-builderyaml)
- [5. HTML 템플릿 규칙 (html-template.yaml)](#5-html-템플릿-규칙-html-templateyaml)
- [6. 검증 규칙 (schema-validation-rules.yaml)](#6-검증-규칙-schema-validation-rulesyaml)
- [빠른 참조](#빠른-참조)

## 1. 스키마 구조 규칙 (schema-logic.yaml)

**위치**: `schema_definitions/civil_gen_definition/enhanced/schema-logic.yaml`

### 1.1 구조 패턴 (schemaStructurePatterns)

스키마를 처리하기 전에 적용되는 변환 패턴입니다.

| 패턴 이름 | 설명 | Priority | Action |
|-----------|------|----------|--------|
| Unwrap Root Wrapper Key | 루트 래퍼 키를 제거하고 내부 스키마를 끌어올림<br>(예: `{ELEM: {...}}` → `{...}`) | 1 | `unwrap-root-key` |

**사용 예시**:
```json
// Before
{
  "ELEM": {
    "type": "object",
    "properties": { ... }
  }
}

// After
{
  "type": "object",
  "properties": { ... },
  "title": "ELEM"
}
```

### 1.2 플랫폼 골격 (platformSkeleton)

플랫폼 표준 컬렉션 구조 정의입니다.

| 항목 | 설명 | 기본값 |
|------|------|--------|
| Entity Collection | ID → Entity 매핑 구조 | `patternProperties: "^[0-9]+$"` |
| Default Body Root | 기본 래퍼 이름 | `"Assign"` |
| Alternative Body Roots | 대체 가능한 래퍼 목록 | `["Argument", "Assign"]` |
| Root Structure | 최상위 구조 | `type: "object"`, `additionalProperties: false` |

**사용 예시**:
```json
{
  "Assign": {
    "1": { "TYPE": "BEAM", ... },
    "2": { "TYPE": "WALL", ... }
  }
}
```

### 1.3 섹션 결정 규칙 (sectionRules)

필드가 어느 섹션에 표시될지 결정합니다.

| 우선순위 | 규칙 이름 | 조건 | 결과 |
|----------|-----------|------|------|
| 1 | Explicit Group | `x-ui.group` 존재 | 해당 그룹 사용 |
| 2 | Fallback | 항상 (조건 없음) | `"General"` 섹션 |

**코드 사용**:
```typescript
// x-ui.group이 있으면 해당 그룹으로
{
  "MATL": {
    "type": "integer",
    "x-ui": {
      "label": "Material No.",
      "group": "Common Keys"  // 이 필드는 "Common Keys" 섹션에 표시
    }
  }
}
```

### 1.4 필드 표시 규칙 (visibilityRules)

필드의 표시 여부를 평가합니다.

| 규칙 이름 | 조건 타입 | 평가 방법 | 결과 |
|-----------|-----------|-----------|------|
| No VisibleWhen | `no-visible-when` | - | 항상 표시 (`true`) |
| TYPE Condition | `has-field-condition` (field: TYPE) | `in` 연산자 | TYPE이 허용 목록에 있으면 표시 |
| Other Field Conditions | `has-other-conditions` | `all-match` | 모든 조건 만족 시 표시 |

**사용 예시**:
```json
{
  "CABLE": {
    "type": "integer",
    "x-ui": {
      "visibleWhen": {
        "TYPE": "TENSTR",
        "STYPE": 3
      }
    }
  }
}
// → TYPE이 "TENSTR"이고 STYPE이 3일 때만 표시
```

### 1.5 Required 상태 계산 (requiredCalculationRules)

필드의 필수 여부를 계산합니다.

| 우선순위 | 규칙 | 조건 | 상태 |
|----------|------|------|------|
| 1 | Base Required | 최상위 `required` 배열에 포함 | `required` |
| 2 | Conditional Required | `allOf` 조건에 매칭 | `required` |
| 3 | Not Visible | `visibleWhen` 조건 불만족 | `n/a` |
| 4 | Default | 위 조건 모두 해당 없음 | `optional` |

**사용 예시**:
```json
{
  "type": "object",
  "required": ["TYPE", "MATL"],  // ← Base Required
  "allOf": [
    {
      "if": {
        "properties": { "TYPE": { "const": "BEAM" } }
      },
      "then": {
        "required": ["SECT"]  // ← Conditional Required (TYPE=BEAM일 때)
      }
    }
  ]
}
```

### 1.6 검증 레이어 규칙 (validationLayerRules)

검증이 STD(표준) 레이어인지 APP(애플리케이션) 레이어인지 결정합니다.

| 레이어 | 설명 | 트리거 |
|--------|------|--------|
| STD | Standard JSON Schema validation | `enum`, `minItems`, `maxItems`, `type` |
| APP | Application-specific validation | `x-enum-by-type`, `x-node-count-by-type`, `x-value-constraint` |

**UI 표시**:
- STD: 파란색 배지
- APP: 주황색 배지

### 1.7 필드 설명 생성 규칙 (descriptionBuildingRules)

Description 컬럼 내용을 생성하는 순서와 템플릿입니다.

**생성 순서**:
1. label (`x-ui.label`)
2. enum-values (일반 `enum`)
3. enum-by-type (`x-enum-by-type`)
4. value-constraints (`x-value-constraint`)
5. node-count-by-type (`x-node-count-by-type`)
6. hint (`x-ui.hint`)

**템플릿**:
```yaml
label: "<strong>{label}</strong>"
enum-values:
  header: "<strong>Enum Values:</strong>"
  item: "<li><code>{value}</code> - {label}</li>"
  wrapper: "<ul>{items}</ul>"
```

---

## 2. UI 렌더링 규칙 (ui-rules.yaml)

**위치**: `schema_definitions/civil_gen_definition/enhanced/ui-rules.yaml`

### 2.1 타입 → 컴포넌트 매핑 (fieldTypeMapping)

JSON Schema 타입을 UI 컴포넌트로 변환합니다.

| JSON Schema Type | UI Component | Input Type | Placeholder |
|------------------|--------------|------------|-------------|
| `string` | `Input` | `text` | - |
| `number` | `Input` | `number` | - |
| `integer` | `Input` | `number` | - |
| `boolean` | `Checkbox` | - | - |
| `array` | `Textarea` | - | `"Enter as JSON array, e.g., [1, 2, 3]"` |
| `object` | `Textarea` | - | `"Enter as JSON object, e.g., {\"key\": \"value\"}"` |
| `enum` | `Select` | - | - |

**공통 Props**:
- `className`: `"bg-zinc-800 border-zinc-700"` (입력 필드)
- `className`: `"w-4 h-4"` (체크박스)

### 2.2 레이아웃 규칙 (layout)

#### Section Header (섹션 헤더)

| 속성 | 값 |
|------|-----|
| Enabled | `true` |
| Border Top | `border-t-2 border-cyan-800/50` |
| Text Color | `text-cyan-400` |
| Font Size | `text-sm` |
| Font Weight | `font-semibold` |
| Icon | `📋` |
| Padding | `pt-4 pb-2` |
| First Section Border | `false` (첫 섹션은 상단 border 없음) |

#### Field Grouping (필드 그룹화)

| 속성 | 값 |
|------|-----|
| Enabled | `true` |
| Group By | `x-ui.group` |

#### Nested Object (중첩 객체)

| 속성 | 값 |
|------|-----|
| Enabled | `true` |
| Accordion | `true` (아코디언 방식) |
| Checkbox | `true` (활성화 체크박스) |
| Border | `border-zinc-700` |
| Background | `bg-zinc-900/50` |
| Header Background | `bg-zinc-800/50` |

### 2.3 Enhanced 기능 (enhancedFeatures)

#### x-ui 메타데이터

| 기능 | 활성화 | 설명 |
|------|--------|------|
| label | ✅ | `x-ui.label`을 필드 레이블로 사용 |
| hint | ✅ | `x-ui.hint` 표시 |
| group | ✅ | `x-ui.group`으로 섹션 생성 |
| visibleWhen | ✅ | 조건부 표시 |

#### x-enum-by-type

| 속성 | 값 |
|------|-----|
| Enabled | `true` |
| Dynamic Update | `true` (TYPE 변경 시 자동 업데이트) |

#### x-value-constraint

| 속성 | 값 |
|------|-----|
| Enabled | `true` |
| Display | `hint` |
| Icon | `💡` |
| Color | `text-amber-400` |
| Font Size | `text-[10px]` |
| Font Style | `italic` |

#### x-node-count-by-type

| 속성 | 값 |
|------|-----|
| Enabled | `true` |
| Display | `hint` |
| Validation Trigger | `onChange` |

### 2.4 렌더링 규칙 (rendering)

#### Required Indicator

| 속성 | 값 |
|------|-----|
| Enabled | `true` |
| Symbol | `*` |
| Color | `text-red-400` |

#### Description/Label

| 속성 | 값 |
|------|-----|
| Enabled | `true` |
| Source | `x-ui.label` (우선) |
| Fallback | `key` |
| Position | `inline` |
| Color | `text-zinc-200` |
| Font Size | `text-sm` |

#### Hint Display

| 속성 | 값 |
|------|-----|
| Enabled | `true` |
| Position | `below` |
| Sources | `x-ui.hint`, `x-value-constraint`, `x-node-count-by-type` |
| Color | `text-amber-400` |
| Font Size | `text-[10px]` |
| Font Style | `italic` |
| Icon | `💡` |

### 2.5 검증 표시 (validation)

#### Standard Validation (STD)

| 속성 | 값 |
|------|-----|
| Enabled | `true` |
| Badge | `STD` |
| Badge Color | `bg-blue-900/50 text-blue-300 border-blue-700/50` |

#### App Layer Validation (APP)

| 속성 | 값 |
|------|-----|
| Enabled | `true` |
| Badge | `APP` |
| Badge Color | `bg-orange-900/50 text-orange-300 border-orange-700/50` |

#### Error/Warning Display

| 타입 | Position | Color | Icon |
|------|----------|-------|------|
| Error | `below` | `text-red-400` | `❌` |
| Warning | `below` | `text-yellow-400` | `⚠️` |

### 2.6 조건부 렌더링 (conditionalRendering)

#### visibleWhen

| 속성 | 값 |
|------|-----|
| Enabled | `true` |
| Evaluate On | `change` |
| Dependencies | `["TYPE"]` |

#### allOf (조건부 required)

| 속성 | 값 |
|------|-----|
| Enabled | `true` |
| Dynamic Required | `true` (TYPE에 따라 required 동적 변경) |

### 2.7 스타일링 (styling)

#### Colors

| 이름 | 색상 |
|------|------|
| Primary | `blue-600` |
| Secondary | `cyan-600` (Enhanced 강조색) |
| Success | `green-600` |
| Error | `red-600` |
| Warning | `amber-600` |
| Section | `cyan-400` (섹션 헤더) |

#### Spacing

| 항목 | 값 |
|------|-----|
| Field Gap | `4` (space-y-4) |
| Section Gap | `6` (space-y-6) |

#### Borders

| 항목 | 값 |
|------|-----|
| Field Border | `border-zinc-700` |
| Section Border | `border-cyan-800/50` |

---

## 3. 테이블 렌더링 규칙 (table.yaml)

**위치**: `schema_definitions/civil_gen_definition/enhanced/table.yaml`
**용도**: Spec Tab의 Visual Schema Table

### 3.1 스키마 확장 필드 추출 (schemaExtensions.extractFromSchema)

컴파일러가 스키마에서 추출할 확장 필드 목록입니다.

| 원본 키 | 변환 후 속성 | Transform Key | Passthrough |
|---------|--------------|---------------|-------------|
| `x-ui` | `ui` | ✅ | ❌ |
| `x-enum-labels` | `enumLabels` | ✅ | ❌ |
| `x-enum-by-type` | `enumByType` | ✅ | ❌ |
| `x-enum-labels-by-type` | `enumLabelsByType` | ✅ | ❌ |
| `x-node-count-by-type` | `nodeCountByType` | ✅ | ❌ |
| `x-value-constraint` | `valueConstraint` | ✅ | ❌ |
| `x-required-by-type` | `requiredByType` | ✅ | ❌ |
| `x-required-when` | `x-required-when` | ❌ | ✅ |
| `x-transport` | `x-transport` | ❌ | ✅ |

**Transform Key**: `x-` prefix 제거
**Passthrough**: 원본 키 그대로 유지

### 3.2 조건부 필드 표시 (schemaExtensions.conditional)

테이블에 조건부 필드 정보를 표시합니다.

| 키 | Display Icon | Display Label | Display Color | Display in Table |
|----|--------------|---------------|---------------|------------------|
| `x-ui.visibleWhen` | 📌 | "Visible when" | `#4c9aff` (파란색) | ✅ |
| `x-ui.hiddenWhen` | 🚫 | "Hidden when" | `#ff4c4c` (빨간색) | ✅ |
| `x-ui.disabledWhen` | ⚠️ | "Disabled when" | `#ffa500` (주황색) | ✅ |
| `x-required-when` | ⚠️ | "Required when" | `#ff9d4c` (주황색) | ✅ |
| `x-value-constraint.dependsOn` | 🔗 | "Depends on" | `#9d4cff` (보라색) | ✅ |

### 3.3 UI 메타데이터 (schemaExtensions.ui)

| 키 | Display in Table | Display Icon | Display Label |
|----|------------------|--------------|---------------|
| `x-ui.label` | ❌ | - | - |
| `x-ui.group` | ❌ | - | - |
| `x-ui.hint` | ✅ | 💡 | "Hint" |

### 3.4 테이블 구조 (tableStructure)

#### 컬럼 정의

| ID | Header | Width | Align |
|----|--------|-------|-------|
| `no` | "No." | 6% | center |
| `description` | "Description" | 35% | left |
| `key` | "Key" | 14% | left |
| `type` | "Value Type" | 10% | center |
| `default` | "Default" | 10% | center |
| `required` | "Required" | 25% | left |

#### 번호 매기기 (numbering)

| 속성 | 값 |
|------|-----|
| Start | `1` |
| Reset On Section | `false` (Enhanced는 연속 번호) |

### 3.5 섹션 헤더 (sectionHeaders)

| 속성 | 값 |
|------|-----|
| Enabled | `true` |
| Background | `bg-cyan-950/30` |
| Text Color | `text-cyan-400` |
| Font Size | `text-xs` |
| Font Weight | `font-semibold` |
| Padding | `p-2` |
| Icon | `📋` |
| Colspan | `6` |

### 3.6 조건 행 (conditionRows)

조건부 필드를 그룹화하여 표시하는 행입니다.

| 속성 | 값 |
|------|-----|
| Enabled | `true` |
| Background | `bg-cyan-950/30` (섹션 헤더와 동일) |
| Text Color | `text-cyan-400` |
| Font Size | `text-xs` |
| Font Weight | `font-semibold` |
| Padding | `p-2` |
| Colspan | `6` |
| Detect By | `["section", "__section_"]` |

### 3.7 행 렌더링 (rowRendering)

#### Standard Row

| 속성 | 값 |
|------|-----|
| Hover | `true` |
| Hover Style | `hover:bg-zinc-800/30` |

#### Description 컬럼

| 속성 | 값 |
|------|-----|
| Source | `x-ui.label` |
| Fallback | `key` |
| Format | `label` |
| Markdown | `true` |

**Features** (Description에 추가로 표시되는 정보):

| Feature | Enabled | Title | Format | Badge |
|---------|---------|-------|--------|-------|
| Enum | ✅ | "**Enum Values:**" | `• {value} - {label}` | - |
| Enum By Type | ✅ | "**Enum Values by Type:**" | `*{type}:*\n• {value} - {label}` | - |
| Value Constraint | ✅ | "**Value Constraints:**" | `• {type}: {constraint}` | `APP` |
| Node Count By Type | ✅ | "**Node Count by Type:**" | `• {type}: {count} nodes` | `APP` |

**Validation Badges**:

| Layer | Text | Class Name |
|-------|------|------------|
| Standard | `STD` | `text-[9px] px-1.5 py-0.5 rounded bg-blue-900/50 text-blue-300 border border-blue-700/50` |
| App Layer | `APP` | `text-[9px] px-1.5 py-0.5 rounded bg-orange-900/50 text-orange-300 border border-orange-700/50` |

#### Key 컬럼

| 속성 | 값 |
|------|-----|
| Format | `"\"{key}\""` (따옴표로 감쌈) |
| Color | `text-blue-400` |
| Font Family | `font-mono` |

#### Type 컬럼

| 속성 | 값 |
|------|-----|
| Format | `capitalize` (첫 글자 대문자) |
| Array Format | `Array[{itemType}]` |

#### Default 컬럼

| 속성 | 값 |
|------|-----|
| Empty Value | `"-"` |
| Format | `raw` |

#### Required 컬럼

| 속성 | 값 |
|------|-----|
| Dynamic By Type | `true` (TYPE별로 다름) |

**Simple Format** (모든 TYPE에 대해 동일):

| 상태 | Text | Class Name |
|------|------|------------|
| Required | "Required" | `px-2 py-0.5 text-xs rounded bg-red-600/20 text-red-400` |
| Optional | "Optional" | `px-2 py-0.5 text-xs rounded bg-zinc-700/50 text-zinc-400` |

**Complex Format** (TYPE별로 다른 경우):

```yaml
format: |
  **Required:** {requiredTypes}
  **Optional:** {optionalTypes}
style: "text-xs"
```

### 3.8 중첩 필드 (nestedFields)

| 속성 | 값 |
|------|-----|
| Enabled | `true` |
| Expandable | `true` |
| Expand Icon | `▼` |
| Collapse Icon | `▶` |
| Indentation | `pl-8` |
| Background | `bg-zinc-900/50` |
| Key Color | `text-amber-400` |

**Nested Section Header**:

| 속성 | 값 |
|------|-----|
| Enabled | `true` |
| Background | `bg-blue-950/30` (일반 섹션과 다름) |
| Text Color | `text-blue-400` |

### 3.9 스타일링 (styling)

#### Table

| 속성 | 값 |
|------|-----|
| Border | `true` |
| Border Color | `border-zinc-800` |
| Border Collapse | `true` |

#### Header

| 속성 | 값 |
|------|-----|
| Background | `bg-zinc-900` |
| Text Color | `text-white` |
| Font Size | `text-sm` |

#### Body

| 속성 | 값 |
|------|-----|
| Alternate Rows | `false` |
| Font Size | `text-sm` |

#### Cell

| 속성 | 값 |
|------|-----|
| Padding | `p-3` |
| Border Color | `border-zinc-800` |

#### Feature Boxes

| 속성 | 값 |
|------|-----|
| Margin | `mt-2` |
| Padding | `p-2` |
| Border Radius | `rounded` |
| Font Size | `text-xs` |

---

## 4. 빌더 폼 규칙 (builder.yaml)

**위치**: `schema_definitions/civil_gen_definition/enhanced/builder.yaml`
**용도**: Builder Tab의 폼 생성

### 4.1 Enhanced Schema 감지 마커 (enhancedSchemaMarkers)

스키마에 다음 필드가 하나라도 있으면 Enhanced Schema로 인식합니다.

```yaml
- "x-ui"
- "x-transport"
- "x-enum-by-type"
- "x-node-count-by-type"
```

### 4.2 래퍼 규칙 (wrapperRules)

URI 패턴에 따라 JSON을 어떤 래퍼로 감쌀지 결정합니다.

| Pattern | Wrapper | 설명 |
|---------|---------|------|
| `^/db/` | `Assign` | Database endpoints |
| `^/post/` | `Argument` | Post endpoints |
| `^/doc/` | `Argument` | Documentation endpoints |
| `.*` | `null` | 기본값 (래퍼 없음) |

**사용 예시**:
- `/db/elem` → `{ "Assign": { "1": { ... } } }`
- `/post/run_analysis` → `{ "Argument": { ... } }`

### 4.3 폼 레이아웃 (formLayout)

#### Container

| 속성 | Class Name |
|------|------------|
| Container | `bg-gradient-to-br from-purple-950/50 to-zinc-900 border-2 border-purple-800/50 rounded-lg p-6` |

#### Title

| 속성 | 값 |
|------|-----|
| Text | `"Schema-Based Fields"` |
| Icon | `📝` |
| Class Name | `text-lg font-semibold mb-4` |

**Badges**:

| Badge | Condition | Class Name |
|-------|-----------|------------|
| `Instance: {instanceKey}` | `useAssignWrapper` | `px-2 py-0.5 bg-blue-600/20 text-blue-400 text-[10px] rounded border border-blue-600/50` |
| `From Spec Tab` | `hasEnhancedSchema` | `px-2 py-0.5 bg-green-600/20 text-green-400 text-[10px] rounded border border-green-600/50` |

#### Field Container

| 속성 | Class Name |
|------|------------|
| Field Container | `space-y-4` |

### 4.4 섹션 헤더 (sectionHeaders)

| 속성 | 값 |
|------|-----|
| Enabled | `true` |
| Container | `pt-4 pb-2 border-t-2 border-cyan-800/50 first:pt-0 first:border-t-0` |
| Title | `text-sm font-semibold text-cyan-400 flex items-center gap-2` |
| Icon | `📋` |
| Detect By | `__section_` (필드명이 이것으로 시작) |

### 4.5 필드 렌더링 (fieldRendering)

#### Standard 필드

**Label**:

| 속성 | 값 |
|------|-----|
| Source | `description` (`x-ui.label`이 들어있음) |
| Fallback | `name` |
| Class Name | `text-sm flex items-center gap-2` |
| Show Required | `true` |
| Show Type | `true` |

**Input**:

| 속성 | 값 |
|------|-----|
| Class Name | `bg-zinc-800 border-zinc-700` |
| Show Placeholder | `true` |
| Placeholder Source | `placeholder` |

#### Enum 필드

| 속성 | 값 |
|------|-----|
| Component | `Select` |
| Dynamic | `true` (TYPE 변경 시 옵션 업데이트) |
| Trigger Field | `TYPE` |
| Show Empty Option | `false` |
| Option Format | `value` |

#### Array 필드

| 속성 | 값 |
|------|-----|
| Component | `Textarea` |
| Parse JSON | `true` |
| Format On Blur | `true` |
| Show Hint | `true` (`x-node-count-by-type` 표시) |

#### Object 필드

| 속성 | 값 |
|------|-----|
| Accordion | `true` |
| Checkbox | `true` |
| Expand By Default | `false` |
| Border | `border-zinc-700 rounded-md bg-zinc-900/50` |
| Header | `flex items-center gap-2 p-3 bg-zinc-800/50` |
| Content | `p-4 space-y-3 bg-zinc-900/30` |
| Child Border | `pl-4 border-l-2 border-zinc-700` |

### 4.6 힌트 표시 (hintsDisplay)

| 속성 | 값 |
|------|-----|
| Enabled | `true` |
| Position | `below` |
| Format | `{icon} {message}` |

**Sources**:

| Type | Icon | Style |
|------|------|-------|
| `x-value-constraint` | 💡 | `text-[10px] text-amber-400 italic` |
| `x-node-count-by-type` | 📊 | `text-[10px] text-cyan-400 italic` |
| `placeholder` | ℹ️ | `text-[10px] text-zinc-500 italic` |

### 4.7 동적 동작 (dynamicBehavior)

#### Conditional Visibility (visibleWhen)

| 속성 | 값 |
|------|-----|
| Enabled | `true` |
| Trigger Fields | `["TYPE", "STYPE"]` |
| Evaluate On | `change` |
| Hide Mode | `remove` |

#### Dynamic Enum (x-enum-by-type)

| 속성 | 값 |
|------|-----|
| Enabled | `true` |
| Trigger Field | `TYPE` |
| Update On | `change` |
| Preserve Value | `false` (TYPE 변경 시 값 초기화) |

#### Dynamic Required (allOf)

| 속성 | 값 |
|------|-----|
| Enabled | `true` |
| Trigger Field | `TYPE` |
| Update On | `change` |
| Show Indicator | `true` |

### 4.8 입력 동작 (inputBehavior)

#### Auto Save

| 속성 | 값 |
|------|-----|
| Enabled | `false` |
| Debounce | `1000` (ms) |

#### Validation

| 속성 | 값 |
|------|-----|
| Trigger | `onChange` |
| Show Errors | `immediate` |

**Layers**:

| Layer | Enabled | Badge |
|-------|---------|-------|
| Standard | `true` | `STD` |
| App Layer | `true` | `APP` |

#### Default Values

| 속성 | 값 |
|------|-----|
| Apply On Mount | `true` |
| Clear On Reset | `true` |

#### Type Coercion

| Type | Enabled |
|------|---------|
| Number | `true` |
| Boolean | `true` |
| Array | `true` |

### 4.9 인스턴스 관리 (instanceManagement)

Assign Wrapper를 사용할 때의 인스턴스 관리 UI입니다.

| 속성 | 값 |
|------|-----|
| Enabled | `true` |
| Wrapper | `Assign` |
| Default Instance | `"1"` |

**UI**:

| 요소 | 값 |
|------|-----|
| Container | `bg-gradient-to-br from-blue-950/50 to-zinc-900 border-2 border-blue-800/50 rounded-lg p-4` |
| Title | `"Assign Instances"`, icon: `🔢` |
| Add Button | `"+ Add Instance"`, `h-7 text-xs bg-blue-600 hover:bg-blue-500` |
| Remove Button | icon: `🗑️`, `p-1 rounded text-red-400 hover:bg-red-900/20` |
| Active Instance | `bg-blue-600 text-white` |
| Inactive Instance | `bg-zinc-800 text-zinc-400 hover:bg-zinc-700` |

### 4.10 JSON 미리보기 (jsonPreview)

| 속성 | 값 |
|------|-----|
| Enabled | `true` |
| Modes | `["annotated", "monaco"]` |
| Default Mode | `annotated` |

**Annotated Mode**:

| 속성 | 값 |
|------|-----|
| Show Badges | `true` |
| Key Color | `text-red-400` |
| String Color | `text-emerald-400` |
| Number Color | `text-green-400` |
| Boolean Color | `text-orange-400` |
| Null Color | `text-purple-400` |

**Monaco Mode**:

| 속성 | 값 |
|------|-----|
| Editable | `true` |
| Minimap | `true` |
| Apply Button | `"✨ Apply to Form"`, `bg-green-600 hover:bg-green-500` |

### 4.11 테스트 케이스 관리 (testCases)

| 속성 | 값 |
|------|-----|
| Enabled | `true` |
| Show Timestamp | `true` |
| Show Description | `true` |
| Inline Rename | `true` |

**Buttons**:

| Button | Text | Icon |
|--------|------|------|
| Save New | "Save as New Test Case" | 💾 |
| Update | "Update Test Case" | 💾 |
| Load | Auto | - |
| Delete | Confirm required | 🗑️ |

### 4.12 검증 메시지 (validationMessages)

| 메시지 타입 | 템플릿 |
|-------------|--------|
| Required | `{field} is required for {TYPE}` |
| Invalid Type | `{field} must be a {type}` |
| Invalid Enum | `{field} must be one of: {values} (for {TYPE})` |
| Min Items | `{field} must have at least {min} items` |
| Max Items | `{field} must have at most {max} items` |
| Node Count | `{field} must have {count} nodes for {TYPE}` |
| Value Constraint | `{field}: {constraint}` |

---

## 5. HTML 템플릿 규칙 (html-template.yaml)

**위치**: `schema_definitions/civil_gen_definition/enhanced/html-template.yaml`
**용도**: Enhanced Schema → HTML 문서 생성

### 5.1 문서 구조 (document)

#### 기본 설정

| 속성 | 값 |
|------|-----|
| Doctype | `<!DOCTYPE html>` |
| Lang | `en` |
| Charset | `UTF-8` |
| Viewport | `width=device-width, initial-scale=1.0` |
| Title | `{schemaTitle} - Enhanced Schema` |

#### 섹션 순서

1. **header**: 제목
2. **info**: 기본 정보 (Title, URI, Methods, Body Root)
3. **validation**: 검증 아키텍처 (STD/APP 레이어 설명)
4. **specifications**: 스펙 테이블
5. **transport**: Transport 상세 (Request Body 구조)

### 5.2 테이블 구조 (table)

#### Header Columns

| ID | Text | Width | Colspan |
|----|------|-------|---------|
| `no` | "No." | 6% | - |
| `description` | "Description" | 35% | 2 |
| `key` | "Key" | 14% | - |
| `type` | "Value Type" | 10% | - |
| `default` | "Default" | 10% | - |
| `required` | "Required" | 25% | - |

#### Section Header Row

```html
<tr>
  <td colspan="6" class="section-header">{sectionName}</td>
</tr>
```

#### Field Row

```html
<tr>
  <td class="centered">{no}</td>
  <td>{description}</td>
  <td class="field-key">"{key}"</td>
  <td class="centered">{type}</td>
  <td class="centered">{default}</td>
  <td>{required}</td>
</tr>
```

### 5.3 Feature Boxes (featureBoxes)

#### Enum Values

```html
<div class="enum-values">
  <strong>Enum Values:</strong>
  <ul class="type-list">
    <li>{value} - {label}</li>
    ...
  </ul>
</div>
```

**스타일**:
- Background: `#e8f5e9` (연한 초록)
- Border Left: `4px solid #4caf50` (초록)

#### Enum By Type

```html
<div class="enum-values">
  <strong>Enum Values by Type:</strong>
  <p><strong>{type}:</strong></p>
  <ul class="type-list">
    <li>{value} - {label}</li>
    ...
  </ul>
</div>
```

#### Value Constraints

```html
<div class="constraint-note">
  <strong>{type}:</strong> {constraint}<br>
  <span class="validation-badge app-validation">APP</span> Validated by application layer
</div>
```

**스타일**:
- Background: `#fff3e0` (연한 주황)
- Border Left: `4px solid #ff9800` (주황)

#### Node Count By Type

```html
<div class="constraint-note">
  <strong>Node count by type:</strong>
  <ul class="type-list">
    <li>{type}: {count} nodes</li>
    ...
  </ul>
  <span class="validation-badge app-validation">APP</span> Validated by application layer
</div>
```

### 5.4 CSS 스타일 (css)

#### Validation Badges

| Class | Background | Text Color |
|-------|------------|------------|
| `.std-validation` | `#2196f3` (파란색) | white |
| `.app-validation` | `#ff9800` (주황색) | white |

#### Section Header

| 속성 | 값 |
|------|-----|
| Background | `#3498db !important` (파란색) |
| Color | white |
| Font Weight | bold |
| Font Size | `1.1rem` |
| Padding | `15px` |

#### Field Key

| 속성 | 값 |
|------|-----|
| Font Family | `'Courier New', monospace` |
| Background | `#ecf0f1` (연한 회색) |
| Color | `#e74c3c` (빨간색) |
| Padding | `4px 8px` |
| Border Radius | `3px` |
| Font Weight | 600 |

---

## 6. 검증 규칙 (schema-validation-rules.yaml)

**위치**: `schema_definitions/civil_gen_definition/schema-validation-rules.yaml`
**용도**: 스키마 작성 규칙을 자동 검증

### 6.1 필수 규칙 (mustRules)

반드시 준수해야 하는 규칙입니다. 위반 시 에러가 발생합니다.

#### 1. Single Entity Schema 원칙

| 속성 | 값 |
|------|-----|
| ID | `single-entity-schema` |
| Severity | `error` |
| Category | `structure` |

**검사 내용**: 최상위 properties에 래퍼 객체(Assign, Argument 등)가 있는지 확인

**잘못된 예**:
```json
{
  "properties": {
    "Argument": {
      "properties": { "TABLE_NAME": { ... } }
    }
  }
}
```

**올바른 예**:
```json
{
  "properties": { "TABLE_NAME": { ... } },
  "x-transport": { "body-root": "Argument" }
}
```

**자동 수정**: ✅ (`flatten-wrapper`)

#### 2. Required 중첩 금지

| 속성 | 값 |
|------|-----|
| ID | `no-nested-required` |
| Severity | `error` |
| Category | `structure` |

**검사 내용**: required 배열이 중첩된 객체 안에 있는지 확인

**잘못된 예**:
```json
{
  "properties": {
    "Argument": {
      "required": ["TABLE_TYPE"]
    }
  }
}
```

**올바른 예**:
```json
{
  "required": ["TABLE_TYPE"],
  "properties": { ... }
}
```

**자동 수정**: ✅ (`move-required-to-root`)

#### 3. allOf에서 점 표기법 금지

| 속성 | 값 |
|------|-----|
| ID | `no-dot-notation-in-allof` |
| Severity | `error` |
| Category | `validation` |

**검사 내용**: allOf의 required에서 점 표기법(예: 'Argument.STAGE_STEP') 사용 확인

**잘못된 예**:
```json
{
  "allOf": [{
    "then": { "required": ["Argument.STAGE_STEP"] }
  }]
}
```

**올바른 예**:
```json
{
  "allOf": [{
    "then": { "required": ["STAGE_STEP"] }
  }]
}
```

**자동 수정**: ✅ (`remove-dot-notation`)

#### 4. Transport 래퍼 일관성

| 속성 | 값 |
|------|-----|
| ID | `transport-wrapper-consistency` |
| Severity | `error` |
| Category | `structure` |

**검사 내용**: x-transport.body-root에 정의된 래퍼가 properties에도 있는지 확인

**잘못된 예**:
```json
{
  "properties": { "Argument": { ... } },
  "x-transport": { "body-root": "Argument" }
}
```

**올바른 예**:
```json
{
  "properties": { "TABLE_NAME": { ... } },
  "x-transport": { "body-root": "Argument" }
}
```

**자동 수정**: ✅ (`remove-wrapper-from-properties`)

### 6.2 권장 규칙 (shouldRules)

권장하는 규칙입니다. 위반 시 경고가 발생합니다.

#### 1. x-ui로 레이블 정의

| 속성 | 값 |
|------|-----|
| ID | `use-x-ui-for-labels` |
| Severity | `warning` |
| Category | `metadata` |

**권장 사항**: 필드 레이블은 `x-ui.label`로 정의

```json
{
  "TABLE_NAME": {
    "type": "string",
    "x-ui": {
      "label": "Table Name",
      "group": "General"
    }
  }
}
```

#### 2. x-ui.group으로 섹션 구분

| 속성 | 값 |
|------|-----|
| ID | `use-x-ui-group` |
| Severity | `info` |
| Category | `metadata` |

**권장 사항**: 관련 필드는 `x-ui.group`으로 그룹화

#### 3. 표준 JSON Schema 우선 사용

| 속성 | 값 |
|------|-----|
| ID | `use-standard-validation-first` |
| Severity | `warning` |
| Category | `validation` |

**권장 사항**: 검증 로직은 표준 JSON Schema로 최대한 표현

**피해야 할 패턴**:
```json
{
  "SECT": {
    "x-required-for-types": ["BEAM", "WALL"]
  }
}
```

**권장 패턴**:
```json
{
  "allOf": [{
    "if": {
      "properties": { "TYPE": { "enum": ["BEAM", "WALL"] } }
    },
    "then": { "required": ["SECT"] }
  }]
}
```

### 6.3 금지 규칙 (mustNotRules)

절대 해서는 안 되는 규칙입니다. 위반 시 에러가 발생합니다.

#### 1. 컬렉션 금지

| 속성 | 값 |
|------|-----|
| ID | `no-collection-in-schema` |
| Severity | `error` |
| Category | `structure` |

**검사 내용**: 최상위 type이 'array'인지 확인

**잘못된 예**:
```json
{
  "type": "array",
  "items": { "type": "object", ... }
}
```

**올바른 예**:
```json
{
  "type": "object",
  "properties": { ... }
}
```

#### 2. x-ui에 검증 로직 금지

| 속성 | 값 |
|------|-----|
| ID | `no-validation-in-x-ui` |
| Severity | `error` |
| Category | `metadata` |

**검사 내용**: x-ui에 검증 관련 키워드(required, minimum, maximum 등) 확인

**잘못된 예**:
```json
{
  "x-ui": {
    "label": "Age",
    "required": true,
    "minimum": 0
  }
}
```

**올바른 예**:
```json
{
  "type": "integer",
  "minimum": 0,
  "x-ui": {
    "label": "Age"
  }
}
```

### 6.4 베스트 프랙티스 (bestPractices)

더 나은 스키마를 위한 제안입니다. 위반 시 정보 메시지가 표시됩니다.

| ID | 제안 | Severity |
|----|------|----------|
| `use-description` | description 추가 | `info` |
| `use-default-values` | default 값 제공 | `info` |
| `use-x-enum-labels` | enum에 레이블 추가 (`x-enum-labels`) | `info` |

### 6.5 검증 수준 (validationConfig.levels)

| Level | 포함 규칙 |
|-------|-----------|
| **Strict** | mustRules + mustNotRules + shouldRules + bestPractices |
| **Standard** | mustRules + mustNotRules + shouldRules |
| **Minimal** | mustRules + mustNotRules |

### 6.6 자동 수정 (validationConfig.autoFix)

| 속성 | 값 |
|------|-----|
| Enabled | `true` |
| Level | `mustRules` (필수 규칙만 자동 수정) |
| Confirm Before Fix | `true` (수정 전 확인) |

### 6.7 검증 시점 (validationConfig.validateOn)

자동 검증이 실행되는 시점입니다.

- `upload`: 스키마 업로드 시
- `save`: 저장 시
- `version-create`: 버전 생성 시

### 6.8 알려진 래퍼 목록 (knownWrappers)

```yaml
- "Assign"
- "Argument"
- "Data"
- "Body"
- "Payload"
- "Request"
- "Response"
```

### 6.9 허용된 x-* 확장 (allowedExtensions)

#### Standard

```yaml
- "x-ui"
- "x-transport"
- "x-enum-labels"
- "x-enum-by-type"
- "x-enum-labels-by-type"
- "x-value-constraint"
- "x-node-count-by-type"
- "x-exclusive-keys"
```

#### UI

```yaml
- "x-section-header"
- "x-section-by-type"
- "x-ui-hint"
- "x-ui-group"
- "x-ui-exclusive-group"
```

#### Validation

```yaml
- "x-validation-layer"
- "x-custom-validator"
```

### 6.10 검증 메시지 템플릿 (messageTemplates)

| 타입 | 템플릿 |
|------|--------|
| Error | `🚫 [{severity}] {ruleName}: {message}` |
| Warning | `⚠️ [{severity}] {ruleName}: {message}` |
| Info | `ℹ️ [{severity}] {ruleName}: {message}` |

**Detailed Error**:
```
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
🚫 오류: {ruleName}
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

위치: {path}
메시지: {message}

설명:
{explanation}

수정 방법:
{fixSteps}

참고: schema_definitions/manual/SCHEMA_CONTRACT_KO.md
```

---

## 빠른 참조

### 코드에서 사용하는 방법

#### 1. 스키마 로직 규칙 초기화

```typescript
import { initSchemaLogicRules } from '@/lib/schema/schemaLogicEngine';

// Enhanced schema 규칙 초기화
await initSchemaLogicRules('civil_gen_definition/enhanced');

// Original schema 규칙 초기화
await initSchemaLogicRules('civil_gen_definition/original');
```

#### 2. 스키마 컴파일

```typescript
import { compileEnhancedSchema } from '@/lib/schema/schemaCompiler';

const compiled = compileEnhancedSchema(jsonSchema, 'civil_gen_definition/enhanced');
// compiled.parameters: 테이블 렌더링용 파라미터
// compiled.fields: 빌더 폼용 필드
```

#### 3. 테이블 정의 가져오기

```typescript
import { loadTableDefinition } from '@/lib/rendering/yamlLoader';

const tableDef = await loadTableDefinition('civil_gen_definition/enhanced');
```

#### 4. 동적 테이블 렌더링 (Spec Tab)

```typescript
import DynamicTableRenderer from '@/lib/rendering/dynamicTableRenderer';

<DynamicTableRenderer 
  definition={tableDefinition}
  parameters={tableParameters}
  expandedParams={expandedParams}
  toggleParam={toggleParam}
/>
```

#### 5. 빌더 폼 필드 생성 (Builder Tab)

```typescript
import { convertToBuilderFields } from '@/lib/schema/builderAdapter';

const builderFields = convertToBuilderFields(
  compiledSchema.fields,
  'civil_gen_definition/enhanced'
);
```

#### 6. 조건부 필드 정보 추출

```typescript
import { collectFieldConditionInfo, groupFieldsByCondition } from '@/lib/schema/conditionExtractor';

const conditionInfo = collectFieldConditionInfo(schema.properties);
const grouped = groupFieldsByCondition(schema.properties, conditionInfo);
```

#### 7. 필드 Description 생성

```typescript
import { buildFieldDescription } from '@/lib/schema/descriptionBuilder';

const description = buildFieldDescription(fieldSchema, 'civil_gen_definition/enhanced');
```

#### 8. 스키마 캐싱

```typescript
import { schemaCache } from '@/lib/cache/schemaCache';

// 캐시에서 가져오기
const cached = schemaCache.get(cacheKey);

// 캐시에 저장
schemaCache.set(cacheKey, compiledResult);

// 캐시 삭제
schemaCache.delete(cacheKey);

// 전체 캐시 클리어
schemaCache.clear();
```

### 주요 x-* 확장 필드 요약

| 확장 필드 | 용도 | 예시 |
|-----------|------|------|
| `x-ui.label` | 필드 레이블 | `"Material No."` |
| `x-ui.group` | 섹션 그룹화 | `"Common Keys"` |
| `x-ui.hint` | 힌트 표시 | `"Enter a positive number"` |
| `x-ui.visibleWhen` | 조건부 표시 | `{"TYPE": "BEAM"}` |
| `x-transport.body-root` | 래퍼 지정 | `"Assign"` |
| `x-enum-by-type` | TYPE별 enum | `{"BEAM": [1,2], "WALL": [3,4]}` |
| `x-enum-labels` | Enum 레이블 | `{"1": "Thick", "2": "Thin"}` |
| `x-enum-labels-by-type` | TYPE별 enum 레이블 | TYPE별 레이블 맵 |
| `x-value-constraint` | 값 제약 | `{"TENSTR": "Negative only"}` |
| `x-node-count-by-type` | TYPE별 노드 수 | `{"BEAM": 2, "PLATE": [3,4]}` |
| `x-required-by-type` | TYPE별 필수 여부 | `{"BEAM": true, "WALL": false}` |
| `x-required-when` | 조건부 필수 | `{"TYPE": "WALL"}` |

### YAML 파일 경로

| YAML 파일 | 경로 |
|------------|------|
| Schema Logic | `schema_definitions/civil_gen_definition/enhanced/schema-logic.yaml` |
| UI Rules | `schema_definitions/civil_gen_definition/enhanced/ui-rules.yaml` |
| Table Rules | `schema_definitions/civil_gen_definition/enhanced/table.yaml` |
| Builder Rules | `schema_definitions/civil_gen_definition/enhanced/builder.yaml` |
| HTML Template | `schema_definitions/civil_gen_definition/enhanced/html-template.yaml` |
| Validation Rules | `schema_definitions/civil_gen_definition/schema-validation-rules.yaml` |

### 참고 문서

- [YAML_GUIDE_KO.md](../schema_definitions/civil_gen_definition/manual/YAML_GUIDE_KO.md): YAML 규칙 작성 가이드
- [SCHEMA_CONTRACT_KO.md](../schema_definitions/civil_gen_definition/manual/SCHEMA_CONTRACT_KO.md): Enhanced Schema 계약서
- [SCHEMA_MODE_GUIDE.md](./SCHEMA_MODE_GUIDE.md): Original vs Enhanced 모드 가이드

---

이 카탈로그는 `schema_definitions/civil_gen_definition/enhanced/` 및 `schema_definitions/civil_gen_definition/` 폴더의 YAML 파일들을 기반으로 작성되었습니다.
