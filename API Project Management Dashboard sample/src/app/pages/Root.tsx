import { Outlet, NavLink, useLocation } from "react-router";
import { Tabs, TabsList, TabsTrigger } from "@/app/components/ui/tabs";

export default function Root() {
  const location = useLocation();
  
  return (
    <div className="min-h-screen bg-gray-50">
      <div className="container mx-auto p-6">
        <div className="mb-6">
          <h1 className="text-3xl font-bold mb-2">API 업무 진행 관리</h1>
          <p className="text-gray-600">
            담당자 간 협업 및 진행 상황 추적
          </p>
        </div>
        
        <Tabs value={location.pathname === "/" ? "dashboard" : "progress"} className="mb-6">
          <TabsList>
            <NavLink to="/">
              {({ isActive }) => (
                <TabsTrigger value="dashboard" className={isActive ? "data-[state=active]" : ""}>
                  Dashboard
                </TabsTrigger>
              )}
            </NavLink>
            <NavLink to="/progress">
              {({ isActive }) => (
                <TabsTrigger value="progress" className={isActive ? "data-[state=active]" : ""}>
                  Progress
                </TabsTrigger>
              )}
            </NavLink>
          </TabsList>
        </Tabs>

        <Outlet />
      </div>
    </div>
  );
}
