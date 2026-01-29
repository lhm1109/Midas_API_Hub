# PSC Design Code Option Schema

PSC (Prestressed Concrete) Design Code Option 설정을 위한 Enhanced JSON Schema

## 📋 구조

### 1. Design Code (드롭다운)
- **필드명**: `DESIGN_CODE`
- **타입**: `string` (enum)
- **위젯**: `dropdown`
- **기본값**: `AASHTO-LRFD20`
- **지원 코드**: AASHTO, AISC, Eurocode, BS, IS, IRC, CSA 등

### 2. Input Parameters (입력 파라미터)

#### Tendon Type (라디오 버튼)
- **필드명**: `TENDON_TYPE`
- **옵션**:
  - `LOW_RELAXATION`: Low Relaxation Tendons
  - `STRESS_RELIEVED`: Stress Relieved Tendons
  - `PRESTRESSING_BARS`: Prestressing Bars

#### Exposure Factor for Crack Width (라디오 + 조건부 입력)
- **필드명**: `EXPOSURE_FACTOR_TYPE`
- **옵션**:
  - `CLASS_I`: Class I (1.0)
  - `CLASS_II`: Class II (0.75)
  - `USER`: User (사용자 입력)
- **조건부 필드**: `EXPOSURE_FACTOR_VALUE`
  - `EXPOSURE_FACTOR_TYPE`이 `USER`일 때만 표시
  - 타입: `number` (0.0 ~ 1.0)

#### Corrosive Condition (라디오 버튼)
- **필드명**: `CORROSIVE_CONDITION`
- **옵션**:
  - `SEVERE`: Severe
  - `MODERATE_MILD`: Moderate/Mild

#### Flexural Strength (라디오 버튼)
- **필드명**: `FLEXURAL_STRENGTH`
- **옵션**:
  - `CODE`: Code
  - `STRAIN_COMPATIBILITY`: Strain Compatibility

#### Construction Type (라디오 버튼)
- **필드명**: `CONSTRUCTION_TYPE`
- **옵션**:
  - `SEGMENTAL`: Segmental
  - `NON_SEGMENTAL`: Non-Segmental

### 3. Output Parameters (출력 파라미터)

#### At Construction Stage/Service Loads (체크박스 그룹)
- **필드명**: `OUTPUT_CONSTRUCTION_SERVICE`
- **타입**: `object` (boolean 속성들)
- **체크박스들**:
  - `STRESS_BY_CONSTRUCTION_STAGE`: Stress by Construction Stage
  - `STRESS_BY_SERVICE_LOAD_COMB`: Stress by Service Load Combinations
  - `STRESS_IN_PRESTRESSING_TENDONS`: Stress in Prestressing Tendons
  - `PRINCIPAL_STRESS_BY_CONSTRUCTION_STAGE`: Principal Stress by Construction Stage
  - `PRINCIPAL_STRESS_BY_SERVICE_MAX_SHEAR`: Principal Stress by Service Load Combinations (Max Shear)
  - `PRINCIPAL_STRESS_BY_SERVICE_MAX_TORSION`: Principal Stress by Service Load Combination (Max Torsion)
  - `CRACK_CHECK`: Crack Check

#### At Factored Loads (체크박스 그룹)
- **필드명**: `OUTPUT_FACTORED_LOADS`
- **타입**: `object` (boolean 속성들)
- **체크박스들**:
  - `FLEXURAL_STRENGTH_CHECK`: Flexural Strength Check
  - `SHEAR_STRENGTH_CHECK`: Shear Strength Check
  - `COMBINED_SHEAR_TORSION_CHECK`: Combined Shear and Torsion Check

## 🎨 UI 렌더링

### 그룹 구조
```
General
  └─ Design Code (dropdown)

Input Parameters
  ├─ Tendon Type (radio)
  ├─ Exposure Factor for Crack Width (radio)
  │   └─ Exposure Factor Value (number, 조건부)
  ├─ Corrosive Condition (radio)
  ├─ Flexural Strength (radio)
  └─ Construction Type (radio)

Output Parameters
  ├─ At Construction Stage/Service Loads (group)
  │   ├─ Stress by Construction Stage (checkbox)
  │   ├─ Stress by Service Load Combinations (checkbox)
  │   ├─ Stress in Prestressing Tendons (checkbox)
  │   ├─ Principal Stress by Construction Stage (checkbox)
  │   ├─ Principal Stress by Service Load Combinations (Max Shear) (checkbox)
  │   ├─ Principal Stress by Service Load Combination (Max Torsion) (checkbox)
  │   └─ Crack Check (checkbox)
  └─ At Factored Loads (group)
      ├─ Flexural Strength Check (checkbox)
      ├─ Shear Strength Check (checkbox)
      └─ Combined Shear and Torsion Check (checkbox)
```

