# BAT_RUN CLI Planning (AI Wrapper + MCP + Supabase)

## 1) 문서 목적
이 문서는 `apiverification` 프로젝트에서 Windows BAT 기반 CLI를 설계하기 위한 기획서다.

핵심 목표는 다음 3가지다.

- CLI가 Codex/Claude Code 인증 세션을 래핑해 AI 명령 실행 진입점이 된다.
- AI가 스키마 생성/수정과 실행 중 앱(`npm run dev`) 편집을 같은 인터페이스로 수행한다.
- Supabase + 로컬 캐시 기반으로 스키마를 빠르게 검색하고, 이후 API 직접 사용 자동화 기반을 만든다.

## 2) 배경과 문제 정의
현재 프로젝트 자산:

- MCP 서버(`mcp-server`) 기반 스키마 저장/검증 툴
- 스키마 산출물(`generated_schemas`, `generated_resources`)
- Supabase 기반 운영 데이터
- Node/TypeScript 기반 앱 + 개발 서버(`npm run dev`)

현재 문제:

- AI 호출 경로가 분산되어 재현성이 낮음
- 개발자 작업(스키마 생성, 코드 수정, 앱 편집)이 하나의 명령면으로 통합되지 않음
- 스키마 검색이 파일 탐색 중심이라 느림

## 3) 설계 원칙

1. 단일 진입점: Windows에서는 `bat_run` 명령 하나로 시작한다.
2. AI 래퍼 우선: `validate -> plan -> apply`는 보조 파이프라인으로 유지하고, 1차 목표는 AI 작업 실행 표준화다.
3. 모드 분리: `developer mode`를 먼저 완성하고 `user mode`는 후속으로 단순화한다.
4. MCP 재사용: 기존 MCP 툴 체계를 그대로 활용한다.
5. 빠른 검색: 스키마 본문 추론보다 메타 인덱스 검색을 우선한다.
6. 기계 친화 출력: 모든 명령은 `--output json`을 지원한다.

## 4) 운영 모드 정의

### 4.1 Developer Mode (우선 개발)

- 대상: 내부 개발자/AI 에이전트
- 기능: 인증, AI 프롬프트 실행, 스키마 생성/저장, 코드/파일 수정, 실행 앱 편집 지원
- 특징: 고권한 기능 포함, 상세 로그/trace 제공

### 4.2 User Mode (후순위)

- 대상: 일반 사용자/운영자
- 기능: 검증된 프리셋 실행, 조회/읽기 중심
- 특징: 제한된 명령, 파괴적 기능 비활성화

## 5) 목표 범위 (v1)

- `bat_run` 터미널 진입점 제공
- Codex/Claude Code 인증 세션 연결(`auth`)
- AI 명령 실행 래퍼(`ai run`) 구현
- 스키마 검색/조회/생성/저장(`schema`) 구현
- 이미지 입력(파일/드래그앤드롭/클립보드) 기반 스키마 생성 구현
- 실행 중 앱 편집 워크플로(`app`) 구현
- Supabase -> 로컬 캐시 동기화 + 고속 검색 구현

비목표(v1 제외):

- 완전 무인 배포 자동화
- 사용자 모드 완전 제품화
- 대규모 멀티노드 오케스트레이션

## 6) 전체 아키텍처

```text
[Developer / AI]
   |
   v
bat_run
   |
   v
Node CLI Core (TypeScript)
   |-- Command Router
   |-- Mode Guard (developer/user)
   |-- Output Formatter (text/json)
   |
   |-- Auth Layer
   |     |-- provider: codex
   |     `-- provider: claude-code
   |
   |-- AI Runner
   |     |-- prompt executor
   |     `-- file edit / repo action adapter
   |
   |-- MCP Connector
   |     |-- save_schema
   |     |-- parse_dialog
   |     `-- rules resources
   |
   |-- App Connector
   |     `-- running dev app context (npm run dev)
   |
   `-- Schema Cache
         |-- local files
         `-- index.sqlite (FTS)
```

## 7) BAT_RUN 명령 진입 설계

### 7.1 파일 위치

- `scripts/cli/bat_run.cmd`
- `scripts/cli/src/*` (TypeScript CLI)
- `scripts/cli/dist/index.js` (빌드 산출물)
- `%APPDATA%\\npm\\bat_run.cmd` (전역 래퍼, 권장)

