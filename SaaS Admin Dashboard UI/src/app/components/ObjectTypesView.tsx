import { useState } from 'react';
import { 
  Stack, 
  Group, 
  Text, 
  Button, 
  Paper,
  Card,
  Badge,
  ActionIcon,
  Modal,
  TextInput,
  Textarea,
  Select,
  ColorInput,
  Table,
  Checkbox,
} from '@mantine/core';
import { 
  IconPlus, 
  IconEdit, 
  IconTrash,
  IconAdjustments,
  IconCode,
  IconSettings,
} from '@tabler/icons-react';
import { dataStore, ObjectType, Attribute, AttributeType } from '@/app/store/dataStore';

const iconOptions = [
  { value: 'IconAdjustments', label: 'Adjustments' },
  { value: 'IconCode', label: 'Code' },
  { value: 'IconSettings', label: 'Settings' },
  { value: 'IconDatabase', label: 'Database' },
  { value: 'IconTable', label: 'Table' },
];

const colorOptions = [
  'blue', 'green', 'red', 'orange', 'violet', 'cyan', 'pink', 'grape', 'teal', 'lime'
];

export function ObjectTypesView() {
  const [objectTypes, setObjectTypes] = useState<ObjectType[]>(dataStore.objectTypes);
  const [isModalOpen, setIsModalOpen] = useState(false);
  const [editingObjectType, setEditingObjectType] = useState<ObjectType | null>(null);
  const [selectedObjectType, setSelectedObjectType] = useState<ObjectType | null>(
    objectTypes.length > 0 ? objectTypes[0] : null
  );

  // Form state
  const [formName, setFormName] = useState('');
  const [formDescription, setFormDescription] = useState('');
  const [formIcon, setFormIcon] = useState('IconAdjustments');
  const [formColor, setFormColor] = useState('blue');
  const [formAttributes, setFormAttributes] = useState<Attribute[]>([]);

  const handleOpenModal = (objectType?: ObjectType) => {
    if (objectType) {
      setEditingObjectType(objectType);
      setFormName(objectType.name);
      setFormDescription(objectType.description);
      setFormIcon(objectType.icon);
      setFormColor(objectType.color);
      setFormAttributes([...objectType.attributes]);
    } else {
      setEditingObjectType(null);
      setFormName('');
      setFormDescription('');
      setFormIcon('IconAdjustments');
      setFormColor('blue');
      setFormAttributes([]);
    }
    setIsModalOpen(true);
  };

  const handleSave = () => {
    if (editingObjectType) {
      dataStore.updateObjectType(editingObjectType.id, {
        name: formName,
        description: formDescription,
        icon: formIcon,
        color: formColor,
        attributes: formAttributes,
      });
    } else {
      const newObjectType = dataStore.addObjectType({
        name: formName,
        description: formDescription,
        icon: formIcon,
        color: formColor,
        attributes: formAttributes,
      });
      setSelectedObjectType(newObjectType);
    }
    setObjectTypes([...dataStore.objectTypes]);
    setIsModalOpen(false);
  };

  const handleDelete = (id: string) => {
    if (confirm('이 오브젝트 타입과 관련된 모든 데이터가 삭제됩니다. 계속하시겠습니까?')) {
      dataStore.deleteObjectType(id);
      setObjectTypes([...dataStore.objectTypes]);
      if (selectedObjectType?.id === id) {
        setSelectedObjectType(objectTypes[0] || null);
      }
    }
  };

  const handleAddAttribute = () => {
    const newAttr: Attribute = {
      id: `attr-${Date.now()}`,
      name: '',
      type: 'text',
      required: false,
    };
    setFormAttributes([...formAttributes, newAttr]);
  };

  const handleUpdateAttribute = (index: number, updates: Partial<Attribute>) => {
    const updated = [...formAttributes];
    updated[index] = { ...updated[index], ...updates };
    setFormAttributes(updated);
  };

  const handleDeleteAttribute = (index: number) => {
    setFormAttributes(formAttributes.filter((_, i) => i !== index));
  };

  const getIconComponent = (iconName: string) => {
    const icons: Record<string, any> = {
      IconAdjustments,
      IconCode,
      IconSettings,
    };
    const Icon = icons[iconName] || IconAdjustments;
    return <Icon size={20} />;
  };

  return (
    <Stack gap="lg">
      {/* Header */}
      <Group justify="space-between">
        <div>
          <Text size="xl" fw={600}>Object Types</Text>
          <Text size="sm" c="dimmed">오브젝트 타입 생성 및 속성 정의</Text>
        </div>
        <Button leftSection={<IconPlus size={16} />} size="sm" onClick={() => handleOpenModal()}>
          새 오브젝트 타입
        </Button>
      </Group>

      {/* Object Types Grid */}
      <div style={{ display: 'grid', gridTemplateColumns: 'repeat(auto-fill, minmax(280px, 1fr))', gap: '1rem' }}>
        {objectTypes.map((objType) => (
          <Card 
            key={objType.id} 
            shadow="sm" 
            padding="lg" 
            withBorder
            style={{ 
              cursor: 'pointer',
              border: selectedObjectType?.id === objType.id ? `2px solid var(--mantine-color-${objType.color}-6)` : undefined,
            }}
            onClick={() => setSelectedObjectType(objType)}
          >
            <Group justify="space-between" mb="xs">
              <Group gap="xs">
                <div style={{ color: `var(--mantine-color-${objType.color}-6)` }}>
                  {getIconComponent(objType.icon)}
                </div>
                <Text fw={600}>{objType.name}</Text>
              </Group>
              <Group gap={4}>
                <ActionIcon 
                  variant="subtle" 
                  size="sm" 
                  onClick={(e) => { e.stopPropagation(); handleOpenModal(objType); }}
                  aria-label="Edit object type"
                >
                  <IconEdit size={16} />
                </ActionIcon>
                <ActionIcon 
                  variant="subtle" 
                  size="sm" 
                  color="red"
                  onClick={(e) => { e.stopPropagation(); handleDelete(objType.id); }}
                  aria-label="Delete object type"
                >
                  <IconTrash size={16} />
                </ActionIcon>
              </Group>
            </Group>
            <Text size="xs" c="dimmed" mb="md">{objType.description}</Text>
            <Badge color={objType.color} size="sm">{objType.attributes.length} 속성</Badge>
          </Card>
        ))}
      </div>

      {/* Selected Object Type Details */}
      {selectedObjectType && (
        <Paper withBorder p="md">
          <Group justify="space-between" mb="md">
            <Group>
              <div style={{ color: `var(--mantine-color-${selectedObjectType.color}-6)` }}>
                {getIconComponent(selectedObjectType.icon)}
              </div>
              <div>
                <Text size="lg" fw={600}>{selectedObjectType.name}</Text>
                <Text size="xs" c="dimmed">{selectedObjectType.description}</Text>
              </div>
            </Group>
            <Button size="xs" variant="light" onClick={() => handleOpenModal(selectedObjectType)}>
              속성 편집
            </Button>
          </Group>

          <Text size="sm" fw={600} mb="xs">Attributes</Text>
          <Table withTableBorder withColumnBorders verticalSpacing="xs" style={{ fontSize: '13px' }}>
            <Table.Thead>
              <Table.Tr style={{ backgroundColor: 'var(--mantine-color-gray-1)' }}>
                <Table.Th>Name</Table.Th>
                <Table.Th>Type</Table.Th>
                <Table.Th width={100}>Required</Table.Th>
                <Table.Th>Options</Table.Th>
              </Table.Tr>
            </Table.Thead>
            <Table.Tbody>
              {selectedObjectType.attributes.map((attr) => (
                <Table.Tr key={attr.id}>
                  <Table.Td>
                    <Text size="sm" fw={500}>{attr.name}</Text>
                  </Table.Td>
                  <Table.Td>
                    <Badge size="sm" variant="light">{attr.type}</Badge>
                  </Table.Td>
                  <Table.Td>
                    <Checkbox checked={attr.required} readOnly aria-label={`${attr.name} required`} />
                  </Table.Td>
                  <Table.Td>
                    <Text size="xs" c="dimmed">
                      {attr.options ? attr.options.join(', ') : '-'}
                    </Text>
                  </Table.Td>
                </Table.Tr>
              ))}
            </Table.Tbody>
          </Table>
        </Paper>
      )}

      {/* Modal for Create/Edit */}
      <Modal
        opened={isModalOpen}
        onClose={() => setIsModalOpen(false)}
        title={editingObjectType ? 'Edit Object Type' : 'New Object Type'}
        size="xl"
      >
        <Stack gap="md">
          <TextInput
            label="Name"
            placeholder="예: Design Parameter"
            value={formName}
            onChange={(e) => setFormName(e.currentTarget.value)}
            required
          />
          <Textarea
            label="Description"
            placeholder="예: Engineering design parameters"
            value={formDescription}
            onChange={(e) => setFormDescription(e.currentTarget.value)}
            minRows={2}
          />
          <Group grow>
            <Select
              label="Icon"
              data={iconOptions}
              value={formIcon}
              onChange={(val) => setFormIcon(val || 'IconAdjustments')}
            />
            <Select
              label="Color"
              data={colorOptions}
              value={formColor}
              onChange={(val) => setFormColor(val || 'blue')}
            />
          </Group>

          <div>
            <Group justify="space-between" mb="xs">
              <Text size="sm" fw={600}>Attributes</Text>
              <Button size="xs" variant="light" onClick={handleAddAttribute}>
                + 속성 추가
              </Button>
            </Group>

            {formAttributes.map((attr, index) => (
              <Paper key={attr.id} withBorder p="sm" mb="xs">
                <Group align="flex-start">
                  <TextInput
                    placeholder="Attribute name"
                    value={attr.name}
                    onChange={(e) => handleUpdateAttribute(index, { name: e.currentTarget.value })}
                    style={{ flex: 1 }}
                    size="xs"
                  />
                  <Select
                    placeholder="Type"
                    data={['text', 'number', 'boolean', 'date', 'select']}
                    value={attr.type}
                    onChange={(val) => handleUpdateAttribute(index, { type: val as AttributeType })}
                    style={{ width: 120 }}
                    size="xs"
                  />
                  <Checkbox
                    label="Required"
                    checked={attr.required}
                    onChange={(e) => handleUpdateAttribute(index, { required: e.currentTarget.checked })}
                    size="xs"
                  />
                  <ActionIcon 
                    color="red" 
                    size="sm" 
                    onClick={() => handleDeleteAttribute(index)}
                    aria-label="Delete attribute"
                  >
                    <IconTrash size={14} />
                  </ActionIcon>
                </Group>
                {attr.type === 'select' && (
                  <TextInput
                    placeholder="Options (comma separated)"
                    value={attr.options?.join(', ') || ''}
                    onChange={(e) => handleUpdateAttribute(index, { 
                      options: e.currentTarget.value.split(',').map(s => s.trim()) 
                    })}
                    size="xs"
                    mt="xs"
                  />
                )}
              </Paper>
            ))}
          </div>

          <Group justify="flex-end" mt="md">
            <Button variant="subtle" onClick={() => setIsModalOpen(false)}>취소</Button>
            <Button onClick={handleSave} disabled={!formName}>저장</Button>
          </Group>
        </Stack>
      </Modal>
    </Stack>
  );
}
