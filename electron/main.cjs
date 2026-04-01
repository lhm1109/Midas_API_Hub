const { app, BrowserWindow, ipcMain, dialog } = require('electron');
const path = require('path');
const fs = require('fs').promises;
const https = require('https');
const os = require('os');
const dotenv = require('dotenv');
// const db = require('./database'); // DB 기능은 나중에 활성화

// Load project-level .env so Zendesk/Supabase settings are available in Electron main process.
const projectEnvPath = path.join(__dirname, '..', '.env');
const dotenvResult = dotenv.config({ path: projectEnvPath });
if (dotenvResult.error) {
  // Fallback to default resolution without overriding already-injected environment variables.
  dotenv.config();
  if (process.env.NODE_ENV === 'development' || !app.isPackaged) {
    console.warn(`⚠️ Failed to load .env from ${projectEnvPath}: ${dotenvResult.error.message}`);
  }
}

function refreshEnvFromFile() {
  dotenv.config({ path: projectEnvPath, override: true });
}

// Terminal management
let pty;
try {
  pty = require('node-pty');
} catch (e) {
  console.warn('node-pty not available, terminal features disabled');
}

const terminals = new Map(); // id -> { pty, logs }
let terminalIdCounter = 0;

let mainWindow;

function createWindow() {
  mainWindow = new BrowserWindow({
    width: 1840,
    height: 1130,
    minWidth: 1200,
    minHeight: 800,
    title: 'Bat Rack',
    webPreferences: {
      nodeIntegration: false,
      contextIsolation: true,
      preload: path.join(__dirname, 'preload.js'),
      zoomFactor: 1.0,
    },
    titleBarStyle: 'default',
    frame: true,
  });

  // 웹 페이지 title이 Electron 창 제목을 덮어쓰지 않도록 방지
  mainWindow.on('page-title-updated', (event) => {
    event.preventDefault();
  });

  // Ctrl+휠 줌 기능 활성화
  mainWindow.webContents.on('zoom-changed', (event, zoomDirection) => {
    const currentZoom = mainWindow.webContents.getZoomFactor();
    if (zoomDirection === 'in') {
      mainWindow.webContents.setZoomFactor(Math.min(currentZoom + 0.1, 3.0));
    } else if (zoomDirection === 'out') {
      mainWindow.webContents.setZoomFactor(Math.max(currentZoom - 0.1, 0.3));
    }
  });

  // Ctrl+0 줌 리셋, Ctrl+Plus/Minus 줌 단축키
  mainWindow.webContents.on('before-input-event', (event, input) => {
    if (input.control && !input.shift && !input.alt) {
      const currentZoom = mainWindow.webContents.getZoomFactor();
      if (input.key === '0') {
        mainWindow.webContents.setZoomFactor(1.0);
      } else if (input.key === '=' || input.key === '+') {
        mainWindow.webContents.setZoomFactor(Math.min(currentZoom + 0.1, 3.0));
      } else if (input.key === '-') {
        mainWindow.webContents.setZoomFactor(Math.max(currentZoom - 0.1, 0.3));
      }
    }
  });

  // Development mode
  if (process.env.NODE_ENV === 'development' || !app.isPackaged) {
    mainWindow.loadURL('http://localhost:5178');
    mainWindow.webContents.openDevTools();
  } else {
    // Production mode
    mainWindow.loadFile(path.join(__dirname, '../dist/index.html'));
  }

  mainWindow.on('closed', () => {
    mainWindow = null;
  });
}

// 🔥 서버에서 사용하는 포트 목록 (충돌 방지를 위해 앱 시작 시 정리)
const SERVER_PORTS = [3001, 5178, 6274, 5173, 9527];

// 🔥 포트를 사용하는 프로세스를 종료하는 함수 (Windows 전용)
async function killProcessOnPort(port) {
  const { exec } = require('child_process');

  return new Promise((resolve) => {
    // Windows: netstat로 PID 찾고 taskkill로 종료
    exec(`netstat -ano | findstr :${port}`, (error, stdout) => {
      if (error || !stdout.trim()) {
        resolve(false);
        return;
      }

      // LISTENING 상태인 PID 추출
      const lines = stdout.trim().split('\n');
      const pids = new Set();

      for (const line of lines) {
        if (line.includes('LISTENING')) {
          const parts = line.trim().split(/\s+/);
          const pid = parts[parts.length - 1];
          if (pid && pid !== '0') {
            pids.add(pid);
          }
        }
      }

      if (pids.size === 0) {
        resolve(false);
        return;
      }

      // 각 PID 종료
      let killed = false;
      for (const pid of pids) {
        exec(`taskkill /PID ${pid} /F`, (killError) => {
          if (!killError) {
            console.log(`  🔫 Killed process ${pid} on port ${port}`);
            killed = true;
          }
        });
      }

      // 약간의 딜레이 후 resolve
      setTimeout(() => resolve(killed), 500);
    });
  });
}

// 🔥 모든 서버 포트 정리
async function clearServerPorts() {
  console.log('🧹 Clearing server ports before startup...');

  for (const port of SERVER_PORTS) {
    const killed = await killProcessOnPort(port);
    if (killed) {
      console.log(`  ✅ Port ${port} cleared`);
    }
  }

  console.log('🧹 Port cleanup complete');
}

app.whenReady().then(async () => {
  // 🔥 앱 시작 전 포트 정리 (개발 모드에서는 건너뛰기 - concurrently가 이미 서버를 시작했으므로)
  const isDev = process.env.NODE_ENV === 'development' || !app.isPackaged;
  if (!isDev) {
    await clearServerPorts();
  } else {
    console.log('🔧 Development mode: skipping port cleanup (servers started by concurrently)');
  }

  // Initialize database
  // db.initDatabase(); // DB 기능은 나중에 활성화

  createWindow();

  app.on('activate', () => {
    if (BrowserWindow.getAllWindows().length === 0) {
      createWindow();
    }
  });
});


app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') {
    app.quit();
  }
});

// IPC Handlers for file system operations

