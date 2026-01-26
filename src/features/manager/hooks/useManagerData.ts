import { useState, useEffect, useCallback } from 'react';
import { ApiTask, Column } from '../types/manager';

const API_BASE_URL = 'http://localhost:9527/api/manager';

export function useManagerData() {
  const [tasks, setTasks] = useState<ApiTask[]>([]);
  const [columns, setColumns] = useState<Column[]>([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);

  // Tasks 조회
  const fetchTasks = useCallback(async () => {
    try {
      const response = await fetch(`${API_BASE_URL}/tasks`);
      if (!response.ok) throw new Error('Failed to fetch tasks');
      const data = await response.json();
      setTasks(data);
    } catch (err) {
      console.error('Error fetching tasks:', err);
      setError(err instanceof Error ? err.message : 'Unknown error');
    }
  }, []);

  // Columns 조회
  const fetchColumns = useCallback(async () => {
    try {
      const response = await fetch(`${API_BASE_URL}/columns`);
      if (!response.ok) throw new Error('Failed to fetch columns');
      const data = await response.json();
      setColumns(data);
    } catch (err) {
      console.error('Error fetching columns:', err);
      setError(err instanceof Error ? err.message : 'Unknown error');
    }
  }, []);

  // 초기 데이터 로드
  useEffect(() => {
    const loadData = async () => {
      setLoading(true);
      setError(null);
      try {
        await Promise.all([fetchTasks(), fetchColumns()]);
      } catch (err) {
        console.error('Error loading manager data:', err);
      } finally {
        setLoading(false);
      }
    };

    loadData();
  }, [fetchTasks, fetchColumns]);

  // Task 추가
  const addTask = useCallback(async (task: ApiTask) => {
    try {
      const response = await fetch(`${API_BASE_URL}/tasks`, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(task),
      });
      if (!response.ok) throw new Error('Failed to add task');
      const newTask = await response.json();
      setTasks((prev) => [...prev, newTask]);
      return newTask;
    } catch (err) {
      console.error('Error adding task:', err);
      throw err;
    }
  }, []);

  // Task 수정
  const updateTask = useCallback(async (task: ApiTask) => {
    try {
      const response = await fetch(`${API_BASE_URL}/tasks/${task.id}`, {
        method: 'PUT',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(task),
      });
      if (!response.ok) throw new Error('Failed to update task');
      const updatedTask = await response.json();
      setTasks((prev) =>
        prev.map((t) => (t.id === updatedTask.id ? updatedTask : t))
      );
      return updatedTask;
    } catch (err) {
      console.error('Error updating task:', err);
      throw err;
    }
  }, []);

  // Task 삭제
  const deleteTask = useCallback(async (taskId: string) => {
    try {
      const response = await fetch(`${API_BASE_URL}/tasks/${taskId}`, {
        method: 'DELETE',
      });
      if (!response.ok) throw new Error('Failed to delete task');
      setTasks((prev) => prev.filter((t) => t.id !== taskId));
    } catch (err) {
      console.error('Error deleting task:', err);
      throw err;
    }
  }, []);

  // Column 표시/숨김 변경
  const updateColumnVisibility = useCallback(
    async (columnId: string, visible: boolean) => {
      try {
        const response = await fetch(
          `${API_BASE_URL}/columns/${columnId}/visibility`,
          {
            method: 'PUT',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ visible }),
          }
        );
        if (!response.ok) throw new Error('Failed to update column visibility');
        const updatedColumn = await response.json();
        setColumns((prev) =>
          prev.map((col) =>
            col.id === updatedColumn.id ? updatedColumn : col
          )
        );
      } catch (err) {
        console.error('Error updating column visibility:', err);
        throw err;
      }
    },
    []
  );

  // Columns 전체 업데이트 (순서 변경 등)
  const updateColumns = useCallback(async (newColumns: Column[]) => {
    try {
      const response = await fetch(`${API_BASE_URL}/columns`, {
        method: 'PUT',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(newColumns),
      });
      if (!response.ok) throw new Error('Failed to update columns');
      const updatedColumns = await response.json();
      setColumns(updatedColumns);
    } catch (err) {
      console.error('Error updating columns:', err);
      throw err;
    }
  }, []);

  // Tasks 전체 교체 (CSV Import용)
  const bulkReplaceTasks = useCallback(async (newTasks: ApiTask[]) => {
    try {
      const response = await fetch(`${API_BASE_URL}/tasks/bulk-replace`, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(newTasks),
      });
      if (!response.ok) throw new Error('Failed to replace tasks');
      const replacedTasks = await response.json();
      setTasks(replacedTasks);
      return replacedTasks;
    } catch (err) {
      console.error('Error replacing tasks:', err);
      throw err;
    }
  }, []);

  return {
    tasks,
    columns,
    loading,
    error,
    addTask,
    updateTask,
    deleteTask,
    updateColumnVisibility,
    updateColumns,
    bulkReplaceTasks,
    refetch: useCallback(async () => {
      await Promise.all([fetchTasks(), fetchColumns()]);
    }, [fetchTasks, fetchColumns]),
  };
}
