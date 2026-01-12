# PipelineX Go-to-Market Strategy (시장 진입 및 확장 전략)

본 문서는 PipelineXLab Launcher의 기술적 특성(Component Builder, Local Runtime, Marketplace Protocol)을 기반으로 한 실질적인 시장 진입 로드맵입니다.

---

## 3-2. 목표 시장 진입 전략 (Go-to-Market Strategy)

**"복잡한 GUI 개발 없이 Python 스크립트를 즉시 앱으로 배포(Script-to-App)하는 압도적 경험으로 시장을 장악한다."**

### 1단계: 거점 시장 확보 (Beachhead Strategy)
**Target: "중소 구조설계사무소 (SMB Engineering Firms)"**
*파이썬/엑셀 매크로 같은 로직은 보유했으나, 이를 배포하고 UI를 입힐 개발 역량이 없는 조직.*

*   **진입 전술 (The 'No-Code Wrapper' Strategy):**
    *   **Pain Point:** 엔지니어들이 만든 훌륭한 계산 로직(`main.py`)들이 UI가 없어 개인 PC에 방치됨.
    *   **Killer Feature - 'Playground Builder':**
        *   **"단 3줄의 코드만으로 앱을 완성한다."**
        *   복잡한 프론트엔드(React/Electron) 지식 없이, `PlaygroundTab`에서 Input/Output만 정의하면 즉시 배포 가능한 UI가 생성되는 **'Zero-Code Wrapper'** 경험 제공.
    *   **Seeding Content:**
        *   텅 빈 플랫폼 방지를 위해 **KDS(구조설계기준) 기반 표준 계산 모듈 100종**을 `Utility` 및 `Structural` 카테고리에 선탑재(Pre-load).
        *   설치 즉시 실무에 투입 가능한 '필수 유틸리티'로 포지셔닝.

### 2단계: 시장 확장 (Expansion Strategy)
**Target: "대형 EPC 및 공공 발주처 (Enterprise & Public)"**
*보안이 중요하며, 사내 표준 프로그램 배포 및 버전 관리에 어려움을 겪는 대형 조직.*

*   **확장 전술 (The 'Standardized Runtime' Strategy):**
    *   **Value Proposition:** `LaunchTab`의 "Service Manager" 기능을 엔터프라이즈 레벨로 확장.
        *   개별 PC마다 제각각인 Python 환경/의존성 문제를 **'PipelineX Launcher'** 하나로 통일.
        *   "PipelineX만 깔려있으면, 본사가 배포한 모든 설계 도구가 즉시 실행된다."
    *   **Enterprise Features:**
        *   **Private Repository:** 사내망 전용 컴포넌트 저장소 구축.
        *   **Audit Log:** `LaunchTab`의 로그 시스템을 중앙 서버로 전송하여, 누가 어떤 설계 도구를 언제 사용했는지 감사(Audit) 가능.

### 3단계: 생태계 구축 (Ecosystem Strategy)
**Target: "글로벌 엔지니어링 마켓플레이스 (Global Engineering Market)"**
*전 세계 500만 명의 엔지니어링 도메인 전문가.*

*   **생태계 전술 (The 'Manifest Economy' Strategy):**
    *   **Technical Enabler:** `component.json` 기반의 **개방형 프로토콜(Open Manifest Schema)** 활용.
        *   누구나 표준 Schema(`https://pipelinexlab.com/schemas/component.json`)만 따르면 자신의 도구를 마켓에 등록 가능.
    *   **Flywheel Effect:**
        *   **Supply:** "당신의 `main.py`가 수익이 됩니다." 엔지니어의 잉여 코드를 자산화(Monetization)하는 **Creator Economy** 활성화.
        *   **Demand:** 검증된 수천 개의 엔지니어링 모듈을 '블록'처럼 조립하여 사용하는 거대한 **Smart Engineering Ecosystem** 완성.
