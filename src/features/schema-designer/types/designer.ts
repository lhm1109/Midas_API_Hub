/**
 * Visual Schema Designer Types
 * 
 * YAML 규칙 기반 스키마 디자이너 타입 정의
 */

// ============================================================================
// Field Types
// ============================================================================

export type FieldType = 'string' | 'number' | 'integer' | 'boolean' | 'array' | 'object' | 'enum';

export interface DesignedField {
    id: string;
    key: string;
    type: FieldType;

    // 기본 JSON Schema 속성
    title?: string;
    description?: string;
    enum?: (string | number)[];
    default?: any;
    minimum?: number;
    maximum?: number;
    minLength?: number;
    maxLength?: number;
    pattern?: string;
    format?: string;

    // Array 전용
    items?: {
        type: string;
        properties?: Record<string, any>;
    };
    minItems?: number;
    maxItems?: number;

    // x-ui 확장
    xui?: {
        label?: string;
        group?: string;
        hint?: string;
        widget?: 'text' | 'textarea' | 'date' | 'radio' | 'dropdown';  // UI 위젯 타입
        visibleWhen?: Record<string, any[]>;
    };

    // x-enum 확장
    xenumLabels?: Record<string, string>;
    xenumByType?: Record<string, any[]>;
    xenumLabelsByType?: Record<string, Record<string, string>>;

    // x-value-constraint
    xvalueConstraint?: Record<string, any>;

    // x-node-count-by-type
    xnodeCountByType?: Record<string, number>;

    // x-exclusive-keys
    xexclusiveKeys?: string[];

    // 필수 여부
    required?: boolean;
    xrequiredWhen?: Record<string, any[]>;

    // 중첩 구조
    children?: DesignedField[];

    // 레이아웃 (행 기반 동적 레이아웃)
    layout?: {
        rowId: string;        // 속한 행 ID (같은 rowId = 같은 행의 컬럼들)
        columnIndex: number;  // 행 내 순서 (0, 1, 2...)
    };
}

// ============================================================================
// Row Layout (행 기반 그리드)
// ============================================================================

export interface DesignerRow {
    id: string;
    fieldIds: string[];  // 이 행에 속한 필드 ID 목록 (순서 유지)
}

// ============================================================================
// Section (x-ui.group 기반)
// ============================================================================

export interface DesignedSection {
    id: string;
    name: string;
    fields: DesignedField[];
    order: number;
}

// ============================================================================
// Designer Config (YAML 기반)
// ============================================================================

export interface WidgetPaletteItem {
    type: FieldType | 'section';
    component: string;
    icon: string;
    label: string;
    props?: Record<string, any>;
}

export interface XExtensionConfig {
    name: string;
    category: 'standard' | 'ui' | 'validation';
    editorType: 'text' | 'object' | 'array' | 'condition' | 'enum-labels' | 'enum-by-type';
    description?: string;
}

export interface DesignerConfig {
    // fieldTypeMapping → 위젯 팔레트
    widgetPalette: WidgetPaletteItem[];

    // allowedExtensions → x-* 속성 편집 UI
    xExtensions: XExtensionConfig[];

    // layout → 섹션/그룹 스타일
    sectionStyle: {
        borderColor: string;
        textColor: string;
        icon: string;
    };

    // 필드 스타일
    fieldStyle: {
        borderColor: string;
        backgroundColor: string;
    };
}

// ============================================================================
// Designer State
// ============================================================================

export interface DesignerState {
    fields: DesignedField[];
    selectedFieldId: string | null;
    sections: DesignedSection[];
    config: DesignerConfig | null;
    isDirty: boolean;

    // Transport 설정
    transport?: {
        uri?: string;
        methods?: string[];
        bodyRoot?: string;
    };
}

// ============================================================================
// Enhanced Schema (출력용)
// ============================================================================

export interface EnhancedSchemaProperty {
    type: string;
    enum?: (string | number)[];
    default?: any;
    minimum?: number;
    maximum?: number;
    minLength?: number;
    maxLength?: number;
    pattern?: string;
    format?: string;
    items?: any;
    minItems?: number;
    maxItems?: number;
    description?: string;
    properties?: Record<string, EnhancedSchemaProperty>;
    'x-ui'?: {
        label?: string;
        group?: string;
        hint?: string;
        visibleWhen?: Record<string, any[]>;
    };
    'x-enum-labels'?: Record<string, string>;
    'x-enum-by-type'?: Record<string, any[]>;
    'x-enum-labels-by-type'?: Record<string, Record<string, string>>;
    'x-value-constraint'?: Record<string, any>;
    'x-node-count-by-type'?: Record<string, number>;
    'x-required-when'?: Record<string, any[]>;
}

export interface EnhancedSchemaOutput {
    $schema?: string;
    type: 'object';
    title?: string;
    description?: string;
    properties: Record<string, EnhancedSchemaProperty>;
    required?: string[];
    'x-transport'?: {
        uri?: string;
        methods?: string[];
        'body-root'?: string;
    };
    'x-exclusive-keys'?: string[];
    allOf?: any[];
}
