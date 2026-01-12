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
import { apiClient } from '@/lib/api-client';

interface ProductGroupDialogProps {
  open: boolean;
  onOpenChange: (open: boolean) => void;
  type: 'product' | 'group';
  productId?: string;
  productName?: string;
  groupName?: string;
  onSuccess: () => void;
}

export function ProductGroupDialog({
  open,
  onOpenChange,
  type,
  productId,
  productName,
  groupName,
  onSuccess,
}: ProductGroupDialogProps) {
  const [name, setName] = useState('');
  const [loading, setLoading] = useState(false);

  useEffect(() => {
    if (open) {
      if (type === 'product') {
        setName(productName || '');
      } else {
        setName(groupName || '');
      }
    }
  }, [open, type, productName, groupName]);

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault();
    
    if (!name.trim()) {
      alert('이름은 필수입니다.');
      return;
    }

    setLoading(true);
    try {
      if (type === 'product') {
        // Product 추가: 더미 엔드포인트를 만들어서 Product 생성
        const dummyEndpointId = `${name.toLowerCase().replace(/\s+/g, '-')}/_placeholder`;
        const endpointData = {
          id: dummyEndpointId,
          name: '_placeholder',
          method: 'GET',
          path: `/${name.toLowerCase().replace(/\s+/g, '-')}/_placeholder`,
          product: name.trim(),
          group_name: '_placeholder',
          description: 'This is a placeholder endpoint for product creation',
          status: null,
        };

        const result = await apiClient.createEndpoint(endpointData);
        if (result.error && !result.error.includes('UNIQUE constraint')) {
          throw new Error(result.error);
        }
        alert('✅ 제품이 추가되었습니다.');
      } else {
        // Group 추가: 더미 엔드포인트를 만들어서 Group 생성
        if (!productId) {
          throw new Error('Product ID is required');
        }
        const dummyEndpointId = `${name.toLowerCase().replace(/\s+/g, '-')}/_placeholder`;
        const endpointData = {
          id: dummyEndpointId,
          name: '_placeholder',
          method: 'GET',
          path: `/${name.toLowerCase().replace(/\s+/g, '-')}/_placeholder`,
          product: productId,
          group_name: name.trim(),
          description: 'This is a placeholder endpoint for group creation',
          status: null,
        };

        const result = await apiClient.createEndpoint(endpointData);
        if (result.error && !result.error.includes('UNIQUE constraint')) {
          throw new Error(result.error);
        }
        alert('✅ 그룹이 추가되었습니다.');
      }

      onSuccess();
      onOpenChange(false);
    } catch (error) {
      console.error('Failed to save:', error);
      alert(`❌ 저장 실패: ${error instanceof Error ? error.message : 'Unknown error'}`);
    } finally {
      setLoading(false);
    }
  };

  return (
    <Dialog open={open} onOpenChange={onOpenChange}>
      <DialogContent className="sm:max-w-[400px]">
        <DialogHeader>
          <DialogTitle>
            {type === 'product' ? '제품 추가' : '그룹 추가'}
          </DialogTitle>
          <DialogDescription>
            {type === 'product'
              ? '새로운 제품을 추가합니다.'
              : '새로운 그룹을 추가합니다.'}
          </DialogDescription>
        </DialogHeader>

        <form onSubmit={handleSubmit} className="space-y-4">
          <div className="space-y-2">
            <Label htmlFor="name">이름 *</Label>
            <Input
              id="name"
              value={name}
              onChange={(e) => setName(e.target.value)}
              placeholder={type === 'product' ? '예: civil-nx, gen-nx' : '예: DB, Gen, Analysis'}
              required
            />
          </div>

          {type === 'group' && productId && (
            <div className="text-xs text-zinc-500">
              <p>• 제품: {productId}</p>
            </div>
          )}

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
              {loading ? '저장 중...' : '추가'}
            </Button>
          </DialogFooter>
        </form>
      </DialogContent>
    </Dialog>
  );
}

