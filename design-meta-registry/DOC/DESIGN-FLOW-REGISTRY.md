# Feature Flow Registry

> **"제품·기능·코드별 작업 절차와 입력 요구사항을 탐색·비교·검증할 수 있도록 구조화한 지식 운영 시스템"**

---

## 1. 목적

| 기능 | 설명 |
|------|------|
| **DB 구축** | 작업 절차의 관계형 데이터 |
| **입력 편의** | CSV 임포트, 복붙, 인라인 편집 |
| **결과 확인** | 그래프 뷰, 테이블 뷰, 필터, 비교 |

---

## 2. 데이터 모델

```
Product → Feature Domain → Context Type → Code → Flow → Variant → Step → Parameter
          (Design)         (Steel)       (KDS)
          (Seismic)        (Pier Eval)   (KALIS)
          (Pushover)       (Column)      (ATC-40)
```

---

## 3. DB 스키마

### 기본 테이블

#### products
```sql
CREATE TABLE products (
  id TEXT PRIMARY KEY,      -- 'civil-nx'
  name TEXT NOT NULL
);
```

#### feature_domains ⭐ NEW
```sql
CREATE TABLE feature_domains (
  id TEXT PRIMARY KEY,      -- 'design', 'seismic-perform', 'pushover'
  name TEXT NOT NULL,       -- 'Steel Design', 'Seismic Performance'
  description TEXT
);
```

#### context_types (구 material_types)
```sql
CREATE TABLE context_types (
  id TEXT PRIMARY KEY,      -- 'steel', 'pier-eval', 'column'
  feature_domain_id TEXT,   -- 어떤 도메인에 속하는가
  name TEXT NOT NULL,
  FOREIGN KEY (feature_domain_id) REFERENCES feature_domains(id)
);
```

#### codes (구 design_codes)
```sql
CREATE TABLE codes (
  id TEXT PRIMARY KEY,      -- 'kds-24-14-31-2018', 'kalis-2023'
  name TEXT NOT NULL,
  context_type_id TEXT,
  region TEXT,              -- 'KR', 'US', 'JP'
  FOREIGN KEY (context_type_id) REFERENCES context_types(id)
);
```

---

### Flow 구조

#### flows (구 design_flows)
```sql
CREATE TABLE flows (
  id TEXT PRIMARY KEY,      -- 'civil-nx-design-steel-kds2018'
  product_id TEXT NOT NULL,
  feature_domain_id TEXT NOT NULL,
  context_type_id TEXT NOT NULL,
  code_id TEXT NOT NULL,
  FOREIGN KEY (product_id) REFERENCES products(id),
  FOREIGN KEY (feature_domain_id) REFERENCES feature_domains(id),
  FOREIGN KEY (context_type_id) REFERENCES context_types(id),
  FOREIGN KEY (code_id) REFERENCES codes(id)
);
```

#### flow_variants
```sql
CREATE TABLE flow_variants (
  id TEXT PRIMARY KEY,
  flow_id TEXT NOT NULL,
  variant_type TEXT,        -- 'simple', 'detailed', 'check'
  description TEXT,
  FOREIGN KEY (flow_id) REFERENCES flows(id)
);
```

---

### Step 구조

#### steps
```sql
CREATE TABLE steps (
  id TEXT PRIMARY KEY,
  name TEXT NOT NULL,
  category TEXT,            -- 'input', 'config', 'analysis', 'result'
  step_type TEXT,           -- 'concept', 'ui-helper'
  parent_step_id TEXT       -- ⭐ 상위 Step (메뉴 그룹)
);
```

> **parent_step_id**: Step을 계층 구조로 표현
> - NULL = 최상위 그룹
> - 값 있음 = 하위 입력 패널

#### flow_steps
```sql
CREATE TABLE flow_steps (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  variant_id TEXT NOT NULL,
  step_id TEXT NOT NULL,
  step_order INTEGER,
  execution_phase TEXT,     -- 'pre', 'main', 'post'
  variation_type TEXT,      -- 'common', 'optional', 'additional', 'alternative', 'excluded'
  alternative_group TEXT,
  activation_reason TEXT,   -- ⭐ 'by_code', 'common', 'by_context', 'legacy'
  FOREIGN KEY (variant_id) REFERENCES flow_variants(id),
  FOREIGN KEY (step_id) REFERENCES steps(id)
);
```

