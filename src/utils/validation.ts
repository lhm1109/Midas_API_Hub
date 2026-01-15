// 🎯 검증 유틸리티

export function isValidJSON(str: string): boolean {
  try {
    JSON.parse(str);
    return true;
  } catch {
    return false;
  }
}

export function isValidURL(str: string): boolean {
  try {
    new URL(str);
    return true;
  } catch {
    return false;
  }
}

export function isValidVersion(version: string): boolean {
  // v1.0.0, 1.0.0, v1.0 등의 형식 허용
  return /^v?\d+\.\d+(\.\d+)?$/.test(version);
}

export function isEmpty(value: any): boolean {
  if (value === null || value === undefined) return true;
  if (typeof value === 'string') return value.trim().length === 0;
  if (Array.isArray(value)) return value.length === 0;
  if (typeof value === 'object') return Object.keys(value).length === 0;
  return false;
}

export function validateRequired(value: any, fieldName: string): string | null {
  if (isEmpty(value)) {
    return `${fieldName}은(는) 필수 항목입니다.`;
  }
  return null;
}









