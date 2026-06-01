# Supabase + OpenAPI 기반 Pydantic 스키마 생성 프롬프트

아래 프롬프트는 이 저장소의 Supabase 구조와 OpenAPI 스펙을 함께 보고, AI가 Pydantic 모델을 안정적으로 생성하도록 설계한 입력 템플릿이다.

## Supabase 인식용 환경 변수 가이드

이 프로젝트에서 Supabase를 인식하는 경로는 하나가 아니다. 실행 위치에 따라 읽는 값이 조금 다르므로, 아래처럼 구분해서 관리해야 한다.

### 0. 현재 프로젝트에서 확인된 실제 상태

현재 루트 환경 파일은 다음 경로다.

- C:/MIDAS/code/apiverification/.env

이 파일에서 실제로 확인된 Supabase 관련 키는 다음 두 개다.

- SUPABASE_URL
- SUPABASE_SERVICE_KEY

현재 이 프로젝트는 VITE_SUPABASE_URL, VITE_SUPABASE_ANON_KEY 같은 브라우저 노출용 키를 기준으로 동작하는 구조가 아니다. 실제 자동화와 백업, import 스크립트는 service key 기반으로 동작한다.

현재 설정 상태를 문서 관점에서 요약하면 다음과 같다.

- Supabase 프로젝트 URL 은 이미 .env 에 설정되어 있다.
- Service role 성격의 키는 SUPABASE_SERVICE_KEY 이름으로 설정되어 있다.
- 일부 문서나 예전 코드에서 SUPABASE_SERVICE_ROLE_KEY 라는 이름을 언급할 수 있지만, 현재 확인된 루트 .env 기준 실사용 키 이름은 SUPABASE_SERVICE_KEY 다.

즉, 이 저장소에서 Supabase 인식을 위해 최소한 먼저 볼 값은 다음 두 개다.

- SUPABASE_URL
- SUPABASE_SERVICE_KEY

### 1. 루트 .env

기본 경로:

- C:/MIDAS/code/apiverification/.env


이 파일은 Electron 메인 프로세스와 여러 루트 scripts 에서 읽는다. 코드상 가장 자주 쓰이는 Supabase 키는 다음 두 개다.

- SUPABASE_URL
- SUPABASE_SERVICE_KEY

실제 로딩 근거:

