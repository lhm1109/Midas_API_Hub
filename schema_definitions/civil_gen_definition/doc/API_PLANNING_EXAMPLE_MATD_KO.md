# API 기획서 예시: Modify Material

이 문서는 Design 계열 API를 어떤 수준으로 기획해야 하는지 보여주는 예시다. 실제 최종 사양은 제품/기존 API 문서/OpenAPI를 확인해 확정해야 한다.

## 1. 기본 정보

| 항목 | 값 |
|---|---|
| Product | civil-nx-mec |
| Module | Design |
| Category | PSC / AASHTO-LRFD24 / Design Input |
| API Name | Modify Material |
| Endpoint | `/DESIGN/PSC/AASHTO-LRFD24/MATD` |
| Method | GET, POST, PUT, DELETE |
| 목적 | PSC AASHTO-LRFD24 설계 입력에서 material 관련 설계 데이터를 조회/수정한다. |
| 기준 화면/문서 | Design Input > Modify Material |
| 관련 기존 API | `/db/MATD`, Design MATD 계열 |

## 2. API 분류

| 항목 | 값 |
|---|---|
| API 유형 | Entity DB / Design Settings |
| Body Root | Assign |
| Entity Type | collection |
| ID Key 사용 | Y |
| 생성/수정 단위 | material ID별 entity |

### 전송 구조

```json
{
  "Assign": {
    "1": {
      "TYPE": "CONC",
      "NAME": "C40",
      "DATA1": {}
    }
  }
}
```

기획서와 schema field 표는 `"1"` 내부의 단일 material entity만 정의한다. `Assign`과 `"1"`은 전송 구조 설명에만 작성한다.

## 3. Field 정의 예시

아래 표는 작성 수준을 보여주는 예시다. 실제 field 전체 목록은 기존 `/db/MATD`와 Design MATD 사양을 대조해서 확정한다.

| Order | Field Key | Label | Type | Required | Default | Enum/Options | Unit | Condition | Description |
|---:|---|---|---|---|---|---|---|---|---|
| 1 | `TYPE` | Type | string | Y | 확인 필요 | `CONC`, `STEEL`, 기타 확인 필요 | - | - | Material type |
| 2 | `NAME` | Name | string | Y | - | - | - | - | Material name |
| 3 | `DATA1` | Material Data | object | Y | - | - | - | `TYPE`별 하위 구조 다름 | Material 상세 데이터 묶음 |
| 4 | `REBAR_CODENAME` | Rebar Code Name | string | N | 확인 필요 | 확인 필요 | - | rebar 사용 시 | Rebar code name |
| 5 | `SERVCHECK` | Serviceability Check | boolean | N | false | true/false | - | - | Serviceability check 사용 여부 |
| 6 | `SHORTTERM` | Short Term | number | 조건부 | 확인 필요 | - | 확인 필요 | `SERVCHECK = true` | 단기 허용값 |
| 7 | `LONGTERM` | Long Term | number | 조건부 | 확인 필요 | - | 확인 필요 | `SERVCHECK = true` | 장기 허용값 |

## 4. 중첩 Object 구조 예시

```text
DATA1
  CODENAME: string
  SUBCODENAME: string
  CODEMATLNAME: string
  HYBRID_FACTOR: boolean
  ANAL
    ELAST: number
    POISSON: number
  DESIGN
    MATLNAME: string
    ALWS1: number
    ALWS2: number
    ...
```

| Parent | Child Field | Type | Required | Default | Description |
|---|---|---|---|---|---|
| `DATA1` | `CODENAME` | string | 확인 필요 | 확인 필요 | Code name |
| `DATA1` | `SUBCODENAME` | string | 확인 필요 | 확인 필요 | Sub code name |
| `DATA1` | `CODEMATLNAME` | string | 확인 필요 | 확인 필요 | Code material name |
| `DATA1` | `HYBRID_FACTOR` | boolean | N | false | Hybrid factor 사용 여부 |
| `DATA1.ANAL` | `ELAST` | number | 확인 필요 | 확인 필요 | Elastic modulus |
| `DATA1.ANAL` | `POISSON` | number | 확인 필요 | 확인 필요 | Poisson ratio |
| `DATA1.DESIGN` | `MATLNAME` | string | 확인 필요 | 확인 필요 | Design material name |

