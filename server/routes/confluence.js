import express from 'express';
import path from 'path';
import dotenv from 'dotenv';

const router = express.Router();
const CONFLUENCE_TIMEOUT_MS = 30000;
const ENV_PATH = path.join(process.cwd(), '.env');

function refreshEnvFromFile() {
  dotenv.config({ path: ENV_PATH, override: true });
}

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

function normalizeConfluenceBaseUrl(value) {
  const raw = asTrimmedString(value);
  if (!raw) return '';

  try {
    const parsed = new URL(raw);
    let pathname = parsed.pathname.replace(/\/+$/, '');

    if (pathname.endsWith('/api/v2')) {
      pathname = pathname.replace(/\/api\/v2$/, '');
    }
    if (pathname.endsWith('/rest/api')) {
      pathname = pathname.replace(/\/rest\/api$/, '');
    }
    if (!pathname) {
      pathname = '/wiki';
    }

    return `${parsed.origin}${pathname}`;
  } catch {
    return '';
  }
}

function buildConfluenceApiBase(baseUrl) {
  const normalizedBaseUrl = normalizeConfluenceBaseUrl(baseUrl);
  if (!normalizedBaseUrl) return '';

  if (/\/(wiki|confluence)$/i.test(normalizedBaseUrl)) {
    return `${normalizedBaseUrl}/api/v2`;
  }

  return `${normalizedBaseUrl}/wiki/api/v2`;
}

function parseConfluenceTarget(targetInput, fallbackBaseUrl = '') {
  const raw = asTrimmedString(targetInput);
  if (!raw) return null;

  if (/^\d+$/.test(raw)) {
    return {
      pageId: raw,
      baseUrl: normalizeConfluenceBaseUrl(fallbackBaseUrl),
    };
  }

  try {
    const parsedUrl = new URL(raw);
    const pageIdFromQuery = asTrimmedString(parsedUrl.searchParams.get('pageId'));
    const pathMatch = parsedUrl.pathname.match(/\/pages\/(\d+)/i);
    const pageId = pageIdFromQuery || pathMatch?.[1] || '';
    if (!pageId) {
      return null;
    }

    let basePath = '';
    if (parsedUrl.pathname.startsWith('/wiki')) {
      basePath = '/wiki';
    } else if (parsedUrl.pathname.startsWith('/confluence')) {
      basePath = '/confluence';
    }

    return {
      pageId,
      baseUrl: normalizeConfluenceBaseUrl(`${parsedUrl.origin}${basePath}`),
    };
  } catch {
    return null;
  }
}

function buildConfluencePageUrl(baseUrl, pageId, pageData = null) {
  const normalizedBaseUrl = normalizeConfluenceBaseUrl(baseUrl);
  const webUi = asTrimmedString(pageData?._links?.webui);

  if (normalizedBaseUrl && webUi) {
    try {
      return new URL(webUi, `${normalizedBaseUrl}/`).toString();
    } catch {
      // fall back below
    }
  }

  if (!normalizedBaseUrl || !pageId) {
    return '';
  }

  return `${normalizedBaseUrl}/pages/viewpage.action?pageId=${encodeURIComponent(String(pageId))}`;
}

function getConfluenceConfigFromEnv() {
  refreshEnvFromFile();

  const baseUrl = normalizeConfluenceBaseUrl(process.env.CONFLUENCE_BASE_URL);
  const token = asTrimmedString(process.env.CONFLUENCE_API_TOKEN);
  const email = asTrimmedString(process.env.CONFLUENCE_EMAIL);
  const authMode = asTrimmedString(process.env.CONFLUENCE_AUTH_MODE).toLowerCase();
  const defaultPageUrl = asTrimmedString(process.env.CONFLUENCE_DEFAULT_PAGE_URL);
  const defaultPageId = asTrimmedString(process.env.CONFLUENCE_DEFAULT_PAGE_ID);
  const defaultSpaceId = asTrimmedString(process.env.CONFLUENCE_DEFAULT_SPACE_ID);
  const defaultParentPageId = asTrimmedString(process.env.CONFLUENCE_DEFAULT_PARENT_PAGE_ID);

  const missingFields = [];
  if (!token) missingFields.push('CONFLUENCE_API_TOKEN');

  const useBasicAuth = authMode === 'basic' || (!!email && authMode !== 'bearer');
  if (useBasicAuth && !email) {
    missingFields.push('CONFLUENCE_EMAIL');
  }

  return {
    baseUrl,
    token,
    email,
    defaultPageUrl,
    defaultPageId,
    defaultSpaceId,
    defaultParentPageId,
    authType: useBasicAuth ? 'basic' : (token ? 'bearer' : null),
    isReady: missingFields.length === 0,
    missingFields,
  };
}

