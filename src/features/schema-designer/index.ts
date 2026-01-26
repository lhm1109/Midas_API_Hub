/**
 * Schema Designer Feature
 * 
 * Visual JSON Schema Builder
 */

// Components
export { SchemaDesigner } from './components/SchemaDesigner';
export { ComponentPalette } from './components/ComponentPalette';
export { CanvasArea } from './components/CanvasArea';
export { PropertyPanel } from './components/PropertyPanel';
export { DesignedField } from './components/DesignedField';

// Store
export { useDesignerStore, selectSelectedField, selectFieldById } from './stores/designerStore';

// Lib
export { loadDesignerConfig, getWidgetPalette, getXExtensions } from './lib/yamlRulesLoader';
export { schemaToLayout, parseSchemaToLayout } from './lib/schemaToLayout';
export { layoutToSchema, layoutToSchemaString } from './lib/layoutToSchema';

// Types
export type {
    DesignedField as DesignedFieldType,
    DesignedSection,
    DesignerConfig,
    WidgetPaletteItem,
    XExtensionConfig,
    FieldType,
    EnhancedSchemaOutput,
} from './types/designer';
