# V3 Behavior Engine - 상세 문서

**Version 3.0 - 완전 동적 시스템**

---

## 📋 목차

1. [개요](#개요)
2. [아키텍처](#아키텍처)
3. [Expression Language](#expression-language)
4. [Action Types](#action-types)
5. [실행 흐름](#실행-흐름)
6. [실제 예제](#실제-예제)
7. [확장 방법](#확장-방법)
8. [성능 고려사항](#성능-고려사항)

---

## 개요

### 목적
**코드 수정 없이 YAML만 수정하여 x-* 필드의 동작 로직까지 정의할 수 있는 완전 동적 시스템**

### 핵심 개념

#### V2의 문제점 ❌
```yaml
# YAML: 선언만 가능
x-enum-by-type:
  renderIn: ["builder"]
```

```typescript
// 코드: 로직 하드코딩 (변경 시 코드 수정 필요!)
if (field['x-enum-by-type']) {
  const enumValues = field['x-enum-by-type'][formValues.TYPE];
  updateEnum(enumValues);
}
```

#### V3의 해결책 ✅
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
```

```typescript
// 코드: 범용 엔진만 (수정 불필요!)
await engine.executeTrigger('dependency-change', context);
```

---

## 아키텍처

### 전체 구조

```
┌─────────────────────────────────────────────────────────────────┐
│                    x-extensions-v3.yaml                          │
│  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━  │
│  extensions:                                                     │
│    - name: "x-enum-by-type"                                      │
│      behaviors:                                                  │
│        - trigger: "dependency-change"                            │
│          when: { field: "TYPE", changed: true }                  │
│          actions:                                                │
│            - type: "update-enum"                                 │
│              source: "self.enumByType[TYPE.value]"               │
└─────────────────────────────────────────────────────────────────┘
                            ↓ loadDefinitions()
┌─────────────────────────────────────────────────────────────────┐
│                      BehaviorEngine                              │
│  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━  │
│                                                                   │
│  extensionDefinitions: Map<string, ExtensionDefinition>          │
│  actionExecutors: Map<string, Function>                          │
│                                                                   │
│  Methods:                                                        │
│  ├─ loadDefinitions(yamlPath)                                    │
│  ├─ executeTrigger(trigger, context, changedField)               │
│  ├─ findAppliedExtensions(field)                                │
│  ├─ checkWhenCondition(when, changedField)                       │
│  └─ executeActions(actions, context)                             │
│                                                                   │
└─────────────────────────────────────────────────────────────────┘
                            ↓ executeTrigger()
┌─────────────────────────────────────────────────────────────────┐
│                   ExpressionEvaluator                            │
│  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━  │
│                                                                   │
│  evaluate(expression, context)                                   │
│    ├─ extractVariables(expression, context)                      │
│    │   ├─ self → field                                           │
│    │   ├─ TYPE → formValues.TYPE                                │
│    │   └─ ...                                                    │
│    └─ new Function(...vars, expression)                          │
│                                                                   │
└─────────────────────────────────────────────────────────────────┘
                            ↓ evaluate()
┌─────────────────────────────────────────────────────────────────┐
│                    ActionExecutors                               │
│  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━  │
│                                                                   │
│  update-enum(action, context)                                    │
│    → newEnum = evaluate(action.source, context)                  │
│    → updateField(fieldName, { enum: newEnum })                   │
│                                                                   │
│  clear-value(action, context)                                    │
│    → shouldClear = evaluate(action.condition, context)           │
│    → if (shouldClear) updateField(fieldName, { value: undefined })│
│                                                                   │
│  validate(action, context)                                       │
│    → for rule in action.rules:                                   │
│        → if evaluate(rule.condition, context):                   │
│            → showMessage(rule.error, 'error')                    │
│                                                                   │
│  calculate, format, show-hint, show-message...                   │
│                                                                   │
└─────────────────────────────────────────────────────────────────┘
```

### 컴포넌트 역할

#### 1. BehaviorEngine
- **역할**: 전체 동작의 오케스트레이터
- **책임**:
  - YAML 정의 로드 및 파싱
  - 트리거 감지 및 실행
  - Extension 찾기
  - Action 실행 관리

#### 2. ExpressionEvaluator
- **역할**: DSL 표현식 평가
- **책임**:
  - 표현식에서 변수 추출
  - 안전한 평가 (Function constructor)
  - 에러 핸들링

#### 3. ActionExecutors
- **역할**: 실제 액션 실행
- **책임**:
  - 내장 액션 구현 (update-enum, validate, etc.)
  - 필드 업데이트
  - UI 업데이트

---

## Expression Language

### 지원 구문

#### 변수 접근
```yaml
self.value              # 현재 필드의 값
self.enum               # 현재 필드의 enum 속성
self.enumByType         # 현재 필드의 x-enum-by-type 속성
self.valueConstraint    # 현재 필드의 x-value-constraint 속성

TYPE.value              # TYPE 필드의 값
WIDTH.value             # WIDTH 필드의 값
{ANY_FIELD}.value       # 모든 필드 접근 가능
```

#### 연산자
```yaml
# 산술 연산자
WIDTH.value * HEIGHT.value
PRICE.value + TAX.value
self.value - 10
self.value / 2
self.value % 3

# 비교 연산자
TYPE.value === 'BEAM'
self.value > 0
self.value <= MAX.value
self.value != null

# 논리 연산자
TYPE.value === 'TENSTR' && self.value < 0
self.value > 0 || self.value < -100
!self.enum.includes(self.value)

# 멤버십 연산자
self.enum.includes(self.value)
['BEAM', 'TRUSS'].includes(TYPE.value)
```

#### 함수 (향후 확장 예정)
```yaml
# 배열 함수
self.enum.length
self.enum.map(x => x * 2)
self.enum.filter(x => x > 0)

# 수학 함수
round(WIDTH.value * 2.54, 2)
abs(self.value)
Math.max(WIDTH.value, HEIGHT.value)

# 문자열 함수
`${self.value} m²`
self.value.toString()
```

### 평가 프로세스

```typescript
// 1. 표현식 정의 (YAML)
source: "self.enumByType[TYPE.value]"

// 2. 변수 추출
{
  self: {
    value: 2,
    enumByType: { BEAM: [1, 2], TRUSS: [1, 2, 3] }
  },
  TYPE: {
    value: 'BEAM'
  }
}

// 3. Function 생성
const func = new Function('self', 'TYPE', 
  '"use strict"; return (self.enumByType[TYPE.value]);'
);

// 4. 실행
func(self, TYPE)  // → [1, 2]
```

### 안전성

- **Sandboxing**: Function constructor 사용으로 격리된 실행 환경
- **Strict Mode**: 'use strict' 자동 적용
- **에러 핸들링**: try-catch로 모든 평가 감싸기
- **타입 체크**: 평가 결과 타입 검증

---

## Action Types

### 1. update-enum

**Enum 옵션을 동적으로 업데이트**

```yaml
- type: "update-enum"
  source: "self.enumByType[TYPE.value]"  # 새 enum 값
  target: "self.enum"                     # 업데이트할 속성
```

**실행 로직**:
```typescript
const newEnum = ExpressionEvaluator.evaluate(action.source, context);
if (Array.isArray(newEnum)) {
  context.updateField(context.fieldName, {
    ...context.field,
    enum: newEnum
  });
}
```

---

### 2. clear-value

**조건에 따라 필드 값을 초기화**

```yaml
- type: "clear-value"
  condition: "!self.enum.includes(self.value)"  # 초기화 조건
```

**실행 로직**:
```typescript
const shouldClear = ExpressionEvaluator.evaluate(action.condition, context);
if (shouldClear) {
  context.updateField(context.fieldName, {
    ...context.field,
    value: undefined
  });
}
```

---

### 3. validate

**검증 규칙을 실행하고 에러 표시**

```yaml
- type: "validate"
  rules:
    - condition: "TYPE.value === 'TENSTR' && self.value > 0"
      error: "TENSTR requires negative value"
      severity: "error"
    - condition: "self.value === 0"
      error: "Value cannot be zero"
      severity: "warning"
```

**실행 로직**:
```typescript
for (const rule of action.rules) {
  const isInvalid = ExpressionEvaluator.evaluate(rule.condition, context);
  if (isInvalid) {
    context.showMessage(rule.error, rule.severity || 'error');
    return;
  }
}
```

---

### 4. calculate

**수식을 계산하여 필드 값 설정**

```yaml
- type: "calculate"
  formula: "WIDTH.value * HEIGHT.value"
  target: "self.value"
```

**실행 로직**:
```typescript
const result = ExpressionEvaluator.evaluate(action.formula, context);
if (action.target === 'self.value') {
  context.updateField(context.fieldName, {
    ...context.field,
    value: result
  });
}
```

---

### 5. format

**값을 포맷팅하여 표시**

```yaml
- type: "format"
  template: "{result} m²"
```

**실행 로직**:
```typescript
const formattedValue = action.template.replace(
  /\{result\}/g,
  String(context.field.value)
);
context.updateField(context.fieldName, {
  ...context.field,
  displayValue: formattedValue
});
```

---

### 6. show-hint

**힌트 메시지를 표시**

```yaml
- type: "show-hint"
  source: "self.valueConstraint[TYPE.value]"
```

**실행 로직**:
```typescript
const message = ExpressionEvaluator.evaluate(action.source, context);
if (message) {
  context.showMessage(message, 'info');
}
```

---

### 7. show-message

**조건부 메시지 표시**

```yaml
- type: "show-message"
  message: "Please select TYPE first"
  condition: "self.enum.length === 0"
```

**실행 로직**:
```typescript
if (action.condition) {
  const shouldShow = ExpressionEvaluator.evaluate(action.condition, context);
  if (!shouldShow) return;
}
context.showMessage(action.message, 'info');
```

---

## 실행 흐름

### 전체 플로우

```
┌─────────────────────────────────────────────────────────────────┐
│ 1. User Action                                                   │
│    TYPE 필드를 "BEAM"으로 변경                                  │
└────────────────────────┬────────────────────────────────────────┘
                         ↓
┌─────────────────────────────────────────────────────────────────┐
│ 2. Event Handler (BuilderTab)                                    │
│    handleFieldChange('TYPE', 'BEAM')                             │
└────────────────────────┬────────────────────────────────────────┘
                         ↓
┌─────────────────────────────────────────────────────────────────┐
│ 3. Behavior Engine Hook                                          │
│    useBehaviorEngine().onFieldChange()                           │
│      ├─ fieldName: 'TYPE'                                        │
│      ├─ field: schema.TYPE                                       │
│      ├─ formValues: { TYPE: 'BEAM', ... }                        │
│      └─ schema: { TYPE: {...}, STYPE: {...}, ... }              │
└────────────────────────┬────────────────────────────────────────┘
                         ↓
┌─────────────────────────────────────────────────────────────────┐
│ 4. Execute Trigger                                               │
│    engine.executeTrigger('value-change', context, 'TYPE')        │
└────────────────────────┬────────────────────────────────────────┘
                         ↓
┌─────────────────────────────────────────────────────────────────┐
│ 5. Find Applied Extensions (STYPE 필드의 extensions)            │
│    findAppliedExtensions(schema.STYPE)                           │
│      → ['x-enum-by-type']                                        │
└────────────────────────┬────────────────────────────────────────┘
                         ↓
┌─────────────────────────────────────────────────────────────────┐
│ 6. Load Behaviors (x-enum-by-type의 behaviors)                  │
│    extensionDefinitions.get('x-enum-by-type').behaviors          │
│      → [{ trigger: 'dependency-change', when: {...}, actions }]  │
└────────────────────────┬────────────────────────────────────────┘
                         ↓
┌─────────────────────────────────────────────────────────────────┐
│ 7. Check When Condition                                          │
│    when: { field: "TYPE", changed: true }                        │
│    changedField === 'TYPE'  → ✅ true                            │
└────────────────────────┬────────────────────────────────────────┘
                         ↓
┌─────────────────────────────────────────────────────────────────┐
│ 8. Execute Actions (순차 실행)                                  │
│                                                                   │
│    Action 1: update-enum                                         │
│    ├─ source: "self.enumByType[TYPE.value]"                      │
│    ├─ ExpressionEvaluator.evaluate()                             │
│    │   ├─ self.enumByType = { BEAM: [1,2], TRUSS: [1,2,3] }     │
│    │   ├─ TYPE.value = 'BEAM'                                    │
│    │   └─ Result: [1, 2]                                         │
│    └─ updateField('STYPE', { enum: [1, 2] })                     │
│                                                                   │
│    Action 2: clear-value                                         │
│    ├─ condition: "!self.enum.includes(self.value)"               │
│    ├─ self.enum = [1, 2]                                         │
│    ├─ self.value = 3                                             │
│    ├─ Result: true (3 is not in [1, 2])                          │
│    └─ updateField('STYPE', { value: undefined })                 │
│                                                                   │
└────────────────────────┬────────────────────────────────────────┘
                         ↓
┌─────────────────────────────────────────────────────────────────┐
│ 9. State Update                                                  │
│    setSchemaFields({ ...schemaFields, STYPE: updatedField })     │
│    setFormValues({ ...formValues, STYPE: undefined })            │
└────────────────────────┬────────────────────────────────────────┘
                         ↓
┌─────────────────────────────────────────────────────────────────┐
│ 10. Component Re-render                                          │
│     <Select> component for STYPE re-renders                      │
│       - Options: [1, 2] (updated!)                               │
│       - Value: undefined (cleared!)                              │
└─────────────────────────────────────────────────────────────────┘
```

### 의존성 변경 감지

```typescript
// BuilderTab.tsx
useEffect(() => {
  if (formValues.TYPE && isEngineReady) {
    // TYPE이 변경되면 STYPE에 대해 dependency-change 트리거
    onDependencyChange(
      'STYPE',                    // 영향받는 필드
      schemaFields.STYPE,         // 필드 정의
      formValues,                 // 전체 폼 값
      schemaFields,               // 전체 스키마
      updateField,                // 업데이트 함수
      showMessage,                // 메시지 표시 함수
      'TYPE'                      // 변경된 필드
    );
  }
}, [formValues.TYPE]);  // TYPE 값 변경 감지
```

---

## 실제 예제

### 예제 1: 타입별 동적 Enum

#### Schema 정의
```json
{
  "TYPE": {
    "type": "string",
    "enum": ["BEAM", "TRUSS", "PLATE"]
  },
  "STYPE": {
    "type": "integer",
    "x-enum-by-type": {
      "BEAM": [1, 2],
      "TRUSS": [1, 2, 3],
      "PLATE": [1, 2, 3, 4]
    }
  }
}
```

#### YAML Behavior 정의
```yaml
extensions:
  - name: "x-enum-by-type"
    internalName: "enumByType"
    type: "Record<string, Array>"
    behaviors:
      - trigger: "dependency-change"
        when:
          field: "TYPE"
          changed: true
        actions:
          - type: "update-enum"
            source: "self.enumByType[TYPE.value]"
            target: "self.enum"
          
          - type: "clear-value"
            condition: "!self.enum.includes(self.value)"
          
          - type: "show-message"
            condition: "self.enum.length === 0"
            message: "Select TYPE first"
```

#### 실행 시나리오

**초기 상태**:
- TYPE: undefined
- STYPE: { enum: [], value: undefined }

**Step 1**: 사용자가 TYPE을 "BEAM"으로 변경
```
Trigger: dependency-change
Field: STYPE
Changed: TYPE

Action 1: update-enum
  - Evaluate: self.enumByType['BEAM']
  - Result: [1, 2]
  - Update: STYPE.enum = [1, 2]

Action 2: clear-value
  - Condition: !self.enum.includes(self.value)
  - Evaluate: ![1, 2].includes(undefined)
  - Result: false (skip)
```

**결과**: STYPE.enum = [1, 2]

**Step 2**: 사용자가 STYPE을 "3"으로 설정

**Step 3**: 사용자가 TYPE을 "TRUSS"로 변경
```
Trigger: dependency-change
Field: STYPE
Changed: TYPE

Action 1: update-enum
  - Evaluate: self.enumByType['TRUSS']
  - Result: [1, 2, 3]
  - Update: STYPE.enum = [1, 2, 3]

Action 2: clear-value
  - Condition: !self.enum.includes(self.value)
  - Evaluate: ![1, 2, 3].includes(3)
  - Result: false (skip, 3 is valid)
```

**결과**: STYPE.enum = [1, 2, 3], STYPE.value = 3 유지

---

### 예제 2: 자동 계산

#### Schema 정의
```json
{
  "WIDTH": { "type": "number" },
  "HEIGHT": { "type": "number" },
  "AREA": {
    "type": "number",
    "x-auto-calculate": {
      "formula": "WIDTH * HEIGHT",
      "dependencies": ["WIDTH", "HEIGHT"],
      "unit": "m²"
    }
  }
}
```

#### YAML Behavior 정의
```yaml
extensions:
  - name: "x-auto-calculate"
    internalName: "autoCalculate"
    type: "object"
    behaviors:
      - trigger: "dependency-change"
        when:
          fields: ["WIDTH", "HEIGHT"]
          changed: true
        actions:
          - type: "calculate"
            formula: "WIDTH.value * HEIGHT.value"
            target: "self.value"
          
          - type: "format"
            template: "{result} m²"
```

#### 실행 시나리오

**Step 1**: WIDTH = 5 입력
```
Trigger: dependency-change (AREA 필드)
Changed: WIDTH

Action 1: calculate
  - Formula: WIDTH.value * HEIGHT.value
  - Evaluate: 5 * undefined = NaN
  - Skip (invalid result)
```

**Step 2**: HEIGHT = 3 입력
```
Trigger: dependency-change (AREA 필드)
Changed: HEIGHT

Action 1: calculate
  - Formula: WIDTH.value * HEIGHT.value
  - Evaluate: 5 * 3 = 15
  - Update: AREA.value = 15

Action 2: format
  - Template: "{result} m²"
  - Result: "15 m²"
  - Update: AREA.displayValue = "15 m²"
```

**결과**: AREA = 15, display = "15 m²"

---

## 확장 방법

### 1. 새 x-* 필드 추가

#### x-extensions-v3.yaml에 정의
```yaml
extensions:
  - name: "x-my-custom-field"
    internalName: "myCustomField"
    type: "object"
    description: "Custom field for special validation"
    
    behaviors:
      - trigger: "value-change"
        when:
          field: "self"
          changed: true
        actions:
          - type: "custom-validate"
            config: "self.myCustomField"
```

#### Schema에 사용
```json
{
  "MY_FIELD": {
    "type": "number",
    "x-my-custom-field": {
      "minValue": 10,
      "maxValue": 100
    }
  }
}
```

#### 끝! 자동으로 파싱되고 렌더링됩니다.

---

### 2. 커스텀 액션 추가

#### 코드에서 한 번만 등록
```typescript
// App.tsx 또는 초기화 코드
import { getBehaviorEngine } from '@/lib/engine/behaviorEngine';

const engine = getBehaviorEngine();

engine.registerCustomAction('custom-validate', async (action, context) => {
  const config = ExpressionEvaluator.evaluate(action.config, context);
  const value = context.field.value;
  
  if (value < config.minValue || value > config.maxValue) {
    context.showMessage(
      `Value must be between ${config.minValue} and ${config.maxValue}`,
      'error'
    );
  }
});
```

#### YAML에서 사용
```yaml
actions:
  - type: "custom-validate"
    config: "self.myCustomField"
```

---

### 3. 플러그인 시스템 (향후 확장)

```yaml
plugins:
  enabled: true
  
  customActions:
    - name: "fetch-remote-data"
      type: "action"
      script: |
        async function execute(context) {
          const url = context.field.apiUrl;
          const data = await fetch(url).then(r => r.json());
          context.updateField('options', data);
        }
```

---

## 성능 고려사항

### 1. 평가 최적화

- **메모이제이션**: 동일한 표현식은 캐싱
- **조건 단락**: AND/OR 연산자는 short-circuit
- **배치 업데이트**: 여러 액션의 결과를 한 번에 적용

### 2. 의존성 추적

```typescript
// 불필요한 트리거 방지
const previousValue = useRef(formValues.TYPE);

useEffect(() => {
  if (formValues.TYPE !== previousValue.current) {
    onDependencyChange(...);
    previousValue.current = formValues.TYPE;
  }
}, [formValues.TYPE]);
```

### 3. 비동기 처리

```typescript
// 액션은 비동기 실행 가능
async executeActions(actions, context) {
  for (const action of actions) {
    await executor(action, context);  // 순차 실행
  }
}
```

### 4. 에러 바운더리

```typescript
try {
  await engine.executeTrigger('value-change', context);
} catch (error) {
  console.error('Behavior execution failed:', error);
  // UI에는 영향 없도록 graceful degradation
}
```

---

## 향후 개선 사항

### 1. 고급 DSL 기능
- 함수 정의 지원
- 로컬 변수 지원
- 루프 및 맵/필터 지원

### 2. 디버깅 도구
- 실행 로그 수집
- 표현식 평가 시각화
- 액션 실행 타임라인

### 3. 성능 최적화
- 표현식 컴파일 캐싱
- 병렬 액션 실행
- 가상 DOM 최적화

### 4. 테스트 프레임워크
- YAML 정의 검증
- 액션 단위 테스트
- 통합 테스트 자동화

---

**Last Updated**: 2026-01-14  
**Version**: 3.0

