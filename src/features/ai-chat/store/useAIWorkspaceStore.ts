import { create } from 'zustand';
import { persist } from 'zustand/middleware';

interface AIWorkspaceState {
  sessionSidebarWidth: number;
  sessionSidebarCollapsed: boolean;
  artifactPanelWidth: number;
  artifactPanelVisible: boolean;

  setSessionSidebarWidth: (w: number) => void;
  setSessionSidebarCollapsed: (v: boolean) => void;
  setArtifactPanelWidth: (w: number) => void;
  setArtifactPanelVisible: (v: boolean) => void;
}

export const useAIWorkspaceStore = create<AIWorkspaceState>()(
  persist(
    (set) => ({
      sessionSidebarWidth: 240,
      sessionSidebarCollapsed: false,
      artifactPanelWidth: 320,
      artifactPanelVisible: false,

      setSessionSidebarWidth: (w) => set({ sessionSidebarWidth: w }),
      setSessionSidebarCollapsed: (v) => set({ sessionSidebarCollapsed: v }),
      setArtifactPanelWidth: (w) => set({ artifactPanelWidth: w }),
      setArtifactPanelVisible: (v) => set({ artifactPanelVisible: v }),
    }),
    { name: 'ai-workspace-layout' }
  )
);