// Select folder
ipcMain.handle('dialog:openDirectory', async () => {
  const result = await dialog.showOpenDialog(mainWindow, {
    properties: ['openDirectory'],
  });

  if (result.canceled) {
    return null;
  }
  return result.filePaths[0];
});

// Select file
ipcMain.handle('dialog:openFile', async (event, filters = []) => {
  const result = await dialog.showOpenDialog(mainWindow, {
    properties: ['openFile', 'multiSelections'],
    filters: filters.length > 0 ? filters : [
      { name: 'All Files', extensions: ['*'] },
    ],
  });

  if (result.canceled) {
    return null;
  }
  return result.filePaths;
});

// Read file
ipcMain.handle('fs:readFile', async (event, filePath) => {
  try {
    const content = await fs.readFile(filePath, 'utf-8');
    return { success: true, content };
  } catch (error) {
    return { success: false, error: error.message };
  }
});

// Write file
ipcMain.handle('fs:writeFile', async (event, filePath, content) => {
  try {
    await fs.writeFile(filePath, content, 'utf-8');
    return { success: true };
  } catch (error) {
    return { success: false, error: error.message };
  }
});

// Read directory
ipcMain.handle('fs:readDirectory', async (event, dirPath) => {
  try {
    const entries = await fs.readdir(dirPath, { withFileTypes: true });
    const items = entries.map(entry => ({
      name: entry.name,
      path: path.join(dirPath, entry.name),
      isDirectory: entry.isDirectory(),
    }));
    return { success: true, items };
  } catch (error) {
    return { success: false, error: error.message };
  }
});

// Check if path exists
ipcMain.handle('fs:exists', async (event, filePath) => {
  try {
    await fs.access(filePath);
    return true;
  } catch {
    return false;
  }
});

// Get file stats
ipcMain.handle('fs:stat', async (event, filePath) => {
  try {
    const stats = await fs.stat(filePath);
    return {
      success: true,
      stats: {
        size: stats.size,
        isDirectory: stats.isDirectory(),
        isFile: stats.isFile(),
        mtime: stats.mtime,
        ctime: stats.ctime,
      },
    };
  } catch (error) {
    return { success: false, error: error.message };
  }
});

// Database IPC Handlers

// .testenv config handlers
ipcMain.handle('env:loadConfig', async () => {
  try {
    // 프로젝트 루트의 .testenv 파일 경로
    const isDev = process.env.NODE_ENV === 'development' || !app.isPackaged;

    // 개발 모드: electron 폴더의 상위 디렉토리 (프로젝트 루트)
    // 프로덕션: userData 폴더
    const envPath = isDev
      ? path.join(__dirname, '..', '.testenv')
      : path.join(app.getPath('userData'), '.testenv');

    console.log('📂 .testenv 경로:', envPath);
    console.log('📂 __dirname:', __dirname);
    console.log('📂 isDev:', isDev);

    let content = '';

    try {
      content = await fs.readFile(envPath, 'utf-8');
      console.log('✅ .testenv 파일 읽기 성공');
      console.log('📝 파일 내용 길이:', content.length);
    } catch (error) {
      console.warn('⚠️ .testenv 파일 읽기 실패:', error.message);
      console.warn('⚠️ 기본값 생성 시도');
      // 파일이 없으면 기본값으로 생성
      content = `# API 검증 시스템 설정
# 나중에 .env로 변경 예정

# API Base URL
VITE_API_BASE_URL=https://moa-engineers.midasit.com:443/civil

# MAPI Key
VITE_MAPI_KEY=

# 자동 연결 (true/false)
VITE_AUTO_CONNECT=false
`;
      try {
        await fs.writeFile(envPath, content, 'utf-8');
        console.log('✅ 기본 .testenv 파일 생성 완료');
      } catch (writeError) {
        console.error('❌ .testenv 파일 생성 실패:', writeError.message);
      }
    }

    const config = {
      baseUrl: '',
      mapiKey: '',
      autoConnect: false,
    };

    const lines = content.split('\n');
    for (const line of lines) {
      const trimmed = line.trim();
      if (!trimmed || trimmed.startsWith('#')) continue;

      const match = trimmed.match(/^([^=]+)=(.*)$/);
      if (match) {
        const key = match[1].trim();
        const value = match[2].trim();

        switch (key) {
          case 'VITE_API_BASE_URL':
            config.baseUrl = value;
            break;
          case 'VITE_MAPI_KEY':
            config.mapiKey = value;
            break;
          case 'VITE_AUTO_CONNECT':
            config.autoConnect = value.toLowerCase() === 'true';
            break;
        }
      }
    }

    console.log('✅ .testenv 파싱 완료:', {
      baseUrl: config.baseUrl ? config.baseUrl.substring(0, 30) + '...' : '(empty)',
      mapiKeyLength: config.mapiKey?.length || 0,
      autoConnect: config.autoConnect,
    });

    return { success: true, data: config };
  } catch (error) {
    console.error('❌ .testenv 로드 에러:', error);
    return { success: false, error: error.message };
  }
});

ipcMain.handle('env:saveConfig', async (event, config) => {
  try {
    const isDev = process.env.NODE_ENV === 'development' || !app.isPackaged;
    const envPath = isDev
      ? path.join(__dirname, '..', '.testenv')
      : path.join(app.getPath('userData'), '.testenv');

    const content = `# API 검증 시스템 설정
# 나중에 .env로 변경 예정

# API Base URL
VITE_API_BASE_URL=${config.baseUrl}

# MAPI Key
VITE_MAPI_KEY=${config.mapiKey}

# 자동 연결 (true/false)
VITE_AUTO_CONNECT=${config.autoConnect}
`;

    await fs.writeFile(envPath, content, 'utf-8');
    console.log('✅ .testenv 저장 완료:', envPath);
    return { success: true };
  } catch (error) {
    console.error('❌ .testenv 저장 에러:', error);
    return { success: false, error: error.message };
  }
});

// Presets
ipcMain.handle('db:createPreset', async (event, preset) => {
  try {
    // db.createPreset(preset);
    return { success: true, message: 'DB 기능은 준비 중입니다' };
  } catch (error) {
    return { success: false, error: error.message };
  }
});

