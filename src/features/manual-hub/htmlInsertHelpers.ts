import type { ApiEndpoint } from '@/types';
import { buildIndexTableDataRowHtml } from './indexTableHtml';

export type HtmlInsertMode =
  | 'before-table'
  | 'after-table'
  | 'before-row'
  | 'after-row'
  | 'append-rows-end';

export type HtmlTableRowSummary = {
  rowIndex: number;
  label: string;
};

export type HtmlTableTarget = {
  tableIndex: number;
  label: string;
  headingPath: string;
  headerLabel: string;
  dataRowCount: number;
  rows: HtmlTableRowSummary[];
};

export const HTML_INSERT_ENDPOINT_DRAG_MIME = 'application/x-manual-hub-endpoint';
const HTML_ANCHOR_PATTERN = /<!--\s*MH_ANCHOR:\s*([A-Za-z0-9._:-]+)\s*-->/gi;

export type HtmlInsertEndpointPayload = {
  id: string;
  name: string;
  path: string;
  method: string;
  detailLabel: string;
  detailHref: string;
};

type ParsedHtmlDocument = {
  document: Document;
  isFullDocument: boolean;
  hadDoctype: boolean;
};

function parseHtmlDocument(html: string): ParsedHtmlDocument {
  const parser = new DOMParser();
  const trimmed = html.trimStart();
  const hadDoctype = /^<!doctype/i.test(trimmed);
  const isFullDocument = /<(?:!doctype|html|body)\b/i.test(trimmed);
  const source = isFullDocument ? html : `<body>${html}</body>`;

  return {
    document: parser.parseFromString(source, 'text/html'),
    isFullDocument,
    hadDoctype,
  };
}

function serializeHtmlDocument(parsed: ParsedHtmlDocument): string {
  if (!parsed.isFullDocument) {
    return parsed.document.body.innerHTML;
  }
  const docType = parsed.hadDoctype ? '<!DOCTYPE html>\n' : '';
  return `${docType}${parsed.document.documentElement.outerHTML}`;
}

function normalizeWhitespace(text: string): string {
  return text.replace(/\s+/g, ' ').trim();
}

function truncateText(text: string, maxLength = 72): string {
  if (text.length <= maxLength) return text;
  return `${text.slice(0, maxLength - 1)}…`;
}

function getDirectTableElements(document: Document): HTMLTableElement[] {
  return Array.from(document.querySelectorAll('table')).filter(
    (table) => table.parentElement?.closest('table') == null
  ) as HTMLTableElement[];
}

function extractTableRows(table: HTMLTableElement): HTMLTableRowElement[] {
  return Array.from(table.rows).filter((row) => row.closest('table') === table);
}

function extractDataRows(table: HTMLTableElement): HTMLTableRowElement[] {
  return extractTableRows(table).filter((row) =>
    Array.from(row.cells).some((cell) => cell.tagName === 'TD')
  );
}

function buildHeadingPaths(document: Document): Map<HTMLTableElement, string[]> {
  const pathByTable = new Map<HTMLTableElement, string[]>();
  const orderedNodes = Array.from(
    document.body.querySelectorAll('h1, h2, h3, h4, h5, h6, table')
  );
  const headingStack: string[] = [];

  for (const node of orderedNodes) {
    if (node.tagName === 'TABLE') {
      pathByTable.set(node as HTMLTableElement, headingStack.filter(Boolean));
      continue;
    }

    const level = Number(node.tagName.slice(1));
    const label = normalizeWhitespace(node.textContent ?? '');
    if (!label || Number.isNaN(level) || level < 1) continue;
    headingStack.splice(level - 1);
    headingStack[level - 1] = label;
  }

  return pathByTable;
}

