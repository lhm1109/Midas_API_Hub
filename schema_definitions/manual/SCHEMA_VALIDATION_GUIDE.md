# 스키마 검증 시스템 사용 가이드

## 개요

YAML 기반 스키마 검증 시스템을 사용하여 Enhanced JSON Schema가 계약 규칙을 준수하는지 자동으로 검증할 수 있습니다.

## 빠른 시작

### 1. 검증 규칙 초기화

```typescript
import { initSchemaValidationRules } from '@/lib/schema/schemaValidator';

// 앱 시작 시 한 번만 실행
await initSchemaValidationRules();
```

### 2. 스키마 검증

```typescript
import { validateSchema, formatValidationResult } from '@/lib/schema/schemaValidator';

// 스키마 검증
const result = validateSchema(mySchema, 'standard');

if (!result.valid) {
  console.log(formatValidationResult(result));
}
```

### 3. 자동 수정 (가능한 경우)

```typescript
import { autoFixSchema } from '@/lib/schema/schemaValidator';

if (result.fixable) {
  const fixedSchema = autoFixSchema(mySchema, result);
  console.log('✅ 스키마가 자동 수정되었습니다!');
}
```

---

## 검증 수준

### strict (엄격)
- mustRules (필수)
- mustNotRules (금지)
- shouldRules (권장)
- bestPractices (베스트 프랙티스)

### standard (표준, 기본값)
- mustRules (필수)
- mustNotRules (금지)
- shouldRules (권장)

### minimal (최소)
- mustRules (필수)
- mustNotRules (금지)

---

## 검증 규칙

### 1. Single Entity Schema (필수) ⭐

**규칙**: 스키마는 단일 엔티티만 정의해야 하며, 래퍼는 `x-transport`로 처리

**자동 수정**: ✅ 가능

**예시**:

```typescript
// ❌ 잘못된 스키마
const badSchema = {
  properties: {
    Argument: {
      properties: {
        TABLE_NAME: { type: 'string' }
      }
    }
  }
};

// 검증
const result = validateSchema(badSchema);
// ❌ 오류: 최상위 properties에 래퍼 객체(Assign, Argument 등)가 있습니다.

// 자동 수정
const fixed = autoFixSchema(badSchema, result);
// ✅ 결과:
// {
//   properties: {
//     TABLE_NAME: { type: 'string' }
//   },
//   x-transport: { body-root: 'Argument' }
// }
```

---

### 2. Required 중첩 금지 (필수)

**규칙**: `required` 배열은 최상위에만 있어야 함

**자동 수정**: ✅ 가능

**예시**:

```typescript
// ❌ 잘못된 스키마
const badSchema = {
  properties: {
    Argument: {
      required: ['TABLE_TYPE'],
      properties: { ... }
    }
  }
};

// 검증 후 자동 수정
// ✅ 결과:
// {
//   required: ['TABLE_TYPE'],
//   properties: { ... }
// }
```

---

### 3. allOf에서 점 표기법 금지 (필수)

**규칙**: `allOf` 조건에서 중첩 경로를 사용하지 않아야 함

**자동 수정**: ✅ 가능

**예시**:

```typescript
// ❌ 잘못된 스키마
const badSchema = {
  allOf: [{
    if: { ... },
    then: {
      required: ['Argument.STAGE_STEP']  // 점 표기법
    }
  }]
};

// 검증 후 자동 수정
// ✅ 결과:
// {
//   allOf: [{
//     if: { ... },
//     then: {
//       required: ['STAGE_STEP']  // 단순 필드명
//     }
//   }]
// }
```

---

### 4. Transport 래퍼 일관성 (필수)

**규칙**: `x-transport.body-root`가 실제 `properties`에 없어야 함

**자동 수정**: ✅ 가능

**예시**:

```typescript
// ❌ 잘못된 스키마
const badSchema = {
  properties: {
    Argument: { properties: { ... } }
  },
  'x-transport': {
    'body-root': 'Argument'  // 중복!
  }
};

// 검증 후 자동 수정
// ✅ 결과:
// {
//   properties: { ... },  // Argument 제거
//   x-transport: { body-root: 'Argument' }
// }
```

---

### 5. 컬렉션 금지 (필수)

**규칙**: 스키마는 배열/컬렉션을 최상위로 가질 수 없음

**자동 수정**: ❌ 수동 수정 필요

**예시**:

```typescript
// ❌ 잘못된 스키마
const badSchema = {
  type: 'array',
  items: {
    type: 'object',
    properties: { ... }
  }
};

// 검증
const result = validateSchema(badSchema);
// ❌ 오류: 최상위 type이 'array'입니다. 스키마는 단일 객체를 정의해야 합니다.

// ✅ 올바른 스키마 (수동 수정)
const goodSchema = {
  type: 'object',
  properties: { ... }
};
```

---

### 6. x-ui에 검증 로직 금지 (필수)

**규칙**: `x-ui`는 UI 메타데이터만 포함

**자동 수정**: ❌ 수동 수정 필요

**예시**:

```typescript
// ❌ 잘못된 스키마
const badSchema = {
  properties: {
    AGE: {
      type: 'integer',
      'x-ui': {
        label: 'Age',
        required: true,     // ❌ 검증 로직
        minimum: 0          // ❌ 검증 로직
      }
    }
  }
};

// ✅ 올바른 스키마
const goodSchema = {
  properties: {
    AGE: {
      type: 'integer',
      minimum: 0,          // ✅ 표준 JSON Schema
      'x-ui': {
        label: 'Age'       // ✅ UI만
      }
    }
  }
};
```

