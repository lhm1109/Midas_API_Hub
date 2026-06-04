import * as React from 'react';
import * as ReactDOMClient from 'react-dom/client';
import * as ReactJsxRuntime from 'react/jsx-runtime';
import { useCallback, useEffect, useMemo, useRef, useState } from 'react';
import yaml from 'js-yaml';
import {
  Bot,
  CheckCircle2,
  Cloud,
  Code2,
  FileArchive,
  FileCode2,
  FileJson,
  FileSpreadsheet,
  FileText,
  FolderOpen,
  Image,
  GitBranch,
  Loader2,
  Play,
  Plug,
  RefreshCw,
  Settings,
  Square,
  Table2,
  TerminalSquare,
  Workflow,
} from 'lucide-react';
import apiClient, {
  type NimbalystBackendModuleSummary,
  type NimbalystBackendPermissionDescriptor,
  type NimbalystCommandSummary,
  type NimbalystConfigurationProperty,
  type NimbalystExtensionBackendModules,
  type NimbalystExtensionConfiguration,
  type NimbalystExtensionContributionSummary,
  type NimbalystExtensionSummary,
  type NimbalystExtensionToolSummary,
  type NimbalystMarketplaceExtensionSummary,
  type NimbalystSlashCommandSummary,
} from '@/lib/api-client';
import { API_CONFIG } from '@/config/constants';

type NimbalystMode = 'files' | 'agent' | 'tracker' | 'collab' | 'extensions' | 'settings';

interface ReferenceStatus {
  referencePath: string;
  electronPath: string;
  exists: boolean;
  packageJson: { name: string; version: string; workspaces: string[] } | null;
  electronPackageJson: { name: string; version: string; main: string } | null;
  dependenciesInstalled: boolean;
  packageLockExists: boolean;
  node: {
    version: string;
    supported: boolean;
    required: string;
  };
  install: {
    running: boolean;
    startedAt: string | null;
    finishedAt: string | null;
    exit: { code: number | null; signal: string | null } | null;
    recentLog: string[];
  };
  runtimeDistExists: boolean;
  electronOutExists: boolean;
  windowsDevScriptExists: boolean;
  shellDevScriptExists: boolean;
  running: boolean;
  pid: number | null;
  launchedAt: string | null;
  recentLog: string[];
  suggestedCommands: string[];
}

interface FileEntry {
  name: string;
  path: string;
  isDirectory: boolean;
}

interface ActiveFile {
  path: string;
  content: string;
}

type NimbalystTrackerStatus = string;
type NimbalystTrackerPriority = string;

type NimbalystTrackerFieldType =
  | 'string'
  | 'text'
  | 'number'
  | 'select'
  | 'multiselect'
  | 'date'
  | 'datetime'
  | 'boolean'
  | 'user'
  | 'reference'
  | 'url'
  | 'array'
  | 'object';

interface NimbalystTrackerFieldOption {
  value: string;
  label: string;
  icon?: string;
  color?: string;
}

interface NimbalystTrackerFieldDefinition {
  name: string;
  type: NimbalystTrackerFieldType;
  required?: boolean;
  default?: unknown;
  displayInline?: boolean;
  readOnly?: boolean;
  min?: number;
  max?: number;
  minLength?: number;
  maxLength?: number;
  options?: NimbalystTrackerFieldOption[];
}

interface NimbalystTrackerSchema {
  type: string;
  displayName: string;
  displayNamePlural: string;
  icon: string;
  color: string;
  idPrefix: string;
  idFormat?: 'ulid' | 'uuid' | 'sequential';
  fields: NimbalystTrackerFieldDefinition[];
  roles?: Partial<Record<'title' | 'workflowStatus' | 'priority' | 'assignee' | 'reporter' | 'tags' | 'startDate' | 'dueDate' | 'progress', string>>;
  sync?: {
    mode: 'local' | 'shared' | 'hybrid';
    scope: 'project' | 'workspace';
  };
  sourcePath?: string;
}

interface NimbalystTrackerRecord {
  id: string;
  primaryType: string;
  typeTags: string[];
  issueKey?: string;
  source: 'native' | 'inline' | 'frontmatter' | 'import';
  sourceRef?: string;
  archived: boolean;
  syncStatus: 'local' | 'pending' | 'synced';
  system: {
    workspace: string;
    documentPath?: string;
    createdAt: string;
    updatedAt: string;
    activity?: Array<{
      id: string;
      action: 'created' | 'updated' | 'status_changed' | 'archived';
      field?: string;
      oldValue?: string;
      newValue?: string;
      timestamp: number;
    }>;
  };
  fields: {
    title: string;
    description?: string;
    status: NimbalystTrackerStatus;
    priority?: NimbalystTrackerPriority;
    tags?: string[];
    [key: string]: unknown;
  };
}

interface NimbalystTrackerStore {
  version: 1;
  updatedAt: string;
  items: NimbalystTrackerRecord[];
}

interface NimbalystSharedDocument {
  id: string;
  title: string;
  content: string;
  sourcePath?: string;
  createdAt: string;
  updatedAt: string;
}

interface NimbalystSharedDocumentStore {
  version: 1;
  updatedAt: string;
  documents: NimbalystSharedDocument[];
}

interface NimbalystThemeContribution {
  id: string;
  name: string;
  isDark: boolean;
  colors: Record<string, string>;
}

const modes: Array<{ id: NimbalystMode; label: string; icon: typeof FileText }> = [
  { id: 'files', label: 'Files', icon: FolderOpen },
  { id: 'agent', label: 'Agent', icon: Code2 },
  { id: 'tracker', label: 'Tracker', icon: Workflow },
  { id: 'collab', label: 'Shared Docs', icon: Cloud },
  { id: 'extensions', label: 'Extensions', icon: Plug },
  { id: 'settings', label: 'Settings', icon: Settings },
];

const DEFAULT_WORKSPACE_PATH = '';

export function NimbalystWorkspaceView() {
  const [mode, setMode] = useState<NimbalystMode>('files');
  const [workspacePath, setWorkspacePath] = useState(
    () => localStorage.getItem('nimbalyst-workspace-path') || DEFAULT_WORKSPACE_PATH
  );
  const [entries, setEntries] = useState<FileEntry[]>([]);
  const [activeFile, setActiveFile] = useState<ActiveFile | null>(null);
  const [status, setStatus] = useState<ReferenceStatus | null>(null);
  const [extensions, setExtensions] = useState<NimbalystExtensionSummary[]>([]);
  const [marketplaceExtensions, setMarketplaceExtensions] = useState<NimbalystMarketplaceExtensionSummary[]>([]);
  const [tools, setTools] = useState<NimbalystExtensionToolSummary[]>([]);
  const [registeredCommands, setRegisteredCommands] = useState<NimbalystCommandSummary[]>([]);
  const [slashCommands, setSlashCommands] = useState<NimbalystSlashCommandSummary[]>([]);
  const [contributions, setContributions] = useState<NimbalystExtensionContributionSummary[]>([]);
  const [toolArgs, setToolArgs] = useState('{}');
  const [toolResult, setToolResult] = useState<string>('');
  const [slashCommandId, setSlashCommandId] = useState<string | null>(null);
  const [commandResult, setCommandResult] = useState<string>('');
  const [slashArgs, setSlashArgs] = useState('');
  const [slashResult, setSlashResult] = useState<string>('');
  const [selectedPanelId, setSelectedPanelId] = useState<string | null>(null);
  const [panelFrameVisible, setPanelFrameVisible] = useState(true);
  const [activeThemeId, setActiveThemeId] = useState(
    () => localStorage.getItem('nimbalyst-extension-theme-id') || ''
  );
  const [marketplaceInstallSummary, setMarketplaceInstallSummary] = useState('');
  const [busy, setBusy] = useState<string | null>(null);
  const [error, setError] = useState<string | null>(null);
  const autoActivatedExtensionIdsRef = useRef<Set<string>>(new Set());

  const selectedTool = tools[0];
  const selectedSlashCommand =
    slashCommands.find((command) => command.id === slashCommandId) || slashCommands[0];
  const newFileContributions = useMemo(
    () => contributions.filter((contribution) => contribution.type === 'newFileMenu'),
    [contributions]
  );
  const fileIconContributions = useMemo(
    () => contributions.filter((contribution) => contribution.type === 'fileIcons'),
    [contributions]
  );
  const customEditorContributions = useMemo(
    () => contributions.filter((contribution) => contribution.type === 'customEditors'),
    [contributions]
  );
  const documentHeaderContributions = useMemo(
    () => contributions.filter((contribution) => contribution.type === 'documentHeaders'),
    [contributions]
  );
  const panelContributions = useMemo(
    () => contributions.filter((contribution) => contribution.type === 'panels'),
    [contributions]
  );
  const commandContributions = useMemo(
    () => contributions.filter((contribution) => contribution.type === 'commands'),
    [contributions]
  );
  const keybindingContributions = useMemo(
    () => contributions.filter((contribution) => contribution.type === 'keybindings'),
    [contributions]
  );
  const settingsPanelContributions = useMemo(
    () => contributions.filter((contribution) => contribution.type === 'settingsPanel'),
    [contributions]
  );
  const configurationContributions = useMemo(
    () => contributions.filter((contribution) => contribution.type === 'configuration'),
    [contributions]
  );
  const backendModuleContributions = useMemo(
    () => contributions.filter((contribution) => contribution.type === 'backendModules'),
    [contributions]
  );
  const agentWorkflowContributions = useMemo(
    () => contributions.filter((contribution) => contribution.type === 'agentWorkflows'),
    [contributions]
  );
  const claudePluginContributions = useMemo(
    () => contributions.filter((contribution) => contribution.type === 'claudePlugin'),
    [contributions]
  );
  const themeContributions = useMemo(
    () => contributions.filter((contribution) => contribution.type === 'themes'),
    [contributions]
  );
  const hostComponentContributions = useMemo(
    () => contributions.filter((contribution) => contribution.type === 'hostComponents'),
    [contributions]
  );
  const activeTheme = useMemo(
    () => findThemeContribution(activeThemeId, themeContributions),
    [activeThemeId, themeContributions]
  );
  const panelToggleCommands = useMemo(
    () =>
      panelContributions.map((panel) => ({
        id: `${panel.extensionId}.${panel.id}.toggle`,
        title: `Toggle ${panel.title} Panel`,
        extensionId: panel.extensionId,
        extensionName: panel.extensionName,
        panel,
      })),
    [panelContributions]
  );
  const activeCustomEditor = activeFile
    ? findCustomEditorContribution(activeFile.path, customEditorContributions)
    : undefined;
  const activeDocumentHeaders = activeFile
    ? findDocumentHeaderContributions(activeFile.path, documentHeaderContributions)
    : [];
  const selectedPanel =
    panelContributions.find((contribution) => `${contribution.extensionId}:${contribution.id}` === selectedPanelId) ||
    panelContributions[0];
  const visibleSelectedPanel = panelFrameVisible ? selectedPanel : undefined;

  const executeNimbalystCommand = useCallback(
    async (commandId: string) => {
      const panelCommand = panelToggleCommands.find((command) => command.id === commandId);
      if (panelCommand) {
        setMode('extensions');
        const nextPanelId = `${panelCommand.panel.extensionId}:${panelCommand.panel.id}`;
        setSelectedPanelId((currentPanelId) => {
          setPanelFrameVisible(!(currentPanelId === nextPanelId && panelFrameVisible));
          return nextPanelId;
        });
        return true;
      }
      const result = await apiClient.executeNimbalystCommand({
        commandId,
        args: {},
        workspacePath,
        activeFilePath: activeFile?.path,
      });
      if (result.error) {
        setError(result.error);
        setCommandResult(JSON.stringify({ success: false, error: result.error }, null, 2));
        return false;
      }
      setCommandResult(JSON.stringify(result.data, null, 2));
      return result.data?.success !== false;
    },
    [activeFile?.path, panelFrameVisible, panelToggleCommands, workspacePath]
  );

  useEffect(() => {
    (window as any).__BAT_RACK_REACT__ = React;
    (window as any).__BAT_RACK_REACT_DOM_CLIENT__ = ReactDOMClient;
    (window as any).__BAT_RACK_REACT_JSX_RUNTIME__ = ReactJsxRuntime;
    (window as any).__nimbalyst_extensions = {
      react: React,
      'react-dom/client': ReactDOMClient,
    };
  }, []);

  useEffect(() => {
    if (keybindingContributions.length === 0) return undefined;

    const handleKeyDown = (event: KeyboardEvent) => {
      if (isEditableEventTarget(event.target)) return;

      for (const contribution of keybindingContributions) {
        const raw = contribution.raw as { key?: string; command?: string };
        if (!raw?.key || !raw?.command) continue;
        if (!eventMatchesManifestKey(event, raw.key)) continue;

        event.preventDefault();
        event.stopPropagation();
        void executeNimbalystCommand(raw.command);
        return;
      }
    };

    window.addEventListener('keydown', handleKeyDown, { capture: true });
    return () => window.removeEventListener('keydown', handleKeyDown, { capture: true });
  }, [executeNimbalystCommand, keybindingContributions]);

  const loadReferenceStatus = useCallback(async () => {
    const response = await fetch(`${API_CONFIG.BASE_URL}/nimbalyst/reference/status`);
    if (!response.ok) throw new Error(`Reference status failed: ${response.status}`);
    setStatus(await response.json());
  }, []);

  const loadExtensions = useCallback(async () => {
    const [extensionResult, marketplaceResult, toolResult, commandResult, slashCommandResult, contributionResult] = await Promise.all([
      apiClient.listNimbalystExtensions(),
      apiClient.listNimbalystMarketplaceExtensions(),
      apiClient.listNimbalystExtensionTools(),
      apiClient.listNimbalystCommands(),
      apiClient.listNimbalystSlashCommands(),
      apiClient.listNimbalystExtensionContributions(),
    ]);
    if (extensionResult.error) throw new Error(extensionResult.error);
    if (marketplaceResult.error) throw new Error(marketplaceResult.error);
    if (toolResult.error) throw new Error(toolResult.error);
    if (commandResult.error) throw new Error(commandResult.error);
    if (slashCommandResult.error) throw new Error(slashCommandResult.error);
    if (contributionResult.error) throw new Error(contributionResult.error);
    setExtensions(extensionResult.data?.extensions || []);
    setMarketplaceExtensions(marketplaceResult.data?.extensions || []);
    setTools(toolResult.data?.tools || []);
    setRegisteredCommands(commandResult.data?.commands || []);
    setSlashCommands(slashCommandResult.data?.commands || []);
    setContributions(contributionResult.data?.contributions || []);
  }, []);

  const refreshAll = useCallback(async () => {
    setError(null);
    try {
      await Promise.all([loadReferenceStatus(), loadExtensions()]);
    } catch (err) {
      setError(err instanceof Error ? err.message : String(err));
    }
  }, [loadExtensions, loadReferenceStatus]);

  const installMarketplaceExtension = useCallback(
    async (extensionId: string, buildFirst = false) => {
      setBusy(`marketplace:${extensionId}`);
      setError(null);
      try {
        const result = buildFirst
          ? await apiClient.buildNimbalystMarketplaceExtension(extensionId)
          : await apiClient.installNimbalystMarketplaceExtension(extensionId);
        if (result.error) throw new Error(result.error);
        await refreshAll();
      } catch (err) {
        setError(err instanceof Error ? err.message : String(err));
      } finally {
        setBusy(null);
      }
    },
    [refreshAll]
  );

  const installAllMarketplaceExtensions = useCallback(async () => {
    if (!status?.dependenciesInstalled) {
      setMarketplaceInstallSummary(
        JSON.stringify(
          {
            installed: 0,
            failed: 0,
            pending: 'Run Reference Source > Install first, then build and install bundled extensions.',
          },
          null,
          2
        )
      );
      return;
    }
    setBusy('marketplace:all');
    setError(null);
    setMarketplaceInstallSummary('');
    try {
      const result = await apiClient.buildAllNimbalystMarketplaceExtensions();
      if (result.error) throw new Error(result.error);
      const payload = result.data;
      setMarketplaceInstallSummary(
        JSON.stringify(
          {
            installed: payload?.installed ?? 0,
            failed: payload?.failed ?? 0,
            failures: (payload?.results || [])
              .filter((item) => !item.success)
              .map((item) => ({ id: item.id, error: item.error })),
          },
          null,
          2
        )
      );
      await refreshAll();
    } catch (err) {
      setError(err instanceof Error ? err.message : String(err));
    } finally {
      setBusy(null);
    }
  }, [refreshAll, status?.dependenciesInstalled]);

  const loadWorkspaceEntries = useCallback(async (targetPath: string) => {
    const electronAPI = (window as any).electronAPI;
    if (!targetPath || !electronAPI?.readDirectory) {
      setEntries([]);
      return;
    }

    const result = await electronAPI.readDirectory(targetPath);
    if (!result?.success) {
      throw new Error(result?.error || 'Failed to read workspace directory.');
    }

    setEntries(
      (result.items || [])
        .filter((item: FileEntry) => !item.name.startsWith('.'))
        .sort((a: FileEntry, b: FileEntry) => Number(b.isDirectory) - Number(a.isDirectory) || a.name.localeCompare(b.name))
        .slice(0, 200)
    );
  }, []);

  useEffect(() => {
    refreshAll();
  }, [refreshAll]);

  useEffect(() => {
    if (!status?.install?.running && !status?.running) return undefined;
    const interval = window.setInterval(() => {
      loadReferenceStatus().catch((err) => setError(err instanceof Error ? err.message : String(err)));
    }, 2000);
    return () => window.clearInterval(interval);
  }, [loadReferenceStatus, status?.install?.running, status?.running]);

  useEffect(() => {
    if (!workspacePath) return;
    localStorage.setItem('nimbalyst-workspace-path', workspacePath);
    loadWorkspaceEntries(workspacePath).catch((err) => setError(err instanceof Error ? err.message : String(err)));
  }, [loadWorkspaceEntries, workspacePath]);

  useEffect(() => {
    if (activeThemeId) {
      localStorage.setItem('nimbalyst-extension-theme-id', activeThemeId);
    } else {
      localStorage.removeItem('nimbalyst-extension-theme-id');
    }
  }, [activeThemeId]);

  useEffect(() => {
    const inactiveExtensions = extensions.filter(
      (extension) => !extension.active && !autoActivatedExtensionIdsRef.current.has(extension.id)
    );
    if (inactiveExtensions.length === 0) return;

    inactiveExtensions.forEach((extension) => autoActivatedExtensionIdsRef.current.add(extension.id));

    let cancelled = false;
    Promise.allSettled(
      inactiveExtensions.map((extension) =>
        apiClient.activateNimbalystExtension(extension.id, { workspacePath })
      )
    )
      .then((results) => {
        if (cancelled) return undefined;
        const failed = results.find((result) => result.status === 'rejected');
        if (failed?.status === 'rejected') {
          setError(failed.reason instanceof Error ? failed.reason.message : String(failed.reason));
        }
        return loadExtensions();
      })
      .catch((err) => {
        if (!cancelled) setError(err instanceof Error ? err.message : String(err));
      });

    return () => {
      cancelled = true;
    };
  }, [extensions, loadExtensions, workspacePath]);

  const chooseWorkspace = async () => {
    const electronAPI = (window as any).electronAPI;
    if (!electronAPI?.openDirectory) {
      setError('Electron directory picker is not available.');
      return;
    }

    const selected = await electronAPI.openDirectory();
    if (selected) {
      setWorkspacePath(selected);
      setActiveFile(null);
    }
  };

  const openEntry = async (entry: FileEntry) => {
    if (entry.isDirectory) {
      setWorkspacePath(entry.path);
      setActiveFile(null);
      return;
    }

    const electronAPI = (window as any).electronAPI;
    const result = await electronAPI?.readFile?.(entry.path);
    if (!result?.success) {
      setError(result?.error || 'Failed to read file.');
      return;
    }
    setActiveFile({ path: entry.path, content: result.content });
  };

  const openFilePath = useCallback(async (filePath: string) => {
    const electronAPI = (window as any).electronAPI;
    const result = await electronAPI?.readFile?.(filePath);
    if (!result?.success) {
      setError(result?.error || 'Failed to read file.');
      return;
    }
    setActiveFile({ path: filePath, content: result.content });
  }, []);

  useEffect(() => {
    const listener = (event: MessageEvent) => {
      const message = event.data;
      if (!message || message.type !== 'open-file') return;
      if (
        message.source !== 'batrack-nimbalyst-panel' &&
        message.source !== 'batrack-nimbalyst-custom-editor' &&
        message.source !== 'batrack-nimbalyst-host-component'
      ) {
        return;
      }
      if (typeof message.path === 'string' && message.path.trim()) {
        void openFilePath(message.path);
      }
    };
    window.addEventListener('message', listener);
    return () => window.removeEventListener('message', listener);
  }, [openFilePath]);

  useEffect(() => {
    const listener = (event: MessageEvent) => {
      const message = event.data;
      if (!message || message.source !== 'batrack-nimbalyst-panel') return;

      if (message.type === 'open-panel' && typeof message.panelId === 'string') {
        const nextPanel = panelContributions.find(
          (panel) =>
            panel.id === message.panelId ||
            `${panel.extensionId}.${panel.id}` === message.panelId ||
            `${panel.extensionId}:${panel.id}` === message.panelId
        );
        if (nextPanel) {
          setMode('extensions');
          setSelectedPanelId(`${nextPanel.extensionId}:${nextPanel.id}`);
          setPanelFrameVisible(true);
        }
      } else if (message.type === 'close') {
        setPanelFrameVisible(false);
      }
    };
    window.addEventListener('message', listener);
    return () => window.removeEventListener('message', listener);
  }, [panelContributions]);

  const createNewFileFromContribution = async (contribution: NimbalystExtensionContributionSummary) => {
    if (!workspacePath) {
      setError('Open a workspace before creating files from Nimbalyst contributions.');
      return;
    }

    const raw = contribution.raw as { extension?: string; defaultContent?: string; displayName?: string };
    const extension = raw?.extension || '';
    const normalizedExtension = extension.startsWith('.') || !extension ? extension : `.${extension}`;
    const defaultName = `untitled${normalizedExtension}`;
    const fileName = window.prompt(`Create ${raw?.displayName || contribution.title}`, defaultName);
    if (!fileName) return;

    const safeFileName = fileName.replace(/[\\/]+/g, '-').trim();
    if (!safeFileName) return;

    const electronAPI = (window as any).electronAPI;
    if (!electronAPI?.writeFile) {
      setError('Electron file writer is not available.');
      return;
    }

    const targetPath = `${workspacePath.replace(/[\\/]+$/, '')}/${safeFileName}`;
    const result = await electronAPI.writeFile(targetPath, raw?.defaultContent || '');
    if (!result?.success) {
      setError(result?.error || 'Failed to create file from Nimbalyst contribution.');
      return;
    }

    await loadWorkspaceEntries(workspacePath);
    setActiveFile({ path: targetPath, content: raw?.defaultContent || '' });
  };

  const saveActiveFile = async (filePath: string, content: string) => {
    const electronAPI = (window as any).electronAPI;
    if (!electronAPI?.writeFile) {
      setActiveFile((current) => (current?.path === filePath ? { ...current, content } : current));
      setError('Electron file writer is not available; updated editor state only.');
      return;
    }

    const result = await electronAPI.writeFile(filePath, content);
    if (!result?.success) {
      setError(result?.error || 'Failed to save custom editor content.');
      return;
    }

    setActiveFile((current) => (current?.path === filePath ? { ...current, content } : current));
    await loadWorkspaceEntries(workspacePath);
  };

  const launchReference = async () => {
    setBusy('launch');
    setError(null);
    try {
      const response = await fetch(`${API_CONFIG.BASE_URL}/nimbalyst/reference/launch`, { method: 'POST' });
      const payload = await response.json();
      if (!response.ok || !payload.ok) throw new Error(payload.error || 'Failed to launch Nimbalyst reference.');
      await loadReferenceStatus();
    } catch (err) {
      setError(err instanceof Error ? err.message : String(err));
    } finally {
      setBusy(null);
    }
  };

  const installReference = async () => {
    setBusy('install');
    setError(null);
    try {
      const response = await fetch(`${API_CONFIG.BASE_URL}/nimbalyst/reference/install`, { method: 'POST' });
      const payload = await response.json();
      if (!response.ok || !payload.ok) throw new Error(payload.error || 'Failed to install Nimbalyst dependencies.');
      await loadReferenceStatus();
    } catch (err) {
      setError(err instanceof Error ? err.message : String(err));
    } finally {
      setBusy(null);
    }
  };

  const stopReference = async () => {
    setBusy('stop');
    setError(null);
    try {
      await fetch(`${API_CONFIG.BASE_URL}/nimbalyst/reference/stop`, { method: 'POST' });
      await loadReferenceStatus();
    } catch (err) {
      setError(err instanceof Error ? err.message : String(err));
    } finally {
      setBusy(null);
    }
  };

  const activateExtension = async (extensionId: string) => {
    setBusy(extensionId);
    setError(null);
    try {
      const result = await apiClient.activateNimbalystExtension(extensionId, { workspacePath });
      if (result.error) throw new Error(result.error);
      await loadExtensions();
    } catch (err) {
      setError(err instanceof Error ? err.message : String(err));
    } finally {
      setBusy(null);
    }
  };

  const executeFirstTool = async () => {
    if (!selectedTool) return;
    setBusy(selectedTool.name);
    setError(null);
    try {
      const args = JSON.parse(toolArgs || '{}');
      const result = await apiClient.executeNimbalystExtensionTool({
        toolName: selectedTool.name,
        args,
        workspacePath,
        activeFilePath: activeFile?.path,
      });
      if (result.error) throw new Error(result.error);
      setToolResult(JSON.stringify(result.data ?? result, null, 2));
    } catch (err) {
      setError(err instanceof Error ? err.message : String(err));
    } finally {
      setBusy(null);
    }
  };

  const executeSlashCommand = async () => {
    if (!selectedSlashCommand) return;
    setBusy(selectedSlashCommand.id);
    setError(null);
    try {
      const result = await apiClient.executeNimbalystSlashCommand({
        commandId: selectedSlashCommand.id,
        args: slashArgs,
        workspacePath,
        activeFilePath: activeFile?.path,
      });
      if (result.error) throw new Error(result.error);
      setSlashResult(JSON.stringify(result.data ?? result, null, 2));
    } catch (err) {
      setError(err instanceof Error ? err.message : String(err));
    } finally {
      setBusy(null);
    }
  };

  const activeTitle = useMemo(() => modes.find((item) => item.id === mode)?.label || 'Nimbalyst', [mode]);

  return (
    <div
      className="relative flex h-full w-full overflow-hidden bg-[#161616] text-[#e6e6e6]"
      style={buildNimbalystThemeStyle(activeTheme)}
    >
      <aside className="flex w-12 shrink-0 flex-col items-center border-r border-[#2f2f2f] bg-[#202020] py-2">
        <div className="mb-3 flex size-8 items-center justify-center rounded-md bg-[#4f46e5] text-white">
          <Bot className="size-4" />
        </div>
        <div className="flex flex-1 flex-col gap-1">
          {modes.map((item) => {
            const Icon = item.icon;
            return (
              <button
                key={item.id}
                type="button"
                onClick={() => setMode(item.id)}
                title={item.label}
                className={`flex size-9 items-center justify-center rounded-md transition-colors ${
                  mode === item.id ? 'bg-[#4f46e5] text-white' : 'text-[#a7a7a7] hover:bg-[#2b2b2b] hover:text-white'
                }`}
              >
                <Icon className="size-4" />
              </button>
            );
          })}
        </div>
      </aside>

      <section className="flex min-w-[280px] max-w-[360px] flex-[0_0_320px] flex-col border-r border-[#2f2f2f] bg-[#1d1d1d]">
        <div className="border-b border-[#2f2f2f] p-3">
          <p className="text-sm font-semibold">Nimbalyst</p>
          <p className="mt-0.5 text-xs text-[#8d8d8d]">Reference workspace host</p>
          <button
            type="button"
            onClick={chooseWorkspace}
            className="mt-3 flex h-8 w-full items-center justify-center gap-2 rounded-md border border-[#3a3a3a] bg-[#252525] text-xs text-[#ededed] hover:border-[#4f46e5]"
          >
            <FolderOpen className="size-3.5" />
            Open Workspace
          </button>
        </div>

        <div className="border-b border-[#2f2f2f] p-3 text-xs">
          <p className="mb-2 font-semibold text-[#bdbdbd]">Reference Source</p>
          <StatusLine label="Source" ok={Boolean(status?.exists)} />
          <StatusLine label="Node" ok={Boolean(status?.node?.supported)} detail={status?.node?.version} />
          <StatusLine label="Lockfile" ok={Boolean(status?.packageLockExists)} />
          <StatusLine label="Dependencies" ok={Boolean(status?.dependenciesInstalled)} />
          <StatusLine label="Process" ok={Boolean(status?.running)} />
          <div className="mt-3 flex gap-2">
            <button
              type="button"
              onClick={installReference}
              disabled={busy === 'install' || status?.install?.running}
              className="flex h-7 items-center justify-center gap-1.5 rounded border border-[#3a3a3a] px-2 text-xs text-[#d8d8d8] disabled:opacity-50"
            >
              {busy === 'install' || status?.install?.running ? <Loader2 className="size-3 animate-spin" /> : <TerminalSquare className="size-3" />}
              Install
            </button>
            <button
              type="button"
              onClick={launchReference}
              disabled={busy === 'launch'}
              className="flex h-7 flex-1 items-center justify-center gap-1.5 rounded bg-[#4f46e5] text-xs text-white disabled:opacity-50"
            >
              {busy === 'launch' ? <Loader2 className="size-3 animate-spin" /> : <Play className="size-3" />}
              Launch
            </button>
            <button
              type="button"
              onClick={stopReference}
              disabled={busy === 'stop'}
              className="flex h-7 items-center justify-center rounded border border-[#3a3a3a] px-2 text-xs text-[#bdbdbd] disabled:opacity-50"
            >
              <Square className="size-3" />
            </button>
          </div>
        </div>

        {status?.install?.recentLog?.length ? (
          <div className="border-b border-[#2f2f2f] p-3">
            <p className="mb-2 text-xs font-semibold text-[#bdbdbd]">Install Log</p>
            <pre className="max-h-36 overflow-auto rounded bg-[#111] p-2 text-[11px] leading-4 text-[#aaa]">
              {status.install.recentLog.join('\n')}
            </pre>
          </div>
        ) : null}

        <div className="flex-1 overflow-auto p-2">
          {mode === 'extensions' ? (
            <ExtensionList extensions={extensions} busy={busy} onActivate={activateExtension} />
          ) : (
            <>
              <NewFileMenu
                contributions={newFileContributions}
                onCreate={createNewFileFromContribution}
              />
              <FileList
                entries={entries}
                workspacePath={workspacePath}
                fileIcons={fileIconContributions}
                onOpen={openEntry}
              />
            </>
          )}
        </div>
      </section>

      <main className="flex min-w-0 flex-1 flex-col">
        <header className="flex h-12 shrink-0 items-center justify-between border-b border-[#2f2f2f] bg-[#202020] px-4">
          <div>
            <p className="text-sm font-semibold">{activeTitle}</p>
            <p className="text-xs text-[#8d8d8d]">{workspacePath || 'No workspace selected'}</p>
          </div>
          <button
            type="button"
            onClick={refreshAll}
            className="flex size-8 items-center justify-center rounded-md text-[#a7a7a7] hover:bg-[#2b2b2b] hover:text-white"
            title="Refresh Nimbalyst state"
          >
            <RefreshCw className="size-4" />
          </button>
        </header>

        {error && (
          <div className="border-b border-red-950 bg-red-950/40 px-4 py-2 text-xs text-red-200">
            {error}
          </div>
        )}

        <div className="min-h-0 flex-1 overflow-auto">
          {mode === 'files' && (
            <FilesMode
              activeFile={activeFile}
              customEditor={activeCustomEditor}
              documentHeaders={activeDocumentHeaders}
              status={status}
              workspacePath={workspacePath}
              onSaveFile={saveActiveFile}
            />
          )}
          {mode === 'agent' && (
            <AgentMode
              status={status}
              tools={tools}
              slashCommands={slashCommands}
              agentWorkflows={agentWorkflowContributions}
              claudePlugins={claudePluginContributions}
              selectedTool={selectedTool}
              selectedSlashCommand={selectedSlashCommand}
              toolArgs={toolArgs}
              toolResult={toolResult}
              slashArgs={slashArgs}
              slashResult={slashResult}
              busy={busy}
              setToolArgs={setToolArgs}
              setSlashCommandId={setSlashCommandId}
              setSlashArgs={setSlashArgs}
              executeFirstTool={executeFirstTool}
              executeSlashCommand={executeSlashCommand}
            />
          )}
          {mode === 'tracker' && (
            <TrackerMode
              workspacePath={workspacePath}
              activeFile={activeFile}
              onOpenWorkspace={chooseWorkspace}
            />
          )}
          {mode === 'collab' && (
            <SharedDocsMode
              workspacePath={workspacePath}
              activeFile={activeFile}
              onOpenWorkspace={chooseWorkspace}
            />
          )}
          {mode === 'extensions' && (
            <ExtensionsMode
              tools={tools}
              marketplaceExtensions={marketplaceExtensions}
              slashCommands={slashCommands}
              contributions={contributions}
              panels={panelContributions}
              commands={commandContributions}
              registeredCommands={registeredCommands}
              keybindings={keybindingContributions}
              panelToggleCommands={panelToggleCommands}
              selectedPanel={visibleSelectedPanel}
              selectedPanelId={selectedPanel ? `${selectedPanel.extensionId}:${selectedPanel.id}` : null}
              panelFrameVisible={panelFrameVisible}
              onSelectPanel={(panel) => {
                setSelectedPanelId(`${panel.extensionId}:${panel.id}`);
                setPanelFrameVisible(true);
              }}
              onExecuteCommand={executeNimbalystCommand}
              workspacePath={workspacePath}
              selectedTool={selectedTool}
              selectedSlashCommand={selectedSlashCommand}
              slashArgs={slashArgs}
              slashResult={slashResult}
              commandResult={commandResult}
              toolArgs={toolArgs}
              toolResult={toolResult}
              busy={busy}
              setToolArgs={setToolArgs}
              setSlashCommandId={setSlashCommandId}
              setSlashArgs={setSlashArgs}
              executeFirstTool={executeFirstTool}
              executeSlashCommand={executeSlashCommand}
              installMarketplaceExtension={installMarketplaceExtension}
              installAllMarketplaceExtensions={installAllMarketplaceExtensions}
              marketplaceInstallSummary={marketplaceInstallSummary}
              referenceDependenciesInstalled={Boolean(status?.dependenciesInstalled)}
            />
          )}
          {mode === 'settings' && (
            <SettingsMode
              status={status}
              settingsPanels={settingsPanelContributions}
              configurations={configurationContributions}
              backendModules={backendModuleContributions}
              themes={themeContributions}
              activeThemeId={activeThemeId}
              onSelectTheme={setActiveThemeId}
              workspacePath={workspacePath}
            />
          )}
        </div>
      </main>
      <HostComponentsLayer
        components={hostComponentContributions}
        workspacePath={workspacePath}
      />
    </div>
  );
}