ipcMain.handle('db:getAllPresets', async () => {
  try {
    // const presets = db.getAllPresets();
    return { success: true, data: [] };
  } catch (error) {
    return { success: false, error: error.message };
  }
});

ipcMain.handle('db:getPresetById', async (event, id) => {
  try {
    const preset = db.getPresetById(id);
    return { success: true, data: preset };
  } catch (error) {
    return { success: false, error: error.message };
  }
});

ipcMain.handle('db:updatePreset', async (event, id, preset) => {
  try {
    db.updatePreset(id, preset);
    return { success: true };
  } catch (error) {
    return { success: false, error: error.message };
  }
});

ipcMain.handle('db:deletePreset', async (event, id) => {
  try {
    db.deletePreset(id);
    return { success: true };
  } catch (error) {
    return { success: false, error: error.message };
  }
});

// Test Results
ipcMain.handle('db:saveTestResult', async (event, result) => {
  try {
    db.saveTestResult(result);
    return { success: true };
  } catch (error) {
    return { success: false, error: error.message };
  }
});

ipcMain.handle('db:getTestResultsByApiId', async (event, apiId, limit) => {
  try {
    const results = db.getTestResultsByApiId(apiId, limit);
    return { success: true, data: results };
  } catch (error) {
    return { success: false, error: error.message };
  }
});

ipcMain.handle('db:getRecentTestResults', async (event, limit) => {
  try {
    const results = db.getRecentTestResults(limit);
    return { success: true, data: results };
  } catch (error) {
    return { success: false, error: error.message };
  }
});

ipcMain.handle('db:getTestStatistics', async () => {
  try {
    const stats = db.getTestStatistics();
    return { success: true, data: stats };
  } catch (error) {
    return { success: false, error: error.message };
  }
});

// ============================================
// Zendesk API Handlers
// ============================================

const ZENDESK_DEFAULT_LOCALE = 'en-us';

function asTrimmedString(value) {
  if (typeof value !== 'string') return '';

  const trimmed = value.trim();
  if (
    (trimmed.startsWith('"') && trimmed.endsWith('"')) ||
    (trimmed.startsWith("'") && trimmed.endsWith("'"))
  ) {
    return trimmed.slice(1, -1).trim();
  }

  return trimmed;
}

function normalizeZendeskLocale(locale = ZENDESK_DEFAULT_LOCALE) {
  const normalized = String(locale || '')
    .trim()
    .replace(/_/g, '-')
    .toLowerCase();
  return normalized || ZENDESK_DEFAULT_LOCALE;
}

function toZendeskPathSegment(value, fieldName) {
  const rawValue = asTrimmedString(String(value ?? ''));
  if (!rawValue) {
    throw new Error(`${fieldName} is required`);
  }
  return encodeURIComponent(rawValue);
}

function parseZendeskSubdomainFromBaseUrl(baseUrl) {
  const trimmed = asTrimmedString(baseUrl);
  if (!trimmed) return '';
  try {
    const parsed = new URL(trimmed);
    const host = parsed.hostname.toLowerCase();
    const match = host.match(/^([a-z0-9-]+)\.zendesk\.com$/i);
    return match ? match[1] : '';
  } catch {
    return '';
  }
}

function parseZendeskTarget(targetInput, fallbackLocale = ZENDESK_DEFAULT_LOCALE) {
  const raw = asTrimmedString(targetInput);
  if (!raw) return null;

  if (/^\d+$/.test(raw)) {
    return { articleId: raw, locale: normalizeZendeskLocale(fallbackLocale) };
  }

  try {
    const parsedUrl = new URL(raw);
    const pathSegments = parsedUrl.pathname.split('/').filter(Boolean);
    const articleIndex = pathSegments.indexOf('articles');
    if (articleIndex === -1 || articleIndex + 1 >= pathSegments.length) {
      return null;
    }

    const articleSegment = pathSegments[articleIndex + 1];
    const articleIdMatch = articleSegment.match(/^(\d+)/);
    if (!articleIdMatch) {
      return null;
    }

    let locale =
      parsedUrl.searchParams.get('locale') ||
      parsedUrl.searchParams.get('lang') ||
      '';

    if (!locale && articleIndex >= 2 && pathSegments[0] === 'hc') {
      locale = pathSegments[1];
    }

    const translationIndex = pathSegments.indexOf('translations');
    if (!locale && translationIndex >= 0 && translationIndex + 1 < pathSegments.length) {
      locale = pathSegments[translationIndex + 1].replace(/\.json$/i, '');
    }

    return {
      articleId: articleIdMatch[1],
      locale: normalizeZendeskLocale(locale || fallbackLocale),
    };
  } catch {
    return null;
  }
}

function isEnvAllValue(value) {
  const normalized = asTrimmedString(value).toLowerCase();
  return normalized === 'all' || normalized === '*' || normalized === 'none' || normalized === 'null';
}

function parseOptionalInteger(value) {
  const raw = asTrimmedString(value);
  if (!raw || isEnvAllValue(raw)) return null;
  const parsed = Number.parseInt(raw, 10);
  return Number.isFinite(parsed) && parsed > 0 ? parsed : null;
}

function parseIntegerList(value, maxItems = Number.POSITIVE_INFINITY) {
  const raw = asTrimmedString(value);
  if (!raw || isEnvAllValue(raw)) return [];

  const results = [];
  const seen = new Set();
  for (const token of raw.split(',')) {
    const parsed = parseOptionalInteger(token);
    if (parsed === null) continue;
    if (seen.has(parsed)) continue;
    seen.add(parsed);
    results.push(parsed);
    if (results.length >= maxItems) break;
  }
  return results;
}

function parseStringList(value, maxItems = Number.POSITIVE_INFINITY) {
  const raw = asTrimmedString(value);
  if (!raw || isEnvAllValue(raw)) return [];

  const results = [];
  const seen = new Set();
  for (const token of raw.split(',')) {
    const normalized = token.trim();
    if (!normalized) continue;
    const key = normalized.toLowerCase();
    if (seen.has(key)) continue;
    seen.add(key);
    results.push(normalized);
    if (results.length >= maxItems) break;
  }
  return results;
}