## 5. 조건부 규칙 예시

| 조건 Field | 조건 값 | 영향 Field | 상태 | 설명 |
|---|---|---|---|---|
| `TYPE` | `CONC` | `DATA1.ANAL`, `DATA1.DESIGN` | Required/Optional 확인 필요 | Concrete material일 때 사용하는 분석/설계 데이터 |
| `SERVCHECK` | `true` | `SHORTTERM`, `LONGTERM` | Required 또는 Optional 확인 필요 | Serviceability Check 사용 시 관련 값 표시 |
| `TYPE` | rebar 관련 type | `REBAR_CODENAME`, `MAINREBAR_REBARNAME` | Optional/Required 확인 필요 | Rebar 정보 사용 시 표시 |

## 6. Enum/Option 확인 필요

| Field | 확인할 내용 |
|---|---|
| `TYPE` | material type 전체 목록과 실제 payload value |
| `CODENAME` | code name 전체 목록 또는 자유 입력 여부 |
| `SUBCODENAME` | code별 sub code 목록 |
| `REBAR_CODENAME` | rebar code 목록 |
| `SERVCHECK` | default가 false인지 기존 데이터 기준인지 |

## 7. UI-only 제외 목록

| UI 요소 | 제외 사유 |
|---|---|
| OK button | 실행 버튼, request field 아님 |
| Cancel button | 실행 버튼, request field 아님 |
| Add/Delete/Replace button | 편집 조작 버튼, request field 아님 |
| Material list row selector | entity ID 선택 UI, field 아님 |

## 8. 예제 Payload

### 8.1 최소 입력 예시

```json
{
  "Assign": {
    "1": {
      "TYPE": "CONC",
      "NAME": "C40",
      "DATA1": {}
    }
  }
}
```

### 8.2 일반 입력 예시

```json
{
  "Assign": {
    "1": {
      "TYPE": "CONC",
      "NAME": "C40",
      "DATA1": {
        "CODENAME": "AASHTO-LRFD24",
        "SUBCODENAME": "PSC",
        "ANAL": {
          "ELAST": 30000,
          "POISSON": 0.2
        },
        "DESIGN": {
          "MATLNAME": "C40"
        }
      },
      "SERVCHECK": true,
      "SHORTTERM": 1.0,
      "LONGTERM": 1.0
    }
  }
}
```

위 값은 형식 예시다. 실제 default, 단위, required 여부는 제품/기존 API로 확정해야 한다.

## 9. 확인 필요 사항

| No. | 질문 | 담당 | 상태 |
|---:|---|---|---|
| 1 | `TYPE` enum 전체 목록과 value는 무엇인가? | 기획/개발 | Open |
| 2 | `DATA1` 하위 field 중 required는 무엇인가? | 개발 | Open |
| 3 | number field의 단위는 무엇인가? | 기획/제품 | Open |
| 4 | GET 응답과 PUT 입력 구조가 완전히 동일한가? | 개발 | Open |
| 5 | `SERVCHECK`가 false일 때 `SHORTTERM`, `LONGTERM`을 생략해야 하는가, default로 보내야 하는가? | 기획/개발 | Open |

## 10. 이 예시에서 배울 점

| 항목 | 적용 기준 |
|---|---|
| 전송 구조 | Design 계열이므로 `Assign` collection을 먼저 의심한다. |
| Field 표 범위 | `Assign` wrapper와 ID key는 field 표에 넣지 않는다. |
| Naming | 화면 label만 보고 긴 field key를 새로 만들지 않는다. |
| 기존 key | 기존 `/db/MATD` key가 있으면 기존 key를 우선한다. |
| 중첩 object | tree와 표를 함께 작성한다. |
| 확인 필요 사항 | 불확실한 required/default/enum은 확정하지 않고 질문으로 남긴다. |
