import { useCallback, useEffect, useMemo, useRef, useState } from 'react';
import { Button } from '@/components/ui/button';
import { Input } from '@/components/ui/input';
import { Label } from '@/components/ui/label';
import { ScrollArea } from '@/components/ui/scroll-area';
import { Tabs, TabsList, TabsTrigger } from '@/components/ui/tabs';
import {
  FileDown,
  FileUp,
  FolderOpen,
  GitCompare,
  Loader2,
  Plus,
  RefreshCw,
  Send,
  Trash2,
  ExternalLink,
  Columns2,
  Link2,
  GripVertical,
  Table2,
} from 'lucide-react';
import Editor from '@monaco-editor/react';
import { toast } from 'sonner';
import { apiClient } from '@/lib/api-client';
import type { ApiEndpoint, ApiProduct, Settings, Version } from '@/types';
import {
  createEmptyStaticPage,
  loadManualHubState,
  resolveIndexLocalFilePath,
  saveManualHubState,
  type IndexTableRow,
  type ManualHubDocEntry,
  type ManualHubLocale,
  type ManualHubStoredState,
} from './manualHubStorage';
import {
  fetchZendeskArticleBody,
  loadZendeskEnvStatus,
  normalizeZendeskLocale,
  publishZendeskHtml,
  type ZendeskEnvStatus,
} from './zendeskHubApi';
import { ZendeskDiffPanel } from './ZendeskDiffPanel';
import {
  DEFAULT_MANUAL_HUB_INDEX_LOCAL_PATH_EN,
  DEFAULT_MANUAL_HUB_INDEX_LOCAL_PATH_KO,
} from '@/config/constants';
import { readTextFileForManualHub } from './loadLocalFile';
import {
  DndContext,
  DragEndEvent,
  KeyboardSensor,
  PointerSensor,
  useDraggable,
  useSensor,
  useSensors,
  closestCorners,
  pointerWithin,
  type CollisionDetection,
} from '@dnd-kit/core';
import { arrayMove, sortableKeyboardCoordinates } from '@dnd-kit/sortable';
import { IndexHtmlPreviewDropZone } from './IndexHtmlPreviewDropZone';
import { IndexTableEditor } from './IndexTableEditor';
import { buildManagedIndexTableHtml, syncManagedIndexTableIntoHtml } from './indexTableHtml';
import { applyEndpointToRowsAt, parseInsertDropTarget } from './indexTableDnd';
import { HtmlInsertionAssistant } from './HtmlInsertionAssistant';
import { LocalHtmlWorkspace } from './LocalHtmlWorkspace';
import {
  buildHtmlInsertEndpointPayload,
  HTML_INSERT_ENDPOINT_DRAG_MIME,
} from './htmlInsertHelpers';

const indexHubCollisionDetection: CollisionDetection = (args) => {
  const pointer = pointerWithin(args);
  if (pointer.length > 0) return pointer;
  return closestCorners(args);
};

type HubSelection =
  | { kind: 'index'; locale: ManualHubLocale }
  | { kind: 'static'; id: string }
  | { kind: 'endpoint'; endpointId: string };

type ZendeskDocSnapshot = {
  body: string;
  title: string | null;
  updatedAt: string | null;
  articleId: string;
};

type FlatEndpointRow = Record<string, unknown>;

function mapFlatRowToEndpoint(row: FlatEndpointRow): ApiEndpoint {
  return {
    id: String(row.id ?? ''),
    name: String(row.name ?? ''),
    method: String(row.method ?? ''),
    path: String(row.path ?? ''),
    status: (row.status as ApiEndpoint['status']) ?? undefined,
    order_index: typeof row.order_index === 'number' ? row.order_index : undefined,
  };
}

type GroupedEndpoints = { productId: string; productName: string; endpoints: ApiEndpoint[] };

function groupEndpointsFromFlatRows(
  rows: FlatEndpointRow[],
  productNameById: Map<string, string>
): GroupedEndpoints[] {
  const byProduct = new Map<string, ApiEndpoint[]>();
  for (const r of rows) {
    const pid = String(r.product ?? '');
    if (!pid) continue;
    const ep = mapFlatRowToEndpoint(r);
    if (!ep.id) continue;
    if (!byProduct.has(pid)) byProduct.set(pid, []);
    byProduct.get(pid)!.push(ep);
  }
  return [...byProduct.entries()]
    .map(([productId, endpoints]) => ({
      productId,
      productName: productNameById.get(productId) ?? `Product ${productId.slice(0, 8)}…`,
      endpoints: [...endpoints].sort((a, b) => (a.name || '').localeCompare(b.name || '')),
    }))
    .sort((a, b) => a.productName.localeCompare(b.productName));
}

function buildLatestManualByEndpoint(
  versions: Version[],
  endpoints: ApiEndpoint[]
): Map<string, { url?: string; versionId: string; versionLabel: string }> {
  const byEndpoint = new Map<string, Version[]>();
  for (const v of versions) {
    const list = byEndpoint.get(v.endpointId) ?? [];
    list.push(v);
    byEndpoint.set(v.endpointId, list);
  }
  const out = new Map<string, { url?: string; versionId: string; versionLabel: string }>();
  for (const ep of endpoints) {
    const list = byEndpoint.get(ep.id);
    if (!list?.length) continue;
    const sorted = [...list].sort(
      (a, b) => new Date(b.updatedAt).getTime() - new Date(a.updatedAt).getTime()
    );
    const latest = sorted[0];
    const url = latest.manualData?.url?.trim();
    out.set(ep.id, {
      url: url || undefined,
      versionId: latest.id,
      versionLabel: latest.version,
    });
  }
  return out;
}

type ManualHubViewProps = {
  products: ApiProduct[];
  settings: Settings;
  onNavigateToProjectEndpoint: (endpoint: ApiEndpoint) => void;
};

