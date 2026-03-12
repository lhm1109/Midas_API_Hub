import express from 'express';

const router = express.Router();

const REQUEST_KEY_REGEX = /_REQUEST(_|$)/i;
const RESPONSE_KEY_REGEX = /_RESPONSE(_|$)/i;
const REQUEST_MAP_BODY_REGEX = /_REQUEST_MAP_BODY$/i;
const RESPONSE_MAP_BODY_REGEX = /_RESPONSE_MAP_BODY$/i;
const RESERVED_WRAPPER_KEYS = new Set(['ASSIGN', 'ARGUMENT', 'MCD']);

const deepClone = (value) => {
  try {
    return JSON.parse(JSON.stringify(value));
  } catch {
    return value;
  }
};

const parseSchemaInput = (value, fieldName) => {
  if (typeof value === 'string') {
    try {
      return JSON.parse(value);
    } catch (error) {
      throw new Error(`${fieldName} is not valid JSON string: ${error.message}`);
    }
  }
  if (!value || typeof value !== 'object') {
    throw new Error(`${fieldName} must be an object or JSON string`);
  }
  return value;
};

const stableStringify = (value) => {
  if (value === null || value === undefined) return String(value);
  if (typeof value !== 'object') return JSON.stringify(value);
  if (Array.isArray(value)) {
    return `[${value.map(stableStringify).join(',')}]`;
  }
  const keys = Object.keys(value).sort();
  return `{${keys.map((key) => `${JSON.stringify(key)}:${stableStringify(value[key])}`).join(',')}}`;
};

const stripOrigin = (schema) => {
  const cloned = deepClone(schema);
  if (cloned && typeof cloned === 'object') {
    delete cloned['x-origin-name'];
  }
  return cloned;
};

const toSchemaToken = (value) =>
  String(value || '')
    .replace(/[^a-zA-Z0-9]+/g, '_')
    .replace(/^_+|_+$/g, '')
    .toUpperCase();

