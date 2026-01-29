# Product Schema Definition (PSD) System (2-Level)

제품별로 다른 YAML 스키마 정의 세트를 관리하고 사용할 수 있는 **2-Level 시스템**입니다.

## 💡 핵심 개념

**🎯 2-Level 구조: PSD 세트 → 스키마 타입**

### Level 1: PSD Set (프로젝트별 세트)
- `default` - 기본 세트
- `civil_gen_definition` - Civil/Gen 프로젝트 전용
- `gts_nx_definition` - GTS NX 프로젝트 전용
- 각 PSD 세트는 **독립적인 프로젝트**를 위한 완전한 스키마 묶음

### Level 2: Schema Type (스키마 종류)
- `enhanced` - 고급 기능 (x-* 확장, HTML 지원)
- `manual` - 수동 작성 문서
- `original` - 원본 스키마 (최소 기능)
- 각 타입은 **다른 방식의 스키마 정의**

### 조합 예시
```
제품: Civil NX
  ├─ PSD Set: civil_gen_definition
  └─ Schema Type: enhanced
  
→ 사용: civil_gen_definition/enhanced/builder.yaml
```

## 📁 폴더 구조

```
schema_definitions/
  │
  ├─ default/                   📦 기본 PSD 세트
  │   ├─ enhanced/                🔧 고급 스키마 타입
  │   │   ├─ ui-rules.yaml
  │   │   ├─ builder.yaml
  │   │   ├─ table.yaml
  │   │   ├─ schema-logic.yaml
  │   │   └─ html-template.yaml
  │   ├─ manual/                  📝 수동 문서 타입
  │   │   └─ (마크다운 가이드들)
  │   ├─ original/                📋 원본 스키마 타입
  │   │   ├─ ui-rules.yaml
  │   │   ├─ builder.yaml
  │   │   └─ table.yaml
  │   └─ schema-validation-rules.yaml
  │
  ├─ civil_gen_definition/      📦 Civil/Gen 전용 PSD 세트
  │   ├─ enhanced/
  │   ├─ manual/
  │   ├─ original/
  │   └─ schema-validation-rules.yaml
  │
  └─ gts_nx_definition/         📦 GTS NX 전용 PSD 세트
      ├─ enhanced/
      ├─ manual/
      ├─ original/
      └─ schema-validation-rules.yaml

2-Level: [PSD Set] / [Schema Type] / [YAML Files]
```

## 🚀 사용 방법

### 1. PSD 세트 관리 (2-Level)

#### 좌측 사이드바에서 "PSD" 탭 클릭

- **📦 PSD 세트 목록 (Level 1)**: default, civil_gen_definition, gts_nx_definition
  - 각 세트를 펼치면 **스키마 타입 목록 (Level 2)** 표시
  - enhanced, manual, original 중 선택
  - YAML 파일 클릭 → 내용 보기/편집

#### 새 PSD 세트 생성 (새 프로젝트용)

1. **"PSD 세트 생성"** 버튼 클릭
2. **세트 이름** 입력 (예: `my_project_definition`)
   - 영문자, 숫자, 하이픈(-), 언더스코어(_)만 사용 가능
   - **Level 1 폴더 이름이 됩니다!**
3. **기반 세트** 선택
   - 기존 PSD 세트를 **전체 복제** (추천: `default`)
   - enhanced, manual, original 폴더 모두 복제됨
4. **생성** 클릭
   - → 새 PSD 세트 생성! 📦
   
```
schema_definitions/
  └─ my_project_definition/  ← 새로 생성됨!
      ├─ enhanced/
      ├─ manual/
      └─ original/
```

#### 스키마 세트 삭제

- 각 스키마 세트 카드의 휴지통 아이콘 클릭
- ⚠️ 주의: `enhanced`, `original`, `manual`은 시스템 기본 세트로 삭제 불가

### 2. 제품에 PSD 할당 (2-Level)

#### PSD 탭에서 할당

1. **제품별 PSD 할당** 섹션에서 원하는 제품 찾기
2. **PSD 세트 (Level 1)** 드롭다운 선택
   - 예: `default`, `civil_gen_definition`, `gts_nx_definition`
3. **스키마 타입 (Level 2)** 드롭다운 선택
   - 예: `enhanced`, `manual`, `original`
4. 자동 저장됨

#### 효과 - 2-Level 경로의 YAML 파일이 적용됩니다! 📦

**예: Civil NX 제품에 할당**
```
PSD Set: civil_gen_definition
Schema Type: enhanced
```

**적용되는 YAML 파일:**
- **Builder 탭**: `civil_gen_definition/enhanced/builder.yaml`
- **Spec 탭**: `civil_gen_definition/enhanced/table.yaml`
- **UI 규칙**: `civil_gen_definition/enhanced/ui-rules.yaml`
- **HTML 생성**: `civil_gen_definition/enhanced/html-template.yaml`
- **로직**: `civil_gen_definition/enhanced/schema-logic.yaml`

→ **PSD Set + Schema Type = 완전한 경로!** ✨