function StatusLine({ label, ok, detail }: { label: string; ok: boolean; detail?: string }) {
  return (
    <div className="flex items-center justify-between py-0.5 text-[#8d8d8d]">
      <span>{label}</span>
      <span className={ok ? 'text-emerald-400' : 'text-[#666]'} title={detail}>
        {detail || (ok ? 'Ready' : 'Missing')}
      </span>
    </div>
  );
}

const NIMBALYST_ICON_COMPONENTS: Record<string, typeof FileText> = {
  archive: FileArchive,
  code: FileCode2,
  data_object: FileJson,
  description: FileText,
  image: Image,
  insert_drive_file: FileText,
  javascript: FileCode2,
  json: FileJson,
  picture_as_pdf: FileText,
  table: Table2,
  table_chart: FileSpreadsheet,
  text_snippet: FileText,
};

function getNimbalystIconComponent(iconName?: string) {
  if (!iconName) return undefined;
  return NIMBALYST_ICON_COMPONENTS[iconName] || undefined;
}

function NimbalystContributionIcon({
  iconName,
  className,
}: {
  iconName?: string;
  className: string;
}) {
  const Icon = getNimbalystIconComponent(iconName) || FileText;
  return <Icon className={className} aria-label={iconName || 'file'} />;
}

function NimbalystFileIcon({
  entry,
  iconName,
}: {
  entry: FileEntry;
  iconName?: string;
}) {
  if (entry.isDirectory) {
    return <FolderOpen className="size-3.5 text-[#a78bfa]" />;
  }

  const Icon = getNimbalystIconComponent(iconName) || FileText;
  const color = iconName === 'picture_as_pdf'
    ? 'text-red-300'
    : iconName
      ? 'text-[#a78bfa]'
      : 'text-[#8d8d8d]';
  return <Icon className={`size-3.5 ${color}`} aria-label={iconName || 'file'} />;
}

function FileList({
  entries,
  workspacePath,
  fileIcons,
  onOpen,
}: {
  entries: FileEntry[];
  workspacePath: string;
  fileIcons: NimbalystExtensionContributionSummary[];
  onOpen: (entry: FileEntry) => void;
}) {
  if (!workspacePath) {
    return <div className="p-3 text-xs text-[#777]">Open a workspace to browse files.</div>;
  }

  return (
    <div className="space-y-0.5">
      {entries.map((entry) => (
        <button
          key={entry.path}
          type="button"
          onClick={() => onOpen(entry)}
          className="flex w-full items-center gap-2 rounded px-2 py-1.5 text-left text-xs text-[#cfcfcf] hover:bg-[#282828]"
        >
          <NimbalystFileIcon entry={entry} iconName={findFileIconName(entry.name, fileIcons)} />
          <span className="truncate">{entry.name}</span>
        </button>
      ))}
    </div>
  );
}

function NewFileMenu({
  contributions,
  onCreate,
}: {
  contributions: NimbalystExtensionContributionSummary[];
  onCreate: (contribution: NimbalystExtensionContributionSummary) => void;
}) {
  if (contributions.length === 0) return null;

  return (
    <div className="mb-3 rounded-md border border-[#303030] bg-[#202020] p-2">
      <p className="mb-2 text-xs font-semibold text-[#bdbdbd]">New File</p>
      <div className="space-y-1">
        {contributions.map((contribution) => (
          <button
            key={`${contribution.extensionId}:${contribution.id}`}
            type="button"
          onClick={() => onCreate(contribution)}
          className="flex w-full items-center gap-2 rounded px-2 py-1.5 text-left text-xs text-[#cfcfcf] hover:bg-[#2b2b2b]"
          title={contribution.extensionName}
        >
            <NimbalystContributionIcon
              iconName={(contribution.raw as { icon?: string })?.icon}
              className="size-3.5 text-[#a78bfa]"
            />
            <span className="min-w-0 flex-1 truncate">{contribution.title}</span>
          </button>
        ))}
      </div>
    </div>
  );
}

function ExtensionList({
  extensions,
  busy,
  onActivate,
}: {
  extensions: NimbalystExtensionSummary[];
  busy: string | null;
  onActivate: (id: string) => void;
}) {
  if (extensions.length === 0) {
    return <div className="p-3 text-xs text-[#777]">No compatible Nimbalyst extensions discovered.</div>;
  }

  return (
    <div className="space-y-2">
      {extensions.map((extension) => (
        <div key={extension.id} className="rounded-md border border-[#303030] bg-[#222] p-2">
          <div className="flex items-start justify-between gap-2">
            <div className="min-w-0">
              <p className="truncate text-xs font-semibold text-[#ededed]">{extension.name}</p>
              <p className="truncate text-[11px] text-[#777]">{extension.id}</p>
            </div>
            {extension.active && <CheckCircle2 className="size-3.5 shrink-0 text-emerald-400" />}
          </div>
          <button
            type="button"
            disabled={busy === extension.id}
            onClick={() => onActivate(extension.id)}
            className="mt-2 h-7 w-full rounded border border-[#3a3a3a] text-xs text-[#cfcfcf] hover:border-[#4f46e5] disabled:opacity-50"
          >
            {extension.active ? 'Reload' : 'Activate'}
          </button>
        </div>
      ))}
    </div>
  );
}

function FilesMode({
  activeFile,
  customEditor,
  documentHeaders,
  status,
  workspacePath,
  onSaveFile,
}: {
  activeFile: ActiveFile | null;
  customEditor?: NimbalystExtensionContributionSummary;
  documentHeaders: NimbalystExtensionContributionSummary[];
  status: ReferenceStatus | null;
  workspacePath: string;
  onSaveFile: (filePath: string, content: string) => Promise<void>;
}) {
  if (!activeFile) {
    return (
      <div className="p-5">
        <h2 className="text-lg font-semibold">Nimbalyst Files Mode</h2>
        <p className="mt-2 max-w-2xl text-sm leading-6 text-[#a7a7a7]">
          This tab now owns the Nimbalyst workspace surface instead of the previous AI-only chat panel.
          The reference source is detected at <span className="font-mono text-[#d5d5d5]">{status?.referencePath || 'nimbalyst'}</span>.
        </p>
      </div>
    );
  }

  const customEditorRaw = customEditor?.raw as { showDocumentHeader?: boolean } | undefined;
  const showDocumentHeaders = documentHeaders.length > 0 && customEditorRaw?.showDocumentHeader !== false;
  const renderedHeaders = showDocumentHeaders ? (
    <DocumentHeaderStrip
      headers={documentHeaders}
      file={activeFile}
      onContentChange={(nextContent) => onSaveFile(activeFile.path, nextContent)}
    />
  ) : null;

  if (customEditor) {
    return (
      <div className="flex h-full flex-col">
        {renderedHeaders}
        <div className="min-h-0 flex-1">
          <CustomEditorFrame
            key={`${customEditor.extensionId}:${customEditor.id}:${activeFile.path}`}
            editor={customEditor}
            file={activeFile}
            workspacePath={workspacePath}
            onSaveFile={onSaveFile}
          />
        </div>
      </div>
    );
  }

  return (
    <div className="flex h-full flex-col">
      <div className="border-b border-[#2f2f2f] px-4 py-2 font-mono text-xs text-[#aaa]">{activeFile.path}</div>
      {renderedHeaders}
      <pre className="flex-1 overflow-auto p-4 font-mono text-xs leading-6 text-[#d6d6d6]">{activeFile.content}</pre>
    </div>
  );
}

function DocumentHeaderStrip({
  headers,
  file,
  onContentChange,
}: {
  headers: NimbalystExtensionContributionSummary[];
  file: ActiveFile;
  onContentChange: (content: string) => void;
}) {
  return (
    <div className="shrink-0 border-b border-[#303030] bg-[#191919]">
      {headers.map((header) => (
        <DocumentHeaderFrame
          key={`${header.extensionId}:${header.id}:${file.path}`}
          header={header}
          file={file}
          onContentChange={onContentChange}
        />
      ))}
    </div>
  );
}

function DocumentHeaderFrame({
  header,
  file,
  onContentChange,
}: {
  header: NimbalystExtensionContributionSummary;
  file: ActiveFile;
  onContentChange: (content: string) => void;
}) {
  const apiBase = API_CONFIG.BASE_URL;
  const raw = header.raw as { component?: string; displayName?: string };
  const componentName = raw?.component || header.id;
  const moduleUrl = `${apiBase}/nimbalyst/extensions/${encodeURIComponent(header.extensionId)}/assets/${header.main}`;
  const frameKey = `${header.extensionId}:${header.id}:${file.path}`;

  useEffect(() => {
    const listener = (event: MessageEvent) => {
      const message = event.data;
      if (!message || message.source !== 'batrack-nimbalyst-document-header') return;
      if (message.frameKey !== frameKey || message.filePath !== file.path) return;

      if (message.type === 'content-change') {
        onContentChange(String(message.content ?? ''));
      }
    };
    window.addEventListener('message', listener);
    return () => window.removeEventListener('message', listener);
  }, [file.path, frameKey, onContentChange]);

  const srcDoc = `<!doctype html>
<html>
  <head>
    <meta charset="utf-8" />
    <style>
      html, body, #root { width: 100%; min-height: 100%; margin: 0; background: #191919; color: #e6e6e6; font-family: Inter, system-ui, sans-serif; }
      .error { padding: 8px 12px; color: #fecaca; font: 12px/1.5 ui-monospace, SFMono-Regular, Menlo, monospace; white-space: pre-wrap; }
    </style>
    <script type="importmap">
      {
        "imports": {
          "react": "${apiBase}/nimbalyst/browser-shims/react.js",
          "react/jsx-runtime": "${apiBase}/nimbalyst/browser-shims/react-jsx-runtime.js",
          "react-dom/client": "${apiBase}/nimbalyst/browser-shims/react-dom-client.js",
          "@nimbalyst/extension-sdk": "${apiBase}/nimbalyst/browser-shims/extension-sdk.js",
          "@nimbalyst/runtime": "${apiBase}/nimbalyst/browser-shims/runtime.js"
        }
      }
    </script>
  </head>
  <body>
    <div id="root"></div>
    <script type="module">
      window.__nimbalyst_extensions = window.parent.__nimbalyst_extensions || {};
      try {
        const React = await import('react');
        const { createRoot } = await import('react-dom/client');
        const module = await import(${JSON.stringify(moduleUrl)});
        const Component = module.components?.[${JSON.stringify(componentName)}];
        if (!Component) {
          throw new Error('Document header component export not found: ${header.extensionId}.${componentName}');
        }
        let content = ${JSON.stringify(file.content)};
        const props = {
          filePath: ${JSON.stringify(file.path)},
          fileName: ${JSON.stringify(basename(file.path))},
          getContent: () => content,
          contentVersion: 0,
          onContentChange: (nextContent) => {
            content = String(nextContent ?? '');
            window.parent.postMessage({
              source: 'batrack-nimbalyst-document-header',
              type: 'content-change',
              frameKey: ${JSON.stringify(frameKey)},
              filePath: ${JSON.stringify(file.path)},
              content
            }, '*');
          },
          editor: null
        };
        createRoot(document.getElementById('root')).render(React.createElement(Component, props));
      } catch (error) {
        const message = error?.stack || error?.message || String(error);
        document.getElementById('root').innerHTML = '<pre class="error"></pre>';
        document.querySelector('.error').textContent = message;
      }
    </script>
  </body>
</html>`;

  return (
    <iframe
      title={`${header.extensionName} ${raw?.displayName || header.title}`}
      srcDoc={srcDoc}
      className="block h-12 w-full border-0"
      sandbox="allow-scripts allow-same-origin allow-modals"
    />
  );
}

