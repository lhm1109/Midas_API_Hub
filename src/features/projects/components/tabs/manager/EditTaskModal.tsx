import { useState, useEffect } from 'react';
import {
  Dialog,
  DialogContent,
  DialogHeader,
  DialogTitle,
  DialogDescription,
  DialogFooter,
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
import { ApiTask, StatusType } from '../../../types/manager';

interface EditTaskModalProps {
  task: ApiTask | null;
  open: boolean;
  onClose: () => void;
  onSave: (task: ApiTask) => void;
}

const statusOptions: StatusType[] = [
  'empty',
  'green',
  'yellow',
  'red',
  'working',
  'done',
  'warning',
];

const statusLabels: Record<StatusType, string> = {
  empty: '시작 안함',
  green: '완료 (초록불)',
  yellow: '진행 중 (노랑불)',
  red: '문제 (빨강불)',
  working: '작업 중',
  done: '완료',
  warning: '경고',
};

export function EditTaskModal({
  task,
  open,
  onClose,
  onSave,
}: EditTaskModalProps) {
  const [formData, setFormData] = useState<ApiTask | null>(null);

  useEffect(() => {
    if (task) {
      setFormData({ ...task });
    }
  }, [task]);

  const handleChange = (field: string, value: string | StatusType) => {
    if (formData) {
      setFormData({ ...formData, [field]: value });
    }
  };

  const handleSave = () => {
    if (formData) {
      onSave(formData);
      onClose();
    }
  };

  if (!formData) return null;

  return (
    <Dialog open={open} onOpenChange={onClose}>
      <DialogContent className="max-w-2xl max-h-[80vh] overflow-y-auto bg-zinc-900 border-zinc-800 text-zinc-100">
        <DialogHeader>
          <DialogTitle className="text-zinc-100">작업 편집</DialogTitle>
          <DialogDescription className="text-zinc-400">
            작업 항목의 상세 정보를 수정합니다.
          </DialogDescription>
        </DialogHeader>

        <div className="grid grid-cols-2 gap-4 py-4">
          <div className="space-y-2">
            <Label htmlFor="tab" className="text-zinc-300">Tab</Label>
            <Input
              id="tab"
              value={formData.tab}
              onChange={(e) => handleChange('tab', e.target.value)}
              className="bg-zinc-950 border-zinc-700 text-zinc-100"
            />
          </div>

          <div className="space-y-2">
            <Label htmlFor="group" className="text-zinc-300">Group</Label>
            <Input
              id="group"
              value={formData.group}
              onChange={(e) => handleChange('group', e.target.value)}
              className="bg-zinc-950 border-zinc-700 text-zinc-100"
            />
          </div>

          <div className="space-y-2">
            <Label htmlFor="sub1" className="text-zinc-300">sub1</Label>
            <Input
              id="sub1"
              value={formData.sub1}
              onChange={(e) => handleChange('sub1', e.target.value)}
              className="bg-zinc-950 border-zinc-700 text-zinc-100"
            />
          </div>

          <div className="space-y-2">
            <Label htmlFor="sub2" className="text-zinc-300">sub2</Label>
            <Input
              id="sub2"
              value={formData.sub2}
              onChange={(e) => handleChange('sub2', e.target.value)}
              className="bg-zinc-950 border-zinc-700 text-zinc-100"
            />
          </div>

          <div className="space-y-2">
            <Label htmlFor="sub3" className="text-zinc-300">sub3</Label>
            <Input
              id="sub3"
              value={formData.sub3}
              onChange={(e) => handleChange('sub3', e.target.value)}
              className="bg-zinc-950 border-zinc-700 text-zinc-100"
            />
          </div>

          <div className="space-y-2">
            <Label htmlFor="seg1" className="text-zinc-300">seg1</Label>
            <Input
              id="seg1"
              value={formData.seg1}
              onChange={(e) => handleChange('seg1', e.target.value)}
              className="bg-zinc-950 border-zinc-700 text-zinc-100"
            />
          </div>

          <div className="space-y-2">
            <Label htmlFor="seg2" className="text-zinc-300">seg2</Label>
            <Input
              id="seg2"
              value={formData.seg2}
              onChange={(e) => handleChange('seg2', e.target.value)}
              className="bg-zinc-950 border-zinc-700 text-zinc-100"
            />
          </div>

          <div className="space-y-2">
            <Label htmlFor="endPoint" className="text-zinc-300">End Point</Label>
            <Input
              id="endPoint"
              value={formData.endPoint}
              onChange={(e) => handleChange('endPoint', e.target.value)}
              placeholder="예: db/pjcf"
              className="bg-zinc-950 border-zinc-700 text-zinc-100"
            />
          </div>

          <div className="space-y-2">
            <Label htmlFor="dev" className="text-zinc-300">Dev</Label>
            <Select
              value={formData.dev as string}
              onValueChange={(value) => handleChange('dev', value as StatusType)}
            >
              <SelectTrigger className="bg-zinc-950 border-zinc-700 text-zinc-100">
                <SelectValue />
              </SelectTrigger>
              <SelectContent className="bg-zinc-900 border-zinc-700">
                {statusOptions.map((status) => (
                  <SelectItem key={status} value={status} className="text-zinc-100">
                    {statusLabels[status]}
                  </SelectItem>
                ))}
              </SelectContent>
            </Select>
          </div>

          <div className="space-y-2">
            <Label htmlFor="vv" className="text-zinc-300">V&V</Label>
            <Select
              value={formData.vv as string}
              onValueChange={(value) => handleChange('vv', value as StatusType)}
            >
              <SelectTrigger className="bg-zinc-950 border-zinc-700 text-zinc-100">
                <SelectValue />
              </SelectTrigger>
              <SelectContent className="bg-zinc-900 border-zinc-700">
                {statusOptions.map((status) => (
                  <SelectItem key={status} value={status} className="text-zinc-100">
                    {statusLabels[status]}
                  </SelectItem>
                ))}
              </SelectContent>
            </Select>
          </div>

          <div className="space-y-2">
            <Label htmlFor="doc" className="text-zinc-300">doc.</Label>
            <Select
              value={formData.doc as string}
              onValueChange={(value) => handleChange('doc', value as StatusType)}
            >
              <SelectTrigger className="bg-zinc-950 border-zinc-700 text-zinc-100">
                <SelectValue />
              </SelectTrigger>
              <SelectContent className="bg-zinc-900 border-zinc-700">
                {statusOptions.map((status) => (
                  <SelectItem key={status} value={status} className="text-zinc-100">
                    {statusLabels[status]}
                  </SelectItem>
                ))}
              </SelectContent>
            </Select>
          </div>

          <div className="space-y-2">
            <Label htmlFor="issue" className="text-zinc-300">Issue</Label>
            <Select
              value={formData.issue as string}
              onValueChange={(value) => handleChange('issue', value as StatusType)}
            >
              <SelectTrigger className="bg-zinc-950 border-zinc-700 text-zinc-100">
                <SelectValue />
              </SelectTrigger>
              <SelectContent className="bg-zinc-900 border-zinc-700">
                {statusOptions.map((status) => (
                  <SelectItem key={status} value={status} className="text-zinc-100">
                    {statusLabels[status]}
                  </SelectItem>
                ))}
              </SelectContent>
            </Select>
          </div>

          <div className="space-y-2">
            <Label htmlFor="status" className="text-zinc-300">Status</Label>
            <Input
              id="status"
              value={formData.status}
              onChange={(e) => handleChange('status', e.target.value)}
              placeholder="Working, Done 등"
              className="bg-zinc-950 border-zinc-700 text-zinc-100"
            />
          </div>

          <div className="space-y-2">
            <Label htmlFor="charge" className="text-zinc-300">담당자</Label>
            <Input
              id="charge"
              value={formData.charge}
              onChange={(e) => handleChange('charge', e.target.value)}
              className="bg-zinc-950 border-zinc-700 text-zinc-100"
            />
          </div>

          <div className="space-y-2 col-span-2">
            <Label htmlFor="remark" className="text-zinc-300">비고</Label>
            <Input
              id="remark"
              value={formData.remark}
              onChange={(e) => handleChange('remark', e.target.value)}
              className="bg-zinc-950 border-zinc-700 text-zinc-100"
            />
          </div>
        </div>

        <DialogFooter>
          <Button variant="outline" onClick={onClose} className="border-zinc-700 text-zinc-300 hover:bg-zinc-800">
            취소
          </Button>
          <Button onClick={handleSave} className="bg-blue-600 hover:bg-blue-700">저장</Button>
        </DialogFooter>
      </DialogContent>
    </Dialog>
  );
}
