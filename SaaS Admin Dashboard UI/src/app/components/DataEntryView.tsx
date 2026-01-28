import { useState } from 'react';
import { 
  Stack, 
  Group, 
  Text, 
  Button, 
  Paper,
  Select,
  Table,
  ActionIcon,
  Modal,
  TextInput,
  Checkbox,
  NumberInput,
  Textarea,
} from '@mantine/core';
import { 
  IconPlus, 
  IconEdit, 
  IconTrash,
  IconCopy,
  IconFileExport,
} from '@tabler/icons-react';
import { dataStore, ObjectType, DataRecord } from '@/app/store/dataStore';
import { parseClipboardData } from '@/app/utils/clipboard';

export function DataEntryView() {
  const [objectTypes] = useState<ObjectType[]>(dataStore.objectTypes);
  const [selectedObjectTypeId, setSelectedObjectTypeId] = useState<string | null>(
    objectTypes.length > 0 ? objectTypes[0].id : null
  );
  const [records, setRecords] = useState<DataRecord[]>(dataStore.dataRecords);
  const [isModalOpen, setIsModalOpen] = useState(false);
  const [editingRecord, setEditingRecord] = useState<DataRecord | null>(null);
  const [formValues, setFormValues] = useState<Record<string, any>>({});
  const [showCsvInput, setShowCsvInput] = useState(false);
  const [csvInput, setCsvInput] = useState('');
  const [selectedCell, setSelectedCell] = useState<{ row: number; col: number } | null>(null);

  const selectedObjectType = objectTypes.find(o => o.id === selectedObjectTypeId);
  const filteredRecords = selectedObjectTypeId 
    ? records.filter(r => r.objectTypeId === selectedObjectTypeId)
    : [];

  const handleTablePaste = (event: React.ClipboardEvent) => {
    if (!selectedObjectType || !selectedCell) return;

    event.preventDefault();
    const text = event.clipboardData.getData('text/plain');
    if (!text) return;

    const data = parseClipboardData(text);
    const { row: startRow, col: startCol } = selectedCell;

    data.forEach((rowData, rowOffset) => {
      const targetRowIndex = startRow + rowOffset;
      if (targetRowIndex >= filteredRecords.length) return;

      const record = filteredRecords[targetRowIndex];
      const updatedValues = { ...record.values };

      rowData.forEach((cellValue, colOffset) => {
        const targetColIndex = startCol + colOffset;
        if (targetColIndex >= selectedObjectType.attributes.length) return;

        const attr = selectedObjectType.attributes[targetColIndex];
        let parsedValue: any = cellValue;

        // Parse based on attribute type
        if (attr.type === 'number') {
          parsedValue = parseFloat(cellValue) || 0;
        } else if (attr.type === 'boolean') {
          parsedValue = cellValue.toLowerCase() === 'true' || 
                        cellValue.toLowerCase() === 'o' || 
                        cellValue === '1';
        }

        updatedValues[attr.name] = parsedValue;
      });

      dataStore.updateDataRecord(record.id, { values: updatedValues });
    });

    setRecords([...dataStore.dataRecords]);
    setSelectedCell(null);
  };

  const handleOpenModal = (record?: DataRecord) => {
    if (record) {
      setEditingRecord(record);
      setFormValues({ ...record.values });
    } else {
      setEditingRecord(null);
      const initialValues: Record<string, any> = {};
      selectedObjectType?.attributes.forEach(attr => {
        initialValues[attr.name] = attr.type === 'boolean' ? false : '';
      });
      setFormValues(initialValues);
    }
    setIsModalOpen(true);
  };

  const handleSave = () => {
    if (!selectedObjectTypeId) return;

    if (editingRecord) {
      dataStore.updateDataRecord(editingRecord.id, { values: formValues });
    } else {
      dataStore.addDataRecord({
        objectTypeId: selectedObjectTypeId,
        values: formValues,
      });
    }
    setRecords([...dataStore.dataRecords]);
    setIsModalOpen(false);
  };

  const handleDelete = (id: string) => {
    if (confirm('이 데이터를 삭제하시겠습니까?')) {
      dataStore.deleteDataRecord(id);
      setRecords([...dataStore.dataRecords]);
    }
  };

  const handleImportCSV = () => {
    if (!selectedObjectType || !csvInput.trim()) return;

    const lines = csvInput.trim().split('\n');
    const headers = lines[0].split('\t').map(h => h.trim());
    
    for (let i = 1; i < lines.length; i++) {
      const values = lines[i].split('\t');
      const record: Record<string, any> = {};
      
      headers.forEach((header, index) => {
        const attr = selectedObjectType.attributes.find(a => a.name === header);
        if (attr) {
          let value = values[index]?.trim() || '';
          if (attr.type === 'number') {
            value = parseFloat(value) || 0;
          } else if (attr.type === 'boolean') {
            value = value.toLowerCase() === 'true' || value.toLowerCase() === 'o' || value === '1';
          }
          record[header] = value;
        }
      });

      dataStore.addDataRecord({
        objectTypeId: selectedObjectType.id,
        values: record,
      });
    }

    setRecords([...dataStore.dataRecords]);
    setShowCsvInput(false);
    setCsvInput('');
  };

  const handleExportCSV = () => {
    if (!selectedObjectType) return;

    const headers = selectedObjectType.attributes.map(a => a.name);
    let csv = headers.join('\t') + '\n';

    filteredRecords.forEach(record => {
      const row = headers.map(h => {
        const value = record.values[h];
        if (typeof value === 'boolean') return value ? 'O' : 'X';
        return value?.toString() || '';
      });
      csv += row.join('\t') + '\n';
    });

    navigator.clipboard.writeText(csv);
    alert('CSV 데이터가 클립보드에 복사되었습니다. Excel에 붙여넣기 하세요.');
  };

  return (
    <Stack gap="lg">
      {/* Header */}
      <Group justify="space-between">
        <div>
          <Text size="xl" fw={600}>Data Entry</Text>
          <Text size="sm" c="dimmed">실제 데이터 입력 및 관리</Text>
        </div>
        <Group gap="xs">
          <Button 
            variant="light" 
            leftSection={<IconCopy size={16} />} 
            size="sm"
            onClick={() => setShowCsvInput(!showCsvInput)}
            disabled={!selectedObjectTypeId}
          >
            CSV 붙여넣기
          </Button>
          <Button 
            variant="light" 
            leftSection={<IconFileExport size={16} />} 
            size="sm"
            onClick={handleExportCSV}
            disabled={!selectedObjectTypeId || filteredRecords.length === 0}
          >
            CSV 내보내기
          </Button>
          <Button 
            leftSection={<IconPlus size={16} />} 
            size="sm"
            onClick={() => handleOpenModal()}
            disabled={!selectedObjectTypeId}
          >
            새 레코드
          </Button>
        </Group>
      </Group>

      {/* Object Type Selector */}
      <Paper p="md" withBorder>
        <Group>
          <Text size="sm" fw={500}>Object Type:</Text>
          <Select
            placeholder="오브젝트 타입 선택"
            data={objectTypes.map(o => ({ value: o.id, label: o.name }))}
            value={selectedObjectTypeId}
            onChange={setSelectedObjectTypeId}
            style={{ width: 300 }}
          />
          {selectedObjectType && (
            <Text size="sm" c="dimmed">
              {filteredRecords.length} records
            </Text>
          )}
        </Group>
      </Paper>

      {/* CSV Input Area */}
      {showCsvInput && selectedObjectType && (
        <Paper p="md" withBorder style={{ backgroundColor: 'var(--mantine-color-blue-0)' }}>
          <Stack gap="md">
            <Text size="sm" fw={500}>CSV 데이터 붙여넣기</Text>
            <Text size="xs" c="dimmed">
              Excel에서 복사한 데이터를 붙여넣으세요. 첫 행은 속성명: {selectedObjectType.attributes.map(a => a.name).join(', ')}
            </Text>
            <Textarea
              placeholder={`${selectedObjectType.attributes.map(a => a.name).join('\t')}\nvalue1\tvalue2\tvalue3`}
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

      {/* Data Table */}
      {selectedObjectType && filteredRecords.length > 0 && (
        <Paper withBorder style={{ overflowX: 'auto' }}>
          <div onPaste={handleTablePaste} tabIndex={0} style={{ outline: 'none' }}>
            <Table 
              striped 
              highlightOnHover 
              withTableBorder={false}
              verticalSpacing="xs"
              style={{ fontSize: '13px' }}
            >
              <Table.Thead>
                <Table.Tr style={{ backgroundColor: 'var(--mantine-color-gray-1)' }}>
                  {selectedObjectType.attributes.map(attr => (
                    <Table.Th key={attr.id}>{attr.name}</Table.Th>
                  ))}
                  <Table.Th width={100}>Actions</Table.Th>
                </Table.Tr>
              </Table.Thead>
              <Table.Tbody>
                {filteredRecords.map((record, rowIndex) => (
                  <Table.Tr key={record.id}>
                    {selectedObjectType.attributes.map((attr, colIndex) => {
                      const value = record.values[attr.name];
                      const isSelected = selectedCell?.row === rowIndex && selectedCell?.col === colIndex;
                      return (
                        <Table.Td 
                          key={attr.id}
                          onClick={() => setSelectedCell({ row: rowIndex, col: colIndex })}
                          style={{ 
                            cursor: 'pointer',
                            backgroundColor: isSelected ? 'var(--mantine-color-blue-1)' : undefined,
                          }}
                        >
                          {attr.type === 'boolean' ? (
                            <Checkbox checked={Boolean(value)} readOnly aria-label={attr.name} />
                          ) : (
                            <Text size="sm">{value?.toString() || '-'}</Text>
                          )}
                        </Table.Td>
                      );
                    })}
                    <Table.Td>
                      <Group gap={4}>
                        <ActionIcon 
                          variant="subtle" 
                          size="sm"
                          onClick={() => handleOpenModal(record)}
                          aria-label="Edit record"
                        >
                          <IconEdit size={16} />
                        </ActionIcon>
                        <ActionIcon 
                          variant="subtle" 
                          size="sm" 
                          color="red"
                          onClick={() => handleDelete(record.id)}
                          aria-label="Delete record"
                        >
                          <IconTrash size={16} />
                        </ActionIcon>
                      </Group>
                    </Table.Td>
                  </Table.Tr>
                ))}
              </Table.Tbody>
            </Table>
          </div>
          <Text size="xs" c="dimmed" p="xs" style={{ borderTop: '1px solid var(--mantine-color-gray-3)' }}>
            💡 Tip: 셀을 클릭한 후 Excel에서 복사한 데이터를 Ctrl+V (또는 Cmd+V)로 붙여넣기 하세요.
          </Text>
        </Paper>
      )}

      {selectedObjectType && filteredRecords.length === 0 && !showCsvInput && (
        <Paper p="xl" withBorder style={{ textAlign: 'center' }}>
          <Text c="dimmed">데이터가 없습니다. "새 레코드" 또는 "CSV 붙여넣기"를 사용하세요.</Text>
        </Paper>
      )}

      {/* Modal for Create/Edit */}
      <Modal
        opened={isModalOpen}
        onClose={() => setIsModalOpen(false)}
        title={editingRecord ? 'Edit Record' : 'New Record'}
        size="lg"
      >
        <Stack gap="md">
          {selectedObjectType?.attributes.map(attr => (
            <div key={attr.id}>
              {attr.type === 'text' && (
                <TextInput
                  label={attr.name}
                  placeholder={`Enter ${attr.name}`}
                  value={formValues[attr.name] || ''}
                  onChange={(e) => setFormValues({ ...formValues, [attr.name]: e.currentTarget.value })}
                  required={attr.required}
                />
              )}
              {attr.type === 'number' && (
                <NumberInput
                  label={attr.name}
                  placeholder={`Enter ${attr.name}`}
                  value={formValues[attr.name] || 0}
                  onChange={(val) => setFormValues({ ...formValues, [attr.name]: val })}
                  required={attr.required}
                />
              )}
              {attr.type === 'boolean' && (
                <Checkbox
                  label={attr.name}
                  checked={Boolean(formValues[attr.name])}
                  onChange={(e) => setFormValues({ ...formValues, [attr.name]: e.currentTarget.checked })}
                />
              )}
              {attr.type === 'select' && (
                <Select
                  label={attr.name}
                  placeholder={`Select ${attr.name}`}
                  data={attr.options || []}
                  value={formValues[attr.name] || null}
                  onChange={(val) => setFormValues({ ...formValues, [attr.name]: val })}
                  required={attr.required}
                />
              )}
            </div>
          ))}

          <Group justify="flex-end" mt="md">
            <Button variant="subtle" onClick={() => setIsModalOpen(false)}>취소</Button>
            <Button onClick={handleSave}>저장</Button>
          </Group>
        </Stack>
      </Modal>
    </Stack>
  );
}