### 7.2 실행 흐름

`bat_run` -> Node 엔트리 -> 명령 디스패치

예시:

```cmd
@echo off
setlocal
set ROOT=%~dp0..\..
node "%ROOT%\scripts\cli\dist\index.js" %*
endlocal
```

### 7.3 진입 UX (codex 스타일)

- `bat_run` (인자 없음): 대화형 세션 진입
- `bat_run exec "<prompt>"`: 비대화형 1회 실행
- `bat_run help`: 전체 명령 확인

## 8) CLI 명령 스펙 (v1, Developer Mode 중심)

### 8.1 공통 옵션

- `--output text|json` (기본 `text`)
- `--mode developer|user` (기본 `developer`)
- `--profile local|dev|staging|prod`
- `--trace-id <id>`
- `--non-interactive` (AI 연동용)

### 8.2 인증 명령

1. `bat_run auth login --provider codex`
2. `bat_run auth login --provider claude-code`
3. `bat_run auth status`
4. `bat_run auth logout --provider <name>`

설계 메모:

- OAuth/세션 토큰은 OS 보안 저장소 또는 사용자 홈 암호화 파일에 저장
- 민감정보는 로그/출력에 마스킹

### 8.3 AI 실행 명령

1. `bat_run ai run --provider <name> --prompt "<text>"`
- 범용 AI 명령 실행

2. `bat_run ai run --provider <name> --prompt-file <file.md>`
- 긴 작업 프롬프트 실행

3. `bat_run ai edit --provider <name> --target <path|glob> --instruction "<text>"`
- 코드/문서 수정 작업 실행

### 8.4 스키마 명령

1. `bat_run schema sync`
- Supabase -> 로컬 캐시 증분 동기화

2. `bat_run schema find <query>`
- skill.sh 스타일 고속 검색(이름/설명/태그/캡빌리티)

3. `bat_run schema show <schema_id> [--version latest|<semver>]`
- 스키마 상세 조회

4. `bat_run schema create --provider <name> --prompt-file <file.md>`
- AI로 스키마 초안 생성

5. `bat_run schema save --input <schema.json>`
- MCP `save_schema` 연계 저장/검증

6. `bat_run schema create --provider <name> --image <path> --prompt "<text>" [--code-search "<query>"] [--dialog-class <ClassName>]`
- 이미지 + 사용자 프롬프트 + 코드 컨텍스트를 결합해 스키마 초안 생성

### 8.5 앱 편집 명령

1. `bat_run app attach --dev-url http://localhost:5178 --root <repo_path>`
- 실행 중 개발 앱 컨텍스트 등록

2. `bat_run app edit --provider <name> --instruction "<text>"`
- 앱 구조/코드 편집 명령 실행

3. `bat_run app snapshot`
- 현재 앱 상태/파일 변경 요약 출력

### 8.6 보조 검증 파이프라인 명령

- `bat_run validate --schema <id> --input <file>`
- `bat_run plan --schema <id> --input <file> --out <plan.json>`
- `bat_run apply --plan <plan.json> [--dry-run] [--approve-token <token>]`

`validate/plan/apply`는 남겨두되, v1의 중심은 `auth + ai + schema + app`이다.

### 8.7 이미지 입력 UX (드래그앤드롭/클립보드)

1. 파일 기반
- `bat_run schema create --image "C:\\path\\ui.png" --prompt "<text>"`
- 다중 이미지: `--image` 반복 허용

2. 드래그앤드롭
- 터미널에 이미지 파일을 드래그하면 경로가 자동 입력됨
- `bat_run schema create --image <드롭된경로> --prompt "<text>"` 형태로 즉시 실행

3. 클립보드 붙여넣기
- `bat_run schema create --from-clipboard --prompt "<text>"`
- 내부 동작: 클립보드 이미지 -> 임시 PNG 저장 -> AI/MCP 파이프라인 전달

4. provider별 처리
- Codex: `codex exec -i <image>` 경로 사용
- Claude Code: 현 버전 CLI 도움말 기준 이미지 인자 미노출, v1은 OCR/메타추출 fallback 또는 업데이트 후 네이티브 이미지 연계

## 9) 로컬 캐시/인덱스 설계

### 9.1 디렉토리

- `.avcache/schemas/<schema_id>/<version>.json`
- `.avcache/index.sqlite`
- `.avcache/sync_state.json`

