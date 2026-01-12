# 컴포넌트 설치 및 배포 시스템 가이드

## 개요

런처 앱에서 컴포넌트(OpenSeesPy, Streamlit 등)가 설치되어 있는지 확인하고, 
없으면 자동으로 다운로드 및 설치하는 시스템을 구현합니다.

---

## 아키텍처

```
┌─────────────────────────────────────────────────────────────────┐
│                    Launcher App (Electron)                       │
├─────────────────────────────────────────────────────────────────┤
│  1. 설치 상태 확인                                               │
│  2. 미설치 시 → GitHub/CDN에서 다운로드                          │
│  3. AppData/Local/PipelineXLab/components/ 에 설치               │
│  4. 실행 시 해당 경로에서 로드                                    │
└─────────────────────────────────────────────────────────────────┘
                              ↓
┌─────────────────────────────────────────────────────────────────┐
│                     컴포넌트 저장소                              │
├─────────────────────────────────────────────────────────────────┤
│  Option A: GitHub Releases (무료, 간단)                          │
│  Option B: CDN (S3, CloudFront) (빠름, 비용 발생)                │
│  Option C: 자체 서버 (완전 제어)                                 │
└─────────────────────────────────────────────────────────────────┘
```

---

## 1. 컴포넌트 구조 정의

### 1.1 컴포넌트 매니페스트 (component.json)

각 컴포넌트는 다음 구조로 패키징됩니다:

```json
{
  "id": "openseespy",
  "name": "OpenSeesPy",
  "version": "3.5.0",
  "description": "Python wrapper for OpenSees structural analysis",
  "category": "structural-analysis",
  
  "requirements": {
    "python": ">=3.9",
    "disk_space_mb": 245,
    "dependencies": ["numpy", "matplotlib"]
  },
  
  "files": {
    "main": "openseespy/__init__.py",
    "config": "config.json"
  },
  
  "install": {
    "type": "pip",
    "command": "pip install openseespy"
  },
  
  "checkInstalled": {
    "type": "python-import",
    "module": "openseespy.opensees"
  },
  
  "repository": {
    "type": "github",
    "url": "https://github.com/pipelinexlab/components",
    "asset": "openseespy-3.5.0.zip"
  }
}
```

### 1.2 컴포넌트 레지스트리 (components-registry.json)

서버에서 제공하는 전체 컴포넌트 목록:

```json
{
  "version": "1.0.0",
  "updated_at": "2025-12-30T00:00:00Z",
  "components": [
    {
      "id": "openseespy",
      "name": "OpenSeesPy",
      "latest_version": "3.5.0",
      "download_url": "https://github.com/pipelinexlab/components/releases/download/v3.5.0/openseespy.zip",
      "size_mb": 245,
      "checksum": "sha256:abc123..."
    },
    {
      "id": "streamlit-dashboard",
      "name": "Streamlit Dashboard",
      "latest_version": "1.30.0",
      "download_url": "https://github.com/pipelinexlab/components/releases/download/v1.30.0/streamlit.zip",
      "size_mb": 125,
      "checksum": "sha256:def456..."
    }
  ]
}
```

---

## 2. 설치 상태 확인

### 2.1 Electron 메인 프로세스

```typescript
// electron/component-manager.ts
import { app } from 'electron';
import path from 'path';
import fs from 'fs';
import { exec } from 'child_process';
import { promisify } from 'util';

const execAsync = promisify(exec);

const COMPONENTS_PATH = path.join(app.getPath('userData'), 'components');
const INSTALLED_DB_PATH = path.join(COMPONENTS_PATH, 'installed.json');

interface InstalledComponent {
  id: string;
  version: string;
  installedAt: string;
  path: string;
}

interface InstalledDB {
  components: InstalledComponent[];
}

class ComponentManager {
  private installedDB: InstalledDB = { components: [] };

  constructor() {
    this.ensureDirectories();
    this.loadInstalledDB();
  }

  private ensureDirectories() {
    if (!fs.existsSync(COMPONENTS_PATH)) {
      fs.mkdirSync(COMPONENTS_PATH, { recursive: true });
    }
  }

  private loadInstalledDB() {
    if (fs.existsSync(INSTALLED_DB_PATH)) {
      this.installedDB = JSON.parse(fs.readFileSync(INSTALLED_DB_PATH, 'utf-8'));
    }
  }

  private saveInstalledDB() {
    fs.writeFileSync(INSTALLED_DB_PATH, JSON.stringify(this.installedDB, null, 2));
  }

  // 설치 여부 확인
  isInstalled(componentId: string): boolean {
    return this.installedDB.components.some(c => c.id === componentId);
  }

  // 설치된 버전 확인
  getInstalledVersion(componentId: string): string | null {
    const component = this.installedDB.components.find(c => c.id === componentId);
    return component?.version || null;
  }

  // Python 모듈 설치 확인
  async checkPythonModule(moduleName: string): Promise<boolean> {
    try {
      await execAsync(`python -c "import ${moduleName}"`);
      return true;
    } catch {
      return false;
    }
  }

  // 시스템 명령어 존재 확인
  async checkCommand(command: string): Promise<boolean> {
    try {
      await execAsync(`where ${command}`); // Windows
      return true;
    } catch {
      return false;
    }
  }
}

export const componentManager = new ComponentManager();
```

