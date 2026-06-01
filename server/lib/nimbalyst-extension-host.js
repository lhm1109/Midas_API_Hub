import fs from 'fs/promises';
import path from 'path';
import { spawn } from 'child_process';
import { Worker } from 'worker_threads';
import { fileURLToPath, pathToFileURL } from 'url';

const THIS_DIR = path.dirname(fileURLToPath(import.meta.url));
const SDK_COMPAT_PACKAGES = ['extension-sdk', 'runtime'];
const MAX_BACKEND_MODULES_PER_EXTENSION = 8;
const BACKEND_MODULE_ID_PATTERN = /^[a-z0-9][a-z0-9._-]{0,63}$/;
const KNOWN_BACKEND_RUNTIMES = ['utility-process', 'worker-thread'];
const KNOWN_BACKEND_PERMISSION_IDS = [
  'workspace-files',
  'nimbalyst-database-read',
  'nimbalyst-database-write',
  'secrets-read',
  'mcp-server-register',
];
const DEPRECATED_BACKEND_PERMISSION_IDS = [
  'spawn-process',
  'network-loopback',
  'network-internet',
  'filesystem',
];
const BACKEND_PERMISSION_DESCRIPTORS = [
  {
    id: 'workspace-files',
    label: 'Workspace files',
    description: 'Read and write files in the current workspace through host brokers.',
    risk: 'low',
  },
  {
    id: 'nimbalyst-database-read',
    label: 'Nimbalyst database read',
    description: 'Read host database records.',
    risk: 'elevated',
  },
  {
    id: 'nimbalyst-database-write',
    label: 'Nimbalyst database write',
    description: 'Create or modify host database records.',
    risk: 'high',
  },
  {
    id: 'secrets-read',
    label: 'Secrets read',
    description: 'Read secrets stored by the host.',
    risk: 'high',
  },
  {
    id: 'mcp-server-register',
    label: 'MCP server registration',
    description: 'Register MCP tools with the host.',
    risk: 'elevated',
  },
];

function asString(value, fallback = '') {
  return typeof value === 'string' && value.trim() ? value.trim() : fallback;
}

function isPlainObject(value) {
  return Boolean(value) && typeof value === 'object' && !Array.isArray(value);
}

function extensionShortId(manifestId) {
  const parts = String(manifestId).split('.').filter(Boolean);
  return parts[parts.length - 1] || String(manifestId);
}

function toPosixPath(filePath) {
  return filePath.split(path.sep).join('/');
}

function escapeRegExp(value) {
  return value.replace(/[|\\{}()[\]^$+?.]/g, '\\$&');
}

function globToRegExp(pattern) {
  const normalized = toPosixPath(pattern || '**/*');
  let source = '';
  for (let i = 0; i < normalized.length; i += 1) {
    const char = normalized[i];
    const next = normalized[i + 1];
    if (char === '*' && next === '*') {
      source += '.*';
      i += 1;
    } else if (char === '*') {
      source += '[^/]*';
    } else {
      source += escapeRegExp(char);
    }
  }
  return new RegExp(`^${source}$`);
}

async function fileExists(filePath) {
  try {
    await fs.access(filePath);
    return true;
  } catch {
    return false;
  }
}

async function walkFiles(rootDir) {
  const out = [];
  async function walk(currentDir) {
    const entries = await fs.readdir(currentDir, { withFileTypes: true });
    for (const entry of entries) {
      const absolutePath = path.join(currentDir, entry.name);
      if (entry.isDirectory()) {
        await walk(absolutePath);
      } else if (entry.isFile()) {
        out.push(absolutePath);
      }
    }
  }
  if (await fileExists(rootDir)) {
    await walk(rootDir);
  }
  return out;
}

async function readPackageJson(packageRoot) {
  try {
    return JSON.parse(await fs.readFile(path.join(packageRoot, 'package.json'), 'utf-8'));
  } catch {
    return null;
  }
}

function runCommand(command, args, options = {}) {
  return new Promise((resolve) => {
    let child;
    let stdout = '';
    let stderr = '';
    const timeoutMs = options.timeoutMs || 120_000;
    try {
      child = spawn(command, args, {
        cwd: options.cwd,
        env: Object.fromEntries(
          Object.entries({ ...process.env, ...(options.env || {}) }).filter(([, value]) => value !== undefined)
        ),
        shell: process.platform === 'win32',
        windowsHide: true,
      });
    } catch (error) {
      resolve({ exitCode: 1, stdout, stderr: error instanceof Error ? error.message : String(error) });
      return;
    }
    const timer = setTimeout(() => {
      child.kill();
      stderr += `\nCommand timed out after ${timeoutMs}ms.`;
    }, timeoutMs);

    child.stdout?.on('data', (chunk) => {
      stdout += chunk.toString();
    });
    child.stderr?.on('data', (chunk) => {
      stderr += chunk.toString();
    });
    child.on('error', (error) => {
      clearTimeout(timer);
      resolve({ exitCode: 1, stdout, stderr: stderr || error.message });
    });
    child.on('close', (exitCode, signal) => {
      clearTimeout(timer);
      resolve({ exitCode, signal, stdout, stderr });
    });
  });
}

function assertInsideRoot(rootDir, targetPath) {
  const root = path.resolve(rootDir);
  const target = path.resolve(root, targetPath);
  const relative = path.relative(root, target);
  if (relative.startsWith('..') || path.isAbsolute(relative)) {
    throw new Error(`Path is outside the workspace root: ${targetPath}`);
  }
  return target;
}

function normalizeManifest(manifest, extensionPath) {
  if (!isPlainObject(manifest)) {
    throw new Error(`Invalid manifest in ${extensionPath}`);
  }

  const id = asString(manifest.id);
  const name = asString(manifest.name, id);
  const version = asString(manifest.version, '0.0.0');
  const main = asString(manifest.main, '');

  if (!id) throw new Error(`Extension manifest is missing id: ${extensionPath}`);

  return {
    ...manifest,
    id,
    name,
    version,
    main,
    permissions: isPlainObject(manifest.permissions) ? manifest.permissions : {},
    contributions: isPlainObject(manifest.contributions) ? manifest.contributions : {},
  };
}

  function formatExtensionSummary(loaded, manifest, extensionPath) {
  return {
    id: manifest.id,
    name: manifest.name,
    version: manifest.version,
    description: manifest.description ?? '',
    extensionPath,
    main: manifest.main || '',
    active: Boolean(loaded?.active),
    permissions: manifest.permissions ?? {},
    contributions: manifest.contributions ?? {},
    toolNames: loaded?.tools?.map((tool) => tool.name) ?? [],
  };
}

const CONTRIBUTION_KEYS = [
  'customEditors',
  'documentHeaders',
  'newFileMenu',
  'commands',
  'keybindings',
  'slashCommands',
  'panels',
  'settingsPanel',
  'themes',
  'fileIcons',
  'aiTools',
  'nodes',
  'transformers',
  'lexicalExtensions',
  'hostComponents',
  'configuration',
  'claudePlugin',
  'agentWorkflows',
  'backendModules',
];

function summarizeContributionItem(type, item, manifest) {
  const raw = item;
  const value = isPlainObject(item) ? item : { value: item };
  const id =
    value.id ||
    value.component ||
    value.command ||
    value.handler ||
    value.extension ||
    value.name ||
    value.path ||
    String(value.value ?? type);
  const title =
    value.title ||
    value.displayName ||
    value.name ||
    value.extension ||
    value.key ||
    value.id ||
    String(value.value ?? id);
  const description = value.description || value.defaultContent || '';

  return {
    type,
    id: String(id),
    title: String(title),
    description: String(description),
    extensionId: manifest.id,
    extensionName: manifest.name,
    main: manifest.main || '',
    raw,
  };
}

