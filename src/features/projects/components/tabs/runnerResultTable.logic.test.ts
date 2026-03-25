import { describe, expect, it } from 'vitest';
import { parseRunnerJsonTable, parseRunnerResultTable } from './runnerResultTable.logic';

describe('parseRunnerResultTable', () => {
  it('parses top-level HEAD/vDATA responses without hardcoded columns', () => {
    const input = {
      FORCE: 'KN',
      DIST: 'M',
      HEAD: ['CHK', 'MEMB', 'COM', 'Pu'],
      vDATA: [
        ['OK', '885', '0.055', '0.00000'],
        ['NG', '1065', '0.265', '-2016.8'],
      ],
    };

    const table = parseRunnerResultTable(input);

    expect(table).not.toBeNull();
    expect(table?.meta).toEqual([
      { key: 'FORCE', value: 'KN' },
      { key: 'DIST', value: 'M' },
    ]);
    expect(table?.columns.map((column) => column.header)).toEqual(['CHK', 'MEMB', 'COM', 'Pu']);
    expect(table?.rows).toHaveLength(2);
    expect(table?.rows[1].values[3]).toBe('-2016.8');
    expect(table?.columns[1].align).toBe('right');
    expect(table?.columns[0].align).toBe('center');
  });

  it('finds nested table payloads recursively and pads missing values', () => {
    const input = {
      result: {
        payload: {
          TABLE_NAME: 'Steel Result',
          HEAD: ['A', 'B', 'C'],
          vDATA: [
            ['1', '2'],
          ],
        },
      },
    };

    const table = parseRunnerResultTable(input);

    expect(table?.title).toBe('Steel Result');
    expect(table?.path).toBe('$.result.payload');
    expect(table?.rows[0].values).toEqual(['1', '2', '']);
  });

  it('parses nested HEAD/DATA responses (Steel Code Check style)', () => {
    const input = {
      'Result Table': {
        FORCE: 'KN',
        DIST: 'M',
        HEAD: [
          'CHK', 'MEMB', 'COM', 'SECT', 'SHR', 'Section',
          'Material', 'Fy', 'LCB', 'Len', 'Lb', 'Ly', 'Lz', 'Cb',
          'Ky', 'Kz', 'B1y', 'B1z', 'B2y', 'B2z', 'RatPc',
          'Pu', 'pPn', 'Muy', 'pMny', 'Muz', 'pMnz',
          'Vuy', 'pVny', 'Vuz', 'pVnz', 'Tu', 'pTn', 'Def', 'Defa',
        ],
        DATA: [
          [
            'OK', '888.000', '0.000', '1.000', '0.000', '400x600',
            'SM355', '305000.000', '10.000', '3.250', '3.250', '3.250', '3.250', '1.000',
            '1.000', '1.000', '1.000', '1.000', '1.000', '1.000', '0.000',
            '0.000', '54900.000', '0.000', '4941.000', '0.000', '3294.000',
            '0.000', '37332.000', '0.000', '37332.000', '-', '-', '-', '-',
          ],
        ],
      },
    };

    const table = parseRunnerResultTable(input);

    expect(table).not.toBeNull();
    expect(table?.columns.map((c) => c.header)).toContain('CHK');
    expect(table?.columns.map((c) => c.header)).toContain('MEMB');
    expect(table?.rows).toHaveLength(1);
    expect(table?.rows[0].values[0]).toBe('OK');
    expect(table?.rows[0].values[1]).toBe('888.000');
    expect(table?.meta).toEqual(expect.arrayContaining([
      { key: 'FORCE', value: 'KN' },
      { key: 'DIST', value: 'M' },
    ]));
  });

  it('returns null for non-table responses', () => {
    expect(parseRunnerResultTable('{"ok":true}')).toBeNull();
    expect(parseRunnerResultTable('not-json')).toBeNull();
  });

  it('creates a generic request table from arbitrary json', () => {
    const table = parseRunnerJsonTable({
      Argument: {
        PRI_SORT: 1,
        RESULT: 0,
        COMPONENTS: ['CHK', 'MEMB'],
      },
    }, {
      title: 'Request Body',
    });

    expect(table).not.toBeNull();
    expect(table?.title).toBe('Request Body');
    expect(table?.columns.map((column) => column.header)).toEqual(['Path', 'Type', 'Value']);
    expect(table?.rows.some((row) => row.values[0] === 'Argument.PRI_SORT' && row.values[2] === '1')).toBe(true);
    expect(table?.rows.some((row) => row.values[0] === 'Argument.COMPONENTS[1]' && row.values[2] === 'MEMB')).toBe(true);
  });
});
