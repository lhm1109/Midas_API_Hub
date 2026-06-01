import { useEffect, useMemo, useRef, useState } from 'react';
import { AlertCircle, Braces, Copy, Loader2, Play, RotateCcw, Save } from 'lucide-react';
import { toast } from 'sonner';
import { CodeEditor } from '@/components/common';
import { Badge } from '@/components/ui/badge';
import { Button } from '@/components/ui/button';
import { Input } from '@/components/ui/input';
import {
  Select,
  SelectContent,
  SelectItem,
  SelectTrigger,
  SelectValue,
} from '@/components/ui/select';
import { apiClient, type PydanticCodeRunResponse, type PydanticResponse } from '@/lib/api-client';
import { useAppStore } from '@/store/useAppStore';
import type { ApiEndpoint, Settings } from '@/types';
import { parseRunnerJsonTable, parseRunnerResultTable, type RunnerResultTableColumn, type RunnerResultTableModel, type RunnerResultTableRow } from './runnerResultTable.logic';
import { RunnerResultTableView } from './RunnerResultTableView';
import { buildPydanticHeaders, buildPythonUsageExample, selectSpecSchema } from './pydanticTab.logic';

function isPolarsOutput(text: string): boolean {
  const trimmed = text.trimStart();
  return trimmed.startsWith('shape: (') && trimmed.includes('┌');
}

function splitPolarsRow(line: string): string[] | null {
  if (!line.startsWith('│')) return null;
  const lastPipe = line.lastIndexOf('│');
  return line.slice(1, lastPipe).split('┆').map((s) => s.trim());
}

function parsePolarsText(text: string): RunnerResultTableModel | null {
  const lines = text.split('\n').map((l) => l.trimEnd());

  const topIdx = lines.findIndex((l) => l.startsWith('┌'));
  if (topIdx === -1) return null;

  const headerCells = splitPolarsRow(lines[topIdx + 1]);
  if (!headerCells || headerCells.length === 0) return null;

  const typeCells = splitPolarsRow(lines[topIdx + 3]) ?? [];

  const sepIdx = lines.findIndex((l) => l.startsWith('╞'));
  if (sepIdx === -1) return null;

  const numericTypes = new Set(['f32', 'f64', 'i8', 'i16', 'i32', 'i64', 'u8', 'u16', 'u32', 'u64']);

  const rawRows: string[][] = [];
  for (let i = sepIdx + 1; i < lines.length; i++) {
    const line = lines[i];
    if (!line.startsWith('│')) continue;
    const cells = splitPolarsRow(line);
    if (!cells) continue;
    if (cells.every((c) => c === '…')) continue;
    if (cells[0] === '' && rawRows.length > 0) {
      const prev = rawRows[rawRows.length - 1];
      cells.forEach((c, j) => { if (c) prev[j] = prev[j] ? `${prev[j]} ${c}` : c; });
    } else {
      rawRows.push([...cells]);
    }
  }

  if (rawRows.length === 0) return null;

  const columns: RunnerResultTableColumn[] = headerCells.map((header, i) => {
    const isNum = numericTypes.has(typeCells[i] || '');
    return { key: `${header}-${i}`, header, align: isNum ? 'right' : 'left', isNumeric: isNum };
  });

  const rows: RunnerResultTableRow[] = rawRows.map((values, idx) => ({
    id: `row-${idx + 1}`,
    values,
  }));

  return { title: 'Polars DataFrame', description: `${rows.length} rows`, path: '$', meta: [], columns, rows };
}

interface PydanticTabProps {
  endpoint: ApiEndpoint;
  settings: Pick<Settings, 'baseUrl' | 'mapiKey' | 'commonHeaders'>;
  productId?: string;
}

