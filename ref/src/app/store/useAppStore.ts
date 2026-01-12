import { create } from 'zustand';

// 🎯 Spec 데이터 타입
export interface SpecData {
  jsonSchema: string; // Original 또는 Enhanced 중 우선순위가 높은 것
  jsonSchemaOriginal?: string; // 원본 스키마
  jsonSchemaEnhanced?: string; // Enhanced 스키마 (조건문, required, optional 반영)
  specifications: string; // HTML table string
}

// 🎯 Builder 데이터 타입
export interface BuilderData {
  formData: Record<string, any>; // JSON 스키마 폼 데이터
}

// 🎯 Runner 데이터 타입
export interface RunnerData {
  requestBody: string;
  responseBody: string;
  testCases: {
    id: string;
    name: string;
    description?: string;
    requestBody: string;
    createdAt: string;
    updatedAt: string;
  }[];
}

// 🎯 Manual 데이터 타입
export interface ManualData {
  title: string;
  category: string;
  inputUri: string;
  activeMethods: string;
  examples: { title: string; code: string }[];
  htmlContent: string;
  articleId?: string;
  sectionId?: string;
  authorId?: string;
  url?: string;
}

// 🎯 **Version이 최상위 개념** - 모든 탭의 데이터를 포함
export interface Version {
  id: string;
  version: string; // e.g., "v1.0.0", "v1.1.0"
  endpointId: string;
  createdAt: string;
  updatedAt: string;
  author?: string;
  changeLog?: string;
  
  // 각 탭의 데이터
  manualData: ManualData;
  specData: SpecData;
  builderData: BuilderData;
  runnerData: RunnerData;
}

export interface AppState {
  currentTab: 'version' | 'manual' | 'spec' | 'builder' | 'runner';
  
  // 🎯 **Version 관리** (최상위)
  versions: Version[];
  currentVersionId: string | null; // 현재 편집 중인 버전
  
  // UI 상태
  hasUnsavedChanges: boolean;
  
  // Tab actions
  setCurrentTab: (tab: 'version' | 'manual' | 'spec' | 'builder' | 'runner') => void;
  
  // 🎯 **Version 관리 액션**
  createVersion: (endpointId: string, version: string, changeLog?: string) => void;
  loadVersion: (id: string) => void;
  saveCurrentVersion: () => void;
  deleteVersion: (id: string) => void;
  getVersionsByEndpoint: (endpointId: string) => Version[];
  getCurrentVersion: () => Version | null;
  
  // 🎯 **Manual 데이터 편집** (현재 버전에 반영)
  updateManualData: (updates: Partial<ManualData>) => void;
  
  // 🎯 **Spec 데이터 편집** (현재 버전에 반영)
  updateSpecData: (updates: Partial<SpecData>) => void;
  
  // 🎯 **Builder 데이터 편집** (현재 버전에 반영)
  updateBuilderData: (updates: Partial<BuilderData>) => void;
  
  // 🎯 **Runner 데이터 편집** (현재 버전에 반영)
  updateRunnerData: (updates: Partial<RunnerData>) => void;
  addTestCase: (name: string, description?: string) => void;
  updateTestCase: (id: string, updates: Partial<{ name: string; description?: string; requestBody: string }>) => void;
  deleteTestCase: (id: string) => void;
}

