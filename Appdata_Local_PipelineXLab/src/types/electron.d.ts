export interface StopOptions {
  removeVolumes?: boolean; // 기본값: false (데이터 보존)
}

export interface ServiceDiagnosis {
  serviceId: string;
  cause: string;
  fix: string;
}

export interface ElectronAPI {
  // Service Control
  launchAll: () => Promise<{ success: boolean; error?: string }>;
  stopAll: (options?: StopOptions) => Promise<{ success: boolean }>;
  resetAll: () => Promise<{ success: boolean }>; // 완전 초기화 (2단계 확인 필요!)
  resetDatabase: () => Promise<{ success: boolean }>; // DB만 초기화
  getServiceStatus: (serviceId: string) => Promise<any>;
  getAllServices: () => Promise<any[]>;

  // Event Listeners (cleanup 함수 반환)
  onServiceStatus: (callback: (data: any) => void) => () => void;
  onServiceLog: (callback: (data: any) => void) => () => void;
  onServiceDiagnosis: (callback: (data: ServiceDiagnosis) => void) => () => void;

  // External Links
  openExternal: (url: string) => Promise<{ success: boolean }>;
}

declare global {
  interface Window {
    electronAPI?: ElectronAPI;
  }
}

export {};
