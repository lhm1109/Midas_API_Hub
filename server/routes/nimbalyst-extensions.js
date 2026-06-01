import express from 'express';
import { exec as execChildProcess } from 'node:child_process';
import { execFile as execFileChildProcess } from 'node:child_process';
import fs from 'node:fs/promises';
import path from 'node:path';
import {
  activateNimbalystExtension,
  buildAllNimbalystMarketplaceExtensions,
  buildNimbalystMarketplaceExtension,
  deactivateNimbalystExtension,
  executeNimbalystBackendModule,
  executeNimbalystCommand,
  executeNimbalystExtensionTool,
  executeNimbalystSlashCommand,
  getNimbalystExtensionAssetPath,
  getNimbalystExtensionBackendModules,
  getNimbalystExtensionConfiguration,
  getNimbalystExtensionMainAssetPath,
  installNimbalystMarketplaceExtension,
  listNimbalystCommands,
  listNimbalystExtensionContributions,
  listNimbalystExtensions,
  listNimbalystExtensionTools,
  listNimbalystMarketplaceExtensions,
  listNimbalystSlashCommands,
  reloadNimbalystExtension,
  setNimbalystExtensionBackendModuleGrant,
  updateNimbalystExtensionConfiguration,
} from '../lib/nimbalyst-extension-host.js';

const router = express.Router();

function runHostCommand(command, options = {}) {
  return new Promise((resolve) => {
    execChildProcess(
      command,
      {
        cwd: options.cwd || options.workspacePath || process.cwd(),
        env: {
          ...process.env,
          ...(options.env || {}),
        },
        timeout: Number(options.timeout) || 60000,
        maxBuffer: Number(options.maxBuffer) || 10 * 1024 * 1024,
        windowsHide: true,
      },
      (error, stdout, stderr) => {
        resolve({
          success: !error,
          stdout: stdout || '',
          stderr: stderr || '',
          exitCode: typeof error?.code === 'number' ? error.code : 0,
        });
      }
    );
  });
}

function runHostFileCommand(file, args = [], options = {}) {
  return new Promise((resolve) => {
    execFileChildProcess(
      file,
      Array.isArray(args) ? args.map((arg) => String(arg)) : [],
      {
        cwd: options.cwd || options.workspacePath || process.cwd(),
        env: {
          ...process.env,
          ...(options.env || {}),
        },
        timeout: Number(options.timeout) || 60000,
        maxBuffer: Number(options.maxBuffer) || 10 * 1024 * 1024,
        windowsHide: true,
      },
      (error, stdout, stderr) => {
        resolve({
          success: !error,
          stdout: stdout || '',
          stderr: stderr || '',
          exitCode: typeof error?.code === 'number' ? error.code : 0,
        });
      }
    );
  });
}

function resolveWorkspaceFilePath(filePath, workspacePath) {
  if (typeof filePath !== 'string' || !filePath.trim()) {
    throw new Error('filePath is required');
  }
  const basePath = path.resolve(workspacePath || process.cwd());
  const resolvedPath = path.isAbsolute(filePath)
    ? path.resolve(filePath)
    : path.resolve(basePath, filePath);
  const baseWithSeparator = basePath.endsWith(path.sep) ? basePath : `${basePath}${path.sep}`;
  const normalizedBase = baseWithSeparator.toLowerCase();
  const normalizedTarget = resolvedPath.toLowerCase();
  if (workspacePath && normalizedTarget !== basePath.toLowerCase() && !normalizedTarget.startsWith(normalizedBase)) {
    throw new Error('filePath must be inside the workspace');
  }
  return resolvedPath;
}

function asyncRoute(handler) {
  return async (req, res, next) => {
    try {
      await handler(req, res, next);
    } catch (error) {
      next(error);
    }
  };
}

router.get('/extensions', asyncRoute(async (_req, res) => {
  res.json({
    extensions: await listNimbalystExtensions(),
  });
}));

router.get('/marketplace/extensions', asyncRoute(async (_req, res) => {
  res.json({
    extensions: await listNimbalystMarketplaceExtensions(),
  });
}));

