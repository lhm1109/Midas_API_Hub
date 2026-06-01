// 🎯 API 클라이언트 (services/api.ts를 lib으로 이동)
import { API_CONFIG } from '@/config/constants';
import type { Version } from '@/types';

interface ApiResponse<T> {
  data?: T;
  error?: string;
}

export interface SchemaSplitResult {
  requestKey: string;
  responseKey: string;
  requestSchema: any;
  responseSchema: any;
}

export interface SchemaCompareResult {
  matches: boolean;
  missingInMerged: string[];
  extraInMerged: string[];
  changedSchemas: string[];
}

export interface SchemaMergeResponse {
  mergedSchema: any;
  expectedSchema: any | null;
  matchesSource: boolean | null;
  comparison: SchemaCompareResult | null;
  autoFixedMerged: any | null;
}

export interface SchemaRoundtripResponse {
  splitResult: SchemaSplitResult;
  mergedSchema: any;
  expectedSchema: any;
  matchesSource: boolean;
  comparison: SchemaCompareResult;
  autoFixedMerged: any | null;
}

export interface PydanticResponse {
  code: string;
  rootClasses: string[];
  filePath: string;
  registryPath: string;
  manifestPath?: string;
  moduleName: string;
  productId: string;
  importPath?: string;
  saved: boolean;
}

export interface PydanticValidationResult {
  status: 'valid' | 'invalid' | 'skipped' | 'missing_model' | 'error';
  valid: boolean;
  modelName?: string;
  message?: string;
  errors?: any[];
  payload?: any;
}

export interface PydanticRunResponse {
  ok: boolean;
  stage: 'request_validation' | 'complete';
  requestValidation: PydanticValidationResult;
  responseValidation: PydanticValidationResult | null;
  http: {
    ok: boolean;
    status: number;
    statusText: string;
    time: number;
  } | null;
  responseBody: string;
  responseIsJson: boolean;
}

export interface PydanticCodeRunResponse {
  ok: boolean;
  exitCode: number | null;
  signal: string | null;
  stdout: string;
  stderr: string;
  parsedStdout: any;
  time: number;
}

export interface NimbalystExtensionSummary {
  id: string;
  name: string;
  version: string;
  description?: string;
  extensionPath?: string;
  main?: string;
  active: boolean;
  permissions?: Record<string, unknown>;
  contributions?: Record<string, unknown>;
  toolNames?: string[];
  error?: string;
}

export interface NimbalystMarketplaceExtensionSummary {
  id: string;
  name: string;
  description: string;
  version: string;
  author: string;
  categories: string[];
  tags: string[];
  icon: string;
  tagline: string;
  longDescription: string;
  highlights: string[];
  fileTypes: string[];
  permissions: string[];
  downloadUrl: string;
  checksum: string;
  availableLocal: boolean;
  installed: boolean;
  installedVersion: string;
  installable: boolean;
  buildable: boolean;
  requiresBuild: boolean;
}

export interface NimbalystMarketplaceInstallAllResult {
  installed: number;
  failed: number;
  results: Array<{
    id: string;
    name: string;
    success: boolean;
    error?: string;
    extension?: NimbalystExtensionSummary;
    build?: { skipped: boolean; stdout: string; stderr: string };
  }>;
}

export interface NimbalystExtensionToolSummary {
  name: string;
  originalName: string;
  description: string;
  inputSchema: any;
  scope: 'global' | 'editor';
  extensionId: string;
  extensionName: string;
}

export interface NimbalystExtensionToolResult {
  success: boolean;
  message?: string;
  data?: unknown;
  error?: string;
  extensionId?: string;
  toolName?: string;
  stack?: string;
  errorContext?: Record<string, unknown>;
}

export interface NimbalystExtensionContributionSummary {
  type: string;
  id: string;
  title: string;
  description: string;
  extensionId: string;
  extensionName: string;
  main: string;
  raw: unknown;
}

export interface NimbalystSlashCommandSummary {
  id: string;
  title: string;
  description: string;
  icon?: string;
  keywords: string[];
  handler: string;
  executable: boolean;
  extensionId: string;
  extensionName: string;
}

export interface NimbalystCommandSummary {
  id: string;
  title: string;
  executable: boolean;
  extensionId: string;
  extensionName: string;
}

export interface NimbalystConfigurationProperty {
  type: 'string' | 'number' | 'boolean' | 'array' | 'object';
  default?: unknown;
  description?: string;
  enum?: Array<string | number>;
  enumDescriptions?: string[];
  scope?: 'user' | 'workspace' | 'both';
  order?: number;
  minimum?: number;
  maximum?: number;
  pattern?: string;
  placeholder?: string;
}

export interface NimbalystExtensionConfiguration {
  extensionId: string;
  extensionName: string;
  title: string;
  properties: Record<string, NimbalystConfigurationProperty>;
  values: Record<string, unknown>;
}

