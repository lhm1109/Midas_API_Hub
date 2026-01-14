# Schema Rendering Definitions

JSON Schema를 UI로 렌더링하는 규칙을 YAML로 정의합니다.

## 폴더 구조

```
schema_definitions/
├── x-extensions-v3.yaml  # 🔥 V3: 완전 동적 X-Extension 시스템
│                          # - 동작 로직도 YAML로 정의
│                          # - Expression Language (DSL)
│                          # - Behavior Engine
│
├── original/             # 기존 JSON Schema (oneOf, allOf 등)
│   ├── ui-rules.yaml       # 공통 UI 렌더링 규칙
│   ├── builder.yaml        # Builder Tab 특화 규칙
│   ├── table.yaml          # Spec Tab 테이블 특화 규칙
│   └── (html-template.yaml)  # HTML 문서 생성 템플릿 (예정)
│
├── enhanced/             # Enhanced JSON Schema (x-ui, x-enum-by-type 등)
│   ├── ui-rules.yaml
│   ├── builder.yaml
│   ├── table.yaml
│   └── html-template.yaml
│
└── README.md
```

## 파일 설명

### 🔥 x-extensions-v3.yaml (V3 완전 동적 시스템)

**코드 수정 없이 YAML만으로 x-* 필드의 동작 로직까지 정의!**

```yaml
extensions:
  - name: "x-enum-by-type"
    behaviors:
      - trigger: "dependency-change"
        when:
          field: "TYPE"
        actions:
          - type: "update-enum"
            source: "self.enumByType[TYPE.value]"  # DSL 표현식
```

**자세한 내용:** `src/lib/engine/BEHAVIOR_ENGINE_README.md` 참고

---

### ui-rules.yaml
공통 UI 렌더링 규칙을 정의합니다:
- 필드 타입별 렌더링 컴포넌트 매핑
- 레이아웃 규칙 (섹션, 그룹 등)
- 스타일 규칙
- 조건부 렌더링 규칙

### builder.yaml
Builder Tab 전용 규칙:
- 폼 필드 렌더링 방식
- 입력 검증 규칙
- 기본값 처리
- 인스턴스 관리 (Assign wrapper)

### table.yaml
Spec Tab 테이블 전용 규칙:
- 테이블 컬럼 구성
- 행 그룹화 방식
- 섹션 헤더 스타일
- enum 값 표시 방식

### html-template.yaml
HTML 문서 생성 규칙:
- 문서 구조
- CSS 스타일
- 섹션 템플릿
- 필드 템플릿

## 사용 방법

Settings에서 사용할 정의를 선택:
```typescript
{
  schemaDefinition: "original" | "enhanced"
}
```

## 확장성

### 새 x-* 필드 추가 (V3)

**코드 수정 없이 YAML만 수정!**

```yaml
# 1. x-extensions-v3.yaml에 추가
extensions:
  - name: "x-my-custom-field"
    behaviors:
      - trigger: "value-change"
        actions:
          - type: "custom-action"

# 2. Schema에 사용
{
  "MY_FIELD": {
    "type": "string",
    "x-my-custom-field": "config"
  }
}

# 3. 끝! 새로고침만 하면 자동 작동! 🎉
```

### 새로운 렌더링 방식 추가

1. `schema_definitions/` 에 새 폴더 생성
2. YAML 파일 정의 (ui-rules, builder, table, html-template)
3. Settings에 옵션 추가

