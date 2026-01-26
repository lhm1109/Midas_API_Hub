/**
 * YAML Rules Loader
 * 
 * schema_definitions/의 YAML 규칙을 로드하여 Designer Config로 변환
 */

import type { DesignerConfig, WidgetPaletteItem, XExtensionConfig } from '../types/designer';

// ============================================================================
// Default Widget Palette (ui-rules.yaml fieldTypeMapping 기반)
// ============================================================================

const DEFAULT_WIDGET_PALETTE: WidgetPaletteItem[] = [
    { type: 'string', component: 'Input', icon: '📝', label: 'Text Input' },
    { type: 'number', component: 'Input', icon: '🔢', label: 'Number' },
    { type: 'integer', component: 'Input', icon: '🔢', label: 'Integer' },
    { type: 'boolean', component: 'Checkbox', icon: '✅', label: 'Checkbox' },
    { type: 'enum', component: 'Select', icon: '📋', label: 'Select / Dropdown' },
    { type: 'array', component: 'Textarea', icon: '📚', label: 'Array' },
    { type: 'object', component: 'Container', icon: '📁', label: 'Object (Nested)' },
    { type: 'section', component: 'SectionHeader', icon: '📦', label: 'Section Header' },
];

// ============================================================================
// Default X-Extensions (schema-validation-rules.yaml allowedExtensions 기반)
// ============================================================================

const DEFAULT_X_EXTENSIONS: XExtensionConfig[] = [
    // Standard
    { name: 'x-ui', category: 'standard', editorType: 'object', description: 'UI 메타데이터 (label, group, hint, visibleWhen)' },
    { name: 'x-transport', category: 'standard', editorType: 'object', description: 'API 전송 설정 (uri, methods, body-root)' },
    { name: 'x-enum-labels', category: 'standard', editorType: 'enum-labels', description: 'Enum 값 레이블' },
    { name: 'x-enum-by-type', category: 'standard', editorType: 'enum-by-type', description: 'TYPE별 다른 enum 값' },
    { name: 'x-enum-labels-by-type', category: 'standard', editorType: 'enum-by-type', description: 'TYPE별 다른 enum 레이블' },
    { name: 'x-value-constraint', category: 'standard', editorType: 'condition', description: 'TYPE별 값 제약' },
    { name: 'x-node-count-by-type', category: 'standard', editorType: 'object', description: 'TYPE별 노드 개수' },
    { name: 'x-exclusive-keys', category: 'standard', editorType: 'array', description: '상호 배타적 키' },

    // UI
    { name: 'x-section-header', category: 'ui', editorType: 'text', description: '섹션 헤더' },
    { name: 'x-ui-hint', category: 'ui', editorType: 'text', description: 'UI 힌트' },
    { name: 'x-ui-group', category: 'ui', editorType: 'text', description: 'UI 그룹' },

    // Validation
    { name: 'x-validation-layer', category: 'validation', editorType: 'object', description: '검증 레이어' },
    { name: 'x-required-when', category: 'validation', editorType: 'condition', description: '조건부 필수' },
];

// ============================================================================
// Default Styles (ui-rules.yaml styling 기반)
// ============================================================================

const DEFAULT_SECTION_STYLE = {
    borderColor: 'border-cyan-800/50',
    textColor: 'text-cyan-400',
    icon: '📋',
};

const DEFAULT_FIELD_STYLE = {
    borderColor: 'border-zinc-700',
    backgroundColor: 'bg-zinc-800',
};

// ============================================================================
// Loader Function
// ============================================================================

/**
 * Designer 설정 로드
 * 
 * 현재는 기본값 사용, 향후 YAML 동적 로드 가능
 */
export async function loadDesignerConfig(_psdSet?: string): Promise<DesignerConfig> {
    // TODO: psdSet에 따라 다른 YAML 파일 로드
    // const yamlPath = `/schema_definitions/${psdSet}/enhanced/ui-rules.yaml`;

    return {
        widgetPalette: DEFAULT_WIDGET_PALETTE,
        xExtensions: DEFAULT_X_EXTENSIONS,
        sectionStyle: DEFAULT_SECTION_STYLE,
        fieldStyle: DEFAULT_FIELD_STYLE,
    };
}

/**
 * 위젯 팔레트 아이템 가져오기
 */
export function getWidgetPalette(): WidgetPaletteItem[] {
    return DEFAULT_WIDGET_PALETTE;
}

/**
 * X-Extension 설정 가져오기
 */
export function getXExtensions(): XExtensionConfig[] {
    return DEFAULT_X_EXTENSIONS;
}

/**
 * 특정 X-Extension 찾기
 */
export function findXExtension(name: string): XExtensionConfig | undefined {
    return DEFAULT_X_EXTENSIONS.find(ext => ext.name === name);
}
