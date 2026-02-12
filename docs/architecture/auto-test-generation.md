# Auto Test Case Generation - 설계 문서

**Status**: 🔴 Draft (설계 중)  
**Version**: 0.1  
**Last Updated**: 2025-01-20

---

## 📋 목차

1. [개요](#개요)
2. [문제 정의](#문제-정의)
3. [목표](#목표)
4. [시스템 설계](#시스템-설계)
5. [알고리즘 상세](#알고리즘-상세)
6. [테스트 시나리오 유형](#테스트-시나리오-유형)
7. [UI/UX 설계](#uiux-설계)
8. [구현 계획](#구현-계획)
9. [열린 질문](#열린-질문)

---

## 개요

### 배경
현재 BuilderTab에서 테스트 케이스는 수동으로 하나씩 생성해야 합니다. 스키마가 복잡할수록 (트리거 필드, 조건부 필드 등) 필요한 테스트 케이스 수가 기하급수적으로 증가합니다.

### 제안
스키마의 **트리거 필드 조합**, **Required/Optional 상태**, **enum 값**, **edge cases**를 분석하여 테스트 케이스를 **자동 생성**하는 기능을 구현합니다.

---

## 문제 정의

### 현재 프로세스

```
사용자가 Builder에서 수동으로:
1. 트리거 필드 값 선택 (예: iMETHOD = 1)
2. Required 필드 채우기
3. Optional 필드 선택적 채우기
4. "Save as Test Case" 클릭
5. 1-4 반복 (다른 iMETHOD 값으로...)
```

### 문제점

| 문제 | 영향 |
|------|------|
| **반복 작업** | iMETHOD가 5개면 최소 5번 수동 생성 |
| **Coverage 누락** | Optional 필드 조합, edge cases 놓치기 쉬움 |
| **일관성 부족** | 테스트 케이스 명명 규칙 불일치 |
| **시간 소모** | 복잡한 스키마일수록 시간 증가 |

### 복잡도 분석

실제 스키마 예시 (SKEW 엔티티):

```
트리거 필드:
- iMETHOD: 5개 값 (1, 2, 3, 4, 5)

조건부 필드:
- iMETHOD=1,2,4: 필드 A, B required
- iMETHOD=3,5: 필드 C, D required
- 공통: 필드 E optional

최소 필요 테스트 케이스:
- Required Only: 5개 (각 iMETHOD 값)
- With Optional: 10개 (각 값 × 2)
- Edge Cases: 추가 N개

총: 15+ 케이스 수동 생성 필요
```

---

## 목표

### Primary Goals

1. **자동 생성**: 클릭 한 번으로 테스트 케이스 일괄 생성
2. **Smart Combinations**: 트리거 필드 값 조합 자동 계산
3. **Required 인식**: 각 조합에서 required 필드 자동 식별

### Secondary Goals

4. **Edge Cases**: 경계값 테스트 (minItems, maxItems 등)
5. **명명 규칙**: 일관된 테스트 케이스 이름 자동 생성
6. **선택적 생성**: 원하는 유형만 선택 가능

### Non-Goals (이번 버전에서 제외)

- ❌ Negative 테스트 (실패해야 하는 케이스)
- ❌ API 실행 후 응답 검증
- ❌ 다른 엔드포인트 간 순차 테스트

---

## 시스템 설계

### 아키텍처

```
┌──────────────────────────────────────────────────────────────────┐
│                     Auto Test Case Generator                      │
├──────────────────────────────────────────────────────────────────┤
│                                                                   │
│  ┌─────────────────────────────────────────────────────────────┐ │
│  │                    1. Schema Analyzer                        │ │
│  │  - extractTriggerFields()     → ['iMETHOD', 'TYPE']         │ │
│  │  - extractEnumValues()        → { iMETHOD: [1,2,3,4,5] }    │ │
│  │  - identifyConditionalFields() → visibleWhen 조건 파싱      │ │
│  └─────────────────────────────────────────────────────────────┘ │
│                              ↓                                    │
│  ┌─────────────────────────────────────────────────────────────┐ │
│  │                2. Combination Calculator                     │ │
│  │  - calculateCombinations()    → [[iMETHOD=1], [iMETHOD=2]]  │ │
│  │  - applyLimit(maxCount)       → 조합 수 제한               │ │
│  │  - prioritize()               → 중요 조합 우선              │ │
│  └─────────────────────────────────────────────────────────────┘ │
│                              ↓                                    │
│  ┌─────────────────────────────────────────────────────────────┐ │
│  │                 3. Runtime State Simulator                   │ │
│  │  - 각 조합에 대해 fieldRuntimeStates 계산                   │ │
│  │  - requiredNow, visible 상태 결정                           │ │
│  │  - 기존 calculateFieldRuntimeStates() 재사용               │ │
│  └─────────────────────────────────────────────────────────────┘ │
│                              ↓                                    │
│  ┌─────────────────────────────────────────────────────────────┐ │
│  │                 4. Test Data Builder                         │ │
│  │  - buildRequiredOnly()        → required 필드만 값 채움     │ │
│  │  - buildWithOptional()        → optional 포함               │ │
│  │  - buildEdgeCases()           → 경계값 케이스               │ │
│  │  - generateName()             → 케이스 이름 생성             │ │
│  └─────────────────────────────────────────────────────────────┘ │
│                              ↓                                    │
│  ┌─────────────────────────────────────────────────────────────┐ │
│  │                   5. Test Case Inserter                      │ │
│  │  - store.addTestCases([...])  → 일괄 추가                   │ │
│  │  - handleConflicts()          → 기존 케이스 충돌 처리        │ │
│  └─────────────────────────────────────────────────────────────┘ │
│                                                                   │
└──────────────────────────────────────────────────────────────────┘
```

### 데이터 플로우

```
User clicks "Auto Generate"
    ↓
Show Options Dialog
  - [ ] Required Only (default: checked)
  - [ ] Include Optional Fields
  - [ ] Include Edge Cases
  - Max combinations: [20]
    ↓
User clicks "Generate"
    ↓
Schema Analyzer
  ├─ extractTriggerFields() using visibleWhen analysis
  │    → ['iMETHOD']
  ├─ extractEnumValues(triggerFields)
  │    → { iMETHOD: [1, 2, 3, 4, 5] }
  └─ compile conditional dependencies
       → { 'SECT': { requiredWhen: 'iMETHOD in [1,2,4]' } }
    ↓
Combination Calculator
  ├─ cartesianProduct(triggerEnums)
  │    → [[1], [2], [3], [4], [5]]
  ├─ applyLimit(20)
  │    → (unchanged if < 20)
  └─ prioritize() based on coverage
    ↓
For each combination:
    ↓
  Runtime State Simulator
    ├─ setFormValues({ iMETHOD: 1 })
    ├─ calculateFieldRuntimeStates()
    │    → { SECT: { requiredNow: true }, ... }
    └─ collect visible + required fields
    ↓
  Test Data Builder
    ├─ buildRequiredOnly()
    │    → { iMETHOD: 1, SECT: 101, ... }
    ├─ (optional) buildWithOptional()
    │    → { iMETHOD: 1, SECT: 101, P2: [0,0,0], ... }
    └─ generateName("iMETHOD=1, Required")
    ↓
Collect all test cases
    ↓
Test Case Inserter
  ├─ Check for conflicts (same name?)
  ├─ addTestCases(testCases)
  └─ Toast: "Generated 5 test cases"
    ↓
UI updates with new test cases ✨
```

---

## 알고리즘 상세

### 1. Trigger Field Extraction

```typescript
function extractTriggerFields(
  schema: EnhancedSchema
): string[] {
  const triggers = new Set<string>();
  
  // 1. visibleWhen에서 참조하는 필드 추출
  for (const field of schema.properties) {
    const visibleWhen = field['x-ui']?.visibleWhen;
    if (visibleWhen) {
      Object.keys(visibleWhen).forEach(key => triggers.add(key));
    }
  }
  
  // 2. allOf if/then에서 조건 필드 추출
  if (schema.allOf) {
    for (const condition of schema.allOf) {
      if (condition.if?.properties) {
        Object.keys(condition.if.properties).forEach(key => 
          triggers.add(key)
        );
      }
    }
  }
  
  // 3. required[type] 형태에서 type 필드 추출
  for (const field of schema.properties) {
    if (typeof field.required === 'object') {
      // { "BEAM": "required", "TRUSS": "optional" }
      // → TYPE 필드가 트리거
      // (heuristic: enum 값이 key와 일치하는 필드 찾기)
    }
  }
  
  return Array.from(triggers);
}
```

### 2. Combination Generation

```typescript
interface TriggerValue {
  field: string;
  value: any;
}

type Combination = TriggerValue[];

function calculateCombinations(
  triggerFields: string[],
  schemaFields: Field[],
  maxCombinations: number
): Combination[] {
  // 각 트리거 필드의 enum 값 수집
  const enumMap: Record<string, any[]> = {};
  
  for (const fieldName of triggerFields) {
    const field = schemaFields.find(f => f.name === fieldName);
    if (field?.enum) {
      enumMap[fieldName] = field.enum;
    }
  }
  
  // Cartesian product 계산
  const fieldNames = Object.keys(enumMap);
  let combinations: Combination[] = [[]];
  
  for (const fieldName of fieldNames) {
    const values = enumMap[fieldName];
    const newCombos: Combination[] = [];
    
    for (const combo of combinations) {
      for (const value of values) {
        newCombos.push([
          ...combo,
          { field: fieldName, value }
        ]);
      }
    }
    
    combinations = newCombos;
  }
  
  // 제한 적용
  if (combinations.length > maxCombinations) {
    console.warn(`Limiting from ${combinations.length} to ${maxCombinations}`);
    combinations = combinations.slice(0, maxCombinations);
  }
  
  return combinations;
}
```

### 3. Test Data Building

```typescript
interface TestCaseData {
  name: string;
  description: string;
  requestBody: string;
}

function buildTestCaseData(
  combination: Combination,
  schemaFields: Field[],
  options: GenerationOptions
): TestCaseData[] {
  const results: TestCaseData[] = [];
  
  // 1. 트리거 값으로 초기 formData 설정
  const baseFormData: Record<string, any> = {};
  combination.forEach(({ field, value }) => {
    baseFormData[field] = value;
  });
  
  // 2. Runtime states 계산
  const runtimeStates = calculateFieldRuntimeStates(
    compiledSchemaSections,
    baseFormData,
    variantAxes
  );
  
  // 3. Required-only 케이스
  const requiredOnlyData = { ...baseFormData };
  
  for (const [fieldName, state] of Object.entries(runtimeStates)) {
    if (state.requiredNow && state.visible) {
      const field = schemaFields.find(f => f.name === fieldName);
      if (field && !(fieldName in requiredOnlyData)) {
        requiredOnlyData[fieldName] = getDefaultValue(field);
      }
    }
  }
  
  results.push({
    name: generateName(combination, 'Required'),
    description: `Auto-generated: ${formatCombination(combination)}, Required fields only`,
    requestBody: JSON.stringify(requiredOnlyData, null, 2)
  });
  
  // 4. With optional 케이스 (옵션 선택 시)
  if (options.includeOptional) {
    const withOptionalData = { ...requiredOnlyData };
    
    for (const [fieldName, state] of Object.entries(runtimeStates)) {
      if (state.visible && !state.requiredNow) {
        const field = schemaFields.find(f => f.name === fieldName);
        if (field && !(fieldName in withOptionalData)) {
          withOptionalData[fieldName] = getDefaultValue(field);
        }
      }
    }
    
    results.push({
      name: generateName(combination, 'Full'),
      description: `Auto-generated: ${formatCombination(combination)}, All fields`,
      requestBody: JSON.stringify(withOptionalData, null, 2)
    });
  }
  
  return results;
}

function generateName(combination: Combination, suffix: string): string {
  const parts = combination.map(c => `${c.field}=${c.value}`);
  return `[Auto] ${parts.join(', ')} - ${suffix}`;
}
```

---

## 테스트 시나리오 유형

### 1. Required Only (기본)

각 트리거 조합에 대해 Required 필드만 채움.

```json
{
  "iMETHOD": 1,
  "SECT": 101,
  "P1": [0, 0, 0]
}
```

**용도**: 최소 유효 데이터 검증

---

### 2. Full Fields (선택)

Required + 모든 Optional 필드 포함.

```json
{
  "iMETHOD": 1,
  "SECT": 101,
  "P1": [0, 0, 0],
  "P2": [0, 0, 0],
  "DESC": "Test description"
}
```

**용도**: 전체 필드 호환성 검증

---

### 3. Edge Cases (선택)

경계값 테스트.

| 대상 | 테스트 값 |
|------|----------|
| `minItems: 2` | 정확히 2개 항목 |
| `maxItems: 10` | 정확히 10개 항목 |
| `minimum: 0` | 0 값 |
| `maxLength: 100` | 100자 문자열 |

```json
{
  "iMETHOD": 1,
  "NODES": [1, 2]  // minItems=2 edge case
}
```

---

## UI/UX 설계

### 버튼 위치

```
┌─────────────────────────────────────────────────────────────────┐
│  🔧 Context-Aware Builder                    [Enhanced Schema] │
├─────────────────────────────────────────────────────────────────┤
│  + Creating New Test Case                                       │
│    구성 완료 후 "Save as New Test Case" 버튼을 눌러 저장하세요  │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  [🧪 Auto Generate]   ← 새 버튼                                │
│                                                                 │
│  ... Form Fields ...                                            │
│                                                                 │
├─────────────────────────────────────────────────────────────────┤
│                [Reset]  [Save as New Test Case]                 │
└─────────────────────────────────────────────────────────────────┘
```

### Options Dialog

```
┌──────────────────────────────────────────────────┐
│  🧪 Auto Generate Test Cases                     │
├──────────────────────────────────────────────────┤
│                                                  │
│  Scenario Types:                                 │
│  ☑ Required Only (minimum valid data)           │
│  ☐ Include Optional Fields                      │
│  ☐ Include Edge Cases (boundary values)         │
│                                                  │
│  Settings:                                       │
│  Max combinations: [20 ▼]                        │
│                                                  │
│  Preview:                                        │
│  ┌────────────────────────────────────────────┐  │
│  │ Will generate approximately 5 test cases   │  │
│  │ - iMETHOD=1: 1 case                       │  │
│  │ - iMETHOD=2: 1 case                       │  │
│  │ - iMETHOD=3: 1 case                       │  │
│  │ - iMETHOD=4: 1 case                       │  │
│  │ - iMETHOD=5: 1 case                       │  │
│  └────────────────────────────────────────────┘  │
│                                                  │
│  Existing Test Cases:                            │
│  ◉ Keep existing (add new)                       │
│  ○ Replace auto-generated only                   │
│  ○ Clear all and regenerate                      │
│                                                  │
├──────────────────────────────────────────────────┤
│              [Cancel]  [Generate]                │
└──────────────────────────────────────────────────┘
```

---

## 구현 계획

### Phase 1: Core Logic (MVP)
- [ ] `testCaseGenerator.ts` 모듈 생성
- [ ] `extractTriggerFields()` 구현
- [ ] `calculateCombinations()` 구현
- [ ] `buildTestCaseData()` 구현

### Phase 2: Integration
- [ ] `addTestCases()` 함수를 store에 추가
- [ ] BuilderTab에 "Auto Generate" 버튼 추가
- [ ] Options Dialog 구현

### Phase 3: Polish
- [ ] Preview 기능
- [ ] Edge cases 지원
- [ ] 기존 케이스 충돌 처리

### Phase 4: Documentation
- [ ] 사용 가이드 작성
- [ ] YAML 설정 옵션 (있다면)

---

## 열린 질문

> 아래 질문들에 대한 결정이 필요합니다.

### Q1: 조합 제한

트리거 필드가 여러 개일 때 조합이 폭발합니다.

예: `iMETHOD(5) × TYPE(10) × MODE(3) = 150개`

**Options**:
- A) 기본 최대값 20개, 사용자가 조정 가능
- B) 스마트 샘플링 (boundary values만)
- C) 계층적 생성 (1차: 주 트리거만, 2차: 조합)

**현재 제안**: Option A (단순한 제한)

---

### Q2: 테스트 케이스 명명

**Options**:
- A) `[Auto] iMETHOD=4, Required Only`
- B) `TC_001_iMETHOD_4_REQ`
- C) `iMETHOD-4-required-only`
- D) 사용자 정의 템플릿

**현재 제안**: Option A (가독성 좋음)

---

### Q3: 기존 케이스 처리

자동 생성 시 기존 수동 케이스는?

**Options**:
- A) 항상 유지, 새 케이스만 추가
- B) `[Auto]` prefix 케이스만 대체
- C) 사용자에게 선택 UI 제공