export function extractHtmlTableTargets(html: string): HtmlTableTarget[] {
  const parsed = parseHtmlDocument(html);
  const tables = getDirectTableElements(parsed.document);
  const headingPaths = buildHeadingPaths(parsed.document);

  return tables.map((table, tableIndex) => {
    const headingPath = headingPaths.get(table)?.join(' > ') ?? '';
    const headerTexts = Array.from(table.querySelectorAll('th'))
      .filter((cell) => cell.closest('table') === table)
      .map((cell) => normalizeWhitespace(cell.textContent ?? ''))
      .filter(Boolean);
    const headerLabel = truncateText(headerTexts.slice(0, 3).join(' | ') || `Table ${tableIndex + 1}`);

    const rows = extractDataRows(table).map((row, rowIndex) => {
      const cells = Array.from(row.cells)
        .map((cell) => normalizeWhitespace(cell.textContent ?? ''))
        .filter(Boolean);
      return {
        rowIndex,
        label: truncateText(cells.join(' | ') || `Row ${rowIndex + 1}`),
      };
    });

    const parts = [`Table ${tableIndex + 1}`];
    if (headingPath) parts.push(headingPath);
    if (headerLabel) parts.push(headerLabel);

    return {
      tableIndex,
      label: parts.join(' · '),
      headingPath,
      headerLabel,
      dataRowCount: rows.length,
      rows,
    };
  });
}

function normalizeRowSnippetHtml(snippetHtml: string): string {
  const trimmed = snippetHtml.trim();
  if (!trimmed) return '';
  if (/<tr[\s>]/i.test(trimmed)) return trimmed;
  return `<tr>${trimmed}</tr>`;
}

function buildDocumentNodes(document: Document, snippetHtml: string): ChildNode[] {
  const template = document.createElement('template');
  template.innerHTML = snippetHtml.trim();
  return Array.from(template.content.childNodes);
}

function buildDocumentRows(document: Document, snippetHtml: string): HTMLTableRowElement[] {
  const template = document.createElement('template');
  template.innerHTML = normalizeRowSnippetHtml(snippetHtml);
  return Array.from(template.content.children).filter(
    (node) => node.tagName === 'TR'
  ) as HTMLTableRowElement[];
}

function shouldRenumberTable(table: HTMLTableElement): boolean {
  const firstHeaderCell = extractTableRows(table)[0]?.cells.item(0);
  const headerText = normalizeWhitespace(firstHeaderCell?.textContent ?? '');
  return /^no\.?$/i.test(headerText);
}

function renumberTableIfNeeded(table: HTMLTableElement): void {
  if (!shouldRenumberTable(table)) return;

  let nextNumber = 1;
  for (const row of extractDataRows(table)) {
    const firstCell = row.cells.item(0);
    if (!firstCell) continue;

    const current = normalizeWhitespace(firstCell.textContent ?? '');
    if (current && !/^\(?\d+\)?$/.test(current)) continue;

    const targetText = String(nextNumber++);
    const paragraph = firstCell.querySelector('p');
    if (paragraph) {
      paragraph.textContent = targetText;
    } else {
      firstCell.textContent = targetText;
    }
  }
}

export function buildEndpointRowSnippet(
  endpoint: ApiEndpoint,
  detailLabel: string,
  detailHref: string
): string {
  return buildIndexTableDataRowHtml({
    numberLabel: '',
    pathText: endpoint.path || endpoint.id,
    detailLabel: detailLabel.trim() || endpoint.name || endpoint.path || endpoint.id,
    detailHref: detailHref.trim(),
  }).trim();
}

export function buildHtmlInsertEndpointPayload(
  endpoint: ApiEndpoint,
  detailLabel: string,
  detailHref: string
): HtmlInsertEndpointPayload {
  return {
    id: endpoint.id,
    name: endpoint.name || endpoint.path || endpoint.id,
    path: endpoint.path || '',
    method: endpoint.method || '',
    detailLabel: detailLabel.trim() || endpoint.name || endpoint.path || endpoint.id,
    detailHref: detailHref.trim(),
  };
}

export function parseHtmlInsertEndpointPayload(
  raw: string
): HtmlInsertEndpointPayload | null {
  if (!raw.trim()) return null;
  try {
    const parsed = JSON.parse(raw) as Partial<HtmlInsertEndpointPayload>;
    if (!parsed.id || !parsed.path) return null;
    return {
      id: String(parsed.id),
      name: String(parsed.name ?? parsed.path ?? parsed.id),
      path: String(parsed.path),
      method: String(parsed.method ?? ''),
      detailLabel: String(parsed.detailLabel ?? parsed.name ?? parsed.path ?? parsed.id),
      detailHref: String(parsed.detailHref ?? ''),
    };
  } catch {
    return null;
  }
}