function hubDocStorageKey(selection: HubSelection): string | null {
  if (selection.kind === 'index') return `index:${selection.locale}`;
  if (selection.kind === 'static') return `static:${selection.id}`;
  return null;
}

function EndpointDragHandle({ endpointId, disabled }: { endpointId: string; disabled: boolean }) {
  const { attributes, listeners, setNodeRef } = useDraggable({
    id: `mh-ep:${endpointId}`,
    disabled,
  });
  if (disabled) {
    return <span className="w-4 shrink-0 inline-block" aria-hidden />;
  }
  return (
    <button
      type="button"
      ref={setNodeRef}
      {...listeners}
      {...attributes}
      className="cursor-grab active:cursor-grabbing shrink-0 rounded p-0.5 !text-[10px] !font-normal text-zinc-500 hover:bg-zinc-800"
      aria-label="Index 테이블로 드래그"
      onClick={(e) => e.stopPropagation()}
    >
      <GripVertical className="h-2.5 w-2.5" />
    </button>
  );
}

function EndpointHtmlInsertHandle(props: {
  endpoint: ApiEndpoint;
  detailHref: string;
  disabled: boolean;
}) {
  const { endpoint, detailHref, disabled } = props;
  if (disabled) {
    return <span className="w-4 shrink-0 inline-block" aria-hidden />;
  }

  return (
    <button
      type="button"
      draggable
      className="mt-0.5 shrink-0 rounded p-0.5 text-zinc-500 hover:bg-zinc-800 hover:text-emerald-300"
      aria-label="HTML 에디터로 드래그 삽입"
      title="HTML 에디터로 드래그 삽입"
      onClick={(e) => e.stopPropagation()}
      onDragStart={(e) => {
        const payload = buildHtmlInsertEndpointPayload(
          endpoint,
          endpoint.name || endpoint.path || endpoint.id,
          detailHref
        );
        e.dataTransfer.effectAllowed = 'copy';
        e.dataTransfer.setData(HTML_INSERT_ENDPOINT_DRAG_MIME, JSON.stringify(payload));
        e.dataTransfer.setData('text/plain', endpoint.path || endpoint.name || endpoint.id);
      }}
    >
      <Send className="h-3 w-3" />
    </button>
  );
}

