import type { Settings } from '@/types';

export function parseSpecJson(value: any, fallback: any = null) {
  if (!value) return fallback;
  if (typeof value === 'string') {
    try {
      return JSON.parse(value);
    } catch {
      return fallback;
    }
  }
  if (typeof value === 'object') return value;
  return fallback;
}

export function isMeaningfulSchema(value: any) {
  if (!value || typeof value !== 'object') return false;
  if (value.request || value.response) return true;
  if (value.type || value.properties || value.components) return true;
  return Object.keys(value).length > 0;
}

export function selectSpecSchema(specData: {
  jsonSchema?: any;
  jsonSchemaOriginal?: any;
  jsonSchemaEnhanced?: any;
} | null | undefined) {
  const enhanced = parseSpecJson(specData?.jsonSchemaEnhanced);
  if (isMeaningfulSchema(enhanced)) {
    return { schema: enhanced, source: 'Enhanced Spec' };
  }

  const base = parseSpecJson(specData?.jsonSchema);
  if (isMeaningfulSchema(base)) {
    return { schema: base, source: 'Spec' };
  }

  const original = parseSpecJson(specData?.jsonSchemaOriginal);
  if (isMeaningfulSchema(original)) {
    return { schema: original, source: 'Original Spec' };
  }

  return { schema: null, source: 'Spec' };
}

export function buildPydanticHeaders(settings: Pick<Settings, 'commonHeaders' | 'mapiKey'>) {
  const headers: Record<string, string> = {
    'Content-Type': 'application/json',
    Accept: 'application/json',
  };

  try {
    Object.assign(headers, JSON.parse(settings.commonHeaders || '{}'));
  } catch {
    return {
      headers,
      error: 'Common Headers is not valid JSON.',
    };
  }

  if (settings.mapiKey) {
    headers['MAPI-Key'] = settings.mapiKey;
  }

  return { headers, error: null };
}

function resolveRef(ref: string, rootSchema: any, seen = new Set<string>()): any {
  if (!ref.startsWith('#/') || seen.has(ref)) return null;
  seen.add(ref);
  const value = ref
    .slice(2)
    .split('/')
    .map((part) => part.replace(/~1/g, '/').replace(/~0/g, '~'))
    .reduce((current, key) => current?.[key], rootSchema);
  if (value?.$ref) return resolveRef(value.$ref, rootSchema, seen);
  return value || null;
}

function mergeObjectSchemas(schemas: any[], rootSchema: any) {
  return schemas.reduce((merged, item) => {
    const schema = normalizeSchema(item, rootSchema);
    return {
      ...merged,
      ...schema,
      properties: {
        ...(merged.properties || {}),
        ...(schema.properties || {}),
      },
      required: Array.from(new Set([...(merged.required || []), ...(schema.required || [])])),
    };
  }, {});
}

function normalizeSchema(schema: any, rootSchema: any): any {
  if (!schema || typeof schema !== 'object') return schema;
  if (schema.$ref) {
    return normalizeSchema(resolveRef(schema.$ref, rootSchema) || {}, rootSchema);
  }
  if (Array.isArray(schema.allOf)) {
    return mergeObjectSchemas(schema.allOf, rootSchema);
  }
  return schema;
}

function sampleValueForSchema(schema: any, rootSchema: any): any {
  const normalized = normalizeSchema(schema, rootSchema);
  if (!normalized || typeof normalized !== 'object') return null;

  if (Object.prototype.hasOwnProperty.call(normalized, 'default')) return normalized.default;
  if (Object.prototype.hasOwnProperty.call(normalized, 'const')) return normalized.const;
  if (Array.isArray(normalized.enum) && normalized.enum.length > 0) return normalized.enum[0];

  const branch = Array.isArray(normalized.oneOf) ? normalized.oneOf[0] : Array.isArray(normalized.anyOf) ? normalized.anyOf[0] : null;
  if (branch) return sampleValueForSchema(branch, rootSchema);

  const type = Array.isArray(normalized.type) ? normalized.type.find((item: string) => item !== 'null') : normalized.type;
  if (type === 'array') return [];
  if (type === 'boolean') return false;
  if (type === 'integer' || type === 'number') return 0;
  if (type === 'string') return '';

  if (type === 'object' || normalized.properties) {
    const result: Record<string, any> = {};
    const required = Array.isArray(normalized.required) ? normalized.required : [];
    for (const key of required) {
      result[key] = sampleValueForSchema(normalized.properties?.[key] || {}, rootSchema);
    }
    return result;
  }

  return null;
}

