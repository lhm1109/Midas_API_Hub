import { type KeyboardEvent, useEffect, useRef, useState } from 'react';
import { AtSign, Paperclip, SendHorizontal, Square, TerminalSquare } from 'lucide-react';
import { useSessionStore } from '../store/useSessionStore';

export function ChatInput() {
  const [value, setValue] = useState('');
  const textareaRef = useRef<HTMLTextAreaElement>(null);
  const {
    activeSessionId,
    createSession,
    addMessage,
    updateSessionTitle,
    isLoading,
  } = useSessionStore();

  useEffect(() => {
    const el = textareaRef.current;
    if (!el) return;
    el.style.height = 'auto';
    el.style.height = `${Math.min(el.scrollHeight, 180)}px`;
  }, [value]);

  const handleSend = () => {
    const prompt = value.trim();
    if (!prompt || isLoading) return;

    const sessionId = activeSessionId ?? createSession();
    addMessage({ sessionId, role: 'user', content: prompt });
    updateSessionTitle(sessionId, prompt.slice(0, 48));
    setValue('');
  };

  const handleKeyDown = (event: KeyboardEvent<HTMLTextAreaElement>) => {
    if (event.key === 'Enter' && !event.shiftKey) {
      event.preventDefault();
      handleSend();
    }
  };

  return (
    <div className="flex-shrink-0 border-t border-zinc-800 bg-zinc-950 px-4 py-3">
      <div className="mx-auto max-w-4xl rounded-xl border border-zinc-700 bg-zinc-900 shadow-lg shadow-black/20 transition-colors focus-within:border-blue-500/70">
        <div className="flex items-center gap-1 border-b border-zinc-800 px-2 py-1.5">
          <button className="rounded-md bg-blue-950/60 px-2 py-1 text-xs font-medium text-blue-200">Agent</button>
          <button className="rounded-md px-2 py-1 text-xs text-zinc-500 hover:bg-zinc-800 hover:text-zinc-300">Plan</button>
          <div className="mx-1 h-4 w-px bg-zinc-800" />
          <button className="flex items-center gap-1 rounded-md px-2 py-1 text-xs text-zinc-500 hover:bg-zinc-800 hover:text-zinc-300">
            <TerminalSquare className="size-3" />
            Commands
          </button>
        </div>

        <div className="flex items-end gap-2 px-3 py-2">
          <textarea
            ref={textareaRef}
            value={value}
            onChange={(event) => setValue(event.target.value)}
            onKeyDown={handleKeyDown}
            placeholder="Ask Nimbalyst to work on an API endpoint..."
            rows={1}
            className="min-h-[28px] flex-1 resize-none bg-transparent text-sm leading-relaxed text-zinc-100 outline-none placeholder:text-zinc-500"
          />
          <div className="mb-0.5 flex items-center gap-1">
            <button className="flex size-8 items-center justify-center rounded-lg text-zinc-500 hover:bg-zinc-800 hover:text-zinc-300">
              <AtSign className="size-4" />
            </button>
            <button className="flex size-8 items-center justify-center rounded-lg text-zinc-500 hover:bg-zinc-800 hover:text-zinc-300">
              <Paperclip className="size-4" />
            </button>
            <button
              type="button"
              onClick={isLoading ? undefined : handleSend}
              disabled={!isLoading && !value.trim()}
              className={`flex size-8 flex-shrink-0 items-center justify-center rounded-lg transition-colors ${
                isLoading
                  ? 'cursor-pointer bg-red-600 text-white hover:bg-red-500'
                  : value.trim()
                    ? 'bg-blue-600 text-white hover:bg-blue-500'
                    : 'cursor-not-allowed bg-zinc-800 text-zinc-600'
              }`}
            >
              {isLoading
                ? <Square className="size-3.5" />
                : <SendHorizontal className="size-3.5" />
              }
            </button>
          </div>
        </div>
      </div>
    </div>
  );
}
