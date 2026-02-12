# 하드코딩된 데이터 정리 가이드

## 📋 개요

이 문서는 API Verification Platform에서 하드코딩된 샘플 데이터를 제거하고, 동적 데이터 관리 시스템으로 전환한 내용을 설명합니다.

**작업 일자**: 2026-01-16  
**버전**: 2.0

---

## 🎯 변경 사항

### 1. PSD 매핑 동적화 ⭐ 주요

**이전**: `src/config/psdMapping.ts`에 제품 ID가 하드코딩됨

```typescript
// ❌ 이전 (하드코딩)
export const PSD_MAPPINGS: Record<string, { psdSet: string; schemaType: string }> = {
  'civil-nx': {
    psdSet: 'civil_gen_definition',
    schemaType: 'enhanced'
  },
  'gts-nx': {
    psdSet: 'gts_nx_definition',
    schemaType: 'enhanced'
  },
  // ...
};
```

**현재**: Supabase `products` 테이블에서 동적으로 가져옴

```typescript
// ✅ 현재 (동적)
// Supabase products 테이블:
// - id: 제품 ID
// - psd_set: PSD 세트명
// - schema_type: 'original' | 'enhanced'

await refreshProductMappings(); // 앱 시작 시 호출
const psd = getPSDForProduct('civil-nx'); // 동적으로 가져옴
```

**장점**:
- ✅ 새 제품 추가 시 코드 수정 불필요
- ✅ DB에서 중앙 집중식 관리
- ✅ 제품별 PSD 설정 변경 가능

---

### 2. API Specs 샘플 데이터 정리

**파일**: `src/data/apiSpecs.ts`

**변경**:
- ⚠️ 경고 주석 추가: 레거시 데이터임을 명시
- 샘플 데이터는 Supabase로 이관
- Fallback 데이터만 유지 (레거시 호환성)

```typescript
/**
 * ⚠️ 이 파일은 레거시 데이터입니다.
 * 새로운 API는 Supabase에서 관리되며, 이 데이터는 fallback으로만 사용됩니다.
 */
export const apiSpecs: Record<string, any> = {
  // 레거시 호환성을 위해 유지 (추후 제거 예정)
  nlct: { /* ... */ },
  mvct: { /* ... */ }
};
```

---

### 3. 샘플 데이터 생성 함수 정리

**파일**: `src/utils/dataImporter.ts`

**변경**:
- `createSampleData()` 함수에 경고 추가
- 테스트/개발 전용임을 명시
- 프로덕션 사용 방지 경고

```typescript
export function createSampleData(): ImportData {
  console.warn('⚠️ createSampleData() is for testing only. Use Import feature for production data.');
  
  return {
    endpoint: {
      id: 'test/sample',  // test 접두사 추가
      name: 'Sample Test API',
      // ...
    },
    // ...
  };
}
```

---

### 4. 샘플 엔드포인트 완전 제거

**파일**: `server/routes/api.js`

**변경**:
- 모든 샘플 라우트 정의 제거 (Node, Beam, NLCT, MVCT 등)
- 빈 템플릿으로 교체
- 새 API 추가 시 참고할 주석 포함

```javascript
// ✅ 깨끗한 상태
const allRoutes = [
  // 여기에 라우트 배열을 추가하세요
];

registerRoutes(router, allRoutes);
```

---

## 🔧 앱 초기화 흐름

### App.tsx 초기화 로직

```typescript
// 🔥 앱 초기화: PSD 매핑 & 스키마 로직 규칙
useEffect(() => {
  async function initializeApp() {
    try {
      // 1. Supabase에서 제품 PSD 매핑 가져오기
      await refreshProductMappings();
      
      // 2. 기본 PSD로 스키마 로직 규칙 초기화
      await initSchemaLogicRules('civil_gen_definition', 'enhanced');
      
      console.log('✅ App initialized successfully');
    } catch (error) {
      console.error('❌ Failed to initialize app:', error);
    }
  }
  
  initializeApp();
}, []);
```

---

## 📊 데이터 관리 흐름

### 이전 (하드코딩)
```
코드 수정 → 빌드 → 배포 → 재시작
```

### 현재 (동적)
```
Supabase DB 수정 → 브라우저 새로고침 (또는 캐시 갱신)
```

---

## 🚀 새 제품 추가 방법

