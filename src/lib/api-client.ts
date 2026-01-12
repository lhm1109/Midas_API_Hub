// 🎯 API 클라이언트 (services/api.ts를 lib으로 이동)
import { API_CONFIG } from '@/config/constants';
import type { Version } from '@/types';

interface ApiResponse<T> {
  data?: T;
  error?: string;
}

class ApiClient {
  private baseUrl: string;

  constructor(baseUrl: string = API_CONFIG.BASE_URL) {
    this.baseUrl = baseUrl;
  }

  private async request<T>(
    endpoint: string,
    options: RequestInit = {}
  ): Promise<ApiResponse<T>> {
    try {
      const response = await fetch(`${this.baseUrl}${endpoint}`, {
        ...options,
        headers: {
          'Content-Type': 'application/json',
          ...options.headers,
        },
      });

      if (!response.ok) {
        const error = await response.json();
        throw new Error(error.error || 'Request failed');
      }

      const data = await response.json();
      return { data };
    } catch (error) {
      console.error('API Error:', error);
      return { error: error instanceof Error ? error.message : 'Unknown error' };
    }
  }

  // Endpoints API
  async getEndpoints() {
    return this.request<any[]>('/endpoints');
  }

  async getEndpointsTree() {
    return this.request<any[]>('/endpoints/tree');
  }

  async getEndpoint(id: string) {
    return this.request<any>(`/endpoints/${id}`);
  }

  async createEndpoint(data: any) {
    return this.request<{ id: string; message: string }>('/endpoints', {
      method: 'POST',
      body: JSON.stringify(data),
    });
  }

  async updateEndpoint(id: string, data: any) {
    return this.request<{ message: string }>(`/endpoints/${id}`, {
      method: 'PUT',
      body: JSON.stringify(data),
    });
  }

  async deleteEndpoint(id: string) {
    return this.request<{ message: string }>(`/endpoints/${id}`, {
      method: 'DELETE',
    });
  }

  async seedEndpoints() {
    return this.request<{ message: string; count: number }>('/endpoints/seed', {
      method: 'POST',
    });
  }

  // Version API
  async getVersions(endpointId?: string) {
    const query = endpointId ? `?endpoint_id=${endpointId}` : '';
    return this.request<Version[]>(`/versions${query}`);
  }

  async getVersion(id: string) {
    return this.request<Version>(`/versions/${id}`);
  }

  async createVersion(data: Version) {
    return this.request<{ id: string; message: string }>('/versions', {
      method: 'POST',
      body: JSON.stringify(data),
    });
  }

  async updateVersion(id: string, data: Partial<Version>) {
    return this.request<{ message: string }>(`/versions/${id}`, {
      method: 'PUT',
      body: JSON.stringify(data),
    });
  }

  async deleteVersion(id: string) {
    return this.request<{ message: string }>(`/versions/${id}`, {
      method: 'DELETE',
    });
  }

  // Working Data API
  async getManualData() {
    return this.request<any>('/data/manual');
  }

  async saveManualData(data: any) {
    return this.request<{ message: string }>('/data/manual', {
      method: 'POST',
      body: JSON.stringify(data),
    });
  }

  async getSpecData() {
    return this.request<any>('/data/spec');
  }

  async saveSpecData(data: any) {
    return this.request<{ message: string }>('/data/spec', {
      method: 'POST',
      body: JSON.stringify(data),
    });
  }

  async getBuilderData() {
    return this.request<any>('/data/builder');
  }

  async saveBuilderData(data: any) {
    return this.request<{ message: string }>('/data/builder', {
      method: 'POST',
      body: JSON.stringify(data),
    });
  }

  async getRunnerData() {
    return this.request<any>('/data/runner');
  }

  async saveRunnerData(data: any) {
    return this.request<{ message: string }>('/data/runner', {
      method: 'POST',
      body: JSON.stringify(data),
    });
  }

  async getAllWorkingData() {
    return this.request<any>('/data/all');
  }

  async clearAllWorkingData() {
    return this.request<{ message: string }>('/data/all', {
      method: 'DELETE',
    });
  }

  // Health check
  async healthCheck() {
    try {
      const response = await fetch(API_CONFIG.HEALTH_CHECK_URL);
      return response.ok;
    } catch {
      return false;
    }
  }
}

export const apiClient = new ApiClient();
export default apiClient;

