import { useState } from 'react';
import { 
  Stack, 
  Group, 
  Text, 
  Button, 
  Table, 
  Paper,
  Select,
  Checkbox,
  Textarea,
  Badge,
  ActionIcon,
  Tooltip,
} from '@mantine/core';
import { 
  IconCopy,
  IconCheck,
  IconX,
  IconFileExport,
} from '@tabler/icons-react';

// Sample parameters (shortened for demo)
const parameters = [
  'Definition of Frame',
  'Load Contribution for Nonlinear Load Case',
  'Member Assignment',
  'Unbraced Length(L,Lb)',
  'Effective Length Factor(K)',
  'Moment Factor(Cm)',
  'Strength Reduction Factors',
  'Bending Coefficient(Cb)',
  'Shear Coefficient(Cv)',
];

// Sample codes (shortened for demo)
const codes = [
  'KDS 24 14 31:2018',
  'KSSC-ASD03',
  'AASHTO-LRFD22(US)',
  'AISC-LRFD2K',
  'Eurocode3-2:05',
  'BS5950-2K',
  'IS:800-2007',
];

// Mock mapping data (code -> parameter -> supported)
type MappingData = Record<string, Record<string, boolean>>;

const mockMapping: MappingData = {
  'KDS 24 14 31:2018': {
    'Definition of Frame': true,
    'Load Contribution for Nonlinear Load Case': true,
    'Member Assignment': true,
    'Unbraced Length(L,Lb)': true,
    'Effective Length Factor(K)': true,
    'Moment Factor(Cm)': true,
    'Strength Reduction Factors': true,
    'Bending Coefficient(Cb)': true,
    'Shear Coefficient(Cv)': true,
  },
  'KSSC-ASD03': {
    'Definition of Frame': true,
    'Load Contribution for Nonlinear Load Case': true,
    'Member Assignment': true,
    'Unbraced Length(L,Lb)': true,
    'Effective Length Factor(K)': true,
    'Moment Factor(Cm)': false,
    'Strength Reduction Factors': false,
    'Bending Coefficient(Cb)': true,
    'Shear Coefficient(Cv)': true,
  },
  'AASHTO-LRFD22(US)': {
    'Definition of Frame': true,
    'Load Contribution for Nonlinear Load Case': true,
    'Member Assignment': true,
    'Unbraced Length(L,Lb)': true,
    'Effective Length Factor(K)': true,
    'Moment Factor(Cm)': true,
    'Strength Reduction Factors': true,
    'Bending Coefficient(Cb)': true,
    'Shear Coefficient(Cv)': false,
  },
  'AISC-LRFD2K': {
    'Definition of Frame': true,
    'Load Contribution for Nonlinear Load Case': false,
    'Member Assignment': true,
    'Unbraced Length(L,Lb)': true,
    'Effective Length Factor(K)': true,
    'Moment Factor(Cm)': true,
    'Strength Reduction Factors': true,
    'Bending Coefficient(Cb)': true,
    'Shear Coefficient(Cv)': true,
  },
  'Eurocode3-2:05': {
    'Definition of Frame': true,
    'Load Contribution for Nonlinear Load Case': true,
    'Member Assignment': true,
    'Unbraced Length(L,Lb)': true,
    'Effective Length Factor(K)': false,
    'Moment Factor(Cm)': false,
    'Strength Reduction Factors': true,
    'Bending Coefficient(Cb)': false,
    'Shear Coefficient(Cv)': true,
  },
  'BS5950-2K': {
    'Definition of Frame': true,
    'Load Contribution for Nonlinear Load Case': true,
    'Member Assignment': true,
    'Unbraced Length(L,Lb)': true,
    'Effective Length Factor(K)': true,
    'Moment Factor(Cm)': true,
    'Strength Reduction Factors': false,
    'Bending Coefficient(Cb)': true,
    'Shear Coefficient(Cv)': true,
  },
  'IS:800-2007': {
    'Definition of Frame': true,
    'Load Contribution for Nonlinear Load Case': false,
    'Member Assignment': true,
    'Unbraced Length(L,Lb)': true,
    'Effective Length Factor(K)': true,
    'Moment Factor(Cm)': false,
    'Strength Reduction Factors': true,
    'Bending Coefficient(Cb)': true,
    'Shear Coefficient(Cv)': false,
  },
};

