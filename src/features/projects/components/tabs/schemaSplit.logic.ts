export type SplitSchemaSliceKind = 'full' | 'entry' | 'ref' | 'inline';

export interface SplitSchemaSlice {
  id: string;
  kind: SplitSchemaSliceKind;
  label: string;
  subtitle?: string;
  description?: string;
  path: string;
  refName?: string;
  schema: any;
}

export interface SplitResult {
  requestKey: string;
  responseKey: string;
  requestSchema: any;
  responseSchema: any;
  requestSlices: SplitSchemaSlice[];
  responseSlices: SplitSchemaSlice[];
}

const REQUEST_KEY_REGEX = /_REQUEST(_|$)/i;
const RESPONSE_KEY_REGEX = /_RESPONSE(_|$)/i;
const REQUEST_MAP_BODY_REGEX = /_REQUEST_MAP_BODY$/i;
const RESPONSE_MAP_BODY_REGEX = /_RESPONSE_MAP_BODY$/i;
const COMPOSITION_KEYS = ['allOf', 'oneOf', 'anyOf'] as const;

const deepClone = <T,>(value: T): T => {
  try {
    return JSON.parse(JSON.stringify(value));
  } catch {
    return value;
  }
};

const truncate = (value: string, maxLength = 48): string => {
  if (value.length <= maxLength) return value;
  return `${value.slice(0, Math.max(0, maxLength - 3)).trim()}...`;
};

const humanizeToken = (value: string): string => {
  return value
    .split('_')
    .filter(Boolean)
    .map((token) => token.charAt(0) + token.slice(1).toLowerCase())
    .join(' ');
};

const formatRefLabel = (refName: string): string => {
  const match = refName.match(/^DTO_[A-Z0-9]+_(.+)$/);
  const token = match?.[1] || refName.replace(/^DTO_/, '');
  return humanizeToken(token);
};

const formatInlineLabel = (
  compositionKey: typeof COMPOSITION_KEYS[number],
  index: number,
  description?: string
): string => {
  if (description) {
    return truncate(description.replace(/\s+/g, ' '), 32);
  }
  const prefix = compositionKey === 'allOf'
    ? 'Rule'
    : compositionKey === 'oneOf'
      ? 'Option'
      : 'Branch';
  return `${prefix} ${index + 1}`;
};

