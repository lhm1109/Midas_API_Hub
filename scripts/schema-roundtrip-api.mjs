#!/usr/bin/env node
import fs from 'node:fs/promises';
import path from 'node:path';

const DEFAULT_BASE_URL = 'http://localhost:9527';

const printUsage = () => {
  console.log(`
Usage:
  node scripts/schema-roundtrip-api.mjs --input <schema.json> [options]

Options:
  --input <path>            Source combined schema JSON path (required)
  --base-url <url>          API server base URL (default: ${DEFAULT_BASE_URL})
  --write-request <path>    Write split request schema JSON
  --write-response <path>   Write split response schema JSON
  --write-merged <path>     Write merged schema JSON
  --write-fixed <path>      Write auto-fixed merged schema JSON (if available)
  --write-report <path>     Write full roundtrip report JSON
  --inplace                 Overwrite input file with auto-fixed schema on mismatch
  --strict                  Exit non-zero when mismatch cannot be auto-fixed (default)
  --no-strict               Always exit 0
  --autofix                 Prefer auto-fixed merged when writing merged output (default)
  --no-autofix              Write raw merged output without auto-fix
  --help                    Show this help
`);
};

const parseArgs = (argv) => {
  const options = {
    baseUrl: DEFAULT_BASE_URL,
    strict: true,
    autofix: true,
  };

  for (let i = 0; i < argv.length; i += 1) {
    const arg = argv[i];
    const next = argv[i + 1];

    switch (arg) {
      case '--input':
        options.input = next;
        i += 1;
        break;
      case '--base-url':
        options.baseUrl = next;
        i += 1;
        break;
      case '--write-request':
        options.writeRequest = next;
        i += 1;
        break;
      case '--write-response':
        options.writeResponse = next;
        i += 1;
        break;
      case '--write-merged':
        options.writeMerged = next;
        i += 1;
        break;
      case '--write-fixed':
        options.writeFixed = next;
        i += 1;
        break;
      case '--write-report':
        options.writeReport = next;
        i += 1;
        break;
      case '--inplace':
        options.inplace = true;
        break;
      case '--strict':
        options.strict = true;
        break;
      case '--no-strict':
        options.strict = false;
        break;
      case '--autofix':
        options.autofix = true;
        break;
      case '--no-autofix':
        options.autofix = false;
        break;
      case '--help':
      case '-h':
        options.help = true;
        break;
      default:
        if (arg.startsWith('--')) {
          throw new Error(`Unknown option: ${arg}`);
        }
    }
  }

  return options;
};

const ensureDir = async (filePath) => {
  const dir = path.dirname(path.resolve(filePath));
  await fs.mkdir(dir, { recursive: true });
};

const writeJson = async (filePath, value) => {
  await ensureDir(filePath);
  await fs.writeFile(path.resolve(filePath), `${JSON.stringify(value, null, 2)}\n`, 'utf8');
};

const postJson = async (url, body) => {
  const response = await fetch(url, {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify(body),
  });

  const text = await response.text();
  let parsed;
  try {
    parsed = text ? JSON.parse(text) : {};
  } catch {
    parsed = { raw: text };
  }

  if (!response.ok) {
    const errorMessage = parsed?.error || parsed?.message || `HTTP ${response.status}`;
    throw new Error(`${url} -> ${errorMessage}`);
  }
  return parsed;
};

const summaryDiff = (comparison) => ({
  missing: Array.isArray(comparison?.missingInMerged) ? comparison.missingInMerged.length : 0,
  extra: Array.isArray(comparison?.extraInMerged) ? comparison.extraInMerged.length : 0,
  changed: Array.isArray(comparison?.changedSchemas) ? comparison.changedSchemas.length : 0,
});

const main = async () => {
  const options = parseArgs(process.argv.slice(2));

  if (options.help) {
    printUsage();
    return;
  }
  if (!options.input) {
    printUsage();
    throw new Error('--input is required');
  }

  const sourcePath = path.resolve(options.input);
  const sourceRaw = await fs.readFile(sourcePath, 'utf8');
  const sourceSchema = JSON.parse(sourceRaw);

  const baseUrl = options.baseUrl.replace(/\/+$/, '');
  const splitUrl = `${baseUrl}/api/schema-tools/split`;
  const mergeUrl = `${baseUrl}/api/schema-tools/merge`;

  const split = await postJson(splitUrl, { sourceSchema });
  const merge = await postJson(mergeUrl, {
    requestSchema: split.requestSchema,
    responseSchema: split.responseSchema,
    requestKey: split.requestKey,
    responseKey: split.responseKey,
    sourceSchema,
    autoFixAgainstSource: true,
  });

  const hasMismatch = merge.matchesSource === false;
  const hasAutoFixed = hasMismatch && !!merge.autoFixedMerged;
  const outputMerged = options.autofix && hasAutoFixed ? merge.autoFixedMerged : merge.mergedSchema;

  if (options.writeRequest) await writeJson(options.writeRequest, split.requestSchema);
  if (options.writeResponse) await writeJson(options.writeResponse, split.responseSchema);
  if (options.writeMerged) await writeJson(options.writeMerged, outputMerged);
  if (options.writeFixed && merge.autoFixedMerged) await writeJson(options.writeFixed, merge.autoFixedMerged);

  const report = {
    input: sourcePath,
    requestKey: split.requestKey,
    responseKey: split.responseKey,
    matchesSource: merge.matchesSource,
    comparison: merge.comparison,
    usedAutoFixedMerged: options.autofix && hasAutoFixed,
    generatedAt: new Date().toISOString(),
  };
  if (options.writeReport) await writeJson(options.writeReport, report);

  if (options.inplace && merge.autoFixedMerged && hasMismatch) {
    await writeJson(sourcePath, merge.autoFixedMerged);
  }

  const diff = summaryDiff(merge.comparison);
  console.log('[schema-roundtrip-api]');
  console.log(`input=${sourcePath}`);
  console.log(`requestKey=${split.requestKey}`);
  console.log(`responseKey=${split.responseKey}`);
  console.log(`matchesSource=${String(merge.matchesSource)}`);
  console.log(`diff=missing:${diff.missing}, extra:${diff.extra}, changed:${diff.changed}`);
  console.log(`autoFixed=${hasAutoFixed ? 'yes' : 'no'}, autoFixedUsed=${options.autofix && hasAutoFixed ? 'yes' : 'no'}`);

  if (merge.matchesSource) {
    return;
  }

  if (hasAutoFixed) {
    console.log('status=FIXED');
    return;
  }

  console.log('status=FAIL');
  if (options.strict) {
    process.exitCode = 2;
  }
};

main().catch((error) => {
  console.error(`[schema-roundtrip-api] ERROR: ${error.message}`);
  process.exitCode = 1;
});
