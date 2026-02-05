/**
 * Schema to HTML Table Generator (YAML-based)
 * 
 * JSON Schema로부터 HTML 문서 테이블을 자동 생성합니다.
 * YAML 정의 파일을 사용하여 HTML 템플릿, CSS, 구조를 동적으로 생성합니다.
 * 
 * @see api_docs_zendesk/schema_to_table_algorithm.md
 * @see schema_definitions/{psdSet}/{schemaType}/html-template.yaml
 */

import {
  compileEnhancedSchema,
  type EnhancedSchema,
  type EnhancedField,
  type SectionGroup
} from './schemaCompiler';
import { loadCachedDefinition, type HTMLTemplateDefinition } from '../rendering/definitionLoader';

// ============================================================================
// HTML Generation (YAML-based)
// ============================================================================

/**
 * Schema로부터 완전한 HTML 문서 생성 (YAML 기반)
 */
export async function generateHTMLDocumentWithYAML(
  schema: EnhancedSchema,
  psdSet: string = 'civil_gen_definition',
  schemaType: string = 'enhanced'
): Promise<string> {
  const template = await loadCachedDefinition(schemaType as 'original' | 'enhanced', 'html') as HTMLTemplateDefinition;
  const sections = compileEnhancedSchema(schema, psdSet, schemaType);
  const tableHTML = generateTableHTML(sections, template);

  return `
<!DOCTYPE html>
<html lang="${template.document?.language || 'en'}">
<head>
    <meta charset="${template.document?.charset || 'UTF-8'}">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>${schema.title} - ${template.document?.titleSuffix || 'Enhanced Schema'}</title>
    ${generateCSSFromYAML(template)}
</head>
<body>
    <h1>${schema.title} ${template.document?.h1Suffix || 'API - Enhanced Schema Specification'}</h1>
    
    ${generateInfoSection(schema, template)}
    ${generateValidationArchitectureSection(template)}
    
    <h2>${template.table?.sectionTitle || 'Specifications'}</h2>
    <table>
        ${tableHTML}
    </table>
    
    ${generateTransportSection(schema, template)}
</body>
</html>
  `.trim();
}

/**
 * 테이블 HTML 생성 (YAML 기반)
 */
function generateTableHTML(sections: SectionGroup[], template: HTMLTemplateDefinition): string {
  let html = generateTableHeader(template);
  html += '<tbody>';

  let rowNumber = 1;
  for (const section of sections) {
    html += generateSectionHeader(section.name, template);

    for (const field of section.fields) {
      html += generateFieldRow(field, rowNumber++, template);
    }
  }

  html += '</tbody>';
  return html;
}

/**
 * 테이블 헤더 생성 (YAML 기반)
 */
function generateTableHeader(template: HTMLTemplateDefinition): string {
  const columns = template.table?.columns || [
    { id: 'no', label: 'No.', width: '6%' },
    { id: 'description', label: 'Description', width: '35%' },
    { id: 'key', label: 'Key', width: '14%' },
    { id: 'type', label: 'Value Type', width: '10%' },
    { id: 'default', label: 'Default', width: '10%' },
    { id: 'required', label: 'Required', width: '25%' }
  ];

  const headerRows = columns.map((col: any) =>
    `<th style="width: ${col.width};">${col.label}</th>`
  ).join('\n        ');

  return `
    <thead>
      <tr>
        ${headerRows}
      </tr>
    </thead>
  `;
}

/**
 * Default 값 포맷팅 (문자열은 따옴표로 감싸서 API에서 사용할 형태로 표시)
 */
function formatDefaultValue(value: any, type: string): string {
  if (value === undefined || value === null) return '-';

  // 문자열 타입이면 따옴표로 감싸기
  if (type === 'string') {
    return `<code>"${escapeHtml(String(value))}"</code>`;
  }
  // boolean과 number는 코드 스타일로 표시
  if (type === 'boolean' || type === 'number' || type === 'integer') {
    return `<code>${escapeHtml(String(value))}</code>`;
  }
  // 그 외
  return escapeHtml(String(value));
}

/**
 * 섹션 헤더 행 생성 (YAML 기반)
 */
function generateSectionHeader(sectionName: string, template: HTMLTemplateDefinition): string {
  const colspan = template.table?.columns?.length || 6;
  const className = template.table?.sectionHeaderClass || 'section-header';

  return `
    <tr>
      <td colspan="${colspan}" class="${className}">${escapeHtml(sectionName)}</td>
    </tr>
  `;
}

/**
 * 필드 행 생성 (YAML 기반, 중첩 객체 지원)
 */
function generateFieldRow(field: EnhancedField, rowNumber: number, template: HTMLTemplateDefinition): string {
  const descriptionHTML = generateFieldDescription(field, template);
  const requiredHTML = generateRequiredCell(field, template);
  const defaultValue = formatDefaultValue(field.default, field.type);
  const typeDisplay = field.type === 'array' ? `Array[${field.items?.type || 'any'}]` : field.type;

  let html = `
    <tr>
      <td style="text-align: center;">${rowNumber}</td>
      <td>${descriptionHTML}</td>
      <td style="text-align: center;"><code>"${escapeHtml(field.key)}"</code></td>
      <td style="text-align: center;">${typeDisplay}</td>
      <td style="text-align: center;">${defaultValue}</td>
      <td>${requiredHTML}</td>
    </tr>
  `;

  // 🔥 중첩 필드 처리 (children이 있으면 하위 행 추가)
  if (field.children && field.children.length > 0) {
    let childNo = 1;
    for (const child of field.children) {
      const childDescriptionHTML = generateFieldDescription(child, template);
      const childRequiredHTML = generateRequiredCell(child, template);
      const childDefaultValue = formatDefaultValue(child.default, child.type);
      const childTypeDisplay = child.type === 'array' ? `Array[${child.items?.type || 'any'}]` : child.type;

      // 중첩 필드의 key에서 부모 prefix 제거 (UNIT.FORCE → FORCE)
      const childKeyDisplay = child.key.includes('.') ? child.key.split('.').pop() : child.key;

      html += `
        <tr style="background-color: rgba(255, 255, 255, 0.02);">
          <td style="text-align: center; padding-left: 2em;">(${childNo++})</td>
          <td style="padding-left: 2em;">${childDescriptionHTML}</td>
          <td style="text-align: center;"><code>"${escapeHtml(childKeyDisplay || '')}"</code></td>
          <td style="text-align: center;">${childTypeDisplay}</td>
          <td style="text-align: center;">${childDefaultValue}</td>
          <td>${childRequiredHTML}</td>
        </tr>
      `;
    }
  }

  return html;
}

