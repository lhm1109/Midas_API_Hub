# Electron 데스크톱 앱 전환 및 인스톨러 구현 가이드

## 개요

현재 Vite + React 웹 앱을 Electron 데스크톱 앱으로 전환하고, Windows 인스톨러(.exe)를 생성하며, AppData 경로에 컴포넌트를 저장하는 방법을 설명합니다.

---

## 1단계: Electron 패키지 설치

```bash
npm install electron electron-builder --save-dev
npm install electron-is-dev --save
```

---

## 2단계: Electron 메인 프로세스 파일 생성

### `electron/main.ts`

```typescript
import { app, BrowserWindow, ipcMain } from 'electron';
import path from 'path';
import fs from 'fs';
import isDev from 'electron-is-dev';

let mainWindow: BrowserWindow | null = null;

// AppData 경로 설정
const APP_DATA_PATH = path.join(app.getPath('userData')); 
// → C:\Users\{사용자}\AppData\Local\PipelineXLab

// 컴포넌트 저장 디렉토리
const COMPONENTS_PATH = path.join(APP_DATA_PATH, 'components');

function createWindow() {
  mainWindow = new BrowserWindow({
    width: 1400,
    height: 900,
    minWidth: 1024,
    minHeight: 700,
    frame: false, // 커스텀 타이틀바 사용 시
    webPreferences: {
      nodeIntegration: false,
      contextIsolation: true,
      preload: path.join(__dirname, 'preload.js'),
    },
  });

  // 개발 모드: Vite 서버 연결
  // 프로덕션: 빌드된 파일 로드
  const url = isDev
    ? 'http://localhost:5173'
    : `file://${path.join(__dirname, '../dist/index.html')}`;

  mainWindow.loadURL(url);

  if (isDev) {
    mainWindow.webContents.openDevTools();
  }

  mainWindow.on('closed', () => {
    mainWindow = null;
  });
}

// 앱 시작 시 필요한 디렉토리 생성
function initializeAppData() {
  if (!fs.existsSync(COMPONENTS_PATH)) {
    fs.mkdirSync(COMPONENTS_PATH, { recursive: true });
  }
}

app.whenReady().then(() => {
  initializeAppData();
  createWindow();
});

app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') {
    app.quit();
  }
});

app.on('activate', () => {
  if (mainWindow === null) {
    createWindow();
  }
});

// IPC 핸들러: 파일 시스템 접근
ipcMain.handle('get-app-data-path', () => APP_DATA_PATH);
ipcMain.handle('get-components-path', () => COMPONENTS_PATH);

ipcMain.handle('read-file', async (_, filePath: string) => {
  return fs.readFileSync(filePath, 'utf-8');
});

ipcMain.handle('write-file', async (_, filePath: string, content: string) => {
  fs.writeFileSync(filePath, content, 'utf-8');
  return true;
});

ipcMain.handle('list-directory', async (_, dirPath: string) => {
  return fs.readdirSync(dirPath);
});

ipcMain.handle('file-exists', async (_, filePath: string) => {
  return fs.existsSync(filePath);
});
```

---

## 3단계: Preload 스크립트 (보안 브릿지)

### `electron/preload.ts`

```typescript
import { contextBridge, ipcRenderer } from 'electron';

contextBridge.exposeInMainWorld('electronAPI', {
  // 경로
  getAppDataPath: () => ipcRenderer.invoke('get-app-data-path'),
  getComponentsPath: () => ipcRenderer.invoke('get-components-path'),
  
  // 파일 시스템
  readFile: (filePath: string) => ipcRenderer.invoke('read-file', filePath),
  writeFile: (filePath: string, content: string) => ipcRenderer.invoke('write-file', filePath, content),
  listDirectory: (dirPath: string) => ipcRenderer.invoke('list-directory', dirPath),
  fileExists: (filePath: string) => ipcRenderer.invoke('file-exists', filePath),
  
  // 윈도우 컨트롤
  minimize: () => ipcRenderer.send('window-minimize'),
  maximize: () => ipcRenderer.send('window-maximize'),
  close: () => ipcRenderer.send('window-close'),
});
```

---

## 4단계: TypeScript 타입 정의

### `src/types/electron.d.ts`

```typescript
export interface ElectronAPI {
  getAppDataPath: () => Promise<string>;
  getComponentsPath: () => Promise<string>;
  readFile: (filePath: string) => Promise<string>;
  writeFile: (filePath: string, content: string) => Promise<boolean>;
  listDirectory: (dirPath: string) => Promise<string[]>;
  fileExists: (filePath: string) => Promise<boolean>;
  minimize: () => void;
  maximize: () => void;
  close: () => void;
}