export function ParameterCodeMappingView() {
  const [mapping, setMapping] = useState<MappingData>(mockMapping);
  const [csvInput, setCsvInput] = useState('');
  const [showCsvInput, setShowCsvInput] = useState(false);

  const toggleMapping = (code: string, parameter: string) => {
    setMapping(prev => ({
      ...prev,
      [code]: {
        ...prev[code],
        [parameter]: !prev[code]?.[parameter],
      },
    }));
  };

  const handlePasteFromCSV = () => {
    setShowCsvInput(!showCsvInput);
  };

  const handleImportCSV = () => {
    alert('CSV 데이터가 임포트됩니다.');
    setShowCsvInput(false);
  };

  const handleExportCSV = () => {
    // Generate CSV
    let csv = '\t' + codes.join('\t') + '\n';
    parameters.forEach(param => {
      csv += param + '\t';
      csv += codes.map(code => mapping[code]?.[param] ? 'O' : 'X').join('\t');
      csv += '\n';
    });
    
    navigator.clipboard.writeText(csv);
    alert('CSV 데이터가 클립보드에 복사되었습니다. Excel에 붙여넣기 하세요.');
  };

  const getSupportedCount = (code: string) => {
    return parameters.filter(p => mapping[code]?.[p]).length;
  };

  return (
    <Stack gap="md">
      {/* Header */}
      <Group justify="space-between">
        <div>
          <Text size="xl" fw={600}>Parameter-Code Mapping</Text>
          <Text size="sm" c="dimmed">각 Steel Code가 지원하는 Design Parameter 정의</Text>
        </div>
        <Group gap="xs">
          <Button variant="light" leftSection={<IconCopy size={16} />} size="sm" onClick={handlePasteFromCSV}>
            CSV 붙여넣기
          </Button>
          <Button variant="light" leftSection={<IconFileExport size={16} />} size="sm" onClick={handleExportCSV}>
            CSV 내보내기
          </Button>
        </Group>
      </Group>

      {/* CSV Input Area */}
      {showCsvInput && (
        <Paper p="md" withBorder style={{ backgroundColor: 'var(--mantine-color-blue-0)' }}>
          <Stack gap="md">
            <Text size="sm" fw={500}>CSV 데이터 붙여넣기</Text>
            <Text size="xs" c="dimmed">
              Excel에서 복사한 매핑 테이블을 붙여넣으세요. 첫 행은 코드명, 첫 열은 파라미터명, 값은 O/X 또는 true/false
            </Text>
            <Textarea
              placeholder="	KDS 24 14 31:2018	KSSC-ASD03	AASHTO-LRFD22(US)
Definition of Frame	O	O	O
Member Assignment	O	O	O
Unbraced Length(L,Lb)	O	O	O"
              value={csvInput}
              onChange={(e) => setCsvInput(e.currentTarget.value)}
              minRows={8}
              styles={{
                input: { fontFamily: 'monospace', fontSize: '11px' },
              }}
            />
            <Group>
              <Button onClick={handleImportCSV} size="sm">임포트</Button>
              <Button variant="subtle" onClick={() => setShowCsvInput(false)} size="sm">취소</Button>
            </Group>
          </Stack>
        </Paper>
      )}

      {/* Mapping Matrix Table */}
      <Paper withBorder style={{ overflowX: 'auto' }}>
        <Table 
          striped 
          highlightOnHover 
          withTableBorder={false}
          verticalSpacing="xs"
          style={{ fontSize: '12px' }}
        >
          <Table.Thead>
            <Table.Tr style={{ backgroundColor: 'var(--mantine-color-gray-1)' }}>
              <Table.Th style={{ position: 'sticky', left: 0, backgroundColor: 'var(--mantine-color-gray-1)', zIndex: 1 }}>
                Parameter / Code
              </Table.Th>
              {codes.map((code) => (
                <Table.Th key={code} style={{ minWidth: 140 }}>
                  <Stack gap={4}>
                    <Text size="xs" fw={500}>{code}</Text>
                    <Badge size="xs" variant="light">
                      {getSupportedCount(code)}/{parameters.length}
                    </Badge>
                  </Stack>
                </Table.Th>
              ))}
            </Table.Tr>
          </Table.Thead>
          <Table.Tbody>
            {parameters.map((parameter) => (
              <Table.Tr key={parameter}>
                <Table.Td style={{ position: 'sticky', left: 0, backgroundColor: 'white', zIndex: 1, borderRight: '1px solid var(--mantine-color-gray-3)' }}>
                  <Text size="sm" style={{ minWidth: 300 }}>{parameter}</Text>
                </Table.Td>
                {codes.map((code) => {
                  const isSupported = mapping[code]?.[parameter];
                  return (
                    <Table.Td key={`${code}-${parameter}`}>
                      <Group justify="center">
                        <Tooltip label={isSupported ? '지원됨' : '지원안됨'}>
                          <ActionIcon
                            variant={isSupported ? 'filled' : 'subtle'}
                            color={isSupported ? 'green' : 'gray'}
                            size="sm"
                            onClick={() => toggleMapping(code, parameter)}
                            aria-label={`Toggle ${parameter} for ${code}`}
                          >
                            {isSupported ? <IconCheck size={14} /> : <IconX size={14} />}
                          </ActionIcon>
                        </Tooltip>
                      </Group>
                    </Table.Td>
                  );
                })}
              </Table.Tr>
            ))}
          </Table.Tbody>
        </Table>
      </Paper>

      {/* Legend */}
      <Paper p="md" withBorder style={{ backgroundColor: 'var(--mantine-color-gray-0)' }}>
        <Group gap="xl">
          <Group gap="xs">
            <ActionIcon variant="filled" color="green" size="sm" aria-label="Supported icon">
              <IconCheck size={14} />
            </ActionIcon>
            <Text size="xs">지원됨 (클릭하여 토글)</Text>
          </Group>
          <Group gap="xs">
            <ActionIcon variant="subtle" color="gray" size="sm" aria-label="Not supported icon">
              <IconX size={14} />
            </ActionIcon>
            <Text size="xs">지원안됨 (클릭하여 토글)</Text>
          </Group>
        </Group>
        <Text size="xs" c="dimmed" mt="xs">
          • Excel에서 O/X 테이블을 복사하여 붙여넣기 할 수 있습니다.<br />
          • "CSV 내보내기"로 현재 매핑을 Excel로 내보낼 수 있습니다.
        </Text>
      </Paper>
    </Stack>
  );
}
