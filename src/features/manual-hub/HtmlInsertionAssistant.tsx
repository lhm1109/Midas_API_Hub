import { useEffect, useMemo, useState } from 'react';
import { toast } from 'sonner';
import { Button } from '@/components/ui/button';
import { Input } from '@/components/ui/input';
import { Label } from '@/components/ui/label';
import {
  Select,
  SelectContent,
  SelectItem,
  SelectTrigger,
  SelectValue,
} from '@/components/ui/select';
import { Textarea } from '@/components/ui/textarea';
import type { ApiEndpoint } from '@/types';
import {
  buildEndpointRowSnippet,
  extractHtmlTableTargets,
  insertSnippetIntoTableHtml,
  type HtmlInsertMode,
} from './htmlInsertHelpers';

type HtmlInsertionAssistantProps = {
  htmlContent: string;
  endpoint: ApiEndpoint | null;
  endpointUrl?: string;
  onApplyHtml: (nextHtml: string) => void;
};

type InsertPositionOption = {
  key: string;
  label: string;
  insertMode: HtmlInsertMode;
  rowIndex?: number;
};

function buildInsertPositionOptions(
  table: ReturnType<typeof extractHtmlTableTargets>[number] | null
): InsertPositionOption[] {
  if (!table) return [];

  if (table.rows.length === 0) {
    return [
      { key: `t-${table.tableIndex}-before`, label: '테이블 바로 위', insertMode: 'before-table' },
      {
        key: `t-${table.tableIndex}-append`,
        label: '테이블 내부 맨 아래',
        insertMode: 'append-rows-end',
      },
      { key: `t-${table.tableIndex}-after`, label: '테이블 바로 아래', insertMode: 'after-table' },
    ];
  }

  const options: InsertPositionOption[] = [
    { key: `t-${table.tableIndex}-before`, label: '테이블 바로 위', insertMode: 'before-table' },
    {
      key: `t-${table.tableIndex}-append`,
      label: '테이블 본문 맨 아래',
      insertMode: 'append-rows-end',
    },
  ];

  for (const row of table.rows) {
    options.push({
      key: `t-${table.tableIndex}-row-${row.rowIndex}-before`,
      label: `${row.rowIndex + 1}번 행 앞 · ${row.label}`,
      insertMode: 'before-row',
      rowIndex: row.rowIndex,
    });
    options.push({
      key: `t-${table.tableIndex}-row-${row.rowIndex}-after`,
      label: `${row.rowIndex + 1}번 행 뒤 · ${row.label}`,
      insertMode: 'after-row',
      rowIndex: row.rowIndex,
    });
  }

  options.push({
    key: `t-${table.tableIndex}-after`,
    label: '테이블 바로 아래',
    insertMode: 'after-table',
  });

  return options;
}

