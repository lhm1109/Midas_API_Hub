import { useState, useEffect } from 'react';
import { GlobalSidebar } from '@/components/layouts';
import { SettingsModal } from '@/components/common';
import { ProjectsView, APIListPanel } from '@/features/projects/components';
import { ManualHubView } from '@/features/manual-hub';
import { HistoryView } from '@/features/history/components';
import { DocsView } from '@/features/docs/components';
import { DebugView } from '@/features/debug/components';
import { SchemaView } from '@/features/schema/components';
import { SchemaBuilderPage } from '@/features/schema-builder';
import { ManagerView } from '@/features/manager/components';
import { useManagerData } from '@/features/manager/hooks/useManagerData';
import { TerminalTab } from '@/components/terminal';
import { DatabaseTab } from '@/features/database/DatabaseTab';
import { NimbalystWorkspaceView } from '@/features/nimbalyst';
import { useAppStore } from '@/store/useAppStore';
import { useEndpoints } from '@/hooks';
import type { ApiEndpoint, Settings } from '@/types';
import { Toaster } from '@/components/ui/sonner';
import { initSchemaLogicRules } from '@/lib/schema/schemaLogicEngine';
import { refreshProductMappings } from '@/config/psdMapping';
import {
  DEFAULT_MANUAL_HUB_INDEX_LOCAL_PATH_EN,
  DEFAULT_MANUAL_HUB_INDEX_LOCAL_PATH_KO,
} from '@/config/constants';
import { ChevronRight } from 'lucide-react';

