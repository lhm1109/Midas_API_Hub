/**
 * YAML 기반 테이블 → HTML 변환
 * DynamicTableRenderer와 동일한 YAML 정의를 사용하여 HTML을 생성합니다.
 */

import type { TableDefinition } from './definitionLoader';

export interface TableParameter {
  no: string | number;
  section?: string;
  name: string;
  type: string;
  default: string;
  required: string;
  description: string;
  options?: string[];
  children?: TableParameter[];
}

/**
 * YAML 정의 기반으로 테이블을 HTML로 변환
 */
export function generateHTMLTable(
  parameters: TableParameter[],
  definition: TableDefinition
): string {
  if (!parameters || parameters.length === 0) {
    return '<p>No parameters available</p>';
  }

  const markdownToHtml = (text: string): string => {
    return text
      .replace(/\*\*(.+?)\*\*/g, '<strong>$1</strong>')
      .replace(/\*(.+?)\*/g, '<em>$1</em>')
      .replace(/`(.+?)`/g, '<code>$1</code>')
      .replace(/\n/g, '<br>');
  };

  const columns = definition.tableStructure?.columns || [
    { id: 'no', label: 'No.', width: '6%' },
    { id: 'description', label: 'Description', width: '44%' },
    { id: 'key', label: 'Key', width: '14%' },
    { id: 'type', label: 'Value Type', width: '10%' },
    { id: 'default', label: 'Default', width: '10%' },
    { id: 'required', label: 'Required', width: '10%' }
  ];

  const sectionStyle = definition.sectionHeaders?.style || {};
  const nestedSectionStyle = definition.nestedFields?.nestedSectionHeader?.style || {};

  let tableHTML = `
<div class="table-wrap">
<table style="border-collapse: collapse; width: 100%;" border="1">
<colgroup>`;

  // Column widths
  columns.forEach((col: any) => {
    tableHTML += `\n  <col style="width: ${col.width};">`;
  });

  tableHTML += `
</colgroup>
<tbody>
<tr>`;

  // Table headers
  columns.forEach((col: any) => {
    if (col.id === 'description') {
      tableHTML += `
<th style="padding: 15px 5px 15px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;" colspan="2">${col.label}</th>`;
    } else if (col.id === 'no') {
      tableHTML += `
<th style="padding: 15px 5px 15px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;">${col.label}</th>`;
    } else {
      tableHTML += `
<th style="padding: 15px 5px 15px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;">${col.label}</th>`;
    }
  });

  tableHTML += `
</tr>`;

  // Table rows
  parameters.forEach((param: TableParameter) => {
    // Section header
    if (param.section) {
      const bgColor = sectionStyle.background === 'bg-cyan-950/30' ? '#e6fcff' : '#f0f0f0';
      const textColor = sectionStyle.textColor === 'text-cyan-400' ? '#4c9aff' : '#333';

      tableHTML += `
<tr>
<td style="background-color: ${bgColor}; padding: 10px 5px 10px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;" colspan="${columns.length}">
<p><span style="color: ${textColor};">${param.section}</span></p>
</td>
</tr>`;

      if (!param.name || !param.type) {
        return;
      }
    }

    // Main row
    tableHTML += `
<tr>
<td style="padding: 10px 5px 10px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;">
<p style="text-align: center;">${param.no}</p>
</td>
<td style="padding: 10px 5px 10px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;" colspan="2">
<p>${markdownToHtml(param.description || param.name)}</p>
${param.options ? param.options.map((opt: string) => `<p>${markdownToHtml(opt)}</p>`).join('') : ''}
</td>
<td style="padding: 10px 5px 10px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;">
<p style="text-align: center;">"${param.name}"</p>
</td>
<td style="padding: 10px 5px 10px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;">
<p style="text-align: center;">${param.type}</p>
</td>
<td style="padding: 10px 5px 10px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;">
<p style="text-align: center;">${param.default || '-'}</p>
</td>
<td style="padding: 10px 5px 10px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;">
<p style="text-align: center;">${param.required}</p>
</td>
</tr>`;

    // Child rows
    if (param.children) {
      param.children.forEach((child: TableParameter) => {
        // Child section header
        if (child.section) {
          const bgColor = nestedSectionStyle.background === 'bg-blue-950/30' ? '#e3f2fd' : '#f5f5f5';
          const textColor = nestedSectionStyle.textColor === 'text-blue-400' ? '#1976d2' : '#666';

          tableHTML += `
<tr>
<td style="background-color: ${bgColor}; padding: 10px 5px 10px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;" colspan="${columns.length}">
<p><span style="color: ${textColor};">${child.section}</span></p>
</td>
</tr>`;
          return;
        }

        if (!child.name || !child.type || child.name === '""') {
          return;
        }

        tableHTML += `
<tr>
<td style="padding: 10px 5px 10px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;">
<p style="text-align: center;">${child.no}</p>
</td>
<td style="padding: 10px 5px 10px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;">
<p>${markdownToHtml(child.description || child.name)}</p>
${child.options ? child.options.map((opt: string) => `<p>${markdownToHtml(opt)}</p>`).join('') : ''}
</td>
<td style="padding: 10px 5px 10px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;">
<p style="text-align: center;">"${child.name}"</p>
</td>
<td style="padding: 10px 5px 10px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;">
<p style="text-align: center;">${child.type}</p>
</td>
<td style="padding: 10px 5px 10px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;">
<p style="text-align: center;">${child.default || '-'}</p>
</td>
<td style="padding: 10px 5px 10px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;">
<p style="text-align: center;">${child.required}</p>
</td>
</tr>`;
      });
    }
  });

  tableHTML += `
</tbody>
</table>
</div>`;

  return tableHTML;
}