function normalizeZendeskLabelNames(values, fallbackValues = undefined) {
  if (Array.isArray(values)) {
    const normalized = values
      .map((value) => asTrimmedString(value))
      .filter(Boolean);
    return Array.from(new Set(normalized));
  }

  if (typeof values === 'string') {
    return parseStringList(values);
  }

  return fallbackValues;
}

function parseOptionalBoolean(value) {
  const raw = asTrimmedString(value).toLowerCase();
  if (!raw) return null;

  if (['1', 'true', 'yes', 'y', 'on'].includes(raw)) return true;
  if (['0', 'false', 'no', 'n', 'off'].includes(raw)) return false;
  return null;
}

function getZendeskConfigFromEnv() {
  refreshEnvFromFile();

  const baseUrl = asTrimmedString(process.env.ZENDESK_BASE_URL);
  const subdomain = asTrimmedString(process.env.ZENDESK_SUBDOMAIN) || parseZendeskSubdomainFromBaseUrl(baseUrl);
  const email = asTrimmedString(process.env.ZENDESK_EMAIL);
  const password = asTrimmedString(process.env.ZENDESK_PASSWORD);
  const apiToken = asTrimmedString(process.env.ZENDESK_API_TOKEN);
  const defaultLocale = normalizeZendeskLocale(process.env.ZENDESK_DEFAULT_LOCALE || ZENDESK_DEFAULT_LOCALE);
  const defaultArticleUrl = asTrimmedString(process.env.ZENDESK_DEFAULT_ARTICLE_URL);
  const defaultArticleId = asTrimmedString(process.env.ZENDESK_DEFAULT_ARTICLE_ID);
  const defaultSectionId = parseOptionalInteger(process.env.ZENDESK_DEFAULT_SECTION_ID);
  const defaultPermissionGroupId = parseOptionalInteger(process.env.ZENDESK_DEFAULT_PERMISSION_GROUP_ID);

  const userSegmentIdRaw = asTrimmedString(process.env.ZENDESK_DEFAULT_USER_SEGMENT_ID);
  const userSegmentIdsRaw = asTrimmedString(process.env.ZENDESK_DEFAULT_USER_SEGMENT_IDS);
  const defaultUserSegmentId = parseOptionalInteger(userSegmentIdRaw);
  const defaultUserSegmentIds = parseIntegerList(userSegmentIdsRaw, 10);
  const hasDefaultUserSegmentId = userSegmentIdRaw !== '';
  const hasDefaultUserSegmentIds = userSegmentIdsRaw !== '';

  const defaultAuthorId = parseOptionalInteger(process.env.ZENDESK_DEFAULT_AUTHOR_ID);
  const defaultLabels = parseStringList(process.env.ZENDESK_DEFAULT_LABELS);
  const defaultContentTagIds = parseStringList(process.env.ZENDESK_DEFAULT_CONTENT_TAG_IDS);
  const defaultAttachmentIds = parseIntegerList(process.env.ZENDESK_DEFAULT_ATTACHMENT_IDS);

  const defaultPromoted = parseOptionalBoolean(process.env.ZENDESK_DEFAULT_PROMOTED);
  const explicitCommentsDisabled = parseOptionalBoolean(process.env.ZENDESK_DEFAULT_COMMENTS_DISABLED);
  const explicitCommentsEnabled = parseOptionalBoolean(process.env.ZENDESK_DEFAULT_COMMENTS_ENABLED);
  const defaultCommentsDisabled = explicitCommentsDisabled !== null
    ? explicitCommentsDisabled
    : (explicitCommentsEnabled !== null ? !explicitCommentsEnabled : null);
  const defaultNotifySubscribers = parseOptionalBoolean(process.env.ZENDESK_DEFAULT_NOTIFY_SUBSCRIBERS);
  const defaultDraft = parseOptionalBoolean(process.env.ZENDESK_DEFAULT_DRAFT);

  const missingFields = [];
  if (!subdomain) missingFields.push('ZENDESK_SUBDOMAIN or ZENDESK_BASE_URL');
  if (!email) missingFields.push('ZENDESK_EMAIL');
  if (!apiToken && !password) missingFields.push('ZENDESK_API_TOKEN or ZENDESK_PASSWORD');

  return {
    baseUrl,
    subdomain,
    email,
    password,
    apiToken,
    defaultLocale,
    defaultArticleUrl,
    defaultArticleId,
    defaultSectionId,
    defaultPermissionGroupId,
    hasDefaultUserSegmentId,
    hasDefaultUserSegmentIds,
    defaultUserSegmentId,
    defaultUserSegmentIds,
    defaultAuthorId,
    defaultLabels,
    defaultContentTagIds,
    defaultAttachmentIds,
    defaultPromoted,
    defaultCommentsDisabled,
    defaultNotifySubscribers,
    defaultDraft,
    missingFields,
    isReady: missingFields.length === 0,
  };
}

function buildZendeskArticleMetadataFromEnv(envConfig, overrides = {}) {
  const metadata = {};

  if (envConfig.defaultPermissionGroupId !== null) {
    metadata.permission_group_id = envConfig.defaultPermissionGroupId;
  }

  if (envConfig.hasDefaultUserSegmentIds) {
    metadata.user_segment_ids = envConfig.defaultUserSegmentIds;
  } else if (envConfig.hasDefaultUserSegmentId) {
    metadata.user_segment_id = envConfig.defaultUserSegmentId;
  }

  if (envConfig.defaultAuthorId !== null) {
    metadata.author_id = envConfig.defaultAuthorId;
  }

  const overrideLabelNames = normalizeZendeskLabelNames(overrides.labelNames);
  if (overrideLabelNames !== undefined) {
    metadata.label_names = overrideLabelNames;
  } else if (envConfig.defaultLabels.length > 0) {
    metadata.label_names = envConfig.defaultLabels;
  }

  if (envConfig.defaultContentTagIds.length > 0) {
    metadata.content_tag_ids = envConfig.defaultContentTagIds;
  }

  if (envConfig.defaultPromoted !== null) {
    metadata.promoted = envConfig.defaultPromoted;
  }

  if (typeof overrides.commentsDisabled === 'boolean') {
    metadata.comments_disabled = overrides.commentsDisabled;
  } else if (envConfig.defaultCommentsDisabled !== null) {
    metadata.comments_disabled = envConfig.defaultCommentsDisabled;
  }

  return metadata;
}

