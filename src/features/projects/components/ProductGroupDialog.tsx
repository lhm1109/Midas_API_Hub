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
      alert('Name is required.');
      return;
    }

    setLoading(true);
    try {
      if (type === 'product') {
        // Product 추가: products 테이블에 직접 저장
        const productId = name.toLowerCase().replace(/\s+/g, '-');
        const productData = {
          id: productId,
          name: name.trim(),
          description: '',
        };

        const result = await apiClient.createProduct(productData);
        if (result.error) {
          throw new Error(result.error);
        }
        alert('✅ Product added successfully.');
      } else {
        // Group 추가: groups 테이블에 직접 저장
        if (!productId) {
          throw new Error('Product ID is required');
        }
        const groupId = `${productId}_${name.toLowerCase().replace(/\s+/g, '-')}`;
        const groupData = {
          id: groupId,
          product_id: productId,
          name: name.trim(),
          description: '',
        };

        const result = await apiClient.createGroup(groupData);
        if (result.error) {
          throw new Error(result.error);
        }
        alert('✅ Group added successfully.');
      }

      onSuccess();
      onOpenChange(false);
    } catch (error) {
      console.error('Failed to save:', error);
      alert(`❌ Save failed: ${error instanceof Error ? error.message : 'Unknown error'}`);
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

