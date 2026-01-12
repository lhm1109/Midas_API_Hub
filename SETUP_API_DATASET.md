# API Verification Platform - 완성된 구조

## ✅ 완료된 작업

### 1. API 데이터셋 폴더 구조 생성 (`api_data_set/`)

```
api_data_set/
├── DB/ (245개)
│   ├── 001_Project_Information/
│   │   ├── product_samples/     # 제품 샘플 파일
│   │   ├── zendesk/            # 001_Project_Information.html
│   │   ├── api_schema/         # API 스키마 정의
│   │   └── test_results/       # 테스트 결과
│   ├── 002_Unit_System/
│   └── ... (243개 더)
├── OPE/ (13개)
│   ├── 001_Project_Status/
│   ├── 005_Line_Beam_Load/
│   └── ...
├── DOC/ (11개)
├── VIEW/ (40개)
└── POST/ (131개)

총 440개 API 엔드포인트
```

### 2. 자동화 스크립트

- **`scripts/setup_api_dataset.js`**: 
  - `api_docs_zendesk/` 폴더를 스캔하여 자동으로 폴더 구조 생성
  - 각 API별로 4개 하위 폴더 생성
  - Zendesk HTML 파일 자동 복사
  - README.md 자동 생성

- **`scripts/generate_api_list.js`**:
  - `api_data_set/` 폴더를 스캔하여 API 목록 생성
  - TypeScript 파일로 export (`src/data/apiEndpoints.ts`)
  - 440개 API 자동 등록

### 3. 애플리케이션 연동

- ✅ `src/data/apiEndpoints.ts`: 440개 실제 API 목록
- ✅ `src/App.tsx`: 실제 API 데이터 사용
- ✅ 사이드바: 카테고리별 그룹화 (DB, OPE, DOC, VIEW, POST)
- ✅ 검색 기능: 440개 API 검색 가능

## 📁 각 엔드포인트 폴더 구성

### product_samples/
- MIDAS 제품 파일(.mcb, .mct)에서 추출한 JSON 데이터
- 실제 사용 패턴 분석
- 파라미터 빈도 통계

### zendesk/
- Zendesk HTML 문서 (자동 복사됨)
- JSON Schema
- Specifications 테이블
- Request/Response Examples

### api_schema/
- `schema.json`: OpenAPI 스키마
- `parameters.json`: 파라미터 상세
- `validation_rules.json`: 검증 규칙
- `discrepancies.json`: 문서-실제 불일치

### test_results/
- 테스트 케이스 및 결과
- Required/Optional 검증
- 통계 및 히스토리

## 🔧 사용 방법

### 1. 새 API 추가 (Zendesk에 새 문서 추가 시)

```bash
node scripts/setup_api_dataset.js
node scripts/generate_api_list.js
```

자동으로:
- `api_data_set/` 에 폴더 생성
- `src/data/apiEndpoints.ts` 업데이트
- 애플리케이션 사이드바에 표시

### 2. 제품 샘플 데이터 추가

```bash
# 예: Line Beam Load 샘플 추가
cp sample.json api_data_set/OPE/005_Line_Beam_Load/product_samples/
```

### 3. API 스키마 생성

Zendesk 문서를 파싱하여 스키마 생성:

```bash
node scripts/parse_zendesk_to_schema.js OPE 005_Line_Beam_Load
```

### 4. 테스트 실행 및 결과 저장

애플리케이션의 "실시간 테스트" 탭에서:
1. 파라미터 설정
2. 테스트 실행
3. 결과 자동 저장 → `test_results/`

## 📊 현재 상태

- **총 API**: 440개
- **카테고리**: 5개
  - DB: 245개
  - OPE: 13개
  - DOC: 11개
  - VIEW: 40개
  - POST: 131개
- **폴더 구조**: 완성 ✅
- **애플리케이션 연동**: 완성 ✅
- **검색 기능**: 작동 ✅

## 🎯 다음 단계

1. ~~Zendesk HTML 파싱하여 스키마 추출~~
2. ~~제품 샘플 파일 수집 및 분석~~
3. ~~실시간 테스트 기능 구현~~
4. ~~테스트 결과 저장 및 시각화~~

## 🚀 실행

```bash
# 개발 서버
npm run dev

# 브라우저
http://localhost:5178
```

왼쪽 사이드바에서 440개 API를 카테고리별로 탐색 가능!







