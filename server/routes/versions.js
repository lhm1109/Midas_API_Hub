import express from 'express';
import supabase from '../database.js';

const router = express.Router();

// Helper functions
const safeParseJSON = (jsonString, defaultValue = {}) => {
  if (!jsonString || jsonString === 'null' || jsonString === 'undefined') {
    return defaultValue;
  }
  try {
    return typeof jsonString === 'string' ? JSON.parse(jsonString) : jsonString;
  } catch (error) {
    console.error('Failed to parse JSON:', error.message);
    return defaultValue;
  }
};

function parseManualData(data) {
  return {
    title: data.title || '',
    category: data.category || '',
    inputUri: data.input_uri || '',
    activeMethods: data.active_methods || '',
    jsonSchema: safeParseJSON(data.json_schema, {}),
    jsonSchemaOriginal: safeParseJSON(data.json_schema_original, null),
    jsonSchemaEnhanced: safeParseJSON(data.json_schema_enhanced, null),
    examples: safeParseJSON(data.examples, []),
    requestExamples: safeParseJSON(data.request_examples, []),
    responseExamples: safeParseJSON(data.response_examples, []),
    specifications: data.specifications || '',
    htmlContent: data.html_content,
    articleId: data.article_id,
    sectionId: data.section_id,
    authorId: data.author_id,
    url: data.url,
  };
}

function parseSpecData(data) {
  return {
    jsonSchema: safeParseJSON(data.json_schema, {}),
    jsonSchemaOriginal: safeParseJSON(data.json_schema_original, null),
    jsonSchemaEnhanced: safeParseJSON(data.json_schema_enhanced, null),
    specifications: data.specifications || '',
  };
}

function parseRunnerData(data, testCases) {
  return {
    requestBody: data?.request_body || '{}',
    responseBody: data?.response_body || '',
    testCases: (testCases || []).map(tc => ({
      id: tc.id,
      name: tc.name,
      description: tc.description,
      requestBody: tc.request_body,
      createdAt: tc.created_at,
      updatedAt: tc.updated_at,
    })),
  };
}

async function saveManualData(versionId, data) {
  const { error } = await supabase
    .from('manual_data')
    .upsert({
      version_id: versionId,
      title: data.title,
      category: data.category,
      input_uri: data.inputUri,
      active_methods: data.activeMethods,
      json_schema: typeof data.jsonSchema === 'string' ? data.jsonSchema : JSON.stringify(data.jsonSchema || {}),
      json_schema_original: typeof data.jsonSchemaOriginal === 'string' ? data.jsonSchemaOriginal : (data.jsonSchemaOriginal ? JSON.stringify(data.jsonSchemaOriginal) : null),
      json_schema_enhanced: typeof data.jsonSchemaEnhanced === 'string' ? data.jsonSchemaEnhanced : (data.jsonSchemaEnhanced ? JSON.stringify(data.jsonSchemaEnhanced) : null),
      examples: JSON.stringify(data.examples || []),
      request_examples: JSON.stringify(data.requestExamples || []),
      response_examples: JSON.stringify(data.responseExamples || []),
      specifications: data.specifications,
      html_content: data.htmlContent,
      article_id: data.articleId,
      section_id: data.sectionId,
      author_id: data.authorId,
      url: data.url
    }, { onConflict: 'version_id' });
  
  if (error) throw error;
}

async function saveSpecData(versionId, data) {
  const { error } = await supabase
    .from('spec_data')
    .upsert({
      version_id: versionId,
      json_schema: typeof data.jsonSchema === 'string' ? data.jsonSchema : JSON.stringify(data.jsonSchema || {}),
      json_schema_original: typeof data.jsonSchemaOriginal === 'string' ? data.jsonSchemaOriginal : (data.jsonSchemaOriginal ? JSON.stringify(data.jsonSchemaOriginal) : null),
      json_schema_enhanced: typeof data.jsonSchemaEnhanced === 'string' ? data.jsonSchemaEnhanced : (data.jsonSchemaEnhanced ? JSON.stringify(data.jsonSchemaEnhanced) : null),
      specifications: data.specifications
    }, { onConflict: 'version_id' });
  
  if (error) throw error;
}

