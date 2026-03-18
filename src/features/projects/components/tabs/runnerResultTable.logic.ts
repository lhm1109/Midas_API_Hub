type Primitive = string | number | boolean | null;

type TableLikeNode = {
  HEAD: unknown[];
  vDATA: unknown[];
  [key: string]: unknown;
};

export interface RunnerResultTableMetaItem {
  key: string;
  value: string;
}

export interface RunnerResultTableColumn {
  key: string;
  header: string;
  align: 'left' | 'center' | 'right';
  isNumeric: boolean;
}

export interface RunnerResultTableRow {
  id: string;
  values: string[];
}

export interface RunnerResultTableModel {
  title: string;
  description?: string;
  path: string;
  meta: RunnerResultTableMetaItem[];
  columns: RunnerResultTableColumn[];
  rows: RunnerResultTableRow[];
}

const TABLE_HEADER_KEY = 'HEAD';
const TABLE_ROWS_KEY = 'vDATA';

const isRecord = (value: unknown): value is Record<string, unknown> => {
  return !!value && typeof value === 'object' && !Array.isArray(value);
};

const isPrimitive = (value: unknown): value is Primitive => {
  return value === null || ['string', 'number', 'boolean'].includes(typeof value);
};

const toDisplayString = (value: unknown): string => {
  if (value === undefined) return '';
  if (value === null) return '';
  if (typeof value === 'string') return value;
  if (typeof value === 'number' || typeof value === 'boolean') return String(value);

  try {
    return JSON.stringify(value);
  } catch {
    return String(value);
  }
};

const isNumericString = (value: string): boolean => {
  const trimmed = value.trim();
  if (!trimmed || trimmed === '-') return false;
  return /^-?(?:\d+\.?\d*|\.\d+)$/.test(trimmed);
};

const sanitizeHeader = (header: unknown, index: number): string => {
  const text = toDisplayString(header).trim();
  return text || `COL_${index + 1}`;
};

const inferColumnAlignment = (header: string, values: string[]): RunnerResultTableColumn['align'] => {
  const nonEmptyValues = values.filter((value) => value.trim().length > 0);
  const numericValues = nonEmptyValues.filter(isNumericString);
  const isNumeric = nonEmptyValues.length > 0 && numericValues.length >= Math.ceil(nonEmptyValues.length * 0.7);

  if (isNumeric) return 'right';
  if (header.length <= 6) return 'center';
  return 'left';
};

const isTableLikeNode = (value: unknown): value is TableLikeNode => {
  if (!isRecord(value)) return false;
  if (!Array.isArray(value[TABLE_HEADER_KEY]) || !Array.isArray(value[TABLE_ROWS_KEY])) return false;

  const headers = value[TABLE_HEADER_KEY];
  const rows = value[TABLE_ROWS_KEY];

  if (headers.length === 0 || rows.length === 0) return false;
  if (!headers.every((header) => typeof header === 'string' || typeof header === 'number')) return false;
  if (!rows.every((row) => Array.isArray(row))) return false;

  return true;
};

type LocatedTable = {
  path: string;
  node: TableLikeNode;
  ancestors: Record<string, unknown>[];
};

const findTableLikeNode = (
  value: unknown,
  path = '$',
  ancestors: Record<string, unknown>[] = [],
  visited = new Set<unknown>()
): LocatedTable | null => {
  if (!value || typeof value !== 'object') return null;
  if (visited.has(value)) return null;
  visited.add(value);

  if (isTableLikeNode(value)) {
    return {
      path,
      node: value,
      ancestors,
    };
  }

  if (Array.isArray(value)) {
    for (let index = 0; index < value.length; index += 1) {
      const result = findTableLikeNode(value[index], `${path}[${index}]`, ancestors, visited);
      if (result) return result;
    }
    return null;
  }

  const nextAncestors = [...ancestors, value as Record<string, unknown>];
  for (const [key, child] of Object.entries(value)) {
    if (!child || typeof child !== 'object') continue;
    const childPath = /^[A-Za-z_$][A-Za-z0-9_$]*$/.test(key) ? `${path}.${key}` : `${path}[${JSON.stringify(key)}]`;
    const result = findTableLikeNode(child, childPath, nextAncestors, visited);
    if (result) return result;
  }

  return null;
};

const collectMeta = (located: LocatedTable): RunnerResultTableMetaItem[] => {
  const ordered = new Map<string, string>();
  const sources = [...located.ancestors, located.node];

  sources.forEach((source) => {
    Object.entries(source).forEach(([key, value]) => {
      if (key === TABLE_HEADER_KEY || key === TABLE_ROWS_KEY) return;
      if (!isPrimitive(value)) return;
      ordered.set(key, toDisplayString(value));
    });
  });

  return Array.from(ordered.entries()).map(([key, value]) => ({ key, value }));
};

