import type { ReactNode } from 'react';
import { CheckCircle2, FileCode, FileJson, FolderGit2, GitPullRequest, X } from 'lucide-react';
import { useAIWorkspaceStore } from '../store/useAIWorkspaceStore';
import { useArtifactStore } from '../store/useArtifactStore';

const LANG_ICONS: Record<string, ReactNode> = {
  python: <FileCode className="size-3" />,
  json: <FileJson className="size-3" />,
};

const emptyFiles = [
  'generated_pydantic/verification',
  'generated_schemas/default',
  'nimbalyst',
];

export function ArtifactPanel() {
  const { setArtifactPanelVisible } = useAIWorkspaceStore();
  const { artifacts, activeArtifactId, selectArtifact } = useArtifactStore();

  const activeArtifact = artifacts.find((artifact) => artifact.id === activeArtifactId);

  return (
    <div className="flex h-full flex-col bg-zinc-950">
      <div className="flex h-12 flex-shrink-0 items-center justify-between border-b border-zinc-800 bg-zinc-900 px-3">
        <div className="min-w-0">
          <p className="text-xs font-semibold uppercase tracking-wide text-zinc-400">Files</p>
          <p className="truncate text-[11px] text-zinc-600">Session artifacts and edited outputs</p>
        </div>
        <button
          type="button"
          onClick={() => setArtifactPanelVisible(false)}
          className="flex size-7 items-center justify-center rounded text-zinc-400 transition-colors hover:bg-zinc-800 hover:text-zinc-200"
          title="Close files"
        >
          <X className="size-4" />
        </button>
      </div>

      {artifacts.length > 0 && (
        <div className="flex flex-shrink-0 items-center gap-1 overflow-x-auto border-b border-zinc-800 px-2 py-1.5">
          {artifacts.map((artifact) => (
            <button
              key={artifact.id}
              type="button"
              onClick={() => selectArtifact(artifact.id)}
              className={`flex items-center gap-1.5 whitespace-nowrap rounded px-2.5 py-1 text-xs transition-colors ${
                artifact.id === activeArtifactId
                  ? 'bg-zinc-700 text-zinc-100'
                  : 'text-zinc-500 hover:bg-zinc-800 hover:text-zinc-300'
              }`}
            >
              {LANG_ICONS[artifact.language] ?? <FileCode className="size-3" />}
              {artifact.filename}
            </button>
          ))}
        </div>
      )}

      <div className="flex-1 overflow-auto">
        {!activeArtifact ? (
          <div className="p-3">
            <div className="rounded-lg border border-zinc-800 bg-zinc-900/70 p-3">
              <div className="flex items-center gap-2 text-sm font-medium text-zinc-200">
                <FolderGit2 className="size-4 text-blue-300" />
                Workspace Files
              </div>
              <div className="mt-3 space-y-1.5">
                {emptyFiles.map((file) => (
                  <div key={file} className="flex items-center gap-2 rounded-md bg-zinc-950 px-2 py-1.5 text-xs text-zinc-500">
                    <CheckCircle2 className="size-3 text-zinc-600" />
                    <span className="truncate">{file}</span>
                  </div>
                ))}
              </div>
            </div>

            <div className="mt-3 rounded-lg border border-dashed border-zinc-800 p-4 text-center">
              <GitPullRequest className="mx-auto size-6 text-zinc-700" />
              <p className="mt-2 text-xs text-zinc-500">Generated files will appear here.</p>
            </div>
          </div>
        ) : (
          <pre className="h-full overflow-auto bg-zinc-950 p-4 font-mono text-xs leading-relaxed text-zinc-300">
            {activeArtifact.content}
          </pre>
        )}
      </div>
    </div>
  );
}
