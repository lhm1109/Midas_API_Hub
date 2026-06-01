import { ResizableLayout } from './layout/ResizableLayout';
import { SessionSidebar } from './panels/SessionSidebar';
import { ChatArea } from './panels/ChatArea';
import { ArtifactPanel } from './panels/ArtifactPanel';

export function AIWorkspaceView() {
  return (
    <div className="flex h-full w-full overflow-hidden bg-zinc-950">
      <ResizableLayout
        left={<SessionSidebar />}
        center={<ChatArea />}
        right={<ArtifactPanel />}
      />
    </div>
  );
}
