# Manager 테이블 마이그레이션 가이드

## 🔄 Product 컬럼 추가 (2026-01-22)

`manager_tasks` 테이블에 `product` 컬럼이 추가되었습니다.

### 기존 데이터베이스 업데이트 방법

#### 옵션 1: 테이블 재생성 (데이터 삭제됨)

```sql
-- 기존 테이블 삭제
DROP TABLE IF EXISTS manager_tasks;
DROP TABLE IF EXISTS manager_columns;

-- 새 스키마로 재생성
-- server/schema-manager.sql 파일의 전체 내용을 실행
```

#### 옵션 2: 컬럼만 추가 (기존 데이터 유지)

```sql
-- manager_tasks 테이블에 product 컬럼 추가
ALTER TABLE manager_tasks ADD COLUMN IF NOT EXISTS product TEXT;

-- 기존 데이터에 기본값 설정 (선택사항)
UPDATE manager_tasks SET product = '' WHERE product IS NULL;

-- manager_columns 테이블 업데이트
DELETE FROM manager_columns WHERE id = 'product';
INSERT INTO manager_columns (id, label, visible, order_index) 
VALUES ('product', 'Product', true, 0);

-- 기존 컬럼들의 order_index 증가
UPDATE manager_columns SET order_index = order_index + 1 WHERE id != 'product';
```

### Supabase에서 실행하기

1. [Supabase Dashboard](https://app.supabase.com) 접속
2. 프로젝트 선택
3. **SQL Editor** 메뉴 클릭
4. 위의 SQL 중 하나를 선택하여 실행
5. **Run** 버튼 클릭

### 변경 사항 확인

```sql
-- 테이블 구조 확인
SELECT column_name, data_type 
FROM information_schema.columns 
WHERE table_name = 'manager_tasks' 
ORDER BY ordinal_position;

-- 컬럼 설정 확인
SELECT * FROM manager_columns ORDER BY order_index;
```

### 주의사항

- **옵션 1**을 선택하면 기존의 모든 작업 데이터가 삭제됩니다.
- **옵션 2**를 선택하면 기존 데이터를 유지하면서 새 컬럼만 추가됩니다.
- 프로덕션 환경에서는 반드시 백업 후 진행하세요.