function buildZendeskArticleUrl(subdomain, locale, articleId) {
  const normalizedLocale = normalizeZendeskLocale(locale);
  return `https://${subdomain}.zendesk.com/hc/${normalizedLocale}/articles/${articleId}`;
}

function getZendeskAuthContext(config) {
  const subdomain = asTrimmedString(config?.subdomain);
  const email = asTrimmedString(config?.email);
  const apiToken = asTrimmedString(config?.apiToken || config?.token);
  const password = asTrimmedString(config?.password);
  const username = apiToken ? `${email}/token` : email;
  const secret = apiToken ? apiToken : password;

  const missingFields = [];
  if (!subdomain) missingFields.push('subdomain');
  if (!email) missingFields.push('email');
  if (!secret) missingFields.push('apiToken/password');

  if (missingFields.length > 0) {
    throw new Error(`Invalid Zendesk config: missing ${missingFields.join(', ')}`);
  }

  const auth = Buffer.from(`${username}:${secret}`).toString('base64');
  return {
    subdomain,
    auth,
    authType: apiToken ? 'token' : 'password',
  };
}

function buildZendeskRequestOptions(config, requestPath, method = 'GET', postData = null) {
  const { subdomain, auth } = getZendeskAuthContext(config);
  const headers = {
    'Authorization': `Basic ${auth}`,
    'Content-Type': 'application/json',
  };

  if (postData !== null && postData !== undefined) {
    headers['Content-Length'] = Buffer.byteLength(postData);
  }

  return {
    hostname: `${subdomain}.zendesk.com`,
    path: requestPath,
    method,
    headers,
    timeout: 30000,
  };
}

/**
 * Helper function to make HTTPS requests
 */
function makeZendeskRequest(options, postData = null) {
  return new Promise((resolve, reject) => {
    const req = https.request(options, (res) => {
      let data = '';

      res.on('data', (chunk) => {
        data += chunk;
      });

      res.on('end', () => {
        if (res.statusCode >= 200 && res.statusCode < 300) {
          try {
            resolve(JSON.parse(data));
          } catch (e) {
            resolve(data);
          }
        } else {
          const normalizedBody = asTrimmedString(data);
          if ((res.statusCode === 401 || res.statusCode === 403) && !normalizedBody) {
            reject(new Error(
              `HTTP ${res.statusCode}: Zendesk 인증/권한 오류. ` +
              `API token 또는 agent 권한(Guide 편집 권한)을 확인하세요.`
            ));
          } else {
            reject(new Error(`HTTP ${res.statusCode}: ${normalizedBody || 'No response body'}`));
          }
        }
      });
    });

    req.setTimeout(options.timeout || 30000, () => {
      req.destroy(new Error('Zendesk request timeout'));
    });

    req.on('error', (error) => {
      reject(error);
    });

    if (postData) {
      req.write(postData);
    }

    req.end();
  });
}

async function updateZendeskArticleTranslation(config, articleId, locale, body, title, draft) {
  const safeArticleId = toZendeskPathSegment(articleId, 'articleId');
  const safeLocale = toZendeskPathSegment(normalizeZendeskLocale(locale), 'locale');

  const payload = {
    translation: {
      body: typeof body === 'string' ? body : String(body ?? ''),
    },
  };

  if (title !== undefined && title !== null && String(title).trim() !== '') {
    payload.translation.title = String(title);
  }

  if (draft !== undefined && draft !== null) {
    payload.translation.draft = !!draft;
  }

  const postData = JSON.stringify(payload);
  const options = buildZendeskRequestOptions(
    config,
    `/api/v2/help_center/articles/${safeArticleId}/translations/${safeLocale}.json`,
    'PUT',
    postData
  );

  const data = await makeZendeskRequest(options, postData);
  return data.translation;
}

async function updateZendeskArticle(config, articleId, articlePayload, notifySubscribers = null) {
  const safeArticleId = toZendeskPathSegment(articleId, 'articleId');
  const payload = {
    article: articlePayload,
  };

  if (notifySubscribers !== null && notifySubscribers !== undefined) {
    payload.notify_subscribers = !!notifySubscribers;
  }

  const postData = JSON.stringify(payload);
  const options = buildZendeskRequestOptions(
    config,
    `/api/v2/help_center/articles/${safeArticleId}.json`,
    'PUT',
    postData
  );

  const data = await makeZendeskRequest(options, postData);
  return data.article;
}

async function createZendeskArticle(config, sectionId, locale, title, body, envConfig, draftOverride, metadataOverrides = {}) {
  const safeSectionId = toZendeskPathSegment(sectionId, 'sectionId');
  const normalizedLocale = normalizeZendeskLocale(locale || envConfig.defaultLocale || ZENDESK_DEFAULT_LOCALE);

  const metadata = buildZendeskArticleMetadataFromEnv(envConfig, metadataOverrides);
  const articleDraft = draftOverride !== null && draftOverride !== undefined
    ? !!draftOverride
    : (envConfig.defaultDraft !== null ? envConfig.defaultDraft : false);

  if (envConfig.defaultPermissionGroupId === null) {
    throw new Error('Article 생성에는 ZENDESK_DEFAULT_PERMISSION_GROUP_ID 설정이 필요합니다.');
  }

  const articlePayload = {
    ...metadata,
    title: asTrimmedString(title) || 'API Manual',
    body: typeof body === 'string' ? body : String(body ?? ''),
    locale: normalizedLocale,
    draft: articleDraft,
  };

  const payload = {
    article: articlePayload,
  };

  if (envConfig.defaultNotifySubscribers !== null) {
    payload.notify_subscribers = envConfig.defaultNotifySubscribers;
  } else {
    payload.notify_subscribers = false;
  }

  const postData = JSON.stringify(payload);
  const options = buildZendeskRequestOptions(
    config,
    `/api/v2/help_center/sections/${safeSectionId}/articles.json`,
    'POST',
    postData
  );

  const data = await makeZendeskRequest(options, postData);
  return data.article;
}

