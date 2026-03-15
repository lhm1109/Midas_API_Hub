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
import { Textarea } from '@/components/ui/textarea';
import {
  Select,
  SelectContent,
  SelectItem,
  SelectTrigger,
  SelectValue,
} from '@/components/ui/select';
import { cn } from '@/components/ui/utils';
import { apiClient } from '@/lib/api-client';
import type { ApiEndpoint } from '@/types';

const ALL_HTTP_METHODS = ['GET', 'POST', 'PUT', 'DELETE', 'PATCH'] as const;

const METHOD_ACTIVE_STYLE: Record<string, string> = {
  GET:    'bg-green-600  text-white border-green-600  hover:bg-green-700',
  POST:   'bg-blue-600   text-white border-blue-600   hover:bg-blue-700',
  PUT:    'bg-yellow-500 text-white border-yellow-500 hover:bg-yellow-600',
  DELETE: 'bg-red-600    text-white border-red-600    hover:bg-red-700',
  PATCH:  'bg-purple-600 text-white border-purple-600 hover:bg-purple-700',
};

// comma-separated string → string[]
function parseMethods(raw: string | undefined | null): string[] {
  if (!raw) return ['POST'];
  const parsed = raw.split(',').map((m) => m.trim()).filter(Boolean);
  return parsed.length > 0 ? parsed : ['POST'];
}

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
  const [methods, setMethods] = useState<string[]>(['POST']);
  const [path, setPath] = useState('');
  const [description, setDescription] = useState('');
  const [status, setStatus] = useState<string>('');
  const [statusMessage, setStatusMessage] = useState('');
  const [loading, setLoading] = useState(false);

  const isEditMode = !!endpoint;

  // 엔드포인트 ID 생성 (path 기반으로 일관되게)
  const generateEndpointId = () => {
    if (isEditMode && endpoint) {
      return endpoint.id;
    }

    // 새 엔드포인트: path에서 첫 번째 세그먼트 + 마지막 세그먼트로 ID 생성
    // 예: "/DESIGN/STEEL/KDS-41-30-2022/HASSIGN" → "design/hassign"
    // 예: "/db/node" → "db/node"
    const cleanPath = path.trim().startsWith('/') ? path.trim().slice(1) : path.trim();
    const pathSegments = cleanPath
      .split('/')
      .map((segment) => segment.trim())
      .filter((segment) => segment.length > 0)
      .map((segment) => segment.toLowerCase());

    if (pathSegments.length >= 2) {
      if (pathSegments.length === 2) {
        return `${pathSegments[0]}/${pathSegments[1]}`;
      }
      return pathSegments.join('/');
    }

    // path가 명확하지 않으면 fallback: groupName/nameSlug
    const groupName = groupId.includes('_') ? groupId.split('_')[1] : groupId;
    const nameSlug = name.toLowerCase().replace(/\s+/g, '-').replace(/[^a-z0-9-]/g, '');
    return `${groupName.toLowerCase()}/${nameSlug}`;
  };

  useEffect(() => {
    if (open) {
      if (endpoint) {
        // 수정 모드
        setName(endpoint.name);
        setMethods(parseMethods(endpoint.method));
        setPath(endpoint.path);
        setDescription('');
        setStatus(endpoint.status || '');
        setStatusMessage((endpoint as any).status_message || '');
      } else {
        // 추가 모드
        setName('');
        setMethods(['POST']);
        setPath('');
        setDescription('');
        setStatus('');
        setStatusMessage('');
      }
    }
  }, [open, endpoint]);

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault();

    if (!name.trim() || !path.trim()) {
      alert('Name and path are required.');
      return;
    }

    if (methods.length === 0) {
      alert('HTTP 메서드를 1개 이상 선택해주세요.');
      return;
    }

    setLoading(true);
    try {
      const endpointId = generateEndpointId();

      // ✅ groupId는 이제 항상 실제 groups.id (UUID 또는 legacy ID)
      // 더 이상 "productId_groupName" 형식으로 변환하지 않음
      const endpointData = {
        id: endpointId,
        name: name.trim(),
        method: methods.join(','),
        path: path.trim().startsWith('/') ? path.trim() : `/${path.trim()}`,
        product: productId,
        product_id: productId,
        group_id: groupId,  // ✅ 직접 사용 (UUID 또는 실제 ID)
        description: description.trim() || null,
        status: status || null,
        status_message: statusMessage.trim() || null,
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
            <Label>HTTP 메서드 * <span className="text-zinc-500 font-normal text-xs">(여러 개 선택 가능)</span></Label>
            <div className="flex flex-wrap gap-2">
              {ALL_HTTP_METHODS.map((m) => {
                const active = methods.includes(m);
                return (
                  <button
                    key={m}
                    type="button"
                    onClick={() => {
                      if (active) {
                        if (methods.length > 1) setMethods(methods.filter((x) => x !== m));
                      } else {
                        setMethods([...methods, m]);
                      }
                    }}
                    className={cn(
                      'px-3 py-1.5 rounded-md text-xs font-semibold border transition-all select-none',
                      active
                        ? METHOD_ACTIVE_STYLE[m]
                        : 'bg-transparent text-zinc-400 border-zinc-600 hover:border-zinc-400 hover:text-zinc-200',
                    )}
                  >
                    {m}
                  </button>
                );
              })}
            </div>
            <p className="text-xs text-zinc-500">선택됨: {methods.join(', ')}</p>
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
                <SelectItem value="success">✅ Success</SelectItem>
                <SelectItem value="error">❌ Error</SelectItem>
              </SelectContent>
            </Select>
          </div>

          {status && status !== 'none' && (
            <div className="space-y-2">
              <Label htmlFor="statusMessage">상태 메시지</Label>
              <Textarea
                id="statusMessage"
                value={statusMessage}
                onChange={(e) => setStatusMessage(e.target.value)}
                placeholder="상태에 대한 상세 설명을 입력하세요 (예: API 응답 오류, 연결 실패 등)"
                className="min-h-[60px]"
              />
            </div>
          )}

          <div className="text-xs text-zinc-500 space-y-1">
            <p>• 제품: {productId}</p>
            <p>• 그룹: {groupId.includes('_') ? groupId.split('_')[1] : groupId}</p>
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

