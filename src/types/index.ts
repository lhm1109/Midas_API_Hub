// 🎯 공통 타입 정의

export interface ApiEndpoint {
  id: string;
  name: string;
  method: 'GET' | 'POST' | 'PUT' | 'DELETE' | 'PATCH';
  path: string;
  status?: 'success' | 'error' | null;
  statusMessage?: string; // 🔥 상태 메시지 추가
  order_index?: number;
}

export interface ApiGroup {
  id: string;
  name: string;
  parent_group_id?: string | null;
  depth: number;           // ✅ 필수로 변경
  order_index?: number;
  subgroups: ApiGroup[];   // ✅ 필수로 변경 (재귀 타입)
  endpoints: ApiEndpoint[];
}

export interface ApiProduct {
  id: string;
  name: string;
  psd_set?: string; // PSD Set (Level 1: default, civil_gen_definition, etc.)
  schema_type?: string; // Schema Type (Level 2: enhanced, manual, original)
  // Deprecated: use psd_set + schema_type instead
  schema_definition?: string;
  groups: ApiGroup[];
}

// Spec 데이터 타입
export interface SpecData {
  jsonSchema: string;
  jsonSchemaOriginal?: string;
  jsonSchemaEnhanced?: string;
  specifications: string;
}

// Builder 데이터 타입
export interface BuilderData {
  formData: Record<string, any>;
}

// Runner 데이터 타입
export interface RunnerData {
  requestBody: string;
  responseBody: string;
  testCases: TestCase[];
}

export interface TestCase {
  id: string;
  name: string;
  description?: string;
  requestBody: string;
  responseBody?: string;  // 🔥 Response 저장용
  responseStatus?: number;  // 🔥 HTTP 상태 코드
  responseTime?: number;    // 🔥 응답 시간 (ms)
  createdAt: string;
  updatedAt: string;
}

// Manual 데이터 타입
export interface ManualData {
  title: string;
  category: string;
  inputUri: string;
  activeMethods: string;
  jsonSchema: string;
  jsonSchemaOriginal?: string;
  jsonSchemaEnhanced?: string;
  examples: Example[]; // deprecated, use requestExamples and responseExamples
  requestExamples: Example[];
  responseExamples: Example[];
  specifications: string;
  htmlContent?: string;
  articleId?: string;
  sectionId?: string;
  authorId?: string;
  url?: string;
}

export interface Example {
  title: string;
  code: string;
}

// Attachment 타입
export interface Attachment {
  id: string;
  versionId: string;
  fileName: string;
  fileSize: number;
  mimeType: string;
  uploadedAt: string;
}

// Version 타입
export interface Version {
  id: string;
  version: string;
  endpointId: string;
  createdAt: string;
  updatedAt: string;
  author?: string;
  changeLog?: string;
  attachments?: Attachment[];
  manualData: ManualData;
  specData: SpecData;
  builderData: BuilderData;
  runnerData: RunnerData;
}

// Settings 타입
export interface Settings {
  baseUrl: string;
  mapiKey: string;
  commonHeaders: string;
  useAssignWrapper?: boolean;
  schemaDefinition?: 'auto' | 'original' | 'enhanced';
  schemaMode?: 'enhanced' | 'normal'; // 🔥 NEW: 스키마 UI 모드
  userName?: string; // 🔥 사용자 이름
  supabaseUrl?: string; // Supabase Project URL
  supabaseServiceKey?: string; // Supabase Service Role Key
  supabaseDbPassword?: string; // Supabase Database Password
}

