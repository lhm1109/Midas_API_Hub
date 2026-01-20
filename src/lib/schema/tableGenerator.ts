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
  const defaultValue = field.default !== undefined ? escapeHtml(String(field.default)) : '-';
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
      const childDefaultValue = child.default !== undefined ? escapeHtml(String(child.default)) : '-';
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
  
  // Standard enum
  if (field.enum && field.enum.length > 0) {
    parts.push('<strong>Enum Values:</strong>');
    parts.push('<ul>');
    field.enum.forEach((val: any) => {
      const enumLabels = (field as any).enumLabels || (field as any)['x-enum-labels'] || {};
      const label = enumLabels[String(val)] || val;
      parts.push(`<li><code>${escapeHtml(String(val))}</code> - ${escapeHtml(String(label))}</li>`);
    });
    parts.push('</ul>');
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
        parts.push(`<li><code>${escapeHtml(String(val))}</code> - ${escapeHtml(String(label))}</li>`);
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
  
  return `
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>${schema.title} - Enhanced Schema</title>
    ${generateCSSLegacy()}
</head>
<body>
    <h2>Specifications</h2>
    <table>
        ${tableHTML}
    </table>
</body>
</html>
  `.trim();
}

function generateTableHTMLLegacy(sections: SectionGroup[]): string {
  let html = generateTableHeaderLegacy();
  html += '<tbody>';
  
  let rowNumber = 1;
  for (const section of sections) {
    
    // 🔥 조건별로 필드 그룹화
    const fieldsWithoutCondition: EnhancedField[] = [];
    const fieldsByCondition: Map<string, EnhancedField[]> = new Map();
    
    for (const field of section.fields) {
      const visibleWhen = (field as any).ui?.visibleWhen || (field as any)['x-ui']?.visibleWhen;
      
      if (visibleWhen && Object.keys(visibleWhen).length > 0) {
        // 조건 키 생성 (예: "iMETHOD: 1" 또는 "iMETHOD: [2,4]")
        const conditionKey = Object.entries(visibleWhen)
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
    
    // 🔥 조건별 필드 렌더링
    for (const [conditionKey, fields] of fieldsByCondition.entries()) {
      const conditionLabel = `${section.name} (When "${conditionKey.split(':')[0].trim()}" is ${conditionKey.split(':')[1].trim()})`;
      html += generateSectionHeaderLegacy(conditionLabel);
      for (const field of fields) {
      html += generateFieldRowLegacy(field, rowNumber++);
      }
    }
  }
  
  html += '</tbody>';
  return html;
}

function generateTableHeaderLegacy(): string {
  return `
    <thead>
      <tr>
        <th style="width: 6%;">No.</th>
        <th style="width: 35%;">Description</th>
        <th style="width: 14%;">Key</th>
        <th style="width: 10%;">Value Type</th>
        <th style="width: 10%;">Default</th>
        <th style="width: 25%;">Required</th>
      </tr>
    </thead>
  `;
}

function generateSectionHeaderLegacy(sectionName: string): string {
  return `
    <tr>
      <td colspan="6" class="section-header">${escapeHtml(sectionName)}</td>
    </tr>
  `;
}

function generateFieldRowLegacy(field: EnhancedField, rowNumber: number): string {
  const descriptionHTML = generateFieldDescriptionLegacy(field);
  const requiredHTML = generateRequiredCellLegacy(field);
  const defaultValue = field.default !== undefined ? escapeHtml(String(field.default)) : '-';
  const typeDisplay = field.type === 'array' ? `Array[${field.items?.type || 'any'}]` : field.type;
  
  return `
    <tr>
      <td style="text-align: center;">${rowNumber}</td>
      <td>${descriptionHTML}</td>
      <td style="text-align: center;"><code>"${escapeHtml(field.key)}"</code></td>
      <td style="text-align: center;">${typeDisplay}</td>
      <td style="text-align: center;">${defaultValue}</td>
      <td style="min-width: 120px;">${requiredHTML}</td>
    </tr>
  `;
}

function generateFieldDescriptionLegacy(field: EnhancedField): string {
  const parts: string[] = [];
  
  // 🔥 우선순위: x-ui.label > description > key
  const displayLabel = field.ui?.label || field.description || field.key;
  if (displayLabel) {
    parts.push(`<strong>${escapeHtml(displayLabel)}</strong>`);
  }
  
  if (field.enum && field.enum.length > 0) {
    parts.push('<strong>Enum Values:</strong>');
    parts.push('<ul>');
    field.enum.forEach((val: any) => {
      const enumLabels = (field as any).enumLabels || (field as any)['x-enum-labels'] || {};
      const label = enumLabels[String(val)] || val;
      parts.push(`<li><code>${escapeHtml(String(val))}</code> - ${escapeHtml(String(label))}</li>`);
    });
    parts.push('</ul>');
  }
  
  if (field.enumByType) {
    parts.push('<strong>Enum Values by Type:</strong>');
    for (const [type, values] of Object.entries(field.enumByType)) {
      parts.push(`<p><em>${escapeHtml(type)}:</em></p>`);
      parts.push('<ul>');
      (values as any[]).forEach((val: any) => {
        const enumLabelsByType = (field as any).enumLabelsByType || (field as any)['x-enum-labels-by-type'] || {};
        const label = enumLabelsByType?.[type]?.[String(val)] || val;
        parts.push(`<li><code>${escapeHtml(String(val))}</code> - ${escapeHtml(String(label))}</li>`);
      });
      parts.push('</ul>');
    }
  }
  
  if (field.valueConstraint) {
    parts.push('<strong>Value Constraints:</strong>');
    parts.push('<ul>');
    for (const [type, constraint] of Object.entries(field.valueConstraint)) {
      parts.push(`<li><em>${escapeHtml(type)}:</em> ${escapeHtml(String(constraint))}</li>`);
    }
    parts.push('</ul>');
  }
  
  if (field.nodeCountByType) {
    parts.push('<strong>Node Count by Type:</strong>');
    parts.push('<ul>');
    for (const [type, count] of Object.entries(field.nodeCountByType)) {
      const countStr = Array.isArray(count) ? count.join(' or ') : count;
      parts.push(`<li><em>${escapeHtml(type)}:</em> ${countStr} nodes</li>`);
    }
    parts.push('</ul>');
  }
  
  if (field.ui?.hint) {
    parts.push(`<p class="hint">${escapeHtml(field.ui.hint)}</p>`);
  }
  
  return parts.join('\n');
}

function generateRequiredCellLegacy(field: EnhancedField): string {
  // 🔥 field.required가 undefined이거나 빈 객체인 경우 처리
  if (!field.required || Object.keys(field.required).length === 0) {
    return '<span class="badge badge-optional">Optional</span>';
  }
  
  const requiredStatuses = Object.values(field.required);
  const hasRequired = requiredStatuses.some(s => s === 'required');
  const hasOptional = requiredStatuses.some(s => s === 'optional');
  const hasConditional = requiredStatuses.some(s => s === 'conditional');
  
  // 🔥 1. Conditional 상태 처리
  if (hasConditional) {
    return '<span class="badge badge-conditional">Conditional</span>';
  }
  
  // 🔥 2. Required/Optional 혼재 (타입별로 다름)
  if (hasRequired && hasOptional) {
    const grouped: Record<string, string[]> = { required: [], optional: [] };
    for (const [type, status] of Object.entries(field.required)) {
      if (status === 'required') grouped.required.push(type);
      if (status === 'optional') grouped.optional.push(type);
    }
    
    const parts: string[] = [];
    if (grouped.required.length > 0) {
      parts.push(`<p style="margin: 2px 0;"><strong class="required">Required:</strong> ${grouped.required.join(', ')}</p>`);
    }
    if (grouped.optional.length > 0) {
      parts.push(`<p style="margin: 2px 0;"><strong class="optional">Optional:</strong> ${grouped.optional.join(', ')}</p>`);
    }
    return parts.join('\n');
  } 
  
  // 🔥 3. Required
  if (hasRequired) {
    return '<span class="badge badge-required">Required</span>';
  } 
  
  // 🔥 4. Optional (기본값)
    return '<span class="badge badge-optional">Optional</span>';
}

function generateCSSLegacy(): string {
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

function generateInfoSectionLegacy(schema: EnhancedSchema): string {
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

function generateValidationArchitectureSectionLegacy(): string {
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

function generateTransportSectionLegacy(schema: EnhancedSchema): string {
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
