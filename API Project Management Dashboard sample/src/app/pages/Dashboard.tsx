import { sampleTasks } from "@/app/data/sampleData";
import { Card, CardContent, CardHeader, CardTitle } from "@/app/components/ui/card";
import { Progress } from "@/app/components/ui/progress";
import {
  CheckCircle2,
  Circle,
  AlertTriangle,
  Loader2,
  Users,
  ListTodo,
} from "lucide-react";

export default function Dashboard() {
  // 통계 계산
  const totalTasks = sampleTasks.length;
  const completedTasks = sampleTasks.filter(
    (task) => task.status === "Done"
  ).length;
  const workingTasks = sampleTasks.filter(
    (task) => task.status === "Working"
  ).length;
  const pendingTasks = totalTasks - completedTasks - workingTasks;

  const completionRate = totalTasks > 0 ? (completedTasks / totalTasks) * 100 : 0;

  // 개발 상태 통계
  const devStats = {
    completed: sampleTasks.filter((task) => task.dev === "green").length,
    inProgress: sampleTasks.filter((task) => task.dev === "yellow").length,
    blocked: sampleTasks.filter((task) => task.dev === "red").length,
  };

  // 담당자별 통계
  const chargeStats: Record<string, number> = {};
  sampleTasks.forEach((task) => {
    if (task.charge) {
      chargeStats[task.charge] = (chargeStats[task.charge] || 0) + 1;
    }
  });

  return (
    <div className="space-y-6">
      {/* 개요 카�� */}
      <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-4 gap-4">
        <Card>
          <CardHeader className="flex flex-row items-center justify-between space-y-0 pb-2">
            <CardTitle className="text-sm font-medium">전체 작업</CardTitle>
            <ListTodo className="h-4 w-4 text-gray-600" />
          </CardHeader>
          <CardContent>
            <div className="text-2xl font-bold">{totalTasks}</div>
            <p className="text-xs text-gray-600 mt-1">총 API 작업 수</p>
          </CardContent>
        </Card>

        <Card>
          <CardHeader className="flex flex-row items-center justify-between space-y-0 pb-2">
            <CardTitle className="text-sm font-medium">완료</CardTitle>
            <CheckCircle2 className="h-4 w-4 text-green-600" />
          </CardHeader>
          <CardContent>
            <div className="text-2xl font-bold">{completedTasks}</div>
            <p className="text-xs text-gray-600 mt-1">
              {completionRate.toFixed(1)}% 완료
            </p>
          </CardContent>
        </Card>

        <Card>
          <CardHeader className="flex flex-row items-center justify-between space-y-0 pb-2">
            <CardTitle className="text-sm font-medium">진행 중</CardTitle>
            <Loader2 className="h-4 w-4 text-blue-600" />
          </CardHeader>
          <CardContent>
            <div className="text-2xl font-bold">{workingTasks}</div>
            <p className="text-xs text-gray-600 mt-1">현재 작업 중</p>
          </CardContent>
        </Card>

        <Card>
          <CardHeader className="flex flex-row items-center justify-between space-y-0 pb-2">
            <CardTitle className="text-sm font-medium">대기 중</CardTitle>
            <Circle className="h-4 w-4 text-gray-400" />
          </CardHeader>
          <CardContent>
            <div className="text-2xl font-bold">{pendingTasks}</div>
            <p className="text-xs text-gray-600 mt-1">시작 전</p>
          </CardContent>
        </Card>
      </div>

      {/* 진행률 */}
      <Card>
        <CardHeader>
          <CardTitle>전체 진행률</CardTitle>
        </CardHeader>
        <CardContent className="space-y-4">
          <div className="space-y-2">
            <div className="flex justify-between text-sm">
              <span>완료율</span>
              <span className="font-semibold">{completionRate.toFixed(1)}%</span>
            </div>
            <Progress value={completionRate} />
          </div>
          <div className="grid grid-cols-3 gap-4 text-sm">
            <div>
              <div className="text-gray-600">완료</div>
              <div className="text-lg font-semibold text-green-600">
                {completedTasks}
              </div>
            </div>
            <div>
              <div className="text-gray-600">진행 중</div>
              <div className="text-lg font-semibold text-blue-600">
                {workingTasks}
              </div>
            </div>
            <div>
              <div className="text-gray-600">대기</div>
              <div className="text-lg font-semibold text-gray-600">
                {pendingTasks}
              </div>
            </div>
          </div>
        </CardContent>
      </Card>

      {/* 개발 상태 */}
      <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
        <Card>
          <CardHeader>
            <CardTitle>개발 상태</CardTitle>
          </CardHeader>
          <CardContent className="space-y-3">
            <div className="flex items-center justify-between">
              <div className="flex items-center gap-2">
                <div className="w-3 h-3 rounded-full bg-green-500" />
                <span className="text-sm">완료</span>
              </div>
              <span className="font-semibold">{devStats.completed}</span>
            </div>
            <div className="flex items-center justify-between">
              <div className="flex items-center gap-2">
                <div className="w-3 h-3 rounded-full bg-yellow-500" />
                <span className="text-sm">진행 중</span>
              </div>
              <span className="font-semibold">{devStats.inProgress}</span>
            </div>
            <div className="flex items-center justify-between">
              <div className="flex items-center gap-2">
                <div className="w-3 h-3 rounded-full bg-red-500" />
                <span className="text-sm">문제</span>
              </div>
              <span className="font-semibold">{devStats.blocked}</span>
            </div>
          </CardContent>
        </Card>

        <Card>
          <CardHeader>
            <CardTitle className="flex items-center gap-2">
              <Users className="w-5 h-5" />
              담당자별 작업
            </CardTitle>
          </CardHeader>
          <CardContent className="space-y-3">
            {Object.entries(chargeStats).length > 0 ? (
              Object.entries(chargeStats).map(([name, count]) => (
                <div key={name} className="flex items-center justify-between">
                  <span className="text-sm">{name}</span>
                  <span className="font-semibold">{count}개</span>
                </div>
              ))
            ) : (
              <p className="text-sm text-gray-500">할당된 작업이 없습니다</p>
            )}
          </CardContent>
        </Card>
      </div>

      {/* 최근 활동 */}
      <Card>
        <CardHeader>
          <CardTitle>최근 업데이트</CardTitle>
        </CardHeader>
        <CardContent>
          <div className="space-y-3">
            {sampleTasks.slice(0, 5).map((task) => (
              <div
                key={task.id}
                className="flex items-start gap-3 pb-3 border-b last:border-0"
              >
                <div className="mt-1">
                  {task.status === "Done" && (
                    <CheckCircle2 className="w-4 h-4 text-green-600" />
                  )}
                  {task.status === "Working" && (
                    <Loader2 className="w-4 h-4 text-blue-600" />
                  )}
                  {!task.status && <Circle className="w-4 h-4 text-gray-400" />}
                </div>
                <div className="flex-1 min-w-0">
                  <p className="text-sm font-medium truncate">
                    {task.tab} - {task.group} - {task.sub1}
                  </p>
                  <p className="text-xs text-gray-600">{task.endPoint || "엔드포인트 없음"}</p>
                </div>
                {task.charge && (
                  <div className="text-xs text-gray-600">{task.charge}</div>
                )}
              </div>
            ))}
          </div>
        </CardContent>
      </Card>
    </div>
  );
}