function CustomEditorFrame({
  editor,
  file,
  workspacePath,
  onSaveFile,
}: {
  editor: NimbalystExtensionContributionSummary;
  file: ActiveFile;
  workspacePath: string;
  onSaveFile: (filePath: string, content: string) => Promise<void>;
}) {
  const [dirty, setDirty] = useState(false);
  const apiBase = API_CONFIG.BASE_URL;
  const raw = editor.raw as { component?: string; displayName?: string; supportsSourceMode?: boolean };
  const moduleUrl = `${apiBase}/nimbalyst/extensions/${encodeURIComponent(editor.extensionId)}/assets/${editor.main}`;

  useEffect(() => {
    const listener = (event: MessageEvent) => {
      const message = event.data;
      if (!message || message.source !== 'batrack-nimbalyst-custom-editor') return;
      if (message.filePath !== file.path) return;

      if (message.type === 'dirty') {
        setDirty(Boolean(message.value));
      } else if (message.type === 'save') {
        onSaveFile(file.path, String(message.content ?? '')).then(() => setDirty(false));
      }
    };
    window.addEventListener('message', listener);
    return () => window.removeEventListener('message', listener);
  }, [file.path, onSaveFile]);

  const hostPayload = {
    extensionId: editor.extensionId,
    componentName: raw?.component || editor.id,
    filePath: file.path,
    fileName: basename(file.path),
    workspacePath,
    initialContent: file.content,
    supportsSourceMode: Boolean(raw?.supportsSourceMode),
  };
  const srcDoc = `<!doctype html>
<html>
  <head>
    <meta charset="utf-8" />
    <style>
      html, body, #root { width: 100%; height: 100%; margin: 0; background: #151515; color: #e6e6e6; font-family: Inter, system-ui, sans-serif; }
      .error { padding: 16px; color: #fecaca; font: 12px/1.5 ui-monospace, SFMono-Regular, Menlo, monospace; white-space: pre-wrap; }
    </style>
    <script type="importmap">
      {
        "imports": {
          "react": "${apiBase}/nimbalyst/browser-shims/react.js",
          "react/jsx-runtime": "${apiBase}/nimbalyst/browser-shims/react-jsx-runtime.js",
          "react-dom/client": "${apiBase}/nimbalyst/browser-shims/react-dom-client.js",
          "@nimbalyst/extension-sdk": "${apiBase}/nimbalyst/browser-shims/extension-sdk.js",
          "@nimbalyst/runtime": "${apiBase}/nimbalyst/browser-shims/runtime.js"
        }
      }
    </script>
  </head>
  <body>
    <div id="root"></div>
    <script type="module">
      window.__BAT_RACK_EDITOR_HOST__ = ${JSON.stringify(hostPayload)};
      ${buildNimbalystIframeElectronBridgeScript(apiBase, workspacePath)}
      window.__nimbalyst_extensions = window.parent.__nimbalyst_extensions || {};
      try {
        const React = await import('react');
        const { createRoot } = await import('react-dom/client');
        const module = await import(${JSON.stringify(moduleUrl)});
        const Component = module.components?.[${JSON.stringify(raw?.component || editor.id)}] || module.default;
        if (!Component) {
          throw new Error('Custom editor component export not found: ${editor.extensionId}.${raw?.component || editor.id}');
        }
        let content = ${JSON.stringify(file.content)};
        const saveCallbacks = new Set();
        const fileCallbacks = new Set();
        const themeCallbacks = new Set();
        const sourceCallbacks = new Set();
        const diffCallbacks = new Set();
        const diffClearedCallbacks = new Set();
        let dirty = false;
        let sourceMode = false;
        const storagePrefix = 'batrack:nimbalyst:' + ${JSON.stringify(editor.extensionId)} + ':';
        const readStored = (scope, key, fallback) => {
          const raw = window.localStorage.getItem(storagePrefix + scope + ':' + key);
          if (raw == null) return fallback;
          try { return JSON.parse(raw); } catch { return raw; }
        };
        const writeStored = (scope, key, value) => {
          window.localStorage.setItem(storagePrefix + scope + ':' + key, JSON.stringify(value));
        };
        const deleteStored = (scope, key) => {
          window.localStorage.removeItem(storagePrefix + scope + ':' + key);
        };
        const joinWorkspacePath = (targetPath = '') => {
          const value = String(targetPath || '');
          if (/^[A-Za-z]:[\\/]/.test(value) || value.startsWith('/') || value.startsWith('\\\\')) return value;
          const base = ${JSON.stringify(workspacePath)}.replace(/[\\/]+$/, '');
          return value ? base + '/' + value.replace(/^[\\/]+/, '') : base;
        };
        const readElectronFile = async (targetPath) => {
          const result = await window.electronAPI?.readFile?.(targetPath);
          if (!result?.success) throw new Error(result?.error || 'Failed to read file: ' + targetPath);
          return result.content || '';
        };
        const writeElectronFile = async (targetPath, nextContent) => {
          const result = await window.electronAPI?.writeFile?.(targetPath, nextContent);
          if (!result?.success) throw new Error(result?.error || 'Failed to write file: ' + targetPath);
        };
        const storage = {
          get: (key, fallback) => readStored('workspace', key, fallback),
          set: (key, value) => writeStored('workspace', key, value),
          delete: (key) => deleteStored('workspace', key),
          getGlobal: (key, fallback) => readStored('global', key, fallback),
          setGlobal: (key, value) => writeStored('global', key, value),
          deleteGlobal: (key) => deleteStored('global', key),
          getSecret: (key) => readStored('secret', key, undefined),
          setSecret: (key, value) => writeStored('secret', key, value),
          deleteSecret: (key) => deleteStored('secret', key),
        };
        const extensionDataDir = '.nimbalyst/extensions/' + ${JSON.stringify(editor.extensionId)};
        const host = {
          filePath: ${JSON.stringify(file.path)},
          fileName: ${JSON.stringify(basename(file.path))},
          theme: 'dark',
          isActive: true,
          readOnly: false,
          embedded: false,
          workspaceId: ${JSON.stringify(workspacePath)},
          supportsSourceMode: ${JSON.stringify(Boolean(raw?.supportsSourceMode))},
          storage,
          files: {
            getBasePath: async () => joinWorkspacePath(extensionDataDir),
            getGlobalBasePath: async () => joinWorkspacePath('.nimbalyst/extensions-global/' + ${JSON.stringify(editor.extensionId)}),
            write: async (relativePath, data) => writeElectronFile(joinWorkspacePath(extensionDataDir + '/' + relativePath), data instanceof Uint8Array ? new TextDecoder().decode(data) : data),
            readText: async (relativePath) => readElectronFile(joinWorkspacePath(extensionDataDir + '/' + relativePath)),
            read: async (relativePath) => new TextEncoder().encode(await readElectronFile(joinWorkspacePath(extensionDataDir + '/' + relativePath))),
            exists: async (relativePath) => {
              try { await readElectronFile(joinWorkspacePath(extensionDataDir + '/' + relativePath)); return true; } catch { return false; }
            },
            delete: async () => undefined,
            list: async (relativePath = '') => {
              const result = await window.electronAPI?.readDirectory?.(joinWorkspacePath(extensionDataDir + '/' + relativePath));
              return (result?.items || []).map((item) => item.name);
            },
            getUsage: async () => ({ usedBytes: 0, limitBytes: Number.MAX_SAFE_INTEGER }),
          },
          loadContent: async () => content,
          loadBinaryContent: async () => new TextEncoder().encode(content).buffer,
          onFileChanged: (callback) => { fileCallbacks.add(callback); return () => fileCallbacks.delete(callback); },
          onThemeChanged: (callback) => { themeCallbacks.add(callback); return () => themeCallbacks.delete(callback); },
          onReadOnlyChanged: () => () => undefined,
          setDirty: (value) => {
            dirty = Boolean(value);
            window.parent.postMessage({ source: 'batrack-nimbalyst-custom-editor', type: 'dirty', filePath: ${JSON.stringify(file.path)}, value: dirty }, '*');
          },
          saveContent: async (nextContent) => {
            content = typeof nextContent === 'string' ? nextContent : new TextDecoder().decode(nextContent);
            window.parent.postMessage({ source: 'batrack-nimbalyst-custom-editor', type: 'save', filePath: ${JSON.stringify(file.path)}, content }, '*');
          },
          onSaveRequested: (callback) => { saveCallbacks.add(callback); return () => saveCallbacks.delete(callback); },
          openHistory: () => undefined,
          openFile: (targetPath) => window.parent.postMessage({ source: 'batrack-nimbalyst-custom-editor', type: 'open-file', path: joinWorkspacePath(targetPath) }, '*'),
          exec: async (command, options = {}) => {
            const response = await fetch(${JSON.stringify(`${apiBase}/nimbalyst/host/exec`)}, {
              method: 'POST',
              headers: { 'Content-Type': 'application/json' },
              body: JSON.stringify({
                command,
                workspacePath: ${JSON.stringify(workspacePath)},
                cwd: options.cwd || ${JSON.stringify(workspacePath)},
                env: options.env,
                timeout: options.timeout,
                maxBuffer: options.maxBuffer,
              }),
            });
            return response.json();
          },
          onDiffRequested: (callback) => { diffCallbacks.add(callback); return () => diffCallbacks.delete(callback); },
          reportDiffResult: () => undefined,
          isDiffModeActive: () => false,
          onDiffCleared: (callback) => { diffClearedCallbacks.add(callback); return () => diffClearedCallbacks.delete(callback); },
          toggleSourceMode: () => {
            sourceMode = !sourceMode;
            sourceCallbacks.forEach((callback) => callback(sourceMode));
          },
          onSourceModeChanged: (callback) => { sourceCallbacks.add(callback); return () => sourceCallbacks.delete(callback); },
          isSourceModeActive: () => sourceMode,
          getConfig: (_key, defaultValue) => defaultValue,
          setEditorContext: () => undefined,
          registerEditorAPI: () => undefined,
          registerMenuItems: () => undefined,
        };
        window.addEventListener('message', (event) => {
          const message = event.data;
          if (!message || message.source !== 'batrack-nimbalyst-custom-editor-command') return;
          if (message.type === 'save-request') {
            saveCallbacks.forEach((callback) => callback());
          }
        });
        createRoot(document.getElementById('root')).render(React.createElement(Component, { host }));
        window.parent.postMessage({ source: 'batrack-nimbalyst-custom-editor', type: 'ready', filePath: ${JSON.stringify(file.path)} }, '*');
      } catch (error) {
        const message = error?.stack || error?.message || String(error);
        document.getElementById('root').innerHTML = '<pre class="error"></pre>';
        document.querySelector('.error').textContent = message;
        window.parent.postMessage({ source: 'batrack-nimbalyst-custom-editor', type: 'error', filePath: ${JSON.stringify(file.path)}, error: message }, '*');
      }
    </script>
  </body>
</html>`;

  const requestSave = () => {
    const frame = document.querySelector<HTMLIFrameElement>(`iframe[data-editor-frame="${cssEscape(file.path)}"]`);
    frame?.contentWindow?.postMessage({ source: 'batrack-nimbalyst-custom-editor-command', type: 'save-request' }, '*');
  };

  return (
    <div className="flex h-full flex-col">
      <div className="flex items-center justify-between border-b border-[#2f2f2f] px-4 py-2">
        <div className="min-w-0">
          <p className="truncate text-xs font-semibold text-[#ededed]">{raw?.displayName || editor.title}</p>
          <p className="truncate font-mono text-[11px] text-[#777]">{file.path}</p>
        </div>
        <button
          type="button"
          onClick={requestSave}
          className="h-7 rounded border border-[#3a3a3a] px-3 text-xs text-[#cfcfcf] hover:border-[#4f46e5]"
        >
          {dirty ? 'Save *' : 'Save'}
        </button>
      </div>
      <iframe
        data-editor-frame={file.path}
        title={`${editor.extensionName} ${raw?.displayName || editor.title}`}
        srcDoc={srcDoc}
        className="min-h-0 flex-1 border-0"
        sandbox="allow-scripts allow-same-origin allow-modals"
      />
    </div>
  );
}

function AgentMode({
  status,
  tools,
  slashCommands,
  agentWorkflows,
  claudePlugins,
  selectedTool,
  selectedSlashCommand,
  toolArgs,
  toolResult,
  slashArgs,
  slashResult,
  busy,
  setToolArgs,
  setSlashCommandId,
  setSlashArgs,
  executeFirstTool,
  executeSlashCommand,
}: {
  status: ReferenceStatus | null;
  tools: NimbalystExtensionToolSummary[];
  slashCommands: NimbalystSlashCommandSummary[];
  agentWorkflows: NimbalystExtensionContributionSummary[];
  claudePlugins: NimbalystExtensionContributionSummary[];
  selectedTool?: NimbalystExtensionToolSummary;
  selectedSlashCommand?: NimbalystSlashCommandSummary;
  toolArgs: string;
  toolResult: string;
  slashArgs: string;
  slashResult: string;
  busy: string | null;
  setToolArgs: (value: string) => void;
  setSlashCommandId: (value: string) => void;
  setSlashArgs: (value: string) => void;
  executeFirstTool: () => void;
  executeSlashCommand: () => void;
}) {
  return (
    <div className="grid min-h-full grid-cols-[minmax(260px,340px)_1fr]">
      <aside className="border-r border-[#2f2f2f] p-4">
        <h2 className="text-lg font-semibold">Nimbalyst Agent</h2>
        <p className="mt-2 text-sm leading-6 text-[#a7a7a7]">
          Extension tools, slash commands, and workflow metadata are surfaced here as the first-tab agent workbench.
        </p>
        <div className="mt-4 grid gap-3">
          <InfoCard icon={GitBranch} title="Workflows" body={`${agentWorkflows.length} agent workflow contribution(s)`} />
          <InfoCard icon={Bot} title="Claude Plugins" body={`${claudePlugins.length} bundled plugin contribution(s)`} />
          <InfoCard icon={Plug} title="Extension Tools" body={`${tools.length} active tool(s); ${status?.running ? 'reference running' : 'reference stopped'}`} />
        </div>

        <AgentContributionList title="Agent Workflows" contributions={agentWorkflows} />
        <AgentContributionList title="Claude Plugins" contributions={claudePlugins} />
      </aside>

      <div className="p-4">
        <section className="rounded-md border border-[#303030] bg-[#202020] p-4">
          <div className="flex items-start justify-between gap-3">
            <div>
              <h3 className="text-sm font-semibold">Extension Tool Runner</h3>
              <p className="mt-1 text-xs text-[#888]">
                Runs active `aiTools` and `context.services.ai.registerTool()` handlers through the Nimbalyst compatibility host.
              </p>
            </div>
            <span className="rounded bg-[#303030] px-2 py-1 text-[11px] text-[#aaa]">{tools.length} tools</span>
          </div>
          <h4 className="mt-4 text-xs font-semibold text-[#ededed]">{selectedTool?.originalName || 'No tool selected'}</h4>
          <textarea
            value={toolArgs}
            onChange={(event) => setToolArgs(event.target.value)}
            className="mt-2 h-32 w-full resize-none rounded-md border border-[#333] bg-[#111] p-3 font-mono text-xs text-[#ddd] outline-none focus:border-[#4f46e5]"
          />
          <button
            type="button"
            onClick={executeFirstTool}
            disabled={!selectedTool || busy === selectedTool.name}
            className="mt-3 h-8 rounded-md bg-[#4f46e5] px-3 text-xs font-medium text-white disabled:opacity-50"
          >
            Execute Tool
          </button>
          {toolResult && (
            <pre className="mt-3 max-h-64 overflow-auto rounded-md border border-[#303030] bg-[#111] p-3 font-mono text-xs text-[#d6d6d6]">
              {toolResult}
            </pre>
          )}
        </section>

        <section className="mt-4 rounded-md border border-[#303030] bg-[#202020] p-4">
          <div className="flex items-start justify-between gap-3">
            <div>
              <h3 className="text-sm font-semibold">Slash Commands</h3>
              <p className="mt-1 text-xs text-[#888]">
                Runs commands declared in `contributions.slashCommands` and backed by `module.slashCommandHandlers`.
              </p>
            </div>
            <span className="rounded bg-[#303030] px-2 py-1 text-[11px] text-[#aaa]">{slashCommands.length} commands</span>
          </div>
          {slashCommands.length > 0 ? (
            <div className="mt-3 flex flex-wrap gap-2">
              {slashCommands.map((command) => (
                <button
                  key={`${command.extensionId}:${command.id}`}
                  type="button"
                  onClick={() => setSlashCommandId(command.id)}
                  className={`h-8 rounded-md border px-3 text-xs ${
                    selectedSlashCommand?.id === command.id
                      ? 'border-[#4f46e5] bg-[#312e81] text-white'
                      : 'border-[#333] bg-[#202020] text-[#cfcfcf] hover:border-[#4f46e5]'
                  }`}
                  title={command.description}
                >
                  /{command.title}
                </button>
              ))}
            </div>
          ) : (
            <div className="mt-3 rounded-md border border-[#303030] bg-[#1b1b1b] p-3 text-xs text-[#777]">
              No executable slash commands from active extensions.
            </div>
          )}
          {selectedSlashCommand && (
            <>
              <p className="mt-3 text-xs text-[#aaa]">
                {selectedSlashCommand.title}
                <span className="ml-2 text-[#666]">{selectedSlashCommand.extensionName}</span>
              </p>
              <textarea
                value={slashArgs}
                onChange={(event) => setSlashArgs(event.target.value)}
                placeholder="Slash command arguments"
                className="mt-2 h-24 w-full resize-none rounded-md border border-[#333] bg-[#111] p-3 font-mono text-xs text-[#ddd] outline-none focus:border-[#4f46e5]"
              />
              <button
                type="button"
                onClick={executeSlashCommand}
                disabled={!selectedSlashCommand.executable || busy === selectedSlashCommand.id}
                className="mt-3 h-8 rounded-md bg-[#4f46e5] px-3 text-xs font-medium text-white disabled:opacity-50"
              >
                Execute Slash Command
              </button>
            </>
          )}
          {slashResult && (
            <pre className="mt-3 max-h-64 overflow-auto rounded-md border border-[#303030] bg-[#111] p-3 font-mono text-xs text-[#d6d6d6]">
              {slashResult}
            </pre>
          )}
        </section>
      </div>
    </div>
  );
}

function AgentContributionList({
  title,
  contributions,
}: {
  title: string;
  contributions: NimbalystExtensionContributionSummary[];
}) {
  return (
    <div className="mt-4 rounded-md border border-[#303030] bg-[#202020] p-3">
      <p className="text-xs font-semibold text-[#ededed]">{title}</p>
      {contributions.length > 0 ? (
        <div className="mt-2 space-y-2">
          {contributions.map((contribution) => {
            const raw = contribution.raw as { path?: string; displayName?: string; description?: string; enabledByDefault?: boolean };
            return (
              <div key={`${contribution.extensionId}:${contribution.id}`} className="rounded bg-[#171717] px-2 py-2 text-xs">
                <div className="flex items-center justify-between gap-2">
                  <span className="min-w-0 truncate font-semibold text-[#d8d8d8]">{raw.displayName || contribution.title}</span>
                  {raw.enabledByDefault && <span className="rounded bg-emerald-950 px-1.5 py-0.5 text-[10px] text-emerald-300">default</span>}
                </div>
                {raw.description && <p className="mt-1 text-[11px] leading-4 text-[#888]">{raw.description}</p>}
                {raw.path && <p className="mt-1 truncate font-mono text-[10px] text-[#666]">{raw.path}</p>}
              </div>
            );
          })}
        </div>
      ) : (
        <p className="mt-2 text-xs text-[#777]">No contributions discovered.</p>
      )}
    </div>
  );
}

function ExtensionsMode({
  tools,
  marketplaceExtensions,
  slashCommands,
  contributions,
  panels,
  commands,
  registeredCommands,
  keybindings,
  panelToggleCommands,
  selectedPanel,
  selectedPanelId,
  panelFrameVisible,
  onSelectPanel,
  onExecuteCommand,
  workspacePath,
  selectedTool,
  selectedSlashCommand,
  slashArgs,
  slashResult,
  commandResult,
  toolArgs,
  toolResult,
  busy,
  setToolArgs,
  setSlashCommandId,
  setSlashArgs,
  executeFirstTool,
  executeSlashCommand,
  installMarketplaceExtension,
  installAllMarketplaceExtensions,
  marketplaceInstallSummary,
  referenceDependenciesInstalled,
}: {
  tools: NimbalystExtensionToolSummary[];
  marketplaceExtensions: NimbalystMarketplaceExtensionSummary[];
  slashCommands: NimbalystSlashCommandSummary[];
  contributions: NimbalystExtensionContributionSummary[];
  panels: NimbalystExtensionContributionSummary[];
  commands: NimbalystExtensionContributionSummary[];
  registeredCommands: NimbalystCommandSummary[];
  keybindings: NimbalystExtensionContributionSummary[];
  panelToggleCommands: Array<{
    id: string;
    title: string;
    extensionId: string;
    extensionName: string;
    panel: NimbalystExtensionContributionSummary;
  }>;
  selectedPanel?: NimbalystExtensionContributionSummary;
  selectedPanelId: string | null;
  panelFrameVisible: boolean;
  onSelectPanel: (panel: NimbalystExtensionContributionSummary) => void;
  onExecuteCommand: (commandId: string) => Promise<boolean>;
  workspacePath: string;
  selectedTool?: NimbalystExtensionToolSummary;
  selectedSlashCommand?: NimbalystSlashCommandSummary;
  slashArgs: string;
  slashResult: string;
  commandResult: string;
  toolArgs: string;
  toolResult: string;
  busy: string | null;
  setToolArgs: (value: string) => void;
  setSlashCommandId: (value: string) => void;
  setSlashArgs: (value: string) => void;
  executeFirstTool: () => void;
  executeSlashCommand: () => void;
  installMarketplaceExtension: (extensionId: string, buildFirst?: boolean) => void;
  installAllMarketplaceExtensions: () => void;
  marketplaceInstallSummary: string;
  referenceDependenciesInstalled: boolean;
}) {
  const visibleMarketplaceExtensions = marketplaceExtensions.filter((extension) => extension.availableLocal);
  return (
    <div className="grid h-full grid-cols-[minmax(260px,360px)_1fr]">
      <div className="border-r border-[#2f2f2f] p-4">
        <h2 className="text-sm font-semibold">Extension SDK Tools</h2>
        <p className="mt-1 text-xs text-[#888]">Tools registered through the Nimbalyst Extension SDK compatibility host.</p>
        <div className="mt-4 space-y-2">
          {tools.map((tool) => (
            <div key={tool.name} className="rounded-md border border-[#303030] bg-[#222] p-2">
              <p className="text-xs font-semibold">{tool.originalName}</p>
              <p className="mt-1 text-[11px] text-[#777]">{tool.description}</p>
            </div>
          ))}
        </div>
        <h2 className="mt-6 text-sm font-semibold">Manifest Contributions</h2>
        <p className="mt-1 text-xs text-[#888]">
          Registry entries parsed from Nimbalyst extension manifests.
        </p>
        <ContributionSummary contributions={contributions} />
        <h2 className="mt-6 text-sm font-semibold">Reference Marketplace</h2>
        <p className="mt-1 text-xs text-[#888]">
          Built-in Nimbalyst extensions discovered from the reference app can be copied into Bat Rack when their runnable bundle is present.
        </p>
        <button
          type="button"
          onClick={installAllMarketplaceExtensions}
          disabled={busy === 'marketplace:all' || visibleMarketplaceExtensions.length === 0 || !referenceDependenciesInstalled}
          className="mt-3 h-8 rounded border border-[#4f46e5] bg-[#312e81] px-3 text-xs font-medium text-white disabled:opacity-50"
        >
          {busy === 'marketplace:all' ? 'Installing...' : 'Build & Install All Reference Extensions'}
        </button>
        {!referenceDependenciesInstalled && (
          <p className="mt-2 text-[11px] text-amber-300">
            Reference dependencies are not installed. Run Reference Source &gt; Install first.
          </p>
        )}
        {marketplaceInstallSummary && (
          <pre className="mt-2 max-h-32 overflow-auto rounded-md border border-[#303030] bg-[#111] p-2 font-mono text-[11px] text-[#cfcfcf]">
            {marketplaceInstallSummary}
          </pre>
        )}
        <div className="mt-3 space-y-2">
          {visibleMarketplaceExtensions.slice(0, 8).map((extension) => {
            const canRunAction = extension.installable || extension.requiresBuild;
            const actionLabel = extension.requiresBuild
              ? 'Build & Install'
              : extension.installed
                ? 'Reinstall'
                : 'Install';
            const buildFirst = extension.requiresBuild;
            return (
              <div key={extension.id} className="rounded-md border border-[#303030] bg-[#1b1b1b] p-2">
                <div className="flex items-start justify-between gap-2">
                  <div className="min-w-0">
                    <p className="truncate text-xs font-semibold text-[#ededed]">{extension.name}</p>
                    <p className="mt-1 line-clamp-2 text-[11px] leading-4 text-[#888]">
                      {extension.tagline || extension.description}
                    </p>
                    <p className="mt-1 truncate font-mono text-[10px] text-[#666]">{extension.id}</p>
                  </div>
                  <button
                    type="button"
                    onClick={() => installMarketplaceExtension(extension.id, buildFirst)}
                    disabled={!canRunAction || busy === `marketplace:${extension.id}`}
                    className="h-7 shrink-0 rounded border border-[#3a3a3a] bg-[#252525] px-2 text-[11px] text-[#d8d8d8] hover:border-[#4f46e5] disabled:opacity-50"
                  >
                    {!canRunAction ? 'Build needed' : actionLabel}
                  </button>
                </div>
              </div>
            );
          })}
          {visibleMarketplaceExtensions.length === 0 && (
            <div className="rounded-md border border-[#303030] bg-[#1b1b1b] p-3 text-xs text-[#777]">
              No bundled reference extensions are available to install.
            </div>
          )}
          {visibleMarketplaceExtensions.length > 8 && (
            <p className="text-[11px] text-[#777]">+{visibleMarketplaceExtensions.length - 8} more bundled reference extension(s)</p>
          )}
        </div>
      </div>
      <div className="p-4">
        <h3 className="text-sm font-semibold">Renderer Panels</h3>
        <p className="mt-1 text-xs text-[#888]">
          Panels exported as Nimbalyst `panels[id].component` are loaded through the Bat Rack iframe host.
        </p>
        {panels.length > 0 ? (
          <div className="mt-3 flex flex-wrap gap-2">
            {panels.map((panel) => (
              <button
                key={`${panel.extensionId}:${panel.id}`}
                type="button"
                onClick={() => onSelectPanel(panel)}
                className={`h-8 rounded-md border px-3 text-xs ${
                  selectedPanelId === `${panel.extensionId}:${panel.id}` && panelFrameVisible
                    ? 'border-[#4f46e5] bg-[#312e81] text-white'
                    : 'border-[#333] bg-[#202020] text-[#cfcfcf] hover:border-[#4f46e5]'
                }`}
              >
                {panel.title}
              </button>
            ))}
          </div>
        ) : (
          <div className="mt-3 rounded-md border border-[#303030] bg-[#1b1b1b] p-3 text-xs text-[#777]">
            No panel contributions discovered.
          </div>
        )}
        {selectedPanel && (
          <NimbalystPanelFrame
            key={`${selectedPanel.extensionId}:${selectedPanel.id}:${workspacePath}`}
            panel={selectedPanel}
            workspacePath={workspacePath}
          />
        )}
        {panels.length > 0 && !panelFrameVisible && (
          <div className="mt-3 rounded-md border border-[#303030] bg-[#1b1b1b] p-3 text-xs text-[#777]">
            Panel closed by extension host.
          </div>
        )}

        <h3 className="mt-6 text-sm font-semibold">Commands & Keybindings</h3>
        <p className="mt-1 text-xs text-[#888]">
          Panel toggle commands are auto-registered as `${'{extensionId}'}.${'{panelId}'}.toggle`, matching Nimbalyst's panel registry.
        </p>
        <div className="mt-3 grid gap-2 md:grid-cols-2">
          {panelToggleCommands.map((command) => (
            <button
              key={command.id}
              type="button"
              onClick={() => onExecuteCommand(command.id)}
              className="rounded-md border border-[#303030] bg-[#222] p-2 text-left text-xs hover:border-[#4f46e5]"
              title={command.id}
            >
              <span className="block font-semibold text-[#ededed]">{command.title}</span>
              <span className="mt-1 block truncate text-[11px] text-[#777]">{command.id}</span>
            </button>
          ))}
          {commands.map((command) => (
            <button
              key={`${command.extensionId}:${command.id}`}
              type="button"
              onClick={() => onExecuteCommand(command.id)}
              className="rounded-md border border-[#303030] bg-[#1b1b1b] p-2 text-left text-xs hover:border-[#4f46e5]"
              title={command.id}
            >
              <span className="block font-semibold text-[#ededed]">{command.title}</span>
              <span className="mt-1 block truncate text-[11px] text-[#777]">
                {command.id}
                {registeredCommands.some((registered) => registered.id === command.id && registered.executable) ? ' - executable' : ''}
              </span>
            </button>
          ))}
        </div>
        {commandResult && (
          <pre className="mt-3 max-h-40 overflow-auto rounded-md border border-[#303030] bg-[#111] p-3 text-xs text-[#cfcfcf]">
            {commandResult}
          </pre>
        )}
        {panelToggleCommands.length === 0 && commands.length === 0 && (
          <div className="mt-3 rounded-md border border-[#303030] bg-[#1b1b1b] p-3 text-xs text-[#777]">
            No command contributions discovered.
          </div>
        )}
        {keybindings.length > 0 && (
          <div className="mt-3 rounded-md border border-[#303030] bg-[#1b1b1b] p-2">
            <p className="text-xs font-semibold text-[#ededed]">Registered Keybindings</p>
            <div className="mt-2 space-y-1">
              {keybindings.map((keybinding) => {
                const raw = keybinding.raw as { key?: string; command?: string };
                return (
                  <div
                    key={`${keybinding.extensionId}:${raw.key}:${raw.command}`}
                    className="flex items-center justify-between gap-3 rounded bg-[#242424] px-2 py-1.5 text-[11px]"
                  >
                    <span className="rounded border border-[#3a3a3a] px-1.5 py-0.5 font-mono text-[#d8d8d8]">
                      {raw.key}
                    </span>
                    <span className="min-w-0 flex-1 truncate text-[#777]">{raw.command}</span>
                  </div>
                );
              })}
            </div>
          </div>
        )}

        <h3 className="mt-6 text-sm font-semibold">{selectedTool?.originalName || 'No tool selected'}</h3>
        <textarea
          value={toolArgs}
          onChange={(event) => setToolArgs(event.target.value)}
          className="mt-3 h-40 w-full resize-none rounded-md border border-[#333] bg-[#111] p-3 font-mono text-xs text-[#ddd] outline-none focus:border-[#4f46e5]"
        />
        <button
          type="button"
          onClick={executeFirstTool}
          disabled={!selectedTool || busy === selectedTool.name}
          className="mt-3 h-8 rounded-md bg-[#4f46e5] px-3 text-xs font-medium text-white disabled:opacity-50"
        >
          Execute Tool
        </button>
        {toolResult && (
          <pre className="mt-4 max-h-[420px] overflow-auto rounded-md border border-[#303030] bg-[#111] p-3 font-mono text-xs text-[#d6d6d6]">
            {toolResult}
          </pre>
        )}

        <h3 className="mt-6 text-sm font-semibold">Slash Commands</h3>
        <p className="mt-1 text-xs text-[#888]">
          Commands declared in `contributions.slashCommands` and backed by `module.slashCommandHandlers`.
        </p>
        {slashCommands.length > 0 ? (
          <div className="mt-3 flex flex-wrap gap-2">
            {slashCommands.map((command) => (
              <button
                key={`${command.extensionId}:${command.id}`}
                type="button"
                onClick={() => setSlashCommandId(command.id)}
                className={`h-8 rounded-md border px-3 text-xs ${
                  selectedSlashCommand?.id === command.id
                    ? 'border-[#4f46e5] bg-[#312e81] text-white'
                    : 'border-[#333] bg-[#202020] text-[#cfcfcf] hover:border-[#4f46e5]'
                }`}
                title={command.description}
              >
                /{command.title}
              </button>
            ))}
          </div>
        ) : (
          <div className="mt-3 rounded-md border border-[#303030] bg-[#1b1b1b] p-3 text-xs text-[#777]">
            No executable slash commands from active extensions.
          </div>
        )}
        {selectedSlashCommand && (
          <>
            <p className="mt-3 text-xs text-[#aaa]">
              {selectedSlashCommand.title}
              <span className="ml-2 text-[#666]">{selectedSlashCommand.extensionName}</span>
            </p>
            <textarea
              value={slashArgs}
              onChange={(event) => setSlashArgs(event.target.value)}
              placeholder="Slash command arguments"
              className="mt-2 h-24 w-full resize-none rounded-md border border-[#333] bg-[#111] p-3 font-mono text-xs text-[#ddd] outline-none focus:border-[#4f46e5]"
            />
            <button
              type="button"
              onClick={executeSlashCommand}
              disabled={!selectedSlashCommand.executable || busy === selectedSlashCommand.id}
              className="mt-3 h-8 rounded-md bg-[#4f46e5] px-3 text-xs font-medium text-white disabled:opacity-50"
            >
              Execute Slash Command
            </button>
          </>
        )}
        {slashResult && (
          <pre className="mt-4 max-h-[420px] overflow-auto rounded-md border border-[#303030] bg-[#111] p-3 font-mono text-xs text-[#d6d6d6]">
            {slashResult}
          </pre>
        )}
      </div>
    </div>
  );
}

