import { create } from 'zustand';
import apiClient from '@/lib/api-client';
import type { Version, ManualData, SpecData, BuilderData, RunnerData } from '@/types';

export interface AppState {
  currentTab: 'version' | 'manual' | 'spec' | 'builder' | 'runner';
  
  // 🎯 **Version 관리** (최상위)
  versions: Version[];
  currentVersionId: string | null; // 현재 편집 중인 버전
  
  // 🎯 **편집 중인 데이터** (버전과 독립적으로 관리)
  manualData: ManualData | null;
  specData: SpecData | null;
  builderData: BuilderData | null;
  runnerData: RunnerData | null;
  
  // UI 상태
  hasUnsavedChanges: boolean;
  isServerConnected: boolean;
  
  // 🔒 **편집 잠금 상태**
  endpointLock: {
    locked: boolean;
    lockedBy?: string;
    lockedAt?: string;
    expiresAt?: string;
  } | null;
  currentUserId: string; // 현재 사용자 ID (이메일 등)
  
  // Tab actions
  setCurrentTab: (tab: 'version' | 'manual' | 'spec' | 'builder' | 'runner') => void;
  
  // 🎯 **서버 연결 확인**
  checkServerConnection: () => Promise<boolean>;
  
  // 🎯 **Version 관리 액션** (서버 연동)
  fetchVersions: (endpointId?: string) => Promise<void>;
  createVersion: (endpointId: string, version: string, changeLog?: string) => Promise<void>;
  loadVersion: (id: string) => Promise<void>;
  saveCurrentVersion: () => Promise<void>;
  deleteVersion: (id: string) => Promise<void>;
  resetCurrentVersion: () => void; // 🔥 현재 버전 리셋
  getVersionsByEndpoint: (endpointId: string) => Version[];
  getCurrentVersion: () => Version | null;
  
  // 🎯 **데이터 편집** 
  setManualData: (data: ManualData) => void;
  updateManualData: (updates: Partial<ManualData>) => void;
  
  setSpecData: (data: SpecData) => void;
  updateSpecData: (updates: Partial<SpecData>) => void;
  
  setBuilderData: (data: BuilderData) => void;
  updateBuilderData: (updates: Partial<BuilderData>) => void;
  
  setRunnerData: (data: RunnerData) => void;
  updateRunnerData: (updates: Partial<RunnerData>) => void;
  addTestCase: (name: string, description?: string) => void;
  updateTestCase: (id: string, updates: Partial<{ name: string; description?: string; requestBody: string }>) => void;
  deleteTestCase: (id: string) => void;
  
  // 🔒 **편집 잠금 관리**
  checkEndpointLock: (endpointId: string) => Promise<void>;
  acquireEndpointLock: (endpointId: string) => Promise<boolean>;
  releaseEndpointLock: (endpointId: string) => Promise<void>;
  setCurrentUserId: (userId: string) => void;
}

