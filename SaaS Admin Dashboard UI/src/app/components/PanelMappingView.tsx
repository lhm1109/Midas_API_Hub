import { useState } from 'react';
import { 
  Stack, 
  Group, 
  Text, 
  Paper,
  Select,
  Accordion,
  Badge,
  Table,
  Tabs,
} from '@mantine/core';
import { IconLayoutGrid, IconCode, IconAdjustments } from '@tabler/icons-react';
import { dataStore, DataRecord } from '@/app/store/dataStore';

export function PanelMappingView() {
  const [selectedPanel, setSelectedPanel] = useState<string>('RcDgn_A');

  // Get all RcDesign records (codes)
  const rcDesignRecords = dataStore.dataRecords.filter(r => r.objectTypeId === 'obj-3');
  
  // Get all RcDesignParameter records
  const rcParamRecords = dataStore.dataRecords.filter(r => r.objectTypeId === 'obj-4');

  // Extract unique panels
  const panels = Array.from(new Set(rcDesignRecords.map(r => r.values['패널'] as string))).sort();

  // Get codes for selected panel
  const codesForPanel = rcDesignRecords.filter(r => r.values['패널'] === selectedPanel);

  // Get parameters for selected panel grouped by section
  const paramsForPanel = rcParamRecords.filter(r => r.values['패널'] === selectedPanel);
  
  // Group parameters by section
  const paramsBySection = paramsForPanel.reduce((acc, param) => {
    const section = param.values['패널 섹션'] as string;
    if (!acc[section]) {
      acc[section] = [];
    }
    acc[section].push(param);
    return acc;
  }, {} as Record<string, DataRecord[]>);

  const sectionColors: Record<string, string> = {
    'Design Input Data': 'blue',
    'Rebar 패널': 'green',
    'Table 패널': 'orange',
    'Perform 패널': 'violet',
    'Design Section 패널': 'cyan',
    'Batch Processing 패널': 'pink',
    'Mesh 패널': 'grape',
  };

  return (
    <Stack gap="lg">
      {/* Header */}
      <Group justify="space-between">
        <div>
          <Text size="xl" fw={600}>Panel Mapping</Text>
          <Text size="sm" c="dimmed">패널별로 지원 코드와 파라미터를 매칭하여 시각화</Text>
        </div>
      </Group>

      {/* Panel Selector */}
      <Paper p="md" withBorder>
        <Group>
          <IconLayoutGrid size={20} />
          <Text size="sm" fw={500}>선택된 패널:</Text>
          <Select
            data={panels}
            value={selectedPanel}
            onChange={(val) => setSelectedPanel(val || 'RcDgn_A')}
            style={{ width: 200 }}
          />
          <Badge variant="light" color="orange">{codesForPanel.length} codes</Badge>
          <Badge variant="light" color="violet">{paramsForPanel.length} parameters</Badge>
        </Group>
      </Paper>

      {/* Main Content - Tabs */}
      <Tabs defaultValue="overview">
        <Tabs.List>
          <Tabs.Tab value="overview" leftSection={<IconLayoutGrid size={16} />}>
            Overview
          </Tabs.Tab>
          <Tabs.Tab value="codes" leftSection={<IconCode size={16} />}>
            Codes ({codesForPanel.length})
          </Tabs.Tab>
          <Tabs.Tab value="parameters" leftSection={<IconAdjustments size={16} />}>
            Parameters ({paramsForPanel.length})
          </Tabs.Tab>
        </Tabs.List>

        {/* Overview Tab */}
        <Tabs.Panel value="overview" pt="md">
          <Stack gap="md">
            {/* Codes Summary */}
            <Paper withBorder p="md">
              <Group justify="space-between" mb="md">
                <Text size="md" fw={600}>Supported Codes</Text>
                <Badge color="orange">{codesForPanel.length}</Badge>
              </Group>
              <Group gap="xs">
                {codesForPanel.map((code, idx) => (
                  <Badge key={idx} variant="light" color="blue" size="lg">
                    {code.values['지원 코드']}
                  </Badge>
                ))}
              </Group>
            </Paper>

            {/* Parameters Summary by Section */}
            <Paper withBorder p="md">
              <Group justify="space-between" mb="md">
                <Text size="md" fw={600}>Parameters by Section</Text>
                <Badge color="violet">{paramsForPanel.length} total</Badge>
              </Group>
              <Stack gap="xs">
                {Object.entries(paramsBySection).map(([section, params]) => (
                  <Group key={section} justify="space-between">
                    <Badge color={sectionColors[section] || 'gray'} variant="light" size="lg">
                      {section}
                    </Badge>
                    <Text size="sm" c="dimmed">{params.length} parameters</Text>
                  </Group>
                ))}
              </Stack>
            </Paper>
          </Stack>
        </Tabs.Panel>

        {/* Codes Tab */}
        <Tabs.Panel value="codes" pt="md">
          <Paper withBorder>
            <Table striped highlightOnHover withTableBorder={false} verticalSpacing="sm">
              <Table.Thead>
                <Table.Tr style={{ backgroundColor: 'var(--mantine-color-gray-1)' }}>
                  <Table.Th>지원 코드</Table.Th>
                  <Table.Th>코드명</Table.Th>
                  <Table.Th>비고</Table.Th>
                </Table.Tr>
              </Table.Thead>
              <Table.Tbody>
                {codesForPanel.map((code, idx) => (
                  <Table.Tr key={idx}>
                    <Table.Td>
                      <Text size="sm" fw={600} ff="monospace">{code.values['지원 코드']}</Text>
                    </Table.Td>
                    <Table.Td>
                      <Text size="sm">{code.values['코드명']}</Text>
                    </Table.Td>
                    <Table.Td>
                      <Text size="sm" c="dimmed">{code.values['비고'] || '-'}</Text>
                    </Table.Td>
                  </Table.Tr>
                ))}
              </Table.Tbody>
            </Table>
          </Paper>
        </Tabs.Panel>

        {/* Parameters Tab */}
        <Tabs.Panel value="parameters" pt="md">
          <Accordion variant="separated">
            {Object.entries(paramsBySection).map(([section, params]) => (
              <Accordion.Item key={section} value={section}>
                <Accordion.Control>
                  <Group>
                    <Badge color={sectionColors[section] || 'gray'} variant="light">
                      {section}
                    </Badge>
                    <Text size="sm" c="dimmed">{params.length} parameters</Text>
                  </Group>
                </Accordion.Control>
                <Accordion.Panel>
                  <Table withTableBorder={false} verticalSpacing="xs" style={{ fontSize: '13px' }}>
                    <Table.Thead>
                      <Table.Tr>
                        <Table.Th>파라미터명</Table.Th>
                        <Table.Th>비고</Table.Th>
                      </Table.Tr>
                    </Table.Thead>
                    <Table.Tbody>
                      {params.map((param, idx) => (
                        <Table.Tr key={idx}>
                          <Table.Td>
                            <Text size="sm">{param.values['파라미터명']}</Text>
                          </Table.Td>
                          <Table.Td>
                            <Text size="xs" c="dimmed">{param.values['비고'] || '-'}</Text>
                          </Table.Td>
                        </Table.Tr>
                      ))}
                    </Table.Tbody>
                  </Table>
                </Accordion.Panel>
              </Accordion.Item>
            ))}
          </Accordion>
        </Tabs.Panel>
      </Tabs>

      {/* Stats */}
      <Paper p="md" withBorder style={{ backgroundColor: 'var(--mantine-color-gray-0)' }}>
        <Text size="sm" fw={600} mb="md">전체 통계</Text>
        <Group gap="xl">
          <div>
            <Text size="xs" c="dimmed">총 패널 수</Text>
            <Text size="lg" fw={600}>{panels.length}</Text>
          </div>
          <div>
            <Text size="xs" c="dimmed">총 코드 수</Text>
            <Text size="lg" fw={600}>{rcDesignRecords.length}</Text>
          </div>
          <div>
            <Text size="xs" c="dimmed">총 파라미터 수</Text>
            <Text size="lg" fw={600}>{rcParamRecords.length}</Text>
          </div>
          <div>
            <Text size="xs" c="dimmed">섹션 종류</Text>
            <Text size="lg" fw={600}>
              {new Set(rcParamRecords.map(r => r.values['패널 섹션'])).size}
            </Text>
          </div>
        </Group>
      </Paper>
    </Stack>
  );
}
