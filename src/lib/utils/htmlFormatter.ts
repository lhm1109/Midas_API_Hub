/**
 * HTML 포맷팅 유틸리티
 * 범용 HTML/JSON 포맷팅 함수들
 */

/**
 * JSON을 색상 있는 HTML로 변환 (Zendesk 호환 색상 팔레트)
 */
export function formatJsonToHTML(jsonStr: string): string {
  try {
    const jsonObj = JSON.parse(jsonStr);
    const formatted = JSON.stringify(jsonObj, null, 2);

    return formatted
      .split('\n')
      .map(line => {
        let result = line;

        // Key (property names) - 빨간색 #c31b1b
        result = result.replace(/"([^"]+)":/g, '<span style="color: #c31b1b;">"$1"</span>:');

        // String values - 파란색 #055bcc
        result = result.replace(/:\s*"([^"]*)"/g, ': <span style="color: #055bcc;">"$1"</span>');

        // Number values - 초록색 #0ab66c
        result = result.replace(/:\s*(-?\d+\.?\d*)/g, ': <span style="color: #0ab66c;">$1</span>');

        // Boolean values - 파란색 굵게
        result = result.replace(/:\s*(true|false)/g, ': <span style="color: #055bcc; font-weight: bold;">$1</span>');

        // Null values - 파란색 굵게
        result = result.replace(/:\s*(null)/g, ': <span style="color: #055bcc; font-weight: bold;">$1</span>');

        // Brackets and braces - 기본 검은색 (스타일 제거)
        // result = result.replace(/([{}\[\],])/g, '<span style="color: #89ddff;">$1</span>');

        // Replace spaces with &nbsp; for indentation
        const leadingSpaces = line.match(/^(\s*)/)?.[1] || '';
        const indentedSpaces = leadingSpaces.replace(/ /g, '&nbsp;');

        return indentedSpaces + result.substring(leadingSpaces.length);
      })
      .join('<br>');
  } catch (error) {
    console.error('JSON parsing error:', error);
    return jsonStr.replace(/\n/g, '<br>').replace(/ /g, '&nbsp;');
  }
}

/**
 * Markdown을 HTML로 변환
 */
export function markdownToHtml(text: string): string {
  return text
    .replace(/\*\*(.+?)\*\*/g, '<strong>$1</strong>')
    .replace(/\*(.+?)\*/g, '<em>$1</em>')
    .replace(/`(.+?)`/g, '<code>$1</code>')
    .replace(/\n/g, '<br>');
}

/**
 * HTML 이스케이프
 */
export function escapeHtml(text: string): string {
  const map: Record<string, string> = {
    '&': '&amp;',
    '<': '&lt;',
    '>': '&gt;',
    '"': '&quot;',
    "'": '&#039;'
  };
  return text.replace(/[&<>"']/g, (m) => map[m]);
}

