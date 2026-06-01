import React from 'react';
import { useEditorLifecycle } from '@nimbalyst/extension-sdk';

export async function activate(context) {
  context.services.ui.showInfo('Bat Rack API Schema Agent activated.');
}

export async function deactivate() {
  console.info('[Bat Rack API Schema Agent] deactivated');
}

const pingTool = {
  name: 'batrack.ping',
  description: 'Checks whether the Nimbalyst extension compatibility layer is running.',
  scope: 'global',
  inputSchema: {
    type: 'object',
    properties: {}
  },
  handler: async (_args, context) => ({
    success: true,
    message: `Nimbalyst compatibility layer is active for ${context.extensionContext.manifest.name}.`,
    data: {
      workspacePath: context.workspacePath || null,
      activeFilePath: context.activeFilePath || null
    }
  })
};

const writeArtifactTool = {
  name: 'batrack.write_artifact',
  description: 'Writes a text artifact through context.services.filesystem.writeFile().',
  scope: 'global',
  inputSchema: {
    type: 'object',
    properties: {
      filename: {
        type: 'string',
        description: 'Relative output filename.'
      },
      content: {
        type: 'string',
        description: 'File content to write.'
      }
    },
    required: ['filename', 'content']
  },
  handler: async (args, context) => {
    const config = context.extensionContext.services.configuration;
    const artifactDir = config?.get('artifactDir', '.bat-rack/nimbalyst-extension-output') ||
      '.bat-rack/nimbalyst-extension-output';
    const filename = String(args.filename || 'artifact.txt').replace(/[\\/]+/g, '-');
    const outputPath = `${artifactDir}/${filename}`;

    await context.extensionContext.services.filesystem.writeFile(outputPath, String(args.content || ''));

    return {
      success: true,
      message: `Wrote artifact: ${outputPath}`,
      data: { outputPath }
    };
  }
};

export const aiTools = [pingTool, writeArtifactTool];

function SchemaAgentPanel({ host }) {
  return React.createElement(
    'div',
    {
      style: {
        boxSizing: 'border-box',
        height: '100%',
        padding: 16,
        background: '#151515',
        color: '#e6e6e6',
        fontFamily: 'Inter, system-ui, sans-serif',
      },
    },
    React.createElement('h2', { style: { margin: '0 0 8px', fontSize: 16 } }, 'Bat Rack Schema Agent'),
    React.createElement(
      'p',
      { style: { margin: 0, color: '#9ca3af', fontSize: 13, lineHeight: 1.6 } },
      'This Nimbalyst-compatible panel is rendered through the Bat Rack iframe panel host.'
    ),
    React.createElement(
      'dl',
      { style: { display: 'grid', gridTemplateColumns: '120px 1fr', gap: 8, marginTop: 18, fontSize: 12 } },
      React.createElement('dt', { style: { color: '#8b8b8b' } }, 'Extension'),
      React.createElement('dd', { style: { margin: 0 } }, host?.extensionId || 'unknown'),
      React.createElement('dt', { style: { color: '#8b8b8b' } }, 'Panel'),
      React.createElement('dd', { style: { margin: 0 } }, host?.panelId || 'unknown'),
      React.createElement('dt', { style: { color: '#8b8b8b' } }, 'Workspace'),
      React.createElement('dd', { style: { margin: 0 } }, host?.workspacePath || 'No workspace selected')
    )
  );
}

export const panels = {
  'schema-agent': {
    component: SchemaAgentPanel,
  },
};

export const slashCommandHandlers = {
  schemaNote(args, context) {
    const target = args?.trim() || context?.activeFilePath || 'current API';
    return {
      success: true,
      message: `Created schema note for ${target}.`,
      data: `## Schema Note\n\nTarget: ${target}\nWorkspace: ${context?.workspacePath || 'unknown'}\n`,
    };
  },
};

export const commandHandlers = {
  'com.batrack.api-schema-agent.describe-workspace': (_args, context) => ({
    success: true,
    message: 'Workspace described.',
    data: {
      workspacePath: context?.workspacePath || null,
      activeFilePath: context?.activeFilePath || null,
    },
  }),
};

function ApiSchemaAgentSettings({ storage, theme }) {
  const [artifactDir, setArtifactDir] = React.useState(
    storage.get('artifactDir') || '.bat-rack/nimbalyst-extension-output'
  );
  const [saved, setSaved] = React.useState(false);

  return React.createElement(
    'div',
    {
      style: {
        boxSizing: 'border-box',
        height: '100%',
        padding: 16,
        background: theme === 'dark' ? '#151515' : '#fff',
        color: theme === 'dark' ? '#e6e6e6' : '#111827',
        fontFamily: 'Inter, system-ui, sans-serif',
        fontSize: 13,
      },
    },
    React.createElement('h3', { style: { margin: '0 0 8px', fontSize: 15 } }, 'API Schema Agent Settings'),
    React.createElement(
      'label',
      { style: { display: 'block', color: '#9ca3af', marginBottom: 6 } },
      'Artifact directory'
    ),
    React.createElement('input', {
      value: artifactDir,
      onChange: (event) => {
        setArtifactDir(event.target.value);
        setSaved(false);
      },
      style: {
        boxSizing: 'border-box',
        width: '100%',
        height: 34,
        border: '1px solid #3a3a3a',
        borderRadius: 6,
        background: '#111',
        color: '#e6e6e6',
        padding: '0 10px',
      },
    }),
    React.createElement(
      'button',
      {
        type: 'button',
        onClick: async () => {
          await storage.set('artifactDir', artifactDir);
          setSaved(true);
        },
        style: {
          marginTop: 12,
          height: 30,
          border: 0,
          borderRadius: 6,
          background: '#4f46e5',
          color: '#fff',
          padding: '0 12px',
          cursor: 'pointer',
        },
      },
      saved ? 'Saved' : 'Save'
    )
  );
}

