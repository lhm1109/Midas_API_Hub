import { useState } from 'react';
import { Bot, Check, Copy, User } from 'lucide-react';
import { MarkdownRenderer } from './MarkdownRenderer';
import type { ChatMessage } from '../store/useSessionStore';

interface MessageBubbleProps {
  message: ChatMessage;
}

export function MessageBubble({ message }: MessageBubbleProps) {
  const [copied, setCopied] = useState(false);
  const isUser = message.role === 'user';

  const handleCopy = () => {
    navigator.clipboard.writeText(message.content);
    setCopied(true);
    setTimeout(() => setCopied(false), 1500);
  };

  return (
    <div className={`flex gap-3 px-4 py-3 group ${isUser ? 'flex-row-reverse' : 'flex-row'}`}>
      <div className={`flex-shrink-0 w-7 h-7 rounded-full flex items-center justify-center mt-0.5 ${
        isUser ? 'bg-blue-600' : 'bg-zinc-700'
      }`}>
        {isUser
          ? <User className="w-3.5 h-3.5 text-white" />
          : <Bot className="w-3.5 h-3.5 text-zinc-200" />
        }
      </div>

      <div className={`relative max-w-[85%] ${isUser ? 'items-end' : 'items-start'} flex flex-col`}>
        <div className={`rounded-2xl px-3.5 py-2.5 text-sm ${
          isUser
            ? 'bg-blue-600 text-white rounded-tr-sm'
            : 'bg-zinc-800 text-zinc-100 rounded-tl-sm'
        }`}>
          {isUser ? (
            <p className="whitespace-pre-wrap leading-relaxed">{message.content}</p>
          ) : (
            <MarkdownRenderer
              content={message.content || ''}
              isStreaming={message.isStreaming}
            />
          )}
        </div>

        {!isUser && !message.isStreaming && message.content && (
          <button
            type="button"
            onClick={handleCopy}
            className="mt-1 opacity-0 group-hover:opacity-100 transition-opacity flex items-center gap-1 text-xs text-zinc-500 hover:text-zinc-300 px-1"
          >
            {copied
              ? <><Check className="w-3 h-3" /> Copied</>
              : <><Copy className="w-3 h-3" /> Copy</>
            }
          </button>
        )}
      </div>
    </div>
  );
}
