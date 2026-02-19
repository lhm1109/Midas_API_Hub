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
