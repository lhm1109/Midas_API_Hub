#!/usr/bin/env node
import fs from 'node:fs/promises';
import path from 'node:path';

const DEFAULT_BASE_URL = 'http://localhost:9527';
const DEFAULT_INPUT = 'scripts/samples/out/slrs_api_import.json';

function makeVersionId() {
  return `v_${Date.now()}_${Math.random().toString(36).slice(2, 11)}`;
}

function normalizeBaseUrl(url) {
  return (url || DEFAULT_BASE_URL).replace(/\/+$/, '');
}

async function postJson(url, body) {
  const res = await fetch(url, {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify(body),
  });

  const text = await res.text();
  let json;
  try {
    json = text ? JSON.parse(text) : {};
  } catch {
    json = { raw: text };
  }

  return {
    ok: res.ok,
    status: res.status,
    data: json,
  };
}

async function main() {
  const baseUrl = normalizeBaseUrl(process.argv[2] || DEFAULT_BASE_URL);
  const inputFile = process.argv[3] || DEFAULT_INPUT;
  const inputPath = path.resolve(inputFile);

  const raw = await fs.readFile(inputPath, 'utf8');
  const data = JSON.parse(raw.replace(/^\uFEFF/, ''));

  if (!data?.endpoint || !data?.version) {
    throw new Error('Invalid import JSON: endpoint and version are required.');
  }

  const endpointUrl = `${baseUrl}/api/endpoints`;
  const versionUrl = `${baseUrl}/api/versions`;

  // 1) create endpoint (ignore duplicate)
  const endpointResult = await postJson(endpointUrl, data.endpoint);
  if (!endpointResult.ok) {
    const msg = endpointResult?.data?.error || endpointResult?.data?.message || `HTTP ${endpointResult.status}`;
    const isDuplicate =
      String(msg).includes('UNIQUE constraint') ||
      String(msg).includes('duplicate key value') ||
      String(msg).includes('already exists');
    if (!isDuplicate) {
      throw new Error(`Create endpoint failed: ${msg}`);
    }
    console.log(`[import-api-json] endpoint already exists: ${data.endpoint.id}`);
  } else {
    console.log(`[import-api-json] endpoint created: ${data.endpoint.id}`);
  }

  // 2) create version
  const versionPayload = {
    id: makeVersionId(),
    version: data.version.version,
    endpointId: data.endpoint.id,
    author: data.version.author || null,
    changeLog: data.version.changeLog || null,
    manualData: {
      title: data.manual?.title || data.endpoint.name,
      category: data.manual?.category || data.endpoint.group_name,
      inputUri: data.manual?.inputUri || data.endpoint.path,
      activeMethods: data.manual?.activeMethods || data.endpoint.method,
      jsonSchema: data.schema?.jsonSchema ? JSON.stringify(data.schema.jsonSchema) : '{}',
      jsonSchemaOriginal: data.schema?.jsonSchemaOriginal ? JSON.stringify(data.schema.jsonSchemaOriginal) : null,
      jsonSchemaEnhanced: data.schema?.jsonSchemaEnhanced ? JSON.stringify(data.schema.jsonSchemaEnhanced) : null,
      htmlContent: data.manual?.htmlContent || null,
      specifications: data.manual?.specifications || null,
      url: data.manual?.url || null,
      examples: Array.isArray(data.examples) ? data.examples : [],
    },
    specData: {
      jsonSchema: data.schema?.jsonSchema ? JSON.stringify(data.schema.jsonSchema) : '{}',
      jsonSchemaOriginal: data.schema?.jsonSchemaOriginal ? JSON.stringify(data.schema.jsonSchemaOriginal) : null,
      jsonSchemaEnhanced: data.schema?.jsonSchemaEnhanced ? JSON.stringify(data.schema.jsonSchemaEnhanced) : null,
      specifications: data.manual?.specifications || null,
    },
    builderData: data.builderData || null,
    runnerData: data.runnerData || null,
  };

  const versionResult = await postJson(versionUrl, versionPayload);
  if (!versionResult.ok) {
    const msg = versionResult?.data?.error || versionResult?.data?.message || `HTTP ${versionResult.status}`;
    throw new Error(`Create version failed: ${msg}`);
  }

  console.log(`[import-api-json] version created: ${versionPayload.id}`);
  console.log(`[import-api-json] done`);
  console.log(JSON.stringify({ endpointId: data.endpoint.id, versionId: versionPayload.id }, null, 2));
}

main().catch((err) => {
  console.error(`[import-api-json] ERROR: ${err.message}`);
  process.exitCode = 1;
});
