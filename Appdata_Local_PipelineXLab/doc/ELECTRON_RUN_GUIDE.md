# PipelineXLab Launcher 실행 가이드

이 문서는 PipelineXLab Launcher를 **Electron 데스크톱 앱** 모드와 **웹 브라우저** 모드로 실행하는 방법을 설명합니다.

## 1. 개발 모드 실행 (Development)

개발 중에는 코드가 변경되면 자동으로 새로고침되는 모드를 사용합니다.

### 🖥️ Electron 데스크톱 앱 모드 (권장)
실제 배포 환경과 가장 유사하며, 로컬 파일 시스템 접근 등 모든 기능을 사용할 수 있습니다.

```bash
npm run electron:dev
```

- **동작 방식**:
  1. TypeScript 컴파일이 실행됩니다.
  2. Vite 개발 서버가 `http://localhost:5180` 포트에서 시작됩니다.
  3. Electron 창이 뜨고 Vite 서버 화면을 로드합니다.
- **주요 기능**:
  - `LaunchTab`에서 실제 백엔드/DB 서비스 관리 및 로그 확인 가능.
  - 파일 시스템 접근 가능.

### 🌐 웹 브라우저 모드
Electron 의존성 없이 빠르게 UI 퍼블리싱/레이아웃 작업을 할 때 유용합니다.

```bash
npm run dev
```

- **동작 방식**:
  - Vite 개발 서버만 실행됩니다.
  - 브라우저(Chrome/Edge)에서 `http://localhost:5180`으로 접속하여 확인합니다.
- **제한 사항**:
  - `LaunchTab`의 서비스 실행/중지 기능이 **시뮬레이션 모드**로 작동합니다.
  - 실제 파일 시스템/OS API 접근 불가.

---

## 2. 배포용 빌드 및 설치 (Build & Install)

사용자에게 배포하기 위한 설치 파일(.exe)을 생성합니다.

```bash
npm run electron:build
```

- **결과물 위치**: `release/` 폴더
  - `PipelineXLab Launcher Setup x.x.x.exe`: 설치 파일 (배포용)
  - `win-unpacked/`: 설치 없이 바로 실행 가능한 포터블 폴더

---

## 3. 문제 해결 (Troubleshooting) & FAQ

### Q1. "Port 5180 is already in use" 에러가 발생해요.
이전에 실행한 서버가 제대로 종료되지 않아서 발생합니다.

**해결 방법**:
1. 다음 명령어로 포트를 점유 중인 프로세스를 강제 종료합니다.
   ```bash
   # 포트 점유 프로세스 확인 및 종료 (자동)
   npx kill-port 5180
   ```
   또는 작업 관리자에서 `Node.js JavaScript Runtime` 또는 `Electron` 프로세스를 모두 종료하세요.

### Q2. Electron 창이 하얗게 나오고 아무것도 안 떠요.
Vite 서버가 아직 준비되지 않았는데 Electron이 먼저 떴을 수 있습니다.

**해결 방법**:
- Electron 창에서 `Ctrl + R`을 눌러 새로고침하세요.
- 그래도 안 되면 터미널을 끄고 `npm run electron:dev`를 다시 실행하세요.

### Q3. 코드를 수정하면 Electron을 껐다 켜야 하나요?
수정한 코드의 위치에 따라 다릅니다.

| 수정 위치 | 설명 | 재시작 필요 여부 |
|-----------|------|------------------|
| **`src/**` (Frontend)** | React 컴포넌트, CSS 등 UI 코드 | **❌ 불필요** (자동 반영됨 - HMR) |
| **`electron/**` (Backend)** | `main.ts`, `preload.ts` 등 Electron 프로세스 코드 | **✅ 필요** (터미널에서 껐다 다시 실행) |

> **꿀팁**: UI만 수정할 때는 `src` 폴더만 건드리면 되므로 Electron을 계속 켜두고 작업하면 편합니다. `electron/main.ts` 같은 파일을 수정했을 때만 터미널에서 `Ctrl + C`로 끄고 다시 실행하세요.
