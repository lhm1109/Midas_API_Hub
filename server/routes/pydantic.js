import express from 'express';
import fs from 'fs';
import path from 'path';
import {
  buildPydanticPaths,
  generatePydanticCode,
  parseSchemaInput,
  savePydanticModule,
} from '../lib/pydantic-generator.js';
import { runPydanticCode } from '../lib/pydantic-code-runner.js';
import { runPydanticEndpoint } from '../lib/pydantic-runner.js';
import supabase from '../database.js';

const router = express.Router();
const packagedPydanticRoot = process.resourcesPath
  ? path.join(process.resourcesPath, 'generated_pydantic')
  : null;
const outputRoot = process.env.PYDANTIC_OUTPUT_ROOT
  || (packagedPydanticRoot && fs.existsSync(packagedPydanticRoot)
    ? packagedPydanticRoot
    : path.join(process.cwd(), 'generated_pydantic'));

function buildEndpointPayload(value = {}) {
  return {
    id: typeof value.id === 'string' ? value.id : '',
    name: typeof value.name === 'string' ? value.name : '',
    method: typeof value.method === 'string' ? value.method : '',
    path: typeof value.path === 'string' ? value.path : '',
    productId: typeof value.productId === 'string' ? value.productId : value.product_id,
    product: typeof value.product === 'string' ? value.product : undefined,
  };
}

router.post('/generate', (req, res) => {
  try {
    const schema = parseSchemaInput(req.body?.schema, 'schema');
    const endpoint = buildEndpointPayload(req.body?.endpoint);
    const generated = generatePydanticCode({ schema, endpoint });
    const paths = buildPydanticPaths({ endpoint, outputRoot });

    res.json({
      ...generated,
      filePath: paths.filePath,
      registryPath: paths.registryPath,
      manifestPath: paths.manifestPath,
      moduleName: paths.moduleName,
      productId: paths.productId,
      saved: false,
    });
  } catch (error) {
    res.status(400).json({ error: error instanceof Error ? error.message : String(error) });
  }
});

router.post('/save', async (req, res) => {
  try {
    const schema = parseSchemaInput(req.body?.schema, 'schema');
    const endpoint = buildEndpointPayload(req.body?.endpoint);
    const generated = await savePydanticModule({ schema, endpoint, outputRoot });

    // Supabase 저장은 파일 저장과 독립적으로 실행 — 실패해도 응답에 영향 없음
    savePydanticCodeToSupabase(endpoint.id, generated.code).catch((err) => {
      console.warn('[pydantic] Supabase save failed (file system save succeeded):', err?.message);
    });

    res.json({
      ...generated,
      saved: true,
    });
  } catch (error) {
    res.status(400).json({ error: error instanceof Error ? error.message : String(error) });
  }
});

/**
 * endpoint.id 기준으로 최신 version의 spec_data에 pydantic_code 저장.
 * pydantic_code / pydantic_generated_at 컬럼만 건드리며 기존 json_schema 등은 변경하지 않음.
 */
async function savePydanticCodeToSupabase(endpointId, code) {
  if (!endpointId || !code) return;

  const { data: versions, error: versionError } = await supabase
    .from('versions')
    .select('id')
    .eq('endpoint_id', endpointId)
    .order('updated_at', { ascending: false })
    .limit(1);

  if (versionError) throw versionError;
  if (!versions?.length) {
    console.warn(`[pydantic] No version found for endpoint: ${endpointId}`);
    return;
  }

  const versionId = versions[0].id;
  const { error } = await supabase
    .from('spec_data')
    .upsert(
      {
        version_id: versionId,
        pydantic_code: code,
        pydantic_generated_at: new Date().toISOString(),
      },
      { onConflict: 'version_id' }
    );

  if (error) throw error;
  console.log(`[pydantic] Saved to Supabase spec_data: ${endpointId} (version: ${versionId})`);
}

router.post('/run', async (req, res) => {
  try {
    const endpoint = buildEndpointPayload(req.body?.endpoint);
    const result = await runPydanticEndpoint({
      endpoint,
      method: req.body?.method,
      url: req.body?.url,
      headers: req.body?.headers || {},
      requestBody: req.body?.requestBody,
      outputRoot,
    });

    res.json(result);
  } catch (error) {
    res.status(400).json({ error: error instanceof Error ? error.message : String(error) });
  }
});

router.post('/execute', async (req, res) => {
  try {
    const endpoint = buildEndpointPayload(req.body?.endpoint);
    const result = await runPydanticCode({
      code: req.body?.code,
      endpoint,
      method: req.body?.method,
      url: req.body?.url,
      headers: req.body?.headers || {},
      outputRoot,
    });

    res.json(result);
  } catch (error) {
    res.status(400).json({ error: error instanceof Error ? error.message : String(error) });
  }
});

export default router;
