import { useCallback, useEffect, useMemo, useRef, useState } from 'react';
import Editor from '@monaco-editor/react';
import type { editor as MonacoEditor } from 'monaco-editor';
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
import { ResizableHandle, ResizablePanel, ResizablePanelGroup } from '@/components/ui/resizable';
import type { ApiEndpoint } from '@/types';
import {
  buildEndpointRowSnippet,
  buildHtmlAnchorMarker,
  buildHtmlInsertEndpointPayload,
  extractHtmlAnchorNames,
  HTML_INSERT_ENDPOINT_DRAG_MIME,
  insertSnippetAtAnchor,
  parseHtmlInsertEndpointPayload,
  type HtmlInsertEndpointPayload,
} from './htmlInsertHelpers';

type LocalHtmlWorkspaceProps = {
  htmlContent: string;
  endpoint: ApiEndpoint | null;
  endpointUrl?: string;
  renderVersion: number;
  onHtmlChange: (nextHtml: string) => void;
};

function clampRatio(value: number): number {
  if (!Number.isFinite(value)) return 0;
  return Math.max(0, Math.min(1, value));
}

function getEditorScrollRatio(editor: MonacoEditor.IStandaloneCodeEditor | null): number {
  if (!editor) return 0;
  const maxScroll = Math.max(0, editor.getScrollHeight() - editor.getLayoutInfo().height);
  if (maxScroll <= 0) return 0;
  return clampRatio(editor.getScrollTop() / maxScroll);
}

function getPreviewScrollingElement(
  iframe: HTMLIFrameElement | null
): HTMLElement | null {
  return (iframe?.contentDocument?.scrollingElement ??
    iframe?.contentDocument?.documentElement ??
    null) as HTMLElement | null;
}

function getPreviewScrollRatio(iframe: HTMLIFrameElement | null): number {
  const el = getPreviewScrollingElement(iframe);
  if (!el) return 0;
  const maxScroll = Math.max(0, el.scrollHeight - el.clientHeight);
  if (maxScroll <= 0) return 0;
  return clampRatio(el.scrollTop / maxScroll);
}

function setPreviewScrollRatio(iframe: HTMLIFrameElement | null, ratio: number): void {
  const el = getPreviewScrollingElement(iframe);
  if (!el) return;
  const maxScroll = Math.max(0, el.scrollHeight - el.clientHeight);
  el.scrollTop = maxScroll * clampRatio(ratio);
}

function payloadToEndpoint(payload: HtmlInsertEndpointPayload): ApiEndpoint {
  return {
    id: payload.id,
    name: payload.name,
    path: payload.path,
    method: payload.method,
  };
}

