import express from 'express';
import db from '../database.js';

const router = express.Router();

// 모든 버전 조회 (엔드포인트별 필터링 가능)
router.get('/', (req, res) => {
  try {
    const { endpoint_id } = req.query;
    
    let query = 'SELECT * FROM versions';
    let params = [];
    
    if (endpoint_id) {
      query += ' WHERE endpoint_id = ?';
      params.push(endpoint_id);
    }
    
    query += ' ORDER BY created_at DESC';
    
    const versions = db.prepare(query).all(...params);
    
    // 각 버전에 연관 데이터 추가
    const versionsWithData = versions.map(version => {
      const manualData = db.prepare('SELECT * FROM manual_data WHERE version_id = ?').get(version.id);
      const specData = db.prepare('SELECT * FROM spec_data WHERE version_id = ?').get(version.id);
      const builderData = db.prepare('SELECT * FROM builder_data WHERE version_id = ?').get(version.id);
      const runnerData = db.prepare('SELECT * FROM runner_data WHERE version_id = ?').get(version.id);
      const testCases = db.prepare('SELECT * FROM test_cases WHERE version_id = ?').all(version.id);
      
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
          specifications: '',
          htmlContent: null,
        },
        specData: specData ? parseSpecData(specData) : {
          jsonSchema: {},
          jsonSchemaOriginal: null,
          jsonSchemaEnhanced: null,
          specifications: '',
        },
        builderData: builderData ? JSON.parse(builderData.form_data || '{}') : { formData: {} },
        runnerData: runnerData ? parseRunnerData(runnerData, testCases) : {
          requestBody: '{}',
          responseBody: '',
          testCases: [],
        },
      };
    });
    
    res.json(versionsWithData);
  } catch (error) {
    console.error('Error fetching versions:', error);
    res.status(500).json({ error: error.message });
  }
});

// 특정 버전 조회
router.get('/:id', (req, res) => {
  try {
    const { id } = req.params;
    const version = db.prepare('SELECT * FROM versions WHERE id = ?').get(id);
    
    if (!version) {
      return res.status(404).json({ error: 'Version not found' });
    }
    
    const manualData = db.prepare('SELECT * FROM manual_data WHERE version_id = ?').get(id);
    const specData = db.prepare('SELECT * FROM spec_data WHERE version_id = ?').get(id);
    const builderData = db.prepare('SELECT * FROM builder_data WHERE version_id = ?').get(id);
    const runnerData = db.prepare('SELECT * FROM runner_data WHERE version_id = ?').get(id);
    const testCases = db.prepare('SELECT * FROM test_cases WHERE version_id = ?').all(id);
    
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
        specifications: '',
        htmlContent: null,
      },
      specData: specData ? parseSpecData(specData) : {
        jsonSchema: {},
        jsonSchemaOriginal: null,
        jsonSchemaEnhanced: null,
        specifications: '',
      },
      builderData: builderData ? JSON.parse(builderData.form_data || '{}') : { formData: {} },
      runnerData: runnerData ? parseRunnerData(runnerData, testCases) : {
        requestBody: '{}',
        responseBody: '',
        testCases: [],
      },
    });
  } catch (error) {
    console.error('Error fetching version:', error);
    res.status(500).json({ error: error.message });
  }
});

// 새 버전 생성
router.post('/', (req, res) => {
  try {
    const { id, version, endpointId, changeLog, author, manualData, specData, builderData, runnerData } = req.body;
    
    const now = new Date().toISOString();
    
    // 버전 생성
    db.prepare(`
      INSERT INTO versions (id, version, endpoint_id, created_at, updated_at, author, change_log)
      VALUES (?, ?, ?, ?, ?, ?, ?)
    `).run(id, version, endpointId, now, now, author || null, changeLog || null);
    
    // 연관 데이터 저장
    if (manualData) {
      saveManualData(id, manualData);
    }
    if (specData) {
      saveSpecData(id, specData);
    }
    if (builderData) {
      saveBuilderData(id, builderData);
    }
    if (runnerData) {
      saveRunnerData(id, runnerData);
    }
    
    res.status(201).json({ id, message: 'Version created successfully' });
  } catch (error) {
    console.error('Error creating version:', error);
    res.status(500).json({ error: error.message });
  }
});

// 버전 업데이트
router.put('/:id', (req, res) => {
  try {
    const { id } = req.params;
    const { version, changeLog, author, manualData, specData, builderData, runnerData } = req.body;
    
    const now = new Date().toISOString();
    
    // 버전 업데이트
    db.prepare(`
      UPDATE versions
      SET version = COALESCE(?, version),
          change_log = COALESCE(?, change_log),
          author = COALESCE(?, author),
          updated_at = ?
      WHERE id = ?
    `).run(version, changeLog, author, now, id);
    
    // 연관 데이터 업데이트
    if (manualData) {
      saveManualData(id, manualData);
    }
    if (specData) {
      saveSpecData(id, specData);
    }
    if (builderData) {
      saveBuilderData(id, builderData);
    }
    if (runnerData) {
      saveRunnerData(id, runnerData);
    }
    
    res.json({ message: 'Version updated successfully' });
  } catch (error) {
    console.error('Error updating version:', error);
    res.status(500).json({ error: error.message });
  }
});