function summarizeContributionRecord(type, record, manifest) {
  if (Array.isArray(record)) {
    return record.map((item) => summarizeContributionItem(type, item, manifest));
  }

  if (isPlainObject(record)) {
    if (type === 'configuration') {
      const properties = isPlainObject(record.properties) ? record.properties : {};
      return [
        {
          type,
          id: `${manifest.id}.configuration`,
          title: record.title || 'Configuration',
          description: `${Object.keys(properties).length} configurable properties`,
          extensionId: manifest.id,
          extensionName: manifest.name,
          main: manifest.main || '',
          raw: record,
        },
      ];
    }

    if (type === 'fileIcons') {
      return Object.entries(record).map(([pattern, icon]) => ({
        type,
        id: pattern,
        title: pattern,
        description: String(icon),
        extensionId: manifest.id,
        extensionName: manifest.name,
        main: manifest.main || '',
        raw: { pattern, icon },
      }));
    }

    return [summarizeContributionItem(type, record, manifest)];
  }

  if (record === undefined || record === null) return [];
  return [summarizeContributionItem(type, record, manifest)];
}

function summarizeManifestContributions(manifest) {
  const contributions = isPlainObject(manifest.contributions) ? manifest.contributions : {};
  return CONTRIBUTION_KEYS.flatMap((key) => summarizeContributionRecord(key, contributions[key], manifest));
}

function getConfigurationDefaults(manifest) {
  const properties = manifest.contributions?.configuration?.properties ?? {};
  return Object.fromEntries(
    Object.entries(properties).map(([key, property]) => [key, property?.default])
  );
}

function getBackendModuleGrantKey(workspacePath) {
  return workspacePath ? path.resolve(workspacePath) : '';
}

function validateBackendModules(backendModules) {
  if (backendModules === undefined) return [];
  if (!Array.isArray(backendModules)) {
    return [{ message: 'contributions.backendModules must be an array' }];
  }

  const issues = [];
  if (backendModules.length > MAX_BACKEND_MODULES_PER_EXTENSION) {
    issues.push({
      message:
        `contributions.backendModules declares ${backendModules.length} modules; ` +
        `the maximum is ${MAX_BACKEND_MODULES_PER_EXTENSION}.`,
    });
  }

  const seenIds = new Set();
  backendModules.forEach((raw, index) => {
    if (!isPlainObject(raw)) {
      issues.push({ message: `backendModules[${index}] must be an object` });
      return;
    }

    const moduleLabel = typeof raw.id === 'string' ? raw.id : `index ${index}`;
    if (typeof raw.id !== 'string' || !BACKEND_MODULE_ID_PATTERN.test(raw.id)) {
      issues.push({
        moduleId: typeof raw.id === 'string' ? raw.id : undefined,
        message: `backendModules[${moduleLabel}].id must be a lowercase string matching ${BACKEND_MODULE_ID_PATTERN.source}`,
      });
    } else if (seenIds.has(raw.id)) {
      issues.push({ moduleId: raw.id, message: `backendModules contains duplicate id "${raw.id}"` });
    } else {
      seenIds.add(raw.id);
    }

    if (typeof raw.entry !== 'string' || raw.entry.length === 0) {
      issues.push({
        moduleId: typeof raw.id === 'string' ? raw.id : undefined,
        message: `backendModules[${moduleLabel}].entry must be a non-empty relative path string`,
      });
    } else if (raw.entry.startsWith('/') || raw.entry.includes('..')) {
      issues.push({
        moduleId: typeof raw.id === 'string' ? raw.id : undefined,
        message: `backendModules[${moduleLabel}].entry must stay inside the extension root`,
      });
    }

    if (!KNOWN_BACKEND_RUNTIMES.includes(raw.runtime)) {
      issues.push({
        moduleId: typeof raw.id === 'string' ? raw.id : undefined,
        message: `backendModules[${moduleLabel}].runtime must be one of: ${KNOWN_BACKEND_RUNTIMES.join(', ')}`,
      });
    }

    if (raw.permissions !== undefined && !Array.isArray(raw.permissions)) {
      issues.push({
        moduleId: typeof raw.id === 'string' ? raw.id : undefined,
        message: `backendModules[${moduleLabel}].permissions must be an array`,
      });
    } else if (Array.isArray(raw.permissions)) {
      const seenPermissions = new Set();
      for (const permission of raw.permissions) {
        if (typeof permission !== 'string') {
          issues.push({
            moduleId: typeof raw.id === 'string' ? raw.id : undefined,
            message: `backendModules[${moduleLabel}].permissions contains non-string entry`,
          });
          continue;
        }
        if (DEPRECATED_BACKEND_PERMISSION_IDS.includes(permission)) {
          issues.push({
            severity: 'warning',
            moduleId: typeof raw.id === 'string' ? raw.id : undefined,
            message: `backendModules[${moduleLabel}].permissions includes deprecated id "${permission}"`,
          });
          continue;
        }
        if (!KNOWN_BACKEND_PERMISSION_IDS.includes(permission)) {
          issues.push({
            moduleId: typeof raw.id === 'string' ? raw.id : undefined,
            message: `backendModules[${moduleLabel}].permissions contains unknown id "${permission}"`,
          });
        }
        if (seenPermissions.has(permission)) {
          issues.push({
            moduleId: typeof raw.id === 'string' ? raw.id : undefined,
            message: `backendModules[${moduleLabel}].permissions contains duplicate "${permission}"`,
          });
        }
        seenPermissions.add(permission);
      }
    }

    const enablement = raw.enablement;
    if (!isPlainObject(enablement)) {
      issues.push({
        moduleId: typeof raw.id === 'string' ? raw.id : undefined,
        message: `backendModules[${moduleLabel}].enablement is required`,
      });
    } else {
      if (enablement.default !== 'disabled') {
        issues.push({
          moduleId: typeof raw.id === 'string' ? raw.id : undefined,
          message: `backendModules[${moduleLabel}].enablement.default must be "disabled"`,
        });
      }
      if (enablement.promptOn !== 'firstUse') {
        issues.push({
          moduleId: typeof raw.id === 'string' ? raw.id : undefined,
          message: `backendModules[${moduleLabel}].enablement.promptOn must be "firstUse"`,
        });
      }
      if (typeof enablement.purpose !== 'string' || enablement.purpose.trim().length === 0) {
        issues.push({
          moduleId: typeof raw.id === 'string' ? raw.id : undefined,
          message: `backendModules[${moduleLabel}].enablement.purpose must be a non-empty string`,
        });
      } else if (enablement.purpose.length > 280) {
        issues.push({
          moduleId: typeof raw.id === 'string' ? raw.id : undefined,
          message: `backendModules[${moduleLabel}].enablement.purpose is too long`,
        });
      }
    }
  });

  return issues;
}

function effectiveBackendModulePermissions(raw) {
  if (!Array.isArray(raw)) return [];
  const permissions = [];
  const seen = new Set();
  for (const permission of raw) {
    if (typeof permission !== 'string' || seen.has(permission)) continue;
    seen.add(permission);
    if (KNOWN_BACKEND_PERMISSION_IDS.includes(permission)) {
      permissions.push(permission);
    }
  }
  return permissions;
}

function serializeBackendModuleError(error) {
  if (error instanceof Error) {
    return {
      name: error.name,
      message: error.message,
      stack: error.stack,
    };
  }
  if (isPlainObject(error) && typeof error.message === 'string') {
    return error;
  }
  return {
    name: 'Error',
    message: String(error),
  };
}

function normalizeToolResult(result) {
  if (isPlainObject(result) && typeof result.success === 'boolean') {
    return result;
  }
  if (isPlainObject(result) && typeof result.error === 'string') {
    return { success: false, error: result.error, data: result };
  }
  return { success: true, data: result };
}