export const useAppStore = create<AppState>((set, get) => ({
  currentTab: 'version',
  
  // 🎯 Version 초기 상태
  versions: [],
  currentVersionId: null,
  
  hasUnsavedChanges: false,
  
  setCurrentTab: (tab) => set({ currentTab: tab }),
  
  // 🎯 **새 버전 생성**
  createVersion: (endpointId, version, changeLog) => {
    const now = new Date().toISOString();
    const newVersion: Version = {
      id: `v_${Date.now()}_${Math.random().toString(36).substr(2, 9)}`,
      version,
      endpointId,
      createdAt: now,
      updatedAt: now,
      changeLog,
      
      // 초기 데이터
      manualData: {
        title: '',
        category: '',
        inputUri: '',
        activeMethods: '',
        examples: [],
        htmlContent: '',
      },
      specData: {
        jsonSchema: '{}',
        specifications: '',
      },
      builderData: {
        formData: {},
      },
      runnerData: {
        requestBody: '{}',
        responseBody: '',
        testCases: [],
      },
    };
    
    set((state) => ({
      versions: [...state.versions, newVersion],
      currentVersionId: newVersion.id,
      hasUnsavedChanges: false,
    }));
  },
  
  // 🎯 **버전 로드**
  loadVersion: (id) => {
    const version = get().versions.find((v) => v.id === id);
    if (version) {
      set({ 
        currentVersionId: id,
        hasUnsavedChanges: false,
      });
    }
  },
  
  // 🎯 **현재 버전 저장**
  saveCurrentVersion: () => {
    const currentVersionId = get().currentVersionId;
    if (!currentVersionId) return;
    
    set((state) => ({
      versions: state.versions.map((v) =>
        v.id === currentVersionId
          ? { ...v, updatedAt: new Date().toISOString() }
          : v
      ),
      hasUnsavedChanges: false,
    }));
  },
  
  // 🎯 **버전 삭제**
  deleteVersion: (id) => {
    set((state) => ({
      versions: state.versions.filter((v) => v.id !== id),
      currentVersionId: state.currentVersionId === id ? null : state.currentVersionId,
    }));
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
  
  // 🎯 **Manual 데이터 업데이트**
  updateManualData: (updates) => {
    const currentVersionId = get().currentVersionId;
    if (!currentVersionId) return;
    
    set((state) => ({
      versions: state.versions.map((v) =>
        v.id === currentVersionId
          ? { ...v, manualData: { ...v.manualData, ...updates } }
          : v
      ),
      hasUnsavedChanges: true,
    }));
  },
  
  // 🎯 **Spec 데이터 업데이트**
  updateSpecData: (updates) => {
    const currentVersionId = get().currentVersionId;
    if (!currentVersionId) return;
    
    set((state) => ({
      versions: state.versions.map((v) =>
        v.id === currentVersionId
          ? { ...v, specData: { ...v.specData, ...updates } }
          : v
      ),
      hasUnsavedChanges: true,
    }));
  },
  
  // 🎯 **Builder 데이터 업데이트**
  updateBuilderData: (updates) => {
    const currentVersionId = get().currentVersionId;
    if (!currentVersionId) return;
    
    set((state) => ({
      versions: state.versions.map((v) =>
        v.id === currentVersionId
          ? { ...v, builderData: { ...v.builderData, ...updates } }
          : v
      ),
      hasUnsavedChanges: true,
    }));
  },
  
  // 🎯 **Runner 데이터 업데이트**
  updateRunnerData: (updates) => {
    const currentVersionId = get().currentVersionId;
    if (!currentVersionId) return;
    
    set((state) => ({
      versions: state.versions.map((v) =>
        v.id === currentVersionId
          ? { ...v, runnerData: { ...v.runnerData, ...updates } }
          : v
      ),
      hasUnsavedChanges: true,
    }));
  },
  
  // 🎯 **Test Case 추가**
  addTestCase: (name, description) => {
    const currentVersion = get().getCurrentVersion();
    if (!currentVersion) return;
    
    const now = new Date().toISOString();
    const newTestCase = {
      id: `tc_${Date.now()}_${Math.random().toString(36).substr(2, 9)}`,
      name,
      description,
      requestBody: currentVersion.runnerData.requestBody,
      createdAt: now,
      updatedAt: now,
    };
    
    set((state) => ({
      versions: state.versions.map((v) =>
        v.id === state.currentVersionId
          ? {
              ...v,
              runnerData: {
                ...v.runnerData,
                testCases: [...v.runnerData.testCases, newTestCase],
              },
            }
          : v
      ),
      hasUnsavedChanges: true,
    }));
  },
  
  // 🎯 **Test Case 업데이트**
  updateTestCase: (id, updates) => {
    const currentVersionId = get().currentVersionId;
    if (!currentVersionId) return;
    
    set((state) => ({
      versions: state.versions.map((v) =>
        v.id === currentVersionId
          ? {
              ...v,
              runnerData: {
                ...v.runnerData,
                testCases: v.runnerData.testCases.map((tc) =>
                  tc.id === id
                    ? { ...tc, ...updates, updatedAt: new Date().toISOString() }
                    : tc
                ),
              },
            }
          : v
      ),
      hasUnsavedChanges: true,
    }));
  },
  
  // 🎯 **Test Case 삭제**
  deleteTestCase: (id) => {
    const currentVersionId = get().currentVersionId;
    if (!currentVersionId) return;
    
    set((state) => ({
      versions: state.versions.map((v) =>
        v.id === currentVersionId
          ? {
              ...v,
              runnerData: {
                ...v.runnerData,
                testCases: v.runnerData.testCases.filter((tc) => tc.id !== id),
              },
            }
          : v
      ),
      hasUnsavedChanges: true,
    }));
  },
}));
