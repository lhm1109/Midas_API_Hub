import {
  DEFAULT_MANUAL_HUB_INDEX_LOCAL_PATH_EN,
  DEFAULT_MANUAL_HUB_INDEX_LOCAL_PATH_KO,
} from '@/config/constants';

const STORAGE_KEY = 'manual-hub-docs-v2';

export type ManualHubLocale = 'ko' | 'en';

/** Site 인덱스 테이블 한 행 (main_kr_link.html 스타일과 호환) */
export type IndexTableRow = {
  id: string;
  endpointId: string;
  /** Details 열 링크 텍스트 */
  detailLabel: string;
  /** Details 열 href (Zendesk 등) */
  detailHref: string;
};

export type ManualHubDocEntry = {
  id: string;
  label: string;
  /** Repo-relative path e.g. api_docs_zendesk/manual_list_ko.html */
  relativePath: string;
  /** Absolute path on disk (optional; empty = use Settings default for index KO/EN) */
  localFilePath: string;
  zendeskUrl: string;
  /** Local HTML draft for Zendesk body */
  htmlContent: string;
  /** Article title when publishing */
  title: string;
  /** Set for Site index rows */
  locale?: ManualHubLocale;
  /** Site 인덱스: 드래그로 구성한 테이블 행 (HTML 반영은 별도 버튼) */
  indexTableRows?: IndexTableRow[];
};

export type ManualHubStoredState = {
  indexKo: ManualHubDocEntry;
  indexEn: ManualHubDocEntry;
  staticPages: ManualHubDocEntry[];
};

const defaultIndexKo = (): ManualHubDocEntry => ({
  id: 'site-index-ko',
  label: 'Index (KO)',
  locale: 'ko',
  relativePath: 'api_docs_zendesk/manual_list_ko.html',
  localFilePath: '',
  zendeskUrl: '',
  htmlContent: '',
  title: 'API Documentation Index (KO)',
  indexTableRows: [],
});

const defaultIndexEn = (): ManualHubDocEntry => ({
  id: 'site-index-en',
  label: 'Index (EN)',
  locale: 'en',
  relativePath: 'api_docs_zendesk/manual_list_en.html',
  localFilePath: '',
  zendeskUrl: '',
  htmlContent: '',
  title: 'API Documentation Index (EN)',
  indexTableRows: [],
});

function migrateFromV1(parsed: Record<string, unknown>): ManualHubStoredState | null {
  const legacyIndex = parsed.index as ManualHubDocEntry | undefined;
  if (!legacyIndex) return null;
  return {
    indexKo: {
      ...defaultIndexKo(),
      ...legacyIndex,
      id: 'site-index-ko',
      label: 'Index (KO)',
      locale: 'ko',
      relativePath: legacyIndex.relativePath || defaultIndexKo().relativePath,
      localFilePath: typeof legacyIndex.localFilePath === 'string' ? legacyIndex.localFilePath : '',
      indexTableRows: [],
    },
    indexEn: defaultIndexEn(),
    staticPages: Array.isArray(parsed.staticPages) ? (parsed.staticPages as ManualHubDocEntry[]) : [],
  };
}

export function loadManualHubState(): ManualHubStoredState {
  try {
    const raw = localStorage.getItem(STORAGE_KEY);
    if (!raw) {
      try {
        const legacyRaw = localStorage.getItem('manual-hub-docs-v1');
        if (legacyRaw) {
          const legacyParsed = JSON.parse(legacyRaw) as Record<string, unknown>;
          const migrated = migrateFromV1(legacyParsed);
          if (migrated) {
            saveManualHubState(migrated);
            localStorage.removeItem('manual-hub-docs-v1');
            return migrated;
          }
        }
      } catch {
        // ignore legacy migration errors
      }
      return { indexKo: defaultIndexKo(), indexEn: defaultIndexEn(), staticPages: [] };
    }
    const parsed = JSON.parse(raw) as Partial<ManualHubStoredState>;

    const indexKo = {
      ...defaultIndexKo(),
      ...parsed.indexKo,
      id: 'site-index-ko',
      locale: 'ko' as const,
      localFilePath: typeof parsed.indexKo?.localFilePath === 'string' ? parsed.indexKo.localFilePath : '',
      indexTableRows: Array.isArray(parsed.indexKo?.indexTableRows)
        ? parsed.indexKo!.indexTableRows!
        : [],
    };
    const indexEn = {
      ...defaultIndexEn(),
      ...parsed.indexEn,
      id: 'site-index-en',
      locale: 'en' as const,
      localFilePath: typeof parsed.indexEn?.localFilePath === 'string' ? parsed.indexEn.localFilePath : '',
      indexTableRows: Array.isArray(parsed.indexEn?.indexTableRows)
        ? parsed.indexEn!.indexTableRows!
        : [],
    };

    const staticPages = Array.isArray(parsed.staticPages)
      ? parsed.staticPages.map((p) => ({
          id: typeof p.id === 'string' && p.id ? p.id : crypto.randomUUID(),
          label: typeof p.label === 'string' ? p.label : 'Untitled page',
          relativePath: typeof p.relativePath === 'string' ? p.relativePath : '',
          localFilePath: typeof p.localFilePath === 'string' ? p.localFilePath : '',
          zendeskUrl: typeof p.zendeskUrl === 'string' ? p.zendeskUrl : '',
          htmlContent: typeof p.htmlContent === 'string' ? p.htmlContent : '',
          title: typeof p.title === 'string' ? p.title : 'Untitled',
        }))
      : [];

    return { indexKo, indexEn, staticPages };
  } catch {
    return { indexKo: defaultIndexKo(), indexEn: defaultIndexEn(), staticPages: [] };
  }
}

export function saveManualHubState(state: ManualHubStoredState): void {
  try {
    localStorage.setItem(STORAGE_KEY, JSON.stringify(state));
  } catch {
    // ignore quota errors
  }
}

export function createEmptyStaticPage(): ManualHubDocEntry {
  return {
    id: crypto.randomUUID(),
    label: 'New page',
    relativePath: 'api_docs_zendesk/Design/new_page_kr_link.html',
    localFilePath: '',
    zendeskUrl: '',
    htmlContent: '<p></p>',
    title: 'New page',
  };
}

/** Effective local path for index rows when `localFilePath` is empty */
export function resolveIndexLocalFilePath(
  locale: ManualHubLocale,
  docLocalPath: string,
  settingsKo: string | undefined,
  settingsEn: string | undefined
): string {
  const trimmed = docLocalPath.trim();
  if (trimmed) return trimmed;
  if (locale === 'ko') {
    return (settingsKo?.trim() || DEFAULT_MANUAL_HUB_INDEX_LOCAL_PATH_KO).trim();
  }
  return (settingsEn?.trim() || DEFAULT_MANUAL_HUB_INDEX_LOCAL_PATH_EN).trim();
}
