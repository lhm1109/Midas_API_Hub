/**
 * Product Schema Definition (PSD) Mapping
 * 
 * 제품별 PSD 세트 매핑 (로컬 관리)
 * Supabase에 저장하지 않고 코드로 관리
 */

export interface PSDMapping {
  productId: string;
  psdSet: string;
  schemaType: 'original' | 'enhanced';
}

/**
 * 제품 ID → PSD 세트 매핑
 */
export const PSD_MAPPINGS: Record<string, { psdSet: string; schemaType: string }> = {
  // Civil-NX 제품군
  'civil-nx': {
    psdSet: 'civil_gen_definition',
    schemaType: 'enhanced'
  },
  
  // GTS-NX 제품군
  'gts-nx': {
    psdSet: 'gts_nx_definition',
    schemaType: 'enhanced'
  },
  
  // 기본값 (제품 ID가 없거나 매핑이 없는 경우)
  'default': {
    psdSet: 'civil_gen_definition',
    schemaType: 'enhanced'
  }
};

/**
 * 제품 ID로 PSD 설정 가져오기
 * 우선순위: 1. localStorage (사용자 커스터마이징) → 2. 기본 매핑
 */
export function getPSDForProduct(productId: string | undefined): { psdSet: string; schemaType: string } {
  if (!productId) {
    return PSD_MAPPINGS['default'];
  }
  
  // 1. localStorage에 커스텀 매핑이 있으면 우선 사용
  const customMappings = getCustomPSDMapping();
  if (customMappings[productId]) {
    return customMappings[productId];
  }
  
  // 2. 정확히 매칭
  if (PSD_MAPPINGS[productId]) {
    return PSD_MAPPINGS[productId];
  }
  
  // 3. prefix 매칭 (예: civil-nx_Gen → civil-nx)
  const prefix = productId.split('_')[0] || productId.split('-')[0];
  if (PSD_MAPPINGS[prefix]) {
    return PSD_MAPPINGS[prefix];
  }
  
  // 4. fallback
  return PSD_MAPPINGS['default'];
}

/**
 * localStorage에서 사용자 커스텀 PSD 설정 가져오기 (선택적)
 */
export function getCustomPSDMapping(): Record<string, { psdSet: string; schemaType: string }> {
  try {
    const stored = localStorage.getItem('customPSDMapping');
    return stored ? JSON.parse(stored) : {};
  } catch {
    return {};
  }
}

/**
 * 사용자 커스텀 PSD 설정 저장 (선택적)
 */
export function setCustomPSDMapping(productId: string, psdSet: string, schemaType: string): void {
  try {
    const custom = getCustomPSDMapping();
    custom[productId] = { psdSet, schemaType };
    localStorage.setItem('customPSDMapping', JSON.stringify(custom));
  } catch (error) {
    console.error('Failed to save custom PSD mapping:', error);
  }
}