### 2.2 상세 설치 확인 로직

```typescript
// electron/installation-checker.ts

interface CheckResult {
  isInstalled: boolean;
  version?: string;
  path?: string;
  error?: string;
}

async function checkComponentInstallation(componentId: string): Promise<CheckResult> {
  switch (componentId) {
    case 'python':
      return checkPython();
    case 'openseespy':
      return checkPythonPackage('openseespy');
    case 'docker':
      return checkDocker();
    case 'streamlit':
      return checkPythonPackage('streamlit');
    default:
      return { isInstalled: false, error: 'Unknown component' };
  }
}

async function checkPython(): Promise<CheckResult> {
  try {
    const { stdout } = await execAsync('python --version');
    const version = stdout.trim().replace('Python ', '');
    return { isInstalled: true, version };
  } catch {
    return { isInstalled: false };
  }
}

async function checkPythonPackage(packageName: string): Promise<CheckResult> {
  try {
    const { stdout } = await execAsync(`pip show ${packageName}`);
    const versionMatch = stdout.match(/Version: (.+)/);
    return {
      isInstalled: true,
      version: versionMatch ? versionMatch[1] : 'unknown',
    };
  } catch {
    return { isInstalled: false };
  }
}

async function checkDocker(): Promise<CheckResult> {
  try {
    const { stdout } = await execAsync('docker --version');
    const version = stdout.match(/Docker version (.+),/)?.[1];
    return { isInstalled: true, version };
  } catch {
    return { isInstalled: false };
  }
}
```

---

## 3. 다운로드 및 설치

### 3.1 GitHub Releases에서 다운로드

```typescript
// electron/downloader.ts
import https from 'https';
import fs from 'fs';
import path from 'path';
import { app, BrowserWindow } from 'electron';
import AdmZip from 'adm-zip';
import crypto from 'crypto';

const COMPONENTS_PATH = path.join(app.getPath('userData'), 'components');
const TEMP_PATH = path.join(app.getPath('temp'), 'pipelinexlab');

interface DownloadProgress {
  componentId: string;
  downloaded: number;
  total: number;
  percent: number;
}

async function downloadComponent(
  componentId: string,
  downloadUrl: string,
  expectedChecksum: string,
  mainWindow: BrowserWindow
): Promise<string> {
  const tempFile = path.join(TEMP_PATH, `${componentId}.zip`);
  
  // 임시 디렉토리 생성
  if (!fs.existsSync(TEMP_PATH)) {
    fs.mkdirSync(TEMP_PATH, { recursive: true });
  }

  return new Promise((resolve, reject) => {
    const file = fs.createWriteStream(tempFile);
    
    https.get(downloadUrl, (response) => {
      // 리다이렉트 처리 (GitHub Releases)
      if (response.statusCode === 302) {
        https.get(response.headers.location!, (finalResponse) => {
          handleDownload(finalResponse, file, componentId, mainWindow, resolve, reject, tempFile);
        });
      } else {
        handleDownload(response, file, componentId, mainWindow, resolve, reject, tempFile);
      }
    }).on('error', reject);
  });
}

function handleDownload(response, file, componentId, mainWindow, resolve, reject, tempFile) {
  const total = parseInt(response.headers['content-length'] || '0', 10);
  let downloaded = 0;

  response.on('data', (chunk) => {
    downloaded += chunk.length;
    file.write(chunk);
    
    // 진행률 UI에 전송
    mainWindow.webContents.send('download-progress', {
      componentId,
      downloaded,
      total,
      percent: total ? Math.round((downloaded / total) * 100) : 0,
    } as DownloadProgress);
  });

  response.on('end', () => {
    file.end();
    resolve(tempFile);
  });

  response.on('error', reject);
}

async function verifyChecksum(filePath: string, expected: string): Promise<boolean> {
  return new Promise((resolve) => {
    const hash = crypto.createHash('sha256');
    const stream = fs.createReadStream(filePath);
    
    stream.on('data', (data) => hash.update(data));
    stream.on('end', () => {
      const actual = hash.digest('hex');
      resolve(actual === expected.replace('sha256:', ''));
    });
  });
}

async function extractAndInstall(
  zipPath: string,
  componentId: string
): Promise<string> {
  const extractPath = path.join(COMPONENTS_PATH, componentId);
  
  // 기존 설치 삭제
  if (fs.existsSync(extractPath)) {
    fs.rmSync(extractPath, { recursive: true });
  }
  
  // 압축 해제
  const zip = new AdmZip(zipPath);
  zip.extractAllTo(extractPath, true);
  
  // 임시 파일 삭제
  fs.unlinkSync(zipPath);
  
  return extractPath;
}
```