router.post('/marketplace/extensions/:id/install', asyncRoute(async (req, res) => {
  res.json({
    extension: await installNimbalystMarketplaceExtension(req.params.id),
  });
}));

router.post('/marketplace/extensions/:id/build-install', asyncRoute(async (req, res) => {
  res.json(await buildNimbalystMarketplaceExtension(req.params.id));
}));

router.post('/marketplace/extensions/build-install-all', asyncRoute(async (_req, res) => {
  res.json(await buildAllNimbalystMarketplaceExtensions());
}));

router.post('/extensions/:id/activate', asyncRoute(async (req, res) => {
  const extension = await activateNimbalystExtension(req.params.id, {
    workspacePath: req.body?.workspacePath,
  });
  res.json({ extension });
}));

router.post('/extensions/:id/deactivate', asyncRoute(async (req, res) => {
  res.json({
    extension: await deactivateNimbalystExtension(req.params.id),
  });
}));

router.post('/extensions/:id/reload', asyncRoute(async (req, res) => {
  const extension = await reloadNimbalystExtension(req.params.id, {
    workspacePath: req.body?.workspacePath,
  });
  res.json({ extension });
}));

router.get('/tools', (_req, res) => {
  res.json({
    tools: listNimbalystExtensionTools(),
  });
});

router.get('/slash-commands', (_req, res) => {
  res.json({
    commands: listNimbalystSlashCommands(),
  });
});

router.get('/commands', (_req, res) => {
  res.json({
    commands: listNimbalystCommands(),
  });
});

router.get('/contributions', asyncRoute(async (_req, res) => {
  res.json({
    contributions: await listNimbalystExtensionContributions(),
  });
}));

router.get('/extensions/:id/module', asyncRoute(async (req, res) => {
  const filePath = await getNimbalystExtensionMainAssetPath(req.params.id);
  res.type('application/javascript').sendFile(filePath);
}));

router.get('/extensions/:id/configuration', asyncRoute(async (req, res) => {
  res.json({
    configuration: await getNimbalystExtensionConfiguration(req.params.id),
  });
}));

router.put('/extensions/:id/configuration', asyncRoute(async (req, res) => {
  res.json({
    configuration: await updateNimbalystExtensionConfiguration(
      req.params.id,
      req.body?.values || {}
    ),
  });
}));

router.get('/extensions/:id/backend-modules', asyncRoute(async (req, res) => {
  res.json({
    backendModules: await getNimbalystExtensionBackendModules(req.params.id, {
      workspacePath: req.query.workspacePath,
    }),
  });
}));

router.put('/extensions/:id/backend-modules/:moduleId/grant', asyncRoute(async (req, res) => {
  res.json({
    backendModules: await setNimbalystExtensionBackendModuleGrant(
      req.params.id,
      req.params.moduleId,
      {
        scope: req.body?.scope,
        enabled: req.body?.enabled,
        workspacePath: req.body?.workspacePath,
      }
    ),
  });
}));

router.post('/extensions/:id/backend-modules/:moduleId/execute', asyncRoute(async (req, res) => {
  const result = await executeNimbalystBackendModule(
    req.params.id,
    req.params.moduleId,
    req.body?.method || 'describe',
    req.body?.params || {},
    {
      workspacePath: req.body?.workspacePath,
    }
  );
  res.json(result);
}));

router.get('/extensions/:id/assets/*', asyncRoute(async (req, res) => {
  const filePath = await getNimbalystExtensionAssetPath(req.params.id, req.params[0]);
  res.sendFile(filePath);
}));

router.post('/host/exec', asyncRoute(async (req, res) => {
  const command = req.body?.command;
  if (typeof command !== 'string' || !command.trim()) {
    res.status(400).json({
      success: false,
      stdout: '',
      stderr: 'command is required',
      exitCode: 1,
    });
    return;
  }

  const result = await runHostCommand(command, {
    cwd: req.body?.cwd,
    env: req.body?.env,
    timeout: req.body?.timeout,
    maxBuffer: req.body?.maxBuffer,
    workspacePath: req.body?.workspacePath,
  });
  res.json(result);
}));