### 9.2 인덱스 테이블(초안)

`schema_index`

- `schema_id` (text)
- `version` (text)
- `name` (text)
- `description` (text)
- `tags` (text/json)
- `capabilities` (text/json)
- `source` (text: supabase/local/generated)
- `updated_at` (text)
- `checksum` (text)
- `search_text` (fts5)

### 9.3 검색 성능 목표

- 5,000 스키마 기준 평균 검색 200ms 이내
- 증분 sync 후 인덱스 반영 3초 이내(소규모 변경 기준)

## 10) Supabase 연계 설계

권장 테이블:

1. `schema_registry`
- `id`, `schema_id`, `version`, `json_schema`, `description`, `tags`, `capabilities`, `checksum`, `is_active`, `updated_at`

2. `schema_alias`
- `alias`, `schema_id`, `target_version`

3. `schema_run_log`
- `run_id`, `mode`, `provider`, `command`, `input_ref`, `result`, `status`, `created_at`, `trace_id`

동기화:

- `schema sync`는 `updated_at > last_sync_at` 기준 증분 pull
- 충돌 시 `version` + `checksum` 우선

## 11) MCP 통합 설계

CLI는 MCP의 기능을 대체하지 않고 래핑/연결한다.

핵심 연결 포인트:

- `schema save` -> `save_schema`
- `schema create` 후 `save_schema`로 검증/저장
- `schema create --from-dialog` 계열 -> `parse_dialog`
- `schema create --code-search "<query>"` -> C/C++ 코드 컨텍스트 수집(검색 + 구조 파싱)

### 11.1 이미지 + 프롬프트 + 코드컨텍스트 스키마 생성 파이프라인

1. 입력 수집
- 사용자 프롬프트
- 이미지(`--image` 또는 `--from-clipboard`)
- 선택 코드 컨텍스트(`--code-search`, `--dialog-class`)

2. 코드 컨텍스트 생성
- `rg` 기반 C/C++ 코드 검색
- MFC 구조 파싱은 `parse_dialog` MCP 사용 (예: `CNodeCreateDlg`)

3. AI 초안 생성
- 이미지 + 프롬프트 + 코드 컨텍스트를 하나의 작업 컨텍스트로 합성
- AI가 스키마 초안을 생성

4. MCP 저장/검증
- `save_schema` 호출로 규칙 검증/자동 변환/질문 생성 처리
- 결과가 `pending_questions`이면 질문 루프, `saved`면 산출물 확정

개발자 모드 기본 흐름:

1. `auth login`
2. `schema find/show`로 컨텍스트 수집
3. `schema create --image ... --prompt ... [--code-search ...]`
4. `schema save` (MCP 검증)
5. 필요 시 `validate/plan/apply`

## 12) 보안/운영 정책

1. 인증정보 관리
- provider 토큰은 하드코딩 금지
- 마스킹 로깅 + 토큰 만료 처리

2. 실행 권한
- `developer` 모드에서만 `ai edit`, `app edit`, `apply` 허용
- `user` 모드는 조회/검증 중심

3. 감사 추적
- 모든 실행에 `run_id`, `trace_id`, `provider`, `mode` 기록
- 재현 가능한 입력 스냅샷 보관

## 13) 실현 가능성 검토 (2026-03-12 기준)

### 13.1 환경 검증 결과

1. Codex CLI
- 명령 확인: `codex --version`
- 결과: `codex-cli 0.108.0-alpha.12`
- 로그인 상태 확인: `codex login status` 정상 동작
- 현재 상태: API key 로그인 확인됨 (`Logged in using an API key`)

2. Claude Code CLI
- 명령 확인: `claude --version` -> `1.0.80`
- 보조 확인: `claude --help` 정상 동작
- 제한 사항: `claude auth status` 실행 시 업데이트 요구 메시지 확인
- 결론: Claude provider 연동은 CLI 업데이트(`claude update`) 선행 필요

3. 개발 앱 연계
- 명령 확인: `npm run dev -- --help` (repo root)
- 결과: Vite dev server help 정상 출력
- 결론: `app attach`/`app edit` 대상 런타임(`npm run dev`)은 현재 환경에서 실현 가능

4. MCP 연계
- 명령 확인: `npm run build` (`mcp-server`)
- 결과: 빌드 성공
- 결론: CLI에서 MCP tool wrapper(`save_schema`, `parse_dialog`) 연결 가능

