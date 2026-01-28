import { useState } from 'react';
import { 
  Stack, 
  Group, 
  Text, 
  Button, 
  Table, 
  Paper,
  Select,
  ActionIcon,
  Tooltip,
} from '@mantine/core';
import { 
  IconPlus, 
  IconFileImport, 
  IconFileExport,
  IconCheck,
  IconX,
  IconReplace,
  IconQuestionMark,
} from '@tabler/icons-react';

// Mock data for rules matrix
const mockSteps = [
  { id: 'STEP-001', name: 'Material Check' },
  { id: 'STEP-002', name: 'Tool Setup' },
  { id: 'STEP-003', name: 'Primary Operation' },
  { id: 'STEP-004', name: 'Quality Check' },
  { id: 'STEP-005', name: 'Secondary Operation' },
  { id: 'STEP-006', name: 'Final Inspection' },
  { id: 'STEP-007', name: 'Documentation' },
];

type RuleType = 'include' | 'exclude' | 'replace' | 'optional' | null;

interface RuleMatrix {
  [stepId: string]: {
    include: boolean;
    exclude: boolean;
    replace: string | null;
    optional: boolean;
  };
}

export function RuleEditorView() {
  const [selectedCode, setSelectedCode] = useState<string | null>('MFG-100');
  const [ruleMatrix, setRuleMatrix] = useState<RuleMatrix>({
    'STEP-001': { include: true, exclude: false, replace: null, optional: false },
    'STEP-002': { include: true, exclude: false, replace: null, optional: false },
    'STEP-003': { include: true, exclude: false, replace: null, optional: false },
    'STEP-004': { include: false, exclude: false, replace: null, optional: true },
    'STEP-005': { include: false, exclude: true, replace: null, optional: false },
    'STEP-006': { include: true, exclude: false, replace: null, optional: false },
    'STEP-007': { include: true, exclude: false, replace: 'STEP-007-ALT', optional: false },
  });

  const toggleRule = (stepId: string, ruleType: 'include' | 'exclude' | 'optional') => {
    setRuleMatrix(prev => ({
      ...prev,
      [stepId]: {
        ...prev[stepId],
        [ruleType]: !prev[stepId]?.[ruleType],
      },
    }));
  };

  const RuleCell = ({ 
    stepId, 
    ruleType, 
    icon: Icon, 
    color, 
    tooltip 
  }: { 
    stepId: string; 
    ruleType: 'include' | 'exclude' | 'optional'; 
    icon: React.ComponentType<any>; 
    color: string;
    tooltip: string;
  }) => {
    const isActive = ruleMatrix[stepId]?.[ruleType];
    return (
      <Table.Td>
        <Group justify="center">
          <Tooltip label={tooltip}>
            <ActionIcon
              variant={isActive ? 'filled' : 'subtle'}
              color={isActive ? color : 'gray'}
              size="sm"
              onClick={() => toggleRule(stepId, ruleType)}
              aria-label={tooltip}
            >
              <Icon size={16} />
            </ActionIcon>
          </Tooltip>
        </Group>
      </Table.Td>
    );
  };

  return (
    <Stack gap="md">
      {/* Header */}
      <Group justify="space-between">
        <div>
          <Text size="xl" fw={600}>Rule Editor</Text>
          <Text size="sm" c="dimmed">Define step inclusion, exclusion, and replacement rules</Text>
        </div>
        <Group gap="xs">
          <Button variant="light" leftSection={<IconFileImport size={16} />} size="sm">
            Import
          </Button>
          <Button variant="light" leftSection={<IconFileExport size={16} />} size="sm">
            Export
          </Button>
          <Button leftSection={<IconPlus size={16} />} size="sm">
            New Rule
          </Button>
        </Group>
      </Group>

      {/* Code Selector */}
      <Paper p="md" withBorder>
        <Group>
          <Text size="sm" fw={500}>Code:</Text>
          <Select
            value={selectedCode}
            onChange={setSelectedCode}
            data={[
              { value: 'MFG-100', label: 'MFG-100 - Standard Manufacturing' },
              { value: 'MFG-101', label: 'MFG-101 - Premium Manufacturing' },
              { value: 'ASM-200', label: 'ASM-200 - Standard Assembly' },
            ]}
            style={{ width: 300 }}
          />
          <Text size="xs" c="dimmed" ml="auto">
            Click icons to toggle rules. Hover for explanation.
          </Text>
        </Group>
      </Paper>

      {/* Rule Matrix */}
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
              <Table.Th>Step ID</Table.Th>
              <Table.Th>Step Name</Table.Th>
              <Table.Th width={100}>
                <Group justify="center">
                  <Tooltip label="Include this step in the workflow">
                    <div>
                      <IconCheck size={16} />
                      <Text size="xs" mt={4}>Include</Text>
                    </div>
                  </Tooltip>
                </Group>
              </Table.Th>
              <Table.Th width={100}>
                <Group justify="center">
                  <Tooltip label="Exclude this step from the workflow">
                    <div>
                      <IconX size={16} />
                      <Text size="xs" mt={4}>Exclude</Text>
                    </div>
                  </Tooltip>
                </Group>
              </Table.Th>
              <Table.Th width={100}>
                <Group justify="center">
                  <Tooltip label="Replace this step with another">
                    <div>
                      <IconReplace size={16} />
                      <Text size="xs" mt={4}>Replace</Text>
                    </div>
                  </Tooltip>
                </Group>
              </Table.Th>
              <Table.Th width={100}>
                <Group justify="center">
                  <Tooltip label="Mark this step as optional">
                    <div>
                      <IconQuestionMark size={16} />
                      <Text size="xs" mt={4}>Optional</Text>
                    </div>
                  </Tooltip>
                </Group>
              </Table.Th>
            </Table.Tr>
          </Table.Thead>
          <Table.Tbody>
            {mockSteps.map((step) => (
              <Table.Tr key={step.id}>
                <Table.Td>
                  <Text size="sm" ff="monospace">{step.id}</Text>
                </Table.Td>
                <Table.Td>
                  <Text size="sm">{step.name}</Text>
                </Table.Td>
                <RuleCell 
                  stepId={step.id} 
                  ruleType="include" 
                  icon={IconCheck} 
                  color="green"
                  tooltip="Include in workflow"
                />
                <RuleCell 
                  stepId={step.id} 
                  ruleType="exclude" 
                  icon={IconX} 
                  color="red"
                  tooltip="Exclude from workflow"
                />
                <Table.Td>
                  <Group justify="center">
                    <Tooltip label={ruleMatrix[step.id]?.replace ? `Replaces with ${ruleMatrix[step.id].replace}` : 'No replacement'}>
                      <ActionIcon
                        variant={ruleMatrix[step.id]?.replace ? 'filled' : 'subtle'}
                        color={ruleMatrix[step.id]?.replace ? 'blue' : 'gray'}
                        size="sm"
                        aria-label="Replace step"
                      >
                        <IconReplace size={16} />
                      </ActionIcon>
                    </Tooltip>
                  </Group>
                </Table.Td>
                <RuleCell 
                  stepId={step.id} 
                  ruleType="optional" 
                  icon={IconQuestionMark} 
                  color="orange"
                  tooltip="Optional step"
                />
              </Table.Tr>
            ))}
          </Table.Tbody>
        </Table>
      </Paper>

      {/* Legend */}
      <Paper p="md" withBorder style={{ backgroundColor: 'var(--mantine-color-gray-0)' }}>
        <Text size="xs" fw={600} mb="xs">Rule Explanation</Text>
        <Group gap="xl">
          <Group gap="xs">
            <ActionIcon variant="filled" color="green" size="sm" aria-label="Include icon">
              <IconCheck size={14} />
            </ActionIcon>
            <Text size="xs">Step is included in workflow</Text>
          </Group>
          <Group gap="xs">
            <ActionIcon variant="filled" color="red" size="sm" aria-label="Exclude icon">
              <IconX size={14} />
            </ActionIcon>
            <Text size="xs">Step is excluded from workflow</Text>
          </Group>
          <Group gap="xs">
            <ActionIcon variant="filled" color="blue" size="sm" aria-label="Replace icon">
              <IconReplace size={14} />
            </ActionIcon>
            <Text size="xs">Step is replaced by another</Text>
          </Group>
          <Group gap="xs">
            <ActionIcon variant="filled" color="orange" size="sm" aria-label="Optional icon">
              <IconQuestionMark size={14} />
            </ActionIcon>
            <Text size="xs">Step is optional</Text>
          </Group>
        </Group>
      </Paper>
    </Stack>
  );
}
