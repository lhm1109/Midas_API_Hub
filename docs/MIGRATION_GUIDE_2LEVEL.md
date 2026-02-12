# PSD 2-Level 구조 마이그레이션 가이드

## 🎯 개요

기존 1-level 구조를 2-level 구조로 마이그레이션합니다.

### 기존 구조 (1-level)
```
schema_definitions/
  ├─ enhanced/      ← 스키마 세트
  ├─ manual/
  └─ original/
```

### 새로운 구조 (2-level)
```
schema_definitions/
  ├─ default/                   ← PSD 세트 (Level 1)
  │   ├─ enhanced/                ← 스키마 타입 (Level 2)
  │   ├─ manual/
  │   └─ original/
  ├─ civil_gen_definition/      ← PSD 세트 (Level 1)
  │   ├─ enhanced/
  │   ├─ manual/
  │   └─ original/
  └─ gts_nx_definition/         ← PSD 세트 (Level 1)
      ├─ enhanced/
      ├─ manual/
      └─ original/
```

## ✅ 완료된 작업

### 1. 폴더 구조 마이그레이션
- ✅ `schema_definitions/default/` 폴더 생성
- ✅ 기존 파일들 `default/`로 이동
- ✅ `civil_gen_definition/`, `gts_nx_definition/` 폴더 생성 및 복제

### 2. Backend API 수정
- ✅ `GET /api/schema-definitions` - PSD 세트 목록
- ✅ `GET /api/schema-definitions/:setName` - 스키마 타입 목록
- ✅ `GET /api/schema-definitions/:setName/:schemaType/:fileName` - YAML 파일 조회
- ✅ `PUT /api/schema-definitions/:setName/:schemaType/:fileName` - YAML 파일 수정
- ✅ `POST /api/schema-definitions` - PSD 세트 생성
- ✅ `DELETE /api/schema-definitions/:setName` - PSD 세트 삭제

### 3. Products API 수정
- ✅ `server/routes/products.js` 수정
  - `schema_definition` → `psd_set` + `schema_type`
  - POST, PUT API 업데이트

### 4. TypeScript 타입 수정
- ✅ `src/types/index.ts` - `ApiProduct` 인터페이스 업데이트

## 🔧 수동 작업 필요

### Step 1: Supabase DB 마이그레이션

**Supabase SQL Editor**에서 다음 SQL을 실행하세요:

```sql
-- 1. 기존 schema_definition 필드를 psd_set으로 변경
ALTER TABLE products 
RENAME COLUMN schema_definition TO psd_set;

-- 2. schema_type 필드 추가 (기본값: enhanced)
ALTER TABLE products 
ADD COLUMN schema_type TEXT DEFAULT 'enhanced';

-- 3. 기존 데이터 마이그레이션
-- psd_set을 'default'로 설정 (기존 모든 제품)
UPDATE products 
SET psd_set = 'default' 
WHERE psd_set IN ('enhanced', 'original', 'manual') OR psd_set IS NULL;

-- 4. 인덱스 추가
CREATE INDEX IF NOT EXISTS idx_products_psd_set ON products(psd_set);
CREATE INDEX IF NOT EXISTS idx_products_schema_type ON products(schema_type);

-- 5. 확인
SELECT id, name, psd_set, schema_type FROM products;
```

### Step 2: 서버 재시작

```bash
# 터미널에서
cd server
npm run dev

# 또는 전체 재시작
npm run dev:all
```

### Step 3: 브라우저 새로고침

```
F5 또는 Ctrl + R
```

## 📋 아직 완료되지 않은 작업

### 1. Frontend SchemaView 수정 (복잡)
- ⏳ PSD 세트 선택 UI
- ⏳ 스키마 타입 선택 UI
- ⏳ 3-depth 트리 구조 (세트 → 타입 → 파일)

### 2. definitionLoader 수정
- ⏳ `loadUIRules(psdSet, schemaType)`
- ⏳ `loadBuilderRules(psdSet, schemaType)`
- ⏳ `loadTableRules(psdSet, schemaType)`

### 3. 제품별 PSD 할당 UI
- ⏳ 두 개의 드롭다운 (PSD 세트 + 스키마 타입)
- ⏳ 제품 생성/수정 다이얼로그 업데이트

## 🎯 사용 예시

### API 호출 (새로운 구조)

```javascript
// PSD 세트 목록 조회
GET /api/schema-definitions
→ [
  { id: 'default', schemaTypes: ['enhanced', 'manual', 'original'] },
  { id: 'civil_gen_definition', schemaTypes: [...] },
  { id: 'gts_nx_definition', schemaTypes: [...] }
]

// 특정 YAML 파일 조회
GET /api/schema-definitions/default/enhanced/builder.yaml
→ { content: "...", path: "default/enhanced/builder.yaml" }

// 제품 생성 (새로운 필드)
POST /api/products
{
  "id": "civil-nx",
  "name": "Civil NX",
  "psd_set": "civil_gen_definition",
  "schema_type": "enhanced"
}
```

### 제품 설정 의미

```
제품: Civil NX
├─ psd_set: civil_gen_definition
└─ schema_type: enhanced

→ 사용되는 YAML 경로:
  - schema_definitions/civil_gen_definition/enhanced/ui-rules.yaml
  - schema_definitions/civil_gen_definition/enhanced/builder.yaml
  - schema_definitions/civil_gen_definition/enhanced/table.yaml
```

## 🚀 다음 단계

1. ✅ SQL 마이그레이션 실행
2. ✅ 서버 재시작
3. ⏳ Frontend 수정 (SchemaView.tsx)
4. ⏳ definitionLoader 수정
5. ⏳ 테스트

## 📞 문의

구조 변경에 대한 질문이나 문제가 있으면 알려주세요!

---

**작성일**: 2026-01-14  
**버전**: 2.0 (2-Level PSD Structure)



