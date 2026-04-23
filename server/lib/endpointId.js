function slugifyIdSegment(value, fallback = '') {
  const normalized = String(value || '')
    .trim()
    .toLowerCase()
    .replace(/[\\/]+/g, '-')
    .replace(/\s+/g, '-')
    .replace(/[^a-z0-9-]/g, '-')
    .replace(/-+/g, '-')
    .replace(/^-|-$/g, '');

  return normalized || fallback;
}

export function normalizeEndpointPath(rawPath) {
  const trimmed = typeof rawPath === 'string' ? rawPath.trim() : '';
  if (!trimmed) return '';
  return trimmed.startsWith('/') ? trimmed : `/${trimmed}`;
}

export function generateEndpointIdFromPath(path, fallbackName, fallbackGroupId, method) {
  const cleanPath = normalizeEndpointPath(path).replace(/^\//, '');
  const pathSegments = cleanPath
    .split('/')
    .map((segment) => slugifyIdSegment(segment))
    .filter(Boolean);

  const fallbackGroup = typeof fallbackGroupId === 'string'
    ? fallbackGroupId.split('_').pop() || fallbackGroupId
    : 'endpoint';

  const baseSegments = pathSegments.length > 0
    ? pathSegments
    : [slugifyIdSegment(fallbackGroup, 'endpoint')];

  const lastSegment = baseSegments[baseSegments.length - 1];
  const nameSlug = slugifyIdSegment(fallbackName);
  const methodSlug = slugifyIdSegment(String(method || '').replace(/,/g, '-'));

  if (nameSlug && nameSlug !== lastSegment) {
    baseSegments.push(nameSlug);
  } else if (!nameSlug && methodSlug && methodSlug !== lastSegment) {
    baseSegments.push(methodSlug);
  }

  return baseSegments.join('/');
}
