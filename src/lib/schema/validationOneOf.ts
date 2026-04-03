export interface ValidationOneOfInfo {
  optionKeyGroups: string[][];
  participantKeys: string[];
  description: string;
}

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

function buildDescription(optionKeyGroups: string[][]): string {
  const allSingleKey = optionKeyGroups.every((group) => group.length === 1);

  if (allSingleKey) {
    const keys = optionKeyGroups.map((group) => group[0]).filter(Boolean);
    return `Choose exactly one of the following keys: ${formatQuotedList(keys)}.`;
  }

  const groupTexts = optionKeyGroups.map((group) => `(${formatQuotedList(group)})`);
  return `Choose exactly one of the following key sets: ${groupTexts.join(' or ')}.`;
}

export function extractValidationOneOfInfo(node: any): ValidationOneOfInfo | null {
  if (!node || typeof node !== 'object' || Array.isArray(node)) {
    return null;
  }

  if (node.type !== 'object' || !Array.isArray(node.oneOf) || node.oneOf.length < 2) {
    return null;
  }

  const hasStructuredOption = node.oneOf.some((option: any) =>
    option &&
    typeof option === 'object' &&
    option.properties &&
    typeof option.properties === 'object' &&
    Object.keys(option.properties).length > 0
  );

  if (hasStructuredOption) {
    return null;
  }

  const optionKeyGroups = node.oneOf
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
    optionKeyGroups: dedupedGroups,
    participantKeys: uniqueStrings(dedupedGroups.flat()),
    description: buildDescription(dedupedGroups),
  };
}

export function getFieldLeafKeyForValidationOneOf(field: any): string {
  const rawKey = String(field?.key || field?.name || '');
  const segments = rawKey.split('.');
  return (segments[segments.length - 1] || rawKey).replace(/\[\]/g, '');
}

export function isValidationOneOfParticipant(field: any, info: ValidationOneOfInfo | null | undefined): boolean {
  if (!info) {
    return false;
  }

  return info.participantKeys.includes(getFieldLeafKeyForValidationOneOf(field));
}