export function PydanticTab({ endpoint, settings, productId }: PydanticTabProps) {
  const { specData, runnerData, updateRunnerData } = useAppStore();
  const [isGenerating, setIsGenerating] = useState(false);
  const [isSaving, setIsSaving] = useState(false);
  const [isRunning, setIsRunning] = useState(false);
  const [result, setResult] = useState<PydanticResponse | null>(null);
  const [executionResult, setExecutionResult] = useState<PydanticCodeRunResponse | null>(null);
  const [errorMessage, setErrorMessage] = useState<string | null>(null);
  const [statusMessage, setStatusMessage] = useState('');
  const [responseView, setResponseView] = useState<'table' | 'polars' | 'json' | 'stdout' | 'stderr'>('table');
  const [editorMode, setEditorMode] = useState<'usage' | 'library'>('usage');
  const selected = useMemo(() => selectSpecSchema(specData), [specData]);

  const endpointPath = endpoint.path || '';
  const defaultUrl = `${settings.baseUrl}${endpointPath}`;
  const endpointIdRef = useRef(endpoint.id);
  const defaultUrlRef = useRef(defaultUrl);

  const availableMethods = useMemo(
    () => endpoint.method
      ? endpoint.method.split(',').map((item) => item.trim()).filter(Boolean)
      : ['GET', 'POST', 'PUT', 'DELETE', 'PATCH'],
    [endpoint.method]
  );
  const [method, setMethod] = useState<string>(availableMethods[0] || 'POST');

  const defaultPythonCode = useMemo(
    () => buildPythonUsageExample({
      id: endpoint.id,
      method: endpoint.method,
      path: endpoint.path,
      productId,
    }, selected.schema),
    [endpoint.id, endpoint.method, endpoint.path, productId, selected.schema]
  );
  const [pythonCode, setPythonCode] = useState(defaultPythonCode);

  const urlInputValue = runnerData?.url ?? defaultUrl;

  const endpointPayload = useMemo(
    () => ({
      id: endpoint.id,
      name: endpoint.name,
      method: endpoint.method,
      path: endpoint.path,
      productId: 'verification',
    }),
    [endpoint.id, endpoint.method, endpoint.name, endpoint.path]
  );

  const stdoutText = executionResult?.stdout || '';
  const parsedStdout = executionResult?.parsedStdout ?? stdoutText;
  const isPolarsText = useMemo(() => isPolarsOutput(stdoutText), [stdoutText]);
  const parsedRealResultTable = useMemo(() => {
    if (!stdoutText) return null;
    return parseRunnerResultTable(parsedStdout);
  }, [parsedStdout, stdoutText]);
  const parsedResultTable = useMemo(() => {
    if (!stdoutText) return null;
    return parsedRealResultTable || parseRunnerJsonTable(parsedStdout, {
      title: 'Python Output',
      description: 'Pydantic Python execution output',
    });
  }, [parsedRealResultTable, parsedStdout, stdoutText]);

  useEffect(() => {
    setPythonCode(defaultPythonCode);
    setExecutionResult(null);
    setResponseView('table');
  }, [defaultPythonCode]);

  useEffect(() => {
    setMethod(availableMethods[0] || 'POST');
  }, [availableMethods]);

  useEffect(() => {
    const endpointChanged = endpointIdRef.current !== endpoint.id;
    const previousDefaultUrl = defaultUrlRef.current;
    const currentUrl = runnerData?.url;

    if (endpointChanged) {
      endpointIdRef.current = endpoint.id;
      defaultUrlRef.current = defaultUrl;
      if (currentUrl !== defaultUrl) {
        updateRunnerData({ url: defaultUrl });
      }
      return;
    }

    const shouldUseDefaultUrl =
      currentUrl === undefined ||
      currentUrl.trim() === '' ||
      currentUrl === previousDefaultUrl;

    defaultUrlRef.current = defaultUrl;

    if (shouldUseDefaultUrl && currentUrl !== defaultUrl) {
      updateRunnerData({ url: defaultUrl });
    }
  }, [defaultUrl, endpoint.id, runnerData?.url, updateRunnerData]);

  const runGenerate = async () => {
    if (!selected.schema) {
      const message = 'No JSON schema found in Spec.';
      setErrorMessage(message);
      setStatusMessage('');
      toast.error(message);
      return;
    }

    setIsGenerating(true);
    setErrorMessage(null);
    setStatusMessage('Generating Pydantic models...');
    try {
      const { data, error } = await apiClient.generatePydantic({
        schema: selected.schema,
        endpoint: endpointPayload,
      });
      if (error || !data) {
        const message = error || 'Pydantic generation failed.';
        setErrorMessage(message);
        setStatusMessage('');
        toast.error(message);
        return;
      }
      setResult(data);
      setStatusMessage(`Generated from ${selected.source}.`);
    } catch (error) {
      const message = error instanceof Error ? error.message : 'Pydantic generation failed.';
      setErrorMessage(message);
      setStatusMessage('');
      toast.error(message);
    } finally {
      setIsGenerating(false);
    }
  };

  const runSave = async () => {
    if (!selected.schema) {
      const message = 'No JSON schema found in Spec.';
      setErrorMessage(message);
      setStatusMessage('');
      toast.error(message);
      return;
    }

    setIsSaving(true);
    setErrorMessage(null);
    setStatusMessage('Saving Pydantic module...');
    try {
      const { data, error } = await apiClient.savePydantic({
        schema: selected.schema,
        endpoint: endpointPayload,
      });
      if (error || !data) {
        const message = error || 'Pydantic save failed.';
        setErrorMessage(message);
        setStatusMessage('');
        toast.error(message);
        return;
      }
      setResult(data);
      setStatusMessage(`Saved: ${data.filePath}`);
      toast.success(`Saved: ${data.filePath}`);
    } catch (error) {
      const message = error instanceof Error ? error.message : 'Pydantic save failed.';
      setErrorMessage(message);
      setStatusMessage('');
      toast.error(message);
    } finally {
      setIsSaving(false);
    }
  };

  const runPythonCode = async () => {
    const targetUrl = urlInputValue.trim();
    if (!targetUrl) {
      toast.error('Please enter a valid request URL.');
      return;
    }

    const { headers, error } = buildPydanticHeaders(settings);
    if (error) {
      toast.error(error);
      return;
    }

    setIsRunning(true);
    setErrorMessage(null);
    setStatusMessage('Running Python code...');
    try {
      const { data, error } = await apiClient.executePydanticCode({
        code: pythonCode,
        endpoint: endpointPayload,
        method,
        url: targetUrl,
        headers,
      });

      if (error || !data) {
        const message = error || 'Python execution failed.';
        setErrorMessage(message);
        setStatusMessage('');
        toast.error(message);
        return;
      }

      setExecutionResult(data);
      const realTable = parseRunnerResultTable(data.parsedStdout);
      const isPolars = isPolarsOutput(data.stdout || '');
      setResponseView(realTable || isPolars ? 'polars' : data.parsedStdout ? 'json' : data.stderr ? 'stderr' : 'stdout');
      setStatusMessage(data.ok ? 'Python code completed.' : 'Python code exited with errors.');
      if (data.ok) {
        toast.success('Python code completed.');
      } else {
        toast.error('Python code exited with errors.');
      }
    } catch (error) {
      const message = error instanceof Error ? error.message : 'Python execution failed.';
      setErrorMessage(message);
      setStatusMessage('');
      toast.error(message);
    } finally {
      setIsRunning(false);
    }
  };

  const copyActiveCode = async () => {
    await navigator.clipboard.writeText(editorMode === 'library' ? result?.code || '' : pythonCode);
    toast.success(editorMode === 'library' ? 'Library code copied.' : 'Python example copied.');
  };

  useEffect(() => {
    setResult(null);
    setErrorMessage(null);
    setStatusMessage('');
    if (!selected.schema) return;
    void runGenerate();
    // Generate whenever the selected endpoint/version schema changes.
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, [endpoint.id, selected.schema]);

  return (
    <div className="h-full w-full flex flex-col bg-zinc-950">
      <div className="flex-shrink-0 bg-zinc-900 border-b border-zinc-800 px-4 py-3">
        <div className="flex items-center justify-between gap-3">
          <div className="min-w-0">
            <div className="flex items-center gap-2">
              <Braces className="w-4 h-4 text-blue-400" />
              <h3 className="text-sm font-medium text-zinc-200">Pydantic</h3>
              <span className="text-[10px] text-zinc-500">{selected.source}</span>
            </div>
            <p className="mt-1 truncate text-xs text-zinc-500">
              {result?.filePath || 'generated_pydantic'}
            </p>
            {(errorMessage || statusMessage) && (
              <div
                className={`mt-2 flex items-center gap-1.5 text-xs ${
                  errorMessage ? 'text-red-300' : 'text-zinc-400'
                }`}
              >
                {errorMessage && <AlertCircle className="w-3.5 h-3.5 flex-shrink-0" />}
                <span className="truncate">{errorMessage || statusMessage}</span>
              </div>
            )}
          </div>

          <div className="flex items-center gap-2">
            <Button
              variant="outline"
              size="sm"
              onClick={runGenerate}
              disabled={!selected.schema || isGenerating}
              className="h-7 px-3 text-xs"
            >
              {isGenerating ? <Loader2 className="w-3.5 h-3.5 mr-2 animate-spin" /> : <Braces className="w-3.5 h-3.5 mr-2" />}
              Generate
            </Button>
            <Button
              variant="outline"
              size="sm"
              onClick={copyActiveCode}
              disabled={editorMode === 'library' && !result?.code}
              className="h-7 px-3 text-xs"
            >
              <Copy className="w-3.5 h-3.5 mr-2" />
              Copy
            </Button>
            <Button
              variant="outline"
              size="sm"
              onClick={() => setPythonCode(defaultPythonCode)}
              className="h-7 px-3 text-xs"
            >
              <RotateCcw className="w-3.5 h-3.5 mr-2" />
              Reset
            </Button>
            <Button
              variant="outline"
              size="sm"
              onClick={runSave}
              disabled={!selected.schema || isSaving}
              className="h-7 px-3 text-xs"
            >
              {isSaving ? <Loader2 className="w-3.5 h-3.5 mr-2 animate-spin" /> : <Save className="w-3.5 h-3.5 mr-2" />}
              Save .py
            </Button>
            <Button
              size="sm"
              onClick={runPythonCode}
              disabled={isRunning}
              className="h-7 px-3 text-xs bg-green-600 hover:bg-green-500 disabled:opacity-50 disabled:cursor-not-allowed"
            >
              {isRunning ? <Loader2 className="w-3.5 h-3.5 mr-2 animate-spin" /> : <Play className="w-3.5 h-3.5 mr-2" />}
              Run
            </Button>
          </div>
        </div>
      </div>

      <div className="border-b border-zinc-800 p-3 bg-zinc-900 flex-shrink-0">
        <div className="flex items-center gap-2">
          <Select value={method} onValueChange={setMethod}>
            <SelectTrigger className="w-28 h-8 bg-zinc-800 border-zinc-700 text-xs">
              <SelectValue />
            </SelectTrigger>
            <SelectContent>
              {availableMethods.map((item) => (
                <SelectItem key={item} value={item}>{item}</SelectItem>
              ))}
            </SelectContent>
          </Select>
          <Input
            value={urlInputValue}
            onChange={(event) => updateRunnerData({ url: event.target.value })}
            className="h-8 flex-1 bg-zinc-800 border-zinc-700 text-xs font-mono"
          />
          {executionResult && (
            <Badge
              variant="outline"
              className={executionResult.ok ? 'border-emerald-700/70 bg-emerald-950/50 text-emerald-300' : 'border-red-700/70 bg-red-950/50 text-red-300'}
            >
              Python {executionResult.exitCode === null ? executionResult.signal : `exit ${executionResult.exitCode}`} / {executionResult.time}ms
            </Badge>
          )}
        </div>
      </div>

      <div className="flex-1 min-h-0 flex">
        <div className="w-[54%] min-w-[420px] flex flex-col border-r border-zinc-800">
          <div className="px-4 py-2 bg-zinc-900 border-b border-zinc-800 flex items-center justify-between">
            <span className="text-sm text-zinc-400">
              {editorMode === 'library' ? 'Generated library code' : 'Python library usage'}
            </span>
            <div className="flex items-center gap-2">
              <Button
                variant={editorMode === 'usage' ? 'default' : 'outline'}
                size="sm"
                onClick={() => setEditorMode('usage')}
                className="h-7 px-3 text-xs"
              >
                Usage Example
              </Button>
              <Button
                variant={editorMode === 'library' ? 'default' : 'outline'}
                size="sm"
                onClick={() => setEditorMode('library')}
                disabled={!result?.code}
                className="h-7 px-3 text-xs"
              >
                Library Code
              </Button>
            </div>
          </div>
          <div className="flex-1 min-h-0">
            <CodeEditor
              key={editorMode}
              value={editorMode === 'library' ? result?.code || '' : pythonCode}
              onChange={(value) => {
                if (editorMode === 'usage') setPythonCode(value || '');
              }}
              language="python"
              minimap={true}
              readOnly={editorMode === 'library'}
            />
          </div>
        </div>

        <div className="flex-1 min-w-0 flex flex-col">
          <div className="px-4 py-2 bg-zinc-900 border-b border-zinc-800 flex items-center justify-between">
            <div className="flex items-center gap-3">
              <span className="text-sm text-zinc-400">Execution Result</span>
              {parsedResultTable && (
                <span className="text-[11px] text-zinc-500">
                  {parsedResultTable.rows.length} rows / {parsedResultTable.columns.length} columns
                </span>
              )}
            </div>
            <div className="flex items-center gap-2">
              <Button
                variant={responseView === 'table' ? 'default' : 'outline'}
                size="sm"
                onClick={() => setResponseView('table')}
                disabled={!parsedResultTable}
                className="h-7 px-3 text-xs"
              >
                Table
              </Button>
              <Button
                variant={responseView === 'polars' ? 'default' : 'outline'}
                size="sm"
                onClick={() => setResponseView('polars')}
                disabled={!parsedRealResultTable && !isPolarsText}
                className="h-7 px-3 text-xs"
              >
                Polars
              </Button>
              <Button
                variant={responseView === 'json' ? 'default' : 'outline'}
                size="sm"
                onClick={() => setResponseView('json')}
                disabled={!executionResult?.parsedStdout}
                className="h-7 px-3 text-xs"
              >
                JSON
              </Button>
              <Button
                variant={responseView === 'stdout' ? 'default' : 'outline'}
                size="sm"
                onClick={() => setResponseView('stdout')}
                disabled={!executionResult?.stdout}
                className="h-7 px-3 text-xs"
              >
                Terminal
              </Button>
              <Button
                variant={responseView === 'stderr' ? 'default' : 'outline'}
                size="sm"
                onClick={() => setResponseView('stderr')}
                disabled={!executionResult?.stderr}
                className="h-7 px-3 text-xs"
              >
                stderr
              </Button>
            </div>
          </div>

          <div className="flex-1 min-h-0 overflow-auto">
            {responseView === 'table' && parsedResultTable ? (
              <RunnerResultTableView model={parsedResultTable} />
            ) : responseView === 'polars' && (parsedRealResultTable || isPolarsText) ? (
              (() => {
                const model = parsedRealResultTable ?? (isPolarsText ? parsePolarsText(stdoutText) : null);
                return model ? <RunnerResultTableView model={model} /> : null;
              })()

            ) : responseView === 'json' && executionResult?.parsedStdout ? (
              <CodeEditor
                value={typeof executionResult.parsedStdout === 'string'
                  ? executionResult.parsedStdout
                  : JSON.stringify(executionResult.parsedStdout, null, 2)}
                onChange={() => {}}
                language="json"
                readOnly={true}
                minimap={false}
              />
            ) : responseView === 'stdout' && executionResult?.stdout ? (
              <div className="h-full flex flex-col bg-[#0c0c0c] font-mono">
                <div className="flex-shrink-0 flex items-center gap-2 px-3 py-2 bg-[#1a1a1a] border-b border-zinc-800">
                  <span className="w-3 h-3 rounded-full bg-red-500/60 inline-block" />
                  <span className="w-3 h-3 rounded-full bg-yellow-500/60 inline-block" />
                  <span className="w-3 h-3 rounded-full bg-green-500/60 inline-block" />
                  <span className="text-zinc-500 text-[11px] ml-2">python</span>
                  {executionResult && (
                    <span className={`ml-auto text-[11px] ${executionResult.ok ? 'text-emerald-400' : 'text-red-400'}`}>
                      exit {executionResult.exitCode ?? executionResult.signal} &middot; {executionResult.time}ms
                    </span>
                  )}
                </div>
                <pre className="flex-1 overflow-auto p-4 text-[13px] leading-relaxed text-zinc-200 whitespace-pre">
                  {executionResult.stdout}
                </pre>
              </div>
            ) : responseView === 'stderr' && executionResult?.stderr ? (
              <div className="h-full flex flex-col bg-[#0c0c0c] font-mono">
                <div className="flex-shrink-0 flex items-center gap-2 px-3 py-2 bg-[#1a1a1a] border-b border-zinc-800">
                  <span className="w-3 h-3 rounded-full bg-red-500/60 inline-block" />
                  <span className="w-3 h-3 rounded-full bg-yellow-500/60 inline-block" />
                  <span className="w-3 h-3 rounded-full bg-green-500/60 inline-block" />
                  <span className="text-zinc-500 text-[11px] ml-2">python · stderr</span>
                  {executionResult && (
                    <span className="ml-auto text-[11px] text-red-400">
                      exit {executionResult.exitCode ?? executionResult.signal} &middot; {executionResult.time}ms
                    </span>
                  )}
                </div>
                <pre className="flex-1 overflow-auto p-4 text-[13px] leading-relaxed text-red-300 whitespace-pre">
                  {executionResult.stderr}
                </pre>
              </div>
            ) : (
              <div className="p-4 text-zinc-600 text-sm">
                Run the Python usage code to execute the generated_pydantic library on the backend.
              </div>
            )}
          </div>
        </div>
      </div>
    </div>
  );
}
