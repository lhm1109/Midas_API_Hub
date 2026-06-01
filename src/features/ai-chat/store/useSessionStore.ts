import { create } from 'zustand';
import { persist } from 'zustand/middleware';

export interface Session {
  id: string;
  title: string;
  model: string;
  messageCount: number;
  createdAt: number;
  updatedAt: number;
}

export interface ChatMessage {
  id: string;
  sessionId: string;
  role: 'user' | 'assistant';
  content: string;
  isStreaming?: boolean;
  createdAt: number;
}

interface SessionState {
  sessions: Session[];
  activeSessionId: string | null;
  messagesBySession: Record<string, ChatMessage[]>;
  isLoading: boolean;
  error: string | null;

  getActiveMessages: () => ChatMessage[];

  createSession: () => string;
  selectSession: (id: string) => void;
  deleteSession: (id: string) => void;
  updateSessionTitle: (id: string, title: string) => void;

  addMessage: (msg: Omit<ChatMessage, 'id' | 'createdAt'>) => string;
  updateMessage: (id: string, content: string, isStreaming?: boolean) => void;

  setLoading: (v: boolean) => void;
  setError: (v: string | null) => void;
}

export const useSessionStore = create<SessionState>()(
  persist(
    (set, get) => ({
      sessions: [],
      activeSessionId: null,
      messagesBySession: {},
      isLoading: false,
      error: null,

      getActiveMessages: () => {
        const { activeSessionId, messagesBySession } = get();
        if (!activeSessionId) return [];
        return (messagesBySession[activeSessionId] ?? []).filter((m) => !m.isStreaming || m.content);
      },

      createSession: () => {
        const id = crypto.randomUUID();
        const newSession: Session = {
          id,
          title: 'New Session',
          model: 'gpt-4o',
          messageCount: 0,
          createdAt: Date.now(),
          updatedAt: Date.now(),
        };
        set((s) => ({
          sessions: [newSession, ...s.sessions],
          activeSessionId: id,
          messagesBySession: { ...s.messagesBySession, [id]: [] },
        }));
        return id;
      },

      selectSession: (id) => set({ activeSessionId: id, error: null }),

      deleteSession: (id) =>
        set((s) => {
          const sessions = s.sessions.filter((session) => session.id !== id);
          const { [id]: _deleted, ...messagesBySession } = s.messagesBySession;
          return {
            sessions,
            messagesBySession,
            activeSessionId: s.activeSessionId === id ? (sessions[0]?.id ?? null) : s.activeSessionId,
          };
        }),

      updateSessionTitle: (id, title) =>
        set((s) => ({
          sessions: s.sessions.map((session) =>
            session.id === id ? { ...session, title, updatedAt: Date.now() } : session
          ),
        })),

      addMessage: (msg) => {
        const id = crypto.randomUUID();
        const message: ChatMessage = { ...msg, id, createdAt: Date.now() };
        set((s) => {
          const prev = s.messagesBySession[msg.sessionId] ?? [];
          const sessions = s.sessions.map((session) =>
            session.id === msg.sessionId
              ? { ...session, messageCount: prev.length + 1, updatedAt: Date.now() }
              : session
          );
          return {
            sessions,
            messagesBySession: { ...s.messagesBySession, [msg.sessionId]: [...prev, message] },
          };
        });
        return id;
      },

      updateMessage: (id, content, isStreaming) =>
        set((s) => {
          const messagesBySession: Record<string, ChatMessage[]> = {};
          for (const [sessionId, messages] of Object.entries(s.messagesBySession)) {
            messagesBySession[sessionId] = messages.map((message) =>
              message.id === id ? { ...message, content, isStreaming: isStreaming ?? false } : message
            );
          }
          return { messagesBySession };
        }),

      setLoading: (v) => set({ isLoading: v }),
      setError: (v) => set({ error: v }),
    }),
    {
      name: 'ai-sessions',
      partialize: (s) => ({
        sessions: s.sessions,
        activeSessionId: s.activeSessionId,
        messagesBySession: Object.fromEntries(
          Object.entries(s.messagesBySession).map(([sessionId, messages]) => [
            sessionId,
            messages.filter((message) => !message.isStreaming),
          ])
        ),
      }),
    }
  )
);