export interface NimbalystBackendPermissionDescriptor {
  id: string;
  label: string;
  description: string;
  risk: 'low' | 'elevated' | 'high';
}

export interface NimbalystBackendModuleIssue {
  moduleId?: string;
  message: string;
  severity?: 'error' | 'warning';
}

export interface NimbalystBackendModuleSummary {
  id: string;
  entry: string;
  runtime: 'utility-process' | 'worker-thread';
  permissions: string[];
  declaredPermissions: string[];
  enablement: {
    default: 'disabled';
    promptOn: 'firstUse';
    purpose: string;
  } | null;
  entryExists: boolean;
  globalEnabled: boolean;
  workspaceEnabled: boolean;
  issues: NimbalystBackendModuleIssue[];
}

export interface NimbalystExtensionBackendModules {
  extensionId: string;
  extensionName: string;
  workspacePath: string;
  permissions: NimbalystBackendPermissionDescriptor[];
  modules: NimbalystBackendModuleSummary[];
  issues: NimbalystBackendModuleIssue[];
}

export interface NimbalystBackendModuleExecutionResult {
  success: boolean;
  data?: unknown;
  error?: string;
  details?: unknown;
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
    return this.request<any>(`/endpoints/${encodeURIComponent(id)}`);
  }

  async createEndpoint(data: any) {
    return this.request<{ id: string; message: string }>('/endpoints', {
      method: 'POST',
      body: JSON.stringify(data),
    });
  }

  async updateEndpoint(id: string, data: any) {
    return this.request<{ message: string; newId?: string }>(`/endpoints/${encodeURIComponent(id)}`, {
      method: 'PUT',
      body: JSON.stringify(data),
    });
  }

  async deleteEndpoint(id: string) {
    return this.request<{ message: string }>(`/endpoints/${encodeURIComponent(id)}`, {
      method: 'DELETE',
    });
  }

  async reorderEndpoints(endpoints: Array<{ id: string; order_index: number }>) {
    return this.request<{ message: string; count: number }>('/endpoints/reorder', {
      method: 'PUT',
      body: JSON.stringify({ endpoints }),
    });
  }

  async duplicateEndpoint(id: string, data?: {
    name?: string;
    path?: string;
    product_id?: string;
    group_id?: string;
  }) {
    return this.request<{ endpoint: any; message: string }>(`/endpoints/${encodeURIComponent(id)}/duplicate`, {
      method: 'POST',
      body: JSON.stringify(data || {}),
    });
  }

  async moveEndpoint(id: string, product: string, group_name: string, order_index?: number) {
    return this.request<{ endpoint: any; message: string }>(`/endpoints/${encodeURIComponent(id)}/move`, {
      method: 'PUT',
      body: JSON.stringify({ product, group_name, order_index }),
    });
  }

  // ✅ 새로운 API: group_id 기반으로 엔드포인트 이동
  async moveEndpointToGroup(id: string, group_id: string, order_index?: number) {
    return this.request<{ endpoint: any; message: string }>(`/endpoints/${encodeURIComponent(id)}/move-to-group`, {
      method: 'PUT',
      body: JSON.stringify({ group_id, order_index }),
    });
  }

  // Products API
  async getProducts() {
    return this.request<any[]>('/products');
  }

  async createProduct(data: { id: string; name: string; description?: string; psd_set?: string; schema_type?: string }) {
    return this.request<{ product: any; message: string }>('/products', {
      method: 'POST',
      body: JSON.stringify(data),
    });
  }

  async updateProduct(id: string, data: { name?: string; description?: string; psd_set?: string; schema_type?: string }) {
    return this.request<{ product: any; message: string }>(`/products/${encodeURIComponent(id)}`, {
      method: 'PUT',
      body: JSON.stringify(data),
    });
  }

  async reorderProducts(products: Array<{ id: string; order_index: number }>) {
    return this.request<{ message: string; count: number }>('/products/reorder', {
      method: 'PUT',
      body: JSON.stringify({ products }),
    });
  }

  async deleteProduct(id: string) {
    return this.request<{ message: string }>(`/products/${encodeURIComponent(id)}`, {
      method: 'DELETE',
    });
  }

  // Groups API
  async getGroups(product_id?: string) {
    const query = product_id ? `?product_id=${encodeURIComponent(product_id)}` : '';
    return this.request<any[]>(`/groups${query}`);
  }

  async createGroup(data: { id: string; product_id: string; name: string; description?: string }) {
    return this.request<{ group: any; message: string }>('/groups', {
      method: 'POST',
      body: JSON.stringify(data),
    });
  }

  async reorderGroups(groups: Array<{ id: string; order_index: number }>) {
    return this.request<{ message: string; count: number }>('/groups/reorder', {
      method: 'PUT',
      body: JSON.stringify({ groups }),
    });
  }

  async moveGroup(id: string, parent_group_id: string | null, order_index?: number) {
    return this.request<{ message: string; count: number }>(`/groups/${encodeURIComponent(id)}/move`, {
      method: 'PUT',
      body: JSON.stringify({ parent_group_id, order_index }),
    });
  }

  async deleteGroup(id: string) {
    return this.request<{ message: string }>(`/groups/${encodeURIComponent(id)}`, {
      method: 'DELETE',
    });
  }

  async renameGroup(id: string, newName: string) {
    return this.request<{ group: any; message: string }>(`/groups/${encodeURIComponent(id)}`, {
      method: 'PUT',
      body: JSON.stringify({ name: newName }),
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

  // Schema Tools API
  async splitSchema(sourceSchema: any) {
    return this.request<SchemaSplitResult>('/schema-tools/split', {
      method: 'POST',
      body: JSON.stringify({ sourceSchema }),
    });
  }

  async mergeSchema(payload: {
    requestSchema: any;
    responseSchema: any;
    requestKey?: string;
    responseKey?: string;
    sourceSchema?: any;
    autoFixAgainstSource?: boolean;
  }) {
    return this.request<SchemaMergeResponse>('/schema-tools/merge', {
      method: 'POST',
      body: JSON.stringify(payload),
    });
  }

  async roundtripSchema(payload: {
    sourceSchema: any;
    autoFixAgainstSource?: boolean;
  }) {
    return this.request<SchemaRoundtripResponse>('/schema-tools/roundtrip', {
      method: 'POST',
      body: JSON.stringify(payload),
    });
  }

  async generatePydantic(payload: {
    schema: any;
    endpoint: {
      id: string;
      name: string;
      method?: string;
      path?: string;
      productId?: string;
    };
  }) {
    return this.request<PydanticResponse>('/pydantic/generate', {
      method: 'POST',
      body: JSON.stringify(payload),
    });
  }

  async savePydantic(payload: {
    schema: any;
    endpoint: {
      id: string;
      name: string;
      method?: string;
      path?: string;
      productId?: string;
    };
  }) {
    return this.request<PydanticResponse>('/pydantic/save', {
      method: 'POST',
      body: JSON.stringify(payload),
    });
  }

  async runPydantic(payload: {
    endpoint: {
      id: string;
      name: string;
      method?: string;
      path?: string;
      productId?: string;
    };
    method: string;
    url: string;
    headers: Record<string, string>;
    requestBody: string;
  }) {
    return this.request<PydanticRunResponse>('/pydantic/run', {
      method: 'POST',
      body: JSON.stringify(payload),
    });
  }

  async executePydanticCode(payload: {
    code: string;
    endpoint: {
      id: string;
      name: string;
      method?: string;
      path?: string;
      productId?: string;
    };
    method: string;
    url: string;
    headers: Record<string, string>;
  }) {
    return this.request<PydanticCodeRunResponse>('/pydantic/execute', {
      method: 'POST',
      body: JSON.stringify(payload),
    });
  }

  async listNimbalystExtensions() {
    return this.request<{ extensions: NimbalystExtensionSummary[] }>('/nimbalyst/extensions');
  }

  async listNimbalystMarketplaceExtensions() {
    return this.request<{ extensions: NimbalystMarketplaceExtensionSummary[] }>(
      '/nimbalyst/marketplace/extensions'
    );
  }

  async installNimbalystMarketplaceExtension(id: string) {
    return this.request<{ extension: NimbalystExtensionSummary }>(
      `/nimbalyst/marketplace/extensions/${encodeURIComponent(id)}/install`,
      { method: 'POST' }
    );
  }

  async buildNimbalystMarketplaceExtension(id: string) {
    return this.request<{
      extension: NimbalystExtensionSummary;
      build: { skipped: boolean; stdout: string; stderr: string };
    }>(
      `/nimbalyst/marketplace/extensions/${encodeURIComponent(id)}/build-install`,
      { method: 'POST' }
    );
  }

  async buildAllNimbalystMarketplaceExtensions() {
    return this.request<NimbalystMarketplaceInstallAllResult>(
      '/nimbalyst/marketplace/extensions/build-install-all',
      { method: 'POST' }
    );
  }

  async activateNimbalystExtension(id: string, payload: { workspacePath?: string } = {}) {
    return this.request<{ extension: NimbalystExtensionSummary }>(
      `/nimbalyst/extensions/${encodeURIComponent(id)}/activate`,
      {
        method: 'POST',
        body: JSON.stringify(payload),
      }
    );
  }

  async deactivateNimbalystExtension(id: string) {
    return this.request<{ extension: { id: string; active: false } }>(
      `/nimbalyst/extensions/${encodeURIComponent(id)}/deactivate`,
      { method: 'POST' }
    );
  }

  async reloadNimbalystExtension(id: string, payload: { workspacePath?: string } = {}) {
    return this.request<{ extension: NimbalystExtensionSummary }>(
      `/nimbalyst/extensions/${encodeURIComponent(id)}/reload`,
      {
        method: 'POST',
        body: JSON.stringify(payload),
      }
    );
  }

  async listNimbalystExtensionTools() {
    return this.request<{ tools: NimbalystExtensionToolSummary[] }>('/nimbalyst/tools');
  }

  async listNimbalystSlashCommands() {
    return this.request<{ commands: NimbalystSlashCommandSummary[] }>('/nimbalyst/slash-commands');
  }

  async listNimbalystCommands() {
    return this.request<{ commands: NimbalystCommandSummary[] }>('/nimbalyst/commands');
  }

  async listNimbalystExtensionContributions() {
    return this.request<{ contributions: NimbalystExtensionContributionSummary[] }>(
      '/nimbalyst/contributions'
    );
  }

  async getNimbalystExtensionConfiguration(id: string) {
    return this.request<{ configuration: NimbalystExtensionConfiguration }>(
      `/nimbalyst/extensions/${encodeURIComponent(id)}/configuration`
    );
  }

  async updateNimbalystExtensionConfiguration(id: string, values: Record<string, unknown>) {
    return this.request<{ configuration: NimbalystExtensionConfiguration }>(
      `/nimbalyst/extensions/${encodeURIComponent(id)}/configuration`,
      {
        method: 'PUT',
        body: JSON.stringify({ values }),
      }
    );
  }

  async getNimbalystExtensionBackendModules(id: string, workspacePath?: string) {
    const query = workspacePath ? `?workspacePath=${encodeURIComponent(workspacePath)}` : '';
    return this.request<{ backendModules: NimbalystExtensionBackendModules }>(
      `/nimbalyst/extensions/${encodeURIComponent(id)}/backend-modules${query}`
    );
  }

  async setNimbalystExtensionBackendModuleGrant(payload: {
    extensionId: string;
    moduleId: string;
    scope: 'workspace' | 'global';
    enabled: boolean;
    workspacePath?: string;
  }) {
    return this.request<{ backendModules: NimbalystExtensionBackendModules }>(
      `/nimbalyst/extensions/${encodeURIComponent(payload.extensionId)}/backend-modules/${encodeURIComponent(payload.moduleId)}/grant`,
      {
        method: 'PUT',
        body: JSON.stringify({
          scope: payload.scope,
          enabled: payload.enabled,
          workspacePath: payload.workspacePath,
        }),
      }
    );
  }

  async executeNimbalystBackendModule(payload: {
    extensionId: string;
    moduleId: string;
    method: string;
    params?: Record<string, unknown>;
    workspacePath?: string;
  }) {
    return this.request<NimbalystBackendModuleExecutionResult>(
      `/nimbalyst/extensions/${encodeURIComponent(payload.extensionId)}/backend-modules/${encodeURIComponent(payload.moduleId)}/execute`,
      {
        method: 'POST',
        body: JSON.stringify({
          method: payload.method,
          params: payload.params || {},
          workspacePath: payload.workspacePath,
        }),
      }
    );
  }

  async executeNimbalystCommand(payload: {
    commandId: string;
    args?: Record<string, unknown>;
    workspacePath?: string;
    activeFilePath?: string;
  }) {
    return this.request<NimbalystExtensionToolResult>(
      `/nimbalyst/commands/${encodeURIComponent(payload.commandId)}/execute`,
      {
        method: 'POST',
        body: JSON.stringify({
          args: payload.args || {},
          workspacePath: payload.workspacePath,
          activeFilePath: payload.activeFilePath,
        }),
      }
    );
  }

  async executeNimbalystExtensionTool(payload: {
    toolName: string;
    args?: Record<string, unknown>;
    workspacePath?: string;
    activeFilePath?: string;
  }) {
    return this.request<NimbalystExtensionToolResult>(
      `/nimbalyst/tools/${encodeURIComponent(payload.toolName)}/execute`,
      {
        method: 'POST',
        body: JSON.stringify({
          args: payload.args || {},
          workspacePath: payload.workspacePath,
          activeFilePath: payload.activeFilePath,
        }),
      }
    );
  }

  async executeNimbalystSlashCommand(payload: {
    commandId: string;
    args?: string;
    workspacePath?: string;
    activeFilePath?: string;
  }) {
    return this.request<NimbalystExtensionToolResult>(
      `/nimbalyst/slash-commands/${encodeURIComponent(payload.commandId)}/execute`,
      {
        method: 'POST',
        body: JSON.stringify({
          args: payload.args || '',
          workspacePath: payload.workspacePath,
          activeFilePath: payload.activeFilePath,
        }),
      }
    );
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