router.post('/host/git', asyncRoute(async (req, res) => {
  const args = req.body?.args;
  if (!Array.isArray(args)) {
    res.status(400).json({
      success: false,
      stdout: '',
      stderr: 'args array is required',
      exitCode: 1,
    });
    return;
  }

  const result = await runHostFileCommand('git', args, {
    cwd: req.body?.cwd,
    env: req.body?.env,
    timeout: req.body?.timeout,
    maxBuffer: req.body?.maxBuffer,
    workspacePath: req.body?.workspacePath,
  });
  res.json(result);
}));

router.post('/host/file/read', asyncRoute(async (req, res) => {
  try {
    const filePath = resolveWorkspaceFilePath(req.body?.filePath, req.body?.workspacePath);
    const buffer = await fs.readFile(filePath);
    res.json({
      success: true,
      content: buffer.toString('utf8'),
      base64: buffer.toString('base64'),
    });
  } catch (error) {
    res.status(400).json({
      success: false,
      error: error instanceof Error ? error.message : String(error),
    });
  }
}));

router.post('/host/file/write-binary', asyncRoute(async (req, res) => {
  try {
    const filePath = resolveWorkspaceFilePath(req.body?.filePath, req.body?.workspacePath);
    const base64 = typeof req.body?.base64 === 'string' ? req.body.base64 : '';
    await fs.mkdir(path.dirname(filePath), { recursive: true });
    await fs.writeFile(filePath, Buffer.from(base64, 'base64'));
    res.json({ success: true });
  } catch (error) {
    res.status(400).json({
      success: false,
      error: error instanceof Error ? error.message : String(error),
    });
  }
}));

