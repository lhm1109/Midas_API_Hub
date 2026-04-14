export interface ValidationChoiceInfo {
  keyword: 'oneOf' | 'anyOf';
  optionKeyGroups: string[][];
  participantKeys: string[];
  description: string;
}

export type ValidationOneOfInfo = ValidationChoiceInfo;

function uniqueStrings(values: unknown[]): string[] {
  const result: string[] = [];
  const seen = new Set<string>();

  values.forEach((value) => {
    if (typeof value !== 'string') {
      return;
    }
    if (seen.has(value)) {
      return;
    }
    seen.add(value);
    result.push(value);
  });

  return result;
}

function formatQuotedList(values: string[]): string {
  const quoted = values.map((value) => `"${value}"`);
  if (quoted.length <= 1) {
    return quoted[0] || '';
  }
  if (quoted.length === 2) {
    return `${quoted[0]} or ${quoted[1]}`;
  }
  return `${quoted.slice(0, -1).join(', ')}, or ${quoted[quoted.length - 1]}`;
}

function buildDescription(
  keyword: 'oneOf' | 'anyOf',
  optionKeyGroups: string[][]
): string {
  const allSingleKey = optionKeyGroups.every((group) => group.length === 1);

  if (allSingleKey) {
    const keys = optionKeyGroups.map((group) => group[0]).filter(Boolean);
    if (keyword === 'anyOf') {
      return `At least one of the following keys must be provided: ${formatQuotedList(keys)}.`;
    }
    return `Choose exactly one of the following keys: ${formatQuotedList(keys)}.`;
  }

  const groupTexts = optionKeyGroups.map((group) => `(${formatQuotedList(group)})`);
  if (keyword === 'anyOf') {
    return `At least one of the following key sets must be provided: ${groupTexts.join(' or ')}.`;
  }
  return `Choose exactly one of the following key sets: ${groupTexts.join(' or ')}.`;
}

function extractValidationGroups(
  node: any,
  keyword: 'oneOf' | 'anyOf'
): ValidationChoiceInfo | null {
  if (!node || typeof node !== 'object' || Array.isArray(node)) {
    return null;
  }

  const options = node[keyword];
  if (node.type !== 'object' || !Array.isArray(options) || options.length < 2) {
    return null;
  }

  const hasStructuredOption = options.some((option: any) =>
    option &&
    typeof option === 'object' &&
    option.properties &&
    typeof option.properties === 'object' &&
    Object.keys(option.properties).length > 0
  );

  if (hasStructuredOption) {
    return null;
  }

  const optionKeyGroups = options
    .map((option: any) => uniqueStrings(Array.isArray(option?.required) ? option.required : []))
    .filter((group: string[]) => group.length > 0);

  if (optionKeyGroups.length < 2) {
    return null;
  }

  const dedupedGroups: string[][] = [];
  const seenGroupKeys = new Set<string>();

  optionKeyGroups.forEach((group: string[]) => {
    const groupKey = group.join('|');
    if (seenGroupKeys.has(groupKey)) {
      return;
    }
    seenGroupKeys.add(groupKey);
    dedupedGroups.push(group);
  });

  if (dedupedGroups.length < 2) {
    return null;
  }

  return {
    keyword,
    optionKeyGroups: dedupedGroups,
    participantKeys: uniqueStrings(dedupedGroups.flat()),
    description: buildDescription(keyword, dedupedGroups),
  };
}

export function extractValidationChoiceInfo(node: any): ValidationChoiceInfo | null {
  return extractValidationGroups(node, 'oneOf') || extractValidationGroups(node, 'anyOf');
}

export function extractValidationOneOfInfo(node: any): ValidationOneOfInfo | null {
  const info = extractValidationGroups(node, 'oneOf');
  return info?.keyword === 'oneOf' ? info : null;
}

export function getFieldLeafKeyForValidationOneOf(field: any): string {
  const rawKey = String(field?.key || field?.name || '');
  const segments = rawKey.split('.');
  return (segments[segments.length - 1] || rawKey).replace(/\[\]/g, '');
}

export function isValidationChoiceParticipant(field: any, info: ValidationChoiceInfo | null | undefined): boolean {
  if (!info) {
    return false;
  }

  return info.participantKeys.includes(getFieldLeafKeyForValidationOneOf(field));
}

export function isValidationOneOfParticipant(field: any, info: ValidationOneOfInfo | null | undefined): boolean {
  return isValidationChoiceParticipant(field, info);
}
