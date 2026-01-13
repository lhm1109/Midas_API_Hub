import { Alert, AlertDescription, AlertTitle } from '@/components/ui/alert';
import { Button } from '@/components/ui/button';
import { Lock, AlertTriangle, Clock, X } from 'lucide-react';
import { useAppStore } from '@/store/useAppStore';
import { useEffect, useState } from 'react';

interface LockBannerProps {
  endpointId: string;
  onLockAcquired?: () => void;
}

export function LockBanner({ endpointId, onLockAcquired }: LockBannerProps) {
  const { endpointLock, currentUserId, checkEndpointLock, acquireEndpointLock, releaseEndpointLock } = useAppStore();
  const [isAcquiring, setIsAcquiring] = useState(false);
  const [remainingTime, setRemainingTime] = useState<string>('');

  // 주기적으로 잠금 상태 확인
  useEffect(() => {
    if (!endpointId) return;
    
    checkEndpointLock(endpointId);
    const interval = setInterval(() => {
      checkEndpointLock(endpointId);
    }, 10000); // 10초마다 확인

    return () => clearInterval(interval);
  }, [endpointId]);

  // 남은 시간 계산
  useEffect(() => {
    if (!endpointLock?.expiresAt) return;

    const updateRemainingTime = () => {
      const now = new Date();
      const expires = new Date(endpointLock.expiresAt!);
      const diff = expires.getTime() - now.getTime();

      if (diff <= 0) {
        setRemainingTime('만료됨');
        checkEndpointLock(endpointId);
      } else {
        const minutes = Math.floor(diff / 60000);
        const seconds = Math.floor((diff % 60000) / 1000);
        setRemainingTime(`${minutes}분 ${seconds}초`);
      }
    };

    updateRemainingTime();
    const interval = setInterval(updateRemainingTime, 1000);

    return () => clearInterval(interval);
  }, [endpointLock?.expiresAt, endpointId]);

  const handleRequestEdit = async () => {
    setIsAcquiring(true);
    const success = await acquireEndpointLock(endpointId);
    setIsAcquiring(false);
    
    if (success && onLockAcquired) {
      onLockAcquired();
    }
  };

  const handleReleaseLock = async () => {
    await releaseEndpointLock(endpointId);
  };

  // 잠금되지 않은 경우 표시 안 함
  if (!endpointLock?.locked) {
    return null;
  }

  const isLockedByMe = endpointLock.lockedBy === currentUserId;

  return (
    <div className="sticky top-0 z-50 animate-in slide-in-from-top duration-300">
      {isLockedByMe ? (
        // 내가 잠금한 경우 - 성공 배너
        <Alert className="rounded-none border-x-0 border-t-0 border-b-2 border-green-500 bg-green-50 dark:bg-green-950">
          <Lock className="h-5 w-5 text-green-600" />
          <AlertTitle className="text-green-900 dark:text-green-100 font-bold flex items-center gap-2">
            ✅ 편집 중
            <Clock className="h-4 w-4" />
            <span className="text-sm font-normal">{remainingTime} 남음</span>
          </AlertTitle>
          <AlertDescription className="text-green-800 dark:text-green-200 flex items-center justify-between">
            <span>현재 이 엔드포인트를 편집하고 있습니다. 저장 또는 취소 시 자동으로 잠금이 해제됩니다.</span>
            <Button
              variant="outline"
              size="sm"
              onClick={handleReleaseLock}
              className="ml-4 border-green-600 text-green-700 hover:bg-green-100"
            >
              <X className="h-4 w-4 mr-1" />
              잠금 해제
            </Button>
          </AlertDescription>
        </Alert>
      ) : (
        // 다른 사용자가 잠금한 경우 - 경고 배너
        <Alert className="rounded-none border-x-0 border-t-0 border-b-4 border-red-500 bg-red-50 dark:bg-red-950 shadow-lg">
          <AlertTriangle className="h-6 w-6 text-red-600 animate-pulse" />
          <AlertTitle className="text-red-900 dark:text-red-100 font-bold text-lg flex items-center gap-3">
            <Lock className="h-5 w-5" />
            🚫 편집 불가 - 다른 사용자가 편집 중
            <Clock className="h-4 w-4" />
            <span className="text-sm font-normal text-red-700">{remainingTime} 후 자동 해제</span>
          </AlertTitle>
          <AlertDescription className="text-red-800 dark:text-red-200 space-y-2">
            <div className="flex items-center gap-2">
              <span className="font-semibold text-red-900 bg-red-200 px-2 py-1 rounded">
                {endpointLock.lockedBy}
              </span>
              <span>님이 현재 편집 중입니다.</span>
            </div>
            <div className="text-sm text-red-700">
              • 읽기 전용 모드로 표시됩니다.
              <br />
              • 잠금이 해제되면 편집할 수 있습니다.
              <br />
              • 5분간 활동이 없으면 자동으로 해제됩니다.
            </div>
            <Button
              variant="outline"
              size="sm"
              onClick={handleRequestEdit}
              disabled={isAcquiring}
              className="mt-2 border-red-600 text-red-700 hover:bg-red-100"
            >
              {isAcquiring ? '확인 중...' : '편집 요청 재시도'}
            </Button>
          </AlertDescription>
        </Alert>
      )}
    </div>
  );
}


