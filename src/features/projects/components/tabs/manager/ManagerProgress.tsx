import { useState } from 'react';
import { Input } from '@/components/ui/input';
import { Search } from 'lucide-react';
import { ApiTable } from './ApiTable';
import { EditTaskModal } from './EditTaskModal';
import { ApiTask, Column } from '../../../types/manager';

interface ManagerProgressProps {
  tasks: ApiTask[];
  columns: Column[];
  onTasksChange: (tasks: ApiTask[]) => void;
  onColumnsChange: (columns: Column[]) => void;
}

export function ManagerProgress({
  tasks,
  columns,
  onTasksChange,
  onColumnsChange,
}: ManagerProgressProps) {
  const [editingTask, setEditingTask] = useState<ApiTask | null>(null);
  const [isEditModalOpen, setIsEditModalOpen] = useState(false);
  const [searchTerm, setSearchTerm] = useState('');

  const handleColumnVisibilityChange = (columnId: string, visible: boolean) => {
    const newColumns = columns.map((col) =>
      col.id === columnId ? { ...col, visible } : col
    );
    onColumnsChange(newColumns);
  };

  const handleTaskEdit = (task: ApiTask) => {
    setEditingTask(task);
    setIsEditModalOpen(true);
  };

  const handleTaskSave = (updatedTask: ApiTask) => {
    const newTasks = tasks.map((task) =>
      task.id === updatedTask.id ? updatedTask : task
    );
    onTasksChange(newTasks);
  };

  const handleTaskDelete = (taskId: string) => {
    if (confirm('이 행을 삭제하시겠습니까?')) {
      const newTasks = tasks.filter((task) => task.id !== taskId);
      onTasksChange(newTasks);
    }
  };

  const handleAddTask = () => {
    const newTask: ApiTask = {
      id: Date.now().toString(),
      tab: '',
      group: '',
      sub1: '',
      sub2: '',
      sub3: '',
      seg1: '',
      seg2: '',
      endPoint: '',
      dev: 'empty',
      vv: 'empty',
      doc: 'empty',
      issue: 'empty',
      status: '',
      charge: '',
      remark: '',
    };
    onTasksChange([...tasks, newTask]);
    setEditingTask(newTask);
    setIsEditModalOpen(true);
  };

  return (
    <div className="space-y-4 p-6">
      <div className="mb-6">
        <h2 className="text-2xl font-semibold mb-2 text-white">
          Product Ribbon & API Pipeline
        </h2>
        <p className="text-zinc-400">API 개발 진행 상황 추적 및 관리</p>
      </div>

      {/* Search Bar */}
      <div className="relative max-w-md">
        <Search className="absolute left-3 top-1/2 -translate-y-1/2 w-4 h-4 text-zinc-500" />
        <Input
          type="text"
          placeholder="전체 검색..."
          value={searchTerm}
          onChange={(e) => setSearchTerm(e.target.value)}
          className="pl-10 bg-zinc-800 border-zinc-700 text-zinc-100 placeholder:text-zinc-500"
        />
      </div>

      <ApiTable
        tasks={tasks}
        columns={columns}
        searchTerm={searchTerm}
        onColumnVisibilityChange={handleColumnVisibilityChange}
        onTaskEdit={handleTaskEdit}
        onTaskDelete={handleTaskDelete}
        onAddTask={handleAddTask}
      />

      <EditTaskModal
        task={editingTask}
        open={isEditModalOpen}
        onClose={() => setIsEditModalOpen(false)}
        onSave={handleTaskSave}
      />
    </div>
  );
}
