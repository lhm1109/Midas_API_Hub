/**
 * Product Schema Definition (PSD) Mapping
 * 
 * 제품별 PSD 세트 매핑
 * Supabase products 테이블에서 동적으로 가져오거나 기본값 사용
 */

export interface PSDMapping {
  productId: string;
  psdSet: string;
  schemaType: 'original' | 'enhanced';
}

/**
 * 기본 PSD 설정 (fallback)
 */
const DEFAULT_PSD = {
  psdSet: 'civil_gen_definition',
  schemaType: 'enhanced'
};

/**
 * 캐시된 제품 매핑 (메모리 캐시)
 */
let cachedProductMappings: Record<string, { psdSet: string; schemaType: string }> | null = null;
let lastFetchTime = 0;
const CACHE_DURATION = 5 * 60 * 1000; // 5분

/**
 * Supabase에서 제품 목록 가져오기
 */
async function fetchProductsFromSupabase(): Promise<Record<string, { psdSet: string; schemaType: string }>> {
  try {
    const response = await fetch('http://localhost:9527/api/products');
    if (!response.ok) {
      console.warn('Failed to fetch products from Supabase, using default');
      return {};
    }
    
    const products = await response.json();
    const mappings: Record<string, { psdSet: string; schemaType: string }> = {};
    
    products.forEach((product: any) => {
      mappings[product.id] = {
        psdSet: product.psd_set || DEFAULT_PSD.psdSet,
        schemaType: product.schema_type || DEFAULT_PSD.schemaType
      };
    });
    
    return mappings;
  } catch (error) {
    console.warn('Error fetching products:', error);
    return {};
  }
}

/**
 * 제품 ID로 PSD 설정 가져오기
 * 우선순위: 
 * 1. localStorage (사용자 커스터마이징)
 * 2. Supabase products 테이블 (캐시)
 * 3. 기본값
 */
export function getPSDForProduct(productId: string | undefined): { psdSet: string; schemaType: string } {
  if (!productId) {
    return DEFAULT_PSD;
  }
  
  // 1. localStorage에 커스텀 매핑이 있으면 우선 사용
  const customMappings = getCustomPSDMapping();
  if (customMappings[productId]) {
    return customMappings[productId];
  }
  
  // 2. 캐시된 Supabase 매핑 사용 (동기적으로)
  if (cachedProductMappings && cachedProductMappings[productId]) {
    return cachedProductMappings[productId];
  }
  
  // 3. prefix 매칭 시도 (예: civil-nx_Gen → civil-nx)
  const prefix = productId.split('_')[0] || productId.split('-')[0];
  if (cachedProductMappings && cachedProductMappings[prefix]) {
    return cachedProductMappings[prefix];
  }
  
  // 4. fallback
  return DEFAULT_PSD;
}

/**
 * Supabase 제품 매핑 캐시 새로고침 (비동기)
 * 앱 시작 시 또는 제품 추가/수정 후 호출
 */
export async function refreshProductMappings(): Promise<void> {
  const now = Date.now();
  
  // 캐시가 유효하면 스킵
  if (cachedProductMappings && (now - lastFetchTime) < CACHE_DURATION) {
    return;
  }
  
  cachedProductMappings = await fetchProductsFromSupabase();
  lastFetchTime = now;
  console.log('✅ Product PSD mappings refreshed:', Object.keys(cachedProductMappings).length, 'products');
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