async function saveBuilderData(versionId, data) {
  const { error } = await supabase
    .from('builder_data')
    .upsert({
      version_id: versionId,
      form_data: JSON.stringify(data.formData || {})
    }, { onConflict: 'version_id' });
  
  if (error) throw error;
}

async function saveRunnerData(versionId, data) {
  // Runner data 저장
  const { error: runnerError } = await supabase
    .from('runner_data')
    .upsert({
      version_id: versionId,
      request_body: data.requestBody,
      response_body: data.responseBody
    }, { onConflict: 'version_id' });
  
  if (runnerError) throw runnerError;
  
  // Test cases 저장
  if (data.testCases && data.testCases.length > 0) {
    // 기존 test cases 삭제
    await supabase
      .from('test_cases')
      .delete()
      .eq('version_id', versionId);
    
    // 새 test cases 삽입
    const testCasesData = data.testCases.map(tc => ({
      id: tc.id,
      version_id: versionId,
      name: tc.name,
      description: tc.description,
      request_body: tc.requestBody,
      created_at: tc.createdAt,
      updated_at: tc.updatedAt
    }));
    
    const { error: testCasesError } = await supabase
      .from('test_cases')
      .insert(testCasesData);
    
    if (testCasesError) throw testCasesError;
  }
}

// 모든 버전 조회 (엔드포인트별 필터링 가능)
router.get('/', async (req, res) => {
  try {
    const { endpoint_id } = req.query;
    
    let query = supabase
      .from('versions')
      .select('*')
      .order('created_at', { ascending: false });
    
    if (endpoint_id) {
      query = query.eq('endpoint_id', endpoint_id);
    }
    
    const { data: versions, error } = await query;
    if (error) throw error;
    
    // 각 버전에 연관 데이터 추가
    const versionsWithData = await Promise.all((versions || []).map(async version => {
      const [manualResult, specResult, builderResult, runnerResult, testCasesResult] = await Promise.all([
        supabase.from('manual_data').select('*').eq('version_id', version.id).single(),
        supabase.from('spec_data').select('*').eq('version_id', version.id).single(),
        supabase.from('builder_data').select('*').eq('version_id', version.id).single(),
        supabase.from('runner_data').select('*').eq('version_id', version.id).single(),
        supabase.from('test_cases').select('*').eq('version_id', version.id)
      ]);
      
      const manualData = manualResult.error ? null : manualResult.data;
      const specData = specResult.error ? null : specResult.data;
      const builderData = builderResult.error ? null : builderResult.data;
      const runnerData = runnerResult.error ? null : runnerResult.data;
      const testCases = testCasesResult.error ? [] : testCasesResult.data;
      
      return {
        id: version.id,
        version: version.version,
        endpointId: version.endpoint_id,
        createdAt: version.created_at,
        updatedAt: version.updated_at,
        author: version.author,
        changeLog: version.change_log,
        manualData: manualData ? parseManualData(manualData) : {
          title: '',
          category: '',
          inputUri: '',
          activeMethods: '',
          jsonSchema: {},
          jsonSchemaOriginal: null,
          jsonSchemaEnhanced: null,
          examples: [],
          requestExamples: [],
          responseExamples: [],
          specifications: '',
          htmlContent: null,
        },
        specData: specData ? parseSpecData(specData) : {
          jsonSchema: {},
          jsonSchemaOriginal: null,
          jsonSchemaEnhanced: null,
          specifications: '',
        },
        builderData: builderData ? { formData: safeParseJSON(builderData.form_data, {}) } : { formData: {} },
        runnerData: parseRunnerData(runnerData, testCases),
      };
    }));
    
    res.json(versionsWithData);
  } catch (error) {
    console.error('Error fetching versions:', error);
    res.status(500).json({ error: error.message });
  }
});

