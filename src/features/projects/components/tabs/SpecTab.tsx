import { ResizableHandle, ResizablePanel, ResizablePanelGroup } from '@/components/ui/resizable';
import { useState, useEffect, useMemo } from 'react';
import { Save, AlertCircle, Sparkles, ArrowRight } from 'lucide-react';
import { convertWithDetails } from '@/lib/schema/schemaConverter';
import { Button } from '@/components/ui/button';
import { CodeEditor } from '@/components/common';
import { apiSpecs } from '@/data/apiSpecs';
import { useAppStore } from '@/store/useAppStore';
import { apiClient } from '@/lib/api-client';
import type { ManualData, Settings } from '@/types';
import { toast } from 'sonner';
import {
  resolveActiveSchema,
  isEnhancedSchemaActive,
  compileSchema,
} from '@/lib/schema';
import {
  compileEnhancedSchema,
  type EnhancedSchema
} from '@/lib/schema/schemaCompiler';
import { generateHTMLDocument } from '@/lib/schema/tableGenerator';
import { DynamicTableRenderer } from '@/lib/rendering/dynamicTableRenderer';
import { loadCachedDefinition, type TableDefinition, type DefinitionType } from '@/lib/rendering/definitionLoader';
import { generateHTMLTable, type TableParameter } from '@/lib/rendering/tableToHTML';
import { useEndpoints } from '@/hooks/useEndpoints';
import { getPSDForProduct } from '@/config/psdMapping';
import {
  collectFieldConditionInfo,
  groupFieldsByCondition
} from '@/lib/schema/conditionExtractor';
import { buildFieldDescription } from '@/lib/schema/descriptionBuilder';
import { schemaCompileCache } from '@/lib/cache/schemaCache';
import { SchemaDesigner } from '@/features/schema-designer/components/SchemaDesigner';

interface SpecTabProps {
  endpoint: {
    id: string;
    name: string;
    method: string;
    path: string;
  };
  settings?: Settings;
}

type SchemaView = 'original' | 'enhanced' | 'merged';
type EnhancedSubView = 'request' | 'response';

const SCHEMA_KEYWORDS = [
  'type',
  'properties',
  '$ref',
  'allOf',
  'oneOf',
  'anyOf',
  'items',
  'required',
  'additionalProperties',
];

const parseIfString = (value: any) => {
  if (!value) return value;
  if (typeof value === 'string') {
    try {
      return JSON.parse(value);
    } catch {
      return value;
    }
  }
  return value;
};

const hasSchemaKeywords = (schema: any): boolean => {
  if (!schema || typeof schema !== 'object') return false;
  return SCHEMA_KEYWORDS.some((key) => Object.prototype.hasOwnProperty.call(schema, key));
};

const isSchemaBundle = (schema: any): boolean => {
  if (!schema || typeof schema !== 'object') return false;
  const hasRequest = Object.prototype.hasOwnProperty.call(schema, 'request');
  const hasResponse = Object.prototype.hasOwnProperty.call(schema, 'response');
  if (!hasRequest && !hasResponse) return false;
  return !hasSchemaKeywords(schema);
};

const extractEnhancedBundle = (value: any) => {
  const parsed = parseIfString(value);
  if (!parsed || typeof parsed !== 'object') {
    return { request: undefined, response: undefined, requestKey: undefined, responseKey: undefined, isBundle: false };
  }
  const requestKey = typeof (parsed as any).requestKey === 'string' ? (parsed as any).requestKey : undefined;
  const responseKey = typeof (parsed as any).responseKey === 'string' ? (parsed as any).responseKey : undefined;
  if (isSchemaBundle(parsed)) {
    return {
      request: parseIfString((parsed as any).request),
      response: parseIfString((parsed as any).response),
      requestKey,
      responseKey,
      isBundle: true,
    };
  }
  return { request: parsed, response: undefined, requestKey, responseKey, isBundle: false };
};

const deepClone = <T,>(value: T): T => {
  if (value === undefined) return value;
  try {
    return JSON.parse(JSON.stringify(value));
  } catch {
    return value;
  }
};

const stableStringify = (value: any): string => {
  if (value === null || value === undefined) return String(value);
  if (typeof value !== 'object') return JSON.stringify(value);
  if (Array.isArray(value)) {
    return `[${value.map(stableStringify).join(',')}]`;
  }
  const keys = Object.keys(value).sort();
  return `{${keys.map((key) => `${JSON.stringify(key)}:${stableStringify(value[key])}`).join(',')}}`;
};

const extractComponents = (schema: any) => {
  if (!schema || typeof schema !== 'object') {
    return { schema: {}, components: {} };
  }

  const components = schema?.components?.schemas;
  const baseSchema = { ...schema };
  if (baseSchema.components) {
    delete baseSchema.components;
  }

  return {
    schema: baseSchema,
    components: components && typeof components === 'object' ? components : {},
  };
};

const rewriteRefs = (node: any, refMap: Record<string, string>): any => {
  if (!node || typeof node !== 'object') return node;
  if (Array.isArray(node)) {
    return node.map((item) => rewriteRefs(item, refMap));
  }

  const next: any = {};
  for (const [key, value] of Object.entries(node)) {
    if (key === '$ref' && typeof value === 'string' && refMap[value]) {
      next[key] = refMap[value];
      continue;
    }
    next[key] = rewriteRefs(value, refMap);
  }
  return next;
};

const toSchemaToken = (value: string): string => {
  return String(value || '')
    .replace(/[^a-zA-Z0-9]+/g, '_')
    .replace(/^_+|_+$/g, '')
    .toUpperCase();
};

