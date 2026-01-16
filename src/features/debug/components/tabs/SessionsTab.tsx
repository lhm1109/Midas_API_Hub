import { useState, useEffect } from 'react';
import { Button } from '@/components/ui/button';
import { RefreshCw, Trash2, User, Lock, Clock } from 'lucide-react';
import {
  Table,
  TableBody,
  TableCell,
  TableHead,
  TableHeader,
  TableRow,
} from '@/components/ui/table';
import { Badge } from '@/components/ui/badge';
import { toast } from 'sonner';

interface LockInfo {
  endpoint_id?: string;
  version_id?: string;
  locked_by: string;
  locked_at: string;
  expires_at: string;
  last_activity: string;
}

interface ActiveSession {
  user: string;
  locks: LockInfo[];
  totalLocks: number;
}

export function SessionsTab() {
  const [sessions, setSessions] = useState<ActiveSession[]>([]);
  const [loading, setLoading] = useState(false);

  const fetchSessions = async () => {
    setLoading(true);
    try {
      const response = await fetch('http://localhost:9527/api/locks/all');
      if (!response.ok) throw new Error('Failed to fetch active sessions');
      
      const data = await response.json();
      const allLocks: LockInfo[] = [
        ...(data.endpointLocks || []).map((lock: any) => ({ ...lock, type: 'endpoint' })),
        ...(data.versionLocks || []).map((lock: any) => ({ ...lock, type: 'version' })),
      ];

      // 사용자별로 그룹화
      const userMap = new Map<string, LockInfo[]>();
      allLocks.forEach(lock => {
        if (!userMap.has(lock.locked_by)) {
          userMap.set(lock.locked_by, []);
        }
        userMap.get(lock.locked_by)!.push(lock);
      });

      // ActiveSession 배열로 변환
      const sessionsList: ActiveSession[] = Array.from(userMap.entries()).map(([user, locks]) => ({
        user,
        locks,
        totalLocks: locks.length,
      }));

      setSessions(sessionsList);
    } catch (error) {
      console.error('Failed to fetch sessions:', error);
      toast.error('❌ 세션 정보를 불러오는데 실패했습니다.');
    } finally {
      setLoading(false);
    }
  };

  useEffect(() => {
    fetchSessions();
    // 10초마다 자동 새로고침
    const interval = setInterval(fetchSessions, 10000);
    return () => clearInterval(interval);
  }, []);

  const handleForceUnlock = async (lock: LockInfo) => {
    if (!confirm(`정말로 "${lock.locked_by}" 사용자의 잠금을 강제로 해제하시겠습니까?`)) {
      return;
    }

    try {
      const endpointId = lock.endpoint_id || lock.version_id;
      if (!endpointId) return;

      const url = lock.endpoint_id
        ? `http://localhost:9527/api/locks/endpoint/${encodeURIComponent(endpointId)}/lock`
        : `http://localhost:9527/api/locks/version/${encodeURIComponent(endpointId)}/lock`;

      const response = await fetch(url, {
        method: 'DELETE',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ userId: 'admin', force: true }), // 🔥 관리자 강제 해제
      });

      if (response.ok) {
        toast.success(`✅ "${lock.locked_by}" 사용자의 잠금이 해제되었습니다.`);
        fetchSessions();
      } else {
        throw new Error('Failed to unlock');
      }
    } catch (error) {
      console.error('Failed to force unlock:', error);
      toast.error('❌ 잠금 해제에 실패했습니다.');
    }
  };

  const handleForceUnlockAll = async (user: string) => {
    if (!confirm(`정말로 "${user}" 사용자의 모든 잠금을 강제로 해제하시겠습니까?`)) {
      return;
    }

    try {
      const userLocks = sessions.find(s => s.user === user)?.locks || [];
      const unlockPromises = userLocks.map(lock => {
        const endpointId = lock.endpoint_id || lock.version_id;
        if (!endpointId) return Promise.resolve();

        const url = lock.endpoint_id
          ? `http://localhost:9527/api/locks/endpoint/${encodeURIComponent(endpointId)}/lock`
          : `http://localhost:9527/api/locks/version/${encodeURIComponent(endpointId)}/lock`;

        return fetch(url, {
          method: 'DELETE',
          headers: { 'Content-Type': 'application/json' },
          body: JSON.stringify({ userId: 'admin', force: true }), // 🔥 관리자 강제 해제
        });
      });

      await Promise.all(unlockPromises);
      toast.success(`✅ "${user}" 사용자의 모든 잠금이 해제되었습니다.`);
      fetchSessions();
    } catch (error) {
      console.error('Failed to force unlock all:', error);
      toast.error('❌ 잠금 해제에 실패했습니다.');
    }
  };

  const formatTime = (dateString: string) => {
    const date = new Date(dateString);
    const now = new Date();
    const diff = now.getTime() - date.getTime();
    const minutes = Math.floor(diff / 60000);
    const seconds = Math.floor((diff % 60000) / 1000);
    
    if (minutes > 0) {
      return `${minutes}분 ${seconds}초 전`;
    }
    return `${seconds}초 전`;
  };

  const getRemainingTime = (expiresAt: string) => {
    const expires = new Date(expiresAt);
    const now = new Date();
    const diff = expires.getTime() - now.getTime();
    
    if (diff <= 0) return '만료됨';
    
    const minutes = Math.floor(diff / 60000);
    const seconds = Math.floor((diff % 60000) / 1000);
    return `${minutes}분 ${seconds}초`;
  };

  return (
    <div className="h-full flex flex-col bg-zinc-950 overflow-hidden">
      {/* Header */}
      <div className="border-b border-zinc-800 bg-zinc-900 px-6 py-4 flex-shrink-0">
        <div className="flex items-center justify-between">
          <div>
            <h3 className="text-lg font-semibold flex items-center gap-2">
              <User className="w-5 h-5 text-blue-400" />
              활성 사용자 세션
            </h3>
            <p className="text-sm text-zinc-500 mt-1">
              현재 접속 중인 사용자와 편집 중인 엔드포인트 목록
            </p>
          </div>
          <Button
            onClick={fetchSessions}
            disabled={loading}
            variant="outline"
            size="sm"
            className="border-zinc-700 text-zinc-300 hover:bg-zinc-800"
          >
            <RefreshCw className={`w-4 h-4 mr-2 ${loading ? 'animate-spin' : ''}`} />
            새로고침
          </Button>
        </div>
      </div>

      {/* Content */}
      <div className="flex-1 overflow-auto p-6">
        {sessions.length === 0 ? (
          <div className="flex items-center justify-center h-full text-zinc-500">
            <div className="text-center">
              <User className="w-12 h-12 mx-auto mb-4 opacity-50" />
              <p className="text-lg font-medium">활성 세션이 없습니다</p>
              <p className="text-sm mt-2">현재 편집 중인 사용자가 없습니다.</p>
            </div>
          </div>
        ) : (
          <div className="space-y-4">
            {sessions.map((session) => (
              <div
                key={session.user}
                className="bg-zinc-900 border border-zinc-800 rounded-lg overflow-hidden"
              >
                {/* User Header */}
                <div className="bg-zinc-800 px-4 py-3 flex items-center justify-between border-b border-zinc-700">
                  <div className="flex items-center gap-3">
                    <div className="w-8 h-8 rounded-full bg-blue-600 flex items-center justify-center">
                      <User className="w-4 h-4 text-white" />
                    </div>
                    <div>
                      <div className="font-semibold text-white">{session.user}</div>
                      <div className="text-xs text-zinc-400">
                        {session.totalLocks}개 엔드포인트 편집 중
                      </div>
                    </div>
                  </div>
                  <Button
                    onClick={() => handleForceUnlockAll(session.user)}
                    variant="outline"
                    size="sm"
                    className="border-red-700 text-red-400 hover:bg-red-900/20"
                  >
                    <Trash2 className="w-4 h-4 mr-2" />
                    모든 세션 끊기
                  </Button>
                </div>

                {/* Locks Table */}
                <div className="overflow-x-auto">
                  <Table>
                    <TableHeader>
                      <TableRow className="bg-zinc-800/50">
                        <TableHead className="text-zinc-300">타입</TableHead>
                        <TableHead className="text-zinc-300">엔드포인트/버전</TableHead>
                        <TableHead className="text-zinc-300">잠금 시간</TableHead>
                        <TableHead className="text-zinc-300">마지막 활동</TableHead>
                        <TableHead className="text-zinc-300">만료까지</TableHead>
                        <TableHead className="text-zinc-300 text-right">작업</TableHead>
                      </TableRow>
                    </TableHeader>
                    <TableBody>
                      {session.locks.map((lock, idx) => (
                        <TableRow key={idx} className="border-zinc-800">
                          <TableCell>
                            <Badge
                              variant={lock.endpoint_id ? 'default' : 'secondary'}
                              className={
                                lock.endpoint_id
                                  ? 'bg-blue-600 text-white'
                                  : 'bg-purple-600 text-white'
                              }
                            >
                              {lock.endpoint_id ? 'Endpoint' : 'Version'}
                            </Badge>
                          </TableCell>
                          <TableCell className="font-mono text-sm">
                            {lock.endpoint_id || lock.version_id}
                          </TableCell>
                          <TableCell className="text-zinc-400 text-sm">
                            {formatTime(lock.locked_at)}
                          </TableCell>
                          <TableCell className="text-zinc-400 text-sm">
                            {formatTime(lock.last_activity)}
                          </TableCell>
                          <TableCell>
                            <div className="flex items-center gap-2">
                              <Clock className="w-3 h-3 text-yellow-400" />
                              <span className="text-sm text-yellow-400">
                                {getRemainingTime(lock.expires_at)}
                              </span>
                            </div>
                          </TableCell>
                          <TableCell className="text-right">
                            <Button
                              onClick={() => handleForceUnlock(lock)}
                              variant="outline"
                              size="sm"
                              className="border-red-700 text-red-400 hover:bg-red-900/20"
                            >
                              <Trash2 className="w-3 h-3 mr-1" />
                              끊기
                            </Button>
                          </TableCell>
                        </TableRow>
                      ))}
                    </TableBody>
                  </Table>
                </div>
              </div>
            ))}
          </div>
        )}
      </div>
    </div>
  );
}
