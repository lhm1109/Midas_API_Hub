/**
 * Schema Builder Feature
 * 
 * Standalone Schema Builder with persistent state
 */

// Pages
export { SchemaBuilderPage } from './pages/SchemaBuilderPage';

// Components (re-export from schema-designer)
export { ComponentPalette } from '../schema-designer/components/ComponentPalette';
export { CanvasArea } from '../schema-designer/components/CanvasArea';
export { PropertyPanel } from '../schema-designer/components/PropertyPanel';

// Store
export { useDesignerStore } from '../schema-designer/stores/designerStore';

// Lib
export { layoutToSchema, layoutToSchemaString } from '../schema-designer/lib/layoutToSchema';
export { schemaToLayout, parseSchemaToLayout } from '../schema-designer/lib/schemaToLayout';

// Types
export type {
    DesignedField as DesignedFieldType,
    DesignedSection,
    DesignerConfig,
    FieldType,
    EnhancedSchemaOutput,
} from '../schema-designer/types/designer';