#### 다른 조합 예시

| 제품 | PSD Set | Schema Type | 용도 |
|---|---|---|---|
| Civil NX | civil_gen_definition | enhanced | Civil 프로젝트, 고급 기능 |
| Gen NX | civil_gen_definition | manual | Gen 프로젝트, 수동 문서 |
| GTS NX | gts_nx_definition | enhanced | GTS 프로젝트, 고급 기능 |
| Test Product | default | original | 테스트용, 최소 기능 |

### 3. 스키마 세트 커스터마이징

#### 직접 파일 수정

1. `schema_definitions/[세트명]/` 폴더로 이동
2. YAML 파일 직접 수정:
   - `ui-rules.yaml`: UI 렌더링 공통 규칙
   - `builder.yaml`: Builder 탭 설정
   - `table.yaml`: Spec 탭 테이블 설정
   - `schema-logic.yaml`: 로직 규칙
   - `html-template.yaml`: HTML 템플릿
3. 브라우저 새로고침 (F5)
   - 또는 PSD 탭에서 "새로고침" 버튼 클릭

#### 버전 관리

- Git으로 각 스키마 세트를 버전 관리
- 팀원과 공유 가능
- 커스텀 세트를 백업하여 안전하게 관리

## 📝 예시 시나리오

### 시나리오 1: Civil NX 전용 커스텀 UI 세트 만들기

```
목표: Civil NX 제품 전용 완전한 커스텀 세트 생성

1. PSD 탭 열기
2. "스키마 세트 생성" 클릭
3. 이름: civil-nx-custom (📦 새 폴더 생성됨)
4. 기반: enhanced (📦 enhanced 폴더 전체 복제)
5. 생성 완료!
   
   schema_definitions/
     └─ civil-nx-custom/     ← 📦 완전한 세트!
         ├─ ui-rules.yaml    ✓
         ├─ builder.yaml     ✓
         ├─ table.yaml       ✓
         └─ (모든 YAML 파일 포함)

6. PSD 탭 → civil-nx-custom 카드 → builder.yaml 클릭
7. 웹에서 편집 → 저장 → F5
8. Civil NX 제품 → civil-nx-custom 세트 할당
9. 완료! Civil NX만 이 세트의 모든 YAML 적용됨! 🎉
```

### 시나리오 2: 다른 API 프로젝트용 독립 세트

```
목표: 완전히 새로운 프로젝트용 독립 세트

1. "스키마 세트 생성" → 이름: my-new-api
2. 기반: 빈 세트 (또는 enhanced 복제)
3. 생성 완료!
   
   schema_definitions/
     └─ my-new-api/         ← 📦 독립적인 완전한 세트!
         └─ (YAML 파일들)

4. PSD 탭에서 YAML 파일들 클릭하여 편집
5. 새 제품 추가 시 my-new-api 세트 선택
6. 완전히 독립적인 규칙 적용! 🎉
```

### 핵심: 각 폴더 = 완전한 독립 세트! 📦

- `enhanced/` 세트 = enhanced 폴더의 모든 YAML
- `civil-nx-custom/` 세트 = civil-nx-custom 폴더의 모든 YAML
- 제품에 세트 할당 = 그 폴더 전체를 사용!

## 🔧 API Endpoints

### 스키마 세트 목록 조회
```bash
GET /api/schema-definitions
```

### 특정 스키마 세트 조회
```bash
GET /api/schema-definitions/:setName
```

### 스키마 세트 생성
```bash
POST /api/schema-definitions
Content-Type: application/json

{
  "name": "my-custom-set",
  "basedOn": "enhanced"  // optional
}
```

### 스키마 세트 삭제
```bash
DELETE /api/schema-definitions/:setName
```

### 제품 스키마 세트 변경
```bash
PUT /api/products/:productId
Content-Type: application/json

{
  "schema_definition": "civil-nx-custom"
}
```

## 🛡️ 제약사항

- **기본 세트 보호**: `enhanced`, `original`, `manual`은 삭제 불가
- **이름 규칙**: 영문자, 숫자, 하이픈, 언더스코어만 사용
- **HTML 템플릿**: 현재 `enhanced` 세트만 지원

## 💡 팁

1. **점진적 마이그레이션**: 새 스키마 세트를 만들고 테스트 후 프로덕션 제품에 적용
2. **백업**: 커스텀 세트는 정기적으로 백업
3. **문서화**: 각 세트의 `README.md`에 용도와 변경 이력 기록
4. **캐시 관리**: 스키마 변경 후 브라우저 새로고침 필수

## 🎯 로드맵

- [ ] 웹 UI에서 YAML 파일 직접 편집
- [ ] 스키마 세트 간 diff 비교
- [ ] 스키마 검증 도구
- [ ] 스키마 세트 Import/Export (ZIP)
- [ ] 버전 관리 UI

---

**관련 문서:**
- [YAML 스키마 정의 가이드](./README.md)
- [UI Rules 상세 설명](./enhanced/README.md)

