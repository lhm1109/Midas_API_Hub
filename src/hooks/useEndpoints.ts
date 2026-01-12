import { useState, useEffect } from 'react';
import type { ApiProduct } from '@/types';

/**
 * DB로부터 엔드포인트 목록을 가져오는 훅
 */
export function useEndpoints() {
  const [endpoints, setEndpoints] = useState<ApiProduct[]>([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);

  useEffect(() => {
    fetchEndpoints();
  }, []);

  const fetchEndpoints = async () => {
    try {
      setLoading(true);
      const response = await fetch('http://localhost:9527/api/endpoints/tree');
      
      if (!response.ok) {
        throw new Error('Failed to fetch endpoints');
      }
      
      const data = await response.json();
      setEndpoints(data);
      setError(null);
    } catch (err) {
      console.error('Error fetching endpoints:', err);
      setError(err instanceof Error ? err.message : 'Unknown error');
      // 에러 시 빈 배열 설정
      setEndpoints([]);
    } finally {
      setLoading(false);
    }
  };

  return { endpoints, loading, error, refetch: fetchEndpoints };
}