export function LocalHtmlWorkspace({
  htmlContent,
  endpoint,
  endpointUrl,
  renderVersion,
  onHtmlChange,
}: LocalHtmlWorkspaceProps) {
  const editorRef = useRef<MonacoEditor.IStandaloneCodeEditor | null>(null);
  const previewIframeRef = useRef<HTMLIFrameElement | null>(null);
  const syncSourceRef = useRef<'editor' | 'preview' | null>(null);
  const pendingPreviewRatioRef = useRef<number | null>(null);
  const previewScrollCleanupRef = useRef<(() => void) | null>(null);
  const [editorMountVersion, setEditorMountVersion] = useState(0);

  const [previewHtml, setPreviewHtml] = useState(htmlContent || '<p></p>');
  const [snippetMode, setSnippetMode] = useState<'endpoint-row' | 'custom'>('endpoint-row');
  const [selectedPayload, setSelectedPayload] = useState<HtmlInsertEndpointPayload | null>(null);
  const [detailLabel, setDetailLabel] = useState('');
  const [detailHref, setDetailHref] = useState('');
  const [customHtml, setCustomHtml] = useState('');
  const [insertTarget, setInsertTarget] = useState<'cursor' | 'anchor'>('cursor');
  const [anchorName, setAnchorName] = useState('');

  const anchorNames = useMemo(() => extractHtmlAnchorNames(htmlContent), [htmlContent]);

  useEffect(() => {
    const nextHtml = htmlContent || '<p></p>';
    const currentRatio = getPreviewScrollRatio(previewIframeRef.current);
    const timer = window.setTimeout(() => {
      pendingPreviewRatioRef.current = currentRatio;
      setPreviewHtml(nextHtml);
    }, 120);
    return () => window.clearTimeout(timer);
  }, [htmlContent, renderVersion]);

  useEffect(() => {
    if (!anchorNames.length) return;
    if (!anchorName.trim()) {
      setAnchorName(anchorNames[0]);
      return;
    }
    if (!anchorNames.includes(anchorName.trim())) {
      setAnchorName(anchorNames[0]);
    }
  }, [anchorName, anchorNames]);

  useEffect(() => {
    if (!endpoint) return;
    setSelectedPayload(
      buildHtmlInsertEndpointPayload(
        endpoint,
        endpoint.name || endpoint.path || endpoint.id,
        endpointUrl ?? ''
      )
    );
  }, [endpoint, endpointUrl]);

  useEffect(() => {
    if (!selectedPayload) return;
    setDetailLabel(selectedPayload.detailLabel);
    setDetailHref(selectedPayload.detailHref);
  }, [selectedPayload?.detailHref, selectedPayload?.detailLabel, selectedPayload?.id]);

  useEffect(() => {
    return () => {
      previewScrollCleanupRef.current?.();
    };
  }, []);

  const endpointSnippet = useMemo(() => {
    if (!selectedPayload) return '';
    return buildEndpointRowSnippet(
      payloadToEndpoint(selectedPayload),
      detailLabel,
      detailHref
    );
  }, [detailHref, detailLabel, selectedPayload]);

  const effectiveSnippet = useMemo(
    () => (snippetMode === 'endpoint-row' ? endpointSnippet : customHtml.trim()),
    [customHtml, endpointSnippet, snippetMode]
  );

  const insertTextAtCursor = useCallback(
    (text: string) => {
      const trimmed = text.trim();
      if (!trimmed) {
        toast.error('삽입할 HTML이 비어 있습니다.');
        return false;
      }

      const editor = editorRef.current;
      if (!editor) {
        onHtmlChange(`${htmlContent}${htmlContent.endsWith('\n') ? '' : '\n'}${trimmed}\n`);
        return true;
      }

      const model = editor.getModel();
      if (!model) return false;
      const position = editor.getPosition() ?? model.getFullModelRange().getEndPosition();
      const selection = editor.getSelection() ?? {
        startLineNumber: position.lineNumber,
        startColumn: position.column,
        endLineNumber: position.lineNumber,
        endColumn: position.column,
      };

      editor.executeEdits('manual-hub-workspace', [
        {
          range: selection,
          text: trimmed,
          forceMoveMarkers: true,
        },
      ]);
      const nextValue = model.getValue();
      onHtmlChange(nextValue);
      editor.focus();
      return true;
    },
    [htmlContent, onHtmlChange]
  );

  const handleInsertSnippet = useCallback(() => {
    if (!effectiveSnippet.trim()) {
      toast.error('삽입할 HTML을 먼저 준비하세요.');
      return;
    }

    if (snippetMode === 'endpoint-row' && !selectedPayload) {
      toast.error('왼쪽 목록에서 엔드포인트를 고르거나 드래그하세요.');
      return;
    }

    if (insertTarget === 'cursor') {
      if (insertTextAtCursor(effectiveSnippet)) {
        toast.success('커서 위치에 HTML을 삽입했습니다.');
      }
      return;
    }

    const result = insertSnippetAtAnchor({
      html: htmlContent,
      anchorName,
      snippetHtml: effectiveSnippet,
      position: 'after',
    });
    if (!result.ok) {
      toast.error(result.message);
      return;
    }
    onHtmlChange(result.html);
    toast.success(`anchor \`${anchorName}\` 아래에 HTML을 삽입했습니다.`);
  }, [
    anchorName,
    effectiveSnippet,
    htmlContent,
    insertTarget,
    insertTextAtCursor,
    onHtmlChange,
    selectedPayload,
    snippetMode,
  ]);

  const handleInsertAnchorMarker = useCallback(() => {
    const trimmedName = anchorName.trim();
    if (!trimmedName) {
      toast.error('anchor 이름을 입력하세요.');
      return;
    }
    if (insertTextAtCursor(buildHtmlAnchorMarker(trimmedName))) {
      toast.success(`anchor \`${trimmedName}\` 마커를 커서 위치에 넣었습니다.`);
    }
  }, [anchorName, insertTextAtCursor]);

  const handlePreviewLoad = useCallback(() => {
    previewScrollCleanupRef.current?.();

    const iframe = previewIframeRef.current;
    const editor = editorRef.current;
    const win = iframe?.contentWindow;
    if (!iframe || !win) return;

    const handlePreviewScroll = () => {
      if (syncSourceRef.current === 'editor') return;
      syncSourceRef.current = 'preview';
      const ratio = getPreviewScrollRatio(iframe);
      const activeEditor = editorRef.current;
      if (activeEditor) {
        const maxScroll = Math.max(
          0,
          activeEditor.getScrollHeight() - activeEditor.getLayoutInfo().height
        );
        activeEditor.setScrollTop(maxScroll * ratio);
      }
      window.requestAnimationFrame(() => {
        syncSourceRef.current = null;
      });
    };

    win.addEventListener('scroll', handlePreviewScroll, { passive: true });
    previewScrollCleanupRef.current = () => {
      win.removeEventListener('scroll', handlePreviewScroll);
    };

    const restoreRatio = pendingPreviewRatioRef.current ?? getEditorScrollRatio(editor);
    pendingPreviewRatioRef.current = null;
    window.requestAnimationFrame(() => {
      setPreviewScrollRatio(iframe, restoreRatio);
    });
  }, []);

  const handleEditorMount = useCallback((editor: MonacoEditor.IStandaloneCodeEditor) => {
    editorRef.current = editor;
    setEditorMountVersion((prev) => prev + 1);
  }, []);

  useEffect(() => {
    const editor = editorRef.current;
    if (!editor) return;

    const disposable = editor.onDidScrollChange(() => {
      if (syncSourceRef.current === 'preview') return;
      syncSourceRef.current = 'editor';
      setPreviewScrollRatio(previewIframeRef.current, getEditorScrollRatio(editor));
      window.requestAnimationFrame(() => {
        syncSourceRef.current = null;
      });
    });

    return () => {
      disposable.dispose();
    };
  }, [editorMountVersion]);

  useEffect(() => {
    const editor = editorRef.current;
    const domNode = editor?.getDomNode();
    if (!editor || !domNode) return;

    const handleDragOver = (event: DragEvent) => {
      const types = event.dataTransfer?.types;
      if (!types || !Array.from(types).includes(HTML_INSERT_ENDPOINT_DRAG_MIME)) return;
      event.preventDefault();
      event.dataTransfer!.dropEffect = 'copy';
    };

    const handleDrop = (event: DragEvent) => {
      const raw = event.dataTransfer?.getData(HTML_INSERT_ENDPOINT_DRAG_MIME) ?? '';
      const payload = parseHtmlInsertEndpointPayload(raw);
      if (!payload) return;

      event.preventDefault();
      event.stopPropagation();

      const dropTarget = editor.getTargetAtClientPoint(event.clientX, event.clientY);
      if (dropTarget?.position) {
        editor.setPosition(dropTarget.position);
      }

      setSelectedPayload(payload);
      const snippet = buildEndpointRowSnippet(
        payloadToEndpoint(payload),
        payload.detailLabel,
        payload.detailHref
      );

      if (insertTextAtCursor(snippet)) {
        toast.success(`${payload.path} 행 HTML을 커서 위치에 삽입했습니다.`);
      }
    };

    domNode.addEventListener('dragover', handleDragOver);
    domNode.addEventListener('drop', handleDrop);

    return () => {
      domNode.removeEventListener('dragover', handleDragOver);
      domNode.removeEventListener('drop', handleDrop);
    };
  }, [editorMountVersion, insertTextAtCursor]);

  return (
    <ResizablePanelGroup direction="horizontal" className="h-full min-h-0">
      <ResizablePanel defaultSize={54} minSize={38}>
        <div className="flex h-full min-h-0 border-r border-zinc-800">
          <div className="flex min-w-0 flex-1 flex-col">
            <div className="flex items-center justify-between gap-3 border-b border-zinc-800 bg-zinc-950 px-3 py-2">
              <div>
                <div className="text-xs font-medium text-zinc-100">Local HTML</div>
                <div className="text-[11px] text-zinc-500">
                  왼쪽 엔드포인트를 에디터로 드래그하거나, 우측 양식으로 커서에 삽입하세요.
                </div>
              </div>
              <Button
                type="button"
                size="sm"
                className="h-8 text-xs"
                onClick={handleInsertSnippet}
                disabled={!effectiveSnippet.trim()}
              >
                {insertTarget === 'cursor' ? '커서에 삽입' : 'Anchor에 삽입'}
              </Button>
            </div>
            <div className="min-h-0 flex-1">
              <Editor
                height="100%"
                defaultLanguage="html"
                theme="vs-dark"
                value={htmlContent}
                onMount={handleEditorMount}
                onChange={(value) => onHtmlChange(value ?? '')}
                options={{
                  minimap: { enabled: false },
                  wordWrap: 'on',
                  fontSize: 12,
                  scrollBeyondLastLine: false,
                  automaticLayout: true,
                  formatOnPaste: true,
                  formatOnType: true,
                }}
              />
            </div>
          </div>

          <div className="flex w-[320px] min-w-[280px] flex-shrink-0 flex-col border-l border-zinc-800 bg-zinc-900/80">
            <div className="border-b border-zinc-800 px-3 py-2">
              <div className="text-xs font-medium text-zinc-100">Insert Toolkit</div>
              <div className="mt-1 text-[11px] leading-snug text-zinc-500">
                anchor 마커를 써두면 HTML 구조가 조금 바뀌어도 같은 지점에 계속 넣을 수 있습니다.
              </div>
            </div>

            <div className="min-h-0 flex-1 space-y-4 overflow-auto p-3">
              <div className="space-y-2">
                <Label className="text-[11px] text-zinc-400">삽입 형식</Label>
                <div className="flex gap-2">
                  <Button
                    type="button"
                    size="sm"
                    variant={snippetMode === 'endpoint-row' ? 'default' : 'outline'}
                    className="h-8 flex-1 text-xs"
                    onClick={() => setSnippetMode('endpoint-row')}
                  >
                    Endpoint Row
                  </Button>
                  <Button
                    type="button"
                    size="sm"
                    variant={snippetMode === 'custom' ? 'default' : 'outline'}
                    className="h-8 flex-1 text-xs"
                    onClick={() => setSnippetMode('custom')}
                  >
                    Custom HTML
                  </Button>
                </div>
              </div>

              {snippetMode === 'endpoint-row' ? (
                <div className="space-y-3">
                  <div className="rounded-md border border-dashed border-zinc-700 bg-zinc-950/70 px-3 py-2 text-[11px] text-zinc-300">
                    {selectedPayload ? (
                      <>
                        <div className="font-medium text-zinc-100">{selectedPayload.name}</div>
                        <div className="mt-1 font-mono text-[10px] text-zinc-400">
                          {selectedPayload.method} {selectedPayload.path}
                        </div>
                      </>
                    ) : (
                      '엔드포인트를 `+`로 선택하거나 에디터로 드래그하세요.'
                    )}
                  </div>
                  <div>
                    <Label className="text-[11px] text-zinc-500">링크 텍스트</Label>
                    <Input
                      value={detailLabel}
                      onChange={(e) => setDetailLabel(e.target.value)}
                      className="mt-1 h-8 border-zinc-700 bg-zinc-950 text-xs"
                      placeholder="Details 링크 텍스트"
                    />
                  </div>
                  <div>
                    <Label className="text-[11px] text-zinc-500">링크 URL</Label>
                    <Input
                      value={detailHref}
                      onChange={(e) => setDetailHref(e.target.value)}
                      className="mt-1 h-8 border-zinc-700 bg-zinc-950 font-mono text-xs"
                      placeholder="/hc/... 또는 https://..."
                    />
                  </div>
                  <div>
                    <Label className="text-[11px] text-zinc-500">생성될 HTML</Label>
                    <Textarea
                      readOnly
                      value={endpointSnippet}
                      className="mt-1 min-h-32 border-zinc-700 bg-zinc-950 font-mono text-[11px]"
                    />
                  </div>
                </div>
              ) : (
                <div>
                  <Label className="text-[11px] text-zinc-500">Custom HTML</Label>
                  <Textarea
                    value={customHtml}
                    onChange={(e) => setCustomHtml(e.target.value)}
                    className="mt-1 min-h-44 border-zinc-700 bg-zinc-950 font-mono text-[11px]"
                    placeholder="<tr>...</tr> 또는 원하는 HTML 블록"
                  />
                </div>
              )}

              <div className="space-y-3 rounded-lg border border-zinc-800 bg-zinc-950/60 p-3">
                <div className="flex gap-2">
                  <Button
                    type="button"
                    size="sm"
                    variant={insertTarget === 'cursor' ? 'default' : 'outline'}
                    className="h-8 flex-1 text-xs"
                    onClick={() => setInsertTarget('cursor')}
                  >
                    커서 기준
                  </Button>
                  <Button
                    type="button"
                    size="sm"
                    variant={insertTarget === 'anchor' ? 'default' : 'outline'}
                    className="h-8 flex-1 text-xs"
                    onClick={() => setInsertTarget('anchor')}
                  >
                    Anchor 기준
                  </Button>
                </div>

                <div>
                  <Label className="text-[11px] text-zinc-500">Anchor 이름</Label>
                  <Input
                    value={anchorName}
                    onChange={(e) => setAnchorName(e.target.value)}
                    className="mt-1 h-8 border-zinc-700 bg-zinc-950 text-xs"
                    placeholder="section_doc_db"
                  />
                </div>

                {anchorNames.length > 0 && (
                  <div>
                    <Label className="text-[11px] text-zinc-500">기존 Anchor</Label>
                    <Select value={anchorName} onValueChange={setAnchorName}>
                      <SelectTrigger className="mt-1 border-zinc-700 bg-zinc-950 text-xs">
                        <SelectValue placeholder="기존 anchor 선택" />
                      </SelectTrigger>
                      <SelectContent>
                        {anchorNames.map((name) => (
                          <SelectItem key={name} value={name} className="text-xs">
                            {name}
                          </SelectItem>
                        ))}
                      </SelectContent>
                    </Select>
                  </div>
                )}

                <Button
                  type="button"
                  size="sm"
                  variant="outline"
                  className="h-8 w-full text-xs"
                  onClick={handleInsertAnchorMarker}
                >
                  커서에 Anchor 마커 삽입
                </Button>
              </div>
            </div>
          </div>
        </div>
      </ResizablePanel>

      <ResizableHandle withHandle />

      <ResizablePanel defaultSize={46} minSize={30}>
        <div className="flex h-full min-h-0 flex-col">
          <div className="border-b border-zinc-800 bg-zinc-950 px-3 py-2">
            <div className="text-xs font-medium text-zinc-100">Local Preview</div>
            <div className="text-[11px] text-zinc-500">
              에디터와 수직 스크롤을 동기화합니다.
            </div>
          </div>
          <iframe
            ref={previewIframeRef}
            title="manual-local-workspace-preview"
            className="min-h-0 w-full flex-1 border-0 bg-white"
            srcDoc={previewHtml}
            onLoad={handlePreviewLoad}
          />
        </div>
      </ResizablePanel>
    </ResizablePanelGroup>
  );
}