function buildOpenAIResponseFormat(format) {
  if (!format || format.type === 'text') return undefined;
  if (format.type === 'json_object') {
    return { type: 'json_object' };
  }
  if (format.type === 'json_schema') {
    return {
      type: 'json_schema',
      json_schema: {
        name: format.name || 'extension_response',
        strict: format.strict !== false,
        schema: format.schema,
      },
    };
  }
  return undefined;
}

function normalizeOpenAIModel(model, fallback) {
  const value = asString(model, fallback);
  const [, providerModel] = value.match(/^openai:(.+)$/) || [];
  return providerModel || value || fallback;
}

export function createNimbalystExtensionHost(options = {}) {
  const extensionsRoot = path.resolve(
    options.extensionsRoot ||
      process.env.NIMBALYST_EXTENSIONS_DIR ||
      path.join(process.cwd(), 'extensions')
  );
  const workspaceRoot = path.resolve(options.workspaceRoot || process.cwd());
  const configurationRoot = path.resolve(
    options.configurationRoot || path.join(process.cwd(), '.bat-rack', 'nimbalyst-extension-config')
  );
  const backendGrantRoot = path.resolve(
    options.backendGrantRoot || path.join(process.cwd(), '.bat-rack', 'nimbalyst-backend-module-grants')
  );
  const referenceRoot = path.resolve(
    options.referenceRoot ||
      process.env.NIMBALYST_REFERENCE_DIR ||
      path.join(process.cwd(), 'docs', 'REFERENCE', 'nimbalyst')
  );
  const fetchImpl = options.fetchImpl || globalThis.fetch;

  const loadedExtensions = new Map();
  const registeredTools = new Map();
  const contextProviders = new Map();
  const backendModuleWorkers = new Map();

  async function readManifest(extensionPath) {
    const manifestPath = path.join(extensionPath, 'manifest.json');
    const raw = await fs.readFile(manifestPath, 'utf-8');
    return normalizeManifest(JSON.parse(raw), extensionPath);
  }

  async function discoverExtensionPaths() {
    if (!(await fileExists(extensionsRoot))) return [];
    const entries = await fs.readdir(extensionsRoot, { withFileTypes: true });
    return entries
      .filter((entry) => entry.isDirectory() && entry.name !== 'node_modules' && !entry.name.startsWith('.'))
      .map((entry) => path.join(extensionsRoot, entry.name));
  }

  async function discoverReferenceExtensionManifests() {
    const referenceExtensionsRoot = path.join(referenceRoot, 'packages', 'extensions');
    if (!(await fileExists(referenceExtensionsRoot))) return [];
    const entries = await fs.readdir(referenceExtensionsRoot, { withFileTypes: true });
    const manifests = [];
    for (const entry of entries) {
      if (!entry.isDirectory() || entry.name === 'node_modules' || entry.name.startsWith('.')) continue;
      const extensionPath = path.join(referenceExtensionsRoot, entry.name);
      try {
        const manifest = await readManifest(extensionPath);
        const packageJson = await readPackageJson(extensionPath);
        manifests.push({
          extensionPath,
          manifest,
          hasRunnableBundle: !manifest.main || (await fileExists(path.join(extensionPath, manifest.main))),
          canBuildFromSource: Boolean(manifest.main && packageJson?.scripts?.build),
        });
      } catch {
        // Marketplace discovery should not fail because one bundled reference
        // extension is malformed or still source-only.
      }
    }
    return manifests;
  }

  async function ensureSdkCompatPackages() {
    const sourceRoot = path.join(THIS_DIR, 'nimbalyst-sdk-compat');
    const scopedTargetRoot = path.join(extensionsRoot, 'node_modules', '@nimbalyst');
    await fs.mkdir(scopedTargetRoot, { recursive: true });

    await Promise.all(
      SDK_COMPAT_PACKAGES.map(async (packageName) => {
        const source = path.join(sourceRoot, packageName);
        if (!(await fileExists(source))) return;
        const target = path.join(scopedTargetRoot, packageName);
        await fs.rm(target, { recursive: true, force: true });
        await fs.cp(source, target, { recursive: true, force: true });
      })
    );
  }

  async function listExtensions() {
    const extensionPaths = await discoverExtensionPaths();
    const summaries = [];
    for (const extensionPath of extensionPaths) {
      try {
        const manifest = await readManifest(extensionPath);
        summaries.push(formatExtensionSummary(loadedExtensions.get(manifest.id), manifest, extensionPath));
      } catch (error) {
        summaries.push({
          id: path.basename(extensionPath),
          name: path.basename(extensionPath),
          version: '0.0.0',
          extensionPath,
          active: false,
          error: error instanceof Error ? error.message : String(error),
        });
      }
    }
    return summaries.sort((a, b) => a.name.localeCompare(b.name));
  }

  async function listMarketplaceExtensions() {
    const referenceManifests = await discoverReferenceExtensionManifests();
    const referenceById = new Map(referenceManifests.map((item) => [item.manifest.id, item]));
    const installedById = new Map();
    for (const extension of await listExtensions()) {
      installedById.set(extension.id, extension);
    }

    const registryPath = path.join(referenceRoot, 'packages', 'electron', 'src', 'main', 'data', 'extensionRegistry.json');
    let registryEntries = [];
    try {
      const registry = JSON.parse(await fs.readFile(registryPath, 'utf-8'));
      registryEntries = Array.isArray(registry.extensions) ? registry.extensions : [];
    } catch {
      registryEntries = [];
    }

    const byId = new Map();
    for (const entry of registryEntries) {
      if (!entry?.id) continue;
      byId.set(entry.id, entry);
    }
    for (const { manifest } of referenceManifests) {
      if (byId.has(manifest.id)) continue;
      byId.set(manifest.id, {
        id: manifest.id,
        name: manifest.name,
        description: manifest.description || '',
        version: manifest.version,
        author: manifest.author || '',
        categories: manifest.marketplace?.categories || [],
        tags: manifest.marketplace?.tags || [],
        icon: manifest.marketplace?.icon || 'extension',
        tagline: manifest.marketplace?.tagline || manifest.description || '',
        longDescription: manifest.marketplace?.longDescription || manifest.description || '',
        highlights: manifest.marketplace?.highlights || [],
        fileTypes: manifest.marketplace?.fileTypes || [],
        permissions: Object.keys(manifest.permissions || {}).filter((key) => manifest.permissions[key]),
      });
    }

    return Array.from(byId.values())
      .map((entry) => {
        const reference = referenceById.get(entry.id);
        const installed = installedById.get(entry.id);
        const installable = Boolean(reference?.hasRunnableBundle);
        const buildable = Boolean(reference?.canBuildFromSource);
        return {
          id: entry.id,
          name: entry.name || entry.id,
          description: entry.description || '',
          version: entry.version || reference?.manifest.version || '0.0.0',
          author: entry.author || reference?.manifest.author || '',
          categories: Array.isArray(entry.categories) ? entry.categories : [],
          tags: Array.isArray(entry.tags) ? entry.tags : [],
          icon: entry.icon || reference?.manifest.marketplace?.icon || 'extension',
          tagline: entry.tagline || entry.description || '',
          longDescription: entry.longDescription || entry.description || '',
          highlights: Array.isArray(entry.highlights) ? entry.highlights : [],
          fileTypes: Array.isArray(entry.fileTypes) ? entry.fileTypes : [],
          permissions: Array.isArray(entry.permissions) ? entry.permissions : [],
          downloadUrl: entry.downloadUrl || '',
          checksum: entry.checksum || '',
          availableLocal: Boolean(reference),
          installed: Boolean(installed),
          installedVersion: installed?.version || '',
          installable,
          buildable,
          requiresBuild: Boolean(reference && !installable && buildable),
        };
      })
      .sort((a, b) => a.name.localeCompare(b.name));
  }

  async function installMarketplaceExtension(extensionId) {
    const reference = (await discoverReferenceExtensionManifests()).find(
      (item) => item.manifest.id === extensionId || path.basename(item.extensionPath) === extensionId
    );
    if (!reference) {
      throw new Error(`Reference marketplace extension is not bundled locally: ${extensionId}`);
    }
    if (!reference.hasRunnableBundle) {
      throw new Error(`Reference marketplace extension is not built locally: ${extensionId}`);
    }
    if (/[\\/]/.test(reference.manifest.id)) {
      throw new Error(`Invalid extension id for install path: ${reference.manifest.id}`);
    }

    await deactivateExtension(reference.manifest.id);
    await fs.mkdir(extensionsRoot, { recursive: true });
    const targetPath = path.join(extensionsRoot, reference.manifest.id);
    if (path.resolve(reference.extensionPath) !== path.resolve(targetPath)) {
      await fs.rm(targetPath, { recursive: true, force: true });
      await fs.cp(reference.extensionPath, targetPath, { recursive: true });
    }

    const manifest = await readManifest(targetPath);
    return formatExtensionSummary(loadedExtensions.get(manifest.id), manifest, targetPath);
  }

  async function buildMarketplaceExtension(extensionId) {
    const reference = (await discoverReferenceExtensionManifests()).find(
      (item) => item.manifest.id === extensionId || path.basename(item.extensionPath) === extensionId
    );
    if (!reference) {
      throw new Error(`Reference marketplace extension is not bundled locally: ${extensionId}`);
    }
    if (reference.hasRunnableBundle) {
      return {
        extension: await installMarketplaceExtension(reference.manifest.id),
        build: {
          skipped: true,
          stdout: '',
          stderr: '',
        },
      };
    }
    if (!reference.canBuildFromSource) {
      throw new Error(`Reference marketplace extension has no build script: ${extensionId}`);
    }

    const command = process.platform === 'win32' ? 'npm.cmd' : 'npm';
    const build = await runCommand(command, ['run', 'build', '--prefix', reference.extensionPath], {
      cwd: referenceRoot,
      timeoutMs: 180_000,
    });
    if (build.exitCode !== 0) {
      throw new Error(
        `Failed to build reference marketplace extension ${extensionId}.\n${build.stderr || build.stdout || ''}`
      );
    }
    if (!(await fileExists(path.join(reference.extensionPath, reference.manifest.main)))) {
      throw new Error(`Build completed but main bundle was not created: ${reference.manifest.main}`);
    }

    return {
      extension: await installMarketplaceExtension(reference.manifest.id),
      build: {
        skipped: false,
        stdout: build.stdout,
        stderr: build.stderr,
      },
    };
  }

  async function buildAllMarketplaceExtensions() {
    const references = await discoverReferenceExtensionManifests();
    const results = [];
    for (const reference of references.sort((a, b) => a.manifest.name.localeCompare(b.manifest.name))) {
      try {
        const result = await buildMarketplaceExtension(reference.manifest.id);
        results.push({
          id: reference.manifest.id,
          name: reference.manifest.name,
          success: true,
          extension: result.extension,
          build: result.build,
        });
      } catch (error) {
        results.push({
          id: reference.manifest.id,
          name: reference.manifest.name,
          success: false,
          error: error instanceof Error ? error.message : String(error),
        });
      }
    }
    return {
      installed: results.filter((result) => result.success).length,
      failed: results.filter((result) => !result.success).length,
      results,
    };
  }

  async function listContributions() {
    const extensionPaths = await discoverExtensionPaths();
    const contributions = [];
    for (const extensionPath of extensionPaths) {
      try {
        const manifest = await readManifest(extensionPath);
        contributions.push(...summarizeManifestContributions(manifest));
      } catch {
        // Invalid manifests are surfaced by listExtensions(); the contribution
        // registry keeps only usable extension metadata.
      }
    }
    return contributions.sort(
      (a, b) =>
        a.type.localeCompare(b.type) ||
        a.extensionName.localeCompare(b.extensionName) ||
        a.title.localeCompare(b.title)
    );
  }

  async function findExtensionById(extensionId) {
    const extensionPaths = await discoverExtensionPaths();
    for (const extensionPath of extensionPaths) {
      const manifest = await readManifest(extensionPath);
      if (manifest.id === extensionId || path.basename(extensionPath) === extensionId) {
        return { extensionPath, manifest };
      }
    }
    throw new Error(`Nimbalyst extension not found: ${extensionId}`);
  }

  async function getExtensionAssetPath(extensionId, assetPath) {
    const { extensionPath } = await findExtensionById(extensionId);
    const resolved = assertInsideRoot(extensionPath, assetPath);
    const stat = await fs.stat(resolved);
    if (!stat.isFile()) {
      throw new Error(`Extension asset is not a file: ${assetPath}`);
    }
    return resolved;
  }

  async function getExtensionMainAssetPath(extensionId) {
    const { manifest } = await findExtensionById(extensionId);
    if (!manifest.main) {
      throw new Error(`Extension has no main module: ${extensionId}`);
    }
    return getExtensionAssetPath(extensionId, manifest.main);
  }

  async function loadConfig(manifestId) {
    const configPath = path.join(configurationRoot, `${manifestId}.json`);
    try {
      return JSON.parse(await fs.readFile(configPath, 'utf-8'));
    } catch {
      return {};
    }
  }

  async function saveConfig(manifestId, config) {
    await fs.mkdir(configurationRoot, { recursive: true });
    await fs.writeFile(
      path.join(configurationRoot, `${manifestId}.json`),
      JSON.stringify(config, null, 2),
      'utf-8'
    );
  }

  async function loadBackendModuleGrants(manifestId) {
    const grantPath = path.join(backendGrantRoot, `${manifestId}.json`);
    try {
      return JSON.parse(await fs.readFile(grantPath, 'utf-8'));
    } catch {
      return { global: {}, workspaces: {} };
    }
  }

  async function saveBackendModuleGrants(manifestId, grants) {
    await fs.mkdir(backendGrantRoot, { recursive: true });
    await fs.writeFile(
      path.join(backendGrantRoot, `${manifestId}.json`),
      JSON.stringify(grants, null, 2),
      'utf-8'
    );
  }

  function getBackendModuleWorkerKey(manifestId, moduleId, workspacePath) {
    return `${manifestId}:${moduleId}:${getBackendModuleGrantKey(workspacePath) || 'no-workspace'}`;
  }

  function hasBackendModuleGrant(grants, moduleId, workspacePath) {
    const workspaceKey = getBackendModuleGrantKey(workspacePath);
    return Boolean(
      grants.global?.[moduleId] ||
        (workspaceKey && grants.workspaces?.[workspaceKey]?.[moduleId])
    );
  }

  function rejectBackendWorkerPending(handle, error) {
    for (const pending of handle.pending.values()) {
      clearTimeout(pending.timer);
      pending.resolve({
        success: false,
        error: error.message,
        details: serializeBackendModuleError(error),
      });
    }
    handle.pending.clear();
  }

  async function terminateBackendModuleWorker(key) {
    const handle = backendModuleWorkers.get(key);
    if (!handle) return;
    backendModuleWorkers.delete(key);
    rejectBackendWorkerPending(handle, new Error('Backend module runtime was terminated.'));
    await handle.worker.terminate();
  }

  async function terminateExtensionBackendModuleWorkers(extensionId) {
    const keys = [...backendModuleWorkers.keys()].filter((key) => key.startsWith(`${extensionId}:`));
    await Promise.all(keys.map((key) => terminateBackendModuleWorker(key)));
  }

  async function terminateBackendModuleWorkersForModule(extensionId, moduleId) {
    const keys = [...backendModuleWorkers.keys()].filter((key) => key.startsWith(`${extensionId}:${moduleId}:`));
    await Promise.all(keys.map((key) => terminateBackendModuleWorker(key)));
  }

  function createBackendModuleWorker(key, moduleUrl, runtimeContext) {
    const worker = new Worker(path.join(THIS_DIR, 'nimbalyst-backend-module-worker.js'), {
      workerData: {
        moduleUrl,
        runtimeContext,
      },
    });
    const handle = {
      worker,
      pending: new Map(),
      nextId: 1,
    };

    worker.on('message', (message) => {
      if (!message || message.type !== 'result') {
        if (message?.type === 'log') {
          console[message.level === 'error' ? 'error' : message.level === 'warn' ? 'warn' : 'log'](
            `[Nimbalyst backend ${runtimeContext.extensionId}/${runtimeContext.moduleId}] ${message.message}`,
            message.data ?? ''
          );
        }
        return;
      }

      const pending = handle.pending.get(message.id);
      if (!pending) return;
      clearTimeout(pending.timer);
      handle.pending.delete(message.id);
      if (message.success) {
        pending.resolve({ success: true, data: message.data });
        return;
      }
      pending.resolve({
        success: false,
        error: message.error?.message || 'Backend module execution failed.',
        details: message.error,
      });
    });

    worker.on('error', (error) => {
      rejectBackendWorkerPending(handle, error);
      backendModuleWorkers.delete(key);
    });
    worker.on('exit', (code) => {
      if (backendModuleWorkers.get(key) === handle) {
        backendModuleWorkers.delete(key);
      }
      if (code !== 0) {
        rejectBackendWorkerPending(handle, new Error(`Backend module runtime exited with code ${code}.`));
      }
    });

    backendModuleWorkers.set(key, handle);
    return handle;
  }

  function executeBackendModuleWorker(handle, method, params) {
    return new Promise((resolve) => {
      const id = String(handle.nextId++);
      const timer = setTimeout(() => {
        handle.pending.delete(id);
        resolve({
          success: false,
          error: 'Backend module execution timed out.',
        });
      }, 30000);

      handle.pending.set(id, { resolve, timer });
      try {
        handle.worker.postMessage({
          type: 'execute',
          id,
          method,
          params,
        });
      } catch (error) {
        clearTimeout(timer);
        handle.pending.delete(id);
        resolve({
          success: false,
          error: error instanceof Error ? error.message : String(error),
          details: serializeBackendModuleError(error),
        });
      }
    });
  }

  async function chatCompletion(requestOptions) {
    const apiKey = process.env.OPENAI_API_KEY || options.openAiApiKey;
    if (!apiKey) {
      throw new Error('OPENAI_API_KEY is required for Nimbalyst extension AI services.');
    }
    if (!fetchImpl) {
      throw new Error('fetch is not available in this Node runtime.');
    }

    const defaultModel = process.env.OPENAI_MODEL || options.openAiModel || 'gpt-4o';
    const model = normalizeOpenAIModel(requestOptions.model, defaultModel);
    const messages = [];
    if (requestOptions.systemPrompt) {
      messages.push({ role: 'system', content: requestOptions.systemPrompt });
    }
    messages.push(...(Array.isArray(requestOptions.messages) ? requestOptions.messages : []));

    const responseFormat = buildOpenAIResponseFormat(requestOptions.responseFormat);
    const body = {
      model,
      messages,
      temperature: requestOptions.temperature,
      max_tokens: requestOptions.maxTokens,
      response_format: responseFormat,
    };

    Object.keys(body).forEach((key) => body[key] === undefined && delete body[key]);

    const response = await fetchImpl('https://api.openai.com/v1/chat/completions', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
        Authorization: `Bearer ${apiKey}`,
      },
      body: JSON.stringify(body),
    });

    const payload = await response.json().catch(async () => ({ error: { message: await response.text() } }));
    if (!response.ok) {
      throw new Error(payload?.error?.message || response.statusText || 'OpenAI request failed');
    }

    const choice = payload.choices?.[0]?.message;
    return {
      content: choice?.content ?? '',
      model: payload.model || model,
      usage: payload.usage
        ? {
            inputTokens: payload.usage.prompt_tokens ?? 0,
            outputTokens: payload.usage.completion_tokens ?? 0,
          }
        : undefined,
    };
  }

  async function chatCompletionStream(requestOptions) {
    const apiKey = process.env.OPENAI_API_KEY || options.openAiApiKey;
    if (!apiKey) {
      throw new Error('OPENAI_API_KEY is required for Nimbalyst extension AI services.');
    }
    if (!fetchImpl) {
      throw new Error('fetch is not available in this Node runtime.');
    }

    const controller = new AbortController();
    const defaultModel = process.env.OPENAI_MODEL || options.openAiModel || 'gpt-4o';
    const model = normalizeOpenAIModel(requestOptions.model, defaultModel);
    const messages = [];
    if (requestOptions.systemPrompt) {
      messages.push({ role: 'system', content: requestOptions.systemPrompt });
    }
    messages.push(...(Array.isArray(requestOptions.messages) ? requestOptions.messages : []));

    const responseFormat = buildOpenAIResponseFormat(requestOptions.responseFormat);
    const result = (async () => {
      const response = await fetchImpl('https://api.openai.com/v1/chat/completions', {
        method: 'POST',
        signal: controller.signal,
        headers: {
          'Content-Type': 'application/json',
          Authorization: `Bearer ${apiKey}`,
        },
        body: JSON.stringify({
          model,
          messages,
          stream: true,
          temperature: requestOptions.temperature,
          max_tokens: requestOptions.maxTokens,
          response_format: responseFormat,
        }),
      });

      if (!response.ok) {
        const text = await response.text();
        throw new Error(text || response.statusText || 'OpenAI stream failed');
      }

      let content = '';
      const reader = response.body.getReader();
      const decoder = new TextDecoder();
      while (true) {
        const { done, value } = await reader.read();
        if (done) break;
        const chunk = decoder.decode(value, { stream: true });
        for (const line of chunk.split('\n')) {
          const trimmed = line.trim();
          if (!trimmed.startsWith('data:')) continue;
          const data = trimmed.slice(5).trim();
          if (data === '[DONE]') continue;
          const payload = JSON.parse(data);
          const delta = payload.choices?.[0]?.delta?.content || '';
          if (delta) {
            content += delta;
            requestOptions.onChunk?.({ type: 'text', content: delta });
          }
        }
      }
      requestOptions.onChunk?.({ type: 'done' });
      return { content, model };
    })().catch((error) => {
      requestOptions.onChunk?.({
        type: 'error',
        error: error instanceof Error ? error.message : String(error),
      });
      throw error;
    });

    return {
      abort: () => controller.abort(),
      result,
    };
  }

  function resolveWorkspacePath(targetPath, activeWorkspaceRoot = workspaceRoot) {
    const root = path.resolve(activeWorkspaceRoot || workspaceRoot);
    return assertInsideRoot(root, targetPath);
  }

  function registerExtensionTool(loaded, tool) {
    if (!tool || typeof tool.name !== 'string' || typeof tool.handler !== 'function') {
      throw new Error(`Invalid AI tool exported by ${loaded.manifest.id}`);
    }

    const canonicalName = tool.name.includes('.')
      ? tool.name
      : `${extensionShortId(loaded.manifest.id)}.${tool.name}`;
    const aliases = canonicalName === tool.name ? [canonicalName] : [canonicalName, tool.name];
    const record = {
      name: canonicalName,
      originalName: tool.name,
      description: tool.description || '',
      inputSchema: tool.inputSchema || tool.parameters || { type: 'object', properties: {} },
      scope: tool.scope || 'global',
      extensionId: loaded.manifest.id,
      extensionName: loaded.manifest.name,
      tool,
      aliases,
    };

    for (const alias of aliases) {
      registeredTools.set(alias, { record, loaded });
    }
    loaded.tools = loaded.tools.filter((existing) => existing.name !== canonicalName);
    loaded.tools.push(record);

    return {
      dispose: () => {
        for (const alias of aliases) {
          registeredTools.delete(alias);
        }
        loaded.tools = loaded.tools.filter((existing) => existing.name !== canonicalName);
      },
    };
  }

  async function createContext(loaded, activeWorkspaceRoot = workspaceRoot) {
    const manifest = loaded.manifest;
    loaded.configCache = await loadConfig(manifest.id);
    const subscriptions = loaded.subscriptions;

    const filesystem = {
      readFile: async (filePath) => fs.readFile(resolveWorkspacePath(filePath, activeWorkspaceRoot), 'utf-8'),
      writeFile: async (filePath, content) => {
        const resolved = resolveWorkspacePath(filePath, activeWorkspaceRoot);
        await fs.mkdir(path.dirname(resolved), { recursive: true });
        await fs.writeFile(resolved, String(content), 'utf-8');
      },
      fileExists: async (filePath) => fileExists(resolveWorkspacePath(filePath, activeWorkspaceRoot)),
      findFiles: async (pattern) => {
        const root = path.resolve(activeWorkspaceRoot || workspaceRoot);
        const matcher = globToRegExp(pattern);
        const files = await walkFiles(root);
        return files
          .map((filePath) => toPosixPath(path.relative(root, filePath)))
          .filter((relativePath) => matcher.test(relativePath));
      },
    };

    const services = {
      filesystem,
      ui: {
        showInfo: (message) => console.info(`[${manifest.name}] ${message}`),
        showWarning: (message) => console.warn(`[${manifest.name}] ${message}`),
        showError: (message) => console.error(`[${manifest.name}] ${message}`),
      },
      collab: {
        registerContentAdapter: () => ({ dispose: () => {} }),
      },
    };

    if (manifest.permissions?.ai) {
      services.ai = {
        registerTool: (tool) => {
          const disposable = registerExtensionTool(loaded, tool);
          subscriptions.push(disposable);
          return disposable;
        },
        registerContextProvider: (provider) => {
          if (!provider?.id || typeof provider.provideContext !== 'function') {
            throw new Error('Context provider must include id and provideContext().');
          }
          contextProviders.set(provider.id, { provider, extensionId: manifest.id });
          const disposable = {
            dispose: () => contextProviders.delete(provider.id),
          };
          subscriptions.push(disposable);
          return disposable;
        },
        sendPrompt: async (promptOptions) => {
          const result = await chatCompletion({
            model: promptOptions.model,
            messages: [{ role: 'user', content: promptOptions.prompt }],
          });
          return {
            sessionId: `extension-${Date.now()}`,
            response: result.content,
          };
        },
        listModels: async () => [
          {
            id: `openai:${process.env.OPENAI_MODEL || options.openAiModel || 'gpt-4o'}`,
            name: process.env.OPENAI_MODEL || options.openAiModel || 'gpt-4o',
            provider: 'openai',
          },
        ],
        chatCompletion,
        chatCompletionStream,
      };
    }

    if (manifest.contributions?.configuration) {
      services.configuration = {
        get: (key, defaultValue) => {
          if (Object.prototype.hasOwnProperty.call(loaded.configCache, key)) {
            return loaded.configCache[key];
          }
          const property = manifest.contributions?.configuration?.properties?.[key];
          return property?.default ?? defaultValue;
        },
        update: async (key, value) => {
          loaded.configCache = { ...loaded.configCache, [key]: value };
          await saveConfig(manifest.id, loaded.configCache);
        },
        getAll: () => {
          return { ...getConfigurationDefaults(manifest), ...loaded.configCache };
        },
      };
    }

    return {
      manifest,
      extensionPath: loaded.extensionPath,
      services,
      subscriptions,
    };
  }

  async function activateExtension(extensionId, activateOptions = {}) {
    const existing = loadedExtensions.get(extensionId);
    if (existing?.active) {
      return formatExtensionSummary(existing, existing.manifest, existing.extensionPath);
    }

    const extensionPaths = await discoverExtensionPaths();
    let match = null;
    let manifest = null;
    for (const extensionPath of extensionPaths) {
      const candidateManifest = await readManifest(extensionPath);
      if (candidateManifest.id === extensionId || path.basename(extensionPath) === extensionId) {
        match = extensionPath;
        manifest = candidateManifest;
        break;
      }
    }
    if (!match || !manifest) {
      throw new Error(`Nimbalyst extension not found: ${extensionId}`);
    }

    const loaded = {
      manifest,
      extensionPath: match,
      module: null,
      context: null,
      subscriptions: [],
      tools: [],
      configCache: {},
      active: false,
    };

    loadedExtensions.set(manifest.id, loaded);

    try {
      if (!manifest.main) {
        loaded.context = await createContext(loaded, activateOptions.workspacePath || workspaceRoot);
        loaded.active = true;
        return formatExtensionSummary(loaded, manifest, match);
      }

      const mainPath = assertInsideRoot(match, manifest.main);
      const stat = await fs.stat(mainPath);
      const moduleUrl = `${pathToFileURL(mainPath).href}?mtime=${stat.mtimeMs}`;
      await ensureSdkCompatPackages();

      const importedModule = await import(moduleUrl);
      const extensionModule = importedModule.default?.activate ? importedModule.default : importedModule;
      loaded.module = extensionModule;
      loaded.context = await createContext(loaded, activateOptions.workspacePath || workspaceRoot);

      if (typeof extensionModule.activate === 'function') {
        await extensionModule.activate(loaded.context);
      }

      const exportedTools = Array.isArray(extensionModule.aiTools) ? extensionModule.aiTools : [];
      for (const tool of exportedTools) {
        const disposable = registerExtensionTool(loaded, tool);
        loaded.subscriptions.push(disposable);
      }

      loaded.active = true;
      return formatExtensionSummary(loaded, manifest, match);
    } catch (error) {
      loadedExtensions.delete(manifest.id);
      throw error;
    }
  }

  async function deactivateExtension(extensionId) {
    const loaded = loadedExtensions.get(extensionId);
    if (!loaded) return { id: extensionId, active: false };

    for (const subscription of [...loaded.subscriptions].reverse()) {
      try {
        subscription.dispose();
      } catch (error) {
        console.warn(`[${loaded.manifest.name}] dispose failed:`, error);
      }
    }

    if (typeof loaded.module?.deactivate === 'function') {
      await loaded.module.deactivate();
    }

    await terminateExtensionBackendModuleWorkers(loaded.manifest.id);

    loaded.active = false;
    loaded.tools = [];
    loaded.subscriptions = [];
    loadedExtensions.delete(extensionId);

    return { id: extensionId, active: false };
  }

  function listTools() {
    const seen = new Set();
    const tools = [];
    for (const { record } of registeredTools.values()) {
      if (seen.has(record.name)) continue;
      seen.add(record.name);
      tools.push({
        name: record.name,
        originalName: record.originalName,
        description: record.description,
        inputSchema: record.inputSchema,
        scope: record.scope,
        extensionId: record.extensionId,
        extensionName: record.extensionName,
      });
    }
    return tools.sort((a, b) => a.name.localeCompare(b.name));
  }

  function listExecutableSlashCommands() {
    const commands = [];
    for (const loaded of loadedExtensions.values()) {
      if (!loaded.active) continue;
      const contributions = Array.isArray(loaded.manifest.contributions?.slashCommands)
        ? loaded.manifest.contributions.slashCommands
        : [];
      const handlers = loaded.module?.slashCommandHandlers || {};

      for (const contribution of contributions) {
        const handler = handlers[contribution.handler] || loaded.module?.[contribution.handler];
        commands.push({
          id: contribution.id,
          title: contribution.title,
          description: contribution.description || '',
          icon: contribution.icon,
          keywords: contribution.keywords || [],
          handler: contribution.handler,
          executable: typeof handler === 'function',
          extensionId: loaded.manifest.id,
          extensionName: loaded.manifest.name,
        });
      }
    }
    return commands.sort((a, b) => a.title.localeCompare(b.title));
  }

  function listExecutableCommands() {
    const commands = [];
    for (const loaded of loadedExtensions.values()) {
      if (!loaded.active) continue;
      const contributions = Array.isArray(loaded.manifest.contributions?.commands)
        ? loaded.manifest.contributions.commands
        : [];
      const handlers = loaded.module?.commandHandlers || {};

      for (const contribution of contributions) {
        const handler = handlers[contribution.id] || loaded.module?.[contribution.id];
        commands.push({
          id: contribution.id,
          title: contribution.title || contribution.id,
          executable: typeof handler === 'function',
          extensionId: loaded.manifest.id,
          extensionName: loaded.manifest.name,
        });
      }
    }
    return commands.sort((a, b) => a.title.localeCompare(b.title));
  }

  async function executeTool(toolName, args = {}, executeOptions = {}) {
    const entry = registeredTools.get(toolName);
    if (!entry) {
      return {
        success: false,
        error: `Extension tool not found: ${toolName}`,
        toolName,
        errorContext: {
          availableTools: listTools().map((tool) => tool.name),
        },
      };
    }

    const { record, loaded } = entry;
    try {
      const result = await record.tool.handler(args, {
        workspacePath: executeOptions.workspacePath || workspaceRoot,
        activeFilePath: executeOptions.activeFilePath,
        extensionContext: loaded.context,
        editorAPI: undefined,
      });

      return {
        ...normalizeToolResult(result),
        extensionId: loaded.manifest.id,
        toolName: record.name,
      };
    } catch (error) {
      return {
        success: false,
        error: error instanceof Error ? error.message : String(error),
        extensionId: loaded.manifest.id,
        toolName: record.name,
        stack: error instanceof Error ? error.stack : undefined,
      };
    }
  }

  async function executeCommand(commandId, args = {}, executeOptions = {}) {
    for (const loaded of loadedExtensions.values()) {
      if (!loaded.active) continue;
      const contributions = Array.isArray(loaded.manifest.contributions?.commands)
        ? loaded.manifest.contributions.commands
        : [];
      const contribution = contributions.find((item) => item.id === commandId);
      if (!contribution) continue;

      const handlers = loaded.module?.commandHandlers || {};
      const handler = handlers[contribution.id] || loaded.module?.[contribution.id];
      if (typeof handler !== 'function') {
        return {
          success: false,
          error: `Command handler not found: ${contribution.id}`,
          extensionId: loaded.manifest.id,
          commandId,
        };
      }

      try {
        const result = await handler(args || {}, {
          workspacePath: executeOptions.workspacePath || workspaceRoot,
          activeFilePath: executeOptions.activeFilePath,
          extensionContext: loaded.context,
        });
        return {
          ...normalizeToolResult(result),
          extensionId: loaded.manifest.id,
          commandId,
        };
      } catch (error) {
        return {
          success: false,
          error: error instanceof Error ? error.message : String(error),
          extensionId: loaded.manifest.id,
          commandId,
          stack: error instanceof Error ? error.stack : undefined,
        };
      }
    }

    return {
      success: false,
      error: `Executable command not found: ${commandId}`,
      commandId,
      errorContext: {
        availableCommands: listExecutableCommands().map((command) => command.id),
      },
    };
  }

  async function executeSlashCommand(commandId, args = '', executeOptions = {}) {
    for (const loaded of loadedExtensions.values()) {
      if (!loaded.active) continue;
      const contributions = Array.isArray(loaded.manifest.contributions?.slashCommands)
        ? loaded.manifest.contributions.slashCommands
        : [];
      const contribution = contributions.find((item) => item.id === commandId);
      if (!contribution) continue;

      const handlers = loaded.module?.slashCommandHandlers || {};
      const handler = handlers[contribution.handler] || loaded.module?.[contribution.handler];
      if (typeof handler !== 'function') {
        return {
          success: false,
          error: `Slash command handler not found: ${contribution.handler}`,
          extensionId: loaded.manifest.id,
          commandId,
        };
      }

      try {
        const result = await handler(String(args ?? ''), {
          workspacePath: executeOptions.workspacePath || workspaceRoot,
          activeFilePath: executeOptions.activeFilePath,
          extensionContext: loaded.context,
        });
        return {
          ...normalizeToolResult(result),
          extensionId: loaded.manifest.id,
          commandId,
        };
      } catch (error) {
        return {
          success: false,
          error: error instanceof Error ? error.message : String(error),
          extensionId: loaded.manifest.id,
          commandId,
          stack: error instanceof Error ? error.stack : undefined,
        };
      }
    }

    return {
      success: false,
      error: `Executable slash command not found: ${commandId}`,
      commandId,
      errorContext: {
        availableCommands: listExecutableSlashCommands().map((command) => command.id),
      },
    };
  }

  async function reloadExtension(extensionId, activateOptions = {}) {
    await deactivateExtension(extensionId);
    return activateExtension(extensionId, activateOptions);
  }

  async function getExtensionConfiguration(extensionId) {
    const { manifest } = await findExtensionById(extensionId);
    const configuration = manifest.contributions?.configuration;
    const properties = isPlainObject(configuration?.properties) ? configuration.properties : {};
    const loaded = loadedExtensions.get(manifest.id);
    const savedConfig = loaded?.configCache ?? (await loadConfig(manifest.id));

    return {
      extensionId: manifest.id,
      extensionName: manifest.name,
      title: configuration?.title || 'Configuration',
      properties,
      values: { ...getConfigurationDefaults(manifest), ...savedConfig },
    };
  }

  async function updateExtensionConfiguration(extensionId, values) {
    if (!isPlainObject(values)) {
      throw new Error('Extension configuration values must be an object.');
    }

    const { manifest } = await findExtensionById(extensionId);
    const properties = manifest.contributions?.configuration?.properties ?? {};
    const current = await loadConfig(manifest.id);
    const declaredKeys = new Set(Object.keys(properties));
    const next = { ...current };

    for (const [key, value] of Object.entries(values)) {
      if (declaredKeys.size > 0 && !declaredKeys.has(key)) continue;
      next[key] = value;
    }

    await saveConfig(manifest.id, next);
    const loaded = loadedExtensions.get(manifest.id);
    if (loaded) {
      loaded.configCache = next;
    }

    return getExtensionConfiguration(manifest.id);
  }

  async function getExtensionBackendModules(extensionId, requestOptions = {}) {
    const { manifest, extensionPath } = await findExtensionById(extensionId);
    const backendModules = Array.isArray(manifest.contributions?.backendModules)
      ? manifest.contributions.backendModules
      : [];
    const issues = validateBackendModules(manifest.contributions?.backendModules);
    const grants = await loadBackendModuleGrants(manifest.id);
    const workspaceKey = getBackendModuleGrantKey(requestOptions.workspacePath);
    const workspaceGrants = workspaceKey ? grants.workspaces?.[workspaceKey] || {} : {};

    const modules = await Promise.all(
      backendModules.map(async (module) => {
        const moduleIssues = issues.filter((issue) => issue.moduleId === module.id);
        const entryPath = typeof module.entry === 'string' && !module.entry.includes('..') && !module.entry.startsWith('/')
          ? path.join(extensionPath, module.entry)
          : null;

        return {
          id: module.id,
          entry: module.entry,
          runtime: module.runtime,
          permissions: effectiveBackendModulePermissions(module.permissions),
          declaredPermissions: Array.isArray(module.permissions) ? module.permissions : [],
          enablement: module.enablement || null,
          entryExists: entryPath ? await fileExists(entryPath) : false,
          globalEnabled: Boolean(grants.global?.[module.id]),
          workspaceEnabled: Boolean(workspaceGrants[module.id]),
          issues: moduleIssues,
        };
      })
    );

    return {
      extensionId: manifest.id,
      extensionName: manifest.name,
      workspacePath: requestOptions.workspacePath || '',
      permissions: BACKEND_PERMISSION_DESCRIPTORS,
      modules,
      issues,
    };
  }

  async function executeBackendModule(extensionId, moduleId, method, params = {}, requestOptions = {}) {
    const { manifest, extensionPath } = await findExtensionById(extensionId);
    const modules = Array.isArray(manifest.contributions?.backendModules)
      ? manifest.contributions.backendModules
      : [];
    const module = modules.find((item) => item.id === moduleId);
    if (!module) {
      return {
        success: false,
        error: `Backend module not found: ${extensionId}.${moduleId}`,
      };
    }

    const fatalIssues = validateBackendModules(modules)
      .filter((issue) => issue.severity !== 'warning' && (!issue.moduleId || issue.moduleId === moduleId));
    if (fatalIssues.length > 0) {
      return {
        success: false,
        error: `Backend module declaration is invalid: ${fatalIssues.map((issue) => issue.message).join('; ')}`,
        issues: fatalIssues,
      };
    }
    if (module.runtime !== 'worker-thread') {
      return {
        success: false,
        error: `Backend module runtime is not supported yet: ${module.runtime}`,
      };
    }

    const workspacePath = requestOptions.workspacePath || workspaceRoot;
    const grants = await loadBackendModuleGrants(manifest.id);
    if (!hasBackendModuleGrant(grants, module.id, workspacePath)) {
      return {
        success: false,
        error: 'Backend module is not enabled for this workspace or globally.',
      };
    }

    let entryPath;
    try {
      entryPath = assertInsideRoot(extensionPath, module.entry);
    } catch (error) {
      return {
        success: false,
        error: error instanceof Error ? error.message : String(error),
      };
    }
    if (!(await fileExists(entryPath))) {
      return {
        success: false,
        error: `Backend module entry file does not exist: ${module.entry}`,
      };
    }

    const stat = await fs.stat(entryPath);
    const moduleUrl = `${pathToFileURL(entryPath).href}?mtime=${stat.mtimeMs}`;
    const runtimeContext = {
      extensionId: manifest.id,
      moduleId: module.id,
      workspacePath,
      extensionPath,
      entryFilePath: entryPath,
      grantedPermissions: effectiveBackendModulePermissions(module.permissions),
    };
    const workerKey = getBackendModuleWorkerKey(manifest.id, module.id, workspacePath);
    const handle =
      backendModuleWorkers.get(workerKey) ||
      createBackendModuleWorker(workerKey, moduleUrl, runtimeContext);

    return executeBackendModuleWorker(handle, method || 'describe', params);
  }

  async function setExtensionBackendModuleGrant(extensionId, moduleId, grantOptions = {}) {
    const { manifest } = await findExtensionById(extensionId);
    const modules = Array.isArray(manifest.contributions?.backendModules)
      ? manifest.contributions.backendModules
      : [];
    const module = modules.find((item) => item.id === moduleId);
    if (!module) {
      throw new Error(`Backend module not found: ${extensionId}.${moduleId}`);
    }

    const fatalIssues = validateBackendModules(modules)
      .filter((issue) => issue.severity !== 'warning' && (!issue.moduleId || issue.moduleId === moduleId));
    if (fatalIssues.length > 0) {
      throw new Error(`Backend module declaration is invalid: ${fatalIssues.map((issue) => issue.message).join('; ')}`);
    }

    const scope = grantOptions.scope === 'global' ? 'global' : 'workspace';
    const enabled = grantOptions.enabled !== false;
    const grants = await loadBackendModuleGrants(manifest.id);

    if (scope === 'global') {
      grants.global = { ...(grants.global || {}), [moduleId]: enabled };
      if (!enabled) delete grants.global[moduleId];
    } else {
      const workspaceKey = getBackendModuleGrantKey(grantOptions.workspacePath);
      if (!workspaceKey) {
        throw new Error('workspacePath is required for workspace backend module grants.');
      }
      grants.workspaces = grants.workspaces || {};
      grants.workspaces[workspaceKey] = { ...(grants.workspaces[workspaceKey] || {}), [moduleId]: enabled };
      if (!enabled) delete grants.workspaces[workspaceKey][moduleId];
      if (Object.keys(grants.workspaces[workspaceKey]).length === 0) {
        delete grants.workspaces[workspaceKey];
      }
    }

    await saveBackendModuleGrants(manifest.id, grants);
    if (!enabled) {
      await terminateBackendModuleWorkersForModule(manifest.id, moduleId);
    }
    return getExtensionBackendModules(manifest.id, { workspacePath: grantOptions.workspacePath });
  }

  return {
    extensionsRoot,
    workspaceRoot,
    listExtensions,
    listMarketplaceExtensions,
    installMarketplaceExtension,
    buildMarketplaceExtension,
    buildAllMarketplaceExtensions,
    listContributions,
    getExtensionAssetPath,
    getExtensionMainAssetPath,
    activateExtension,
    deactivateExtension,
    reloadExtension,
    listTools,
    executeTool,
    listExecutableCommands,
    executeCommand,
    listExecutableSlashCommands,
    executeSlashCommand,
    getExtensionConfiguration,
    updateExtensionConfiguration,
    getExtensionBackendModules,
    executeBackendModule,
    setExtensionBackendModuleGrant,
  };
}

