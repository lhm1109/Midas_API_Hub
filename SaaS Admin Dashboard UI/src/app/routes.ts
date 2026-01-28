import { createBrowserRouter } from 'react-router';
import { DashboardLayout } from '@/app/components/DashboardLayout';
import { ObjectTypesView } from '@/app/components/ObjectTypesView';
import { DataEntryView } from '@/app/components/DataEntryView';
import { RelationshipsView } from '@/app/components/RelationshipsView';
import { VisualizationView } from '@/app/components/VisualizationView';
import { PanelMappingView } from '@/app/components/PanelMappingView';

export const router = createBrowserRouter([
  {
    path: "/",
    Component: DashboardLayout,
    children: [
      { index: true, Component: ObjectTypesView },
      { path: "data", Component: DataEntryView },
      { path: "relationships", Component: RelationshipsView },
      { path: "visualization", Component: VisualizationView },
      { path: "panel-mapping", Component: PanelMappingView },
    ],
  },
]);
