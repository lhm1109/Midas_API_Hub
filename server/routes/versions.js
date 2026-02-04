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
      responseBody: tc.response_body || undefined,       // 🔥 Response 추가
      responseStatus: tc.response_status || undefined,   // 🔥 HTTP 상태 코드
      responseTime: tc.response_time || undefined,       // 🔥 응답 시간
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

    // 새 test cases 삽입 (🔥 response 필드 포함)
    const testCasesData = data.testCases.map(tc => ({
      id: tc.id,
      version_id: versionId,
      name: tc.name,
      description: tc.description,
      request_body: tc.requestBody,
      response_body: tc.responseBody || null,        // 🔥 Response 저장
      response_status: tc.responseStatus || null,    // 🔥 HTTP 상태 코드
      response_time: tc.responseTime || null,        // 🔥 응답 시간
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

// 🔥 버전 Export - JSON 파일로 내보내기
router.get('/:id/export', async (req, res) => {
  try {
    const { id } = req.params;

    // 버전 기본 정보
    const { data: version, error: versionError } = await supabase
      .from('versions')
      .select('*')
      .eq('id', id)
      .single();

    if (versionError) throw versionError;
    if (!version) {
      return res.status(404).json({ error: 'Version not found' });
    }

    // Manual 데이터
    const { data: manualData } = await supabase
      .from('manual_data')
      .select('*')
      .eq('version_id', id)
      .single();

    // Spec 데이터
    const { data: specData } = await supabase
      .from('spec_data')
      .select('*')
      .eq('version_id', id)
      .single();

    // Builder 데이터
    const { data: builderData } = await supabase
      .from('builder_data')
      .select('*')
      .eq('version_id', id)
      .single();

    // Runner 데이터 및 테스트 케이스
    const { data: runnerData } = await supabase
      .from('runner_data')
      .select('*')
      .eq('version_id', id)
      .single();

    const { data: testCases } = await supabase
      .from('test_cases')
      .select('*')
      .eq('version_id', id);

    // Export JSON 구조
    const exportData = {
      // 메타 정보
      exportedAt: new Date().toISOString(),
      exportVersion: '1.0',

      // 버전 기본 정보
      version: {
        version: version.version,
        changeLog: version.change_log,
        author: version.author,
        createdAt: version.created_at,
      },

      // 스키마 (Original + Enhanced)
      schemas: {
        original: safeParseJSON(specData?.json_schema_original, null),
        enhanced: safeParseJSON(specData?.json_schema_enhanced, null),
        // 레거시 스키마 (호환성)
        jsonSchema: safeParseJSON(specData?.json_schema, {}),
      },

      // 예제 데이터
      examples: {
        request: safeParseJSON(manualData?.request_examples, []),
        response: safeParseJSON(manualData?.response_examples, []),
        // 레거시 examples (호환성)
        legacy: safeParseJSON(manualData?.examples, []),
      },

      // 매뉴얼 내용
      manual: {
        title: manualData?.title || '',
        category: manualData?.category || '',
        specifications: manualData?.specifications || '',
        htmlContent: manualData?.html_content || null,
      },

      // Builder 데이터 (선택적)
      builder: builderData ? {
        formData: safeParseJSON(builderData.form_data, {}),
      } : null,

      // Runner 데이터 (선택적)
      runner: runnerData ? {
        url: runnerData.url,
        method: runnerData.method,
        headers: safeParseJSON(runnerData.headers, {}),
        body: safeParseJSON(runnerData.body, {}),
        testCases: testCases?.map(tc => ({
          name: tc.name,
          description: tc.description,
          input: safeParseJSON(tc.input, {}),
          expectedOutput: safeParseJSON(tc.expected_output, {}),
        })) || [],
      } : null,
    };

    res.json(exportData);
  } catch (error) {
    console.error('Error exporting version:', error);
    res.status(500).json({ error: error.message });
  }
});

// 🔥 버전 Import - JSON 파일에서 가져오기
router.post('/import', async (req, res) => {
  try {
    const { endpointId, importData } = req.body;

    if (!endpointId || !importData) {
      return res.status(400).json({ error: 'endpointId and importData are required' });
    }

    // 버전 생성
    const versionData = {
      endpoint_id: endpointId,
      version: importData.version?.version || 'Imported',
      change_log: importData.version?.changeLog || 'Imported from JSON',
      author: importData.version?.author || 'System',
      created_at: new Date().toISOString(),
    };

    const { data: newVersion, error: versionError } = await supabase
      .from('versions')
      .insert([versionData])
      .select()
      .single();

    if (versionError) throw versionError;

    const versionId = newVersion.id;

    // Manual 데이터 저장
    if (importData.manual || importData.examples) {
      const manualInsert = {
        version_id: versionId,
        title: importData.manual?.title || '',
        category: importData.manual?.category || '',
        specifications: importData.manual?.specifications || '',
        html_content: importData.manual?.htmlContent || null,
        request_examples: JSON.stringify(importData.examples?.request || []),
        response_examples: JSON.stringify(importData.examples?.response || []),
        examples: JSON.stringify(importData.examples?.legacy || []),
      };

      const { error: manualError } = await supabase
        .from('manual_data')
        .insert([manualInsert]);

      if (manualError) console.error('Manual data import error:', manualError);
    }

    // Spec 데이터 저장
    if (importData.schemas) {
      const specInsert = {
        version_id: versionId,
        json_schema: JSON.stringify(importData.schemas.jsonSchema || {}),
        json_schema_original: JSON.stringify(importData.schemas.original || null),
        json_schema_enhanced: JSON.stringify(importData.schemas.enhanced || null),
        specifications: importData.manual?.specifications || '',
      };

      const { error: specError } = await supabase
        .from('spec_data')
        .insert([specInsert]);

      if (specError) console.error('Spec data import error:', specError);
    }

    // Builder 데이터 저장 (선택적)
    if (importData.builder) {
      const builderInsert = {
        version_id: versionId,
        form_data: JSON.stringify(importData.builder.formData || {}),
      };

      const { error: builderError } = await supabase
        .from('builder_data')
        .insert([builderInsert]);

      if (builderError) console.error('Builder data import error:', builderError);
    }

    // Runner 데이터 저장 (선택적)
    if (importData.runner) {
      const runnerInsert = {
        version_id: versionId,
        url: importData.runner.url || '',
        method: importData.runner.method || 'POST',
        headers: JSON.stringify(importData.runner.headers || {}),
        body: JSON.stringify(importData.runner.body || {}),
      };

      const { data: newRunner, error: runnerError } = await supabase
        .from('runner_data')
        .insert([runnerInsert])
        .select()
        .single();

      if (runnerError) {
        console.error('Runner data import error:', runnerError);
      } else if (importData.runner.testCases && importData.runner.testCases.length > 0) {
        // 테스트 케이스 저장
        const testCaseInserts = importData.runner.testCases.map(tc => ({
          version_id: versionId,
          name: tc.name,
          description: tc.description || '',
          input: JSON.stringify(tc.input || {}),
          expected_output: JSON.stringify(tc.expectedOutput || {}),
        }));

        const { error: testCaseError } = await supabase
          .from('test_cases')
          .insert(testCaseInserts);

        if (testCaseError) console.error('Test case import error:', testCaseError);
      }
    }

    res.json({
      message: 'Version imported successfully',
      versionId: versionId,
      version: newVersion,
    });
  } catch (error) {
    console.error('Error importing version:', error);
    res.status(500).json({ error: error.message });
  }
});

// Update version metadata (e.g., changeLog)
router.patch('/:versionId', async (req, res) => {
  try {
    const { versionId } = req.params;
    const {
      version,
      changeLog,
      jsonSchema,
      jsonSchemaOriginal,
      jsonSchemaEnhanced,
      requestExample,
      responseExample,
      runnerData
    } = req.body;

    // 🔥 Build update object with only provided fields
    const updateData = {
      updated_at: new Date().toISOString()
    };

    if (version !== undefined) updateData.version = version;
    if (changeLog !== undefined) updateData.change_log = changeLog;

    // Update version table
    const { error: updateError } = await supabase
      .from('versions')
      .update(updateData)
      .eq('id', versionId);

    if (updateError) {
      throw updateError;
    }

    // 🔥 Update spec_data if schema fields provided
    if (jsonSchema !== undefined || jsonSchemaOriginal !== undefined || jsonSchemaEnhanced !== undefined) {
      const specUpdateData = {};
      if (jsonSchema !== undefined) specUpdateData.json_schema = typeof jsonSchema === 'string' ? jsonSchema : JSON.stringify(jsonSchema);
      if (jsonSchemaOriginal !== undefined) specUpdateData.json_schema_original = typeof jsonSchemaOriginal === 'string' ? jsonSchemaOriginal : JSON.stringify(jsonSchemaOriginal);
      if (jsonSchemaEnhanced !== undefined) specUpdateData.json_schema_enhanced = typeof jsonSchemaEnhanced === 'string' ? jsonSchemaEnhanced : JSON.stringify(jsonSchemaEnhanced);

      const { error: specError } = await supabase
        .from('spec_data')
        .upsert({
          version_id: versionId,
          ...specUpdateData
        }, { onConflict: 'version_id' });

      if (specError) console.error('Error updating spec_data:', specError);
    }

    // 🔥 Update runner_data if provided
    if (runnerData !== undefined) {
      const { error: runnerError } = await supabase
        .from('runner_data')
        .upsert({
          version_id: versionId,
          request_body: runnerData.requestBody || '{}',
          response_body: runnerData.responseBody || ''
        }, { onConflict: 'version_id' });

      if (runnerError) console.error('Error updating runner_data:', runnerError);
    }

    // Get updated version
    const { data: updatedVersion, error: fetchError } = await supabase
      .from('versions')
      .select('*')
      .eq('id', versionId)
      .single();

    if (fetchError) {
      throw fetchError;
    }

    res.json({
      message: 'Version updated successfully',
      version: updatedVersion
    });
  } catch (error) {
    console.error('Error updating version:', error);
    res.status(500).json({ error: error.message });
  }
});

export default router;
