export function createManifestValidationPlugin() {
  return {
    name: 'batrack-nimbalyst-manifest-validation-compat',
  };
}

export function createExtensionConfig(options = {}) {
  return options;
}

export function mergeExtensionConfig(baseConfig = {}, overrideConfig = {}) {
  return {
    ...baseConfig,
    ...overrideConfig,
    plugins: [
      ...(baseConfig.plugins || []),
      ...(overrideConfig.plugins || []),
    ],
  };
}
