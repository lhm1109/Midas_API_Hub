import { useState } from 'react';
import { Tabs, TabsContent, TabsList, TabsTrigger } from '@/components/ui/tabs';
import { BarChart3, Table2 } from 'lucide-react';
import { ManagerDashboard } from './manager/ManagerDashboard';
import { ManagerProgress } from './manager/ManagerProgress';
import { sampleTasks } from '../../data/sampleTasks';
import { defaultColumns } from '../../types/manager';
import type { ApiTask, Column } from '../../types/manager';

export function ManagerTab() {
  const [tasks, setTasks] = useState<ApiTask[]>(sampleTasks);
  const [columns, setColumns] = useState<Column[]>(defaultColumns);
  const [activeSubTab, setActiveSubTab] = useState('dashboard');

  return (
    <div className="flex-1 flex flex-col bg-zinc-950 overflow-hidden">
      <Tabs
        value={activeSubTab}
        onValueChange={setActiveSubTab}
        className="flex flex-col h-full overflow-hidden"
      >
        {/* Sub Tab Headers */}
        <div className="border-b border-zinc-800 bg-zinc-900 px-6 pt-3 flex-shrink-0">
          <TabsList className="bg-zinc-800 h-10 p-1 rounded-lg">
            <TabsTrigger
              value="dashboard"
              className="px-4 py-2 rounded-md text-sm font-medium transition-colors data-[state=active]:bg-zinc-700 data-[state=active]:text-white text-zinc-400 hover:text-zinc-200"
            >
              <BarChart3 className="w-4 h-4 mr-2" />
              Dashboard
            </TabsTrigger>
            <TabsTrigger
              value="progress"
              className="px-4 py-2 rounded-md text-sm font-medium transition-colors data-[state=active]:bg-zinc-700 data-[state=active]:text-white text-zinc-400 hover:text-zinc-200"
            >
              <Table2 className="w-4 h-4 mr-2" />
              Progress
            </TabsTrigger>
          </TabsList>
        </div>

        {/* Sub Tab Content */}
        <TabsContent
          value="dashboard"
          className="flex-1 m-0 overflow-auto data-[state=active]:flex"
        >
          <div className="flex-1 overflow-auto">
            <ManagerDashboard tasks={tasks} />
          </div>
        </TabsContent>

        <TabsContent
          value="progress"
          className="flex-1 m-0 overflow-auto data-[state=active]:flex"
        >
          <div className="flex-1 overflow-auto">
            <ManagerProgress
              tasks={tasks}
              columns={columns}
              onTasksChange={setTasks}
              onColumnsChange={setColumns}
            />
          </div>
        </TabsContent>
      </Tabs>
    </div>
  );
}
