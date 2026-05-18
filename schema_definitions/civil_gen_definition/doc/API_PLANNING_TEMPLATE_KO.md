# API 기획서 템플릿

이 문서를 복사해서 API별 기획서를 작성한다. 모르는 값은 비워두지 말고 `확인 필요`라고 명시한다.

## 1. 기본 정보

| 항목 | 값 |
|---|---|
| Product |  |
| Module |  |
| Category |  |
| API Name |  |
| Endpoint |  |
| Method | GET / POST / PUT / DELETE |
| 목적 |  |
| 기준 화면/문서 |  |
| 관련 기존 API |  |

## 2. API 분류

| 항목 | 선택/값 |
|---|---|
| API 유형 | Settings / Table / Entity DB / Action / 기타 |
| Body Root | Assign / Argument / 없음 / 확인 필요 |
| Entity Type | collection / single / 확인 필요 |
| ID Key 사용 | Y / N / 확인 필요 |
| 생성/수정 단위 | 단일 entity / 여러 entity / 실행 인자 / 기타 |

### 전송 구조 예시

```json
{
  "Assign": {
    "1": {
      "FIELD_A": "value"
    }
  }
}
```

또는

```json
{
  "Argument": {
    "FIELD_A": "value"
  }
}
```

## 3. Field 정의

| Order | Field Key | Label | Type | Required | Default | Enum/Options | Unit | Condition | Description |
|---:|---|---|---|---|---|---|---|---|---|
| 1 |  |  | string/integer/number/boolean/array/object | Y/N/조건부 |  |  |  |  |  |
| 2 |  |  |  |  |  |  |  |  |  |

### Type 작성 기준

| Type | 사용 기준 |
|---|---|
| string | 이름, code, path, load case name |
| integer | 정수 option, index, count |
| number | 실수 값, factor, coefficient |
| boolean | checkbox, on/off |
| array | 여러 값 선택, node list, component list |
| object | 하위 field 묶음 |

## 4. Enum/Option 상세

선택형 field마다 전체 option을 작성한다.

### Field: `{FIELD_KEY}`

| Value | Label | Default | 조건 | 비고 |
|---|---|---|---|---|
|  |  | Y/N |  |  |

| 확인 | 확인 필요 항목 | 작성 기준 |
|---|---|---|
| [ ] | 화면에 현재 선택값만 보임 | 전체 option을 추가 확인한다. |
| [ ] | code별 option 변화 | code/type 조건에 따라 option이 달라지는지 확인한다. |
| [ ] | value와 label 관계 | 실제 payload value와 화면 label이 같은지 확인한다. |

## 5. 조건부 규칙

| 조건 Field | 조건 값 | 영향 Field | 상태 | 설명 |
|---|---|---|---|---|
|  |  |  | Required/Optional/Hidden/Disabled |  |

예:

| 조건 Field | 조건 값 | 영향 Field | 상태 | 설명 |
|---|---|---|---|---|
| `EXP_TYPE` | `2` | `EXP_USER` | Required | User 선택 시 직접 입력 |

## 6. 중첩 Object 구조

중첩 object가 있으면 field tree로 작성한다.

```text
DATA1
  CODENAME: string
  SUBCODENAME: string
  ANAL
    ELAST: number
    POISSON: number
```

| Parent | Child Field | Type | Required | Default | Description |
|---|---|---|---|---|---|
|  |  |  |  |  |  |

## 7. Array 구조

array field마다 item type과 입력 방식을 작성한다.

| Field Key | Item Type | 중복 허용 | 최소/최대 개수 | 입력 예 | 설명 |
|---|---|---|---|---|---|
|  | string/integer/object | Y/N |  |  |  |

## 8. UI-only 요소 제외 목록

화면에는 있지만 API field가 아닌 요소를 명시한다.

| UI 요소 | 제외 사유 |
|---|---|
| OK button | 실행 버튼, request field 아님 |
| Cancel button | 실행 버튼, request field 아님 |

## 9. 예제 Payload

### 9.1 최소 입력

```json
{}
```

### 9.2 일반 입력

```json
{}
```

### 9.3 전체 입력

```json
{}
```

## 10. 응답/결과 확인

| 항목 | 내용 |
|---|---|
| 성공 시 기대 결과 |  |
| 실패/검증 오류 조건 |  |
| GET 응답과 PUT 입력 구조 동일 여부 | Y / N / 확인 필요 |
| 기존 데이터 유지/덮어쓰기 방식 |  |

## 11. 확인 필요 사항

| No. | 질문 | 담당 | 상태 |
|---:|---|---|---|
| 1 |  |  | Open |

## 12. 제출 전 자기 검토

| 확인 | 항목 | 기준 |
|---|---|---|
| [ ] | Endpoint / Method | 실제 호출 경로와 method를 확인했다. |
| [ ] | Wrapper | `Assign` / `Argument` / 없음 중 하나로 확인했다. |
| [ ] | Entity Type | collection / single 여부를 확인했다. |
| [ ] | Field Key | 모든 field key를 실제 payload 기준으로 작성했다. |
| [ ] | Enum | 모든 enum option의 value와 label을 작성했다. |
| [ ] | Default | default 값을 확인했다. |
| [ ] | Required | required와 조건부 required를 구분했다. |
| [ ] | UI-only | API field가 아닌 UI 요소를 제외했다. |
| [ ] | Payload | 최소/일반/전체 payload 예제를 작성했다. |
| [ ] | 질문 | 확인 불가 항목은 질문으로 남겼다. |