// 버전 삭제
router.delete('/:id', (req, res) => {
  try {
    const { id } = req.params;
    
    db.prepare('DELETE FROM versions WHERE id = ?').run(id);
    
    res.json({ message: 'Version deleted successfully' });
  } catch (error) {
    console.error('Error deleting version:', error);
    res.status(500).json({ error: error.message });
  }
});

// Helper functions
function parseManualData(data) {
  const safeParseJSON = (jsonString, defaultValue = {}) => {
    if (!jsonString || jsonString === 'null' || jsonString === 'undefined') {
      return defaultValue;
    }
    try {
      return JSON.parse(jsonString);
    } catch (error) {
      console.error('Failed to parse JSON:', error.message, 'Value:', jsonString);
      return defaultValue;
    }
  };

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
  const safeParseJSON = (jsonString, defaultValue = {}) => {
    if (!jsonString || jsonString === 'null' || jsonString === 'undefined') {
      return defaultValue;
    }
    try {
      return JSON.parse(jsonString);
    } catch (error) {
      console.error('Failed to parse JSON:', error.message, 'Value:', jsonString);
      return defaultValue;
    }
  };

  return {
    jsonSchema: safeParseJSON(data.json_schema, {}),
    jsonSchemaOriginal: safeParseJSON(data.json_schema_original, null),
    jsonSchemaEnhanced: safeParseJSON(data.json_schema_enhanced, null),
    specifications: data.specifications || '',
  };
}

function parseRunnerData(data, testCases) {
  return {
    requestBody: data.request_body || '{}',
    responseBody: data.response_body || '',
    testCases: testCases.map(tc => ({
      id: tc.id,
      name: tc.name,
      description: tc.description,
      requestBody: tc.request_body,
      createdAt: tc.created_at,
      updatedAt: tc.updated_at,
    })),
  };
}

function saveManualData(versionId, data) {
  db.prepare(`
    INSERT OR REPLACE INTO manual_data 
    (version_id, title, category, input_uri, active_methods, json_schema, json_schema_original, json_schema_enhanced, examples, request_examples, response_examples, specifications, html_content, article_id, section_id, author_id, url)
    VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
  `).run(
    versionId,
    data.title,
    data.category,
    data.inputUri,
    data.activeMethods,
    typeof data.jsonSchema === 'string' ? data.jsonSchema : JSON.stringify(data.jsonSchema || {}),
    typeof data.jsonSchemaOriginal === 'string' ? data.jsonSchemaOriginal : (data.jsonSchemaOriginal ? JSON.stringify(data.jsonSchemaOriginal) : null),
    typeof data.jsonSchemaEnhanced === 'string' ? data.jsonSchemaEnhanced : (data.jsonSchemaEnhanced ? JSON.stringify(data.jsonSchemaEnhanced) : null),
    JSON.stringify(data.examples || []),
    JSON.stringify(data.requestExamples || []),
    JSON.stringify(data.responseExamples || []),
    data.specifications,
    data.htmlContent,
    data.articleId,
    data.sectionId,
    data.authorId,
    data.url
  );
}

function saveSpecData(versionId, data) {
  db.prepare(`
    INSERT OR REPLACE INTO spec_data 
    (version_id, json_schema, json_schema_original, json_schema_enhanced, specifications)
    VALUES (?, ?, ?, ?, ?)
  `).run(
    versionId,
    typeof data.jsonSchema === 'string' ? data.jsonSchema : JSON.stringify(data.jsonSchema || {}),
    typeof data.jsonSchemaOriginal === 'string' ? data.jsonSchemaOriginal : (data.jsonSchemaOriginal ? JSON.stringify(data.jsonSchemaOriginal) : null),
    typeof data.jsonSchemaEnhanced === 'string' ? data.jsonSchemaEnhanced : (data.jsonSchemaEnhanced ? JSON.stringify(data.jsonSchemaEnhanced) : null),
    data.specifications
  );
}

function saveBuilderData(versionId, data) {
  db.prepare(`
    INSERT OR REPLACE INTO builder_data (version_id, form_data)
    VALUES (?, ?)
  `).run(versionId, JSON.stringify(data.formData || {}));
}

function saveRunnerData(versionId, data) {
  // Runner data 저장
  db.prepare(`
    INSERT OR REPLACE INTO runner_data (version_id, request_body, response_body)
    VALUES (?, ?, ?)
  `).run(versionId, data.requestBody, data.responseBody);
  
  // Test cases 저장
  if (data.testCases && data.testCases.length > 0) {
    // 기존 test cases 삭제
    db.prepare('DELETE FROM test_cases WHERE version_id = ?').run(versionId);
    
    // 새 test cases 삽입
    const insertTestCase = db.prepare(`
      INSERT INTO test_cases (id, version_id, name, description, request_body, created_at, updated_at)
      VALUES (?, ?, ?, ?, ?, ?, ?)
    `);
    
    data.testCases.forEach(tc => {
      insertTestCase.run(
        tc.id,
        versionId,
        tc.name,
        tc.description,
        tc.requestBody,
        tc.createdAt,
        tc.updatedAt
      );
    });
  }
}

export default router;

