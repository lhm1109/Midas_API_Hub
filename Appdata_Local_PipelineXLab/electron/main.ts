import { app, BrowserWindow, ipcMain, shell } from 'electron';
import path from 'path';
import fs from 'fs/promises';
import isDev from 'electron-is-dev';
import { ServiceManager } from './services/ServiceManager';

let mainWindow: BrowserWindow | null = null;
let serviceManager: ServiceManager | null = null;

// 프로젝트 루트 경로 (런처의 상위 디렉토리)
// 프로젝트 루트 경로 (런처의 상위 디렉토리)
const PROJECT_ROOT = isDev
  ? path.join(__dirname, '..', '..') // Dev: dist-electron -> Launcher -> PipelineXLab
  : path.join(process.resourcesPath, '..'); // Prod: resources -> app (sibling to resources?) or inside resources. 
// Actually, extraResources puts flow-editor in resources/flow-editor. 
// So PROJECT_ROOT should point to where flow-editor is relative to.

// Better approach: Point explicit COMPONENT_ROOT
const COMPONENT_ROOT = isDev
  ? path.join(__dirname, '..', '..') // D:/.../PipelineXLab (contains flow-editor)
  : process.resourcesPath; // resources folder (contains flow-editor)

console.log('📂 Environment:', isDev ? 'Development' : 'Production');
console.log('📂 Component Root:', COMPONENT_ROOT);

function createWindow() {
  mainWindow = new BrowserWindow({
    width: 1400,
    height: 900,
    minWidth: 1024,
    minHeight: 700,
    webPreferences: {
      nodeIntegration: false,
      contextIsolation: true,
      preload: path.join(__dirname, 'preload.js'),
      webSecurity: !isDev, // 개발 모드에서는 iframe 임베딩 허용
      webviewTag: true, // webview 태그 활성화
    },
    icon: path.join(__dirname, '../public/assets/logo.png'),
  });

  const url = isDev
    ? 'http://localhost:5180'  // 런처 전용 포트
    : `file://${path.join(__dirname, '../dist/index.html')}`;

  mainWindow.loadURL(url);

  if (isDev) {
    mainWindow.webContents.openDevTools();
  }

  mainWindow.on('closed', () => {
    mainWindow = null;
  });

  // 외부 링크는 기본 브라우저로 열기
  mainWindow.webContents.setWindowOpenHandler(({ url }) => {
    shell.openExternal(url);
    return { action: 'deny' };
  });

  // 🔍 디버그: 로드 실패 이벤트
  mainWindow.webContents.on('did-fail-load', (_e, errorCode, errorDescription, validatedURL) => {
    console.error('❌ did-fail-load:', { errorCode, errorDescription, validatedURL });
  });

  mainWindow.webContents.on('render-process-gone', (_e, details) => {
    console.error('❌ render-process-gone:', details);
  });

  // iframe/webview 콘솔 메시지 캡처
  mainWindow.webContents.on('console-message', (_e, level, message, line, sourceId) => {
    if (level >= 2) { // warning 이상만
      console.log(`[Renderer] ${message}`);
    }
  });
}

app.whenReady().then(() => {
  // Service Manager 초기화
  serviceManager = new ServiceManager(COMPONENT_ROOT);

  createWindow();

  if (mainWindow && serviceManager) {
    serviceManager.setMainWindow(mainWindow);
  }
});

app.on('window-all-closed', async () => {
  // 앱 종료 시 모든 서비스 중지 
  try {
    if (serviceManager) {
      console.log('🛑 Stopping all services before quit...');
      await serviceManager.stopAll();
      console.log('✅ All services stopped');
    }
  } catch (error) {
    console.error('Failed to stop services:', error);
  }

  if (process.platform !== 'darwin') {
    app.quit();
  }
});

// 앱 종료 전 cleanup (강제 종료 포함)
app.on('before-quit', async (event) => {
  if (serviceManager) {
    event.preventDefault();
    try {
      console.log('🛑 Force stopping all services...');
      await serviceManager.stopAll();
    } catch (error) {
      console.error('Force stop failed:', error);
    }
    serviceManager = null;
    app.exit(0);
  }
});