---

## 실전 예제: BeamForceTable 스키마

### 잘못된 스키마

```typescript
const badSchema = {
  "$schema": "http://json-schema.org/draft-07/schema#",
  "title": "BeamForceTable",
  "type": "object",
  "required": ["Argument"],
  "properties": {
    "Argument": {
      "type": "object",
      "required": ["TABLE_TYPE"],
      "properties": {
        "TABLE_NAME": { "type": "string" },
        "TABLE_TYPE": { "type": "string" }
      }
    }
  }
};
```

### 검증 및 수정

```typescript
import { 
  validateSchema, 
  autoFixSchema, 
  formatValidationResult 
} from '@/lib/schema/schemaValidator';

// 1. 검증
const result = validateSchema(badSchema, 'standard');

if (!result.valid) {
  console.log(formatValidationResult(result));
  // 출력:
  // 🚫 오류: Single Entity Schema 원칙
  // 위치: properties.Argument
  // 메시지: 최상위 properties에 래퍼 객체(Assign, Argument 등)가 있습니다.
}

// 2. 자동 수정
const fixedSchema = autoFixSchema(badSchema, result);

// 3. 재검증
const recheck = validateSchema(fixedSchema, 'standard');
console.log(recheck.valid); // true ✅
```

### 수정된 스키마

```json
{
  "$schema": "http://json-schema.org/draft-07/schema#",
  "title": "BeamForceTable",
  "type": "object",
  "required": ["TABLE_TYPE"],
  "properties": {
    "TABLE_NAME": { "type": "string" },
    "TABLE_TYPE": { "type": "string" }
  },
  "x-transport": {
    "uri": "post/TABLE",
    "methods": ["POST"],
    "body-root": "Argument"
  }
}
```

---

## UI 통합

### SpecTab에서 사용

```typescript
// src/features/projects/components/tabs/SpecTab.tsx

import { validateSchema, formatValidationResult } from '@/lib/schema/schemaValidator';
import { toast } from 'sonner';

const handleValidateSchema = () => {
  const result = validateSchema(activeSchema, 'standard');
  
  if (result.valid) {
    toast.success('✅ 스키마가 모든 규칙을 준수합니다!');
  } else {
    toast.error('❌ 스키마 검증 실패');
    console.error(formatValidationResult(result));
    
    if (result.fixable) {
      // 자동 수정 제안
      const shouldFix = confirm('일부 오류는 자동 수정이 가능합니다. 수정하시겠습니까?');
      if (shouldFix) {
        const fixed = autoFixSchema(activeSchema, result);
        updateSpecData(fixed);
        toast.success('✅ 스키마가 자동 수정되었습니다!');
      }
    }
  }
};

// UI에 버튼 추가
<Button onClick={handleValidateSchema}>
  🔍 Validate Schema
</Button>
```

### EndpointDialog에서 사용

```typescript
// src/features/projects/components/EndpointDialog.tsx

import { validateSchema } from '@/lib/schema/schemaValidator';

const handleSubmit = async () => {
  // 스키마 검증
  if (jsonSchemaEnhanced) {
    const result = validateSchema(JSON.parse(jsonSchemaEnhanced), 'minimal');
    if (!result.valid) {
      toast.error('❌ 스키마가 규칙을 준수하지 않습니다.');
      console.error(result.errors);
      return; // 제출 중단
    }
  }
  
  // 제출 진행
  await apiClient.createEndpoint(endpointData);
};
```

---

## 검증 결과 예시

### 성공

```
✅ 스키마 검증 성공
- 0 errors
- 0 warnings
- 2 infos
```

### 실패 (자동 수정 가능)

```
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
🚫 스키마 검증 오류
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

🚫 오류: Single Entity Schema 원칙
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

위치: properties.Argument
메시지: 최상위 properties에 래퍼 객체(Assign, Argument 등)가 있습니다.

설명:
스키마는 단일 엔티티를 정의합니다. 래퍼는 x-transport.body-root로 처리하세요.

❌ 잘못됨:
{
  "properties": {
    "Argument": { "properties": { ... } }
  }
}

✅ 올바름:
{
  "properties": { "TABLE_NAME": { ... } },
  "x-transport": { "body-root": "Argument" }
}

수정 방법:
1. 래퍼 객체의 properties를 최상위로 이동
2. 래퍼 객체의 required를 최상위로 이동
3. x-transport.body-root에 래퍼 이름 추가

참고: schema_definitions/manual/SCHEMA_CONTRACT_KO.md

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

✨ 일부 오류는 자동 수정이 가능합니다.
   autoFixSchema() 함수를 사용하세요.
```

---

## 규칙 커스터마이징

`schema_definitions/schema-validation-rules.yaml` 파일을 수정하여 규칙을 추가/수정/삭제할 수 있습니다.

### 새 규칙 추가 예시

```yaml
mustRules:
  - id: "custom-rule"
    name: "커스텀 규칙"
    description: "우리 팀만의 규칙"
    severity: "error"
    category: "custom"
    
    checks:
      - type: "custom-check"
        message: "커스텀 규칙 위반"
        explanation: "..."
        
        detect:
          conditions:
            - path: "properties.*"
              customLogic: true
```

---

## 참고 문서

- **계약 규칙**: `schema_definitions/manual/SCHEMA_CONTRACT_KO.md`
- **YAML 가이드**: `schema_definitions/manual/YAML_GUIDE_KO.md`
- **검증 규칙 정의**: `schema_definitions/schema-validation-rules.yaml`
- **검증 엔진 코드**: `src/lib/schema/schemaValidator.ts`


