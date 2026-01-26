import { Circle, CheckCircle2, AlertTriangle, Loader2 } from "lucide-react";
import { cn } from "@/components/ui/utils";
import { StatusType } from "../types/manager";

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
            "w-4 h-4 rounded-full bg-green-500 border-2 border-black",
            className
          )}
          title="완료"
        />
      );
    case "yellow":
      return (
        <div
          className={cn(
            "w-4 h-4 rounded-full bg-yellow-500 border-2 border-black",
            className
          )}
          title="진행 중"
        />
      );
    case "red":
      return (
        <div
          className={cn(
            "w-4 h-4 rounded-full bg-red-500 border-2 border-black",
            className
          )}
          title="문제"
        />
      );
    case "empty":
      return (
        <div
          className={cn(
            "w-4 h-4 rounded-full bg-transparent border-2 border-gray-400",
            className
          )}
          title="시작 안함"
        />
      );
    case "working":
      return (
        <Loader2
          className={cn("w-4 h-4 text-blue-500 animate-spin", className)}
          title="작업 중"
        />
      );
    case "done":
      return (
        <CheckCircle2
          className={cn("w-4 h-4 text-green-600", className)}
          title="완료"
        />
      );
    case "warning":
      return (
        <AlertTriangle
          className={cn("w-4 h-4 text-orange-500", className)}
          title="경고"
        />
      );
    case "na":
      return (
        <div
          className={cn(
            "w-4 h-4 rounded-full bg-zinc-600 border-2 border-black flex items-center justify-center",
            className
          )}
          title="해당없음"
        >
          <span className="text-[8px] font-bold text-white">N/A</span>
        </div>
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
          "inline-flex items-center gap-1 px-2 py-0.5 rounded-md bg-blue-600 text-white text-xs font-medium",
          className
        )}
      >
        <Loader2 className="w-3 h-3 animate-spin" />
        Working
      </div>
    );
  }
  
  if (status === "In Progress") {
    return (
      <div
        className={cn(
          "inline-flex items-center gap-1 px-2 py-0.5 rounded-md bg-yellow-600 text-white text-xs font-medium",
          className
        )}
      >
        <Loader2 className="w-3 h-3 animate-spin" />
        Progress
      </div>
    );
  }
  
  if (status === "Done") {
    return (
      <div
        className={cn(
          "inline-flex items-center gap-1 px-2 py-0.5 rounded-md bg-green-600 text-white text-xs font-medium",
          className
        )}
      >
        <CheckCircle2 className="w-3 h-3" />
        Done
      </div>
    );
  }

  if (status === "Cancel") {
    return (
      <div
        className={cn(
          "inline-flex items-center gap-1 px-2 py-0.5 rounded-md bg-red-600 text-white text-xs font-medium",
          className
        )}
      >
        Cancel
      </div>
    );
  }

  if (status === "none" || !status) {
    return null;
  }

  return (
    <div className={cn("px-2 py-0.5 rounded-md bg-zinc-700 text-zinc-300 text-xs font-medium", className)}>
      {status}
    </div>
  );
}
