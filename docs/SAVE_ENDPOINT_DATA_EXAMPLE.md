# 엔드포인트 데이터 저장 예제

## 1. 엔드포인트 등록하기

```javascript
// Step 1: 먼저 엔드포인트를 등록
const createEndpoint = async () => {
  const response = await fetch('http://localhost:9527/api/endpoints', {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({
      id: 'db/nlct',
      name: 'NLCT',
      method: 'POST',
      path: '/db/nlct',
      product: 'civil-nx',
      group_name: 'DB',
      description: 'Non-Linear Constraint Table API'
    })
  });
  
  return response.json();
};
```

## 2. 버전과 함께 모든 데이터 저장하기

```javascript
// Step 2: 버전 생성 시 스키마, HTML, 예제 등 모든 데이터 포함
const createVersionWithAllData = async () => {
  const versionData = {
    id: `v_${Date.now()}_${Math.random().toString(36).substr(2, 9)}`,
    version: '1',
    endpointId: 'db/nlct',
    author: 'John Doe',
    changeLog: '초기 버전',
    
    // 📖 Manual 탭 데이터
    manualData: {
      title: 'NLCT - Non-Linear Constraint Table',
      category: 'Database',
      inputUri: '/db/nlct',
      activeMethods: 'POST',
      
      // JSON 스키마 (3가지 버전)
      jsonSchema: JSON.stringify({
        type: 'object',
        properties: {
          name: { type: 'string' },
          values: { type: 'array' }
        }
      }),
      jsonSchemaOriginal: JSON.stringify({ /* 원본 스키마 */ }),
      jsonSchemaEnhanced: JSON.stringify({ /* 향상된 스키마 */ }),
      
      // HTML 메뉴얼
      htmlContent: `
        <div class="manual">
          <h1>NLCT API 매뉴얼</h1>
          <p>이 API는 비선형 제약 테이블을 생성합니다.</p>
          <h2>사용 방법</h2>
          <pre><code>POST /db/nlct</code></pre>
        </div>
      `,
      
      // 예제 응답들
      examples: [
        {
          name: 'Example 1: 기본 사용',
          description: '간단한 NLCT 생성 예제',
          request: JSON.stringify({
            name: 'NLCT1',
            values: [1, 2, 3]
          }),
          response: JSON.stringify({
            success: true,
            id: 'nlct_123'
          })
        },
        {
          name: 'Example 2: 복잡한 케이스',
          description: '다중 값을 포함한 예제',
          request: JSON.stringify({
            name: 'NLCT2',
            values: [[1, 2], [3, 4]]
          }),
          response: JSON.stringify({
            success: true,
            id: 'nlct_456'
          })
        }
      ],
      
      specifications: '상세 스펙 문서...',
      url: 'https://docs.example.com/api/nlct'
    },
    
    // 📄 Spec 탭 데이터
    specData: {
      jsonSchema: JSON.stringify({ /* 스펙 스키마 */ }),
      specifications: '기술 사양...'
    },
    
    // 🏗️ Builder 탭 데이터
    builderData: {
      formData: {
        fields: [
          { name: 'name', type: 'string', required: true },
          { name: 'values', type: 'array', required: true }
        ]
      }
    },
    
    // 🚀 Runner 탭 데이터
    runnerData: {
      requestBody: JSON.stringify({ name: 'test', values: [1, 2, 3] }),
      responseBody: JSON.stringify({ success: true, id: 'nlct_789' }),
      testCases: [
        {
          id: `tc_${Date.now()}`,
          name: 'Test Case 1',
          description: '정상 케이스',
          requestBody: JSON.stringify({ name: 'test1', values: [1, 2] }),
          createdAt: new Date().toISOString(),
          updatedAt: new Date().toISOString()
        }
      ]
    }
  };
  
  const response = await fetch('http://localhost:9527/api/versions', {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify(versionData)
  });
  
  return response.json();
};
```

## 3. 기존 버전 업데이트하기

```javascript
// Step 3: 기존 버전의 데이터 업데이트
const updateVersion = async (versionId) => {
  const updateData = {
    // 필요한 데이터만 보내면 해당 부분만 업데이트됨
    manualData: {
      // HTML 메뉴얼 업데이트
      htmlContent: '<div>업데이트된 메뉴얼...</div>',
      
      // 새로운 예제 추가
      examples: [
        // ... 기존 예제들 + 새 예제
        {
          name: 'Example 3: 새로운 예제',
          request: JSON.stringify({ ... }),
          response: JSON.stringify({ ... })
        }
      ]
    }
  };
  
  const response = await fetch(`http://localhost:9527/api/versions/${versionId}`, {
    method: 'PUT',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify(updateData)
  });
  
  return response.json();
};
```

## 4. 저장된 데이터 조회하기

```javascript
// Step 4: 특정 엔드포인트의 모든 버전 조회
const getVersions = async (endpointId) => {
  const response = await fetch(
    `http://localhost:9527/api/versions?endpoint_id=${endpointId}`
  );
  const versions = await response.json();
  
  // versions 배열의 각 항목에는 모든 데이터가 포함됨:
  // - manualData (스키마, HTML, 예제 등)
  // - specData
  // - builderData
  // - runnerData
  
  return versions;
};

// Step 5: 특정 버전의 상세 데이터 조회
const getVersionDetail = async (versionId) => {
  const response = await fetch(
    `http://localhost:9527/api/versions/${versionId}`
  );
  return response.json();
};
```

## 5. 한번에 여러 엔드포인트 등록하기

```javascript
const seedMultipleEndpoints = async () => {
  const endpoints = [
    {
      id: 'db/node',
      name: 'Node',
      method: 'POST',
      path: '/db/node',
      product: 'civil-nx',
      group_name: 'DB'
    },
    {
      id: 'db/beam',
      name: 'Beam',
      method: 'POST',
      path: '/db/beam',
      product: 'civil-nx',
      group_name: 'DB'
    },
    // ... 더 많은 엔드포인트
  ];
  
  for (const endpoint of endpoints) {
    await fetch('http://localhost:9527/api/endpoints', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify(endpoint)
    });
  }
};
```

## 📊 데이터베이스 구조

```
endpoints (엔드포인트 목록)
  ↓
versions (버전 정보)
  ↓
  ├─ manual_data (메뉴얼: 스키마, HTML, 예제)
  ├─ spec_data (스펙 데이터)
  ├─ builder_data (빌더 데이터)
  ├─ runner_data (러너 데이터)
  └─ test_cases (테스트 케이스)
```

## 🎯 핵심 포인트

1. **먼저 endpoint를 등록** (`POST /api/endpoints`)
2. **그 다음 버전을 생성하면서 모든 데이터 포함** (`POST /api/versions`)
3. **각 버전은 독립적인 스키마, HTML, 예제를 가질 수 있음**
4. **필요할 때 부분 업데이트 가능** (`PUT /api/versions/:id`)

## ✅ 이미 구현된 기능들

- ✅ JSON 스키마 3종 저장 (original, enhanced, normal)
- ✅ HTML 메뉴얼 저장
- ✅ 여러 예제 응답 저장 (배열)
- ✅ 스펙 문서 저장
- ✅ 테스트 케이스 저장
- ✅ Foreign Key로 데이터 무결성 보장
- ✅ CASCADE 삭제로 관련 데이터 자동 정리

