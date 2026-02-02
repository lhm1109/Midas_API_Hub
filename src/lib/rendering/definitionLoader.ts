/**
 * YAML Schema Definition Loader
 * YAML 정의 파일을 로드하고 파싱합니다.
 */

import yaml from 'js-yaml';

export type DefinitionType = 'original' | 'enhanced';

export interface UIRulesDefinition {
  version: string;
  type: string;
  fieldTypeMapping: Record<string, any>;
  layout: any;
  rendering: any;
  validation: any;
  conditionalRendering: any;
  styling: any;
  enhancedFeatures?: any;  // deprecated, use markerInterpretation
  markerInterpretation?: any;  // 🔥 NEW: x-* 마커 UI 해석 규칙
  legacyMarkers?: any;  // 🔥 NEW: deprecated 마커 호환성
}


export interface BuilderDefinition extends UIRulesDefinition {
  formLayout: any;
  fieldRendering: any;
  inputBehavior: any;
  instanceManagement: any;
  jsonPreview: any;
  testCases: any;
  validationMessages: any;
  sectionHeaders?: any;
  hintsDisplay?: any;
  dynamicBehavior?: any;
  enhancedSchemaMarkers?: string[];
  wrapperRules?: Array<{
    pattern: string;
    wrapper: string;
    description?: string;
    priority?: number;  // 🔥 NEW
  }>;
  wrapperPriorityDefault?: number;  // 🔥 NEW: shared.yaml에서 로드
}

export interface TableDefinition extends UIRulesDefinition {
  // 🔥 NEW: Schema Extensions (확장 필드 정의)
  schemaExtensions?: {
    conditional?: Array<{
      key: string;
      description?: string;
      format?: string;
      displayInTable?: boolean;
      displayIcon?: string;
      displayLabel?: string;
      displayColor?: string;
    }>;
    ui?: Array<{
      key: string;
      description?: string;
      displayInTable?: boolean;
      displayIcon?: string;
      displayLabel?: string;
      displayColor?: string;
    }>;
    extractFromSchema?: Array<{
      key: string;
      targetProperty: string;
      passthrough?: boolean;
      transformKey?: boolean;
    }>;
  };

  // 🔥 NEW: Condition Rows (조건부 필드 표시 행)
  conditionRows?: {
    enabled?: boolean;
    style?: {
      background?: string;
      textColor?: string;
      fontSize?: string;
      fontWeight?: string;
      padding?: string;
    };
    colspan?: number;
  };

  tableStructure: any;
  sectionHeaders: any;
  rowRendering: any;
  nestedFields: any;
}

export interface HTMLTemplateDefinition {
  version: string;
  document: any;
  templates: any;
  table: any;
  featureBoxes: any;
  css: any;
}

// ============================================================================
// NEW: 통합 규칙 정의 (3파일 체계)
// ============================================================================

export interface SharedRulesDefinition {
  // 🔥 v1.5: SSOT 100점 구조
  versioning: {
    rulesSpecVersion: string;
    inputSpec: {
      schemaDraft: string;
      min: string;
      max: string;
    };
  };
  unknownPolicy: any;
  defaultHandlers: {
    typeInferenceFallback: { type: string; warn: boolean };
    missingXui: { label: string; warn: boolean };
    wrapperPriorityDefault: number;
  };
  // 🔥 v1.4: object 구조로 변경
  precedence: {
    order: string[];
    merge: {
      sameKey: 'override' | 'merge';
      list: 'replace' | 'concat';
      missingKey: 'inherit' | 'ignore';
    };
  };
  // 🔥 v1.5: integrityRules 추가
  integrityRules: {
    requireMarkerIdInRegistry: boolean;
    requireSectionIdInRegistry: boolean;
    requireConditionTypeInRegistry: boolean;
    requireXuiSectionIdInRegistry: boolean;
  };
  markerRegistry: Array<{
    id: string;
    key: string;
    description?: string;
    pureUI?: boolean;  // 🔥 NEW: true = 삭제해도 검증 영향 없음
    schema?: any;  // 마커 값 스키마
    example?: string;  // 사용 예시
  }>;
  // 🔥 v1.4: conditionRegistry 전역 이동
  conditionRegistry: Array<{
    type: string;
    requiredParams: string[];
    description?: string;
  }>;
  // 🔥 v1.4: sectionRegistry 추가
  sectionRegistry: Array<{
    id: string;
    name: string;
    description?: string;
    isDefault?: boolean;
  }>;
  // 🔥 v1.4: wrapperRegistryPolicy 추가
  wrapperRegistryPolicy: {
    sort: 'priorityDescStable' | 'priorityAscStable';
    match: 'first' | 'all';
  };
  wrapperRegistry: Array<{
    id: string;
    pattern: string;
    wrapper: string | null;
    description?: string;
    priority?: number;
  }>;
  typeInferenceRegistry: Array<{ id: string; prefix: string; type: string; example?: string }>;
  componentRegistry: Record<string, { component: string; props?: any }>;
  // 🔥 v1.4: prefixSource 추가
  naming: {
    yamlKeyPolicy: { min: number; max: number; style: string };
    fieldNamePolicy: { prefixSource?: string; allowUppercase: boolean; maxLength: number };
    conflict: { strategy: string; format: string };
  };
  diagnostics: {
    errors: Array<{ code: string; level: string; message: string; fix?: string }>;
    warnings: Array<{ code: string; level: string; message: string; fix?: string }>;
  };
  outputMeta: {
    defaults: { required: boolean; onMissing: 'warn' | 'error' | 'null' };
    fields: Array<{ alias: string; source: string; required?: boolean; onMissing?: string }>;
  };
  schemaLogic: any;
  // 🔥 v1.6: schemaStructurePatterns 추가
  schemaStructurePatterns?: {
    wrapperKeys: Array<{
      key: string;
      description?: string;
      example?: string;
    }>;
    entityCollectionPattern: {
      detect: Array<{ path: string; value?: any; exists?: boolean }>;
      entityPath: string;
      fieldExtractionRules: {
        properties: string;
        required: string;
        allOf: string;
        xUi: string;
      };
    };
    conditionalValidation: {
      pattern: string;
      ifStructure: { properties: string; required: string };
      thenStructure: { required: string; properties: string };
    };
    conditionalRequiredMapping: {
      conditionField: string;
      multiCondition: boolean;
    };
    conditionalEnumMapping: {
      extractFrom: string;
    };
    conditionalNodeCountMapping: {
      extractFrom: string;
    };
    uiVisibilityRules: {
      markerKey: string;
      visibleWhenPath: string;
      supportedConditionFields: string[];
      conditionFormats: {
        single: string;
        multi: string;
      };
    };
  };
}

