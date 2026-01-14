# Enhanced JSON Schema to HTML Table Algorithm

## 개요
Enhanced JSON Schema로부터 HTML 문서 테이블을 자동 생성하는 알고리즘

---

## Phase 1: Schema 분석 (Parse & Analyze)

### 1.1 기본 정보 추출
```javascript
function extractBasicInfo(schema) {
  return {
    title: schema.title,                    // "ELEM"
    baseRequired: schema.required,          // ["TYPE", "MATL", "NODE"]
    transport: schema.properties.TYPE['x-transport']
  };
}
```

### 1.2 필드 목록 추출
```javascript
function extractFields(schema) {
  const fields = [];
  
  for (const [key, value] of Object.entries(schema.properties)) {
    fields.push({
      key: key,
      type: value.type,
      default: value.default || '-',
      enum: value.enum,
      enumLabels: value['x-enum-labels'],
      enumByType: value['x-enum-by-type'],
      enumLabelsByType: value['x-enum-labels-by-type'],
      nodeCountByType: value['x-node-count-by-type'],
      valueConstraint: value['x-value-constraint'],
      ui: value['x-ui'],
      items: value.items,
      minItems: value.minItems,
      maxItems: value.maxItems
    });
  }
  
  return fields;
}
```

### 1.3 조건부 Required 규칙 추출
```javascript
function extractConditionalRequired(schema) {
  const rules = [];
  
  if (!schema.allOf) return rules;
  
  for (const rule of schema.allOf) {
    if (rule.if && rule.then) {
      const condition = rule.if.properties;
      const requiredFields = rule.then.required || [];
      
      rules.push({
        condition: condition,
        required: requiredFields
      });
    }
  }
  
  return rules;
}

// 결과 예시:
// [
//   { condition: { TYPE: { const: "TENSTR" } }, required: ["STYPE"] },
//   { condition: { TYPE: { const: "TENSTR" }, STYPE: { const: 3 } }, required: ["CABLE", "NON_LEN"] },
//   { condition: { TYPE: { const: "WALL" } }, required: ["STYPE", "WALL", "W_CON"] }
// ]
```

---

## Phase 2: 필드 분류 (Classify Fields)

### 2.1 TYPE 값 추출
```javascript
function getElementTypes(schema) {
  return schema.properties.TYPE.enum;
  // ["BEAM", "TRUSS", "TENSTR", "COMPTR", "PLATE", "WALL", "PLSTRS", "PLSTRN", "AXISYM", "SOLID"]
}
```

### 2.2 필드별 Required 상태 계산
```javascript
function calculateRequiredStatus(field, types, baseRequired, conditionalRules) {
  const status = {};
  
  // 1. Base required 체크
  if (baseRequired.includes(field.key)) {
    types.forEach(type => {
      status[type] = 'required';
    });
    return status;
  }
  
  // 2. Conditional required 체크
  types.forEach(type => {
    status[type] = 'optional'; // 기본값
    
    for (const rule of conditionalRules) {
      if (matchesCondition(rule.condition, type) && 
          rule.required.includes(field.key)) {
        status[type] = 'required';
        break;
      }
    }
    
    // 3. visibleWhen 체크 (N/A 판단)
    if (field.ui?.visibleWhen) {
      if (!isVisible(field.ui.visibleWhen, type)) {
        status[type] = 'n/a';
      }
    }
  });
  
  return status;
}

function matchesCondition(condition, type, stype = null) {
  if (condition.TYPE?.const === type) return true;
  if (condition.TYPE?.enum?.includes(type)) return true;
  // STYPE 조건이 있으면 추가 체크 필요
  return false;
}

function isVisible(visibleWhen, type) {
  if (Array.isArray(visibleWhen.TYPE)) {
    return visibleWhen.TYPE.includes(type);
  }
  if (visibleWhen.TYPE === type) {
    return true;
  }
  return false;
}
```