// 특정 버전 조회
router.get('/:id', async (req, res) => {
  try {
    const { id } = req.params;
    
    const { data: version, error } = await supabase
      .from('versions')
      .select('*')
      .eq('id', id)
      .single();
    
    if (error) {
      if (error.code === 'PGRST116') {
        return res.status(404).json({ error: 'Version not found' });
      }
      throw error;
    }
    
    const [manualResult, specResult, builderResult, runnerResult, testCasesResult] = await Promise.all([
      supabase.from('manual_data').select('*').eq('version_id', id).single(),
      supabase.from('spec_data').select('*').eq('version_id', id).single(),
      supabase.from('builder_data').select('*').eq('version_id', id).single(),
      supabase.from('runner_data').select('*').eq('version_id', id).single(),
      supabase.from('test_cases').select('*').eq('version_id', id)
    ]);
    
    const manualData = manualResult.error ? null : manualResult.data;
    const specData = specResult.error ? null : specResult.data;
    const builderData = builderResult.error ? null : builderResult.data;
    const runnerData = runnerResult.error ? null : runnerResult.data;
    const testCases = testCasesResult.error ? [] : testCasesResult.data;
    
    res.json({
      id: version.id,
      version: version.version,
      endpointId: version.endpoint_id,
      createdAt: version.created_at,
      updatedAt: version.updated_at,
      author: version.author,
      changeLog: version.change_log,
      manualData: manualData ? parseManualData(manualData) : {
        title: '',
        category: '',
        inputUri: '',
        activeMethods: '',
        jsonSchema: {},
        jsonSchemaOriginal: null,
        jsonSchemaEnhanced: null,
        examples: [],
        requestExamples: [],
        responseExamples: [],
        specifications: '',
        htmlContent: null,
      },
      specData: specData ? parseSpecData(specData) : {
        jsonSchema: {},
        jsonSchemaOriginal: null,
        jsonSchemaEnhanced: null,
        specifications: '',
      },
      builderData: builderData ? { formData: safeParseJSON(builderData.form_data, {}) } : { formData: {} },
      runnerData: parseRunnerData(runnerData, testCases),
    });
  } catch (error) {
    console.error('Error fetching version:', error);
    res.status(500).json({ error: error.message });
  }
});

// 새 버전 생성
router.post('/', async (req, res) => {
  try {
    const { id, version, endpointId, changeLog, author, manualData, specData, builderData, runnerData } = req.body;
    
    const now = new Date().toISOString();
    
    // 버전 생성
    const { error: versionError } = await supabase
      .from('versions')
      .insert({
        id,
        version,
        endpoint_id: endpointId,
        created_at: now,
        updated_at: now,
        author: author || null,
        change_log: changeLog || null
      });
    
    if (versionError) throw versionError;
    
    // 연관 데이터 저장
    if (manualData) {
      await saveManualData(id, manualData);
    }
    if (specData) {
      await saveSpecData(id, specData);
    }
    if (builderData) {
      await saveBuilderData(id, builderData);
    }
    if (runnerData) {
      await saveRunnerData(id, runnerData);
    }
    
    res.status(201).json({ id, message: 'Version created successfully' });
  } catch (error) {
    console.error('Error creating version:', error);
    res.status(500).json({ error: error.message });
  }
});

// 버전 업데이트
router.put('/:id', async (req, res) => {
  try {
    const { id } = req.params;
    const { version, changeLog, author, manualData, specData, builderData, runnerData } = req.body;
    
    const now = new Date().toISOString();
    
    // 버전 업데이트
    const updateData = { updated_at: now };
    if (version) updateData.version = version;
    if (changeLog !== undefined) updateData.change_log = changeLog;
    if (author !== undefined) updateData.author = author;
    
    const { error: versionError } = await supabase
      .from('versions')
      .update(updateData)
      .eq('id', id);
    
    if (versionError) throw versionError;
    
    // 연관 데이터 업데이트
    if (manualData) {
      await saveManualData(id, manualData);
    }
    if (specData) {
      await saveSpecData(id, specData);
    }
    if (builderData) {
      await saveBuilderData(id, builderData);
    }
    if (runnerData) {
      await saveRunnerData(id, runnerData);
    }
    
    res.json({ message: 'Version updated successfully' });
  } catch (error) {
    console.error('Error updating version:', error);
    res.status(500).json({ error: error.message });
  }
});

// 버전 삭제
router.delete('/:id', async (req, res) => {
  try {
    const { id } = req.params;
    
    const { error } = await supabase
      .from('versions')
      .delete()
      .eq('id', id);
    
    if (error) throw error;
    
    res.json({ message: 'Version deleted successfully' });
  } catch (error) {
    console.error('Error deleting version:', error);
    res.status(500).json({ error: error.message });
  }
});

export default router;
