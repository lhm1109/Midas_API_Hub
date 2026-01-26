import { Card, CardContent, CardHeader, CardTitle } from '@/components/ui/card';
import { Progress } from '@/components/ui/progress';
import {
  CheckCircle2,
  Circle,
  Loader2,
  Users,
  ListTodo,
} from 'lucide-react';
import { ApiTask } from '../types/manager';

interface ManagerDashboardProps {
  tasks: ApiTask[];
}

export function ManagerDashboard({ tasks }: ManagerDashboardProps) {
  // 통계 계산
  const totalTasks = tasks.length;
  const completedTasks = tasks.filter((task) => task.status === 'Done').length;
  const workingTasks = tasks.filter((task) => task.status === 'Working').length;
  const pendingTasks = totalTasks - completedTasks - workingTasks;

  const completionRate = totalTasks > 0 ? (completedTasks / totalTasks) * 100 : 0;

  // 개발 상태 통계
  const devStats = {
    completed: tasks.filter((task) => task.dev === 'green').length,
    inProgress: tasks.filter((task) => task.dev === 'yellow').length,
    blocked: tasks.filter((task) => task.dev === 'red').length,
  };

  // 담당자별 통계
  const chargeStats: Record<string, number> = {};
  tasks.forEach((task) => {
    if (task.charge) {
      chargeStats[task.charge] = (chargeStats[task.charge] || 0) + 1;
    }
  });

  return (
    <div className="space-y-6 p-6">
      {/* 개요 카드 */}
      <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-4 gap-4">
        <Card className="bg-zinc-900 border-zinc-800">
          <CardHeader className="flex flex-row items-center justify-between space-y-0 pb-2">
            <CardTitle className="text-sm font-medium text-zinc-400">전체 작업</CardTitle>
            <ListTodo className="h-4 w-4 text-zinc-400" />
          </CardHeader>
          <CardContent>
            <div className="text-2xl font-bold text-white">{totalTasks}</div>
            <p className="text-xs text-zinc-500 mt-1">총 API 작업 수</p>
          </CardContent>
        </Card>

        <Card className="bg-zinc-900 border-zinc-800">
          <CardHeader className="flex flex-row items-center justify-between space-y-0 pb-2">
            <CardTitle className="text-sm font-medium text-zinc-400">완료</CardTitle>
            <CheckCircle2 className="h-4 w-4 text-green-600" />
          </CardHeader>
          <CardContent>
            <div className="text-2xl font-bold text-white">{completedTasks}</div>
            <p className="text-xs text-zinc-500 mt-1">
              {completionRate.toFixed(1)}% 완료
            </p>
          </CardContent>
        </Card>

        <Card className="bg-zinc-900 border-zinc-800">
          <CardHeader className="flex flex-row items-center justify-between space-y-0 pb-2">
            <CardTitle className="text-sm font-medium text-zinc-400">진행 중</CardTitle>
            <Loader2 className="h-4 w-4 text-blue-600" />
          </CardHeader>
          <CardContent>
            <div className="text-2xl font-bold text-white">{workingTasks}</div>
            <p className="text-xs text-zinc-500 mt-1">현재 작업 중</p>
          </CardContent>
        </Card>

        <Card className="bg-zinc-900 border-zinc-800">
          <CardHeader className="flex flex-row items-center justify-between space-y-0 pb-2">
            <CardTitle className="text-sm font-medium text-zinc-400">대기 중</CardTitle>
            <Circle className="h-4 w-4 text-zinc-400" />
          </CardHeader>
          <CardContent>
            <div className="text-2xl font-bold text-white">{pendingTasks}</div>
            <p className="text-xs text-zinc-500 mt-1">시작 전</p>
          </CardContent>
        </Card>
      </div>

      {/* 진행률 */}
      <Card className="bg-zinc-900 border-zinc-800">
        <CardHeader>
          <CardTitle className="text-white">전체 진행률</CardTitle>
        </CardHeader>
        <CardContent className="space-y-4">
          <div className="space-y-2">
            <div className="flex justify-between text-sm">
              <span className="text-zinc-400">완료율</span>
              <span className="font-semibold text-white">{completionRate.toFixed(1)}%</span>
            </div>
            <Progress value={completionRate} className="bg-zinc-800" />
          </div>
          <div className="grid grid-cols-3 gap-4 text-sm">
            <div>
              <div className="text-zinc-400">완료</div>
              <div className="text-lg font-semibold text-green-600">
                {completedTasks}
              </div>
            </div>
            <div>
              <div className="text-zinc-400">진행 중</div>
              <div className="text-lg font-semibold text-blue-600">
                {workingTasks}
              </div>
            </div>
            <div>
              <div className="text-zinc-400">대기</div>
              <div className="text-lg font-semibold text-zinc-400">
                {pendingTasks}
              </div>
            </div>
          </div>
        </CardContent>
      </Card>

      {/* 개발 상태 */}
      <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
        <Card className="bg-zinc-900 border-zinc-800">
          <CardHeader>
            <CardTitle className="text-white">개발 상태</CardTitle>
          </CardHeader>
          <CardContent className="space-y-3">
            <div className="flex items-center justify-between">
              <div className="flex items-center gap-2">
                <div className="w-3 h-3 rounded-full bg-green-500" />
                <span className="text-sm text-zinc-300">완료</span>
              </div>
              <span className="font-semibold text-white">{devStats.completed}</span>
            </div>
            <div className="flex items-center justify-between">
              <div className="flex items-center gap-2">
                <div className="w-3 h-3 rounded-full bg-yellow-500" />
                <span className="text-sm text-zinc-300">진행 중</span>
              </div>
              <span className="font-semibold text-white">{devStats.inProgress}</span>
            </div>
            <div className="flex items-center justify-between">
              <div className="flex items-center gap-2">
                <div className="w-3 h-3 rounded-full bg-red-500" />
                <span className="text-sm text-zinc-300">문제</span>
              </div>
              <span className="font-semibold text-white">{devStats.blocked}</span>
            </div>
          </CardContent>
        </Card>

        <Card className="bg-zinc-900 border-zinc-800">
          <CardHeader>
            <CardTitle className="flex items-center gap-2 text-white">
              <Users className="w-5 h-5" />
              담당자별 작업
            </CardTitle>
          </CardHeader>
          <CardContent className="space-y-3">
            {Object.entries(chargeStats).length > 0 ? (
              Object.entries(chargeStats).map(([name, count]) => (
                <div key={name} className="flex items-center justify-between">
                  <span className="text-sm text-zinc-300">{name}</span>
                  <span className="font-semibold text-white">{count}개</span>
                </div>
              ))
            ) : (
              <p className="text-sm text-zinc-500">할당된 작업이 없습니다</p>
            )}
          </CardContent>
        </Card>
      </div>

      {/* 최근 활동 */}
      <Card className="bg-zinc-900 border-zinc-800">
        <CardHeader>
          <CardTitle className="text-white">최근 업데이트</CardTitle>
        </CardHeader>
        <CardContent>
          <div className="space-y-3">
            {tasks.slice(0, 5).map((task) => (
              <div
                key={task.id}
                className="flex items-start gap-3 pb-3 border-b border-zinc-800 last:border-0"
              >
                <div className="mt-1">
                  {task.status === 'Done' && (
                    <CheckCircle2 className="w-4 h-4 text-green-600" />
                  )}
                  {task.status === 'Working' && (
                    <Loader2 className="w-4 h-4 text-blue-600" />
                  )}
                  {!task.status && <Circle className="w-4 h-4 text-zinc-600" />}
                </div>
                <div className="flex-1 min-w-0">
                  <p className="text-sm font-medium truncate text-zinc-300">
                    {task.tab} - {task.group} - {task.sub1}
                  </p>
                  <p className="text-xs text-zinc-500">
                    {task.endPoint || '엔드포인트 없음'}
                  </p>
                </div>
                {task.charge && (
                  <div className="text-xs text-zinc-500">{task.charge}</div>
                )}
              </div>
            ))}
          </div>
        </CardContent>
      </Card>
    </div>
  );
}
