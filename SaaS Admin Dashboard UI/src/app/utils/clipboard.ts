/**
 * Parse clipboard data from Excel/Sheets
 * Returns 2D array of cells
 */
export function parseClipboardData(text: string): string[][] {
  const rows = text.split(/\r?\n/).filter(row => row.trim() !== '');
  return rows.map(row => row.split('\t'));
}

/**
 * Handle paste event for table
 */
export function handleTablePaste(
  event: ClipboardEvent,
  startRowIndex: number,
  startColIndex: number,
  updateCell: (rowIndex: number, colIndex: number, value: string) => void,
  maxRows?: number,
  maxCols?: number
) {
  event.preventDefault();
  
  const text = event.clipboardData?.getData('text/plain');
  if (!text) return;

  const data = parseClipboardData(text);
  
  data.forEach((row, rowOffset) => {
    const targetRow = startRowIndex + rowOffset;
    if (maxRows !== undefined && targetRow >= maxRows) return;

    row.forEach((cell, colOffset) => {
      const targetCol = startColIndex + colOffset;
      if (maxCols !== undefined && targetCol >= maxCols) return;

      updateCell(targetRow, targetCol, cell);
    });
  });
}
