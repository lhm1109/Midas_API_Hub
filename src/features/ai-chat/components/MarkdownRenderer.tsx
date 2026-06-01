import { useEffect } from 'react';
import ReactMarkdown from 'react-markdown';
import remarkGfm from 'remark-gfm';
import { Prism as SyntaxHighlighter } from 'react-syntax-highlighter';

// Nimbalyst MarkdownRenderer 기반 (MIT License)
// Electron IPC 제거, contributions/styles hook 제거, CSS 변수 이 앱 테마로 맞춤

const injectStyles = () => {
  const styleId = 'ai-markdown-renderer-styles';
  if (document.getElementById(styleId)) return;
  const style = document.createElement('style');
  style.id = styleId;
  style.textContent = `
    .ai-md .token { background: none !important; }
    .ai-md .token.comment, .ai-md .token.prolog, .ai-md .token.doctype { color: #71717a; font-style: italic; }
    .ai-md .token.punctuation { color: #a1a1aa; }
    .ai-md .token.property, .ai-md .token.boolean, .ai-md .token.number,
    .ai-md .token.constant, .ai-md .token.deleted { color: #60a5fa; }
    .ai-md .token.selector, .ai-md .token.string, .ai-md .token.char,
    .ai-md .token.inserted { color: #86efac; }
    .ai-md .token.atrule, .ai-md .token.attr-value, .ai-md .token.keyword { color: #c084fc; font-weight: 500; }
    .ai-md .token.function, .ai-md .token.class-name { color: #f9fafb; font-weight: 500; }
    .ai-md .token.regex, .ai-md .token.variable { color: #fbbf24; }
    .ai-md pre::-webkit-scrollbar { height: 6px; }
    .ai-md pre::-webkit-scrollbar-track { background: #27272a; border-radius: 4px; }
    .ai-md pre::-webkit-scrollbar-thumb { background: #52525b; border-radius: 4px; }
    .ai-md pre::-webkit-scrollbar-thumb:hover { background: #71717a; }
    .ai-md-wrap-toggle { opacity: 0; transition: opacity 0.15s; }
    .ai-md-overflow:hover .ai-md-wrap-toggle { opacity: 1; }
  `;
  document.head.appendChild(style);
};

if (typeof document !== 'undefined') injectStyles();

interface MarkdownRendererProps {
  content: string;
  isUser?: boolean;
  isStreaming?: boolean;
}

export function MarkdownRenderer({ content, isUser = false, isStreaming = false }: MarkdownRendererProps) {
  useEffect(() => { injectStyles(); }, []);

  return (
    <div className="ai-md text-sm leading-relaxed text-zinc-100">
      <ReactMarkdown
        remarkPlugins={[remarkGfm]}
        components={{
          code({ node, className, children, ...props }) {
            const match = /language-(\w+)/.exec(className || '');
            const inline = !match && !String(children).includes('\n');
            if (inline) {
              return (
                <code
                  className="px-1.5 py-0.5 rounded text-xs font-mono bg-zinc-800 text-zinc-200"
                  {...props}
                >
                  {children}
                </code>
              );
            }
            return (
              <div className="ai-md-overflow relative my-2 rounded-lg overflow-hidden">
                <div className="flex items-center justify-between bg-zinc-800 px-3 py-1.5">
                  <span className="text-xs text-zinc-400 font-mono">{match?.[1] ?? 'code'}</span>
                </div>
                <SyntaxHighlighter
                  language={match?.[1] ?? 'text'}
                  PreTag="div"
                  customStyle={{
                    margin: 0,
                    padding: '0.75rem 1rem',
                    background: '#18181b',
                    fontSize: '0.8125rem',
                    lineHeight: '1.6',
                    overflowX: 'auto',
                  }}
                  codeTagProps={{ style: { fontFamily: 'ui-monospace, monospace' } }}
                >
                  {String(children).replace(/\n$/, '')}
                </SyntaxHighlighter>
              </div>
            );
          },
          h1: ({ children }) => <h1 className="text-lg font-bold mt-4 mb-2 text-zinc-100">{children}</h1>,
          h2: ({ children }) => <h2 className="text-base font-bold mt-3 mb-2 text-zinc-100">{children}</h2>,
          h3: ({ children }) => <h3 className="text-sm font-semibold mt-3 mb-1.5 text-zinc-100">{children}</h3>,
          p: ({ children }) => (
            <p className={`mt-1.5 mb-1.5 leading-relaxed text-zinc-200 ${isUser ? 'whitespace-pre-wrap' : ''}`}>
              {children}
            </p>
          ),
          a: ({ href, children }) => (
            <a href={href} target="_blank" rel="noopener noreferrer" className="text-blue-400 underline">
              {children}
            </a>
          ),
          ul: ({ children }) => <ul className="mt-1.5 mb-1.5 pl-5 list-disc text-zinc-200">{children}</ul>,
          ol: ({ children }) => <ol className="mt-1.5 mb-1.5 pl-5 list-decimal text-zinc-200">{children}</ol>,
          li: ({ children }) => <li className="mt-1 mb-1 leading-relaxed">{children}</li>,
          blockquote: ({ children }) => (
            <blockquote className="border-l-4 border-zinc-600 pl-3 my-2 text-zinc-400 italic">
              {children}
            </blockquote>
          ),
          table: ({ children }) => (
            <div className="overflow-x-auto my-2">
              <table className="w-full text-sm border border-zinc-700 border-collapse">{children}</table>
            </div>
          ),
          thead: ({ children }) => <thead className="bg-zinc-800 border-b-2 border-zinc-700">{children}</thead>,
          tr: ({ children }) => <tr className="border-b border-zinc-700">{children}</tr>,
          th: ({ children }) => <th className="px-3 py-2 text-left font-semibold text-zinc-200">{children}</th>,
          td: ({ children }) => <td className="px-3 py-2 text-zinc-300">{children}</td>,
          hr: () => <hr className="border-t border-zinc-700 my-3" />,
          strong: ({ children }) => <strong className="font-bold text-zinc-100">{children}</strong>,
          em: ({ children }) => <em className="italic text-zinc-300">{children}</em>,
        }}
      >
        {content}
      </ReactMarkdown>
      {isStreaming && (
        <span className="inline-block w-1.5 h-4 bg-blue-400 ml-0.5 animate-pulse align-middle" />
      )}
    </div>
  );
}