export const useAppStore = create<AppState>((set, get) => ({
  currentTab: 'version',
  
  // 🎯 Version 초기 상태
  versions: [],
  currentVersionId: null,
  
  // 🎯 편집 중인 데이터 초기 상태
  manualData: null,
  specData: null,
  builderData: null,
  runnerData: null,
  
  hasUnsavedChanges: false,
  isServerConnected: false,
  
  // 🔒 잠금 초기 상태
  endpointLock: null,
  currentUserId: localStorage.getItem('userId') || `user_${Date.now()}`,
  
  setCurrentTab: (tab) => set({ currentTab: tab }),
  
  // 🎯 **서버 연결 확인**
  checkServerConnection: async () => {
    const isConnected = await apiClient.healthCheck();
    set({ isServerConnected: isConnected });
    return isConnected;
  },
  
  // 🎯 **버전 목록 조회** (서버에서)
  fetchVersions: async (endpointId) => {
    const response = await apiClient.getVersions(endpointId);
    if (response.data) {
      set({ versions: response.data });
    }
  },
  
  // 🎯 **새 버전 생성** (서버에 저장)
  createVersion: async (endpointId, version, changeLog) => {
    const now = new Date().toISOString();
    const id = `v_${Date.now()}_${Math.random().toString(36).substr(2, 9)}`;
    
    const state = get();
    const newVersion: Version = {
      id,
      version,
      endpointId,
      createdAt: now,
      updatedAt: now,
      changeLog,
      
      // 현재 편집 중인 데이터 사용
      manualData: state.manualData || {
        title: '',
        category: '',
        inputUri: '',
        activeMethods: '',
        jsonSchema: '{}',
        jsonSchemaOriginal: undefined,
        jsonSchemaEnhanced: undefined,
        examples: [],
        specifications: '',
      },
      specData: state.specData || {
        jsonSchema: '{}',
        jsonSchemaOriginal: undefined,
        jsonSchemaEnhanced: undefined,
        specifications: '',
      },
      builderData: state.builderData || {
        formData: {},
      },
      runnerData: state.runnerData || {
        requestBody: '{}',
        responseBody: '',
        testCases: [],
      },
    };
    
    // 서버에 저장
    const response = await apiClient.createVersion(newVersion);
    
    if (response.data) {
      set((state) => ({
        versions: [...state.versions, newVersion],
        currentVersionId: id,
        hasUnsavedChanges: false,
      }));
    }
  },
  
  // 🎯 **버전 로드** (서버에서 조회)
  loadVersion: async (id) => {
    const response = await apiClient.getVersion(id);
    
    if (response.data) {
      const version = response.data;
      
      set({ 
        currentVersionId: id,
        manualData: version.manualData,
        specData: version.specData,
        builderData: version.builderData,
        runnerData: version.runnerData,
        hasUnsavedChanges: false,
      });
      
      // 로컬 버전 목록 업데이트
      set((state) => ({
        versions: state.versions.some(v => v.id === id)
          ? state.versions.map(v => v.id === id ? version : v)
          : [...state.versions, version],
      }));
    }
  },
  
  // 🎯 **현재 버전 저장** (서버에 업데이트)
  saveCurrentVersion: async () => {
    const state = get();
    const currentVersionId = state.currentVersionId;
    if (!currentVersionId) {
      throw new Error('No version selected');
    }
    
    const version = state.versions.find(v => v.id === currentVersionId);
    if (!version) {
      throw new Error('Version not found in store');
    }
    
    console.log('💾 Saving version:', {
      id: currentVersionId,
      version: version.version,
      hasManualData: !!state.manualData,
      hasSpecData: !!state.specData,
      hasBuilderData: !!state.builderData,
      hasRunnerData: !!state.runnerData,
    });
    
    // 🔥 null 데이터는 기존 버전의 데이터 유지
    const updatedVersion: Version = {
      ...version,
      updatedAt: new Date().toISOString(),
      manualData: state.manualData ?? version.manualData,
      specData: state.specData ?? version.specData,
      builderData: state.builderData ?? version.builderData,
      runnerData: state.runnerData ?? version.runnerData,
    };
    
    console.log('📤 Sending updated version to server:', updatedVersion);
    
    try {
      // 서버에 업데이트
      const response = await apiClient.updateVersion(currentVersionId, updatedVersion);
      
      console.log('✅ Server response:', response);
      
      if (!response.data) {
        throw new Error('Server returned no data');
      }
      
      // 성공하면 store 업데이트
      set((state) => ({
        versions: state.versions.map((v) =>
          v.id === currentVersionId ? updatedVersion : v
        ),
        hasUnsavedChanges: false,
      }));
      
      console.log('✅ Version saved successfully');
    } catch (error) {
      console.error('❌ Save version failed:', error);
      throw error;
    }
  },
  
  // 🎯 **버전 삭제** (서버에서도 삭제)
  deleteVersion: async (id) => {
    await apiClient.deleteVersion(id);
    
    set((state) => ({
      versions: state.versions.filter((v) => v.id !== id),
      currentVersionId: state.currentVersionId === id ? null : state.currentVersionId,
    }));
  },
  
  // 🎯 **현재 버전 리셋** (엔드포인트 변경 시 사용)
  resetCurrentVersion: () => {
    set({
      currentVersionId: null,
      manualData: null,
      specData: null,
      builderData: null,
      runnerData: {
        requestBody: '{}',
        responseBody: '',
        testCases: [],
      },
      hasUnsavedChanges: false,
    });
  },
  
  // 🎯 **엔드포인트별 버전 조회**
  getVersionsByEndpoint: (endpointId) => {
    return get().versions.filter((v) => v.endpointId === endpointId);
  },
  
  // 🎯 **현재 버전 조회**
  getCurrentVersion: () => {
    const id = get().currentVersionId;
    if (!id) return null;
    return get().versions.find((v) => v.id === id) || null;
  },
  
  // 🎯 **Manual 데이터 설정 및 업데이트**
  setManualData: (data) => set({ manualData: data, hasUnsavedChanges: data !== null }),
  
  updateManualData: (updates) => {
    set((state) => ({
      manualData: state.manualData ? { ...state.manualData, ...updates } : null,
      hasUnsavedChanges: true,
    }));
  },
  
  // 🎯 **Spec 데이터 설정 및 업데이트**
  setSpecData: (data) => set({ specData: data, hasUnsavedChanges: data !== null }),
  
  updateSpecData: (updates) => {
    set((state) => ({
      specData: state.specData ? { ...state.specData, ...updates } : null,
      hasUnsavedChanges: true,
    }));
  },
  
  // 🎯 **Builder 데이터 설정 및 업데이트**
  setBuilderData: (data) => set({ builderData: data, hasUnsavedChanges: data !== null }),
  
  updateBuilderData: (updates) => {
    set((state) => ({
      builderData: state.builderData ? { ...state.builderData, ...updates } : null,
      hasUnsavedChanges: true,
    }));
  },
  
  // 🎯 **Runner 데이터 설정 및 업데이트**
  setRunnerData: (data) => set({ runnerData: data, hasUnsavedChanges: data !== null }),
  
  updateRunnerData: (updates) => {
    set((state) => ({
      runnerData: state.runnerData ? { ...state.runnerData, ...updates } : null,
      hasUnsavedChanges: true,
    }));
  },
  
  // 🎯 **Test Case 추가**
  addTestCase: (name, description) => {
    const runnerData = get().runnerData;
    if (!runnerData) return;
    
    const now = new Date().toISOString();
    const newTestCase = {
      id: `tc_${Date.now()}_${Math.random().toString(36).substr(2, 9)}`,
      name,
      description,
      requestBody: runnerData.requestBody,
      createdAt: now,
      updatedAt: now,
    };
    
    set((state) => ({
      runnerData: state.runnerData ? {
        ...state.runnerData,
        testCases: [...state.runnerData.testCases, newTestCase],
      } : null,
      hasUnsavedChanges: true,
    }));
  },
  
  // 🎯 **Test Case 업데이트**
  updateTestCase: (id, updates) => {
    set((state) => ({
      runnerData: state.runnerData ? {
        ...state.runnerData,
        testCases: state.runnerData.testCases.map((tc) =>
          tc.id === id
            ? { ...tc, ...updates, updatedAt: new Date().toISOString() }
            : tc
        ),
      } : null,
      hasUnsavedChanges: true,
    }));
  },
  
  // 🎯 **Test Case 삭제**
  deleteTestCase: (id) => {
    set((state) => ({
      runnerData: state.runnerData ? {
        ...state.runnerData,
        testCases: state.runnerData.testCases.filter((tc) => tc.id !== id),
      } : null,
      hasUnsavedChanges: true,
    }));
  },
  
  // 🔒 **편집 잠금 상태 확인**
  checkEndpointLock: async (endpointId) => {
    try {
      const response = await fetch(`http://localhost:9527/api/locks/endpoint/${encodeURIComponent(endpointId)}/lock`);
      if (response.ok) {
        const data = await response.json();
        set({ endpointLock: data });
      }
    } catch (error) {
      console.error('Failed to check lock:', error);
    }
  },
  
  // 🔒 **편집 잠금 획득**
  acquireEndpointLock: async (endpointId) => {
    const { currentUserId } = get();
    try {
      const response = await fetch(`http://localhost:9527/api/locks/endpoint/${encodeURIComponent(endpointId)}/lock`, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ userId: currentUserId }),
      });
      
      if (response.ok) {
        const data = await response.json();
        set({ endpointLock: data.lock });
        return true;
      } else if (response.status === 423) {
        const data = await response.json();
        set({ endpointLock: { locked: true, ...data } });
        return false;
      }
      return false;
    } catch (error) {
      console.error('Failed to acquire lock:', error);
      return false;
    }
  },
  
  // 🔒 **편집 잠금 해제**
  releaseEndpointLock: async (endpointId) => {
    const { currentUserId } = get();
    try {
      await fetch(`http://localhost:9527/api/locks/endpoint/${encodeURIComponent(endpointId)}/lock`, {
        method: 'DELETE',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ userId: currentUserId }),
      });
      set({ endpointLock: null });
    } catch (error) {
      console.error('Failed to release lock:', error);
    }
  },
  
  // 🔒 **사용자 ID 설정**
  setCurrentUserId: (userId) => {
    localStorage.setItem('userId', userId);
    set({ currentUserId: userId });
  },
}));
