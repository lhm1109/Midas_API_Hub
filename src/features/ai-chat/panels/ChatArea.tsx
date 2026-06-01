import { useEffect, useRef } from 'react';
import {
  Bot,
  Boxes,
  ChevronDown,
  Code2,
  FileJson,
  PanelRight,
  PanelRightOpen,
  Route,
  Sparkles,
} from 'lucide-react';
import { useAIWorkspaceStore } from '../store/useAIWorkspaceStore';
import { useSessionStore } from '../store/useSessionStore';
import { MessageBubble } from '../components/MessageBubble';
import { ChatInput } from '../components/ChatInput';

const quickStarts = [
  { icon: FileJson, title: 'Generate schema', body: 'Create an Argument wrapper schema for the selected endpoint.' },
  { icon: Code2, title: 'Build SDK', body: 'Generate Pydantic models and an SDK-style helper.' },
  { icon: Route, title: 'Plan tests', body: 'List boundary cases and runner payloads for this API.' },
];

export function ChatArea() {
  const messagesEndRef = useRef<HTMLDivElement>(null);
  const {
    artifactPanelVisible,
    setArtifactPanelVisible,
    sessionSidebarCollapsed,
    setSessionSidebarCollapsed,
  } = useAIWorkspaceStore();
  const { activeSessionId, getActiveMessages } = useSessionStore();
  const messages = getActiveMessages();

  useEffect(() => {
    messagesEndRef.current?.scrollIntoView({ behavior: 'smooth' });
  }, [messages]);

  return (
    <div className="flex h-full flex-col bg-zinc-950">
      <div className="flex h-12 flex-shrink-0 items-center justify-between border-b border-zinc-800 bg-zinc-900 px-4">
        <div className="flex min-w-0 items-center gap-2">
          {sessionSidebarCollapsed && (
            <button
              type="button"
              onClick={() => setSessionSidebarCollapsed(false)}
              className="flex size-7 items-center justify-center rounded text-zinc-400 transition-colors hover:bg-zinc-800 hover:text-zinc-200"
              title="Open sessions"
            >
              <PanelRight className="size-4" />
            </button>
          )}
          <div className="flex size-7 items-center justify-center rounded-md bg-blue-600 text-white">
            <Bot className="size-3.5" />
          </div>
          <div className="min-w-0">
            <div className="flex items-center gap-2">
              <span className="truncate text-sm font-semibold text-zinc-100">
                {activeSessionId ? 'Agent Session' : 'New Agent Session'}
              </span>
              <span className="rounded border border-zinc-700 px-1.5 py-0.5 text-[10px] font-medium text-zinc-400">
                gpt-4o
              </span>
            </div>
            <p className="text-xs text-zinc-500">API schema, Pydantic, runner, and manual context</p>
          </div>
        </div>

        <div className="flex items-center gap-1.5">
          <button className="flex h-7 items-center gap-1 rounded border border-zinc-700 px-2 text-xs text-zinc-300 hover:bg-zinc-800">
            Agent
            <ChevronDown className="size-3" />
          </button>
          <button
            type="button"
            onClick={() => setArtifactPanelVisible(!artifactPanelVisible)}
            className={`flex size-7 items-center justify-center rounded transition-colors ${
              artifactPanelVisible
                ? 'bg-blue-950/50 text-blue-300'
                : 'text-zinc-400 hover:bg-zinc-800 hover:text-zinc-200'
            }`}
            title="Files panel"
          >
            <PanelRightOpen className="size-4" />
          </button>
        </div>
      </div>

      <div className="flex-1 overflow-y-auto">
        {messages.length === 0 ? (
          <EmptyState />
        ) : (
          <div className="mx-auto max-w-4xl py-3">
            {messages.map((msg) => (
              <MessageBubble key={msg.id} message={msg} />
            ))}
            <div ref={messagesEndRef} />
          </div>
        )}
      </div>

      <ChatInput />
    </div>
  );
}

function EmptyState() {
  return (
    <div className="flex h-full flex-col items-center justify-center px-6">
      <div className="w-full max-w-3xl">
        <div className="mb-6 flex items-center gap-3">
          <div className="flex size-10 items-center justify-center rounded-xl bg-blue-600 text-white">
            <Sparkles className="size-5" />
          </div>
          <div>
            <h1 className="text-xl font-semibold text-zinc-100">Nimbalyst Agent Workspace</h1>
            <p className="text-sm text-zinc-500">Start with an API task, then keep generated files in the right panel.</p>
          </div>
        </div>

        <div className="grid gap-3 md:grid-cols-3">
          {quickStarts.map((item) => {
            const Icon = item.icon;
            return (
              <div key={item.title} className="rounded-lg border border-zinc-800 bg-zinc-900/70 p-4">
                <div className="mb-3 flex size-8 items-center justify-center rounded-md bg-zinc-800 text-blue-300">
                  <Icon className="size-4" />
                </div>
                <p className="text-sm font-medium text-zinc-200">{item.title}</p>
                <p className="mt-1 text-xs leading-5 text-zinc-500">{item.body}</p>
              </div>
            );
          })}
        </div>

        <div className="mt-4 flex items-center gap-2 rounded-lg border border-zinc-800 bg-zinc-900/60 px-3 py-2 text-xs text-zinc-500">
          <Boxes className="size-3.5 text-zinc-400" />
          Context sources: selected project endpoint, spec tab schema, runner payload, generated Pydantic files.
        </div>
      </div>
    </div>
  );
}