async function associateZendeskAttachments(config, articleId, attachmentIds) {
  if (!Array.isArray(attachmentIds) || attachmentIds.length === 0) {
    return [];
  }

  const safeArticleId = toZendeskPathSegment(articleId, 'articleId');
  const payload = {
    attachment_ids: attachmentIds,
  };
  const postData = JSON.stringify(payload);

  const options = buildZendeskRequestOptions(
    config,
    `/api/v2/help_center/articles/${safeArticleId}/bulk_attachments.json`,
    'POST',
    postData
  );

  const data = await makeZendeskRequest(options, postData);
  return data.article_attachments || [];
}

/**
 * Load Zendesk runtime status from .env (without exposing secrets)
 */
ipcMain.handle('zendesk:getEnvConfig', async () => {
  try {
    const envConfig = getZendeskConfigFromEnv();
    return {
      success: true,
      data: {
        baseUrl: envConfig.baseUrl,
        subdomain: envConfig.subdomain,
        defaultLocale: envConfig.defaultLocale,
        defaultArticleUrl: envConfig.defaultArticleUrl,
        defaultArticleId: envConfig.defaultArticleId,
        defaultSectionId: envConfig.defaultSectionId,
        defaultPermissionGroupId: envConfig.defaultPermissionGroupId,
        defaultUserSegmentId: envConfig.defaultUserSegmentId,
        defaultUserSegmentIds: envConfig.defaultUserSegmentIds,
        defaultLabels: envConfig.defaultLabels,
        defaultContentTagIds: envConfig.defaultContentTagIds,
        defaultPromoted: envConfig.defaultPromoted,
        defaultCommentsDisabled: envConfig.defaultCommentsDisabled,
        defaultNotifySubscribers: envConfig.defaultNotifySubscribers,
        defaultDraft: envConfig.defaultDraft,
        defaultAttachmentIds: envConfig.defaultAttachmentIds,
        hasCredentials: envConfig.isReady,
        authType: envConfig.apiToken ? 'token' : (envConfig.password ? 'password' : null),
        missingFields: envConfig.missingFields,
      },
    };
  } catch (error) {
    console.error('Zendesk getEnvConfig error:', error);
    return { success: false, error: error.message };
  }
});

/**
 * Get all translations for an article
 * GET /api/v2/help_center/articles/{article_id}/translations.json
 */
ipcMain.handle('zendesk:getAllTranslations', async (event, config, articleId) => {
  try {
    const safeArticleId = toZendeskPathSegment(articleId, 'articleId');

    const options = buildZendeskRequestOptions(
      config,
      `/api/v2/help_center/articles/${safeArticleId}/translations.json`
    );

    const data = await makeZendeskRequest(options);
    return { success: true, data: data.translations || [] };
  } catch (error) {
    console.error('Zendesk getAllTranslations error:', error);
    return { success: false, error: error.message };
  }
});

/**
 * Get article translation by locale
 * GET /api/v2/help_center/articles/{article_id}/translations/{locale}.json
 */
ipcMain.handle('zendesk:getArticleTranslation', async (event, config, articleId, locale = 'en-us') => {
  try {
    const safeArticleId = toZendeskPathSegment(articleId, 'articleId');
    const safeLocale = toZendeskPathSegment(normalizeZendeskLocale(locale), 'locale');

    const options = buildZendeskRequestOptions(
      config,
      `/api/v2/help_center/articles/${safeArticleId}/translations/${safeLocale}.json`
    );

    const data = await makeZendeskRequest(options);
    return { success: true, data: data.translation };
  } catch (error) {
    console.error('Zendesk getArticleTranslation error:', error);
    return { success: false, error: error.message };
  }
});

/**
 * Update article translation
 * PUT /api/v2/help_center/articles/{article_id}/translations/{locale}.json
 */
ipcMain.handle('zendesk:updateArticleTranslation', async (event, config, articleId, locale, body, title, draft) => {
  try {
    const translation = await updateZendeskArticleTranslation(
      config,
      articleId,
      locale || ZENDESK_DEFAULT_LOCALE,
      body,
      title,
      draft
    );
    return { success: true, data: translation };
  } catch (error) {
    console.error('Zendesk updateArticleTranslation error:', error);
    return { success: false, error: error.message };
  }
});

/**
 * Update article translation using .env credentials from Electron main process
 * PUT /api/v2/help_center/articles/{article_id}/translations/{locale}.json
 */
ipcMain.handle('zendesk:updateArticleTranslationWithEnv', async (event, articleId, locale, body, title, draft) => {
  try {
    const envConfig = getZendeskConfigFromEnv();
    if (!envConfig.isReady) {
      throw new Error(`Zendesk .env settings missing: ${envConfig.missingFields.join(', ')}`);
    }

    const config = {
      subdomain: envConfig.subdomain,
      email: envConfig.email,
      apiToken: envConfig.apiToken,
      password: envConfig.password,
    };

    const translation = await updateZendeskArticleTranslation(
      config,
      articleId,
      locale || envConfig.defaultLocale,
      body,
      title,
      draft
    );

    return { success: true, data: translation };
  } catch (error) {
    console.error('Zendesk updateArticleTranslationWithEnv error:', error);
    return { success: false, error: error.message };
  }
});

/**
 * Publish Manual with .env defaults.
 * - If target article (URL/ID) is provided: update metadata + translation
 * - If target is omitted: create article in default section, then update translation
 */