router.get('/browser-shims/:name', (req, res) => {
  const shims = {
    'react.js': `
      const React = window.parent.__BAT_RACK_REACT__;
      export default React;
      export const Children = React.Children;
      export const Component = React.Component;
      export const Fragment = React.Fragment;
      export const Profiler = React.Profiler;
      export const PureComponent = React.PureComponent;
      export const StrictMode = React.StrictMode;
      export const Suspense = React.Suspense;
      export const cloneElement = React.cloneElement;
      export const createContext = React.createContext;
      export const createElement = React.createElement;
      export const createFactory = React.createFactory;
      export const createRef = React.createRef;
      export const forwardRef = React.forwardRef;
      export const isValidElement = React.isValidElement;
      export const lazy = React.lazy;
      export const memo = React.memo;
      export const startTransition = React.startTransition;
      export const useCallback = React.useCallback;
      export const useContext = React.useContext;
      export const useDebugValue = React.useDebugValue;
      export const useDeferredValue = React.useDeferredValue;
      export const useEffect = React.useEffect;
      export const useId = React.useId;
      export const useImperativeHandle = React.useImperativeHandle;
      export const useInsertionEffect = React.useInsertionEffect;
      export const useLayoutEffect = React.useLayoutEffect;
      export const useMemo = React.useMemo;
      export const useReducer = React.useReducer;
      export const useRef = React.useRef;
      export const useState = React.useState;
      export const useSyncExternalStore = React.useSyncExternalStore;
      export const useTransition = React.useTransition;
      export const version = React.version;
    `,
    'react-jsx-runtime.js': `
      const JSXRuntime = window.parent.__BAT_RACK_REACT_JSX_RUNTIME__;
      export const Fragment = JSXRuntime.Fragment;
      export const jsx = JSXRuntime.jsx;
      export const jsxs = JSXRuntime.jsxs;
      export default JSXRuntime;
    `,
    'react-dom-client.js': `
      const ReactDOMClient = window.parent.__BAT_RACK_REACT_DOM_CLIENT__;
      export const createRoot = ReactDOMClient.createRoot;
      export const hydrateRoot = ReactDOMClient.hydrateRoot;
      export default ReactDOMClient;
    `,
    'extension-sdk.js': `
      const React = window.parent.__BAT_RACK_REACT__;
      export const COLLAB_INIT_ORIGIN = Symbol.for('nimbalyst:collab-init');
      export async function copyToClipboard(text) { await window.parent.navigator.clipboard?.writeText?.(String(text ?? '')); }
      export async function readClipboard() { return await window.parent.navigator.clipboard?.readText?.() || ''; }
      export function useDocumentPath() {
        const host = window.__BAT_RACK_EDITOR_HOST__ || window.__BAT_RACK_PANEL_HOST__ || {};
        const documentPath = host.filePath || null;
        const documentDir = documentPath ? String(documentPath).replace(/[\\\\/][^\\\\/]*$/, '') : null;
        return { documentPath, documentDir };
      }
      export function MaterialSymbol(props = {}) { return React.createElement('span', { className: props.className, title: props.title }, props.children || props.icon || props.name || ''); }
      export function useEditorLifecycle(host, options) {
        const [isLoading, setIsLoading] = React.useState(true);
        const [error, setError] = React.useState(null);
        const [isDirty, setIsDirty] = React.useState(false);
        const [theme, setTheme] = React.useState(host.theme || 'dark');
        const [isSourceMode, setIsSourceMode] = React.useState(host.isSourceModeActive?.() || false);
        const [diffState, setDiffState] = React.useState(null);
        const optionsRef = React.useRef(options);
        const lastSavedContentRef = React.useRef('');
        optionsRef.current = options;
        const parseContent = React.useCallback((raw) => {
          const parse = optionsRef.current?.parse;
          return parse ? parse(raw) : raw;
        }, []);
        const serializeContent = React.useCallback((content) => {
          const serialize = optionsRef.current?.serialize;
          return serialize ? serialize(content) : String(content ?? '');
        }, []);
        const clearDirty = React.useCallback(() => {
          setIsDirty(false);
          host.setDirty?.(false);
        }, [host]);
        const markDirty = React.useCallback(() => {
          setIsDirty(true);
          host.setDirty?.(true);
        }, [host]);
        React.useEffect(() => {
          let mounted = true;
          const load = optionsRef.current?.binary ? host.loadBinaryContent?.() : host.loadContent?.();
          Promise.resolve(load)
            .then((raw) => {
              if (!mounted) return;
              const content = optionsRef.current?.binary ? raw : parseContent(raw);
              if (!optionsRef.current?.binary) lastSavedContentRef.current = raw;
              optionsRef.current?.applyContent?.(content);
              setIsLoading(false);
              optionsRef.current?.onLoaded?.();
            })
            .catch((err) => {
              if (!mounted) return;
              setError(err instanceof Error ? err : new Error(String(err)));
              setIsLoading(false);
            });
          return () => { mounted = false; };
        }, [host, parseContent]);
        React.useEffect(() => host.onSaveRequested?.(async () => {
          const opts = optionsRef.current || {};
          if (opts.onSave) {
            await opts.onSave();
          } else if (opts.getCurrentContent) {
            const content = opts.getCurrentContent();
            if (opts.binary) {
              await host.saveContent(content);
            } else {
              const serialized = serializeContent(content);
              lastSavedContentRef.current = serialized;
              await host.saveContent(serialized);
            }
          }
          clearDirty();
        }) || (() => undefined), [host, serializeContent, clearDirty]);
        React.useEffect(() => host.onFileChanged?.((raw) => {
          if (raw === lastSavedContentRef.current) return;
          lastSavedContentRef.current = raw;
          optionsRef.current?.applyContent?.(parseContent(raw));
          clearDirty();
          optionsRef.current?.onExternalChange?.(parseContent(raw));
        }) || (() => undefined), [host, parseContent, clearDirty]);
        React.useEffect(() => host.onThemeChanged?.((nextTheme) => setTheme(nextTheme)) || (() => undefined), [host]);
        React.useEffect(() => host.onDiffRequested?.((config) => {
          const opts = optionsRef.current || {};
          if (opts.onDiffRequested) {
            opts.onDiffRequested(config);
            return;
          }
          if (!opts.binary) {
            const original = parseContent(config.originalContent || '');
            const modified = parseContent(config.modifiedContent || '');
            setDiffState({
              original,
              modified,
              tagId: config.tagId,
              sessionId: config.sessionId,
              accept: () => {
                opts.applyContent?.(modified);
                host.reportDiffResult?.({ content: serializeContent(modified), action: 'accept' });
                setDiffState(null);
                clearDirty();
              },
              reject: () => {
                opts.applyContent?.(original);
                host.reportDiffResult?.({ content: serializeContent(original), action: 'reject' });
                setDiffState(null);
                clearDirty();
              },
            });
          }
        }) || (() => undefined), [host, parseContent, serializeContent, clearDirty]);
        React.useEffect(() => host.onDiffCleared?.(() => {
          setDiffState(null);
          optionsRef.current?.onDiffCleared?.();
        }) || (() => undefined), [host]);
        React.useEffect(() => host.onSourceModeChanged?.((next) => setIsSourceMode(Boolean(next))) || (() => undefined), [host]);
        return {
          isLoading,
          error,
          theme,
          markDirty,
          isDirty,
          diffState,
          toggleSourceMode: host.toggleSourceMode?.bind(host),
          isSourceMode,
        };
      }
      export function useCollaborativeEditor(host) {
        return {
          isCollaborative: Boolean(host?.collaboration),
          status: host?.collaboration?.getStatus?.() || 'disconnected',
          collaborators: new Map(),
          binding: null,
        };
      }
      export function createReadOnlyHost(options = {}) { return { ...options, readonly: true, isReadOnly: true }; }
    `,
    'runtime.js': `
      export * from './extension-sdk.js';
      export function getExtensionLoader() { return null; }
      const transcriptMarkdownContributions = window.parent.__BAT_RACK_NIMBALYST_TRANSCRIPT_MARKDOWN__
        || (window.parent.__BAT_RACK_NIMBALYST_TRANSCRIPT_MARKDOWN__ = new Map());
      export function setTranscriptMarkdownContributions(source, contributions) {
        transcriptMarkdownContributions.set(source, contributions);
        window.parent.dispatchEvent?.(new CustomEvent('batrack:nimbalyst-transcript-markdown-changed'));
      }
      export function clearTranscriptMarkdownContributions(source) {
        transcriptMarkdownContributions.delete(source);
        window.parent.dispatchEvent?.(new CustomEvent('batrack:nimbalyst-transcript-markdown-changed'));
      }
      export function getTranscriptMarkdownContributions() {
        return Array.from(transcriptMarkdownContributions.values());
      }
    `,
  };

  const source = shims[req.params.name];
  if (!source) {
    res.status(404).type('text/plain').send('Not found');
    return;
  }
  res.type('application/javascript').send(source);
});

router.post('/tools/:name/execute', asyncRoute(async (req, res) => {
  const result = await executeNimbalystExtensionTool(
    req.params.name,
    req.body?.args || {},
    {
      workspacePath: req.body?.workspacePath,
      activeFilePath: req.body?.activeFilePath,
    }
  );
  res.status(result.success === false ? 400 : 200).json(result);
}));

router.post('/commands/:id/execute', asyncRoute(async (req, res) => {
  const result = await executeNimbalystCommand(
    req.params.id,
    req.body?.args || {},
    {
      workspacePath: req.body?.workspacePath,
      activeFilePath: req.body?.activeFilePath,
    }
  );
  res.status(result.success === false ? 400 : 200).json(result);
}));

router.post('/slash-commands/:id/execute', asyncRoute(async (req, res) => {
  const result = await executeNimbalystSlashCommand(
    req.params.id,
    req.body?.args || '',
    {
      workspacePath: req.body?.workspacePath,
      activeFilePath: req.body?.activeFilePath,
    }
  );
  res.status(result.success === false ? 400 : 200).json(result);
}));

export default router;