function buildConfluenceAuthHeaders(config) {
  const token = asTrimmedString(config?.token);
  const authType = config?.authType;

  if (!token || !authType) {
    throw new Error('Invalid Confluence config: missing token/authType');
  }

  if (authType === 'basic') {
    const email = asTrimmedString(config?.email);
    if (!email) {
      throw new Error('Invalid Confluence config: missing email for basic auth');
    }

    return {
      Authorization: `Basic ${Buffer.from(`${email}:${token}`).toString('base64')}`,
    };
  }

  return {
    Authorization: `Bearer ${token}`,
  };
}

async function makeConfluenceRequest(config, requestPath, method = 'GET', payload = null) {
  const apiBase = buildConfluenceApiBase(config?.baseUrl);
  if (!apiBase) {
    throw new Error('Confluence base URL is required. Set CONFLUENCE_BASE_URL or use a full page URL.');
  }

  const controller = new AbortController();
  const timeoutId = setTimeout(() => controller.abort(), CONFLUENCE_TIMEOUT_MS);

  try {
    const response = await fetch(`${apiBase}${requestPath}`, {
      method,
      headers: {
        Accept: 'application/json',
        'Content-Type': 'application/json',
        ...buildConfluenceAuthHeaders(config),
      },
      body: payload !== null && payload !== undefined ? JSON.stringify(payload) : undefined,
      signal: controller.signal,
    });

    const rawBody = await response.text();
    let data = rawBody;
    if (rawBody) {
      try {
        data = JSON.parse(rawBody);
      } catch {
        // Keep plain text response.
      }
    }

    if (!response.ok) {
      const bodyMessage = typeof data === 'string' ? data : JSON.stringify(data);
      const normalizedBody = asTrimmedString(bodyMessage);

      if ((response.status === 401 || response.status === 403) && !normalizedBody) {
        throw new Error(`HTTP ${response.status}: Confluence 인증 또는 권한을 확인하세요.`);
      }

      throw new Error(`HTTP ${response.status}: ${normalizedBody || 'No response body'}`);
    }

    return data;
  } catch (error) {
    if (error?.name === 'AbortError') {
      throw new Error('Confluence request timeout');
    }
    throw error;
  } finally {
    clearTimeout(timeoutId);
  }
}

async function getConfluencePage(config, pageId) {
  return makeConfluenceRequest(
    config,
    `/pages/${encodeURIComponent(String(pageId))}?body-format=storage`,
    'GET'
  );
}

async function updateConfluencePage(config, pageId, title, body, currentPage) {
  const payload = {
    id: String(pageId),
    status: 'current',
    title,
    body: {
      representation: 'storage',
      value: typeof body === 'string' ? body : String(body ?? ''),
    },
    version: {
      number: Number(currentPage?.version?.number || 0) + 1,
    },
  };

  if (currentPage?.parentId) {
    payload.parentId = String(currentPage.parentId);
  }

  return makeConfluenceRequest(
    config,
    `/pages/${encodeURIComponent(String(pageId))}`,
    'PUT',
    payload
  );
}

async function createConfluencePage(config, title, body, spaceId, parentId = '') {
  const payload = {
    spaceId: String(spaceId),
    status: 'current',
    title,
    body: {
      representation: 'storage',
      value: typeof body === 'string' ? body : String(body ?? ''),
    },
  };

  if (asTrimmedString(parentId)) {
    payload.parentId = String(parentId);
  }

  return makeConfluenceRequest(config, '/pages', 'POST', payload);
}