export interface MCPRulesDefinition {
  version: string;
  extends: string;
  security: any;
  determinism: any;
  validation: any;
  transform: any;
  output: any;
  toolContract: any;
}

export interface UIRulesDefinitionV2 extends SharedRulesDefinition {
  layout: any;
  enhancedFeatures: any;
  rendering: any;
  validation: any;
  builder: any;
  table: any;
  styling: any;
  validationMessages: any;
  fallbackUI: any;
}

// ============================================================================
// YAML 파일 로더
// ============================================================================

/**
 * Shared Rules YAML 로드 (SSOT - 공통 규칙)
 * @param psdSet - PSD 세트 (Level 1: default, civil_gen_definition, etc.)
 * @param schemaType - 스키마 타입 (Level 2: enhanced, manual, original)
 */
export async function loadSharedRules(
  psdSet: string = 'civil_gen_definition',
  schemaType: string = 'enhanced'
): Promise<SharedRulesDefinition> {
  try {
    const path = `/schema_definitions/${psdSet}/${schemaType}/shared.yaml`;
    const response = await fetch(path);
    const yamlText = await response.text();
    const parsed = yaml.load(yamlText) as SharedRulesDefinition;

    console.log(`✅ Loaded ${psdSet}/${schemaType}/shared.yaml`, parsed);
    return parsed;
  } catch (error) {
    console.error(`❌ Failed to load ${psdSet}/${schemaType}/shared.yaml:`, error);
    throw error;
  }
}

/**
 * UI Rules YAML 로드 (통합된 ui.yaml - shared.yaml 상속)
 * @param psdSet - PSD 세트 (Level 1: default, civil_gen_definition, etc.)
 * @param schemaType - 스키마 타입 (Level 2: enhanced, manual, original)
 */
export async function loadUIRules(
  psdSet: string = 'civil_gen_definition',
  schemaType: string = 'enhanced'
): Promise<UIRulesDefinition> {
  try {
    // 🔥 NEW: shared.yaml + ui.yaml 병합
    const sharedRules = await loadSharedRules(psdSet, schemaType);
    const path = `/schema_definitions/${psdSet}/${schemaType}/ui.yaml`;
    const response = await fetch(path);
    const yamlText = await response.text();
    const parsed = yaml.load(yamlText) as UIRulesDefinition;

    const merged = { ...sharedRules, ...parsed };
    console.log(`✅ Loaded ${psdSet}/${schemaType}/ui.yaml (merged with shared)`, merged);
    return merged;
  } catch (error) {
    console.error(`❌ Failed to load ${psdSet}/${schemaType}/ui.yaml:`, error);
    throw error;
  }
}

/**
 * Builder Rules YAML 로드 (ui.yaml의 builder 섹션 사용)
 * @param psdSet - PSD 세트 (Level 1)
 * @param schemaType - 스키마 타입 (Level 2)
 */