### 1. Supabase에 제품 추가

**방법 A: UI 사용**
1. 프론트엔드 실행
2. 프로젝트 리스트에서 "+ 제품 추가" 클릭
3. 제품 정보 입력:
   - ID: `my-product`
   - Name: `My Product`
   - PSD Set: `civil_gen_definition`
   - Schema Type: `enhanced`

**방법 B: SQL 직접 실행**
```sql
INSERT INTO products (id, name, description, psd_set, schema_type, order_index, created_at, updated_at)
VALUES (
  'my-product',
  'My Product',
  'Product description',
  'civil_gen_definition',
  'enhanced',
  0,
  NOW(),
  NOW()
);
```

**방법 C: API 호출**
```bash
curl -X POST http://localhost:9527/api/products \
  -H "Content-Type: application/json" \
  -d '{
    "id": "my-product",
    "name": "My Product",
    "description": "Product description",
    "psd_set": "civil_gen_definition",
    "schema_type": "enhanced"
  }'
```

### 2. PSD 매핑 갱신

```typescript
// 자동: 앱 시작 시 refreshProductMappings() 호출
// 수동: 필요 시 호출
import { refreshProductMappings } from '@/config/psdMapping';
await refreshProductMappings();
```

---

## 🧪 테스트

### 1. PSD 매핑 확인

```typescript
import { getPSDForProduct } from '@/config/psdMapping';

// 제품 존재
const psd1 = getPSDForProduct('civil-nx');
console.log(psd1); // { psdSet: 'civil_gen_definition', schemaType: 'enhanced' }

// 제품 없음 (기본값)
const psd2 = getPSDForProduct('unknown-product');
console.log(psd2); // { psdSet: 'civil_gen_definition', schemaType: 'enhanced' }
```

### 2. 샘플 엔드포인트 제거 확인

```bash
# 서버 재시작
npm run server

# 엔드포인트 목록 조회
curl http://localhost:9527/api/endpoints/tree

# 예상 결과: 샘플 엔드포인트 없음
```

---

## 📝 주의사항

### 1. 캐시 관리

PSD 매핑은 5분간 캐시됩니다. 즉시 반영하려면:

```typescript
import { refreshProductMappings } from '@/config/psdMapping';
await refreshProductMappings();
```

### 2. 레거시 데이터

`apiSpecs.ts`의 `nlct`, `mvct` 데이터는 레거시 호환성을 위해 유지됩니다.  
추후 모든 데이터가 Supabase로 이전되면 제거 예정입니다.

### 3. 샘플 데이터 함수

`createSampleData()`는 **테스트 전용**입니다.  
프로덕션에서는 Import 기능을 사용하세요.

---

## 🔄 마이그레이션 체크리스트

- [x] `server/routes/api.js` 샘플 라우트 제거
- [x] `server/cleanup-sample-endpoints.js` 실행하여 DB 정리
- [x] `src/config/psdMapping.ts` 동적 로딩으로 변경
- [x] `src/App.tsx` 초기화 로직 추가
- [x] `src/data/apiSpecs.ts` 경고 주석 추가
- [x] `src/utils/dataImporter.ts` 샘플 함수 경고 추가
- [x] 서버 재시작 테스트
- [x] 프론트엔드 새로고침 테스트

---

## 📚 관련 문서

- [API Settings Guide](./API_SETTINGS.md)
- [Supabase Setup Guide](./SUPABASE_SETUP_GUIDE.md)
- [Migration Guide](./MIGRATION_GUIDE_2LEVEL.md)

---

## 💡 FAQ

**Q: 기존 하드코딩된 제품이 보이지 않습니다.**  
A: Supabase `products` 테이블에 제품을 추가하세요. 위의 "새 제품 추가 방법" 참고.

**Q: PSD 매핑이 적용되지 않습니다.**  
A: 브라우저를 새로고침하거나 `refreshProductMappings()`를 호출하세요.

**Q: 샘플 엔드포인트가 계속 생성됩니다.**  
A: `server/routes/api.js`를 확인하고 `allRoutes` 배열이 비어있는지 확인하세요.

**Q: 레거시 데이터는 언제 제거되나요?**  
A: 모든 데이터가 Supabase로 이전되고 안정화된 후 제거 예정입니다.

---

**마지막 업데이트**: 2026-01-16