export default function App() {
  const { setRunnerData, releaseEndpointLock } = useAppStore();
  const { endpoints: apiData, loading: endpointsLoading, refetch: refetchEndpoints } = useEndpoints();
  const { tasks: managerTasks } = useManagerData();  // Manager 작업 데이터
  const [activeView, setActiveView] = useState<
    | 'ai'
    | 'batrack'
    | 'terminal'
    | 'manager'
    | 'projects'
    | 'manualHub'
    | 'history'
    | 'docs'
    | 'debug'
    | 'schema'
    | 'builder'
    | 'database'
  >('ai');
  const [settingsOpen, setSettingsOpen] = useState(false);
  const [selectedEndpoint, setSelectedEndpoint] = useState<ApiEndpoint | null>(null);
  const [panelWidth, setPanelWidth] = useState(256); // 기본 너비 256px (w-64)
  const [isPanelCollapsed, setIsPanelCollapsed] = useState(false); // 🔥 패널 접기/펴기 상태

  // 🔥 앱 초기화: PSD 매핑 & 스키마 로직 규칙
  useEffect(() => {
    async function initializeApp() {
      try {
        // 1. Supabase에서 제품 PSD 매핑 가져오기
        await refreshProductMappings();

        // 2. 기본 PSD로 스키마 로직 규칙 초기화 (original + enhanced)
        await initSchemaLogicRules('civil_gen_definition', 'original');
        await initSchemaLogicRules('civil_gen_definition', 'enhanced');

        // 3. 🔥 사용자 이름이 설정되어 있으면 useAppStore에 반영
        const savedUserName = localStorage.getItem('userName');
        if (savedUserName) {
          const { setCurrentUserId } = useAppStore.getState();
          setCurrentUserId(savedUserName);
        }

        console.log('✅ App initialized successfully');
      } catch (error) {
        console.error('❌ Failed to initialize app:', error);
      }
    }

    initializeApp();
  }, []);

  // 🔓 앱 종료 시 락 해제 (세션 정리)
  useEffect(() => {
    const handleBeforeUnload = async () => {
      const userId = useAppStore.getState().currentUserId;
      if (!userId) return;

      // Electron 환경에서는 IPC로 락 해제
      const electronAPI = (window as any).electronAPI;
      if (electronAPI?.locks?.releaseAll) {
        try {
          await electronAPI.locks.releaseAll(userId);
          console.log('🔓 All locks released on app close');
        } catch (e) {
          console.warn('🔓 Failed to release locks on close:', e);
        }
      } else {
        // 웹 환경에서는 beacon API 사용 (비권장이지만 fallback)
        try {
          navigator.sendBeacon(
            `http://localhost:9527/api/locks/user/${encodeURIComponent(userId)}/all`,
            JSON.stringify({})
          );
        } catch (e) {
          // ignore
        }
      }
    };

    window.addEventListener('beforeunload', handleBeforeUnload);
    return () => window.removeEventListener('beforeunload', handleBeforeUnload);
  }, []);

  // localStorage에서 패널 너비 로드
  useEffect(() => {
    const saved = localStorage.getItem('api-list-panel-width');
    if (saved) {
      setPanelWidth(parseInt(saved, 10));
    }
  }, []);

  // 패널 너비 변경 시 localStorage에 저장
  useEffect(() => {
    localStorage.setItem('api-list-panel-width', panelWidth.toString());
  }, [panelWidth]);

  // 패널 리사이즈 핸들러 (최적화: requestAnimationFrame 사용)
  const handleMouseDown = (e: React.MouseEvent) => {
    e.preventDefault();
    const startX = e.clientX;
    const startWidth = panelWidth;
    let rafId: number | null = null;

    const handleMouseMove = (moveEvent: MouseEvent) => {
      // 이전 프레임 취소
      if (rafId !== null) {
        cancelAnimationFrame(rafId);
      }

      // 다음 프레임에 업데이트 예약
      rafId = requestAnimationFrame(() => {
        const delta = moveEvent.clientX - startX;
        const newWidth = Math.max(200, Math.min(600, startWidth + delta)); // 최소 200px, 최대 600px
        setPanelWidth(newWidth);
      });
    };

    const handleMouseUp = () => {
      if (rafId !== null) {
        cancelAnimationFrame(rafId);
      }
      document.removeEventListener('mousemove', handleMouseMove);
      document.removeEventListener('mouseup', handleMouseUp);
      document.body.style.cursor = '';
      document.body.style.userSelect = '';
    };

    document.addEventListener('mousemove', handleMouseMove);
    document.addEventListener('mouseup', handleMouseUp);
    document.body.style.cursor = 'col-resize';
    document.body.style.userSelect = 'none';
  };

  // 엔드포인트 로딩 완료 후 기본 선택
  useEffect(() => {
    if (!endpointsLoading && apiData.length > 0 && !selectedEndpoint) {
      // 첫 번째 엔드포인트를 기본 선택 (있으면)
      const firstEndpoint = apiData[0]?.groups?.[0]?.endpoints?.[0];
      if (firstEndpoint) {
        setSelectedEndpoint(firstEndpoint);
      }
    }
  }, [endpointsLoading, apiData, selectedEndpoint]);

  useEffect(() => {
    if (endpointsLoading || !selectedEndpoint?.id) return;

    const findInGroups = (groups: any[]): ApiEndpoint | null => {
      for (const group of groups || []) {
        const found = group.endpoints?.find((ep: ApiEndpoint) => ep.id === selectedEndpoint.id);
        if (found) return found;

        const foundInSubgroup = findInGroups(group.subgroups || []);
        if (foundInSubgroup) return foundInSubgroup;
      }

      return null;
    };

    for (const product of apiData) {
      const refreshedEndpoint = findInGroups(product.groups || []);
      if (refreshedEndpoint && refreshedEndpoint !== selectedEndpoint) {
        setSelectedEndpoint(refreshedEndpoint);
        useAppStore.setState({ endpoint: refreshedEndpoint });
        return;
      }
    }
  }, [endpointsLoading, apiData, selectedEndpoint]);

  // 🎯 Settings 초기값 (localStorage에서 로드)
  const [settings, setSettings] = useState<Settings>(() => {
    const defaults: Settings = {
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
      useAssignWrapper: true,
      schemaMode: 'enhanced' as const,
      userName: localStorage.getItem('userName') || '',
      supabaseUrl: '',
      supabaseServiceKey: '',
      supabaseDbPassword: '',
      manualHubIndexLocalPathKo: DEFAULT_MANUAL_HUB_INDEX_LOCAL_PATH_KO,
      manualHubIndexLocalPathEn: DEFAULT_MANUAL_HUB_INDEX_LOCAL_PATH_EN,
    };
    try {
      const savedSettings = localStorage.getItem('api-settings');
      if (savedSettings) {
        const parsed = JSON.parse(savedSettings) as Record<string, unknown>;
        return {
          ...defaults,
          ...parsed,
          manualHubIndexLocalPathKo: Object.prototype.hasOwnProperty.call(
            parsed,
            'manualHubIndexLocalPathKo'
          )
            ? String(parsed.manualHubIndexLocalPathKo ?? '')
            : defaults.manualHubIndexLocalPathKo,
          manualHubIndexLocalPathEn: Object.prototype.hasOwnProperty.call(
            parsed,
            'manualHubIndexLocalPathEn'
          )
            ? String(parsed.manualHubIndexLocalPathEn ?? '')
            : defaults.manualHubIndexLocalPathEn,
        };
      }
    } catch (error) {
      console.error('Failed to load settings from localStorage:', error);
    }
    return defaults;
  });

  // 🎯 Settings가 변경될 때마다 localStorage에 저장
  useEffect(() => {
    try {
      localStorage.setItem('api-settings', JSON.stringify(settings));
      // 🔥 사용자 이름이 변경되면 useAppStore에도 반영
      if (settings.userName) {
        const { setCurrentUserId } = useAppStore.getState();
        setCurrentUserId(settings.userName);
      }
    } catch (error) {
      console.error('Failed to save settings to localStorage:', error);
    }
  }, [settings]);

  // Initialize runnerData on mount (preserve existing store state)
  useEffect(() => {
    const { runnerData } = useAppStore.getState();
    if (runnerData) return;

    setRunnerData({
      requestBody: '{}',
      responseBody: '',
      testCases: [],
      selectedTestCaseId: null,
      selectedTestCaseDraftBody: null,
    });
  }, [setRunnerData]);

  // 🔥 [변경됨] 엔드포인트 선택 시 자동 락 제거 - 버전 로드 시에만 락을 획득
  // 하트비트 로직도 버전 로드 시에만 작동하도록 변경됨 (VersionTab.tsx에서 처리)

  // 🔥 페이지 이탈 시 잠금 해제
  useEffect(() => {
    const handleBeforeUnload = async () => {
      if (selectedEndpoint?.id) {
        // 동기적으로 해제 (navigator.sendBeacon 사용)
        await releaseEndpointLock(selectedEndpoint.id);
      }
    };

    window.addEventListener('beforeunload', handleBeforeUnload);

    return () => {
      window.removeEventListener('beforeunload', handleBeforeUnload);
      // 컴포넌트 언마운트 시에도 해제
      if (selectedEndpoint?.id) {
        releaseEndpointLock(selectedEndpoint.id);
      }
    };
  }, [selectedEndpoint?.id, releaseEndpointLock]);

  const handleEndpointSelect = async (endpoint: ApiEndpoint) => {
    const {
      resetCurrentVersion,
      releaseEndpointLock,
      endpoint: currentEndpoint
    } = useAppStore.getState();

    // 🔥 이전 엔드포인트의 잠금 해제
    if (currentEndpoint?.id && currentEndpoint.id !== endpoint.id) {
      await releaseEndpointLock(currentEndpoint.id);
    }

    setSelectedEndpoint(endpoint);

    // 🔥 Store에 엔드포인트 저장
    useAppStore.setState({ endpoint });

    // 🔥 엔드포인트 변경 시 현재 버전과 편집 상태 리셋
    resetCurrentVersion();
  };

  return (
    <div className="size-full flex bg-zinc-950 text-zinc-100 overflow-hidden">
      {/* 1. Global Sidebar */}
      <GlobalSidebar
        activeView={activeView}
        onViewChange={setActiveView}
        onSettingsClick={() => setSettingsOpen(true)}
      />

      {/* 2. API List Panel (only show in projects view) */}
      {activeView === 'projects' && (
        <>
          {endpointsLoading ? (
            <div
              style={{ width: isPanelCollapsed ? '0px' : `${panelWidth}px` }}
              className="bg-zinc-900 border-r border-zinc-800 flex items-center justify-center transition-all duration-300 overflow-hidden"
            >
              {!isPanelCollapsed && <p className="text-zinc-500">Loading...</p>}
            </div>
          ) : (
            <>
              {/* 패널 컨테이너 - 접혔을 때도 작은 영역 유지 */}
              <div
                style={{
                  width: isPanelCollapsed ? '40px' : `${panelWidth}px`,
                  willChange: 'width', // GPU 가속
                }}
                className="relative bg-zinc-900 border-r border-zinc-800 flex-shrink-0 overflow-hidden h-full"
              >
                {isPanelCollapsed ? (
                  // 접힌 상태: 펼치기 버튼만 표시
                  <div className="h-full flex items-start justify-center pt-3">
                    <button
                      onClick={() => setIsPanelCollapsed(false)}
                      className="w-8 h-8 bg-zinc-800 hover:bg-zinc-700 border border-zinc-700 rounded-md flex items-center justify-center transition-colors shadow-lg"
                      title="패널 펼치기"
                    >
                      <ChevronRight className="w-4 h-4 text-zinc-400" />
                    </button>
                  </div>
                ) : (
                  // 펼쳐진 상태: 패널 전체 표시
                  <APIListPanel
                    products={apiData}
                    selectedEndpoint={selectedEndpoint?.id || null}
                    onEndpointSelect={handleEndpointSelect}
                    onEndpointsChange={refetchEndpoints}
                    onToggleCollapse={() => setIsPanelCollapsed(true)}
                    linkedEndpointIds={new Set(
                      managerTasks
                        .filter(t => t.linkedEndpointId)
                        .map(t => t.linkedEndpointId!)
                    )}
                  />
                )}
              </div>

              {/* 리사이즈 핸들 (펼쳐진 상태에서만) */}
              {!isPanelCollapsed && (
                <div
                  onMouseDown={handleMouseDown}
                  className="w-1 bg-zinc-800 hover:bg-blue-500 cursor-col-resize transition-colors flex-shrink-0 relative group"
                  style={{ touchAction: 'none' }}
                >
                  <div className="absolute inset-y-0 -left-1 -right-1" />
                </div>
              )}
            </>
          )}
        </>
      )}

      {/* 3. Main Content Area */}
      {activeView === 'ai' || activeView === 'batrack' ? (
        <NimbalystWorkspaceView />
      ) : activeView === 'terminal' ? (
        <TerminalTab />
      ) : activeView === 'manager' ? (
        <ManagerView
          endpoints={apiData}
          onNavigateToEndpoint={(endpointId) => {
            // 모든 프로덕트에서 해당 엔드포인트 찾기
            for (const product of apiData) {
              for (const group of product.groups) {
                const found = group.endpoints.find(ep => ep.id === endpointId);
                if (found) {
                  setSelectedEndpoint(found);
                  setActiveView('projects');
                  return;
                }
                // 하위 그룹도 탐색
                const findInSubgroups = (subgroups: typeof group.subgroups): boolean => {
                  for (const subgroup of subgroups) {
                    const foundInSub = subgroup.endpoints.find(ep => ep.id === endpointId);
                    if (foundInSub) {
                      setSelectedEndpoint(foundInSub);
                      setActiveView('projects');
                      return true;
                    }
                    if (findInSubgroups(subgroup.subgroups)) return true;
                  }
                  return false;
                };
                if (findInSubgroups(group.subgroups)) return;
              }
            }
            console.warn('Endpoint not found:', endpointId);
          }}
        />
      ) : activeView === 'manualHub' ? (
        <ManualHubView
          products={apiData}
          settings={settings}
          onNavigateToProjectEndpoint={(ep) => {
            setSelectedEndpoint(ep);
            useAppStore.setState({ endpoint: ep });
            useAppStore.getState().setCurrentTab('manual');
            setActiveView('projects');
          }}
        />
      ) : activeView === 'projects' ? (
        <ProjectsView
          endpoint={selectedEndpoint}
          products={apiData}
          settings={settings}
        />
      ) : activeView === 'history' ? (
        <HistoryView />
      ) : activeView === 'docs' ? (
        <DocsView />
      ) : activeView === 'schema' ? (
        <SchemaView />
      ) : activeView === 'builder' ? (
        <SchemaBuilderPage />
      ) : activeView === 'database' ? (
        <DatabaseTab settings={settings} />
      ) : (
        <DebugView />
      )}

      {/* Settings Modal */}
      <SettingsModal
        open={settingsOpen}
        onOpenChange={setSettingsOpen}
        settings={settings}
        onSettingsChange={setSettings}
      />

      {/* Toaster for toast notifications */}
      <Toaster richColors position="top-right" />
    </div>
  );
}
