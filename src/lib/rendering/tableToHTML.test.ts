import { describe, expect, it } from 'vitest';

import { generateHTMLTable } from './tableToHTML';

describe('generateHTMLTable', () => {
  it('renders nested oneOf headers and deep child rows for manual HTML export', () => {
    const definition: any = {
      tableStructure: {
        columns: [
          { id: 'no', label: 'No.', width: '10%' },
          { id: 'description', label: 'Description', width: '40%' },
          { id: 'key', label: 'Key', width: '20%' },
          { id: 'type', label: 'Type', width: '10%' },
          { id: 'default', label: 'Default', width: '10%' },
          { id: 'required', label: 'Required', width: '10%' },
        ],
      },
      sectionHeaders: {
        style: {},
      },
      nestedFields: {
        nestedSectionHeader: {
          style: {},
        },
      },
    };

    const html = generateHTMLTable(
      [
        {
          no: 1,
          name: 'ITEMS',
          type: 'array',
          default: '-',
          required: 'Required',
          description: 'Items',
          children: [
            {
              no: '1.1',
              name: 'ELEMS',
              type: 'object',
              default: '-',
              required: 'Optional',
              description: 'Element list',
              children: [
                {
                  no: '',
                  name: '',
                  type: 'one-of-header',
                  title: 'oneOf',
                  default: '',
                  required: '',
                  description: 'Exactly one of KEYS, TO, STRUCTURE_GROUP_NAME must be provided.',
                },
                {
                  no: '1.1.1',
                  name: 'KEYS',
                  type: 'array',
                  default: '-',
                  required: 'Optional',
                  description: 'Specify Each ID',
                },
                {
                  no: '1.1.2',
                  name: 'TO',
                  type: 'string',
                  default: '-',
                  required: 'Optional',
                  description: 'Specify ID Range',
                },
              ],
            },
          ],
        },
      ],
      definition
    );

    expect(html).toContain('Exactly one of KEYS, TO, STRUCTURE_GROUP_NAME must be provided.');
    expect(html).toContain('"KEYS"');
    expect(html).toContain('"TO"');
  });
});
