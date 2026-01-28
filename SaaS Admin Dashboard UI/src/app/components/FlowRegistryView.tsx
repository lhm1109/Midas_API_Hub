import { useState } from 'react';
import { 
  Stack, 
  Group, 
  Text, 
  Button, 
  Table, 
  ActionIcon, 
  Badge,
  Paper,
  TextInput,
  Drawer,
} from '@mantine/core';
import { 
  IconPlus, 
  IconEdit, 
  IconCopy, 
  IconTrash, 
  IconFileImport, 
  IconFileExport,
  IconSearch,
} from '@tabler/icons-react';

// Mock data
const mockFlows = [
  { id: 'FLOW-001', product: 'Product A', featureDomain: 'Manufacturing', context: 'Standard', code: 'MFG-100', variants: 3 },
  { id: 'FLOW-002', product: 'Product A', featureDomain: 'Manufacturing', context: 'Premium', code: 'MFG-101', variants: 2 },
  { id: 'FLOW-003', product: 'Product B', featureDomain: 'Assembly', context: 'Standard', code: 'ASM-200', variants: 5 },
  { id: 'FLOW-004', product: 'Product B', featureDomain: 'Assembly', context: 'Express', code: 'ASM-201', variants: 1 },
  { id: 'FLOW-005', product: 'Product C', featureDomain: 'QA', context: 'Standard', code: 'QA-300', variants: 4 },
];

export function FlowRegistryView() {
  const [selectedFlow, setSelectedFlow] = useState<string | null>(null);
  const [detailOpen, setDetailOpen] = useState(false);
  const [searchQuery, setSearchQuery] = useState('');

  const openDetail = (flowId: string) => {
    setSelectedFlow(flowId);
    setDetailOpen(true);
  };

  const filteredFlows = mockFlows.filter(flow => 
    flow.id.toLowerCase().includes(searchQuery.toLowerCase()) ||
    flow.product.toLowerCase().includes(searchQuery.toLowerCase()) ||
    flow.code.toLowerCase().includes(searchQuery.toLowerCase())
  );

  return (
    <Stack gap="md">
      {/* Header */}
      <Group justify="space-between">
        <div>
          <Text size="xl" fw={600}>Flow Registry</Text>
          <Text size="sm" c="dimmed">Manage workflow definitions and variants</Text>
        </div>
        <Group gap="xs">
          <Button variant="light" leftSection={<IconFileImport size={16} />} size="sm">
            Import
          </Button>
          <Button variant="light" leftSection={<IconFileExport size={16} />} size="sm">
            Export
          </Button>
          <Button leftSection={<IconPlus size={16} />} size="sm">
            New Flow
          </Button>
        </Group>
      </Group>

      {/* Search and Filter */}
      <Paper p="md" withBorder>
        <Group>
          <TextInput
            placeholder="Search flows by ID, product, or code..."
            leftSection={<IconSearch size={16} />}
            value={searchQuery}
            onChange={(e) => setSearchQuery(e.currentTarget.value)}
            style={{ flex: 1 }}
          />
        </Group>
      </Paper>

      {/* Table */}
      <Paper withBorder>
        <Table 
          striped 
          highlightOnHover 
          withTableBorder={false}
          verticalSpacing="xs"
          style={{ fontSize: '14px' }}
        >
          <Table.Thead>
            <Table.Tr style={{ backgroundColor: 'var(--mantine-color-gray-1)' }}>
              <Table.Th>Flow ID</Table.Th>
              <Table.Th>Product</Table.Th>
              <Table.Th>Feature Domain</Table.Th>
              <Table.Th>Context</Table.Th>
              <Table.Th>Code</Table.Th>
              <Table.Th>Variants</Table.Th>
              <Table.Th width={120}>Actions</Table.Th>
            </Table.Tr>
          </Table.Thead>
          <Table.Tbody>
            {filteredFlows.map((flow) => (
              <Table.Tr 
                key={flow.id} 
                style={{ cursor: 'pointer' }}
                onClick={() => openDetail(flow.id)}
              >
                <Table.Td>
                  <Text size="sm" fw={500}>{flow.id}</Text>
                </Table.Td>
                <Table.Td>
                  <Text size="sm">{flow.product}</Text>
                </Table.Td>
                <Table.Td>
                  <Badge variant="light" size="sm">{flow.featureDomain}</Badge>
                </Table.Td>
                <Table.Td>
                  <Text size="sm">{flow.context}</Text>
                </Table.Td>
                <Table.Td>
                  <Text size="sm" ff="monospace">{flow.code}</Text>
                </Table.Td>
                <Table.Td>
                  <Badge variant="outline" size="sm">{flow.variants}</Badge>
                </Table.Td>
                <Table.Td onClick={(e) => e.stopPropagation()}>
                  <Group gap={4}>
                    <ActionIcon variant="subtle" size="sm" aria-label="Edit">
                      <IconEdit size={16} />
                    </ActionIcon>
                    <ActionIcon variant="subtle" size="sm" aria-label="Duplicate">
                      <IconCopy size={16} />
                    </ActionIcon>
                    <ActionIcon variant="subtle" size="sm" color="red" aria-label="Delete">
                      <IconTrash size={16} />
                    </ActionIcon>
                  </Group>
                </Table.Td>
              </Table.Tr>
            ))}
          </Table.Tbody>
        </Table>
      </Paper>

      {/* Detail Drawer */}
      <Drawer
        opened={detailOpen}
        onClose={() => setDetailOpen(false)}
        title={<Text fw={600}>Flow Details</Text>}
        position="right"
        size="lg"
      >
        {selectedFlow && (
          <Stack gap="md">
            <div>
              <Text size="xs" c="dimmed" tt="uppercase" fw={600} mb="xs">Flow ID</Text>
              <Text>{selectedFlow}</Text>
            </div>
            <div>
              <Text size="xs" c="dimmed" tt="uppercase" fw={600} mb="xs">Description</Text>
              <Text>Detailed information about this flow would appear here.</Text>
            </div>
            <div>
              <Text size="xs" c="dimmed" tt="uppercase" fw={600} mb="xs">Steps</Text>
              <Text size="sm" c="dimmed">No steps configured</Text>
            </div>
          </Stack>
        )}
      </Drawer>
    </Stack>
  );
}
