import { useEffect, useMemo, useState } from 'react';
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
import type { ApiEndpoint, ApiGroup, ApiProduct } from '@/types';

type DuplicateEndpointDialogProps = {
  open: boolean;
  onOpenChange: (open: boolean) => void;
  endpoint: ApiEndpoint | null;
  products: ApiProduct[];
  sourceProductId: string;
  sourceGroupId: string;
  onSuccess: () => void;
};

type GroupOption = {
  id: string;
  label: string;
};

function buildCopiedPath(path: string): string {
  const trimmed = path.trim();
  if (!trimmed) return '/copy';
  const normalized = trimmed.startsWith('/') ? trimmed : `/${trimmed}`;
  return normalized.endsWith('/') ? `${normalized}copy` : `${normalized}-copy`;
}

function flattenGroups(groups: ApiGroup[], parentLabels: string[] = []): GroupOption[] {
  return groups.flatMap((group) => {
    const labels = [...parentLabels, group.name];
    return [
      {
        id: group.id,
        label: labels.join(' / '),
      },
      ...flattenGroups(group.subgroups ?? [], labels),
    ];
  });
}

export function DuplicateEndpointDialog({
  open,
  onOpenChange,
  endpoint,
  products,
  sourceProductId,
  sourceGroupId,
  onSuccess,
}: DuplicateEndpointDialogProps) {
  const [name, setName] = useState('');
  const [path, setPath] = useState('');
  const [targetProductId, setTargetProductId] = useState('');
  const [targetGroupId, setTargetGroupId] = useState('');
  const [loading, setLoading] = useState(false);

  const groupOptions = useMemo(() => {
    const product = products.find((item) => item.id === targetProductId);
    return product ? flattenGroups(product.groups ?? []) : [];
  }, [products, targetProductId]);

  useEffect(() => {
    if (!open || !endpoint) return;

    setName(`${endpoint.name} Copy`);
    setPath(buildCopiedPath(endpoint.path));
    setTargetProductId(sourceProductId);
    setTargetGroupId(sourceGroupId);
  }, [endpoint, open, sourceGroupId, sourceProductId]);

  useEffect(() => {
    if (!targetProductId) return;
    if (groupOptions.length === 0) {
      setTargetGroupId('');
      return;
    }
    if (!groupOptions.some((option) => option.id === targetGroupId)) {
      setTargetGroupId(groupOptions[0].id);
    }
  }, [groupOptions, targetGroupId, targetProductId]);

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault();

    if (!endpoint) return;
    if (!name.trim() || !path.trim()) {
      alert('Name and path are required.');
      return;
    }
    if (!targetProductId || !targetGroupId) {
      alert('Target product and group are required.');
      return;
    }

    setLoading(true);
    try {
      const result = await apiClient.duplicateEndpoint(endpoint.id, {
        name: name.trim(),
        path: path.trim(),
        product_id: targetProductId,
        group_id: targetGroupId,
      });

      if (result.error) {
        throw new Error(result.error);
      }

      alert(`✅ Endpoint duplicated: ${result.data?.endpoint?.name || name.trim()}`);
      onSuccess();
      onOpenChange(false);
    } catch (error) {
      console.error('Failed to duplicate endpoint:', error);
      alert(`❌ Duplicate failed: ${error instanceof Error ? error.message : 'Unknown error'}`);
    } finally {
      setLoading(false);
    }
  };

  return (
    <Dialog open={open} onOpenChange={onOpenChange}>
      <DialogContent className="sm:max-w-[520px]">
        <DialogHeader>
          <DialogTitle>엔드포인트 복제</DialogTitle>
          <DialogDescription>
            엔드포인트와 연결된 버전, 스펙, 빌더, 러너, 매뉴얼, 테스트케이스까지 함께 복제합니다.
          </DialogDescription>
        </DialogHeader>

        <form onSubmit={handleSubmit} className="space-y-4">
          <div className="space-y-2">
            <Label>원본</Label>
            <div className="rounded-md border border-zinc-700 bg-zinc-900 px-3 py-2 text-sm text-zinc-300">
              <div className="font-medium text-zinc-100">{endpoint?.name || '-'}</div>
              <div className="mt-1 font-mono text-xs text-zinc-400">{endpoint?.path || '-'}</div>
            </div>
          </div>

          <div className="space-y-2">
            <Label htmlFor="duplicate-name">복제 이름 *</Label>
            <Input
              id="duplicate-name"
              value={name}
              onChange={(e) => setName(e.target.value)}
              placeholder="예: Beam Design Result Copy"
              required
            />
          </div>

          <div className="space-y-2">
            <Label htmlFor="duplicate-path">복제 Endpoint Path *</Label>
            <Input
              id="duplicate-path"
              value={path}
              onChange={(e) => setPath(e.target.value)}
              placeholder="예: /post/beamdesignforces-copy"
              required
            />
          </div>

          <div className="grid gap-4 md:grid-cols-2">
            <div className="space-y-2">
              <Label>대상 Product *</Label>
              <Select value={targetProductId} onValueChange={setTargetProductId}>
                <SelectTrigger>
                  <SelectValue placeholder="Product 선택" />
                </SelectTrigger>
                <SelectContent>
                  {products.map((product) => (
                    <SelectItem key={product.id} value={product.id}>
                      {product.name}
                    </SelectItem>
                  ))}
                </SelectContent>
              </Select>
            </div>

            <div className="space-y-2">
              <Label>대상 Group *</Label>
              <Select
                value={targetGroupId}
                onValueChange={setTargetGroupId}
                disabled={!targetProductId || groupOptions.length === 0}
              >
                <SelectTrigger>
                  <SelectValue placeholder="Group 선택" />
                </SelectTrigger>
                <SelectContent>
                  {groupOptions.map((group) => (
                    <SelectItem key={group.id} value={group.id}>
                      {group.label}
                    </SelectItem>
                  ))}
                </SelectContent>
              </Select>
            </div>
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
            <Button type="submit" disabled={loading || !targetGroupId || !targetProductId}>
              {loading ? '복제 중...' : '복제'}
            </Button>
          </DialogFooter>
        </form>
      </DialogContent>
    </Dialog>
  );
}