const resolveRefNameFromRef = (ref) => {
  if (typeof ref !== 'string') return null;
  const match = ref.match(/^#\/components\/schemas\/(.+)$/);
  return match ? match[1] : null;
};

const parseMapBodyPrefix = (name) => {
  if (typeof name !== 'string' || !name) return null;
  const match = name.match(/^(.*)_(REQUEST|RESPONSE)_MAP_BODY$/i);
  return match ? match[1] : null;
};

const pickMapBodyKey = (keys, kind) => {
  if (!Array.isArray(keys) || keys.length === 0) return undefined;
  const tokenRegex = kind === 'request' ? REQUEST_KEY_REGEX : RESPONSE_KEY_REGEX;
  const mapBodyRegex = kind === 'request' ? REQUEST_MAP_BODY_REGEX : RESPONSE_MAP_BODY_REGEX;
  const tokenCandidates = keys.filter((key) => tokenRegex.test(key));
  if (tokenCandidates.length === 0) return undefined;

  const mapBodyCandidates = tokenCandidates.filter((key) => mapBodyRegex.test(key));
  if (mapBodyCandidates.length === 1) return mapBodyCandidates[0];
  if (tokenCandidates.length === 1) return tokenCandidates[0];

  return [...(mapBodyCandidates.length > 1 ? mapBodyCandidates : tokenCandidates)].sort((a, b) =>
    a.localeCompare(b)
  )[0];
};

const extractComponents = (schema) => {
  if (!schema || typeof schema !== 'object') {
    return { schema: {}, components: {} };
  }

  const components = schema?.components?.schemas;
  const baseSchema = { ...schema };
  if (baseSchema.components) delete baseSchema.components;

  return {
    schema: baseSchema,
    components: components && typeof components === 'object' ? deepClone(components) : {},
  };
};

const rewriteRefs = (node, refMap) => {
  if (!node || typeof node !== 'object') return node;
  if (Array.isArray(node)) return node.map((item) => rewriteRefs(item, refMap));

  const next = {};
  for (const [key, value] of Object.entries(node)) {
    if (key === '$ref' && typeof value === 'string' && refMap[value]) {
      next[key] = refMap[value];
      continue;
    }
    next[key] = rewriteRefs(value, refMap);
  }
  return next;
};

const inlineRefsToSingleSchema = (root, components) => {
  const dereference = (node, refStack = new Set()) => {
    if (!node || typeof node !== 'object') return node;
    if (Array.isArray(node)) return node.map((item) => dereference(item, refStack));

    if (typeof node.$ref === 'string') {
      const refName = resolveRefNameFromRef(node.$ref);
      if (refName && components[refName] && !refStack.has(refName)) {
        const nextStack = new Set(refStack);
        nextStack.add(refName);
        const resolved = dereference(deepClone(components[refName]), nextStack);
        const { $ref, ...rest } = node;
        return dereference({ ...resolved, ...rest }, nextStack);
      }
      return node;
    }

    const next = {};
    for (const [key, value] of Object.entries(node)) {
      if (key === 'components') continue;
      next[key] = dereference(value, refStack);
    }
    return next;
  };

  const inlined = dereference(deepClone(root), new Set());
  if (inlined && typeof inlined === 'object' && inlined.components) {
    delete inlined.components;
  }
  return inlined;
};

const getMapEntryContext = (schema) => {
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

const applyMapEntryRef = (schema, ctx, refName) => {
  if (!schema?.properties?.[ctx.wrapperKey]) return;
  const wrapper = schema.properties[ctx.wrapperKey];
  wrapper.additionalProperties = { $ref: `#/components/schemas/${refName}` };
  if (wrapper.patternProperties && typeof wrapper.patternProperties === 'object') {
    delete wrapper.patternProperties;
  }
};

const resolveMapEntrySource = (ctx, components) => {
  if (!ctx?.entrySchema || typeof ctx.entrySchema !== 'object') return null;
  const refName = resolveRefNameFromRef(ctx.entrySchema.$ref);
  if (refName && components?.[refName]) {
    return deepClone(components[refName]);
  }
  return deepClone(ctx.entrySchema);
};

const inferMapBodyComponentName = (schema, kind) => {
  const defaultName = kind === 'request' ? 'Request' : 'Response';
  if (!schema || typeof schema !== 'object') return defaultName;

  const origin = schema['x-origin-name'];
  if (typeof origin === 'string' && origin.trim()) {
    return origin.trim();
  }

  const ctx = getMapEntryContext(schema);
  if (!ctx) return defaultName;
  const wrapperToken = toSchemaToken(ctx.wrapperKey);
  if (wrapperToken && !RESERVED_WRAPPER_KEYS.has(wrapperToken)) {
    return `DTO_${wrapperToken}_${kind === 'request' ? 'REQUEST' : 'RESPONSE'}_MAP_BODY`;
  }

  return defaultName;
};

const reserveName = (schemas, usedNames, preferred, schema) => {
  if (!preferred || typeof preferred !== 'string') preferred = 'Component';
  if (!schemas[preferred]) {
    usedNames.add(preferred);
    return preferred;
  }
  if (stableStringify(stripOrigin(schemas[preferred])) === stableStringify(stripOrigin(schema))) {
    usedNames.add(preferred);
    return preferred;
  }
  let index = 1;
  let candidate = `${preferred}_${index}`;
  while (usedNames.has(candidate) || schemas[candidate]) {
    index += 1;
    candidate = `${preferred}_${index}`;
  }
  usedNames.add(candidate);
  return candidate;
};

const mergeRequestResponseSchemas = (requestSchema, responseSchema, options = {}) => {
  const requestClone = deepClone(requestSchema ?? {});
  const responseClone = deepClone(responseSchema ?? {});

  const { schema: requestBody, components: requestComponents } = extractComponents(requestClone);
  const { schema: responseBody, components: responseComponents } = extractComponents(responseClone);

  const mergedComponents = { ...requestComponents };
  const usedNames = new Set(Object.keys(mergedComponents));
  const refMap = {};

  for (const [name, schema] of Object.entries(responseComponents)) {
    if (!mergedComponents[name]) {
      mergedComponents[name] = schema;
      usedNames.add(name);
      continue;
    }
    if (stableStringify(stripOrigin(mergedComponents[name])) === stableStringify(stripOrigin(schema))) {
      continue;
    }
    let newName = `Response_${name}`;
    let i = 1;
    while (usedNames.has(newName) || mergedComponents[newName]) {
      i += 1;
      newName = `Response_${name}_${i}`;
    }
    mergedComponents[newName] = schema;
    usedNames.add(newName);
    refMap[`#/components/schemas/${name}`] = `#/components/schemas/${newName}`;
  }

  const normalizedResponseBody = Object.keys(refMap).length > 0 ? rewriteRefs(responseBody, refMap) : responseBody;
  if (requestBody && typeof requestBody === 'object') delete requestBody['x-origin-name'];
  if (normalizedResponseBody && typeof normalizedResponseBody === 'object') delete normalizedResponseBody['x-origin-name'];

  let requestName = String(options.requestKey || '').trim() || inferMapBodyComponentName(requestBody, 'request');
  let responseName = String(options.responseKey || '').trim() || inferMapBodyComponentName(normalizedResponseBody, 'response');

  const requestPrefix = parseMapBodyPrefix(requestName);
  const responsePrefix = parseMapBodyPrefix(responseName);
  if (!requestPrefix && responsePrefix) {
    requestName = `${responsePrefix}_REQUEST_MAP_BODY`;
  } else if (!responsePrefix && requestPrefix) {
    responseName = `${requestPrefix}_RESPONSE_MAP_BODY`;
  }

  const nextSchemas = { ...mergedComponents };
  const requestComponentName = reserveName(nextSchemas, usedNames, requestName, requestBody);
  const responseComponentName = reserveName(nextSchemas, usedNames, responseName, normalizedResponseBody);
  nextSchemas[requestComponentName] = deepClone(requestBody);
  nextSchemas[responseComponentName] = deepClone(normalizedResponseBody);

  const requestMap = getMapEntryContext(nextSchemas[requestComponentName]);
  const responseMap = getMapEntryContext(nextSchemas[responseComponentName]);
  const promotedNames = [];

  if (requestMap && responseMap) {
    const requestEntry = resolveMapEntrySource(requestMap, nextSchemas);
    const responseEntry = resolveMapEntrySource(responseMap, nextSchemas);
    if (requestEntry && responseEntry) {
      const requestSig = stableStringify(stripOrigin(requestEntry));
      const responseSig = stableStringify(stripOrigin(responseEntry));
      const prefix =
        parseMapBodyPrefix(requestComponentName) ||
        parseMapBodyPrefix(responseComponentName) ||
        (() => {
          const token = [requestMap.wrapperKey, responseMap.wrapperKey]
            .map((v) => toSchemaToken(v))
            .find((v) => v && !RESERVED_WRAPPER_KEYS.has(v));
          return token ? `DTO_${token}` : 'DTO_COMMON';
        })();

      if (requestSig === responseSig) {
        const sharedName = reserveName(nextSchemas, usedNames, `${prefix}_ITEM`, requestEntry);
        nextSchemas[sharedName] = requestEntry;
        applyMapEntryRef(nextSchemas[requestComponentName], requestMap, sharedName);
        applyMapEntryRef(nextSchemas[responseComponentName], responseMap, sharedName);
        promotedNames.push(sharedName);
      } else {
        const requestItemName = reserveName(nextSchemas, usedNames, `${prefix}_REQUEST_ITEM`, requestEntry);
        const responseItemName = reserveName(nextSchemas, usedNames, `${prefix}_RESPONSE_ITEM`, responseEntry);
        nextSchemas[requestItemName] = requestEntry;
        nextSchemas[responseItemName] = responseEntry;
        applyMapEntryRef(nextSchemas[requestComponentName], requestMap, requestItemName);
        applyMapEntryRef(nextSchemas[responseComponentName], responseMap, responseItemName);
        promotedNames.push(requestItemName, responseItemName);
      }
    }
  }

  const orderedSchemas = {};
  for (const name of [requestComponentName, responseComponentName, ...promotedNames]) {
    if (name && nextSchemas[name] && !orderedSchemas[name]) {
      orderedSchemas[name] = nextSchemas[name];
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

const splitCombinedSchema = (sourceSchema) => {
  const schemas = sourceSchema?.components?.schemas;
  if (!schemas || typeof schemas !== 'object') {
    throw new Error('components.schemas not found in source schema');
  }
  const keys = Object.keys(schemas);
  const requestKey = pickMapBodyKey(keys, 'request');
  const responseKey = pickMapBodyKey(keys, 'response');

  if (!requestKey || !responseKey) {
    throw new Error(
      `Request/Response key must be unique.\nFound REQUEST: ${keys.filter((k) => REQUEST_KEY_REGEX.test(k)).join(', ') || 'none'}\nFound RESPONSE: ${keys.filter((k) => RESPONSE_KEY_REGEX.test(k)).join(', ') || 'none'}`
    );
  }

  const requestSchema = inlineRefsToSingleSchema(schemas[requestKey], schemas);
  const responseSchema = inlineRefsToSingleSchema(schemas[responseKey], schemas);
  if (requestSchema && typeof requestSchema === 'object') {
    requestSchema['x-origin-name'] = requestKey;
  }
  if (responseSchema && typeof responseSchema === 'object') {
    responseSchema['x-origin-name'] = responseKey;
  }

  return {
    requestKey,
    responseKey,
    requestSchema,
    responseSchema,
  };
};

const toCanonicalComponentsSchema = (schema) => {
  const components = schema?.components?.schemas;
  if (components && typeof components === 'object') {
    return {
      components: {
        schemas: deepClone(components),
      },
    };
  }
  return deepClone(schema);
};

const compareSchemas = (expected, actual) => {
  const expectedSchemas = expected?.components?.schemas || {};
  const actualSchemas = actual?.components?.schemas || {};

  const expectedKeys = Object.keys(expectedSchemas);
  const actualKeys = Object.keys(actualSchemas);

  const missingInMerged = expectedKeys.filter((key) => !actualKeys.includes(key));
  const extraInMerged = actualKeys.filter((key) => !expectedKeys.includes(key));
  const changedSchemas = expectedKeys.filter(
    (key) =>
      actualSchemas[key] &&
      stableStringify(stripOrigin(expectedSchemas[key])) !== stableStringify(stripOrigin(actualSchemas[key]))
  );

  const signaturesEqual = stableStringify(stripOrigin(expected)) === stableStringify(stripOrigin(actual));
  return {
    matches: signaturesEqual,
    missingInMerged,
    extraInMerged,
    changedSchemas,
  };
};

router.post('/split', (req, res) => {
  try {
    const sourceSchema = parseSchemaInput(req.body?.sourceSchema, 'sourceSchema');
    const splitResult = splitCombinedSchema(sourceSchema);
    res.json(splitResult);
  } catch (error) {
    res.status(400).json({ error: error.message });
  }
});

router.post('/merge', (req, res) => {
  try {
    const requestSchema = parseSchemaInput(req.body?.requestSchema, 'requestSchema');
    const responseSchema = parseSchemaInput(req.body?.responseSchema, 'responseSchema');
    const requestKey = typeof req.body?.requestKey === 'string' ? req.body.requestKey : undefined;
    const responseKey = typeof req.body?.responseKey === 'string' ? req.body.responseKey : undefined;
    const sourceSchema =
      req.body?.sourceSchema !== undefined
        ? parseSchemaInput(req.body.sourceSchema, 'sourceSchema')
        : undefined;
    const autoFixAgainstSource = req.body?.autoFixAgainstSource !== false;

    const mergedSchema = mergeRequestResponseSchemas(requestSchema, responseSchema, { requestKey, responseKey });
    if (!sourceSchema) {
      res.json({ mergedSchema, matchesSource: null, comparison: null, autoFixedMerged: null });
      return;
    }

    const expectedSchema = toCanonicalComponentsSchema(sourceSchema);
    const comparison = compareSchemas(expectedSchema, mergedSchema);
    const autoFixedMerged = !comparison.matches && autoFixAgainstSource ? expectedSchema : null;

    res.json({
      mergedSchema,
      expectedSchema,
      matchesSource: comparison.matches,
      comparison,
      autoFixedMerged,
    });
  } catch (error) {
    res.status(400).json({ error: error.message });
  }
});

router.post('/roundtrip', (req, res) => {
  try {
    const sourceSchema = parseSchemaInput(req.body?.sourceSchema, 'sourceSchema');
    const autoFixAgainstSource = req.body?.autoFixAgainstSource !== false;

    const splitResult = splitCombinedSchema(sourceSchema);
    const mergedSchema = mergeRequestResponseSchemas(splitResult.requestSchema, splitResult.responseSchema, {
      requestKey: splitResult.requestKey,
      responseKey: splitResult.responseKey,
    });
    const expectedSchema = toCanonicalComponentsSchema(sourceSchema);
    const comparison = compareSchemas(expectedSchema, mergedSchema);
    const autoFixedMerged = !comparison.matches && autoFixAgainstSource ? expectedSchema : null;

    res.json({
      splitResult,
      mergedSchema,
      expectedSchema,
      matchesSource: comparison.matches,
      comparison,
      autoFixedMerged,
    });
  } catch (error) {
    res.status(400).json({ error: error.message });
  }
});

export default router;
