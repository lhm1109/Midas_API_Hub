const { contextBridge, ipcRenderer } = require('electron');

// Expose protected methods that allow the renderer process to use
// the ipcRenderer without exposing the entire object
contextBridge.exposeInMainWorld('electronAPI', {
  // Generic IPC invoke method
  invoke: (channel, ...args) => ipcRenderer.invoke(channel, ...args),
  
  // Dialog operations
  openDirectory: () => ipcRenderer.invoke('dialog:openDirectory'),
  openFile: (filters) => ipcRenderer.invoke('dialog:openFile', filters),

  // File system operations
  readFile: (filePath) => ipcRenderer.invoke('fs:readFile', filePath),
  writeFile: (filePath, content) => ipcRenderer.invoke('fs:writeFile', filePath, content),
  readDirectory: (dirPath) => ipcRenderer.invoke('fs:readDirectory', dirPath),
  exists: (filePath) => ipcRenderer.invoke('fs:exists', filePath),
  stat: (filePath) => ipcRenderer.invoke('fs:stat', filePath),

  // Zendesk API operations
  zendesk: {
    getAllTranslations: (config, articleId) => 
      ipcRenderer.invoke('zendesk:getAllTranslations', config, articleId),
    getArticleTranslation: (config, articleId, locale) => 
      ipcRenderer.invoke('zendesk:getArticleTranslation', config, articleId, locale),
    updateArticleTranslation: (config, articleId, locale, body, title, draft) => 
      ipcRenderer.invoke('zendesk:updateArticleTranslation', config, articleId, locale, body, title, draft),
    getAllArticles: (config, perPage, page) => 
      ipcRenderer.invoke('zendesk:getAllArticles', config, perPage, page),
    getArticlesBySection: (config, sectionId, locale) => 
      ipcRenderer.invoke('zendesk:getArticlesBySection', config, sectionId, locale),
  },
});

