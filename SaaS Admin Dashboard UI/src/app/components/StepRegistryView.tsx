import { useState } from 'react';
import React from 'react';
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
  Collapse,
} from '@mantine/core';
import { 
  IconPlus, 
  IconEdit, 
  IconTrash, 
  IconFileImport, 
  IconFileExport,
  IconSearch,
  IconChevronRight,
  IconChevronDown,
  IconGripVertical,
} from '@tabler/icons-react';

// Mock data with hierarchical structure
const mockSteps = [
  { 
    id: 'STEP-001', 
    name: 'Preparation Phase',
    category: 'Phase',
    order: 1,
    activationReason: 'Always',
    children: [
      { id: 'STEP-001-A', name: 'Material Check', category: 'Validation', order: 1, activationReason: 'Material Present' },
      { id: 'STEP-001-B', name: 'Tool Setup', category: 'Setup', order: 2, activationReason: 'Always' },
    ]
  },
  { 
    id: 'STEP-002', 
    name: 'Execution Phase',
    category: 'Phase',
    order: 2,
    activationReason: 'Preparation Complete',
    children: [
      { id: 'STEP-002-A', name: 'Primary Operation', category: 'Operation', order: 1, activationReason: 'Always' },
      { id: 'STEP-002-B', name: 'Quality Check', category: 'Validation', order: 2, activationReason: 'QA Required' },
      { id: 'STEP-002-C', name: 'Secondary Operation', category: 'Operation', order: 3, activationReason: 'Conditional' },
    ]
  },
  { 
    id: 'STEP-003', 
    name: 'Completion Phase',
    category: 'Phase',
    order: 3,
    activationReason: 'Execution Complete',
    children: [
      { id: 'STEP-003-A', name: 'Final Inspection', category: 'Validation', order: 1, activationReason: 'Always' },
      { id: 'STEP-003-B', name: 'Documentation', category: 'Administrative', order: 2, activationReason: 'Always' },
    ]
  },
];