### 2.3 섹션 그룹 결정
```javascript
function determineSection(field, type) {
  // x-ui.group이 있으면 사용
  if (field.ui?.group) {
    return field.ui.group;
  }
  
  // visibleWhen으로 섹션 추론
  const visibleWhen = field.ui?.visibleWhen;
  if (!visibleWhen) return 'Advanced';
  
  const visibleTypes = Array.isArray(visibleWhen.TYPE) 
    ? visibleWhen.TYPE 
    : [visibleWhen.TYPE];
  
  // 패턴 매칭으로 섹션 결정
  if (visibleTypes.includes('TENSTR') && !visibleTypes.includes('COMPTR')) {
    if (visibleWhen.STYPE === 3) return 'Tension only - Cable';
    return 'Tension only';
  }
  if (visibleTypes.includes('COMPTR')) {
    return 'Compression only';
  }
  if (visibleTypes.includes('WALL') && visibleTypes.length === 1) {
    return 'Wall';
  }
  if (visibleTypes.includes('PLATE') && visibleTypes.length === 1) {
    return 'Plate';
  }
  
  return 'Common';
}
```

---

## Phase 3: 섹션별 필드 그룹화

### 3.1 STYPE 기반 섹션 분리
```javascript
function groupFieldsBySections(fields, conditionalRules) {
  const sections = new Map();
  
  // STYPE enum을 가진 TYPE들 파악
  const stypeField = fields.find(f => f.key === 'STYPE');
  const typesWithStype = stypeField?.enumByType 
    ? Object.keys(stypeField.enumByType) 
    : [];
  
  for (const field of fields) {
    const visibleWhen = field.ui?.visibleWhen;
    
    // Case 1: TYPE과 STYPE 조건이 있는 경우
    if (visibleWhen?.TYPE && visibleWhen?.STYPE) {
      const type = visibleWhen.TYPE;
      const stype = visibleWhen.STYPE;
      const stypeLabel = getStypeLabel(type, stype, stypeField);
      const sectionName = `${getTypeLabel(type)} - ${stypeLabel}`;
      
      addToSection(sections, sectionName, field, type, stype);
    }
    // Case 2: TYPE만 있는 경우
    else if (visibleWhen?.TYPE) {
      const types = Array.isArray(visibleWhen.TYPE) 
        ? visibleWhen.TYPE 
        : [visibleWhen.TYPE];
      
      // TYPE이 STYPE을 가지는 경우, 각 STYPE별 섹션 생성
      if (types.length === 1 && typesWithStype.includes(types[0])) {
        const type = types[0];
        const stypes = stypeField.enumByType[type];
        
        for (const stype of stypes) {
          const stypeLabel = getStypeLabel(type, stype, stypeField);
          const sectionName = `${getTypeLabel(type)} - ${stypeLabel}`;
          
          addToSection(sections, sectionName, field, type, stype);
        }
      }
      // 여러 TYPE에 공통인 경우
      else {
        const sectionName = getSectionNameForTypes(types);
        addToSection(sections, sectionName, field);
      }
    }
    // Case 3: visibleWhen이 없는 경우 (전체 공통)
    else {
      addToSection(sections, 'Common Keys and Solid', field);
    }
  }
  
  return sections;
}

function getStypeLabel(type, stype, stypeField) {
  return stypeField.enumLabelsByType?.[type]?.[stype] || `Type ${stype}`;
}

function getTypeLabel(type) {
  const labels = {
    'TENSTR': 'Tension only',
    'COMPTR': 'Compression only',
    'BEAM': 'Beam',
    'TRUSS': 'Truss',
    'PLATE': 'Plate',
    'WALL': 'Wall',
    'PLSTRS': 'Plane Stress',
    'PLSTRN': 'Plane Strain',
    'AXISYM': 'Axisymmetric',
    'SOLID': 'Solid'
  };
  return labels[type] || type;
}

function getSectionNameForTypes(types) {
  // 공통 패턴 감지
  const hasBeam = types.includes('BEAM');
  const hasTruss = types.includes('TRUSS');
  const hasPlstrn = types.includes('PLSTRN');
  const hasAxisym = types.includes('AXISYM');
  
  if (hasBeam && hasTruss && hasPlstrn && hasAxisym) {
    return 'Beam, Truss, Plane Strain, Axisymmetric';
  }
  
  return types.map(getTypeLabel).join(', ');
}
```

### 3.2 섹션 정렬
```javascript
function sortSections(sections) {
  const order = [
    'Common Keys and Solid',
    'Beam, Truss, Plane Strain, Axisymmetric',
    'Tension only - Truss',
    'Tension only - Hook',
    'Tension only - Cable',
    'Compression only - Truss',
    'Compression only - Gap',
    'Wall',
    'Plate',
    'Plane Stress',
    'Advanced'
  ];
  
  return new Map([...sections.entries()].sort((a, b) => {
    const aIndex = order.indexOf(a[0]);
    const bIndex = order.indexOf(b[0]);
    
    if (aIndex === -1 && bIndex === -1) return 0;
    if (aIndex === -1) return 1;
    if (bIndex === -1) return -1;
    
    return aIndex - bIndex;
  }));
}
```

