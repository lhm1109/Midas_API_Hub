import { useState } from 'react';
import { Button } from './ui/button';
import { Input } from './ui/input';
import { Label } from './ui/label';
import { ScrollArea } from './ui/scroll-area';
import { GitBranch, GitCompare, Plus, Clock, Trash2, FileText, Calendar, User, FileCode, PlayCircle } from 'lucide-react';
import { useAppStore, Version } from '../store/useAppStore';
import { Dialog, DialogContent, DialogDescription, DialogHeader, DialogTitle } from './ui/dialog';
import { Textarea } from './ui/textarea';
import { CompareVersionsDialog } from './CompareVersionsDialog';
import type { ApiEndpoint } from './APIListPanel';

interface VersionTabProps {
  endpoint: ApiEndpoint | null;
}

export function VersionTab({ endpoint }: VersionTabProps) {
  const {
    versions,
    currentVersionId,
    createVersion,
    loadVersion,
    deleteVersion,
    getVersionsByEndpoint,
    getCurrentVersion,
  } = useAppStore();

  const [showCreateDialog, setShowCreateDialog] = useState(false);
  const [showCompareDialog, setShowCompareDialog] = useState(false);
  const [newVersionNumber, setNewVersionNumber] = useState('');
  const [changeLog, setChangeLog] = useState('');

  if (!endpoint) {
    return (
      <div className="flex-1 flex items-center justify-center text-zinc-600">
        <div className="text-center">
          <GitBranch className="w-12 h-12 mx-auto mb-3 opacity-50" />
          <p className="text-sm">Select an endpoint to manage versions</p>
        </div>
      </div>
    );
  }

  const endpointVersions = getVersionsByEndpoint(endpoint.id).sort(
    (a, b) => new Date(b.createdAt).getTime() - new Date(a.createdAt).getTime()
  );

  const handleCreateVersion = () => {
    if (!newVersionNumber.trim()) return;
    createVersion(endpoint.id, newVersionNumber.trim(), changeLog.trim() || undefined);
    setNewVersionNumber('');
    setChangeLog('');
    setShowCreateDialog(false);
  };

  const handleLoadVersion = (versionId: string) => {
    loadVersion(versionId);
  };

  const handleDeleteVersion = (versionId: string) => {
    if (confirm('Are you sure you want to delete this version?')) {
      deleteVersion(versionId);
    }
  };

  return (
    <div className="flex-1 flex flex-col bg-zinc-950 overflow-hidden">
      {/* Header */}
      <div className="border-b border-zinc-800 bg-zinc-900 px-6 py-4 flex items-center justify-between flex-shrink-0">
        <div>
          <h2 className="text-xl font-bold flex items-center gap-2">
            <GitBranch className="w-5 h-5 text-blue-400" />
            Version Control
          </h2>
          <p className="text-sm text-zinc-500 mt-1">
            Manage versions for <span className="text-white font-mono">{endpoint.title}</span>
          </p>
        </div>
        <div className="flex gap-2">
          <Button
            onClick={() => setShowCompareDialog(true)}
            variant="outline"
            disabled={endpointVersions.length < 2}
            className="bg-zinc-800 border-zinc-700 hover:bg-zinc-700"
          >
            <GitCompare className="w-4 h-4 mr-2" />
            Compare Versions
          </Button>
          <Button
            onClick={() => setShowCreateDialog(true)}
            className="bg-blue-600 hover:bg-blue-700 text-white"
          >
            <Plus className="w-4 h-4 mr-2" />
            Create New Version
          </Button>
        </div>
      </div>

      {/* Version List */}
      <div className="flex-1 overflow-hidden p-6">
        {endpointVersions.length === 0 ? (
          <div className="h-full flex items-center justify-center text-center">
            <div className="text-zinc-600">
              <GitBranch className="w-16 h-16 mx-auto mb-4 opacity-50" />
              <h3 className="text-lg font-semibold mb-2">No Versions Yet</h3>
              <p className="text-sm mb-4">Create your first version to start working</p>
              <Button onClick={() => setShowCreateDialog(true)} className="bg-blue-600 hover:bg-blue-700">
                <Plus className="w-4 h-4 mr-2" />
                Create Version
              </Button>
            </div>
          </div>
        ) : (
          <ScrollArea className="h-full">
            <div className="space-y-3">
              {endpointVersions.map((version) => {
                const isCurrentVersion = version.id === currentVersionId;
                
                return (
                  <div
                    key={version.id}
                    className={`p-4 rounded-lg border transition-all ${
                      isCurrentVersion
                        ? 'bg-blue-900/20 border-blue-600'
                        : 'bg-zinc-900 border-zinc-800 hover:border-zinc-700'
                    }`}
                  >
                    <div className="flex items-start justify-between">
                      <div className="flex-1">
                        <div className="flex items-center gap-3 mb-2">
                          <h3 className="text-lg font-semibold text-white flex items-center gap-2">
                            <GitBranch className="w-4 h-4 text-blue-400" />
                            {version.version}
                          </h3>
                          {isCurrentVersion && (
                            <span className="text-xs px-2 py-0.5 bg-blue-600 text-white rounded-full">
                              CURRENT
                            </span>
                          )}
                        </div>

                        {version.changeLog && (
                          <p className="text-sm text-zinc-400 mb-3">📝 {version.changeLog}</p>
                        )}

                        <div className="flex items-center gap-4 text-xs text-zinc-500">
                          <div className="flex items-center gap-1">
                            <Calendar className="w-3 h-3" />
                            {new Date(version.createdAt).toLocaleString()}
                          </div>
                          <div className="flex items-center gap-1">
                            <Clock className="w-3 h-3" />
                            Updated: {new Date(version.updatedAt).toLocaleString()}
                          </div>
                        </div>

                        {/* Data Summary */}
                        <div className="mt-3 flex items-center gap-4 text-xs">
                          <div className="flex items-center gap-1 text-zinc-400">
                            <FileText className="w-3 h-3" />
                            Manual: {version.manualData.htmlContent ? '✓' : '—'}
                          </div>
                          <div className="flex items-center gap-1 text-zinc-400">
                            <FileCode className="w-3 h-3" />
                            Spec: {version.specData.jsonSchema !== '{}' ? '✓' : '—'}
                          </div>
                          <div className="flex items-center gap-1 text-zinc-400">
                            <PlayCircle className="w-3 h-3" />
                            Test Cases: {version.runnerData.testCases.length}
                          </div>
                        </div>
                      </div>

                      <div className="flex flex-col gap-2 ml-4">
                        {!isCurrentVersion && (
                          <Button
                            onClick={() => handleLoadVersion(version.id)}
                            size="sm"
                            className="bg-blue-600 hover:bg-blue-700 text-white"
                          >
                            <PlayCircle className="w-3 h-3 mr-1" />
                            Load
                          </Button>
                        )}
                        <Button
                          onClick={() => handleDeleteVersion(version.id)}
                          size="sm"
                          variant="outline"
                          className="border-red-900 text-red-400 hover:bg-red-900/20"
                        >
                          <Trash2 className="w-3 h-3 mr-1" />
                          Delete
                        </Button>
                      </div>
                    </div>
                  </div>
                );
              })}
            </div>
          </ScrollArea>
        )}
      </div>

      {/* Create Version Dialog */}
      <Dialog open={showCreateDialog} onOpenChange={setShowCreateDialog}>
        <DialogContent className="bg-zinc-900 border-zinc-700">
          <DialogHeader>
            <DialogTitle>🎯 Create New Version</DialogTitle>
            <DialogDescription>Create a new version to track your changes</DialogDescription>
          </DialogHeader>

          <div className="space-y-4 py-4">
            <div className="space-y-2">
              <Label htmlFor="version">Version Number</Label>
              <Input
                id="version"
                placeholder="e.g., v1.0.0, v2.1.0"
                value={newVersionNumber}
                onChange={(e) => setNewVersionNumber(e.target.value)}
                className="bg-zinc-800 border-zinc-700"
              />
            </div>

            <div className="space-y-2">
              <Label htmlFor="changelog">Change Log (Optional)</Label>
              <Textarea
                id="changelog"
                placeholder="Describe what changed in this version..."
                value={changeLog}
                onChange={(e) => setChangeLog(e.target.value)}
                className="bg-zinc-800 border-zinc-700 min-h-[100px]"
              />
            </div>
          </div>

          <div className="flex justify-end gap-2">
            <Button
              variant="outline"
              onClick={() => {
                setShowCreateDialog(false);
                setNewVersionNumber('');
                setChangeLog('');
              }}
            >
              Cancel
            </Button>
            <Button
              onClick={handleCreateVersion}
              disabled={!newVersionNumber.trim()}
              className="bg-blue-600 hover:bg-blue-700"
            >
              <Plus className="w-4 h-4 mr-2" />
              Create Version
            </Button>
          </div>
        </DialogContent>
      </Dialog>

      {/* Compare Versions Dialog */}
      <CompareVersionsDialog
        open={showCompareDialog}
        onOpenChange={setShowCompareDialog}
        versions={endpointVersions}
      />
    </div>
  );
}
