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

  // 2. Enum Values logic (Prioritize oneOf if available to avoid duplication)
  // oneOf has explicit 'title' and 'const', making it the primary source for documentation
  if (fieldAny.oneOf && Array.isArray(fieldAny.oneOf)) {
    // Check if oneOf looks like an enum (has const values)
    const hasConstValues = fieldAny.oneOf.some((opt: any) => opt.const !== undefined);

    if (hasConstValues) {
      descParts.push('**Enum Values:**');
      fieldAny.oneOf.forEach((option: any) => {
        if (option.const !== undefined) {
          const val = option.const;
          const label = option.title || val;
          const formattedVal = typeof val === 'string' ? `"${val}"` : val;
          descParts.push(`• ${label} : ${formattedVal}`);
        }
      });
    }
  }
  // Fallback to standard enum if oneOf is not present (or doesn't have const)
  // Support both field.enum and field.items.enum for arrays
  else {
    const fieldEnum = field.enum || fieldAny.items?.enum;
    const enumLabelsByType = fieldAny.enumLabelsByType || fieldAny['x-enum-labels-by-type'];

    if (fieldEnum && fieldEnum.length > 0) {
      // If x-enum-labels-by-type exists, show labels (without type headers)
      if (enumLabelsByType && typeof enumLabelsByType === 'object' && Object.keys(enumLabelsByType).length > 0) {
        descParts.push('**Enum Values by Type:**');
        // Get the first type's labels (all types should have same enum values)
        const firstType = Object.keys(enumLabelsByType)[0];
        const typeLabels = enumLabelsByType[firstType];
        fieldEnum.forEach((val: any) => {
          const label = (typeLabels as Record<string, string>)[String(val)] || val;
          const formattedVal = typeof val === 'string' ? `"${val}"` : val;
          descParts.push(`• ${label} : ${formattedVal}`);
        });
      } else {
        // Fallback to simple enum labels
        descParts.push('**Enum Values:**');
        fieldEnum.forEach((val: any) => {
          const label = fieldAny.enumLabels?.[String(val)] ||
            fieldAny['x-enum-labels']?.[String(val)] ||
            val;
          const formattedVal = typeof val === 'string' ? `"${val}"` : val;
          descParts.push(`• ${label} : ${formattedVal}`);
        });
      }
    } else if (enumLabelsByType && typeof enumLabelsByType === 'object' && Object.keys(enumLabelsByType).length > 0) {
      // x-enum-labels-by-type 단독 (enum 배열 없이) — TYPE별 레이블 직접 렌더링
      descParts.push('**Enum Values by Type:**');
      for (const [type, labels] of Object.entries(enumLabelsByType)) {
        descParts.push(`*${type}:*`);
        Object.entries(labels as Record<string, string>).forEach(([val, label]) => {
          descParts.push(`• ${label} : ${val}`);
        });
      }
    }
  }

  // Note: x-enum-by-type, x-value-constraint, x-node-count-by-type는
  // shared.yaml SSOT에 따라 allOf[].if.then으로 대체되어 제거됨

  // 🔥 6. Conditional Hints from x-optional-when array format
  // 확장된 필드는 개별 힌트만 표시, 원본 필드는 모든 힌트 표시
  const conditionalHint = fieldAny._conditionalHint;
  const optionalWhen = fieldAny['x-optional-when'];

  if (conditionalHint) {
    // 🔥 확장된 필드: 해당 조건의 힌트만 표시
    descParts.push(`**💡 Hint:** ${conditionalHint}`);
  } else if (Array.isArray(optionalWhen)) {
    // 원본 필드 (확장되지 않음): 모든 조건별 힌트 표시
    const hintsWithCondition = optionalWhen
      .filter((item: any) => item.hint && item.condition)
      .map((item: any) => {
        const conditionParts = Object.entries(item.condition)
          .map(([key, val]) => `${key}=${val}`)
          .join(', ');
        return { condition: conditionParts, hint: item.hint };
      });

    if (hintsWithCondition.length > 0) {
      descParts.push('**💡 Value Hints by Type:**');
      for (const { condition, hint } of hintsWithCondition) {
        descParts.push(`• *${condition}:* ${hint}`);
      }
    }
  }

  // 7. Hint (x-ui.hint)
  if (fieldAny.ui?.hint) {
    descParts.push(`💡 ${fieldAny.ui.hint}`);
  }


  return descParts.join('\n');
}