export function buildHtmlAnchorMarker(name: string): string {
  return `<!-- MH_ANCHOR:${name.trim()} -->`;
}

export function extractHtmlAnchorNames(html: string): string[] {
  const names = new Set<string>();
  for (const match of html.matchAll(HTML_ANCHOR_PATTERN)) {
    const name = match[1]?.trim();
    if (name) names.add(name);
  }
  return [...names];
}

export function insertSnippetAtAnchor(options: {
  html: string;
  anchorName: string;
  snippetHtml: string;
  position?: 'before' | 'after';
}): { ok: true; html: string } | { ok: false; message: string } {
  const { html, anchorName, snippetHtml, position = 'after' } = options;
  const trimmedName = anchorName.trim();
  const trimmedSnippet = snippetHtml.trim();

  if (!trimmedName) {
    return { ok: false, message: 'Anchor 이름이 비어 있습니다.' };
  }
  if (!trimmedSnippet) {
    return { ok: false, message: '삽입할 HTML이 비어 있습니다.' };
  }

  const marker = buildHtmlAnchorMarker(trimmedName);
  if (!html.includes(marker)) {
    return { ok: false, message: `HTML에서 anchor \`${trimmedName}\` 를 찾지 못했습니다.` };
  }

  const nextHtml =
    position === 'before'
      ? html.replace(marker, `${trimmedSnippet}\n${marker}`)
      : html.replace(marker, `${marker}\n${trimmedSnippet}`);

  return { ok: true, html: nextHtml };
}

export function insertSnippetIntoTableHtml(options: {
  html: string;
  tableIndex: number;
  insertMode: HtmlInsertMode;
  rowIndex?: number;
  snippetHtml: string;
}): { ok: true; html: string } | { ok: false; message: string } {
  const { html, tableIndex, insertMode, rowIndex, snippetHtml } = options;
  const trimmedSnippet = snippetHtml.trim();
  if (!trimmedSnippet) {
    return { ok: false, message: '삽입할 HTML이 비어 있습니다.' };
  }

  const parsed = parseHtmlDocument(html);
  const tables = getDirectTableElements(parsed.document);
  const table = tables[tableIndex];
  if (!table) {
    return { ok: false, message: '대상 테이블을 찾을 수 없습니다.' };
  }

  if (insertMode === 'before-table' || insertMode === 'after-table') {
    const nodes = buildDocumentNodes(parsed.document, trimmedSnippet);
    if (nodes.length === 0) {
      return { ok: false, message: '삽입할 노드를 만들지 못했습니다.' };
    }

    if (insertMode === 'before-table') {
      table.before(...nodes);
    } else {
      table.after(...nodes);
    }

    return { ok: true, html: serializeHtmlDocument(parsed) };
  }

  const rows = buildDocumentRows(parsed.document, trimmedSnippet);
  if (rows.length === 0) {
    return { ok: false, message: '행 삽입 모드에서는 `<tr>...</tr>` 형식의 HTML이 필요합니다.' };
  }

  const body = table.tBodies[0] ?? table.createTBody();
  const dataRows = extractDataRows(table);

  if (insertMode === 'append-rows-end' || dataRows.length === 0) {
    for (const row of rows) {
      body.append(row);
    }
    renumberTableIfNeeded(table);
    return { ok: true, html: serializeHtmlDocument(parsed) };
  }

  const safeRowIndex = Math.max(0, Math.min(rowIndex ?? 0, dataRows.length - 1));
  const referenceRow = dataRows[safeRowIndex];
  const anchorNode = insertMode === 'before-row' ? referenceRow : referenceRow.nextSibling;

  for (const row of rows) {
    body.insertBefore(row, anchorNode);
  }

  renumberTableIfNeeded(table);
  return { ok: true, html: serializeHtmlDocument(parsed) };
}
