import { useState } from 'react';
import { Button } from './ui/button';
import { Input } from './ui/input';
import { Label } from './ui/label';
import { GitBranch, GitCompare, Plus, Clock, Trash2 } from 'lucide-react';
import { useAppStore, ManualVersion } from '../store/useAppStore';
import { Dialog, DialogContent, DialogDescription, DialogHeader, DialogTitle } from './ui/dialog';
import { Textarea } from './ui/textarea';
import { Select, SelectContent, SelectItem, SelectTrigger, SelectValue } from './ui/select';
import type { ManualData } from '../store/useAppStore';

interface VersionManagerProps {
  endpointId: string;
  currentManualData: ManualData | null;
  currentHTMLContent: string;
}

export function VersionManager({ endpointId, currentManualData, currentHTMLContent }: VersionManagerProps) {
  const {
    manualVersions,
    selectedVersionId,
    saveManualVersion,
    deleteManualVersion,
    getVersionsByEndpoint,
    getLatestVersion,
    selectVersion,
  } = useAppStore();

  const [showSaveDialog, setShowSaveDialog] = useState(false);
  const [showCompareDialog, setShowCompareDialog] = useState(false);
  const [newVersion, setNewVersion] = useState('');
  const [changeLog, setChangeLog] = useState('');
  const [compareVersionA, setCompareVersionA] = useState<string>('');
  const [compareVersionB, setCompareVersionB] = useState<string>('');

  const versions = getVersionsByEndpoint(endpointId);
  const latestVersion = getLatestVersion(endpointId);
  const selectedVersion = versions.find(v => v.id === selectedVersionId);

  const handleSaveVersion = () => {
    if (!newVersion.trim() || !currentManualData) {
      alert('Please enter a version number and ensure manual data exists.');
      return;
    }

    saveManualVersion(
      endpointId,
      newVersion,
      currentManualData,
      currentHTMLContent,
      changeLog
    );

    setNewVersion('');
    setChangeLog('');
    setShowSaveDialog(false);
    alert(`✅ Version ${newVersion} saved successfully!`);
  };

  const handleCompareVersions = () => {
    if (!compareVersionA || !compareVersionB) {
      alert('Please select two versions to compare.');
      return;
    }

    const versionA = versions.find(v => v.id === compareVersionA);
    const versionB = versions.find(v => v.id === compareVersionB);

    if (!versionA || !versionB) return;

    // Open comparison view (to be implemented)
    alert(`Comparing ${versionA.version} with ${versionB.version}`);
  };

  return (
    <>
      {/* Version Manager Bar */}
      <div className="p-3 border-b border-zinc-800 bg-zinc-900/80 flex-shrink-0">
        <div className="flex items-center gap-3">
          {/* Version Selector */}
          <div className="flex items-center gap-2 flex-1">
            <GitBranch className="w-4 h-4 text-zinc-500" />
            <Label className="text-xs text-zinc-400 whitespace-nowrap">Version:</Label>
            <Select value={selectedVersionId || 'draft'} onValueChange={(value) => selectVersion(value === 'draft' ? null : value)}>
              <SelectTrigger className="h-8 text-xs bg-zinc-800 border-zinc-700 w-[180px]">
                <SelectValue placeholder="Current Draft" />
              </SelectTrigger>
              <SelectContent>
                <SelectItem value="draft">
                  <div className="flex items-center gap-2">
                    <Clock className="w-3 h-3" />
                    <span>Current Draft</span>
                  </div>
                </SelectItem>
                {versions.map((version) => (
                  <SelectItem key={version.id} value={version.id}>
                    <div className="flex items-center gap-2">
                      <GitBranch className="w-3 h-3" />
                      <span>{version.version}</span>
                      {version.id === latestVersion?.id && (
                        <span className="text-[10px] px-1.5 py-0.5 bg-green-600 text-white rounded">LATEST</span>
                      )}
                    </div>
                  </SelectItem>
                ))}
              </SelectContent>
            </Select>

            {selectedVersion && (
              <div className="text-xs text-zinc-500 ml-2">
                {new Date(selectedVersion.updatedAt).toLocaleDateString()}
              </div>
            )}
          </div>

          <div className="h-6 w-px bg-zinc-700" />

          {/* Actions */}
          <Button
            size="sm"
            variant="outline"
            onClick={() => setShowSaveDialog(true)}
            className="h-8 text-xs"
            disabled={!currentManualData}
          >
            <Plus className="w-3 h-3 mr-1" />
            Save as Version
          </Button>

          <Button
            size="sm"
            variant="outline"
            onClick={() => setShowCompareDialog(true)}
            className="h-8 text-xs"
            disabled={versions.length < 2}
          >
            <GitCompare className="w-3 h-3 mr-1" />
            Compare
          </Button>
        </div>
      </div>

      {/* Save Version Dialog */}
      <Dialog open={showSaveDialog} onOpenChange={setShowSaveDialog}>
        <DialogContent className="bg-zinc-900 border-zinc-700 max-w-md">
          <DialogHeader>
            <DialogTitle className="text-xl">💾 Save as New Version</DialogTitle>
            <DialogDescription className="text-zinc-400">
              Create a new version of this manual for version control and comparison.
            </DialogDescription>
          </DialogHeader>

          <div className="space-y-4 py-4">
            <div className="space-y-2">
              <Label htmlFor="version-number" className="text-sm font-semibold">
                Version Number *
              </Label>
              <Input
                id="version-number"
                value={newVersion}
                onChange={(e) => setNewVersion(e.target.value)}
                placeholder="e.g., v1.0.0, v1.1.0, v2.0.0"
                className="bg-zinc-800 border-zinc-700"
              />
              <p className="text-xs text-zinc-500">
                Suggested: {latestVersion ? `v${incrementVersion(latestVersion.version)}` : 'v1.0.0'}
              </p>
            </div>

            <div className="space-y-2">
              <Label htmlFor="changelog" className="text-sm font-semibold">
                Change Log <span className="text-zinc-500 font-normal">(optional)</span>
              </Label>
              <Textarea
                id="changelog"
                value={changeLog}
                onChange={(e) => setChangeLog(e.target.value)}
                placeholder="e.g., Updated JSON schema, fixed examples, added new specifications"
                className="bg-zinc-800 border-zinc-700 min-h-[80px]"
              />
            </div>
          </div>

          <div className="flex justify-end gap-2">
            <Button
              variant="outline"
              onClick={() => setShowSaveDialog(false)}
            >
              Cancel
            </Button>
            <Button
              onClick={handleSaveVersion}
              disabled={!newVersion.trim()}
              className="bg-blue-600 hover:bg-blue-500"
            >
              Save Version
            </Button>
          </div>
        </DialogContent>
      </Dialog>

      {/* Compare Versions Dialog */}
      <Dialog open={showCompareDialog} onOpenChange={setShowCompareDialog}>
        <DialogContent className="bg-zinc-900 border-zinc-700 max-w-2xl">
          <DialogHeader>
            <DialogTitle className="text-xl">🔍 Compare Versions</DialogTitle>
            <DialogDescription className="text-zinc-400">
              Select two versions to compare their differences.
            </DialogDescription>
          </DialogHeader>

          <div className="space-y-4 py-4">
            <div className="grid grid-cols-2 gap-4">
              <div className="space-y-2">
                <Label className="text-sm font-semibold">Version A</Label>
                <Select value={compareVersionA} onValueChange={setCompareVersionA}>
                  <SelectTrigger className="bg-zinc-800 border-zinc-700">
                    <SelectValue placeholder="Select version" />
                  </SelectTrigger>
                  <SelectContent>
                    {versions.map((version) => (
                      <SelectItem key={version.id} value={version.id}>
                        {version.version} - {new Date(version.updatedAt).toLocaleDateString()}
                      </SelectItem>
                    ))}
                  </SelectContent>
                </Select>
              </div>

              <div className="space-y-2">
                <Label className="text-sm font-semibold">Version B</Label>
                <Select value={compareVersionB} onValueChange={setCompareVersionB}>
                  <SelectTrigger className="bg-zinc-800 border-zinc-700">
                    <SelectValue placeholder="Select version" />
                  </SelectTrigger>
                  <SelectContent>
                    {versions.map((version) => (
                      <SelectItem key={version.id} value={version.id}>
                        {version.version} - {new Date(version.updatedAt).toLocaleDateString()}
                      </SelectItem>
                    ))}
                  </SelectContent>
                </Select>
              </div>
            </div>

            {compareVersionA && compareVersionB && (
              <div className="p-4 bg-zinc-800 rounded-lg border border-zinc-700">
                <h4 className="text-sm font-semibold mb-2">Comparison Preview</h4>
                <div className="text-xs text-zinc-400 space-y-1">
                  <p>• Version A: {versions.find(v => v.id === compareVersionA)?.version}</p>
                  <p>• Version B: {versions.find(v => v.id === compareVersionB)?.version}</p>
                  <p className="text-orange-400 mt-2">⚠️ Diff view coming soon...</p>
                </div>
              </div>
            )}
          </div>

          <div className="flex justify-end gap-2">
            <Button
              variant="outline"
              onClick={() => setShowCompareDialog(false)}
            >
              Cancel
            </Button>
            <Button
              onClick={handleCompareVersions}
              disabled={!compareVersionA || !compareVersionB}
              className="bg-blue-600 hover:bg-blue-500"
            >
              <GitCompare className="w-4 h-4 mr-2" />
              Compare
            </Button>
          </div>
        </DialogContent>
      </Dialog>
    </>
  );
}

// Helper function to increment version
function incrementVersion(version: string): string {
  const match = version.match(/v?(\d+)\.(\d+)\.(\d+)/);
  if (!match) return '1.0.1';
  
  const [, major, minor, patch] = match;
  return `${major}.${minor}.${parseInt(patch) + 1}`;
}