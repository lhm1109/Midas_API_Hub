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
 * UI Rules YAML 로드
 */
export async function loadUIRules(type: DefinitionType): Promise<UIRulesDefinition> {
  try {
    const response = await fetch(`/schema_definitions/${type}/ui-rules.yaml`);
    const yamlText = await response.text();
    const parsed = yaml.load(yamlText) as UIRulesDefinition;
    
    console.log(`✅ Loaded ${type} ui-rules.yaml`, parsed);
    return parsed;
  } catch (error) {
    console.error(`❌ Failed to load ${type} ui-rules.yaml:`, error);
    throw error;
  }
}

/**
 * Builder Rules YAML 로드
 */
export async function loadBuilderRules(type: DefinitionType): Promise<BuilderDefinition> {
  try {
    const response = await fetch(`/schema_definitions/${type}/builder.yaml`);
    const yamlText = await response.text();
    const parsed = yaml.load(yamlText) as BuilderDefinition;
    
    // extends 처리: ui-rules.yaml 병합
    const uiRules = await loadUIRules(type);
    const merged = { ...uiRules, ...parsed };
    
    console.log(`✅ Loaded ${type} builder.yaml`, merged);
    return merged;
  } catch (error) {
    console.error(`❌ Failed to load ${type} builder.yaml:`, error);
    throw error;
  }
}

/**
 * Table Rules YAML 로드
 */
export async function loadTableRules(type: DefinitionType): Promise<TableDefinition> {
  try {
    const response = await fetch(`/schema_definitions/${type}/table.yaml`);
    
    if (!response.ok) {
      throw new Error(`HTTP ${response.status}: ${response.statusText}`);
    }
    
    const yamlText = await response.text();
    const parsed = yaml.load(yamlText) as TableDefinition;
    
    // extends 처리: ui-rules.yaml 병합 (실패해도 계속 진행)
    try {
      const uiRules = await loadUIRules(type);
      const merged = { ...uiRules, ...parsed };
      console.log(`✅ Loaded ${type} table.yaml`, merged);
      return merged;
    } catch (uiRulesError) {
      console.warn(`⚠️ Failed to load ui-rules.yaml, using table.yaml only:`, uiRulesError);
      console.log(`✅ Loaded ${type} table.yaml (without ui-rules)`, parsed);
      return parsed;
    }
  } catch (error) {
    console.error(`❌ Failed to load ${type} table.yaml:`, error);
    // 🔥 기본 구조 반환하여 앱이 크래시하지 않도록 함
    return {
      version: '1.0',
      type: type,
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
 * HTML Template YAML 로드 (Enhanced만 지원)
 */
export async function loadHTMLTemplate(): Promise<HTMLTemplateDefinition> {
  try {
    const response = await fetch(`/schema_definitions/enhanced/html-template.yaml`);
    const yamlText = await response.text();
    const parsed = yaml.load(yamlText) as HTMLTemplateDefinition;
    
    console.log(`✅ Loaded enhanced html-template.yaml`, parsed);
    return parsed;
  } catch (error) {
    console.error(`❌ Failed to load html-template.yaml:`, error);
    throw error;
  }
}

// ============================================================================
// 캐시 관리
// ============================================================================

const cache = new Map<string, any>();

export async function loadCachedDefinition(
  type: DefinitionType,
  category: 'ui' | 'builder' | 'table' | 'html'
): Promise<any> {
  const cacheKey = `${type}-${category}`;
  
  if (cache.has(cacheKey)) {
    console.log(`📦 Using cached ${cacheKey}`);
    return cache.get(cacheKey);
  }
  
  let definition: any;
  
  switch (category) {
    case 'ui':
      definition = await loadUIRules(type);
      break;
    case 'builder':
      definition = await loadBuilderRules(type);
      break;
    case 'table':
      definition = await loadTableRules(type);
      break;
    case 'html':
      if (type === 'enhanced') {
        definition = await loadHTMLTemplate();
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

