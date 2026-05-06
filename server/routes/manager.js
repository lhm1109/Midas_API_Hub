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

function normalizeEndpointKey(value) {
  if (!value || typeof value !== 'string') return null;
  const normalized = value.trim().replace(/^\/+/, '').replace(/\/+$/, '').toLowerCase();
  return normalized || null;
}

async function resolveTaskZendeskUrls(tasks) {
  const safeTasks = Array.isArray(tasks) ? tasks : [];
  if (safeTasks.length === 0) return new Map();

  const { data: endpoints, error: endpointsError } = await supabase
    .from('endpoints')
    .select('id, path');
  if (endpointsError) throw endpointsError;

  const endpointIdByKey = new Map();
  const endpointIdsByPathKey = new Map();
  for (const endpoint of endpoints || []) {
    const idKey = normalizeEndpointKey(endpoint?.id);
    const pathKey = normalizeEndpointKey(endpoint?.path);
    if (idKey && !endpointIdByKey.has(idKey)) endpointIdByKey.set(idKey, endpoint.id);
    if (pathKey) {
      const existing = endpointIdsByPathKey.get(pathKey) || [];
      endpointIdsByPathKey.set(pathKey, [...existing, endpoint.id]);
      if (!endpointIdByKey.has(pathKey)) endpointIdByKey.set(pathKey, endpoint.id);
    }
  }

  const endpointCandidatesByTaskId = new Map();
  for (const task of safeTasks) {
    const taskId = task?.id;
    if (!taskId) continue;

    const linkedKey = normalizeEndpointKey(task?.linked_endpoint_id);
    const pathKey = normalizeEndpointKey(task?.endPoint);

    const linkedEndpointId = linkedKey ? endpointIdByKey.get(linkedKey) : null;
    const pathEndpointIds = pathKey ? (endpointIdsByPathKey.get(pathKey) || []) : [];
    endpointCandidatesByTaskId.set(taskId, { linkedEndpointId, pathEndpointIds });
  }

  const endpointIds = Array.from(
    new Set(
      Array.from(endpointCandidatesByTaskId.values())
        .flatMap((candidate) => [candidate?.linkedEndpointId, ...(candidate?.pathEndpointIds || [])])
        .filter(Boolean)
    )
  );
  if (endpointIds.length === 0) return new Map();

  const { data: versions, error: versionsError } = await supabase
    .from('versions')
    .select('id, endpoint_id, updated_at, created_at')
    .in('endpoint_id', endpointIds)
    .order('updated_at', { ascending: false });
  if (versionsError) throw versionsError;

  const latestVersionByEndpoint = new Map();
  for (const version of versions || []) {
    if (!version?.endpoint_id || latestVersionByEndpoint.has(version.endpoint_id)) continue;
    latestVersionByEndpoint.set(version.endpoint_id, version.id);
  }

  const latestVersionIds = Array.from(latestVersionByEndpoint.values()).filter(Boolean);
  if (latestVersionIds.length === 0) return new Map();

  const { data: manualRows, error: manualError } = await supabase
    .from('manual_data')
    .select('version_id, url')
    .in('version_id', latestVersionIds);
  if (manualError) throw manualError;

  const manualUrlByVersionId = new Map(
    (manualRows || [])
      .filter((row) => row?.url && String(row.url).trim() !== '')
      .map((row) => [row.version_id, String(row.url).trim()])
  );

  const zendeskUrlByEndpointId = new Map();
  for (const [endpointId, versionId] of latestVersionByEndpoint.entries()) {
    zendeskUrlByEndpointId.set(endpointId, manualUrlByVersionId.get(versionId) || '');
  }

  const zendeskUrlByTaskId = new Map();
  for (const [taskId, candidate] of endpointCandidatesByTaskId.entries()) {
    const linkedUrl = candidate?.linkedEndpointId ? zendeskUrlByEndpointId.get(candidate.linkedEndpointId) : '';
    const pathUrl =
      (candidate?.pathEndpointIds || [])
        .map((endpointId) => zendeskUrlByEndpointId.get(endpointId) || '')
        .find((url) => Boolean(url)) || '';
    zendeskUrlByTaskId.set(taskId, linkedUrl || pathUrl || '');
  }

  return zendeskUrlByTaskId;
}

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

    const zendeskUrlByTaskId = await resolveTaskZendeskUrls(tasks || []);

    // snake_case -> camelCase 변환
    const convertedTasks = (tasks || []).map(task => ({
      ...task,
      zendeskUrl: zendeskUrlByTaskId.get(task.id) || '',
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

    const zendeskUrlByTaskId = await resolveTaskZendeskUrls([task]);
    res.json({
      ...task,
      zendeskUrl: zendeskUrlByTaskId.get(task.id) || '',
      linkedEndpointId: task.linked_endpoint_id || undefined,
      linked_endpoint_id: undefined,
    });
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

    const { zendeskUrl, url, ...insertableTaskData } = taskData;

    const { data, error } = await supabase
      .from('manager_tasks')
      .insert({
        ...insertableTaskData,
        // camelCase -> snake_case 변환
        linked_endpoint_id: insertableTaskData.linkedEndpointId || null,
        linkedEndpointId: undefined, // 원본 필드 제거
        created_at: now,
        updated_at: now,
      })
      .select()
      .single();

    if (error) throw error;

    const zendeskUrlByTaskId = await resolveTaskZendeskUrls([data]);
    const convertedData = {
      ...data,
      zendeskUrl: zendeskUrlByTaskId.get(data.id) || '',
      linkedEndpointId: data.linked_endpoint_id || undefined,
      linked_endpoint_id: undefined,
    };

    res.status(201).json(convertedData);
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
    const { id: _, created_at, linkedEndpointId, zendeskUrl, url, ...updateData } = taskData;

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
    const zendeskUrlByTaskId = await resolveTaskZendeskUrls([data]);
    const convertedData = {
      ...data,
      zendeskUrl: zendeskUrlByTaskId.get(data.id) || '',
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
        zendeskUrl: undefined,
        url: undefined,
        linked_endpoint_id: task.linkedEndpointId || null,
        linkedEndpointId: undefined,
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