/**
 * 필드 설명 생성 (YAML 기반)
 */
function generateFieldDescription(field: EnhancedField, _template: HTMLTemplateDefinition): string {
  const parts: string[] = [];

  // Label
  if (field.ui?.label) {
    parts.push(`<strong>${escapeHtml(field.ui.label)}</strong>`);
  }

  // Standard enum (supports both field.enum and field.items.enum for arrays)
  const fieldEnum = field.enum || ((field as any).items?.enum);

  // 🔍 DEBUG: COMPONENTS 필드 추적
  if (field.key === 'COMPONENTS') {
    console.log('🔍 COMPONENTS field debug:', {
      hasFieldEnum: !!field.enum,
      hasItemsEnum: !!((field as any).items?.enum),
      fieldEnum: fieldEnum,
      enumLabelsByType: (field as any)['x-enum-labels-by-type'],
      enumLabels: (field as any).enumLabels,
      items: (field as any).items,
    });
  }

  if (fieldEnum && fieldEnum.length > 0) {
    // Check both x-enum-labels-by-type and enumLabels (camelCase version)
    const enumLabelsByType = (field as any).enumLabelsByType || (field as any)['x-enum-labels-by-type'] || (field as any).enumLabels;
    const enumLabelsSimple = (field as any).enumLabels || (field as any)['x-enum-labels'] || {};

    // If x-enum-labels-by-type exists, show labels grouped by type
    // If x-enum-labels-by-type exists, show labels (without type headers)
    if (enumLabelsByType && Object.keys(enumLabelsByType).length > 0) {
      parts.push('<strong>Enum Values by Type:</strong>');
      parts.push('<ul>');
      // Get the first type's labels (all types should have same enum values)
      const firstType = Object.keys(enumLabelsByType)[0];
      const typeLabels = enumLabelsByType[firstType];
      fieldEnum.forEach((val: any) => {
        const label = (typeLabels as Record<string, string>)[String(val)] || val;
        parts.push(`<li>${escapeHtml(String(label))} : <code>"${escapeHtml(String(val))}"</code></li>`);
      });
      parts.push('</ul>');
    } else {
      // Fallback to x-enum-labels
      parts.push('<strong>Enum Values:</strong>');
      parts.push('<ul>');
      fieldEnum.forEach((val: any) => {
        const label = enumLabelsSimple[String(val)] || val;
        // Format: Label : "value" (shows what to actually use in API)
        parts.push(`<li>${escapeHtml(String(label))} : <code>"${escapeHtml(String(val))}"</code></li>`);
      });
      parts.push('</ul>');
    }
  }



  // Enum by type
  if (field.enumByType) {
    parts.push('<strong>Enum Values by Type:</strong>');
    for (const [type, values] of Object.entries(field.enumByType)) {
      parts.push(`<p><em>${escapeHtml(type)}:</em></p>`);
      parts.push('<ul>');
      (values as any[]).forEach((val: any) => {
        const enumLabelsByType = (field as any).enumLabelsByType || (field as any)['x-enum-labels-by-type'] || {};
        const label = enumLabelsByType?.[type]?.[String(val)] || val;
        // Format: Label : "value" (shows what to actually use in API)
        parts.push(`<li>${escapeHtml(String(label))} : <code>"${escapeHtml(String(val))}"</code></li>`);
      });
      parts.push('</ul>');
    }
  }

  // Value constraints
  if (field.valueConstraint) {
    parts.push('<strong>Value Constraints:</strong>');
    parts.push('<ul>');
    for (const [type, constraint] of Object.entries(field.valueConstraint)) {
      parts.push(`<li><em>${escapeHtml(type)}:</em> ${escapeHtml(String(constraint))}</li>`);
    }
    parts.push('</ul>');
  }

  // Node count by type
  if (field.nodeCountByType) {
    parts.push('<strong>Node Count by Type:</strong>');
    parts.push('<ul>');
    for (const [type, count] of Object.entries(field.nodeCountByType)) {
      const countStr = Array.isArray(count) ? count.join(' or ') : count;
      parts.push(`<li><em>${escapeHtml(type)}:</em> ${countStr} nodes</li>`);
    }
    parts.push('</ul>');
  }

  // Hint
  if (field.ui?.hint) {
    parts.push(`<p class="hint">${escapeHtml(field.ui.hint)}</p>`);
  }

  return parts.join('\n');
}

/**
 * Required 셀 생성 (YAML 기반)
 */
function generateRequiredCell(field: EnhancedField, _template: HTMLTemplateDefinition): string {
  const requiredStatuses = Object.values(field.required);
  const hasRequired = requiredStatuses.some(s => s === 'required');
  const hasOptional = requiredStatuses.some(s => s === 'optional');

  if (hasRequired && hasOptional) {
    // Mixed: show detail
    const grouped: Record<string, string[]> = { required: [], optional: [] };
    for (const [type, status] of Object.entries(field.required)) {
      if (status === 'required') grouped.required.push(type);
      if (status === 'optional') grouped.optional.push(type);
    }

    const parts: string[] = [];
    if (grouped.required.length > 0) {
      parts.push(`<p><strong class="required">Required:</strong> ${grouped.required.join(', ')}</p>`);
    }
    if (grouped.optional.length > 0) {
      parts.push(`<p><strong class="optional">Optional:</strong> ${grouped.optional.join(', ')}</p>`);
    }
    return parts.join('\n');
  } else if (hasRequired) {
    return '<span class="badge badge-required">Required</span>';
  } else {
    return '<span class="badge badge-optional">Optional</span>';
  }
}

/**
 * CSS 생성 (YAML 기반)
 */
