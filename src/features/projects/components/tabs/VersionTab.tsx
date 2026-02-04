import { useState, useEffect, useRef, useMemo } from 'react';
import { Button } from '@/components/ui/button';
import { Input } from '@/components/ui/input';
import { Label } from '@/components/ui/label';
import { ScrollArea } from '@/components/ui/scroll-area';
import { GitBranch, GitCompare, Plus, Clock, Trash2, FileText, Calendar, FileCode, PlayCircle, Paperclip, Download, Upload, X, Copy, Edit2, Check, ChevronRight } from 'lucide-react';
import { useAppStore } from '@/store/useAppStore';
import { Dialog, DialogContent, DialogDescription, DialogHeader, DialogTitle } from '@/components/ui/dialog';
import { Textarea } from '@/components/ui/textarea';
import { CompareVersionsDialog } from './CompareVersionsDialog';
import { useEndpoints } from '@/hooks/useEndpoints';
import type { ApiEndpoint, Version as _Version, Attachment, ApiGroup } from '@/types';
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
    getCurrentVersion: _getCurrentVersion,
    fetchVersions,
    acquireEndpointLock,
    releaseEndpointLock,
  } = useAppStore();

  // 🔥 products 트리에서 경로 계산
  const { endpoints: products } = useEndpoints();

  // 🔥 endpoint의 전체 경로를 계산하는 함수
  const endpointBreadcrumb = useMemo(() => {
    if (!endpoint || products.length === 0) return null;

    // 재귀적으로 그룹 탐색
    const findPath = (groups: ApiGroup[], targetId: string, path: string[]): string[] | null => {
      for (const group of groups) {
        const newPath = [...path, group.name];

        // 현재 그룹의 endpoints에서 찾기
        const foundEndpoint = group.endpoints?.find(ep => ep.id === targetId);
        if (foundEndpoint) {
          return newPath;
        }

        // 하위 그룹에서 재귀 탐색
        if (group.subgroups && group.subgroups.length > 0) {
          const result = findPath(group.subgroups, targetId, newPath);
          if (result) return result;
        }
      }
      return null;
    };

    // 모든 product에서 탐색
    for (const product of products) {
      const groupPath = findPath(product.groups, endpoint.id, []);
      if (groupPath) {
        return [product.name, ...groupPath];
      }
    }

    return null;
  }, [endpoint, products]);

  const [showCreateDialog, setShowCreateDialog] = useState(false);
  const [showCompareDialog, setShowCompareDialog] = useState(false);
  const [showLockDialog, setShowLockDialog] = useState(false);
  const [lockInfo, setLockInfo] = useState<{ locked: boolean; lockedBy?: string } | null>(null);
  const [pendingVersionId, setPendingVersionId] = useState<string | null>(null);
  const [newVersionNumber, setNewVersionNumber] = useState('');
  const [changeLog, setChangeLog] = useState('');
  const [attachments, setAttachments] = useState<Record<string, Attachment[]>>({});
  const [uploadingFiles, setUploadingFiles] = useState<Set<string>>(new Set());
  const [isCreatingVersion, setIsCreatingVersion] = useState(false);
  const [editingChangeLogId, setEditingChangeLogId] = useState<string | null>(null);
  const [editingChangeLogValue, setEditingChangeLogValue] = useState('');
  const [editingVersionTitleId, setEditingVersionTitleId] = useState<string | null>(null);
  const [editingVersionTitleValue, setEditingVersionTitleValue] = useState('');
  const fileInputRefs = useRef<Record<string, HTMLInputElement | null>>({});
  const importFileInputRef = useRef<HTMLInputElement | null>(null);

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
      toast.error('❌ File size cannot exceed 100MB');
      return;
    }

    setUploadingFiles(prev => new Set(prev).add(versionId));
    toast.info(`⏳ Uploading ${file.name}...`);

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
        toast.success(`✅ ${file.name} (${fileSizeMB}MB) uploaded successfully`);
        await loadAttachments(versionId);
      } else {
        const errorText = await response.text();
        console.error('❌ Upload failed:', errorText);
        throw new Error(`Upload failed: ${response.status} ${errorText}`);
      }
    } catch (error) {
      console.error('❌ Upload error:', error);
      toast.error(`❌ Upload failed: ${error instanceof Error ? error.message : 'Unknown error'}`);
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
        toast.success(`✅ ${fileName} downloaded successfully`);
      } else {
        throw new Error('Download failed');
      }
    } catch (error) {
      console.error('Download error:', error);
      toast.error('❌ Failed to download file');
    }
  };

  // 파일 삭제
  const handleDeleteAttachment = async (versionId: string, attachmentId: string, fileName: string) => {
    if (!confirm(`Are you sure you want to delete "${fileName}"?`)) return;

    try {
      const response = await fetch(`http://localhost:9527/api/attachments/${attachmentId}`, {
        method: 'DELETE'
      });

      if (response.ok) {
        toast.success(`✅ ${fileName} deleted successfully`);
        await loadAttachments(versionId);
      } else {
        throw new Error('Delete failed');
      }
    } catch (error) {
      console.error('Delete error:', error);
      toast.error('❌ Failed to delete file');
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

  const handleCreateVersion = async () => {
    if (!newVersionNumber.trim()) {
      toast.error('❌ Please enter a version number');
      return;
    }

    if (isCreatingVersion) {
      return; // 이미 생성 중이면 무시
    }

    setIsCreatingVersion(true);
    try {
      await createVersion(endpoint.id, newVersionNumber.trim(), changeLog.trim() || undefined);
      toast.success(`✅ Version ${newVersionNumber.trim()} created successfully`);
      setNewVersionNumber('');
      setChangeLog('');
      setShowCreateDialog(false);
    } catch (error) {
      toast.error(`❌ Failed to create version: ${error instanceof Error ? error.message : 'Unknown error'}`);
    } finally {
      setIsCreatingVersion(false);
    }
  };

  // 🔥 엔드포인트 잠금 상태 확인
  const checkEndpointLock = async (endpointId: string): Promise<{ locked: boolean; lockedBy?: string }> => {
    try {
      const response = await fetch(`http://localhost:9527/api/locks/endpoint/${encodeURIComponent(endpointId)}/lock`);
      if (response.ok) {
        const data = await response.json();
        const { currentUserId } = useAppStore.getState();
        return {
          locked: data.locked && data.lockedBy !== currentUserId,
          lockedBy: data.lockedBy,
        };
      }
      return { locked: false };
    } catch (error) {
      console.error('Failed to check lock status:', error);
      return { locked: false };
    }
  };

  const handleLoadVersion = async (versionId: string) => {
    if (!endpoint?.id) {
      toast.error('❌ No endpoint selected');
      return;
    }

    // 🔥 잠금 상태 확인
    const lockStatus = await checkEndpointLock(endpoint.id);

    if (lockStatus.locked) {
      // 잠금되어 있으면 팝업 표시
      setLockInfo(lockStatus);
      setPendingVersionId(versionId);
      setShowLockDialog(true);
      return;
    }

    // 🔒 버전 로드 전 락 획득
    const lockAcquired = await acquireEndpointLock(endpoint.id);
    if (!lockAcquired) {
      toast.error('❌ Failed to acquire lock');
      return;
    }

    // 잠금 획득 후 로드 (락은 로드 완료 후에도 유지됨 - 새로고침 또는 다른 엔드포인트 선택 시 해제)
    try {
      await loadVersion(versionId);
      const version = versions.find(v => v.id === versionId);
      toast.success(`✅ Version ${version?.version || versionId} loaded successfully`);
      toast.info('🔒 Lock acquired - will be released on refresh or endpoint change');
      // 🔄 리스트 패널 락 상태 갱신 이벤트
      window.dispatchEvent(new CustomEvent('lock-status-changed'));
    } catch (error) {
      toast.error(`❌ Failed to load version: ${error instanceof Error ? error.message : 'Unknown error'}`);
      // 로드 실패 시에만 락 해제
      await releaseEndpointLock(endpoint.id);
    }
  };

  // 🔥 잠금 팝업에서 강제 로드
  const handleForceLoad = async () => {
    if (!pendingVersionId) return;

    setShowLockDialog(false);
    try {
      await loadVersion(pendingVersionId);
      const version = versions.find(v => v.id === pendingVersionId);
      toast.success(`✅ Version ${version?.version || pendingVersionId} loaded successfully`);
      toast.warning('⚠️ This endpoint is being edited by another user. You are in read-only mode.');
    } catch (error) {
      toast.error(`❌ Failed to load version: ${error instanceof Error ? error.message : 'Unknown error'}`);
    } finally {
      setPendingVersionId(null);
      setLockInfo(null);
    }
  };

  const handleDeleteVersion = (versionId: string) => {
    if (confirm('Are you sure you want to delete this version?')) {
      deleteVersion(versionId);
      toast.success('✅ Version deleted successfully');
    }
  };

  // Change Log 편집 시작
  const handleStartEditChangeLog = (versionId: string, currentChangeLog: string) => {
    setEditingChangeLogId(versionId);
    setEditingChangeLogValue(currentChangeLog || '');
  };

  // Change Log 편집 취소
  const handleCancelEditChangeLog = () => {
    setEditingChangeLogId(null);
    setEditingChangeLogValue('');
  };

  // Change Log 저장
  const handleSaveChangeLog = async (versionId: string) => {
    try {
      const response = await fetch(`http://localhost:9527/api/versions/${versionId}`, {
        method: 'PATCH',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({
          changeLog: editingChangeLogValue.trim() || null
        })
      });

      if (!response.ok) {
        throw new Error('Failed to update change log');
      }

      await fetchVersions(endpoint!.id);
      toast.success('✅ Change log updated successfully');
      setEditingChangeLogId(null);
      setEditingChangeLogValue('');
    } catch (error) {
      toast.error(`❌ Failed to update change log: ${error instanceof Error ? error.message : 'Unknown error'}`);
    }
  };

  // 🎯 버전 제목 편집
  const handleEditVersionTitle = (versionId: string, currentTitle: string) => {
    setEditingVersionTitleId(versionId);
    setEditingVersionTitleValue(currentTitle);
  };

  const handleCancelEditVersionTitle = () => {
    setEditingVersionTitleId(null);
    setEditingVersionTitleValue('');
  };

  const handleSaveVersionTitle = async (versionId: string) => {
    if (!editingVersionTitleValue.trim()) {
      toast.error('❌ Version title cannot be empty');
      return;
    }

    try {
      const response = await fetch(`http://localhost:9527/api/versions/${versionId}`, {
        method: 'PATCH',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({
          version: editingVersionTitleValue.trim()
        })
      });

      if (!response.ok) {
        throw new Error('Failed to update version title');
      }

      await fetchVersions(endpoint!.id);
      toast.success('✅ Version title updated successfully');
      setEditingVersionTitleId(null);
      setEditingVersionTitleValue('');
    } catch (error) {
      toast.error(`❌ Failed to update version title: ${error instanceof Error ? error.message : 'Unknown error'}`);
    }
  };

  // 🎯 버전 복사
  const handleCopyVersion = async (versionId: string) => {
    try {
      const version = versions.find(v => v.id === versionId);
      if (!version || !endpoint?.id) {
        toast.error('❌ Version not found');
        return;
      }

      toast.info('⏳ Copying version...');

      // 원본 버전 데이터 가져오기
      const response = await fetch(`http://localhost:9527/api/versions/${versionId}`);
      if (!response.ok) {
        throw new Error('Failed to fetch version data');
      }
      const versionData = await response.json();

      // 복사된 버전의 새 제목 생성
      const newVersionTitle = `${version.version} (copy)`;

      // 새 버전 생성
      await createVersion(endpoint.id, newVersionTitle, version.changeLog);

      // 방금 생성된 버전 찾기 (가장 최근 버전)
      await fetchVersions(endpoint.id);
      const updatedVersions = useAppStore.getState().versions;
      const newVersion = updatedVersions
        .filter(v => v.endpointId === endpoint.id)
        .sort((a, b) => new Date(b.createdAt).getTime() - new Date(a.createdAt).getTime())[0];

      if (newVersion) {
        // 새 버전에 데이터 복사
        const updateResponse = await fetch(`http://localhost:9527/api/versions/${newVersion.id}`, {
          method: 'PATCH',
          headers: { 'Content-Type': 'application/json' },
          body: JSON.stringify({
            jsonSchema: versionData.jsonSchema,
            jsonSchemaOriginal: versionData.jsonSchemaOriginal,
            jsonSchemaEnhanced: versionData.jsonSchemaEnhanced,
            requestExample: versionData.requestExample,
            responseExample: versionData.responseExample,
            runnerData: versionData.runnerData,
          })
        });

        if (!updateResponse.ok) {
          throw new Error('Failed to copy version data');
        }
      }

      toast.success(`✅ Version copied: ${newVersionTitle}`);
    } catch (error) {
      toast.error(`❌ Failed to copy version: ${error instanceof Error ? error.message : 'Unknown error'}`);
      console.error('Failed to copy version:', error);
    }
  };

  // 🔥 Export 버전을 JSON 파일로 내보내기
  const handleExportVersion = async (versionId: string, versionName: string) => {
    try {
      toast.info('⏳ Exporting version...');

      const response = await fetch(`http://localhost:9527/api/versions/${versionId}/export`);
      if (!response.ok) {
        throw new Error('Export failed');
      }

      const exportData = await response.json();

      // JSON 파일로 다운로드
      const blob = new Blob([JSON.stringify(exportData, null, 2)], { type: 'application/json' });
      const url = URL.createObjectURL(blob);
      const a = document.createElement('a');
      a.href = url;
      a.download = `${endpoint.name}_v${versionName}_${new Date().toISOString().split('T')[0]}.json`;
      document.body.appendChild(a);
      a.click();
      document.body.removeChild(a);
      URL.revokeObjectURL(url);

      toast.success(`✅ Version exported successfully`);
    } catch (error) {
      toast.error(`❌ Export failed: ${error instanceof Error ? error.message : 'Unknown error'}`);
      console.error('Export error:', error);
    }
  };

  // 🔥 Import JSON 파일에서 버전 가져오기
  const handleImportVersion = () => {
    importFileInputRef.current?.click();
  };

  const handleImportFileSelected = async (event: React.ChangeEvent<HTMLInputElement>) => {
    const file = event.target.files?.[0];
    if (!file) return;

    try {
      toast.info('⏳ Importing version...');

      const text = await file.text();
      const importData = JSON.parse(text);

      // 서버로 전송
      const response = await fetch('http://localhost:9527/api/versions/import', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({
          endpointId: endpoint.id,
          importData,
        }),
      });

      if (!response.ok) {
        throw new Error('Import failed');
      }

      const result = await response.json();

      // 버전 목록 새로고침
      await fetchVersions(endpoint.id);

      toast.success(`✅ Version imported successfully: ${result.version.version}`);

      // 파일 입력 초기화
      if (importFileInputRef.current) {
        importFileInputRef.current.value = '';
      }
    } catch (error) {
      toast.error(`❌ Import failed: ${error instanceof Error ? error.message : 'Unknown error'}`);
      console.error('Import error:', error);
    }
  };

  return (
    <div className="flex-1 flex flex-col bg-zinc-950 overflow-hidden">
      {/* Header */}
      <div className="border-b border-zinc-800 bg-zinc-900 px-6 py-4 flex items-center justify-between flex-shrink-0">
        <div>
          <div className="flex items-center gap-3">
            <h2 className="text-xl font-bold flex items-center gap-2">
              <GitBranch className="w-5 h-5 text-blue-400" />
              Version Control
            </h2>
            {/* Path Breadcrumb */}
            <div className="text-sm text-zinc-500 flex items-center gap-1">
              <span className="text-zinc-600">|</span>
              {endpointBreadcrumb ? (
                <span className="flex items-center gap-1">
                  {endpointBreadcrumb.map((segment, idx) => (
                    <span key={idx} className="flex items-center">
                      <span className={idx === endpointBreadcrumb.length - 1 ? 'text-zinc-400' : 'text-zinc-500'}>
                        {segment}
                      </span>
                      {idx < endpointBreadcrumb.length - 1 && (
                        <ChevronRight className="w-3 h-3 text-zinc-600 mx-0.5" />
                      )}
                    </span>
                  ))}
                  <ChevronRight className="w-3 h-3 text-zinc-600 mx-0.5" />
                  <span className="text-blue-400 font-medium">{endpoint.name}</span>
                </span>
              ) : (
                <span className="font-mono text-blue-400">{endpoint.name}</span>
              )}
            </div>
          </div>
          <p className="text-sm text-zinc-500 mt-1">
            Manage versions for <span className="text-white font-mono">{endpoint.name}</span>
          </p>
        </div>
        <div className="flex gap-2">
          <Button
            onClick={handleImportVersion}
            variant="outline"
            className="bg-zinc-800 border-zinc-700 hover:bg-zinc-700"
          >
            <Upload className="w-4 h-4 mr-2" />
            Import Version
          </Button>
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
                    className={`p-4 rounded-lg border transition-all ${isCurrentVersion
                      ? 'bg-blue-900/20 border-blue-600'
                      : 'bg-zinc-900 border-zinc-800 hover:border-zinc-700'
                      }`}
                  >
                    <div className="flex items-start justify-between">
                      <div className="flex-1">
                        {/* Version Title - Editable */}
                        {editingVersionTitleId === version.id ? (
                          <div className="flex items-center gap-2 mb-2">
                            <GitBranch className="w-4 h-4 text-blue-400 flex-shrink-0 mt-1" />
                            <Input
                              value={editingVersionTitleValue}
                              onChange={(e) => setEditingVersionTitleValue(e.target.value)}
                              className="bg-zinc-800 border-zinc-700 text-white text-lg font-semibold h-8 px-2"
                              autoFocus
                              onKeyDown={(e) => {
                                if (e.key === 'Enter') handleSaveVersionTitle(version.id);
                                if (e.key === 'Escape') handleCancelEditVersionTitle();
                              }}
                            />
                            <Button
                              size="sm"
                              onClick={() => handleSaveVersionTitle(version.id)}
                              className="h-7 px-2 bg-blue-600 hover:bg-blue-700 text-white border-0"
                            >
                              <Check className="w-3 h-3" />
                            </Button>
                            <Button
                              size="sm"
                              variant="outline"
                              onClick={handleCancelEditVersionTitle}
                              className="h-7 px-2 bg-zinc-900 border-zinc-600 text-zinc-300 hover:bg-zinc-800 hover:text-white"
                            >
                              <X className="w-3 h-3" />
                            </Button>
                          </div>
                        ) : (
                          <div className="flex items-center gap-3 mb-2">
                            <h3 className="text-lg font-semibold text-white flex items-center gap-2">
                              <GitBranch className="w-4 h-4 text-blue-400" />
                              {version.version}
                            </h3>
                            <Button
                              size="sm"
                              variant="ghost"
                              onClick={() => handleEditVersionTitle(version.id, version.version)}
                              className="h-6 w-6 p-0 text-zinc-400 hover:text-blue-400 hover:bg-zinc-800"
                            >
                              <Edit2 className="w-3 h-3" />
                            </Button>
                            {isCurrentVersion && (
                              <span className="text-xs px-2 py-0.5 bg-blue-600 text-white rounded-full">
                                CURRENT
                              </span>
                            )}
                          </div>
                        )}

                        {/* Change Log - Editable */}
                        {editingChangeLogId === version.id ? (
                          <div className="mb-3 space-y-2">
                            <Textarea
                              value={editingChangeLogValue}
                              onChange={(e) => setEditingChangeLogValue(e.target.value)}
                              className="bg-zinc-800 border-zinc-700 text-zinc-100 text-sm min-h-[60px]"
                              placeholder="Describe what changed in this version..."
                              autoFocus
                            />
                            <div className="flex gap-2">
                              <Button
                                size="sm"
                                onClick={() => handleSaveChangeLog(version.id)}
                                className="h-7 text-xs bg-blue-600 hover:bg-blue-700 text-white border-0"
                              >
                                Save
                              </Button>
                              <Button
                                size="sm"
                                variant="outline"
                                onClick={handleCancelEditChangeLog}
                                className="h-7 text-xs bg-zinc-900 border-zinc-600 text-zinc-300 hover:bg-zinc-800 hover:text-white"
                              >
                                Cancel
                              </Button>
                            </div>
                          </div>
                        ) : (
                          <div
                            className="mb-3 group cursor-pointer"
                            onClick={() => handleStartEditChangeLog(version.id, version.changeLog || '')}
                          >
                            {version.changeLog ? (
                              <p className="text-sm text-zinc-400 group-hover:text-zinc-300 transition-colors">
                                📝 {version.changeLog}
                                <span className="ml-2 text-xs text-zinc-600 group-hover:text-zinc-500">✏️ Click to edit</span>
                              </p>
                            ) : (
                              <p className="text-sm text-zinc-600 group-hover:text-zinc-500 transition-colors">
                                📝 <span className="italic">No change log - Click to add</span>
                              </p>
                            )}
                          </div>
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
                        <Button
                          onClick={() => handleCopyVersion(version.id)}
                          size="sm"
                          variant="outline"
                          className="bg-zinc-900 border-cyan-600 text-cyan-300 hover:bg-cyan-950 hover:text-cyan-200 hover:border-cyan-500"
                        >
                          <Copy className="w-3 h-3 mr-1" />
                          Copy
                        </Button>
                        <Button
                          onClick={() => handleExportVersion(version.id, version.version)}
                          size="sm"
                          variant="outline"
                          className="bg-zinc-900 border-green-600 text-green-300 hover:bg-green-950 hover:text-green-200 hover:border-green-500"
                        >
                          <Download className="w-3 h-3 mr-1" />
                          Export
                        </Button>
                        {!isCurrentVersion && (
                          <Button
                            onClick={() => handleLoadVersion(version.id)}
                            size="sm"
                            className="bg-blue-600 hover:bg-blue-700 text-white border-0"
                          >
                            <PlayCircle className="w-3 h-3 mr-1" />
                            Load
                          </Button>
                        )}
                        <Button
                          onClick={() => handleDeleteVersion(version.id)}
                          size="sm"
                          variant="outline"
                          className="bg-zinc-900 border-red-600 text-red-300 hover:bg-red-950 hover:text-red-200 hover:border-red-500"
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
              disabled={!newVersionNumber.trim() || isCreatingVersion}
              className="bg-blue-600 hover:bg-blue-700 text-white disabled:opacity-50 disabled:cursor-not-allowed"
            >
              <Plus className="w-4 h-4 mr-2" />
              {isCreatingVersion ? 'Creating...' : 'Create Version'}
            </Button>
          </div>
        </DialogContent>
      </Dialog>

      {/* Lock Warning Dialog */}
      <Dialog open={showLockDialog} onOpenChange={setShowLockDialog}>
        <DialogContent className="bg-zinc-900 border-zinc-700">
          <DialogHeader>
            <DialogTitle className="text-zinc-100 flex items-center gap-2">
              <span className="text-red-500">🔒</span>
              Endpoint is Being Edited
            </DialogTitle>
            <DialogDescription className="text-zinc-400">
              This endpoint is currently being edited by another user.
            </DialogDescription>
          </DialogHeader>
          <div className="space-y-4 py-4">
            <div className="bg-red-900/20 border border-red-800/50 rounded-lg p-4">
              <p className="text-sm text-zinc-300 mb-2">
                <strong className="text-red-400">Editor:</strong> {lockInfo?.lockedBy || 'Unknown'}
              </p>
              <p className="text-xs text-zinc-400">
                If you load this version, you will be in <strong className="text-yellow-400">read-only mode</strong>.
                You won't be able to save changes while someone else is editing.
              </p>
            </div>
            <div className="flex gap-2 justify-end">
              <Button
                variant="outline"
                onClick={() => {
                  setShowLockDialog(false);
                  setPendingVersionId(null);
                  setLockInfo(null);
                }}
                className="border-zinc-700 text-zinc-300 hover:bg-zinc-800"
              >
                Cancel
              </Button>
              <Button
                onClick={handleForceLoad}
                className="bg-yellow-600 hover:bg-yellow-700 text-white"
              >
                Load Anyway (Read-only)
              </Button>
            </div>
          </div>
        </DialogContent>
      </Dialog>

      {/* Compare Versions Dialog */}
      <CompareVersionsDialog
        open={showCompareDialog}
        onOpenChange={setShowCompareDialog}
        versions={endpointVersions}
      />

      {/* Hidden Import File Input */}
      <input
        ref={importFileInputRef}
        type="file"
        accept=".json"
        onChange={handleImportFileSelected}
        className="hidden"
      />
    </div>
  );
}