const buildTitle = (meta: RunnerResultTableMetaItem[]): string => {
  const explicitTitle = meta.find((item) => /title|name/i.test(item.key) && item.value.trim().length > 0);
  if (explicitTitle) {
    return explicitTitle.value;
  }
  return 'Result Table';
};

const flattenJsonRows = (
  value: unknown,
  path: string,
  rows: RunnerResultTableRow[],
  visited = new Set<unknown>()
) => {
  if (value && typeof value === 'object') {
    if (visited.has(value)) {
      rows.push({
        id: `row-${rows.length + 1}`,
        values: [path, 'circular', '[Circular]'],
      });
      return;
    }
    visited.add(value);
  }

  if (Array.isArray(value)) {
    if (value.length === 0) {
      rows.push({
        id: `row-${rows.length + 1}`,
        values: [path, 'array', '[]'],
      });
      return;
    }

    value.forEach((item, index) => {
      flattenJsonRows(item, `${path}[${index}]`, rows, visited);
    });
    return;
  }

  if (isRecord(value)) {
    const entries = Object.entries(value);
    if (entries.length === 0) {
      rows.push({
        id: `row-${rows.length + 1}`,
        values: [path, 'object', '{}'],
      });
      return;
    }

    entries.forEach(([key, child]) => {
      const nextPath = path === '$' ? key : `${path}.${key}`;
      flattenJsonRows(child, nextPath, rows, visited);
    });
    return;
  }

  rows.push({
    id: `row-${rows.length + 1}`,
    values: [path, value === null ? 'null' : typeof value, toDisplayString(value)],
  });
};

export const parseRunnerResultTable = (value: string | unknown): RunnerResultTableModel | null => {
  let parsed: unknown = value;

  if (typeof value === 'string') {
    try {
      parsed = JSON.parse(value);
    } catch {
      return null;
    }
  }

  const located = findTableLikeNode(parsed);
  if (!located) return null;

  const rowsSource = Array.isArray(located.node[TABLE_ROWS_KEY]) ? located.node[TABLE_ROWS_KEY] : [];
  const headersSource = Array.isArray(located.node[TABLE_HEADER_KEY]) ? located.node[TABLE_HEADER_KEY] : [];
  const maxColumnCount = Math.max(
    headersSource.length,
    ...rowsSource.map((row) => (Array.isArray(row) ? row.length : 0))
  );

  if (maxColumnCount === 0) return null;

  const headers = Array.from({ length: maxColumnCount }, (_, index) => sanitizeHeader(headersSource[index], index));

  const rows: RunnerResultTableRow[] = rowsSource.map((row, rowIndex) => {
    const normalizedRow = Array.isArray(row) ? row : [];
    const values = Array.from({ length: maxColumnCount }, (_, columnIndex) => toDisplayString(normalizedRow[columnIndex]));
    return {
      id: `row-${rowIndex + 1}`,
      values,
    };
  });

  const columns: RunnerResultTableColumn[] = headers.map((header, columnIndex) => {
    const values = rows.map((row) => row.values[columnIndex] || '');
    const align = inferColumnAlignment(header, values);
    return {
      key: `${header}-${columnIndex}`,
      header,
      align,
      isNumeric: align === 'right',
    };
  });

  const meta = collectMeta(located);

  return {
    title: buildTitle(meta),
    description: 'HEAD/vDATA response table preview',
    path: located.path,
    meta,
    columns,
    rows,
  };
};

export const parseRunnerJsonTable = (
  value: string | unknown,
  options?: {
    title?: string;
    description?: string;
  }
): RunnerResultTableModel | null => {
  let parsed: unknown = value;

  if (typeof value === 'string') {
    try {
      parsed = JSON.parse(value);
    } catch {
      return null;
    }
  }

  if (parsed === null || parsed === undefined || typeof parsed !== 'object') {
    return null;
  }

  const rows: RunnerResultTableRow[] = [];
  flattenJsonRows(parsed, '$', rows);

  if (rows.length === 0) return null;

  return {
    title: options?.title || 'JSON Table',
    description: options?.description || 'Generic JSON table preview',
    path: '$',
    meta: [],
    columns: [
      { key: 'path', header: 'Path', align: 'left', isNumeric: false },
      { key: 'type', header: 'Type', align: 'center', isNumeric: false },
      { key: 'value', header: 'Value', align: 'left', isNumeric: false },
    ],
    rows,
  };
};
