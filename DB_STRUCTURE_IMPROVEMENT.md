ㅈㅈㅈㅈㅈㅈㅈㅈㅈㅈㅈㅈㅈㅈㅈㅈㅈㅈㅈㅈㅈㅈㅈㅈㅈㅈㅈㅈㅈㅈㅈㅈㅈㅈㅈㅈㅈㅈㅈㅈㅈㅈ# 🗄️ 데이터베이스 구조 개선 완료

## ✅ 개선 내용

### 1. **엔드포인트 테이블 추가**

새로운 `endpoints` 테이블을 추가하여 엔드포인트 목록을 DB에서 관리합니다.

```sql
CREATE TABLE endpoints (
  id TEXT PRIMARY KEY,              -- 예: "db/nlct", "gen/project"
  name TEXT NOT NULL,               -- 표시 이름: "NLCT", "Project"
  method TEXT NOT NULL,             -- HTTP 메서드
  path TEXT NOT NULL,               -- API 경로: "/db/nlct"
  product TEXT NOT NULL,            -- 제품: "civil-nx", "gen-nx"
  group_name TEXT NOT NULL,         -- 그룹: "DB", "Gen"
  description TEXT,
  status TEXT,
  created_at TEXT NOT NULL,
  updated_at TEXT NOT NULL
);
```

### 2. **계층 구조 지원**

엔드포인트 ID를 경로 형태로 저장하여 자동 분류가 가능합니다:

```
이전: endpoint_id = "nlct"
개선: endpoint_id = "db/nlct"

구조:
civil-nx/
├─ db/
│  ├─ db/node
│  ├─ db/beam
│  ├─ db/nlct
│  └─ db/mvct
└─ gen/
   ├─ gen/project
   └─ gen/material

gen-nx/
└─ analysis/
   ├─ analysis/static
   └─ analysis/dynamic
```

### 3. **새로운 API 엔드포인트**

#### 기본 CRUD
- `GET /api/endpoints` - 모든 엔드포인트 조회
- `GET /api/endpoints/:id` - 특정 엔드포인트 조회
- `POST /api/endpoints` - 새 엔드포인트 생성
- `PUT /api/endpoints/:id` - 엔드포인트 수정
- `DELETE /api/endpoints/:id` - 엔드포인트 삭제

#### 트리 구조
- `GET /api/endpoints/tree` - 계층 구조로 조회

#### 초기화
- `POST /api/endpoints/seed` - 기본 엔드포인트 데이터 생성

### 4. **프론트엔드 개선**

#### useEndpoints 훅 추가
```typescript
const { endpoints, loading, error, refetch } = useEndpoints();
```

#### App.tsx 변경
- ❌ 이전: 하드코딩된 `mockApiData`
- ✅ 개선: DB에서 동적으로 로드

```typescript
// 이전
const mockApiData = [ /* 하드코딩 */ ];

// 개선
const { endpoints: apiData, loading } = useEndpoints();
```

## 🎯 장점

### 1. **자동 분류**
엔드포인트를 추가하면 자동으로 트리 구조에 반영됩니다.

```json
{
  "id": "db/nlct",
  "name": "NLCT",
  "product": "civil-nx",
  "group_name": "DB"
}
```

→ 자동으로 Civil NX > DB > NLCT 구조로 표시

### 2. **동적 관리**
코드 수정 없이 DB에서 엔드포인트를 추가/수정/삭제할 수 있습니다.

### 3. **확장성**
새로운 제품이나 그룹을 추가하기 쉽습니다.

### 4. **데이터 무결성**
Foreign Key로 엔드포인트와 버전 간의 관계를 보장합니다.

## 📝 사용 방법

### 1. 초기 데이터 생성
```bash
curl -X POST http://localhost:9527/api/endpoints/seed
```

### 2. 새 엔드포인트 추가
```bash
curl -X POST http://localhost:9527/api/endpoints \
  -H "Content-Type: application/json" \
  -d '{
    "id": "load/moving",
    "name": "Moving Load",
    "method": "POST",
    "path": "/load/moving",
    "product": "civil-nx",
    "group_name": "Load"
  }'
```

### 3. 트리 구조 조회
```bash
curl http://localhost:9527/api/endpoints/tree
```

## 🔄 마이그레이션

### 기존 데이터 업데이트

기존 버전 데이터의 `endpoint_id`를 경로 형태로 업데이트:

```sql
-- 예시: "nlct" → "db/nlct"로 변경
UPDATE versions 
SET endpoint_id = 'db/' || endpoint_id 
WHERE endpoint_id IN ('node', 'beam', 'nlct', 'mvct');
```

## 📊 데이터 구조 예시

```json
{
  "products": [
    {
      "id": "civil-nx",
      "name": "civil-nx",
      "groups": [
        {
          "id": "db",
          "name": "DB",
          "endpoints": [
            {
              "id": "db/nlct",
              "name": "NLCT",
              "method": "POST",
              "path": "/db/nlct",
              "status": "active"
            }
          ]
        }
      ]
    }
  ]
}
```

---

**업데이트 완료**: 2026-01-08
**주요 개선**: 엔드포인트 계층 구조 지원, 동적 로딩, 자동 분류