export function ManualHubView({ products, settings, onNavigateToProjectEndpoint }: ManualHubViewProps) {
  const [hub, setHub] = useState<ManualHubStoredState>(() => loadManualHubState());
  const [selection, setSelection] = useState<HubSelection>({ kind: 'index', locale: 'ko' });
  const [versions, setVersions] = useState<Version[]>([]);
  const [versionsLoading, setVersionsLoading] = useState(false);
  const [zendeskEnv, setZendeskEnv] = useState<ZendeskEnvStatus | null>(null);
  const [viewMode, setViewMode] = useState<'preview' | 'code' | 'workspace' | 'diff' | 'split' | 'indexTable' | 'insert'>('preview');
  const [isFetchingZendesk, setIsFetchingZendesk] = useState(false);
  const [isSendingZendesk, setIsSendingZendesk] = useState(false);
  const fileInputRef = useRef<HTMLInputElement>(null);
  const [zendeskSnapshots, setZendeskSnapshots] = useState<Record<string, ZendeskDocSnapshot>>({});
  const [groupedEndpoints, setGroupedEndpoints] = useState<GroupedEndpoints[]>([]);
  const [endpointsFlatLoading, setEndpointsFlatLoading] = useState(false);
  const [selectedInsertEndpointId, setSelectedInsertEndpointId] = useState<string | null>(null);
  const [isApplyingDefaultPath, setIsApplyingDefaultPath] = useState(false);
  /** iframe srcDoc 갱신 강제용 (동일 내용 재로드 시에도 반영) */
  const [localHtmlRenderKey, setLocalHtmlRenderKey] = useState(0);
  /** 인덱스 문서에서 엔드포인트를 미리보기 iframe 위로 드롭할 때 오버레이 활성화 */
  const [isDraggingEpForIndex, setIsDraggingEpForIndex] = useState(false);
  const productNameById = useMemo(() => {
    const m = new Map<string, string>();
    for (const p of products) {
      m.set(p.id, p.name);
    }
    return m;
  }, [products]);

  const endpointsFlat = useMemo(
    () => groupedEndpoints.flatMap((g) => g.endpoints),
    [groupedEndpoints]
  );

  const endpointsById = useMemo(() => {
    const m = new Map<string, ApiEndpoint>();
    for (const e of endpointsFlat) {
      m.set(e.id, e);
    }
    return m;
  }, [endpointsFlat]);

  const endpointMeta = useMemo(
    () => buildLatestManualByEndpoint(versions, endpointsFlat),
    [versions, endpointsFlat]
  );

  const selectedInsertEndpoint = useMemo(
    () => (selectedInsertEndpointId ? endpointsById.get(selectedInsertEndpointId) ?? null : null),
    [endpointsById, selectedInsertEndpointId]
  );

  const persist = useCallback((next: ManualHubStoredState) => {
    setHub(next);
    saveManualHubState(next);
  }, []);

  const sensors = useSensors(
    useSensor(PointerSensor, { activationConstraint: { distance: 6 } }),
    useSensor(KeyboardSensor, { coordinateGetter: sortableKeyboardCoordinates })
  );

  const handleDndDragEnd = useCallback(
    (event: DragEndEvent) => {
      const { active, over } = event;
      if (!over) return;
      if (selection.kind !== 'index') return;

      const activeId = String(active.id);
      const overId = String(over.id);
      const docKey = selection.locale === 'ko' ? 'indexKo' : 'indexEn';
      const currentRows = hub[docKey].indexTableRows ?? [];

      if (activeId.startsWith('mh-ep:')) {
        const parsed = parseInsertDropTarget(overId);
        if (!parsed) return;
        const endpointId = activeId.slice('mh-ep:'.length);
        const ep = endpointsFlat.find((e) => e.id === endpointId);
        if (!ep) return;
        const existed = currentRows.some((r) => r.endpointId === endpointId);
        const meta = endpointMeta.get(endpointId);
        const newRow: IndexTableRow = {
          id: crypto.randomUUID(),
          endpointId,
          detailLabel: ep.name,
          detailHref: meta?.url || '',
        };
        const insertIndex = Math.max(0, Math.min(parsed.index, currentRows.length));
        const indexTableRows = applyEndpointToRowsAt(currentRows, endpointId, insertIndex, newRow);
        const next: ManualHubStoredState = {
          ...hub,
          [docKey]: {
            ...hub[docKey],
            indexTableRows,
          },
        };
        persist(next);
        toast.success(existed ? '행 위치를 변경했습니다.' : 'Index 테이블에 행을 추가했습니다.');
        return;
      }

      if (activeId.startsWith('mh-row:') && overId.startsWith('mh-row:')) {
        const oldIndex = currentRows.findIndex((r) => `mh-row:${r.id}` === activeId);
        const newIndex = currentRows.findIndex((r) => `mh-row:${r.id}` === overId);
        if (oldIndex < 0 || newIndex < 0 || oldIndex === newIndex) return;
        const next: ManualHubStoredState = {
          ...hub,
          [docKey]: {
            ...hub[docKey],
            indexTableRows: arrayMove(currentRows, oldIndex, newIndex),
          },
        };
        persist(next);
      }
    },
    [selection, hub, endpointsFlat, endpointMeta, persist]
  );

  const refreshVersions = useCallback(async () => {
    setVersionsLoading(true);
    try {
      const response = await apiClient.getVersions();
      if (response.error) {
        toast.error(response.error);
        return;
      }
      setVersions(response.data ?? []);
    } catch {
      toast.error('버전 목록을 불러오지 못했습니다.');
    } finally {
      setVersionsLoading(false);
    }
  }, []);

  useEffect(() => {
    void refreshVersions();
  }, [refreshVersions]);

  const refreshFlatEndpoints = useCallback(async () => {
    setEndpointsFlatLoading(true);
    try {
      const response = await apiClient.getEndpoints();
      if (response.error) {
        toast.error(response.error);
        return;
      }
      const rows = (response.data ?? []) as FlatEndpointRow[];
      setGroupedEndpoints(groupEndpointsFromFlatRows(rows, productNameById));
    } catch {
      toast.error('엔드포인트 목록(평면)을 불러오지 못했습니다.');
    } finally {
      setEndpointsFlatLoading(false);
    }
  }, [productNameById]);

  useEffect(() => {
    void refreshFlatEndpoints();
  }, [refreshFlatEndpoints]);

  useEffect(() => {
    if (selection.kind !== 'index' && viewMode === 'indexTable') {
      setViewMode('preview');
    }
  }, [selection.kind, viewMode]);

  useEffect(() => {
    void (async () => {
      const env = await loadZendeskEnvStatus();
      setZendeskEnv(env);
    })();
  }, []);

  const currentDoc: ManualHubDocEntry | null = useMemo(() => {
    if (selection.kind === 'index') {
      return selection.locale === 'ko' ? hub.indexKo : hub.indexEn;
    }
    if (selection.kind === 'static') {
      return hub.staticPages.find((p) => p.id === selection.id) ?? null;
    }
    return null;
  }, [hub, selection]);

  const effectiveIndexLocalPath = useMemo(() => {
    if (selection.kind !== 'index' || !currentDoc) return '';
    return resolveIndexLocalFilePath(
      selection.locale,
      currentDoc.localFilePath,
      settings.manualHubIndexLocalPathKo,
      settings.manualHubIndexLocalPathEn
    );
  }, [selection, currentDoc, settings]);

  const activeDocKey = hubDocStorageKey(selection);
  const zendeskSnap = activeDocKey ? zendeskSnapshots[activeDocKey] : undefined;

  const updateCurrentDoc = useCallback(
    (patch: Partial<ManualHubDocEntry>) => {
      if (selection.kind === 'index') {
        if (selection.locale === 'ko') {
          persist({ ...hub, indexKo: { ...hub.indexKo, ...patch } });
        } else {
          persist({ ...hub, indexEn: { ...hub.indexEn, ...patch } });
        }
        return;
      }
      if (selection.kind === 'static') {
        persist({
          ...hub,
          staticPages: hub.staticPages.map((p) =>
            p.id === selection.id ? { ...p, ...patch } : p
          ),
        });
      }
    },
    [hub, persist, selection]
  );

  const handleAddStaticPage = () => {
    const page = createEmptyStaticPage();
    persist({ ...hub, staticPages: [...hub.staticPages, page] });
    setSelection({ kind: 'static', id: page.id });
  };

  const handleRemoveStaticPage = (id: string) => {
    if (!window.confirm('이 정적 페이지 항목을 삭제할까요?')) return;
    persist({ ...hub, staticPages: hub.staticPages.filter((p) => p.id !== id) });
    setSelection({ kind: 'index', locale: 'ko' });
  };

  const applySettingsDefaultLocalPath = async () => {
    if (selection.kind !== 'index') return;
    const path =
      selection.locale === 'ko'
        ? settings.manualHubIndexLocalPathKo?.trim() || DEFAULT_MANUAL_HUB_INDEX_LOCAL_PATH_KO
        : settings.manualHubIndexLocalPathEn?.trim() || DEFAULT_MANUAL_HUB_INDEX_LOCAL_PATH_EN;

    setIsApplyingDefaultPath(true);
    try {
      const fileResult = await readTextFileForManualHub(path);
      if (fileResult.ok) {
        updateCurrentDoc({
          localFilePath: path,
          htmlContent: fileResult.content,
        });
        setLocalHtmlRenderKey((k) => k + 1);
        setViewMode('preview');
        const via =
          fileResult.source === 'http'
            ? '로컬 API 서버(`npm run server` / dev:all)'
            : 'Electron';
        toast.success(`기본 경로를 적용하고 파일을 불러왔습니다. (${via})`);
        return;
      }
      updateCurrentDoc({ localFilePath: path });
      toast.error(
        `파일을 읽지 못했습니다: ${fileResult.message}. 워크스페이스 루트 이하 경로인지, API 서버(9527)가 떠 있는지 확인하세요.`
      );
    } finally {
      setIsApplyingDefaultPath(false);
    }
  };

  const handleImportHtmlFile = (file: File | null) => {
    if (!file) return;
    const reader = new FileReader();
    reader.onload = () => {
      const text = typeof reader.result === 'string' ? reader.result : '';
      updateCurrentDoc({ htmlContent: text });
      setLocalHtmlRenderKey((k) => k + 1);
      setViewMode('preview');
      toast.success('HTML을 불러왔습니다.');
    };
    reader.readAsText(file);
  };

  const handleExportHtml = () => {
    if (!currentDoc) return;
    const blob = new Blob([currentDoc.htmlContent || '<p></p>'], { type: 'text/html' });
    const url = URL.createObjectURL(blob);
    const a = document.createElement('a');
    a.href = url;
    a.download = `${currentDoc.relativePath.replace(/\//g, '_') || 'page'}_${Date.now()}.html`;
    document.body.appendChild(a);
    a.click();
    document.body.removeChild(a);
    URL.revokeObjectURL(url);
  };

  const handleFetchZendesk = async () => {
    if (!currentDoc) return;
    const key = hubDocStorageKey(selection);
    if (!key) return;
    const target = currentDoc.zendeskUrl.trim();
    if (!target) {
      toast.error('Zendesk URL 또는 Article ID를 입력하세요.');
      return;
    }
    const env = zendeskEnv ?? (await loadZendeskEnvStatus());
    if (!env?.hasCredentials) {
      toast.error('.env에 Zendesk 설정이 필요합니다.');
      return;
    }
    setIsFetchingZendesk(true);
    try {
      const data = await fetchZendeskArticleBody(target, env.defaultLocale);
      if (!data) return;
      setZendeskSnapshots((prev) => ({
        ...prev,
        [key]: {
          body: data.body,
          title: data.title || null,
          updatedAt: data.updatedAt || null,
          articleId: data.articleId,
        },
      }));
      toast.success(`Zendesk 본문을 가져왔습니다 (${data.articleId}). 로컬 HTML은 변경하지 않았습니다. Zendesk compare/Diff에서 비교하세요.`);
      setViewMode('split');
    } finally {
      setIsFetchingZendesk(false);
    }
  };

  const handleSendZendesk = async () => {
    if (!currentDoc) return;
    const env = zendeskEnv ?? (await loadZendeskEnvStatus());
    if (!env?.hasCredentials) {
      toast.error('.env에 Zendesk 설정이 필요합니다.');
      return;
    }
    const locale = normalizeZendeskLocale(env.defaultLocale);
    const html = currentDoc.htmlContent || '<p></p>';
    const title = currentDoc.title.trim() || 'Untitled';
    setIsSendingZendesk(true);
    try {
      const result = await publishZendeskHtml({
        targetInput: currentDoc.zendeskUrl.trim(),
        locale,
        html,
        title,
      });
      if (!result.success) {
        toast.error(`Zendesk 전송 실패: ${result.error}`);
        return;
      }
      if (result.articleUrl) {
        updateCurrentDoc({ zendeskUrl: result.articleUrl });
      }
      toast.success(
        `Zendesk ${result.mode === 'create' ? '생성' : '업데이트'} 완료 (${result.articleId ?? ''})`
      );
    } finally {
      setIsSendingZendesk(false);
    }
  };

  const zendeskReady = !!zendeskEnv?.hasCredentials;

  const handleIndexRowPatch = useCallback(
    (rowId: string, patch: Partial<Pick<IndexTableRow, 'detailLabel' | 'detailHref'>>) => {
      if (selection.kind !== 'index') return;
      const docKey = selection.locale === 'ko' ? 'indexKo' : 'indexEn';
      const rows = [...(hub[docKey].indexTableRows ?? [])];
      const i = rows.findIndex((r) => r.id === rowId);
      if (i < 0) return;
      rows[i] = { ...rows[i], ...patch };
      persist({ ...hub, [docKey]: { ...hub[docKey], indexTableRows: rows } });
    },
    [selection, hub, persist]
  );

  const handleRemoveIndexRow = useCallback(
    (rowId: string) => {
      if (selection.kind !== 'index') return;
      const docKey = selection.locale === 'ko' ? 'indexKo' : 'indexEn';
      const rows = (hub[docKey].indexTableRows ?? []).filter((r) => r.id !== rowId);
      persist({ ...hub, [docKey]: { ...hub[docKey], indexTableRows: rows } });
    },
    [selection, hub, persist]
  );

  const handleApplyIndexTableToHtml = useCallback(() => {
    if (selection.kind !== 'index') return;
    const docKey = selection.locale === 'ko' ? 'indexKo' : 'indexEn';
    const doc = hub[docKey];
    const rows = doc.indexTableRows ?? [];
    const fragment = buildManagedIndexTableHtml(rows, endpointsById);
    const nextHtml = syncManagedIndexTableIntoHtml(doc.htmlContent || '', fragment);
    persist({
      ...hub,
      [docKey]: { ...doc, htmlContent: nextHtml },
    });
    setLocalHtmlRenderKey((k) => k + 1);
    toast.success('로컬 HTML에 MH_INDEX_TABLE 마커 블록을 반영했습니다. Local preview에서 확인하세요.');
  }, [selection, hub, endpointsById, persist]);

  const handleApplyInsertedHtml = useCallback(
    (nextHtml: string) => {
      updateCurrentDoc({ htmlContent: nextHtml });
      setLocalHtmlRenderKey((k) => k + 1);
    },
    [updateCurrentDoc]
  );

  const handleDndDragEndWithReset = useCallback(
    (event: DragEndEvent) => {
      setIsDraggingEpForIndex(false);
      handleDndDragEnd(event);
    },
    [handleDndDragEnd]
  );

  return (
    <DndContext
      sensors={sensors}
      collisionDetection={indexHubCollisionDetection}
      onDragStart={(e) => {
        if (String(e.active.id).startsWith('mh-ep:')) setIsDraggingEpForIndex(true);
      }}
      onDragEnd={handleDndDragEndWithReset}
      onDragCancel={() => setIsDraggingEpForIndex(false)}
    >
    <div className="flex h-full w-full min-h-0 bg-zinc-950 text-zinc-100">
      <div className="flex w-64 min-w-0 flex-shrink-0 flex-col border-r border-zinc-800 bg-zinc-900 min-h-0">
        <div className="flex-shrink-0 border-b border-zinc-800 p-2.5">
          <h2 className="text-sm font-semibold text-white">Manual Hub</h2>
          <p className="mt-1 text-[10px] leading-snug text-zinc-500">
            인덱스, 정적 HTML, 엔드포인트별 Zendesk 링크를 한곳에서 봅니다.
          </p>
        </div>
        <div className="flex min-h-0 flex-1 flex-col">
          <div className="flex-shrink-0 space-y-2 px-2 pt-2">
            <div>
              <div className="mb-1 px-1 text-[10px] uppercase tracking-wider text-zinc-500">Site</div>
              <div className="space-y-0.5">
                <button
                  type="button"
                  onClick={() => setSelection({ kind: 'index', locale: 'ko' })}
                  className={`flex w-full items-center gap-2 rounded px-2 py-1 text-left text-xs ${
                    selection.kind === 'index' && selection.locale === 'ko'
                      ? 'bg-blue-600 text-white'
                      : 'text-zinc-300 hover:bg-zinc-800'
                  }`}
                >
                  <FolderOpen className="h-3.5 w-3.5 flex-shrink-0" />
                  <span className="truncate">{hub.indexKo.label}</span>
                </button>
                <button
                  type="button"
                  onClick={() => setSelection({ kind: 'index', locale: 'en' })}
                  className={`flex w-full items-center gap-2 rounded px-2 py-1 text-left text-xs ${
                    selection.kind === 'index' && selection.locale === 'en'
                      ? 'bg-blue-600 text-white'
                      : 'text-zinc-300 hover:bg-zinc-800'
                  }`}
                >
                  <FolderOpen className="h-3.5 w-3.5 flex-shrink-0" />
                  <span className="truncate">{hub.indexEn.label}</span>
                </button>
              </div>
            </div>

            <div>
              <div className="mb-1 flex items-center justify-between px-1">
                <span className="text-[10px] uppercase tracking-wider text-zinc-500">Static pages</span>
                <Button
                  type="button"
                  variant="ghost"
                  size="sm"
                  className="h-6 px-1.5 text-zinc-400"
                  onClick={handleAddStaticPage}
                  title="Add page"
                >
                  <Plus className="h-3.5 w-3.5" />
                </Button>
              </div>
              <div className="max-h-28 space-y-0.5 overflow-y-auto pr-0.5">
                {hub.staticPages.map((p) => (
                  <div key={p.id} className="flex items-center gap-0.5">
                    <button
                      type="button"
                      onClick={() => setSelection({ kind: 'static', id: p.id })}
                      className={`min-w-0 flex-1 truncate rounded px-2 py-1 text-left text-xs ${
                        selection.kind === 'static' && selection.id === p.id
                          ? 'bg-blue-600 text-white'
                          : 'text-zinc-300 hover:bg-zinc-800'
                      }`}
                    >
                      {p.label}
                    </button>
                    <Button
                      type="button"
                      variant="ghost"
                      size="sm"
                      className="h-6 w-6 p-0 text-zinc-500 hover:text-red-400"
                      onClick={() => handleRemoveStaticPage(p.id)}
                      title="Delete"
                    >
                      <Trash2 className="h-3 w-3" />
                    </Button>
                  </div>
                ))}
              </div>
            </div>
          </div>

          <div className="flex min-h-0 flex-1 flex-col border-t border-zinc-800">
            <div className="flex flex-shrink-0 items-center justify-between px-2 pb-2 pt-2">
              <span className="text-[10px] uppercase tracking-wider text-zinc-500">Endpoints</span>
              <div className="flex items-center gap-0.5">
                <Button
                  type="button"
                  variant="ghost"
                  size="sm"
                  className="h-6 px-1.5 text-zinc-400"
                  onClick={() => void refreshFlatEndpoints()}
                  disabled={endpointsFlatLoading}
                  title="DB에서 엔드포인트 목록 다시 로드 (/api/endpoints)"
                >
                  {endpointsFlatLoading ? (
                    <Loader2 className="h-3.5 w-3.5 animate-spin" />
                  ) : (
                    <RefreshCw className="h-3.5 w-3.5" />
                  )}
                </Button>
                <Button
                  type="button"
                  variant="ghost"
                  size="sm"
                  className="h-6 px-1.5 text-zinc-400"
                  onClick={() => void refreshVersions()}
                  disabled={versionsLoading}
                  title="버전 기준 Zendesk 링크 갱신"
                >
                  {versionsLoading ? (
                    <Loader2 className="h-3.5 w-3.5 animate-spin" />
                  ) : (
                    <Link2 className="h-3.5 w-3.5" />
                  )}
                </Button>
              </div>
            </div>
            <div className="flex-shrink-0 px-2 pb-1 text-[10px] text-zinc-600">DB `/api/endpoints` 평면</div>
            <ScrollArea className="min-h-0 flex-1">
              <div className="space-y-1.5 px-2 pb-3 pr-1">
                {groupedEndpoints.map((group) => (
                  <div key={group.productId}>
                    <div className="sticky top-0 z-10 bg-zinc-900/95 px-1 py-0.5 !text-[9px] font-normal text-zinc-500">
                      {group.productName}
                      <span className="ml-1 text-zinc-600">({group.endpoints.length})</span>
                    </div>
                    <div className="space-y-0">
                      {group.endpoints.map((ep) => (
                        <div
                          key={ep.id}
                          className={`flex items-start gap-0 rounded leading-tight ${
                            selection.kind === 'endpoint' && selection.endpointId === ep.id
                              ? 'bg-blue-600 text-white'
                              : 'text-zinc-300 hover:bg-zinc-800'
                          }`}
                        >
                          <EndpointDragHandle
                            endpointId={ep.id}
                            disabled={selection.kind !== 'index'}
                          />
                          <button
                            type="button"
                            onClick={() => setSelection({ kind: 'endpoint', endpointId: ep.id })}
                            className="min-w-0 flex-1 rounded px-1 py-0.5 text-left !text-[10px] !font-normal leading-snug text-inherit"
                          >
                            <div className="truncate">{ep.name}</div>
                            <div className="truncate font-mono !text-[8px] leading-[1.1] opacity-80">
                              {ep.method} {ep.path}
                            </div>
                          </button>
                          {currentDoc && (
                            <EndpointHtmlInsertHandle
                              endpoint={ep}
                              detailHref={endpointMeta.get(ep.id)?.url ?? ''}
                              disabled={!currentDoc}
                            />
                          )}
                          {currentDoc && (
                            <Button
                              type="button"
                              variant="ghost"
                              size="sm"
                              className={`mt-0.5 h-5 w-5 p-0 ${
                                selectedInsertEndpointId === ep.id
                                  ? 'text-emerald-300 hover:text-emerald-200'
                                  : 'text-zinc-500 hover:text-emerald-300'
                              }`}
                              title="작업공간 삽입 대상으로 선택"
                              onClick={(e) => {
                                e.stopPropagation();
                                setSelectedInsertEndpointId(ep.id);
                                setViewMode('workspace');
                              }}
                            >
                              <Plus className="h-3 w-3" />
                            </Button>
                          )}
                        </div>
                      ))}
                    </div>
                  </div>
                ))}
              </div>
            </ScrollArea>
          </div>
        </div>
      </div>

      <div className="flex-1 flex flex-col min-w-0 min-h-0">
        {selection.kind === 'endpoint' ? (
          <EndpointDetailPanel
            endpoint={endpointsFlat.find((e) => e.id === selection.endpointId) ?? null}
            meta={endpointMeta.get(selection.endpointId)}
            onOpenInProjects={onNavigateToProjectEndpoint}
          />
        ) : currentDoc ? (
          <>
            <div className="border-b border-zinc-800 bg-zinc-900 px-4 py-3 space-y-3 flex-shrink-0">
              <div className="grid gap-3 md:grid-cols-2">
                <div>
                  <Label className="text-[10px] text-zinc-500">Title (Zendesk)</Label>
                  <Input
                    value={currentDoc.title}
                    onChange={(e) => updateCurrentDoc({ title: e.target.value })}
                    className="mt-1 h-8 text-xs bg-zinc-800 border-zinc-700"
                  />
                </div>
                <div>
                  <Label className="text-[10px] text-zinc-500">Relative path (repo)</Label>
                  <Input
                    value={currentDoc.relativePath}
                    onChange={(e) => updateCurrentDoc({ relativePath: e.target.value })}
                    className="mt-1 h-8 text-xs bg-zinc-800 border-zinc-700 font-mono"
                  />
                </div>
              </div>
              {selection.kind === 'index' && (
                <div className="space-y-1.5">
                  <Label className="text-[10px] text-zinc-500">로컬 파일 경로 (절대 경로)</Label>
                  <div className="flex flex-col gap-2 sm:flex-row sm:items-center sm:gap-2">
                    <Input
                      value={currentDoc.localFilePath}
                      onChange={(e) => updateCurrentDoc({ localFilePath: e.target.value })}
                      placeholder={
                        selection.locale === 'ko'
                          ? settings.manualHubIndexLocalPathKo?.trim() ||
                            DEFAULT_MANUAL_HUB_INDEX_LOCAL_PATH_KO
                          : settings.manualHubIndexLocalPathEn?.trim() ||
                            DEFAULT_MANUAL_HUB_INDEX_LOCAL_PATH_EN
                      }
                      className="h-8 text-xs bg-zinc-800 border-zinc-700 font-mono sm:flex-1 min-w-0"
                    />
                    <Button
                      type="button"
                      variant="outline"
                      size="sm"
                      disabled={isApplyingDefaultPath}
                      className="h-8 shrink-0 whitespace-nowrap px-3 text-xs border-amber-600/50 text-amber-300 sm:self-center inline-flex items-center justify-center"
                      onClick={() => void applySettingsDefaultLocalPath()}
                    >
                      {isApplyingDefaultPath ? (
                        <>
                          <Loader2 className="w-3 h-3 mr-1.5 animate-spin" />
                          불러오는 중
                        </>
                      ) : (
                        '설정 기본값 적용'
                      )}
                    </Button>
                  </div>
                  <p className="text-[10px] text-zinc-500">
                    비어 있으면 Settings의 Manual Hub 기본 경로가 사용됩니다. 실제 참조 경로:{' '}
                    <span className="text-zinc-300 break-all">{effectiveIndexLocalPath}</span>
                  </p>
                </div>
              )}
              <div>
                <Label className="text-[10px] text-zinc-500">Zendesk target (URL or article ID)</Label>
                <div className="flex flex-wrap gap-2 mt-1">
                  <Input
                    value={currentDoc.zendeskUrl}
                    onChange={(e) => updateCurrentDoc({ zendeskUrl: e.target.value })}
                    placeholder="비우면 기본 Section에 신규 생성"
                    className="h-8 text-xs bg-zinc-800 border-zinc-700 flex-1 min-w-[200px]"
                  />
                  <span
                    className={`text-[10px] self-center ${zendeskReady ? 'text-emerald-400' : 'text-amber-400'}`}
                  >
                    {zendeskReady ? 'Env OK' : 'Env missing'}
                  </span>
                  <Button
                    type="button"
                    size="sm"
                    variant="outline"
                    className="h-8 text-xs border-violet-600/60 text-violet-300"
                    disabled={isFetchingZendesk || !currentDoc.zendeskUrl.trim()}
                    onClick={() => void handleFetchZendesk()}
                  >
                    {isFetchingZendesk ? (
                      <Loader2 className="w-3 h-3 mr-1 animate-spin" />
                    ) : (
                      <GitCompare className="w-3 h-3 mr-1" />
                    )}
                    Fetch
                  </Button>
                  <Button
                    type="button"
                    size="sm"
                    className="h-8 text-xs bg-blue-600"
                    disabled={isSendingZendesk}
                    onClick={() => void handleSendZendesk()}
                  >
                    {isSendingZendesk ? (
                      <Loader2 className="w-3 h-3 mr-1 animate-spin" />
                    ) : (
                      <Send className="w-3 h-3 mr-1" />
                    )}
                    Send
                  </Button>
                </div>
                <p className="text-[10px] text-zinc-500 leading-snug">
                  Fetch는 Zendesk에 올라간 본문만 스냅샷으로 보관합니다. 로컬 HTML은 HTML 탭에서 수정한 뒤 Send로 생성·갱신할 수 있습니다.
                </p>
              </div>
              <div className="flex flex-wrap gap-2">
                <input
                  ref={fileInputRef}
                  type="file"
                  accept=".html,.htm,text/html"
                  className="hidden"
                  onChange={(e) => {
                    handleImportHtmlFile(e.target.files?.[0] ?? null);
                    e.target.value = '';
                  }}
                />
                <Button
                  type="button"
                  variant="outline"
                  size="sm"
                  className="h-7 text-xs"
                  onClick={() => fileInputRef.current?.click()}
                >
                  <FileUp className="w-3 h-3 mr-1" />
                  Import HTML
                </Button>
                <Button type="button" variant="outline" size="sm" className="h-7 text-xs" onClick={handleExportHtml}>
                  <FileDown className="w-3 h-3 mr-1" />
                  Export
                </Button>
                {selection.kind === 'static' && (
                  <Input
                    value={currentDoc.label}
                    onChange={(e) => updateCurrentDoc({ label: e.target.value })}
                    className="h-7 text-xs max-w-[200px] bg-zinc-800 border-zinc-700"
                    placeholder="Sidebar label"
                  />
                )}
              </div>
            </div>

            <div className="border-b border-zinc-800 px-4 py-2 flex items-center gap-2 bg-zinc-950 flex-shrink-0">
                <Tabs
                value={viewMode}
                onValueChange={(v) =>
                  setViewMode(v as 'preview' | 'code' | 'workspace' | 'diff' | 'split' | 'indexTable' | 'insert')
                }
              >
                <TabsList className="bg-zinc-800 h-8 p-1 flex flex-wrap h-auto gap-1">
                  <TabsTrigger value="preview" className="text-xs h-7 px-3">
                    Local preview
                  </TabsTrigger>
                  <TabsTrigger value="code" className="text-xs h-7 px-3">
                    Local HTML
                  </TabsTrigger>
                  <TabsTrigger value="workspace" className="text-xs h-7 px-3">
                    <Columns2 className="w-3 h-3 mr-1 inline" />
                    Workspace
                  </TabsTrigger>
                  <TabsTrigger value="insert" className="text-xs h-7 px-3">
                    HTML insert
                  </TabsTrigger>
                  {selection.kind === 'index' && (
                    <TabsTrigger value="indexTable" className="text-xs h-7 px-3">
                      <Table2 className="w-3 h-3 mr-1 inline" />
                      Index table
                    </TabsTrigger>
                  )}
                  <TabsTrigger value="split" className="text-xs h-7 px-3">
                    <Columns2 className="w-3 h-3 mr-1 inline" />
                    Zendesk compare
                  </TabsTrigger>
                  <TabsTrigger value="diff" className="text-xs h-7 px-3">
                    Diff
                  </TabsTrigger>
                </TabsList>
              </Tabs>
            </div>

            <div className="flex-1 min-h-0 flex flex-col">
              {viewMode === 'insert' && (
                <HtmlInsertionAssistant
                  htmlContent={currentDoc.htmlContent || ''}
                  endpoint={selectedInsertEndpoint}
                  endpointUrl={selectedInsertEndpoint ? endpointMeta.get(selectedInsertEndpoint.id)?.url : undefined}
                  onApplyHtml={handleApplyInsertedHtml}
                />
              )}
              {viewMode === 'indexTable' && selection.kind === 'index' && (
                <IndexTableEditor
                  rows={currentDoc.indexTableRows ?? []}
                  endpointsById={endpointsById}
                  endpointDragActive={isDraggingEpForIndex}
                  onRowChange={handleIndexRowPatch}
                  onRemoveRow={handleRemoveIndexRow}
                  onApplyToHtml={handleApplyIndexTableToHtml}
                />
              )}
              {viewMode === 'preview' &&
                (selection.kind === 'index' ? (
                  <IndexHtmlPreviewDropZone
                    dragActive={isDraggingEpForIndex}
                    rowCount={(currentDoc.indexTableRows ?? []).length}
                  >
                    <iframe
                      key={`local-preview-${localHtmlRenderKey}`}
                      title="manual-preview-local"
                      className="h-full min-h-0 w-full flex-1 border-0 bg-white"
                      srcDoc={currentDoc.htmlContent || '<p></p>'}
                    />
                  </IndexHtmlPreviewDropZone>
                ) : (
                  <iframe
                    key={`local-preview-${localHtmlRenderKey}`}
                    title="manual-preview-local"
                    className="w-full flex-1 min-h-0 border-0 bg-white"
                    srcDoc={currentDoc.htmlContent || '<p></p>'}
                  />
                ))}
              {viewMode === 'code' && (
                <div className="flex-1 min-h-0">
                  <Editor
                    height="100%"
                    defaultLanguage="html"
                    theme="vs-dark"
                    value={currentDoc.htmlContent}
                    onChange={(v) => updateCurrentDoc({ htmlContent: v ?? '' })}
                    options={{ minimap: { enabled: false }, wordWrap: 'on', fontSize: 12 }}
                  />
                </div>
              )}
              {viewMode === 'workspace' && (
                <LocalHtmlWorkspace
                  htmlContent={currentDoc.htmlContent || ''}
                  endpoint={selectedInsertEndpoint}
                  endpointUrl={selectedInsertEndpoint ? endpointMeta.get(selectedInsertEndpoint.id)?.url : undefined}
                  renderVersion={localHtmlRenderKey}
                  onHtmlChange={(nextHtml) => updateCurrentDoc({ htmlContent: nextHtml })}
                />
              )}
              {viewMode === 'split' && (
                <div className="flex flex-1 min-h-0">
                  <div className="flex-1 flex flex-col min-w-0 min-h-0 border-r border-zinc-800">
                    <div className="text-[10px] px-2 py-1 bg-zinc-900 text-zinc-400 border-b border-zinc-800 flex-shrink-0">
                      Zendesk (Fetch 스냅샷)
                    </div>
                    <iframe
                      title="zendesk-html"
                      className="flex-1 w-full min-h-0 border-0 bg-white"
                      srcDoc={
                        zendeskSnap?.body ??
                        '<p style="padding:12px;font-family:system-ui;color:#64748b">Fetch를 실행하면 여기에 Zendesk 본문이 표시됩니다.</p>'
                      }
                    />
                  </div>
                  <div className="flex-1 flex flex-col min-w-0 min-h-0">
                    <div className="text-[10px] px-2 py-1 bg-zinc-900 text-zinc-400 border-b border-zinc-800 flex-shrink-0">
                      Local (이 탭에서 편집은 Local HTML)
                    </div>
                    {selection.kind === 'index' ? (
                      <IndexHtmlPreviewDropZone
                        dragActive={isDraggingEpForIndex}
                        rowCount={(currentDoc.indexTableRows ?? []).length}
                      >
                        <iframe
                          key={`split-local-${localHtmlRenderKey}`}
                          title="local-html"
                          className="h-full min-h-0 w-full flex-1 border-0 bg-white"
                          srcDoc={currentDoc.htmlContent || '<p></p>'}
                        />
                      </IndexHtmlPreviewDropZone>
                    ) : (
                      <iframe
                        key={`split-local-${localHtmlRenderKey}`}
                        title="local-html"
                        className="flex-1 w-full min-h-0 border-0 bg-white"
                        srcDoc={currentDoc.htmlContent || '<p></p>'}
                      />
                    )}
                  </div>
                </div>
              )}
              {viewMode === 'diff' && (
                <ZendeskDiffPanel
                  localHtml={currentDoc.htmlContent || ''}
                  remoteHtml={zendeskSnap?.body ?? ''}
                  remoteTitle={zendeskSnap?.title ?? null}
                  remoteUpdatedAt={zendeskSnap?.updatedAt ?? null}
                  onUseLocal={() => void handleSendZendesk()}
                  onUseRemote={() => {
                    if (!zendeskSnap) {
                      toast.error('먼저 Fetch로 Zendesk 본문을 가져오세요.');
                      return;
                    }
                    updateCurrentDoc({
                      htmlContent: zendeskSnap.body,
                      ...(zendeskSnap.title?.trim() ? { title: zendeskSnap.title } : {}),
                    });
                    setViewMode('code');
                    toast.success('Zendesk 본문을 로컬 HTML에 반영했습니다.');
                  }}
                />
              )}
            </div>
          </>
        ) : (
          <div className="flex-1 flex items-center justify-center text-zinc-500 text-sm">Select an item</div>
        )}
      </div>
    </div>
    </DndContext>
  );
}