export async function loadBuilderRules(
  psdSet: string = 'civil_gen_definition',
  schemaType: string = 'enhanced'
): Promise<BuilderDefinition> {
  try {
    // 🔥 NEW: ui.yaml에서 builder 섹션 추출 (shared.yaml 상속됨)
    const uiRules = await loadUIRules(psdSet, schemaType) as any;

    // ui.yaml의 builder 섹션 + 공통 규칙 병합
    const builderConfig = uiRules.builder || {};
    const merged = {
      ...uiRules,
      formLayout: builderConfig.formLayout,
      fieldRendering: builderConfig.fieldRendering,
      dynamicBehavior: builderConfig.dynamicBehavior,
      instanceManagement: builderConfig.instanceManagement,
      jsonPreview: builderConfig.jsonPreview,
      // shared.yaml에서 가져온 규칙들
      enhancedSchemaMarkers: uiRules.markerRegistry?.map((m: any) => m.key) || [],
      wrapperRules: uiRules.wrapperRegistry?.map((w: any) => ({
        pattern: w.pattern,
        wrapper: w.wrapper,
        description: w.description,
        priority: w.priority,  // 🔥 NEW: priority 포함
      })) || [],
      // 🔥 NEW: shared.yaml의 defaultHandlers.wrapperPriorityDefault
      wrapperPriorityDefault: uiRules.defaultHandlers?.wrapperPriorityDefault ?? 0,
    };

    console.log(`✅ Loaded builder rules from ${psdSet}/${schemaType}/ui.yaml`, merged);
    return merged as BuilderDefinition;
  } catch (error) {
    console.error(`❌ Failed to load builder rules:`, error);
    throw error;
  }
}

/**
 * Table Rules YAML 로드 (ui.yaml의 table 섹션 사용)
 * @param psdSet - PSD 세트 (Level 1)
 * @param schemaType - 스키마 타입 (Level 2)
 */
export async function loadTableRules(
  psdSet: string = 'civil_gen_definition',
  schemaType: string = 'enhanced'
): Promise<TableDefinition> {
  try {
    // 🔥 NEW: ui.yaml에서 table 섹션 추출 (shared.yaml 상속됨)
    const uiRules = await loadUIRules(psdSet, schemaType) as any;

    // ui.yaml의 table 섹션 + 공통 규칙 병합
    const tableConfig = uiRules.table || {};
    const merged = {
      ...uiRules,
      schemaExtensions: tableConfig.schemaExtensions,
      tableStructure: tableConfig.structure,
      sectionHeaders: uiRules.layout?.sectionHeader,
      rowRendering: tableConfig.rowRendering,
      nestedFields: tableConfig.nestedFields,
      conditionRows: tableConfig.conditionRows,
    };

    console.log(`✅ Loaded table rules from ${psdSet}/${schemaType}/ui.yaml`, merged);
    console.log(`🔍 schemaExtensions in merged:`, merged.schemaExtensions);
    return merged as TableDefinition;
  } catch (error) {
    console.error(`❌ Failed to load table rules:`, error);
    // 🔥 기본 구조 반환하여 앱이 크래시하지 않도록 함
    return {
      version: '1.0',
      type: psdSet,
      fieldTypeMapping: {},
      layout: {},
      rendering: {},
      validation: {},
      conditionalRendering: {},
      styling: {},
      tableStructure: {},
      sectionHeaders: {},
      rowRendering: {},
      nestedFields: {},
    } as TableDefinition;
  }
}

/**
 * HTML Template YAML 로드 (Enhanced만 지원, 2-Level)
 * @param psdSet - PSD 세트 (Level 1)
 */
export async function loadHTMLTemplate(
  psdSet: string = 'default'
): Promise<HTMLTemplateDefinition> {
  try {
    const path = `/schema_definitions/${psdSet}/enhanced/html-template.yaml`;
    const response = await fetch(path);
    const yamlText = await response.text();
    const parsed = yaml.load(yamlText) as HTMLTemplateDefinition;

    console.log(`✅ Loaded ${psdSet}/enhanced/html-template.yaml`, parsed);
    return parsed;
  } catch (error) {
    console.error(`❌ Failed to load ${psdSet}/enhanced/html-template.yaml:`, error);
    throw error;
  }
}

// ============================================================================
// 캐시 관리
// ============================================================================

const cache = new Map<string, any>();

export async function loadCachedDefinition(
  type: DefinitionType = 'enhanced',
  category: 'ui' | 'builder' | 'table' | 'html',
  schemaSet?: string,
  psdSet?: string,
  schemaType?: string
): Promise<any> {
  // 2-level 구조 지원 (우선순위: psdSet/schemaType > schemaSet > type)
  const finalPsdSet = psdSet || schemaSet || 'default';
  const finalSchemaType = schemaType || type || 'enhanced';

  const cacheKey = `${finalPsdSet}-${finalSchemaType}-${category}`;

  if (cache.has(cacheKey)) {
    console.log(`📦 Using cached ${cacheKey}`);
    return cache.get(cacheKey);
  }

  let definition: any;

  switch (category) {
    case 'ui':
      definition = await loadUIRules(finalPsdSet, finalSchemaType);
      break;
    case 'builder':
      definition = await loadBuilderRules(finalPsdSet, finalSchemaType);
      break;
    case 'table':
      definition = await loadTableRules(finalPsdSet, finalSchemaType);
      break;
    case 'html':
      if (finalSchemaType === 'enhanced') {
        definition = await loadHTMLTemplate(finalPsdSet);
      }
      break;
  }

  cache.set(cacheKey, definition);
  return definition;
}

/**
 * 캐시 초기화 (개발 중 유용)
 */
export function clearDefinitionCache() {
  cache.clear();
  console.log('🗑️ Definition cache cleared');
}

