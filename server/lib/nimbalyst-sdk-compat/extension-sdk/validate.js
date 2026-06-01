export function printValidationResult(result) {
  if (!result) return;
  const issues = Array.isArray(result.issues) ? result.issues : [];
  if (issues.length === 0) {
    console.info('[Nimbalyst SDK compat] manifest validation passed');
    return;
  }
  for (const issue of issues) {
    console.warn('[Nimbalyst SDK compat] manifest validation issue:', issue);
  }
}