function ContributionSummary({ contributions }: { contributions: NimbalystExtensionContributionSummary[] }) {
  const grouped = useMemo(() => {
    const map = new Map<string, NimbalystExtensionContributionSummary[]>();
    for (const contribution of contributions) {
      map.set(contribution.type, [...(map.get(contribution.type) || []), contribution]);
    }
    return Array.from(map.entries()).sort(([a], [b]) => a.localeCompare(b));
  }, [contributions]);

  if (contributions.length === 0) {
    return (
      <div className="mt-3 rounded-md border border-[#303030] bg-[#1b1b1b] p-3 text-xs text-[#777]">
        No manifest contributions discovered yet.
      </div>
    );
  }

  return (
    <div className="mt-3 space-y-3">
      {grouped.map(([type, items]) => (
        <div key={type} className="rounded-md border border-[#303030] bg-[#1b1b1b] p-2">
          <div className="flex items-center justify-between">
            <p className="text-xs font-semibold text-[#ededed]">{formatContributionType(type)}</p>
            <span className="rounded bg-[#303030] px-1.5 py-0.5 text-[10px] text-[#aaa]">{items.length}</span>
          </div>
          <div className="mt-2 space-y-1">
            {items.slice(0, 4).map((item) => (
              <div key={`${item.extensionId}:${item.type}:${item.id}`} className="rounded bg-[#242424] px-2 py-1.5">
                <p className="truncate text-[11px] text-[#d8d8d8]">{item.title}</p>
                <p className="truncate text-[10px] text-[#777]">{item.extensionName}</p>
              </div>
            ))}
            {items.length > 4 && (
              <p className="px-2 pt-1 text-[10px] text-[#777]">+{items.length - 4} more</p>
            )}
          </div>
        </div>
      ))}
    </div>
  );
}

function NimbalystPanelFrame({
  panel,
  workspacePath,
}: {
  panel: NimbalystExtensionContributionSummary;
  workspacePath: string;
}) {
  const apiBase = API_CONFIG.BASE_URL;
  const panelId = panel.id;
  const moduleUrl = `${apiBase}/nimbalyst/extensions/${encodeURIComponent(panel.extensionId)}/assets/${panel.main}`;
  const hostPayload = {
    extensionId: panel.extensionId,
    panelId,
    title: panel.title,
    workspacePath,
  };
  const srcDoc = `<!doctype html>
<html>
  <head>
    <meta charset="utf-8" />
    <style>
      html, body, #root { width: 100%; height: 100%; margin: 0; background: #151515; color: #e6e6e6; font-family: Inter, system-ui, sans-serif; }
      .error { padding: 16px; color: #fecaca; font: 12px/1.5 ui-monospace, SFMono-Regular, Menlo, monospace; white-space: pre-wrap; }
    </style>
    <script type="importmap">
      {
        "imports": {
          "react": "${apiBase}/nimbalyst/browser-shims/react.js",
          "react/jsx-runtime": "${apiBase}/nimbalyst/browser-shims/react-jsx-runtime.js",
          "react-dom/client": "${apiBase}/nimbalyst/browser-shims/react-dom-client.js",
          "@nimbalyst/extension-sdk": "${apiBase}/nimbalyst/browser-shims/extension-sdk.js",
          "@nimbalyst/runtime": "${apiBase}/nimbalyst/browser-shims/runtime.js"
        }
      }
    </script>
  </head>
  <body>
    <div id="root"></div>
    <script type="module">
      window.__BAT_RACK_PANEL_HOST__ = ${JSON.stringify(hostPayload)};
      ${buildNimbalystIframeElectronBridgeScript(apiBase, workspacePath)}
      window.__nimbalyst_extensions = window.parent.__nimbalyst_extensions || {};
      try {
        const React = await import('react');
        const { createRoot } = await import('react-dom/client');
        const module = await import(${JSON.stringify(moduleUrl)});
        const panel = module.panels?.[${JSON.stringify(panelId)}];
        const Component = panel?.component || panel?.default || module.default;
        if (!Component) {
          throw new Error('Panel component export not found for ${panel.extensionId}.${panelId}');
        }
        const storagePrefix = 'batrack:nimbalyst:' + ${JSON.stringify(panel.extensionId)} + ':';
        const readStored = (scope, key, fallback) => {
          const raw = window.localStorage.getItem(storagePrefix + scope + ':' + key);
          if (raw == null) return fallback;
          try { return JSON.parse(raw); } catch { return raw; }
        };
        const writeStored = (scope, key, value) => {
          window.localStorage.setItem(storagePrefix + scope + ':' + key, JSON.stringify(value));
        };
        const deleteStored = (scope, key) => {
          window.localStorage.removeItem(storagePrefix + scope + ':' + key);
        };
        const joinWorkspacePath = (targetPath = '') => {
          const value = String(targetPath || '');
          if (/^[A-Za-z]:[\\/]/.test(value) || value.startsWith('/') || value.startsWith('\\\\')) return value;
          const base = ${JSON.stringify(workspacePath)}.replace(/[\\/]+$/, '');
          return value ? base + '/' + value.replace(/^[\\/]+/, '') : base;
        };
        const readElectronFile = async (targetPath) => {
          const result = await window.electronAPI?.readFile?.(targetPath);
          if (!result?.success) throw new Error(result?.error || 'Failed to read file: ' + targetPath);
          return result.content || '';
        };
        const writeElectronFile = async (targetPath, content) => {
          const result = await window.electronAPI?.writeFile?.(targetPath, content);
          if (!result?.success) throw new Error(result?.error || 'Failed to write file: ' + targetPath);
        };
        const storage = {
          get: (key, fallback) => readStored('workspace', key, fallback),
          set: (key, value) => writeStored('workspace', key, value),
          delete: (key) => deleteStored('workspace', key),
          getGlobal: (key, fallback) => readStored('global', key, fallback),
          setGlobal: (key, value) => writeStored('global', key, value),
          deleteGlobal: (key) => deleteStored('global', key),
          getSecret: (key) => readStored('secret', key, undefined),
          setSecret: (key, value) => writeStored('secret', key, value),
          deleteSecret: (key) => deleteStored('secret', key),
        };
        const extensionDataDir = '.nimbalyst/extensions/' + ${JSON.stringify(panel.extensionId)};
        const host = {
          workspacePath: ${JSON.stringify(workspacePath)},
          extensionId: ${JSON.stringify(panel.extensionId)},
          panelId: ${JSON.stringify(panelId)},
          theme: 'dark',
          manifest: { id: ${JSON.stringify(panel.extensionId)} },
          isSettingsOpen: false,
          data: { query: async () => [] },
          ai: {
            setContext: (context) => { host.ai.context = context; },
            clearContext: () => { host.ai.context = null; },
            context: null,
          },
          storage,
          files: {
            getBasePath: async () => joinWorkspacePath(extensionDataDir),
            getGlobalBasePath: async () => joinWorkspacePath('.nimbalyst/extensions-global/' + ${JSON.stringify(panel.extensionId)}),
            write: async (relativePath, data) => writeElectronFile(joinWorkspacePath(extensionDataDir + '/' + relativePath), data instanceof Uint8Array ? new TextDecoder().decode(data) : data),
            readText: async (relativePath) => readElectronFile(joinWorkspacePath(extensionDataDir + '/' + relativePath)),
            read: async (relativePath) => new TextEncoder().encode(await readElectronFile(joinWorkspacePath(extensionDataDir + '/' + relativePath))),
            exists: async (relativePath) => {
              try { await readElectronFile(joinWorkspacePath(extensionDataDir + '/' + relativePath)); return true; } catch { return false; }
            },
            delete: async () => undefined,
            list: async (relativePath = '') => {
              const result = await window.electronAPI?.readDirectory?.(joinWorkspacePath(extensionDataDir + '/' + relativePath));
              return (result?.items || []).map((item) => item.name);
            },
            getUsage: async () => ({ usedBytes: 0, limitBytes: Number.MAX_SAFE_INTEGER }),
          },
          exec: async (command, options = {}) => {
            const response = await fetch(${JSON.stringify(`${apiBase}/nimbalyst/host/exec`)}, {
              method: 'POST',
              headers: { 'Content-Type': 'application/json' },
              body: JSON.stringify({
                command,
                workspacePath: ${JSON.stringify(workspacePath)},
                cwd: options.cwd || ${JSON.stringify(workspacePath)},
                env: options.env,
                timeout: options.timeout,
                maxBuffer: options.maxBuffer,
              }),
            });
            return response.json();
          },
          openFile: (targetPath) => window.parent.postMessage({ source: 'batrack-nimbalyst-panel', type: 'open-file', path: joinWorkspacePath(targetPath) }, '*'),
          openPanel: (nextPanelId) => window.parent.postMessage({ source: 'batrack-nimbalyst-panel', type: 'open-panel', panelId: nextPanelId }, '*'),
          close: () => window.parent.postMessage({ source: 'batrack-nimbalyst-panel', type: 'close', panelId: ${JSON.stringify(panelId)} }, '*'),
          openSettings: () => { host.isSettingsOpen = true; },
          closeSettings: () => { host.isSettingsOpen = false; },
          onThemeChanged: () => () => undefined,
          onWorkspaceEvent: () => () => undefined,
        };
        createRoot(document.getElementById('root')).render(React.createElement(Component, { host, workspacePath: host.workspacePath }));
        window.parent.postMessage({ source: 'batrack-nimbalyst-panel', ok: true, panelId: ${JSON.stringify(panelId)} }, '*');
      } catch (error) {
        const message = error?.stack || error?.message || String(error);
        document.getElementById('root').innerHTML = '<pre class="error"></pre>';
        document.querySelector('.error').textContent = message;
        window.parent.postMessage({ source: 'batrack-nimbalyst-panel', ok: false, panelId: ${JSON.stringify(panelId)}, error: message }, '*');
      }
    </script>
  </body>
</html>`;

  return (
    <div className="mt-3 overflow-hidden rounded-md border border-[#303030] bg-[#111]">
      <div className="flex items-center justify-between border-b border-[#303030] px-3 py-2">
        <div className="min-w-0">
          <p className="truncate text-xs font-semibold text-[#ededed]">{panel.title}</p>
          <p className="truncate text-[11px] text-[#777]">{panel.extensionName}</p>
        </div>
        <span className="rounded bg-[#252525] px-2 py-1 text-[10px] text-[#aaa]">iframe host</span>
      </div>
      <iframe
        title={`${panel.extensionName} ${panel.title}`}
        srcDoc={srcDoc}
        className="h-[360px] w-full border-0"
        sandbox="allow-scripts allow-same-origin allow-modals"
      />
    </div>
  );
}

function HostComponentsLayer({
  components,
  workspacePath,
}: {
  components: NimbalystExtensionContributionSummary[];
  workspacePath: string;
}) {
  if (components.length === 0) return null;

  return (
    <div className="pointer-events-none absolute right-4 top-4 z-50 flex max-w-[360px] flex-col gap-2">
      {components.map((component, index) => (
        <HostComponentFrame
          key={`${component.extensionId}:${component.id}`}
          component={component}
          workspacePath={workspacePath}
          index={index}
        />
      ))}
    </div>
  );
}

function HostComponentFrame({
  component,
  workspacePath,
  index,
}: {
  component: NimbalystExtensionContributionSummary;
  workspacePath: string;
  index: number;
}) {
  const apiBase = API_CONFIG.BASE_URL;
  const componentName = getHostComponentName(component);
  const moduleUrl = `${apiBase}/nimbalyst/extensions/${encodeURIComponent(component.extensionId)}/assets/${component.main}`;
  const hostPayload = {
    extensionId: component.extensionId,
    componentName,
    workspacePath,
  };
  const srcDoc = `<!doctype html>
<html>
  <head>
    <meta charset="utf-8" />
    <style>
      html, body, #root { width: 100%; height: 100%; margin: 0; background: transparent; color: #e6e6e6; font-family: Inter, system-ui, sans-serif; overflow: hidden; }
      .error { box-sizing: border-box; height: 100%; border: 1px solid #7f1d1d; border-radius: 8px; background: rgba(69, 10, 10, 0.92); padding: 10px; color: #fecaca; font: 11px/1.45 ui-monospace, SFMono-Regular, Menlo, monospace; white-space: pre-wrap; overflow: auto; }
    </style>
    <script type="importmap">
      {
        "imports": {
          "react": "${apiBase}/nimbalyst/browser-shims/react.js",
          "react/jsx-runtime": "${apiBase}/nimbalyst/browser-shims/react-jsx-runtime.js",
          "react-dom/client": "${apiBase}/nimbalyst/browser-shims/react-dom-client.js",
          "@nimbalyst/extension-sdk": "${apiBase}/nimbalyst/browser-shims/extension-sdk.js",
          "@nimbalyst/runtime": "${apiBase}/nimbalyst/browser-shims/runtime.js"
        }
      }
    </script>
  </head>
  <body>
    <div id="root"></div>
    <script type="module">
      window.__BAT_RACK_HOST_COMPONENT_HOST__ = ${JSON.stringify(hostPayload)};
      ${buildNimbalystIframeElectronBridgeScript(apiBase, workspacePath)}
      window.__nimbalyst_extensions = window.parent.__nimbalyst_extensions || {};
      try {
        const React = await import('react');
        const { createRoot } = await import('react-dom/client');
        const module = await import(${JSON.stringify(moduleUrl)});
        const Component = module.hostComponents?.[${JSON.stringify(componentName)}] || module.default?.hostComponents?.[${JSON.stringify(componentName)}];
        if (!Component) {
          throw new Error('Host component export not found: ${component.extensionId}.${componentName}');
        }
        createRoot(document.getElementById('root')).render(React.createElement(Component));
        window.parent.postMessage({ source: 'batrack-nimbalyst-host-component', ok: true, componentName: ${JSON.stringify(componentName)} }, '*');
      } catch (error) {
        const message = error?.stack || error?.message || String(error);
        document.getElementById('root').innerHTML = '<pre class="error"></pre>';
        document.querySelector('.error').textContent = message;
        window.parent.postMessage({ source: 'batrack-nimbalyst-host-component', ok: false, componentName: ${JSON.stringify(componentName)}, error: message }, '*');
      }
    </script>
  </body>
</html>`;

  return (
    <iframe
      title={`${component.extensionName} ${componentName}`}
      srcDoc={srcDoc}
      className="pointer-events-auto h-24 w-80 border-0"
      style={{ transform: `translateY(${index * 2}px)` }}
      sandbox="allow-scripts allow-same-origin allow-modals"
    />
  );
}

function formatContributionType(type: string) {
  return type.replace(/([a-z0-9])([A-Z])/g, '$1 $2').replace(/^./, (char) => char.toUpperCase());
}

function getHostComponentName(component: NimbalystExtensionContributionSummary) {
  const raw = component.raw as string | { component?: string; name?: string; value?: string };
  if (typeof raw === 'string') return raw;
  return raw?.component || raw?.name || raw?.value || component.id;
}

function getThemeContribution(contribution: NimbalystExtensionContributionSummary): NimbalystThemeContribution | null {
  const raw = contribution.raw as Partial<NimbalystThemeContribution>;
  if (!raw || typeof raw !== 'object') return null;
  return {
    id: typeof raw.id === 'string' ? raw.id : contribution.id,
    name: typeof raw.name === 'string' ? raw.name : contribution.title,
    isDark: raw.isDark !== false,
    colors: raw.colors && typeof raw.colors === 'object' ? raw.colors as Record<string, string> : {},
  };
}

function getThemeContributionId(contribution: NimbalystExtensionContributionSummary) {
  const theme = getThemeContribution(contribution);
  return `${contribution.extensionId}:${theme?.id || contribution.id}`;
}

function findThemeContribution(
  themeId: string,
  contributions: NimbalystExtensionContributionSummary[]
) {
  if (!themeId) return null;
  const contribution = contributions.find((item) => getThemeContributionId(item) === themeId);
  return contribution ? getThemeContribution(contribution) : null;
}

function buildNimbalystThemeStyle(theme: NimbalystThemeContribution | null): React.CSSProperties {
  if (!theme) return {};
  const colors = theme.colors || {};
  return {
    '--nim-bg': colors.bg,
    '--nim-bg-secondary': colors['bg-secondary'],
    '--nim-bg-tertiary': colors['bg-tertiary'],
    '--nim-bg-hover': colors['bg-hover'],
    '--nim-bg-selected': colors['bg-selected'],
    '--nim-bg-active': colors['bg-active'],
    '--nim-text': colors.text,
    '--nim-text-muted': colors['text-muted'],
    '--nim-text-faint': colors['text-faint'],
    '--nim-border': colors.border,
    '--nim-border-focus': colors['border-focus'],
    '--nim-primary': colors.primary,
    '--nim-primary-hover': colors['primary-hover'],
    '--nim-success': colors.success,
    '--nim-warning': colors.warning,
    '--nim-error': colors.error,
    '--nim-info': colors.info,
    backgroundColor: colors.bg || undefined,
    color: colors.text || undefined,
  } as React.CSSProperties;
}

function buildJsonDrafts(configuration: NimbalystExtensionConfiguration | null) {
  const jsonDrafts: Record<string, string> = {};
  if (!configuration) return jsonDrafts;
  for (const [key, property] of Object.entries(configuration.properties)) {
    if (property.type === 'array' || property.type === 'object') {
      jsonDrafts[key] = JSON.stringify(configuration.values[key] ?? property.default ?? null, null, 2);
    }
  }
  return jsonDrafts;
}

function coerceConfigurationValue(value: string, type: NimbalystConfigurationProperty['type']) {
  if (type === 'number') return Number(value);
  if (type === 'boolean') return value === 'true';
  return value;
}

function basename(filePath: string) {
  return filePath.split(/[\\/]/).pop() || filePath;
}

