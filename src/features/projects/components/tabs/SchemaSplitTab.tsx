import { useEffect, useMemo, useState } from 'react';
import { CodeEditor } from '@/components/common';
import { Button } from '@/components/ui/button';
import { AlertCircle, SplitSquareHorizontal } from 'lucide-react';
import { toast } from 'sonner';
import { useAppStore } from '@/store/useAppStore';
import type { ApiEndpoint } from '@/types';

interface SchemaSplitTabProps {
  endpoint: ApiEndpoint;
}

type SplitResult = {
  requestKey: string;
  responseKey: string;
  requestSchema: any;
  responseSchema: any;
};

const deepClone = <T,>(value: T): T => {
  try {
    return JSON.parse(JSON.stringify(value));
  } catch {
    return value;
  }
};

const normalizeSchema = (root: any, components: Record<string, any>) => {
  const clonedRoot = deepClone(root);
  const clonedComponents = deepClone(components);
  return {
    ...clonedRoot,
    components: {
      schemas: clonedComponents,
    },
  };
};

export function SchemaSplitTab({ endpoint }: SchemaSplitTabProps) {
  const { updateSpecData } = useAppStore();
  const [sourceSchema, setSourceSchema] = useState('');
  const [splitResult, setSplitResult] = useState<SplitResult | null>(null);
  const [error, setError] = useState<string | null>(null);
  const storageKey = useMemo(() => `schemaSplit:${endpoint.id}`, [endpoint.id]);

  const computeSplitFromParsed = (parsed: any): SplitResult => {
    const schemas = parsed?.components?.schemas;
    if (!schemas || typeof schemas !== 'object') {
      throw new Error('components.schemas not found. Please provide a schema with components.schemas.');
    }

    const keys = Object.keys(schemas);
    const requestRegex = /_REQUEST(_|$)/;
    const responseRegex = /_RESPONSE(_|$)/;

    const requestKeys = keys.filter((key) => requestRegex.test(key));
    const responseKeys = keys.filter((key) => responseRegex.test(key));

    if (requestKeys.length !== 1 || responseKeys.length !== 1) {
      throw new Error(
        `Request/Response key must be unique.\nFound REQUEST: ${requestKeys.join(', ') || 'none'}\nFound RESPONSE: ${responseKeys.join(', ') || 'none'}`
      );
    }

    const requestKey = requestKeys[0];
    const responseKey = responseKeys[0];
    const requestSchema = normalizeSchema(schemas[requestKey], schemas);
    const responseSchema = normalizeSchema(schemas[responseKey], schemas);

    if (requestSchema && typeof requestSchema === 'object') {
      (requestSchema as any)['x-origin-name'] = requestKey;
    }
    if (responseSchema && typeof responseSchema === 'object') {
      (responseSchema as any)['x-origin-name'] = responseKey;
    }

    return { requestKey, responseKey, requestSchema, responseSchema };
  };

  useEffect(() => {
    try {
      const raw = localStorage.getItem(storageKey);
      if (!raw) return;
      const parsed = JSON.parse(raw);
      if (typeof parsed?.sourceSchema === 'string') {
        setSourceSchema(parsed.sourceSchema);
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
      }),
    });

    toast.success('✅ Request/Response schemas saved to Enhanced Spec');
  };

  const previewRequest = useMemo(() => {
    if (!splitResult) return '{}';
    return JSON.stringify(splitResult.requestSchema, null, 2);
  }, [splitResult]);

  const previewResponse = useMemo(() => {
    if (!splitResult) return '{}';
    return JSON.stringify(splitResult.responseSchema, null, 2);
  }, [splitResult]);

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
