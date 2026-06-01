import { mkdtemp, mkdir, readFile, rm, writeFile } from 'fs/promises';
import os from 'os';
import path from 'path';
import { describe, expect, it, afterEach } from 'vitest';
import { createNimbalystExtensionHost } from './nimbalyst-extension-host.js';

const tempRoots = [];

async function createTempRoot() {
  const root = await mkdtemp(path.join(os.tmpdir(), 'batrack-nimbalyst-ext-'));
  tempRoots.push(root);
  return root;
}

afterEach(async () => {
  await Promise.all(tempRoots.splice(0).map((root) => rm(root, { recursive: true, force: true })));
});

async function writeExtension(root, dirName, files) {
  const extensionPath = path.join(root, dirName);
  await Promise.all(
    Object.entries(files).map(async ([name, content]) => {
      const filePath = path.join(extensionPath, name);
      await mkdir(path.dirname(filePath), { recursive: true });
      await writeFile(filePath, content, 'utf-8');
    })
  );
  return extensionPath;
}

describe('createNimbalystExtensionHost', () => {
  it('discovers, activates, and executes exported aiTools', async () => {
    const root = await createTempRoot();
    const extensionsRoot = path.join(root, 'extensions');
    const workspaceRoot = path.join(root, 'workspace');
    await mkdir(workspaceRoot, { recursive: true });
    await writeExtension(extensionsRoot, 'sample', {
      'manifest.json': JSON.stringify({
        id: 'com.example.sample',
        name: 'Sample',
        version: '1.0.0',
        main: 'index.mjs',
        permissions: { filesystem: true, ai: true },
      }),
      'index.mjs': `
        export const aiTools = [{
          name: 'sample.write',
          description: 'write a file',
          inputSchema: { type: 'object', properties: { content: { type: 'string' } } },
          handler: async (args, context) => {
            await context.extensionContext.services.filesystem.writeFile('out.txt', args.content || '');
            return { success: true, message: 'ok' };
          }
        }];
      `,
    });

    const host = createNimbalystExtensionHost({ extensionsRoot, workspaceRoot });
    const extensions = await host.listExtensions();
    expect(extensions).toHaveLength(1);
    expect(extensions[0].active).toBe(false);

    await host.activateExtension('com.example.sample');
    expect(host.listTools().map((tool) => tool.name)).toContain('sample.write');

    const result = await host.executeTool('sample.write', { content: 'hello' });
    expect(result.success).toBe(true);
    await expect(readFile(path.join(workspaceRoot, 'out.txt'), 'utf-8')).resolves.toBe('hello');
  });

  it('accepts manifest-only extensions for contribution metadata', async () => {
    const root = await createTempRoot();
    const extensionsRoot = path.join(root, 'extensions');
    await writeExtension(extensionsRoot, 'planning', {
      'manifest.json': JSON.stringify({
        id: 'com.nimbalyst.planning',
        name: 'Planning',
        version: '1.0.0',
        description: 'Structured planning and tracking with YAML frontmatter',
        contributions: {
          claudePlugin: {
            path: 'claude-plugin',
            displayName: 'Planning Tools',
            commands: [
              {
                name: 'design',
                description: 'Create a structured plan document',
              },
            ],
          },
          agentWorkflows: {
            path: 'workflows',
            displayName: 'Planning Workflows',
          },
        },
      }),
    });

    const host = createNimbalystExtensionHost({ extensionsRoot, workspaceRoot: root });
    const extensions = await host.listExtensions();
    expect(extensions).toMatchObject([
      {
        id: 'com.nimbalyst.planning',
        main: '',
        active: false,
      },
    ]);

    const contributions = await host.listContributions();
    expect(contributions.map((contribution) => contribution.type)).toEqual([
      'agentWorkflows',
      'claudePlugin',
    ]);

    const activated = await host.activateExtension('com.nimbalyst.planning', { workspacePath: root });
    expect(activated).toMatchObject({
      id: 'com.nimbalyst.planning',
      main: '',
      active: true,
    });
    expect(host.listExecutableCommands()).toEqual([]);
    expect(host.listExecutableSlashCommands()).toEqual([]);
  });

  it('lists and installs bundled reference marketplace extensions', async () => {
    const root = await createTempRoot();
    const extensionsRoot = path.join(root, 'extensions');
    const referenceRoot = path.join(root, 'reference');
    const referenceExtensionsRoot = path.join(referenceRoot, 'packages', 'extensions');
    const registryPath = path.join(referenceRoot, 'packages', 'electron', 'src', 'main', 'data', 'extensionRegistry.json');
    await mkdir(path.dirname(registryPath), { recursive: true });
    await writeFile(
      registryPath,
      JSON.stringify({
        extensions: [
          {
            id: 'com.nimbalyst.planning',
            name: 'Planning',
            description: 'Structured planning and tracking',
            version: '1.0.0',
            categories: ['productivity'],
            tags: ['planning'],
            permissions: ['filesystem'],
          },
        ],
      }),
      'utf-8'
    );
    await writeExtension(referenceExtensionsRoot, 'planning', {
      'manifest.json': JSON.stringify({
        id: 'com.nimbalyst.planning',
        name: 'Planning',
        version: '1.0.0',
        contributions: {
          claudePlugin: {
            path: 'claude-plugin',
            displayName: 'Planning Tools',
          },
        },
      }),
      'claude-plugin/commands/design.md': '# Design\n',
    });

    const host = createNimbalystExtensionHost({ extensionsRoot, workspaceRoot: root, referenceRoot });
    expect(await host.listExtensions()).toEqual([]);

    const marketplace = await host.listMarketplaceExtensions();
    expect(marketplace).toMatchObject([
      {
        id: 'com.nimbalyst.planning',
        installable: true,
        installed: false,
        availableLocal: true,
      },
    ]);

    const installed = await host.installMarketplaceExtension('com.nimbalyst.planning');
    expect(installed).toMatchObject({
      id: 'com.nimbalyst.planning',
      active: false,
      main: '',
    });
    await expect(readFile(path.join(extensionsRoot, 'com.nimbalyst.planning', 'claude-plugin', 'commands', 'design.md'), 'utf-8'))
      .resolves.toBe('# Design\n');

    const afterInstall = await host.listMarketplaceExtensions();
    expect(afterInstall[0]).toMatchObject({
      id: 'com.nimbalyst.planning',
      installed: true,
      installedVersion: '1.0.0',
    });
  });

  it('builds and installs source-only bundled reference marketplace extensions', async () => {
    const root = await createTempRoot();
    const extensionsRoot = path.join(root, 'extensions');
    const referenceRoot = path.join(root, 'reference');
    const referenceExtensionsRoot = path.join(referenceRoot, 'packages', 'extensions');
    await writeExtension(referenceExtensionsRoot, 'panel-source', {
      'manifest.json': JSON.stringify({
        id: 'com.nimbalyst.panel-source',
        name: 'Panel Source',
        version: '1.0.0',
        main: 'dist/index.js',
        contributions: {
          panels: [
            {
              id: 'panel',
              title: 'Panel',
            },
          ],
        },
      }),
      'package.json': JSON.stringify({
        scripts: {
          build: 'node build.mjs',
        },
      }),
      'build.mjs': `
        import fs from 'fs';
        fs.mkdirSync('dist', { recursive: true });
        fs.writeFileSync('dist/index.js', 'export const panels = {};\\n');
      `,
    });

    const host = createNimbalystExtensionHost({ extensionsRoot, workspaceRoot: root, referenceRoot });
    const marketplace = await host.listMarketplaceExtensions();
    expect(marketplace).toMatchObject([
      {
        id: 'com.nimbalyst.panel-source',
        installable: false,
        buildable: true,
        requiresBuild: true,
      },
    ]);

    const result = await host.buildMarketplaceExtension('com.nimbalyst.panel-source');
    expect(result.build.skipped).toBe(false);
    expect(result.extension).toMatchObject({
      id: 'com.nimbalyst.panel-source',
      main: 'dist/index.js',
    });
    await expect(readFile(path.join(extensionsRoot, 'com.nimbalyst.panel-source', 'dist', 'index.js'), 'utf-8'))
      .resolves.toContain('panels');
  });

  it('builds and installs all bundled reference marketplace extensions', async () => {
    const root = await createTempRoot();
    const extensionsRoot = path.join(root, 'extensions');
    const referenceRoot = path.join(root, 'reference');
    const referenceExtensionsRoot = path.join(referenceRoot, 'packages', 'extensions');
    await writeExtension(referenceExtensionsRoot, 'manifest-only', {
      'manifest.json': JSON.stringify({
        id: 'com.nimbalyst.manifest-only',
        name: 'Manifest Only',
        version: '1.0.0',
        contributions: {
          themes: [
            {
              id: 'theme',
              name: 'Theme',
            },
          ],
        },
      }),
    });
    await writeExtension(referenceExtensionsRoot, 'source-only', {
      'manifest.json': JSON.stringify({
        id: 'com.nimbalyst.source-only',
        name: 'Source Only',
        version: '1.0.0',
        main: 'dist/index.js',
        contributions: {
          panels: [
            {
              id: 'panel',
              title: 'Panel',
            },
          ],
        },
      }),
      'package.json': JSON.stringify({
        scripts: {
          build: 'node build.mjs',
        },
      }),
      'build.mjs': `
        import fs from 'fs';
        fs.mkdirSync('dist', { recursive: true });
        fs.writeFileSync('dist/index.js', 'export const panels = {};\\n');
      `,
    });

    const host = createNimbalystExtensionHost({ extensionsRoot, workspaceRoot: root, referenceRoot });
    const result = await host.buildAllMarketplaceExtensions();
    expect(result).toMatchObject({
      installed: 2,
      failed: 0,
    });
    expect(result.results.map((item) => item.id).sort()).toEqual([
      'com.nimbalyst.manifest-only',
      'com.nimbalyst.source-only',
    ]);
    await expect(readFile(path.join(extensionsRoot, 'com.nimbalyst.source-only', 'dist', 'index.js'), 'utf-8'))
      .resolves.toContain('panels');
    await expect(readFile(path.join(extensionsRoot, 'com.nimbalyst.manifest-only', 'manifest.json'), 'utf-8'))
      .resolves.toContain('Manifest Only');
  });

  it('supports context.services.ai.registerTool during activate', async () => {
    const root = await createTempRoot();
    const extensionsRoot = path.join(root, 'extensions');
    await writeExtension(extensionsRoot, 'registered', {
      'manifest.json': JSON.stringify({
        id: 'com.example.registered',
        name: 'Registered',
        version: '1.0.0',
        main: 'index.mjs',
        permissions: { ai: true },
      }),
      'index.mjs': `
        export async function activate(context) {
          context.services.ai.registerTool({
            name: 'registered.echo',
            description: 'echo',
            handler: async (args) => ({ success: true, data: args })
          });
        }
      `,
    });

    const host = createNimbalystExtensionHost({ extensionsRoot, workspaceRoot: root });
    await host.activateExtension('com.example.registered');

    const result = await host.executeTool('registered.echo', { value: 42 });
    expect(result.success).toBe(true);
    expect(result.data).toEqual({ value: 42 });
  });

  it('executes manifest slashCommands through exported slashCommandHandlers', async () => {
    const root = await createTempRoot();
    const extensionsRoot = path.join(root, 'extensions');
    await writeExtension(extensionsRoot, 'slash', {
      'manifest.json': JSON.stringify({
        id: 'com.example.slash',
        name: 'Slash',
        version: '1.0.0',
        main: 'index.mjs',
        contributions: {
          slashCommands: [
            {
              id: 'slash.echo',
              title: 'Echo',
              handler: 'echo',
            },
          ],
        },
      }),
      'index.mjs': `
        export const slashCommandHandlers = {
          echo: async (args, context) => ({
            success: true,
            data: { args, workspacePath: context.workspacePath }
          })
        };
      `,
    });

    const host = createNimbalystExtensionHost({ extensionsRoot, workspaceRoot: root });
    expect(host.listExecutableSlashCommands()).toEqual([]);

    await host.activateExtension('com.example.slash', { workspacePath: root });
    expect(host.listExecutableSlashCommands()).toMatchObject([
      {
        id: 'slash.echo',
        title: 'Echo',
        executable: true,
      },
    ]);

    const result = await host.executeSlashCommand('slash.echo', 'hello', { workspacePath: root });
    expect(result.success).toBe(true);
    expect(result.data).toEqual({ args: 'hello', workspacePath: root });
  });

  it('executes manifest commands through exported commandHandlers', async () => {
    const root = await createTempRoot();
    const extensionsRoot = path.join(root, 'extensions');
    await writeExtension(extensionsRoot, 'command', {
      'manifest.json': JSON.stringify({
        id: 'com.example.command',
        name: 'Command',
        version: '1.0.0',
        main: 'index.mjs',
        contributions: {
          commands: [
            {
              id: 'command.describe',
              title: 'Describe',
            },
          ],
        },
      }),
      'index.mjs': `
        export const commandHandlers = {
          'command.describe': async (args, context) => ({
            success: true,
            data: { args, workspacePath: context.workspacePath }
          })
        };
      `,
    });

    const host = createNimbalystExtensionHost({ extensionsRoot, workspaceRoot: root });
    expect(host.listExecutableCommands()).toEqual([]);

    await host.activateExtension('com.example.command', { workspacePath: root });
    expect(host.listExecutableCommands()).toMatchObject([
      {
        id: 'command.describe',
        title: 'Describe',
        executable: true,
      },
    ]);

    const result = await host.executeCommand('command.describe', { value: 7 }, { workspacePath: root });
    expect(result.success).toBe(true);
    expect(result.data).toEqual({ args: { value: 7 }, workspacePath: root });
  });

  it('blocks filesystem access outside the workspace root', async () => {
    const root = await createTempRoot();
    const extensionsRoot = path.join(root, 'extensions');
    const workspaceRoot = path.join(root, 'workspace');
    await mkdir(workspaceRoot, { recursive: true });
    await writeExtension(extensionsRoot, 'unsafe', {
      'manifest.json': JSON.stringify({
        id: 'com.example.unsafe',
        name: 'Unsafe',
        version: '1.0.0',
        main: 'index.mjs',
        permissions: { filesystem: true, ai: true },
      }),
      'index.mjs': `
        export const aiTools = [{
          name: 'unsafe.escape',
          description: 'attempt path escape',
          handler: async (_args, context) => {
            await context.extensionContext.services.filesystem.writeFile('../escape.txt', 'bad');
            return { success: true };
          }
        }];
      `,
    });

    const host = createNimbalystExtensionHost({ extensionsRoot, workspaceRoot });
    await host.activateExtension('com.example.unsafe');

    const result = await host.executeTool('unsafe.escape');
    expect(result.success).toBe(false);
    expect(result.error).toContain('outside the workspace root');
  });

  it('loads extensions that import the Nimbalyst extension SDK package', async () => {
    const root = await createTempRoot();
    const extensionsRoot = path.join(root, 'extensions');
    await writeExtension(extensionsRoot, 'sdk-import', {
      'manifest.json': JSON.stringify({
        id: 'com.example.sdk-import',
        name: 'SDK Import',
        version: '1.0.0',
        main: 'index.mjs',
        permissions: { ai: true },
      }),
      'index.mjs': `
        import { COLLAB_INIT_ORIGIN, copyToClipboard, readClipboard } from '@nimbalyst/extension-sdk';

        export async function activate(context) {
          context.services.ai.registerTool({
            name: 'sdk.check',
            description: 'check sdk compatibility',
            handler: async () => {
              await copyToClipboard('from-sdk');
              return {
                success: true,
                data: {
                  clipboard: await readClipboard(),
                  collabSymbol: String(COLLAB_INIT_ORIGIN)
                }
              };
            }
          });
        }
      `,
    });

    const host = createNimbalystExtensionHost({ extensionsRoot, workspaceRoot: root });
    await host.activateExtension('com.example.sdk-import');

    const extensions = await host.listExtensions();
    expect(extensions.map((extension) => extension.id)).toEqual(['com.example.sdk-import']);

    const result = await host.executeTool('sdk.check');
    expect(result.success).toBe(true);
    expect(result.data).toEqual({
      clipboard: 'from-sdk',
      collabSymbol: 'Symbol(nimbalyst:collab-init)',
    });
  });

  it('persists manifest configuration and exposes updates to active extensions', async () => {
    const root = await createTempRoot();
    const extensionsRoot = path.join(root, 'extensions');
    const configurationRoot = path.join(root, 'config');
    await writeExtension(extensionsRoot, 'configurable', {
      'manifest.json': JSON.stringify({
        id: 'com.example.configurable',
        name: 'Configurable',
        version: '1.0.0',
        main: 'index.mjs',
        permissions: { ai: true },
        contributions: {
          configuration: {
            title: 'Configurable Settings',
            properties: {
              artifactDir: {
                type: 'string',
                default: 'artifacts',
              },
              enabled: {
                type: 'boolean',
                default: true,
              },
            },
          },
        },
      }),
      'index.mjs': `
        export const aiTools = [{
          name: 'config.read',
          description: 'read config',
          handler: async (_args, context) => ({
            success: true,
            data: context.extensionContext.services.configuration.getAll()
          })
        }];
      `,
    });

    const host = createNimbalystExtensionHost({ extensionsRoot, workspaceRoot: root, configurationRoot });

    await expect(host.getExtensionConfiguration('com.example.configurable')).resolves.toMatchObject({
      title: 'Configurable Settings',
      values: {
        artifactDir: 'artifacts',
        enabled: true,
      },
    });

    await host.updateExtensionConfiguration('com.example.configurable', { artifactDir: 'custom' });
    await host.activateExtension('com.example.configurable');

    await expect(host.executeTool('config.read')).resolves.toMatchObject({
      success: true,
      data: {
        artifactDir: 'custom',
        enabled: true,
      },
    });

    await host.updateExtensionConfiguration('com.example.configurable', { enabled: false });
    await expect(host.executeTool('config.read')).resolves.toMatchObject({
      success: true,
      data: {
        artifactDir: 'custom',
        enabled: false,
      },
    });
    await expect(readFile(path.join(configurationRoot, 'com.example.configurable.json'), 'utf-8'))
      .resolves.toContain('"enabled": false');
  });

  it('validates backend module declarations and persists workspace/global grants', async () => {
    const root = await createTempRoot();
    const extensionsRoot = path.join(root, 'extensions');
    const workspaceRoot = path.join(root, 'workspace');
    const backendGrantRoot = path.join(root, 'backend-grants');
    await mkdir(workspaceRoot, { recursive: true });
    await writeExtension(extensionsRoot, 'backend', {
      'manifest.json': JSON.stringify({
        id: 'com.example.backend',
        name: 'Backend',
        version: '1.0.0',
        main: 'index.mjs',
        contributions: {
          backendModules: [
            {
              id: 'schema-indexer',
              entry: 'backend/indexer.mjs',
              runtime: 'worker-thread',
              permissions: ['workspace-files', 'filesystem'],
              enablement: {
                default: 'disabled',
                promptOn: 'firstUse',
                purpose: 'Index schemas in this workspace.',
              },
            },
          ],
        },
      }),
      'index.mjs': `export const aiTools = [];`,
      'backend/indexer.mjs': `
        export function describe(params, context) {
          return {
            name: 'schema-indexer',
            workspacePath: context.services.workspacePath,
            canReadWorkspace: context.services.hasPermission('workspace-files'),
            params,
          };
        }
      `,
    });

    const host = createNimbalystExtensionHost({ extensionsRoot, workspaceRoot, backendGrantRoot });
    const initial = await host.getExtensionBackendModules('com.example.backend', { workspacePath: workspaceRoot });

    expect(initial.modules).toMatchObject([
      {
        id: 'schema-indexer',
        entryExists: true,
        permissions: ['workspace-files'],
        workspaceEnabled: false,
        globalEnabled: false,
      },
    ]);
    expect(initial.modules[0].issues).toEqual(
      expect.arrayContaining([
        expect.objectContaining({ severity: 'warning' }),
      ])
    );

    await expect(host.executeBackendModule('com.example.backend', 'schema-indexer', 'describe', {}, {
      workspacePath: workspaceRoot,
    })).resolves.toMatchObject({
      success: false,
      error: expect.stringContaining('not enabled'),
    });

    const workspaceGrant = await host.setExtensionBackendModuleGrant('com.example.backend', 'schema-indexer', {
      scope: 'workspace',
      enabled: true,
      workspacePath: workspaceRoot,
    });
    expect(workspaceGrant.modules[0].workspaceEnabled).toBe(true);
    expect(workspaceGrant.modules[0].globalEnabled).toBe(false);

    await expect(host.executeBackendModule('com.example.backend', 'schema-indexer', 'describe', { target: 'schemas' }, {
      workspacePath: workspaceRoot,
    })).resolves.toMatchObject({
      success: true,
      data: {
        name: 'schema-indexer',
        workspacePath: workspaceRoot,
        canReadWorkspace: true,
        params: {
          target: 'schemas',
        },
      },
    });

    const globalGrant = await host.setExtensionBackendModuleGrant('com.example.backend', 'schema-indexer', {
      scope: 'global',
      enabled: true,
      workspacePath: workspaceRoot,
    });
    expect(globalGrant.modules[0].workspaceEnabled).toBe(true);
    expect(globalGrant.modules[0].globalEnabled).toBe(true);

    await expect(readFile(path.join(backendGrantRoot, 'com.example.backend.json'), 'utf-8'))
      .resolves.toContain('schema-indexer');

    await host.setExtensionBackendModuleGrant('com.example.backend', 'schema-indexer', {
      scope: 'global',
      enabled: false,
      workspacePath: workspaceRoot,
    });
  });

  it('builds a Nimbalyst-style manifest contribution registry', async () => {
    const root = await createTempRoot();
    const extensionsRoot = path.join(root, 'extensions');
    await writeExtension(extensionsRoot, 'contrib', {
      'manifest.json': JSON.stringify({
        id: 'com.example.contrib',
        name: 'Contribution Test',
        version: '1.0.0',
        main: 'index.mjs',
        permissions: { ai: true },
        contributions: {
          customEditors: [
            {
              filePatterns: ['*.csv'],
              displayName: 'CSV Editor',
              component: 'CsvEditor',
            },
          ],
          documentHeaders: [
            {
              id: 'csv-header',
              filePatterns: ['*.csv'],
              displayName: 'CSV Header',
              component: 'CsvHeader',
              priority: 75,
            },
          ],
          panels: [
            {
              id: 'database',
              title: 'Database',
              icon: 'database',
            },
          ],
          newFileMenu: [
            {
              extension: '.api.md',
              displayName: 'API Note',
              icon: 'description',
              defaultContent: '# API',
            },
          ],
          slashCommands: [
            {
              id: 'contrib.generate',
              title: 'Generate',
              handler: 'generate',
            },
          ],
          commands: [
            {
              id: 'com.example.contrib.database.toggle',
              title: 'Toggle Database',
            },
          ],
          keybindings: [
            {
              key: 'ctrl+alt+d',
              command: 'com.example.contrib.database.toggle',
            },
          ],
          hostComponents: ['ContributionHostToast'],
          themes: [
            {
              id: 'review-dark',
              name: 'Review Dark',
              isDark: true,
              colors: {
                bg: '#111111',
              },
            },
          ],
          backendModules: [
            {
              id: 'schema-indexer',
              entry: 'backend/indexer.mjs',
              runtime: 'worker-thread',
              permissions: ['workspace-files'],
              enablement: {
                default: 'disabled',
                promptOn: 'firstUse',
                purpose: 'Index schema files.',
              },
            },
          ],
          settingsPanel: {
            component: 'ContributionSettings',
            title: 'Contribution Settings',
            icon: 'settings',
            order: 80,
          },
          fileIcons: {
            '*.api.md': 'api',
          },
          configuration: {
            properties: {
              enabled: {
                type: 'boolean',
                default: true,
              },
            },
          },
        },
      }),
      'index.mjs': `export const aiTools = [];`,
    });

    await mkdir(path.join(extensionsRoot, 'node_modules'), { recursive: true });

    const host = createNimbalystExtensionHost({ extensionsRoot, workspaceRoot: root });
    const contributions = await host.listContributions();
    const byType = contributions.reduce((acc, contribution) => {
      acc[contribution.type] = [...(acc[contribution.type] || []), contribution];
      return acc;
    }, {});

    expect(contributions.map((contribution) => contribution.extensionId)).not.toContain('node_modules');
    expect(byType.customEditors?.[0]).toMatchObject({
      id: 'CsvEditor',
      title: 'CSV Editor',
      extensionId: 'com.example.contrib',
    });
    expect(byType.documentHeaders?.[0]).toMatchObject({
      id: 'csv-header',
      title: 'CSV Header',
      extensionId: 'com.example.contrib',
    });
    expect(byType.panels?.[0]).toMatchObject({ id: 'database', title: 'Database' });
    expect(byType.newFileMenu?.[0]).toMatchObject({ id: '.api.md', title: 'API Note' });
    expect(byType.slashCommands?.[0]).toMatchObject({ id: 'contrib.generate', title: 'Generate' });
    expect(byType.commands?.[0]).toMatchObject({
      id: 'com.example.contrib.database.toggle',
      title: 'Toggle Database',
    });
    expect(byType.keybindings?.[0]).toMatchObject({
      id: 'com.example.contrib.database.toggle',
      title: 'ctrl+alt+d',
    });
    expect(byType.hostComponents?.[0]).toMatchObject({
      id: 'ContributionHostToast',
      title: 'ContributionHostToast',
    });
    expect(byType.themes?.[0]).toMatchObject({
      id: 'review-dark',
      title: 'Review Dark',
    });
    expect(byType.backendModules?.[0]).toMatchObject({
      id: 'schema-indexer',
      title: 'schema-indexer',
    });
    expect(byType.settingsPanel?.[0]).toMatchObject({
      id: 'ContributionSettings',
      title: 'Contribution Settings',
    });
    expect(byType.fileIcons?.[0]).toMatchObject({ id: '*.api.md', title: '*.api.md' });
    expect(byType.configuration?.[0]).toMatchObject({
      id: 'com.example.contrib.configuration',
      description: '1 configurable properties',
    });

    await expect(host.getExtensionMainAssetPath('com.example.contrib')).resolves.toBe(
      path.join(extensionsRoot, 'contrib', 'index.mjs')
    );
    await expect(host.getExtensionAssetPath('com.example.contrib', '../escape.js')).rejects.toThrow(
      'outside the workspace root'
    );
  });
});
