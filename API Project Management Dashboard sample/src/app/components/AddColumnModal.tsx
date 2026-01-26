import { useState } from "react";
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

interface AddColumnModalProps {
  open: boolean;
  onClose: () => void;
  onAdd: (columnId: string, columnLabel: string) => void;
}

export default function AddColumnModal({
  open,
  onClose,
  onAdd,
}: AddColumnModalProps) {
  const [columnId, setColumnId] = useState("");
  const [columnLabel, setColumnLabel] = useState("");

  const handleAdd = () => {
    if (columnId && columnLabel) {
      onAdd(columnId, columnLabel);
      setColumnId("");
      setColumnLabel("");
      onClose();
    }
  };

  const handleClose = () => {
    setColumnId("");
    setColumnLabel("");
    onClose();
  };

  return (
    <Dialog open={open} onOpenChange={handleClose}>
      <DialogContent>
        <DialogHeader>
          <DialogTitle>커스텀 컬럼 추가</DialogTitle>
          <DialogDescription>
            새로운 컬럼을 추가하여 테이블을 커스터마이징합니다.
          </DialogDescription>
        </DialogHeader>

        <div className="space-y-4 py-4">
          <div className="space-y-2">
            <Label htmlFor="columnId">컬럼 ID</Label>
            <Input
              id="columnId"
              value={columnId}
              onChange={(e) => setColumnId(e.target.value)}
              placeholder="예: custom1"
            />
            <p className="text-sm text-gray-500">
              영문, 숫자, 언더스코어만 사용 가능
            </p>
          </div>

          <div className="space-y-2">
            <Label htmlFor="columnLabel">컬럼 이름</Label>
            <Input
              id="columnLabel"
              value={columnLabel}
              onChange={(e) => setColumnLabel(e.target.value)}
              placeholder="예: 커스텀 필드 1"
            />
          </div>
        </div>

        <DialogFooter>
          <Button variant="outline" onClick={handleClose}>
            취소
          </Button>
          <Button onClick={handleAdd} disabled={!columnId || !columnLabel}>
            추가
          </Button>
        </DialogFooter>
      </DialogContent>
    </Dialog>
  );
}