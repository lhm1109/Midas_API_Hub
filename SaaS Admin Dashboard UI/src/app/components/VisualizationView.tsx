import { useEffect, useRef, useState } from 'react';
import { Stack, Group, Text, Paper, Badge, Select, Tabs, SegmentedControl } from '@mantine/core';
import { IconGraph, IconCode } from '@tabler/icons-react';
import cytoscape from 'cytoscape';
import { dataStore, ObjectType, Relationship } from '@/app/store/dataStore';

export function VisualizationView() {
  const cyRef = useRef<HTMLDivElement>(null);
  const cyInstanceRef = useRef<any>(null);
  const [objectTypes] = useState<ObjectType[]>(dataStore.objectTypes);
  const [relationships] = useState<Relationship[]>(dataStore.relationships);
  const [selectedLayout, setSelectedLayout] = useState<string>('cose');
  const [viewMode, setViewMode] = useState<'schema' | 'code-mapping'>('schema');
  const [selectedCode, setSelectedCode] = useState<string>('');

  // Get all RcDesign codes
  const rcDesignRecords = dataStore.dataRecords.filter(r => r.objectTypeId === 'obj-3');
  const allCodes = rcDesignRecords.map(r => ({
    value: r.id,
    label: `${r.values['지원 코드']} (${r.values['패널']})`,
    panel: r.values['패널'] as string,
    code: r.values['지원 코드'] as string,
  }));

  useEffect(() => {
    if (!cyRef.current) return;

    // Cleanup previous instance
    if (cyInstanceRef.current) {
      try {
        const instance = cyInstanceRef.current;
        cyInstanceRef.current = null;
        
        // Stop and destroy synchronously
        if (instance && typeof instance.destroy === 'function') {
          instance.stop();
          instance.removeAllListeners();
          instance.destroy();
        }
      } catch (error) {
        // Silently handle - instance may already be destroyed
      }
    }

    let elements;

    if (viewMode === 'schema') {
      // Original schema view
      const nodes = objectTypes.map(objType => ({
        data: {
          id: objType.id,
          label: objType.name,
          color: objType.color,
          attributes: objType.attributes.length,
          type: 'objectType',
        },
      }));

      const edges = relationships.map(rel => ({
        data: {
          id: rel.id,
          source: rel.fromObjectTypeId,
          target: rel.toObjectTypeId,
          label: rel.name,
          type: rel.type,
        },
      }));

      elements = [...nodes, ...edges];
    } else {
      // Code mapping view
      if (!selectedCode) {
        elements = [];
      } else {
        const codeRecord = rcDesignRecords.find(r => r.id === selectedCode);
        if (!codeRecord) {
          elements = [];
        } else {
          const panel = codeRecord.values['패널'] as string;
          const code = codeRecord.values['지원 코드'] as string;

          // Get all parameters for this panel
          const paramRecords = dataStore.dataRecords.filter(
            r => r.objectTypeId === 'obj-4' && r.values['패널'] === panel
          );

          // Group parameters by section
          const paramsBySection = paramRecords.reduce((acc, param) => {
            const section = param.values['패널 섹션'] as string;
            if (!acc[section]) {
              acc[section] = [];
            }
            acc[section].push(param);
            return acc;
          }, {} as Record<string, any[]>);

          // Create nodes
          const nodes = [
            // Code node (center)
            {
              data: {
                id: `code-${selectedCode}`,
                label: code,
                color: 'orange',
                type: 'code',
              },
            },
            // Panel node
            {
              data: {
                id: `panel-${panel}`,
                label: panel,
                color: 'blue',
                type: 'panel',
              },
            },
            // Section nodes
            ...Object.keys(paramsBySection).map((section, idx) => ({
              data: {
                id: `section-${idx}`,
                label: section,
                color: 'violet',
                type: 'section',
                count: paramsBySection[section].length,
              },
            })),
            // Parameter nodes (sample to avoid overcrowding)
            ...Object.entries(paramsBySection).flatMap(([section, params], sectionIdx) => 
              params.slice(0, 5).map((param, paramIdx) => ({
                data: {
                  id: `param-${sectionIdx}-${paramIdx}`,
                  label: (param.values['파라미터명'] as string).substring(0, 30) + 
                         ((param.values['파라미터명'] as string).length > 30 ? '...' : ''),
                  color: 'green',
                  type: 'parameter',
                  section: section,
                },
              }))
            ),
          ];

          // Create edges
          const edges = [
            // Code to Panel
            {
              data: {
                id: `edge-code-panel`,
                source: `code-${selectedCode}`,
                target: `panel-${panel}`,
                label: 'uses',
              },
            },
            // Panel to Sections
            ...Object.keys(paramsBySection).map((section, idx) => ({
              data: {
                id: `edge-panel-section-${idx}`,
                source: `panel-${panel}`,
                target: `section-${idx}`,
                label: 'has',
              },
            })),
            // Sections to Parameters
            ...Object.entries(paramsBySection).flatMap(([section, params], sectionIdx) => 
              params.slice(0, 5).map((param, paramIdx) => ({
                data: {
                  id: `edge-section-param-${sectionIdx}-${paramIdx}`,
                  source: `section-${sectionIdx}`,
                  target: `param-${sectionIdx}-${paramIdx}`,
                  label: '',
                },
              }))
            ),
          ];

          elements = [...nodes, ...edges];
        }
      }
    }

    // Don't create cytoscape instance if no elements
    if (!elements || elements.length === 0) {
      return;
    }

    try {
      const cy = cytoscape({
        container: cyRef.current,
        elements: elements,
        style: [
          {
            selector: 'node',
            style: {
              'label': 'data(label)',
              'text-valign': 'center',
              'text-halign': 'center',
              'background-color': (ele: any) => {
                const color = ele.data('color');
                const colorMap: Record<string, string> = {
                  blue: '#228be6',
                  green: '#40c057',
                  red: '#fa5252',
                  orange: '#fd7e14',
                  violet: '#7950f2',
                  cyan: '#15aabf',
                  pink: '#e64980',
                  grape: '#be4bdb',
                  teal: '#12b886',
                  lime: '#82c91e',
                };
                return colorMap[color] || '#868e96';
              },
              'color': '#fff',
              'font-size': (ele: any) => {
                const type = ele.data('type');
                if (type === 'code') return '16px';
                if (type === 'panel') return '14px';
                if (type === 'section') return '12px';
                return '10px';
              },
              'font-weight': (ele: any) => {
                const type = ele.data('type');
                return (type === 'code' || type === 'panel') ? 'bold' : 'normal';
              },
              'text-outline-width': 2,
              'text-outline-color': (ele: any) => {
                const color = ele.data('color');
                const colorMap: Record<string, string> = {
                  blue: '#1971c2',
                  green: '#2f9e44',
                  red: '#c92a2a',
                  orange: '#d9480f',
                  violet: '#5f3dc4',
                  cyan: '#0c8599',
                  pink: '#a61e4d',
                  grape: '#862e9c',
                  teal: '#087f5b',
                  lime: '#5c940d',
                };
                return colorMap[color] || '#495057';
              },
              'width': (ele: any) => {
                const type = ele.data('type');
                if (type === 'code') return '100px';
                if (type === 'panel') return '80px';
                if (type === 'section') return '70px';
                return '60px';
              },
              'height': (ele: any) => {
                const type = ele.data('type');
                if (type === 'code') return '100px';
                if (type === 'panel') return '80px';
                if (type === 'section') return '70px';
                return '60px';
              },
              'border-width': 3,
              'border-color': (ele: any) => {
                const color = ele.data('color');
                const colorMap: Record<string, string> = {
                  blue: '#1971c2',
                  green: '#2f9e44',
                  red: '#c92a2a',
                  orange: '#d9480f',
                  violet: '#5f3dc4',
                  cyan: '#0c8599',
                  pink: '#a61e4d',
                  grape: '#862e9c',
                  teal: '#087f5b',
                  lime: '#5c940d',
                };
                return colorMap[color] || '#495057';
              },
            },
          },
          {
            selector: 'edge',
            style: {
              'width': 3,
              'line-color': '#868e96',
              'target-arrow-color': '#868e96',
              'target-arrow-shape': (ele: any) => {
                const type = ele.data('type');
                if (type === 'many-to-many') return 'diamond';
                if (type === 'one-to-many') return 'triangle';
                return 'triangle';
              },
              'curve-style': 'bezier',
              'label': 'data(label)',
              'font-size': '11px',
              'text-rotation': 'autorotate',
              'text-margin-y': -10,
              'color': '#495057',
              'text-background-color': '#fff',
              'text-background-opacity': 0.8,
              'text-background-padding': '3px',
            },
          },
        ],
        layout: {
          name: selectedLayout,
          padding: 50,
          animate: false,
          animationDuration: 0,
          refresh: 0,
        },
      });
      
      cyInstanceRef.current = cy;
    } catch (error) {
      console.error('Error initializing cytoscape:', error);
    }

    return () => {
      const instance = cyInstanceRef.current;
      if (instance) {
        cyInstanceRef.current = null;
        
        try {
          // Check if container still exists in DOM
          if (cyRef.current && document.contains(cyRef.current)) {
            instance.stop();
            instance.removeAllListeners();
            instance.destroy();
          } else {
            // Container removed from DOM, manually clean up
            if (instance.container && instance.container()) {
              // Just remove elements without DOM manipulation
              instance.elements().remove();
            }
            // Force internal cleanup
            if (instance._private) {
              instance._private.renderer = null;
            }
          }
        } catch (error) {
          // Silently handle any cleanup errors
        }
      }
    };
  }, [objectTypes, relationships, selectedLayout, viewMode, selectedCode]);

  return (
    <Stack gap="lg">
      {/* Header */}
      <Group justify="space-between">
        <div>
          <Text size="xl" fw={600}>Visualization</Text>
          <Text size="sm" c="dimmed">
            {viewMode === 'schema' 
              ? '오브젝트 타입과 관계를 그래프로 시각화'
              : '코드별 파라미터 매핑을 그래프로 시각화'}
          </Text>
        </div>
        <Group>
          <Select
            label="Layout"
            data={[
              { value: 'cose', label: 'COSE (Force-directed)' },
              { value: 'circle', label: 'Circle' },
              { value: 'grid', label: 'Grid' },
              { value: 'breadthfirst', label: 'Hierarchical' },
              { value: 'concentric', label: 'Concentric' },
            ]}
            value={selectedLayout}
            onChange={(val) => setSelectedLayout(val || 'cose')}
            style={{ width: 200 }}
            size="sm"
          />
        </Group>
      </Group>

      {/* View Mode Selector */}
      <Paper p="md" withBorder>
        <Group align="flex-end">
          <div style={{ flex: 1 }}>
            <Text size="sm" fw={500} mb="xs">Visualization Mode</Text>
            <SegmentedControl
              value={viewMode}
              onChange={(val) => setViewMode(val as 'schema' | 'code-mapping')}
              data={[
                {
                  label: (
                    <Group gap="xs" style={{ padding: '0 8px' }}>
                      <IconGraph size={16} />
                      <Text size="sm">Schema</Text>
                    </Group>
                  ),
                  value: 'schema',
                },
                {
                  label: (
                    <Group gap="xs" style={{ padding: '0 8px' }}>
                      <IconCode size={16} />
                      <Text size="sm">Code Mapping</Text>
                    </Group>
                  ),
                  value: 'code-mapping',
                },
              ]}
            />
          </div>
          
          {viewMode === 'code-mapping' && (
            <Select
              label="Select Code"
              placeholder="코드를 선택하세요"
              data={allCodes}
              value={selectedCode}
              onChange={(val) => setSelectedCode(val || '')}
              style={{ width: 300 }}
              size="sm"
              searchable
            />
          )}
        </Group>
      </Paper>

      {/* Graph Canvas */}
      <Paper 
        withBorder 
        style={{ 
          height: '600px',
          position: 'relative',
        }}
      >
        {viewMode === 'code-mapping' && !selectedCode ? (
          <div style={{ 
            display: 'flex', 
            alignItems: 'center', 
            justifyContent: 'center', 
            height: '100%',
            color: 'var(--mantine-color-gray-6)',
          }}>
            <Stack align="center" gap="xs">
              <IconCode size={48} stroke={1.5} />
              <Text size="lg" fw={500}>코드를 선택하세요</Text>
              <Text size="sm" c="dimmed">위 드롭다운에서 코드를 선택하면 매핑을 시각화합니다</Text>
            </Stack>
          </div>
        ) : (
          <div 
            ref={cyRef} 
            style={{ 
              width: '100%', 
              height: '100%',
            }} 
          />
        )}
      </Paper>

      {/* Legend */}
      {viewMode === 'schema' ? (
        <Paper p="md" withBorder style={{ backgroundColor: 'var(--mantine-color-gray-0)' }}>
          <Text size="sm" fw={600} mb="md">범례</Text>
          <Group gap="xl">
            <div>
              <Text size="xs" c="dimmed" mb="xs">노드 (Object Types)</Text>
              <Group gap="xs">
                {objectTypes.map(objType => (
                  <Badge key={objType.id} variant="filled" color={objType.color}>
                    {objType.name}
                  </Badge>
                ))}
              </Group>
            </div>
            <div>
              <Text size="xs" c="dimmed" mb="xs">엣지 (Relationships)</Text>
              <Stack gap="xs">
                <Group gap="xs">
                  <div style={{ 
                    width: 30, 
                    height: 3, 
                    backgroundColor: '#868e96',
                    position: 'relative',
                  }}>
                    <div style={{
                      position: 'absolute',
                      right: -6,
                      top: -4,
                      width: 0,
                      height: 0,
                      borderLeft: '6px solid #868e96',
                      borderTop: '5px solid transparent',
                      borderBottom: '5px solid transparent',
                    }} />
                  </div>
                  <Text size="xs">one-to-many</Text>
                </Group>
                <Group gap="xs">
                  <div style={{ 
                    width: 30, 
                    height: 3, 
                    backgroundColor: '#868e96',
                    position: 'relative',
                  }}>
                    <div style={{
                      position: 'absolute',
                      right: -4,
                      top: -4,
                      width: 8,
                      height: 8,
                      backgroundColor: '#868e96',
                      transform: 'rotate(45deg)',
                    }} />
                  </div>
                  <Text size="xs">many-to-many</Text>
                </Group>
              </Stack>
            </div>
          </Group>
        </Paper>
      ) : (
        <Paper p="md" withBorder style={{ backgroundColor: 'var(--mantine-color-gray-0)' }}>
          <Text size="sm" fw={600} mb="md">노드 타입</Text>
          <Group gap="xl">
            <Group gap="xs">
              <Badge variant="filled" color="orange" size="lg">Code</Badge>
              <Text size="xs" c="dimmed">설계 코드</Text>
            </Group>
            <Group gap="xs">
              <Badge variant="filled" color="blue" size="lg">Panel</Badge>
              <Text size="xs" c="dimmed">패널</Text>
            </Group>
            <Group gap="xs">
              <Badge variant="filled" color="violet" size="lg">Section</Badge>
              <Text size="xs" c="dimmed">섹션</Text>
            </Group>
            <Group gap="xs">
              <Badge variant="filled" color="green" size="lg">Parameter</Badge>
              <Text size="xs" c="dimmed">파라미터 (샘플)</Text>
            </Group>
          </Group>
          <Text size="xs" c="dimmed" mt="md">
            * 각 섹션당 최대 5개의 파라미터만 표시됩니다 (시각화 성능 최적화)
          </Text>
        </Paper>
      )}

      {/* Stats */}
      {viewMode === 'schema' ? (
        <Paper p="md" withBorder>
          <Group gap="xl">
            <div>
              <Text size="xs" c="dimmed">Object Types</Text>
              <Text size="lg" fw={600}>{objectTypes.length}</Text>
            </div>
            <div>
              <Text size="xs" c="dimmed">Relationships</Text>
              <Text size="lg" fw={600}>{relationships.length}</Text>
            </div>
            <div>
              <Text size="xs" c="dimmed">Total Attributes</Text>
              <Text size="lg" fw={600}>
                {objectTypes.reduce((sum, obj) => sum + obj.attributes.length, 0)}
              </Text>
            </div>
          </Group>
        </Paper>
      ) : selectedCode && (
        <Paper p="md" withBorder>
          <Group gap="xl">
            {(() => {
              const codeRecord = rcDesignRecords.find(r => r.id === selectedCode);
              if (!codeRecord) return null;
              
              const panel = codeRecord.values['패널'] as string;
              const paramRecords = dataStore.dataRecords.filter(
                r => r.objectTypeId === 'obj-4' && r.values['패널'] === panel
              );
              
              const sections = new Set(paramRecords.map(p => p.values['패널 섹션']));
              
              return (
                <>
                  <div>
                    <Text size="xs" c="dimmed">Panel</Text>
                    <Text size="lg" fw={600}>{panel}</Text>
                  </div>
                  <div>
                    <Text size="xs" c="dimmed">Sections</Text>
                    <Text size="lg" fw={600}>{sections.size}</Text>
                  </div>
                  <div>
                    <Text size="xs" c="dimmed">Total Parameters</Text>
                    <Text size="lg" fw={600}>{paramRecords.length}</Text>
                  </div>
                  <div>
                    <Text size="xs" c="dimmed">Code Name</Text>
                    <Text size="lg" fw={600}>{codeRecord.values['코드명']}</Text>
                  </div>
                </>
              );
            })()}
          </Group>
        </Paper>
      )}
    </Stack>
  );
}