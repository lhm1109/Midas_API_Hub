# generated_pydantic 사용 가이드

이 폴더는 API Verification에서 생성한 **Pydantic v2 모델 패키지**입니다.  
이 폴더만 다른 Python 프로젝트로 복사해서 request/response payload 검증과 JSON 변환에 사용할 수 있습니다.

## 1. 포함 범위

포함:

- API별 Pydantic 모델 파일
- endpoint id -> Python module/class 매핑 `registry.py`
- 전체 목록 `manifest.json`

미포함:

- API 서버 접속 client
- `base_url`, `MAPI-Key` 저장/관리
- HTTP 요청 실행 로직

즉, 이 패키지는 payload 모델 계층입니다. 실제 API 호출과 인증 값 주입은 사용하는 프로젝트에서 처리해야 합니다.

## 2. 요구 사항

- Python 3.10 이상
- Pydantic v2

설치:

```bash
pip install "pydantic>=2"
```

API 호출까지 직접 하려면 예를 들어 `requests`도 설치합니다.

```bash
pip install requests
```

## 3. 폴더 복사 방식

다른 프로젝트에 아래처럼 `generated_pydantic` 폴더를 그대로 복사합니다.

```text
my_project/
  main.py
  generated_pydantic/
    __init__.py
    registry.py
    manifest.json
    verification/
    civil_nx/
    civil_nx_mec/
    gen_nx/
    test/
```

중요: 폴더명은 `generated_pydantic` 그대로 유지해야 합니다. `registry.py`의 import path가 이 이름을 기준으로 생성되어 있습니다.

## 4. 모델 불러오기

endpoint id로 모델을 가져올 수 있습니다.

```python
from generated_pydantic import get_model

OpenProjectRequest = get_model("doc/open/open-project")
```

직접 import도 가능합니다.

```python
from generated_pydantic.verification.doc_open_open_project import OpenProjectRequest
```

사용 가능한 endpoint id 목록은 아래처럼 확인합니다.

```python
from generated_pydantic import PYDANTIC_MODULES

for endpoint_id in PYDANTIC_MODULES:
    print(endpoint_id)
```

## 5. Payload 검증 및 JSON 변환

API 원본 키는 대문자 alias로 유지됩니다.  
제품 API로 보낼 때는 반드시 `by_alias=True`를 사용하세요.

```python
from generated_pydantic import get_model

OpenProjectRequest = get_model("doc/open/open-project")

payload = OpenProjectRequest.model_validate({
    "Argument": {
        "FILE_PATH": "C:/MIDAS/sample.mcb"
    }
})

body = payload.model_dump(by_alias=True, exclude_none=True)
print(body)
```

출력 예:

```python
{
    "Argument": {
        "FILE_PATH": "C:/MIDAS/sample.mcb"
    }
}
```

`populate_by_name=True`가 설정되어 있어서 Python field name으로도 생성할 수 있지만, 외부 API로 전송하는 JSON은 alias 기준을 권장합니다.

## 6. API 호출 예시

인증 정보는 코드에 직접 박지 말고 환경 변수나 설정 파일에서 주입하세요.

```python
import os
import requests
from generated_pydantic import get_model

base_url = os.getenv("MIDAS_BASE_URL", "https://api-beta.midasit.com/civil").rstrip("/")
mapi_key = os.environ["MIDAS_MAPI_KEY"]

OpenProjectRequest = get_model("doc/open/open-project")

payload = OpenProjectRequest.model_validate({
    "Argument": {
        "FILE_PATH": "C:/MIDAS/sample.mcb"
    }
})

response = requests.post(
    f"{base_url}/doc/OPEN",
    headers={
        "Content-Type": "application/json",
        "MAPI-Key": mapi_key,
    },
    json=payload.model_dump(by_alias=True, exclude_none=True),
    timeout=30,
)

response.raise_for_status()
print(response.json())
```

## 7. Response 모델이 있는 경우

일부 endpoint는 request/response 모델을 함께 가집니다.  
`get_models()`로 해당 endpoint의 모든 root model을 확인할 수 있습니다.

```python
from generated_pydantic import get_models

models = get_models("db/epmt-m1")
print(models.keys())

ResponseModel = models.get("DbEPMTM1Response")
if ResponseModel:
    parsed = ResponseModel.model_validate(response.json())
```

## 8. 주의 사항

- 이 폴더는 API 호출 SDK가 아니라 Pydantic 모델 패키지입니다.
- `MAPI-Key`, `base_url`, retry, timeout, proxy, logging은 사용하는 쪽에서 관리합니다.
- endpoint별 실제 method/path는 API 매뉴얼, 스펙, 또는 각 생성 파일 상단 주석을 확인하세요.
- `manifest.json`은 endpoint id, import path, class 이름을 찾는 용도입니다.
- 모델 검증을 통과해도 제품 API의 런타임 상태, 라이선스, 모델 파일 상태, 서버 권한 오류까지 보장하지는 않습니다.

## 9. MCP 서버로 AI 코딩 도구에 연결하기

이 폴더에는 AI 코딩 도구가 Pydantic 모델과 API 스펙을 바로 조회할 수 있는 MCP 서버가 포함되어 있습니다.

제공 도구:

- `list_endpoints`: 생성된 endpoint id 목록 검색
- `get_pydantic_model`: endpoint의 Pydantic module/class/source 조회
- `get_api_spec`: Supabase에서 최신 API spec schema 조회
- `get_endpoint_context`: Pydantic 모델과 API spec을 한 번에 조회
- `get_usage_guide`: 이 README 내용 조회

### MCP 의존성 설치

```bash
cd my_project
pip install -r generated_pydantic/requirements-mcp.txt
```

### Supabase 설정

API spec 조회가 필요하면 `generated_pydantic/.env.example`을 `generated_pydantic/.env`로 복사한 뒤 내부용 값을 채웁니다.

```bash
SUPABASE_URL=https://your-project.supabase.co
SUPABASE_SERVICE_KEY=your-internal-service-role-key
```

주의: 실제 service role key는 zip/source에 포함하지 말고 승인된 내부 경로로만 전달하세요. `.gitignore`는 `.env`를 제외하도록 설정되어 있습니다.

### MCP 실행 명령

AI 코딩 도구의 MCP 설정에 아래 command를 등록합니다.

```bash
python -m generated_pydantic.mcp_server
```

예시 설정:

```json
{
  "mcpServers": {
    "generated-pydantic-api": {
      "command": "python",
      "args": ["-m", "generated_pydantic.mcp_server"],
      "cwd": "/path/to/my_project"
    }
  }
}
```

Windows 예시:

```json
{
  "mcpServers": {
    "generated-pydantic-api": {
      "command": "python",
      "args": ["-m", "generated_pydantic.mcp_server"],
      "cwd": "C:/path/to/my_project"
    }
  }
}
```

MCP 서버는 먼저 `generated_pydantic/.env`를 읽고, 없으면 현재 작업 디렉터리의 `.env`를 읽습니다.
