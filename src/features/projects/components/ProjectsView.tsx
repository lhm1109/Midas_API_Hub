import { FileText, Wrench, Rocket, BookOpen, GitBranch, Save, AlertCircle, Loader2 } from 'lucide-react';
import { Tabs, TabsContent, TabsList, TabsTrigger } from '@/components/ui/tabs';
import { VersionTab, ManualTab, SpecTab, BuilderTab, RunnerTab } from './tabs';
import { LockBanner } from './LockBanner';
import type { ApiEndpoint } from '@/types';
import { useAppStore } from '@/store/useAppStore';
import { Button } from '@/components/ui/button';
import { toast } from 'sonner';
import { useEffect, useState } from 'react';

interface MainWorkspaceProps {
  endpoint: ApiEndpoint | null;
  settings: {
    baseUrl: string;
    mapiKey: string;
    commonHeaders: string;
    useAssignWrapper?: boolean;
  };
}

export function ProjectsView({ endpoint, settings }: MainWorkspaceProps) {
  const { 
    currentVersionId, 
    getCurrentVersion, 
    saveCurrentVersion, 
    hasUnsavedChanges,
    currentTab,
    setCurrentTab,
    checkEndpointLock,
    endpointLock,
  } = useAppStore();

  // 🔥 저장 중 상태
  const [isSaving, setIsSaving] = useState(false);

  // 엔드포인트 변경 시 잠금 상태 확인
  useEffect(() => {
    if (endpoint?.id) {
      checkEndpointLock(endpoint.id);
    }
  }, [endpoint?.id, checkEndpointLock]);

  // 버전이 로드되지 않았을 때 자동으로 Version 탭으로 이동
  useEffect(() => {
    if (!currentVersionId && currentTab !== 'version') {
      setCurrentTab('version');
    }
  }, [currentVersionId, currentTab, setCurrentTab]);

  // 엔드포인트 변경 시 Version 탭으로 자동 이동
  useEffect(() => {
    if (endpoint?.id) {
      setCurrentTab('version');
    }
  }, [endpoint?.id, setCurrentTab]);

  if (!endpoint) {
    return (
      <div className="flex-1 flex items-center justify-center bg-zinc-950 text-zinc-600">
        <div className="text-center">
          <FileText className="w-16 h-16 mx-auto mb-4 opacity-50" />
          <p className="text-lg">Select an API endpoint from the left panel to get started</p>
        </div>
      </div>
    );
  }

  const currentVersion = getCurrentVersion();
  
  // 다른 사용자가 잠금한 경우 읽기 전용 모드
  const isReadOnly = endpointLock?.locked && endpointLock?.lockedBy !== useAppStore.getState().currentUserId;
  const isLocked = !currentVersionId || isReadOnly; // 버전이 선택되지 않았거나 다른 사용자가 편집 중이면 탭 잠김

  // 🔥 Save Version 핸들러 (toast 추가)
  const handleSaveVersion = async () => {
    if (!currentVersionId) {
      toast.error('❌ No version selected');
      return;
    }

    if (!hasUnsavedChanges) {
      toast.info('ℹ️ No changes to save');
      return;
    }

    setIsSaving(true);
    const startTime = Date.now();
    
    try {
      console.log('💾 Starting save...');
      await saveCurrentVersion();
      const duration = Date.now() - startTime;
      console.log('✅ Save completed in', duration, 'ms');
      toast.success(`✅ Version saved successfully! (${duration}ms)`);
    } catch (error) {
      const duration = Date.now() - startTime;
      console.error('❌ Save version failed:', error);
      toast.error(`❌ Failed to save version: ${error instanceof Error ? error.message : 'Unknown error'}`);
    } finally {
      setIsSaving(false);
    }
  };

  return (
    <div className="flex-1 flex flex-col bg-zinc-950 overflow-hidden">
      {/* 🔒 편집 잠금 배너 */}
      {endpoint && <LockBanner endpointId={endpoint.id} />}
      
      {/* 🎯 상단 버전 정보 + 저장 버튼 */}
      <div className="border-b border-zinc-800 bg-zinc-900 px-6 py-3 flex items-center justify-between flex-shrink-0">
        <div className="flex items-center gap-3">
          {currentVersion ? (
            <>
              <div className="flex items-center gap-2 text-sm">
                <GitBranch className="w-4 h-4 text-blue-400" />
                <span className="text-zinc-400">Current Version:</span>
                <span className="font-semibold text-white">{currentVersion.version}</span>
              </div>
              {hasUnsavedChanges && (
                <div className="flex items-center gap-1 text-xs text-yellow-400">
                  <AlertCircle className="w-3 h-3" />
                  <span>Unsaved changes</span>
                </div>
              )}
            </>
          ) : (
            <div className="flex items-center gap-2 text-sm text-zinc-500">
              <AlertCircle className="w-4 h-4" />
              <span>No version selected - Please create or load a version</span>
            </div>
          )}
        </div>

        {currentVersion && (
          <Button
            onClick={handleSaveVersion}
            disabled={!hasUnsavedChanges || isReadOnly || isSaving}
            className="bg-blue-600 hover:bg-blue-700 text-white h-9 px-4 disabled:opacity-50 disabled:cursor-not-allowed"
            title={
              isReadOnly ? '다른 사용자가 편집 중입니다' 
              : !hasUnsavedChanges ? '변경사항이 없습니다'
              : isSaving ? '저장 중...'
              : 'Click to save current version'
            }
          >
            {isSaving ? (
              <>
                <Loader2 className="w-4 h-4 mr-2 animate-spin" />
                Saving...
              </>
            ) : (
              <>
                <Save className="w-4 h-4 mr-2" />
                {isReadOnly ? '읽기 전용' : 'Save Version'}
              </>
            )}
          </Button>
        )}
      </div>

      <Tabs value={currentTab} onValueChange={(value) => setCurrentTab(value as any)} className="flex flex-col h-full overflow-hidden">
        {/* Tab Headers */}
        <div className="border-b border-zinc-800 bg-zinc-900 px-6 pt-3 flex-shrink-0">
          <TabsList className="bg-zinc-800 h-10 p-1 rounded-lg">
            <TabsTrigger
              value="version"
              className="px-4 py-2 rounded-md text-sm font-medium transition-colors data-[state=active]:bg-zinc-700 data-[state=active]:text-white text-zinc-400 hover:text-zinc-200"
            >
              <GitBranch className="w-4 h-4 mr-2" />
              Version
            </TabsTrigger>
            <TabsTrigger
              value="manual"
              disabled={isLocked}
              className="px-4 py-2 rounded-md text-sm font-medium transition-colors data-[state=active]:bg-zinc-700 data-[state=active]:text-white text-zinc-400 hover:text-zinc-200 disabled:opacity-30 disabled:cursor-not-allowed"
            >
              <BookOpen className="w-4 h-4 mr-2" />
              Manual
            </TabsTrigger>
            <TabsTrigger
              value="spec"
              disabled={isLocked}
              className="px-4 py-2 rounded-md text-sm font-medium transition-colors data-[state=active]:bg-zinc-700 data-[state=active]:text-white text-zinc-400 hover:text-zinc-200 disabled:opacity-30 disabled:cursor-not-allowed"
            >
              <FileText className="w-4 h-4 mr-2" />
              Spec
            </TabsTrigger>
            <TabsTrigger
              value="builder"
              disabled={isLocked}
              className="px-4 py-2 rounded-md text-sm font-medium transition-colors data-[state=active]:bg-zinc-700 data-[state=active]:text-white text-zinc-400 hover:text-zinc-200 disabled:opacity-30 disabled:cursor-not-allowed"
            >
              <Wrench className="w-4 h-4 mr-2" />
              Builder
            </TabsTrigger>
            <TabsTrigger
              value="runner"
              disabled={isLocked}
              className="px-4 py-2 rounded-md text-sm font-medium transition-colors data-[state=active]:bg-zinc-700 data-[state=active]:text-white text-zinc-400 hover:text-zinc-200 disabled:opacity-30 disabled:cursor-not-allowed"
            >
              <Rocket className="w-4 h-4 mr-2" />
              Runner
            </TabsTrigger>
          </TabsList>
        </div>

        {/* Tab Content */}
        <TabsContent value="version" className="flex-1 m-0 overflow-hidden data-[state=active]:flex">
          <VersionTab key={`version-${endpoint.id}`} endpoint={endpoint} />
        </TabsContent>

        <TabsContent value="manual" className="flex-1 m-0 overflow-hidden data-[state=active]:flex">
          {isLocked ? (
            <div className="flex-1 flex items-center justify-center text-zinc-600">
              <div className="text-center">
                <AlertCircle className="w-12 h-12 mx-auto mb-3 opacity-50" />
                <p className="text-sm">Please create or load a version to access Manual tab</p>
              </div>
            </div>
          ) : (
            <ManualTab key={`manual-${endpoint.id}-${currentVersionId || 'none'}`} endpoint={endpoint} />
          )}
        </TabsContent>

        <TabsContent value="spec" className="flex-1 m-0 overflow-hidden data-[state=active]:flex">
          {isLocked ? (
            <div className="flex-1 flex items-center justify-center text-zinc-600">
              <div className="text-center">
                <AlertCircle className="w-12 h-12 mx-auto mb-3 opacity-50" />
                <p className="text-sm">Please create or load a version to access Spec tab</p>
              </div>
            </div>
          ) : (
            <SpecTab key={`spec-${endpoint.id}-${currentVersionId || 'none'}`} endpoint={endpoint} />
          )}
        </TabsContent>

        <TabsContent value="builder" className="flex-1 m-0 overflow-hidden data-[state=active]:flex">
          {isLocked ? (
            <div className="flex-1 flex items-center justify-center text-zinc-600">
              <div className="text-center">
                <AlertCircle className="w-12 h-12 mx-auto mb-3 opacity-50" />
                <p className="text-sm">Please create or load a version to access Builder tab</p>
              </div>
            </div>
          ) : (
            <BuilderTab 
              key={`builder-${endpoint.id}-${currentVersionId || 'none'}`} 
              endpoint={endpoint}
              settings={settings}
            />
          )}
        </TabsContent>

        <TabsContent value="runner" className="flex-1 m-0 overflow-hidden data-[state=active]:flex">
          {isLocked ? (
            <div className="flex-1 flex items-center justify-center text-zinc-600">
              <div className="text-center">
                <AlertCircle className="w-12 h-12 mx-auto mb-3 opacity-50" />
                <p className="text-sm">Please create or load a version to access Runner tab</p>
              </div>
            </div>
          ) : (
            <RunnerTab
              key={`runner-${endpoint.id}-${currentVersionId || 'none'}`}
              endpoint={endpoint}
              settings={settings}
            />
          )}
        </TabsContent>
      </Tabs>
    </div>
  );
}
