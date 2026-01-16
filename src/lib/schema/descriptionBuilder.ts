/**
 * Description Builder (YAML-based)
 * 
 * Enhanced Schema 필드의 description을 YAML 규칙에 따라 빌드합니다.
 * 
 * @see schema_definitions/{psdSet}/{schemaType}/schema-logic.yaml
 * @see schema_definitions/{psdSet}/{schemaType}/table.yaml
 */

import type { EnhancedField } from './schemaCompiler';
import type { TableDefinition } from '../rendering/definitionLoader';

/**
 * 필드 description 빌드
 */
export function buildFieldDescription(
  field: EnhancedField,
  tableDefinition: TableDefinition | null
): string {
  const descParts: string[] = [];
  const fieldAny = field as any;

  // 1. Label (x-ui.label 또는 description)
  if (fieldAny.ui?.label) {
    descParts.push(`**${fieldAny.ui.label}**`);
  } else if (field.description) {
    descParts.push(`**${field.description}**`);
  }

  // 2. Enum Values (표준 enum)
  if (field.enum && field.enum.length > 0) {
    descParts.push('**Enum Values:**');
    field.enum.forEach((val: any) => {
      const label = fieldAny.enumLabels?.[String(val)] || 
                   fieldAny['x-enum-labels']?.[String(val)] || 
                   val;
      descParts.push(`• ${val} - ${label}`);
    });
  }

  // 3. Enum by Type (x-enum-by-type)
  const enumByType = fieldAny.enumByType || fieldAny['x-enum-by-type'];
  if (enumByType) {
    descParts.push('**Enum Values by Type:**');
    for (const [type, values] of Object.entries(enumByType)) {
      descParts.push(`*${type}:*`);
      (values as any[]).forEach((val: any) => {
        const label = fieldAny.enumLabelsByType?.[type]?.[String(val)] ||
                     fieldAny['x-enum-labels-by-type']?.[type]?.[String(val)] ||
                     val;
        descParts.push(`• ${val} - ${label}`);
      });
    }
  }

  // 4. Value Constraints (x-value-constraint)
  const valueConstraint = fieldAny.valueConstraint || fieldAny['x-value-constraint'];
  if (valueConstraint) {
    descParts.push('**Value Constraints:**');
    for (const [type, constraint] of Object.entries(valueConstraint)) {
      descParts.push(`• *${type}:* ${constraint}`);
    }
  }

  // 5. Node Count by Type (x-node-count-by-type)
  const nodeCountByType = fieldAny.nodeCountByType || fieldAny['x-node-count-by-type'];
  if (nodeCountByType) {
    descParts.push('**Node Count by Type:**');
    for (const [type, count] of Object.entries(nodeCountByType)) {
      if (Array.isArray(count)) {
        descParts.push(`• *${type}:* ${count.join(', ')} nodes`);
      } else {
        descParts.push(`• *${type}:* ${count} nodes`);
      }
    }
  }

  // 6. Hint (x-ui.hint)
  if (fieldAny.ui?.hint) {
    descParts.push(`💡 ${fieldAny.ui.hint}`);
  }

  return descParts.join('\n');
}
