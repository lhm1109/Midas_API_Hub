# 📖 API 문서 기능 가이드

## 개요

왼쪽 사이드바의 **API Docs 탭**에서 모든 엔드포인트의 상세 문서를 확인할 수 있습니다.

## 주요 기능

### 1. 📚 문서 보기
- **왼쪽 패널**: 엔드포인트 트리 (Projects 탭과 동일)
- **오른쪽 패널**: 선택된 엔드포인트의 상세 문서

### 2. 📋 탭 구조

#### 📖 Overview
- 기본 정보 (Endpoint ID, 버전, 작성자, 생성일)
- 변경 사항 (Change Log)
- API 정보 (제목, 카테고리, 설명)

#### 📋 Schema
- **JSON Schema**: 요청/응답 데이터 구조
- **Original Schema**: 원본 스키마
- **Enhanced Schema**: 향상된 스키마

#### 💡 Examples
- 여러 예제 요청/응답
- 각 예제의 이름과 설명
- 실제 사용 사례

#### 📄 Manual
- HTML 형식의 상세 매뉴얼
- 사용법, 주의사항 등

### 3. 🔄 버전 선택
- 오른쪽 상단에서 버전 선택 가능
- 각 버전별로 다른 문서 확인 가능

## 사용 시나리오

### 시나리오 1: 새로운 API 확인
```
1. API Docs 탭 클릭
2. 왼쪽 패널에서 엔드포인트 선택 (예: DB > NLCT)
3. Overview 탭에서 기본 정보 확인
4. Schema 탭에서 데이터 구조 확인
5. Examples 탭에서 실제 사용 예제 확인
```

### 시나리오 2: API 변경 이력 추적
```
1. API Docs 탭에서 엔드포인트 선택
2. 버전 드롭다운에서 이전 버전 선택
3. 각 버전의 Change Log 비교
4. Schema 변경사항 확인
```

### 시나리오 3: 개발 중 참조
```
Projects 탭에서 작업 중:
  → API Docs 탭으로 전환하여 스펙 확인
  → 다시 Projects 탭으로 돌아와 개발 계속
```

## 데이터 구조

### API 문서에 포함되는 정보

```typescript
{
  // 기본 정보
  endpointId: "db/nlct",
  version: "1",
  author: "John Doe",
  changeLog: "초기 버전",
  
  // 메뉴얼 데이터
  manualData: {
    title: "NLCT API",
    category: "Database",
    
    // JSON 스키마 (3종)
    jsonSchema: { ... },           // 일반 스키마
    jsonSchemaOriginal: { ... },   // 원본 스키마
    jsonSchemaEnhanced: { ... },   // 향상된 스키마
    
    // HTML 매뉴얼
    htmlContent: "<div>...</div>",
    
    // 상세 설명
    specifications: "...",
    
    // 예제들
    examples: [
      {
        name: "Example 1",
        description: "...",
        request: { ... },
        response: { ... }
      }
    ]
  }
}
```

## 문서 작성 방법

### 방법 1: Debug → Import 탭 사용
```json
{
  "endpoint": {
    "id": "db/sample",
    "name": "Sample API",
    "method": "POST",
    "path": "/db/sample",
    "product": "civil-nx",
    "group_name": "DB"
  },
  "version": {
    "version": "1",
    "author": "Your Name"
  },
  "manual": {
    "title": "Sample API",
    "category": "Database",
    "htmlContent": "<h1>Manual</h1><p>...</p>",
    "specifications": "This API..."
  },
  "schema": {
    "jsonSchema": { ... }
  },
  "examples": [
    {
      "name": "Basic Usage",
      "request": { ... },
      "response": { ... }
    }
  ]
}
```

### 방법 2: 프로그래밍 방식
```typescript
import { importEndpointData } from '@/utils/dataImporter';

await importEndpointData({
  endpoint: { ... },
  version: { ... },
  manual: {
    htmlContent: `
      <div class="api-doc">
        <h1>API 제목</h1>
        <p>설명...</p>
      </div>
    `
  },
  schema: { ... },
  examples: [ ... ]
});
```

### 방법 3: 직접 API 호출
```bash
curl -X POST http://localhost:9527/api/versions \
  -H "Content-Type: application/json" \
  -d '{
    "id": "v_...",
    "version": "1",
    "endpointId": "db/nlct",
    "manualData": { ... }
  }'
```

## 문서 스타일 가이드

### HTML 매뉴얼 작성 시
```html
<div class="manual">
  <h1>API 이름</h1>
  
  <section>
    <h2>개요</h2>
    <p>API에 대한 간단한 설명...</p>
  </section>
  
  <section>
    <h2>사용 방법</h2>
    <pre><code>POST /api/endpoint
{
  "param": "value"
}</code></pre>
  </section>
  
  <section>
    <h2>주의사항</h2>
    <ul>
      <li>주의할 점 1</li>
      <li>주의할 점 2</li>
    </ul>
  </section>
</div>
```

### 예제 작성 시
```json
{
  "name": "명확하고 간결한 제목",
  "description": "이 예제가 보여주는 내용 설명",
  "request": {
    "// 주석으로 설명 추가 가능": "",
    "param1": "value1",
    "param2": "value2"
  },
  "response": {
    "success": true,
    "data": { ... }
  }
}
```

## 비교: Projects vs Docs

| 기능 | Projects 탭 | API Docs 탭 |
|------|------------|-------------|
| 목적 | API 개발 및 테스트 | API 문서 조회 |
| 편집 | ✅ 가능 | ❌ 읽기 전용 |
| 버전 생성 | ✅ 가능 | ❌ 불가능 |
| 테스트 실행 | ✅ 가능 | ❌ 불가능 |
| 문서 보기 | 제한적 | ✅ 전체 |
| 여러 버전 비교 | 제한적 | ✅ 쉬움 |

## 팁

1. **빠른 전환**: Projects ↔ Docs 탭을 오가며 개발과 문서 확인
2. **버전 관리**: 각 버전마다 명확한 Change Log 작성
3. **풍부한 예제**: 다양한 사용 케이스를 예제로 제공
4. **HTML 활용**: 복잡한 설명은 HTML로 구조화
5. **Schema 3종**: 
   - Original: 원본 그대로
   - Normal: 일반 사용
   - Enhanced: 설명 추가된 버전

## 문제 해결

### Q: 엔드포인트를 선택했는데 "문서가 없습니다"라고 나옵니다
**A**: 해당 엔드포인트에 버전이 생성되지 않았습니다.
- Projects 탭에서 버전 생성
- Debug → Import 탭에서 데이터 임포트

### Q: 버전은 있는데 내용이 비어있습니다
**A**: 버전 생성 시 manualData가 포함되지 않았습니다.
- 버전 업데이트 (PUT /api/versions/:id)
- 또는 새 버전 생성

### Q: HTML 매뉴얼이 제대로 표시되지 않습니다
**A**: HTML 형식을 확인하세요.
- 닫는 태그 누락 확인
- 유효한 HTML 구조 확인

---

**더 많은 예제**: `SAVE_ENDPOINT_DATA_EXAMPLE.md` 참조  
**DB 구조**: `DB_STRUCTURE_IMPROVEMENT.md` 참조






