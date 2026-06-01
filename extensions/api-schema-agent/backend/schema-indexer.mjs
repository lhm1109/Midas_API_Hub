export function activate({ services }) {
  return {
    methods: {
      describe(params = {}) {
        return {
          name: 'schema-indexer',
          status: 'running',
          workspacePath: services.workspacePath,
          canReadWorkspace: services.hasPermission('workspace-files'),
          params,
        };
      },
    },
  };
}