export const settingsPanel = {
  ApiSchemaAgentSettings,
};

function ApiSchemaAgentHostBadge() {
  const [visible, setVisible] = React.useState(true);
  if (!visible) return null;

  return React.createElement(
    'div',
    {
      style: {
        boxSizing: 'border-box',
        height: '100%',
        padding: 10,
        border: '1px solid #3a3a3a',
        borderRadius: 8,
        background: 'rgba(24, 24, 27, 0.96)',
        color: '#e6e6e6',
        boxShadow: '0 12px 30px rgba(0, 0, 0, 0.28)',
        fontFamily: 'Inter, system-ui, sans-serif',
        fontSize: 12,
      },
    },
    React.createElement(
      'div',
      { style: { display: 'flex', alignItems: 'center', justifyContent: 'space-between', gap: 8 } },
      React.createElement('strong', { style: { fontSize: 12 } }, 'API Schema Agent'),
      React.createElement(
        'button',
        {
          type: 'button',
          onClick: () => setVisible(false),
          style: {
            width: 22,
            height: 22,
            border: '1px solid #3a3a3a',
            borderRadius: 4,
            background: '#202020',
            color: '#a7a7a7',
            cursor: 'pointer',
          },
        },
        '×'
      )
    ),
    React.createElement(
      'p',
      { style: { margin: '8px 0 0', color: '#9ca3af', lineHeight: 1.45 } },
      'Host component rendered at the Nimbalyst app level.'
    )
  );
}

export const hostComponents = {
  ApiSchemaAgentHostBadge,
};

function ApiNoteEditor({ host }) {
  const [content, setContent] = React.useState('');
  const { isLoading, error, markDirty, theme } = useEditorLifecycle(host, {
    applyContent: setContent,
    getCurrentContent: () => content,
  });

  if (isLoading) {
    return React.createElement('div', { style: { padding: 16, color: '#9ca3af' } }, 'Loading API note...');
  }

  if (error) {
    return React.createElement('pre', { style: { padding: 16, color: '#fecaca' } }, error.message);
  }

  return React.createElement(
    'div',
    {
      style: {
        boxSizing: 'border-box',
        height: '100%',
        display: 'grid',
        gridTemplateRows: 'auto 1fr',
        background: theme === 'dark' ? '#151515' : '#fff',
        color: theme === 'dark' ? '#e6e6e6' : '#111827',
        fontFamily: 'Inter, system-ui, sans-serif',
      },
    },
    React.createElement(
      'div',
      {
        style: {
          borderBottom: '1px solid #303030',
          padding: '10px 12px',
          fontSize: 12,
          color: '#9ca3af',
        },
      },
      `Nimbalyst custom editor host: ${host.fileName}`
    ),
    React.createElement('textarea', {
      value: content,
      onChange: (event) => {
        setContent(event.target.value);
        markDirty();
      },
      style: {
        width: '100%',
        height: '100%',
        boxSizing: 'border-box',
        border: 0,
        outline: 'none',
        resize: 'none',
        padding: 16,
        background: 'transparent',
        color: 'inherit',
        font: '13px/1.6 ui-monospace, SFMono-Regular, Menlo, monospace',
      },
    })
  );
}

function ApiNoteHeader({ fileName, getContent, contentVersion, onContentChange }) {
  const [lineCount, setLineCount] = React.useState(0);

  React.useEffect(() => {
    setLineCount(getContent().split(/\r?\n/).length);
  }, [contentVersion, getContent]);

  return React.createElement(
    'div',
    {
      style: {
        boxSizing: 'border-box',
        height: 48,
        display: 'flex',
        alignItems: 'center',
        justifyContent: 'space-between',
        gap: 12,
        padding: '0 12px',
        background: '#191919',
        borderBottom: '1px solid #303030',
        color: '#d6d6d6',
        fontFamily: 'Inter, system-ui, sans-serif',
        fontSize: 12,
      },
    },
    React.createElement(
      'div',
      { style: { minWidth: 0 } },
      React.createElement('strong', { style: { display: 'block', color: '#ededed' } }, 'API Note'),
      React.createElement(
        'span',
        { style: { color: '#8b8b8b' } },
        `${fileName} · ${lineCount} line${lineCount === 1 ? '' : 's'}`
      )
    ),
    React.createElement(
      'button',
      {
        type: 'button',
        onClick: () => {
          const content = getContent();
          const nextContent = `${content.replace(/\s*$/, '')}\n\n<!-- touched by ApiNoteHeader -->\n`;
          onContentChange?.(nextContent);
        },
        style: {
          height: 28,
          border: '1px solid #3a3a3a',
          borderRadius: 6,
          background: '#242424',
          color: '#e6e6e6',
          padding: '0 10px',
          cursor: 'pointer',
          fontSize: 12,
        },
      },
      'Touch'
    )
  );
}

export const components = {
  ApiNoteEditor,
  ApiNoteHeader,
};