function buildNimbalystIframeElectronBridgeScript(apiBase: string, workspacePath: string) {
  return `
      const __batrackParentElectronAPI = window.parent.electronAPI || {};
      const __batrackWorkspacePath = ${JSON.stringify(workspacePath)};
      const __batrackApiBase = ${JSON.stringify(apiBase)};
      const __resolveWorkspacePath = (targetPath = '', fallbackWorkspacePath = __batrackWorkspacePath) => {
        const value = String(targetPath || '');
        if (/^[A-Za-z]:[\\\\/]/.test(value) || value.startsWith('/') || value.startsWith('\\\\\\\\')) return value;
        const base = String(fallbackWorkspacePath || '').replace(/[\\\\/]+$/, '');
        return value ? base + '/' + value.replace(/^[\\\\/]+/, '') : base;
      };
      const __hostExec = async (command, options = {}) => {
        const response = await fetch(__batrackApiBase + '/nimbalyst/host/exec', {
          method: 'POST',
          headers: { 'Content-Type': 'application/json' },
          body: JSON.stringify({
            command,
            workspacePath: options.workspacePath || __batrackWorkspacePath,
            cwd: options.cwd || options.workspacePath || __batrackWorkspacePath,
            env: options.env,
            timeout: options.timeout,
            maxBuffer: options.maxBuffer,
          }),
        });
        return response.json();
      };
      const __hostGit = async (args, options = {}) => {
        const response = await fetch(__batrackApiBase + '/nimbalyst/host/git', {
          method: 'POST',
          headers: { 'Content-Type': 'application/json' },
          body: JSON.stringify({
            args,
            workspacePath: options.workspacePath || __batrackWorkspacePath,
            cwd: options.cwd || options.workspacePath || __batrackWorkspacePath,
            env: options.env,
            timeout: options.timeout,
            maxBuffer: options.maxBuffer,
          }),
        });
        return response.json();
      };
      const __hostFile = async (operation, payload = {}) => {
        const response = await fetch(__batrackApiBase + '/nimbalyst/host/file/' + operation, {
          method: 'POST',
          headers: { 'Content-Type': 'application/json' },
          body: JSON.stringify({
            ...payload,
            workspacePath: payload.workspacePath || __batrackWorkspacePath,
          }),
        });
        return response.json();
      };
      const __binaryFromBase64 = (base64) => {
        const raw = atob(String(base64 || ''));
        const bytes = new Uint8Array(raw.length);
        for (let index = 0; index < raw.length; index += 1) {
          bytes[index] = raw.charCodeAt(index);
        }
        return bytes;
      };
      const __contentTypeForFile = (filePath) => {
        const lower = String(filePath || '').toLowerCase();
        if (lower.endsWith('.png')) return 'image/png';
        if (lower.endsWith('.jpg') || lower.endsWith('.jpeg')) return 'image/jpeg';
        if (lower.endsWith('.gif')) return 'image/gif';
        if (lower.endsWith('.webp')) return 'image/webp';
        if (lower.endsWith('.svg')) return 'image/svg+xml';
        return 'application/octet-stream';
      };
      const __nativeFetch = window.fetch.bind(window);
      window.fetch = async (input, init) => {
        const url = typeof input === 'string' ? input : input?.url;
        if (typeof url === 'string' && url.startsWith('file://')) {
          const filePath = decodeURIComponent(url.replace(/^file:\\/\\//, ''));
          const result = await __hostFile('read', { filePath });
          if (!result?.success) throw new Error(result?.error || 'Failed to read file');
          return new Response(__binaryFromBase64(result.base64), {
            status: 200,
            headers: { 'Content-Type': __contentTypeForFile(filePath) },
          });
        }
        return __nativeFetch(input, init);
      };
      const __gitLines = (value) => String(value || '').split(/\\r?\\n/).filter(Boolean);
      const __gitError = (result, fallback) => String(result?.stderr || result?.stdout || fallback || 'Git command failed').trim();
      const __gitActionResult = (result, fallback) => ({
        success: !!result?.success,
        error: result?.success ? undefined : __gitError(result, fallback),
      });
      const __normalizeGitPath = (pathValue) => {
        const value = String(pathValue || '').replace(/^"|"$/g, '');
        const renameArrow = ' -> ';
        return value.includes(renameArrow) ? value.slice(value.lastIndexOf(renameArrow) + renameArrow.length) : value;
      };
      const __gitStatus = async (gitWorkspacePath) => {
        const result = await __hostGit(['status', '--porcelain=v1', '--branch'], {
          workspacePath: gitWorkspacePath || __batrackWorkspacePath,
          timeout: 30000,
        });
        if (!result?.success) return { branch: '', ahead: 0, behind: 0, hasUncommitted: false };
        const lines = __gitLines(result.stdout);
        const branchLine = lines[0]?.startsWith('## ') ? lines[0].slice(3) : '';
        const branchText = branchLine.split('...')[0].replace(/^No commits yet on /, '') || 'HEAD';
        const aheadMatch = branchLine.match(/ahead (\\d+)/);
        const behindMatch = branchLine.match(/behind (\\d+)/);
        return {
          branch: branchText,
          ahead: aheadMatch ? Number(aheadMatch[1]) : 0,
          behind: behindMatch ? Number(behindMatch[1]) : 0,
          hasUncommitted: lines.slice(1).length > 0,
        };
      };
      const __gitBranches = async (gitWorkspacePath) => {
        const result = await __hostGit(['branch', '--format=%(HEAD)%09%(refname:short)'], {
          workspacePath: gitWorkspacePath || __batrackWorkspacePath,
          timeout: 30000,
        });
        if (!result?.success) return { branches: [], current: '' };
        const rows = __gitLines(result.stdout).map((line) => {
          const [head = '', name = ''] = line.split('\\t');
          return { head: head.trim(), name: name.trim() };
        }).filter((row) => row.name);
        return {
          branches: rows.map((row) => row.name),
          current: rows.find((row) => row.head === '*')?.name || '',
        };
      };
      const __gitLog = async (gitWorkspacePath, limit = 10, filters = {}) => {
        const safeLimit = Math.max(1, Math.min(Number(limit) || 10, 200));
        const args = ['log', '--pretty=format:%H%x1f%an%x1f%ad%x1f%s', '--date=iso', '-n', String(safeLimit)];
        if (filters?.since) args.push('--since=' + String(filters.since));
        if (filters?.author) args.push('--author=' + String(filters.author));
        if (filters?.branch) args.push(String(filters.branch));
        const result = await __hostGit(args, {
          workspacePath: gitWorkspacePath || __batrackWorkspacePath,
          timeout: 30000,
        });
        if (!result?.success) return [];
        return String(result.stdout || '')
          .split(/\\r?\\n/)
          .filter(Boolean)
          .map((line) => {
            const [hash = '', author = '', date = '', ...messageParts] = line.split('\\x1f');
            return { hash, author, date, message: messageParts.join('\\x1f') };
          });
      };
      const __gitWorkingChanges = async (gitWorkspacePath) => {
        const result = await __hostGit(['status', '--porcelain=v1', '-uall'], {
          workspacePath: gitWorkspacePath || __batrackWorkspacePath,
          timeout: 30000,
        });
        if (!result?.success) throw new Error(__gitError(result, 'Failed to load git changes'));
        const staged = [];
        const unstaged = [];
        const untracked = [];
        const conflicted = [];
        for (const line of __gitLines(result.stdout)) {
          const x = line[0] || ' ';
          const y = line[1] || ' ';
          const pathValue = __normalizeGitPath(line.slice(3));
          if (!pathValue) continue;
          if (x === '?' && y === '?') {
            untracked.push({ path: pathValue });
          } else if (x === 'U' || y === 'U' || (x === 'A' && y === 'A') || (x === 'D' && y === 'D')) {
            conflicted.push({ path: pathValue });
          } else {
            if (x !== ' ') staged.push({ path: pathValue, status: x });
            if (y !== ' ') unstaged.push({ path: pathValue, status: y });
          }
        }
        return { staged, unstaged, untracked, conflicted };
      };
      const __gitFileDiff = async (gitWorkspacePath, target = {}) => {
        const targetPath = String(target.path || '');
        if (!targetPath) return { unifiedDiff: '', isBinary: false };
        if (target.group === 'untracked') {
          return { unifiedDiff: '', isBinary: false };
        }
        const args = target.group === 'staged'
          ? ['diff', '--cached', '--', targetPath]
          : ['diff', '--', targetPath];
        const result = await __hostGit(args, {
          workspacePath: gitWorkspacePath || __batrackWorkspacePath,
          timeout: 30000,
          maxBuffer: 20 * 1024 * 1024,
        });
        const output = String(result?.stdout || result?.stderr || '');
        return { unifiedDiff: output, isBinary: /Binary files/i.test(output) };
      };
      const __gitCommitFileDiff = async (gitWorkspacePath, hash, filePath) => {
        const result = await __hostGit(['show', '--format=', '--patch', '--no-ext-diff', String(hash || ''), '--', String(filePath || '')], {
          workspacePath: gitWorkspacePath || __batrackWorkspacePath,
          timeout: 30000,
          maxBuffer: 20 * 1024 * 1024,
        });
        const output = String(result?.stdout || result?.stderr || '');
        return { unifiedDiff: output, isBinary: /Binary files/i.test(output) };
      };
      const __gitCommitDetail = async (gitWorkspacePath, hash) => {
        const workspace = gitWorkspacePath || __batrackWorkspacePath;
        const safeHash = String(hash || '');
        const [bodyResult, numstatResult, nameStatusResult] = await Promise.all([
          __hostGit(['show', '-s', '--format=%B', safeHash], { workspacePath: workspace, timeout: 30000 }),
          __hostGit(['show', '--format=', '--numstat', '--no-renames', safeHash], { workspacePath: workspace, timeout: 30000 }),
          __hostGit(['show', '--format=', '--name-status', '--no-renames', safeHash], { workspacePath: workspace, timeout: 30000 }),
        ]);
        const statusByPath = new Map();
        for (const line of __gitLines(nameStatusResult?.stdout)) {
          const [status = 'M', ...pathParts] = line.split('\\t');
          const pathValue = pathParts.join('\\t');
          if (pathValue) statusByPath.set(pathValue, status);
        }
        const files = __gitLines(numstatResult?.stdout).map((line) => {
          const [addedRaw = '0', deletedRaw = '0', ...pathParts] = line.split('\\t');
          const pathValue = pathParts.join('\\t');
          return {
            status: statusByPath.get(pathValue) || 'M',
            path: pathValue,
            added: addedRaw === '-' ? 0 : Number(addedRaw) || 0,
            deleted: deletedRaw === '-' ? 0 : Number(deletedRaw) || 0,
          };
        }).filter((file) => file.path);
        return {
          body: String(bodyResult?.stdout || '').trim(),
          files,
          summary: {
            filesChanged: files.length,
            insertions: files.reduce((sum, file) => sum + file.added, 0),
            deletions: files.reduce((sum, file) => sum + file.deleted, 0),
          },
        };
      };
      const __gitSimple = async (gitWorkspacePath, args, fallback) => {
        const result = await __hostGit(args, {
          workspacePath: gitWorkspacePath || __batrackWorkspacePath,
          timeout: 120000,
          maxBuffer: 20 * 1024 * 1024,
        });
        return __gitActionResult(result, fallback);
      };
      const __workspaceStateKey = (gitWorkspacePath) => 'batrack:nimbalyst:workspace-state:' + (gitWorkspacePath || __batrackWorkspacePath);
      const __getWorkspaceState = (gitWorkspacePath) => {
        try {
          return JSON.parse(window.localStorage.getItem(__workspaceStateKey(gitWorkspacePath)) || '{}');
        } catch {
          return {};
        }
      };
      const __updateWorkspaceState = (gitWorkspacePath, patch) => {
        const current = __getWorkspaceState(gitWorkspacePath);
        const next = { ...current, ...(patch || {}) };
        window.localStorage.setItem(__workspaceStateKey(gitWorkspacePath), JSON.stringify(next));
        return next;
      };
      const __postOpenFile = (payload) => {
        const workspace = payload?.workspacePath || __batrackWorkspacePath;
        const targetPath = typeof payload === 'string' ? payload : payload?.filePath || payload?.path;
        if (!targetPath) return false;
        window.parent.postMessage({
          source: 'batrack-nimbalyst-custom-editor',
          type: 'open-file',
          path: __resolveWorkspacePath(targetPath, workspace),
        }, '*');
        return true;
      };
      window.electronAPI = {
        ...__batrackParentElectronAPI,
        invoke: async (channel, ...args) => {
          if (channel === 'git:status') return __gitStatus(args[0]);
          if (channel === 'git:branches') return __gitBranches(args[0]);
          if (channel === 'git:log') return __gitLog(args[0], args[1], args[2] || {});
          if (channel === 'git:working-changes') return __gitWorkingChanges(args[0]);
          if (channel === 'git:file-diff') return __gitFileDiff(args[0], args[1]);
          if (channel === 'git:commit-file-diff') return __gitCommitFileDiff(args[0], args[1], args[2]);
          if (channel === 'git:commit-detail') return __gitCommitDetail(args[0], args[1]);
          if (channel === 'git:stage') return __gitSimple(args[0], ['add', '--', ...(Array.isArray(args[1]) ? args[1].map(String) : [])], 'Failed to stage files');
          if (channel === 'git:unstage') return __gitSimple(args[0], ['reset', 'HEAD', '--', ...(Array.isArray(args[1]) ? args[1].map(String) : [])], 'Failed to unstage files');
          if (channel === 'git:discard-changes') return __gitSimple(args[0], ['checkout', '--', ...(Array.isArray(args[1]) ? args[1].map(String) : [])], 'Failed to discard changes');
          if (channel === 'git:commit') return __gitSimple(args[0], ['commit', '-m', String(args[1] || '')], 'Commit failed');
          if (channel === 'git:push') return __gitSimple(args[0], ['push'], 'Push failed');
          if (channel === 'git:pull') return __gitSimple(args[0], ['pull', ...(args[1]?.rebase ? ['--rebase'] : []), ...(args[1]?.ffOnly ? ['--ff-only'] : [])], 'Pull failed');
          if (channel === 'git:fetch') return __gitSimple(args[0], ['fetch'], 'Fetch failed');
          if (channel === 'git:checkout') return __gitSimple(args[0], ['checkout', String(args[1] || '')], 'Checkout failed');
          if (channel === 'git:cherry-pick') return __gitSimple(args[0], ['cherry-pick', String(args[1] || '')], 'Cherry-pick failed');
          if (channel === 'git:create-branch') return __gitSimple(args[0], ['branch', String(args[1] || ''), String(args[2] || '')], 'Create branch failed');
          if (channel === 'ai:getSettings') return { diffPeekSize: 4000 };
          if (channel === 'ai:saveSettings') return { success: true };
          if (channel === 'workspace:get-state') return __getWorkspaceState(args[0]);
          if (channel === 'workspace:update-state') return __updateWorkspaceState(args[0], args[1]);
          if (channel === 'session-files:get-sessions-by-file') return { success: true, sessionIds: [] };
          if (channel === 'sessions:list') return { success: true, sessions: [] };
          if (channel === 'session-files:get-by-sessions') return { success: true, files: [] };
          if (channel === 'extensions:read-file') {
            const result = await __hostFile('read', { filePath: args[0] });
            if (!result?.success) throw new Error(result?.error || 'Failed to read file');
            return result.base64 || result.content || '';
          }
          if (channel === 'extensions:write-binary') {
            const result = await __hostFile('write-binary', { filePath: args[0], base64: args[1] });
            if (!result?.success) throw new Error(result?.error || 'Failed to write binary file');
            return result;
          }
          if (channel === 'workspace:open-file') return __postOpenFile(args[0]);
          if (channel === 'mockup:screenshot-result') return undefined;
          if (__batrackParentElectronAPI.invoke) return __batrackParentElectronAPI.invoke(channel, ...args);
          throw new Error('Electron IPC channel is not available: ' + channel);
        },
        on: (channel, callback) => {
          if (__batrackParentElectronAPI.on) return __batrackParentElectronAPI.on(channel, callback);
          return () => undefined;
        },
      };
  `;
}

