import { describe, expect, it } from 'vitest';
import { generateEndpointIdFromPath, normalizeEndpointPath } from './endpointId.js';

describe('endpoint id helpers', () => {
  it('normalizes endpoint paths with a leading slash', () => {
    expect(normalizeEndpointPath('db/node')).toBe('/db/node');
    expect(normalizeEndpointPath('/db/node')).toBe('/db/node');
  });

  it('keeps stable ids for ordinary path-based endpoints', () => {
    expect(generateEndpointIdFromPath('/db/nlct', 'NLCT', 'civil_DB', 'POST')).toBe('db/nlct');
  });

  it('adds the endpoint name when the path is too generic', () => {
    expect(
      generateEndpointIdFromPath(
        '/DESIGN/MS/KDS-41-20-2022/TABLE',
        'Slab Flexural Checking Forces Table',
        'design',
        'POST',
      ),
    ).toBe('design/ms/kds-41-20-2022/table/slab-flexural-checking-forces-table');
  });

  it('falls back to the group when the path is empty', () => {
    expect(generateEndpointIdFromPath('', 'Beam Result', 'gen_result', 'POST')).toBe('result/beam-result');
  });
});
