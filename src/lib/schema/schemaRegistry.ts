/**
 * Schema Registry - 스키마 우선순위 정책 레이어
 * 
 * Enhanced Schema가 있으면 항상 우선
 * 저장된 스키마가 있으면 그것을 최우선
 */

export interface SchemaData {
  jsonSchema?: any;
  jsonSchemaOriginal?: any;
  jsonSchemaEnhanced?: any;
  savedSchema?: any;
}

/**
 * 스키마가 유효한지 확인 (빈 객체가 아닌지)
 */
function isValidSchema(schema: any): boolean {
  if (!schema) return false;
  if (typeof schema !== 'object') return false;
  
  // 빈 객체 체크
  const keys = Object.keys(schema);
  if (keys.length === 0) return false;
  
  // $schema만 있는 경우도 무효로 간주
  if (keys.length === 1 && keys[0] === '$schema') return false;
  
  return true;
}

/**
 * 활성 스키마 결정
 * 우선순위: savedSchema > jsonSchemaEnhanced > jsonSchemaOriginal > jsonSchema
 * 빈 객체는 스킵하고 다음 우선순위로 넘어감
 */
export function resolveActiveSchema(specData: SchemaData | null | undefined): any {
  if (!specData) return null;
  
  // 우선순위 순서대로 체크, 유효한 첫 번째 스키마 반환
  if (isValidSchema(specData.savedSchema)) {
    return specData.savedSchema;
  }
  
  if (isValidSchema(specData.jsonSchemaEnhanced)) {
    return specData.jsonSchemaEnhanced;
  }
  
  if (isValidSchema(specData.jsonSchemaOriginal)) {
    return specData.jsonSchemaOriginal;
  }
  
  if (isValidSchema(specData.jsonSchema)) {
    return specData.jsonSchema;
  }
  
  return null;
}

/**
 * Enhanced 스키마가 활성화되어 있는지 확인
 */
export function isEnhancedSchemaActive(specData: SchemaData | null | undefined): boolean {
  if (!specData) return false;
  
  const active = resolveActiveSchema(specData);
  return active === specData.jsonSchemaEnhanced || active === specData.savedSchema;
}

/**
 * 스키마 소스 타입 반환
 */
export function getSchemaSource(specData: SchemaData | null | undefined): 'saved' | 'enhanced' | 'original' | 'none' {
  if (!specData) return 'none';
  
  const active = resolveActiveSchema(specData);
  
  if (active === specData.savedSchema) return 'saved';
  if (active === specData.jsonSchemaEnhanced) return 'enhanced';
  if (active === specData.jsonSchemaOriginal || active === specData.jsonSchema) return 'original';
  
  return 'none';
}

