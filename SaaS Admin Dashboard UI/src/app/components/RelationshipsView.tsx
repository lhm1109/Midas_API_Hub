import { useState } from 'react';
import { 
  Stack, 
  Group, 
  Text, 
  Button, 
  Paper,
  Table,
  ActionIcon,
  Modal,
  TextInput,
  Textarea,
  Select,
  Badge,
} from '@mantine/core';
import { 
  IconPlus, 
  IconTrash,
  IconArrowRight,
  IconArrowsExchange,
  IconArrowRightCircle,
  IconEdit,
  IconCopy,
} from '@tabler/icons-react';
import { dataStore, Relationship, ObjectType } from '@/app/store/dataStore';

export function RelationshipsView() {
  const [objectTypes] = useState<ObjectType[]>(dataStore.objectTypes);
  const [relationships, setRelationships] = useState<Relationship[]>(dataStore.relationships);
  const [isModalOpen, setIsModalOpen] = useState(false);
  const [editingId, setEditingId] = useState<string | null>(null);
  const [formName, setFormName] = useState('');
  const [formDescription, setFormDescription] = useState('');
  const [formFromObjectTypeId, setFormFromObjectTypeId] = useState<string | null>(null);
  const [formToObjectTypeId, setFormToObjectTypeId] = useState<string | null>(null);
  const [formType, setFormType] = useState<'one-to-one' | 'one-to-many' | 'many-to-many'>('many-to-many');

  const handleOpenModal = () => {
    setEditingId(null);
    setFormName('');
    setFormDescription('');
    setFormFromObjectTypeId(objectTypes.length > 0 ? objectTypes[0].id : null);
    setFormToObjectTypeId(objectTypes.length > 1 ? objectTypes[1].id : objectTypes[0]?.id || null);
    setFormType('many-to-many');
    setIsModalOpen(true);
  };

  const handleEdit = (rel: Relationship) => {
    setEditingId(rel.id);
    setFormName(rel.name);
    setFormDescription(rel.description);
    setFormFromObjectTypeId(rel.fromObjectTypeId);
    setFormToObjectTypeId(rel.toObjectTypeId);
    setFormType(rel.type);
    setIsModalOpen(true);
  };

  const handleCopy = (rel: Relationship) => {
    setEditingId(null);
    setFormName(rel.name + ' (복사)');
    setFormDescription(rel.description);
    setFormFromObjectTypeId(rel.fromObjectTypeId);
    setFormToObjectTypeId(rel.toObjectTypeId);
    setFormType(rel.type);
    setIsModalOpen(true);
  };

  const handleSave = () => {
    if (!formName || !formFromObjectTypeId || !formToObjectTypeId) return;

    if (editingId) {
      // Update existing relationship
      dataStore.relationships = dataStore.relationships.map(r => 
        r.id === editingId 
          ? { ...r, name: formName, description: formDescription, fromObjectTypeId: formFromObjectTypeId, toObjectTypeId: formToObjectTypeId, type: formType }
          : r
      );
    } else {
      // Create new relationship
      dataStore.addRelationship({
        name: formName,
        description: formDescription,
        fromObjectTypeId: formFromObjectTypeId,
        toObjectTypeId: formToObjectTypeId,
        type: formType,
      });
    }

    setRelationships([...dataStore.relationships]);
    setIsModalOpen(false);
  };

  const handleDelete = (id: string) => {
    if (confirm('이 관계와 모든 연결을 삭제하시겠습니까?')) {
      dataStore.deleteRelationship(id);
      setRelationships([...dataStore.relationships]);
    }
  };

  const getObjectTypeName = (id: string) => {
    return objectTypes.find(o => o.id === id)?.name || 'Unknown';
  };

  const getRelationshipIcon = (type: string) => {
    switch (type) {
      case 'one-to-one':
        return <IconArrowRight size={16} />;
      case 'one-to-many':
        return <IconArrowRightCircle size={16} />;
      case 'many-to-many':
        return <IconArrowsExchange size={16} />;
      default:
        return <IconArrowRight size={16} />;
    }
  };

  const getRelationshipColor = (type: string) => {
    switch (type) {
      case 'one-to-one':
        return 'blue';
      case 'one-to-many':
        return 'green';
      case 'many-to-many':
        return 'violet';
      default:
        return 'gray';
    }
  };

  return (
    <Stack gap="lg">
      {/* Header */}
      <Group justify="space-between">
        <div>
          <Text size="xl" fw={600}>Relationships</Text>
          <Text size="sm" c="dimmed">오브젝트 타입 간 관계 정의</Text>
        </div>
        <Button 
          leftSection={<IconPlus size={16} />} 
          size="sm"
          onClick={handleOpenModal}
          disabled={objectTypes.length < 2}
        >
          새 관계
        </Button>
      </Group>

      {/* Relationships Table */}
      {relationships.length > 0 ? (
        <Paper withBorder>
          <Table 
            striped 
            highlightOnHover 
            withTableBorder={false}
            verticalSpacing="sm"
            style={{ fontSize: '14px' }}
          >
            <Table.Thead>
              <Table.Tr style={{ backgroundColor: 'var(--mantine-color-gray-1)' }}>
                <Table.Th>Relationship Name</Table.Th>
                <Table.Th>From Object</Table.Th>
                <Table.Th width={100}>Type</Table.Th>
                <Table.Th>To Object</Table.Th>
                <Table.Th>Description</Table.Th>
                <Table.Th width={80}>Actions</Table.Th>
              </Table.Tr>
            </Table.Thead>
            <Table.Tbody>
              {relationships.map((rel) => (
                <Table.Tr key={rel.id}>
                  <Table.Td>
                    <Text size="sm" fw={600}>{rel.name}</Text>
                  </Table.Td>
                  <Table.Td>
                    <Badge variant="light" color="blue">
                      {getObjectTypeName(rel.fromObjectTypeId)}
                    </Badge>
                  </Table.Td>
                  <Table.Td>
                    <Group gap="xs" justify="center">
                      <Badge 
                        variant="light" 
                        color={getRelationshipColor(rel.type)}
                        leftSection={getRelationshipIcon(rel.type)}
                        size="sm"
                      >
                        {rel.type}
                      </Badge>
                    </Group>
                  </Table.Td>
                  <Table.Td>
                    <Badge variant="light" color="green">
                      {getObjectTypeName(rel.toObjectTypeId)}
                    </Badge>
                  </Table.Td>
                  <Table.Td>
                    <Text size="sm" c="dimmed">{rel.description}</Text>
                  </Table.Td>
                  <Table.Td>
                    <Group gap={4}>
                      <ActionIcon 
                        variant="subtle" 
                        size="sm" 
                        color="blue"
                        onClick={() => handleEdit(rel)}
                        aria-label="Edit relationship"
                      >
                        <IconEdit size={16} />
                      </ActionIcon>
                      <ActionIcon 
                        variant="subtle" 
                        size="sm" 
                        color="green"
                        onClick={() => handleCopy(rel)}
                        aria-label="Copy relationship"
                      >
                        <IconCopy size={16} />
                      </ActionIcon>
                      <ActionIcon 
                        variant="subtle" 
                        size="sm" 
                        color="red"
                        onClick={() => handleDelete(rel.id)}
                        aria-label="Delete relationship"
                      >
                        <IconTrash size={16} />
                      </ActionIcon>
                    </Group>
                  </Table.Td>
                </Table.Tr>
              ))}
            </Table.Tbody>
          </Table>
        </Paper>
      ) : (
        <Paper p="xl" withBorder style={{ textAlign: 'center' }}>
          <Text c="dimmed">
            {objectTypes.length < 2 
              ? '관계를 만들려면 먼저 2개 이상의 오브젝트 타입을 생성하세요.'
              : '관계가 없습니다. "새 관계"를 클릭하세요.'}
          </Text>
        </Paper>
      )}

      {/* Relationship Types Info */}
      <Paper p="md" withBorder style={{ backgroundColor: 'var(--mantine-color-gray-0)' }}>
        <Text size="sm" fw={600} mb="xs">관계 타입</Text>
        <Stack gap="xs">
          <Group gap="xs">
            <Badge variant="light" color="blue" leftSection={<IconArrowRight size={14} />}>
              one-to-one
            </Badge>
            <Text size="xs" c="dimmed">1:1 관계 (예: User - Profile)</Text>
          </Group>
          <Group gap="xs">
            <Badge variant="light" color="green" leftSection={<IconArrowRightCircle size={14} />}>
              one-to-many
            </Badge>
            <Text size="xs" c="dimmed">1:N 관계 (예: Department - Employees)</Text>
          </Group>
          <Group gap="xs">
            <Badge variant="light" color="violet" leftSection={<IconArrowsExchange size={14} />}>
              many-to-many
            </Badge>
            <Text size="xs" c="dimmed">N:M 관계 (예: Steel Code - Design Parameters)</Text>
          </Group>
        </Stack>
      </Paper>

      {/* Modal for Create */}
      <Modal
        opened={isModalOpen}
        onClose={() => setIsModalOpen(false)}
        title={editingId ? "관계 수정" : "새 관계"}
        size="lg"
      >
        <Stack gap="md">
          <TextInput
            label="Relationship Name"
            placeholder="예: supports, contains, belongs_to"
            value={formName}
            onChange={(e) => setFormName(e.currentTarget.value)}
            required
          />
          <Textarea
            label="Description"
            placeholder="예: Steel Code supports Design Parameters"
            value={formDescription}
            onChange={(e) => setFormDescription(e.currentTarget.value)}
            minRows={2}
          />
          <Select
            label="From Object Type"
            placeholder="Select object type"
            data={objectTypes.map(o => ({ value: o.id, label: o.name }))}
            value={formFromObjectTypeId}
            onChange={setFormFromObjectTypeId}
            required
          />
          <Select
            label="Relationship Type"
            data={[
              { value: 'one-to-one', label: 'One-to-One' },
              { value: 'one-to-many', label: 'One-to-Many' },
              { value: 'many-to-many', label: 'Many-to-Many' },
            ]}
            value={formType}
            onChange={(val) => setFormType(val as any)}
            required
          />
          <Select
            label="To Object Type"
            placeholder="Select object type"
            data={objectTypes.map(o => ({ value: o.id, label: o.name }))}
            value={formToObjectTypeId}
            onChange={setFormToObjectTypeId}
            required
          />

          <Group justify="flex-end" mt="md">
            <Button variant="subtle" onClick={() => setIsModalOpen(false)}>취소</Button>
            <Button 
              onClick={handleSave} 
              disabled={!formName || !formFromObjectTypeId || !formToObjectTypeId}
            >
              저장
            </Button>
          </Group>
        </Stack>
      </Modal>
    </Stack>
  );
}