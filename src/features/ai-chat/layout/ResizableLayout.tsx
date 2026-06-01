import { useCallback } from 'react';
import { ResizableDivider } from './ResizableDivider';
import { useAIWorkspaceStore } from '../store/useAIWorkspaceStore';

const SESSION_SIDEBAR_MIN = 160;
const SESSION_SIDEBAR_MAX = 400;
const ARTIFACT_PANEL_MIN = 200;
const ARTIFACT_PANEL_MAX = 600;

interface ResizableLayoutProps {
  left: React.ReactNode;
  center: React.ReactNode;
  right: React.ReactNode;
}

export function ResizableLayout({ left, center, right }: ResizableLayoutProps) {
  const {
    sessionSidebarWidth,
    sessionSidebarCollapsed,
    artifactPanelWidth,
    artifactPanelVisible,
    setSessionSidebarWidth,
    setArtifactPanelWidth,
  } = useAIWorkspaceStore();

  const handleLeftResize = useCallback((delta: number) => {
    setSessionSidebarWidth(
      Math.max(SESSION_SIDEBAR_MIN, Math.min(SESSION_SIDEBAR_MAX, sessionSidebarWidth + delta))
    );
  }, [sessionSidebarWidth, setSessionSidebarWidth]);

  const handleRightResize = useCallback((delta: number) => {
    // 우측 패널은 왼쪽으로 당기면 넓어지므로 delta 반전
    setArtifactPanelWidth(
      Math.max(ARTIFACT_PANEL_MIN, Math.min(ARTIFACT_PANEL_MAX, artifactPanelWidth - delta))
    );
  }, [artifactPanelWidth, setArtifactPanelWidth]);

  const leftVisible = !sessionSidebarCollapsed;

  return (
    <div className="flex flex-row h-full w-full overflow-hidden">
      {/* 좌측: Session Sidebar */}
      {leftVisible && (
        <>
          <div
            className="flex flex-col flex-shrink-0 overflow-hidden bg-zinc-900 border-r border-zinc-800"
            style={{ width: sessionSidebarWidth }}
          >
            {left}
          </div>
          <ResizableDivider direction="horizontal" onResize={handleLeftResize} />
        </>
      )}

      {/* 중앙: Chat Area */}
      <div className="flex flex-col flex-1 min-w-0 overflow-hidden">
        {center}
      </div>

      {/* 우측: Artifact Panel */}
      {artifactPanelVisible && (
        <>
          <ResizableDivider direction="horizontal" onResize={handleRightResize} />
          <div
            className="flex flex-col flex-shrink-0 overflow-hidden bg-zinc-900 border-l border-zinc-800"
            style={{ width: artifactPanelWidth }}
          >
            {right}
          </div>
        </>
      )}
    </div>
  );
}
