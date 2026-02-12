# 🤖 자동 문서화 시스템 가이드

## 개요

라우터를 정의하면 **자동으로 API 문서가 생성**됩니다!

```javascript
// 라우터 정의 = 문서 생성
createRoute({
  id: 'db/node',
  name: 'Node',
  method: 'POST',
  path: '/node',
  handler: (req, res) => { ... },
  docs: { ... }  // 문서 정보
});

// ↓ 서버 시작 시 자동으로

// 1. DB에 endpoints 등록
// 2. 문서가 있으면 versions + manual_data 생성
// 3. API Docs 탭에서 확인 가능
```

## 🚀 빠른 시작

### 1. 새 API 추가하기

`server/routes/api.js` 파일을 열고 라우트를 추가합니다:

```javascript
{
  id: 'db/myapi',           // 고유 ID (경로 형태)
  name: 'My API',           // 표시 이름
  method: 'POST',           // HTTP 메서드
  path: '/myapi',           // 엔드포인트 경로
  product: 'civil-nx',      // 제품명
  group: 'DB',              // 그룹명
  description: '간단한 설명',
  
  // 실제 API 로직
  handler: async (req, res) => {
    try {
      const { data } = req.body;
      res.json({ success: true, data });
    } catch (error) {
      res.status(500).json({ error: error.message });
    }
  },
  
  // 📖 문서 (선택사항)
  docs: {
    title: 'My API 문서',
    schema: { ... },
    examples: [ ... ],
    htmlContent: '<div>...</div>'
  }
}
```

### 2. 서버 재시작

```bash
npm run server
```

### 3. 확인

- **API Docs 탭**: 자동으로 문서 생성됨
- **Debug 탭 → Database**: endpoints 테이블에 등록 확인
- **실제 API 호출**: `POST http://localhost:9527/api/civil/db/myapi`

## 📋 라우트 정의 구조

### 필수 필드

```javascript
{
  id: 'group/name',        // 고유 ID (슬래시로 계층 구조)
  name: 'Display Name',    // UI에 표시될 이름
  method: 'POST',          // GET, POST, PUT, DELETE
  path: '/endpoint',       // API 경로
  product: 'civil-nx',     // 제품 (civil-nx, gen-nx 등)
  group: 'DB',             // 그룹 (DB, Gen, Analysis 등)
  handler: (req, res) => { ... }  // Express 핸들러
}
```

### 선택 필드

```javascript
{
  description: 'API 설명',  // 간단한 설명
  
  docs: {
    // 제목
    title: 'API 제목',
    
    // JSON 스키마
    schema: {
      type: 'object',
      properties: {
        id: { type: 'string', description: 'ID' },
        value: { type: 'number' }
      },
      required: ['id']
    },
    
    // 원본 스키마 (선택)
    schemaOriginal: { ... },
    
    // 향상된 스키마 (선택)
    schemaEnhanced: { ... },
    
    // 예제들
    examples: [
      {
        name: '예제 1',
        description: '설명',
        request: { id: '1', value: 100 },
        response: { success: true }
      }
    ],
    
    // HTML 매뉴얼
    htmlContent: `
      <div>
        <h1>상세 매뉴얼</h1>
        <p>...</p>
      </div>
    `,
    
    // 상세 설명
    specifications: '기술 사양...'
  }
}
```

## 🎯 실제 예제

### 예제 1: 간단한 API (문서 없음)

```javascript
{
  id: 'db/simple',
  name: 'Simple API',
  method: 'GET',
  path: '/simple',
  product: 'civil-nx',
  group: 'DB',
  
  handler: (req, res) => {
    res.json({ message: 'Hello!' });
  }
}
```

결과:
- ✅ endpoints 테이블에 등록
- ❌ 문서 없음 (API Docs 탭에서 "문서가 없습니다" 표시)

### 예제 2: 완전한 문서 포함

```javascript
{
  id: 'db/node',
  name: 'Node',
  method: 'POST',
  path: '/node',
  product: 'civil-nx',
  group: 'DB',
  description: 'Create structural nodes',
  
  handler: async (req, res) => {
    const { id, x, y, z } = req.body;
    // 비즈니스 로직...
    res.json({ success: true, data: { id, x, y, z } });
  },
  
  docs: {
    title: 'Node API',
    
    schema: {
      type: 'object',
      properties: {
        id: { type: 'string', description: 'Node ID' },
        x: { type: 'number', description: 'X coordinate' },
        y: { type: 'number', description: 'Y coordinate' },
        z: { type: 'number', description: 'Z coordinate' },
      },
      required: ['id', 'x', 'y', 'z']
    },
    
    examples: [
      {
        name: 'Create node at origin',
        request: { id: 'N1', x: 0, y: 0, z: 0 },
        response: { success: true, data: { id: 'N1', x: 0, y: 0, z: 0 } }
      },
      {
        name: 'Create node at custom position',
        request: { id: 'N2', x: 10, y: 20, z: 5 },
        response: { success: true, data: { id: 'N2', x: 10, y: 20, z: 5 } }
      }
    ],
    
    htmlContent: `
      <div class="api-manual">
        <h1>Node API</h1>
        <p>구조 모델에 노드를 생성합니다.</p>
        
        <h2>사용 방법</h2>
        <pre><code>POST /api/civil/db/node
{
  "id": "N1",
  "x": 0,
  "y": 0,
  "z": 0
}</code></pre>

        <h2>주의사항</h2>
        <ul>
          <li>Node ID는 고유해야 합니다</li>
          <li>좌표는 전역 좌표계 기준입니다</li>
        </ul>
      </div>
    `,
    
    specifications: 'Creates nodes in 3D space for structural modeling.'
  }
}
```

