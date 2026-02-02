# Manager 기능 데이터베이스 설정 가이드

Manager 기능은 API 업무 진행 관리를 위한 작업(tasks)과 컬럼 설정을 Supabase 데이터베이스에 저장합니다.

## ⚠️ 중요: 처음 사용 전 필수 설정

Manager 탭을 사용하기 전에 반드시 아래 설정을 완료해야 합니다.

## 📋 필요한 테이블

- `manager_tasks`: API 작업 데이터
- `manager_columns`: 컬럼 표시/숨김 설정

## 🚀 설정 방법 (5분 소요)

### 1. Supabase 대시보드 접속

1. [Supabase Dashboard](https://app.supabase.com)에 로그인
2. 프로젝트 선택
3. 왼쪽 메뉴에서 **SQL Editor** 클릭

### 2. 스키마 실행

1. `server/schema-manager.sql` 파일의 내용을 복사
2. SQL Editor에 붙여넣기
3. **Run** 버튼 클릭하여 실행

### 3. 테이블 확인

1. 왼쪽 메뉴에서 **Table Editor** 클릭
2. `manager_tasks`와 `manager_columns` 테이블이 생성되었는지 확인
3. `manager_columns` 테이블에 기본 컬럼 데이터(15개)가 삽입되었는지 확인

## 📊 테이블 구조

### manager_tasks

| 컬럼명 | 타입 | 설명 |
|--------|------|------|
| id | TEXT (PK) | 작업 고유 ID |
| product | TEXT | Product Ribbon - Product |
| tab | TEXT | Product Ribbon - Tab |
| group | TEXT | Product Ribbon - Group |
| sub1 | TEXT | Product Ribbon - sub1 |
| sub2 | TEXT | Product Ribbon - sub2 |
| sub3 | TEXT | Product Ribbon - sub3 |
| seg1 | TEXT | API - seg1 |
| seg2 | TEXT | API - seg2 |
| end_point | TEXT | API - End Point |
| mode | TEXT | API - mode |
| plan | TEXT | Pipeline - Plan 상태 |
| dev | TEXT | Pipeline - Dev 상태 |
| vv | TEXT | Pipeline - V&V 상태 |
| doc | TEXT | Pipeline - doc. 상태 |
| deploy | TEXT | Pipeline - 배포 관리 상태 |
| issue | TEXT | Pipeline - Issue 상태 |
| status | TEXT | Pipeline - status |
| charge | TEXT | Pipeline - charge (담당자) |
| remark | TEXT | Pipeline - remark (비고) |
| created_at | TIMESTAMPTZ | 생성 시간 |
| updated_at | TIMESTAMPTZ | 수정 시간 |

### manager_columns

| 컬럼명 | 타입 | 설명 |
|--------|------|------|
| id | TEXT (PK) | 컬럼 고유 ID |
| label | TEXT | 컬럼 표시 이름 |
| visible | BOOLEAN | 표시/숨김 여부 |
| order_index | INTEGER | 정렬 순서 |
| updated_at | TIMESTAMPTZ | 수정 시간 |

## 🔌 API 엔드포인트

### Tasks

- `GET /api/manager/tasks` - 모든 작업 조회
- `GET /api/manager/tasks/:id` - 특정 작업 조회
- `POST /api/manager/tasks` - 새 작업 생성
- `PUT /api/manager/tasks/:id` - 작업 수정
- `DELETE /api/manager/tasks/:id` - 작업 삭제

### Columns

- `GET /api/manager/columns` - 컬럼 설정 조회
- `PUT /api/manager/columns` - 컬럼 설정 전체 업데이트
- `PUT /api/manager/columns/:id/visibility` - 특정 컬럼 표시/숨김 변경

## 🔄 데이터 동기화

모든 사용자가 같은 데이터를 공유합니다:
- 한 사용자가 작업을 추가/수정/삭제하면 서버에 즉시 반영
- 다른 사용자는 페이지를 새로고침하면 최신 데이터 확인 가능
- 컬럼 표시/숨김 설정도 모든 사용자가 공유

## ⚠️ 주의사항

1. **RLS (Row Level Security)**: 현재는 비활성화 상태입니다. 필요시 SQL 파일의 주석을 해제하여 활성화할 수 있습니다.
2. **권한 관리**: 모든 사용자가 읽기/쓰기 권한을 가집니다. 필요시 RLS 정책을 추가하여 권한을 제한할 수 있습니다.
3. **데이터 백업**: 중요한 데이터는 정기적으로 백업하는 것을 권장합니다.

## 🐛 문제 해결

### 테이블이 생성되지 않는 경우

1. Supabase 프로젝트의 데이터베이스 상태 확인
2. SQL 실행 시 에러 메시지 확인
3. 필요시 기존 테이블 삭제 후 재생성:
   ```sql
   DROP TABLE IF EXISTS manager_tasks;
   DROP TABLE IF EXISTS manager_columns;
   ```

### 데이터가 로드되지 않는 경우

1. 브라우저 콘솔에서 네트워크 에러 확인
2. 서버가 정상적으로 실행 중인지 확인 (`http://localhost:9527/health`)
3. `.env` 파일의 Supabase 설정 확인
4. Supabase 대시보드에서 테이블 데이터 직접 확인

## 📝 샘플 데이터 추가 (선택사항)

초기 테스트를 위해 샘플 데이터를 추가하려면:

```sql
INSERT INTO manager_tasks (id, tab, "group", sub1, sub2, sub3, seg1, seg2, end_point, dev, vv, doc, issue, status, charge, remark)
VALUES 
  ('sample-1', 'Tab1', 'Group1', 'Sub1-1', 'Sub1-2', 'Sub1-3', 'Seg1-1', 'Seg1-2', '/api/test', 'green', 'yellow', 'empty', 'empty', 'Working', 'John', 'Test task'),
  ('sample-2', 'Tab2', 'Group2', 'Sub2-1', 'Sub2-2', 'Sub2-3', 'Seg2-1', 'Seg2-2', '/api/demo', 'done', 'green', 'green', 'empty', 'Done', 'Jane', 'Demo task');
```
