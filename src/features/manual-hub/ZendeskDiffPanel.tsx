import { GitCompare, ArrowUpToLine } from 'lucide-react';
import { Button } from '@/components/ui/button';
import { ScrollArea } from '@/components/ui/scroll-area';

export type ZendeskDiffPanelProps = {
  localHtml: string;
  remoteHtml: string;
  remoteTitle: string | null;
  remoteUpdatedAt: string | null;
  onUseLocal: () => void;
  onUseRemote: () => void;
};

function stripHtmlTags(html: string): string {
  return html
    .replace(/<script[\s\S]*?<\/script>/gi, '')
    .replace(/<style[\s\S]*?<\/style>/gi, '')
    .replace(/<[^>]+>/g, '')
    .replace(/&nbsp;/gi, ' ')
    .replace(/&lt;/gi, '<')
    .replace(/&gt;/gi, '>')
    .replace(/&amp;/gi, '&')
    .replace(/&quot;/gi, '"')
    .replace(/&#039;/gi, "'")
    .replace(/\r\n/g, '\n')
    .replace(/\r/g, '\n')
    .replace(/\n{3,}/g, '\n\n')
    .trim();
}

function computeDiffLines(
  oldText: string,
  newText: string
): Array<{ type: 'same' | 'removed' | 'added'; text: string }> {
  const oldLines = oldText.split('\n');
  const newLines = newText.split('\n');

  const m = oldLines.length;
  const n = newLines.length;

  const LIMIT = 1000;
  const oldTrunc = oldLines.slice(0, LIMIT);
  const newTrunc = newLines.slice(0, LIMIT);

  const dp: number[][] = Array.from({ length: oldTrunc.length + 1 }, () =>
    new Array(newTrunc.length + 1).fill(0)
  );

  for (let i = oldTrunc.length - 1; i >= 0; i--) {
    for (let j = newTrunc.length - 1; j >= 0; j--) {
      if (oldTrunc[i] === newTrunc[j]) {
        dp[i][j] = dp[i + 1][j + 1] + 1;
      } else {
        dp[i][j] = Math.max(dp[i + 1][j], dp[i][j + 1]);
      }
    }
  }

  const result: Array<{ type: 'same' | 'removed' | 'added'; text: string }> = [];
  let i = 0;
  let j = 0;

  while (i < oldTrunc.length || j < newTrunc.length) {
    if (i < oldTrunc.length && j < newTrunc.length && oldTrunc[i] === newTrunc[j]) {
      result.push({ type: 'same', text: oldTrunc[i] });
      i++;
      j++;
    } else if (j < newTrunc.length && (i >= oldTrunc.length || dp[i + 1]?.[j] <= dp[i]?.[j + 1])) {
      result.push({ type: 'added', text: newTrunc[j] });
      j++;
    } else {
      result.push({ type: 'removed', text: oldTrunc[i] });
      i++;
    }
  }

  if (m > LIMIT || n > LIMIT) {
    result.push({ type: 'added', text: `... (truncated, ${m} vs ${n} total lines)` });
  }

  return result;
}

export function ZendeskDiffPanel({
  localHtml,
  remoteHtml,
  remoteTitle,
  remoteUpdatedAt,
  onUseLocal,
  onUseRemote,
}: ZendeskDiffPanelProps) {
  const localText = stripHtmlTags(localHtml);
  const remoteText = stripHtmlTags(remoteHtml);
  const diffLines = computeDiffLines(remoteText, localText);

  const addedCount = diffLines.filter((l) => l.type === 'added').length;
  const removedCount = diffLines.filter((l) => l.type === 'removed').length;
  const isIdentical = addedCount === 0 && removedCount === 0;

  return (
    <div className="h-full flex flex-col bg-zinc-950 overflow-hidden">
      <div className="flex-shrink-0 px-4 py-3 border-b border-zinc-800 bg-zinc-900 flex flex-wrap items-center justify-between gap-3">
        <div className="flex items-center gap-3 min-w-0">
          <GitCompare className="w-4 h-4 text-violet-400 flex-shrink-0" />
          <span className="text-sm font-medium text-white">Zendesk vs Local</span>
          {remoteTitle && (
            <span className="text-xs text-zinc-400 truncate max-w-[200px]" title={remoteTitle}>
              {remoteTitle}
            </span>
          )}
          {remoteUpdatedAt && (
            <span className="text-xs text-zinc-500 whitespace-nowrap">
              Zendesk: {new Date(remoteUpdatedAt).toLocaleString()}
            </span>
          )}
        </div>
        <div className="flex items-center gap-2 flex-wrap">
          {isIdentical ? (
            <span className="text-xs text-emerald-400 bg-emerald-900/30 border border-emerald-600/40 px-2 py-1 rounded">
              No text diff
            </span>
          ) : (
            <span className="text-xs text-zinc-300 bg-zinc-800 border border-zinc-700 px-2 py-1 rounded">
              <span className="text-emerald-400">+{addedCount}</span> <span className="text-red-400">-{removedCount}</span> lines
            </span>
          )}
          <Button
            size="sm"
            variant="outline"
            onClick={onUseRemote}
            className="h-7 px-2 text-xs border-amber-600/50 text-amber-400 hover:bg-amber-900/20"
            title="Replace local HTML with Zendesk body"
          >
            <ArrowUpToLine className="w-3 h-3 mr-1 rotate-180" />
            Zendesk to Local
          </Button>
          <Button size="sm" onClick={onUseLocal} className="h-7 px-2 text-xs bg-blue-600 hover:bg-blue-500" title="Send local HTML to Zendesk">
            <ArrowUpToLine className="w-3 h-3 mr-1" />
            Local to Zendesk
          </Button>
        </div>
      </div>

      <div className="flex-shrink-0 grid grid-cols-2 border-b border-zinc-800 bg-zinc-900/60 text-xs">
        <div className="px-4 py-1.5 text-zinc-400 border-r border-zinc-800">
          <span className="text-red-400 font-medium">Zendesk (remote)</span>
        </div>
        <div className="px-4 py-1.5 text-zinc-400">
          <span className="text-emerald-400 font-medium">Local (editable)</span>
        </div>
      </div>

      <div className="flex-1 overflow-hidden grid grid-cols-2 divide-x divide-zinc-800">
        <ScrollArea className="h-full">
          <div className="font-mono text-xs leading-relaxed p-2 min-w-0">
            {diffLines.map((line, idx) => (
              <div
                key={idx}
                className={
                  line.type === 'removed'
                    ? 'bg-red-900/30 text-red-300 px-2 py-px whitespace-pre-wrap break-all'
                    : line.type === 'added'
                      ? 'text-zinc-700 px-2 py-px select-none'
                      : 'text-zinc-400 px-2 py-px whitespace-pre-wrap break-all'
                }
              >
                {line.type === 'removed' ? `- ${line.text}` : line.type === 'added' ? '\u00a0' : `  ${line.text}`}
              </div>
            ))}
          </div>
        </ScrollArea>
        <ScrollArea className="h-full">
          <div className="font-mono text-xs leading-relaxed p-2 min-w-0">
            {diffLines.map((line, idx) => (
              <div
                key={idx}
                className={
                  line.type === 'added'
                    ? 'bg-emerald-900/30 text-emerald-300 px-2 py-px whitespace-pre-wrap break-all'
                    : line.type === 'removed'
                      ? 'text-zinc-700 px-2 py-px select-none'
                      : 'text-zinc-400 px-2 py-px whitespace-pre-wrap break-all'
                }
              >
                {line.type === 'added' ? `+ ${line.text}` : line.type === 'removed' ? '\u00a0' : `  ${line.text}`}
              </div>
            ))}
          </div>
        </ScrollArea>
      </div>
    </div>
  );
}