const defaultHost = createNimbalystExtensionHost();

export const listNimbalystExtensions = () => defaultHost.listExtensions();
export const listNimbalystMarketplaceExtensions = () => defaultHost.listMarketplaceExtensions();
export const installNimbalystMarketplaceExtension = (extensionId) =>
  defaultHost.installMarketplaceExtension(extensionId);
export const buildNimbalystMarketplaceExtension = (extensionId) =>
  defaultHost.buildMarketplaceExtension(extensionId);
export const buildAllNimbalystMarketplaceExtensions = () =>
  defaultHost.buildAllMarketplaceExtensions();
export const listNimbalystExtensionContributions = () => defaultHost.listContributions();
export const getNimbalystExtensionAssetPath = (extensionId, assetPath) =>
  defaultHost.getExtensionAssetPath(extensionId, assetPath);
export const getNimbalystExtensionMainAssetPath = (extensionId) =>
  defaultHost.getExtensionMainAssetPath(extensionId);
export const activateNimbalystExtension = (extensionId, options) =>
  defaultHost.activateExtension(extensionId, options);
export const deactivateNimbalystExtension = (extensionId) =>
  defaultHost.deactivateExtension(extensionId);
export const reloadNimbalystExtension = (extensionId, options) =>
  defaultHost.reloadExtension(extensionId, options);
