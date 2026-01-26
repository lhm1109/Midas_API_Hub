import { useState } from "react";
import ApiTable from "@/app/components/ApiTable";
import EditTaskModal from "@/app/components/EditTaskModal";
import AddColumnModal from "@/app/components/AddColumnModal";
import ShareModal from "@/app/components/ShareModal";
import { sampleTasks, defaultColumns, ApiTask } from "@/app/data/sampleData";
import { Button } from "@/app/components/ui/button";
import { Share2 } from "lucide-react";

interface Column {
  id: string;
  label: string;
  visible: boolean;
}

export default function ProgressPage() {
  const [tasks, setTasks] = useState<ApiTask[]>(sampleTasks);
  const [columns, setColumns] = useState<Column[]>(defaultColumns);
  const [editingTask, setEditingTask] = useState<ApiTask | null>(null);
  const [isEditModalOpen, setIsEditModalOpen] = useState(false);
  const [isAddColumnModalOpen, setIsAddColumnModalOpen] = useState(false);
  const [isShareModalOpen, setIsShareModalOpen] = useState(false);

  const handleColumnVisibilityChange = (columnId: string, visible: boolean) => {
    setColumns((prev) =>
      prev.map((col) => (col.id === columnId ? { ...col, visible } : col))
    );
  };

  const handleTaskEdit = (task: ApiTask) => {
    setEditingTask(task);
    setIsEditModalOpen(true);
  };

  const handleTaskSave = (updatedTask: ApiTask) => {
    setTasks((prev) =>
      prev.map((task) => (task.id === updatedTask.id ? updatedTask : task))
    );
  };

  const handleTaskDelete = (taskId: string) => {
    if (confirm("이 행을 삭제하시겠습니까?")) {
      setTasks((prev) => prev.filter((task) => task.id !== taskId));
    }
  };

  const handleAddTask = () => {
    const newTask: ApiTask = {
      id: Date.now().toString(),
      tab: "",
      group: "",
      sub1: "",
      sub2: "",
      sub3: "",
      seg1: "",
      seg2: "",
      endPoint: "",
      dev: "empty",
      vv: "empty",
      doc: "empty",
      issue: "empty",
      status: "",
      charge: "",
      remark: "",
    };
    setTasks((prev) => [...prev, newTask]);
    setEditingTask(newTask);
    setIsEditModalOpen(true);
  };

  const handleAddColumn = () => {
    setIsAddColumnModalOpen(true);
  };

  const handleAddColumnConfirm = (columnId: string, columnLabel: string) => {
    // 커스텀 컬럼 추가
    const newColumn: Column = {
      id: columnId,
      label: columnLabel,
      visible: true,
    };
    setColumns((prev) => [...prev, newColumn]);

    // 모든 태스크에 해당 컬럼 필드 추가
    setTasks((prev) =>
      prev.map((task) => ({
        ...task,
        [columnId]: "",
      }))
    );
  };

  return (
    <div className="space-y-4">
      <div className="bg-white rounded-lg shadow p-6">
        <div className="mb-6 flex items-center justify-between">
          <div>
            <h2 className="text-2xl font-semibold mb-2">Product Ribbon & API Pipeline</h2>
            <p className="text-gray-600">
              API 개발 진행 상황 추적 및 관리
            </p>
          </div>
          <Button onClick={() => setIsShareModalOpen(true)} variant="outline">
            <Share2 className="w-4 h-4 mr-2" />
            공유
          </Button>
        </div>

        <ApiTable
          tasks={tasks}
          columns={columns}
          onColumnVisibilityChange={handleColumnVisibilityChange}
          onTaskEdit={handleTaskEdit}
          onTaskDelete={handleTaskDelete}
          onAddTask={handleAddTask}
          onAddColumn={handleAddColumn}
        />
      </div>

      <EditTaskModal
        task={editingTask}
        open={isEditModalOpen}
        onClose={() => setIsEditModalOpen(false)}
        onSave={handleTaskSave}
      />

      <AddColumnModal
        open={isAddColumnModalOpen}
        onClose={() => setIsAddColumnModalOpen(false)}
        onAdd={handleAddColumnConfirm}
      />

      <ShareModal
        open={isShareModalOpen}
        onClose={() => setIsShareModalOpen(false)}
      />
    </div>
  );
}