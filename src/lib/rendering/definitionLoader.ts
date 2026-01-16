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
  enhancedFeatures?: any;
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
  }>;
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
// YAML 파일 로더
// ============================================================================

/**
 * UI Rules YAML 로드 (2-Level)
 * @param psdSet - PSD 세트 (Level 1: default, civil_gen_definition, etc.)
 * @param schemaType - 스키마 타입 (Level 2: enhanced, manual, original)
 */
export async function loadUIRules(
  psdSet: string = 'civil_gen_definition', 
  schemaType: string = 'enhanced'
): Promise<UIRulesDefinition> {
  try {
    const path = `/schema_definitions/${psdSet}/${schemaType}/ui-rules.yaml`;
    const response = await fetch(path);
    const yamlText = await response.text();
    const parsed = yaml.load(yamlText) as UIRulesDefinition;
    
    console.log(`✅ Loaded ${psdSet}/${schemaType}/ui-rules.yaml`, parsed);
    return parsed;
  } catch (error) {
    console.error(`❌ Failed to load ${psdSet}/${schemaType}/ui-rules.yaml:`, error);
    throw error;
  }
}

/**
 * Builder Rules YAML 로드 (2-Level)
 * @param psdSet - PSD 세트 (Level 1)
 * @param schemaType - 스키마 타입 (Level 2)
 */
export async function loadBuilderRules(
  psdSet: string = 'civil_gen_definition',
  schemaType: string = 'enhanced'
): Promise<BuilderDefinition> {
  try {
    const path = `/schema_definitions/${psdSet}/${schemaType}/builder.yaml`;
    const response = await fetch(path);
    const yamlText = await response.text();
    const parsed = yaml.load(yamlText) as BuilderDefinition;
    
    // extends 처리: ui-rules.yaml 병합
    const uiRules = await loadUIRules(psdSet, schemaType);
    const merged = { ...uiRules, ...parsed };
    
    console.log(`✅ Loaded ${psdSet}/${schemaType}/builder.yaml`, merged);
    return merged;
  } catch (error) {
    console.error(`❌ Failed to load ${psdSet}/${schemaType}/builder.yaml:`, error);
    throw error;
  }
}

/**
 * Table Rules YAML 로드 (2-Level)
 * @param psdSet - PSD 세트 (Level 1)
 * @param schemaType - 스키마 타입 (Level 2)
 */
export async function loadTableRules(
  psdSet: string = 'civil_gen_definition',
  schemaType: string = 'enhanced'
): Promise<TableDefinition> {
  try {
    const path = `/schema_definitions/${psdSet}/${schemaType}/table.yaml`;
    const response = await fetch(path);
    
    if (!response.ok) {
      throw new Error(`HTTP ${response.status}: ${response.statusText}`);
    }
    
    const yamlText = await response.text();
    const parsed = yaml.load(yamlText) as TableDefinition;
    
    // extends 처리: ui-rules.yaml 병합 (실패해도 계속 진행)
    try {
      const uiRules = await loadUIRules(psdSet, schemaType);
      const merged = { ...uiRules, ...parsed };
      console.log(`✅ Loaded ${psdSet}/${schemaType}/table.yaml`, merged);
      console.log(`🔍 schemaExtensions in merged:`, merged.schemaExtensions);
      return merged;
    } catch (uiRulesError) {
      console.warn(`⚠️ Failed to load ui-rules.yaml, using table.yaml only:`, uiRulesError);
      console.log(`✅ Loaded ${psdSet}/${schemaType}/table.yaml (without ui-rules)`, parsed);
      console.log(`🔍 schemaExtensions in parsed:`, parsed.schemaExtensions);
      return parsed;
    }
  } catch (error) {
    console.error(`❌ Failed to load ${psdSet}/${schemaType}/table.yaml:`, error);
    // 🔥 기본 구조 반환하여 앱이 크래시하지 않도록 함
    return {
      version: '1.0',
      type: setName,
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