const resolveRefName = (ref: any): string | null => {
  if (typeof ref !== 'string') return null;
  const match = ref.match(/^#\/components\/schemas\/(.+)$/);
  return match ? match[1] : null;
};

const getMapEntryContext = (schema: any) => {
  if (!schema || typeof schema !== 'object') return null;
  const props = schema?.properties;
  if (!props || typeof props !== 'object') return null;

  const keys = Object.keys(props);
  if (keys.length !== 1) return null;

  const wrapperKey = keys[0];
  const wrapper = props[wrapperKey];
  if (!wrapper || typeof wrapper !== 'object') return null;

  const additionalProperties = wrapper.additionalProperties;
  if (additionalProperties && typeof additionalProperties === 'object' && !Array.isArray(additionalProperties)) {
    return {
      wrapperKey,
      mode: 'additionalProperties' as const,
      entrySchema: additionalProperties,
    };
  }

  const patternProperties = wrapper.patternProperties;
  if (patternProperties && typeof patternProperties === 'object') {
    const entries = Object.entries(patternProperties).filter(([, value]) => value && typeof value === 'object');
    if (entries.length === 1) {
      const [patternKey, entrySchema] = entries[0];
      return {
        wrapperKey,
        mode: 'patternProperties' as const,
        patternKey,
        entrySchema,
      };
    }
  }

  return null;
};

const inlineRefsToSingleSchema = (root: any, components: Record<string, any>) => {
  const dereference = (node: any, refStack = new Set<string>()): any => {
    if (!node || typeof node !== 'object') return node;
    if (Array.isArray(node)) {
      return node.map((item) => dereference(item, refStack));
    }

    if (typeof node.$ref === 'string') {
      const refName = resolveRefName(node.$ref);
      if (refName && components[refName] && !refStack.has(refName)) {
        const nextStack = new Set(refStack);
        nextStack.add(refName);
        const resolved = dereference(deepClone(components[refName]), nextStack);
        const { $ref, ...rest } = node;
        return dereference({ ...resolved, ...rest }, nextStack);
      }
      return node;
    }

    const next: Record<string, any> = {};
    for (const [key, value] of Object.entries(node)) {
      if (key === 'components') continue;
      next[key] = dereference(value, refStack);
    }
    return next;
  };

  const inlined = dereference(deepClone(root), new Set());
  if (inlined && typeof inlined === 'object' && 'components' in inlined) {
    delete (inlined as any).components;
  }
  if (inlined && typeof inlined === 'object' && 'x-origin-name' in inlined) {
    delete (inlined as any)['x-origin-name'];
  }
  return inlined;
};

const pushRefSlice = (
  slices: SplitSchemaSlice[],
  path: string,
  refName: string,
  components: Record<string, any>,
  description?: string
) => {
  if (!components[refName]) return;

  slices.push({
    id: `ref:${path}:${refName}`,
    kind: 'ref',
    label: formatRefLabel(refName),
    subtitle: refName,
    description,
    path,
    refName,
    schema: inlineRefsToSingleSchema(components[refName], components),
  });
};

const pushInlineSlice = (
  slices: SplitSchemaSlice[],
  ownerName: string,
  compositionKey: typeof COMPOSITION_KEYS[number],
  index: number,
  path: string,
  node: any,
  components: Record<string, any>
) => {
  const description = typeof node?.description === 'string' ? node.description.trim() : undefined;
  slices.push({
    id: `inline:${path}`,
    kind: 'inline',
    label: formatInlineLabel(compositionKey, index, description),
    subtitle: `${ownerName} ${compositionKey}[${index}]`,
    description,
    path,
    schema: inlineRefsToSingleSchema(node, components),
  });
};

const pickMapBodyKey = (keys: string[], kind: 'request' | 'response') => {
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

const appendNestedSlices = (
  slices: SplitSchemaSlice[],
  ownerName: string,
  ownerSchema: any,
  components: Record<string, any>,
  basePath = ownerName,
  traversedRefs = new Set<string>()
) => {
  if (!ownerSchema || typeof ownerSchema !== 'object') return;

  const visit = (node: any, path: string) => {
    if (!node || typeof node !== 'object') return;

    if (Array.isArray(node)) {
      node.forEach((item, index) => {
        visit(item, `${path}[${index}]`);
      });
      return;
    }

    const refName = resolveRefName(node?.$ref);
    if (refName && components[refName]) {
      const description = typeof node?.description === 'string' ? node.description.trim() : undefined;
      pushRefSlice(slices, path, refName, components, description);

      if (!traversedRefs.has(refName)) {
        traversedRefs.add(refName);
        visit(components[refName], refName);
      }

      Object.entries(node).forEach(([key, value]) => {
        if (key === '$ref') return;
        visit(value, `${path}.${key}`);
      });
      return;
    }

    COMPOSITION_KEYS.forEach((compositionKey) => {
      const nodes = node?.[compositionKey];
      if (!Array.isArray(nodes)) return;

      nodes.forEach((child, index) => {
        const childPath = `${path}.${compositionKey}[${index}]`;
        if (!resolveRefName(child?.$ref)) {
          pushInlineSlice(slices, ownerName, compositionKey, index, childPath, child, components);
        }
        visit(child, childPath);
      });
    });

    Object.entries(node).forEach(([key, value]) => {
      if (key === 'components') return;
      if ((COMPOSITION_KEYS as readonly string[]).includes(key)) return;
      visit(value, `${path}.${key}`);
    });
  };

  visit(ownerSchema, basePath);
};

const dedupeSlices = (slices: SplitSchemaSlice[]): SplitSchemaSlice[] => {
  const seen = new Set<string>();
  return slices.filter((slice) => {
    if (seen.has(slice.id)) return false;
    seen.add(slice.id);
    return true;
  });
};

const buildSlicesForMapBody = (rootKey: string, schemas: Record<string, any>): SplitSchemaSlice[] => {
  const rootSchema = schemas[rootKey];
  const slices: SplitSchemaSlice[] = [
    {
      id: 'full',
      kind: 'full',
      label: 'Full',
      subtitle: rootKey,
      path: rootKey,
      refName: rootKey,
      schema: inlineRefsToSingleSchema(rootSchema, schemas),
    },
  ];

  const ctx = getMapEntryContext(rootSchema);
  if (!ctx) {
    appendNestedSlices(slices, rootKey, rootSchema, schemas, rootKey);
    return dedupeSlices(slices);
  }

  const entryRefName = resolveRefName(ctx.entrySchema?.$ref);
  const entrySource = entryRefName && schemas[entryRefName]
    ? schemas[entryRefName]
    : ctx.entrySchema;
  const entryPath = `${rootKey}.properties.${ctx.wrapperKey}.${ctx.mode}`;
  const entryLabel = entryRefName ? 'Item' : `${ctx.wrapperKey} Item`;
  const entrySubtitle = entryRefName || entryPath;

  slices.push({
    id: `entry:${rootKey}:${entryRefName || ctx.wrapperKey}`,
    kind: 'entry',
    label: entryLabel,
    subtitle: entrySubtitle,
    path: entryPath,
    refName: entryRefName || undefined,
    schema: inlineRefsToSingleSchema(entrySource, schemas),
  });

  appendNestedSlices(
    slices,
    entryRefName || ctx.wrapperKey,
    entrySource,
    schemas,
    entryRefName || entryPath
  );

  return dedupeSlices(slices);
};

export const computeSplitFromParsed = (parsed: any): SplitResult => {
  const schemas = parsed?.components?.schemas;
  if (!schemas || typeof schemas !== 'object') {
    throw new Error('components.schemas not found. Please provide a schema with components.schemas.');
  }

  const keys = Object.keys(schemas);
  const requestKey = pickMapBodyKey(keys, 'request');
  const responseKey = pickMapBodyKey(keys, 'response');

  if (!requestKey || !responseKey) {
    throw new Error(
      [
        'Could not find unique request/response MAP_BODY schema.',
        'Split tab only targets *_REQUEST_MAP_BODY and *_RESPONSE_MAP_BODY.',
        `Found REQUEST MAP_BODY: ${keys.filter((key) => REQUEST_MAP_BODY_REGEX.test(key)).join(', ') || 'none'}`,
        `Found RESPONSE MAP_BODY: ${keys.filter((key) => RESPONSE_MAP_BODY_REGEX.test(key)).join(', ') || 'none'}`,
      ].join('\n')
    );
  }

  const requestSchema = inlineRefsToSingleSchema(schemas[requestKey], schemas);
  const responseSchema = inlineRefsToSingleSchema(schemas[responseKey], schemas);

  if (requestSchema && typeof requestSchema === 'object') {
    (requestSchema as any)['x-origin-name'] = requestKey;
  }
  if (responseSchema && typeof responseSchema === 'object') {
    (responseSchema as any)['x-origin-name'] = responseKey;
  }

  return {
    requestKey,
    responseKey,
    requestSchema,
    responseSchema,
    requestSlices: buildSlicesForMapBody(requestKey, schemas),
    responseSlices: buildSlicesForMapBody(responseKey, schemas),
  };
};
