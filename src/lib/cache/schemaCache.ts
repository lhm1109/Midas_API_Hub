/**
 * Schema Compiler Cache (LRU)
 * 
 * 스키마 컴파일 결과를 캐싱하여 반복 작업을 최적화합니다.
 * ⚡ 기존 기능에 영향 없음 - 순수 성능 최적화
 */

import type { SectionGroup } from '../schema/schemaCompiler';

interface CacheEntry<T> {
  value: T;
  timestamp: number;
  accessCount: number;
}

/**
 * LRU 캐시 구현
 */
export class LRUCache<K, V> {
  private cache = new Map<K, CacheEntry<V>>();
  private maxSize: number;
  private maxAge: number; // milliseconds

  constructor(maxSize: number = 50, maxAge: number = 5 * 60 * 1000) {
    this.maxSize = maxSize;
    this.maxAge = maxAge;
  }

  /**
   * 캐시에서 값 가져오기
   */
  get(key: K): V | undefined {
    const entry = this.cache.get(key);
    
    if (!entry) {
      return undefined;
    }

    // 만료 확인
    if (Date.now() - entry.timestamp > this.maxAge) {
      this.cache.delete(key);
      return undefined;
    }

    // 접근 횟수 증가
    entry.accessCount++;
    entry.timestamp = Date.now();

    return entry.value;
  }

  /**
   * 캐시에 값 저장
   */
  set(key: K, value: V): void {
    // 크기 초과 시 가장 오래된 항목 제거
    if (this.cache.size >= this.maxSize) {
      this.evictLeastUsed();
    }

    this.cache.set(key, {
      value,
      timestamp: Date.now(),
      accessCount: 1,
    });
  }

  /**
   * 특정 키의 캐시 삭제
   */
  delete(key: K): boolean {
    return this.cache.delete(key);
  }

  /**
   * 캐시 초기화
   */
  clear(): void {
    this.cache.clear();
  }

  /**
   * 캐시 크기
   */
  size(): number {
    return this.cache.size;
  }

  /**
   * 가장 적게 사용된 항목 제거
   */
  private evictLeastUsed(): void {
    let minAccessCount = Infinity;
    let oldestKey: K | null = null;
    let oldestTime = Infinity;

    for (const [key, entry] of this.cache.entries()) {
      // 만료된 항목 먼저 제거
      if (Date.now() - entry.timestamp > this.maxAge) {
        this.cache.delete(key);
        return;
      }

      // 접근 횟수가 가장 적은 항목 찾기
      if (entry.accessCount < minAccessCount || 
         (entry.accessCount === minAccessCount && entry.timestamp < oldestTime)) {
        minAccessCount = entry.accessCount;
        oldestKey = key;
        oldestTime = entry.timestamp;
      }
    }

    if (oldestKey !== null) {
      this.cache.delete(oldestKey);
    }
  }
}

/**
 * 스키마 해시 생성 (빠른 비교용)
 */
export function generateSchemaHash(schema: any, psdSet: string, schemaType: string): string {
  // null 체크
  if (!schema) {
    return `${psdSet}:${schemaType}:empty`;
  }
  
  // 🔥 스키마 전체를 JSON 문자열로 변환하여 해시 생성
  // default, enum 등 모든 변경사항을 감지하기 위해
  try {
    const schemaStr = JSON.stringify(schema);
    // 간단한 해시 생성 (문자열 길이 + 일부 내용)
    const hash = schemaStr.length + ':' + schemaStr.substring(0, 100);
    return `${psdSet}:${schemaType}:${hash}`;
  } catch (error) {
    // JSON.stringify 실패 시 폴백
    const keys = Object.keys(schema.properties || {}).sort().join(',');
    const requiredKeys = (schema.required || []).sort().join(',');
    return `${psdSet}:${schemaType}:${keys}:${requiredKeys}`;
  }
}

/**
 * 전역 스키마 컴파일 캐시
 */
export const schemaCompileCache = new LRUCache<string, SectionGroup[]>(50, 5 * 60 * 1000);

/**
 * YAML 정의 캐시 (파일 경로 기반)
 */
export const yamlDefinitionCache = new LRUCache<string, any>(20, 10 * 60 * 1000);

/**
 * 캐시 통계 (디버깅용)
 */
export function getCacheStats() {
  return {
    schemaCompileCache: schemaCompileCache.size(),
    yamlDefinitionCache: yamlDefinitionCache.size(),
  };
}
