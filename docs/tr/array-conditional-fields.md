# 배열 아이템 조건부 필드 (`x-required-when`) 구현 가이드

> **Technical Reference (TR)** | 2026-02-06  
> 배열 내부 필드의 조건부 Required/Visibility 처리

---

## 📋 개요

`x-required-when` 조건이 배열 아이템 내부 필드에 적용될 때의 처리 방법을 설명합니다.

---

## 🔧 구현 포인트

### 1. Schema Compiler (`schemaCompiler.ts`)

배열 items.properties 처리 시 조건부 속성 복사:

```typescript
// Line ~1251
'x-required-when': propDef['x-required-when'],
'x-optional-when': propDef['x-optional-when'],
```

---

### 2. Builder Tab Children 변환 (`BuilderTab.tsx`)

`required` 속성이 객체 형태일 때 처리:

```typescript
// Line ~246
required: typeof child.required === 'boolean' 
  ? child.required 
  : (typeof child.required === 'object' && child.required?.['*'] === 'required'),
```

---

### 3. Dynamic Renderer - Builder UI (`dynamicRenderer.tsx`)

조건 평가 및 Required 별(*) 표시:

```tsx
// Line ~371-396
const shouldShow = Object.entries(condition).every(([key, expectedValue]) => {
  const actualValue = dynamicFormData[key];
  if (typeof expectedValue === 'number') {
    return Number(actualValue) === expectedValue;
  }
  return actualValue === expectedValue;
});
if (!shouldShow) return null;

// Required 별(*) 표시
{child['x-required-when'] && <span className="text-red-400">*</span>}
```

---

### 4. JSON Preview - 배열 아이템 경로 (`BuilderTab.tsx`)

배열 렌더링 시 아이템 경로 전달:

```tsx
// Line ~955-960
const itemPath = key ? `${key}.${idx}` : String(idx);
{renderValue(item, itemPath, depth + 1)}
```

---

### 5. JSON Preview - getFieldMetadata (`BuilderTab.tsx`)

배열 자식의 required 상태 체크:

```typescript
// Line ~797-808
const isRequired = childField.required === true || 
  (typeof childField.required === 'object' && childField.required?.['*'] === 'required');

if (isRequired) {
  return { type: 'required', color: 'text-red-400', label: 'Required' };
}
```

---

### 6. JSON 출력 필터링 (`BuilderTab.tsx`)

`filterConditionalArrayFields` 함수에서 조건 체크:

```typescript
// Line ~1310
const conditionMet = Object.entries(condition).every(([condKey, expectedValue]) => {
  const actualValue = flatData[condKey];
  if (typeof expectedValue === 'number') {
    return Number(actualValue) === expectedValue;
  }
  return actualValue === expectedValue;
});

if (!conditionMet) continue; // 필드 제외
```

---

### 7. Manual Tab 테이블 생성 (`tableGenerator.ts`)

자식 필드 번호를 `parent.child` 형식으로 변경, section-header 처리:

```typescript
// Line ~742-749: rowspan 계산 시 section-header 제외
const nonHeaderChildren = hasChildren 
  ? field.children!.filter(c => c.type !== 'section-header').length 
  : 0;
const rowspanValue = nonHeaderChildren > 0 ? nonHeaderChildren + 1 : 1;

// Line ~775-787: section-header 타입 처리
if (child.type === 'section-header') {
  const sectionLabel = (child as any).section || child.ui?.label || child.key;
  html += `<tr><td style="background-color: #e6fcff;" colspan="7">
    <span style="color: #4c9aff;">${sectionLabel}</span>
  </td></tr>`;
  continue;
}

// Line ~802: 번호 형식 변경
<p>${rowNumber}.${childNo++}</p>
```

**변경 전**: `(1)`, `(2)`, `(3)`  
**변경 후**: `4.1`, `4.2`, `4.3`

---

## 📁 관련 파일

| 파일 | 역할 |
|------|------|
| `schemaCompiler.ts` | 조건부 속성 전달 |
| `dynamicRenderer.tsx` | Builder UI 조건부 렌더링 |
| `BuilderTab.tsx` | JSON Preview, 필터링, 메타데이터 |
| `tableGenerator.ts` | Manual Tab 테이블 생성 |
