import { useState } from 'react';
import { Tabs, TabsContent, TabsList, TabsTrigger } from '@/components/ui/tabs';
import { BarChart3, Table2, Loader2 } from 'lucide-react';
import { ManagerDashboard } from './ManagerDashboard';
import { ManagerProgress } from './ManagerProgress';
import { useManagerData } from '../hooks/useManagerData';
import type { ApiProduct } from '@/types';

interface ManagerViewProps {
  endpoints?: ApiProduct[];
  onNavigateToEndpoint?: (endpointId: string) => void;
}

export function ManagerView({ endpoints = [], onNavigateToEndpoint }: ManagerViewProps) {
  const {
    tasks,
    columns,
    loading,
    error,
    addTask,
    updateTask,
    deleteTask,
    updateColumnVisibility,
    bulkReplaceTasks,
    refetch,
  } = useManagerData();
  const [activeTab, setActiveTab] = useState('progress');

  // 로딩 상태
  if (loading) {
    return (
      <div className="flex-1 flex items-center justify-center bg-zinc-950">
        <div className="text-center">
          <Loader2 className="w-8 h-8 text-cyan-500 animate-spin mx-auto mb-4" />
          <p className="text-zinc-400">데이터를 불러오는 중...</p>
        </div>
      </div>
    );
  }

  // 에러 상태
  if (error) {
    return (
      <div className="flex-1 flex items-center justify-center bg-zinc-950 p-8">
        <div className="max-w-2xl text-center space-y-4">
          <div className="text-red-400 text-5xl mb-4">⚠️</div>
          <h2 className="text-2xl font-bold text-white mb-2">데이터를 불러오지 못했습니다</h2>
          <p className="text-zinc-400 mb-4">{error}</p>

          <div className="bg-zinc-900 border border-zinc-800 rounded-lg p-6 text-left">
            <h3 className="text-lg font-semibold text-cyan-400 mb-3">🔧 해결 방법</h3>
            <ol className="space-y-2 text-zinc-300 text-sm">
              <li className="flex items-start gap-2">
                <span className="font-bold text-cyan-400">1.</span>
                <span>Supabase Dashboard에 접속 (https://app.supabase.com)</span>
              </li>
              <li className="flex items-start gap-2">
                <span className="font-bold text-cyan-400">2.</span>
                <span>프로젝트 선택 → SQL Editor 메뉴 클릭</span>
              </li>
              <li className="flex items-start gap-2">
                <span className="font-bold text-cyan-400">3.</span>
                <span><code className="bg-zinc-800 px-2 py-1 rounded">server/schema-manager.sql</code> 파일의 내용을 복사하여 실행</span>
              </li>
              <li className="flex items-start gap-2">
                <span className="font-bold text-cyan-400">4.</span>
                <span>이 페이지를 새로고침</span>
              </li>
            </ol>
          </div>

          <div className="text-xs text-zinc-500 mt-4">
            자세한 설명: <code className="bg-zinc-800 px-2 py-1 rounded">server/MANAGER_SETUP.md</code>
          </div>
        </div>
      </div>
    );
  }

  return (
    <div className="flex-1 flex flex-col bg-zinc-950 overflow-hidden">

      <Tabs
        value={activeTab}
        onValueChange={setActiveTab}
        className="flex flex-col h-full overflow-hidden"
      >
        {/* Tab Headers */}
        <div className="border-b border-zinc-800 bg-zinc-900 px-6 pt-3 flex-shrink-0">
          <TabsList className="bg-zinc-800 h-10 p-1 rounded-lg">
            <TabsTrigger
              value="progress"
              className="px-4 py-2 rounded-md text-sm font-medium transition-colors data-[state=active]:bg-zinc-700 data-[state=active]:text-white text-zinc-400 hover:text-zinc-200"
            >
              <Table2 className="w-4 h-4 mr-2" />
              Progress
            </TabsTrigger>
            <TabsTrigger
              value="dashboard"
              className="px-4 py-2 rounded-md text-sm font-medium transition-colors data-[state=active]:bg-zinc-700 data-[state=active]:text-white text-zinc-400 hover:text-zinc-200"
            >
              <BarChart3 className="w-4 h-4 mr-2" />
              Dashboard
            </TabsTrigger>
          </TabsList>
        </div>

        {/* Tab Content */}
        <TabsContent
          value="progress"
          className="flex-1 m-0 overflow-auto data-[state=active]:flex"
        >
          <div className="flex-1 overflow-auto">
            <ManagerProgress
              tasks={tasks}
              columns={columns}
              onTaskEdit={updateTask}
              onTaskDelete={deleteTask}
              onAddTask={addTask}
              onColumnVisibilityChange={updateColumnVisibility}
              onRefresh={refetch}
              onBulkReplace={bulkReplaceTasks}
              endpoints={endpoints}
              onNavigateToEndpoint={onNavigateToEndpoint}
            />
          </div>
        </TabsContent>

        <TabsContent
          value="dashboard"
          className="flex-1 m-0 overflow-auto data-[state=active]:flex"
        >
          <div className="flex-1 overflow-auto">
            <ManagerDashboard tasks={tasks} />
          </div>
        </TabsContent>
      </Tabs>
    </div>
  );
}