declare global {
  interface Window {
    electronAPI: ElectronAPI;
  }
}
```

---

## 5단계: package.json 수정

```json
{
  "name": "pipelinexlab-launcher",
  "version": "1.0.0",
  "main": "electron/main.js",
  "scripts": {
    "dev": "vite",
    "build": "tsc && vite build",
    "electron:dev": "concurrently \"vite\" \"wait-on http://localhost:5173 && electron .\"",
    "electron:build": "npm run build && electron-builder",
    "electron:pack": "electron-builder --dir"
  },
  "build": {
    "appId": "com.pipelinexlab.launcher",
    "productName": "PipelineXLab Launcher",
    "directories": {
      "output": "release"
    },
    "files": [
      "dist/**/*",
      "electron/**/*"
    ],
    "win": {
      "target": [
        {
          "target": "nsis",
          "arch": ["x64"]
        }
      ],
      "icon": "public/icon.ico"
    },
    "nsis": {
      "oneClick": false,
      "allowToChangeInstallationDirectory": true,
      "createDesktopShortcut": true,
      "createStartMenuShortcut": true,
      "shortcutName": "PipelineXLab"
    }
  }
}
```

---

## 6단계: React에서 파일 시스템 사용

```typescript
// 컴포넌트에서 사용 예시
async function saveComponentData(componentId: string, data: any) {
  const componentsPath = await window.electronAPI.getComponentsPath();
  const filePath = `${componentsPath}/${componentId}.json`;
  await window.electronAPI.writeFile(filePath, JSON.stringify(data, null, 2));
}

async function loadComponentData(componentId: string) {
  const componentsPath = await window.electronAPI.getComponentsPath();
  const filePath = `${componentsPath}/${componentId}.json`;
  
  if (await window.electronAPI.fileExists(filePath)) {
    const content = await window.electronAPI.readFile(filePath);
    return JSON.parse(content);
  }
  return null;
}
```

---

## 7단계: 인스톨러 빌드

```bash
# 개발 모드 실행
npm run electron:dev

# 프로덕션 빌드 + 인스톨러 생성
npm run electron:build
```

빌드 후 `release/` 폴더에 생성되는 파일:
- `PipelineXLab Launcher Setup 1.0.0.exe` (NSIS 인스톨러)
- `win-unpacked/` (포터블 버전)

---

## 권한 관련 참고사항

| 경로 | 권한 필요 여부 |
|------|---------------|
| `AppData/Local` | ❌ 불필요 (사용자 권한으로 충분) |
| `Program Files` | ⚠️ 관리자 권한 필요 |
| 레지스트리 접근 | ⚠️ 관리자 권한 필요 |

`AppData/Local/PipelineXLab` 경로는 별도의 권한 요청 없이 읽기/쓰기가 가능합니다.

---

## 추가 패키지 설치

```bash
npm install concurrently wait-on --save-dev
```

---

## 디렉토리 구조 (최종)

```
PipelineXLab_Launcher/
├── electron/
│   ├── main.ts
│   └── preload.ts
├── src/
│   ├── components/
│   ├── types/
│   │   └── electron.d.ts
│   ├── App.tsx
│   └── main.tsx
├── public/
│   └── icon.ico
├── package.json
├── vite.config.ts
└── tsconfig.json
```
