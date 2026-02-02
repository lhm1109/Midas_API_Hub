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
  _tableDefinition: TableDefinition | null
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
      // 🔥 개선: "설명 : 값" 형식, 문자열은 따옴표로 감싸기
      const formattedVal = typeof val === 'string' ? `"${val}"` : val;
      descParts.push(`• ${label} : ${formattedVal}`);
    });
  }

  // 🔥 2-1. oneOf 형식 (JSON Schema 표준 - const + title)
  if (fieldAny.oneOf && Array.isArray(fieldAny.oneOf)) {
    descParts.push('**Enum Values:**');
    fieldAny.oneOf.forEach((option: any) => {
      const val = option.const;
      const label = option.title || val;
      const formattedVal = typeof val === 'string' ? `"${val}"` : val;
      descParts.push(`• ${label} : ${formattedVal}`);
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
        // 🔥 개선: "설명 : 값" 형식, 문자열은 따옴표로 감싸기
        const formattedVal = typeof val === 'string' ? `"${val}"` : val;
        descParts.push(`• ${label} : ${formattedVal}`);
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

  // 🔥 6. Value Hints by Type (x-value-hints-by-type) - 순수 UI 힌트
  const valueHintsByType = fieldAny.valueHintsByType || fieldAny['x-value-hints-by-type'];
  if (valueHintsByType) {
    descParts.push('**💡 Value Hints by Type:**');
    for (const [type, hint] of Object.entries(valueHintsByType)) {
      descParts.push(`• *${type}:* ${hint}`);
    }
  }

  // 7. Hint (x-ui.hint)
  if (fieldAny.ui?.hint) {
    descParts.push(`💡 ${fieldAny.ui.hint}`);
  }


  return descParts.join('\n');
}