---

## Phase 4: HTML 테이블 생성

### 4.1 테이블 헤더 생성
```javascript
function generateTableHeader() {
  return `
    <thead>
      <tr>
        <th style="width: 6%;">No.</th>
        <th style="width: 35%;">Description</th>
        <th style="width: 14%;">Key</th>
        <th style="width: 10%;">Value Type</th>
        <th style="width: 10%;">Default</th>
        <th style="width: 25%;">Required</th>
      </tr>
    </thead>
  `;
}
```

### 4.2 섹션 헤더 생성
```javascript
function generateSectionHeader(sectionName) {
  return `
    <tr>
      <td colspan="6" class="section-header">${sectionName}</td>
    </tr>
  `;
}
```

### 4.3 필드 행 생성
```javascript
function generateFieldRow(field, rowNumber, requiredStatus) {
  const description = generateDescription(field);
  const required = generateRequiredColumn(field, requiredStatus);
  
  return `
    <tr>
      <td class="centered">${rowNumber}</td>
      <td>${description}</td>
      <td class="field-key">"${field.key}"</td>
      <td class="centered">${formatType(field)}</td>
      <td class="centered">${field.default}</td>
      <td>${required}</td>
    </tr>
  `;
}

function formatType(field) {
  if (field.type === 'array') {
    return `Array<br>[${field.items.type}]`;
  }
  return field.type.charAt(0).toUpperCase() + field.type.slice(1);
}
```

### 4.4 Description 컬럼 생성
```javascript
function generateDescription(field) {
  let html = `<strong>${field.ui?.label || field.key}</strong>`;
  
  // 1. Enum 값 표시
  if (field.enum) {
    html += generateEnumBox(field.enum, field.enumLabels);
  }
  
  if (field.enumByType) {
    html += generateEnumByTypeBox(field.enumByType, field.enumLabelsByType);
  }
  
  // 2. 제약사항 표시
  if (field.valueConstraint) {
    html += generateConstraintBox(field.valueConstraint);
  }
  
  if (field.nodeCountByType) {
    html += generateNodeCountBox(field.nodeCountByType);
  }
  
  if (field.minItems || field.maxItems) {
    html += `<span class="constraint">minItems: ${field.minItems}, maxItems: ${field.maxItems}</span>`;
  }
  
  // 3. Validation 레이어 배지
  html += generateValidationBadges(field);
  
  return html;
}

function generateEnumBox(enumValues, labels) {
  let html = '<div class="enum-values"><strong>Enum Values:</strong><ul class="type-list">';
  
  for (const value of enumValues) {
    const label = labels?.[value] || value;
    html += `<li>${value} - ${label}</li>`;
  }
  
  html += '</ul></div>';
  return html;
}

function generateEnumByTypeBox(enumByType, labelsByType) {
  let html = '<div class="enum-values"><strong>Enum Values by Type:</strong>';
  
  for (const [type, values] of Object.entries(enumByType)) {
    html += `<p><strong>${type}:</strong></p><ul class="type-list">`;
    
    for (const value of values) {
      const label = labelsByType?.[type]?.[value] || value;
      html += `<li>${value} - ${label}</li>`;
    }
    
    html += '</ul>';
  }
  
  html += '</div>';
  return html;
}

function generateConstraintBox(constraints) {
  let html = '<div class="constraint-note">';
  
  for (const [type, constraint] of Object.entries(constraints)) {
    html += `<strong>${type}:</strong> ${constraint}<br>`;
  }
  
  html += '<span class="validation-badge app-validation">APP</span> Validated by application layer';
  html += '</div>';
  return html;
}

function generateNodeCountBox(nodeCounts) {
  let html = '<div class="constraint-note">';
  html += '<strong>Node count by type:</strong><ul class="type-list">';
  
  for (const [type, count] of Object.entries(nodeCounts)) {
    const countStr = Array.isArray(count) ? count.join(' or ') : count;
    html += `<li>${type}: ${countStr} nodes</li>`;
  }
  
  html += '</ul>';
  html += '<span class="validation-badge app-validation">APP</span> Validated by application layer';
  html += '</div>';
  return html;
}

function generateValidationBadges(field) {
  let html = '';
  
  // Standard validation 배지
  if (field.enum || (field.minItems !== undefined) || (field.maxItems !== undefined)) {
    html += '<span class="validation-badge std-validation">STD</span> ';
  }
  
  // Application validation 배지
  if (field.enumByType || field.nodeCountByType || field.valueConstraint) {
    html += '<span class="validation-badge app-validation">APP</span> ';
  }
  
  return html;
}
```

