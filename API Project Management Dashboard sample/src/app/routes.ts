import { createBrowserRouter } from "react-router";
import Root from "@/app/pages/Root";
import Dashboard from "@/app/pages/Dashboard";
import ProgressPage from "@/app/pages/ProgressPage";

export const router = createBrowserRouter([
  {
    path: "/",
    Component: Root,
    children: [
      { index: true, Component: Dashboard },
      { path: "progress", Component: ProgressPage },
    ],
  },
]);
