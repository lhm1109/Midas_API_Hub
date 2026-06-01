export const COLLAB_INIT_ORIGIN = Symbol.for('nimbalyst:collab-init');

export function MaterialSymbol() {
  return null;
}

export function useDocumentPath() {
  return {
    documentPath: null,
    workspacePath: null,
  };
}