const resolveRefNameFromRef = (ref: any): string | null => {
  if (typeof ref !== 'string') return null;
  const match = ref.match(/^#\/components\/schemas\/(.+)$/);
  return match ? match[1] : null;
};

const inferMapBodyComponentName = (
  schema: any,
  kind: 'request' | 'response'
): string => {
  const defaultName = kind === 'request' ? 'Request' : 'Response';
  if (!schema || typeof schema !== 'object') return defaultName;

  const origin = schema?.['x-origin-name'];
  if (typeof origin === 'string' && origin.trim().length > 0) {
    return origin.trim();
  }

  const props = schema?.properties;
  if (!props || typeof props !== 'object') return defaultName;
  const propKeys = Object.keys(props);
  if (propKeys.length !== 1) return defaultName;

  const wrapperKey = propKeys[0];
  const wrapperSchema: any = (props as any)[wrapperKey];
  if (!wrapperSchema || typeof wrapperSchema !== 'object') return defaultName;

  const refCandidates: string[] = [];
  const additionalRef = resolveRefNameFromRef(wrapperSchema?.additionalProperties?.$ref);
  if (additionalRef) refCandidates.push(additionalRef);

  const patternProps = wrapperSchema?.patternProperties;
  if (patternProps && typeof patternProps === 'object') {
    for (const candidate of Object.values(patternProps as Record<string, any>)) {
      const refName = resolveRefNameFromRef((candidate as any)?.$ref);
      if (refName) refCandidates.push(refName);
    }
  }

  const requestOrResponse = kind === 'request' ? 'REQUEST' : 'RESPONSE';

  for (const refName of refCandidates) {
    if (/_ITEM$/i.test(refName)) {
      const prefix = refName.replace(/_ITEM$/i, '');
      if (prefix) return `${prefix}_${requestOrResponse}_MAP_BODY`;
    }

    if (/_REQUEST_MAP_BODY$/i.test(refName) || /_RESPONSE_MAP_BODY$/i.test(refName)) {
      const prefix = refName.replace(/_(REQUEST|RESPONSE)_MAP_BODY$/i, '');
      if (prefix) return `${prefix}_${requestOrResponse}_MAP_BODY`;
    }
  }

  const wrapperToken = toSchemaToken(wrapperKey);
  if (wrapperToken && !['ASSIGN', 'ARGUMENT', 'MCD'].includes(wrapperToken)) {
    return `DTO_${wrapperToken}_${requestOrResponse}_MAP_BODY`;
  }

  return defaultName;
};

const isNonEmptySchemaObject = (value: any): boolean => {
  return !!(value && typeof value === 'object' && Object.keys(value).length > 0);
};

type MapEntryContext = {
  wrapperKey: string;
  mode: 'additionalProperties' | 'patternProperties';
  patternKey?: string;
  entrySchema: any;
};

const RESERVED_WRAPPER_KEYS = new Set(['ASSIGN', 'ARGUMENT', 'MCD']);

const getMapEntryContext = (schema: any): MapEntryContext | null => {
  if (!schema || typeof schema !== 'object') return null;
  const props = schema.properties;
  if (!props || typeof props !== 'object') return null;
  const keys = Object.keys(props);
  if (keys.length !== 1) return null;

  const wrapperKey = keys[0];
  const wrapper = props[wrapperKey];
  if (!wrapper || typeof wrapper !== 'object') return null;

  const additionalProps = wrapper.additionalProperties;
  if (additionalProps && typeof additionalProps === 'object' && !Array.isArray(additionalProps)) {
    return {
      wrapperKey,
      mode: 'additionalProperties',
      entrySchema: additionalProps,
    };
  }

  const patternProps = wrapper.patternProperties;
  if (patternProps && typeof patternProps === 'object') {
    const entries = Object.entries(patternProps).filter(([, value]) => value && typeof value === 'object');
    if (entries.length === 1) {
      const [patternKey, entrySchema] = entries[0];
      return {
        wrapperKey,
        mode: 'patternProperties',
        patternKey,
        entrySchema,
      };
    }
  }

  return null;
};

const applyMapEntryRef = (schema: any, ctx: MapEntryContext, refName: string) => {
  if (!schema?.properties?.[ctx.wrapperKey]) return;
  const wrapper = schema.properties[ctx.wrapperKey];
  const refNode = { $ref: `#/components/schemas/${refName}` };

  // Canonical map-body style for merged schema:
  // always represent entry schema by additionalProperties + $ref.
  wrapper.additionalProperties = refNode;
  if (wrapper.patternProperties && typeof wrapper.patternProperties === 'object') {
    delete wrapper.patternProperties;
  }
};

const parseMapBodyPrefix = (name: string): string | null => {
  if (typeof name !== 'string' || !name) return null;
  const match = name.match(/^(.*)_(REQUEST|RESPONSE)_MAP_BODY$/i);
  return match ? match[1] : null;
};

const pickMapBodyKey = (keys: string[], kind: 'request' | 'response'): string | undefined => {
  if (!Array.isArray(keys) || keys.length === 0) return undefined;
  const tokenRegex = kind === 'request' ? /_REQUEST(_|$)/i : /_RESPONSE(_|$)/i;
  const mapBodyRegex = kind === 'request' ? /_REQUEST_MAP_BODY$/i : /_RESPONSE_MAP_BODY$/i;
  const tokenCandidates = keys.filter((key) => tokenRegex.test(key));
  if (tokenCandidates.length === 0) return undefined;

  const mapBodyCandidates = tokenCandidates.filter((key) => mapBodyRegex.test(key));
  if (mapBodyCandidates.length === 1) return mapBodyCandidates[0];
  if (tokenCandidates.length === 1) return tokenCandidates[0];

  const sorted = (mapBodyCandidates.length > 1 ? mapBodyCandidates : tokenCandidates)
    .slice()
    .sort((a, b) => a.localeCompare(b));
  return sorted[0];
};

const getRequestResponseNameHints = (schema: any): { requestKey?: string; responseKey?: string } => {
  const componentSchemas = schema?.components?.schemas;
  if (!componentSchemas || typeof componentSchemas !== 'object') return {};
  const keys = Object.keys(componentSchemas);
  if (keys.length === 0) return {};

  return {
    requestKey: pickMapBodyKey(keys, 'request'),
    responseKey: pickMapBodyKey(keys, 'response'),
  };
};

const getSplitCacheSourceSchema = (endpointId: string): any => {
  if (typeof window === 'undefined') return undefined;

  try {
    const raw = localStorage.getItem(`schemaSplit:${endpointId}`);
    if (!raw) return undefined;
    const parsed = JSON.parse(raw);
    if (typeof parsed?.sourceSchema !== 'string') return undefined;
    return JSON.parse(parsed.sourceSchema);
  } catch {
    return undefined;
  }
};

const getSplitCacheNameHints = (endpointId: string): { requestKey?: string; responseKey?: string } => {
  if (typeof window === 'undefined') return {};

  try {
    const raw = localStorage.getItem(`schemaSplit:${endpointId}`);
    if (!raw) return {};

    const parsed = JSON.parse(raw);
    const fromSplitResult = {
      requestKey: typeof parsed?.splitResult?.requestKey === 'string' ? parsed.splitResult.requestKey : undefined,
      responseKey: typeof parsed?.splitResult?.responseKey === 'string' ? parsed.splitResult.responseKey : undefined,
    };
    if (fromSplitResult.requestKey || fromSplitResult.responseKey) {
      return fromSplitResult;
    }

    const sourceSchema = typeof parsed?.sourceSchema === 'string' ? JSON.parse(parsed.sourceSchema) : undefined;
    return getRequestResponseNameHints(sourceSchema);
  } catch {
    return {};
  }
};

const pickMapBodyNameByWrapper = (
  components: Record<string, any> | undefined,
  wrapperKey: string | undefined,
  kind: 'request' | 'response'
): string | undefined => {
  if (!components || typeof components !== 'object' || !wrapperKey) return undefined;
  const nameRegex = kind === 'request' ? /_REQUEST_MAP_BODY$/i : /_RESPONSE_MAP_BODY$/i;
  const candidates = Object.entries(components)
    .filter(([name, schema]) => {
      if (!nameRegex.test(name)) return false;
      const ctx = getMapEntryContext(schema);
      return ctx?.wrapperKey === wrapperKey;
    })
    .map(([name]) => name);

  if (candidates.length === 1) return candidates[0];
  return undefined;
};

const getNameHintsFromSourceComponents = (
  requestSchema: any,
  responseSchema: any,
  components: Record<string, any> | undefined
): { requestKey?: string; responseKey?: string } => {
  if (!components || typeof components !== 'object') return {};

  const requestCtx = getMapEntryContext(requestSchema);
  const responseCtx = getMapEntryContext(responseSchema);
  if (!requestCtx && !responseCtx) return {};

  let requestKey = pickMapBodyNameByWrapper(components, requestCtx?.wrapperKey, 'request');
  let responseKey = pickMapBodyNameByWrapper(components, responseCtx?.wrapperKey, 'response');

  if (!requestKey && responseKey) {
    const prefix = parseMapBodyPrefix(responseKey);
    if (prefix) requestKey = `${prefix}_REQUEST_MAP_BODY`;
  }
  if (!responseKey && requestKey) {
    const prefix = parseMapBodyPrefix(requestKey);
    if (prefix) responseKey = `${prefix}_RESPONSE_MAP_BODY`;
  }

  return { requestKey, responseKey };
};

const getOriginNameHints = (requestSchema: any, responseSchema: any): { requestKey?: string; responseKey?: string } => {
  const requestOrigin = typeof requestSchema?.['x-origin-name'] === 'string'
    ? String(requestSchema['x-origin-name']).trim()
    : '';
  const responseOrigin = typeof responseSchema?.['x-origin-name'] === 'string'
    ? String(responseSchema['x-origin-name']).trim()
    : '';

  const requestKey = /_REQUEST(_|$)/i.test(requestOrigin) ? requestOrigin : undefined;
  const responseKey = /_RESPONSE(_|$)/i.test(responseOrigin) ? responseOrigin : undefined;
  return { requestKey, responseKey };
};

const inferDtoPrefixFromPath = (path: string): string | undefined => {
  if (typeof path !== 'string' || !path.trim()) return undefined;
  const tokens = path
    .toUpperCase()
    .split(/[^A-Z0-9]+/)
    .map((token) => token.trim())
    .filter(Boolean);
  if (tokens.length === 0) return undefined;

  const ignored = new Set([
    'API', 'DB', 'GET', 'POST', 'PUT', 'PATCH', 'DELETE',
    'V', 'V1', 'V2', 'V3', 'V4'
  ]);
  const candidate = [...tokens]
    .reverse()
    .find((token) => !ignored.has(token) && /[A-Z]/.test(token) && !/^\d+$/.test(token));
  if (!candidate) return undefined;
  return `DTO_${candidate}`;
};

const getPathNameHints = (path: string): { requestKey?: string; responseKey?: string } => {
  const prefix = inferDtoPrefixFromPath(path);
  if (!prefix) return {};
  return {
    requestKey: `${prefix}_REQUEST_MAP_BODY`,
    responseKey: `${prefix}_RESPONSE_MAP_BODY`,
  };
};

const replaceWrapperNameInDescription = (value: unknown, fromKey: string, toKey: string) => {
  if (typeof value !== 'string' || !fromKey || !toKey || fromKey === toKey) {
    return value;
  }
  return value.split(fromKey).join(toKey);
};

const inferResponseOriginNameFromRequest = (
  requestSchema: any,
  options?: { requestKey?: string; responseKey?: string }
): string | undefined => {
  const explicitResponseKey = typeof options?.responseKey === 'string' && options.responseKey.trim().length > 0
    ? options.responseKey.trim()
    : undefined;
  if (explicitResponseKey) return explicitResponseKey;

  const requestOrigin = typeof requestSchema?.['x-origin-name'] === 'string'
    ? String(requestSchema['x-origin-name']).trim()
    : '';
  if (requestOrigin) {
    if (/_REQUEST(_|$)/i.test(requestOrigin)) {
      return requestOrigin.replace(/_REQUEST(_|$)/i, '_RESPONSE$1');
    }
    const requestPrefix = parseMapBodyPrefix(requestOrigin);
    if (requestPrefix) {
      return `${requestPrefix}_RESPONSE_MAP_BODY`;
    }
  }

  const requestKey = typeof options?.requestKey === 'string' && options.requestKey.trim().length > 0
    ? options.requestKey.trim()
    : inferMapBodyComponentName(requestSchema, 'request');
  const fallbackPrefix = parseMapBodyPrefix(requestKey);
  return fallbackPrefix ? `${fallbackPrefix}_RESPONSE_MAP_BODY` : undefined;
};

const inferResponseWrapperKeyFromRequest = (requestSchema: any, responseOriginName?: string): string => {
  const ctx = getMapEntryContext(requestSchema);
  if (!ctx) return 'Response';

  const wrapperToken = toSchemaToken(ctx.wrapperKey);
  if (wrapperToken && !RESERVED_WRAPPER_KEYS.has(wrapperToken)) {
    return ctx.wrapperKey;
  }

  const prefix = parseMapBodyPrefix(responseOriginName || '');
  const prefixTokens = prefix
    ? prefix
      .split('_')
      .map((token) => toSchemaToken(token))
      .filter((token) => token && token !== 'DTO')
    : [];

  const entryProperties = ctx.entrySchema?.properties && typeof ctx.entrySchema.properties === 'object'
    ? Object.keys(ctx.entrySchema.properties)
    : [];
  const entryRequired = Array.isArray(ctx.entrySchema?.required)
    ? ctx.entrySchema.required.filter((key: unknown): key is string => typeof key === 'string')
    : [];

  const pickMatchingKey = (candidates: string[]) => {
    for (const candidate of candidates) {
      const candidateToken = toSchemaToken(candidate);
      const matched = prefixTokens.some((token) =>
        candidateToken === token || candidateToken.startsWith(token) || token.startsWith(candidateToken)
      );
      if (matched) {
        return candidate;
      }
    }
    return undefined;
  };

  return (
    pickMatchingKey(entryRequired)
    || pickMatchingKey(entryProperties)
    || (entryProperties.length === 1 ? entryProperties[0] : undefined)
    || (entryRequired.length === 1 ? entryRequired[0] : undefined)
    || ctx.wrapperKey
    || 'Response'
  );
};

const buildMirroredResponseSchema = (
  requestSchema: any,
  options?: { requestKey?: string; responseKey?: string }
) => {
  const nextSchema = deepClone(requestSchema ?? {});
  if (!nextSchema || typeof nextSchema !== 'object') {
    return nextSchema;
  }

  const ctx = getMapEntryContext(nextSchema);
  const responseOriginName = inferResponseOriginNameFromRequest(nextSchema, options);
  const nextWrapperKey = inferResponseWrapperKeyFromRequest(nextSchema, responseOriginName);

  if (responseOriginName) {
    nextSchema['x-origin-name'] = responseOriginName;
  }

  if (!ctx || !ctx.wrapperKey || !nextSchema.properties?.[ctx.wrapperKey]) {
    return nextSchema;
  }

  const wrapperSchema = deepClone(nextSchema.properties[ctx.wrapperKey]);
  if (wrapperSchema && typeof wrapperSchema === 'object' && typeof wrapperSchema.description === 'string') {
    wrapperSchema.description = replaceWrapperNameInDescription(wrapperSchema.description, ctx.wrapperKey, nextWrapperKey);
  }

  if (ctx.wrapperKey !== nextWrapperKey) {
    const reorderedProperties: Record<string, any> = {};
    for (const [key, value] of Object.entries(nextSchema.properties)) {
      if (key === ctx.wrapperKey) {
        reorderedProperties[nextWrapperKey] = wrapperSchema;
      } else {
        reorderedProperties[key] = value;
      }
    }
    nextSchema.properties = reorderedProperties;
  } else {
    nextSchema.properties[ctx.wrapperKey] = wrapperSchema;
  }

  if (Array.isArray(nextSchema.required)) {
    nextSchema.required = nextSchema.required.map((key: string) => key === ctx.wrapperKey ? nextWrapperKey : key);
  } else {
    nextSchema.required = [nextWrapperKey];
  }

  return nextSchema;
};

const mergeRequestResponseSchemas = (
  requestSchema: any,
  responseSchema: any,
  options?: { requestNameHint?: string; responseNameHint?: string }
) => {
  const requestClone = deepClone(requestSchema ?? {});
  const responseClone = deepClone(responseSchema ?? {});

  const { schema: requestBody, components: requestComponents } = extractComponents(requestClone);
  const { schema: responseBody, components: responseComponents } = extractComponents(responseClone);

  const mergedComponents: Record<string, any> = { ...requestComponents };
  const usedNames = new Set(Object.keys(mergedComponents));
  const refMap: Record<string, string> = {};

  for (const [name, schema] of Object.entries(responseComponents)) {
    if (!mergedComponents[name]) {
      mergedComponents[name] = schema;
      usedNames.add(name);
      continue;
    }

    if (stableStringify(mergedComponents[name]) === stableStringify(schema)) {
      continue;
    }

    let newName = `Response_${name}`;
    let counter = 1;
    while (usedNames.has(newName)) {
      newName = `Response_${name}_${counter++}`;
    }

    mergedComponents[newName] = schema;
    usedNames.add(newName);
    refMap[`#/components/schemas/${name}`] = `#/components/schemas/${newName}`;
  }

  const getUniqueName = (base: string) => {
    if (!usedNames.has(base)) {
      usedNames.add(base);
      return base;
    }
    let counter = 1;
    let candidate = `${base}_${counter}`;
    while (usedNames.has(candidate)) {
      counter += 1;
      candidate = `${base}_${counter}`;
    }
    usedNames.add(candidate);
    return candidate;
  };

  const stripOrigin = (schema: any) => {
    if (!schema || typeof schema !== 'object') return schema;
    const cloned = deepClone(schema);
    if (cloned && typeof cloned === 'object') {
      delete (cloned as any)['x-origin-name'];
    }
    return cloned;
  };

  const resolveOriginName = (schema: any, fallback: string) => {
    const origin = schema?.['x-origin-name'];
    if (typeof origin === 'string' && origin.trim().length > 0) {
      return origin.trim();
    }
    return fallback;
  };

  const findMatchingComponentName = (schema: any) => {
    const target = stripOrigin(schema);
    const targetSig = stableStringify(target);
    for (const [name, comp] of Object.entries(mergedComponents)) {
      if (stableStringify(stripOrigin(comp)) === targetSig) {
        return name;
      }
    }
    return null;
  };

  const resolveComponentName = (schema: any, fallback: string) => {
    const origin = resolveOriginName(schema, '');
    const match = findMatchingComponentName(schema);
    return origin || match || fallback;
  };

  const pickComponentName = (schema: any, fallback: string) => {
    const desired = resolveComponentName(schema, fallback);
    if (mergedComponents[desired] && stableStringify(stripOrigin(mergedComponents[desired])) === stableStringify(stripOrigin(schema))) {
      return desired;
    }
    return getUniqueName(desired);
  };

  const normalizedResponse = Object.keys(refMap).length > 0
    ? rewriteRefs(responseBody, refMap)
    : responseBody;
  const requestNameHint = typeof options?.requestNameHint === 'string' && options.requestNameHint.trim().length > 0
    ? options.requestNameHint.trim()
    : undefined;
  const responseNameHint = typeof options?.responseNameHint === 'string' && options.responseNameHint.trim().length > 0
    ? options.responseNameHint.trim()
    : undefined;
  let requestName = pickComponentName(
    requestBody,
    requestNameHint || inferMapBodyComponentName(requestBody, 'request')
  );
  let responseName = pickComponentName(
    normalizedResponse,
    responseNameHint || inferMapBodyComponentName(normalizedResponse, 'response')
  );

  const responsePrefix = parseMapBodyPrefix(responseName);
  const requestPrefix = parseMapBodyPrefix(requestName);
  if (requestName === 'Request' && responsePrefix) {
    requestName = pickComponentName(requestBody, `${responsePrefix}_REQUEST_MAP_BODY`);
  } else if (responseName === 'Response' && requestPrefix) {
    responseName = pickComponentName(normalizedResponse, `${requestPrefix}_RESPONSE_MAP_BODY`);
  }

  if (requestBody && typeof requestBody === 'object') {
    delete (requestBody as any)['x-origin-name'];
  }
  if (normalizedResponse && typeof normalizedResponse === 'object') {
    delete (normalizedResponse as any)['x-origin-name'];
  }

  const nextSchemas: Record<string, any> = { ...mergedComponents };

  const reserveComponentName = (preferred: string, schema: any) => {
    if (!preferred || typeof preferred !== 'string') {
      return getUniqueName('DTO_COMMON_ITEM');
    }
    if (!nextSchemas[preferred]) {
      if (!usedNames.has(preferred)) usedNames.add(preferred);
      return preferred;
    }
    if (stableStringify(stripOrigin(nextSchemas[preferred])) === stableStringify(stripOrigin(schema))) {
      return preferred;
    }
    return getUniqueName(preferred);
  };

  const requestMap = getMapEntryContext(requestBody);
  const responseMap = getMapEntryContext(normalizedResponse);
  const requestEntryIsRef = !!requestMap?.entrySchema?.$ref;
  const responseEntryIsRef = !!responseMap?.entrySchema?.$ref;

  if (requestMap && responseMap && !requestEntryIsRef && !responseEntryIsRef) {
    const prefixFromHint = parseMapBodyPrefix(responseNameHint || '') || parseMapBodyPrefix(requestNameHint || '');
    const prefixFromName = prefixFromHint || parseMapBodyPrefix(responseName) || parseMapBodyPrefix(requestName);
    const wrapperCandidate = [responseMap.wrapperKey, requestMap.wrapperKey]
      .map((key) => toSchemaToken(key))
      .find((key) => key && !RESERVED_WRAPPER_KEYS.has(key));
    const itemPrefix = prefixFromName
      || (wrapperCandidate ? `DTO_${wrapperCandidate}` : 'DTO_COMMON');

    const requestEntrySig = stableStringify(stripOrigin(requestMap.entrySchema));
    const responseEntrySig = stableStringify(stripOrigin(responseMap.entrySchema));

    if (requestEntrySig === responseEntrySig) {
      const sharedItemName = getUniqueName(`${itemPrefix}_ITEM`);
      nextSchemas[sharedItemName] = deepClone(requestMap.entrySchema);
      applyMapEntryRef(requestBody, requestMap, sharedItemName);
      applyMapEntryRef(normalizedResponse, responseMap, sharedItemName);
    } else {
      const requestItemName = getUniqueName(`${itemPrefix}_REQUEST_ITEM`);
      const responseItemName = getUniqueName(`${itemPrefix}_RESPONSE_ITEM`);
      nextSchemas[requestItemName] = deepClone(requestMap.entrySchema);
      nextSchemas[responseItemName] = deepClone(responseMap.entrySchema);
      applyMapEntryRef(requestBody, requestMap, requestItemName);
      applyMapEntryRef(normalizedResponse, responseMap, responseItemName);
    }
  }

  if (isNonEmptySchemaObject(requestBody)) {
    if (!nextSchemas[requestName] || stableStringify(stripOrigin(nextSchemas[requestName])) !== stableStringify(stripOrigin(requestBody))) {
      nextSchemas[requestName] = requestBody;
    }
  }
  if (isNonEmptySchemaObject(normalizedResponse)) {
    if (!nextSchemas[responseName] || stableStringify(stripOrigin(nextSchemas[responseName])) !== stableStringify(stripOrigin(normalizedResponse))) {
      nextSchemas[responseName] = normalizedResponse;
    }
  }

  const normalizeMapBodyRefs = (): string[] => {
    const requestMapBody = nextSchemas[requestName];
    const responseMapBody = nextSchemas[responseName];
    const reqCtx = getMapEntryContext(requestMapBody);
    const resCtx = getMapEntryContext(responseMapBody);
    if (!reqCtx || !resCtx) return [];

    const reqRefName = resolveRefNameFromRef(reqCtx.entrySchema?.$ref);
    const resRefName = resolveRefNameFromRef(resCtx.entrySchema?.$ref);
    const reqEntrySource = reqRefName && nextSchemas[reqRefName] ? nextSchemas[reqRefName] : reqCtx.entrySchema;
    const resEntrySource = resRefName && nextSchemas[resRefName] ? nextSchemas[resRefName] : resCtx.entrySchema;
    if (!reqEntrySource || !resEntrySource || typeof reqEntrySource !== 'object' || typeof resEntrySource !== 'object') {
      return [];
    }

    const reqEntry = deepClone(reqEntrySource);
    const resEntry = deepClone(resEntrySource);
    const reqSig = stableStringify(stripOrigin(reqEntry));
    const resSig = stableStringify(stripOrigin(resEntry));

    const prefixFromHint = parseMapBodyPrefix(responseNameHint || '') || parseMapBodyPrefix(requestNameHint || '');
    const prefixFromName = prefixFromHint || parseMapBodyPrefix(responseName) || parseMapBodyPrefix(requestName);
    const refPrefixCandidate = (reqRefName || resRefName || '')
      .replace(/_(REQUEST_ITEM|RESPONSE_ITEM|ITEM)$/i, '')
      .trim();
    const wrapperCandidate = [resCtx.wrapperKey, reqCtx.wrapperKey]
      .map((key) => toSchemaToken(key))
      .find((key) => key && !RESERVED_WRAPPER_KEYS.has(key));
    const itemPrefix = prefixFromName
      || refPrefixCandidate
      || (wrapperCandidate ? `DTO_${wrapperCandidate}` : 'DTO_COMMON');

    const promotedNames: string[] = [];
    if (reqSig === resSig) {
      const preferredShared = (reqRefName && /_ITEM$/i.test(reqRefName))
        ? reqRefName
        : (resRefName && /_ITEM$/i.test(resRefName))
          ? resRefName
          : `${itemPrefix}_ITEM`;
      const sharedName = reserveComponentName(preferredShared, reqEntry);
      nextSchemas[sharedName] = reqEntry;
      applyMapEntryRef(requestMapBody, reqCtx, sharedName);
      applyMapEntryRef(responseMapBody, resCtx, sharedName);
      promotedNames.push(sharedName);
    } else {
      const preferredReq = (reqRefName && /_ITEM$/i.test(reqRefName)) ? reqRefName : `${itemPrefix}_REQUEST_ITEM`;
      const preferredRes = (resRefName && /_ITEM$/i.test(resRefName)) ? resRefName : `${itemPrefix}_RESPONSE_ITEM`;
      const reqName = reserveComponentName(preferredReq, reqEntry);
      const resName = reserveComponentName(preferredRes, resEntry);
      nextSchemas[reqName] = reqEntry;
      nextSchemas[resName] = resEntry;
      applyMapEntryRef(requestMapBody, reqCtx, reqName);
      applyMapEntryRef(responseMapBody, resCtx, resName);
      promotedNames.push(reqName, resName);
    }

    nextSchemas[requestName] = requestMapBody;
    nextSchemas[responseName] = responseMapBody;
    return promotedNames;
  };

  const promotedMapItemNames = normalizeMapBodyRefs();
  const orderedSchemas: Record<string, any> = {};
  for (const key of [requestName, responseName, ...promotedMapItemNames]) {
    if (key && nextSchemas[key] && !orderedSchemas[key]) {
      orderedSchemas[key] = nextSchemas[key];
    }
  }
  for (const [name, schema] of Object.entries(nextSchemas)) {
    if (!orderedSchemas[name]) {
      orderedSchemas[name] = schema;
    }
  }

  return {
    components: {
      schemas: orderedSchemas,
    },
  };
};

export function SpecTab({ endpoint, settings }: SpecTabProps) {
  const {
    setManualData,
    manualData,
    currentVersionId,
    // getCurrentVersion,
    updateSpecData,
    // fetchVersions,
    // loadVersion,
    specData,
    saveCurrentVersion,
  } = useAppStore();

  // 🔥 제품 ID로 PSD 설정 가져오기 (로컬 매핑)
  const { endpoints: products } = useEndpoints();
  const currentProduct = products.find(p => p.id === (endpoint as any).product);
  const productId = (endpoint as any).product || currentProduct?.id;

  // PSD 매핑 (로컬 관리)
  const { psdSet, schemaType: defaultSchemaType } = useMemo(() => {
    return getPSDForProduct(productId);
  }, [productId]);
  const schemaType = defaultSchemaType as 'original' | 'enhanced';

  // 🔥 YAML Definition 로드
  const [tableDefinition, setTableDefinition] = useState<TableDefinition | null>(null);
  const [isLoadingDefinition, setIsLoadingDefinition] = useState(true);
  const [initializedSchemaTypes, setInitializedSchemaTypes] = useState<Set<string>>(new Set());

  // 🔥 Schema Registry로 활성 스키마 결정 (우선순위 정책 분리)
  // 🔥 fallbackSpec을 useMemo로 감싸서 매 렌더링마다 새 객체 생성 방지
  const fallbackSpec = useMemo(() => {
    return apiSpecs[endpoint.id] || {
      title: endpoint.name,
      description: 'API endpoint specification',
      parameters: [],
      jsonSchema: {},
    };
  }, [endpoint.id, endpoint.name]);

  // 🎯 specData 상태 확인
  const hasSpecData = !!(specData?.jsonSchema);
  const isUsingFallback = !hasSpecData;

  // specData와 fallback을 결합 (specData 변경 시 재계산)
  const combinedSpecData = useMemo(() => {
    const result = {
      jsonSchema: parseIfString(specData?.jsonSchema) || parseIfString(specData?.jsonSchemaOriginal) || fallbackSpec.jsonSchema,
      jsonSchemaOriginal: parseIfString(specData?.jsonSchemaOriginal) || parseIfString(specData?.jsonSchema) || fallbackSpec.jsonSchema,
      jsonSchemaEnhanced: parseIfString(specData?.jsonSchemaEnhanced) || fallbackSpec.jsonSchemaEnhanced,
      savedSchema: (specData as any)?.savedSchema,
    };

    return result;
  }, [specData, fallbackSpec]);

  const enhancedBundle = useMemo(() => {
    return extractEnhancedBundle(combinedSpecData.jsonSchemaEnhanced);
  }, [combinedSpecData.jsonSchemaEnhanced]);

  const enhancedRequestSchema = useMemo(() => {
    return enhancedBundle.request
      || combinedSpecData.jsonSchemaOriginal
      || combinedSpecData.jsonSchema
      || {};
  }, [enhancedBundle.request, combinedSpecData.jsonSchemaOriginal, combinedSpecData.jsonSchema]);

  const enhancedResponseSchema = useMemo(() => {
    return enhancedBundle.response || {};
  }, [enhancedBundle.response]);

  const sourceSchemaForApi = useMemo(() => {
    const candidates = [combinedSpecData.jsonSchemaOriginal, combinedSpecData.jsonSchema];
    return candidates.find((candidate) => candidate?.components?.schemas && typeof candidate.components.schemas === 'object');
  }, [combinedSpecData.jsonSchemaOriginal, combinedSpecData.jsonSchema]);

  const mergeNameHints = useMemo(() => {
    let requestKey = enhancedBundle.requestKey;
    let responseKey = enhancedBundle.responseKey;

    const applyCandidate = (candidate: { requestKey?: string; responseKey?: string }) => {
      if (!requestKey && candidate.requestKey) requestKey = candidate.requestKey;
      if (!responseKey && candidate.responseKey) responseKey = candidate.responseKey;
    };

    applyCandidate(getOriginNameHints(enhancedRequestSchema, enhancedResponseSchema));
    applyCandidate(getRequestResponseNameHints(combinedSpecData.jsonSchemaOriginal));
    applyCandidate(getRequestResponseNameHints(combinedSpecData.jsonSchema));
    applyCandidate(getSplitCacheNameHints(endpoint.id));
    applyCandidate(
      getNameHintsFromSourceComponents(
        enhancedRequestSchema,
        enhancedResponseSchema,
        combinedSpecData?.jsonSchemaOriginal?.components?.schemas
      )
    );
    applyCandidate(
      getNameHintsFromSourceComponents(
        enhancedRequestSchema,
        enhancedResponseSchema,
        combinedSpecData?.jsonSchema?.components?.schemas
      )
    );

    const splitCacheSourceSchema = getSplitCacheSourceSchema(endpoint.id);
    applyCandidate(
      getNameHintsFromSourceComponents(
        enhancedRequestSchema,
        enhancedResponseSchema,
        splitCacheSourceSchema?.components?.schemas
      )
    );
    applyCandidate(getPathNameHints(endpoint.path));

    return { requestKey, responseKey };
  }, [
    enhancedBundle.requestKey,
    enhancedBundle.responseKey,
    enhancedRequestSchema,
    enhancedResponseSchema,
    combinedSpecData.jsonSchemaOriginal,
    combinedSpecData.jsonSchema,
    endpoint.id,
    endpoint.path,
  ]);

  const mergedSchema = useMemo(() => {
    const merged = mergeRequestResponseSchemas(enhancedRequestSchema, enhancedResponseSchema, {
      requestNameHint: mergeNameHints.requestKey,
      responseNameHint: mergeNameHints.responseKey,
    });
    const mergedComponents = merged?.components?.schemas;
    const mergedKeys = mergedComponents && typeof mergedComponents === 'object'
      ? Object.keys(mergedComponents)
      : [];
    const isRequestResponseOnly = mergedKeys.length > 0
      && mergedKeys.every((key) => key === 'Request' || key === 'Response');

    const originalComponents = combinedSpecData?.jsonSchemaOriginal?.components?.schemas;
    if (isRequestResponseOnly && originalComponents && typeof originalComponents === 'object') {
      return {
        components: {
          schemas: deepClone(originalComponents),
        },
      };
    }

    return merged;
  }, [
    enhancedRequestSchema,
    enhancedResponseSchema,
    mergeNameHints.requestKey,
    mergeNameHints.responseKey,
    combinedSpecData.jsonSchemaOriginal
  ]);

  const computeMergedSnapshot = () => {
    const merged = mergeRequestResponseSchemas(enhancedRequestSchema, enhancedResponseSchema, {
      requestNameHint: mergeNameHints.requestKey,
      responseNameHint: mergeNameHints.responseKey,
    });
    const mergedComponents = merged?.components?.schemas;
    const mergedKeys = mergedComponents && typeof mergedComponents === 'object'
      ? Object.keys(mergedComponents)
      : [];
    const isRequestResponseOnly = mergedKeys.length > 0
      && mergedKeys.every((key) => key === 'Request' || key === 'Response');

    if (!isRequestResponseOnly) return merged;

    const originalComponents = combinedSpecData?.jsonSchemaOriginal?.components?.schemas
      || combinedSpecData?.jsonSchema?.components?.schemas;
    if (originalComponents && typeof originalComponents === 'object' && Object.keys(originalComponents).length > 0) {
      return {
        components: {
          schemas: deepClone(originalComponents),
        },
      };
    }

    try {
      const cached = localStorage.getItem(`schemaSplit:${endpoint.id}`);
      if (cached) {
        const parsed = JSON.parse(cached);
        const sourceSchema = typeof parsed?.sourceSchema === 'string' ? JSON.parse(parsed.sourceSchema) : null;
        const cachedComponents = sourceSchema?.components?.schemas;
        if (cachedComponents && typeof cachedComponents === 'object') {
          return {
            components: {
              schemas: deepClone(cachedComponents),
            },
          };
        }
      }
    } catch {
      // ignore cache errors
    }

    return merged;
  };


  // 🎯 활성 스키마 (우선순위: savedSchema > enhanced > original)
  const activeSchema = resolveActiveSchema(combinedSpecData);
  const hasEnhancedSchema = isEnhancedSchemaActive(combinedSpecData);

  // 🔥 NEW Enhanced Schema 감지: x-ui, x-transport, x-enum-by-type 등의 필드가 있는지 확인
  // ⚡ 최적화: JSON.stringify 대신 객체 직접 탐색 (10-100배 빠름)
  const isNewEnhancedSchema = useMemo(() => {
    if (!activeSchema || typeof activeSchema !== 'object') return false;

    // 재귀적으로 x-* 필드 검색
    const hasEnhancedFields = (obj: any, depth: number = 0): boolean => {
      if (depth > 10 || !obj || typeof obj !== 'object') return false;

      for (const key in obj) {
        if (key === 'x-ui' || key === 'x-transport' ||
          key === 'x-enum-by-type' || key === 'x-node-count-by-type') {
          return true;
        }
        if (typeof obj[key] === 'object' && hasEnhancedFields(obj[key], depth + 1)) {
          return true;
        }
      }
      return false;
    };

    return hasEnhancedFields(activeSchema);
  }, [activeSchema]);

  // 🔥 Schema Definition 결정 (Settings 우선, 없으면 자동 감지)
  const effectiveDefinitionType: DefinitionType = useMemo(() => {
    if (settings?.schemaDefinition === 'original') return 'original';
    if (settings?.schemaDefinition === 'enhanced') return 'enhanced';
    // Auto: 자동 감지
    return isNewEnhancedSchema ? 'enhanced' : 'original';
  }, [settings?.schemaDefinition, isNewEnhancedSchema]);

  // 🎯 Schema View Toggle: 'original' | 'enhanced' (⚠️ table view보다 먼저 선언)
  // 🔥 일반 모드에서는 schemaView 고정 (탭 숨김)
  const [schemaView, setSchemaView] = useState<SchemaView>(() => {
    if (settings?.schemaMode === 'normal') {
      return 'original'; // 일반 모드는 항상 original
    }
    return hasEnhancedSchema ? 'enhanced' : 'original';
  });
  const [enhancedSubView, setEnhancedSubView] = useState<EnhancedSubView>('request');
  const [mergedSnapshot, setMergedSnapshot] = useState<any | null>(null);
  const [isSchemaApiBusy, setIsSchemaApiBusy] = useState(false);
  const roundtripStorageKey = useMemo(() => `schemaRoundtrip:${endpoint.id}`, [endpoint.id]);

  const [tableView, setTableView] = useState<'request' | 'response' | 'merged'>('request');


  // 🎨 Designer Mode Toggle: 'code' | 'visual'
  const [designerMode, _setDesignerMode] = useState<'code' | 'visual'>('code');

  // 🔥 endpoint 변경 시 schemaView 재설정 (Enhanced 우선)
  useEffect(() => {
    setEnhancedSubView('request');

    // 일반 모드에서는 항상 original
    if (settings?.schemaMode === 'normal') {
      setSchemaView('original');
      return;
    }

    // 개선 모드에서만 자동 전환
    // 1. Enhanced 스키마가 명시적으로 있으면 Enhanced 우선
    if (hasEnhancedSchema) {
      setSchemaView('enhanced');
    }
    // 2. Original 스키마에 x-ui 등이 있으면 (New Enhanced Schema) Enhanced로 전환
    else if (isNewEnhancedSchema) {
      console.log('🔄 Auto-switching to Enhanced view (x-ui detected in schema)');
      setSchemaView('enhanced');
    }
    // 3. 순수 Original 스키마만 있으면 Original
    else {
      setSchemaView('original');
    }
  }, [endpoint.id, hasEnhancedSchema, isNewEnhancedSchema, settings?.schemaMode]);

  useEffect(() => {
    if (schemaView === 'original') {
      setTableView('request');
      return;
    }

    if (schemaView === 'merged') {
      setTableView('merged');
    }
  }, [schemaView, endpoint.id]);

  // 🔥 YAML Definition 로드 (schemaView 변경 시)
  useEffect(() => {
    setIsLoadingDefinition(true);

    // 🔥 일반 모드에서는 definition 하나만 로드
    // 🔥 개선 모드에서는 original 또는 enhanced 로드
    const currentSchemaType = settings?.schemaMode === 'normal'
      ? 'definition'  // 일반 모드
      : schemaView === 'original' ? 'original' : schemaType; // 개선 모드

    const key = `${psdSet}/${currentSchemaType}`;

    console.log(`🔄 Loading YAML definition: ${key} (mode: ${settings?.schemaMode || 'enhanced'})`);

    // 🔥 Schema Logic Rules 초기화 (먼저!)
    import('@/lib/schema/schemaLogicEngine').then(({ initSchemaLogicRules }) => {
      return initSchemaLogicRules(psdSet, currentSchemaType);
    }).then(() => {
      console.log(`✅ Initialized schema logic rules for ${key}`);

      // 🔥 초기화 완료 추적
      setInitializedSchemaTypes(prev => new Set(prev).add(key));

      // 🔥 제품의 PSD 설정 사용
      return loadCachedDefinition(
        effectiveDefinitionType,
        'table',
        undefined, // schemaSet (deprecated)
        psdSet, // psdSet (Level 1)
        currentSchemaType // schemaType (Level 2) - 현재 뷰 기준
      );
    })
      .then((def) => {
        console.log(`✅ Loaded table definition from ${key}`);
        console.log(`🔍 tableDefinition object:`, def);
        console.log(`🔍 tableDefinition.schemaExtensions:`, (def as any)?.schemaExtensions);
        setTableDefinition(def as TableDefinition);
        setIsLoadingDefinition(false);
      })
      .catch((error) => {
        console.error('Failed to load table definition:', error);
        setIsLoadingDefinition(false);
      });
  }, [effectiveDefinitionType, psdSet, schemaType, schemaView, settings?.schemaMode]);

  // 🔥 NEW: Schema Compiler로 정규화된 AST 생성
  const canonicalFields = useMemo(() => {
    // 🔥 현재 schemaView에 맞는 YAML 규칙이 초기화되었는지 확인
    const currentSchemaType = schemaView === 'original' ? 'original' : schemaType;
    const key = `${psdSet}/${currentSchemaType}`;

    if (!initializedSchemaTypes.has(key)) {
      console.log(`⏳ Waiting for ${key} to be initialized...`);
      return [];
    }

    if (isNewEnhancedSchema) {
      // New Enhanced Schema: 무시하고 빈 배열 반환 (새 컴파일러 사용)
      return [];
    }
    return compileSchema(activeSchema, psdSet, schemaType);
  }, [activeSchema, isNewEnhancedSchema, initializedSchemaTypes, psdSet, schemaType, schemaView]);

  // 🎯 Helper: Convert required status to display string
  const formatRequiredStatus = (requiredStatus: Record<string, string> | undefined): string => {
    if (!requiredStatus || !requiredStatus['*']) {
      return 'Optional';
    }

    const status = requiredStatus['*'];
    if (status === 'required') return 'Required';
    if (status === 'conditional') return 'Conditional';
    if (status === 'optional') return 'Optional';
    if (status === 'n/a') return 'N/A';

    return 'Optional'; // Default fallback
  };

  const formatDefaultValue = (value: any): string => {
    if (value === undefined || value === null) return '-';

    if (Array.isArray(value)) {
      return `[${value.map((item) => formatDefaultValue(item)).join(', ')}]`;
    }

    if (typeof value === 'string') return `"${value}"`;
    if (typeof value === 'number' || typeof value === 'boolean') return String(value);
    if (typeof value === 'object') return JSON.stringify(value);

    return String(value);
  };

  const isConditionalField = (field: any): boolean => {
    return Boolean(field?.['x-required-when'] || field?.['x-optional-when']);
  };

  const getRequiredLabel = (field: any, _inheritedConditional: boolean = false): string => {
    if (field?.['x-required-when']) {
      return 'Conditional';
    }

    if (field?.['x-optional-when']) {
      return 'Optional';
    }

    if (field?._requiredByParent) {
      return 'Required';
    }

    return formatRequiredStatus(field?.required);
  };

  const getWrapperKey = (schema: any) => {
    if (!schema || typeof schema !== 'object') return null;
    const props = schema?.properties;
    if (!props || typeof props !== 'object') return null;
    if (props.Assign) return 'Assign';
    if (props.Argument) return 'Argument';
    if (props.MCD) return 'MCD';
    const keys = Object.keys(props);
    if (keys.length === 1) {
      const loneKey = keys[0];
      const loneProp = props[loneKey];
      if (loneProp && typeof loneProp === 'object' && (loneProp.additionalProperties || loneProp.patternProperties)) {
        return loneKey;
      }
    }
    return null;
  };

  const resolveRefName = (ref: string) => {
    const match = ref.match(/^#\/components\/schemas\/(.+)$/);
    return match ? match[1] : null;
  };

  const inlineSchemaRefsForTable = (schema: any) => {
    if (!schema || typeof schema !== 'object') return schema;
    const cloned = deepClone(schema);
    const components = cloned?.components?.schemas && typeof cloned.components.schemas === 'object'
      ? cloned.components.schemas
      : {};

    const dereference = (node: any, refStack = new Set<string>()): any => {
      if (!node || typeof node !== 'object') return node;
      if (Array.isArray(node)) return node.map((item) => dereference(item, refStack));

      if (typeof node.$ref === 'string') {
        const ref = node.$ref;
        const refName = resolveRefName(ref);
        if (!refName) return node;
        if (refStack.has(ref)) return node;
        const target = components?.[refName];
        if (!target || typeof target !== 'object') return node;
        const nextStack = new Set(refStack);
        nextStack.add(ref);
        const resolved = dereference(target, nextStack);
        const { $ref, ...rest } = node;
        return dereference({ ...resolved, ...rest }, nextStack);
      }

      const result: Record<string, any> = {};
      for (const [key, value] of Object.entries(node)) {
        result[key] = dereference(value, refStack);
      }
      return result;
    };

    const inlined = dereference(cloned, new Set());
    if (inlined && typeof inlined === 'object' && 'components' in inlined) {
      delete (inlined as any).components;
    }
    return inlined;
  };

  const unwrapSchemaForTable = (schema: any) => {
    if (!schema || typeof schema !== 'object') return schema;
    const wrapperKey = getWrapperKey(schema);
    if (!wrapperKey) return schema;
    const wrapperInfo = schema?.properties?.[wrapperKey];
    if (!wrapperInfo || typeof wrapperInfo !== 'object') return schema;

    const patternProps = wrapperInfo.patternProperties;
    if (patternProps && typeof patternProps === 'object') {
      const first = Object.values(patternProps)[0];
      if (first && typeof first === 'object') return first;
    }

    const additionalProps = wrapperInfo.additionalProperties;
    if (additionalProps && typeof additionalProps === 'object') {
      return additionalProps;
    }

    return schema;
  };

  // 🔥 NEW: UI Schema Adapter로 테이블 스키마 생성
  const buildTableParametersForSchema = (schemaToUse: any, isEnhancedStructure: boolean, currentSchemaType: string) => {
    const key = `${psdSet}/${currentSchemaType}`;

    // 🔥 YAML 규칙이 초기화되었는지 확인
    if (!initializedSchemaTypes.has(key)) {
      return [];
    }

    // 🔥 스키마가 비어있거나 유효하지 않으면 빈 배열 반환
    if (!schemaToUse || typeof schemaToUse !== 'object' || Object.keys(schemaToUse).length === 0) {
      return [];
    }

    const effectiveSchema = unwrapSchemaForTable(inlineSchemaRefsForTable(schemaToUse));

    if (isEnhancedStructure) {
      // New Enhanced Schema: 새 컴파일러로 섹션 생성
      try {
        const sections = compileEnhancedSchema(effectiveSchema as EnhancedSchema, psdSet, currentSchemaType);

        // Convert sections to table parameters format
        const params: any[] = [];
        let rowNumber = 1;

        for (const section of sections) {
          // 🔥 조건 추출 및 그룹핑 (모듈화된 함수 사용)
          const fieldInfoMap = collectFieldConditionInfo(
            section.fields,
            tableDefinition?.schemaExtensions?.conditional || []
          );

          const { fieldGroups, noConditionFields } = groupFieldsByCondition(
            section.fields,
            fieldInfoMap
          );

          // 🔥 조건 없는 필드들이 있으면 일반 섹션 헤더 추가
          if (noConditionFields.length > 0) {
            params.push({
              no: '',
              section: section.name,
              name: '',
              type: '',
              default: '',
              required: '',
              description: '',
            });
          }

          // 🔥 조건 없는 필드들 렌더링
          for (const { field } of noConditionFields) {
            const param: any = {
              no: rowNumber++,
              name: field.key,
              type: field.type === 'array' ? `Array[${field.items?.type || 'any'}]` : field.type,
              default: formatDefaultValue(field.default),
              description: field.ui?.label || field.description || field.key,
              required: getRequiredLabel(field),
            };

            // 중첩 필드 처리 - 조건별 그룹화 지원
            if (field.children && field.children.length > 0) {
              const parentConditional = isConditionalField(field);
              // 🔥 3-depth 필드들을 조건별로 그룹화
              const childSectionHeaders = field.children.filter((c: any) => c.type === 'section-header');
              const childrenToProcess = field.children.filter((c: any) => c.type !== 'section-header');
              
              const childFieldInfoMap = collectFieldConditionInfo(
                childrenToProcess,
                tableDefinition?.schemaExtensions?.conditional || []
              );

              const { fieldGroups: childGroups, noConditionFields: childrenWithoutCondition } = groupFieldsByCondition(
                childrenToProcess,
                childFieldInfoMap
              );

              param.children = childSectionHeaders.map((header: any) => ({
                no: '',
                name: '',
                type: 'section-header',
                section: header.section || header.ui?.label || header.description || header.key,
                default: '',
                description: '',
                required: '',
              }));
              let childNo = 1;

              const buildArrayItemChildren = (arrayField: any) => {
                const items = arrayField?.items;
                if (!items || items.type !== 'object' || !items.properties) {
                  return [];
                }

                const itemRequired = items.required || [];
                return Object.entries(items.properties).map(([itemKey, itemProp]) => {
                  const mappedItem: any = {
                    key: `${arrayField.key}[].${itemKey}`,
                    type: (itemProp as any).type,
                    default: (itemProp as any).default,
                    description: (itemProp as any).description,
                    required: itemRequired.includes(itemKey) ? { '*': 'required' } : { '*': 'optional' },
                  };

                  if ((itemProp as any).items) mappedItem.items = (itemProp as any).items;
                  if ((itemProp as any).enum) mappedItem.enum = (itemProp as any).enum;
                  if ((itemProp as any)['x-ui']) mappedItem.ui = (itemProp as any)['x-ui'];
                  if ((itemProp as any)['x-optional-when']) mappedItem['x-optional-when'] = (itemProp as any)['x-optional-when'];
                  if ((itemProp as any)['x-required-when']) mappedItem['x-required-when'] = (itemProp as any)['x-required-when'];

                  return mappedItem;
                });
              };

              const mapGrandchildren = (grandchildren: any[], parentNo: string, parentField?: any, inheritedConditional: boolean = false) => {
                const mapGreatGrandchildren = (greatGrandchildren: any[], grandParentNo: string, grandParentField?: any, inheritedConditionalForGreat: boolean = false) => {
                  const resolvedGreatGrandchildren = greatGrandchildren.length > 0
                    ? greatGrandchildren
                    : buildArrayItemChildren(grandParentField);
                  const greatGrandchildrenToProcess = resolvedGreatGrandchildren.filter((c: any) => c.type !== 'section-header');
                  const greatGrandchildFieldInfoMap = collectFieldConditionInfo(
                    greatGrandchildrenToProcess,
                    tableDefinition?.schemaExtensions?.conditional || []
                  );

                  const { fieldGroups: greatGrandchildGroups, noConditionFields: greatGrandchildrenWithoutCondition } = groupFieldsByCondition(
                    greatGrandchildrenToProcess,
                    greatGrandchildFieldInfoMap
                  );

                  const mappedGreatGrandchildren: any[] = [];
                  let greatGrandchildNo = 1;

                  for (const { field: greatGrandchild } of greatGrandchildrenWithoutCondition) {
                    mappedGreatGrandchildren.push({
                      no: `${grandParentNo}.${greatGrandchildNo++}`,
                      name: greatGrandchild.key.split('.').pop() || greatGrandchild.key,
                      type: greatGrandchild.type === 'array' ? `Array[${greatGrandchild.items?.type || 'any'}]` : greatGrandchild.type,
                      default: formatDefaultValue(greatGrandchild.default),
                      description: buildFieldDescription(greatGrandchild, tableDefinition),
                      required: getRequiredLabel(greatGrandchild, inheritedConditionalForGreat),
                    });
                  }

                  for (const [conditionKey, greatGrandchildrenWithCondition] of greatGrandchildGroups) {
                    const { conditionInfo } = greatGrandchildrenWithCondition[0];
                    const isRequired = conditionInfo?.type === 'x-required-when';
                    const parts = conditionKey.split(':');
                    const conditionName = parts[0];
                    const conditionValue = parts.slice(1).join(':');
                    const conditionText = isRequired
                      ? `Required (When "${conditionName}" is ${conditionValue})`
                      : `Optional (When "${conditionName}" is ${conditionValue})`;

                    mappedGreatGrandchildren.push({
                      no: '', name: '', type: 'section-header',
                      section: conditionText,
                      default: '', description: '', required: '',
                    });

                    for (const { field: greatGrandchild } of greatGrandchildrenWithCondition) {
                      mappedGreatGrandchildren.push({
                        no: `${grandParentNo}.${greatGrandchildNo++}`,
                        name: greatGrandchild.key.split('.').pop() || greatGrandchild.key,
                        type: greatGrandchild.type === 'array' ? `Array[${greatGrandchild.items?.type || 'any'}]` : greatGrandchild.type,
                        default: formatDefaultValue(greatGrandchild.default),
                        description: buildFieldDescription(greatGrandchild, tableDefinition),
                        required: getRequiredLabel(greatGrandchild, inheritedConditionalForGreat),
                      });
                    }
                  }

                  return mappedGreatGrandchildren;
                };

                const resolvedGrandchildren = grandchildren.length > 0
                  ? grandchildren
                  : buildArrayItemChildren(parentField);
                const grandchildSectionHeaders = resolvedGrandchildren.filter((c: any) => c.type === 'section-header');
                const grandchildrenToProcess = resolvedGrandchildren.filter((c: any) => c.type !== 'section-header');
                const grandchildFieldInfoMap = collectFieldConditionInfo(
                  grandchildrenToProcess,
                  tableDefinition?.schemaExtensions?.conditional || []
                );

                const { fieldGroups: grandchildGroups, noConditionFields: grandchildrenWithoutCondition } = groupFieldsByCondition(
                  grandchildrenToProcess,
                  grandchildFieldInfoMap
                );

                const mappedGrandchildren: any[] = grandchildSectionHeaders.map((header: any) => ({
                  no: '',
                  name: '',
                  type: 'section-header',
                  section: header.section || header.ui?.label || header.description || header.key,
                  default: '',
                  description: '',
                  required: '',
                }));
                let grandchildNo = 1;

                for (const { field: grandchild } of grandchildrenWithoutCondition) {
                  const mappedGrandchild: any = {
                    no: `${parentNo}.${grandchildNo++}`,
                    name: grandchild.key.split('.').pop() || grandchild.key,
                    type: grandchild.type === 'array' ? `Array[${grandchild.items?.type || 'any'}]` : grandchild.type,
                    default: formatDefaultValue(grandchild.default),
                    description: buildFieldDescription(grandchild, tableDefinition),
                    required: getRequiredLabel(grandchild, inheritedConditional),
                  };

                  if (grandchild.children && grandchild.children.length > 0) {
                    mappedGrandchild.children = mapGreatGrandchildren(
                      grandchild.children,
                      `${parentNo}.${grandchildNo - 1}`,
                      grandchild,
                      inheritedConditional || isConditionalField(grandchild)
                    );
                  }

                  mappedGrandchildren.push(mappedGrandchild);
                }

                for (const [conditionKey, grandchildrenWithCondition] of grandchildGroups) {
                  const { conditionInfo } = grandchildrenWithCondition[0];
                  const isRequired = conditionInfo?.type === 'x-required-when';
                  const parts = conditionKey.split(':');
                  const conditionName = parts[0];
                  const conditionValue = parts.slice(1).join(':');
                  const conditionText = isRequired
                    ? `Required (When "${conditionName}" is ${conditionValue})`
                    : `Optional (When "${conditionName}" is ${conditionValue})`;

                  mappedGrandchildren.push({
                    no: '', name: '', type: 'section-header',
                    section: conditionText,
                    default: '', description: '', required: '',
                  });

                  for (const { field: grandchild } of grandchildrenWithCondition) {
                    const mappedGrandchild: any = {
                      no: `${parentNo}.${grandchildNo++}`,
                      name: grandchild.key.split('.').pop() || grandchild.key,
                      type: grandchild.type === 'array' ? `Array[${grandchild.items?.type || 'any'}]` : grandchild.type,
                      default: formatDefaultValue(grandchild.default),
                      description: buildFieldDescription(grandchild, tableDefinition),
                      required: getRequiredLabel(grandchild, inheritedConditional),
                    };

                    if (grandchild.children && grandchild.children.length > 0) {
                      mappedGrandchild.children = mapGreatGrandchildren(
                        grandchild.children,
                        `${parentNo}.${grandchildNo - 1}`,
                        grandchild,
                        inheritedConditional || isConditionalField(grandchild)
                      );
                    }

                    mappedGrandchildren.push(mappedGrandchild);
                  }
                }

                return mappedGrandchildren;
              };

              // 🔥 조건 없는 children 먼저 렌더링
              for (const { field: child } of childrenWithoutCondition) {
                const currentNo = childNo++;
                const mappedChild: any = {
                  no: `${rowNumber - 1}.${currentNo}`,
                  name: child.key.split('.').pop() || child.key,
                  type: child.type === 'array' ? `Array[${child.items?.type || 'any'}]` : child.type,
                  default: formatDefaultValue(child.default),
                  description: buildFieldDescription(child, tableDefinition),
                  required: getRequiredLabel(child, parentConditional),
                };

                // 🔥 3-depth: Grandchildren mapping
                if ((child.children && child.children.length > 0) || (child.type === 'array' && child.items?.properties)) {
                  mappedChild.children = mapGrandchildren(
                    child.children || [],
                    `${rowNumber - 1}.${currentNo}`,
                    child,
                    parentConditional || isConditionalField(child)
                  );
                }

                param.children.push(mappedChild);
              }

              // 🔥 조건별 children 렌더링 - section-header 추가
              for (const [conditionKey, childrenWithCondition] of childGroups) {
                // 조건 정보 가져오기
                const { conditionInfo } = childrenWithCondition[0];
                const isRequired = conditionInfo?.type === 'x-required-when';
                
                // 조건 키에서 이름과 값 추출
                const parts = conditionKey.split(':');
                const conditionName = parts[0];
                const conditionValue = parts.slice(1).join(':');
                
                const conditionText = isRequired
                  ? `Required (When "${conditionName}" is ${conditionValue})`
                  : `Optional (When "${conditionName}" is ${conditionValue})`;

                // section-header 추가
                param.children.push({
                  no: '', name: '', type: 'section-header',
                  section: conditionText,
                  default: '', description: '', required: '',
                });

                // 조건에 맞는 children 추가
                for (const { field: child } of childrenWithCondition) {
                  const currentNo = childNo++;
                  const mappedChild: any = {
                    no: `${rowNumber - 1}.${currentNo}`,
                    name: child.key.split('.').pop() || child.key,
                    type: child.type === 'array' ? `Array[${child.items?.type || 'any'}]` : child.type,
                    default: formatDefaultValue(child.default),
                    description: buildFieldDescription(child, tableDefinition),
                    required: getRequiredLabel(child, parentConditional),
                  };

                  // 🔥 3-depth: Grandchildren mapping
                  if (child.children && child.children.length > 0) {
                    mappedChild.children = mapGrandchildren(
                      child.children,
                      `${rowNumber - 1}.${currentNo}`,
                      child,
                      parentConditional || isConditionalField(child)
                    );
                  } else if (child.type === 'array' && child.items?.properties) {
                    mappedChild.children = mapGrandchildren(
                      [],
                      `${rowNumber - 1}.${currentNo}`,
                      child,
                      parentConditional || isConditionalField(child)
                    );
                  }

                  param.children.push(mappedChild);
                }
              }
            }

            // 🔥 Description 빌드 (모듈화된 함수 사용)
            param.description = buildFieldDescription(field, tableDefinition);

            // 🔥 Required 상태 재계산 (TYPE별 다른 상태가 있는 경우)
            const requiredStatuses = Object.values(field.required);
            const hasRequired = requiredStatuses.some(s => s === 'required');
            const hasOptional = requiredStatuses.some(s => s === 'optional');
            const hasConditional = requiredStatuses.some(s => s === 'conditional');

            // 조건부 required 또는 TYPE별로 다른 경우 Conditional로 표시
            if (hasConditional || (hasRequired && hasOptional)) {
              param.required = 'Conditional';
            } else if (hasRequired) {
              param.required = 'Required';
            }

            params.push(param);
          }

          // 🔥 조건별 그룹 렌더링 (조건 헤더 + 필드들)
          for (const [conditionKey, fieldsWithCondition] of fieldGroups) {
            // 조건 정보 가져오기
            const conditionInfo = fieldsWithCondition[0].conditionInfo;
            const isRequired = conditionInfo?.type === 'x-required-when';
            
            // 조건 헤더 추가
            const conditionText = isRequired
              ? `Required (When ${conditionInfo?.conditionText || conditionKey})`
              : `Optional (When ${conditionInfo?.conditionText || conditionKey})`;
            
            params.push({
              no: '',
              section: conditionText,
              name: '',
              type: '',
              default: '',
              required: '',
              description: '',
            });

            // 해당 조건의 필드들 렌더링
            for (const { field } of fieldsWithCondition) {
              const param: any = {
                no: rowNumber++,
                name: field.key,
                type: field.type === 'array' ? `Array[${field.items?.type || 'any'}]` : field.type,
                default: formatDefaultValue(field.default),
                description: field.ui?.label || field.description || field.key,
                required: getRequiredLabel(field),
              };

              // 중첩 필드 처리 - section-header를 건너뛰는 번호 계산
              if (field.children && field.children.length > 0) {
                const parentConditional = isConditionalField(field);
                let childNo = 1;

                const buildArrayItemChildren = (arrayField: any) => {
                  const items = arrayField?.items;
                  if (!items || items.type !== 'object' || !items.properties) {
                    return [];
                  }

                  const itemRequired = items.required || [];
                  return Object.entries(items.properties).map(([itemKey, itemProp]) => {
                    const mappedItem: any = {
                      key: `${arrayField.key}[].${itemKey}`,
                      type: (itemProp as any).type,
                      default: (itemProp as any).default,
                      description: (itemProp as any).description,
                      required: itemRequired.includes(itemKey) ? { '*': 'required' } : { '*': 'optional' },
                    };

                    if ((itemProp as any).items) mappedItem.items = (itemProp as any).items;
                    if ((itemProp as any).enum) mappedItem.enum = (itemProp as any).enum;
                    if ((itemProp as any)['x-ui']) mappedItem.ui = (itemProp as any)['x-ui'];
                    if ((itemProp as any)['x-optional-when']) mappedItem['x-optional-when'] = (itemProp as any)['x-optional-when'];
                    if ((itemProp as any)['x-required-when']) mappedItem['x-required-when'] = (itemProp as any)['x-required-when'];

                    return mappedItem;
                  });
                };

                const mapGrandchildren = (grandchildren: any[], parentNo: string, parentField?: any, inheritedConditional: boolean = false) => {
                  const mapGreatGrandchildren = (greatGrandchildren: any[], grandParentNo: string, grandParentField?: any, inheritedConditionalForGreat: boolean = false) => {
                    const resolvedGreatGrandchildren = greatGrandchildren.length > 0
                      ? greatGrandchildren
                      : buildArrayItemChildren(grandParentField);
                    const greatGrandchildSectionHeaders = resolvedGreatGrandchildren.filter((c: any) => c.type === 'section-header');
                    const greatGrandchildrenToProcess = resolvedGreatGrandchildren.filter((c: any) => c.type !== 'section-header');
                    const greatGrandchildFieldInfoMap = collectFieldConditionInfo(
                      greatGrandchildrenToProcess,
                      tableDefinition?.schemaExtensions?.conditional || []
                    );

                    const { fieldGroups: greatGrandchildGroups, noConditionFields: greatGrandchildrenWithoutCondition } = groupFieldsByCondition(
                      greatGrandchildrenToProcess,
                      greatGrandchildFieldInfoMap
                    );

                    const mappedGreatGrandchildren: any[] = greatGrandchildSectionHeaders.map((header: any) => ({
                      no: '',
                      name: '',
                      type: 'section-header',
                      section: header.section || header.ui?.label || header.description || header.key,
                      default: '',
                      description: '',
                      required: '',
                    }));
                    let greatGrandchildNo = 1;

                    for (const { field: greatGrandchild } of greatGrandchildrenWithoutCondition) {
                      mappedGreatGrandchildren.push({
                        no: `${grandParentNo}.${greatGrandchildNo++}`,
                        name: greatGrandchild.key.split('.').pop() || greatGrandchild.key,
                        type: greatGrandchild.type === 'array' ? `Array[${greatGrandchild.items?.type || 'any'}]` : greatGrandchild.type,
                        default: formatDefaultValue(greatGrandchild.default),
                        description: buildFieldDescription(greatGrandchild, tableDefinition),
                        required: getRequiredLabel(greatGrandchild, inheritedConditionalForGreat),
                      });
                    }

                    for (const [conditionKey, greatGrandchildrenWithCondition] of greatGrandchildGroups) {
                      const { conditionInfo } = greatGrandchildrenWithCondition[0];
                      const isRequired = conditionInfo?.type === 'x-required-when';
                      const parts = conditionKey.split(':');
                      const conditionName = parts[0];
                      const conditionValue = parts.slice(1).join(':');
                      const conditionText = isRequired
                        ? `Required (When "${conditionName}" is ${conditionValue})`
                        : `Optional (When "${conditionName}" is ${conditionValue})`;

                      mappedGreatGrandchildren.push({
                        no: '', name: '', type: 'section-header',
                        section: conditionText,
                        default: '', description: '', required: '',
                      });

                      for (const { field: greatGrandchild } of greatGrandchildrenWithCondition) {
                        mappedGreatGrandchildren.push({
                          no: `${grandParentNo}.${greatGrandchildNo++}`,
                          name: greatGrandchild.key.split('.').pop() || greatGrandchild.key,
                          type: greatGrandchild.type === 'array' ? `Array[${greatGrandchild.items?.type || 'any'}]` : greatGrandchild.type,
                          default: formatDefaultValue(greatGrandchild.default),
                          description: buildFieldDescription(greatGrandchild, tableDefinition),
                          required: getRequiredLabel(greatGrandchild, inheritedConditionalForGreat),
                        });
                      }
                    }

                    return mappedGreatGrandchildren;
                  };

                  const resolvedGrandchildren = grandchildren.length > 0
                    ? grandchildren
                    : buildArrayItemChildren(parentField);
                  const grandchildrenToProcess = resolvedGrandchildren.filter((c: any) => c.type !== 'section-header');
                  const grandchildFieldInfoMap = collectFieldConditionInfo(
                    grandchildrenToProcess,
                    tableDefinition?.schemaExtensions?.conditional || []
                  );

                  const { fieldGroups: grandchildGroups, noConditionFields: grandchildrenWithoutCondition } = groupFieldsByCondition(
                    grandchildrenToProcess,
                    grandchildFieldInfoMap
                  );

                  const mappedGrandchildren: any[] = [];
                  let grandchildNo = 1;

                  for (const { field: grandchild } of grandchildrenWithoutCondition) {
                    const mappedGrandchild: any = {
                      no: `${parentNo}.${grandchildNo++}`,
                      name: grandchild.key.split('.').pop() || grandchild.key,
                      type: grandchild.type === 'array' ? `Array[${grandchild.items?.type || 'any'}]` : grandchild.type,
                      default: formatDefaultValue(grandchild.default),
                      description: buildFieldDescription(grandchild, tableDefinition),
                      required: getRequiredLabel(grandchild, inheritedConditional),
                    };

                    if (grandchild.children && grandchild.children.length > 0) {
                      mappedGrandchild.children = mapGreatGrandchildren(
                        grandchild.children,
                        `${parentNo}.${grandchildNo - 1}`,
                        grandchild,
                        inheritedConditional || isConditionalField(grandchild)
                      );
                    }

                    mappedGrandchildren.push(mappedGrandchild);
                  }

                  for (const [conditionKey, grandchildrenWithCondition] of grandchildGroups) {
                    const { conditionInfo } = grandchildrenWithCondition[0];
                    const isRequired = conditionInfo?.type === 'x-required-when';
                    const parts = conditionKey.split(':');
                    const conditionName = parts[0];
                    const conditionValue = parts.slice(1).join(':');
                    const conditionText = isRequired
                      ? `Required (When "${conditionName}" is ${conditionValue})`
                      : `Optional (When "${conditionName}" is ${conditionValue})`;

                    mappedGrandchildren.push({
                      no: '', name: '', type: 'section-header',
                      section: conditionText,
                      default: '', description: '', required: '',
                    });

                    for (const { field: grandchild } of grandchildrenWithCondition) {
                      const mappedGrandchild: any = {
                        no: `${parentNo}.${grandchildNo++}`,
                        name: grandchild.key.split('.').pop() || grandchild.key,
                        type: grandchild.type === 'array' ? `Array[${grandchild.items?.type || 'any'}]` : grandchild.type,
                        default: formatDefaultValue(grandchild.default),
                        description: buildFieldDescription(grandchild, tableDefinition),
                        required: getRequiredLabel(grandchild, inheritedConditional),
                      };

                      if (grandchild.children && grandchild.children.length > 0) {
                        mappedGrandchild.children = mapGreatGrandchildren(
                          grandchild.children,
                          `${parentNo}.${grandchildNo - 1}`,
                          grandchild,
                          inheritedConditional || isConditionalField(grandchild)
                        );
                      }

                      mappedGrandchildren.push(mappedGrandchild);
                    }
                  }

                  return mappedGrandchildren;
                };

                const childrenToProcess = field.children.filter((c: any) => c.type !== 'section-header');
                const childFieldInfoMap = collectFieldConditionInfo(
                  childrenToProcess,
                  tableDefinition?.schemaExtensions?.conditional || []
                );

                const { fieldGroups: childGroups, noConditionFields: childrenWithoutCondition } = groupFieldsByCondition(
                  childrenToProcess,
                  childFieldInfoMap
                );

                param.children = [];

                for (const { field: child } of childrenWithoutCondition) {
                  const currentNo = childNo++;
                  const mappedChild: any = {
                    no: `${rowNumber - 1}.${currentNo}`,
                    name: child.key.split('.').pop() || child.key,
                    type: child.type === 'array' ? `Array[${child.items?.type || 'any'}]` : child.type,
                    default: formatDefaultValue(child.default),
                    description: buildFieldDescription(child, tableDefinition),
                    required: getRequiredLabel(child, parentConditional),
                  };

                  if ((child.children && child.children.length > 0) || (child.type === 'array' && child.items?.properties)) {
                    mappedChild.children = mapGrandchildren(
                      child.children || [],
                      `${rowNumber - 1}.${currentNo}`,
                      child,
                      parentConditional || isConditionalField(child)
                    );
                  }

                  param.children.push(mappedChild);
                }

                for (const [conditionKey, childrenWithCondition] of childGroups) {
                  const { conditionInfo } = childrenWithCondition[0];
                  const isRequired = conditionInfo?.type === 'x-required-when';
                  const parts = conditionKey.split(':');
                  const conditionName = parts[0];
                  const conditionValue = parts.slice(1).join(':');
                  const conditionText = isRequired
                    ? `Required (When "${conditionName}" is ${conditionValue})`
                    : `Optional (When "${conditionName}" is ${conditionValue})`;

                  param.children.push({
                    no: '', name: '', type: 'section-header',
                    section: conditionText,
                    default: '', description: '', required: '',
                  });

                  for (const { field: child } of childrenWithCondition) {
                    const currentNo = childNo++;
                    const mappedChild: any = {
                      no: `${rowNumber - 1}.${currentNo}`,
                      name: child.key.split('.').pop() || child.key,
                      type: child.type === 'array' ? `Array[${child.items?.type || 'any'}]` : child.type,
                      default: formatDefaultValue(child.default),
                      description: buildFieldDescription(child, tableDefinition),
                      required: getRequiredLabel(child, parentConditional),
                    };

                    if (child.children && child.children.length > 0) {
                      mappedChild.children = mapGrandchildren(
                        child.children,
                        `${rowNumber - 1}.${currentNo}`,
                        child,
                        parentConditional || isConditionalField(child)
                      );
                    } else if (child.type === 'array' && child.items?.properties) {
                      mappedChild.children = mapGrandchildren(
                        [],
                        `${rowNumber - 1}.${currentNo}`,
                        child,
                        parentConditional || isConditionalField(child)
                      );
                    }

                    param.children.push(mappedChild);
                  }
                }
              }

              // 🔥 Description 빌드 (모듈화된 함수 사용)
              param.description = buildFieldDescription(field, tableDefinition);

              // 🔥 Required 상태 재계산 (TYPE별 다른 상태가 있는 경우)
              const requiredStatuses = Object.values(field.required);
              const hasRequired = requiredStatuses.some(s => s === 'required');
              const hasOptional = requiredStatuses.some(s => s === 'optional');
              const hasConditional = requiredStatuses.some(s => s === 'conditional');

              // 조건부 required 또는 TYPE별로 다른 경우 Conditional로 표시
              if (hasConditional || (hasRequired && hasOptional)) {
                param.required = 'Conditional';
              } else if (hasRequired) {
                param.required = 'Required';
              }

              params.push(param);
            }
          }
        }

        return params;
      } catch (error) {
        console.error('❌ Failed to compile enhanced schema for table:', error);
        return [];
      }
    }

    // Original Schema: compileSchema 결과를 직접 사용
    // schemaView가 'original'이면 Original 스키마로 컴파일
    // currentSchemaType은 위에서 이미 선언됨
    const sections = schemaView === 'original'
      ? compileSchema(effectiveSchema, psdSet, currentSchemaType)
      : canonicalFields;

    // Convert sections to table parameters format (same as Enhanced)
    const params: any[] = [];
    let rowNumber = 1;

    for (const section of sections) {
      // Add section header
      params.push({
        no: '',
        section: section.name,
        name: '',
        type: '',
        default: '',
        required: '',
        description: '',
      });

      // Add fields
      for (const field of section.fields) {
        params.push({
          no: rowNumber++,
          name: field.key,
          type: field.type === 'array' ? `Array[${field.items?.type || 'any'}]` : field.type,
          default: formatDefaultValue(field.default),
          description: field.ui?.label || field.description || field.key,
          required: getRequiredLabel(field),
        });
      }
    }

    return params;
  };

  const requestTableParameters = useMemo(
    () => buildTableParametersForSchema(enhancedRequestSchema, true, schemaType),
    [enhancedRequestSchema, initializedSchemaTypes, psdSet, schemaType, tableDefinition]
  );

  const responseTableParameters = useMemo(
    () => buildTableParametersForSchema(enhancedResponseSchema, true, schemaType),
    [enhancedResponseSchema, initializedSchemaTypes, psdSet, schemaType, tableDefinition]
  );

  const originalTableParameters = useMemo(
    () => buildTableParametersForSchema(
      combinedSpecData.jsonSchemaOriginal || combinedSpecData.jsonSchema,
      false,
      'original'
    ),
    [combinedSpecData.jsonSchemaOriginal, combinedSpecData.jsonSchema, initializedSchemaTypes, psdSet, tableDefinition]
  );

  const activeTableParameters = useMemo(() => {
    if (schemaView === 'original') {
      return originalTableParameters;
    }

    if (schemaView === 'enhanced') {
      if (tableView === 'response') return responseTableParameters;
      if (tableView === 'merged') return [];
      return requestTableParameters;
    }

    if (tableView === 'response') return responseTableParameters;
    if (tableView === 'request') return requestTableParameters;
    return [];
  }, [schemaView, tableView, originalTableParameters, requestTableParameters, responseTableParameters]);

  const canSendTable = useMemo(() => {
    if (schemaView !== 'original' && tableView === 'merged') {
      return requestTableParameters.length > 0 || responseTableParameters.length > 0;
    }
    return activeTableParameters.length > 0;
  }, [schemaView, tableView, activeTableParameters, requestTableParameters, responseTableParameters]);

  const spec = useMemo(() => ({
    title: fallbackSpec.title,
    description: fallbackSpec.description,
    jsonSchema: combinedSpecData.jsonSchemaOriginal || {},
    jsonSchemaEnhanced: combinedSpecData.jsonSchemaEnhanced,
    uri: fallbackSpec?.uri,
    methods: fallbackSpec?.methods,
  }), [combinedSpecData, fallbackSpec]);

  // Track which parameters are expanded
  const [expandedParams, setExpandedParams] = useState<Set<number>>(new Set());

  const tableParamsForExpansion = useMemo(() => {
    if (schemaView !== 'original' && tableView === 'merged') {
      return [...requestTableParameters, ...responseTableParameters];
    }

    return activeTableParameters;
  }, [schemaView, tableView, activeTableParameters, requestTableParameters, responseTableParameters]);

  // 🔥 초기 로드 시 모든 아코디언을 열린 상태로 설정
  useEffect(() => {
    if (tableParamsForExpansion && tableParamsForExpansion.length > 0) {
      const allParamsWithChildren = new Set<number>();
      tableParamsForExpansion.forEach((param: any) => {
        if (param.children && param.children.length > 0) {
          allParamsWithChildren.add(param.no);
        }
      });
      setExpandedParams(allParamsWithChildren);
    }
  }, [tableParamsForExpansion]);

  // 🎯 Editable Schema State
  const [editableSchema, setEditableSchema] = useState<string>('');
  const [isSchemaModified, setIsSchemaModified] = useState(false);

  // 🎯 저장된 스키마를 추적하는 state (리렌더링 트리거용)
  const [, setSavedSchema] = useState<any>(null);

  // 🔥 FIX: 이전 schemaView를 추적하여 탭 전환 감지
  const getViewKey = () => (schemaView === 'enhanced' ? `${schemaView}:${enhancedSubView}` : schemaView);
  const [prevViewKey, setPrevViewKey] = useState<string>(() => getViewKey());

  // Initialize editable schema
  useEffect(() => {
    // 🔥 FIX: schemaView가 변경되면 무조건 리셋 (탭 전환)
    const currentViewKey = getViewKey();
    const isTabSwitch = prevViewKey !== currentViewKey;

    if (isTabSwitch) {
      setPrevViewKey(currentViewKey);
      // 탭 전환 시 수정 상태도 리셋
      setIsSchemaModified(false);
    } else {
      // 🔥 같은 탭에서 수정 중인 경우에만 초기화 방지
      if (isSchemaModified && editableSchema) {
        return;
      }
    }

    // 스키마가 문자열인 경우 파싱, 객체인 경우 그대로 사용
    const getSchemaForView = () => {
      if (schemaView === 'original') {
        return spec.jsonSchema;
      }
      if (schemaView === 'enhanced') {
        return enhancedSubView === 'response' ? enhancedResponseSchema : enhancedRequestSchema;
      }
      return mergedSnapshot ?? mergedSchema;
    };

    const schemaForView = getSchemaForView();

    if (!schemaForView || (typeof schemaForView === 'object' && Object.keys(schemaForView).length === 0)) {
      setEditableSchema('{}');
    } else if (typeof schemaForView === 'string') {
      // 이미 문자열인 경우 파싱 시도
      try {
        const parsed = JSON.parse(schemaForView);
        setEditableSchema(JSON.stringify(parsed, null, 2));
      } catch {
        setEditableSchema(schemaForView);
      }
    } else {
      setEditableSchema(JSON.stringify(schemaForView, null, 2));
    }
    setIsSchemaModified(false);
  }, [schemaView, enhancedSubView, endpoint.id, spec.jsonSchema, spec.jsonSchemaEnhanced, enhancedRequestSchema, enhancedResponseSchema, mergedSchema, mergedSnapshot, prevViewKey]); // spec 의존성 추가

  // Handle schema changes
  const handleSchemaChange = (value: string) => {
    if (schemaView === 'merged') {
      return;
    }
    setEditableSchema(value);
    setIsSchemaModified(true);
  };

  // 🎯 Save Schema - 스키마 저장 및 테이블 반영
  const handleSaveSchema = async () => {
    try {
      const parsedSchema = JSON.parse(editableSchema);

      console.log('💾 handleSaveSchema - parsedSchema:', parsedSchema);
      console.log('💾 handleSaveSchema - schemaView:', schemaView);

      // 현재 스키마 뷰에 따라 적절한 스키마 업데이트
      // 항상 JSON 문자열로 저장 (서버와 호환성을 위해)
      if (schemaView === 'merged') {
        toast.info('Merged schema is read-only.');
        return;
      }

      const preserveComponents = (nextSchema: any, baseSchema: any) => {
        if (!nextSchema || typeof nextSchema !== 'object') return nextSchema;
        const result = deepClone(nextSchema);
        if (!result.components && baseSchema?.components) {
          result.components = deepClone(baseSchema.components);
        }
        if (!result['x-origin-name'] && baseSchema?.['x-origin-name']) {
          result['x-origin-name'] = baseSchema['x-origin-name'];
        }
        return result;
      };

      const updates: any = {};
      if (schemaView === 'original') {
        updates.jsonSchemaOriginal = JSON.stringify(parsedSchema);
        updates.jsonSchema = updates.jsonSchemaOriginal; // 호환성을 위해
      } else {
        const baseRequest = enhancedBundle.request
          || combinedSpecData.jsonSchemaOriginal
          || combinedSpecData.jsonSchema
          || {};
        const baseResponse = enhancedBundle.response || {};
        const nextRequestRaw = enhancedSubView === 'request' ? parsedSchema : baseRequest;
        const nextResponseRaw = enhancedSubView === 'response' ? parsedSchema : baseResponse;
        const nextRequest = preserveComponents(nextRequestRaw, baseRequest);
        const nextResponse = preserveComponents(nextResponseRaw, baseResponse);
        const nextEnhancedBundle: Record<string, any> = {
          request: nextRequest,
          response: nextResponse,
        };
        const nextRequestKey = enhancedBundle.requestKey || mergeNameHints.requestKey;
        const nextResponseKey = enhancedBundle.responseKey || mergeNameHints.responseKey;
        if (nextRequestKey) nextEnhancedBundle.requestKey = nextRequestKey;
        if (nextResponseKey) nextEnhancedBundle.responseKey = nextResponseKey;
        updates.jsonSchemaEnhanced = JSON.stringify(nextEnhancedBundle);
      }

      console.log('💾 handleSaveSchema - updates:', updates);

      // 로컬 상태 업데이트
      updateSpecData(updates);
      setSavedSchema(parsedSchema); // 리렌더링 트리거
      setIsSchemaModified(false);

      // 저장된 스키마로 editableSchema도 업데이트 (포맷팅 유지)
      setEditableSchema(JSON.stringify(parsedSchema, null, 2));

      // 🔥 스키마 캐시 클리어 (변경사항을 즉시 반영하기 위해)
      schemaCompileCache.clear();
      console.log('🗑️ Schema compilation cache cleared');

      // 서버에 저장 (버전이 있는 경우)
      if (currentVersionId) {
        try {
          console.log('💾 handleSaveSchema - Before saveCurrentVersion, specData:', specData);
          await saveCurrentVersion();
          console.log('💾 handleSaveSchema - After saveCurrentVersion, specData:', useAppStore.getState().specData);
          toast.success('✅ Schema saved to server!\n\nThe visual table has been updated with your changes.');
        } catch (error) {
          console.error('Failed to save to server:', error);
          toast.warning('⚠️ Schema saved locally, but failed to save to server.\n\nPlease create a version first.');
        }
      } else {
        toast.success('✅ Schema saved locally!\n\nPlease create a version to save to server.');
      }

      // 🔥 강제 리렌더링을 위해 specData 재설정
      const currentSpecData = useAppStore.getState().specData;
      console.log('💾 handleSaveSchema - Force re-render, currentSpecData:', currentSpecData);
    } catch (error) {
      toast.error('❌ Invalid JSON!\n\nPlease fix the syntax errors before saving.');
    }
  };

  // 🎯 Reset Schema - 원래 스키마로 되돌리기
  const handleResetSchema = () => {
    if (schemaView === 'merged') {
      const currentMerged = mergedSnapshot ?? mergedSchema;
      setEditableSchema(JSON.stringify(currentMerged, null, 2));
      setIsSchemaModified(false);
      return;
    }

    if (schemaView === 'original') {
      setEditableSchema(JSON.stringify(spec.jsonSchema, null, 2));
    } else {
      const resetSchema = enhancedSubView === 'response' ? enhancedResponseSchema : enhancedRequestSchema;
      setEditableSchema(JSON.stringify(resetSchema || {}, null, 2));
    }
    setIsSchemaModified(false);
  };

  // 🎯 Prettify Schema - 배열을 한 줄로 정렬
  const handlePrettifySchema = () => {
    if (schemaView === 'merged') {
      toast.info('Merged schema is read-only.');
      return;
    }

    try {
      const parsed = JSON.parse(editableSchema);

      // Custom stringify with inline arrays
      const stringifyWithInlineArrays = (obj: any, indent = 0): string => {
        const spaces = '  '.repeat(indent);
        const nextSpaces = '  '.repeat(indent + 1);

        if (Array.isArray(obj)) {
          // Check if array contains only primitives (numbers, strings, booleans)
          const isPrimitiveArray = obj.every(item =>
            typeof item === 'number' ||
            typeof item === 'string' ||
            typeof item === 'boolean' ||
            item === null
          );

          if (isPrimitiveArray) {
            // Format primitive arrays inline
            const items = obj.map(item =>
              typeof item === 'string' ? `"${item}"` : JSON.stringify(item)
            ).join(', ');
            return `[${items}]`;
          } else {
            // Complex arrays stay multi-line
            const items = obj.map(item =>
              nextSpaces + stringifyWithInlineArrays(item, indent + 1)
            ).join(',\n');
            return `[\n${items}\n${spaces}]`;
          }
        } else if (obj !== null && typeof obj === 'object') {
          const keys = Object.keys(obj);
          if (keys.length === 0) return '{}';

          const items = keys.map(key => {
            const value = obj[key];
            const valueStr = stringifyWithInlineArrays(value, indent + 1);
            return `${nextSpaces}"${key}": ${valueStr}`;
          }).join(',\n');

          return `{\n${items}\n${spaces}}`;
        } else {
          return JSON.stringify(obj);
        }
      };

      const prettified = stringifyWithInlineArrays(parsed, 0);
      setEditableSchema(prettified);
      setIsSchemaModified(true);
      toast.success('✅ Schema prettified successfully');
    } catch (error) {
      toast.error('❌ Invalid JSON!\n\nCannot prettify invalid JSON.');
    }
  };

  const getComparisonSummary = (comparison: any) => {
    if (!comparison) return 'comparison unavailable';
    return [
      `missing:${Array.isArray(comparison.missingInMerged) ? comparison.missingInMerged.length : 0}`,
      `extra:${Array.isArray(comparison.extraInMerged) ? comparison.extraInMerged.length : 0}`,
      `changed:${Array.isArray(comparison.changedSchemas) ? comparison.changedSchemas.length : 0}`,
    ].join(', ');
  };

  const handleApiMergeFromEnhanced = async () => {
    setIsSchemaApiBusy(true);
    try {
      const sourceSchema = sourceSchemaForApi || undefined;
      const { data, error } = await apiClient.mergeSchema({
        requestSchema: enhancedRequestSchema,
        responseSchema: enhancedResponseSchema,
        requestKey: mergeNameHints.requestKey,
        responseKey: mergeNameHints.responseKey,
        sourceSchema,
        autoFixAgainstSource: true,
      });

      if (error || !data) {
        toast.error(error || 'Schema merge API failed.');
        return;
      }

      const nextMerged = data.autoFixedMerged || data.mergedSchema;
      setMergedSnapshot(nextMerged);
      setSchemaView('merged');

      try {
        localStorage.setItem(
          roundtripStorageKey,
          JSON.stringify({
            sourceSchema,
            mergedSchema: data.mergedSchema,
            expectedSchema: data.expectedSchema,
            comparison: data.comparison,
            matchesSource: data.matchesSource,
            autoFixedMerged: data.autoFixedMerged,
            ranAt: new Date().toISOString(),
            mode: 'merge',
          })
        );
      } catch {
        // ignore cache errors
      }

      if (data.matchesSource === false) {
        toast.warning(`API merge mismatch detected (${getComparisonSummary(data.comparison)}). Auto-fixed merged output applied.`);
      } else if (data.matchesSource === true) {
        toast.success('API merge completed and matched source schema.');
      } else {
        toast.success('API merge completed.');
      }
    } finally {
      setIsSchemaApiBusy(false);
    }
  };

  const handleApiRoundtripAutofix = async () => {
    if (!sourceSchemaForApi) {
      toast.error('No source components schema available for roundtrip.');
      return;
    }

    setIsSchemaApiBusy(true);
    try {
      const { data, error } = await apiClient.roundtripSchema({
        sourceSchema: sourceSchemaForApi,
        autoFixAgainstSource: true,
      });
      if (error || !data) {
        toast.error(error || 'Schema roundtrip API failed.');
        return;
      }

      const split = data.splitResult;
      updateSpecData({
        jsonSchemaEnhanced: JSON.stringify({
          request: split.requestSchema,
          response: split.responseSchema,
          requestKey: split.requestKey,
          responseKey: split.responseKey,
        }),
      });

      setMergedSnapshot(data.autoFixedMerged || data.mergedSchema);
      setSchemaView('merged');

      try {
        localStorage.setItem(
          roundtripStorageKey,
          JSON.stringify({
            sourceSchema: sourceSchemaForApi,
            ...data,
            ranAt: new Date().toISOString(),
            mode: 'roundtrip',
          })
        );
      } catch {
        // ignore cache errors
      }

      if (data.matchesSource) {
        toast.success('API roundtrip passed. Enhanced Request/Response and merged output are synchronized.');
      } else {
        toast.warning(`API roundtrip mismatch detected (${getComparisonSummary(data.comparison)}). Auto-fixed merged output applied.`);
      }
    } finally {
      setIsSchemaApiBusy(false);
    }
  };

  const handleGenerateResponseFromRequest = () => {
    if (schemaView !== 'enhanced' || enhancedSubView !== 'request') {
      toast.info('Response auto-generation is available only for Enhanced Request schema.');
      return;
    }

    try {
      const parsedRequestSchema = JSON.parse(editableSchema);
      const preserveComponents = (nextSchema: any, baseSchema: any) => {
        if (!nextSchema || typeof nextSchema !== 'object') return nextSchema;
        const result = deepClone(nextSchema);
        if (!result.components && baseSchema?.components) {
          result.components = deepClone(baseSchema.components);
        }
        if (!result['x-origin-name'] && baseSchema?.['x-origin-name']) {
          result['x-origin-name'] = baseSchema['x-origin-name'];
        }
        return result;
      };

      const requestKey = enhancedBundle.requestKey
        || mergeNameHints.requestKey
        || inferMapBodyComponentName(parsedRequestSchema, 'request');
      const responseKey = enhancedBundle.responseKey
        || mergeNameHints.responseKey
        || inferResponseOriginNameFromRequest(parsedRequestSchema, { requestKey });

      const baseRequest = enhancedBundle.request
        || combinedSpecData.jsonSchemaOriginal
        || combinedSpecData.jsonSchema
        || {};
      const baseResponse = enhancedBundle.response || {};

      const nextRequest = preserveComponents(parsedRequestSchema, baseRequest);
      const generatedResponseSchema = buildMirroredResponseSchema(nextRequest, {
        requestKey,
        responseKey,
      });
      const nextResponse = preserveComponents(generatedResponseSchema, baseResponse);

      const nextEnhancedBundle: Record<string, any> = {
        request: nextRequest,
        response: nextResponse,
      };
      if (requestKey) nextEnhancedBundle.requestKey = requestKey;
      if (responseKey) nextEnhancedBundle.responseKey = responseKey;

      updateSpecData({
        jsonSchemaEnhanced: JSON.stringify(nextEnhancedBundle),
      });

      setSavedSchema(nextResponse);
      setEditableSchema(JSON.stringify(nextResponse, null, 2));
      setIsSchemaModified(false);
      setEnhancedSubView('response');

      toast.success('✅ Response schema generated from Request schema.\n\nReview the Response tab and click Save when ready.');
    } catch (error) {
      toast.error('❌ Invalid JSON!\n\nPlease fix the Request schema syntax before generating Response.');
    }
  };

  // 🎯 Enhanced → Original 변환 (Original 탭에 저장)
  const handleConvertToOriginal = () => {
    if (schemaView !== 'enhanced' || enhancedSubView !== 'request') {
      toast.info('Convert is available only for Enhanced Request schema.');
      return;
    }

    try {
      const parsedSchema = JSON.parse(editableSchema);

      // Enhanced → Original 변환 (x-* 마커 제거)
      const result = convertWithDetails(parsedSchema);

      if (result.removedMarkersCount === 0) {
        toast.info('ℹ️ No x-* markers found. Schema is already in Original format.');
        return;
      }

      // 🔥 변환된 스키마를 Original 탭에 저장 (Enhanced 탭은 유지)
      const originalSchemaStr = JSON.stringify(result.schema, null, 2);
      updateSpecData({
        jsonSchemaOriginal: originalSchemaStr,
        jsonSchema: originalSchemaStr,  // 호환성을 위해
      });

      // 🔥 FIX: 직접 editableSchema 설정 + isSchemaModified 리셋
      // useEffect가 isSchemaModified=true일 때 스킵하므로 직접 설정 필요
      setEditableSchema(originalSchemaStr);
      setIsSchemaModified(false);

      toast.success(
        `✅ Converted to Original tab!\n\nRemoved ${result.removedMarkersCount} x-* markers:\n${result.removedMarkerKeys.slice(0, 5).join(', ')}${result.removedMarkerKeys.length > 5 ? '...' : ''}`
      );

      console.log('🔄 Converted Enhanced → Original (saved to Original tab):', {
        removedCount: result.removedMarkersCount,
        removedKeys: result.removedMarkerKeys,
      });

      // 🔥 Original 탭으로 전환
      setSchemaView('original');
    } catch (error) {
      toast.error('❌ Invalid JSON!\n\nPlease fix the syntax errors before converting.');
    }
  };




  // 🎯 Display parameters - 이제 table view 직접 사용
  // const displayParameters = table view; // Unused - using table view directly

  const toggleParam = (paramNo: number) => {
    setExpandedParams((prev) => {
      const next = new Set(prev);
      if (next.has(paramNo)) {
        next.delete(paramNo);
      } else {
        next.add(paramNo);
      }
      return next;
    });
  };

  const renderWrapperInfoTable = (rawSchema: any) => {
    const wrapperKey = getWrapperKey(rawSchema);
    const wrapperInfo = wrapperKey ? rawSchema?.properties?.[wrapperKey] : null;

    const isAssignStyle = wrapperInfo?.additionalProperties || wrapperInfo?.patternProperties;
    const isArgumentStyle = wrapperKey === 'Argument' && wrapperInfo?.properties;

    if (!wrapperKey || (!isAssignStyle && !isArgumentStyle)) return null;

    const sectionTitle = 'Keyed Object Entry';
    const descriptionText = isArgumentStyle
      ? 'Request body wrapper object for Table API.'
      : wrapperKey === 'MCD'
        ? 'Response body wrapper object (keyed map).'
        : wrapperInfo.description || 'Map of keyed objects where each key is a string identifier.';

    return (
      <div>
        <h4 className="text-sm font-semibold text-cyan-400 mb-2">{sectionTitle}</h4>
        <p className="text-xs text-zinc-400 mb-3"></p>
        <div className="border rounded-lg overflow-hidden border-zinc-800">
          <table className="w-full text-sm">
            <thead className="bg-zinc-900">
              <tr>
                <th className="text-left p-3 border-b border-zinc-800" style={{ width: '6%' }}>No.</th>
                <th className="text-left p-3 border-b border-zinc-800" style={{ width: '35%' }}>Description</th>
                <th className="text-left p-3 border-b border-zinc-800" style={{ width: '14%' }}>Key</th>
                <th className="text-left p-3 border-b border-zinc-800" style={{ width: '10%' }}>Value Type</th>
                <th className="text-left p-3 border-b border-zinc-800" style={{ width: '10%' }}>Default</th>
                <th className="text-left p-3 border-b border-zinc-800" style={{ width: '25%' }}>Required</th>
              </tr>
            </thead>
            <tbody className="text-sm">
              <tr className="bg-cyan-950/30 border-b border-zinc-800">
                <td colSpan={6} className="p-2 text-cyan-400 font-semibold text-xs">
                  Root Object
                </td>
              </tr>
              <tr className="border-b border-zinc-800 hover:bg-zinc-800/30">
                <td className="p-3 text-zinc-400">1</td>
                <td className="p-3">
                  <div className="text-zinc-300">
                    {descriptionText}
                  </div>
                </td>
                <td className="p-3">
                  <code className="font-mono text-blue-400">"{wrapperKey}"</code>
                </td>
                <td className="p-3 text-zinc-400">object</td>
                <td className="p-3 text-zinc-500 font-mono text-xs">-</td>
                <td className="p-3">
                  <span className="px-2 py-0.5 text-xs rounded bg-red-600/20 text-red-400">Required</span>
                </td>
              </tr>
            </tbody>
          </table>
        </div>
      </div>
    );
  };

  // 🎯 Schema를 Manual로 전송
  const handleSendSchemaToManual = (schemaType: 'original' | 'enhanced') => {

    // 🔥 Enhanced 스키마인지 확인하는 함수
    const isEnhancedSchemaStructure = (schema: any): boolean => {
      if (!schema || typeof schema !== 'object') return false;

      const schemaStr = JSON.stringify(schema);
      // Enhanced 스키마의 특징: oneOf, x-section-header 등
      return schemaStr.includes('oneOf') ||
        schemaStr.includes('x-section-header') ||
        schemaStr.includes('allOf');
    };

    // 🔥 Original: fallbackSpec 또는 DB에서 가져오되, Enhanced 구조가 아닌 것만
    // Enhanced: DB의 jsonSchemaEnhanced 또는 Enhanced 구조를 가진 jsonSchemaOriginal
    let originalSchema: any;
    let enhancedSchema: any;

    if (fallbackSpec.jsonSchema && Object.keys(fallbackSpec.jsonSchema).length > 0) {
      // fallbackSpec이 있으면 그것을 Original로 사용
      originalSchema = fallbackSpec.jsonSchema;
      enhancedSchema = specData?.jsonSchemaEnhanced;
    } else {
      // fallbackSpec이 없으면 DB 데이터 사용
      const dbOriginal = specData?.jsonSchemaOriginal || specData?.jsonSchema;
      const dbEnhanced = specData?.jsonSchemaEnhanced;

      if (isEnhancedSchemaStructure(dbOriginal) && (!dbEnhanced || Object.keys(dbEnhanced).length === 0)) {
        // DB에 Enhanced가 Original에 저장되어 있는 경우
        console.warn('⚠️ Enhanced schema found in jsonSchemaOriginal! Using it as Enhanced.');
        originalSchema = {}; // Original이 없음
        enhancedSchema = dbOriginal; // Enhanced로 사용
      } else {
        // 정상적인 경우
        originalSchema = dbOriginal || {};
        enhancedSchema = dbEnhanced;
      }
    }

    // 🔥 스키마 타입에 따라 레이블 설정
    const enhancedBundleForManual = extractEnhancedBundle(enhancedSchema);
    const normalizedEnhancedSchema = enhancedBundleForManual.request || enhancedSchema;

    let schemaLabel: string;

    if (schemaType === 'original') {
      schemaLabel = 'Original';
    } else {
      if (!enhancedSchema || Object.keys(enhancedSchema).length === 0) {
        toast.error('❌ Enhanced schema is not available!');
        return;
      }
      schemaLabel = 'Enhanced';
    }

    // 🔥 FIX: Ensure schemas are objects for prettify
    // DB에서 가져온 데이터가 문자열일 수 있으므로 파싱하여 객체로 변환
    const safeParse = (val: any) => {
      if (!val) return {};
      if (typeof val === 'string') {
        try {
          return JSON.parse(val);
        } catch {
          return {};
        }
      }
      return val;
    };

    const parsedOriginal = safeParse(originalSchema);
    const parsedEnhanced = normalizedEnhancedSchema ? safeParse(normalizedEnhancedSchema) : undefined;
    const selectedSchema = schemaType === 'original' ? parsedOriginal : parsedEnhanced;

    // 🔥 기존 ManualData를 유지하면서 업데이트 (누적 방식)
    // 🎯 JSON으로 저장 (HTML이 아닌 실제 JSON 문자열)
    const newManualData: ManualData = {
      title: spec.title || endpoint.name,
      category: endpoint.method,
      inputUri: endpoint.path,
      activeMethods: endpoint.method,
      jsonSchema: JSON.stringify(selectedSchema, null, 2),  // 🔥 선택한 스키마 (JSON)
      jsonSchemaOriginal: JSON.stringify(parsedOriginal, null, 2),  // 🔥 항상 Original (JSON)
      jsonSchemaEnhanced: parsedEnhanced ? JSON.stringify(parsedEnhanced, null, 2) : undefined,  // 🔥 항상 Enhanced (JSON)
      examples: manualData?.examples || [],  // 🔥 기존 examples 유지
      requestExamples: manualData?.requestExamples || [],  // 🔥 기존 requestExamples 유지
      responseExamples: manualData?.responseExamples || [],  // 🔥 기존 responseExamples 유지
      specifications: manualData?.specifications || '',  // 🔥 기존 specifications 유지
    };

    setManualData(newManualData);
    toast.success(`✅ ${schemaLabel} Schema sent to Manual tab!`);
  };

  // 🎯 Table을 Manual로 전송
  const handleSendTableToManual = () => {
    let specificationsHTML = '';

    if (schemaView !== 'original' && tableView === 'merged') {
      if (!tableDefinition) {
        toast.error('? Table definition not loaded!');
        return;
      }

      const requestHtml = requestTableParameters.length > 0
        ? generateHTMLTable(requestTableParameters as TableParameter[], tableDefinition)
        : '<p>No request schema available.</p>';
      const responseHtml = responseTableParameters.length > 0
        ? generateHTMLTable(responseTableParameters as TableParameter[], tableDefinition)
        : '<p>No response schema available.</p>';

      specificationsHTML = `
<h3>Request</h3>
${requestHtml}
<br/>
<h3>Response</h3>
${responseHtml}`.trim();
    } else if ((isNewEnhancedSchema || (schemaView === 'enhanced' && activeSchema)) && tableView !== 'response') {
      try {
        const htmlDocument = generateHTMLDocument(activeSchema as EnhancedSchema, psdSet, schemaType);

        // ?? Extract body content only (remove <!DOCTYPE>, <html>, <head>, <body> tags)
        const bodyMatch = htmlDocument.match(/<body[^>]*>([\s\S]*)<\/body>/i);
        if (bodyMatch && bodyMatch[1]) {
          specificationsHTML = bodyMatch[1].trim();
        } else {
          specificationsHTML = htmlDocument;
        }
      } catch (error) {
        console.error('?Failed to generate HTML:', error);
        toast.error('?Failed to generate schema table');
        return;
      }
    } else {
      // ?? YAML ??: ??? ??? ???? HTML ??
      if (!tableDefinition) {
        toast.error('? Table definition not loaded!');
        return;
      }
      specificationsHTML = generateHTMLTable(activeTableParameters as TableParameter[], tableDefinition);
    }

    // ?? ?? ??? ????? ????
    const newManualData: ManualData = {
      title: spec.title || endpoint.name,
      category: endpoint.method,
      inputUri: endpoint.path,
      activeMethods: endpoint.method,
      jsonSchema: manualData?.jsonSchema || JSON.stringify(spec.jsonSchema, null, 2),
      jsonSchemaOriginal: manualData?.jsonSchemaOriginal || JSON.stringify(spec.jsonSchema, null, 2),
      jsonSchemaEnhanced: manualData?.jsonSchemaEnhanced || (spec.jsonSchemaEnhanced ? JSON.stringify(spec.jsonSchemaEnhanced, null, 2) : undefined),
      examples: manualData?.examples || [],  // deprecated
      requestExamples: manualData?.requestExamples || [],
      responseExamples: manualData?.responseExamples || [],
      specifications: specificationsHTML,  // ?? ??? HTML ??? ??
    };

    setManualData(newManualData);
    toast.success('?Table sent to Manual tab!');
  };


  return (
    <div className="h-full w-full flex flex-col">
      {/* 🔥 중앙 토글 헤더 */}
      <div className="flex-shrink-0 bg-zinc-900 border-b border-zinc-800 px-4 py-3">
        <div className="relative flex items-center justify-center">
          {/* 왼쪽: Schema View 레이블 */}
          <div className="absolute left-0 flex items-center gap-2">
            <h3 className="text-sm text-zinc-400">Schema View:</h3>
          </div>

          {/* 중앙: 토글 버튼 (절대 위치) - 개선 모드에서만 표시 */}
          {settings?.schemaMode !== 'normal' && (
            <div className="flex items-center gap-1 bg-zinc-800 rounded-lg p-1 w-[320px]">
              <button
                onClick={() => setSchemaView('original')}
                className={`flex-1 py-1.5 text-xs rounded transition-colors font-medium ${schemaView === 'original'
                  ? 'bg-blue-600 text-white'
                  : 'text-zinc-400 hover:text-zinc-200'
                  }`}
              >
                Original
              </button>
              <button
                onClick={() => setSchemaView('enhanced')}
                className={`flex-1 py-1.5 text-xs rounded transition-colors font-medium ${schemaView === 'enhanced'
                  ? 'bg-green-600 text-white'
                  : 'text-zinc-400 hover:text-zinc-200'
                  }`}
              >
                Enhanced
              </button>
              <button
                onClick={() => {
                  const recomputed = computeMergedSnapshot();
                  setMergedSnapshot(recomputed);
                  setSchemaView('merged');
                }}
                className={`flex-1 py-1.5 text-xs rounded transition-colors font-medium ${schemaView === 'merged'
                  ? 'bg-amber-600 text-white'
                  : 'text-zinc-400 hover:text-zinc-200'
                  }`}
              >
                Merged
              </button>
            </div>
          )}

          {/* 일반 모드에서는 제목만 표시 */}
          {settings?.schemaMode === 'normal' && (
            <div className="text-sm font-medium text-zinc-300">
              Schema Editor
            </div>
          )}

          {/* 오른쪽: 설명 텍스트 - 개선 모드에서만 */}
          {settings?.schemaMode !== 'normal' && (
            <div className="absolute right-0">
              <span className="text-xs text-zinc-500">
                {schemaView === 'original'
                  ? '(Original schema definition)'
                  : schemaView === 'enhanced'
                    ? (hasEnhancedSchema || isNewEnhancedSchema
                      ? '(Enhanced with x-ui, x-transport, conditions)'
                      : '(No enhanced schema - showing original)')
                    : '(Merged request + response with $ref)'}
              </span>
            </div>
          )}
        </div>
      </div>

      {/* 🔥 경고: Fallback 스키마 사용 중 */}
      {isUsingFallback && (
        <div className="flex-shrink-0 bg-yellow-900/30 border-b border-yellow-700/50 px-4 py-2 flex items-center gap-2">
          <AlertCircle className="w-4 h-4 text-yellow-400" />
          <span className="text-xs text-yellow-200">
            Using default schema template. Load a version to see saved schema.
          </span>
        </div>
      )}

      {/* 🎨 Visual Designer Mode */}
      {designerMode === 'visual' ? (
        <div className="flex-1 overflow-hidden">
          <SchemaDesigner
            initialSchema={activeSchema}
            psdSet={psdSet}
            onSchemaChange={(schemaJson) => {
              setEditableSchema(schemaJson);
              setIsSchemaModified(true);
            }}
          />
        </div>
      ) : (
        /* Resizable Panel Group - Code Mode */
        <ResizablePanelGroup direction="horizontal" className="flex-1">
          {/* Left Pane - JSON Schema Editor */}
          <ResizablePanel defaultSize={50} minSize={30}>
            <div className="h-full flex flex-col bg-zinc-950 overflow-hidden">
              <div className="p-4 border-b border-zinc-800 bg-zinc-900 flex-shrink-0">
                <div className="flex items-center justify-between">
                  <div className="flex items-center gap-2">
                    <h3 className="text-sm font-medium">JSON Schema Editor</h3>
                    {schemaView === 'enhanced' && (
                      <div className="flex items-center gap-1 bg-zinc-800 rounded-md p-1">
                        <button
                          onClick={() => setEnhancedSubView('request')}
                          className={`px-2 py-0.5 text-[10px] rounded transition-colors ${enhancedSubView === 'request'
                            ? 'bg-blue-600 text-white'
                            : 'text-zinc-400 hover:text-zinc-200'
                            }`}
                        >
                          Request
                        </button>
                        <button
                          onClick={() => setEnhancedSubView('response')}
                          className={`px-2 py-0.5 text-[10px] rounded transition-colors ${enhancedSubView === 'response'
                            ? 'bg-emerald-600 text-white'
                            : 'text-zinc-400 hover:text-zinc-200'
                            }`}
                        >
                          Response
                        </button>
                      </div>
                    )}
                    {schemaView === 'merged' && (
                      <span className="text-xs text-zinc-500">Read-only</span>
                    )}
                  </div>

                  <div className="flex items-center gap-2">
                    <Button
                      onClick={handlePrettifySchema}
                      variant="outline"
                      size="sm"
                      className="h-7 px-2 text-xs"
                      disabled={schemaView === 'merged'}
                    >
                      <Sparkles className="w-3 h-3 mr-1" />
                      Prettify
                    </Button>
                    {settings?.schemaMode !== 'normal' && (
                      <Button
                        onClick={handleApiMergeFromEnhanced}
                        variant="outline"
                        size="sm"
                        className="h-7 px-2 text-xs"
                        disabled={isSchemaApiBusy}
                      >
                        {isSchemaApiBusy ? 'API...' : 'API Merge'}
                      </Button>
                    )}
                    {settings?.schemaMode !== 'normal' && (
                      <Button
                        onClick={handleApiRoundtripAutofix}
                        variant="outline"
                        size="sm"
                        className="h-7 px-2 text-xs"
                        disabled={isSchemaApiBusy || !sourceSchemaForApi}
                      >
                        {isSchemaApiBusy ? 'API...' : 'API Roundtrip AutoFix'}
                      </Button>
                    )}
                  </div>
                </div>
              </div>

              {/* Monaco Editor - Full Height */}
              <div className="flex-1 relative min-h-0">
                <CodeEditor
                  value={editableSchema}
                  onChange={(value) => handleSchemaChange(value || '')}
                  language="json"
                  minimap={true}
                  readOnly={schemaView === 'merged'}
                />

                {/* Modified Indicator */}
                {isSchemaModified && (
                  <div className="absolute top-4 right-4 px-2 py-1 bg-orange-600/20 border border-orange-600/50 rounded text-xs text-orange-400 z-10">
                    Modified
                  </div>
                )}
              </div>

              {/* Footer with Save Button */}
              <div className="border-t border-zinc-800 bg-zinc-900 p-4 flex items-center justify-between flex-shrink-0 sticky bottom-0 z-10">
                <div className="flex items-center gap-2 text-xs text-zinc-500">
                  {isSchemaModified ? (
                    <>
                      <div className="w-2 h-2 bg-orange-500 rounded-full animate-pulse"></div>
                      <span>Unsaved changes</span>
                    </>
                  ) : (
                    <>
                      <div className="w-2 h-2 bg-green-500 rounded-full"></div>
                      <span>All changes saved</span>
                    </>
                  )}
                </div>

                <div className="flex items-center gap-2">
                  <Button
                    onClick={handleResetSchema}
                    variant="outline"
                    size="sm"
                    disabled={!isSchemaModified || schemaView === 'merged'}
                    className="h-7 px-2 text-xs"
                  >
                    Reset
                  </Button>

                  <Button
                    onClick={handleSaveSchema}
                    size="sm"
                    disabled={!isSchemaModified || schemaView === 'merged'}
                    className="h-7 px-2 text-xs bg-blue-600 hover:bg-blue-500 disabled:opacity-50 disabled:cursor-not-allowed"
                  >
                    <Save className="w-3 h-3 mr-1" />
                    Save
                  </Button>

                  {/* Enhanced → Original 변환 버튼 */}
                  {schemaView === 'enhanced' && enhancedSubView === 'request' && (
                    <Button
                      onClick={handleGenerateResponseFromRequest}
                      variant="outline"
                      size="sm"
                      className="h-7 px-2 text-xs border-emerald-600/50 text-emerald-400 hover:bg-emerald-600/20"
                    >
                      <Sparkles className="w-3 h-3 mr-1" />
                      Generate Response
                    </Button>
                  )}

                  {schemaView === 'enhanced' && enhancedSubView === 'request' && (
                    <Button
                      onClick={handleConvertToOriginal}
                      variant="outline"
                      size="sm"
                      className="h-7 px-2 text-xs border-orange-600/50 text-orange-400 hover:bg-orange-600/20"
                    >
                      <ArrowRight className="w-3 h-3 mr-1" />
                      Convert to Original
                    </Button>
                  )}

                  <Button
                    variant="outline"
                    size="sm"
                    onClick={() => handleSendSchemaToManual('original')}
                    disabled={!spec.jsonSchema}
                    className="text-xs"
                  >
                    📤 Send Original to Manual
                  </Button>

                  {(hasEnhancedSchema || isNewEnhancedSchema) && (
                    <Button
                      variant="outline"
                      size="sm"
                      onClick={() => handleSendSchemaToManual('enhanced')}
                      className="text-xs"
                    >
                      📤 Send Enhanced to Manual
                    </Button>
                  )}
                </div>
              </div>
            </div>
          </ResizablePanel>

          {/* Resize Handle */}
          <ResizableHandle className="w-1 bg-zinc-800 hover:bg-blue-500" />

          {/* Right Pane - Visual Table Renderer */}
          <ResizablePanel defaultSize={50} minSize={30}>
            <div className="h-full flex flex-col bg-zinc-950 overflow-hidden">
              <div className="p-4 border-b border-zinc-800 bg-zinc-900 flex-shrink-0">
                <div className="flex items-center justify-between">
                  <h3 className="text-sm font-medium">Visual Schema Table</h3>
                  {schemaView !== 'original' && (
                    <div className="flex items-center gap-1 bg-zinc-800 rounded-md p-1">
                      <button
                        onClick={() => setTableView('request')}
                        className={`px-2 py-0.5 text-[10px] rounded transition-colors ${tableView === 'request'
                          ? 'bg-blue-600 text-white'
                          : 'text-zinc-400 hover:text-zinc-200'
                          }`}
                      >
                        Request
                      </button>
                      <button
                        onClick={() => setTableView('response')}
                        className={`px-2 py-0.5 text-[10px] rounded transition-colors ${tableView === 'response'
                          ? 'bg-emerald-600 text-white'
                          : 'text-zinc-400 hover:text-zinc-200'
                          }`}
                      >
                        Response
                      </button>
                      <button
                        onClick={() => setTableView('merged')}
                        className={`px-2 py-0.5 text-[10px] rounded transition-colors ${tableView === 'merged'
                          ? 'bg-amber-600 text-white'
                          : 'text-zinc-400 hover:text-zinc-200'
                          }`}
                      >
                        Merged
                      </button>
                    </div>
                  )}
                </div>
              </div>

              {/* Table Content - Scrollable */}
              <div className="flex-1 overflow-auto p-4 space-y-6">
                {schemaView !== 'original' && tableView === 'merged' ? (
                  <div className="space-y-8">
                    <div>
                      <h4 className="text-sm font-semibold text-cyan-400 mb-2">Request</h4>
                      {requestTableParameters.length > 0 && tableDefinition ? (
                        <>
                          {renderWrapperInfoTable(enhancedRequestSchema)}
                          <div>
                            <h4 className="text-sm font-semibold text-cyan-400 mb-2">Item (Value Object Schema)</h4>
                            <p className="text-xs text-zinc-400 mb-3"></p>
                            <DynamicTableRenderer
                              definition={tableDefinition}
                              parameters={requestTableParameters}
                              expandedParams={expandedParams}
                              toggleParam={toggleParam}
                            />
                          </div>
                        </>
                      ) : (
                        <div className="flex items-center justify-center h-32 text-zinc-500">
                          No request schema loaded
                        </div>
                      )}
                    </div>

                    <div>
                      <h4 className="text-sm font-semibold text-cyan-400 mb-2">Response</h4>
                      {responseTableParameters.length > 0 && tableDefinition ? (
                        <>
                          {renderWrapperInfoTable(enhancedResponseSchema)}
                          <div>
                            <h4 className="text-sm font-semibold text-cyan-400 mb-2">Item (Value Object Schema)</h4>
                            <p className="text-xs text-zinc-400 mb-3"></p>
                            <DynamicTableRenderer
                              definition={tableDefinition}
                              parameters={responseTableParameters}
                              expandedParams={expandedParams}
                              toggleParam={toggleParam}
                            />
                          </div>
                        </>
                      ) : (
                        <div className="flex items-center justify-center h-32 text-zinc-500">
                          No response schema loaded
                        </div>
                      )}
                    </div>
                  </div>
                ) : (
                  activeTableParameters.length > 0 && tableDefinition ? (
                    <>
                      {(() => {
                        const rawSchema = schemaView === 'enhanced'
                          ? (tableView === 'response' ? enhancedResponseSchema : enhancedRequestSchema)
                          : combinedSpecData.jsonSchemaOriginal;

                        return renderWrapperInfoTable(rawSchema);
                      })()}

                      <div>
                        <h4 className="text-sm font-semibold text-cyan-400 mb-2">Item (Value Object Schema)</h4>
                        <p className="text-xs text-zinc-400 mb-3"></p>
                        <DynamicTableRenderer
                          definition={tableDefinition}
                          parameters={activeTableParameters}
                          expandedParams={expandedParams}
                          toggleParam={toggleParam}
                        />
                      </div>
                    </>
                  ) : (
                    <div className="flex items-center justify-center h-full text-zinc-500">
                      {schemaView === 'merged'
                        ? 'Merged schema preview only'
                        : isLoadingDefinition ? 'Loading table definition...' : 'No schema loaded'}
                    </div>
                  )
                )}
              </div>

              {/* Footer with Send to Manual Button */}
              <div className="border-t border-zinc-800 bg-zinc-900 p-4 flex items-center justify-end flex-shrink-0">
                <Button
                  variant="outline"
                  size="sm"
                  onClick={handleSendTableToManual}
                  disabled={!canSendTable}
                  className="text-xs"
                >
                  📤 Send Table to Manual
                </Button>
              </div>
            </div>
          </ResizablePanel>
        </ResizablePanelGroup>
      )}
    </div>
  );
}
