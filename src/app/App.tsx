import { useState, useEffect } from 'react';
import { GlobalSidebar } from './components/GlobalSidebar';
import { APIListPanel, type ApiEndpoint, type ApiProduct } from './components/APIListPanel';
import { MainWorkspace } from './components/MainWorkspace';
import { HistoryView } from './components/HistoryView';
import { SettingsModal } from './components/SettingsModal';
import { useAppStore } from './store/useAppStore';

// Mock API data structure
const mockApiData: ApiProduct[] = [
  {
    id: 'civil-nx',
    name: 'Civil NX',
    groups: [
      {
        id: 'db',
        name: 'DB',
        endpoints: [
          {
            id: 'node',
            name: 'Node',
            method: 'POST',
            path: '/db/node',
            status: 'success',
          },
          {
            id: 'beam',
            name: 'Beam',
            method: 'POST',
            path: '/db/beam',
            status: null,
          },
          {
            id: 'nlct',
            name: 'NLCT',
            method: 'POST',
            path: '/db/nlct',
            status: 'success',
          },
          {
            id: 'mvct',
            name: 'MVCT',
            method: 'POST',
            path: '/db/mvct',
            status: null,
          },
        ],
      },
      {
        id: 'gen',
        name: 'Gen',
        endpoints: [
          {
            id: 'project',
            name: 'Project',
            method: 'GET',
            path: '/gen/project',
            status: null,
          },
          {
            id: 'material',
            name: 'Material',
            method: 'POST',
            path: '/gen/material',
            status: null,
          },
        ],
      },
    ],
  },
  {
    id: 'gen-nx',
    name: 'Gen NX',
    groups: [
      {
        id: 'analysis',
        name: 'Analysis',
        endpoints: [
          {
            id: 'static',
            name: 'Static',
            method: 'POST',
            path: '/analysis/static',
            status: null,
          },
          {
            id: 'dynamic',
            name: 'Dynamic',
            method: 'POST',
            path: '/analysis/dynamic',
            status: null,
          },
        ],
      },
    ],
  },
];

// Default request bodies for each endpoint
const defaultRequestBodies: Record<string, string> = {
  nlct: JSON.stringify(
    {
      Assign: {
        '1': {
          NONLINEAR_TYPE: 'GEOM+MATL',
          ITERATION_METHOD: 'NEWTON',
          NUMBER_STEPS: 1,
          MAX_ITERATIONS: 30,
          OPT_ENERGY_NORM: true,
          ENERGY_NORM: 0.001,
          OPT_DISPLACEMENT_NORM: true,
          DISPLACEMENT_NORM: 0.001,
          OPT_FORCE_NORM: true,
          FORCE_NORM: 0.001,
          NEWTON_ITEMS: [
            {
              ITERATION_METHOD: 'NEWTON',
              LCNAME: 'A',
              NUMBER_STEPS: 1,
              MAX_ITERATIONS: 30,
              LOAD_FACTORS: [1],
            },
          ],
          DISPCT_ITEMS: [
            {
              ITERATION_METHOD: 'DISP',
              LCNAME: 'B',
              NUMBER_STEPS: 1,
              MAX_ITERATIONS: 10,
              MASTER_NODE: 1,
              DIRECTION: 0,
              MAXIMUM_DISPLACEMENT: 0.1,
              LOAD_FACTORS: [1],
            },
          ],
        },
      },
    },
    null,
    2
  ),
  mvct: JSON.stringify(
    {
      Assign: {
        '1': {
          CODE: 'AASHTO',
          NUMLN: 2,
          LNSPAC: 1.8,
          TRKLNWID: 3.0,
          SNGMOV: 'YES',
        },
      },
    },
    null,
    2
  ),
  default: JSON.stringify({}, null, 2),
};

export default function App() {
  const { setRunnerData } = useAppStore();
  const [activeView, setActiveView] = useState<'projects' | 'history'>('projects');
  const [settingsOpen, setSettingsOpen] = useState(false);
  const [selectedEndpoint, setSelectedEndpoint] = useState<ApiEndpoint | null>(
    mockApiData[0].groups[0].endpoints[2] // NLCT by default
  );
  const [settings, setSettings] = useState({
    baseUrl: 'https://api-beta.midasit.com/civil',
    mapiKey: '',
    commonHeaders: JSON.stringify(
      {
        'Content-Type': 'application/json',
        Accept: 'application/json',
      },
      null,
      2
    ),
  });

  // Initialize runnerData on mount
  useEffect(() => {
    setRunnerData({
      requestBody: '{}',
      responseBody: '',
      testCases: [],
    });
  }, [setRunnerData]);

  const handleEndpointSelect = (endpoint: ApiEndpoint) => {
    setSelectedEndpoint(endpoint);
  };

  return (
    <div className="size-full flex bg-zinc-950 text-zinc-100 overflow-hidden">
      {/* 1. Global Sidebar */}
      <GlobalSidebar
        activeView={activeView}
        onViewChange={setActiveView}
        onSettingsClick={() => setSettingsOpen(true)}
      />

      {/* 2. API List Panel (only show in projects view) */}
      {activeView === 'projects' && (
        <APIListPanel
          products={mockApiData}
          selectedEndpoint={selectedEndpoint?.id || null}
          onEndpointSelect={handleEndpointSelect}
        />
      )}

      {/* 3. Main Content Area */}
      {activeView === 'projects' ? (
        <MainWorkspace
          endpoint={selectedEndpoint}
          baseUrl={settings.baseUrl}
        />
      ) : (
        <HistoryView />
      )}

      {/* Settings Modal */}
      <SettingsModal
        open={settingsOpen}
        onOpenChange={setSettingsOpen}
        settings={settings}
        onSettingsChange={setSettings}
      />
    </div>
  );
}