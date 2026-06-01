import fs from 'fs/promises';
import path from 'path';

const PYTHON_RESERVED = new Set([
  'False',
  'None',
  'True',
  'and',
  'as',
  'assert',
  'async',
  'await',
  'break',
  'class',
  'continue',
  'def',
  'del',
  'elif',
  'else',
  'except',
  'finally',
  'for',
  'from',
  'global',
  'if',
  'import',
  'in',
  'is',
  'lambda',
  'nonlocal',
  'not',
  'or',
  'pass',
  'raise',
  'return',
  'try',
  'while',
  'with',
  'yield',
]);

const isObject = (value) => value && typeof value === 'object' && !Array.isArray(value);

const deepClone = (value) => {
  try {
    return JSON.parse(JSON.stringify(value));
  } catch {
    return value;
  }
};

export function parseSchemaInput(value, fieldName = 'schema') {
  if (typeof value === 'string') {
    try {
      return JSON.parse(value);
    } catch (error) {
      throw new Error(`${fieldName} is not valid JSON: ${error.message}`);
    }
  }
  if (!isObject(value)) {
    throw new Error(`${fieldName} must be a JSON object or JSON string`);
  }
  return value;
}

function safeSlug(value, fallback = 'model') {
  const slug = String(value || '')
    .trim()
    .replace(/([a-z0-9])([A-Z])/g, '$1_$2')
    .replace(/[^a-zA-Z0-9]+/g, '_')
    .replace(/^_+|_+$/g, '')
    .toLowerCase();
  return slug || fallback;
}

function toPascalCase(value, fallback = 'Model') {
  const words = String(value || '')
    .replace(/([a-z0-9])([A-Z])/g, '$1 $2')
    .split(/[^a-zA-Z0-9]+/)
    .filter(Boolean);
  const name = words.map((word) => word.charAt(0).toUpperCase() + word.slice(1)).join('');
  if (!name) return fallback;
  return /^[0-9]/.test(name) ? `${fallback}${name}` : name;
}

function toSnakeCase(value, fallback = 'field') {
  let name = String(value || '')
    .trim()
    .replace(/([a-z0-9])([A-Z])/g, '$1_$2')
    .replace(/[^a-zA-Z0-9]+/g, '_')
    .replace(/^_+|_+$/g, '')
    .toLowerCase();
  if (!name) name = fallback;
  if (/^[0-9]/.test(name)) name = `${fallback}_${name}`;
  if (PYTHON_RESERVED.has(name)) name = `${name}_`;
  return name;
}

function pyString(value) {
  return JSON.stringify(String(value));
}

function pyLiteral(value) {
  if (value === null) return 'None';
  if (value === true) return 'True';
  if (value === false) return 'False';
  if (typeof value === 'number') return Number.isFinite(value) ? String(value) : 'None';
  if (typeof value === 'string') return pyString(value);
  return 'None';
}

function isDefaultCompatible(schema, defaultValue) {
  if (defaultValue === null || defaultValue === undefined) return false;
  const type = schema?.type;
  if (Array.isArray(type)) return type.some((item) => isDefaultCompatible({ ...schema, type: item }, defaultValue));
  if (type === 'object') return isObject(defaultValue);
  if (type === 'array') return Array.isArray(defaultValue);
  if (type === 'integer') return Number.isInteger(defaultValue);
  if (type === 'number') return typeof defaultValue === 'number';
  if (type === 'boolean') return typeof defaultValue === 'boolean';
  if (type === 'string') return typeof defaultValue === 'string';
  return ['string', 'number', 'boolean'].includes(typeof defaultValue);
}

function compactUnion(types) {
  const unique = [...new Set(types.filter(Boolean))];
  if (unique.length === 0) return 'Any';
  if (unique.length === 1) return unique[0];
  return unique.join(' | ');
}

