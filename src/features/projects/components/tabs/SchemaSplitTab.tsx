import { useEffect, useMemo, useState } from 'react';
import { CodeEditor } from '@/components/common';
import { Button } from '@/components/ui/button';
import { AlertCircle, SplitSquareHorizontal } from 'lucide-react';
import { toast } from 'sonner';
import { useAppStore } from '@/store/useAppStore';
import { apiClient } from '@/lib/api-client';
import type { ApiEndpoint } from '@/types';
import {
  computeSplitFromParsed,
  type SplitResult,
  type SplitSchemaSlice,
} from './schemaSplit.logic';

interface SchemaSplitTabProps {
  endpoint: ApiEndpoint;
}

export function SchemaSplitTab({ endpoint }: SchemaSplitTabProps) {
  const { updateSpecData } = useAppStore();
  const [sourceSchema, setSourceSchema] = useState('');
  const [splitResult, setSplitResult] = useState<SplitResult | null>(null);
  const [isApiRunning, setIsApiRunning] = useState(false);
  const [error, setError] = useState<string | null>(null);
  const [activeRequestSliceId, setActiveRequestSliceId] = useState('full');
  const [activeResponseSliceId, setActiveResponseSliceId] = useState('full');
  const storageKey = useMemo(() => `schemaSplit:${endpoint.id}`, [endpoint.id]);
  const roundtripKey = useMemo(() => `schemaRoundtrip:${endpoint.id}`, [endpoint.id]);

  useEffect(() => {
    try {
      const raw = localStorage.getItem(storageKey);
      if (!raw) return;
      const parsed = JSON.parse(raw);
      if (typeof parsed?.sourceSchema === 'string') {
        setSourceSchema(parsed.sourceSchema);
        try {
          const sourceParsed = JSON.parse(parsed.sourceSchema || '{}');
          const recomputed = computeSplitFromParsed(sourceParsed);
          setSplitResult(recomputed);
          setError(null);
          return;
        } catch {
          // ignore and fallback to cached splitResult
        }
      }
      if (parsed?.splitResult) {
        setSplitResult(parsed.splitResult as SplitResult);
      }
    } catch {
      // ignore cache errors
    }
  }, [storageKey]);

  useEffect(() => {
    try {
      localStorage.setItem(
        storageKey,
        JSON.stringify({
          sourceSchema,
          splitResult,
        })
      );
    } catch {
      // ignore cache errors
    }
  }, [storageKey, sourceSchema, splitResult]);

  useEffect(() => {
    if (!splitResult) return;

    const requestSlices = splitResult.requestSlices || [];
    const responseSlices = splitResult.responseSlices || [];

    if (!requestSlices.some((slice) => slice.id === activeRequestSliceId)) {
      setActiveRequestSliceId(requestSlices[0]?.id || 'full');
    }
    if (!responseSlices.some((slice) => slice.id === activeResponseSliceId)) {
      setActiveResponseSliceId(responseSlices[0]?.id || 'full');
    }
  }, [splitResult, activeRequestSliceId, activeResponseSliceId]);

  const handleSplit = () => {
    setError(null);
    setSplitResult(null);

    let parsed: any;
    try {
      parsed = JSON.parse(sourceSchema || '{}');
    } catch {
      setError('Invalid JSON. Please fix the syntax errors first.');
      return;
    }

    try {
      const result = computeSplitFromParsed(parsed);
      setSplitResult(result);
    } catch (err) {
      setError(err instanceof Error ? err.message : String(err));
    }
  };

  const handleApplyToSpec = () => {
    if (!sourceSchema) {
      toast.error('No source schema available. Please paste the schema first.');
      return;
    }

    let parsed: any;
    try {
      parsed = JSON.parse(sourceSchema || '{}');
    } catch {
      toast.error('Invalid JSON. Please fix the syntax errors first.');
      return;
    }

    let result: SplitResult;
    try {
      result = computeSplitFromParsed(parsed);
    } catch (err) {
      toast.error(err instanceof Error ? err.message : String(err));
      return;
    }

    setSplitResult(result);

    updateSpecData({
      jsonSchemaEnhanced: JSON.stringify({
        request: result.requestSchema,
        response: result.responseSchema,
        requestKey: result.requestKey,
        responseKey: result.responseKey,
      }),
    });

    toast.success('✅ Request/Response schemas saved to Enhanced Spec');
  };

  const handleApiRoundtrip = async () => {
    if (!sourceSchema) {
      toast.error('No source schema available. Please paste the schema first.');
      return;
    }

    let parsed: any;
    try {
      parsed = JSON.parse(sourceSchema || '{}');
    } catch {
      toast.error('Invalid JSON. Please fix the syntax errors first.');
      return;
    }

    setIsApiRunning(true);
    try {
      const { data, error: apiError } = await apiClient.roundtripSchema({
        sourceSchema: parsed,
        autoFixAgainstSource: true,
      });
      if (apiError || !data) {
        toast.error(apiError || 'Schema roundtrip API failed.');
        return;
      }

      const result = computeSplitFromParsed(parsed);
      setSplitResult(result);
      updateSpecData({
        jsonSchemaEnhanced: JSON.stringify({
          request: result.requestSchema,
          response: result.responseSchema,
          requestKey: result.requestKey,
          responseKey: result.responseKey,
        }),
      });

      try {
        localStorage.setItem(
          roundtripKey,
          JSON.stringify({
            sourceSchema: parsed,
            ...data,
            ranAt: new Date().toISOString(),
          })
        );
      } catch {
        // ignore cache errors
      }

      if (data.matchesSource) {
        toast.success('API roundtrip passed. Enhanced Request/Response and Merged output are consistent.');
      } else {
        const diffSummary = [
          `missing:${data.comparison?.missingInMerged?.length || 0}`,
          `extra:${data.comparison?.extraInMerged?.length || 0}`,
          `changed:${data.comparison?.changedSchemas?.length || 0}`,
        ].join(', ');
        toast.warning(`API roundtrip found mismatch (${diffSummary}). Auto-fix snapshot stored.`);
      }
    } finally {
      setIsApiRunning(false);
    }
  };

  const previewRequest = useMemo(() => {
    if (!splitResult) return '{}';
    const slice = (splitResult.requestSlices || []).find((item) => item.id === activeRequestSliceId);
    return JSON.stringify(slice?.schema || splitResult.requestSchema, null, 2);
  }, [splitResult, activeRequestSliceId]);

  const previewResponse = useMemo(() => {
    if (!splitResult) return '{}';
    const slice = (splitResult.responseSlices || []).find((item) => item.id === activeResponseSliceId);
    return JSON.stringify(slice?.schema || splitResult.responseSchema, null, 2);
  }, [splitResult, activeResponseSliceId]);

  const activeRequestSlice = useMemo(
    () => (splitResult?.requestSlices || []).find((slice) => slice.id === activeRequestSliceId),
    [splitResult, activeRequestSliceId]
  );
  const activeResponseSlice = useMemo(
    () => (splitResult?.responseSlices || []).find((slice) => slice.id === activeResponseSliceId),
    [splitResult, activeResponseSliceId]
  );

  const renderSliceSelector = (
    slices: SplitSchemaSlice[] | undefined,
    activeSliceId: string,
    onSelect: (sliceId: string) => void
  ) => {
    if (!slices || slices.length <= 1) return null;

    return (
      <div className="px-4 py-2 border-b border-zinc-800 bg-zinc-950/70">
        <div className="flex items-center justify-between gap-2">
          <p className="text-[10px] text-zinc-500">
            Multi-ref split preview
          </p>
          <p className="text-[10px] text-zinc-600">
            {slices.filter((slice) => slice.kind === 'ref').length} refs, {slices.filter((slice) => slice.kind === 'inline').length} rules
          </p>
        </div>
        <div className="mt-2 flex gap-1 overflow-x-auto pb-1">
          {slices.map((slice) => {
            const isActive = slice.id === activeSliceId;
            return (
              <button
                key={slice.id}
                type="button"
                onClick={() => onSelect(slice.id)}
                title={slice.subtitle || slice.path}
                className={[
                  'flex-shrink-0 rounded-md border px-2.5 py-1 text-[10px] transition-colors',
                  isActive
                    ? 'border-blue-500 bg-blue-500/15 text-blue-200'
                    : 'border-zinc-700 bg-zinc-900 text-zinc-400 hover:border-zinc-500 hover:text-zinc-200',
                ].join(' ')}
              >
                {slice.label}
              </button>
            );
          })}
        </div>
      </div>
    );
  };

  return (
    <div className="h-full w-full flex flex-col bg-zinc-950">
      <div className="flex-shrink-0 bg-zinc-900 border-b border-zinc-800 px-4 py-3">
        <div className="flex items-center justify-between">
          <div className="flex items-center gap-2">
            <SplitSquareHorizontal className="w-4 h-4 text-blue-400" />
            <h3 className="text-sm font-medium text-zinc-200">Schema Split</h3>
          </div>
          <div className="flex items-center gap-2">
            <Button
              variant="outline"
              size="sm"
              onClick={handleSplit}
              className="h-7 px-3 text-xs"
            >
              Split
            </Button>
            <Button
              size="sm"
              onClick={handleApplyToSpec}
              disabled={!splitResult}
              className="h-7 px-3 text-xs bg-blue-600 hover:bg-blue-500 disabled:opacity-50 disabled:cursor-not-allowed"
            >
              Apply to Spec (Enhanced)
            </Button>
            <Button
              variant="outline"
              size="sm"
              onClick={handleApiRoundtrip}
              disabled={!sourceSchema || isApiRunning}
              className="h-7 px-3 text-xs"
            >
              {isApiRunning ? 'API Roundtrip...' : 'API Roundtrip + AutoFix'}
            </Button>
          </div>
        </div>
        <p className="mt-2 text-xs text-zinc-500">
          Rule: request/response schemas are matched by key containing <code className="font-mono text-zinc-300">_REQUEST</code> /
          <code className="font-mono text-zinc-300">_RESPONSE</code> (token). Prefix/suffix can vary.
        </p>
      </div>

      {error && (
        <div className="flex-shrink-0 bg-red-900/20 border-b border-red-700/40 px-4 py-2 flex items-center gap-2">
          <AlertCircle className="w-4 h-4 text-red-400" />
          <pre className="text-xs text-red-300 whitespace-pre-wrap">{error}</pre>
        </div>
      )}

      <div className="flex-1 grid grid-cols-2 gap-0 overflow-hidden">
        <div className="flex flex-col border-r border-zinc-800">
          <div className="px-4 py-2 border-b border-zinc-800 bg-zinc-900">
            <h4 className="text-sm text-zinc-200">Source Schema</h4>
            <p className="text-xs text-zinc-500">Paste the combined schema with components.schemas</p>
          </div>
          <div className="flex-1 min-h-0">
            <CodeEditor
              value={sourceSchema}
              onChange={(value) => setSourceSchema(value || '')}
              language="json"
              minimap={true}
            />
          </div>
        </div>

        <div className="flex flex-col">
          <div className="grid grid-rows-2 h-full">
            <div className="flex flex-col border-b border-zinc-800">
              <div className="px-4 py-2 border-b border-zinc-800 bg-zinc-900 flex items-center justify-between">
                <div>
                  <h4 className="text-sm text-zinc-200">Request Schema</h4>
                  <p className="text-[10px] text-zinc-500">{splitResult?.requestKey || '-'}</p>
                </div>
              </div>
              {renderSliceSelector(splitResult?.requestSlices, activeRequestSliceId, setActiveRequestSliceId)}
              {activeRequestSlice && (
                <div className="px-4 py-2 border-b border-zinc-800 bg-zinc-950/50">
                  <p className="text-[10px] text-zinc-400">{activeRequestSlice.subtitle || activeRequestSlice.path}</p>
                </div>
              )}
              <div className="flex-1 min-h-0">
                <CodeEditor
                  value={previewRequest}
                  onChange={() => {}}
                  language="json"
                  minimap={true}
                  readOnly={true}
                />
              </div>
            </div>

            <div className="flex flex-col">
              <div className="px-4 py-2 border-b border-zinc-800 bg-zinc-900 flex items-center justify-between">
                <div>
                  <h4 className="text-sm text-zinc-200">Response Schema</h4>
                  <p className="text-[10px] text-zinc-500">{splitResult?.responseKey || '-'}</p>
                </div>
              </div>
              {renderSliceSelector(splitResult?.responseSlices, activeResponseSliceId, setActiveResponseSliceId)}
              {activeResponseSlice && (
                <div className="px-4 py-2 border-b border-zinc-800 bg-zinc-950/50">
                  <p className="text-[10px] text-zinc-400">{activeResponseSlice.subtitle || activeResponseSlice.path}</p>
                </div>
              )}
              <div className="flex-1 min-h-0">
                <CodeEditor
                  value={previewResponse}
                  onChange={() => {}}
                  language="json"
                  minimap={true}
                  readOnly={true}
                />
              </div>
            </div>
          </div>
        </div>
      </div>
    </div>
  );
}
