import { useState } from 'react';
import { 
  Stack, 
  Group, 
  Text, 
  Button, 
  Table, 
  Paper,
  Textarea,
  ActionIcon,
  Badge,
  TextInput,
} from '@mantine/core';
import { 
  IconPlus, 
  IconEdit, 
  IconTrash,
  IconCopy,
  IconSearch,
} from '@tabler/icons-react';

// Mock data for steel codes based on the image
const mockSteelCodes = [
  { id: '1', code: 'KDS 24 14 31:2018', country: 'Korea', type: 'Design Standard', year: '2018' },
  { id: '2', code: 'KDS 24 14 30:2019', country: 'Korea', type: 'Design Standard', year: '2019' },
  { id: '3', code: 'KSSC-ASD03', country: 'Korea', type: 'Steel Structure', year: '2003' },
  { id: '4', code: 'KSSC-LSD16', country: 'Korea', type: 'Steel Structure', year: '2016' },
  { id: '5', code: 'KSCE-LS015', country: 'Korea', type: 'Civil Engineering', year: '2015' },
  { id: '6', code: 'KSCE-ASD10', country: 'Korea', type: 'Civil Engineering', year: '2010' },
  { id: '7', code: 'KSCE-RAIL-ASD11', country: 'Korea', type: 'Railway', year: '2011' },
  { id: '8', code: 'KSCE-ASD05', country: 'Korea', type: 'Civil Engineering', year: '2005' },
  { id: '9', code: 'KSCE-ASD96', country: 'Korea', type: 'Civil Engineering', year: '1996' },
  { id: '10', code: 'AASHTO-LRFD22(US)', country: 'USA', type: 'Bridge', year: '2022' },
  { id: '11', code: 'AASHTO-LRFD17(US)', country: 'USA', type: 'Bridge', year: '2017' },
  { id: '12', code: 'AASHTO-LRFD16(US)', country: 'USA', type: 'Bridge', year: '2016' },
  { id: '13', code: 'AASHTO-LRFD12(US)', country: 'USA', type: 'Bridge', year: '2012' },
  { id: '14', code: 'AASHTO-LRFD02(US)', country: 'USA', type: 'Bridge', year: '2002' },
  { id: '15', code: 'AASHTO-LRFD96(US)', country: 'USA', type: 'Bridge', year: '1996' },
  { id: '16', code: 'AASHTO-ASD96(US)', country: 'USA', type: 'Bridge', year: '1996' },
  { id: '17', code: 'AISC-LRFD2K', country: 'USA', type: 'Steel Construction', year: '2000' },
  { id: '18', code: 'AISC-LRFD93', country: 'USA', type: 'Steel Construction', year: '1993' },
  { id: '19', code: 'AISC-ASD89', country: 'USA', type: 'Steel Construction', year: '1989' },
  { id: '20', code: 'Eurocode3-2:05', country: 'Europe', type: 'Eurocode', year: '2005' },
  { id: '21', code: 'BS5950-2K', country: 'UK', type: 'British Standard', year: '2000' },
  { id: '22', code: 'BS5950-00', country: 'UK', type: 'British Standard', year: '2000' },
  { id: '23', code: 'JTJ025-86', country: 'China', type: 'Highway Bridge', year: '1986' },
  { id: '24', code: 'IS:800-2007', country: 'India', type: 'Steel Structure', year: '2007' },
  { id: '25', code: 'IS:800-1984', country: 'India', type: 'Steel Structure', year: '1984' },
  { id: '26', code: 'IRS SBC', country: 'India', type: 'Railway', year: '-' },
  { id: '27', code: 'TWN-BRG-LSD90', country: 'Taiwan', type: 'Bridge', year: '1990' },
  { id: '28', code: 'TWN-BRG-ASD90', country: 'Taiwan', type: 'Bridge', year: '1990' },
  { id: '29', code: 'IRC-24-2010', country: 'India', type: 'Road Congress', year: '2010' },
  { id: '30', code: 'CSA-S6-19', country: 'Canada', type: 'Bridge', year: '2019' },
  { id: '31', code: 'CSA-S6-14', country: 'Canada', type: 'Bridge', year: '2014' },
  { id: '32', code: 'Japan Road II-H24', country: 'Japan', type: 'Road', year: 'H24' },
  { id: '33', code: 'Japan Road II-H14', country: 'Japan', type: 'Road', year: 'H14' },
];