app.on('activate', () => {
  if (mainWindow === null) {
    createWindow();
  }
});

// ==================== IPC Handlers ====================

/**
 * 모든 서비스 시작
 */
ipcMain.handle('launch-all', async () => {
  if (serviceManager) {
    await serviceManager.startAll();
    return { success: true };
  }
  return { success: false, error: 'Service manager not initialized' };
});

/**
 * 모든 서비스 중지 (데이터 보존)
 */
ipcMain.handle('stop-all', async (_, options?: any) => {
  if (serviceManager) {
    await serviceManager.stopAll(options);
    return { success: true };
  }
  return { success: false };
});

/**
 * 완전 초기화 (데이터 삭제)
 */
ipcMain.handle('reset-all', async () => {
  if (serviceManager) {
    await serviceManager.resetAll();
    return { success: true };
  }
  return { success: false };
});

/**
 * 데이터베이스만 초기화
 */
ipcMain.handle('reset-database', async () => {
  if (serviceManager) {
    await serviceManager.resetDatabase();
    return { success: true };
  }
  return { success: false };
});

/**
 * 특정 서비스 상태 조회
 */
ipcMain.handle('get-service-status', (_, serviceId: string) => {
  if (serviceManager) {
    return serviceManager.getServiceStatus(serviceId);
  }
  return null;
});

/**
 * 특정 서비스 시작
 */
ipcMain.handle('start-service', async (_, serviceId: string) => {
  if (serviceManager) {
    const success = await serviceManager.startService(serviceId);
    return { success };
  }
  return { success: false, error: 'Service manager not initialized' };
});

/**
 * 특정 서비스 중지
 */
ipcMain.handle('stop-service', async (_, serviceId: string) => {
  if (serviceManager) {
    await serviceManager.stopService(serviceId);
    return { success: true };
  }
  return { success: false };
});

/**
 * 특정 서비스 재시작
 * ✅ 포트 해제를 대기하여 중복 실행 방지
 */
ipcMain.handle('restart-service', async (_, serviceId: string) => {
  if (serviceManager) {
    const success = await serviceManager.restartService(serviceId);
    return { success };
  }
  return { success: false, error: 'Service manager not initialized' };
});

/**
 * 모든 서비스 상태 조회
 */
ipcMain.handle('get-all-services', () => {
  if (serviceManager) {
    return serviceManager.getAllServices();
  }
  return [];
});

/**
 * 외부 URL 열기
 */
ipcMain.handle('open-external', (_, url: string) => {
  shell.openExternal(url);
  return { success: true };
});

// Component Management Path
const APPDATA_COMPONENTS_ROOT = isDev
  ? path.join(__dirname, '..', '..', 'Appdata_Local_PipelineXLab', 'components')
  : path.join(process.resourcesPath, 'Appdata_Local_PipelineXLab', 'components');

/**
 * 컴포넌트 목록 가져오기
 */
ipcMain.handle('get-components', async () => {
  try {
    const categories = await fs.readdir(APPDATA_COMPONENTS_ROOT, { withFileTypes: true });
    const components: any[] = [];

    for (const category of categories) {
      // Skip hidden folders and non-directories
      if (!category.isDirectory() || category.name.startsWith('_') || category.name.startsWith('.')) {
        continue;
      }

      const categoryPath = path.join(APPDATA_COMPONENTS_ROOT, category.name);
      const items = await fs.readdir(categoryPath, { withFileTypes: true });

      for (const item of items) {
        if (!item.isDirectory()) continue;

        const configPath = path.join(categoryPath, item.name, 'config.json');
        try {
          const configContent = await fs.readFile(configPath, 'utf-8');
          const config = JSON.parse(configContent);

          // Get folder size (simplified - just check if ui folder exists)
          const componentPath = path.join(categoryPath, item.name);
          let hasUI = false;
          try {
            await fs.access(path.join(componentPath, 'ui'));
            hasUI = true;
          } catch { }

          components.push({
            id: `${category.name}/${item.name}`,
            name: config.name || item.name,
            category: category.name,
            version: config.version || '1.0.0',
            description: config.description || '',
            icon: config.icon || '📦',
            kind: config.kind || 'python',
            runtime: config.runtime || 'node',
            path: componentPath,
            hasUI,
            inputs: config.inputs?.length || 0,
            outputs: config.outputs?.length || 0,
          });
        } catch {
          // Skip components without valid config
        }
      }
    }

    return { success: true, components };
  } catch (error: any) {
    console.error('Failed to scan components:', error);
    return { success: false, error: error.message, components: [] };
  }
});

