# Spec Tab Enhanced 테이블: 조건 헤더 누락

날짜: 2026-02-19

## 문제
Spec Tab의 Enhanced 테이블에서 중첩 필드의 조건 헤더가 표시되지 않았다.
예: `BEAM_COL_JNT_DES` 조건 헤더(조건 충족 시 Optional)가 `JOINT` 항목 위에 나타나지 않았다.

## 원인
Enhanced 테이블 경로에 다음 두 문제가 있었다.
1) 조건 그룹핑이 최상위 필드와 비조건 블록의 자식에만 적용됨.
   조건 필드 블록에서는 자식을 바로 매핑하여 `x-optional-when` / `x-required-when` 헤더가 생성되지 않음.
2) 렌더러가 3-depth(손자) 섹션 헤더를 처리하지 못해, 생성된 헤더가 렌더링에서 누락됨.

## 해결 요약
- SpecTab의 조건 필드 블록 내부에서도 자식 조건 그룹핑을 적용.
  이를 통해 "Optional (When \"BEAM_COL_JNT_DES\" is true)" 같은 헤더가 생성됨.
- DynamicTableRenderer에서 3-depth 섹션 헤더 렌더링을 추가.

## 코드 변경
- SpecTab 매핑:
  - 조건 필드 블록에 자식 그룹핑 및 헤더 삽입 추가.
  - 파일: src/features/projects/components/tabs/SpecTab.tsx
- 렌더러:
  - 3-depth 섹션 헤더 처리 추가.
  - 파일: src/lib/rendering/dynamicTableRenderer.tsx

## 확인 방법
1) Spec Tab을 Enhanced 모드로 열기.
2) SEISMIC을 펼치기.
3) `JOINT` 위에 "Optional (When \"BEAM_COL_JNT_DES\" is true)" 헤더가 표시되는지 확인.
4) 3-depth 조건 헤더가 정상적으로 렌더링되는지 확인.

---

# 재발 방지 가이드 (Spec Tab 테이블 작업)

목표: Enhanced/Original 테이블에서 중첩/조건/배열 아이템 누락을 사전에 막는다.

## 1) 작업 전 체크리스트
- 어떤 스키마가 렌더링되는지 확인 (Enhanced 탭이면 `jsonSchemaEnhanced` 우선, 없으면 `jsonSchemaOriginal`).
- 테이블 YAML 정의가 로드되는지 확인 (콘솔: "Loaded table definition").
- 필드 깊이(2-depth/3-depth/4-depth)와 구조 타입(object/array/items.properties)을 먼저 메모.

## 2) 데이터 흐름 요약
1. SpecTab에서 스키마 선택 → tableParameters 생성
2. `compileEnhancedSchema`로 섹션/필드 트리 생성
3. `collectFieldConditionInfo` + `groupFieldsByCondition`로 조건 그룹핑
4. `DynamicTableRenderer`가 section/children 렌더링

## 3) 실패 지점 패턴
### A. 조건 헤더가 안 보일 때
- 조건 헤더는 "조건 그룹핑" 단계에서 만들어짐.
- 조건 블록 내부(childrenWithCondition)에서도 반드시 그룹핑을 다시 적용해야 함.

### B. 3-depth/4-depth가 안 보일 때
- `schemaCompiler`에서 children 생성 여부 확인.
- 렌더러가 해당 depth의 section-header를 처리하는지 확인.

### C. array items.properties가 안 보일 때
- 구조가 `object → array(items.properties)` 인지 확인.
- `schemaCompiler`가 해당 depth에서 items.properties를 children으로 확장하는지 확인.

## 4) 변경 포인트 가이드
### SpecTab 매핑 (tableParameters 생성)
- 조건 없는 블록과 조건 블록 모두에서 children 그룹핑을 수행.
- array field는 items.properties가 있으면 fallback으로 children 생성.

### schemaCompiler (필드 트리 생성)
- object children
- array items.properties
- object → array(items.properties)
- array items → object properties

## 5) 빠른 디버깅 절차
1. 콘솔에서 `compileEnhancedSchema` 결과에 children이 붙었는지 확인.
2. `tableParameters` 생성 결과에서 children이 포함됐는지 확인.
3. 렌더러에서 children 렌더링 조건(expanded/section-header)이 정상인지 확인.

## 6) 최소 검증 시나리오
- 조건 헤더가 있는 필드 1개
- 3-depth + array items.properties 필드 1개
- Enhanced/Original 각각 1회 확인

## 7) 관련 코드 위치
- SpecTab 매핑: src/features/projects/components/tabs/SpecTab.tsx
- 테이블 렌더러: src/lib/rendering/dynamicTableRenderer.tsx
- 스키마 컴파일러: src/lib/schema/schemaCompiler.ts
- 조건 추출: src/lib/schema/conditionExtractor.ts
