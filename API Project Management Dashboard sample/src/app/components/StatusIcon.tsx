import { Circle, CheckCircle2, AlertTriangle, Loader2 } from "lucide-react";
import { cn } from "@/app/components/ui/utils";

export type StatusType =
  | "green" // 초록불 - 완료
  | "yellow" // 노랑불 - 진행 중
  | "red" // 빨강불 - 문제
  | "empty" // 비어있는원 - 시작 안함
  | "working" // Working 스핀 - 작업 중
  | "done" // Done 체크 - 완료
  | "warning"; // 주의판 - 경고

interface StatusIconProps {
  status: StatusType;
  className?: string;
}

export function StatusIcon({ status, className }: StatusIconProps) {
  switch (status) {
    case "green":
      return (
        <div
          className={cn(
            "w-6 h-6 rounded-full bg-green-500 flex items-center justify-center",
            className
          )}
          title="완료"
        >
          <div className="w-3 h-3 rounded-full bg-white" />
        </div>
      );
    case "yellow":
      return (
        <div
          className={cn(
            "w-6 h-6 rounded-full bg-yellow-500 flex items-center justify-center",
            className
          )}
          title="진행 중"
        >
          <div className="w-3 h-3 rounded-full bg-white" />
        </div>
      );
    case "red":
      return (
        <div
          className={cn(
            "w-6 h-6 rounded-full bg-red-500 flex items-center justify-center",
            className
          )}
          title="문제"
        >
          <div className="w-3 h-3 rounded-full bg-white" />
        </div>
      );
    case "empty":
      return (
        <Circle
          className={cn("w-6 h-6 text-gray-300", className)}
          strokeWidth={2}
          title="시작 안함"
        />
      );
    case "working":
      return (
        <Loader2
          className={cn("w-6 h-6 text-blue-500 animate-spin", className)}
          title="작업 중"
        />
      );
    case "done":
      return (
        <CheckCircle2
          className={cn("w-6 h-6 text-green-600", className)}
          title="완료"
        />
      );
    case "warning":
      return (
        <AlertTriangle
          className={cn("w-6 h-6 text-orange-500", className)}
          title="경고"
        />
      );
    default:
      return null;
  }
}

// 상태 텍스트 뱃지
interface StatusBadgeProps {
  status: "Working" | "Done" | string;
  className?: string;
}

export function StatusBadge({ status, className }: StatusBadgeProps) {
  if (status === "Working") {
    return (
      <div
        className={cn(
          "inline-flex items-center gap-2 px-3 py-1 rounded-full bg-blue-100 text-blue-700 text-sm",
          className
        )}
      >
        <Loader2 className="w-4 h-4 animate-spin" />
        Working
      </div>
    );
  }
  
  if (status === "Done") {
    return (
      <div
        className={cn(
          "inline-flex items-center gap-2 px-3 py-1 rounded-full bg-green-100 text-green-700 text-sm",
          className
        )}
      >
        <CheckCircle2 className="w-4 h-4" />
        Done
      </div>
    );
  }

  return (
    <div className={cn("px-3 py-1 rounded-full bg-gray-100 text-gray-700 text-sm", className)}>
      {status}
    </div>
  );
}