> **activation_reason**: 시각화용 - "왜 이 Step이 활성화됐는가"

---

### ⭐ Code별 Step 활성화 규칙 (핵심)

#### code_step_rules
```sql
CREATE TABLE code_step_rules (
  code_id TEXT NOT NULL,
  step_id TEXT NOT NULL,
  rule_type TEXT,           -- 'include', 'exclude', 'optional', 'replace'
  replace_with_step_id TEXT,-- rule_type='replace'일 때
  note TEXT,
  PRIMARY KEY (code_id, step_id),
  FOREIGN KEY (code_id) REFERENCES codes(id),
  FOREIGN KEY (step_id) REFERENCES steps(id)
);
```

> **code_step_rules**: Code 선택 시 Step 자동 솔팅의 핵심
> - `include` - 포함
> - `exclude` - 제외
> - `optional` - 선택
> - `replace` - 다른 Step으로 대체

---

### Parameter 구조

#### parameters
```sql
CREATE TABLE parameters (
  id TEXT PRIMARY KEY,
  name TEXT NOT NULL,
  data_type TEXT,
  unit TEXT
);
```

#### step_parameters
```sql
CREATE TABLE step_parameters (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  step_id TEXT NOT NULL,
  parameter_id TEXT NOT NULL,
  is_required BOOLEAN DEFAULT 0,
  default_value TEXT,
  ui_visible BOOLEAN DEFAULT 1,
  display_order INTEGER,
  FOREIGN KEY (step_id) REFERENCES steps(id),
  FOREIGN KEY (parameter_id) REFERENCES parameters(id)
);
```

---

### Override 테이블

#### step_code_overrides
```sql
CREATE TABLE step_code_overrides (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  step_id TEXT NOT NULL,
  code_id TEXT NOT NULL,
  override_type TEXT,
  note TEXT,
  FOREIGN KEY (step_id) REFERENCES steps(id),
  FOREIGN KEY (code_id) REFERENCES codes(id)
);
```

#### parameter_code_overrides
```sql
CREATE TABLE parameter_code_overrides (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  parameter_id TEXT NOT NULL,
  code_id TEXT NOT NULL,
  override_type TEXT,
  override_value TEXT,
  note TEXT,
  FOREIGN KEY (parameter_id) REFERENCES parameters(id),
  FOREIGN KEY (code_id) REFERENCES codes(id)
);
```

---

## 4. 예시

### Design (Steel + KDS)
```
Product: civil-nx
Feature: design
Context: steel
Code: kds-24-14-31
Steps: Frame → Load → Member → Length → K-Factor → Perform
```

### Seismic Performance (Pier Eval + KALIS)
```
Product: civil-nx
Feature: seismic-perform
Context: pier-eval
Code: kalis-2023
Steps: Bridge Info → Rebar Input → Capacity → Demand → Analysis → Result
```

---

## 5. 입력 편의

| 기능 | 설명 |
|------|------|
| CSV 임포트 | 파일 → 미리보기 → 저장 |
| 복붙 | 엑셀 Ctrl+C → 그리드 Ctrl+V |
| 인라인 편집 | 셀 더블클릭 → 수정 |
| FK 드롭다운 | 연결된 테이블 자동 선택 |

---

## 6. 결과 확인

| 뷰 | 설명 |
|----|------|
| **그래프** | Flow → Step 시각화 |
| **테이블** | 피벗 매트릭스 |
| **비교** | Flow A vs Flow B |
| **필터** | Product/Feature/Context/Code |

---

## 7. 기술 스택

| 영역 | 기술 |
|------|------|
| DB | SQLite |
| API | Express |
| UI | React + Vite |
| 그래프 | Cytoscape.js |
| 테이블 | AG-Grid |

---

## 8. 로드맵

### Phase 1: 기반
- [ ] DB 스키마 생성
- [ ] API 서버
- [ ] 샘플 데이터 (Design-Steel-KDS)

### Phase 2: 입력
- [ ] AG-Grid 편집
- [ ] CSV 임포트

### Phase 3: 확인
- [ ] Cytoscape 그래프
- [ ] 비교 뷰
- [ ] 필터 패널