- [electron/main.cjs](electron/main.cjs#L8) 에서 루트 .env 를 직접 로드한다.
- [scripts/backup-supabase.mjs](scripts/backup-supabase.mjs#L52) 에서 SUPABASE_URL 과 SUPABASE_SERVICE_KEY 를 읽는다.
- [scripts/import-zendesk-doc-to-supabase.mjs](scripts/import-zendesk-doc-to-supabase.mjs#L250) 에서도 같은 키 조합을 사용한다.

권장 예시는 다음 형태다.

```env
SUPABASE_URL=https://your-project-ref.supabase.co
SUPABASE_SERVICE_KEY=your_service_role_key

# 일부 마이그레이션 코드 호환용
SUPABASE_SERVICE_ROLE_KEY=your_service_role_key

# UI/도구에서 별도 DB 연결 테스트가 필요할 때 사용
SUPABASE_DB_PASSWORD=your_database_password
```

### 2. server/.env 를 따로 읽는 스크립트

일부 서버 스크립트는 루트 .env 가 아니라 server/.env 를 직접 읽는다. 따라서 자동화 스크립트를 돌릴 때는 둘 중 하나를 맞춰야 한다.

- 루트 .env 와 server/.env 를 동일하게 유지
- 또는 스크립트가 읽는 경로를 루트 .env 기준으로 통일

대표적으로 [server/scripts/merge-missing-endpoints-from-backup.mjs](server/scripts/merge-missing-endpoints-from-backup.mjs#L15) 는 server/.env 기준 설명을 갖고 있다.

### 3. 프론트 UI 설정값

앱의 Settings UI 는 환경 변수 이름과 별개로 다음 필드를 사용한다.

- supabaseUrl
- supabaseServiceKey
- supabaseDbPassword

이 값들은 Settings 화면에서 입력되며, 데이터베이스 탭의 연결 테스트나 SQL 실행 시 사용된다.

### 4. 이 저장소에서 실제로 확인된 키 이름

현재 코드 기준으로 Supabase 관련 키 이름은 아래처럼 쓰이고 있다.

- 일반 서버/스크립트 다수: SUPABASE_URL + SUPABASE_SERVICE_KEY
- 일부 마이그레이션 코드: SUPABASE_URL + SUPABASE_SERVICE_ROLE_KEY
- UI 연결 테스트용 추가 값: Database Password

추가로 정리하면 다음과 같다.

- 현재 루트 .env 에 실제 존재하는 Supabase 키는 SUPABASE_URL, SUPABASE_SERVICE_KEY 다.
- 현재 확인 범위에서는 anon key 기반 프론트 공개 접근보다 server-side script 접근이 중심이다.
- 따라서 AI가 "이 프로젝트가 Supabase를 어떻게 인식하느냐"를 설명할 때는 service key 기반 자동화라고 이해하는 것이 맞다.

따라서 실무적으로는 다음 3개를 같이 관리하는 편이 안전하다.

- SUPABASE_URL
- SUPABASE_SERVICE_KEY
- SUPABASE_SERVICE_ROLE_KEY

service key 와 service role key 를 같은 값으로 두는 방식은 이 저장소의 현재 코드 호환성 확보용이다.

### 5. AI 프롬프트에 무엇을 넣어야 하는가

Pydantic 생성 프롬프트 자체에는 보통 비밀키가 필요 없다. AI가 필요한 것은 인증값이 아니라 스키마 근거다.

프롬프트에 넣어도 되는 것:

- SUPABASE_URL 또는 project ref
- 관련 테이블명
- 샘플 row 에서 비밀값을 제거한 구조 정보
- OpenAPI path, schema, example

이 저장소에서 실제로 바로 넣을 수 있는 최소 환경 컨텍스트 예시는 다음과 같다.

```text
Environment context for schema analysis:
- env file path: C:/MIDAS/code/apiverification/.env
- supabase url is configured
- required runtime keys used by scripts:
   - SUPABASE_URL
   - SUPABASE_SERVICE_KEY
- optional compatibility key:
   - SUPABASE_SERVICE_ROLE_KEY
```

프롬프트에 넣지 말아야 하는 것:

- SUPABASE_SERVICE_KEY
- SUPABASE_SERVICE_ROLE_KEY
- DB 비밀번호
- Zendesk, Confluence 토큰

### 6. AI가 실제 Supabase를 조회해야 할 때의 규칙

AI에게 프롬프트만 던지는 용도라면 비밀키를 노출하지 않는다. 실제 DB introspection 이 필요한 자동화라면 다음처럼 분리한다.

- 프롬프트에는 키 이름만 설명한다.
- 실행 환경에서만 .env 를 로드한다.
- LLM 입력에는 값 대신 placeholder 를 사용한다.

예시:

```text
Use local environment variables for database access.
Required keys are SUPABASE_URL and SUPABASE_SERVICE_KEY.
Do not print secrets in the response.
Only summarize schema/table structure.
```

### 7. 권장 체크리스트

- 루트 .env 에 SUPABASE_URL 이 있는지 확인
- 루트 .env 에 SUPABASE_SERVICE_KEY 가 있는지 확인
- 일부 마이그레이션 호환을 위해 SUPABASE_SERVICE_ROLE_KEY 도 같이 둘지 확인
- Database 탭을 쓸 경우 supabaseDbPassword 입력 경로 확인
- server/.env 를 따로 읽는 스크립트가 있는지 확인
- 프롬프트에는 절대 비밀값 원문을 붙이지 않기

## 사용 목적

- Supabase 테이블 구조에서 메타데이터 관계를 파악한다.
- OpenAPI 스펙과 example 값에서 실제 API payload 구조를 추론한다.
- Python Pydantic 모델을 생성한다.
- 추론 근거와 불확실한 부분을 분리해서 출력하게 만든다.

## 전제

- Supabase는 제품, 그룹, 엔드포인트, 버전, 스펙/매뉴얼 데이터를 저장하는 메타 저장소다.
- 실제 요청/응답 payload 구조의 1차 근거는 OpenAPI components, requestBody, responses, example 이다.
- Supabase의 manual_data, spec_data, runner_data 같은 테이블은 API 본문 자체라기보다 스키마 원문, 예시, 실행 결과를 담는 저장소일 수 있다.
- 따라서 DB 컬럼명과 API JSON 필드명을 섞어서 추론하면 안 된다.
- 환경 변수는 DB 접근을 위한 실행 설정일 뿐, Pydantic 필드 추론의 근거가 아니다.

## 그대로 복사해서 쓰는 프롬프트

```text
너는 API 스펙 분석가이자 Python 데이터 모델러다.
목표는 Supabase 메타데이터와 OpenAPI 스펙을 함께 읽고, 실제 API 요청/응답 payload에 맞는 Pydantic v2 모델 코드를 생성하는 것이다.

다음 규칙을 반드시 지켜라.

[역할]
1. Supabase SQL/백업은 데이터 저장 구조와 엔드포인트-버전-스펙의 관계를 파악하는 용도로 사용한다.
2. OpenAPI paths, components.schemas, requestBody, responses, content.application/json.schema, example 은 실제 payload 모델 생성의 주 근거로 사용한다.
3. manual_data.json_schema, spec_data.json_schema, runner_data.request_body, runner_data.response_body 가 제공되면 OpenAPI와 교차 검증한다.
4. 근거가 없는 필드는 추측해서 만들지 않는다.

[입력 데이터]
아래 자료를 순서대로 해석하라.

0. 실행 환경 정보
<RUNTIME_ENV>
supabase_url: {{SUPABASE_URL_OR_PROJECT_REF}}
env_keys_available:
- SUPABASE_URL
- SUPABASE_SERVICE_KEY
- SUPABASE_SERVICE_ROLE_KEY
secret_values_are_not_included: true
</RUNTIME_ENV>

1. Supabase SQL 스키마
<SUPABASE_SQL>
{{SUPABASE_SQL}}
</SUPABASE_SQL>

2. Supabase 백업 일부 또는 샘플 row
<SUPABASE_BACKUP>
{{SUPABASE_BACKUP}}
</SUPABASE_BACKUP>

3. OpenAPI 스펙 일부 또는 전체
<OPENAPI_SPEC>
{{OPENAPI_SPEC}}
</OPENAPI_SPEC>

4. 타겟 엔드포인트 정보
<TARGET>
product: {{PRODUCT}}
endpoint_path: {{ENDPOINT_PATH}}
http_method: {{HTTP_METHOD}}
operation_id: {{OPERATION_ID}}
</TARGET>

[분석 절차]
1. Supabase에서 endpoint, version, spec/manual/runner 관련 테이블과 컬럼을 식별하라.
2. 실행 환경 정보는 연결 컨텍스트로만 사용하고, payload 모델 추론 근거로 사용하지 마라.
3. 타겟 엔드포인트와 연결될 수 있는 메타데이터 구조를 요약하라.
4. OpenAPI에서 해당 엔드포인트의 requestBody 와 responses 를 찾고, components.schemas 참조를 모두 해석하라.
5. example 이 있으면 타입, 중첩 구조, 맵 구조, nullable 여부를 보정하라.
6. schema 와 example 이 충돌하면 다음 우선순위를 적용하라.
   - 명시적 schema type, required, enum, oneOf/anyOf/allOf
   - $ref 로 연결된 components.schemas
   - example 구조
   - Supabase에 저장된 json_schema 또는 runner 예시
7. API 필드명이 대문자 또는 원문 그대로 유지되어야 하면 Python 필드명은 snake_case 로 만들고 alias 를 사용하라.
8. 숫자처럼 보이는 키("1", "2")가 반복되는 object 는 list 로 바꾸지 말고 dict[str, SubModel] 또는 dict[int, SubModel] 후보를 검토하라. 스펙에 키 타입 명시가 없으면 dict[str, SubModel] 을 기본으로 사용하라.
9. 날짜/시간은 format 이 date-time 이거나 DB/스펙 근거가 명확할 때만 datetime 으로 선언하라. 그 외에는 str 로 유지하라.
10. 불명확한 JSON blob 은 dict[str, Any] 로 두고, 왜 세분화하지 않았는지 설명하라.
11. 요청 모델과 응답 모델을 분리하라.

[출력 규칙]
반드시 아래 섹션 순서로 출력하라.

1. Schema understanding
   - Supabase 관점에서 관련 테이블과 역할 요약
   - OpenAPI 관점에서 타겟 엔드포인트의 요청/응답 구조 요약

2. Modeling decisions
   - 필수/선택 필드 판단 근거
   - alias 처리한 필드
   - dict 로 모델링한 필드
   - Any 또는 보수적으로 둔 필드와 이유

3. Pydantic code
   - Pydantic v2 문법 사용
   - 필요한 import 포함
   - BaseModel 공통 설정이 필요하면 별도 BaseSchema 작성
   - 모델명은 Endpoint/Operation 기준으로 명확하게 작성
   - 가능하면 Request, Response, Item, Detail, MapEntry 같은 접미사 사용

4. Usage example
   - request 검증 예시 1개
   - response 검증 예시 1개

5. Assumptions and unknowns
   - 확실하지 않은 점
   - 추가로 받으면 정확도가 올라가는 데이터

[코드 생성 규칙]
- Python 버전은 3.11 기준으로 작성하라.
- Pydantic v2 스타일을 사용하라.
- model_config = ConfigDict(populate_by_name=True, extra='forbid') 를 기본으로 검토하라.
- 원본 JSON 키를 보존해야 하면 Field(alias='ORIGINAL_KEY') 를 사용하라.
- enum 후보가 명확하면 Literal 또는 Enum 을 사용하라.
- 재사용되는 하위 구조는 중복 정의하지 말고 별도 모델로 분리하라.
- 최상위 응답이 object wrapping 형태이면 wrapper 모델을 유지하라.
- components.schemas 이름이 DTO_* 형식이면 그대로 활용하거나 더 읽기 쉬운 이름으로 바꾸되 주석 없이 일관되게 적용하라.
- 설명 텍스트만 길게 쓰지 말고, 최종 산출물은 바로 복사 가능한 Python 코드 중심으로 제공하라.

[금지]
- DB 테이블 컬럼을 그대로 API payload 필드라고 가정하지 마라.
- 비밀키나 비밀번호를 출력하거나 코드 블록에 재현하지 마라.
- example 하나만 보고 required 를 단정하지 마라.
- list 와 dict 를 임의로 바꾸지 마라.
- 근거 없는 기본값을 넣지 마라.
- v1 스타일 Pydantic 문법을 쓰지 마라.

이제 입력을 분석하고 결과를 작성하라.
```

## 이 저장소 기준 추천 입력값 구성

### 0. 환경 변수 컨텍스트

- 루트 환경 파일: C:/MIDAS/code/apiverification/.env
- 주요 키: SUPABASE_URL, SUPABASE_SERVICE_KEY
- 호환용 추가 키: SUPABASE_SERVICE_ROLE_KEY
- UI 별도 값: supabaseDbPassword

현재 프로젝트 인식 요약:

- Electron main 이 루트 .env 를 읽는다.
- 여러 import/backup 스크립트가 SUPABASE_URL 과 SUPABASE_SERVICE_KEY 로 Supabase client 를 생성한다.
- 즉, AI가 이 저장소의 Supabase 구조를 다룰 때 참고할 런타임 컨텍스트는 root .env + service key 기반 script execution 이다.

주의:

- AI 프롬프트에는 실제 secret 값을 넣지 않는다.
- 필요한 경우 URL, project ref, 키 이름만 전달한다.

### 1. Supabase SQL

- [server/schema-with-locks.sql](server/schema-with-locks.sql)
- [server/schema-manager.sql](server/schema-manager.sql)

### 2. Supabase 백업 샘플

- [backups/supabase-backup-20260227-134254.json](backups/supabase-backup-20260227-134254.json)

### 3. OpenAPI 스펙

- [api_data_set/openapi 1.json](api_data_set/openapi%201.json)

## 입력 조합 예시

```text
{{SUPABASE_SQL}}:
- server/schema-with-locks.sql 의 endpoints, versions, manual_data, spec_data, runner_data 관련 부분
- server/schema-manager.sql 의 manager_tasks, manager_columns 관련 부분

{{SUPABASE_BACKUP}}:
- products, groups, endpoints, versions 중 타겟 엔드포인트와 관련된 row

{{OPENAPI_SPEC}}:
- 해당 path 의 method 블록
- 연결된 components.schemas 정의
- example 값
```

## 이 저장소에 맞는 추가 지침

- OpenAPI example 에서 PROJECT, VER, ACTL 같은 대문자 루트 키는 응답 wrapper 일 가능성이 높다.
- ACTL 아래의 "1": { ... } 구조처럼 숫자 문자열 키가 있으면 list 로 바꾸지 말고 map 구조로 먼저 해석해야 한다.
- Supabase의 manual_data/spec_data 의 json_schema 계열 컬럼은 원문 JSON 스키마 저장소일 가능성이 있으므로, 실제 Pydantic 생성 시 OpenAPI와 내용이 일치하는지 먼저 비교해야 한다.
- runner_data 의 request_body, response_body 는 실측 예시일 수 있으므로 schema 누락 시 보조 근거로만 사용해야 한다.

## 빠른 사용 예시

예를 들어 [api_data_set/openapi 1.json](api_data_set/openapi%201.json) 의 /DB/ACTL GET 응답을 모델링하려면, 프롬프트 입력에 다음을 포함하면 된다.

- OpenAPI path: /DB/ACTL GET
- 응답 example: ACTL 아래 숫자 문자열 키를 가지는 object
- 관련 schema ref: DTO_ACTL_RESPONSE_MAP_BODY
- Supabase 메타데이터: endpoints, versions, spec_data, runner_data 구조

이 경우 AI가 기대해야 하는 핵심 판단은 다음과 같다.

- 최상위는 wrapper 응답 모델이다.
- ACTL 은 배열이 아니라 map 구조일 가능성이 높다.
- map value 는 ARDC, ANRC, CSECF, TRS, BMSTRESS, CRBAR, CLATS, ITER, TOL 필드를 가진 하위 모델이다.
- ITER 는 int, TOL 은 float, 나머지는 bool 후보로 본다. 단 최종 결정은 schema 와 example 교차 확인 후 내려야 한다.