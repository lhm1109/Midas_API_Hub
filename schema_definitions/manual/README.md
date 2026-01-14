# 📚 YAML 스키마 정의 매뉴얼

> API Verification 시스템의 YAML 기반 스키마 정의 완전 가이드

---

## 📖 문서 목록

### 0️⃣ [스키마 작성 계약 규칙 ⭐ (SCHEMA_CONTRACT_KO.md)](./SCHEMA_CONTRACT_KO.md)

**대상**: Enhanced JSON Schema를 작성하는 모든 사용자 (필독!)

**내용**:
- 🎯 핵심 원칙: Single Entity Schema
- 🚀 Transport vs Schema 분리
- 🔢 Multiple Instances 처리
- 🏗️ 중첩 객체 처리
- ❌ 흔한 실수와 해결 방법
- 📋 x-exclusive-keys 사용법
- 🔍 스키마 검증 체크리스트

**특징**:
- **반드시 지켜야 하는 규칙** (규칙 위반 시 시스템 오작동)
- 잘못된 예시와 올바른 예시 비교
- 실전 체크리스트 제공

---

### 1️⃣ [YAML 완전 가이드 (YAML_GUIDE_KO.md)](./YAML_GUIDE_KO.md)

**대상**: 코딩 경험이 없거나 YAML을 처음 접하는 사용자

**내용**:
- 📘 서론: YAML이란 무엇인가?
- 📐 기본 이론: YAML의 기초 문법
- ✏️ 실전: 스키마 정의 작성하기
- 🔬 심화: 시스템 동작 원리
- 📚 참고 자료 및 문제 해결

**특징**:
- ASCII 아트를 활용한 시각적 설명
- 단계별 예제
- 초보자 친화적인 설명

---

### 2️⃣ [YAML 실전 예제 모음 (YAML_PRACTICAL_EXAMPLES.md)](./YAML_PRACTICAL_EXAMPLES.md)

**대상**: 실제 업무에 바로 적용하고 싶은 사용자

**내용**:
- 🌱 초급 예제: 기본 섹션 규칙, 타입별 분류
- 🌿 중급 예제: 복합 조건, 검증 레이어
- 🌳 고급 예제: 동적 레이블, 복잡한 조건 매칭
- 🏗️ 실전 프로젝트: 완전한 ELEM 스키마 로직
- 📝 실습 문제 (정답 포함)

**특징**:
- 복사해서 바로 사용 가능한 코드
- 실제 운영 환경 예제
- 테스트 케이스 포함

---

## 🗂️ 폴더 구조

```
schema_definitions/
├── manual/                          # 📚 이 폴더
│   ├── README.md                    # 📄 이 문서
│   ├── SCHEMA_CONTRACT_KO.md        # ⭐ 스키마 작성 계약 규칙 (필독!)
│   ├── YAML_GUIDE_KO.md             # 📘 완전 가이드
│   └── YAML_PRACTICAL_EXAMPLES.md   # 📗 실전 예제
│
├── enhanced/                        # 🔧 Enhanced 스키마 정의
│   ├── ui-rules.yaml                # UI 공통 규칙
│   ├── builder.yaml                 # Builder 탭 렌더링 규칙
│   ├── table.yaml                   # Table 렌더링 규칙
│   ├── html-template.yaml           # HTML 생성 템플릿
│   └── schema-logic.yaml            # 🔥 스키마 로직 (섹션/검증)
│
└── original/                        # 📋 Original 스키마 정의
    ├── ui-rules.yaml                # UI 공통 규칙
    ├── builder.yaml                 # Builder 탭 렌더링 규칙
    └── table.yaml                   # Table 렌더링 규칙
```

---

## 🚀 빠른 시작

### 0단계: 계약 규칙 숙지 ⭐

**먼저 [SCHEMA_CONTRACT_KO.md](./SCHEMA_CONTRACT_KO.md)를 읽으세요!**

```
예상 시간: 10분
습득 내용: 스키마 작성 시 반드시 지켜야 하는 규칙
중요도: ⭐⭐⭐⭐⭐ (필수)
```

**이 단계를 건너뛰면:**
- ❌ 스키마가 시스템에서 인식되지 않음
- ❌ UI가 깨짐
- ❌ 검증이 실패함

### 1단계: 기본 이해

