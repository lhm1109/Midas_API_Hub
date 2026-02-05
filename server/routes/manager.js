/**
 * Manager API Routes
 * 프로젝트 관리 작업(tasks)과 컬럼 설정을 관리하는 API
 */
import express from 'express';
import { createClient } from '@supabase/supabase-js';

const router = express.Router();

const supabase = createClient(
  process.env.SUPABASE_URL,
  process.env.SUPABASE_SERVICE_KEY
);

/**
 * GET /api/manager/tasks
 * 모든 작업 조회
 */
router.get('/tasks', async (req, res) => {
  try {
    const { data: tasks, error } = await supabase
      .from('manager_tasks')
      .select('*')
      .order('order_index', { ascending: true });

    if (error) throw error;

    // snake_case -> camelCase 변환
    const convertedTasks = (tasks || []).map(task => ({
      ...task,
      linkedEndpointId: task.linked_endpoint_id || undefined,
      linked_endpoint_id: undefined,
    }));

    res.json(convertedTasks);
  } catch (error) {
    console.error('Get manager tasks error:', error);
    res.status(500).json({ error: error.message });
  }
});

/**
 * GET /api/manager/tasks/:id
 * 특정 작업 조회
 */
router.get('/tasks/:id', async (req, res) => {
  try {
    const { id } = req.params;
    const { data: task, error } = await supabase
      .from('manager_tasks')
      .select('*')
      .eq('id', id)
      .single();

    if (error) {
      if (error.code === 'PGRST116') {
        return res.status(404).json({ error: 'Task not found' });
      }
      throw error;
    }

    res.json(task);
  } catch (error) {
    console.error('Get manager task error:', error);
    res.status(500).json({ error: error.message });
  }
});

/**
 * POST /api/manager/tasks
 * 새 작업 생성
 */
router.post('/tasks', async (req, res) => {
  try {
    const taskData = req.body;
    const now = new Date().toISOString();

    if (!taskData.id) {
      return res.status(400).json({ error: 'id is required' });
    }

    const { data, error } = await supabase
      .from('manager_tasks')
      .insert({
        ...taskData,
        // camelCase -> snake_case 변환
        linked_endpoint_id: taskData.linkedEndpointId || null,
        linkedEndpointId: undefined, // 원본 필드 제거
        created_at: now,
        updated_at: now,
      })
      .select()
      .single();

    if (error) throw error;

    res.status(201).json(data);
  } catch (error) {
    console.error('Create manager task error:', error);
    res.status(500).json({ error: error.message });
  }
});

/**
 * PUT /api/manager/tasks/:id
 * 작업 업데이트
 */
router.put('/tasks/:id', async (req, res) => {
  try {
    const { id } = req.params;
    const taskData = req.body;
    const now = new Date().toISOString();

    // id 필드는 업데이트하지 않음
    const { id: _, created_at, linkedEndpointId, ...updateData } = taskData;

    const { data, error } = await supabase
      .from('manager_tasks')
      .update({
        ...updateData,
        // camelCase -> snake_case 변환
        linked_endpoint_id: linkedEndpointId || null,
        updated_at: now,
      })
      .eq('id', id)
      .select()
      .single();

    if (error) {
      if (error.code === 'PGRST116') {
        return res.status(404).json({ error: 'Task not found' });
      }
      throw error;
    }

    // snake_case -> camelCase 변환하여 응답
    const convertedData = {
      ...data,
      linkedEndpointId: data.linked_endpoint_id || undefined,
      linked_endpoint_id: undefined,
    };

    res.json(convertedData);
  } catch (error) {
    console.error('Update manager task error:', error);
    res.status(500).json({ error: error.message });
  }
});

/**
 * DELETE /api/manager/tasks/:id
 * 작업 삭제
 */
router.delete('/tasks/:id', async (req, res) => {
  try {
    const { id } = req.params;

    const { error } = await supabase
      .from('manager_tasks')
      .delete()
      .eq('id', id);

    if (error) throw error;

    res.json({ message: 'Task deleted successfully' });
  } catch (error) {
    console.error('Delete manager task error:', error);
    res.status(500).json({ error: error.message });
  }
});

/**
 * GET /api/manager/columns
 * 컬럼 설정 조회
 */
router.get('/columns', async (req, res) => {
  try {
    const { data: columns, error } = await supabase
      .from('manager_columns')
      .select('*')
      .order('order_index', { ascending: true });

    if (error) throw error;

    res.json(columns || []);
  } catch (error) {
    console.error('Get manager columns error:', error);
    res.status(500).json({ error: error.message });
  }
});

/**
 * PUT /api/manager/columns
 * 컬럼 설정 전체 업데이트 (배치)
 */
router.put('/columns', async (req, res) => {
  try {
    const columns = req.body;

    if (!Array.isArray(columns)) {
      return res.status(400).json({ error: 'columns must be an array' });
    }

    const now = new Date().toISOString();

    // 기존 컬럼 모두 삭제
    await supabase.from('manager_columns').delete().neq('id', '');

    // 새 컬럼 삽입
    const columnsToInsert = columns.map((col, index) => ({
      ...col,
      order_index: index,
      updated_at: now,
    }));

    const { data, error } = await supabase
      .from('manager_columns')
      .insert(columnsToInsert)
      .select();

    if (error) throw error;

    res.json(data);
  } catch (error) {
    console.error('Update manager columns error:', error);
    res.status(500).json({ error: error.message });
  }
});

/**
 * PUT /api/manager/columns/:id/visibility
 * 특정 컬럼의 표시/숨김 상태 변경
 */
router.put('/columns/:id/visibility', async (req, res) => {
  try {
    const { id } = req.params;
    const { visible } = req.body;
    const now = new Date().toISOString();

    if (typeof visible !== 'boolean') {
      return res.status(400).json({ error: 'visible must be a boolean' });
    }

    const { data, error } = await supabase
      .from('manager_columns')
      .update({
        visible,
        updated_at: now,
      })
      .eq('id', id)
      .select()
      .single();

    if (error) {
      if (error.code === 'PGRST116') {
        return res.status(404).json({ error: 'Column not found' });
      }
      throw error;
    }

    res.json(data);
  } catch (error) {
    console.error('Update column visibility error:', error);
    res.status(500).json({ error: error.message });
  }
});

/**
 * POST /api/manager/tasks/bulk-replace
 * 모든 작업을 새 데이터로 교체 (덮어쓰기)
 */
router.post('/tasks/bulk-replace', async (req, res) => {
  try {
    const tasks = req.body;

    if (!Array.isArray(tasks)) {
      return res.status(400).json({ error: 'tasks must be an array' });
    }

    const now = new Date().toISOString();

    // 기존 작업 모두 삭제
    await supabase.from('manager_tasks').delete().neq('id', '');

    // 새 작업 삽입
    if (tasks.length > 0) {
      const tasksToInsert = tasks.map((task) => ({
        ...task,
        created_at: task.created_at || now,
        updated_at: now,
      }));

      const { data, error } = await supabase
        .from('manager_tasks')
        .insert(tasksToInsert)
        .select();

      if (error) throw error;

      res.json(data);
    } else {
      res.json([]);
    }
  } catch (error) {
    console.error('Bulk replace manager tasks error:', error);
    res.status(500).json({ error: error.message });
  }
});

export default router;
