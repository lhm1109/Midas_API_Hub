import { useState, useEffect, useRef } from 'react';
import { Button } from '@/components/ui/button';
import { Input } from '@/components/ui/input';
import { Label } from '@/components/ui/label';
import { ScrollArea } from '@/components/ui/scroll-area';
import { GitBranch, GitCompare, Plus, Clock, Trash2, FileText, Calendar, User, FileCode, PlayCircle, Paperclip, Download, X } from 'lucide-react';
import { useAppStore } from '@/store/useAppStore';
import { Dialog, DialogContent, DialogDescription, DialogHeader, DialogTitle } from '@/components/ui/dialog';
import { Textarea } from '@/components/ui/textarea';
import { CompareVersionsDialog } from './CompareVersionsDialog';
import type { ApiEndpoint, Version, Attachment } from '@/types';
import { toast } from 'sonner';

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
    fetchVersions,
  } = useAppStore();

  const [showCreateDialog, setShowCreateDialog] = useState(false);
  const [showCompareDialog, setShowCompareDialog] = useState(false);
  const [newVersionNumber, setNewVersionNumber] = useState('');
  const [changeLog, setChangeLog] = useState('');
  const [attachments, setAttachments] = useState<Record<string, Attachment[]>>({});
  const [uploadingFiles, setUploadingFiles] = useState<Set<string>>(new Set());
  const fileInputRefs = useRef<Record<string, HTMLInputElement | null>>({});

  // 엔드포인트가 변경될 때마다 해당 엔드포인트의 버전 목록을 가져옴
  useEffect(() => {
    if (endpoint?.id) {
      fetchVersions(endpoint.id);
    }
  }, [endpoint?.id, fetchVersions]);

  // 첨부파일 목록 로드
  const loadAttachments = async (versionId: string) => {
    try {
      const response = await fetch(`http://localhost:9527/api/attachments/version/${versionId}`);
      if (response.ok) {
        const data = await response.json();
        setAttachments(prev => ({ ...prev, [versionId]: data }));
      }
    } catch (error) {
      console.error('Failed to load attachments:', error);
    }
  };

  // 파일 업로드 핸들러 - FormData 사용 (스트리밍, 큰 파일 지원)
  const handleFileUpload = async (versionId: string, event: React.ChangeEvent<HTMLInputElement>) => {
    const files = event.target.files;
    if (!files || files.length === 0) {
      console.log('❌ No file selected');
      return;
    }

    const file = files[0];
    console.log('📁 Selected file:', {
      name: file.name,
      size: file.size,
      type: file.type,
      extension: file.name.split('.').pop()
    });

    const maxSize = 100 * 1024 * 1024; // 100MB

    if (file.size > maxSize) {
      toast.error('파일 크기는 100MB를 초과할 수 없습니다.');
      return;
    }

    setUploadingFiles(prev => new Set(prev).add(versionId));
    toast.info(`${file.name} 업로드 중...`);

    try {
      // FormData 사용 - 스트리밍 방식으로 전송
      const formData = new FormData();
      formData.append('file', file);
      formData.append('versionId', versionId);

      console.log('🚀 Uploading file...');

      const response = await fetch('http://localhost:9527/api/attachments/upload', {
        method: 'POST',
        body: formData
        // Content-Type은 자동으로 설정됨 (multipart/form-data)
      });

      console.log('📡 Server response:', response.status, response.statusText);

      if (response.ok) {
        const result = await response.json();
        console.log('✅ Upload success:', result);
        const fileSizeMB = (file.size / (1024 * 1024)).toFixed(2);
        toast.success(`${file.name} (${fileSizeMB}MB) 파일이 업로드되었습니다`);
        await loadAttachments(versionId);
      } else {
        const errorText = await response.text();
        console.error('❌ Upload failed:', errorText);
        throw new Error(`Upload failed: ${response.status} ${errorText}`);
      }
    } catch (error) {
      console.error('❌ Upload error:', error);
      toast.error(`파일 업로드 실패: ${error instanceof Error ? error.message : '알 수 없는 오류'}`);
    } finally {
      setUploadingFiles(prev => {
        const next = new Set(prev);
        next.delete(versionId);
        return next;
      });
      // 파일 input 초기화
      if (fileInputRefs.current[versionId]) {
        fileInputRefs.current[versionId]!.value = '';
      }
    }
  };

  // 파일 다운로드
  const handleDownload = async (attachmentId: string, fileName: string) => {
    try {
      const response = await fetch(`http://localhost:9527/api/attachments/download/${attachmentId}`);
      if (response.ok) {
        const blob = await response.blob();
        const url = window.URL.createObjectURL(blob);
        const a = document.createElement('a');
        a.href = url;
        a.download = fileName;
        document.body.appendChild(a);
        a.click();
        document.body.removeChild(a);
        window.URL.revokeObjectURL(url);
        toast.success(`${fileName} 다운로드 완료`);
      } else {
        throw new Error('Download failed');
      }
    } catch (error) {
      console.error('Download error:', error);
      toast.error('파일 다운로드에 실패했습니다');
    }
  };

  // 파일 삭제
  const handleDeleteAttachment = async (versionId: string, attachmentId: string, fileName: string) => {
    if (!confirm(`"${fileName}" 파일을 삭제하시겠습니까?`)) return;

    try {
      const response = await fetch(`http://localhost:9527/api/attachments/${attachmentId}`, {
        method: 'DELETE'
      });

      if (response.ok) {
        toast.success(`${fileName} 파일이 삭제되었습니다`);
        await loadAttachments(versionId);
      } else {
        throw new Error('Delete failed');
      }
    } catch (error) {
      console.error('Delete error:', error);
      toast.error('파일 삭제에 실패했습니다');
    }
  };

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

  // 버전 목록이 변경되면 각 버전의 첨부파일 로드
  useEffect(() => {
    endpointVersions.forEach(version => {
      loadAttachments(version.id);
    });
  }, [endpointVersions.length]);

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
            Manage versions for <span className="text-white font-mono">{endpoint.name}</span>
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
                            Manual: {version.manualData?.htmlContent ? '✓' : '—'}
                          </div>
                          <div className="flex items-center gap-1 text-zinc-400">
                            <FileCode className="w-3 h-3" />
                            Spec: {version.specData?.jsonSchema && version.specData.jsonSchema !== '{}' ? '✓' : '—'}
                          </div>
                          <div className="flex items-center gap-1 text-zinc-400">
                            <PlayCircle className="w-3 h-3" />
                            Test Cases: {version.runnerData?.testCases?.length || 0}
                          </div>
                          <div className="flex items-center gap-1 text-zinc-400">
                            <Paperclip className="w-3 h-3" />
                            Attachments: {attachments[version.id]?.length || 0}
                          </div>
                        </div>

                        {/* Attachments Section */}
                        {attachments[version.id] && attachments[version.id].length > 0 && (
                          <div className="mt-3 p-2 bg-zinc-800/50 rounded border border-zinc-700">
                            <div className="text-xs text-zinc-400 mb-2 font-semibold">📎 Attached Files:</div>
                            <div className="space-y-1">
                              {attachments[version.id].map((attachment) => (
                                <div
                                  key={attachment.id}
                                  className="flex items-center justify-between bg-zinc-900/50 p-2 rounded text-xs group"
                                >
                                  <div className="flex items-center gap-2 flex-1 min-w-0">
                                    <FileText className="w-3 h-3 text-blue-400 flex-shrink-0" />
                                    <span className="truncate text-zinc-300">{attachment.fileName}</span>
                                    <span className="text-zinc-500 text-[10px]">
                                      ({attachment.fileSize >= 1024 * 1024 
                                        ? `${(attachment.fileSize / (1024 * 1024)).toFixed(2)} MB`
                                        : `${(attachment.fileSize / 1024).toFixed(1)} KB`
                                      })
                                    </span>
                                  </div>
                                  <div className="flex items-center gap-1 opacity-0 group-hover:opacity-100 transition-opacity">
                                    <Button
                                      size="sm"
                                      variant="ghost"
                                      onClick={() => handleDownload(attachment.id, attachment.fileName)}
                                      className="h-6 w-6 p-0 text-blue-400 hover:text-blue-300 hover:bg-blue-900/20"
                                    >
                                      <Download className="w-3 h-3" />
                                    </Button>
                                    <Button
                                      size="sm"
                                      variant="ghost"
                                      onClick={() => handleDeleteAttachment(version.id, attachment.id, attachment.fileName)}
                                      className="h-6 w-6 p-0 text-red-400 hover:text-red-300 hover:bg-red-900/20"
                                    >
                                      <X className="w-3 h-3" />
                                    </Button>
                                  </div>
                                </div>
                              ))}
                            </div>
                          </div>
                        )}

                        {/* File Upload Button */}
                        <div className="mt-3">
                          <input
                            ref={(el) => fileInputRefs.current[version.id] = el}
                            type="file"
                            onChange={(e) => handleFileUpload(version.id, e)}
                            className="hidden"
                            accept="*/*"
                          />
                          <Button
                            size="sm"
                            variant="outline"
                            onClick={() => fileInputRefs.current[version.id]?.click()}
                            disabled={uploadingFiles.has(version.id)}
                            className="h-7 text-xs border-zinc-700 hover:bg-zinc-800"
                          >
                            <Paperclip className="w-3 h-3 mr-1" />
                            {uploadingFiles.has(version.id) ? 'Uploading...' : 'Attach File'}
                          </Button>
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
            <DialogTitle className="text-zinc-100">🎯 Create New Version</DialogTitle>
            <DialogDescription className="text-zinc-400">Create a new version to track your changes</DialogDescription>
          </DialogHeader>

          <div className="space-y-4 py-4">
            <div className="space-y-2">
              <Label htmlFor="version" className="text-zinc-200">Version Number</Label>
              <Input
                id="version"
                placeholder="e.g., v1.0.0, v2.1.0"
                value={newVersionNumber}
                onChange={(e) => setNewVersionNumber(e.target.value)}
                className="bg-zinc-800 border-zinc-700 text-zinc-100 placeholder:text-zinc-500"
              />
            </div>

            <div className="space-y-2">
              <Label htmlFor="changelog" className="text-zinc-200">Change Log (Optional)</Label>
              <Textarea
                id="changelog"
                placeholder="Describe what changed in this version..."
                value={changeLog}
                onChange={(e) => setChangeLog(e.target.value)}
                className="bg-zinc-800 border-zinc-700 text-zinc-100 placeholder:text-zinc-500 min-h-[100px]"
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
              className="border-zinc-700 text-zinc-200 hover:bg-zinc-800"
            >
              Cancel
            </Button>
            <Button
              onClick={handleCreateVersion}
              disabled={!newVersionNumber.trim()}
              className="bg-blue-600 hover:bg-blue-700 text-white"
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
