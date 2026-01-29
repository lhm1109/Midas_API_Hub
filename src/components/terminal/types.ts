/**
 * Type definitions for Electron terminal API
 */

export interface TerminalCreateOptions {
    name?: string;
    cwd?: string;
    cols?: number;
    rows?: number;
    env?: Record<string, string>;
}

export interface TerminalInfo {
    id: number;
    serverId?: string;
    name: string;
    cwd: string;
}

export interface ElectronTerminalAPI {
    create: (options?: TerminalCreateOptions) => Promise<{ success: boolean; id?: number; name?: string; error?: string }>;
    write: (id: number, data: string) => Promise<{ success: boolean; error?: string }>;
    resize: (id: number, cols: number, rows: number) => Promise<{ success: boolean; error?: string }>;
    kill: (id: number) => Promise<{ success: boolean; error?: string }>;
    list: () => Promise<{ success: boolean; data?: TerminalInfo[]; error?: string }>;
    getLogs: (id: number) => Promise<{ success: boolean; data?: string; error?: string }>;
    killPort: (port: number) => Promise<{ success: boolean; message?: string; error?: string }>;
    syncState: () => Promise<{ success: boolean; data?: TerminalInfo[]; error?: string }>;
    setServerId: (id: number, serverId: string) => Promise<{ success: boolean; error?: string }>;
    onData: (callback: (payload: { id: number; data: string }) => void) => void;
    onExit: (callback: (payload: { id: number; exitCode: number }) => void) => void;
    removeListeners: () => void;
}

declare global {
    interface Window {
        electronAPI?: {
            terminal?: ElectronTerminalAPI;
            // ... other APIs
        };
    }
}

export const isElectron = (): boolean => {
    return typeof window !== 'undefined' && !!window.electronAPI?.terminal;
};

export const terminalAPI = (): ElectronTerminalAPI | null => {
    if (isElectron()) {
        return window.electronAPI!.terminal!;
    }
    return null;
};