export function SteelCodeView() {
  const [searchQuery, setSearchQuery] = useState('');
  const [csvInput, setCsvInput] = useState('');
  const [showCsvInput, setShowCsvInput] = useState(false);

  const handlePasteFromCSV = () => {
    setShowCsvInput(!showCsvInput);
  };

  const handleImportCSV = () => {
    alert('CSV 데이터가 임포트됩니다.');
    setShowCsvInput(false);
  };

  const filteredCodes = mockSteelCodes.filter(code => 
    code.code.toLowerCase().includes(searchQuery.toLowerCase()) ||
    code.country.toLowerCase().includes(searchQuery.toLowerCase()) ||
    code.type.toLowerCase().includes(searchQuery.toLowerCase())
  );

  const getCountryColor = (country: string) => {
    const colors: Record<string, string> = {
      'Korea': 'blue',
      'USA': 'red',
      'Europe': 'green',
      'UK': 'violet',
      'China': 'orange',
      'India': 'cyan',
      'Taiwan': 'pink',
      'Canada': 'grape',
      'Japan': 'teal',
    };
    return colors[country] || 'gray';
  };

  return (
    <Stack gap="md">
      {/* Header */}
      <Group justify="space-between">
        <div>
          <Text size="xl" fw={600}>Steel Design Codes</Text>
          <Text size="sm" c="dimmed">강구조 설계 코드 목록 관리</Text>
        </div>
        <Group gap="xs">
          <Button variant="light" leftSection={<IconCopy size={16} />} size="sm" onClick={handlePasteFromCSV}>
            CSV 붙여넣기
          </Button>
          <Button leftSection={<IconPlus size={16} />} size="sm">
            새 코드
          </Button>
        </Group>
      </Group>

      {/* Search */}
      <Paper p="md" withBorder>
        <TextInput
          placeholder="코드명, 국가, 타입으로 검색..."
          leftSection={<IconSearch size={16} />}
          value={searchQuery}
          onChange={(e) => setSearchQuery(e.currentTarget.value)}
        />
      </Paper>

      {/* CSV Input Area */}
      {showCsvInput && (
        <Paper p="md" withBorder style={{ backgroundColor: 'var(--mantine-color-blue-0)' }}>
          <Stack gap="md">
            <Text size="sm" fw={500}>CSV 데이터 붙여넣기</Text>
            <Text size="xs" c="dimmed">
              Excel에서 복사한 데이터를 붙여넣으세요. 형식: Code, Country, Type, Year
            </Text>
            <Textarea
              placeholder="KDS 24 14 31:2018	Korea	Design Standard	2018
KSSC-ASD03	Korea	Steel Structure	2003
AASHTO-LRFD22(US)	USA	Bridge	2022"
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

      {/* Codes Table */}
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
              <Table.Th width={60}>ID</Table.Th>
              <Table.Th>Code</Table.Th>
              <Table.Th width={120}>Country</Table.Th>
              <Table.Th width={180}>Type</Table.Th>
              <Table.Th width={80}>Year</Table.Th>
              <Table.Th width={100}>Actions</Table.Th>
            </Table.Tr>
          </Table.Thead>
          <Table.Tbody>
            {filteredCodes.map((code) => (
              <Table.Tr key={code.id}>
                <Table.Td>
                  <Text size="sm" c="dimmed">{code.id}</Text>
                </Table.Td>
                <Table.Td>
                  <Text size="sm" fw={500} ff="monospace">{code.code}</Text>
                </Table.Td>
                <Table.Td>
                  <Badge variant="light" color={getCountryColor(code.country)} size="sm">
                    {code.country}
                  </Badge>
                </Table.Td>
                <Table.Td>
                  <Text size="sm">{code.type}</Text>
                </Table.Td>
                <Table.Td>
                  <Text size="sm" c="dimmed">{code.year}</Text>
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

      {/* Stats */}
      <Paper p="md" withBorder style={{ backgroundColor: 'var(--mantine-color-gray-0)' }}>
        <Group gap="xl">
          <div>
            <Text size="xs" c="dimmed">전체 코드</Text>
            <Text size="lg" fw={600}>{mockSteelCodes.length}</Text>
          </div>
          <div>
            <Text size="xs" c="dimmed">검색 결과</Text>
            <Text size="lg" fw={600}>{filteredCodes.length}</Text>
          </div>
          <div>
            <Text size="xs" c="dimmed">국가</Text>
            <Text size="lg" fw={600}>{new Set(mockSteelCodes.map(c => c.country)).size}</Text>
          </div>
        </Group>
      </Paper>
    </Stack>
  );
}
