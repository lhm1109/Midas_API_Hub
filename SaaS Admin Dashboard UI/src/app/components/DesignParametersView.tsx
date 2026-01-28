import { useState } from 'react';
import { 
  Stack, 
  Group, 
  Text, 
  Button, 
  Table, 
  Paper,
  Select,
  Textarea,
  ActionIcon,
  Checkbox,
  Badge,
} from '@mantine/core';
import { 
  IconPlus, 
  IconEdit, 
  IconTrash,
  IconCopy,
} from '@tabler/icons-react';

// Mock data for design parameters
const designTypes = [
  { value: 'steel-frame', label: 'Steel Frame Design' },
  { value: 'concrete', label: 'Concrete Design' },
  { value: 'composite', label: 'Composite Design' },
];

const mockParameters = [
  { id: '1', name: 'Definition of Frame', type: 'General', enabled: true },
  { id: '2', name: 'Load Contribution for Nonlinear Load Case', type: 'Load', enabled: true },
  { id: '3', name: 'Short/Long term Load Case', type: 'Load', enabled: true },
  { id: '4', name: 'Member Assignment', type: 'General', enabled: true },
  { id: '5', name: 'Reverse Member Local Direction', type: 'General', enabled: false },
  { id: '6', name: 'Unbraced Length(L,Lb)', type: 'Member', enabled: true },
  { id: '7', name: 'Effective Length Factor(K)', type: 'Member', enabled: true },
  { id: '8', name: 'Limiting Slenderness Ratio', type: 'Member', enabled: true },
  { id: '9', name: 'Moment Factor(Cm)', type: 'Member', enabled: true },
  { id: '10', name: 'Moment Magnifier(B1/Delta_b,B2/Delta_s)', type: 'Member', enabled: true },
  { id: '11', name: 'Imperfection factor(α)', type: 'Member', enabled: false },
  { id: '12', name: 'Modify Member Type', type: 'General', enabled: false },
  { id: '13', name: 'Strength Reduction Factors', type: 'Strength', enabled: true },
  { id: '14', name: 'Serviceability Parameters', type: 'Serviceability', enabled: false },
  { id: '15', name: 'Serviceability Load Combination Type', type: 'Serviceability', enabled: false },
  { id: '16', name: 'Bending Coefficient(Cb)', type: 'Member', enabled: true },
  { id: '17', name: 'Shear Coefficient(Cv)', type: 'Member', enabled: true },
  { id: '18', name: 'Specify Allowable Stress', type: 'Strength', enabled: false },
  { id: '19', name: 'Longitudinal Stiffener of Box Section', type: 'Section', enabled: false },
  { id: '20', name: 'Transverse Stiffener of Section', type: 'Section', enabled: false },
  { id: '21', name: 'Combined Ratio Calculation Method for Circular Section', type: 'Section', enabled: true },
];

export function DesignParametersView() {
  const [selectedType, setSelectedType] = useState<string | null>('steel-frame');
  const [csvInput, setCsvInput] = useState('');
  const [showCsvInput, setShowCsvInput] = useState(false);

  const handlePasteFromCSV = () => {
    setShowCsvInput(!showCsvInput);
  };

  const handleImportCSV = () => {
    // Parse CSV and update parameters
    alert('CSV 데이터가 임포트됩니다.');
    setShowCsvInput(false);
  };

  const getTypeColor = (type: string) => {
    const colors: Record<string, string> = {
      'General': 'blue',
      'Load': 'green',
      'Member': 'violet',
      'Strength': 'orange',
      'Serviceability': 'cyan',
      'Section': 'pink',
    };
    return colors[type] || 'gray';
  };

  return (
    <Stack gap="md">
      {/* Header */}
      <Group justify="space-between">
        <div>
          <Text size="xl" fw={600}>Design Parameters</Text>
          <Text size="sm" c="dimmed">디자인 타입 선택 및 파라미터 정의</Text>
        </div>
        <Group gap="xs">
          <Button variant="light" leftSection={<IconCopy size={16} />} size="sm" onClick={handlePasteFromCSV}>
            CSV 붙여넣기
          </Button>
          <Button leftSection={<IconPlus size={16} />} size="sm">
            새 파라미터
          </Button>
        </Group>
      </Group>

      {/* Design Type Selector */}
      <Paper p="md" withBorder>
        <Group>
          <Text size="sm" fw={500}>Design Type:</Text>
          <Select
            value={selectedType}
            onChange={setSelectedType}
            data={designTypes}
            style={{ width: 300 }}
          />
        </Group>
      </Paper>

      {/* CSV Input Area */}
      {showCsvInput && (
        <Paper p="md" withBorder style={{ backgroundColor: 'var(--mantine-color-blue-0)' }}>
          <Stack gap="md">
            <Text size="sm" fw={500}>CSV 데이터 붙여넣기</Text>
            <Text size="xs" c="dimmed">
              Excel에서 복사한 데이터를 붙여넣으세요. 형식: Name, Type, Enabled (true/false)
            </Text>
            <Textarea
              placeholder="Definition of Frame	General	true
Load Contribution for Nonlinear Load Case	Load	true
Member Assignment	General	true"
              value={csvInput}
              onChange={(e) => setCsvInput(e.currentTarget.value)}
              minRows={6}
              styles={{
                input: { fontFamily: 'monospace', fontSize: '12px' },
              }}
            />
            <Group>
              <Button onClick={handleImportCSV} size="sm">임포트</Button>
              <Button variant="subtle" onClick={() => setShowCsvInput(false)} size="sm">취소</Button>
            </Group>
          </Stack>
        </Paper>
      )}

      {/* Parameters Table */}
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
              <Table.Th width={60}>
                <Checkbox aria-label="Select all" />
              </Table.Th>
              <Table.Th width={60}>ID</Table.Th>
              <Table.Th>Parameter Name</Table.Th>
              <Table.Th width={150}>Type</Table.Th>
              <Table.Th width={100}>Enabled</Table.Th>
              <Table.Th width={100}>Actions</Table.Th>
            </Table.Tr>
          </Table.Thead>
          <Table.Tbody>
            {mockParameters.map((param) => (
              <Table.Tr key={param.id}>
                <Table.Td>
                  <Checkbox aria-label={`Select ${param.name}`} />
                </Table.Td>
                <Table.Td>
                  <Text size="sm" c="dimmed">{param.id}</Text>
                </Table.Td>
                <Table.Td>
                  <Text size="sm">{param.name}</Text>
                </Table.Td>
                <Table.Td>
                  <Badge variant="light" color={getTypeColor(param.type)} size="sm">
                    {param.type}
                  </Badge>
                </Table.Td>
                <Table.Td>
                  <Checkbox 
                    checked={param.enabled} 
                    readOnly
                    aria-label={`${param.name} enabled status`}
                  />
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
          </Table.Tbody>
        </Table>
      </Paper>

      {/* Help Text */}
      <Paper p="md" withBorder style={{ backgroundColor: 'var(--mantine-color-gray-0)' }}>
        <Text size="xs" fw={600} mb="xs">사용 방법</Text>
        <Text size="xs" c="dimmed">
          • Excel에서 파라미터 목록을 복사하여 "CSV 붙여넣기" 버튼으로 임포트할 수 있습니다.<br />
          • 각 파라미터는 이름, 타입, 활성화 상태로 구성됩니다.<br />
          • 파라미터는 Steel Code와 매핑되어 코드별로 지원 여부가 결정됩니다.
        </Text>
      </Paper>
    </Stack>
  );
}