ipcMain.handle('zendesk:publishManualWithEnv', async (event, targetInput, locale, body, title, draft, options = {}) => {
  try {
    const envConfig = getZendeskConfigFromEnv();
    if (!envConfig.isReady) {
      throw new Error(`Zendesk .env settings missing: ${envConfig.missingFields.join(', ')}`);
    }

    const config = {
      subdomain: envConfig.subdomain,
      email: envConfig.email,
      apiToken: envConfig.apiToken,
      password: envConfig.password,
    };

    const fallbackLocale = normalizeZendeskLocale(locale || envConfig.defaultLocale);
    const directTarget = parseZendeskTarget(targetInput, fallbackLocale);
    if (asTrimmedString(targetInput) && !directTarget) {
      throw new Error('Zendesk URL 또는 Article ID 형식이 올바르지 않습니다.');
    }
    const envTarget =
      parseZendeskTarget(envConfig.defaultArticleUrl, fallbackLocale) ||
      parseZendeskTarget(envConfig.defaultArticleId, fallbackLocale);
    const resolvedTarget = directTarget || envTarget;

    const normalizedBody = typeof body === 'string' ? body : String(body ?? '');
    const normalizedTitle = asTrimmedString(title) || 'API Manual';
    const metadataOverrides = {
      labelNames: normalizeZendeskLabelNames(options?.labelNames),
      commentsDisabled: typeof options?.commentsDisabled === 'boolean' ? options.commentsDisabled : undefined,
    };
    const metadata = buildZendeskArticleMetadataFromEnv(envConfig, metadataOverrides);
    const draftFlag = draft !== undefined && draft !== null
      ? !!draft
      : (envConfig.defaultDraft !== null ? envConfig.defaultDraft : undefined);

    let articleId = '';
    let effectiveLocale = fallbackLocale;
    let articleUrl = '';
    let mode = 'update';

    if (resolvedTarget) {
      articleId = String(resolvedTarget.articleId);
      effectiveLocale = normalizeZendeskLocale(resolvedTarget.locale || fallbackLocale);

      if (Object.keys(metadata).length > 0 || envConfig.defaultNotifySubscribers !== null) {
        await updateZendeskArticle(config, articleId, metadata, envConfig.defaultNotifySubscribers);
      }

      await updateZendeskArticleTranslation(
        config,
        articleId,
        effectiveLocale,
        normalizedBody,
        normalizedTitle,
        draftFlag
      );
    } else {
      if (envConfig.defaultSectionId === null) {
        throw new Error('기본 섹션 생성 모드에는 ZENDESK_DEFAULT_SECTION_ID 설정이 필요합니다.');
      }

      mode = 'create';
      const created = await createZendeskArticle(
        config,
        envConfig.defaultSectionId,
        fallbackLocale,
        normalizedTitle,
        normalizedBody,
        envConfig,
        draftFlag,
        metadataOverrides
      );

      articleId = String(created?.id || '');
      if (!articleId) {
        throw new Error('Zendesk article 생성 결과에서 article id를 찾을 수 없습니다.');
      }

      effectiveLocale = normalizeZendeskLocale(created?.source_locale || fallbackLocale);
      articleUrl = asTrimmedString(created?.html_url);

      await updateZendeskArticleTranslation(
        config,
        articleId,
        effectiveLocale,
        normalizedBody,
        normalizedTitle,
        draftFlag
      );
    }

    let associatedAttachments = [];
    if (envConfig.defaultAttachmentIds.length > 0) {
      associatedAttachments = await associateZendeskAttachments(
        config,
        articleId,
        envConfig.defaultAttachmentIds
      );
    }

    if (!articleUrl) {
      articleUrl = buildZendeskArticleUrl(config.subdomain, effectiveLocale, articleId);
    }

    return {
      success: true,
      data: {
        mode,
        articleId,
        locale: effectiveLocale,
        articleUrl,
        associatedAttachmentCount: associatedAttachments.length,
      },
    };
  } catch (error) {
    console.error('Zendesk publishManualWithEnv error:', error);
    return { success: false, error: error.message };
  }
});

/**
 * Get all articles (for Zendesk Manager)
 * GET /api/v2/help_center/articles.json?per_page={per_page}&page={page}
 */
ipcMain.handle('zendesk:getAllArticles', async (event, config, perPage = 100, page = 1) => {
  try {
    const normalizedPerPage = Math.max(1, Math.min(100, Number.parseInt(String(perPage), 10) || 100));
    const normalizedPage = Math.max(1, Number.parseInt(String(page), 10) || 1);

    const options = buildZendeskRequestOptions(
      config,
      `/api/v2/help_center/articles.json?per_page=${normalizedPerPage}&page=${normalizedPage}`
    );

    const data = await makeZendeskRequest(options);
    return { success: true, data };
  } catch (error) {
    console.error('Zendesk getAllArticles error:', error);
    return { success: false, error: error.message };
  }
});

/**
 * Get articles by section ID
 * GET /api/v2/help_center/sections/{section_id}/articles.json
 */
ipcMain.handle('zendesk:getArticlesBySection', async (event, config, sectionId, locale = 'en-us') => {
  try {
    const safeSectionId = toZendeskPathSegment(sectionId, 'sectionId');
    const normalizedLocale = encodeURIComponent(normalizeZendeskLocale(locale));

    const options = buildZendeskRequestOptions(
      config,
      `/api/v2/help_center/sections/${safeSectionId}/articles.json?locale=${normalizedLocale}`
    );

    const data = await makeZendeskRequest(options);
    return { success: true, data: data.articles };
  } catch (error) {
    console.error('Zendesk getArticlesBySection error:', error);
    return { success: false, error: error.message };
  }
});

// ============================================
// Terminal IPC Handlers (Bat Rack)
// ============================================

/**
 * Create a new terminal session
 */
