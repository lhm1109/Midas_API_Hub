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

export interface ZendeskEnvConfig {
    baseUrl?: string;
    subdomain: string;
    defaultLocale: string;
    defaultArticleUrl: string;
    defaultArticleId?: string;
    defaultSectionId?: number | null;
    defaultPermissionGroupId?: number | null;
    defaultUserSegmentId?: number | null;
    defaultUserSegmentIds?: number[];
    defaultLabels?: string[];
    defaultContentTagIds?: string[];
    defaultPromoted?: boolean | null;
    defaultCommentsDisabled?: boolean | null;
    defaultNotifySubscribers?: boolean | null;
    defaultDraft?: boolean | null;
    defaultAttachmentIds?: number[];
    hasCredentials: boolean;
    authType: 'token' | 'password' | null;
    missingFields: string[];
}

export interface ElectronZendeskAPI {
    getEnvConfig: () => Promise<{ success: boolean; data?: ZendeskEnvConfig; error?: string }>;
    getAllTranslations: (config: any, articleId: string) => Promise<{ success: boolean; data?: any; error?: string }>;
    getArticleTranslation: (config: any, articleId: string, locale: string) => Promise<{ success: boolean; data?: any; error?: string }>;
    updateArticleTranslation: (
        config: any,
        articleId: string,
        locale: string,
        body: string,
        title?: string,
        draft?: boolean
    ) => Promise<{ success: boolean; data?: any; error?: string }>;
    updateArticleTranslationWithEnv: (
        articleId: string,
        locale: string,
        body: string,
        title?: string,
        draft?: boolean
    ) => Promise<{ success: boolean; data?: any; error?: string }>;
    publishManualWithEnv: (
        targetInput: string,
        locale: string,
        body: string,
        title?: string,
        draft?: boolean,
        options?: {
            labelNames?: string[];
            commentsDisabled?: boolean;
        }
    ) => Promise<{
        success: boolean;
        data?: {
            mode: 'create' | 'update';
            articleId: string;
            locale: string;
            articleUrl: string;
            associatedAttachmentCount: number;
        };
        error?: string;
    }>;
    getAllArticles: (config: any, perPage?: number, page?: number) => Promise<{ success: boolean; data?: any; error?: string }>;
    getArticlesBySection: (config: any, sectionId: string, locale?: string) => Promise<{ success: boolean; data?: any; error?: string }>;
}

declare global {
    interface Window {
        electronAPI?: {
            invoke?: (channel: string, ...args: any[]) => Promise<any>;
            terminal?: ElectronTerminalAPI;
            zendesk?: ElectronZendeskAPI;
            locks?: {
                releaseAll: (userId: string) => Promise<{ success: boolean; error?: string }>;
            };
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