**현재 제안**: Option C (유연성)

---

### Q4: Negative Tests

유효하지 않은 데이터 테스트도 생성?

예: required 필드 누락, 범위 밖 값 등

**Options**:
- A) 이번 버전에서 제외 (Non-goal)
- B) 별도 옵션으로 제공 ("Include Negative Tests")
- C) 별도 기능으로 분리 ("Negative Test Generator")

**현재 제안**: Option A (복잡도 관리)

---

## 참고 자료

- [Architecture README](./README.md) - 전체 시스템 아키텍처
- [Data Flow](./data-flow.md) - 데이터 흐름 상세
- [fieldRuntimeState.ts](../../src/lib/schema/fieldRuntimeState.ts) - 런타임 상태 계산

---

## 변경 이력

| 날짜 | 버전 | 변경 내용 |
|------|------|----------|
| 2025-01-20 | 0.1 | 초안 작성 |
# LLM 기반 스키마 자동 생성 계획
> 기존 API 개발 방식의 한계를 극복하고, YAML 규칙 시스템을 활용하여 자연어 기반으로 API 스키마를 자동 생성하는 프로세스
---
## 핵심 아키텍처
> [!IMPORTANT]
> **YAML은 중간 산출물이 아니다.**
> **YAML은 LLM의 "참조 규칙 집합 (Constraint / Policy)"이다.**
┌─────────────────────────────────────────────────────────────┐
│ LLM │
│ ┌─────────────────┐ ┌─────────────────────────────┐ │
│ │ 입력 1: │ │ 입력 2: │ │
│ │ 자연어 시나리오 │ + │ YAML 규칙 (PSD / Rules) │ │
│ └────────┬────────┘ └──────────────┬──────────────┘ │
│ │ │ │
│ └──────────┬─────────────────┘ │
│ ▼ │
│ [규칙 기반 변환 로직] │
│ - "이 문장은 enum인가?" │
│ - "이건 visibleWhen이네" │
│ - "규칙을 어기지 않는 JSON만 출력" │
└──────────────────────┬──────────────────────────────────────┘
▼
┌─────────────────────┐
│ JSON Schema (ECS) │
│ enhanced.json │
└─────────────────────┘
```mermaid
flowchart TB
subgraph LLM["🤖 LLM"]
direction TB
subgraph inputs["입력"]
direction LR
input1["📝 입력 1:
자연어 시나리오"]
input2["📋 입력 2:
YAML 규칙
(PSD / Rules)"]
end
logic["⚙️ 규칙 기반 변환 로직
• 이 문장은 enum인가?
• 이건 visibleWhen이네
• 규칙을 어기지 않는 JSON만 출력"]
inputs --> logic
end
output["📄 JSON Schema (ECS)
enhanced.json"]
LLM --> output
style LLM fill:#e3f2fd,stroke:#1976d2,stroke-width:2px
style input1 fill:#fff3e0,stroke:#f57c00
style input2 fill:#f3e5f5,stroke:#7b1fa2
style logic fill:#e8f5e9,stroke:#388e3c
style output fill:#fffde7,stroke:#fbc02d,stroke-width:2px
```
**비유:**
- `YAML` = 컴파일러의 **문법 정의**
- `JSON` = **컴파일 결과물**
---
## 실제 예시: Structure Type 다이얼로그
### 입력 1: 자연어 시나리오 (기획자 작성)