export function StepRegistryView() {
  const [expandedSteps, setExpandedSteps] = useState<Set<string>>(new Set(['STEP-001', 'STEP-002', 'STEP-003']));
  const [searchQuery, setSearchQuery] = useState('');

  const toggleExpand = (stepId: string) => {
    const newExpanded = new Set(expandedSteps);
    if (newExpanded.has(stepId)) {
      newExpanded.delete(stepId);
    } else {
      newExpanded.add(stepId);
    }
    setExpandedSteps(newExpanded);
  };

  const getCategoryColor = (category: string) => {
    const colors: Record<string, string> = {
      'Phase': 'blue',
      'Operation': 'green',
      'Validation': 'orange',
      'Setup': 'violet',
      'Administrative': 'gray',
    };
    return colors[category] || 'gray';
  };

  return (
    <Stack gap="md">
      {/* Header */}
      <Group justify="space-between">
        <div>
          <Text size="xl" fw={600}>Step Registry</Text>
          <Text size="sm" c="dimmed">Manage hierarchical workflow steps and sequences</Text>
        </div>
        <Group gap="xs">
          <Button variant="light" leftSection={<IconFileImport size={16} />} size="sm">
            Import
          </Button>
          <Button variant="light" leftSection={<IconFileExport size={16} />} size="sm">
            Export
          </Button>
          <Button leftSection={<IconPlus size={16} />} size="sm">
            New Step
          </Button>
        </Group>
      </Group>

      {/* Search */}
      <Paper p="md" withBorder>
        <TextInput
          placeholder="Search steps by ID, name, or category..."
          leftSection={<IconSearch size={16} />}
          value={searchQuery}
          onChange={(e) => setSearchQuery(e.currentTarget.value)}
        />
      </Paper>

      {/* Hierarchical Table */}
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
              <Table.Th width={40}></Table.Th>
              <Table.Th width={40}></Table.Th>
              <Table.Th>Step ID</Table.Th>
              <Table.Th>Name</Table.Th>
              <Table.Th>Category</Table.Th>
              <Table.Th width={80}>Order</Table.Th>
              <Table.Th>Activation Reason</Table.Th>
              <Table.Th width={100}>Actions</Table.Th>
            </Table.Tr>
          </Table.Thead>
          <Table.Tbody>
            {mockSteps.map((step) => (
              <React.Fragment key={step.id}>
                {/* Parent Step */}
                <Table.Tr style={{ backgroundColor: 'var(--mantine-color-gray-0)' }}>
                  <Table.Td>
                    <ActionIcon 
                      variant="subtle" 
                      size="sm" 
                      onClick={() => toggleExpand(step.id)}
                      aria-label="Expand/collapse"
                    >
                      {expandedSteps.has(step.id) ? (
                        <IconChevronDown size={16} />
                      ) : (
                        <IconChevronRight size={16} />
                      )}
                    </ActionIcon>
                  </Table.Td>
                  <Table.Td>
                    <ActionIcon variant="subtle" size="sm" style={{ cursor: 'grab' }} aria-label="Drag to reorder">
                      <IconGripVertical size={16} />
                    </ActionIcon>
                  </Table.Td>
                  <Table.Td>
                    <Text size="sm" fw={600}>{step.id}</Text>
                  </Table.Td>
                  <Table.Td>
                    <Text size="sm" fw={500}>{step.name}</Text>
                  </Table.Td>
                  <Table.Td>
                    <Badge variant="light" color={getCategoryColor(step.category)} size="sm">
                      {step.category}
                    </Badge>
                  </Table.Td>
                  <Table.Td>
                    <Text size="sm">{step.order}</Text>
                  </Table.Td>
                  <Table.Td>
                    <Text size="sm">{step.activationReason}</Text>
                  </Table.Td>
                  <Table.Td>
                    <Group gap={4}>
                      <ActionIcon variant="subtle" size="sm" aria-label="Edit">
                        <IconEdit size={16} />
                      </ActionIcon>
                      <ActionIcon variant="subtle" size="sm" color="red" aria-label="Delete">
                        <IconTrash size={16} />
                      </ActionIcon>
                    </Group>
                  </Table.Td>
                </Table.Tr>

                {/* Child Steps */}
                {expandedSteps.has(step.id) && step.children?.map((child) => (
                  <Table.Tr key={child.id} style={{ backgroundColor: 'var(--mantine-color-gray-0)' }}>
                    <Table.Td></Table.Td>
                    <Table.Td>
                      <ActionIcon variant="subtle" size="sm" style={{ cursor: 'grab' }} aria-label="Drag to reorder">
                        <IconGripVertical size={16} />
                      </ActionIcon>
                    </Table.Td>
                    <Table.Td>
                      <Group gap="xs">
                        <Text size="xs" c="dimmed">└─</Text>
                        <Text size="sm">{child.id}</Text>
                      </Group>
                    </Table.Td>
                    <Table.Td>
                      <Text size="sm">{child.name}</Text>
                    </Table.Td>
                    <Table.Td>
                      <Badge variant="light" color={getCategoryColor(child.category)} size="sm">
                        {child.category}
                      </Badge>
                    </Table.Td>
                    <Table.Td>
                      <Text size="sm">{child.order}</Text>
                    </Table.Td>
                    <Table.Td>
                      <Text size="sm">{child.activationReason}</Text>
                    </Table.Td>
                    <Table.Td>
                      <Group gap={4}>
                        <ActionIcon variant="subtle" size="sm" aria-label="Edit">
                          <IconEdit size={16} />
                        </ActionIcon>
                        <ActionIcon variant="subtle" size="sm" color="red" aria-label="Delete">
                          <IconTrash size={16} />
                        </ActionIcon>
                      </Group>
                    </Table.Td>
                  </Table.Tr>
                ))}
              </React.Fragment>
            ))}
          </Table.Tbody>
        </Table>
      </Paper>
    </Stack>
  );
}