결과:
- ✅ endpoints 테이블에 등록
- ✅ versions 테이블에 v1.0 자동 생성
- ✅ manual_data에 모든 문서 저장
- ✅ API Docs 탭에서 완전한 문서 확인 가능

## 🔄 자동화 흐름

```
1. server/routes/api.js에서 라우트 정의
   ↓
2. registerRoutes()가 라우트 등록
   ↓
3. 서버 시작 시 syncRoutesToDatabase() 실행
   ↓
4. DB에 자동 저장:
   - endpoints 테이블: 엔드포인트 정보
   - versions 테이블: v1.0 자동 생성 (문서 있을 경우)
   - manual_data 테이블: 문서 내용
   ↓
5. API Docs 탭에서 확인 가능
```

## 📊 데이터베이스 구조

```sql
-- 1. 엔드포인트 등록
INSERT INTO endpoints (id, name, method, path, product, group_name, ...)
VALUES ('db/node', 'Node', 'POST', '/node', 'civil-nx', 'DB', ...);

-- 2. 버전 생성 (문서 있을 경우)
INSERT INTO versions (id, version, endpoint_id, author, ...)
VALUES ('v_auto_db_node', '1.0', 'db/node', 'System', ...);

-- 3. 문서 저장
INSERT INTO manual_data (version_id, title, json_schema, examples, html_content, ...)
VALUES ('v_auto_db_node', 'Node API', '{ ... }', '[ ... ]', '<div>...</div>', ...);
```

## 🎨 문서 작성 팁

### 1. 명확한 예제 제공
```javascript
examples: [
  {
    name: '기본 사용법',           // 명확한 제목
    description: '가장 간단한 예제',  // 설명 추가
    request: { ... },
    response: { ... }
  },
  {
    name: '고급 사용법',
    description: '옵션을 포함한 예제',
    request: { ... },
    response: { ... }
  }
]
```

### 2. 구조화된 HTML
```html
<div class="api-manual">
  <h1>API 이름</h1>
  
  <section>
    <h2>개요</h2>
    <p>...</p>
  </section>
  
  <section>
    <h2>사용 방법</h2>
    <pre><code>...</code></pre>
  </section>
  
  <section>
    <h2>주의사항</h2>
    <ul>
      <li>...</li>
    </ul>
  </section>
</div>
```

### 3. 상세한 스키마
```javascript
schema: {
  type: 'object',
  properties: {
    id: { 
      type: 'string', 
      description: 'Unique identifier',
      example: 'N1'
    },
    coordinates: {
      type: 'object',
      properties: {
        x: { type: 'number', description: 'X coordinate' },
        y: { type: 'number', description: 'Y coordinate' },
        z: { type: 'number', description: 'Z coordinate' }
      }
    }
  },
  required: ['id', 'coordinates']
}
```

## 🔧 고급 기능

### 여러 라우트 그룹 관리

```javascript
// server/routes/api.js

const civilDbRoutes = [ ... ];
const civilGenRoutes = [ ... ];
const genAnalysisRoutes = [ ... ];

registerRoutes(router, [
  ...civilDbRoutes,
  ...civilGenRoutes,
  ...genAnalysisRoutes
]);
```

### 라우트 맵 출력

서버 시작 시 자동으로 출력됩니다:

```
📋 Registered API Routes:
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
📦 civil-nx/DB
  POST    /node                          → Node
  POST    /beam                          → Beam
  POST    /nlct                          → NLCT
  POST    /mvct                          → MVCT

📦 civil-nx/Gen
  GET     /project                       → Project
  POST    /material                      → Material

📦 gen-nx/Analysis
  POST    /static                        → Static Analysis
  POST    /dynamic                       → Dynamic Analysis
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
```

## ✅ 체크리스트

새 API를 추가할 때:

- [ ] `id`가 고유한가? (예: `db/myapi`)
- [ ] `method`와 `path`가 올바른가?
- [ ] `handler` 함수가 구현되었는가?
- [ ] `docs.schema`가 정의되었는가?
- [ ] 최소 1개 이상의 `examples`가 있는가?
- [ ] `htmlContent`로 상세 설명을 작성했는가?
- [ ] 서버를 재시작했는가?
- [ ] API Docs 탭에서 확인했는가?

## 🐛 문제 해결

### Q: 라우트를 추가했는데 API Docs에 안 나타납니다
**A**: 서버를 재시작하세요. 라우트 동기화는 서버 시작 시에만 실행됩니다.

### Q: 엔드포인트는 보이는데 문서가 없습니다
**A**: `docs` 객체를 추가하고 서버를 재시작하세요.

### Q: 기존 문서를 업데이트하려면?
**A**: `server/routes/api.js`에서 `docs` 내용을 수정하고 서버 재시작하면 자동으로 업데이트됩니다.

### Q: 수동으로 추가한 문서가 덮어씌워집니다
**A**: 자동 생성된 버전 ID는 `v_auto_*` 형식입니다. 수동 버전은 다른 ID를 사용하세요.

## 📚 관련 문서

- `server/routeRegistry.js` - 라우트 레지스트리 구현
- `server/routes/api.js` - 실제 API 라우트 정의
- `API_DOCS_GUIDE.md` - API 문서 기능 가이드
- `SAVE_ENDPOINT_DATA_EXAMPLE.md` - 수동 데이터 임포트 가이드

---

**핵심**: 라우터 정의 = 문서 생성! 🚀






