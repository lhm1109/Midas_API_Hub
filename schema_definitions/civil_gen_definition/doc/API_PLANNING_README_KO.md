# API 기획 문서 세트

이 폴더의 기존 문서는 스키마 작성자와 검증자를 위한 기술 문서가 많다. 새 API를 처음 기획하는 사람에게는 YAML 규칙을 직접 읽히기보다, 아래 순서의 기획자용 문서를 전달한다.

## 읽는 순서

| 순서 | 문서 | 용도 |
|---:|---|---|
| 1 | [API_PLANNING_GUIDE_KO.md](API_PLANNING_GUIDE_KO.md) | 우리 API의 공통 구조, wrapper, field naming, 조건부 필드, UI-only 요소 구분을 설명한다. |
| 2 | [API_PLANNING_TEMPLATE_KO.md](API_PLANNING_TEMPLATE_KO.md) | 새 API마다 복사해서 작성하는 표준 기획서 양식이다. |
| 3 | [API_PLANNING_CHECKLIST_KO.md](API_PLANNING_CHECKLIST_KO.md) | 기획 완료 전에 누락 여부를 확인하는 체크리스트다. |
| 4 | [API_PLANNING_EXAMPLE_MATD_KO.md](API_PLANNING_EXAMPLE_MATD_KO.md) | `DESIGN/PSC/AASHTO-LRFD24/MATD` 같은 Design 계열 API를 어떤 수준으로 작성해야 하는지 보여주는 예시다. |

## 이 문서 세트의 목적

기획자는 JSON Schema나 MCP 저장 규칙을 모두 알 필요는 없다. 대신 다음 정보를 빠짐없이 결정해야 한다.

| 결정 항목 | 기획서에 필요한 내용 |
|---|---|
| API 목적 | API가 어떤 업무를 수행하는지 |
| Endpoint / Method | 실제 호출 경로와 지원 method |
| 요청 body 구조 | collection인지 single인지, wrapper가 무엇인지 |
| Field 정의 | key, label, type, required, default, enum, 조건 |
| UI/API 구분 | API field와 단순 UI 조작 요소의 구분 |
| 예제 payload | 기존 API 형식에 맞는 실제 요청 예제 |

이 정보가 충분하면 스키마 작성자는 기존 `enhanced` 규칙에 맞춰 JSON Schema, Builder UI, Manual 문서를 일관되게 만들 수 있다.

## 기준 파일

| 파일 | 기준 내용 |
|---|---|
| `schema_definitions/civil_gen_definition/enhanced/shared.yaml` | wrapper, component, table API 공통 필드의 기준 |
| `schema_definitions/civil_gen_definition/enhanced/promptRules.yaml` | table/settings 구분, entity type 선택, naming, field order, 질문 정책 |
| `schema_definitions/civil_gen_definition/enhanced/ui.yaml` | label, hint, group, conditional visibility 같은 UI 표현 규칙 |
| `schema_definitions/civil_gen_definition/schema-validation-rules.yaml` | wrapper 중복, required 위치, x-ui 검증 로직 금지 같은 검증 규칙 |

## 작성 원칙

| 원칙 | 기준 |
|---|---|
| 추측 제거 | 기획 문서는 “개발자가 추측하지 않아도 되는 상태”가 목표다. |
| UI/API 분리 | 보이는 UI를 그대로 베끼지 말고, API request에 필요한 데이터만 field로 정의한다. |
| 불확실성 표시 | enum, default, required, 조건부 required가 불명확하면 임의로 확정하지 말고 확인 필요 사항으로 남긴다. |
| 예제 작성 | 예제 payload는 최소 입력과 전체 입력을 모두 작성한다. |