function cssEscape(value: string) {
  return typeof CSS !== 'undefined' && CSS.escape
    ? CSS.escape(value)
    : value.replace(/["\\]/g, '\\$&');
}

function findCustomEditorContribution(
  filePath: string,
  contributions: NimbalystExtensionContributionSummary[]
) {
  const fileName = basename(filePath).toLowerCase();
  return contributions.find((contribution) => {
    const raw = contribution.raw as { filePatterns?: string[] };
    return (raw.filePatterns || []).some((pattern) => matchesGlob(fileName, pattern.toLowerCase()));
  });
}

function findDocumentHeaderContributions(
  filePath: string,
  contributions: NimbalystExtensionContributionSummary[]
) {
  const fileName = basename(filePath).toLowerCase();
  return contributions
    .filter((contribution) => {
      const raw = contribution.raw as { filePatterns?: string[] };
      return (raw.filePatterns || []).some((pattern) => matchesGlob(fileName, pattern.toLowerCase()));
    })
    .sort((a, b) => {
      const rawA = a.raw as { priority?: number };
      const rawB = b.raw as { priority?: number };
      return (rawB.priority ?? 50) - (rawA.priority ?? 50);
    });
}

function findFileIconName(
  filePath: string,
  contributions: NimbalystExtensionContributionSummary[]
) {
  const fileName = basename(filePath).toLowerCase();
  const match = contributions.find((contribution) => {
    const raw = contribution.raw as { pattern?: string; icon?: string };
    if (!raw.pattern || !raw.icon) return false;
    return matchesGlob(fileName, raw.pattern.toLowerCase());
  });
  return (match?.raw as { icon?: string } | undefined)?.icon;
}

function matchesGlob(fileName: string, pattern: string) {
  if (pattern.startsWith('*.')) return fileName.endsWith(pattern.slice(1));
  if (!pattern.includes('*')) return fileName === pattern;
  const source = pattern
    .split('*')
    .map((part) => part.replace(/[|\\{}()[\]^$+?.]/g, '\\$&'))
    .join('.*');
  return new RegExp(`^${source}$`).test(fileName);
}

function isEditableEventTarget(target: EventTarget | null) {
  if (!(target instanceof HTMLElement)) return false;
  const tag = target.tagName.toLowerCase();
  return tag === 'input' || tag === 'textarea' || tag === 'select' || target.isContentEditable;
}

function eventMatchesManifestKey(event: KeyboardEvent, manifestKey: string) {
  const parts = manifestKey.toLowerCase().split('+').map((part) => part.trim()).filter(Boolean);
  const key = parts.find((part) => !['ctrl', 'shift', 'alt', 'cmd'].includes(part));
  const wantsCtrl = parts.includes('ctrl');
  const wantsShift = parts.includes('shift');
  const wantsAlt = parts.includes('alt');
  const wantsCmd = parts.includes('cmd');
  const isMac = navigator.platform.startsWith('Mac');

  if (event.shiftKey !== wantsShift) return false;
  if (event.altKey !== wantsAlt) return false;
  if (wantsCmd) {
    const cmdPressed = isMac ? event.metaKey : event.ctrlKey;
    if (!cmdPressed) return false;
    if (isMac && event.ctrlKey) return false;
  } else if (event.metaKey) {
    return false;
  }
  if (wantsCtrl && !event.ctrlKey) return false;
  if (!wantsCtrl && !wantsCmd && event.ctrlKey) return false;

  return normalizeKeyboardEventKey(event) === key;
}

function normalizeKeyboardEventKey(event: KeyboardEvent) {
  if (event.key === ' ') return 'space';
  if (event.key === '`') return 'backquote';
  return event.key.toLowerCase();
}

function SettingsMode({
  status,
  settingsPanels,
  configurations,
  backendModules,
  themes,
  activeThemeId,
  onSelectTheme,
  workspacePath,
}: {
  status: ReferenceStatus | null;
  settingsPanels: NimbalystExtensionContributionSummary[];
  configurations: NimbalystExtensionContributionSummary[];
  backendModules: NimbalystExtensionContributionSummary[];
  themes: NimbalystExtensionContributionSummary[];
  activeThemeId: string;
  onSelectTheme: (themeId: string) => void;
  workspacePath: string;
}) {
  const sortedPanels = useMemo(
    () =>
      [...settingsPanels].sort((a, b) => {
        const rawA = a.raw as { order?: number };
        const rawB = b.raw as { order?: number };
        return (rawA.order ?? 100) - (rawB.order ?? 100);
      }),
    [settingsPanels]
  );

  return (
    <div className="p-5">
      <h2 className="text-lg font-semibold">Nimbalyst Host Settings</h2>
      <div className="mt-4 max-w-4xl rounded-md border border-[#303030] bg-[#202020] p-4">
        <p className="text-sm font-semibold">Reference commands</p>
        <div className="mt-3 space-y-1">
          {(status?.suggestedCommands || []).map((command) => (
            <code key={command} className="block rounded bg-[#111] px-3 py-2 text-xs text-[#d6d6d6]">
              {command}
            </code>
          ))}
        </div>
        {status?.node && !status.node.supported && (
          <p className="mt-3 rounded border border-amber-900 bg-amber-950/40 px-3 py-2 text-xs leading-5 text-amber-200">
            Current Node is {status.node.version}. Nimbalyst's toolchain requires {status.node.required}.
          </p>
        )}
      </div>

      <div className="mt-4 max-w-4xl rounded-md border border-[#303030] bg-[#202020] p-4">
        <p className="text-sm font-semibold">Extension Themes</p>
        <p className="mt-1 text-xs text-[#888]">
          Themes declared through Nimbalyst `contributions.themes` can be selected for this workbench.
        </p>
        <ExtensionThemesPanel
          themes={themes}
          activeThemeId={activeThemeId}
          onSelectTheme={onSelectTheme}
        />
      </div>

      <div className="mt-4 max-w-4xl rounded-md border border-[#303030] bg-[#202020] p-4">
        <p className="text-sm font-semibold">Privileged Backend Modules</p>
        <p className="mt-1 text-xs text-[#888]">
          Backend modules declared through Nimbalyst `contributions.backendModules` require explicit consent before the Bat Rack worker runtime can execute them.
        </p>
        <BackendModulesPanel
          contributions={backendModules}
          workspacePath={workspacePath}
        />
      </div>

      <div className="mt-4 max-w-4xl rounded-md border border-[#303030] bg-[#202020] p-4">
        <p className="text-sm font-semibold">Extension Configuration</p>
        <p className="mt-1 text-xs text-[#888]">
          JSON-schema-like settings declared through Nimbalyst `contributions.configuration` are persisted by the Bat Rack extension host.
        </p>
        {configurations.length > 0 ? (
          <div className="mt-3 space-y-3">
            {configurations.map((configuration) => (
              <ConfigurationPanel
                key={`${configuration.extensionId}:configuration`}
                contribution={configuration}
              />
            ))}
          </div>
        ) : (
          <div className="mt-3 rounded-md border border-[#303030] bg-[#1b1b1b] p-3 text-xs text-[#777]">
            No configuration contributions discovered.
          </div>
        )}
      </div>

      <div className="mt-4 max-w-4xl rounded-md border border-[#303030] bg-[#202020] p-4">
        <p className="text-sm font-semibold">Extension Settings Panels</p>
        <p className="mt-1 text-xs text-[#888]">
          Panels declared through Nimbalyst `contributions.settingsPanel` are rendered with the Bat Rack iframe host.
        </p>
        {sortedPanels.length > 0 ? (
          <div className="mt-3 space-y-3">
            {sortedPanels.map((panel) => (
              <SettingsPanelFrame
                key={`${panel.extensionId}:${panel.id}:${workspacePath}`}
                panel={panel}
                workspacePath={workspacePath}
              />
            ))}
          </div>
        ) : (
          <div className="mt-3 rounded-md border border-[#303030] bg-[#1b1b1b] p-3 text-xs text-[#777]">
            No settings panel contributions discovered.
          </div>
        )}
      </div>
    </div>
  );
}

function ExtensionThemesPanel({
  themes,
  activeThemeId,
  onSelectTheme,
}: {
  themes: NimbalystExtensionContributionSummary[];
  activeThemeId: string;
  onSelectTheme: (themeId: string) => void;
}) {
  if (themes.length === 0) {
    return (
      <div className="mt-3 rounded-md border border-[#303030] bg-[#1b1b1b] p-3 text-xs text-[#777]">
        No extension theme contributions discovered.
      </div>
    );
  }

  return (
    <div className="mt-3 grid gap-3 md:grid-cols-2">
      <button
        type="button"
        onClick={() => onSelectTheme('')}
        className={`rounded-md border p-3 text-left text-xs ${
          !activeThemeId
            ? 'border-[#4f46e5] bg-[#312e81] text-white'
            : 'border-[#303030] bg-[#1b1b1b] text-[#d8d8d8] hover:border-[#4f46e5]'
        }`}
      >
        <span className="block font-semibold">Bat Rack Default</span>
        <span className="mt-1 block text-[11px] text-[#aaa]">Use the host default colors.</span>
      </button>
      {themes.map((contribution) => {
        const theme = getThemeContribution(contribution);
        const themeId = getThemeContributionId(contribution);
        const colors = theme?.colors || {};
        return (
          <button
            key={`${contribution.extensionId}:${contribution.id}`}
            type="button"
            onClick={() => onSelectTheme(themeId)}
            className={`rounded-md border p-3 text-left text-xs ${
              activeThemeId === themeId
                ? 'border-[#4f46e5] bg-[#312e81] text-white'
                : 'border-[#303030] bg-[#1b1b1b] text-[#d8d8d8] hover:border-[#4f46e5]'
            }`}
          >
            <span className="flex items-center gap-2 font-semibold">
              <span
                className="inline-block size-3 rounded-full border border-white/20"
                style={{ backgroundColor: colors.primary || colors.bg || '#4f46e5' }}
              />
              {theme?.name || contribution.title}
            </span>
            <span className="mt-1 block text-[11px] text-[#aaa]">
              {theme?.isDark ? 'Dark' : 'Light'} · {contribution.extensionName}
            </span>
          </button>
        );
      })}
    </div>
  );
}

function BackendModulesPanel({
  contributions,
  workspacePath,
}: {
  contributions: NimbalystExtensionContributionSummary[];
  workspacePath: string;
}) {
  const extensionIds = useMemo(
    () => Array.from(new Set(contributions.map((contribution) => contribution.extensionId))).sort(),
    [contributions]
  );

  if (extensionIds.length === 0) {
    return (
      <div className="mt-3 rounded-md border border-[#303030] bg-[#1b1b1b] p-3 text-xs text-[#777]">
        No backend module contributions discovered.
      </div>
    );
  }

  return (
    <div className="mt-3 space-y-3">
      {extensionIds.map((extensionId) => (
        <BackendModuleExtensionCard
          key={extensionId}
          extensionId={extensionId}
          workspacePath={workspacePath}
        />
      ))}
    </div>
  );
}

function BackendModuleExtensionCard({
  extensionId,
  workspacePath,
}: {
  extensionId: string;
  workspacePath: string;
}) {
  const [snapshot, setSnapshot] = useState<NimbalystExtensionBackendModules | null>(null);
  const [busy, setBusy] = useState<string | null>(null);
  const [error, setError] = useState<string | null>(null);

  const loadSnapshot = useCallback(async () => {
    const result = await apiClient.getNimbalystExtensionBackendModules(extensionId, workspacePath);
    if (result.error) throw new Error(result.error);
    setSnapshot(result.data?.backendModules || null);
  }, [extensionId, workspacePath]);

  useEffect(() => {
    let mounted = true;
    setError(null);
    apiClient.getNimbalystExtensionBackendModules(extensionId, workspacePath).then((result) => {
      if (!mounted) return;
      if (result.error) {
        setError(result.error);
        return;
      }
      setSnapshot(result.data?.backendModules || null);
    });
    return () => {
      mounted = false;
    };
  }, [extensionId, workspacePath]);

  const setGrant = async (moduleId: string, scope: 'workspace' | 'global', enabled: boolean) => {
    setBusy(`${moduleId}:${scope}`);
    setError(null);
    try {
      const result = await apiClient.setNimbalystExtensionBackendModuleGrant({
        extensionId,
        moduleId,
        scope,
        enabled,
        workspacePath,
      });
      if (result.error) throw new Error(result.error);
      setSnapshot(result.data?.backendModules || null);
      await loadSnapshot();
    } catch (grantError) {
      setError(grantError instanceof Error ? grantError.message : String(grantError));
    } finally {
      setBusy(null);
    }
  };

  return (
    <div className="rounded-md border border-[#303030] bg-[#151515] p-3">
      <div className="flex items-center justify-between gap-3">
        <div className="min-w-0">
          <p className="truncate text-xs font-semibold text-[#ededed]">{snapshot?.extensionName || extensionId}</p>
          <p className="truncate text-[11px] text-[#777]">{extensionId}</p>
        </div>
        <button
          type="button"
          onClick={() => loadSnapshot().catch((loadError) => setError(loadError instanceof Error ? loadError.message : String(loadError)))}
          className="h-7 rounded border border-[#3a3a3a] px-2 text-xs text-[#cfcfcf] hover:border-[#4f46e5]"
        >
          Refresh
        </button>
      </div>
      <div className="mt-3 space-y-2">
        {(snapshot?.modules || []).map((module) => (
          <BackendModuleCard
            key={module.id}
            extensionId={extensionId}
            module={module}
            permissions={snapshot?.permissions || []}
            workspacePath={workspacePath}
            busy={busy}
            onSetGrant={setGrant}
          />
        ))}
      </div>
      {snapshot?.issues?.some((issue) => !issue.moduleId) && (
        <div className="mt-3 space-y-1">
          {snapshot.issues.filter((issue) => !issue.moduleId).map((issue) => (
            <p key={issue.message} className="rounded border border-red-900 bg-red-950/40 px-3 py-2 text-xs text-red-200">
              {issue.message}
            </p>
          ))}
        </div>
      )}
      {error && <p className="mt-3 rounded border border-red-900 bg-red-950/40 px-3 py-2 text-xs text-red-200">{error}</p>}
    </div>
  );
}

function BackendModuleCard({
  extensionId,
  module,
  permissions,
  workspacePath,
  busy,
  onSetGrant,
}: {
  extensionId: string;
  module: NimbalystBackendModuleSummary;
  permissions: NimbalystBackendPermissionDescriptor[];
  workspacePath: string;
  busy: string | null;
  onSetGrant: (moduleId: string, scope: 'workspace' | 'global', enabled: boolean) => void;
}) {
  const permissionById = useMemo(
    () => new Map(permissions.map((permission) => [permission.id, permission])),
    [permissions]
  );
  const hasFatalIssue = module.issues.some((issue) => issue.severity !== 'warning');
  const workspaceBusy = busy === `${module.id}:workspace`;
  const globalBusy = busy === `${module.id}:global`;
  const enabled = module.workspaceEnabled || module.globalEnabled;
  const [method, setMethod] = useState('describe');
  const [paramsDraft, setParamsDraft] = useState('{}');
  const [execution, setExecution] = useState<string>('');
  const [executing, setExecuting] = useState(false);

  const runBackendModule = async () => {
    setExecuting(true);
    try {
      const params = paramsDraft.trim() ? JSON.parse(paramsDraft) : {};
      if (!params || typeof params !== 'object' || Array.isArray(params)) {
        throw new Error('Params must be a JSON object.');
      }
      const result = await apiClient.executeNimbalystBackendModule({
        extensionId,
        moduleId: module.id,
        method: method.trim() || 'describe',
        params: params as Record<string, unknown>,
        workspacePath,
      });
      if (result.error) throw new Error(result.error);
      setExecution(JSON.stringify(result.data, null, 2));
    } catch (runError) {
      setExecution(JSON.stringify({
        success: false,
        error: runError instanceof Error ? runError.message : String(runError),
      }, null, 2));
    } finally {
      setExecuting(false);
    }
  };

  return (
    <div className="rounded-md border border-[#303030] bg-[#202020] p-3 text-xs">
      <div className="flex flex-wrap items-start justify-between gap-3">
        <div className="min-w-0">
          <p className="font-semibold text-[#ededed]">
            {module.id}
            <span className="ml-2 rounded bg-[#303030] px-1.5 py-0.5 text-[10px] font-normal text-[#aaa]">{module.runtime}</span>
          </p>
          <p className="mt-1 truncate font-mono text-[11px] text-[#777]">{module.entry}</p>
        </div>
        <span className={`rounded px-1.5 py-0.5 text-[10px] ${module.entryExists ? 'bg-emerald-950 text-emerald-300' : 'bg-amber-950 text-amber-300'}`}>
          {module.entryExists ? 'Entry found' : 'Entry missing'}
        </span>
      </div>
      {module.enablement?.purpose && (
        <p className="mt-2 text-[11px] leading-4 text-[#aaa]">{module.enablement.purpose}</p>
      )}
      <div className="mt-3 space-y-1">
        {module.permissions.length > 0 ? module.permissions.map((permissionId) => {
          const descriptor = permissionById.get(permissionId);
          return (
            <div key={permissionId} className="flex gap-2 rounded bg-[#171717] px-2 py-1.5">
              <span className={descriptor?.risk === 'high' ? 'text-red-300' : descriptor?.risk === 'elevated' ? 'text-amber-300' : 'text-[#aaa]'}>
                {descriptor?.risk || 'low'}
              </span>
              <span className="min-w-0 flex-1">
                <span className="font-medium text-[#d8d8d8]">{descriptor?.label || permissionId}</span>
                {descriptor?.description && <span className="ml-1 text-[#777]">{descriptor.description}</span>}
              </span>
            </div>
          );
        }) : (
          <p className="rounded bg-[#171717] px-2 py-1.5 text-[#777]">No host-brokered permissions declared.</p>
        )}
      </div>
      {module.issues.length > 0 && (
        <div className="mt-3 space-y-1">
          {module.issues.map((issue) => (
            <p
              key={issue.message}
              className={`rounded border px-2 py-1.5 text-[11px] ${
                issue.severity === 'warning'
                  ? 'border-amber-900 bg-amber-950/40 text-amber-200'
                  : 'border-red-900 bg-red-950/40 text-red-200'
              }`}
            >
              {issue.message}
            </p>
          ))}
        </div>
      )}
      <div className="mt-3 flex flex-wrap gap-2">
        <button
          type="button"
          disabled={!workspacePath || hasFatalIssue || workspaceBusy}
          onClick={() => onSetGrant(module.id, 'workspace', !module.workspaceEnabled)}
          className={`h-8 rounded border px-3 text-xs disabled:opacity-50 ${
            module.workspaceEnabled
              ? 'border-emerald-800 bg-emerald-950 text-emerald-200'
              : 'border-[#3a3a3a] bg-[#171717] text-[#cfcfcf] hover:border-[#4f46e5]'
          }`}
        >
          {workspaceBusy ? 'Saving...' : module.workspaceEnabled ? 'Workspace Enabled' : 'Enable Workspace'}
        </button>
        <button
          type="button"
          disabled={hasFatalIssue || globalBusy}
          onClick={() => onSetGrant(module.id, 'global', !module.globalEnabled)}
          className={`h-8 rounded border px-3 text-xs disabled:opacity-50 ${
            module.globalEnabled
              ? 'border-emerald-800 bg-emerald-950 text-emerald-200'
              : 'border-[#3a3a3a] bg-[#171717] text-[#cfcfcf] hover:border-[#4f46e5]'
          }`}
        >
          {globalBusy ? 'Saving...' : module.globalEnabled ? 'Global Enabled' : 'Enable Global'}
        </button>
      </div>
      <div className="mt-3 rounded border border-[#303030] bg-[#171717] p-3">
        <div className="flex flex-wrap items-end gap-2">
          <label className="flex min-w-[160px] flex-1 flex-col gap-1 text-[11px] text-[#888]">
            Method
            <input
              value={method}
              onChange={(event) => setMethod(event.target.value)}
              className="h-8 rounded border border-[#333] bg-[#111] px-2 text-xs text-[#ddd] outline-none focus:border-[#4f46e5]"
            />
          </label>
          <button
            type="button"
            disabled={!enabled || !module.entryExists || hasFatalIssue || module.runtime !== 'worker-thread' || executing}
            onClick={runBackendModule}
            className="h-8 rounded border border-[#4f46e5] bg-[#312e81] px-3 text-xs text-white disabled:border-[#333] disabled:bg-[#222] disabled:text-[#777]"
          >
            {executing ? 'Running...' : 'Execute'}
          </button>
        </div>
        <label className="mt-2 flex flex-col gap-1 text-[11px] text-[#888]">
          Params
          <textarea
            value={paramsDraft}
            onChange={(event) => setParamsDraft(event.target.value)}
            rows={3}
            className="resize-y rounded border border-[#333] bg-[#111] px-2 py-1.5 font-mono text-xs text-[#ddd] outline-none focus:border-[#4f46e5]"
          />
        </label>
        {!enabled && (
          <p className="mt-2 text-[11px] text-[#777]">Enable this backend module before execution.</p>
        )}
        {execution && (
          <pre className="mt-2 max-h-44 overflow-auto rounded bg-[#0f0f0f] p-2 text-[11px] text-[#cfcfcf]">
            {execution}
          </pre>
        )}
      </div>
    </div>
  );
}

function ConfigurationPanel({
  contribution,
}: {
  contribution: NimbalystExtensionContributionSummary;
}) {
  const [configuration, setConfiguration] = useState<NimbalystExtensionConfiguration | null>(null);
  const [draft, setDraft] = useState<Record<string, unknown>>({});
  const [jsonDrafts, setJsonDrafts] = useState<Record<string, string>>({});
  const [busy, setBusy] = useState(false);
  const [error, setError] = useState<string | null>(null);
  const [saved, setSaved] = useState(false);

  useEffect(() => {
    let mounted = true;
    setError(null);
    apiClient.getNimbalystExtensionConfiguration(contribution.extensionId).then((result) => {
      if (!mounted) return;
      if (result.error) {
        setError(result.error);
        return;
      }
      const nextConfiguration = result.data?.configuration || null;
      setConfiguration(nextConfiguration);
      setDraft(nextConfiguration?.values || {});
      setJsonDrafts(buildJsonDrafts(nextConfiguration));
    });
    return () => {
      mounted = false;
    };
  }, [contribution.extensionId]);

  const sortedProperties = useMemo(() => {
    const properties = configuration?.properties || {};
    return Object.entries(properties).sort(([, a], [, b]) => (a.order ?? 100) - (b.order ?? 100));
  }, [configuration]);

  const updateDraft = (key: string, value: unknown) => {
    setDraft((current) => ({ ...current, [key]: value }));
    setSaved(false);
  };

  const updateJsonDraft = (key: string, value: string) => {
    setJsonDrafts((current) => ({ ...current, [key]: value }));
    try {
      updateDraft(key, value.trim() ? JSON.parse(value) : undefined);
      setError(null);
    } catch {
      setError(`${key} must be valid JSON before saving.`);
      setSaved(false);
    }
  };

  const saveConfiguration = async () => {
    setBusy(true);
    setError(null);
    setSaved(false);
    try {
      const result = await apiClient.updateNimbalystExtensionConfiguration(contribution.extensionId, draft);
      if (result.error) throw new Error(result.error);
      const nextConfiguration = result.data?.configuration || null;
      setConfiguration(nextConfiguration);
      setDraft(nextConfiguration?.values || {});
      setJsonDrafts(buildJsonDrafts(nextConfiguration));
      setSaved(true);
    } catch (saveError) {
      setError(saveError instanceof Error ? saveError.message : String(saveError));
    } finally {
      setBusy(false);
    }
  };

  return (
    <div className="rounded-md border border-[#303030] bg-[#151515] p-3">
      <div className="flex items-start justify-between gap-3">
        <div className="min-w-0">
          <p className="truncate text-xs font-semibold text-[#ededed]">
            {configuration?.title || contribution.title}
          </p>
          <p className="truncate text-[11px] text-[#777]">{contribution.extensionName}</p>
        </div>
        <button
          type="button"
          onClick={saveConfiguration}
          disabled={busy || Boolean(error?.includes('valid JSON'))}
          className="h-8 rounded-md bg-[#4f46e5] px-3 text-xs font-medium text-white disabled:opacity-50"
        >
          {busy ? 'Saving...' : saved ? 'Saved' : 'Save'}
        </button>
      </div>
      <div className="mt-3 grid gap-3">
        {sortedProperties.map(([key, property]) => (
          <label key={key} className="grid gap-1 text-xs">
            <span className="font-medium text-[#d8d8d8]">{key}</span>
            {property.description && <span className="text-[11px] leading-4 text-[#777]">{property.description}</span>}
            {property.enum?.length ? (
              <select
                value={String(draft[key] ?? property.default ?? '')}
                onChange={(event) => updateDraft(key, coerceConfigurationValue(event.target.value, property.type))}
                className="h-9 rounded-md border border-[#333] bg-[#111] px-2 text-xs text-[#ddd] outline-none focus:border-[#4f46e5]"
              >
                {property.enum.map((value, index) => (
                  <option key={String(value)} value={String(value)}>
                    {property.enumDescriptions?.[index] || String(value)}
                  </option>
                ))}
              </select>
            ) : property.type === 'boolean' ? (
              <input
                type="checkbox"
                checked={Boolean(draft[key])}
                onChange={(event) => updateDraft(key, event.target.checked)}
                className="size-4 accent-[#4f46e5]"
              />
            ) : property.type === 'number' ? (
              <input
                type="number"
                value={typeof draft[key] === 'number' ? String(draft[key]) : ''}
                min={property.minimum}
                max={property.maximum}
                onChange={(event) => updateDraft(key, event.target.value === '' ? undefined : Number(event.target.value))}
                className="h-9 rounded-md border border-[#333] bg-[#111] px-2 text-xs text-[#ddd] outline-none focus:border-[#4f46e5]"
              />
            ) : property.type === 'array' || property.type === 'object' ? (
              <textarea
                value={jsonDrafts[key] ?? ''}
                onChange={(event) => updateJsonDraft(key, event.target.value)}
                className="h-24 resize-none rounded-md border border-[#333] bg-[#111] p-2 font-mono text-xs text-[#ddd] outline-none focus:border-[#4f46e5]"
              />
            ) : (
              <input
                type="text"
                value={String(draft[key] ?? '')}
                placeholder={property.placeholder}
                onChange={(event) => updateDraft(key, event.target.value)}
                className="h-9 rounded-md border border-[#333] bg-[#111] px-2 text-xs text-[#ddd] outline-none focus:border-[#4f46e5]"
              />
            )}
          </label>
        ))}
        {sortedProperties.length === 0 && (
          <p className="rounded bg-[#202020] px-3 py-2 text-xs text-[#777]">No configurable properties.</p>
        )}
      </div>
      {error && <p className="mt-3 rounded border border-red-900 bg-red-950/40 px-3 py-2 text-xs text-red-200">{error}</p>}
    </div>
  );
}

function SettingsPanelFrame({
  panel,
  workspacePath,
}: {
  panel: NimbalystExtensionContributionSummary;
  workspacePath: string;
}) {
  const apiBase = API_CONFIG.BASE_URL;
  const raw = panel.raw as { component?: string; title?: string };
  const componentName = raw?.component || panel.id;
  const moduleUrl = `${apiBase}/nimbalyst/extensions/${encodeURIComponent(panel.extensionId)}/assets/${panel.main}`;
  const storagePrefix = `batrack:nimbalyst:settings:${panel.extensionId}`;
  const workspaceStoragePrefix = `${storagePrefix}:workspace:${workspacePath || 'default'}`;

  const srcDoc = `<!doctype html>
<html>
  <head>
    <meta charset="utf-8" />
    <style>
      html, body, #root { width: 100%; height: 100%; margin: 0; background: #151515; color: #e6e6e6; font-family: Inter, system-ui, sans-serif; }
      .error { padding: 16px; color: #fecaca; font: 12px/1.5 ui-monospace, SFMono-Regular, Menlo, monospace; white-space: pre-wrap; }
    </style>
    <script type="importmap">
      {
        "imports": {
          "react": "${apiBase}/nimbalyst/browser-shims/react.js",
          "react/jsx-runtime": "${apiBase}/nimbalyst/browser-shims/react-jsx-runtime.js",
          "react-dom/client": "${apiBase}/nimbalyst/browser-shims/react-dom-client.js",
          "@nimbalyst/extension-sdk": "${apiBase}/nimbalyst/browser-shims/extension-sdk.js",
          "@nimbalyst/runtime": "${apiBase}/nimbalyst/browser-shims/runtime.js"
        }
      }
    </script>
  </head>
  <body>
    <div id="root"></div>
    <script type="module">
      window.__nimbalyst_extensions = window.parent.__nimbalyst_extensions || {};
      try {
        const React = await import('react');
        const { createRoot } = await import('react-dom/client');
        const module = await import(${JSON.stringify(moduleUrl)});
        const Component = module.settingsPanel?.[${JSON.stringify(componentName)}] || module.default?.settingsPanel?.[${JSON.stringify(componentName)}];
        if (!Component) {
          throw new Error('Settings panel component export not found: ${panel.extensionId}.${componentName}');
        }
        const getItem = (prefix, key) => {
          const raw = window.localStorage.getItem(prefix + ':' + key);
          if (raw === null) return undefined;
          try { return JSON.parse(raw); } catch { return raw; }
        };
        const setItem = async (prefix, key, value) => {
          window.localStorage.setItem(prefix + ':' + key, JSON.stringify(value));
        };
        const deleteItem = async (prefix, key) => {
          window.localStorage.removeItem(prefix + ':' + key);
        };
        const storage = {
          get: (key) => getItem(${JSON.stringify(workspaceStoragePrefix)}, key),
          set: (key, value) => setItem(${JSON.stringify(workspaceStoragePrefix)}, key, value),
          delete: (key) => deleteItem(${JSON.stringify(workspaceStoragePrefix)}, key),
          getGlobal: (key) => getItem(${JSON.stringify(storagePrefix + ':global')}, key),
          setGlobal: (key, value) => setItem(${JSON.stringify(storagePrefix + ':global')}, key, value),
          deleteGlobal: (key) => deleteItem(${JSON.stringify(storagePrefix + ':global')}, key),
          getSecret: async (key) => getItem(${JSON.stringify(storagePrefix + ':secret')}, key),
          setSecret: (key, value) => setItem(${JSON.stringify(storagePrefix + ':secret')}, key, value),
          deleteSecret: (key) => deleteItem(${JSON.stringify(storagePrefix + ':secret')}, key),
        };
        createRoot(document.getElementById('root')).render(React.createElement(Component, { storage, theme: 'dark' }));
      } catch (error) {
        const message = error?.stack || error?.message || String(error);
        document.getElementById('root').innerHTML = '<pre class="error"></pre>';
        document.querySelector('.error').textContent = message;
      }
    </script>
  </body>
</html>`;

  return (
    <div className="overflow-hidden rounded-md border border-[#303030] bg-[#111]">
      <div className="border-b border-[#303030] px-3 py-2">
        <p className="text-xs font-semibold text-[#ededed]">{raw?.title || panel.title}</p>
        <p className="text-[11px] text-[#777]">{panel.extensionName}</p>
      </div>
      <iframe
        title={`${panel.extensionName} ${raw?.title || panel.title}`}
        srcDoc={srcDoc}
        className="h-[260px] w-full border-0"
        sandbox="allow-scripts allow-same-origin allow-modals"
      />
    </div>
  );
}

const TRACKER_FILE_NAME = '.nimbalyst-tracker.json';
const TRACKER_SCHEMA_DIR = '.nimbalyst/trackers';
const TRACKER_STATUSES: NimbalystTrackerStatus[] = ['to-do', 'in-progress', 'in-review', 'done', 'blocked'];
const TRACKER_PRIORITIES: NimbalystTrackerPriority[] = ['low', 'medium', 'high', 'critical'];
const TRACKER_SCAN_MAX_MARKDOWN_FILES = 1000;
const TRACKER_SCAN_SKIP_DIRS = new Set(['.git', 'node_modules', 'dist', 'build', 'coverage', '.vite', '.next']);
const LEGACY_TRACKER_FRONTMATTER_KEYS: Record<string, string> = {
  planStatus: 'plan',
  decisionStatus: 'decision',
  bugStatus: 'bug',
  taskStatus: 'task',
  ideaStatus: 'idea',
  automationStatus: 'automation',
};
const BUILTIN_TRACKER_SCHEMAS: NimbalystTrackerSchema[] = [
  {
    type: 'plan',
    displayName: 'Plan',
    displayNamePlural: 'Plans',
    icon: 'route',
    color: '#7c3aed',
    idPrefix: 'pln',
    idFormat: 'ulid',
    sync: { mode: 'shared', scope: 'project' },
    fields: [
      { name: 'title', type: 'string', required: true },
      {
        name: 'status',
        type: 'select',
        default: 'to-do',
        options: [
          { value: 'to-do', label: 'To Do' },
          { value: 'in-progress', label: 'In Progress' },
          { value: 'in-review', label: 'In Review' },
          { value: 'done', label: 'Done' },
          { value: 'blocked', label: 'Blocked' },
        ],
      },
      {
        name: 'priority',
        type: 'select',
        default: 'medium',
        options: [
          { value: 'low', label: 'Low' },
          { value: 'medium', label: 'Medium' },
          { value: 'high', label: 'High' },
          { value: 'critical', label: 'Critical' },
        ],
      },
      { name: 'owner', type: 'user' },
      { name: 'description', type: 'text' },
      { name: 'tags', type: 'array' },
    ],
    roles: {
      title: 'title',
      workflowStatus: 'status',
      priority: 'priority',
      assignee: 'owner',
      tags: 'tags',
    },
  },
  {
    type: 'decision',
    displayName: 'Decision',
    displayNamePlural: 'Decisions',
    icon: 'rule',
    color: '#0891b2',
    idPrefix: 'dec',
    idFormat: 'ulid',
    sync: { mode: 'shared', scope: 'project' },
    fields: [
      { name: 'title', type: 'string', required: true },
      {
        name: 'status',
        type: 'select',
        default: 'proposed',
        options: [
          { value: 'proposed', label: 'Proposed' },
          { value: 'in-discussion', label: 'In Discussion' },
          { value: 'decided', label: 'Decided' },
          { value: 'rejected', label: 'Rejected' },
          { value: 'superseded', label: 'Superseded' },
        ],
      },
      { name: 'owner', type: 'user' },
      { name: 'context', type: 'text' },
      { name: 'tags', type: 'array' },
    ],
    roles: {
      title: 'title',
      workflowStatus: 'status',
      assignee: 'owner',
      tags: 'tags',
    },
  },
  {
    type: 'task',
    displayName: 'Task',
    displayNamePlural: 'Tasks',
    icon: 'task_alt',
    color: '#2563eb',
    idPrefix: 'tsk',
    idFormat: 'ulid',
    sync: { mode: 'shared', scope: 'project' },
    fields: [
      { name: 'title', type: 'string', required: true },
      {
        name: 'status',
        type: 'select',
        default: 'to-do',
        options: [
          { value: 'to-do', label: 'To Do' },
          { value: 'in-progress', label: 'In Progress' },
          { value: 'in-review', label: 'In Review' },
          { value: 'done', label: 'Done' },
        ],
      },
      {
        name: 'priority',
        type: 'select',
        options: [
          { value: 'low', label: 'Low' },
          { value: 'medium', label: 'Medium' },
          { value: 'high', label: 'High' },
          { value: 'critical', label: 'Critical' },
        ],
      },
      { name: 'owner', type: 'user' },
      { name: 'description', type: 'text' },
      { name: 'tags', type: 'array' },
    ],
    roles: {
      title: 'title',
      workflowStatus: 'status',
      priority: 'priority',
      assignee: 'owner',
      tags: 'tags',
    },
  },
  {
    type: 'bug',
    displayName: 'Bug',
    displayNamePlural: 'Bugs',
    icon: 'bug_report',
    color: '#dc2626',
    idPrefix: 'bug',
    idFormat: 'ulid',
    sync: { mode: 'shared', scope: 'project' },
    fields: [
      { name: 'title', type: 'string', required: true },
      {
        name: 'status',
        type: 'select',
        default: 'to-do',
        options: [
          { value: 'to-do', label: 'To Do' },
          { value: 'in-progress', label: 'In Progress' },
          { value: 'in-review', label: 'In Review' },
          { value: 'done', label: 'Done' },
        ],
      },
      {
        name: 'priority',
        type: 'select',
        options: [
          { value: 'low', label: 'Low' },
          { value: 'medium', label: 'Medium' },
          { value: 'high', label: 'High' },
          { value: 'critical', label: 'Critical' },
        ],
      },
      { name: 'owner', type: 'user' },
      { name: 'description', type: 'text' },
      { name: 'tags', type: 'array' },
    ],
    roles: {
      title: 'title',
      workflowStatus: 'status',
      priority: 'priority',
      assignee: 'owner',
      tags: 'tags',
    },
  },
  {
    type: 'idea',
    displayName: 'Idea',
    displayNamePlural: 'Ideas',
    icon: 'lightbulb',
    color: '#ca8a04',
    idPrefix: 'id',
    idFormat: 'ulid',
    sync: { mode: 'local', scope: 'project' },
    fields: [
      { name: 'title', type: 'string', required: true },
      {
        name: 'status',
        type: 'select',
        default: 'new',
        options: [
          { value: 'new', label: 'New' },
          { value: 'considering', label: 'Considering' },
          { value: 'accepted', label: 'Accepted' },
          { value: 'rejected', label: 'Rejected' },
        ],
      },
      { name: 'tags', type: 'array' },
    ],
    roles: {
      title: 'title',
      workflowStatus: 'status',
      tags: 'tags',
    },
  },
];

function TrackerMode({
  workspacePath,
  activeFile,
  onOpenWorkspace,
}: {
  workspacePath: string;
  activeFile: ActiveFile | null;
  onOpenWorkspace: () => void;
}) {
  const [items, setItems] = useState<NimbalystTrackerRecord[]>([]);
  const [loading, setLoading] = useState(false);
  const [saving, setSaving] = useState(false);
  const [indexing, setIndexing] = useState(false);
  const [message, setMessage] = useState<string | null>(null);
  const [schemaMessage, setSchemaMessage] = useState<string | null>(null);
  const [trackerSchemas, setTrackerSchemas] = useState<NimbalystTrackerSchema[]>(BUILTIN_TRACKER_SCHEMAS);
  const [title, setTitle] = useState('');
  const [description, setDescription] = useState('');
  const [type, setType] = useState('task');
  const [newStatus, setNewStatus] = useState<NimbalystTrackerStatus>('to-do');
  const [priority, setPriority] = useState<NimbalystTrackerPriority>('medium');
  const [fieldDrafts, setFieldDrafts] = useState<Record<string, string>>({});
  const [statusFilter, setStatusFilter] = useState<'all' | NimbalystTrackerStatus>('all');

  const trackerPath = useMemo(
    () => (workspacePath ? joinWorkspacePath(workspacePath, TRACKER_FILE_NAME) : ''),
    [workspacePath]
  );
  const trackerSchemaDir = useMemo(
    () => (workspacePath ? joinWorkspacePath(workspacePath, TRACKER_SCHEMA_DIR) : ''),
    [workspacePath]
  );
  const selectedSchema = useMemo(
    () => trackerSchemas.find((schema) => schema.type === type) || null,
    [trackerSchemas, type]
  );
  const titleFieldName = getTrackerRoleField(selectedSchema, 'title', 'title');
  const statusFieldName = getTrackerRoleField(selectedSchema, 'workflowStatus', 'status');
  const priorityFieldName = getTrackerRoleField(selectedSchema, 'priority', 'priority');
  const statusOptions = useMemo(
    () => getTrackerFieldOptions(selectedSchema, statusFieldName, TRACKER_STATUSES),
    [selectedSchema, statusFieldName]
  );
  const priorityOptions = useMemo(
    () => getTrackerFieldOptions(selectedSchema, priorityFieldName, TRACKER_PRIORITIES),
    [priorityFieldName, selectedSchema]
  );
  const dynamicFields = useMemo(
    () => getTrackerDynamicFields(selectedSchema, [titleFieldName, statusFieldName, priorityFieldName, 'description']),
    [priorityFieldName, selectedSchema, statusFieldName, titleFieldName]
  );

  const loadTrackerSchemas = useCallback(async () => {
    const electronAPI = (window as any).electronAPI;
    if (!workspacePath || !electronAPI?.readDirectory || !electronAPI?.readFile || !trackerSchemaDir) {
      setTrackerSchemas(BUILTIN_TRACKER_SCHEMAS);
      setSchemaMessage(null);
      return;
    }

    try {
      const result = await electronAPI.readDirectory(trackerSchemaDir);
      if (!result?.success) {
        if (!isMissingFileError(result?.error)) {
          setSchemaMessage(result?.error || 'Failed to read tracker schema directory.');
        } else {
          setSchemaMessage(null);
        }
        setTrackerSchemas(BUILTIN_TRACKER_SCHEMAS);
        return;
      }

      const customSchemas: NimbalystTrackerSchema[] = [];
      for (const entry of (result.items || []) as FileEntry[]) {
        if (entry.isDirectory || !/\.(ya?ml)$/i.test(entry.name)) continue;
        const fileResult = await electronAPI.readFile(entry.path);
        if (!fileResult?.success) {
          setSchemaMessage(fileResult?.error || `Failed to read tracker schema ${entry.name}.`);
          continue;
        }
        customSchemas.push(parseTrackerSchemaYAML(fileResult.content, entry.path));
      }
      const nextSchemas = mergeTrackerSchemas(BUILTIN_TRACKER_SCHEMAS, customSchemas);
      setTrackerSchemas(nextSchemas);
      setSchemaMessage(customSchemas.length ? `Loaded ${customSchemas.length} workspace tracker schema(s).` : null);
      if (!nextSchemas.some((schema) => schema.type === type)) {
        setType(nextSchemas[0]?.type || 'task');
      }
    } catch (error) {
      setTrackerSchemas(BUILTIN_TRACKER_SCHEMAS);
      setSchemaMessage(error instanceof Error ? error.message : String(error));
    }
  }, [trackerSchemaDir, type, workspacePath]);

  const loadTracker = useCallback(async () => {
    if (!workspacePath) {
      setItems([]);
      return;
    }

    setLoading(true);
    setMessage(null);
    try {
      const electronAPI = (window as any).electronAPI;
      let raw: string | null = null;
      if (electronAPI?.readFile && trackerPath) {
        const result = await electronAPI.readFile(trackerPath);
        if (result?.success) {
          raw = result.content;
        } else if (!isMissingFileError(result?.error)) {
          throw new Error(result?.error || 'Failed to read tracker file.');
        }
      } else {
        raw = localStorage.getItem(getTrackerStorageKey(workspacePath));
      }

      if (!raw) {
        setItems([]);
        return;
      }

      const parsed = JSON.parse(raw) as Partial<NimbalystTrackerStore>;
      setItems(Array.isArray(parsed.items) ? parsed.items.filter(isTrackerRecord) : []);
    } catch (error) {
      setMessage(error instanceof Error ? error.message : String(error));
      setItems([]);
    } finally {
      setLoading(false);
    }
  }, [trackerPath, workspacePath]);

  useEffect(() => {
    loadTracker();
  }, [loadTracker]);

  useEffect(() => {
    loadTrackerSchemas();
  }, [loadTrackerSchemas]);

  useEffect(() => {
    if (!selectedSchema) return;
    const nextStatus = String(getTrackerFieldDefault(selectedSchema, statusFieldName, statusOptions[0]?.value || 'to-do'));
    const nextPriority = String(getTrackerFieldDefault(selectedSchema, priorityFieldName, priorityOptions[0]?.value || 'medium'));
    setNewStatus(nextStatus);
    setPriority(nextPriority);
    setFieldDrafts(buildTrackerFieldDrafts(selectedSchema, dynamicFields));
  }, [dynamicFields, priorityFieldName, priorityOptions, selectedSchema, statusFieldName, statusOptions]);

  const persistItems = async (nextItems: NimbalystTrackerRecord[]) => {
    const payload: NimbalystTrackerStore = {
      version: 1,
      updatedAt: new Date().toISOString(),
      items: nextItems,
    };
    const raw = JSON.stringify(payload, null, 2);
    const electronAPI = (window as any).electronAPI;

    setSaving(true);
    setMessage(null);
    try {
      if (electronAPI?.writeFile && trackerPath) {
        const result = await electronAPI.writeFile(trackerPath, raw);
        if (!result?.success) {
          throw new Error(result?.error || 'Failed to write tracker file.');
        }
      } else {
        localStorage.setItem(getTrackerStorageKey(workspacePath), raw);
      }
      setItems(nextItems);
      setMessage(`Saved ${nextItems.length} tracker item(s).`);
    } catch (error) {
      setMessage(error instanceof Error ? error.message : String(error));
    } finally {
      setSaving(false);
    }
  };

  const addItem = async () => {
    const trimmedTitle = title.trim();
    if (!trimmedTitle) {
      setMessage('Title is required.');
      return;
    }
    if (!workspacePath) {
      setMessage('Open a workspace before creating tracker items.');
      return;
    }

    const now = new Date().toISOString();
    const primaryType = selectedSchema?.type || type.trim() || 'task';
    const schemaFields = coerceTrackerFieldDrafts(dynamicFields, fieldDrafts);
    const fieldValues: NimbalystTrackerRecord['fields'] = {
      ...schemaFields,
      title: trimmedTitle,
      description: description.trim() || undefined,
      status: newStatus,
      priority,
      [titleFieldName]: trimmedTitle,
      [statusFieldName]: newStatus,
    };
    if (priorityFieldName) {
      fieldValues[priorityFieldName] = priority;
    }
    if (description.trim()) {
      fieldValues.description = description.trim();
    }
    const item: NimbalystTrackerRecord = {
      id: createTrackerItemId(),
      primaryType,
      typeTags: [primaryType],
      source: 'native',
      sourceRef: activeFile?.path,
      archived: false,
      syncStatus: selectedSchema?.sync?.mode === 'shared' ? 'pending' : 'local',
      system: {
        workspace: workspacePath,
        documentPath: activeFile?.path,
        createdAt: now,
        updatedAt: now,
        activity: [
          {
            id: createTrackerItemId(),
            action: 'created',
            timestamp: Date.now(),
          },
        ],
      },
      fields: fieldValues,
    };

    await persistItems([item, ...items]);
    setTitle('');
    setDescription('');
    setFieldDrafts(buildTrackerFieldDrafts(selectedSchema, dynamicFields));
  };

  const indexFrontmatterTrackers = async () => {
    if (!workspacePath) {
      setMessage('Open a workspace before indexing tracker frontmatter.');
      return;
    }

    const electronAPI = (window as any).electronAPI;
    if (!electronAPI?.readDirectory || !electronAPI?.readFile) {
      setMessage('Electron file reader is not available for tracker indexing.');
      return;
    }

    setIndexing(true);
    setMessage(null);
    try {
      const markdownFiles = await collectMarkdownFiles(workspacePath, electronAPI, TRACKER_SCAN_MAX_MARKDOWN_FILES);
      const indexedRecords: NimbalystTrackerRecord[] = [];
      for (const file of markdownFiles) {
        const fileResult = await electronAPI.readFile(file.path);
        if (!fileResult?.success) continue;
        const record = buildFrontmatterTrackerRecord({
          workspacePath,
          filePath: file.path,
          content: fileResult.content,
          schemas: trackerSchemas,
        });
        if (record) indexedRecords.push(record);
      }
      const manualRecords = items.filter((item) => item.source !== 'frontmatter');
      await persistItems([...indexedRecords, ...manualRecords]);
      setMessage(`Indexed ${indexedRecords.length} frontmatter tracker item(s) from ${markdownFiles.length} markdown file(s).`);
    } catch (error) {
      setMessage(error instanceof Error ? error.message : String(error));
    } finally {
      setIndexing(false);
    }
  };

  const loadTrackerRef = useRef(loadTracker);
  const loadTrackerSchemasRef = useRef(loadTrackerSchemas);
  const indexFrontmatterTrackersRef = useRef(indexFrontmatterTrackers);

  useEffect(() => {
    loadTrackerRef.current = loadTracker;
    loadTrackerSchemasRef.current = loadTrackerSchemas;
    indexFrontmatterTrackersRef.current = indexFrontmatterTrackers;
  }, [indexFrontmatterTrackers, loadTracker, loadTrackerSchemas]);

  useEffect(() => {
    const electronAPI = (window as any).electronAPI;
    if (!workspacePath || !electronAPI?.watchNimbalystWorkspace || !electronAPI?.onNimbalystWorkspaceChanged) {
      return undefined;
    }

    let disposed = false;
    let timer: number | null = null;
    const pending = {
      tracker: false,
      schemas: false,
      frontmatter: false,
    };

    const flush = () => {
      timer = null;
      if (disposed) return;
      if (pending.schemas) void loadTrackerSchemasRef.current();
      if (pending.tracker) void loadTrackerRef.current();
      if (pending.frontmatter) void indexFrontmatterTrackersRef.current();
      pending.tracker = false;
      pending.schemas = false;
      pending.frontmatter = false;
    };

    const scheduleFlush = () => {
      if (timer) window.clearTimeout(timer);
      timer = window.setTimeout(flush, 350);
    };

    const unsubscribe = electronAPI.onNimbalystWorkspaceChanged((event: { workspacePath?: string; path?: string }) => {
      if (!event || event.workspacePath !== workspacePath) return;
      const changedPath = String(event.path || '').replace(/\\/g, '/');
      if (changedPath === TRACKER_FILE_NAME) pending.tracker = true;
      if (changedPath.startsWith(`${TRACKER_SCHEMA_DIR}/`)) pending.schemas = true;
      if (/\.(md|mdx)$/i.test(changedPath)) pending.frontmatter = true;
      if (pending.tracker || pending.schemas || pending.frontmatter) scheduleFlush();
    });

    void electronAPI.watchNimbalystWorkspace(workspacePath).then((result: { success?: boolean; error?: string }) => {
      if (!disposed && result?.success === false) {
        setMessage(`Workspace watcher unavailable: ${result.error || 'unknown error'}`);
      }
    });

    return () => {
      disposed = true;
      if (timer) window.clearTimeout(timer);
      if (typeof unsubscribe === 'function') unsubscribe();
      void electronAPI.unwatchNimbalystWorkspace?.();
    };
  }, [workspacePath]);

  const updateItem = async (
    itemId: string,
    patch: (item: NimbalystTrackerRecord) => NimbalystTrackerRecord
  ) => {
    const nextItems = items.map((item) => (item.id === itemId ? patch(item) : item));
    await persistItems(nextItems);
  };

  const filteredItems = useMemo(
    () =>
      items
        .filter((item) => !item.archived)
        .filter((item) => statusFilter === 'all' || item.fields.status === statusFilter),
    [items, statusFilter]
  );

  const statusCounts = useMemo(() => {
    const counts = new Map<NimbalystTrackerStatus, number>();
    for (const status of statusOptions.map((option) => option.value)) counts.set(status, 0);
    for (const item of items) {
      if (item.archived) continue;
      counts.set(item.fields.status, (counts.get(item.fields.status) || 0) + 1);
    }
    return counts;
  }, [items, statusOptions]);

  const visibleStatusOptions = useMemo(
    () => mergeTrackerStatusOptions(statusOptions, items),
    [items, statusOptions]
  );

  if (!workspacePath) {
    return (
      <div className="flex h-full items-center justify-center p-8">
        <div className="max-w-lg text-center">
          <div className="mx-auto flex size-12 items-center justify-center rounded-xl bg-[#252525] text-[#a78bfa]">
            <Workflow className="size-6" />
          </div>
          <h2 className="mt-4 text-lg font-semibold">Nimbalyst Tracker</h2>
          <p className="mt-2 text-sm leading-6 text-[#999]">
            Open a workspace to create Nimbalyst-style tracker records backed by a local workspace file.
          </p>
          <button
            type="button"
            onClick={onOpenWorkspace}
            className="mt-4 h-9 rounded-md bg-[#4f46e5] px-4 text-xs font-medium text-white"
          >
            Open Workspace
          </button>
        </div>
      </div>
    );
  }

  return (
    <div className="grid min-h-full grid-cols-[minmax(280px,360px)_1fr]">
      <aside className="border-r border-[#2f2f2f] p-4">
        <h2 className="text-lg font-semibold">Nimbalyst Tracker</h2>
        <p className="mt-2 text-sm leading-6 text-[#a7a7a7]">
          Local tracker records are stored in <span className="font-mono text-[#d5d5d5]">{TRACKER_FILE_NAME}</span> using the same record shape as the reference tracker core.
        </p>
        <div className="mt-3 rounded-md border border-[#303030] bg-[#171717] px-3 py-2 text-xs text-[#aaa]">
          <div className="flex items-center justify-between gap-2">
            <span>{trackerSchemas.length} tracker schema(s)</span>
            <button
              type="button"
              onClick={loadTrackerSchemas}
              className="rounded border border-[#3a3a3a] px-2 py-1 text-[11px] text-[#cfcfcf] hover:border-[#4f46e5]"
            >
              Reload Schemas
            </button>
          </div>
          <p className="mt-1 truncate font-mono text-[10px] text-[#666]">{trackerSchemaDir}</p>
          {schemaMessage && <p className="mt-1 text-[11px] text-[#888]">{schemaMessage}</p>}
        </div>

        <div className="mt-4 rounded-md border border-[#303030] bg-[#202020] p-3">
          <p className="text-xs font-semibold text-[#ededed]">New Tracker Item</p>
          <label className="mt-3 block text-xs text-[#888]">
            Title
            <input
              value={title}
              onChange={(event) => setTitle(event.target.value)}
              className="mt-1 h-9 w-full rounded-md border border-[#333] bg-[#111] px-2 text-xs text-[#ddd] outline-none focus:border-[#4f46e5]"
            />
          </label>
          <label className="mt-3 block text-xs text-[#888]">
            Description
            <textarea
              value={description}
              onChange={(event) => setDescription(event.target.value)}
              rows={4}
              className="mt-1 w-full resize-y rounded-md border border-[#333] bg-[#111] px-2 py-1.5 text-xs text-[#ddd] outline-none focus:border-[#4f46e5]"
            />
          </label>
          <div className="mt-3 grid grid-cols-2 gap-2">
            <label className="text-xs text-[#888]">
              Type
              <select
                value={type}
                onChange={(event) => setType(event.target.value)}
                className="mt-1 h-9 w-full rounded-md border border-[#333] bg-[#111] px-2 text-xs text-[#ddd] outline-none focus:border-[#4f46e5]"
              >
                {trackerSchemas.map((schema) => (
                  <option key={schema.type} value={schema.type}>{schema.displayName}</option>
                ))}
              </select>
            </label>
            <label className="text-xs text-[#888]">
              Status
              <select
                value={newStatus}
                onChange={(event) => setNewStatus(event.target.value)}
                className="mt-1 h-9 w-full rounded-md border border-[#333] bg-[#111] px-2 text-xs text-[#ddd] outline-none focus:border-[#4f46e5]"
              >
                {statusOptions.map((option) => (
                  <option key={option.value} value={option.value}>{option.label}</option>
                ))}
              </select>
            </label>
          </div>
          <div className="mt-3 grid grid-cols-2 gap-2">
            <label className="text-xs text-[#888]">
              Priority
              <select
                value={priority}
                onChange={(event) => setPriority(event.target.value as NimbalystTrackerPriority)}
                className="mt-1 h-9 w-full rounded-md border border-[#333] bg-[#111] px-2 text-xs text-[#ddd] outline-none focus:border-[#4f46e5]"
              >
                {priorityOptions.map((option) => (
                  <option key={option.value} value={option.value}>{option.label}</option>
                ))}
              </select>
            </label>
          </div>
          {dynamicFields.length > 0 && (
            <div className="mt-3 grid gap-2">
              {dynamicFields.map((field) => (
                <TrackerDynamicFieldInput
                  key={field.name}
                  field={field}
                  value={fieldDrafts[field.name] || ''}
                  onChange={(value) => setFieldDrafts((current) => ({ ...current, [field.name]: value }))}
                />
              ))}
            </div>
          )}
          {activeFile && (
            <p className="mt-3 truncate rounded bg-[#171717] px-2 py-1.5 font-mono text-[11px] text-[#777]">
              Source: {activeFile.path}
            </p>
          )}
          <button
            type="button"
            onClick={addItem}
            disabled={saving}
            className="mt-3 h-9 w-full rounded-md bg-[#4f46e5] text-xs font-medium text-white disabled:opacity-50"
          >
            {saving ? 'Saving...' : 'Create Tracker Item'}
          </button>
        </div>

        <div className="mt-4 rounded-md border border-[#303030] bg-[#202020] p-3">
          <p className="text-xs font-semibold text-[#ededed]">Status Summary</p>
          <div className="mt-2 grid grid-cols-2 gap-2">
            {visibleStatusOptions.map((option) => (
              <button
                key={option.value}
                type="button"
                onClick={() => setStatusFilter(statusFilter === option.value ? 'all' : option.value)}
                className={`rounded border px-2 py-2 text-left text-xs ${
                  statusFilter === option.value
                    ? 'border-[#4f46e5] bg-[#312e81] text-white'
                    : 'border-[#303030] bg-[#171717] text-[#cfcfcf] hover:border-[#4f46e5]'
                }`}
              >
                <span className="block font-semibold">{statusCounts.get(option.value) || 0}</span>
                <span className="mt-1 block text-[11px] text-[#aaa]">{option.label}</span>
              </button>
            ))}
          </div>
          <button
            type="button"
            onClick={loadTracker}
            disabled={loading}
            className="mt-3 flex h-8 w-full items-center justify-center gap-2 rounded border border-[#3a3a3a] text-xs text-[#cfcfcf] hover:border-[#4f46e5] disabled:opacity-50"
          >
            <RefreshCw className={`size-3 ${loading ? 'animate-spin' : ''}`} />
            Reload Tracker File
          </button>
          <button
            type="button"
            onClick={indexFrontmatterTrackers}
            disabled={indexing || saving}
            className="mt-2 flex h-8 w-full items-center justify-center gap-2 rounded border border-[#3a3a3a] text-xs text-[#cfcfcf] hover:border-[#4f46e5] disabled:opacity-50"
          >
            <FileText className="size-3" />
            {indexing ? 'Indexing...' : 'Index Frontmatter'}
          </button>
        </div>

        {message && (
          <p className="mt-4 rounded border border-[#303030] bg-[#1b1b1b] px-3 py-2 text-xs text-[#aaa]">
            {message}
          </p>
        )}
      </aside>

      <section className="p-4">
        <div className="flex items-start justify-between gap-3">
          <div>
            <h3 className="text-sm font-semibold">Items</h3>
            <p className="mt-1 font-mono text-xs text-[#777]">{trackerPath}</p>
          </div>
          <span className="rounded bg-[#303030] px-2 py-1 text-[11px] text-[#aaa]">
            {filteredItems.length} visible / {items.filter((item) => !item.archived).length} active
          </span>
        </div>

        {filteredItems.length === 0 ? (
          <div className="mt-4 rounded-md border border-[#303030] bg-[#1b1b1b] p-6 text-sm text-[#777]">
            No tracker items match the current filter.
          </div>
        ) : (
          <div className="mt-4 space-y-3">
            {filteredItems.map((item) => (
              <TrackerItemCard
                key={item.id}
                item={item}
                statusOptions={mergeTrackerStatusOptions(
                  getTrackerFieldOptions(
                    trackerSchemas.find((schema) => schema.type === item.primaryType) || null,
                    getTrackerRoleField(trackerSchemas.find((schema) => schema.type === item.primaryType) || null, 'workflowStatus', 'status'),
                    TRACKER_STATUSES
                  ),
                  [item]
                )}
                onStatusChange={(nextStatus) =>
                  updateItem(item.id, (current) => updateTrackerStatus(current, nextStatus))
                }
                onArchive={() =>
                  updateItem(item.id, (current) => ({
                    ...current,
                    archived: true,
                    system: {
                      ...current.system,
                      updatedAt: new Date().toISOString(),
                      activity: [
                        ...(current.system.activity || []),
                        {
                          id: createTrackerItemId(),
                          action: 'archived',
                          timestamp: Date.now(),
                        },
                      ],
                    },
                  }))
                }
              />
            ))}
          </div>
        )}
      </section>
    </div>
  );
}

function TrackerItemCard({
  item,
  statusOptions,
  onStatusChange,
  onArchive,
}: {
  item: NimbalystTrackerRecord;
  statusOptions: NimbalystTrackerFieldOption[];
  onStatusChange: (status: NimbalystTrackerStatus) => void;
  onArchive: () => void;
}) {
  return (
    <article className="rounded-md border border-[#303030] bg-[#202020] p-4">
      <div className="flex flex-wrap items-start justify-between gap-3">
        <div className="min-w-0">
          <div className="flex flex-wrap items-center gap-2">
            <span className="rounded bg-[#303030] px-1.5 py-0.5 text-[10px] text-[#aaa]">{item.primaryType}</span>
            <span className="rounded bg-[#1f2937] px-1.5 py-0.5 text-[10px] text-[#93c5fd]">{item.syncStatus}</span>
            {item.fields.priority && (
              <span className="rounded bg-[#3a2a16] px-1.5 py-0.5 text-[10px] text-[#facc15]">{item.fields.priority}</span>
            )}
          </div>
          <h4 className="mt-2 truncate text-sm font-semibold text-[#ededed]">{item.fields.title}</h4>
          {item.fields.description && (
            <p className="mt-2 whitespace-pre-wrap text-xs leading-5 text-[#aaa]">{item.fields.description}</p>
          )}
        </div>
        <div className="flex shrink-0 items-center gap-2">
          <select
            value={item.fields.status}
            onChange={(event) => onStatusChange(event.target.value as NimbalystTrackerStatus)}
            className="h-8 rounded border border-[#333] bg-[#111] px-2 text-xs text-[#ddd] outline-none focus:border-[#4f46e5]"
          >
            {statusOptions.map((status) => (
              <option key={status.value} value={status.value}>{status.label}</option>
            ))}
          </select>
          <button
            type="button"
            onClick={onArchive}
            className="h-8 rounded border border-[#3a3a3a] px-3 text-xs text-[#cfcfcf] hover:border-[#4f46e5]"
          >
            Archive
          </button>
        </div>
      </div>
      <div className="mt-3 flex flex-wrap gap-3 border-t border-[#303030] pt-3 text-[11px] text-[#777]">
        <span className="font-mono">{item.id}</span>
        <span>Created {new Date(item.system.createdAt).toLocaleString()}</span>
        {item.system.documentPath && <span className="min-w-0 truncate font-mono">Source {item.system.documentPath}</span>}
      </div>
    </article>
  );
}

function TrackerDynamicFieldInput({
  field,
  value,
  onChange,
}: {
  field: NimbalystTrackerFieldDefinition;
  value: string;
  onChange: (value: string) => void;
}) {
  const label = formatTrackerFieldLabel(field.name);
  const requiredMark = field.required ? ' *' : '';

  if (field.type === 'select' && field.options?.length) {
    return (
      <label className="text-xs text-[#888]">
        {label}{requiredMark}
        <select
          value={value}
          onChange={(event) => onChange(event.target.value)}
          className="mt-1 h-9 w-full rounded-md border border-[#333] bg-[#111] px-2 text-xs text-[#ddd] outline-none focus:border-[#4f46e5]"
        >
          <option value="">-</option>
          {field.options.map((option) => (
            <option key={option.value} value={option.value}>{option.label}</option>
          ))}
        </select>
      </label>
    );
  }

  if (field.type === 'boolean') {
    return (
      <label className="flex items-center gap-2 text-xs text-[#888]">
        <input
          type="checkbox"
          checked={value === 'true'}
          onChange={(event) => onChange(event.target.checked ? 'true' : 'false')}
          className="size-4 accent-[#4f46e5]"
        />
        {label}{requiredMark}
      </label>
    );
  }

  if (field.type === 'text' || field.type === 'array' || field.type === 'object' || field.type === 'multiselect') {
    return (
      <label className="text-xs text-[#888]">
        {label}{requiredMark}
        <textarea
          value={value}
          onChange={(event) => onChange(event.target.value)}
          rows={field.type === 'text' ? 3 : 2}
          placeholder={field.type === 'array' || field.type === 'multiselect' ? 'comma,separated,values' : undefined}
          className="mt-1 w-full resize-y rounded-md border border-[#333] bg-[#111] px-2 py-1.5 text-xs text-[#ddd] outline-none focus:border-[#4f46e5]"
        />
      </label>
    );
  }

  return (
    <label className="text-xs text-[#888]">
      {label}{requiredMark}
      <input
        type={field.type === 'number' ? 'number' : field.type === 'date' ? 'date' : field.type === 'datetime' ? 'datetime-local' : 'text'}
        value={value}
        min={field.min}
        max={field.max}
        minLength={field.minLength}
        maxLength={field.maxLength}
        onChange={(event) => onChange(event.target.value)}
        className="mt-1 h-9 w-full rounded-md border border-[#333] bg-[#111] px-2 text-xs text-[#ddd] outline-none focus:border-[#4f46e5]"
      />
    </label>
  );
}

function updateTrackerStatus(
  item: NimbalystTrackerRecord,
  nextStatus: NimbalystTrackerStatus
): NimbalystTrackerRecord {
  const previousStatus = item.fields.status;
  if (previousStatus === nextStatus) return item;
  return {
    ...item,
    fields: {
      ...item.fields,
      status: nextStatus,
    },
    system: {
      ...item.system,
      updatedAt: new Date().toISOString(),
      activity: [
        ...(item.system.activity || []),
        {
          id: createTrackerItemId(),
          action: 'status_changed',
          field: 'status',
          oldValue: previousStatus,
          newValue: nextStatus,
          timestamp: Date.now(),
        },
      ],
    },
  };
}

function parseTrackerSchemaYAML(content: string, sourcePath: string): NimbalystTrackerSchema {
  const data = yaml.load(content) as Partial<NimbalystTrackerSchema> | null;
  if (!data || typeof data !== 'object') {
    throw new Error(`Empty tracker schema: ${sourcePath}`);
  }
  if (!data.type || !data.displayName || !data.displayNamePlural || !Array.isArray(data.fields)) {
    throw new Error(`Invalid tracker schema: ${sourcePath}`);
  }

  return {
    type: data.type,
    displayName: data.displayName,
    displayNamePlural: data.displayNamePlural,
    icon: data.icon || 'category',
    color: data.color || '#8b5cf6',
    idPrefix: data.idPrefix || data.type.slice(0, 4),
    idFormat: data.idFormat || 'ulid',
    fields: data.fields.map(normalizeTrackerFieldDefinition).filter(Boolean) as NimbalystTrackerFieldDefinition[],
    roles: data.roles || {},
    sync: data.sync || { mode: 'local', scope: 'project' },
    sourcePath,
  };
}

function normalizeTrackerFieldDefinition(field: unknown): NimbalystTrackerFieldDefinition | null {
  if (!field || typeof field !== 'object') return null;
  const raw = field as Partial<NimbalystTrackerFieldDefinition>;
  if (!raw.name || !raw.type) return null;
  return {
    name: raw.name,
    type: raw.type,
    required: Boolean(raw.required),
    default: raw.default,
    displayInline: raw.displayInline,
    readOnly: raw.readOnly,
    min: raw.min,
    max: raw.max,
    minLength: raw.minLength,
    maxLength: raw.maxLength,
    options: Array.isArray(raw.options) ? raw.options.map(normalizeTrackerFieldOption).filter(Boolean) as NimbalystTrackerFieldOption[] : undefined,
  };
}

function normalizeTrackerFieldOption(option: unknown): NimbalystTrackerFieldOption | null {
  if (typeof option === 'string') {
    return { value: option.toLowerCase().replace(/\s+/g, '-'), label: option };
  }
  if (!option || typeof option !== 'object') return null;
  const raw = option as Partial<NimbalystTrackerFieldOption>;
  if (!raw.value && !raw.label) return null;
  return {
    value: String(raw.value || raw.label),
    label: String(raw.label || raw.value),
    icon: raw.icon,
    color: raw.color,
  };
}

function mergeTrackerSchemas(
  builtinSchemas: NimbalystTrackerSchema[],
  customSchemas: NimbalystTrackerSchema[]
) {
  const byType = new Map<string, NimbalystTrackerSchema>();
  for (const schema of builtinSchemas) byType.set(schema.type, schema);
  for (const schema of customSchemas) byType.set(schema.type, schema);
  return Array.from(byType.values()).sort((a, b) => a.displayName.localeCompare(b.displayName));
}

function getTrackerRoleField(
  schema: NimbalystTrackerSchema | null,
  role: keyof NonNullable<NimbalystTrackerSchema['roles']>,
  fallback: string
) {
  return schema?.roles?.[role] || fallback;
}

function getTrackerField(schema: NimbalystTrackerSchema | null, fieldName: string) {
  return schema?.fields.find((field) => field.name === fieldName);
}

function getTrackerFieldOptions(
  schema: NimbalystTrackerSchema | null,
  fieldName: string,
  fallback: string[]
): NimbalystTrackerFieldOption[] {
  const field = getTrackerField(schema, fieldName);
  if (field?.options?.length) return field.options;
  return fallback.map((value) => ({ value, label: formatTrackerFieldLabel(value) }));
}

function getTrackerFieldDefault(
  schema: NimbalystTrackerSchema | null,
  fieldName: string,
  fallback: string
) {
  const field = getTrackerField(schema, fieldName);
  return field?.default ?? fallback;
}

function getTrackerDynamicFields(schema: NimbalystTrackerSchema | null, excludedNames: string[]) {
  if (!schema) return [];
  const excluded = new Set(excludedNames.filter(Boolean));
  return schema.fields.filter((field) => !field.readOnly && !excluded.has(field.name));
}

function buildTrackerFieldDrafts(
  schema: NimbalystTrackerSchema | null,
  fields: NimbalystTrackerFieldDefinition[]
) {
  const drafts: Record<string, string> = {};
  if (!schema) return drafts;
  for (const field of fields) {
    if (field.default === undefined || field.default === null) {
      drafts[field.name] = field.type === 'boolean' ? 'false' : '';
    } else if (Array.isArray(field.default)) {
      drafts[field.name] = field.default.join(', ');
    } else if (typeof field.default === 'object') {
      drafts[field.name] = JSON.stringify(field.default);
    } else {
      drafts[field.name] = String(field.default);
    }
  }
  return drafts;
}

function coerceTrackerFieldDrafts(
  fields: NimbalystTrackerFieldDefinition[],
  drafts: Record<string, string>
) {
  const values: Record<string, unknown> = {};
  for (const field of fields) {
    const rawValue = drafts[field.name];
    if (rawValue === undefined || rawValue === '') continue;
    values[field.name] = coerceTrackerFieldValue(field, rawValue);
  }
  return values;
}

function coerceTrackerFieldValue(field: NimbalystTrackerFieldDefinition, rawValue: string) {
  if (field.type === 'number') return Number(rawValue);
  if (field.type === 'boolean') return rawValue === 'true';
  if (field.type === 'array' || field.type === 'multiselect') {
    return rawValue.split(',').map((item) => item.trim()).filter(Boolean);
  }
  if (field.type === 'object') {
    try {
      return JSON.parse(rawValue);
    } catch {
      return rawValue;
    }
  }
  return rawValue;
}

function mergeTrackerStatusOptions(
  baseOptions: NimbalystTrackerFieldOption[],
  items: NimbalystTrackerRecord[]
) {
  const byValue = new Map<string, NimbalystTrackerFieldOption>();
  for (const option of baseOptions) byValue.set(option.value, option);
  for (const item of items) {
    if (!byValue.has(item.fields.status)) {
      byValue.set(item.fields.status, {
        value: item.fields.status,
        label: formatTrackerFieldLabel(item.fields.status),
      });
    }
  }
  return Array.from(byValue.values());
}

async function collectMarkdownFiles(
  rootPath: string,
  electronAPI: { readDirectory: (path: string) => Promise<{ success?: boolean; items?: FileEntry[]; error?: string }> },
  maxFiles: number
) {
  const files: FileEntry[] = [];
  const queue = [rootPath];
  const visited = new Set<string>();

  while (queue.length > 0 && files.length < maxFiles) {
    const dirPath = queue.shift();
    if (!dirPath || visited.has(dirPath)) continue;
    visited.add(dirPath);

    const result = await electronAPI.readDirectory(dirPath);
    if (!result?.success) continue;
    for (const entry of result.items || []) {
      if (entry.isDirectory) {
        if (!TRACKER_SCAN_SKIP_DIRS.has(entry.name)) {
          queue.push(entry.path);
        }
      } else if (/\.(md|mdx)$/i.test(entry.name)) {
        files.push(entry);
        if (files.length >= maxFiles) break;
      }
    }
  }

  return files;
}

function buildFrontmatterTrackerRecord({
  workspacePath,
  filePath,
  content,
  schemas,
}: {
  workspacePath: string;
  filePath: string;
  content: string;
  schemas: NimbalystTrackerSchema[];
}) {
  const frontmatter = extractMarkdownFrontmatter(content);
  const resolved = resolveTrackerFrontmatter(frontmatter);
  if (!resolved) return null;

  const relativePath = toWorkspaceRelativePath(workspacePath, filePath);
  const schema = schemas.find((item) => item.type === resolved.type) || null;
  const titleField = getTrackerRoleField(schema, 'title', 'title');
  const statusField = getTrackerRoleField(schema, 'workflowStatus', 'status');
  const priorityField = getTrackerRoleField(schema, 'priority', 'priority');
  const titleValue = resolved.data[titleField] ?? resolved.data.title ?? basename(filePath).replace(/\.(md|mdx)$/i, '');
  const statusValue = resolved.data[statusField] ?? resolved.data.status ?? getTrackerFieldDefault(schema, statusField, 'to-do');
  const priorityValue = resolved.data[priorityField] ?? resolved.data.priority ?? getTrackerFieldDefault(schema, priorityField, 'medium');
  const fields: NimbalystTrackerRecord['fields'] = {
    ...resolved.data,
    title: String(titleValue || 'Untitled'),
    status: String(statusValue || 'to-do').toLowerCase(),
    priority: String(priorityValue || 'medium'),
    [titleField]: titleValue,
    [statusField]: statusValue,
  };
  if (priorityField) fields[priorityField] = priorityValue;

  const createdAt = stringifyFrontmatterDate(resolved.data.created) || new Date(0).toISOString();
  const updatedAt = stringifyFrontmatterDate(resolved.data.updated) || new Date().toISOString();

  return {
    id: String(resolved.data.id || resolved.data.planId || resolved.data.decisionId || `fm:${resolved.type}:${relativePath}`),
    primaryType: resolved.type,
    typeTags: [resolved.type],
    source: 'frontmatter',
    sourceRef: relativePath,
    archived: false,
    syncStatus: schema?.sync?.mode === 'shared' ? 'pending' : 'local',
    system: {
      workspace: workspacePath,
      documentPath: relativePath,
      createdAt,
      updatedAt,
    },
    fields,
  } satisfies NimbalystTrackerRecord;
}

function extractMarkdownFrontmatter(content: string) {
  const match = content.match(/^---\r?\n([\s\S]*?)\r?\n---/);
  if (!match) return null;
  try {
    const parsed = yaml.load(match[1]) as Record<string, unknown>;
    return parsed && typeof parsed === 'object' ? parsed : null;
  } catch {
    return null;
  }
}

function resolveTrackerFrontmatter(frontmatter: Record<string, unknown> | null) {
  if (!frontmatter) return null;

  const trackerStatus = frontmatter.trackerStatus;
  if (trackerStatus && typeof trackerStatus === 'object' && !Array.isArray(trackerStatus)) {
    const trackerData = trackerStatus as Record<string, unknown>;
    const trackerType = typeof trackerData.type === 'string' ? trackerData.type : undefined;
    if (trackerType) {
      const { trackerStatus: _trackerStatus, ...topLevel } = frontmatter;
      return {
        type: trackerType,
        data: { ...trackerData, ...topLevel },
      };
    }
  }

  for (const [frontmatterKey, trackerType] of Object.entries(LEGACY_TRACKER_FRONTMATTER_KEYS)) {
    const legacyBlock = frontmatter[frontmatterKey];
    if (legacyBlock && typeof legacyBlock === 'object' && !Array.isArray(legacyBlock)) {
      const { [frontmatterKey]: _legacyBlock, trackerStatus: _trackerStatus, ...topLevel } = frontmatter;
      return {
        type: trackerType,
        data: { ...(legacyBlock as Record<string, unknown>), ...topLevel },
      };
    }
  }

  return null;
}

function toWorkspaceRelativePath(workspacePath: string, filePath: string) {
  const normalizedWorkspace = workspacePath.replace(/\\/g, '/').replace(/\/+$/, '');
  const normalizedFile = filePath.replace(/\\/g, '/');
  if (normalizedFile.startsWith(`${normalizedWorkspace}/`)) {
    return normalizedFile.slice(normalizedWorkspace.length + 1);
  }
  return normalizedFile;
}

function stringifyFrontmatterDate(value: unknown) {
  if (!value) return '';
  if (value instanceof Date && !Number.isNaN(value.getTime())) return value.toISOString();
  const parsed = new Date(String(value));
  return Number.isNaN(parsed.getTime()) ? String(value) : parsed.toISOString();
}

function formatTrackerFieldLabel(value: string) {
  return value
    .replace(/[_-]+/g, ' ')
    .replace(/\b\w/g, (char) => char.toUpperCase());
}

function joinWorkspacePath(workspacePath: string, fileName: string) {
  const separator = workspacePath.includes('\\') ? '\\' : '/';
  return `${workspacePath.replace(/[\\/]+$/, '')}${separator}${fileName}`;
}

function getTrackerStorageKey(workspacePath: string) {
  return `batrack:nimbalyst:tracker:${workspacePath || 'default'}`;
}

function createTrackerItemId() {
  if (typeof crypto !== 'undefined' && 'randomUUID' in crypto) {
    return crypto.randomUUID();
  }
  return `tracker-${Date.now()}-${Math.random().toString(36).slice(2, 8)}`;
}

function isMissingFileError(error?: string) {
  if (!error) return false;
  const lower = error.toLowerCase();
  return lower.includes('enoent') || lower.includes('not found') || lower.includes('no such file');
}

function isTrackerRecord(value: unknown): value is NimbalystTrackerRecord {
  if (!value || typeof value !== 'object') return false;
  const item = value as Partial<NimbalystTrackerRecord>;
  const fields = item.fields;
  return (
    typeof item.id === 'string' &&
    typeof item.primaryType === 'string' &&
    Boolean(item.system && typeof item.system.workspace === 'string') &&
    Boolean(fields && typeof fields.title === 'string') &&
    Boolean(fields && TRACKER_STATUSES.includes(fields.status as NimbalystTrackerStatus))
  );
}

const SHARED_DOCS_FILE_NAME = '.nimbalyst-shared-docs.json';

function SharedDocsMode({
  workspacePath,
  activeFile,
  onOpenWorkspace,
}: {
  workspacePath: string;
  activeFile: ActiveFile | null;
  onOpenWorkspace: () => void;
}) {
  const [documents, setDocuments] = useState<NimbalystSharedDocument[]>([]);
  const [selectedDocumentId, setSelectedDocumentId] = useState<string | null>(null);
  const [loading, setLoading] = useState(false);
  const [saving, setSaving] = useState(false);
  const [message, setMessage] = useState<string | null>(null);
  const [newTitle, setNewTitle] = useState('');
  const [draftTitle, setDraftTitle] = useState('');
  const [draftContent, setDraftContent] = useState('');

  const sharedDocsPath = useMemo(
    () => (workspacePath ? joinWorkspacePath(workspacePath, SHARED_DOCS_FILE_NAME) : ''),
    [workspacePath]
  );
  const selectedDocument = documents.find((document) => document.id === selectedDocumentId) || null;

  const loadDocuments = useCallback(async () => {
    if (!workspacePath) {
      setDocuments([]);
      setSelectedDocumentId(null);
      return;
    }

    setLoading(true);
    setMessage(null);
    try {
      const electronAPI = (window as any).electronAPI;
      let raw: string | null = null;
      if (electronAPI?.readFile && sharedDocsPath) {
        const result = await electronAPI.readFile(sharedDocsPath);
        if (result?.success) {
          raw = result.content;
        } else if (!isMissingFileError(result?.error)) {
          throw new Error(result?.error || 'Failed to read shared document file.');
        }
      } else {
        raw = localStorage.getItem(getSharedDocsStorageKey(workspacePath));
      }

      if (!raw) {
        setDocuments([]);
        setSelectedDocumentId(null);
        return;
      }

      const parsed = JSON.parse(raw) as Partial<NimbalystSharedDocumentStore>;
      const nextDocuments = Array.isArray(parsed.documents)
        ? parsed.documents.filter(isSharedDocument)
        : [];
      setDocuments(nextDocuments);
      setSelectedDocumentId((current) => (
        current && nextDocuments.some((document) => document.id === current)
          ? current
          : nextDocuments[0]?.id || null
      ));
    } catch (error) {
      setMessage(error instanceof Error ? error.message : String(error));
      setDocuments([]);
      setSelectedDocumentId(null);
    } finally {
      setLoading(false);
    }
  }, [sharedDocsPath, workspacePath]);

  useEffect(() => {
    loadDocuments();
  }, [loadDocuments]);

  const loadDocumentsRef = useRef(loadDocuments);

  useEffect(() => {
    loadDocumentsRef.current = loadDocuments;
  }, [loadDocuments]);

  useEffect(() => {
    const electronAPI = (window as any).electronAPI;
    if (!workspacePath || !electronAPI?.watchNimbalystWorkspace || !electronAPI?.onNimbalystWorkspaceChanged) {
      return undefined;
    }

    let disposed = false;
    let timer: number | null = null;

    const scheduleLoad = () => {
      if (timer) window.clearTimeout(timer);
      timer = window.setTimeout(() => {
        timer = null;
        if (!disposed) void loadDocumentsRef.current();
      }, 350);
    };

    const unsubscribe = electronAPI.onNimbalystWorkspaceChanged((event: { workspacePath?: string; path?: string }) => {
      if (!event || event.workspacePath !== workspacePath) return;
      const changedPath = String(event.path || '').replace(/\\/g, '/');
      if (changedPath === SHARED_DOCS_FILE_NAME) scheduleLoad();
    });

    void electronAPI.watchNimbalystWorkspace(workspacePath).then((result: { success?: boolean; error?: string }) => {
      if (!disposed && result?.success === false) {
        setMessage(`Workspace watcher unavailable: ${result.error || 'unknown error'}`);
      }
    });

    return () => {
      disposed = true;
      if (timer) window.clearTimeout(timer);
      if (typeof unsubscribe === 'function') unsubscribe();
      void electronAPI.unwatchNimbalystWorkspace?.();
    };
  }, [workspacePath]);

  useEffect(() => {
    setDraftTitle(selectedDocument?.title || '');
    setDraftContent(selectedDocument?.content || '');
  }, [selectedDocument?.id, selectedDocument?.title, selectedDocument?.content]);

  const persistDocuments = async (nextDocuments: NimbalystSharedDocument[]) => {
    const payload: NimbalystSharedDocumentStore = {
      version: 1,
      updatedAt: new Date().toISOString(),
      documents: nextDocuments,
    };
    const raw = JSON.stringify(payload, null, 2);
    const electronAPI = (window as any).electronAPI;

    setSaving(true);
    setMessage(null);
    try {
      if (electronAPI?.writeFile && sharedDocsPath) {
        const result = await electronAPI.writeFile(sharedDocsPath, raw);
        if (!result?.success) {
          throw new Error(result?.error || 'Failed to write shared document file.');
        }
      } else {
        localStorage.setItem(getSharedDocsStorageKey(workspacePath), raw);
      }
      setDocuments(nextDocuments);
      setMessage(`Saved ${nextDocuments.length} shared document(s).`);
    } catch (error) {
      setMessage(error instanceof Error ? error.message : String(error));
    } finally {
      setSaving(false);
    }
  };

  const createDocument = async (fromActiveFile: boolean) => {
    if (!workspacePath) {
      setMessage('Open a workspace before creating shared documents.');
      return;
    }

    const sourceFile = fromActiveFile ? activeFile : null;
    const resolvedTitle =
      newTitle.trim() ||
      (sourceFile ? basename(sourceFile.path) : '') ||
      `Shared Doc ${documents.length + 1}`;
    const now = new Date().toISOString();
    const document: NimbalystSharedDocument = {
      id: createSharedDocumentId(),
      title: resolvedTitle,
      content: sourceFile?.content || '',
      sourcePath: sourceFile?.path,
      createdAt: now,
      updatedAt: now,
    };

    await persistDocuments([document, ...documents]);
    setSelectedDocumentId(document.id);
    setNewTitle('');
  };

  const saveSelectedDocument = async () => {
    if (!selectedDocument) return;
    const updatedAt = new Date().toISOString();
    const nextDocuments = documents.map((document) => (
      document.id === selectedDocument.id
        ? {
            ...document,
            title: draftTitle.trim() || document.title,
            content: draftContent,
            updatedAt,
          }
        : document
    ));
    await persistDocuments(nextDocuments);
  };

  const deleteSelectedDocument = async () => {
    if (!selectedDocument) return;
    const nextDocuments = documents.filter((document) => document.id !== selectedDocument.id);
    await persistDocuments(nextDocuments);
    setSelectedDocumentId(nextDocuments[0]?.id || null);
  };

  if (!workspacePath) {
    return (
      <div className="flex h-full items-center justify-center p-8">
        <div className="max-w-lg text-center">
          <div className="mx-auto flex size-12 items-center justify-center rounded-xl bg-[#252525] text-[#a78bfa]">
            <Cloud className="size-6" />
          </div>
          <h2 className="mt-4 text-lg font-semibold">Shared Docs</h2>
          <p className="mt-2 text-sm leading-6 text-[#999]">
            Open a workspace to create local shared-document records for the Nimbalyst collaboration surface.
          </p>
          <button
            type="button"
            onClick={onOpenWorkspace}
            className="mt-4 h-9 rounded-md bg-[#4f46e5] px-4 text-xs font-medium text-white"
          >
            Open Workspace
          </button>
        </div>
      </div>
    );
  }

  return (
    <div className="grid min-h-full grid-cols-[minmax(280px,360px)_1fr]">
      <aside className="border-r border-[#2f2f2f] p-4">
        <h2 className="text-lg font-semibold">Shared Docs</h2>
        <p className="mt-2 text-sm leading-6 text-[#a7a7a7]">
          Local shared documents are stored in <span className="font-mono text-[#d5d5d5]">{SHARED_DOCS_FILE_NAME}</span> until the encrypted reference sync services are hosted.
        </p>

        <div className="mt-4 rounded-md border border-[#303030] bg-[#202020] p-3">
          <p className="text-xs font-semibold text-[#ededed]">Create Document</p>
          <input
            value={newTitle}
            onChange={(event) => setNewTitle(event.target.value)}
            placeholder="Document title"
            className="mt-3 h-9 w-full rounded-md border border-[#333] bg-[#111] px-2 text-xs text-[#ddd] outline-none focus:border-[#4f46e5]"
          />
          <button
            type="button"
            onClick={() => createDocument(false)}
            disabled={saving}
            className="mt-3 h-8 w-full rounded-md bg-[#4f46e5] text-xs font-medium text-white disabled:opacity-50"
          >
            Create Empty Document
          </button>
          <button
            type="button"
            onClick={() => createDocument(true)}
            disabled={saving || !activeFile}
            className="mt-2 h-8 w-full rounded border border-[#3a3a3a] text-xs text-[#cfcfcf] hover:border-[#4f46e5] disabled:opacity-50"
          >
            Create From Active File
          </button>
        </div>

        <div className="mt-4 rounded-md border border-[#303030] bg-[#202020] p-3">
          <div className="flex items-center justify-between gap-2">
            <p className="text-xs font-semibold text-[#ededed]">Documents</p>
            <span className="rounded bg-[#303030] px-1.5 py-0.5 text-[10px] text-[#aaa]">{documents.length}</span>
          </div>
          <div className="mt-2 space-y-1">
            {documents.length > 0 ? documents.map((document) => (
              <button
                key={document.id}
                type="button"
                onClick={() => setSelectedDocumentId(document.id)}
                className={`w-full rounded px-2 py-2 text-left text-xs ${
                  selectedDocumentId === document.id
                    ? 'bg-[#312e81] text-white'
                    : 'bg-[#171717] text-[#cfcfcf] hover:bg-[#262626]'
                }`}
              >
                <span className="block truncate font-semibold">{document.title}</span>
                <span className="mt-1 block truncate font-mono text-[10px] text-[#777]">
                  {document.sourcePath || document.id}
                </span>
              </button>
            )) : (
              <p className="rounded bg-[#171717] px-2 py-2 text-xs text-[#777]">No shared documents yet.</p>
            )}
          </div>
          <button
            type="button"
            onClick={loadDocuments}
            disabled={loading}
            className="mt-3 flex h-8 w-full items-center justify-center gap-2 rounded border border-[#3a3a3a] text-xs text-[#cfcfcf] hover:border-[#4f46e5] disabled:opacity-50"
          >
            <RefreshCw className={`size-3 ${loading ? 'animate-spin' : ''}`} />
            Reload Shared Docs
          </button>
        </div>

        {message && (
          <p className="mt-4 rounded border border-[#303030] bg-[#1b1b1b] px-3 py-2 text-xs text-[#aaa]">
            {message}
          </p>
        )}
      </aside>

      <section className="flex min-h-full flex-col p-4">
        <div className="flex items-start justify-between gap-3">
          <div>
            <h3 className="text-sm font-semibold">{selectedDocument?.title || 'No document selected'}</h3>
            <p className="mt-1 font-mono text-xs text-[#777]">{sharedDocsPath}</p>
          </div>
          <div className="flex gap-2">
            <button
              type="button"
              onClick={saveSelectedDocument}
              disabled={!selectedDocument || saving}
              className="h-8 rounded-md bg-[#4f46e5] px-3 text-xs font-medium text-white disabled:opacity-50"
            >
              {saving ? 'Saving...' : 'Save'}
            </button>
            <button
              type="button"
              onClick={deleteSelectedDocument}
              disabled={!selectedDocument || saving}
              className="h-8 rounded border border-[#3a3a3a] px-3 text-xs text-[#cfcfcf] hover:border-[#4f46e5] disabled:opacity-50"
            >
              Delete
            </button>
          </div>
        </div>

        {selectedDocument ? (
          <div className="mt-4 flex min-h-0 flex-1 flex-col rounded-md border border-[#303030] bg-[#202020]">
            <input
              value={draftTitle}
              onChange={(event) => setDraftTitle(event.target.value)}
              className="h-11 border-b border-[#303030] bg-[#171717] px-3 text-sm font-semibold text-[#ededed] outline-none focus:border-[#4f46e5]"
            />
            {selectedDocument.sourcePath && (
              <div className="border-b border-[#303030] px-3 py-2 font-mono text-[11px] text-[#777]">
                Source {selectedDocument.sourcePath}
              </div>
            )}
            <textarea
              value={draftContent}
              onChange={(event) => setDraftContent(event.target.value)}
              className="min-h-[420px] flex-1 resize-none bg-[#111] p-4 font-mono text-xs leading-6 text-[#d6d6d6] outline-none"
            />
          </div>
        ) : (
          <div className="mt-4 flex flex-1 items-center justify-center rounded-md border border-[#303030] bg-[#1b1b1b] p-8 text-sm text-[#777]">
            Create or select a shared document.
          </div>
        )}
      </section>
    </div>
  );
}

function createSharedDocumentId() {
  if (typeof crypto !== 'undefined' && 'randomUUID' in crypto) {
    return crypto.randomUUID();
  }
  return `shared-doc-${Date.now()}-${Math.random().toString(36).slice(2, 8)}`;
}

function getSharedDocsStorageKey(workspacePath: string) {
  return `batrack:nimbalyst:shared-docs:${workspacePath || 'default'}`;
}

function isSharedDocument(value: unknown): value is NimbalystSharedDocument {
  if (!value || typeof value !== 'object') return false;
  const document = value as Partial<NimbalystSharedDocument>;
  return (
    typeof document.id === 'string' &&
    typeof document.title === 'string' &&
    typeof document.content === 'string' &&
    typeof document.createdAt === 'string' &&
    typeof document.updatedAt === 'string'
  );
}

function InfoCard({ icon: Icon, title, body }: { icon: typeof FileText; title: string; body: string }) {
  return (
    <div className="rounded-md border border-[#303030] bg-[#202020] p-4">
      <Icon className="size-5 text-[#a78bfa]" />
      <p className="mt-3 text-sm font-semibold">{title}</p>
      <p className="mt-1 text-xs leading-5 text-[#888]">{body}</p>
    </div>
  );
}