export const listNimbalystExtensionTools = () => defaultHost.listTools();
export const listNimbalystCommands = () => defaultHost.listExecutableCommands();
export const executeNimbalystCommand = (commandId, args, options) =>
  defaultHost.executeCommand(commandId, args, options);
export const executeNimbalystExtensionTool = (toolName, args, options) =>
  defaultHost.executeTool(toolName, args, options);
export const listNimbalystSlashCommands = () => defaultHost.listExecutableSlashCommands();
export const executeNimbalystSlashCommand = (commandId, args, options) =>
  defaultHost.executeSlashCommand(commandId, args, options);
export const getNimbalystExtensionConfiguration = (extensionId) =>
  defaultHost.getExtensionConfiguration(extensionId);
export const updateNimbalystExtensionConfiguration = (extensionId, values) =>
  defaultHost.updateExtensionConfiguration(extensionId, values);
export const getNimbalystExtensionBackendModules = (extensionId, options) =>
  defaultHost.getExtensionBackendModules(extensionId, options);
export const executeNimbalystBackendModule = (extensionId, moduleId, method, params, options) =>
  defaultHost.executeBackendModule(extensionId, moduleId, method, params, options);
export const setNimbalystExtensionBackendModuleGrant = (extensionId, moduleId, options) =>
  defaultHost.setExtensionBackendModuleGrant(extensionId, moduleId, options);
