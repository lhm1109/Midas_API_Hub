const { app } = require('electron');
const path = require('path');
const Database = require('better-sqlite3');

let db = null;

function initDatabase() {
  const userDataPath = app.getPath('userData');
  const dbPath = path.join(userDataPath, 'api-verification.db');
  
  db = new Database(dbPath);
  
  // Create tables
  db.exec(`
    CREATE TABLE IF NOT EXISTS presets (
      id TEXT PRIMARY KEY,
      name TEXT NOT NULL,
      description TEXT,
      flow_data TEXT NOT NULL,
      created_at TEXT NOT NULL,
      updated_at TEXT NOT NULL
    );

    CREATE TABLE IF NOT EXISTS test_results (
      id TEXT PRIMARY KEY,
      api_id TEXT NOT NULL,
      preset_id TEXT,
      method TEXT NOT NULL,
      endpoint TEXT NOT NULL,
      parameters TEXT,
      status_code INTEGER,
      response_time INTEGER,
      success INTEGER NOT NULL,
      error_message TEXT,
      created_at TEXT NOT NULL
    );

    CREATE TABLE IF NOT EXISTS api_test_history (
      id TEXT PRIMARY KEY,
      api_id TEXT NOT NULL,
      test_count INTEGER DEFAULT 0,
      success_count INTEGER DEFAULT 0,
      last_tested_at TEXT,
      updated_at TEXT NOT NULL
    );
  `);

  console.log('Database initialized at:', dbPath);
  return db;
}

function getDatabase() {
  if (!db) {
    throw new Error('Database not initialized');
  }
  return db;
}

// Preset operations
function createPreset(preset) {
  const stmt = db.prepare(`
    INSERT INTO presets (id, name, description, flow_data, created_at, updated_at)
    VALUES (?, ?, ?, ?, ?, ?)
  `);
  
  return stmt.run(
    preset.id,
    preset.name,
    preset.description,
    JSON.stringify(preset.flowData),
    preset.createdAt,
    preset.updatedAt
  );
}

function getAllPresets() {
  const stmt = db.prepare('SELECT * FROM presets ORDER BY updated_at DESC');
  const rows = stmt.all();
  
  return rows.map(row => ({
    id: row.id,
    name: row.name,
    description: row.description,
    flowData: JSON.parse(row.flow_data),
    createdAt: row.created_at,
    updatedAt: row.updated_at
  }));
}

function getPresetById(id) {
  const stmt = db.prepare('SELECT * FROM presets WHERE id = ?');
  const row = stmt.get(id);
  
  if (!row) return null;
  
  return {
    id: row.id,
    name: row.name,
    description: row.description,
    flowData: JSON.parse(row.flow_data),
    createdAt: row.created_at,
    updatedAt: row.updated_at
  };
}

function updatePreset(id, preset) {
  const stmt = db.prepare(`
    UPDATE presets
    SET name = ?, description = ?, flow_data = ?, updated_at = ?
    WHERE id = ?
  `);
  
  return stmt.run(
    preset.name,
    preset.description,
    JSON.stringify(preset.flowData),
    new Date().toISOString(),
    id
  );
}

function deletePreset(id) {
  const stmt = db.prepare('DELETE FROM presets WHERE id = ?');
  return stmt.run(id);
}

// Test result operations
function saveTestResult(result) {
  const stmt = db.prepare(`
    INSERT INTO test_results (
      id, api_id, preset_id, method, endpoint, parameters,
      status_code, response_time, success, error_message, created_at
    )
    VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
  `);
  
  return stmt.run(
    result.id,
    result.apiId,
    result.presetId || null,
    result.method,
    result.endpoint,
    JSON.stringify(result.parameters),
    result.statusCode,
    result.responseTime,
    result.success ? 1 : 0,
    result.errorMessage || null,
    result.createdAt
  );
}

function getTestResultsByApiId(apiId, limit = 50) {
  const stmt = db.prepare(`
    SELECT * FROM test_results
    WHERE api_id = ?
    ORDER BY created_at DESC
    LIMIT ?
  `);
  
  const rows = stmt.all(apiId, limit);
  
  return rows.map(row => ({
    id: row.id,
    apiId: row.api_id,
    presetId: row.preset_id,
    method: row.method,
    endpoint: row.endpoint,
    parameters: JSON.parse(row.parameters),
    statusCode: row.status_code,
    responseTime: row.response_time,
    success: row.success === 1,
    errorMessage: row.error_message,
    createdAt: row.created_at
  }));
}

function getRecentTestResults(limit = 20) {
  const stmt = db.prepare(`
    SELECT * FROM test_results
    ORDER BY created_at DESC
    LIMIT ?
  `);
  
  const rows = stmt.all(limit);
  
  return rows.map(row => ({
    id: row.id,
    apiId: row.api_id,
    presetId: row.preset_id,
    method: row.method,
    endpoint: row.endpoint,
    parameters: JSON.parse(row.parameters),
    statusCode: row.status_code,
    responseTime: row.response_time,
    success: row.success === 1,
    errorMessage: row.error_message,
    createdAt: row.created_at
  }));
}

// Statistics
function getTestStatistics() {
  const totalTests = db.prepare('SELECT COUNT(*) as count FROM test_results').get();
  const successTests = db.prepare('SELECT COUNT(*) as count FROM test_results WHERE success = 1').get();
  const totalPresets = db.prepare('SELECT COUNT(*) as count FROM presets').get();
  
  return {
    totalTests: totalTests.count,
    successTests: successTests.count,
    failedTests: totalTests.count - successTests.count,
    totalPresets: totalPresets.count
  };
}

module.exports = {
  initDatabase,
  getDatabase,
  createPreset,
  getAllPresets,
  getPresetById,
  updatePreset,
  deletePreset,
  saveTestResult,
  getTestResultsByApiId,
  getRecentTestResults,
  getTestStatistics
};



