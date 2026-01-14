# API Verification Backend Server

## 개요
MIDAS API Verification Platform의 백엔드 서버입니다.
SQLite를 사용하여 버전 및 데이터를 저장하고 관리합니다.

## 포트
- **9527** (일반적인 포트 3000, 8080 등을 피함)

## 서버 시작

```bash
# 백엔드 서버만 실행
npm run server

# 프론트엔드 + 백엔드 동시 실행
npm run dev:all

# Electron + 백엔드 + 프론트엔드 동시 실행
npm run electron:dev
```

## API 엔드포인트

### Health Check
- `GET /health` - 서버 상태 확인

### Versions
- `GET /api/versions` - 모든 버전 조회 (query: endpoint_id)
- `GET /api/versions/:id` - 특정 버전 조회
- `POST /api/versions` - 새 버전 생성
- `PUT /api/versions/:id` - 버전 업데이트
- `DELETE /api/versions/:id` - 버전 삭제

### Working Data (실시간 편집 데이터)
- `GET /api/data/manual` - Manual 데이터 조회
- `POST /api/data/manual` - Manual 데이터 저장
- `GET /api/data/spec` - Spec 데이터 조회
- `POST /api/data/spec` - Spec 데이터 저장
- `GET /api/data/builder` - Builder 데이터 조회
- `POST /api/data/builder` - Builder 데이터 저장
- `GET /api/data/runner` - Runner 데이터 조회
- `POST /api/data/runner` - Runner 데이터 저장
- `GET /api/data/all` - 모든 작업 데이터 조회
- `DELETE /api/data/all` - 모든 작업 데이터 초기화

## 데이터베이스 구조

### versions
- id (PK)
- version
- endpoint_id
- created_at
- updated_at
- author
- change_log

### manual_data
- version_id (FK)
- title, category, input_uri, active_methods
- json_schema, json_schema_original, json_schema_enhanced
- examples, specifications, html_content
- article_id, section_id, author_id, url

### spec_data
- version_id (FK)
- json_schema, json_schema_original, json_schema_enhanced
- specifications

### builder_data
- version_id (FK)
- form_data (JSON)

### runner_data
- version_id (FK)
- request_body, response_body

### test_cases
- id (PK)
- version_id (FK)
- name, description
- request_body
- created_at, updated_at

## 특징

- **자동 백업**: SQLite 파일로 데이터 영구 저장
- **버전 관리**: 각 API 엔드포인트별 버전 관리
- **실시간 동기화**: 작업 중인 데이터 실시간 저장
- **파일 공유**: 데이터베이스 파일을 공유하여 팀 협업 가능