export function buildMinimalPayload(schema: any) {
  const requestSchema = schema?.request && typeof schema.request === 'object' ? schema.request : schema;
  return sampleValueForSchema(requestSchema, requestSchema);
}

function safeSlug(value: any, fallback = 'default') {
  const slug = String(value || '')
    .trim()
    .replace(/([a-z0-9])([A-Z])/g, '$1_$2')
    .replace(/[^a-zA-Z0-9]+/g, '_')
    .replace(/^_+|_+$/g, '')
    .toLowerCase();
  return slug || fallback;
}

function toSnakeCase(value: string) {
  const name = String(value || '')
    .trim()
    .replace(/([a-z0-9])([A-Z])/g, '$1_$2')
    .replace(/[^a-zA-Z0-9]+/g, '_')
    .replace(/^_+|_+$/g, '')
    .toLowerCase();
  return /^[0-9]/.test(name) ? `field_${name}` : name || 'value';
}

function pythonLiteral(value: any): string {
  if (value === null || value === undefined) return 'None';
  if (value === true) return 'True';
  if (value === false) return 'False';
  if (typeof value === 'number') return Number.isFinite(value) ? String(value) : 'None';
  if (typeof value === 'string') return JSON.stringify(value);
  if (Array.isArray(value)) return `[${value.map((item) => pythonLiteral(item)).join(', ')}]`;
  if (typeof value === 'object') {
    const entries = Object.entries(value)
      .map(([key, item]) => `${JSON.stringify(key)}: ${pythonLiteral(item)}`)
      .join(', ');
    return `{${entries}}`;
  }
  return 'None';
}

function requestSchemaFromBundle(schema: any) {
  return schema?.request && typeof schema.request === 'object' ? schema.request : schema;
}

function schemaProperty(schema: any, key: string, rootSchema: any) {
  const normalized = normalizeSchema(schema, rootSchema);
  return normalizeSchema(normalized?.properties?.[key] || {}, rootSchema);
}

function isImplicitSdkValue(fieldSchema: any) {
  if (!fieldSchema || typeof fieldSchema !== 'object') return false;
  if (Object.prototype.hasOwnProperty.call(fieldSchema, 'default')) return true;
  if (Object.prototype.hasOwnProperty.call(fieldSchema, 'const')) return true;
  return Array.isArray(fieldSchema.enum) && fieldSchema.enum.length === 1;
}

function buildSdkKeywordArguments(schema: any) {
  const requestSchema = requestSchemaFromBundle(schema);
  const minimalPayload = buildMinimalPayload(schema) || {};
  const wrapperKey = minimalPayload.Argument && typeof minimalPayload.Argument === 'object'
    ? 'Argument'
    : minimalPayload.Assign && typeof minimalPayload.Assign === 'object'
      ? 'Assign'
      : null;

  const source = wrapperKey ? minimalPayload[wrapperKey] : minimalPayload;
  const sourceSchema = wrapperKey
    ? schemaProperty(requestSchema, wrapperKey, requestSchema)
    : requestSchema;

  if (!source || typeof source !== 'object' || Array.isArray(source)) return [];

  return Object.entries(source)
    .filter(([key]) => !isImplicitSdkValue(schemaProperty(sourceSchema, key, requestSchema)))
    .map(([key, value]) => ({
      name: toSnakeCase(key),
      value,
    }));
}

export function buildPythonUsageExample(endpoint: {
  id: string;
  method?: string;
  path?: string;
  productId?: string;
}, schema?: any) {
  const fallbackMethod = String(endpoint.method || 'POST').split(',')[0].trim().toUpperCase() || 'POST';
  const functionName = safeSlug(endpoint.id, 'endpoint');
  const keywordArguments = buildSdkKeywordArguments(schema);
  const keywordLines = keywordArguments.map((item) => `    ${item.name}=${pythonLiteral(item.value)},`);

  return `from __future__ import annotations

import os

from generated_pydantic import ${functionName}


API_URL = os.environ["PYDANTIC_API_URL"]
API_METHOD = os.environ.get("PYDANTIC_API_METHOD", ${JSON.stringify(fallbackMethod)})
HEADERS = os.environ.get("PYDANTIC_HEADERS", "{}")

result = ${functionName}(
    url=API_URL,
    method=API_METHOD,
    headers=HEADERS,
${keywordLines.length ? `${keywordLines.join('\n')}\n` : ''})

# full JSON output (status + request + response)
print(result.model_dump_json(by_alias=True, indent=2))

# # response data only
# print(result.response)

# # status code
# print(result.status, result.status_text)

# # raw response string
# print(result.raw_response)

# # Polars DataFrame (pip install polars)
# print(result.to_polars_string())
`;
}
