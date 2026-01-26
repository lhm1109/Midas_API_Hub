import { useState, useEffect } from "react";
import {
  Dialog,
  DialogContent,
  DialogHeader,
  DialogTitle,
  DialogDescription,
  DialogFooter,
} from "@/app/components/ui/dialog";
import { Button } from "@/app/components/ui/button";
import { Input } from "@/app/components/ui/input";
import { Label } from "@/app/components/ui/label";
import {
  Select,
  SelectContent,
  SelectItem,
  SelectTrigger,
  SelectValue,
} from "@/app/components/ui/select";
import { ApiTask } from "@/app/data/sampleData";
import { StatusType } from "@/app/components/StatusIcon";

interface EditTaskModalProps {
  task: ApiTask | null;
  open: boolean;
  onClose: () => void;
  onSave: (task: ApiTask) => void;
}

const statusOptions: StatusType[] = [
  "empty",
  "green",
  "yellow",
  "red",
  "working",
  "done",
  "warning",
];

const statusLabels: Record<StatusType, string> = {
  empty: "시작 안함",
  green: "완료 (초록불)",
  yellow: "진행 중 (노랑불)",
  red: "문제 (빨강불)",
  working: "작업 중",
  done: "완료",
  warning: "경고",
};

export default function EditTaskModal({
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
      <DialogContent className="max-w-2xl max-h-[80vh] overflow-y-auto">
        <DialogHeader>
          <DialogTitle>작업 편집</DialogTitle>
          <DialogDescription>
            작업 항목의 상세 정보를 수정합니다.
          </DialogDescription>
        </DialogHeader>

        <div className="grid grid-cols-2 gap-4 py-4">
          <div className="space-y-2">
            <Label htmlFor="tab">Tab</Label>
            <Input
              id="tab"
              value={formData.tab}
              onChange={(e) => handleChange("tab", e.target.value)}
            />
          </div>

          <div className="space-y-2">
            <Label htmlFor="group">Group</Label>
            <Input
              id="group"
              value={formData.group}
              onChange={(e) => handleChange("group", e.target.value)}
            />
          </div>

          <div className="space-y-2">
            <Label htmlFor="sub1">sub1</Label>
            <Input
              id="sub1"
              value={formData.sub1}
              onChange={(e) => handleChange("sub1", e.target.value)}
            />
          </div>

          <div className="space-y-2">
            <Label htmlFor="sub2">sub2</Label>
            <Input
              id="sub2"
              value={formData.sub2}
              onChange={(e) => handleChange("sub2", e.target.value)}
            />
          </div>

          <div className="space-y-2">
            <Label htmlFor="sub3">sub3</Label>
            <Input
              id="sub3"
              value={formData.sub3}
              onChange={(e) => handleChange("sub3", e.target.value)}
            />
          </div>

          <div className="space-y-2">
            <Label htmlFor="seg1">seg1</Label>
            <Input
              id="seg1"
              value={formData.seg1}
              onChange={(e) => handleChange("seg1", e.target.value)}
            />
          </div>

          <div className="space-y-2">
            <Label htmlFor="seg2">seg2</Label>
            <Input
              id="seg2"
              value={formData.seg2}
              onChange={(e) => handleChange("seg2", e.target.value)}
            />
          </div>

          <div className="space-y-2">
            <Label htmlFor="endPoint">End Point</Label>
            <Input
              id="endPoint"
              value={formData.endPoint}
              onChange={(e) => handleChange("endPoint", e.target.value)}
              placeholder="예: db/pjcf"
            />
          </div>

          <div className="space-y-2">
            <Label htmlFor="dev">Dev</Label>
            <Select
              value={formData.dev as string}
              onValueChange={(value) => handleChange("dev", value as StatusType)}
            >
              <SelectTrigger>
                <SelectValue />
              </SelectTrigger>
              <SelectContent>
                {statusOptions.map((status) => (
                  <SelectItem key={status} value={status}>
                    {statusLabels[status]}
                  </SelectItem>
                ))}
              </SelectContent>
            </Select>
          </div>

          <div className="space-y-2">
            <Label htmlFor="vv">V&V</Label>
            <Select
              value={formData.vv as string}
              onValueChange={(value) => handleChange("vv", value as StatusType)}
            >
              <SelectTrigger>
                <SelectValue />
              </SelectTrigger>
              <SelectContent>
                {statusOptions.map((status) => (
                  <SelectItem key={status} value={status}>
                    {statusLabels[status]}
                  </SelectItem>
                ))}
              </SelectContent>
            </Select>
          </div>

          <div className="space-y-2">
            <Label htmlFor="doc">doc.</Label>
            <Select
              value={formData.doc as string}
              onValueChange={(value) => handleChange("doc", value as StatusType)}
            >
              <SelectTrigger>
                <SelectValue />
              </SelectTrigger>
              <SelectContent>
                {statusOptions.map((status) => (
                  <SelectItem key={status} value={status}>
                    {statusLabels[status]}
                  </SelectItem>
                ))}
              </SelectContent>
            </Select>
          </div>

          <div className="space-y-2">
            <Label htmlFor="issue">Issue</Label>
            <Select
              value={formData.issue as string}
              onValueChange={(value) => handleChange("issue", value as StatusType)}
            >
              <SelectTrigger>
                <SelectValue />
              </SelectTrigger>
              <SelectContent>
                {statusOptions.map((status) => (
                  <SelectItem key={status} value={status}>
                    {statusLabels[status]}
                  </SelectItem>
                ))}
              </SelectContent>
            </Select>
          </div>

          <div className="space-y-2">
            <Label htmlFor="status">Status</Label>
            <Input
              id="status"
              value={formData.status}
              onChange={(e) => handleChange("status", e.target.value)}
              placeholder="Working, Done 등"
            />
          </div>

          <div className="space-y-2">
            <Label htmlFor="charge">담당자</Label>
            <Input
              id="charge"
              value={formData.charge}
              onChange={(e) => handleChange("charge", e.target.value)}
            />
          </div>

          <div className="space-y-2 col-span-2">
            <Label htmlFor="remark">비고</Label>
            <Input
              id="remark"
              value={formData.remark}
              onChange={(e) => handleChange("remark", e.target.value)}
            />
          </div>
        </div>

        <DialogFooter>
          <Button variant="outline" onClick={onClose}>
            취소
          </Button>
          <Button onClick={handleSave}>저장</Button>
        </DialogFooter>
      </DialogContent>
    </Dialog>
  );
}