function generateCSSFromYAML(template: HTMLTemplateDefinition): string {
  const css = template.css || {};

  return `
    <style>
      /* Base styles */
      body {
        font-family: ${css.fontFamily || '-apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif'};
        line-height: ${css.lineHeight || '1.6'};
        max-width: ${css.maxWidth || '1200px'};
        margin: ${css.margin || '0 auto'};
        padding: ${css.padding || '20px'};
        background-color: ${css.backgroundColor || '#ffffff'};
        color: ${css.textColor || '#333333'};
      }
      
      /* Table styles */
      table {
        width: 100%;
        border-collapse: collapse;
        margin: 20px 0;
        box-shadow: 0 2px 4px rgba(0,0,0,0.1);
      }
      
      th, td {
        padding: ${css.tableCellPadding || '12px'};
        text-align: left;
        border: ${css.tableBorder || '1px solid #ddd'};
      }
      
      thead th {
        background-color: ${css.tableHeaderBg || '#f5f5f5'};
        font-weight: ${css.tableHeaderWeight || 'bold'};
        color: ${css.tableHeaderColor || '#333'};
      }
      
      tbody tr:nth-child(even) {
        background-color: ${css.tableRowEvenBg || '#f9f9f9'};
      }
      
      tbody tr:hover {
        background-color: ${css.tableRowHoverBg || '#f0f0f0'};
      }
      
      /* Section header */
      .section-header {
        background-color: ${css.sectionHeaderBg || '#e3f2fd'};
        color: ${css.sectionHeaderColor || '#1976d2'};
        font-weight: ${css.sectionHeaderWeight || 'bold'};
        font-size: ${css.sectionHeaderSize || '0.9em'};
        text-transform: ${css.sectionHeaderTransform || 'uppercase'};
        letter-spacing: ${css.sectionHeaderSpacing || '0.5px'};
      }
      
      /* Badges */
      .badge {
        display: inline-block;
        padding: ${css.badgePadding || '4px 8px'};
        border-radius: ${css.badgeRadius || '4px'};
        font-size: ${css.badgeSize || '0.85em'};
        font-weight: ${css.badgeWeight || 'bold'};
      }
      
      .badge-required {
        background-color: ${css.badgeRequiredBg || '#ffebee'};
        color: ${css.badgeRequiredColor || '#c62828'};
      }
      
      .badge-optional {
        background-color: ${css.badgeOptionalBg || '#f5f5f5'};
        color: ${css.badgeOptionalColor || '#757575'};
      }
      
      .badge-conditional {
        background-color: ${css.badgeConditionalBg || '#fff9c4'};
        color: ${css.badgeConditionalColor || '#f57f17'};
      }
      
      /* Code */
      code {
        background-color: ${css.codeBg || '#f5f5f5'};
        padding: ${css.codePadding || '2px 4px'};
        border-radius: ${css.codeRadius || '3px'};
        font-family: ${css.codeFont || '"Courier New", monospace'};
        font-size: ${css.codeSize || '0.9em'};
      }
      
      /* Hint */
      .hint {
        color: ${css.hintColor || '#757575'};
        font-size: ${css.hintSize || '0.9em'};
        font-style: ${css.hintStyle || 'italic'};
        margin-top: ${css.hintMargin || '4px'};
      }
      
      /* Feature boxes */
      .feature-box {
        background-color: ${css.featureBoxBg || '#f5f5f5'};
        border-left: ${css.featureBoxBorder || '4px solid #2196f3'};
        padding: ${css.featureBoxPadding || '15px'};
        margin: ${css.featureBoxMargin || '15px 0'};
      }
      
      .feature-box h3 {
        margin-top: 0;
        color: ${css.featureBoxTitleColor || '#2196f3'};
      }
    </style>
  `;
}

/**
 * Info Section 생성 (YAML 기반)
 */
function generateInfoSection(schema: EnhancedSchema, template: HTMLTemplateDefinition): string {
  const transport = (schema as any)['x-transport'];
  if (!transport) return '';

  const featureBox = template.featureBoxes?.info || {};

  return `
    <div class="feature-box">
      <h3>${featureBox.title || '📋 API Information'}</h3>
      <p><strong>URI:</strong> <code>${escapeHtml(transport.uri || '')}</code></p>
      <p><strong>Methods:</strong> ${(transport.methods || []).map((m: string) => `<code>${m}</code>`).join(', ')}</p>
      ${transport['body-root'] ? `<p><strong>Body Root:</strong> <code>${escapeHtml(transport['body-root'])}</code></p>` : ''}
    </div>
  `;
}

/**
 * Validation Architecture Section 생성 (YAML 기반)
 */
function generateValidationArchitectureSection(template: HTMLTemplateDefinition): string {
  const featureBox = template.featureBoxes?.validation || {};

  return `
    <div class="feature-box">
      <h3>${featureBox.title || '🏗️ Validation Architecture'}</h3>
      <p>${featureBox.description || 'This schema uses a multi-layered validation approach:'}</p>
      <ul>
        ${(featureBox.items || [
      'Type-specific required fields (e.g., BEAM requires SECT, SOLID does not)',
      'Dynamic enum values based on TYPE selection',
      'Conditional field visibility using visibleWhen rules',
      'Value constraints that vary by element type'
    ]).map((item: string) => `<li>${escapeHtml(item)}</li>`).join('\n        ')}
      </ul>
    </div>
  `;
}

/**
 * Transport Section 생성 (YAML 기반)
 */
function generateTransportSection(schema: EnhancedSchema, template: HTMLTemplateDefinition): string {
  const transport = (schema as any)['x-transport'];
  if (!transport || !transport['body-root']) return '';

  const featureBox = template.featureBoxes?.transport || {};

  return `
    <div class="feature-box">
      <h3>${featureBox.title || '🚀 Transport Layer'}</h3>
      <p>${featureBox.description || 'The actual HTTP request body structure differs from the schema definition:'}</p>
      <pre><code>{
  "${escapeHtml(transport['body-root'])}": {
    "1": { /* Schema fields here */ },
    "2": { /* Schema fields here */ }
  }
}</code></pre>
      <p>${featureBox.note || 'The schema defines the structure of each instance, while the transport layer wraps multiple instances.'}</p>
    </div>
  `;
}

/**
 * HTML Escape
 */