## Structure Type 다이얼로그
### 1. Structure Type 선택
- 라디오 버튼 그룹에서 다음 중 하나를 선택한다:
- 3-D (기본값), X-Z Plane, Y-Z Plane, X-Y Plane, Constraint RZ
### 2. Mass Control Parameter
- "Lumped Mass" 또는 "Consistent Mass" 중 하나를 선택
### 3. Convert Self-weight into Masses (조건부 필드)
- 체크박스로 활성화할 수 있다
- **이 옵션이 활성화되면** "Convert to X,Y,Z / X,Y / Z" 라디오 버튼이 표시된다
- **비활성화되면** 해당 옵션은 숨김 처리된다
### 4. 수치 입력
- Gravity Acceleration: 숫자, 기본값 9.806 m/sec²
- Initial Temperature: 숫자, 기본값 0 [C]
```
### 입력 2: LLM에 주입된 YAML 규칙
```yaml
uiRules:
- when:
type: boolean
then:
canHave:
visibleWhen: true
requiredRules:
- when:
visibleWhen: exists
then:
mustDefine: x-required-when
enumRules:
- when:
description: contains "중 하나를 선택"
then:
mustHave: enum, x-enum-labels
```
### LLM 출력: JSON Schema (enhanced.json)
```json
{
"STYP": {
"type": "object",
"properties": {
"STYP": {
"type": "integer",
"description": "Structure type",
"enum": [0, 1, 2, 3, 4],
"x-enum-labels": {
"0": "3-D",
"1": "X-Z Plane",
"2": "Y-Z Plane",
"3": "X-Y Plane",
"4": "Constraint RZ"
}
},
"bSELFWEIGHT": {
"type": "boolean",
"description": "Convert self-weight into masses",
"default": false
},
"SMASS": {
"type": "integer",
"enum": [1, 2, 3],
"x-enum-labels": {
"1": "Convert to X, Y, Z",
"2": "Convert to X, Y",
"3": "Convert to Z"
},
"x-ui": {
"visibleWhen": { "bSELFWEIGHT": true }
},
"x-required-when": { "bSELFWEIGHT": true }
},
"GRAV": {
"type": "number",
"description": "Gravity acceleration (m/sec²)",
"default": 9.806
},
"TEMP": {
"type": "number",
"description": "Initial temperature (°C)",
"default": 0
}
}
}
}
```
> [!NOTE]
> **YAML은 출력에 나타나지 않는다.**
> **하지만 출력을 강제한다.**
---
## 자연어 → 스키마 변환 규칙
| 자연어 표현 | YAML 규칙 적용 | JSON 스키마 매핑 |
|------------|---------------|-----------------|
| "~중 하나를 선택한다" | `enumRules` | `enum` + `x-enum-labels` |
| "이 옵션이 활성화되면 ~가 표시된다" | `uiRules.visibleWhen` | `x-ui.visibleWhen` |
| "활성화되면 필수 입력" | `requiredRules` | `x-required-when` |
| "기본값은 ~이다" | - | `default` |
| "숫자를 입력한다" | - | `type: number` |
| "체크박스로 활성화" | - | `type: boolean` |
---
## 1. 개요 및 배경
### 1.1 현재 API 개발의 문제점
현재 API 개발 방식은 스키마 정의, 문서화, 실제 동작 검증이 분리되어 있어 다음과 같은 문제가 발생합니다:
검증 불가: 정의된 스키마가 실제 API 동작과 일치하는지 사전에 확인할 수 없습니다.
불일치 발생: 스키마, 문서, 실제 API 동작 간의 불일치로 인해 테스트 누락 및 런타임 오류가 발생합니다.
유지보수 비용 증가: API 변경 시마다 문서를 수동으로 수정해야 하므로 시간이 많이 소요되고 오류가 발생하기 쉽습니다.
신뢰성 저하: 검증 히스토리를 추적할 수 없어 변경에 따른 안정성을 보장하기 어렵습니다.
1.2 목표: Schema-Driven Development
우리의 목표는 스키마를 모든 것의 원천(Source of Truth)으로 만드는 것입니다.
단일 스키마 활용: 생성된 JSON Schema 파일 하나로 유효성 검증, UI 자동 생성, 문서화, 테스트 시나리오 생성을 모두 수행합니다.
사전 검증: 스키마를 통해 가능한 모든 입력 조합을 생성하고 실행하여 계약 위반을 사전에 차단합니다.
자동화: 자연어로 기술된 제품 사용 흐름을 LLM이 분석하여 올바른 스키마 구조를 자동으로 생성함으로써 기획 속도를 높입니다.
2. 자연어 기반 기획 프로세스
이 프로세스는 기획자가 UI 동작과 입력 흐름을 자연어로 기술하면, 시스템(LLM)이 이를 분석하여 표준화된 YAML 스키마(PSD, ECS)로 변환하는 과정을 거칩니다.
2.1 프로세스 단계
Step 1: 자연어 시나리오 작성 기획자는 제품의 사용 흐름을 다음과 같이 자연어로 상세하게 기술합니다.
UI 구성: "사용자는 '요소 생성' 폼에서 '타입' 드롭다운을 본다."
조건부 동작: "'타입'이 'BEAM'이면 '단면 번호' 입력 필드가 활성화되고, 'SOLID'면 비활성화된다."
입력 제약: "'인장력' 필드는 음수만 입력 가능하다."
단계별 조작: "Step 1에서 타입을 선택하고, Step 2에서 재료 번호를 입력한 후 제출한다."
Step 2: LLM 기반 스키마 자동 생성 LLM은 사전 정의된 Product Schema Definition (PSD) 규칙과 Schema Validation Rules를 참조하여 자연어 시나리오를 분석합니다.
+1
분석: 자연어에서 엔티티(Entity), 필드(Field), 조건(Condition), 제약(Constraint)을 추출합니다.
매핑: 추출된 정보를 x-ui, x-required-by-type, x-value-constraint 등의 확장 메타데이터에 매핑합니다.
생성: 표준화된 Endpoint Contract Schema (ECS) 구조의 초안을 생성합니다.
+1
Step 3: 스키마 검증 및 보정 생성된 스키마는 schema-validation-rules.yaml에 의해 자동으로 검증됩니다.
규칙 검사: 필수 필드 누락, 잘못된 타입 정의, 메타데이터 형식 오류 등을 확인합니다.
수정 제안: 오류가 발견되면 수정 제안을 제공하여 기획자가 빠르게 수정할 수 있도록 돕습니다.
Step 4: 결과물 자동 생성 확정된 스키마를 기반으로 다음 결과물들이 자동 생성됩니다.
UI 프로토타입: builder.yaml 규칙에 따라 입력 폼 UI가 렌더링되어 즉시 동작 확인 가능.
API 명세서: table.yaml 및 html-template.yaml을 활용하여 HTML 문서 자동 생성.
테스트 시나리오: 가능한 모든 입력 조합(Positive/Negative)에 대한 테스트 케이스 자동 생성.
3. 핵심 기술 요소
3.1 Product Schema Definition (PSD)
제품 전체의 공통 규칙과 구조를 정의한 YAML 파일입니다.
역할: 모든 엔드포인트가 따르는 최상위 기준.
구성: UI 매핑 규칙(ui-rules.yaml), 문서화 규칙(table.yaml), 스키마 검증 규칙(schema-validation-rules.yaml) 등을 포함합니다.
3.2 Endpoint Contract Schema (ECS)
특정 API 엔드포인트의 구체적인 입출력 계약을 정의한 JSON Schema입니다.
+1
특징: PSD를 참조하여 필요한 부분만 구체화합니다.
확장 메타데이터: x-ui, x-required-by-type 등을 통해 동적인 UI 동작과 비즈니스 로직을 표현합니다.
3.3 LLM 프롬프트 엔지니어링
LLM이 자연어를 올바른 스키마로 변환하도록 돕는 핵심 기술입니다.
Context: PSD 및 ECS 구조에 대한 지식을 주입합니다.
Instruction: "자연어 시나리오를 분석하여 x-required-by-type 조건을 포함한 ECS 스키마를 생성하라"와 같은 명확한 지시를 내립니다.
4. 기대 효과
기획 속도 향상: 복잡한 스키마 문법을 몰라도 자연어로 기능 기획이 가능해집니다.
개발-기획 간극 해소: 기획 산출물이 곧바로 실행 가능한 스키마가 되어 커뮤니케이션 오류를 줄입니다.
품질 확보: 자동 생성된 테스트 시나리오를 통해 API의 모든 분기 처리를 사전에 검증할 수 있습니다.
문서 최신화: 스키마 변경 시 문서가 자동으로 업데이트되어 항상 최신 상태를 유지합니다.
이 프로세스를 통해 API 개발은