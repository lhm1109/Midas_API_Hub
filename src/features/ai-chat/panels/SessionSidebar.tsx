import { ChevronLeft, Clock3, GitBranch, MessageSquare, Plus, Search, Sparkles } from 'lucide-react';
import { useAIWorkspaceStore } from '../store/useAIWorkspaceStore';
import { useSessionStore } from '../store/useSessionStore';

const starterSessions = [
  'Schema generation review',
  'Pydantic SDK runner',
  'Result table component audit',
];

export function SessionSidebar() {
  const { setSessionSidebarCollapsed } = useAIWorkspaceStore();
  const { sessions, activeSessionId, createSession, selectSession } = useSessionStore();

  return (
    <div className="flex h-full flex-col bg-zinc-950">
      <div className="border-b border-zinc-800 px-3 py-3">
        <div className="flex items-center justify-between">
          <div className="flex min-w-0 items-center gap-2">
            <div className="flex size-7 items-center justify-center rounded-md bg-blue-600 text-white">
              <Sparkles className="size-3.5" />
            </div>
            <div className="min-w-0">
              <p className="truncate text-sm font-semibold text-zinc-100">Nimbalyst</p>
              <p className="text-xs text-zinc-500">Agent workspace</p>
            </div>
          </div>
          <button
            type="button"
            title="Collapse sidebar"
            onClick={() => setSessionSidebarCollapsed(true)}
            className="flex size-7 items-center justify-center rounded text-zinc-400 transition-colors hover:bg-zinc-800 hover:text-zinc-200"
          >
            <ChevronLeft className="size-4" />
          </button>
        </div>

        <button
          type="button"
          onClick={createSession}
          className="mt-3 flex h-8 w-full items-center justify-center gap-2 rounded-md border border-zinc-700 bg-zinc-900 text-xs font-medium text-zinc-200 transition-colors hover:border-blue-500/70 hover:bg-blue-950/30 hover:text-blue-200"
        >
          <Plus className="size-3.5" />
          New agent session
        </button>

        <div className="mt-3 flex h-8 items-center gap-2 rounded-md border border-zinc-800 bg-zinc-900 px-2 text-zinc-500">
          <Search className="size-3.5" />
          <span className="text-xs">Search sessions</span>
        </div>
      </div>

      <div className="flex-1 overflow-y-auto py-2">
        <div className="px-3 pb-2">
          <div className="flex items-center gap-1.5 text-[11px] font-semibold uppercase tracking-wide text-zinc-500">
            <GitBranch className="size-3" />
            Workstreams
          </div>
          <div className="mt-2 space-y-1">
            {starterSessions.map((title) => (
              <div
                key={title}
                className="rounded-md border border-zinc-800 bg-zinc-900/60 px-2.5 py-2"
              >
                <p className="truncate text-xs font-medium text-zinc-300">{title}</p>
                <p className="mt-1 text-[11px] text-zinc-600">Ready</p>
              </div>
            ))}
          </div>
        </div>

        <div className="mt-2 border-t border-zinc-800 px-3 pt-3">
          <div className="flex items-center gap-1.5 text-[11px] font-semibold uppercase tracking-wide text-zinc-500">
            <Clock3 className="size-3" />
            Recent sessions
          </div>
          <div className="mt-2 space-y-1">
            {sessions.length === 0 ? (
              <div className="flex h-24 flex-col items-center justify-center gap-2 rounded-md border border-dashed border-zinc-800 text-zinc-600">
                <MessageSquare className="size-4" />
                <p className="text-xs">No saved sessions</p>
              </div>
            ) : (
              sessions.map((session) => (
                <button
                  key={session.id}
                  type="button"
                  onClick={() => selectSession(session.id)}
                  className={`w-full rounded-md px-2.5 py-2 text-left transition-colors ${
                    session.id === activeSessionId
                      ? 'bg-blue-950/40 text-blue-100 ring-1 ring-blue-500/40'
                      : 'text-zinc-300 hover:bg-zinc-900'
                  }`}
                >
                  <p className="truncate text-xs font-medium">{session.title}</p>
                  <p className="mt-1 text-[11px] text-zinc-500">{session.messageCount} messages</p>
                </button>
              ))
            )}
          </div>
        </div>
      </div>
    </div>
  );
}
