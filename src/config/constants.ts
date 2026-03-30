// 🎯 애플리케이션 상수

export const API_CONFIG = {
  BASE_URL: 'http://localhost:9527/api',
  HEALTH_CHECK_URL: 'http://localhost:9527/health',
  TIMEOUT: 30000,
} as const;

export const DEFAULT_SETTINGS = {
  baseUrl: 'https://api-beta.midasit.com/civil',
  mapiKey: '',
  commonHeaders: JSON.stringify(
    {
      'Content-Type': 'application/json',
      Accept: 'application/json',
    },
    null,
    2
  ),
} as const;

/** Manual Hub Site 인덱스 기본 로컬 경로 (Settings 기본값과 동일) */
export const DEFAULT_MANUAL_HUB_INDEX_LOCAL_PATH_KO =
  'C:\\MIDAS\\code\\apiverification\\api_docs_zendesk\\manual_list_ko.html';
export const DEFAULT_MANUAL_HUB_INDEX_LOCAL_PATH_EN =
  'C:\\MIDAS\\code\\apiverification\\api_docs_zendesk\\manual_list_en.html';

export const ROUTES = {
  PROJECTS: 'projects',
  HISTORY: 'history',
} as const;

export const TABS = {
  VERSION: 'version',
  MANUAL: 'manual',
  SPEC: 'spec',
  BUILDER: 'builder',
  RUNNER: 'runner',
} as const;

export const HTTP_METHODS = ['GET', 'POST', 'PUT', 'DELETE', 'PATCH'] as const;

export const STORAGE_KEYS = {
  SETTINGS: 'api-verification-settings',
  LAST_ENDPOINT: 'api-verification-last-endpoint',
} as const;