[YAML_GUIDE_KO.md](./YAML_GUIDE_KO.md)의 **1. 서론**과 **2. 기본 이론**을 읽으세요.

```
예상 시간: 15분
습득 내용: YAML 기본 문법, 들여쓰기 규칙, 주석 작성법
```

### 2단계: 실전 연습

[YAML_PRACTICAL_EXAMPLES.md](./YAML_PRACTICAL_EXAMPLES.md)의 **초급 예제**를 따라 해보세요.

```
예상 시간: 30분
습득 내용: 기본 섹션 규칙 작성, 타입별 분류
```

### 3단계: 심화 학습

[YAML_GUIDE_KO.md](./YAML_GUIDE_KO.md)의 **4. 심화**를 읽고, [YAML_PRACTICAL_EXAMPLES.md](./YAML_PRACTICAL_EXAMPLES.md)의 **중급/고급 예제**를 실습하세요.

```
예상 시간: 1시간
습득 내용: 시스템 동작 원리, 복잡한 조건 작성, 검증 레이어
```

### 4단계: 실전 프로젝트

[YAML_PRACTICAL_EXAMPLES.md](./YAML_PRACTICAL_EXAMPLES.md)의 **실전 프로젝트**를 참고하여 실제 업무에 적용하세요.

```
예상 시간: 30분
습득 내용: 운영 환경 적용, 테스트, 디버깅
```

---

## 📋 학습 로드맵

```
┌──────────────────────────────────────────────────────────────┐
│                                                               │
│                  YAML 스키마 정의 마스터 로드맵               │
│                  ═══════════════════════════════              │
│                                                               │
│  0️⃣ 계약 규칙 (시작 전)                                      │
│  ┌────────────────────────────────────────────────┐         │
│  │ ⭐ SCHEMA_CONTRACT_KO.md 필독!                 │         │
│  │ • Single Entity Schema 원칙                    │         │
│  │ • Transport vs Schema 분리                     │         │
│  │ • 흔한 실수 방지                               │         │
│  └────────────────────────────────────────────────┘         │
│           ↓                                                   │
│  1️⃣ 기초 (1주차)                                             │
│  ┌────────────────────────────────────────────────┐         │
│  │ • YAML 기본 문법                                │         │
│  │ • 키-값 쌍, 리스트, 중첩 구조                   │         │
│  │ • 주석 및 들여쓰기 규칙                         │         │
│  └────────────────────────────────────────────────┘         │
│           ↓                                                   │
│  2️⃣ 실전 (2주차)                                             │
│  ┌────────────────────────────────────────────────┐         │
│  │ • 섹션 규칙 작성                                │         │
│  │ • 조건 평가 이해                                │         │
│  │ • 액션 정의                                     │         │
│  └────────────────────────────────────────────────┘         │
│           ↓                                                   │
│  3️⃣ 심화 (3주차)                                             │
│  ┌────────────────────────────────────────────────┐         │
│  │ • 복잡한 조건 조합                              │         │
│  │ • 검증 레이어 설계                              │         │
│  │ • 동적 레이블 활용                              │         │
│  └────────────────────────────────────────────────┘         │
│           ↓                                                   │
│  4️⃣ 마스터 (4주차)                                           │
│  ┌────────────────────────────────────────────────┐         │
│  │ • 실전 프로젝트 적용                            │         │
│  │ • 성능 최적화                                   │         │
│  │ • 디버깅 및 문제 해결                           │         │
│  └────────────────────────────────────────────────┘         │
│           ↓                                                   │
│  🎓 졸업!                                                     │
│  ┌────────────────────────────────────────────────┐         │
│  │ 이제 당신은 YAML 스키마 정의 전문가입니다!      │         │
│  └────────────────────────────────────────────────┘         │
│                                                               │
└──────────────────────────────────────────────────────────────┘
```

---

## 🔧 주요 파일 설명

### schema-logic.yaml

**위치**: `schema_definitions/enhanced/schema-logic.yaml`

**역할**: 스키마의 핵심 로직을 정의
- 📦 섹션 분류 규칙
- 🔢 섹션 표시 순서
- 🏷️ 타입 레이블 매핑
- ✅ 검증 레이어 규칙

**편집 방법**: [YAML_GUIDE_KO.md](./YAML_GUIDE_KO.md)의 **3. 실전** 섹션 참조

---

### builder.yaml / table.yaml

