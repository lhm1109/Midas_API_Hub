import { toast } from 'sonner';

export const MANUAL_SERVER_BASE_URL = 'http://localhost:9527';
export const DEFAULT_ZENDESK_LOCALE = 'en-us';

export type ZendeskEnvStatus = {
  baseUrl?: string;
  subdomain: string;
  defaultLocale: string;
  defaultArticleUrl: string;
  defaultArticleId?: string;
  defaultSectionId?: number | null;
  hasCredentials: boolean;
  authType: 'token' | 'password' | null;
  missingFields: string[];
};

export function normalizeZendeskLocale(locale?: string): string {
  const normalized = String(locale || '')
    .trim()
    .replace(/_/g, '-')
    .toLowerCase();
  return normalized || DEFAULT_ZENDESK_LOCALE;
}

export async function loadZendeskEnvStatus(): Promise<ZendeskEnvStatus | null> {
  try {
    const zendeskAPI = window.electronAPI?.zendesk;
    let result: { success: boolean; data?: ZendeskEnvStatus; error?: string } | null = null;

    if (zendeskAPI?.getEnvConfig) {
      result = await zendeskAPI.getEnvConfig();
    } else {
      const response = await fetch(`${MANUAL_SERVER_BASE_URL}/api/zendesk/env`);
      const payload = await response.json().catch(() => null);
      if (!response.ok) {
        const errorMessage = payload?.error || `HTTP ${response.status}`;
        throw new Error(errorMessage);
      }
      result = payload;
    }

    if (!result?.success || !result.data) {
      return null;
    }
    return result.data;
  } catch (e) {
    console.warn('Failed to load Zendesk env config:', e);
    return null;
  }
}

export async function fetchZendeskArticleBody(
  targetUrl: string,
  defaultLocale: string
): Promise<{ body: string; title: string; updatedAt: string | null; articleId: string } | null> {
  const locale = normalizeZendeskLocale(defaultLocale);
  const params = new URLSearchParams({ targetInput: targetUrl, locale });
  const response = await fetch(`${MANUAL_SERVER_BASE_URL}/api/zendesk/article?${params}`);
  const payload = await response.json().catch(() => null);
  if (!response.ok || !payload?.success) {
    toast.error(`Zendesk 불러오기 실패: ${payload?.error || `HTTP ${response.status}`}`);
    return null;
  }
  return {
    body: payload.data.body || '',
    title: payload.data.title || '',
    updatedAt: payload.data.updatedAt || null,
    articleId: String(payload.data.articleId || ''),
  };
}

export async function publishZendeskHtml(params: {
  targetInput: string;
  locale: string;
  html: string;
  title?: string;
}): Promise<
  | {
      success: true;
      articleUrl?: string;
      articleId?: string;
      mode?: 'create' | 'update';
    }
  | { success: false; error: string }
> {
  const zendeskAPI = window.electronAPI?.zendesk;
  const useElectronPublisher = !!zendeskAPI?.publishManualWithEnv;

  if (useElectronPublisher) {
    const result = await zendeskAPI!.publishManualWithEnv(
      params.targetInput.trim(),
      params.locale,
      params.html,
      params.title || undefined,
      undefined
    );
    if (!result.success) {
      return { success: false, error: result.error || 'Unknown error' };
    }
    return {
      success: true,
      articleUrl: result.data?.articleUrl,
      articleId: result.data?.articleId,
      mode: result.data?.mode,
    };
  }

  const response = await fetch(`${MANUAL_SERVER_BASE_URL}/api/zendesk/publish`, {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({
      targetInput: params.targetInput.trim(),
      locale: params.locale,
      body: params.html,
      title: params.title || undefined,
      draft: undefined,
    }),
  });

  const payload = await response.json().catch(() => null);
  if (!response.ok || !payload?.success) {
    return { success: false, error: payload?.error || `HTTP ${response.status}` };
  }
  return {
    success: true,
    articleUrl: payload.data?.articleUrl,
    articleId: payload.data?.articleId,
    mode: payload.data?.mode,
  };
}
