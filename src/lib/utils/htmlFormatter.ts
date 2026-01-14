/**
 * HTML 포맷팅 유틸리티
 * 범용 HTML/JSON 포맷팅 함수들
 */

/**
 * JSON을 색상 있는 HTML로 변환
 */
export function formatJsonToHTML(jsonStr: string): string {
  try {
    const jsonObj = JSON.parse(jsonStr);
    const formatted = JSON.stringify(jsonObj, null, 2);
    
    return formatted
      .split('\n')
      .map(line => {
        let result = line;
        
        // Key (property names)
        result = result.replace(/"([^"]+)":/g, '<span style="color: #c792ea;">"$1"</span>:');
        
        // String values
        result = result.replace(/:\s*"([^"]*)"/g, ': <span style="color: #c3e88d;">"$1"</span>');
        
        // Number values
        result = result.replace(/:\s*(-?\d+\.?\d*)/g, ': <span style="color: #f78c6c;">$1</span>');
        
        // Boolean values
        result = result.replace(/:\s*(true|false)/g, ': <span style="color: #ff5370;">$1</span>');
        
        // Null values
        result = result.replace(/:\s*(null)/g, ': <span style="color: #82aaff;">$1</span>');
        
        // Brackets and braces
        result = result.replace(/([{}\[\],])/g, '<span style="color: #89ddff;">$1</span>');
        
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