5. 이미지 입력 경로
- 명령 확인: `Get-Command Get-Clipboard` 정상
- 결과: `Get-Clipboard` 사용 가능, `-Format Image` 지원 확인(현재 클립보드 이미지는 없음)
- 명령 확인: `codex exec --help`
- 결과: `-i, --image <FILE>...` 지원 확인
- 명령 확인: `claude --help`
- 결과: 이미지 인자 미노출(현 버전은 fallback 전략 필요)

### 13.2 실현 가능성 요약

- `developer mode` v1: 실현 가능 (즉시 착수 가능)
- `bat_run auth`의 Codex provider: 실현 가능 (현재 설치/로그인 상태 확인)
- `bat_run auth`의 Claude provider: 조건부 가능 (업데이트 후 적용)
- `bat_run schema` + MCP + 로컬 캐시: 실현 가능 (기존 자산 활용)
- `bat_run app`(실행 앱 편집): 실현 가능 (dev server 확인)
- `bat_run schema create --image / --from-clipboard`: 실현 가능 (Codex 네이티브 이미지 + Windows 클립보드 API)
- OAuth 직접 구현: v1에서는 비권장, provider CLI 로그인 세션 재사용 방식이 현실적

### 13.3 주요 리스크와 대응

1. CLI 명령명 충돌 가능성
- 리스크: 짧은 명령어는 기존 PATH 도구와 충돌 가능
- 대응: 기본 바이너리를 `bat_run`으로 고정하고 별칭은 선택 제공

2. Provider 인증 방식 차이
- 리스크: Codex/Claude 인증 방식 및 세션 포맷이 다름
- 대응: `auth adapter` 인터페이스 분리 (`provider`별 구현)

3. 앱 편집의 안전 경계
- 리스크: AI 편집이 의도치 않은 파일을 변경할 수 있음
- 대응: 허용 경로 allowlist + 변경 전/후 diff 강제 출력

4. 클립보드 이미지 획득 실패
- 리스크: 원격 세션/헤드리스 환경에서 클립보드 접근 실패 가능
- 대응: `--from-clipboard` 실패 시 즉시 `--image <path>` fallback 안내

## 14) 구현 로드맵

### Phase 1 (우선, 1주)

- `bat_run.cmd` + CLI 뼈대
- `auth`/`ai run` 최소 구현
- `schema create --image`/`--from-clipboard` 입력 경로 구현
- 표준 출력 포맷(`--output json`)

### Phase 2 (1주)

- `schema sync/find/show`
- 로컬 캐시 + SQLite FTS 인덱스
- 검색 성능 측정

### Phase 3 (1주)

- `schema create/save` + MCP 연동
- `schema create --code-search` + `parse_dialog` 컨텍스트 결합
- `app attach/edit/snapshot` 구현
- 실행 로그 저장

### Phase 4 (1주)

- `validate/plan/apply` 보조 파이프라인 정리
- `user` 모드 제약 정책 적용
- 운영 가이드/장애 대응 문서화

## 15) 완료 기준 (Definition of Done)

1. `bat_run auth login --provider codex` 또는 `claude-code` 세션 상태를 `auth status`로 확인할 수 있다.
2. `bat_run schema find "design"`가 1초 이내 응답한다.
3. `bat_run schema create --image ... -> schema save` 흐름이 MCP 검증까지 포함해 동작한다.
4. `bat_run schema create --from-clipboard`가 임시 이미지 저장 후 동일 파이프라인으로 동작한다.
5. `bat_run app attach -> app edit`로 개발 앱 대상 편집 워크플로가 동작한다.
6. 모든 핵심 명령이 `--output json`에서 파싱 가능한 결과를 반환한다.

## 16) 오픈 이슈

1. provider별 인증 구현 방식(로컬 세션 재사용 vs 별도 OAuth device flow) 확정 필요
2. Claude Code 이미지 입력의 네이티브 지원 버전/전략 확정 필요
3. 실행 중 앱 편집의 안전 경계(허용 파일/금지 파일) 정책 확정 필요
4. `user` 모드에 허용할 최소 명령 세트 확정 필요

---

이 문서는 `developer mode` 우선 기준의 v1 기획이며, 구현 시작 시 `docs/BAT_CLI_IMPLEMENTATION_PLAN.md`에서 단계별 태스크로 분해한다.
