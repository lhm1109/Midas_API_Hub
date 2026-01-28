import { useState, useEffect, useRef } from 'react';
import { 
  Stack, 
  Group, 
  Text, 
  Paper,
  Select,
  Checkbox,
  Badge,
  Grid,
} from '@mantine/core';
import { IconCheck, IconAlertCircle } from '@tabler/icons-react';
import cytoscape from 'cytoscape';

// Mock data
const mockResults = [
  { value: 'RESULT-001', label: 'Final Product Assembly' },
  { value: 'RESULT-002', label: 'Quality Certification' },
  { value: 'RESULT-003', label: 'Packaged Product' },
];

const mockRequiredSteps = [
  { id: 'STEP-001', name: 'Material Check', completed: true },
  { id: 'STEP-002', name: 'Tool Setup', completed: true },
  { id: 'STEP-003', name: 'Primary Operation', completed: true },
  { id: 'STEP-004', name: 'Quality Check', completed: false },
  { id: 'STEP-006', name: 'Final Inspection', completed: false },
  { id: 'STEP-007', name: 'Documentation', completed: true },
];

export function ResultCentricView() {
  const [selectedResult, setSelectedResult] = useState<string | null>('RESULT-001');
  const cyRef = useRef<HTMLDivElement>(null);
  const cyInstance = useRef<cytoscape.Core | null>(null);

  useEffect(() => {
    if (!cyRef.current) return;

    // Initialize Cytoscape
    const cy = cytoscape({
      container: cyRef.current,
      style: [
        {
          selector: 'node',
          style: {
            'background-color': '#fff',
            'border-width': 2,
            'border-color': '#ddd',
            'label': 'data(label)',
            'text-valign': 'center',
            'text-halign': 'center',
            'font-size': '12px',
            'width': 100,
            'height': 40,
            'shape': 'roundrectangle',
          }
        },
        {
          selector: 'node.result',
          style: {
            'background-color': '#228be6',
            'border-color': '#1c7ed6',
            'color': '#fff',
            'width': 120,
            'height': 50,
            'font-weight': 'bold',
          }
        },
        {
          selector: 'node.required',
          style: {
            'background-color': '#51cf66',
            'border-color': '#40c057',
          }
        },
        {
          selector: 'node.excluded',
          style: {
            'background-color': '#fafafa',
            'border-color': '#e9ecef',
            'color': '#adb5bd',
            'opacity': 0.5,
          }
        },
        {
          selector: 'node.missing',
          style: {
            'background-color': '#ff6b6b',
            'border-color': '#fa5252',
            'color': '#fff',
          }
        },
        {
          selector: 'edge',
          style: {
            'width': 2,
            'line-color': '#dee2e6',
            'target-arrow-color': '#dee2e6',
            'target-arrow-shape': 'triangle',
            'curve-style': 'bezier',
            'label': 'data(label)',
            'font-size': '10px',
            'text-rotation': 'autorotate',
          }
        },
        {
          selector: 'edge.dependency',
          style: {
            'line-color': '#228be6',
            'target-arrow-color': '#228be6',
          }
        },
        {
          selector: 'edge.excluded',
          style: {
            'line-style': 'dashed',
            'opacity': 0.3,
          }
        },
      ],
      elements: [
        // Nodes
        { data: { id: 'result', label: 'Final Product\nAssembly' }, classes: 'result' },
        { data: { id: 'step1', label: 'Material\nCheck' }, classes: 'required' },
        { data: { id: 'step2', label: 'Tool Setup' }, classes: 'required' },
        { data: { id: 'step3', label: 'Primary\nOperation' }, classes: 'required' },
        { data: { id: 'step4', label: 'Quality\nCheck' }, classes: 'missing' },
        { data: { id: 'step5', label: 'Secondary\nOperation' }, classes: 'excluded' },
        { data: { id: 'step6', label: 'Final\nInspection' }, classes: 'missing' },
        { data: { id: 'step7', label: 'Documentation' }, classes: 'required' },
        
        // Edges
        { data: { source: 'step1', target: 'step2', label: 'dependency' }, classes: 'dependency' },
        { data: { source: 'step2', target: 'step3', label: 'dependency' }, classes: 'dependency' },
        { data: { source: 'step3', target: 'step4', label: 'requires' }, classes: 'dependency' },
        { data: { source: 'step3', target: 'step5', label: 'optional' }, classes: 'excluded' },
        { data: { source: 'step4', target: 'step6', label: 'dependency' }, classes: 'dependency' },
        { data: { source: 'step6', target: 'result', label: 'produces' }, classes: 'dependency' },
        { data: { source: 'step3', target: 'step7', label: 'documents' }, classes: 'dependency' },
        { data: { source: 'step7', target: 'result', label: 'required for' }, classes: 'dependency' },
      ],
      layout: {
        name: 'breadthfirst',
        directed: true,
        padding: 50,
        spacingFactor: 1.5,
      },
      userZoomingEnabled: true,
      userPanningEnabled: true,
      boxSelectionEnabled: false,
    });

    cyInstance.current = cy;

    return () => {
      cy.destroy();
    };
  }, [selectedResult]);

  const completedCount = mockRequiredSteps.filter(s => s.completed).length;
  const totalCount = mockRequiredSteps.length;

  return (
    <Stack gap="md">
      {/* Header */}
      <Group justify="space-between">
        <div>
          <Text size="xl" fw={600}>Result-Centric View</Text>
          <Text size="sm" c="dimmed">Visualize step dependencies and requirements for a specific result</Text>
        </div>
        <Badge 
          size="lg" 
          color={completedCount === totalCount ? 'green' : 'orange'}
          leftSection={completedCount === totalCount ? <IconCheck size={14} /> : <IconAlertCircle size={14} />}
        >
          {completedCount} / {totalCount} Steps Complete
        </Badge>
      </Group>

      {/* Result Selector */}
      <Paper p="md" withBorder>
        <Group>
          <Text size="sm" fw={500}>Result:</Text>
          <Select
            value={selectedResult}
            onChange={setSelectedResult}
            data={mockResults}
            style={{ width: 300 }}
          />
        </Group>
      </Paper>

      {/* Main Content Grid */}
      <Grid>
        {/* Required Steps Checklist */}
        <Grid.Col span={3}>
          <Paper p="md" withBorder h="100%">
            <Text size="sm" fw={600} mb="md">Required Steps</Text>
            <Stack gap="xs">
              {mockRequiredSteps.map((step) => (
                <Checkbox
                  key={step.id}
                  label={
                    <Group gap="xs" wrap="nowrap">
                      <Text size="sm" ff="monospace" c="dimmed" style={{ minWidth: 80 }}>
                        {step.id}
                      </Text>
                      <Text size="sm">{step.name}</Text>
                    </Group>
                  }
                  checked={step.completed}
                  readOnly
                  styles={{
                    label: { cursor: 'default' },
                  }}
                />
              ))}
            </Stack>

            <Paper p="sm" mt="md" style={{ backgroundColor: 'var(--mantine-color-gray-0)' }}>
              <Text size="xs" fw={600} mb="xs">Missing Steps</Text>
              {mockRequiredSteps
                .filter(s => !s.completed)
                .map(s => (
                  <Group key={s.id} gap="xs" mb={4}>
                    <IconAlertCircle size={14} color="var(--mantine-color-red-6)" />
                    <Text size="xs" c="red">{s.name}</Text>
                  </Group>
                ))}
            </Paper>
          </Paper>
        </Grid.Col>

        {/* Graph Visualization */}
        <Grid.Col span={9}>
          <Paper p="md" withBorder>
            <Text size="sm" fw={600} mb="md">Dependency Graph (Read-Only)</Text>
            <div 
              ref={cyRef} 
              style={{ 
                width: '100%', 
                height: '500px',
                border: '1px solid var(--mantine-color-gray-3)',
                borderRadius: '4px',
                backgroundColor: '#fafafa',
              }} 
            />
            
            {/* Legend */}
            <Group mt="md" gap="xl">
              <Group gap="xs">
                <div style={{ 
                  width: 20, 
                  height: 20, 
                  backgroundColor: '#228be6', 
                  borderRadius: 4,
                  border: '2px solid #1c7ed6',
                }} />
                <Text size="xs">Result Node</Text>
              </Group>
              <Group gap="xs">
                <div style={{ 
                  width: 20, 
                  height: 20, 
                  backgroundColor: '#51cf66', 
                  borderRadius: 4,
                  border: '2px solid #40c057',
                }} />
                <Text size="xs">Required & Complete</Text>
              </Group>
              <Group gap="xs">
                <div style={{ 
                  width: 20, 
                  height: 20, 
                  backgroundColor: '#ff6b6b', 
                  borderRadius: 4,
                  border: '2px solid #fa5252',
                }} />
                <Text size="xs">Missing Step</Text>
              </Group>
              <Group gap="xs">
                <div style={{ 
                  width: 20, 
                  height: 20, 
                  backgroundColor: '#fafafa', 
                  borderRadius: 4,
                  border: '2px solid #e9ecef',
                  opacity: 0.5,
                }} />
                <Text size="xs">Excluded Step</Text>
              </Group>
            </Group>
          </Paper>
        </Grid.Col>
      </Grid>
    </Stack>
  );
}
