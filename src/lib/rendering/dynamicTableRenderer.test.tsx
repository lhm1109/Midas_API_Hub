import { renderToStaticMarkup } from 'react-dom/server';
import { describe, expect, it } from 'vitest';

import { DynamicTableRenderer } from './dynamicTableRenderer';

describe('DynamicTableRenderer nested rows', () => {
  it('renders deeply nested children beyond the previous 3-level limit', () => {
    const definition: any = {
      styling: {
        table: {},
        header: {},
        body: {},
      },
      tableStructure: {
        columns: [
          { id: 'no', header: 'No.', width: '10%' },
          { id: 'description', header: 'Description', width: '40%' },
          { id: 'key', header: 'Key', width: '20%' },
          { id: 'type', header: 'Type', width: '10%' },
          { id: 'default', header: 'Default', width: '10%' },
          { id: 'required', header: 'Required', width: '10%' },
        ],
      },
      sectionHeaders: {
        colspan: 6,
      },
      rowRendering: {
        required: {
          simpleFormat: true,
        },
      },
      nestedFields: {
        style: {},
        nestedSectionHeader: {
          style: {},
        },
      },
    };

    const parameters = [
      {
        no: 1,
        name: 'RESULT_GRAPHIC',
        type: 'object',
        description: 'Result Graphic',
        default: '-',
        required: 'Required',
        children: [
          {
            no: '1.1',
            name: 'TYPE_OF_DISPLAY',
            type: 'object',
            description: 'Type of Display',
            default: '-',
            required: 'Required',
            children: [
              {
                no: '1.1.1',
                name: 'CODE_CHECKING_RATIO',
                type: 'object',
                description: 'Code Checking Ratio',
                default: '-',
                required: 'Optional',
                children: [
                  {
                    no: '1.1.1.1',
                    name: 'DISPLAY_MEMBERS',
                    type: 'object',
                    description: 'Display Members',
                    default: '-',
                    required: 'Optional',
                    children: [
                      {
                        no: '1.1.1.1.1',
                        name: 'BEAM',
                        type: 'boolean',
                        description: 'Beam',
                        default: 'true',
                        required: 'Optional',
                      },
                      {
                        no: '1.1.1.1.2',
                        name: 'COLUMN',
                        type: 'boolean',
                        description: 'Column',
                        default: 'true',
                        required: 'Optional',
                      },
                      {
                        no: '1.1.1.1.3',
                        name: 'BRACE',
                        type: 'boolean',
                        description: 'Brace',
                        default: 'true',
                        required: 'Optional',
                      },
                    ],
                  },
                ],
              },
            ],
          },
        ],
      },
    ];

    const html = renderToStaticMarkup(
      <DynamicTableRenderer
        definition={definition}
        parameters={parameters}
        expandedParams={new Set([1])}
        toggleParam={() => {}}
      />
    );

    expect(html).toContain('1.1.1.1.1');
    expect(html).toContain('1.1.1.1.2');
    expect(html).toContain('1.1.1.1.3');
    expect(html).toContain('&quot;BEAM&quot;');
    expect(html).toContain('&quot;COLUMN&quot;');
    expect(html).toContain('&quot;BRACE&quot;');
  });

  it('renders anyOf validation headers in the visual table', () => {
    const definition: any = {
      styling: {
        table: {},
        header: {},
        body: {},
      },
      tableStructure: {
        columns: [
          { id: 'no', header: 'No.', width: '10%' },
          { id: 'description', header: 'Description', width: '40%' },
          { id: 'key', header: 'Key', width: '20%' },
          { id: 'type', header: 'Type', width: '10%' },
          { id: 'default', header: 'Default', width: '10%' },
          { id: 'required', header: 'Required', width: '10%' },
        ],
      },
      sectionHeaders: {
        colspan: 6,
      },
      rowRendering: {
        required: {
          simpleFormat: true,
        },
      },
      nestedFields: {
        style: {},
        nestedSectionHeader: {
          style: {},
        },
      },
    };

    const parameters = [
      {
        no: 1,
        name: 'SECTION',
        type: 'object',
        description: 'Section',
        default: '-',
        required: 'Required',
        children: [
          {
            no: '',
            name: '',
            type: 'one-of-header',
            title: 'anyOf',
            default: '',
            required: '',
            description: 'At least one of the following keys must be provided: "BAR_SECTOR_I", "BAR_SECTOR_M", or "BAR_SECTOR_J".',
          },
          {
            no: '1.1',
            name: 'BAR_SECTOR_I',
            type: 'object',
            description: 'I-section rebar configuration',
            default: '-',
            required: 'Optional',
          },
        ],
      },
    ];

    const html = renderToStaticMarkup(
      <DynamicTableRenderer
        definition={definition}
        parameters={parameters}
        expandedParams={new Set([1])}
        toggleParam={() => {}}
      />
    );

    expect(html).toContain('anyOf');
    expect(html).toContain('At least one of the following keys must be provided');
    expect(html).toContain('&quot;BAR_SECTOR_I&quot;');
  });
});
