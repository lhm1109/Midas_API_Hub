import { useState, useRef } from 'react';
import { ApiTable } from './ApiTable';
import { EditTaskModal } from './EditTaskModal';
import { ApiTask, Column } from '../types/manager';
import { Button } from '@/components/ui/button';
import { RefreshCw, Download, Upload } from 'lucide-react';

interface ManagerProgressProps {
  tasks: ApiTask[];
  columns: Column[];
  onTaskEdit: (task: ApiTask) => Promise<ApiTask>;
  onTaskDelete: (taskId: string) => Promise<void>;
  onAddTask: (task: ApiTask) => Promise<ApiTask>;
  onColumnVisibilityChange: (columnId: string, visible: boolean) => Promise<void>;
  onRefresh: () => Promise<void>;
  onBulkReplace: (tasks: ApiTask[]) => Promise<void>;
}

export function ManagerProgress({
  tasks,
  columns,
  onTaskEdit,
  onTaskDelete,
  onAddTask,
  onColumnVisibilityChange,
  onRefresh,
  onBulkReplace,
}: ManagerProgressProps) {
  const [editingTask, setEditingTask] = useState<ApiTask | null>(null);
  const [isEditModalOpen, setIsEditModalOpen] = useState(false);
  const [isRefreshing, setIsRefreshing] = useState(false);
  const fileInputRef = useRef<HTMLInputElement>(null);

  const handleColumnVisibilityChange = async (columnId: string, visible: boolean) => {
    try {
      await onColumnVisibilityChange(columnId, visible);
    } catch (error) {
      console.error('Failed to update column visibility:', error);
      alert('컬럼 설정 변경 실패: 데이터베이스 연결을 확인해주세요.');
    }
  };

  const handleTaskEdit = (task: ApiTask) => {
    setEditingTask(task);
    setIsEditModalOpen(true);
  };

  const handleTaskSave = async (updatedTask: ApiTask) => {
    try {
      await onTaskEdit(updatedTask);
      setIsEditModalOpen(false);
    } catch (error) {
      console.error('Failed to save task:', error);
      alert('작업 저장 실패: 데이터베이스 연결을 확인해주세요.');
    }
  };

  const handleTaskDelete = async (taskId: string) => {
    if (confirm('이 행을 삭제하시겠습니까?')) {
      try {
        await onTaskDelete(taskId);
      } catch (error) {
        console.error('Failed to delete task:', error);
        alert('작업 삭제 실패: 데이터베이스 연결을 확인해주세요.');
      }
    }
  };

  const handleAddTask = async () => {
    try {
      const newTask: ApiTask = {
        id: Date.now().toString(),
        product: '',
        tab: '',
        group: '',
        sub1: '',
        sub2: '',
        sub3: '',
        seg1: '',
        seg2: '',
        endPoint: '',
        mode: '',
        plan: 'empty',  // plan 컬럼 추가
        dev: 'empty',
        vv: 'empty',
        doc: 'empty',
        issue: 'empty',
        status: '',
        charge: '',
        remark: '',
      };
      const addedTask = await onAddTask(newTask);
      setEditingTask(addedTask);
      setIsEditModalOpen(true);
    } catch (error) {
      console.error('Failed to add task:', error);
      alert('작업 추가 실패: 데이터베이스 테이블을 먼저 생성해주세요.\n\n자세한 내용은 server/MANAGER_SETUP.md를 참고하세요.');
    }
  };

  // 새로고침
  const handleRefresh = async () => {
    setIsRefreshing(true);
    try {
      await onRefresh();
    } catch (error) {
      console.error('Failed to refresh:', error);
      alert('새로고침 실패: 서버 연결을 확인해주세요.');
    } finally {
      setIsRefreshing(false);
    }
  };

  // CSV Export
  const handleExportCSV = () => {
    try {
      // CSV 헤더
      const headers = ['Product', 'Tab', 'Group', 'sub1', 'sub2', 'sub3', 'seg1', 'seg2', 'End Point', 'mode', 'Plan', 'Dev', 'V&V', 'doc', 'Issue', 'status', 'charge', 'remark'];

      // CSV 데이터 생성
      const csvRows = [headers.join(',')];

      tasks.forEach(task => {
        const row = [
          task.product,
          task.tab,
          task.group,
          task.sub1,
          task.sub2,
          task.sub3,
          task.seg1,
          task.seg2,
          task.endPoint,
          task.mode,
          task.plan,  // plan 컬럼 추가
          task.dev,
          task.vv,
          task.doc,
          task.issue,
          task.status,
          task.charge,
          task.remark,
        ].map(value => {
          // CSV 이스케이프: 쉼표나 따옴표가 있으면 감싸기
          const str = String(value || '');
          if (str.includes(',') || str.includes('"') || str.includes('\n')) {
            return `"${str.replace(/"/g, '""')}"`;
          }
          return str;
        });
        csvRows.push(row.join(','));
      });

      // BOM 추가 (엑셀에서 한글 깨짐 방지)
      const csvContent = '\uFEFF' + csvRows.join('\n');
      const blob = new Blob([csvContent], { type: 'text/csv;charset=utf-8;' });
      const link = document.createElement('a');
      const url = URL.createObjectURL(blob);

      link.setAttribute('href', url);
      link.setAttribute('download', `manager_tasks_${new Date().toISOString().split('T')[0]}.csv`);
      link.style.visibility = 'hidden';
      document.body.appendChild(link);
      link.click();
      document.body.removeChild(link);
    } catch (error) {
      console.error('Failed to export CSV:', error);
      alert('CSV 내보내기 실패');
    }
  };

  // CSV Import
  const handleImportCSV = (event: React.ChangeEvent<HTMLInputElement>) => {
    const file = event.target.files?.[0];
    if (!file) return;

    const reader = new FileReader();
    reader.onload = async (e) => {
      try {
        const text = e.target?.result as string;
        const lines = text.split('\n').filter(line => line.trim());

        if (lines.length < 2) {
          alert('CSV 파일이 비어있거나 형식이 잘못되었습니다.');
          return;
        }

        // 헤더 제거
        const dataLines = lines.slice(1);

        // CSV 라인을 파싱하는 함수 (따옴표 내 쉼표 처리)
        const parseCSVLine = (line: string): string[] => {
          const result: string[] = [];
          let current = '';
          let inQuotes = false;

          for (let i = 0; i < line.length; i++) {
            const char = line[i];

            if (char === '"') {
              if (inQuotes && line[i + 1] === '"') {
                // 이스케이프된 따옴표 ("") -> (")
                current += '"';
                i++;
              } else {
                // 따옴표 영역 토글
                inQuotes = !inQuotes;
              }
            } else if (char === ',' && !inQuotes) {
              // 따옴표 밖의 쉼표 = 구분자
              result.push(current.trim());
              current = '';
            } else {
              current += char;
            }
          }
          // 마지막 필드 추가
          result.push(current.trim());

          return result;
        };

        const newTasks: ApiTask[] = dataLines.map((line, index) => {
          // CSV 파싱 (따옴표 내 쉼표 올바르게 처리)
          const values = parseCSVLine(line);

          return {
            id: `imported-${Date.now()}-${index}`,
            product: values[0] || '',
            tab: values[1] || '',
            group: values[2] || '',
            sub1: values[3] || '',
            sub2: values[4] || '',
            sub3: values[5] || '',
            seg1: values[6] || '',
            seg2: values[7] || '',
            endPoint: values[8] || '',
            mode: values[9] || '',
            plan: (values[10] as any) || 'empty',  // plan 컬럼 추가
            dev: (values[11] as any) || 'empty',
            vv: (values[12] as any) || 'empty',
            doc: (values[13] as any) || 'empty',
            issue: (values[14] as any) || 'empty',
            status: values[15] || '',
            charge: values[16] || '',
            remark: values[17] || '',
          };
        });

        if (confirm(`${newTasks.length}개의 작업을 가져와서 기존 데이터를 모두 덮어씁니다. 계속하시겠습니까?`)) {
          await onBulkReplace(newTasks);
          alert('CSV 가져오기 완료!');
        }
      } catch (error) {
        console.error('Failed to import CSV:', error);
        alert('CSV 가져오기 실패: 파일 형식을 확인해주세요.');
      }

      // 파일 입력 초기화
      if (fileInputRef.current) {
        fileInputRef.current.value = '';
      }
    };

    reader.readAsText(file, 'UTF-8');
  };

  return (
    <div className="flex flex-col h-full p-6 overflow-hidden">
      <div className="mb-6 flex items-center justify-between">
        <div>
          <h2 className="text-2xl font-semibold mb-2 text-white">
            Product Ribbon & API Pipeline
          </h2>
          <p className="text-zinc-400">API 개발 진행 상황 추적 및 관리</p>
        </div>

        {/* 액션 버튼들 */}
        <div className="flex items-center gap-2">
          <Button
            onClick={handleRefresh}
            disabled={isRefreshing}
            variant="outline"
            size="sm"
            className="bg-zinc-800 border-zinc-700 text-zinc-100 hover:bg-zinc-700"
          >
            <RefreshCw className={`w-4 h-4 mr-2 ${isRefreshing ? 'animate-spin' : ''}`} />
            새로고침
          </Button>

          <Button
            onClick={handleExportCSV}
            variant="outline"
            size="sm"
            className="bg-zinc-800 border-zinc-700 text-zinc-100 hover:bg-zinc-700"
          >
            <Download className="w-4 h-4 mr-2" />
            CSV 내보내기
          </Button>

          <input
            ref={fileInputRef}
            type="file"
            accept=".csv"
            onChange={handleImportCSV}
            className="hidden"
          />
          <Button
            onClick={() => fileInputRef.current?.click()}
            variant="outline"
            size="sm"
            className="bg-zinc-800 border-zinc-700 text-zinc-100 hover:bg-zinc-700"
          >
            <Upload className="w-4 h-4 mr-2" />
            CSV 가져오기
          </Button>
        </div>
      </div>

      <div className="flex-1 min-h-0 mt-4">
        <ApiTable
          tasks={tasks}
          columns={columns}
          onColumnVisibilityChange={handleColumnVisibilityChange}
          onTaskEdit={handleTaskEdit}
          onTaskDelete={handleTaskDelete}
          onAddTask={handleAddTask}
        />
      </div>

      <EditTaskModal
        task={editingTask}
        open={isEditModalOpen}
        onClose={() => setIsEditModalOpen(false)}
        onSave={handleTaskSave}
      />
    </div>
  );
}
