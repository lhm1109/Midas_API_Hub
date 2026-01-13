import { useState, useEffect } from 'react';
import {
  Dialog,
  DialogContent,
  DialogDescription,
  DialogFooter,
  DialogHeader,
  DialogTitle,
} from '@/components/ui/dialog';
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
import { apiClient } from '@/lib/api-client';
import type { ApiEndpoint } from '@/types';

interface EndpointDialogProps {
  open: boolean;
  onOpenChange: (open: boolean) => void;
  endpoint?: ApiEndpoint | null;
  productId: string;
  groupId: string;
  onSuccess: () => void;
}

export function EndpointDialog({
  open,
  onOpenChange,
  endpoint,
  productId,
  groupId,
  onSuccess,
}: EndpointDialogProps) {
  const [name, setName] = useState('');
  const [method, setMethod] = useState<'GET' | 'POST' | 'PUT' | 'DELETE' | 'PATCH'>('POST');
  const [path, setPath] = useState('');
  const [description, setDescription] = useState('');
  const [status, setStatus] = useState<string>('');
  const [loading, setLoading] = useState(false);

  const isEditMode = !!endpoint;

  // 엔드포인트 ID 생성 (product/group/name 기반)
  const generateEndpointId = () => {
    if (isEditMode && endpoint) {
      return endpoint.id;
    }
    // 새 엔드포인트: groupId/name 형태로 생성
    const nameSlug = name.toLowerCase().replace(/\s+/g, '-');
    return `${groupId}/${nameSlug}`;
  };

  useEffect(() => {
    if (open) {
      if (endpoint) {
        // 수정 모드
        setName(endpoint.name);
        setMethod(endpoint.method);
        setPath(endpoint.path);
        setDescription('');
        setStatus(endpoint.status || '');
      } else {
        // 추가 모드
        setName('');
        setMethod('POST');
        setPath('');
        setDescription('');
        setStatus('');
      }
    }
  }, [open, endpoint]);

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault();
    
    if (!name.trim() || !path.trim()) {
      alert('Name and path are required.');
      return;
    }

    setLoading(true);
    try {
      const endpointId = generateEndpointId();
      const endpointData = {
        id: endpointId,
        name: name.trim(),
        method,
        path: path.trim().startsWith('/') ? path.trim() : `/${path.trim()}`,
        product: productId,
        group_name: groupId,
        description: description.trim() || null,
        status: status || null,
      };

      if (isEditMode) {
        // 수정
        const result = await apiClient.updateEndpoint(endpoint.id, endpointData);
        if (result.error) {
          throw new Error(result.error);
        }
        alert('✅ Endpoint updated successfully.');
      } else {
        // 추가
        const result = await apiClient.createEndpoint(endpointData);
        if (result.error) {
          throw new Error(result.error);
        }
        alert('✅ Endpoint created successfully.');
      }

      onSuccess();
      onOpenChange(false);
    } catch (error) {
      console.error('Failed to save endpoint:', error);
      alert(`❌ Save failed: ${error instanceof Error ? error.message : 'Unknown error'}`);
    } finally {
      setLoading(false);
    }
  };

  return (
    <Dialog open={open} onOpenChange={onOpenChange}>
      <DialogContent className="sm:max-w-[500px]">
        <DialogHeader>
          <DialogTitle>
            {isEditMode ? '엔드포인트 수정' : '엔드포인트 추가'}
          </DialogTitle>
          <DialogDescription>
            {isEditMode
              ? '엔드포인트 정보를 수정합니다.'
              : '새로운 엔드포인트를 추가합니다.'}
          </DialogDescription>
        </DialogHeader>

        <form onSubmit={handleSubmit} className="space-y-4">
          <div className="space-y-2">
            <Label htmlFor="name">이름 *</Label>
            <Input
              id="name"
              value={name}
              onChange={(e) => setName(e.target.value)}
              placeholder="예: NLCT, Node, Beam"
              required
            />
          </div>

          <div className="space-y-2">
            <Label htmlFor="method">HTTP 메서드 *</Label>
            <Select value={method} onValueChange={(value: any) => setMethod(value)}>
              <SelectTrigger id="method">
                <SelectValue />
              </SelectTrigger>
              <SelectContent>
                <SelectItem value="GET">GET</SelectItem>
                <SelectItem value="POST">POST</SelectItem>
                <SelectItem value="PUT">PUT</SelectItem>
                <SelectItem value="DELETE">DELETE</SelectItem>
                <SelectItem value="PATCH">PATCH</SelectItem>
              </SelectContent>
            </Select>
          </div>

          <div className="space-y-2">
            <Label htmlFor="path">경로 *</Label>
            <Input
              id="path"
              value={path}
              onChange={(e) => setPath(e.target.value)}
              placeholder="예: /db/nlct, /gen/project"
              required
            />
          </div>

          <div className="space-y-2">
            <Label htmlFor="description">설명</Label>
            <Input
              id="description"
              value={description}
              onChange={(e) => setDescription(e.target.value)}
              placeholder="엔드포인트 설명 (선택사항)"
            />
          </div>

          <div className="space-y-2">
            <Label htmlFor="status">상태</Label>
            <Select value={status || 'none'} onValueChange={(value) => setStatus(value === 'none' ? '' : value)}>
              <SelectTrigger id="status">
                <SelectValue placeholder="상태 선택 (선택사항)" />
              </SelectTrigger>
              <SelectContent>
                <SelectItem value="none">없음</SelectItem>
                <SelectItem value="active">Active</SelectItem>
                <SelectItem value="deprecated">Deprecated</SelectItem>
              </SelectContent>
            </Select>
          </div>

          <div className="text-xs text-zinc-500 space-y-1">
            <p>• 제품: {productId}</p>
            <p>• 그룹: {groupId}</p>
            {!isEditMode && (
              <p>• ID: {generateEndpointId()}</p>
            )}
          </div>

          <DialogFooter>
            <Button
              type="button"
              variant="outline"
              onClick={() => onOpenChange(false)}
              disabled={loading}
            >
              취소
            </Button>
            <Button type="submit" disabled={loading}>
              {loading ? '저장 중...' : isEditMode ? '수정' : '추가'}
            </Button>
          </DialogFooter>
        </form>
      </DialogContent>
    </Dialog>
  );
}