/**
 * 컴포넌트 상세 정보 가져오기
 */
ipcMain.handle('get-component-details', async (_, category: string, name: string) => {
  try {
    const configPath = path.join(APPDATA_COMPONENTS_ROOT, category, name, 'config.json');
    const configContent = await fs.readFile(configPath, 'utf-8');
    const config = JSON.parse(configContent);
    return { success: true, config };
  } catch (error: any) {
    return { success: false, error: error.message };
  }
});

/**
 * 컴포넌트 폴더 열기
 */
ipcMain.handle('open-component-folder', (_, folderPath: string) => {
  shell.openPath(folderPath);
  return { success: true };
});

// ============================================
// Docker Management
// ============================================

import { exec, spawn } from 'child_process';
import { promisify } from 'util';
const execAsync = promisify(exec);

// Settings file path
const SETTINGS_PATH = isDev
  ? path.join(__dirname, '..', '..', 'launcher-settings.json')
  : path.join(app.getPath('userData'), 'launcher-settings.json');

// Default settings
const DEFAULT_SETTINGS = {
  dockerPath: 'C:\\Program Files\\Docker\\Docker\\Docker Desktop.exe',
  autoStartDocker: true,
  theme: 'dark',
  componentsPath: '',
};

// Load settings
async function loadSettings() {
  try {
    const content = await fs.readFile(SETTINGS_PATH, 'utf-8');
    return { ...DEFAULT_SETTINGS, ...JSON.parse(content) };
  } catch {
    return DEFAULT_SETTINGS;
  }
}

// Save settings
async function saveSettings(settings: any) {
  await fs.writeFile(SETTINGS_PATH, JSON.stringify(settings, null, 2));
}

/**
 * Docker 실행 상태 확인
 */
ipcMain.handle('check-docker', async () => {
  try {
    // Check if Docker daemon is running
    await execAsync('docker info');
    return { success: true, running: true };
  } catch {
    return { success: true, running: false };
  }
});

/**
 * Docker Desktop 실행
 */
ipcMain.handle('start-docker', async () => {
  try {
    const settings = await loadSettings();
    const dockerPath = settings.dockerPath;

    // Check if path exists
    try {
      await fs.access(dockerPath);
    } catch {
      return { success: false, error: 'Docker Desktop not found at specified path' };
    }

    // Start Docker Desktop
    spawn(dockerPath, [], { detached: true, stdio: 'ignore' }).unref();

    // Wait for Docker to be ready (max 60 seconds)
    for (let i = 0; i < 30; i++) {
      await new Promise(resolve => setTimeout(resolve, 2000));
      try {
        await execAsync('docker info');
        return { success: true };
      } catch {
        // Docker not ready yet
      }
    }

    return { success: false, error: 'Docker started but not ready' };
  } catch (error: any) {
    return { success: false, error: error.message };
  }
});

/**
 * 설정 가져오기
 */
ipcMain.handle('get-settings', async () => {
  return await loadSettings();
});

/**
 * 설정 저장
 */
ipcMain.handle('set-setting', async (_, key: string, value: any) => {
  const settings = await loadSettings();
  settings[key] = value;
  await saveSettings(settings);
  return { success: true };
});

/**
 * 경로 선택 다이얼로그
 */
ipcMain.handle('browse-for-path', async (_, title: string) => {
  const { dialog } = await import('electron');
  const result = await dialog.showOpenDialog({
    title,
    properties: ['openFile'],
    filters: [{ name: 'Executable', extensions: ['exe'] }]
  });

  if (result.canceled || result.filePaths.length === 0) {
    return { success: false, canceled: true };
  }

  return { success: true, path: result.filePaths[0] };
});
