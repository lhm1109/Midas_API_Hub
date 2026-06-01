export const COLLAB_INIT_ORIGIN = Symbol.for('nimbalyst:collab-init');

export async function copyToClipboard(text) {
  globalThis.__batrackNimbalystClipboard = String(text ?? '');
}

export async function readClipboard() {
  return globalThis.__batrackNimbalystClipboard || '';
}

export function useDocumentPath() {
  return {
    documentPath: null,
    workspacePath: null,
  };
}

export function MaterialSymbol() {
  return null;
}

export function useEditorLifecycle() {
  throw new Error('Nimbalyst editor lifecycle hooks require a renderer panel and are not available in Bat Rack server-hosted extensions.');
}

export function useCollaborativeEditor() {
  throw new Error('Nimbalyst collaborative editor hooks require a renderer panel and are not available in Bat Rack server-hosted extensions.');
}

export function createReadOnlyHost(options = {}) {
  return {
    ...options,
    readonly: true,
    isReadOnly: true,
    save: async () => {
      throw new Error('Read-only host cannot save content.');
    },
  };
}