### 3.2 pip 패키지 설치

```typescript
// electron/pip-installer.ts
import { exec } from 'child_process';
import { promisify } from 'util';
import { BrowserWindow } from 'electron';

const execAsync = promisify(exec);

async function installPipPackage(
  packageName: string,
  version?: string,
  mainWindow?: BrowserWindow
): Promise<void> {
  const packageSpec = version ? `${packageName}==${version}` : packageName;
  
  // 진행 상태 전송
  mainWindow?.webContents.send('install-status', {
    component: packageName,
    status: 'installing',
    message: `Installing ${packageSpec}...`,
  });

  try {
    await execAsync(`pip install ${packageSpec}`);
    
    mainWindow?.webContents.send('install-status', {
      component: packageName,
      status: 'complete',
      message: `Successfully installed ${packageSpec}`,
    });
  } catch (error) {
    mainWindow?.webContents.send('install-status', {
      component: packageName,
      status: 'error',
      message: error.message,
    });
    throw error;
  }
}

// 여러 패키지 동시 설치
async function installPipPackages(packages: string[]): Promise<void> {
  const packageList = packages.join(' ');
  await execAsync(`pip install ${packageList}`);
}
```

---

## 4. IPC 핸들러

### 4.1 메인 프로세스 핸들러

```typescript
// electron/main.ts에 추가
import { ipcMain } from 'electron';
import { componentManager } from './component-manager';
import { downloadComponent, extractAndInstall } from './downloader';
import { installPipPackage } from './pip-installer';

// 컴포넌트 설치 상태 확인
ipcMain.handle('component:check-installed', async (_, componentId: string) => {
  return componentManager.isInstalled(componentId);
});

// 모든 컴포넌트 상태 조회
ipcMain.handle('component:get-all-status', async () => {
  const registry = await fetchComponentRegistry();
  
  return registry.components.map(comp => ({
    ...comp,
    isInstalled: componentManager.isInstalled(comp.id),
    installedVersion: componentManager.getInstalledVersion(comp.id),
    hasUpdate: componentManager.getInstalledVersion(comp.id) !== comp.latest_version,
  }));
});

// 컴포넌트 설치
ipcMain.handle('component:install', async (event, componentId: string) => {
  const registry = await fetchComponentRegistry();
  const component = registry.components.find(c => c.id === componentId);
  
  if (!component) {
    throw new Error(`Component not found: ${componentId}`);
  }

  const mainWindow = BrowserWindow.fromWebContents(event.sender);
  
  // 1. 다운로드
  const zipPath = await downloadComponent(
    componentId,
    component.download_url,
    component.checksum,
    mainWindow!
  );
  
  // 2. 압축 해제 및 설치
  const installPath = await extractAndInstall(zipPath, componentId);
  
  // 3. pip 의존성 설치 (필요한 경우)
  if (component.pip_dependencies) {
    for (const pkg of component.pip_dependencies) {
      await installPipPackage(pkg, undefined, mainWindow!);
    }
  }
  
  // 4. 설치 DB 업데이트
  componentManager.markInstalled(componentId, component.latest_version, installPath);
  
  return { success: true, path: installPath };
});

// 컴포넌트 삭제
ipcMain.handle('component:uninstall', async (_, componentId: string) => {
  return componentManager.uninstall(componentId);
});
```

---

## 5. React UI 연동

### 5.1 설치 상태 훅