**위치**: 
- `schema_definitions/enhanced/builder.yaml`
- `schema_definitions/enhanced/table.yaml`

**역할**: UI 렌더링 규칙 정의
- 🎨 스타일 정의
- 📋 레이아웃 구조
- 🔤 필드 포맷

**편집 방법**: 기존 패턴을 참고하여 수정

---

## 🆘 문제 해결

### 자주 묻는 질문 (FAQ)

<details>
<summary><b>Q1: YAML 파일을 수정했는데 변경사항이 반영되지 않아요</b></summary>

**A1**: 브라우저 캐시 문제일 수 있습니다.

1. 브라우저 완전히 새로고침 (Ctrl+Shift+R / Cmd+Shift+R)
2. 개발자 도구 → Network → "Disable cache" 체크
3. 브라우저 재시작

```javascript
// 또는 콘솔에서 캐시 초기화
import { clearSchemaLogicCache } from '@/lib/schema/schemaLogicEngine';
clearSchemaLogicCache();
window.location.reload();
```

</details>

<details>
<summary><b>Q2: YAML 파싱 에러가 발생해요</b></summary>

**A2**: YAML 문법 오류일 가능성이 높습니다.

**체크리스트**:
- [ ] 들여쓰기가 스페이스 2칸인가? (탭 X)
- [ ] 콜론(:) 뒤에 공백이 있는가?
- [ ] 따옴표("")가 제대로 닫혔는가?
- [ ] 리스트 하이픈(-) 뒤에 공백이 있는가?

**온라인 검증 도구**:
- http://www.yamllint.com/
- https://codebeautify.org/yaml-validator

</details>

<details>
<summary><b>Q3: 섹션이 예상대로 분류되지 않아요</b></summary>

**A3**: 규칙 우선순위 문제일 수 있습니다.

**해결 방법**:
1. `sectionRules`는 위에서부터 순서대로 평가됩니다
2. 더 구체적인 규칙을 위에, 일반적인 규칙을 아래에 배치
3. 마지막에는 항상 `type: "always"` 규칙 추가 (폴백)

```yaml
sectionRules:
  # ✅ 구체적 (위)
  - name: "Explicit Group"
    condition:
      type: "has-explicit-group"
  
  # ✅ 중간
  - name: "Tension and Compression"
    condition:
      type: "visible-types-include-all"
      types: ["TENSTR", "COMPTR"]
  
  # ✅ 일반적 (아래)
  - name: "Fallback"
    condition:
      type: "always"
    action:
      type: "assign-section"
      section: "Advanced"
```

</details>

<details>
<summary><b>Q4: 새로운 조건 타입을 추가하고 싶어요</b></summary>

**A4**: 현재 시스템은 미리 정의된 조건 타입만 지원합니다.

**사용 가능한 조건 타입**:
- `has-explicit-group`
- `no-visible-when`
- `visible-types-include-all`
- `visible-types-include-any`
- `visible-types-exact`
- `visible-types-match`
- `visible-types-multiple`
- `always`

새로운 조건 타입이 필요하면 개발팀에 문의하세요.

</details>

---

## 🔗 추가 자료

### 시스템 아키텍처

시스템의 전체 구조를 이해하고 싶다면:
- `doc/architecture/README.md` (준비 중)

### 코드 레퍼런스

실제 코드를 확인하고 싶다면:
- `src/lib/schema/schemaLogicEngine.ts` - YAML 로드 및 실행
- `src/lib/schema/enhancedSchemaCompiler.ts` - 스키마 컴파일
- `src/features/projects/components/tabs/BuilderTab.tsx` - UI 렌더링

---

## 📞 지원

### 문의처

- **이메일**: support@example.com
- **이슈 트래커**: [GitHub Issues](https://github.com/your-org/api-verification/issues)
- **슬랙**: #api-verification

### 기여하기

이 문서를 개선하고 싶다면:
1. 오타나 잘못된 내용 발견 → 이슈 등록
2. 더 좋은 예제 제안 → Pull Request
3. 새로운 섹션 추가 제안 → Discussion

---

## 📜 라이선스

Copyright © 2026 MIDAS IT Co., Ltd. All rights reserved.

---

**문서 버전**: 1.0  
**최종 업데이트**: 2026-01-14  
**다음 업데이트 예정**: 2026-02-14