export function HtmlInsertionAssistant({
  htmlContent,
  endpoint,
  endpointUrl,
  onApplyHtml,
}: HtmlInsertionAssistantProps) {
  const [snippetMode, setSnippetMode] = useState<'endpoint-row' | 'custom'>('endpoint-row');
  const [detailLabel, setDetailLabel] = useState('');
  const [detailHref, setDetailHref] = useState('');
  const [customHtml, setCustomHtml] = useState('');
  const [selectedTableIndex, setSelectedTableIndex] = useState<string>('');
  const [selectedPositionKey, setSelectedPositionKey] = useState<string>('');

  const tableTargets = useMemo(() => extractHtmlTableTargets(htmlContent), [htmlContent]);

  const selectedTable = useMemo(() => {
    const parsedIndex = Number(selectedTableIndex);
    if (Number.isFinite(parsedIndex)) {
      return tableTargets.find((table) => table.tableIndex === parsedIndex) ?? tableTargets[0] ?? null;
    }
    return tableTargets[0] ?? null;
  }, [selectedTableIndex, tableTargets]);

  const endpointSnippet = useMemo(() => {
    if (!endpoint) return '';
    return buildEndpointRowSnippet(endpoint, detailLabel, detailHref);
  }, [endpoint, detailHref, detailLabel]);

  const effectiveSnippet = snippetMode === 'endpoint-row' ? endpointSnippet : customHtml;
  const insertPositionOptions = useMemo(
    () => buildInsertPositionOptions(selectedTable),
    [selectedTable]
  );
  const selectedPosition = useMemo(
    () => insertPositionOptions.find((option) => option.key === selectedPositionKey) ?? insertPositionOptions[0] ?? null,
    [insertPositionOptions, selectedPositionKey]
  );

  useEffect(() => {
    if (!endpoint) return;
    setDetailLabel(endpoint.name || endpoint.path || endpoint.id);
    setDetailHref(endpointUrl ?? '');
  }, [endpoint, endpointUrl]);

  useEffect(() => {
    if (!tableTargets.length) {
      setSelectedTableIndex('');
      return;
    }

    const currentExists = tableTargets.some((table) => String(table.tableIndex) === selectedTableIndex);
    if (!currentExists) {
      setSelectedTableIndex(String(tableTargets[0].tableIndex));
    }
  }, [selectedTableIndex, tableTargets]);

  useEffect(() => {
    if (!insertPositionOptions.length) {
      setSelectedPositionKey('');
      return;
    }
    const currentExists = insertPositionOptions.some((option) => option.key === selectedPositionKey);
    if (!currentExists) {
      setSelectedPositionKey(insertPositionOptions[0].key);
    }
  }, [insertPositionOptions, selectedPositionKey]);

  const handleInsert = () => {
    if (!selectedTable) {
      toast.error('삽입할 대상 테이블을 먼저 선택하세요.');
      return;
    }

    if (snippetMode === 'endpoint-row' && !endpoint) {
      toast.error('왼쪽 엔드포인트 목록의 `+` 버튼으로 삽입할 엔드포인트를 먼저 선택하세요.');
      return;
    }
    if (!selectedPosition) {
      toast.error('정확한 삽입 위치를 먼저 선택하세요.');
      return;
    }

    const result = insertSnippetIntoTableHtml({
      html: htmlContent,
      tableIndex: selectedTable.tableIndex,
      insertMode: selectedPosition.insertMode,
      rowIndex: selectedPosition.rowIndex,
      snippetHtml: effectiveSnippet,
    });

    if (!result.ok) {
      toast.error(result.message);
      return;
    }

    onApplyHtml(result.html);
    toast.success('선택한 위치에 HTML을 삽입했습니다.');
  };

  return (
    <div className="flex h-full min-h-0 flex-col bg-zinc-950">
      <div className="border-b border-zinc-800 px-4 py-3">
        <h3 className="text-sm font-medium text-zinc-100">HTML 삽입 도우미</h3>
        <p className="mt-1 text-[11px] leading-snug text-zinc-500">
          프리뷰 위에 정확히 드롭하지 않아도, 테이블과 행 위치를 직접 고른 뒤 HTML을 삽입할 수 있습니다.
        </p>
      </div>

      <div className="grid min-h-0 flex-1 gap-4 overflow-auto p-4 xl:grid-cols-[minmax(0,0.95fr)_minmax(0,1.05fr)]">
        <div className="space-y-4">
          <div className="rounded-lg border border-zinc-800 bg-zinc-900/60 p-3">
            <Label className="text-[11px] text-zinc-400">삽입할 HTML</Label>
            <div className="mt-2 flex flex-wrap gap-2">
              <Button
                type="button"
                size="sm"
                variant={snippetMode === 'endpoint-row' ? 'default' : 'outline'}
                className="h-8 text-xs"
                onClick={() => setSnippetMode('endpoint-row')}
              >
                Endpoint Row
              </Button>
              <Button
                type="button"
                size="sm"
                variant={snippetMode === 'custom' ? 'default' : 'outline'}
                className="h-8 text-xs"
                onClick={() => {
                  if (!customHtml.trim() && endpointSnippet) {
                    setCustomHtml(endpointSnippet);
                  }
                  setSnippetMode('custom');
                }}
              >
                Custom HTML
              </Button>
            </div>

            {snippetMode === 'endpoint-row' ? (
              <div className="mt-3 space-y-3">
                <div className="rounded-md border border-dashed border-zinc-700 bg-zinc-950/70 px-3 py-2 text-[11px] text-zinc-300">
                  {endpoint ? (
                    <>
                      <div className="font-medium text-zinc-100">{endpoint.name}</div>
                      <div className="mt-1 font-mono text-[10px] text-zinc-400">
                        {endpoint.method} {endpoint.path}
                      </div>
                    </>
                  ) : (
                    '왼쪽 엔드포인트 목록의 `+` 버튼으로 삽입할 엔드포인트를 고르세요.'
                  )}
                </div>
                <div className="grid gap-3 md:grid-cols-2">
                  <div>
                    <Label className="text-[11px] text-zinc-500">링크 텍스트</Label>
                    <Input
                      value={detailLabel}
                      onChange={(e) => setDetailLabel(e.target.value)}
                      className="mt-1 h-8 bg-zinc-950 text-xs border-zinc-700"
                      placeholder="Details 링크 텍스트"
                    />
                  </div>
                  <div>
                    <Label className="text-[11px] text-zinc-500">링크 URL</Label>
                    <Input
                      value={detailHref}
                      onChange={(e) => setDetailHref(e.target.value)}
                      className="mt-1 h-8 bg-zinc-950 text-xs border-zinc-700 font-mono"
                      placeholder="/hc/... 또는 https://..."
                    />
                  </div>
                </div>
                <div>
                  <Label className="text-[11px] text-zinc-500">생성될 행 미리보기</Label>
                  <Textarea
                    readOnly
                    value={endpointSnippet}
                    className="mt-1 min-h-44 bg-zinc-950 text-[11px] border-zinc-700 font-mono"
                  />
                </div>
              </div>
            ) : (
              <div className="mt-3">
                <Label className="text-[11px] text-zinc-500">Custom HTML</Label>
                <Textarea
                  value={customHtml}
                  onChange={(e) => setCustomHtml(e.target.value)}
                  className="mt-1 min-h-56 bg-zinc-950 text-[11px] border-zinc-700 font-mono"
                  placeholder="<tr>...</tr> 또는 원하는 HTML 블록을 입력하세요."
                />
              </div>
            )}
          </div>
        </div>

        <div className="space-y-4">
          <div className="rounded-lg border border-zinc-800 bg-zinc-900/60 p-3">
            <Label className="text-[11px] text-zinc-400">삽입 위치</Label>
            <div className="mt-3 grid gap-3">
              <div>
                <Label className="text-[11px] text-zinc-500">대상 테이블</Label>
                <Select value={selectedTableIndex} onValueChange={setSelectedTableIndex}>
                  <SelectTrigger className="mt-1 bg-zinc-950 border-zinc-700 text-xs">
                    <SelectValue placeholder="테이블을 선택하세요" />
                  </SelectTrigger>
                  <SelectContent>
                    {tableTargets.map((table) => (
                      <SelectItem
                        key={table.tableIndex}
                        value={String(table.tableIndex)}
                        className="text-xs"
                      >
                        {table.label}
                      </SelectItem>
                    ))}
                  </SelectContent>
                </Select>
                {tableTargets.length === 0 && (
                  <p className="mt-2 text-[11px] text-amber-400">
                    현재 HTML에서 삽입 가능한 테이블을 찾지 못했습니다.
                  </p>
                )}
              </div>

              <div>
                <Label className="text-[11px] text-zinc-500">정확한 삽입 위치</Label>
                <Select value={selectedPositionKey} onValueChange={setSelectedPositionKey}>
                  <SelectTrigger className="mt-1 bg-zinc-950 border-zinc-700 text-xs">
                    <SelectValue placeholder="삽입 위치를 선택하세요" />
                  </SelectTrigger>
                  <SelectContent>
                    {insertPositionOptions.map((option) => (
                      <SelectItem key={option.key} value={option.key} className="text-xs">
                        {option.label}
                      </SelectItem>
                    ))}
                  </SelectContent>
                </Select>
              </div>
            </div>
          </div>

          <div className="rounded-lg border border-zinc-800 bg-zinc-900/60 p-3">
            <div className="flex items-center justify-between gap-2">
              <div>
                <div className="text-[11px] text-zinc-400">선택된 테이블 요약</div>
                <div className="mt-1 text-xs text-zinc-100">
                  {selectedTable ? selectedTable.label : '선택된 테이블 없음'}
                </div>
                {selectedPosition && (
                  <div className="mt-1 text-[11px] text-emerald-300">
                    {selectedPosition.label}
                  </div>
                )}
              </div>
              <Button
                type="button"
                size="sm"
                className="h-8 bg-blue-600 text-xs"
                disabled={!selectedTable || !effectiveSnippet.trim()}
                onClick={handleInsert}
              >
                HTML 삽입
              </Button>
            </div>

            {selectedTable && (
              <div className="mt-3 space-y-2 text-[11px] text-zinc-400">
                <div>
                  <span className="text-zinc-500">Heading:</span>{' '}
                  <span className="text-zinc-300">{selectedTable.headingPath || '없음'}</span>
                </div>
                <div>
                  <span className="text-zinc-500">Header:</span>{' '}
                  <span className="text-zinc-300">{selectedTable.headerLabel}</span>
                </div>
                <div>
                  <span className="text-zinc-500">Data rows:</span>{' '}
                  <span className="text-zinc-300">{selectedTable.dataRowCount}</span>
                </div>
                {selectedTable.rows.length > 0 && (
                  <div className="rounded-md border border-zinc-800 bg-zinc-950/70 p-2">
                    <div className="mb-1 text-[10px] uppercase tracking-wider text-zinc-500">
                      Row Preview
                    </div>
                    <div className="space-y-1">
                      {selectedTable.rows.slice(0, 6).map((row) => (
                        <div key={row.rowIndex} className="truncate text-[11px] text-zinc-300">
                          {`Row ${row.rowIndex + 1} · ${row.label}`}
                        </div>
                      ))}
                      {selectedTable.rows.length > 6 && (
                        <div className="text-[10px] text-zinc-500">
                          + {selectedTable.rows.length - 6} more rows
                        </div>
                      )}
                    </div>
                  </div>
                )}
              </div>
            )}
          </div>
        </div>
      </div>
    </div>
  );
}