function escapeHtml(text: string): string {
  const map: Record<string, string> = {
    '&': '&amp;',
    '<': '&lt;',
    '>': '&gt;',
    '"': '&quot;',
    "'": '&#039;'
  };
  return text.replace(/[&<>"']/g, (m) => map[m]);
}

// ============================================================================
// Legacy Sync Function (for backward compatibility)
// ============================================================================

/**
 * Schema로부터 완전한 HTML 문서 생성 (동기 버전, 하드코딩)
 * @deprecated Use generateHTMLDocumentWithYAML instead
 */
export function generateHTMLDocument(
  schema: EnhancedSchema,
  psdSet: string = 'civil_gen_definition',
  schemaType: string = 'enhanced'
): string {
  const sections = compileEnhancedSchema(schema, psdSet, schemaType);
  const tableHTML = generateTableHTMLLegacy(sections);

  // 🔥 Wrapper key (Assign/Argument) 정보 추출 - properties에서 실제 wrapper key 찾기
  const wrapperKey = getWrapperKey(schema);
  const wrapperDescription = getWrapperDescription(schema);

  // Zendesk 호환 테이블 (inline 스타일) - 두 개의 테이블
  return `
    <!-- 🔥 Table 1: Keyed Object Entry (Map Key Description) -->
    <h3 id="h_keyed_object_entry">
      <strong>Keyed Object Entry</strong>
    </h3>
    <div class="table-wrap">
      <table style="border-collapse: collapse; width: 100%;" border="1">
        <colgroup>
          <col style="width: 6.00%;">
          <col style="width: 6.00%;">
          <col style="width: 44.00%;">
          <col style="width: 14.00%;">
          <col style="width: 10.00%;">
          <col style="width: 10.00%;">
          <col style="width: 10.00%;">
        </colgroup>
        <tbody>
          <tr>
            <th style="${ZENDESK_HEADER_STYLE}">No.</th>
            <th style="${ZENDESK_HEADER_STYLE}" colspan="2">Description</th>
            <th style="${ZENDESK_HEADER_STYLE}">Key</th>
            <th style="${ZENDESK_HEADER_STYLE}">Value Type</th>
            <th style="${ZENDESK_HEADER_STYLE}">Default</th>
            <th style="${ZENDESK_HEADER_STYLE}">Required</th>
          </tr>
          <tr>
            <td style="background-color: #e6fcff; ${ZENDESK_CELL_STYLE}" colspan="7">
              <p><span style="color: #4c9aff;">Root Object</span></p>
            </td>
          </tr>
          <tr>
            <td style="${ZENDESK_CELL_STYLE}">
              <p style="text-align: center;">1</p>
            </td>
            <td style="${ZENDESK_CELL_STYLE}" colspan="2">
              <p>${wrapperDescription}</p>
            </td>
            <td style="${ZENDESK_CELL_STYLE}">
              <p style="text-align: center;">"${escapeHtml(wrapperKey)}"</p>
            </td>
            <td style="${ZENDESK_CELL_STYLE}">
              <p style="text-align: center;">object</p>
            </td>
            <td style="${ZENDESK_CELL_STYLE}">
              <p style="text-align: center;">-</p>
            </td>
            <td style="${ZENDESK_CELL_STYLE}">
              <p style="text-align: center;">Required</p>
            </td>
          </tr>
        </tbody>
      </table>
    </div>

    <!-- 🔥 Table 2: Item (Value Object Schema) -->
    <h3 id="h_specifications">
      <strong>Item (Value Object Schema)</strong>
    </h3>
    <div class="table-wrap">
      <table style="border-collapse: collapse; width: 100%;" border="1">
        <colgroup>
          <col style="width: 6.00%;">
          <col style="width: 6.00%;">
          <col style="width: 44.00%;">
          <col style="width: 14.00%;">
          <col style="width: 10.00%;">
          <col style="width: 10.00%;">
          <col style="width: 10.00%;">
        </colgroup>
        ${tableHTML}
      </table>
    </div>
  `.trim();
}

/**
 * Wrapper key 추출 (schema에서 Assign/Argument key 찾기)
 */
function getWrapperKey(schema: EnhancedSchema): string {
  const properties = (schema as any).properties;
  if (!properties) return 'Assign';

  // Assign 또는 Argument wrapper 찾기
  const wrapperKeys = ['Assign', 'Argument'];
  for (const key of wrapperKeys) {
    if (properties[key]) {
      return key;
    }
  }

  return 'Assign';
}

/**
 * Wrapper description 추출 (schema에서 Assign/Argument의 설명 가져오기)
 */
function getWrapperDescription(schema: EnhancedSchema): string {
  const properties = (schema as any).properties;
  if (!properties) return 'Map of keyed objects.';

  // Assign 또는 Argument wrapper 찾기
  const wrapperKeys = ['Assign', 'Argument'];
  for (const key of wrapperKeys) {
    if (properties[key]?.description) {
      return escapeHtml(properties[key].description);
    }
  }

  return 'Map of keyed objects where each key is a string identifier.';
}

function generateTableHTMLLegacy(sections: SectionGroup[]): string {
  // Zendesk 스타일: <tbody> 안에 헤더 행 포함
  let html = '<tbody>\n';
  html += generateTableHeaderLegacy();

  let rowNumber = 1;
  for (const section of sections) {

    // 🔥 조건별로 필드 그룹화
    const fieldsWithoutCondition: EnhancedField[] = [];
    const fieldsByCondition: Map<string, EnhancedField[]> = new Map();

    for (const field of section.fields) {
      // 🔥 조건 소스: x-required-when + x-optional-when (ui.visibleWhen은 사용하지 않음)
      const fieldAny = field as any;
      const requiredWhen = fieldAny['x-required-when'];
      const optionalWhen = fieldAny['x-optional-when'];

      // 조건 중 하나라도 있으면 조건부 필드로 처리
      const condition = requiredWhen || optionalWhen;

      if (condition && typeof condition === 'object' && Object.keys(condition).length > 0) {
        // 조건 키 생성 (예: "TYPE: BEAM,TRUSS" 또는 "iMETHOD: [2,4]")
        const conditionKey = Object.entries(condition)
          .map(([k, v]) => `${k}: ${Array.isArray(v) ? v.join(',') : v}`)
          .join(', ');

        if (!fieldsByCondition.has(conditionKey)) {
          fieldsByCondition.set(conditionKey, []);
        }
        fieldsByCondition.get(conditionKey)!.push(field);
      } else {
        fieldsWithoutCondition.push(field);
      }
    }

    // 🔥 조건 없는 필드 먼저 렌더링
    if (fieldsWithoutCondition.length > 0) {
      html += generateSectionHeaderLegacy(section.name);
      for (const field of fieldsWithoutCondition) {
        html += generateFieldRowLegacy(field, rowNumber++);
      }
    }

    // 🔥 조건별 필드 렌더링 - "Advanced" 그룹으로 표시
    for (const [conditionKey, fields] of fieldsByCondition.entries()) {
      const conditionLabel = `Advanced (When "${conditionKey.split(':')[0].trim()}" is ${conditionKey.split(':')[1].trim()})`;
      html += generateSectionHeaderLegacy(conditionLabel);
      for (const field of fields) {
        html += generateFieldRowLegacy(field, rowNumber++);
      }
    }
  }

  html += '</tbody>';
  return html;
}

// Zendesk 호환 테이블 스타일 (기존 Zendesk 문서와 동일한 형태)
// 헤더: 배경색은 CSS에서 처리 (th { background-color: #3498db; color: white; })
const ZENDESK_HEADER_STYLE = 'padding: 15px 5px 15px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;';
// 일반 셀: 배경색은 CSS에서 처리 (tr:nth-child(even) { background-color: #f2f2f2; })
const ZENDESK_CELL_STYLE = 'padding: 10px 5px 10px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;';

function generateTableHeaderLegacy(): string {
  return `
    <tr>
      <th style="${ZENDESK_HEADER_STYLE}">No.</th>
      <th style="${ZENDESK_HEADER_STYLE}" colspan="2">Description</th>
      <th style="${ZENDESK_HEADER_STYLE}">Key</th>
      <th style="${ZENDESK_HEADER_STYLE}">Value Type</th>
      <th style="${ZENDESK_HEADER_STYLE}">Default</th>
      <th style="${ZENDESK_HEADER_STYLE}">Required</th>
    </tr>
  `;
}

function generateSectionHeaderLegacy(sectionName: string): string {
  // Zendesk 스타일: inline 배경색 + 파란 텍스트
  // 전체 7개 열 병합 (No. ~ Required 까지 전체)
  return `
    <tr>
      <td style="background-color: #e6fcff; ${ZENDESK_CELL_STYLE}" colspan="7">
        <p><span style="color: #4c9aff;">${escapeHtml(sectionName)}</span></p>
      </td>
    </tr>
  `;
}

function generateFieldRowLegacy(field: EnhancedField, rowNumber: number): string {
  const descriptionHTML = generateFieldDescriptionLegacy(field);
  const requiredHTML = generateRequiredCellLegacy(field);
  const defaultValue = formatDefaultValue(field.default, field.type);
  const typeDisplay = field.type === 'array' ? `Array [${field.items?.type || 'any'}]` : field.type;

  // 🔥 Zendesk 스타일: children이 있으면 No. 칼럼에 rowspan 적용
  const hasChildren = field.children && field.children.length > 0;
  const rowspanValue = hasChildren ? field.children!.length + 1 : 1;
  const rowspanAttr = hasChildren ? ` rowspan="${rowspanValue}"` : '';

  // Zendesk 스타일: inline 패딩 + <p> 태그 + text-align: center
  let html = `
    <tr>
      <td style="${ZENDESK_CELL_STYLE}"${rowspanAttr}>
        <p style="text-align: center;">${rowNumber}</p>
      </td>
      <td style="${ZENDESK_CELL_STYLE}" colspan="2">
        ${descriptionHTML}
      </td>
      <td style="${ZENDESK_CELL_STYLE}">
        <p style="text-align: center;">"${escapeHtml(field.key)}"</p>
      </td>
      <td style="${ZENDESK_CELL_STYLE}">
        <p style="text-align: center;">${typeDisplay}</p>
      </td>
      <td style="${ZENDESK_CELL_STYLE}">
        <p style="text-align: center;">${defaultValue}</p>
      </td>
      <td style="${ZENDESK_CELL_STYLE}">
        <p style="text-align: center;">${requiredHTML}</p>
      </td>
    </tr>
  `;

  // 🔥 Zendesk 스타일: 중첩 필드는 No. 칼럼 없이, Description이 두 칼럼으로 분리 (인덱스 + 내용)
  if (hasChildren) {
    let childNo = 1;
    for (const child of field.children!) {
      const childDescriptionHTML = generateFieldDescriptionLegacy(child);
      const childRequiredHTML = generateRequiredCellLegacy(child);
      const childDefaultValue = formatDefaultValue(child.default, child.type);
      const childTypeDisplay = child.type === 'array' ? `Array [${child.items?.type || 'any'}]` : child.type;

      // 중첩 필드의 key에서 부모 prefix 제거 (UNIT.FORCE → FORCE)
      const childKeyDisplay = child.key.includes('.') ? child.key.split('.').pop() : child.key;

      // 🔥 Zendesk 스타일: child row는 No. 칼럼 없음 (rowspan으로 parent가 점유)
      // Description이 두 개의 td로 분리: (인덱스) + (설명)
      html += `
        <tr>
          <td style="${ZENDESK_CELL_STYLE}">
            <p style="text-align: center;">(${childNo++})</p>
          </td>
          <td style="${ZENDESK_CELL_STYLE}">
            ${childDescriptionHTML}
          </td>
          <td style="${ZENDESK_CELL_STYLE}">
            <p style="text-align: center;">"${escapeHtml(childKeyDisplay || '')}"</p>
          </td>
          <td style="${ZENDESK_CELL_STYLE}">
            <p style="text-align: center;">${childTypeDisplay}</p>
          </td>
          <td style="${ZENDESK_CELL_STYLE}">
            <p style="text-align: center;">${childDefaultValue}</p>
          </td>
          <td style="${ZENDESK_CELL_STYLE}">
            <p style="text-align: center;">${childRequiredHTML}</p>
          </td>
        </tr>
      `;
    }
  }

  return html;
}

function generateFieldDescriptionLegacy(field: EnhancedField): string {
  const parts: string[] = [];

  // 🔥 우선순위: x-ui.label > description > key
  const displayLabel = field.ui?.label || field.description || field.key;
  if (displayLabel) {
    parts.push(`<p>${escapeHtml(displayLabel)}</p>`);
  }

  // Zendesk 스타일: enum 값은 <p> • value 형식
  // Support both field.enum and field.items.enum for arrays
  const fieldEnum = field.enum || ((field as any).items?.enum);

  // 🔍 DEBUG: COMPONENTS 필드 추적 (Legacy)
  if (field.key === 'COMPONENTS') {
    console.log('🔍 COMPONENTS field debug (Legacy):', {
      hasFieldEnum: !!field.enum,
      hasItemsEnum: !!((field as any).items?.enum),
      fieldEnum: fieldEnum,
      enumLabelsByType: (field as any)['x-enum-labels-by-type'],
      enumLabels: (field as any).enumLabels,
      items: (field as any).items,
    });
  }

  if (fieldEnum && fieldEnum.length > 0) {
    // Check both x-enum-labels-by-type and enumLabels (camelCase version)
    const enumLabelsByType = (field as any).enumLabelsByType || (field as any)['x-enum-labels-by-type'] || (field as any).enumLabels;
    const enumLabelsSimple = (field as any).enumLabels || (field as any)['x-enum-labels'] || {};

    // If x-enum-labels-by-type exists, show labels (without type headers)
    if (enumLabelsByType && Object.keys(enumLabelsByType).length > 0) {
      // Get the first type's labels (all types should have same enum values)
      const firstType = Object.keys(enumLabelsByType)[0];
      const typeLabels = enumLabelsByType[firstType];
      fieldEnum.forEach((val: any) => {
        const label = (typeLabels as Record<string, string>)[String(val)] || val;
        parts.push(`<p> • ${escapeHtml(String(label))}: "${escapeHtml(String(val))}"</p>`);
      });
    } else {
      // Fallback to x-enum-labels
      fieldEnum.forEach((val: any) => {
        const label = enumLabelsSimple[String(val)] || val;
        parts.push(`<p> • ${escapeHtml(String(label))}: "${escapeHtml(String(val))}"</p>`);
      });
    }
  }

  if (field.enumByType) {
    for (const [type, values] of Object.entries(field.enumByType)) {
      parts.push(`<p><em>${escapeHtml(type)}:</em></p>`);
      (values as any[]).forEach((val: any) => {
        const enumLabelsByType = (field as any).enumLabelsByType || (field as any)['x-enum-labels-by-type'] || {};
        const label = enumLabelsByType?.[type]?.[String(val)] || val;
        parts.push(`<p> • ${escapeHtml(String(label))}: "${escapeHtml(String(val))}"</p>`);
      });
    }
  }

  if (field.valueConstraint) {
    parts.push(`<p><strong>Value Constraints:</strong></p>`);
    for (const [type, constraint] of Object.entries(field.valueConstraint)) {
      parts.push(`<p> • ${escapeHtml(type)}: ${escapeHtml(String(constraint))}</p>`);
    }
  }

  if (field.nodeCountByType) {
    parts.push(`<p><strong>Node Count by Type:</strong></p>`);
    for (const [type, count] of Object.entries(field.nodeCountByType)) {
      const countStr = Array.isArray(count) ? count.join(' or ') : count;
      parts.push(`<p> • ${escapeHtml(type)}: ${countStr} nodes</p>`);
    }
  }

  // 🔥 Conditional Hints from x-optional-when array format
  // 확장된 필드는 개별 힌트만 표시, 원본 필드는 모든 힌트 표시
  const fieldAny = field as any;
  const conditionalHint = fieldAny._conditionalHint;
  const optionalWhen = fieldAny['x-optional-when'];

  if (conditionalHint) {
    // 🔥 확장된 필드: 해당 조건의 힌트만 표시
    parts.push(`<p><strong>💡 Hint:</strong> ${escapeHtml(String(conditionalHint))}</p>`);
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
      parts.push(`<p><strong>💡 Value Hints by Type:</strong></p>`);
      for (const { condition, hint } of hintsWithCondition) {
        parts.push(`<p> • <em>${escapeHtml(condition)}:</em> ${escapeHtml(String(hint))}</p>`);
      }
    }
  }

  if (field.ui?.hint) {
    parts.push(`<p style="color: #757575; font-style: italic;">${escapeHtml(field.ui.hint)}</p>`);
  }

  return parts.join('\n');
}

function generateRequiredCellLegacy(field: EnhancedField): string {
  // 🔥 field.required가 undefined이거나 빈 객체인 경우 처리
  if (!field.required || Object.keys(field.required).length === 0) {
    return 'Optional';
  }

  const requiredStatuses = Object.values(field.required);
  const hasRequired = requiredStatuses.some(s => s === 'required');
  const hasOptional = requiredStatuses.some(s => s === 'optional');
  const hasConditional = requiredStatuses.some(s => s === 'conditional');

  // 🔥 1. Conditional 상태 처리
  if (hasConditional) {
    return 'Conditional';
  }

  // 🔥 2. Required/Optional 혼재 (타입별로 다름) - Zendesk 스타일로 간소화
  if (hasRequired && hasOptional) {
    const grouped: Record<string, string[]> = { required: [], optional: [] };
    for (const [type, status] of Object.entries(field.required)) {
      if (status === 'required') grouped.required.push(type);
      if (status === 'optional') grouped.optional.push(type);
    }

    const parts: string[] = [];
    if (grouped.required.length > 0) {
      parts.push(`Required: ${grouped.required.join(', ')}`);
    }
    if (grouped.optional.length > 0) {
      parts.push(`Optional: ${grouped.optional.join(', ')}`);
    }
    return parts.join('<br>');
  }

  // 🔥 3. Required
  if (hasRequired) {
    return 'Required';
  }

  // 🔥 4. Optional (기본값)
  return 'Optional';
}

function _generateCSSLegacy(): string {
  return `
    <style>
      body {
        font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif;
        line-height: 1.6;
        max-width: 1200px;
        margin: 0 auto;
        padding: 20px;
        background-color: #ffffff;
        color: #333333;
      }
      
      table {
        width: 100%;
        border-collapse: collapse;
        margin: 20px 0;
        box-shadow: 0 2px 4px rgba(0,0,0,0.1);
      }
      
      th, td {
        padding: 12px;
        text-align: left;
        border: 1px solid #ddd;
      }
      
      thead th {
        background-color: #f5f5f5;
        font-weight: bold;
        color: #333;
      }
      
      thead th:last-child,
      tbody td:last-child {
        min-width: 150px;
      }
      
      tbody tr:nth-child(even) {
        background-color: #f9f9f9;
      }
      
      tbody tr:hover {
        background-color: #f0f0f0;
      }
      
      .section-header {
        background-color: #e3f2fd;
        color: #1976d2;
        font-weight: bold;
        font-size: 0.9em;
        text-transform: uppercase;
        letter-spacing: 0.5px;
      }
      
      .badge {
        display: inline-block;
        padding: 4px 8px;
        border-radius: 4px;
        font-size: 0.85em;
        font-weight: bold;
      }
      
      .badge-required {
        background-color: #ffebee;
        color: #c62828;
      }
      
      .badge-optional {
        background-color: #f5f5f5;
        color: #757575;
      }
      
      .badge-conditional {
        background-color: #fff9c4;
        color: #f57f17;
      }
      
      code {
        background-color: #f5f5f5;
        padding: 2px 4px;
        border-radius: 3px;
        font-family: "Courier New", monospace;
        font-size: 0.9em;
      }
      
      .hint {
        color: #757575;
        font-size: 0.9em;
        font-style: italic;
        margin-top: 4px;
      }
      
      .feature-box {
        background-color: #f5f5f5;
        border-left: 4px solid #2196f3;
        padding: 15px;
        margin: 15px 0;
      }
      
      .feature-box h3 {
        margin-top: 0;
        color: #2196f3;
      }
    </style>
  `;
}

function _generateInfoSectionLegacy(schema: EnhancedSchema): string {
  const transport = (schema as any)['x-transport'];
  if (!transport) return '';

  return `
    <div class="feature-box">
      <h3>📋 API Information</h3>
      <p><strong>URI:</strong> <code>${escapeHtml(transport.uri || '')}</code></p>
      <p><strong>Methods:</strong> ${(transport.methods || []).map((m: string) => `<code>${m}</code>`).join(', ')}</p>
      ${transport['body-root'] ? `<p><strong>Body Root:</strong> <code>${escapeHtml(transport['body-root'])}</code></p>` : ''}
    </div>
  `;
}

function _generateValidationArchitectureSectionLegacy(): string {
  return `
    <div class="feature-box">
      <h3>🏗️ Validation Architecture</h3>
      <p>This schema uses a multi-layered validation approach:</p>
      <ul>
        <li>Type-specific required fields (e.g., BEAM requires SECT, SOLID does not)</li>
        <li>Dynamic enum values based on TYPE selection</li>
        <li>Conditional field visibility using visibleWhen rules</li>
        <li>Value constraints that vary by element type</li>
      </ul>
    </div>
  `;
}

function _generateTransportSectionLegacy(schema: EnhancedSchema): string {
  const transport = (schema as any)['x-transport'];
  if (!transport || !transport['body-root']) return '';

  return `
    <div class="feature-box">
      <h3>🚀 Transport Layer</h3>
      <p>The actual HTTP request body structure differs from the schema definition:</p>
      <pre><code>{
  "${escapeHtml(transport['body-root'])}": {
    "1": { /* Schema fields here */ },
    "2": { /* Schema fields here */ }
  }
}</code></pre>
      <p>The schema defines the structure of each instance, while the transport layer wraps multiple instances.</p>
    </div>
  `;
}

// ============================================================================
// Zendesk Compatible Table Generator (Inline Styles)
// ============================================================================

export interface ZendeskTableRow {
  no: string | number;
  description: string;
  key: string;
  valueType: string;
  defaultValue: string;
  required: string;
  isHeader?: boolean;       // 섹션 헤더 여부
  isSubRow?: boolean;       // 하위 행 여부 (들여쓰기)
  rowspan?: number;         // rowspan 값
  colspan?: number;         // colspan 값
  bgColor?: string;         // 배경색
}

/**
 * Zendesk 호환 Specifications 테이블 HTML 생성
 * 기존 Zendesk 문서 스타일 (007_Import_to_Json.html) 참고
 */
export function generateZendeskTable(rows: ZendeskTableRow[]): string {
  // 테이블 헤더
  const headerHTML = `
    <tr>
      <th style="padding: 15px 5px 15px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;">No.</th>
      <th style="padding: 15px 5px 15px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;" colspan="2">Description</th>
      <th style="padding: 15px 5px 15px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;">Key</th>
      <th style="padding: 15px 5px 15px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;">Value Type</th>
      <th style="padding: 15px 5px 15px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;">Default</th>
      <th style="padding: 15px 5px 15px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;">Required</th>
    </tr>`;

  // 테이블 행 생성
  const rowsHTML = rows.map((row) => {
    const cellStyle = 'padding: 10px 5px 10px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;';
    const bgStyle = row.bgColor ? ` background-color: ${row.bgColor};` : '';
    const rowspanAttr = row.rowspan ? ` rowspan="${row.rowspan}"` : '';

    if (row.isSubRow) {
      // 하위 행 (들여쓰기)
      return `
    <tr>
      <td style="${cellStyle}">
        <p style="text-align: center;">${escapeHtml(String(row.no))}</p>
      </td>
      <td style="${cellStyle}" colspan="2">
        <p>${row.description}</p>
      </td>
      <td style="${cellStyle}">
        <p style="text-align: center;">${escapeHtml(row.key)}</p>
      </td>
      <td style="${cellStyle}">
        <p style="text-align: center;">${escapeHtml(row.valueType)}</p>
      </td>
      <td style="${cellStyle}">
        <p style="text-align: center;">${escapeHtml(row.defaultValue)}</p>
      </td>
      <td style="${cellStyle}">
        <p style="text-align: center;">${escapeHtml(row.required)}</p>
      </td>
    </tr>`;
    }

    if (row.bgColor) {
      // 섹션 구분 행 (배경색 있음)
      return `
    <tr>
      <td style="${cellStyle}${bgStyle}" colspan="7">
        <p><span style="color: #4c9aff;">${row.description}</span></p>
      </td>
    </tr>`;
    }

    // 일반 행
    return `
    <tr>
      <td style="${cellStyle}"${rowspanAttr}>
        <p style="text-align: center;">${escapeHtml(String(row.no))}</p>
      </td>
      <td style="${cellStyle}" colspan="2">
        <p>${row.description}</p>
      </td>
      <td style="${cellStyle}">
        <p style="text-align: center;">${escapeHtml(row.key)}</p>
      </td>
      <td style="${cellStyle}">
        <p style="text-align: center;">${escapeHtml(row.valueType)}</p>
      </td>
      <td style="${cellStyle}">
        <p style="text-align: center;">${escapeHtml(row.defaultValue)}</p>
      </td>
      <td style="${cellStyle}">
        <p style="text-align: center;">${escapeHtml(row.required)}</p>
      </td>
    </tr>`;
  }).join('\n');

  return `
    <h3 id="h_specifications">
      <strong>Specifications</strong>
    </h3>
    <div class="table-wrap">
      <table style="border-collapse: collapse; width: 100%; margin-left: 0px; margin-right: auto;" border="1">
        <colgroup>
          <col style="width: 6.00%;">
          <col style="width: 6.00%;">
          <col style="width: 44.00%;">
          <col style="width: 14.00%;">
          <col style="width: 10.00%;">
          <col style="width: 10.00%;">
          <col style="width: 10.00%;">
        </colgroup>
        <tbody>
          ${headerHTML}
          ${rowsHTML}
        </tbody>
      </table>
    </div>`;
}

/**
 * EnhancedSchema에서 Zendesk 테이블 행 데이터 추출
 */
export function schemaToZendeskTableRows(
  schema: EnhancedSchema,
  psdSet: string = 'civil_gen_definition',
  schemaType: string = 'enhanced'
): ZendeskTableRow[] {
  const sections = compileEnhancedSchema(schema, psdSet, schemaType);
  const rows: ZendeskTableRow[] = [];
  let rowNumber = 1;

  for (const section of sections) {
    for (const field of section.fields) {
      // Description 생성 (enum 값 포함)
      let description = field.ui?.label || field.description || field.key;

      // Support both field.enum and field.items.enum for arrays
      const fieldEnum = field.enum || ((field as any).items?.enum);
      if (fieldEnum && fieldEnum.length > 0) {
        // Check both x-enum-labels-by-type and enumLabels (camelCase version)
        const enumLabelsByType = (field as any).enumLabelsByType || (field as any)['x-enum-labels-by-type'] || (field as any).enumLabels;

        if (enumLabelsByType && typeof enumLabelsByType === 'object' && Object.keys(enumLabelsByType).length > 0) {
          // Render without type headers
          const firstType = Object.keys(enumLabelsByType)[0];
          const typeLabels = enumLabelsByType[firstType];
          const enumList = fieldEnum.map((val: any) => {
            const label = (typeLabels as Record<string, string>)[String(val)] || val;
            return `&nbsp;•&nbsp;${label}: "${val}"`;
          }).join('<br>');
          description += `<br>${enumList}`;
        } else {
          // Fallback to simple labels
          const enumLabels = (field as any).enumLabels || (field as any)['x-enum-labels'] || {};
          const enumList = fieldEnum.map((val: any) => {
            const label = enumLabels[String(val)] || val;
            return `&nbsp;•&nbsp;${label}: "${val}"`;
          }).join('<br>');
          description += `<br>${enumList}`;
        }
      }

      rows.push({
        no: rowNumber++,
        description,
        key: `"${field.key}"`,
        valueType: field.type === 'array' ? `Array[${field.items?.type || 'any'}]` : field.type,
        defaultValue: field.default !== undefined ? String(field.default) : '-',
        required: Object.values(field.required || {}).some(s => s === 'required') ? 'Required' : 'Optional'
      });

      // 중첩 필드 처리
      if (field.children && field.children.length > 0) {
        let childNo = 1;
        for (const child of field.children) {
          let childDescription = child.ui?.label || child.description || child.key;

          // Support both child.enum and child.items.enum for arrays
          const childEnum = child.enum || ((child as any).items?.enum);
          if (childEnum && childEnum.length > 0) {
            const enumLabelsByType = (child as any).enumLabelsByType || (child as any)['x-enum-labels-by-type'] || (child as any).enumLabels;

            if (enumLabelsByType && typeof enumLabelsByType === 'object' && Object.keys(enumLabelsByType).length > 0) {
              // Render without type headers
              const firstType = Object.keys(enumLabelsByType)[0];
              const typeLabels = enumLabelsByType[firstType];
              const enumList = childEnum.map((val: any) => {
                const label = (typeLabels as Record<string, string>)[String(val)] || val;
                return `&nbsp;•&nbsp;${label}: "${val}"`;
              }).join('<br>');
              childDescription += `<br>${enumList}`;
            } else {
              const enumLabels = (child as any).enumLabels || (child as any)['x-enum-labels'] || {};
              const enumList = childEnum.map((val: any) => {
                const label = enumLabels[String(val)] || val;
                return `&nbsp;•&nbsp;${label}: "${val}"`;
              }).join('<br>');
              childDescription += `<br>${enumList}`;
            }
          }

          const childKeyDisplay = child.key.includes('.') ? child.key.split('.').pop() : child.key;

          rows.push({
            no: `(${childNo++})`,
            description: childDescription,
            key: `"${childKeyDisplay}"`,
            valueType: child.type === 'array' ? `Array[${child.items?.type || 'any'}]` : child.type,
            defaultValue: child.default !== undefined ? String(child.default) : '-',
            required: Object.values(child.required || {}).some(s => s === 'required') ? 'Required' : 'Optional',
            isSubRow: true
          });
        }
      }
    }
  }

  return rows;
}