function resolveRefName(ref) {
  if (typeof ref !== 'string') return null;
  const match = ref.match(/^#\/components\/schemas\/(.+)$/);
  return match ? match[1] : null;
}

function flattenComponentSchemas(schemas, prefix = '') {
  if (!isObject(schemas)) return {};
  const result = {};
  for (const [key, value] of Object.entries(schemas)) {
    if (!isObject(value)) continue;
    const nextKey = prefix ? `${prefix}/${key}` : key;
    if (
      Object.prototype.hasOwnProperty.call(value, 'type') ||
      Object.prototype.hasOwnProperty.call(value, 'properties') ||
      Object.prototype.hasOwnProperty.call(value, '$ref') ||
      Object.prototype.hasOwnProperty.call(value, 'oneOf') ||
      Object.prototype.hasOwnProperty.call(value, 'anyOf') ||
      Object.prototype.hasOwnProperty.call(value, 'allOf')
    ) {
      result[nextKey] = value;
    } else {
      Object.assign(result, flattenComponentSchemas(value, nextKey));
    }
  }
  return result;
}

function normalizeSourceSchema(sourceSchema) {
  const schema = deepClone(sourceSchema);
  const components = flattenComponentSchemas(schema?.components?.schemas || {});
  const roots = [];

  if (isObject(schema?.request)) {
    roots.push({ suffix: 'Request', schema: schema.request });
  }
  if (isObject(schema?.response) && Object.keys(schema.response).length > 0) {
    roots.push({ suffix: 'Response', schema: schema.response });
  }

  if (roots.length === 0) {
    roots.push({ suffix: 'Request', schema });
  }

  return { roots, components };
}

class PydanticBuilder {
  constructor({ schema, endpoint }) {
    const normalized = normalizeSourceSchema(schema);
    this.roots = normalized.roots;
    this.components = normalized.components;
    this.endpoint = endpoint || {};
    this.prefix = toPascalCase(this.endpoint.name || this.endpoint.id || 'Endpoint', 'Endpoint');
    this.imports = new Set(['Any', 'Literal']);
    this.fieldImportNeeded = false;
    this.modelBlocks = [];
    this.modelNames = new Map();
    this.emitted = new Set();
  }

  reserveModelName(key, fallback) {
    if (this.modelNames.has(key)) return this.modelNames.get(key);
    const base = toPascalCase(fallback, 'Model');
    let name = base;
    let index = 2;
    const used = new Set(this.modelNames.values());
    while (used.has(name)) {
      name = `${base}${index}`;
      index += 1;
    }
    this.modelNames.set(key, name);
    return name;
  }

  typeFor(schema, suggestedName) {
    if (!isObject(schema)) return 'Any';

    if (schema.$ref) {
      const refName = resolveRefName(schema.$ref);
      if (refName && this.components[refName]) {
        const className = this.reserveModelName(`component:${refName}`, `${this.prefix}${toPascalCase(refName)}`);
        this.emitModel(className, this.components[refName], `component:${refName}`);
        return className;
      }
      return 'Any';
    }

    if (Array.isArray(schema.enum) && schema.enum.length > 0) {
      return `Literal[${schema.enum.map(pyLiteral).join(', ')}]`;
    }

    const constSchemas = [...(schema.oneOf || []), ...(schema.anyOf || [])].filter((item) =>
      Object.prototype.hasOwnProperty.call(item || {}, 'const')
    );
    if (constSchemas.length > 0 && constSchemas.length === (schema.oneOf || schema.anyOf || []).length) {
      return `Literal[${constSchemas.map((item) => pyLiteral(item.const)).join(', ')}]`;
    }

    if (Array.isArray(schema.allOf) && schema.allOf.length === 1) {
      return this.typeFor(schema.allOf[0], suggestedName);
    }
    if (Array.isArray(schema.oneOf) && schema.oneOf.length > 0) {
      return compactUnion(schema.oneOf.map((item, index) => this.typeFor(item, `${suggestedName}${index + 1}`)));
    }
    if (Array.isArray(schema.anyOf) && schema.anyOf.length > 0) {
      return compactUnion(schema.anyOf.map((item, index) => this.typeFor(item, `${suggestedName}${index + 1}`)));
    }

    const type = Array.isArray(schema.type) ? schema.type.find((item) => item !== 'null') : schema.type;
    if (type === 'array') {
      return `list[${this.typeFor(schema.items || {}, `${suggestedName}Item`)}]`;
    }
    if (type === 'object' || schema.properties) {
      if (schema.properties && isObject(schema.properties)) {
        const className = this.reserveModelName(`inline:${suggestedName}:${this.modelBlocks.length}`, suggestedName);
        this.emitModel(className, schema, `inline:${suggestedName}:${this.modelBlocks.length}`);
        return className;
      }
      if (schema.additionalProperties && isObject(schema.additionalProperties)) {
        return `dict[str, ${this.typeFor(schema.additionalProperties, `${suggestedName}Value`)}]`;
      }
      return 'dict[str, Any]';
    }
    if (type === 'integer') return 'int';
    if (type === 'number') return 'float';
    if (type === 'boolean') return 'bool';
    if (type === 'string') return 'str';
    return 'Any';
  }

  emitModel(className, schema, key) {
    if (this.emitted.has(key)) return;
    this.emitted.add(key);

    const properties = isObject(schema?.properties) ? schema.properties : {};
    const required = new Set(Array.isArray(schema?.required) ? schema.required : []);
    const lines = [`class ${className}(BaseModel):`, `    model_config = ConfigDict(populate_by_name=True, extra='forbid')`];

    const entries = Object.entries(properties);
    if (entries.length === 0) {
      lines.push('    pass');
      this.modelBlocks.push(lines.join('\n'));
      return;
    }

    for (const [rawKey, childSchema] of entries) {
      const fieldName = toSnakeCase(rawKey);
      const baseType = this.typeFor(childSchema, `${className}${toPascalCase(rawKey)}`);
      const isRequired = required.has(rawKey);
      const needsAlias = fieldName !== rawKey;
      let typeName = baseType;
      let assignment = '';

      if (!isRequired) {
        typeName = `${baseType} | None`;
        assignment = ' = None';
      }

      if (Object.prototype.hasOwnProperty.call(childSchema || {}, 'default') && isDefaultCompatible(childSchema, childSchema.default)) {
        assignment = ` = ${needsAlias ? `Field(${pyLiteral(childSchema.default)}, alias=${pyString(rawKey)})` : pyLiteral(childSchema.default)}`;
        if (!isRequired) typeName = baseType;
      } else if (needsAlias) {
        assignment = ` = Field(${isRequired ? '...' : 'None'}, alias=${pyString(rawKey)})`;
      }

      if (needsAlias) this.fieldImportNeeded = true;
      lines.push(`    ${fieldName}: ${typeName}${assignment}`);
    }

    this.modelBlocks.push(lines.join('\n'));
  }

  build() {
    const rootClasses = [];
    const rootInfos = [];
    for (const root of this.roots) {
      const className = this.reserveModelName(`root:${root.suffix}`, `${this.prefix}${root.suffix}`);
      rootClasses.push(className);
      rootInfos.push({ ...root, className });
      this.emitModel(className, root.schema, `root:${root.suffix}`);
    }

    const importNames = ['BaseModel', 'ConfigDict'];
    if (this.fieldImportNeeded) importNames.push('Field');
    const requestRoot = rootInfos.find((root) => root.suffix === 'Request');
    const helperLines = requestRoot ? [
      `ENDPOINT_ID = ${pyString(this.endpoint.id || '')}`,
      `DEFAULT_METHOD = ${pyString(String(this.endpoint.method || 'POST').split(',')[0].trim().toUpperCase() || 'POST')}`,
      `DEFAULT_PATH = ${pyString(this.endpoint.path || '')}`,
      '',
      'from generated_pydantic.client import endpoint as _endpoint',
      '',
      '_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)',
      'build_payload = _command.build_payload',
      'run = _command.run',
      '',
    ] : [];

    const lines = [
      'from __future__ import annotations',
      '',
      `from typing import ${[...this.imports].sort().join(', ')}`,
      `from pydantic import ${importNames.join(', ')}`,
      '',
      '',
      `# Generated from ${this.endpoint.name || this.endpoint.id || 'endpoint'} (${this.endpoint.method || ''} ${this.endpoint.path || ''})`,
      '',
      ...this.modelBlocks.flatMap((block) => [block, '']),
      ...helperLines,
      `__all__ = [${rootClasses.map(pyString).join(', ')}]`,
      '',
    ];

    return { code: lines.join('\n'), rootClasses };
  }
}

export function generatePydanticCode({ schema, endpoint }) {
  const parsed = parseSchemaInput(schema);
  const builder = new PydanticBuilder({ schema: parsed, endpoint });
  return builder.build();
}

export function buildPydanticPaths({ endpoint, outputRoot }) {
  const productId = safeSlug(endpoint?.productId || endpoint?.product_id || endpoint?.product || 'verification', 'verification');
  const moduleName = safeSlug(endpoint?.id || endpoint?.name || 'endpoint', 'endpoint');
  const productDir = path.join(outputRoot, productId);
  return {
    productId,
    moduleName,
    productDir,
    filePath: path.join(productDir, `${moduleName}.py`),
    manifestPath: path.join(outputRoot, 'manifest.json'),
    registryPath: path.join(outputRoot, 'registry.py'),
    rootInitPath: path.join(outputRoot, '__init__.py'),
    productInitPath: path.join(productDir, '__init__.py'),
  };
}

async function readManifest(manifestPath) {
  try {
    return JSON.parse(await fs.readFile(manifestPath, 'utf-8'));
  } catch {
    return { modules: [] };
  }
}

async function writeRegistry(outputRoot, manifest) {
  const modules = Array.isArray(manifest.modules) ? manifest.modules : [];
  const registryLines = [
    'from __future__ import annotations',
    '',
    'from importlib import import_module',
    'from types import ModuleType',
    'from typing import Any',
    '',
    '# Auto-generated by the API Verification Pydantic tab.',
    '',
    'PYDANTIC_MODULES = {',
  ];
  for (const item of modules) {
    registryLines.push(`    ${pyString(item.endpointId)}: ${pyString(item.importPath)},`);
  }
  registryLines.push('}', '');
  registryLines.push('PYDANTIC_MODEL_CLASSES = {');
  for (const item of modules) {
    registryLines.push(`    ${pyString(item.endpointId)}: ${JSON.stringify(item.rootClasses || [])},`);
  }
  registryLines.push('}', '');
  registryLines.push('');
  registryLines.push('def get_module(endpoint_id: str) -> ModuleType:');
  registryLines.push('    """Import and return the generated module for an endpoint id."""');
  registryLines.push('    module_path = PYDANTIC_MODULES[endpoint_id]');
  registryLines.push('    return import_module(module_path)');
  registryLines.push('');
  registryLines.push('def get_models(endpoint_id: str) -> dict[str, type[Any]]:');
  registryLines.push('    """Return generated Pydantic model classes for an endpoint id."""');
  registryLines.push('    module = get_module(endpoint_id)');
  registryLines.push('    return {name: getattr(module, name) for name in PYDANTIC_MODEL_CLASSES[endpoint_id]}');
  registryLines.push('');
  registryLines.push('def get_model(endpoint_id: str, class_name: str | None = None) -> type[Any]:');
  registryLines.push('    """Return one generated model class. Defaults to the first model for the endpoint."""');
  registryLines.push('    models = get_models(endpoint_id)');
  registryLines.push('    if class_name is None:');
  registryLines.push('        return next(iter(models.values()))');
  registryLines.push('    return models[class_name]');
  registryLines.push('');

  await fs.writeFile(path.join(outputRoot, 'registry.py'), registryLines.join('\n'), 'utf-8');
  await fs.writeFile(
    path.join(outputRoot, '__init__.py'),
    [
      'from typing import Any',
      '',
      'from .client import EndpointCommand, EndpointResult, build_request, endpoint, run_endpoint, validate_response',
      'from .registry import PYDANTIC_MODEL_CLASSES, PYDANTIC_MODULES, get_model, get_models, get_module',
      '',
      '',
      'def _make_endpoint_function(endpoint_id: str, function_name: str):',
      '    def _run(',
      '        *,',
      '        url: str,',
      '        headers: dict[str, str] | None = None,',
      '        method: str | None = None,',
      '        timeout: int = 30,',
      '        **values: Any,',
      '    ) -> EndpointResult:',
      '        return endpoint(endpoint_id, method=method or "POST").run(',
      '            url=url,',
      '            headers=headers,',
      '            timeout=timeout,',
      '            **values,',
      '        )',
      '',
      '    _run.__name__ = function_name',
      '    _run.__doc__ = f"Run generated Pydantic endpoint helper for {endpoint_id}."',
      '    return _run',
      '',
      '',
      'for _endpoint_id, _module_path in PYDANTIC_MODULES.items():',
      '    _function_name = _module_path.rsplit(".", 1)[-1]',
      '    if _function_name.isidentifier() and _function_name not in globals():',
      '        globals()[_function_name] = _make_endpoint_function(_endpoint_id, _function_name)',
      '',
      '',
      '__all__ = [',
      '    "EndpointCommand",',
      '    "EndpointResult",',
      '    "PYDANTIC_MODEL_CLASSES",',
      '    "PYDANTIC_MODULES",',
      '    "build_request",',
      '    "endpoint",',
      '    "get_model",',
      '    "get_models",',
      '    "get_module",',
      '    "run_endpoint",',
      '    "validate_response",',
      '    *[',
      '        _module_path.rsplit(".", 1)[-1]',
      '        for _module_path in PYDANTIC_MODULES.values()',
      '        if _module_path.rsplit(".", 1)[-1].isidentifier()',
      '    ],',
      ']',
      '',
      'del _endpoint_id, _module_path, _function_name',
      '',
    ].join('\n'),
    'utf-8'
  );
}

async function writeProductInit(productDir) {
  const entries = await fs.readdir(productDir, { withFileTypes: true });
  const modules = entries
    .filter((entry) => entry.isFile() && entry.name.endsWith('.py') && entry.name !== '__init__.py')
    .map((entry) => entry.name.replace(/\.py$/, ''))
    .sort();
  const lines = ['# Auto-generated package exports.'];
  for (const moduleName of modules) {
    lines.push(`from .${moduleName} import *`);
  }
  lines.push('');
  await fs.writeFile(path.join(productDir, '__init__.py'), lines.join('\n'), 'utf-8');
}

export async function savePydanticModule({ schema, endpoint, outputRoot }) {
  const paths = buildPydanticPaths({ endpoint, outputRoot });
  const generated = generatePydanticCode({ schema, endpoint });
  await fs.mkdir(paths.productDir, { recursive: true });
  await fs.writeFile(paths.filePath, generated.code, 'utf-8');

  const manifest = await readManifest(paths.manifestPath);
  const importPath = `generated_pydantic.${paths.productId}.${paths.moduleName}`;
  const nextItem = {
    endpointId: endpoint?.id || paths.moduleName,
    endpointName: endpoint?.name || '',
    productId: paths.productId,
    moduleName: paths.moduleName,
    importPath,
    filePath: path.relative(outputRoot, paths.filePath).replace(/\\/g, '/'),
    rootClasses: generated.rootClasses,
    updatedAt: new Date().toISOString(),
  };

  manifest.modules = [
    ...(manifest.modules || []).filter((item) => item.endpointId !== nextItem.endpointId),
    nextItem,
  ].sort((a, b) => String(a.endpointId).localeCompare(String(b.endpointId)));

  await fs.writeFile(paths.manifestPath, JSON.stringify(manifest, null, 2), 'utf-8');
  await writeProductInit(paths.productDir);
  await writeRegistry(outputRoot, manifest);

  return {
    ...generated,
    filePath: paths.filePath,
    registryPath: paths.registryPath,
    manifestPath: paths.manifestPath,
    moduleName: paths.moduleName,
    productId: paths.productId,
    importPath,
  };
}
