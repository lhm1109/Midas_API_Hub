import { useState, useEffect } from 'react';
import type { ApiProduct } from '@/types';

/**
 * DB로부터 엔드포인트 목록을 가져오는 훅
 * - 초기 로드
 * - 페이지 포커스 시 자동 갱신 (다른 탭에서 수정 후 돌아왔을 때 반영)
 */
export function useEndpoints() {
  const [endpoints, setEndpoints] = useState<ApiProduct[]>([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);

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

  // 초기 로드
  useEffect(() => {
    fetchEndpoints();
  }, []);

  // 페이지 가시성 변경 시 자동 갱신 (다른 브라우저/탭에서 수정 후 돌아왔을 때)
  useEffect(() => {
    const handleVisibilityChange = () => {
      if (document.visibilityState === 'visible') {
        console.log('🔄 Page became visible, refetching endpoints...');
        fetchEndpoints();
      }
    };

    document.addEventListener('visibilitychange', handleVisibilityChange);
    
    return () => {
      document.removeEventListener('visibilitychange', handleVisibilityChange);
    };
  }, []);

  return { endpoints, loading, error, refetch: fetchEndpoints };
}