function EndpointDetailPanel(props: {
  endpoint: ApiEndpoint | null;
  meta?: { url?: string; versionId: string; versionLabel: string };
  onOpenInProjects: (endpoint: ApiEndpoint) => void;
}) {
  const { endpoint, meta, onOpenInProjects } = props;
  if (!endpoint) {
    return (
      <div className="flex-1 flex items-center justify-center text-zinc-500 text-sm">엔드포인트를 찾을 수 없습니다.</div>
    );
  }

  return (
    <div className="flex-1 flex flex-col p-6 gap-4 overflow-auto">
      <div>
        <h3 className="text-lg font-semibold text-white">{endpoint.name}</h3>
        <p className="text-sm text-zinc-400 mt-1">
          {endpoint.method} {endpoint.path}
        </p>
      </div>
      <div className="rounded-lg border border-zinc-800 bg-zinc-900 p-4 space-y-2 max-w-3xl">
        <div className="text-xs text-zinc-500">Latest saved version</div>
        <div className="text-sm text-zinc-200">{meta?.versionLabel ?? 'No version in database'}</div>
        <div className="text-xs text-zinc-500 pt-2">Zendesk URL (from version manual data)</div>
        {meta?.url ? (
          <a
            href={meta.url}
            target="_blank"
            rel="noreferrer"
            className="text-sm text-blue-400 hover:underline break-all inline-flex items-center gap-1"
          >
            {meta.url}
            <ExternalLink className="w-3.5 h-3.5 flex-shrink-0" />
          </a>
        ) : (
          <div className="text-sm text-zinc-500">저장된 Zendesk URL이 없습니다. Projects의 Manual 탭에서 전송하면 여기에 표시됩니다.</div>
        )}
      </div>
      <Button type="button" className="w-fit bg-blue-600" onClick={() => onOpenInProjects(endpoint)}>
        Open in Projects (Manual tab에서 편집)
      </Button>
    </div>
  );
}
