import { contextBridge, ipcRenderer } from 'electron';

contextBridge.exposeInMainWorld('electronAPI', {
  // Service Control
  launchAll: () => ipcRenderer.invoke('launch-all'),
  stopAll: (options?: any) => ipcRenderer.invoke('stop-all', options),
  startService: (serviceId: string) => ipcRenderer.invoke('start-service', serviceId),
  stopService: (serviceId: string) => ipcRenderer.invoke('stop-service', serviceId),
  restartService: (serviceId: string) => ipcRenderer.invoke('restart-service', serviceId),
  resetAll: () => ipcRenderer.invoke('reset-all'),
  resetDatabase: () => ipcRenderer.invoke('reset-database'),
  getServiceStatus: (serviceId: string) =>
    ipcRenderer.invoke('get-service-status', serviceId),
  getAllServices: () => ipcRenderer.invoke('get-all-services'),

  // Service Events (cleanup 함수 반환)
  onServiceStatus: (callback: (data: any) => void) => {
    const listener = (_: any, data: any) => callback(data);
    ipcRenderer.on('service-status', listener);
    return () => ipcRenderer.removeListener('service-status', listener);
  },

  onServiceLog: (callback: (data: any) => void) => {
    const listener = (_: any, data: any) => callback(data);
    ipcRenderer.on('service-log', listener);
    return () => ipcRenderer.removeListener('service-log', listener);
  },

  onServiceDiagnosis: (callback: (data: any) => void) => {
    const listener = (_: any, data: any) => callback(data);
    ipcRenderer.on('service-diagnosis', listener);
    return () => ipcRenderer.removeListener('service-diagnosis', listener);
  },

  // Component Management
  getComponents: () => ipcRenderer.invoke('get-components'),
  getComponentDetails: (category: string, name: string) =>
    ipcRenderer.invoke('get-component-details', category, name),
  openComponentFolder: (path: string) => ipcRenderer.invoke('open-component-folder', path),

  // Docker Management
  checkDocker: () => ipcRenderer.invoke('check-docker'),
  startDocker: () => ipcRenderer.invoke('start-docker'),

  // Settings
  getSettings: () => ipcRenderer.invoke('get-settings'),
  setSetting: (key: string, value: any) => ipcRenderer.invoke('set-setting', key, value),
  browseForPath: (title: string) => ipcRenderer.invoke('browse-for-path', title),

  // External Links
  openExternal: (url: string) => ipcRenderer.invoke('open-external', url),
});

