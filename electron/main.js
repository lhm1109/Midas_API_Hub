const { app, BrowserWindow, ipcMain, dialog } = require('electron');
const path = require('path');
const fs = require('fs').promises;
const https = require('https');
// const db = require('./database'); // DB 기능은 나중에 활성화

let mainWindow;

function createWindow() {
  mainWindow = new BrowserWindow({
    width: 1600,
    height: 1000,
    minWidth: 1200,
    minHeight: 800,
    webPreferences: {
      nodeIntegration: false,
      contextIsolation: true,
      preload: path.join(__dirname, 'preload.js'),
    },
    titleBarStyle: 'default',
    frame: true,
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

app.whenReady().then(() => {
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
          reject(new Error(`HTTP ${res.statusCode}: ${data}`));
        }
      });
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

/**
 * Get all translations for an article
 * GET /api/v2/help_center/articles/{article_id}/translations.json
 */
ipcMain.handle('zendesk:getAllTranslations', async (event, config, articleId) => {
  try {
    const auth = Buffer.from(`${config.email}:${config.password}`).toString('base64');
    
    const options = {
      hostname: `${config.subdomain}.zendesk.com`,
      path: `/api/v2/help_center/articles/${articleId}/translations.json`,
      method: 'GET',
      headers: {
        'Authorization': `Basic ${auth}`,
        'Content-Type': 'application/json'
      }
    };

    const data = await makeZendeskRequest(options);
    return { success: true, data: data.translations };
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
    const auth = Buffer.from(`${config.email}:${config.password}`).toString('base64');
    
    const options = {
      hostname: `${config.subdomain}.zendesk.com`,
      path: `/api/v2/help_center/articles/${articleId}/translations/${locale}.json`,
      method: 'GET',
      headers: {
        'Authorization': `Basic ${auth}`,
        'Content-Type': 'application/json'
      }
    };

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
    const auth = Buffer.from(`${config.email}:${config.password}`).toString('base64');
    
    const payload = {
      translation: {
        body: body
      }
    };

    if (title !== undefined && title !== null) {
      payload.translation.title = title;
    }

    if (draft !== undefined && draft !== null) {
      payload.translation.draft = draft;
    }

    const postData = JSON.stringify(payload);

    const options = {
      hostname: `${config.subdomain}.zendesk.com`,
      path: `/api/v2/help_center/articles/${articleId}/translations/${locale}.json`,
      method: 'PUT',
      headers: {
        'Authorization': `Basic ${auth}`,
        'Content-Type': 'application/json',
        'Content-Length': Buffer.byteLength(postData)
      }
    };

    const data = await makeZendeskRequest(options, postData);
    return { success: true, data: data.translation };
  } catch (error) {
    console.error('Zendesk updateArticleTranslation error:', error);
    return { success: false, error: error.message };
  }
});

/**
 * Get all articles (for Zendesk Manager)
 * GET /api/v2/help_center/articles.json?per_page={per_page}&page={page}
 */
ipcMain.handle('zendesk:getAllArticles', async (event, config, perPage = 100, page = 1) => {
  try {
    const auth = Buffer.from(`${config.email}:${config.password}`).toString('base64');
    
    const options = {
      hostname: `${config.subdomain}.zendesk.com`,
      path: `/api/v2/help_center/articles.json?per_page=${perPage}&page=${page}`,
      method: 'GET',
      headers: {
        'Authorization': `Basic ${auth}`,
        'Content-Type': 'application/json'
      }
    };

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
    const auth = Buffer.from(`${config.email}:${config.password}`).toString('base64');
    
    const options = {
      hostname: `${config.subdomain}.zendesk.com`,
      path: `/api/v2/help_center/sections/${sectionId}/articles.json?locale=${locale}`,
      method: 'GET',
      headers: {
        'Authorization': `Basic ${auth}`,
        'Content-Type': 'application/json'
      }
    };

    const data = await makeZendeskRequest(options);
    return { success: true, data: data.articles };
  } catch (error) {
    console.error('Zendesk getArticlesBySection error:', error);
    return { success: false, error: error.message };
  }
});

