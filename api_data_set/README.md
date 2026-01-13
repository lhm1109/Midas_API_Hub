# API Data Set

엔드포인트별로 관리되는 API 검증 데이터 저장소

## 📁 폴더 구조

```
api_data_set/
├── DB/              # Database 관련 API
│   ├── 001_Project_Information/
│   │   ├── product_samples/    # 제품 샘플 파일 (.mcb, .json 등)
│   │   ├── zendesk/            # Zendesk 공식 문서
│   │   ├── api_schema/         # API 스키마 정의
│   │   └── test_results/       # 테스트 결과
│   ├── 002_Unit_System/
│   └── ...
├── OPE/             # Operation 관련 API
│   ├── 001_Project_Status/
│   ├── 005_Line_Beam_Load/
│   └── ...
├── DOC/             # Document 관련 API
│   ├── 001_New_Project/
│   └── ...
├── VIEW/            # View 관련 API
└── POST/            # Post-processing 관련 API
```

## 🎯 각 엔드포인트 폴더 구성

### 1. product_samples/
MIDAS 제품에서 추출한 실제 파일 데이터

- `.mcb` 파일에서 추출한 JSON 데이터
- 실제 사용 패턴 분석 결과
- 파라미터 빈도 통계

### 2. zendesk/
Zendesk 공식 문서

- 원본 HTML 파일
- 파싱된 JSON Schema
- Specifications 테이블
- Request/Response Examples

### 3. api_schema/
API 스키마 정의

- `schema.json`: OpenAPI 형식 스키마
- `parameters.json`: 파라미터 상세 정보
- `validation_rules.json`: 검증 규칙
- `discrepancies.json`: 문서와 실제의 차이점

### 4. test_results/
API 테스트 결과 및 검증 데이터

- 테스트 케이스 및 결과
- Required/Optional 검증 결과
- 성공/실패 통계
- 타임스탬프별 히스토리

## 🔧 사용 방법

### 1. 새 엔드포인트 추가

```bash
node scripts/setup_api_dataset.js
```

자동으로 `api_docs_zendesk/` 폴더를 스캔하여 구조 생성

### 2. 데이터 수집

1. **제품 샘플**: MIDAS 제품 파일을 열어 해당 API 데이터 추출
2. **Zendesk 문서**: 자동으로 복사됨 (스크립트 실행 시)
3. **API 스키마**: Zendesk 문서를 파싱하여 생성
4. **테스트 결과**: 실시간 테스트 실행 후 저장

### 3. 애플리케이션에서 사용

```typescript
// API 데이터 로드
const apiData = loadApiData('DB', '001_Project_Information');

// 제품 샘플 파일 읽기
const samples = apiData.productSamples;

// Zendesk 문서 읽기
const docs = apiData.zendesk;

// API 스키마 읽기
const schema = apiData.schema;

// 테스트 결과 읽기
const testResults = apiData.testResults;
```

## 📊 통계

- **총 API 개수**: ~500개
- **카테고리**: 5개 (DB, OPE, DOC, VIEW, POST)
- **DB**: 246개
- **OPE**: 13개
- **DOC**: 11개
- **VIEW**: 40개
- **POST**: 131개

## 🔄 자동 업데이트

`api_docs_zendesk/` 폴더가 업데이트되면:

```bash
node scripts/setup_api_dataset.js
```

실행하여 새로운 API 폴더 자동 생성









