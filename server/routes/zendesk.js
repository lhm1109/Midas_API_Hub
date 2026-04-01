import express from 'express';
import path from 'path';
import dotenv from 'dotenv';

const router = express.Router();
const ZENDESK_DEFAULT_LOCALE = 'en-us';
const ZENDESK_TIMEOUT_MS = 30000;
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
  return { subdomain, auth };
}

async function makeZendeskRequest(config, requestPath, method = 'GET', payload = null) {
  const { subdomain, auth } = getZendeskAuthContext(config);
  const controller = new AbortController();
  const timeoutId = setTimeout(() => controller.abort(), ZENDESK_TIMEOUT_MS);

  try {
    const response = await fetch(`https://${subdomain}.zendesk.com${requestPath}`, {
      method,
      headers: {
        Authorization: `Basic ${auth}`,
        'Content-Type': 'application/json',
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
        // keep plain text
      }
    }

    if (!response.ok) {
      const bodyMessage = typeof data === 'string' ? data : JSON.stringify(data);
      const normalizedBody = asTrimmedString(bodyMessage);

      if ((response.status === 401 || response.status === 403) && !normalizedBody) {
        throw new Error(
          `HTTP ${response.status}: Zendesk 인증/권한 오류. ` +
          `API token 또는 agent 권한(Guide 편집 권한)을 확인하세요.`
        );
      }

      throw new Error(`HTTP ${response.status}: ${normalizedBody || 'No response body'}`);
    }

    return data;
  } catch (error) {
    if (error?.name === 'AbortError') {
      throw new Error('Zendesk request timeout');
    }
    throw error;
  } finally {
    clearTimeout(timeoutId);
  }
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

  const data = await makeZendeskRequest(
    config,
    `/api/v2/help_center/articles/${safeArticleId}/translations/${safeLocale}.json`,
    'PUT',
    payload
  );

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

  const data = await makeZendeskRequest(
    config,
    `/api/v2/help_center/articles/${safeArticleId}.json`,
    'PUT',
    payload
  );

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

  const payload = {
    article: {
      ...metadata,
      title: asTrimmedString(title) || 'API Manual',
      body: typeof body === 'string' ? body : String(body ?? ''),
      locale: normalizedLocale,
      draft: articleDraft,
    },
    notify_subscribers: envConfig.defaultNotifySubscribers !== null
      ? envConfig.defaultNotifySubscribers
      : false,
  };

  const data = await makeZendeskRequest(
    config,
    `/api/v2/help_center/sections/${safeSectionId}/articles.json`,
    'POST',
    payload
  );

  return data.article;
}

async function associateZendeskAttachments(config, articleId, attachmentIds) {
  if (!Array.isArray(attachmentIds) || attachmentIds.length === 0) {
    return [];
  }

  const safeArticleId = toZendeskPathSegment(articleId, 'articleId');
  const payload = { attachment_ids: attachmentIds };
  const data = await makeZendeskRequest(
    config,
    `/api/v2/help_center/articles/${safeArticleId}/bulk_attachments.json`,
    'POST',
    payload
  );
  return data.article_attachments || [];
}

router.get('/env', async (req, res) => {
  try {
    const envConfig = getZendeskConfigFromEnv();
    res.json({
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
    });
  } catch (error) {
    console.error('Zendesk env error:', error);
    res.status(500).json({ success: false, error: error.message });
  }
});

router.get('/article', async (req, res) => {
  try {
    const envConfig = getZendeskConfigFromEnv();
    if (!envConfig.isReady) {
      return res.status(400).json({
        success: false,
        error: `Zendesk .env settings missing: ${envConfig.missingFields.join(', ')}`,
      });
    }

    const config = {
      subdomain: envConfig.subdomain,
      email: envConfig.email,
      apiToken: envConfig.apiToken,
      password: envConfig.password,
    };

    const { targetInput = '', locale = '' } = req.query || {};
    const fallbackLocale = normalizeZendeskLocale(locale || envConfig.defaultLocale);
    const directTarget = parseZendeskTarget(targetInput, fallbackLocale);

    if (!directTarget) {
      return res.status(400).json({
        success: false,
        error: 'Zendesk URL 또는 Article ID 형식이 올바르지 않습니다.',
      });
    }

    const safeArticleId = toZendeskPathSegment(directTarget.articleId, 'articleId');
    const safeLocale = toZendeskPathSegment(directTarget.locale, 'locale');

    // 번역본(body HTML) 가져오기
    const translationData = await makeZendeskRequest(
      config,
      `/api/v2/help_center/articles/${safeArticleId}/translations/${safeLocale}.json`
    );
    const translation = translationData?.translation || {};

    res.json({
      success: true,
      data: {
        articleId: directTarget.articleId,
        locale: directTarget.locale,
        title: asTrimmedString(translation.title),
        body: asTrimmedString(translation.body),
        articleUrl: buildZendeskArticleUrl(config.subdomain, directTarget.locale, directTarget.articleId),
        updatedAt: translation.updated_at || null,
      },
    });
  } catch (error) {
    console.error('Zendesk fetch article error:', error);
    res.status(500).json({ success: false, error: error.message });
  }
});

router.post('/publish', async (req, res) => {
  try {
    const envConfig = getZendeskConfigFromEnv();
    if (!envConfig.isReady) {
      return res.status(400).json({
        success: false,
        error: `Zendesk .env settings missing: ${envConfig.missingFields.join(', ')}`,
      });
    }

    const config = {
      subdomain: envConfig.subdomain,
      email: envConfig.email,
      apiToken: envConfig.apiToken,
      password: envConfig.password,
    };

    const { targetInput = '', locale = '', body = '', title, draft, labelNames, commentsDisabled } = req.body || {};

    const fallbackLocale = normalizeZendeskLocale(locale || envConfig.defaultLocale);
    const directTarget = parseZendeskTarget(targetInput, fallbackLocale);
    if (asTrimmedString(targetInput) && !directTarget) {
      return res.status(400).json({
        success: false,
        error: 'Zendesk URL 또는 Article ID 형식이 올바르지 않습니다.',
      });
    }

    const envTarget =
      parseZendeskTarget(envConfig.defaultArticleUrl, fallbackLocale) ||
      parseZendeskTarget(envConfig.defaultArticleId, fallbackLocale);
    const resolvedTarget = directTarget || envTarget;

    const normalizedBody = typeof body === 'string' ? body : String(body ?? '');
    const normalizedTitle = asTrimmedString(title) || 'API Manual';
    const metadataOverrides = {
      labelNames: normalizeZendeskLabelNames(labelNames),
      commentsDisabled: typeof commentsDisabled === 'boolean' ? commentsDisabled : undefined,
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
        return res.status(400).json({
          success: false,
          error: '기본 섹션 생성 모드에는 ZENDESK_DEFAULT_SECTION_ID 설정이 필요합니다.',
        });
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

    res.json({
      success: true,
      data: {
        mode,
        articleId,
        locale: effectiveLocale,
        articleUrl,
        associatedAttachmentCount: associatedAttachments.length,
      },
    });
  } catch (error) {
    console.error('Zendesk publish error:', error);
    res.status(500).json({ success: false, error: error.message });
  }
});

export default router;