ipcMain.handle('terminal:create', async (event, options = {}) => {
  if (!pty) {
    return { success: false, error: 'node-pty not available' };
  }

  try {
    const id = ++terminalIdCounter;
    const shell = os.platform() === 'win32' ? 'powershell.exe' : 'bash';
    const cwd = options.cwd || process.cwd();

    const ptyProcess = pty.spawn(shell, [], {
      name: 'xterm-256color',
      cols: options.cols || 80,
      rows: options.rows || 24,
      cwd: cwd,
      env: { ...process.env, ...options.env },
    });

    const terminal = {
      pty: ptyProcess,
      logs: [],
      name: options.name || `Terminal ${id}`,
      cwd: cwd,
    };

    terminals.set(id, terminal);

    // Forward data to renderer
    ptyProcess.onData((data) => {
      if (mainWindow && !mainWindow.isDestroyed()) {
        mainWindow.webContents.send('terminal:data', { id, data });
      }
      // Keep last 1000 lines of logs
      terminal.logs.push(data);
      if (terminal.logs.length > 1000) {
        terminal.logs.shift();
      }
    });

    ptyProcess.onExit(({ exitCode }) => {
      if (mainWindow && !mainWindow.isDestroyed()) {
        mainWindow.webContents.send('terminal:exit', { id, exitCode });
      }
      terminals.delete(id);
    });

    console.log(`Terminal created: id=${id}, cwd=${cwd}`);
    return { success: true, id, name: terminal.name };
  } catch (error) {
    console.error('Terminal create error:', error);
    return { success: false, error: error.message };
  }
});

/**
 * Write data to terminal
 */
ipcMain.handle('terminal:write', async (event, id, data) => {
  const terminal = terminals.get(id);
  if (!terminal) {
    return { success: false, error: 'Terminal not found' };
  }

  try {
    terminal.pty.write(data);
    return { success: true };
  } catch (error) {
    return { success: false, error: error.message };
  }
});

/**
 * Resize terminal
 */
ipcMain.handle('terminal:resize', async (event, id, cols, rows) => {
  const terminal = terminals.get(id);
  if (!terminal) {
    return { success: false, error: 'Terminal not found' };
  }

  try {
    terminal.pty.resize(cols, rows);
    return { success: true };
  } catch (error) {
    return { success: false, error: error.message };
  }
});

/**
 * Kill terminal session
 */
ipcMain.handle('terminal:kill', async (event, id) => {
  const terminal = terminals.get(id);
  if (!terminal) {
    return { success: false, error: 'Terminal not found' };
  }

  try {
    terminal.pty.kill();
    terminals.delete(id);
    console.log(`Terminal killed: id=${id}`);
    return { success: true };
  } catch (error) {
    return { success: false, error: error.message };
  }
});

/**
 * List active terminals
 */
ipcMain.handle('terminal:list', async () => {
  const list = [];
  for (const [id, terminal] of terminals) {
    list.push({
      id,
      name: terminal.name,
      cwd: terminal.cwd,
    });
  }
  return { success: true, data: list };
});

/**
 * Get terminal logs
 */
ipcMain.handle('terminal:getLogs', async (event, id) => {
  const terminal = terminals.get(id);
  if (!terminal) {
    return { success: false, error: 'Terminal not found' };
  }
  return { success: true, data: terminal.logs.join('') };
});

/**
 * Kill process by port (Windows)
 */
ipcMain.handle('terminal:killPort', async (event, port) => {
  if (!port) {
    return { success: false, error: 'Port is required' };
  }

  try {
    const { exec } = require('child_process');

    return new Promise((resolve) => {
      // Windows: netstat로 PID 찾고 taskkill로 종료
      exec(`netstat -ano | findstr :${port}`, (err, stdout) => {
        if (err || !stdout.trim()) {
          resolve({ success: true, message: `No process on port ${port}` });
          return;
        }

        // PID 추출 (마지막 열)
        const lines = stdout.trim().split('\n');
        const pids = new Set();
        for (const line of lines) {
          const parts = line.trim().split(/\s+/);
          const pid = parts[parts.length - 1];
          if (pid && /^\d+$/.test(pid) && pid !== '0') {
            pids.add(pid);
          }
        }

        if (pids.size === 0) {
          resolve({ success: true, message: `No process on port ${port}` });
          return;
        }

        // 각 PID 종료
        let killed = 0;
        for (const pid of pids) {
          exec(`taskkill /F /PID ${pid}`, (killErr) => {
            if (!killErr) killed++;
          });
        }

        setTimeout(() => {
          resolve({ success: true, message: `Killed ${killed} process(es) on port ${port}` });
        }, 500);
      });
    });
  } catch (error) {
    return { success: false, error: error.message };
  }
});

/**
 * Sync terminal session state (for persistent state across tab switches)
 */
ipcMain.handle('terminal:syncState', async () => {
  const list = [];
  for (const [id, terminal] of terminals) {
    list.push({
      id,
      serverId: terminal.serverId || null,
      name: terminal.name,
      cwd: terminal.cwd,
    });
  }
  return { success: true, data: list };
});

/**
 * Set serverId for a terminal (for tracking which server it belongs to)
 */
ipcMain.handle('terminal:setServerId', async (event, id, serverId) => {
  const terminal = terminals.get(id);
  if (!terminal) {
    return { success: false, error: 'Terminal not found' };
  }
  terminal.serverId = serverId;
  return { success: true };
});

// IPC handler for releasing all locks (called on app close)
ipcMain.handle('locks:releaseAll', async (event, userId) => {
  if (!userId) {
    return { success: false, error: 'userId is required' };
  }

  try {
    const http = require('http');

    return new Promise((resolve) => {
      const options = {
        hostname: 'localhost',
        port: 9527,
        path: `/api/locks/user/${encodeURIComponent(userId)}/all`,
        method: 'DELETE',
        headers: {
          'Content-Type': 'application/json'
        }
      };

      const req = http.request(options, (res) => {
        let data = '';
        res.on('data', (chunk) => { data += chunk; });
        res.on('end', () => {
          console.log('🔓 Locks released:', data);
          resolve({ success: true, data });
        });
      });

      req.on('error', (error) => {
        console.error('🔓 Lock release error:', error.message);
        resolve({ success: false, error: error.message });
      });

      req.setTimeout(3000, () => {
        req.destroy();
        resolve({ success: false, error: 'Timeout' });
      });

      req.end();
    });
  } catch (error) {
    console.error('Lock release error:', error);
    return { success: false, error: error.message };
  }
});

// Clean up terminals on app quit
app.on('before-quit', () => {
  console.log('🔥 Cleaning up terminals before quit...');
  for (const [id, terminal] of terminals) {
    try {
      terminal.pty.kill();
      console.log(`  Killed terminal ${id}`);
    } catch (e) {
      // ignore
    }
  }
  terminals.clear();
});