## 📝 사용 예시

### JSON 요청 예시
```json
{
  "DESIGN_CODE": "AASHTO-LRFD20",
  "TENDON_TYPE": "LOW_RELAXATION",
  "EXPOSURE_FACTOR_TYPE": "CLASS_I",
  "CORROSIVE_CONDITION": "SEVERE",
  "FLEXURAL_STRENGTH": "CODE",
  "CONSTRUCTION_TYPE": "NON_SEGMENTAL",
  "OUTPUT_CONSTRUCTION_SERVICE": {
    "STRESS_BY_CONSTRUCTION_STAGE": true,
    "STRESS_BY_SERVICE_LOAD_COMB": true,
    "STRESS_IN_PRESTRESSING_TENDONS": true,
    "PRINCIPAL_STRESS_BY_CONSTRUCTION_STAGE": true,
    "PRINCIPAL_STRESS_BY_SERVICE_MAX_SHEAR": true,
    "PRINCIPAL_STRESS_BY_SERVICE_MAX_TORSION": true,
    "CRACK_CHECK": true
  },
  "OUTPUT_FACTORED_LOADS": {
    "FLEXURAL_STRENGTH_CHECK": true,
    "SHEAR_STRENGTH_CHECK": true,
    "COMBINED_SHEAR_TORSION_CHECK": true
  }
}
```

### User 입력 예시
```json
{
  "DESIGN_CODE": "AASHTO-LRFD20",
  "TENDON_TYPE": "LOW_RELAXATION",
  "EXPOSURE_FACTOR_TYPE": "USER",
  "EXPOSURE_FACTOR_VALUE": 0.85,
  "CORROSIVE_CONDITION": "SEVERE",
  "FLEXURAL_STRENGTH": "STRAIN_COMPATIBILITY",
  "CONSTRUCTION_TYPE": "SEGMENTAL",
  "OUTPUT_CONSTRUCTION_SERVICE": {
    "STRESS_BY_CONSTRUCTION_STAGE": true,
    "STRESS_BY_SERVICE_LOAD_COMB": false,
    "STRESS_IN_PRESTRESSING_TENDONS": true,
    "PRINCIPAL_STRESS_BY_CONSTRUCTION_STAGE": true,
    "PRINCIPAL_STRESS_BY_SERVICE_MAX_SHEAR": true,
    "PRINCIPAL_STRESS_BY_SERVICE_MAX_TORSION": false,
    "CRACK_CHECK": true
  },
  "OUTPUT_FACTORED_LOADS": {
    "FLEXURAL_STRENGTH_CHECK": true,
    "SHEAR_STRENGTH_CHECK": true,
    "COMBINED_SHEAR_TORSION_CHECK": false
  }
}
```

## 🔧 Enhanced Schema 확장

### x-ui 확장
- `x-ui.label`: 필드 레이블
- `x-ui.group`: 그룹핑 (General, Input Parameters, Output Parameters)
- `x-ui.widget`: 위젯 타입 (dropdown, radio, checkbox, number, group)
- `x-ui.hint`: 힌트 텍스트
- `x-ui.visibleWhen`: 조건부 표시

### x-enum-labels 확장
- enum 값에 대한 사용자 친화적 레이블

### x-required-when 확장
- 조건부 필수 필드 (EXPOSURE_FACTOR_VALUE는 USER 선택 시 필수)

## 📂 파일 위치

```
api_docs_zendesk/Design/
└── PSC_Design_Code_Option/
    ├── PSC_Design_Code_Option_enhanced.json  (이 파일)
    └── README.md  (이 문서)
```

## 🎯 다음 단계

1. ✅ Enhanced JSON Schema 작성 완료
2. 🔜 UI 렌더링 테스트
3. 🔜 API 엔드포인트 연동
4. 🔜 Validation 규칙 추가