```typescript
// src/hooks/useComponentStatus.ts
import { useState, useEffect } from 'react';

interface ComponentStatus {
  id: string;
  name: string;
  isInstalled: boolean;
  installedVersion: string | null;
  latestVersion: string;
  hasUpdate: boolean;
}

export function useComponentStatus() {
  const [components, setComponents] = useState<ComponentStatus[]>([]);
  const [loading, setLoading] = useState(true);

  useEffect(() => {
    async function fetchStatus() {
      const status = await window.electronAPI.getComponentStatus();
      setComponents(status);
      setLoading(false);
    }
    fetchStatus();
  }, []);

  const refresh = async () => {
    setLoading(true);
    const status = await window.electronAPI.getComponentStatus();
    setComponents(status);
    setLoading(false);
  };

  return { components, loading, refresh };
}
```

### 5.2 설치 진행률 컴포넌트

```tsx
// src/components/InstallProgress.tsx
import { useState, useEffect } from 'react';
import { Download, Check, AlertCircle } from 'lucide-react';

interface InstallProgressProps {
  componentId: string;
  onComplete: () => void;
}

export function InstallProgress({ componentId, onComplete }: InstallProgressProps) {
  const [status, setStatus] = useState<'downloading' | 'installing' | 'complete' | 'error'>('downloading');
  const [progress, setProgress] = useState(0);
  const [message, setMessage] = useState('Preparing...');

  useEffect(() => {
    // 다운로드 진행률 이벤트 수신
    const unsubscribeDownload = window.electronAPI.onDownloadProgress((data) => {
      if (data.componentId === componentId) {
        setProgress(data.percent);
        setMessage(`Downloading... ${data.percent}%`);
      }
    });

    // 설치 상태 이벤트 수신
    const unsubscribeInstall = window.electronAPI.onInstallStatus((data) => {
      if (data.component === componentId) {
        setStatus(data.status);
        setMessage(data.message);
        if (data.status === 'complete') {
          onComplete();
        }
      }
    });

    // 설치 시작
    window.electronAPI.installComponent(componentId);

    return () => {
      unsubscribeDownload();
      unsubscribeInstall();
    };
  }, [componentId]);

  return (
    <div className="p-4 bg-[#1a1f2e] rounded-lg">
      <div className="flex items-center gap-3 mb-3">
        {status === 'downloading' && <Download className="w-5 h-5 text-blue-400 animate-pulse" />}
        {status === 'installing' && <Download className="w-5 h-5 text-yellow-400 animate-spin" />}
        {status === 'complete' && <Check className="w-5 h-5 text-green-400" />}
        {status === 'error' && <AlertCircle className="w-5 h-5 text-red-400" />}
        <span className="text-sm text-slate-300">{message}</span>
      </div>
      
      {status === 'downloading' && (
        <div className="w-full h-2 bg-slate-700 rounded-full overflow-hidden">
          <div 
            className="h-full bg-blue-500 transition-all duration-300"
            style={{ width: `${progress}%` }}
          />
        </div>
      )}
    </div>
  );
}
```

---

## 6. GitHub Releases 배포 방법

### 6.1 컴포넌트 패키징

```bash
# 컴포넌트 폴더 구조
openseespy/
├── component.json       # 메타데이터
├── config.json          # 기본 설정
├── __init__.py          # Python 진입점
└── ...

# ZIP으로 압축
zip -r openseespy-3.5.0.zip openseespy/
```

### 6.2 GitHub Release 생성

1. GitHub 저장소에서 **Releases** → **Create new release**
2. 태그: `v3.5.0`
3. 제목: `OpenSeesPy v3.5.0`
4. 파일 첨부: `openseespy-3.5.0.zip`
5. **Publish release**

### 6.3 다운로드 URL 형식

```
https://github.com/{owner}/{repo}/releases/download/{tag}/{filename}

예시:
https://github.com/pipelinexlab/components/releases/download/v3.5.0/openseespy-3.5.0.zip
```

---

## 7. 필요 패키지

```bash
# Electron
npm install adm-zip node-fetch

# 타입
npm install -D @types/adm-zip
```

---

## 8. 보안 고려사항

| 항목 | 권장 사항 |
|------|----------|
| 체크섬 검증 | 다운로드 후 SHA256 해시 확인 필수 |
| HTTPS | 모든 다운로드는 HTTPS 사용 |
| 코드 서명 | 실행 파일은 코드 서명 권장 |
| 권한 최소화 | AppData 내에서만 작업 |
