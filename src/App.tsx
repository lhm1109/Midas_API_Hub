import { useState, useEffect } from 'react';
import { GlobalSidebar } from '@/components/layouts';
import { SettingsModal } from '@/components/common';
import { ProjectsView, APIListPanel } from '@/features/projects/components';
import { HistoryView } from '@/features/history/components';
import { DocsView } from '@/features/docs/components';
import { DebugView } from '@/features/debug/components';
import { useAppStore } from '@/store/useAppStore';
import { useEndpoints } from '@/hooks';
import type { ApiEndpoint } from '@/types';
import { Toaster } from '@/components/ui/sonner';
import { initSchemaLogicRules } from '@/lib/schema/schemaLogicEngine';

export default function App() {
  const { setRunnerData } = useAppStore();
  const { endpoints: apiData, loading: endpointsLoading, refetch: refetchEndpoints } = useEndpoints();
  const [activeView, setActiveView] = useState<'projects' | 'history' | 'docs' | 'debug'>('projects');
  const [settingsOpen, setSettingsOpen] = useState(false);
  const [selectedEndpoint, setSelectedEndpoint] = useState<ApiEndpoint | null>(null);
  const [panelWidth, setPanelWidth] = useState(256); // 기본 너비 256px (w-64)

  // 🔥 스키마 로직 규칙 초기화 (앱 시작 시 한 번만)
  useEffect(() => {
    initSchemaLogicRules().catch(error => {
      console.error('Failed to initialize schema logic rules:', error);
    });
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

  // 패널 리사이즈 핸들러
  const handleMouseDown = (e: React.MouseEvent) => {
    e.preventDefault();
    const startX = e.clientX;
    const startWidth = panelWidth;

    const handleMouseMove = (moveEvent: MouseEvent) => {
      const delta = moveEvent.clientX - startX;
      const newWidth = Math.max(200, Math.min(600, startWidth + delta)); // 최소 200px, 최대 600px
      setPanelWidth(newWidth);
    };

    const handleMouseUp = () => {
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
  
  // 🎯 Settings 초기값 (localStorage에서 로드)
  const [settings, setSettings] = useState(() => {
    try {
      const savedSettings = localStorage.getItem('api-settings');
      if (savedSettings) {
        return JSON.parse(savedSettings);
      }
    } catch (error) {
      console.error('Failed to load settings from localStorage:', error);
    }
    
    // 기본값
    return {
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
      useAssignWrapper: true, // 기본값: Assign 래퍼 사용
    };
  });

  // 🎯 Settings가 변경될 때마다 localStorage에 저장
  useEffect(() => {
    try {
      localStorage.setItem('api-settings', JSON.stringify(settings));
    } catch (error) {
      console.error('Failed to save settings to localStorage:', error);
    }
  }, [settings]);

  // Initialize runnerData on mount
  useEffect(() => {
    setRunnerData({
      requestBody: '{}',
      responseBody: '',
      testCases: [],
    });
  }, [setRunnerData]);

  const handleEndpointSelect = async (endpoint: ApiEndpoint) => {
    setSelectedEndpoint(endpoint);
    
    // 🔥 엔드포인트 변경 시 현재 버전과 모든 탭 데이터 리셋
    const { resetCurrentVersion, fetchVersions } = useAppStore.getState();
    
    // 🔥 1. 현재 버전 및 모든 데이터 리셋
    resetCurrentVersion();
    
    // 🔥 2. 새 엔드포인트의 버전 목록 불러오기
    try {
      await fetchVersions(endpoint.id);
    } catch (error) {
      console.error('Failed to fetch versions:', error);
    }
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
              style={{ width: `${panelWidth}px` }}
              className="bg-zinc-900 border-r border-zinc-800 flex items-center justify-center"
            >
              <p className="text-zinc-500">Loading...</p>
            </div>
          ) : (
            <>
              <div 
                style={{ width: `${panelWidth}px` }}
                className="relative bg-zinc-900 border-r border-zinc-800 flex-shrink-0"
              >
                <APIListPanel
                  products={apiData}
                  selectedEndpoint={selectedEndpoint?.id || null}
                  onEndpointSelect={handleEndpointSelect}
                  onEndpointsChange={refetchEndpoints}
                />
              </div>
              {/* Resize Handle */}
              <div
                onMouseDown={handleMouseDown}
                className="w-1 bg-zinc-800 hover:bg-blue-500 cursor-col-resize transition-colors flex-shrink-0 relative group"
                style={{ touchAction: 'none' }}
              >
                <div className="absolute inset-y-0 -left-1 -right-1" />
              </div>
            </>
          )}
        </>
      )}

      {/* 3. Main Content Area */}
      {activeView === 'projects' ? (
        <ProjectsView
          endpoint={selectedEndpoint}
          settings={settings}
        />
      ) : activeView === 'history' ? (
        <HistoryView />
      ) : activeView === 'docs' ? (
        <DocsView />
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