### 4.5 Required 컬럼 생성
```javascript
function generateRequiredColumn(field, requiredStatus) {
  // 모든 TYPE에 대해 동일한 상태인지 확인
  const statuses = Object.values(requiredStatus);
  const uniqueStatuses = [...new Set(statuses)];
  
  if (uniqueStatuses.length === 1) {
    // 모두 동일
    const status = uniqueStatuses[0];
    return `<div class="centered">${capitalizeStatus(status)}</div>`;
  }
  
  // TYPE별로 다른 경우
  let html = '';
  const grouped = groupByStatus(requiredStatus);
  
  for (const [status, types] of Object.entries(grouped)) {
    if (types.length > 0) {
      html += `<strong>${capitalizeStatus(status)}:</strong><ul class="type-list">`;
      types.forEach(type => {
        html += `<li>${type}</li>`;
      });
      html += '</ul>';
    }
  }
  
  return html;
}

function groupByStatus(requiredStatus) {
  const grouped = { required: [], optional: [], 'n/a': [] };
  
  for (const [type, status] of Object.entries(requiredStatus)) {
    if (grouped[status]) {
      grouped[status].push(type);
    }
  }
  
  return grouped;
}

function capitalizeStatus(status) {
  if (status === 'n/a') return 'N/A';
  return status.charAt(0).toUpperCase() + status.slice(1);
}
```

---

## Phase 5: 전체 통합

### 5.1 메인 함수
```javascript
function generateHTMLTable(schema) {
  // Phase 1: 분석
  const basicInfo = extractBasicInfo(schema);
  const fields = extractFields(schema);
  const conditionalRules = extractConditionalRequired(schema);
  const types = getElementTypes(schema);
  
  // Phase 2: 분류
  const fieldsWithStatus = fields.map(field => ({
    ...field,
    requiredStatus: calculateRequiredStatus(
      field, 
      types, 
      basicInfo.baseRequired, 
      conditionalRules
    )
  }));
  
  // Phase 3: 그룹화
  const sections = groupFieldsBySections(fieldsWithStatus, conditionalRules);
  const sortedSections = sortSections(sections);
  
  // Phase 4: HTML 생성
  let html = generateTableHeader();
  html += '<tbody>';
  
  let rowNumber = 1;
  for (const [sectionName, sectionFields] of sortedSections) {
    html += generateSectionHeader(sectionName);
    
    for (const field of sectionFields) {
      html += generateFieldRow(field, rowNumber, field.requiredStatus);
      rowNumber++;
    }
  }
  
  html += '</tbody>';
  
  return html;
}
```

### 5.2 완전한 HTML 문서 생성
```javascript
function generateCompleteHTML(schema) {
  const basicInfo = extractBasicInfo(schema);
  const tableHTML = generateHTMLTable(schema);
  
  return `
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>${basicInfo.title} - Enhanced Schema</title>
    ${generateCSS()}
</head>
<body>
    <h1>${basicInfo.title} API - Enhanced Schema Specification</h1>
    
    ${generateInfoSection(basicInfo)}
    ${generateValidationArchitectureSection()}
    
    <h2>Specifications</h2>
    <table>
        ${tableHTML}
    </table>
    
    ${generateTransportSection(basicInfo.transport)}
</body>
</html>
  `;
}
```

---

## 핵심 알고리즘 요약

### 1. 섹션 결정 로직
```
IF field has x-ui.group:
  USE x-ui.group
ELSE IF field.ui.visibleWhen.TYPE AND field.ui.visibleWhen.STYPE:
  SECTION = "{TYPE Label} - {STYPE Label}"
ELSE IF field.ui.visibleWhen.TYPE (single):
  IF TYPE has STYPE enum:
    CREATE section per STYPE value
  ELSE:
    SECTION = "{TYPE Label}"
ELSE IF field.ui.visibleWhen.TYPE (multiple):
  SECTION = "{TYPE1, TYPE2, ...}"
ELSE:
  SECTION = "Common Keys and Solid"
```

