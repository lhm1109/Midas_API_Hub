import type { ApiEndpoint } from '@/types';
import type { IndexTableRow } from './manualHubStorage';

export const MH_INDEX_TABLE_START = '<!-- MH_INDEX_TABLE_START -->';
export const MH_INDEX_TABLE_END = '<!-- MH_INDEX_TABLE_END -->';

export const INDEX_TABLE_TD_STYLE =
  'padding: 10px 5px 10px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;';
export const INDEX_TABLE_TH_STYLE =
  'padding: 15px 5px 15px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;';

function escapeAttr(s: string): string {
  return s
    .replace(/&/g, '&amp;')
    .replace(/"/g, '&quot;')
    .replace(/</g, '&lt;')
    .replace(/>/g, '&gt;');
}

function escapeText(s: string): string {
  return escapeAttr(s);
}

export function buildIndexTableDataRowHtml(input: {
  numberLabel: string;
  pathText: string;
  detailLabel: string;
  detailHref: string;
}): string {
  const { numberLabel, pathText, detailLabel, detailHref } = input;
  const linkInner = detailHref.trim()
    ? `<a style="text-indent: 10px;" href="${escapeAttr(detailHref.trim())}" target="_blank" rel="noopener noreferrer">${escapeText(detailLabel.trim())} ↗</a>`
    : escapeText(detailLabel.trim());

  return `
        <tr>
          <td style="${INDEX_TABLE_TD_STYLE}">
            <p style="text-align: center;">${escapeText(numberLabel)}</p>
          </td>
          <td style="${INDEX_TABLE_TD_STYLE}">
            <p style="text-indent: 10px;">${escapeText(pathText)}</p>
          </td>
          <td style="${INDEX_TABLE_TD_STYLE}">
            <p>${linkInner}</p>
          </td>
        </tr>`;
}

/**
 * main_kr_link.html 과 동일한 colgroup/헤더 스타일의 테이블 마크업 (tbody 데이터 행만 관리).
 */
export function buildManagedIndexTableHtml(
  rows: IndexTableRow[],
  endpointById: Map<string, ApiEndpoint>
): string {
  const headerRow = `
        <tr>
          <th style="${INDEX_TABLE_TH_STYLE}">No.</th>
          <th style="${INDEX_TABLE_TH_STYLE}">End Point</th>
          <th style="${INDEX_TABLE_TH_STYLE}">Details</th>
        </tr>`;

  const bodyRows = rows
    .map((row, idx) => {
      const ep = endpointById.get(row.endpointId);
      const pathText = ep?.path ?? row.endpointId;
      const label = row.detailLabel.trim() || ep?.name || 'Link';
      const href = row.detailHref.trim();
      return buildIndexTableDataRowHtml({
        numberLabel: String(idx + 1),
        pathText,
        detailLabel: label,
        detailHref: href,
      });
    })
    .join('');

  return `
<zd-html-block>
  <div class="table-wrap">
    <table style="border-collapse: collapse; width: 100%; margin-left: 0px; margin-right: auto;" border="1">
      <colgroup>
        <col style="width: 6.00%;">
        <col style="width: 24.00%;">
        <col style="width: 70.00%;">
      </colgroup>
      <tbody>
${headerRow}
${bodyRows}
      </tbody>
    </table>
  </div>
</zd-html-block>`;
}

function escapeRegex(s: string): string {
  return s.replace(/[.*+?^${}()|[\]\\]/g, '\\$&');
}

/**
 * 마커가 있으면 교체, 없으면 </body> 앞 또는 문서 끝에 삽입.
 */
export function syncManagedIndexTableIntoHtml(html: string, fragment: string): string {
  const block = `\n${MH_INDEX_TABLE_START}\n${fragment}\n${MH_INDEX_TABLE_END}\n`;
  if (html.includes(MH_INDEX_TABLE_START) && html.includes(MH_INDEX_TABLE_END)) {
    return html.replace(
      new RegExp(`${escapeRegex(MH_INDEX_TABLE_START)}[\\s\\S]*?${escapeRegex(MH_INDEX_TABLE_END)}`),
      block.trim()
    );
  }
  if (/<\/body>/i.test(html)) {
    return html.replace(/<\/body>/i, `${block}</body>`);
  }
  return `${html}${block}`;
}
