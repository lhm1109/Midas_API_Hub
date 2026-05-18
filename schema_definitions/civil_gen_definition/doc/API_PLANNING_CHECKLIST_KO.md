# API 기획 제출 전 체크리스트

이 체크리스트는 API 기획서를 스키마 작성자에게 넘기기 전에 사용한다. `확인` 칸에 체크할 수 없는 항목은 기획서의 `확인 필요 사항`에 질문으로 남긴다.

## 1. 기본 정보

| 확인 | 항목 | 기준 |
|---|---|---|
| [ ] | Product / Module / Category | API가 어느 제품, 모듈, 메뉴에 속하는지 명확하다. |
| [ ] | API Name | 화면/문서/기능 목적과 이름이 일치한다. |
| [ ] | Endpoint | 실제 호출 경로가 정확하다. |
| [ ] | Method | GET / POST / PUT / DELETE 중 실제 지원 method가 정리됐다. |
| [ ] | 목적 | API 목적이 한 문장으로 설명된다. |
| [ ] | 기준 자료 | 기준 화면, 기존 문서, 기존 API 링크가 있다. |

## 2. 전송 구조

| 확인 | 항목 | 기준 |
|---|---|---|
| [ ] | Body Root | `Assign`, `Argument`, 없음 중 하나로 결정됐다. |
| [ ] | Entity Type | collection / single 여부가 결정됐다. |
| [ ] | ID Key | collection이면 `"1"`, `"2"` 같은 ID key 예시가 있다. |
| [ ] | Single 근거 | single이면 ID 없이 단일 객체인 근거가 있다. |
| [ ] | Wrapper 중복 방지 | wrapper를 field 목록에 중복으로 넣지 않았다. |

## 3. Field 목록

| 확인 | 항목 | 기준 |
|---|---|---|
| [ ] | 전체 field | 모든 API field가 표에 있다. |
| [ ] | Field Key | 화면 label이 아니라 실제 payload key를 기준으로 작성했다. |
| [ ] | Order | field 표시 순서가 있다. |
| [ ] | Type | string / integer / number / boolean / array / object 중 하나로 정리됐다. |
| [ ] | Required | 항상 필수 / 선택 / 조건부로 구분됐다. |
| [ ] | Default | 기본값이 확인됐거나 `확인 필요`로 남아 있다. |
| [ ] | Unit | number field의 단위가 확인됐다. |
| [ ] | Object | object field는 child field까지 작성했다. |
| [ ] | Array | array field는 item type과 입력 예시가 있다. |

## 4. Naming

| 확인 | 항목 | 기준 |
|---|---|---|
| [ ] | 기존 key 우선 | 기존 API key가 있으면 기존 key를 우선했다. |
| [ ] | 신규 key | 새 key는 사람이 읽을 수 있는 의미 중심 `UPPER_SNAKE_CASE`다. |
| [ ] | 타입 접두어 금지 | 신규 key에 `b`, `i`, `d`, `s` 같은 타입 접두어를 붙이지 않았다. |
| [ ] | 고정 key 유지 | 기존 대문자 key(`TYPE`, `NAME`, `DATA1` 등)는 임의로 바꾸지 않았다. |
| [ ] | 중복 방지 | 같은 의미의 field가 여러 이름으로 중복되지 않는다. |

## 5. Enum과 Option

| 확인 | 항목 | 기준 |
|---|---|---|
| [ ] | 전체 option | dropdown / radio / checkbox option의 전체 목록을 확인했다. |
| [ ] | Value / Label 분리 | enum value와 화면 label을 분리해 작성했다. |
| [ ] | Default | default option을 표시했다. |
| [ ] | 조건별 변화 | code/type별로 option이 달라지는 경우 조건을 작성했다. |
| [ ] | 부분 확정 방지 | 화면에 한 option만 보이는 dropdown을 그대로 확정하지 않았다. |

## 6. 조건부 규칙

| 확인 | 항목 | 기준 |
|---|---|---|
| [ ] | 조건 Field | 조건 field와 조건 값을 표로 작성했다. |
| [ ] | 영향 Field | 영향을 받는 field를 모두 작성했다. |
| [ ] | 상태 | Required / Optional / Hidden / Disabled 상태를 구분했다. |
| [ ] | 복합 조건 | `TYPE + STYPE` 같은 복합 조건이 있으면 모두 작성했다. |
| [ ] | Default 처리 | 조건부 field의 default 처리 방식을 확인했다. |

## 7. UI-only 제외