### 2. Required 상태 결정 로직
```
FOR each TYPE:
  IF field in schema.required:
    status[TYPE] = 'required'
  ELSE IF exists allOf rule matching TYPE AND field in rule.required:
    status[TYPE] = 'required'
  ELSE IF field.ui.visibleWhen excludes TYPE:
    status[TYPE] = 'n/a'
  ELSE:
    status[TYPE] = 'optional'
```

### 3. Validation Layer 배지 로직
```
IF field has enum OR minItems OR maxItems:
  ADD "STD" badge (Standard JSON Schema validates this)

IF field has x-enum-by-type OR x-node-count-by-type OR x-value-constraint:
  ADD "APP" badge (Application layer validates this)
```

---

## 구현 예시 (TypeScript)

```typescript
interface EnhancedSchema {
  $schema: string;
  title: string;
  type: string;
  required: string[];
  properties: Record<string, FieldSchema>;
  allOf?: ConditionalRule[];
}

interface FieldSchema {
  type: string;
  enum?: (string | number)[];
  default?: any;
  minItems?: number;
  maxItems?: number;
  items?: { type: string };
  'x-enum-labels'?: Record<string, string>;
  'x-enum-by-type'?: Record<string, (string | number)[]>;
  'x-enum-labels-by-type'?: Record<string, Record<string, string>>;
  'x-node-count-by-type'?: Record<string, number | number[]>;
  'x-value-constraint'?: Record<string, string>;
  'x-ui'?: {
    label?: string;
    group?: string;
    visibleWhen?: {
      TYPE?: string | string[];
      STYPE?: number;
    };
  };
  'x-transport'?: {
    uri: string;
    methods: string[];
    'body-root': string;
  };
}

interface ConditionalRule {
  if: {
    properties: Record<string, { const?: string; enum?: string[] }>;
  };
  then: {
    required: string[];
  };
}

class SchemaToTableGenerator {
  constructor(private schema: EnhancedSchema) {}
  
  generate(): string {
    const fields = this.extractFields();
    const sections = this.groupFieldsBySections(fields);
    return this.generateHTML(sections);
  }
  
  private extractFields(): Field[] {
    // Implementation
  }
  
  private groupFieldsBySections(fields: Field[]): Map<string, Field[]> {
    // Implementation
  }
  
  private generateHTML(sections: Map<string, Field[]>): string {
    // Implementation
  }
}
```

---

## 테스트 케이스

### Test 1: Common Field
```json
{
  "MATL": {
    "type": "integer",
    "x-ui": {
      "label": "Material No.",
      "group": "Common Keys and Solid"
    }
  }
}
```
**Expected:**
- Section: "Common Keys and Solid"
- Required: Required (All types)
- Validation: None

### Test 2: Type-specific Field with Enum
```json
{
  "STYPE": {
    "type": "integer",
    "x-enum-by-type": {
      "TENSTR": [1, 2, 3],
      "COMPTR": [1, 2]
    },
    "x-enum-labels-by-type": {
      "TENSTR": {
        "1": "Truss",
        "2": "Hook",
        "3": "Cable"
      }
    },
    "x-ui": {
      "label": "Element Subtype",
      "visibleWhen": {
        "TYPE": ["TENSTR", "COMPTR", "PLATE", "WALL", "PLSTRS"]
      }
    }
  }
}
```
**Expected:**
- Multiple sections based on TYPE and STYPE combinations
- Enum values displayed with labels
- APP validation badge
- Required when TYPE matches

### Test 3: Conditional Required
```json
{
  "allOf": [
    {
      "if": {
        "properties": {
          "TYPE": { "const": "TENSTR" },
          "STYPE": { "const": 3 }
        }
      },
      "then": {
        "required": ["CABLE", "NON_LEN"]
      }
    }
  ]
}
```
**Expected:**
- CABLE and NON_LEN show as Required only for TENSTR with STYPE=3
- Other combinations show as Optional or N/A

---

## 결론

이 알고리즘을 구현하면:
1. ✅ Enhanced JSON Schema만으로 HTML 테이블 자동 생성
2. ✅ 섹션 구분 자동화
3. ✅ Required/Optional 상태 자동 계산
4. ✅ Validation Layer 자동 표시
5. ✅ 일관된 문서 형식 유지

스키마가 변경되면 자동으로 문서도 업데이트됩니다!

