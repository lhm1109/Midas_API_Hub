import { useState } from 'react';
import { Dialog, DialogContent, DialogHeader, DialogTitle, DialogDescription } from './ui/dialog';
import { Button } from './ui/button';
import { Label } from './ui/label';
import { Select, SelectContent, SelectItem, SelectTrigger, SelectValue } from './ui/select';
import { ScrollArea } from './ui/scroll-area';
import { diffLines } from 'diff';
import type { Version } from '../store/useAppStore';

interface CompareVersionsDialogProps {
  open: boolean;
  onOpenChange: (open: boolean) => void;
  versions: Version[];
}

export function CompareVersionsDialog({ open, onOpenChange, versions }: CompareVersionsDialogProps) {
  const [versionA, setVersionA] = useState<string>('');
  const [versionB, setVersionB] = useState<string>('');

  const selectedVersionA = versions.find(v => v.id === versionA);
  const selectedVersionB = versions.find(v => v.id === versionB);

  // Generate diff when both versions are selected
  const diffResult = selectedVersionA && selectedVersionB 
    ? diffLines(selectedVersionA.manualData.htmlContent || '', selectedVersionB.manualData.htmlContent || '')
    : [];

  return (
    <Dialog open={open} onOpenChange={onOpenChange}>
      <DialogContent className="bg-zinc-900 border-zinc-700 max-w-6xl max-h-[90vh] flex flex-col">
        <DialogHeader>
          <DialogTitle className="text-xl">🔍 Compare Versions - HTML Diff</DialogTitle>
          <DialogDescription className="text-sm text-zinc-500">Select two versions to compare their HTML differences.</DialogDescription>
        </DialogHeader>

        {/* Version Selectors */}
        <div className="grid grid-cols-2 gap-4 py-4 border-b border-zinc-800">
          <div className="space-y-2">
            <Label className="text-sm font-semibold text-red-400">Version A (Base)</Label>
            <Select value={versionA} onValueChange={setVersionA}>
              <SelectTrigger className="bg-zinc-800 border-zinc-700">
                <SelectValue placeholder="Select base version" />
              </SelectTrigger>
              <SelectContent>
                {versions.map((version) => (
                  <SelectItem key={version.id} value={version.id}>
                    {version.version} - {new Date(version.updatedAt).toLocaleDateString()}
                  </SelectItem>
                ))}
              </SelectContent>
            </Select>
            {selectedVersionA && (
              <div className="text-xs text-zinc-500 mt-2">
                Created: {new Date(selectedVersionA.createdAt).toLocaleString()}
                {selectedVersionA.changeLog && (
                  <div className="mt-1 text-zinc-400">📝 {selectedVersionA.changeLog}</div>
                )}
              </div>
            )}
          </div>

          <div className="space-y-2">
            <Label className="text-sm font-semibold text-green-400">Version B (Compare)</Label>
            <Select value={versionB} onValueChange={setVersionB}>
              <SelectTrigger className="bg-zinc-800 border-zinc-700">
                <SelectValue placeholder="Select compare version" />
              </SelectTrigger>
              <SelectContent>
                {versions.map((version) => (
                  <SelectItem key={version.id} value={version.id}>
                    {version.version} - {new Date(version.updatedAt).toLocaleDateString()}
                  </SelectItem>
                ))}
              </SelectContent>
            </Select>
            {selectedVersionB && (
              <div className="text-xs text-zinc-500 mt-2">
                Created: {new Date(selectedVersionB.createdAt).toLocaleString()}
                {selectedVersionB.changeLog && (
                  <div className="mt-1 text-zinc-400">📝 {selectedVersionB.changeLog}</div>
                )}
              </div>
            )}
          </div>
        </div>

        {/* Diff View */}
        {diffResult.length > 0 ? (
          <div className="flex-1 min-h-0">
            <div className="mb-3 flex items-center gap-2 text-xs">
              <div className="flex items-center gap-1">
                <div className="w-3 h-3 bg-red-900/50 border border-red-600 rounded"></div>
                <span className="text-zinc-400">Removed from Version A</span>
              </div>
              <div className="flex items-center gap-1 ml-4">
                <div className="w-3 h-3 bg-green-900/50 border border-green-600 rounded"></div>
                <span className="text-zinc-400">Added in Version B</span>
              </div>
              <div className="flex items-center gap-1 ml-4">
                <div className="w-3 h-3 bg-zinc-800 border border-zinc-700 rounded"></div>
                <span className="text-zinc-400">Unchanged</span>
              </div>
            </div>

            <ScrollArea className="h-[500px] rounded border border-zinc-800 bg-zinc-950">
              <div className="font-mono text-xs">
                {diffResult.map((part, index) => {
                  let bgColor = 'bg-zinc-950';
                  let textColor = 'text-zinc-300';
                  let borderColor = '';
                  let prefix = '  ';

                  if (part.added) {
                    bgColor = 'bg-green-900/20';
                    textColor = 'text-green-200';
                    borderColor = 'border-l-2 border-green-600';
                    prefix = '+ ';
                  } else if (part.removed) {
                    bgColor = 'bg-red-900/20';
                    textColor = 'text-red-200';
                    borderColor = 'border-l-2 border-red-600';
                    prefix = '- ';
                  }

                  const lines = part.value.split('\n');

                  return lines.map((line, lineIndex) => {
                    // Skip empty last line
                    if (lineIndex === lines.length - 1 && line === '') return null;

                    return (
                      <div
                        key={`${index}-${lineIndex}`}
                        className={`px-4 py-1 ${bgColor} ${textColor} ${borderColor} hover:bg-opacity-30 transition-colors`}
                      >
                        <span className="select-none opacity-50 mr-2">{prefix}</span>
                        <span className="whitespace-pre-wrap break-all">{line || ' '}</span>
                      </div>
                    );
                  });
                })}
              </div>
            </ScrollArea>

            {/* Summary Stats */}
            <div className="mt-3 flex items-center gap-4 text-xs text-zinc-400">
              <div>
                <span className="font-semibold text-red-400">
                  {diffResult.filter(d => d.removed).reduce((acc, d) => acc + d.count!, 0)}
                </span> lines removed
              </div>
              <div>
                <span className="font-semibold text-green-400">
                  {diffResult.filter(d => d.added).reduce((acc, d) => acc + d.count!, 0)}
                </span> lines added
              </div>
              <div>
                <span className="font-semibold text-zinc-300">
                  {diffResult.filter(d => !d.added && !d.removed).reduce((acc, d) => acc + d.count!, 0)}
                </span> lines unchanged
              </div>
            </div>
          </div>
        ) : (
          <div className="flex-1 flex items-center justify-center text-center py-12">
            <div className="text-zinc-500">
              {!versionA || !versionB ? (
                <>
                  <div className="text-4xl mb-3">📊</div>
                  <p className="text-sm">Select two versions to compare their HTML differences</p>
                </>
              ) : (
                <>
                  <div className="text-4xl mb-3">✅</div>
                  <p className="text-sm">Versions are identical - no differences found</p>
                </>
              )}
            </div>
          </div>
        )}

        {/* Actions */}
        <div className="flex justify-end gap-2 pt-4 border-t border-zinc-800">
          <Button
            variant="outline"
            onClick={() => {
              setVersionA('');
              setVersionB('');
            }}
            disabled={!versionA && !versionB}
          >
            Clear Selection
          </Button>
          <Button
            variant="outline"
            onClick={() => onOpenChange(false)}
          >
            Close
          </Button>
        </div>
      </DialogContent>
    </Dialog>
  );
}