router.get('/env', async (_req, res) => {
  try {
    const envConfig = getConfluenceConfigFromEnv();
    res.json({
      success: true,
      data: {
        baseUrl: envConfig.baseUrl,
        defaultPageUrl: envConfig.defaultPageUrl,
        defaultPageId: envConfig.defaultPageId,
        defaultSpaceId: envConfig.defaultSpaceId,
        defaultParentPageId: envConfig.defaultParentPageId,
        hasCredentials: envConfig.isReady,
        authType: envConfig.authType,
        missingFields: envConfig.missingFields,
      },
    });
  } catch (error) {
    console.error('Confluence env error:', error);
    res.status(500).json({ success: false, error: error.message });
  }
});

router.post('/publish', async (req, res) => {
  try {
    const envConfig = getConfluenceConfigFromEnv();
    if (!envConfig.isReady) {
      return res.status(400).json({
        success: false,
        error: `Confluence .env settings missing: ${envConfig.missingFields.join(', ')}`,
      });
    }

    const { targetInput = '', body = '', title } = req.body || {};
    const normalizedBody = typeof body === 'string' ? body : String(body ?? '');
    const normalizedTitle = asTrimmedString(title) || 'API Manual';

    const directTarget = parseConfluenceTarget(targetInput, envConfig.baseUrl);
    if (asTrimmedString(targetInput) && !directTarget) {
      return res.status(400).json({
        success: false,
        error: 'Confluence 페이지 URL 또는 Page ID 형식이 올바르지 않습니다.',
      });
    }

    const envTarget =
      parseConfluenceTarget(envConfig.defaultPageUrl, envConfig.baseUrl) ||
      parseConfluenceTarget(envConfig.defaultPageId, envConfig.baseUrl);

    const resolvedTarget = directTarget || envTarget;
    const effectiveBaseUrl = normalizeConfluenceBaseUrl(
      resolvedTarget?.baseUrl || envConfig.baseUrl
    );

    const requestConfig = {
      ...envConfig,
      baseUrl: effectiveBaseUrl,
    };

    let pageId = '';
    let pageUrl = '';
    let mode = 'update';
    let versionNumber = 1;

    if (resolvedTarget?.pageId) {
      const currentPage = await getConfluencePage(requestConfig, resolvedTarget.pageId);
      const updatedPage = await updateConfluencePage(
        requestConfig,
        resolvedTarget.pageId,
        normalizedTitle,
        normalizedBody,
        currentPage
      );

      pageId = String(updatedPage?.id || resolvedTarget.pageId);
      versionNumber = Number(updatedPage?.version?.number || (Number(currentPage?.version?.number || 0) + 1));
      pageUrl = buildConfluencePageUrl(effectiveBaseUrl, pageId, updatedPage);
    } else {
      if (!asTrimmedString(envConfig.defaultSpaceId)) {
        return res.status(400).json({
          success: false,
          error: '새 페이지 생성에는 CONFLUENCE_DEFAULT_SPACE_ID 설정이 필요합니다.',
        });
      }

      mode = 'create';
      const createdPage = await createConfluencePage(
        requestConfig,
        normalizedTitle,
        normalizedBody,
        envConfig.defaultSpaceId,
        envConfig.defaultParentPageId
      );

      pageId = String(createdPage?.id || '');
      if (!pageId) {
        throw new Error('Confluence page 생성 결과에서 page id를 찾을 수 없습니다.');
      }

      versionNumber = Number(createdPage?.version?.number || 1);
      pageUrl = buildConfluencePageUrl(effectiveBaseUrl, pageId, createdPage);
    }

    res.json({
      success: true,
      data: {
        mode,
        pageId,
        pageUrl,
        versionNumber,
        title: normalizedTitle,
      },
    });
  } catch (error) {
    console.error('Confluence publish error:', error);
    res.status(500).json({ success: false, error: error.message });
  }
});

export default router;