| 확인 | UI 요소 | 기준 |
|---|---|---|
| [ ] | OK / Cancel / Help / Close | 실행 또는 닫기 버튼을 field로 넣지 않았다. |
| [ ] | Add / Delete / Replace / Intersect | 편집 조작 버튼을 field로 넣지 않았다. |
| [ ] | All / None / Inverse / Prev | 버튼 자체를 field로 넣지 않았다. |
| [ ] | 필터 / 검색 / 정렬 / 탭 전환 | 화면 조작 상태를 field로 넣지 않았다. |
| [ ] | label / header | 단순 표시용 텍스트를 field로 넣지 않았다. |
| [ ] | 저장되는 UI 결과 | UI 조작 결과가 payload로 저장되는 경우에만 field로 정의했다. |

## 8. Table API인 경우

| 확인 | 항목 | 기준 |
|---|---|---|
| [ ] | API 유형 | Table API인지 Settings API인지 명확히 결정했다. |
| [ ] | `TABLE_TYPE` | 실제 `TABLE_TYPE` 값을 작성했다. |
| [ ] | Column Header | table column header 전체 목록을 작성했다. |
| [ ] | `COMPONENTS` | `COMPONENTS` enum을 작성했다. |
| [ ] | 공통 field | `TABLE_NAME`, `EXPORT_PATH`, `UNIT`, `STYLES` 사용 여부를 검토했다. |
| [ ] | `NODE_ELEMS` | 입력 방식(`KEYS`, `TO`, `STRUCTURE_GROUP_NAME`)을 검토했다. |
| [ ] | `LOAD_CASE_NAMES` | load case 입력 형식을 확인했다. |
| [ ] | Construction Stage | `OPT_CS`, `STAGE_STEP` 필요 여부를 확인했다. |

## 9. Settings/Design API인 경우

| 확인 | 항목 | 기준 |
|---|---|---|
| [ ] | 선택지 | 설계 기준, code option, material option의 전체 선택지를 확인했다. |
| [ ] | Collection 가능성 | Design/DB 계열이라도 collection일 수 있음을 검토했다. |
| [ ] | Group/Section | 설정 화면의 group/section을 field group으로 정리했다. |
| [ ] | Checkbox default | 계산 옵션 checkbox의 default를 확인했다. |
| [ ] | Key 매핑 | 기존 DB key와 화면 label이 다른 경우 매핑표를 작성했다. |

## 10. 예제 Payload

| 확인 | 항목 | 기준 |
|---|---|---|
| [ ] | 최소 입력 | required 중심의 최소 입력 예제가 있다. |
| [ ] | 일반 입력 | 일반 사용 흐름에 맞는 입력 예제가 있다. |
| [ ] | 전체 입력 | 조건부/선택 field를 포함한 전체 입력 예제가 있다. |
| [ ] | Wrapper | 예제에 wrapper가 실제 요청 형태로 들어 있다. |
| [ ] | Field 표 일치 | 예제의 enum/default/required가 field 표와 일치한다. |
| [ ] | GET/PUT 차이 | GET 응답 구조와 PUT 입력 구조가 다르면 차이를 설명했다. |

## 11. 검증 관점

| 확인 | 항목 | 기준 |
|---|---|---|
| [ ] | Wrapper 중복 | wrapper가 properties 안에 중복으로 들어가지 않는다. |
| [ ] | Required 위치 | required를 nested object 안에 무분별하게 넣지 않는다. |
| [ ] | 조건부 required | dot notation이 아니라 field 이름 기준으로 설명한다. |
| [ ] | UI/Validation 분리 | UI 정보와 validation 정보를 섞지 않는다. |
| [ ] | JSON Schema 표현 | validation은 가능하면 표준 JSON Schema로 표현 가능하게 설명한다. |

## 12. 제출 가능 판정

아래 항목 중 하나라도 해당하면 제출하지 않고 질문을 남긴다.

| 확인 | 제출 보류 조건 | 처리 |
|---|---|---|
| [ ] | enum 전체 목록을 모른다. | 확인 필요 사항에 질문으로 남긴다. |
| [ ] | default 값을 모른다. | 확인 필요 사항에 질문으로 남긴다. |
| [ ] | field key가 실제 payload key인지 확신할 수 없다. | 기존 API 또는 개발 담당자에게 확인한다. |
| [ ] | wrapper/collection 여부를 모른다. | request 예제를 확인한다. |
| [ ] | 예제 payload를 만들 수 없다. | 기획 완료로 보지 않는다. |
| [ ] | UI-only인지 API field인지 구분이 안 된다. | 제외 목록과 payload 저장 여부를 확인한다. |

| 최종 확인 | 제출 가능 기준 |
|---|---|
| [ ] | 스키마 작성자가 기획서를 보고 field/key/type/required/default/enum/condition을 추측 없이 작성할 수 있다. |
