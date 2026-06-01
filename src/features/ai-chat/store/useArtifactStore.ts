import { create } from 'zustand';
import { persist } from 'zustand/middleware';

export interface Artifact {
  id: string;
  sessionId: string;
  filename: string;
  language: string;
  content: string;
  createdAt: number;
}

interface ArtifactState {
  artifacts: Artifact[];
  activeArtifactId: string | null;

  addArtifact: (a: Omit<Artifact, 'id' | 'createdAt'>) => string;
  selectArtifact: (id: string) => void;
  clearArtifacts: () => void;
  setArtifactsForSession: (sessionId: string, artifacts: Artifact[]) => void;
}

export const useArtifactStore = create<ArtifactState>()(
  persist(
    (set) => ({
      artifacts: [],
      activeArtifactId: null,

      addArtifact: (a) => {
        const id = crypto.randomUUID();
        const artifact: Artifact = { ...a, id, createdAt: Date.now() };
        set((s) => ({
          artifacts: [...s.artifacts, artifact],
          activeArtifactId: id,
        }));
        return id;
      },

      selectArtifact: (id) => set({ activeArtifactId: id }),
      clearArtifacts: () => set({ artifacts: [], activeArtifactId: null }),
      setArtifactsForSession: (sessionId, artifacts) => {
        const sessionArtifacts = artifacts.filter((a) => a.sessionId === sessionId);
        set({
          artifacts: sessionArtifacts,
          activeArtifactId: sessionArtifacts[0]?.id ?? null,
        });
      },
    }),
    { name: 'ai-artifacts' }
  )
);
