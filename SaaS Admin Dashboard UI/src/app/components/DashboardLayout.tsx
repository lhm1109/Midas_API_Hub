import { AppShell, Stack, NavLink, Group, Text } from '@mantine/core';
import { IconDatabase, IconTable, IconLink, IconGraph, IconLayoutGrid } from '@tabler/icons-react';
import { Outlet, useNavigate, useLocation } from 'react-router';

export function DashboardLayout() {
  const navigate = useNavigate();
  const location = useLocation();

  const activeRoute = location.pathname === '/' ? 'objects' 
    : location.pathname.slice(1);

  return (
    <AppShell
      navbar={{ width: 280, breakpoint: 'sm' }}
      padding="lg"
      styles={{
        main: {
          backgroundColor: 'var(--mantine-color-gray-0)',
        },
      }}
    >
      <AppShell.Navbar p="md" style={{ borderRight: '1px solid var(--mantine-color-gray-3)' }}>
        <Stack gap="md">
          <div>
            <Text size="xl" fw={700} mb="xs">Data Model Manager</Text>
            <Text size="xs" c="dimmed">범용 데이터 & 관계 정의 시스템</Text>
          </div>

          <div>
            <Text size="xs" c="dimmed" tt="uppercase" fw={600} mb="xs">
              Configuration
            </Text>
            <Stack gap="xs">
              <NavLink
                label="Object Types"
                description="오브젝트 타입 & 속성 정의"
                leftSection={<IconDatabase size={18} />}
                active={activeRoute === 'objects'}
                onClick={() => navigate('/')}
                styles={{
                  root: { borderRadius: 6 },
                  label: { fontSize: '14px' },
                  description: { fontSize: '11px' },
                }}
              />
              <NavLink
                label="Data Entry"
                description="실제 데이터 입력 (CSV 지원)"
                leftSection={<IconTable size={18} />}
                active={activeRoute === 'data'}
                onClick={() => navigate('/data')}
                styles={{
                  root: { borderRadius: 6 },
                  label: { fontSize: '14px' },
                  description: { fontSize: '11px' },
                }}
              />
              <NavLink
                label="Relationships"
                description="오브젝트 간 관계 정의"
                leftSection={<IconLink size={18} />}
                active={activeRoute === 'relationships'}
                onClick={() => navigate('/relationships')}
                styles={{
                  root: { borderRadius: 6 },
                  label: { fontSize: '14px' },
                  description: { fontSize: '11px' },
                }}
              />
              <NavLink
                label="Panel Mapping"
                description="패널별 코드 & 파라미터 매칭"
                leftSection={<IconLayoutGrid size={18} />}
                active={activeRoute === 'panel-mapping'}
                onClick={() => navigate('/panel-mapping')}
                styles={{
                  root: { borderRadius: 6 },
                  label: { fontSize: '14px' },
                  description: { fontSize: '11px' },
                }}
              />
              <NavLink
                label="Visualization"
                description="관계 그래프 시각화"
                leftSection={<IconGraph size={18} />}
                active={activeRoute === 'visualization'}
                onClick={() => navigate('/visualization')}
                styles={{
                  root: { borderRadius: 6 },
                  label: { fontSize: '14px' },
                  description: { fontSize: '11px' },
                }}
              />
            </Stack>
          </div>
        </Stack>
      </AppShell.Navbar>

      <AppShell.Main>
        <Outlet />
      </AppShell.Main>
    </AppShell>
  );
}