/**
 * 엔드포인트 데이터를 DB에 저장하는 유틸리티
 * 
 * 사용 예제:
 * ```typescript
 * import { importEndpointData } from '@/utils/dataImporter';
 * 
 * const data = {
 *   endpoint: { id: 'db/nlct', name: 'NLCT', ... },
 *   version: { version: '1', ... },
 *   schema: { ... },
 *   html: '<div>...</div>',
 *   examples: [...]
 * };
 * 
 * await importEndpointData(data);
 * ```
 */

import { apiClient } from '@/lib/api-client';

export interface EndpointInfo {
  id: string;
  name: string;
  method: string;
  path: string;
  product: string;
  group_name: string;
  description?: string;
  status?: string;
}

export interface VersionInfo {
  version: string;
  author?: string;
  changeLog?: string;
}

export interface SchemaInfo {
  jsonSchema?: any;
  jsonSchemaOriginal?: any;
  jsonSchemaEnhanced?: any;
}

export interface ManualInfo {
  title?: string;
  category?: string;
  inputUri?: string;
  activeMethods?: string;
  htmlContent?: string;
  specifications?: string;
  url?: string;
}

export interface ExampleInfo {
  name: string;
  description?: string;
  request: any;
  response: any;
}

export interface ImportData {
  endpoint: EndpointInfo;
  version: VersionInfo;
  schema?: SchemaInfo;
  manual?: ManualInfo;
  examples?: ExampleInfo[];
  builderData?: any;
  runnerData?: any;
}

/**
 * 엔드포인트와 관련 데이터를 한번에 DB에 저장
 */
export async function importEndpointData(data: ImportData) {
  try {
    // 1. 엔드포인트 등록 (이미 있으면 무시)
    console.log('📌 Creating endpoint:', data.endpoint.id);
    const endpointResult = await apiClient.createEndpoint(data.endpoint);
    
    if (endpointResult.error && !endpointResult.error.includes('UNIQUE constraint')) {
      console.warn('⚠️ Endpoint already exists, continuing...');
    }

    // 2. 버전 ID 생성
    const versionId = `v_${Date.now()}_${Math.random().toString(36).substr(2, 9)}`;
    
    // 3. 버전 데이터 구성
    const versionData: any = {
      id: versionId,
      version: data.version.version,
      endpointId: data.endpoint.id,
      author: data.version.author || null,
      changeLog: data.version.changeLog || null,
    };

    // 4. Manual 데이터 구성
    if (data.schema || data.manual || data.examples) {
      versionData.manualData = {
        title: data.manual?.title || data.endpoint.name,
        category: data.manual?.category || data.endpoint.group_name,
        inputUri: data.manual?.inputUri || data.endpoint.path,
        activeMethods: data.manual?.activeMethods || data.endpoint.method,
        
        // 스키마
        jsonSchema: data.schema?.jsonSchema ? JSON.stringify(data.schema.jsonSchema) : '{}',
        jsonSchemaOriginal: data.schema?.jsonSchemaOriginal ? JSON.stringify(data.schema.jsonSchemaOriginal) : null,
        jsonSchemaEnhanced: data.schema?.jsonSchemaEnhanced ? JSON.stringify(data.schema.jsonSchemaEnhanced) : null,
        
        // HTML 및 스펙
        htmlContent: data.manual?.htmlContent || null,
        specifications: data.manual?.specifications || null,
        url: data.manual?.url || null,
        
        // 예제들
        examples: data.examples || [],
      };
    }

    // 5. Spec 데이터 구성
    if (data.schema) {
      versionData.specData = {
        jsonSchema: data.schema.jsonSchema ? JSON.stringify(data.schema.jsonSchema) : '{}',
        jsonSchemaOriginal: data.schema.jsonSchemaOriginal ? JSON.stringify(data.schema.jsonSchemaOriginal) : null,
        jsonSchemaEnhanced: data.schema.jsonSchemaEnhanced ? JSON.stringify(data.schema.jsonSchemaEnhanced) : null,
        specifications: data.manual?.specifications || null,
      };
    }

    // 6. Builder 데이터
    if (data.builderData) {
      versionData.builderData = data.builderData;
    }

    // 7. Runner 데이터
    if (data.runnerData) {
      versionData.runnerData = data.runnerData;
    }

    // 8. 버전 생성
    console.log('📦 Creating version:', versionId);
    const versionResult = await apiClient.createVersion(versionData);
    
    if (versionResult.error) {
      throw new Error(versionResult.error);
    }

    console.log('✅ Import successful!', { endpointId: data.endpoint.id, versionId });
    return { success: true, endpointId: data.endpoint.id, versionId };
    
  } catch (error) {
    console.error('❌ Import failed:', error);
    return { 
      success: false, 
      error: error instanceof Error ? error.message : 'Unknown error' 
    };
  }
}

/**
 * 여러 엔드포인트를 한번에 임포트
 */
export async function importMultipleEndpoints(dataArray: ImportData[]) {
  const results = [];
  
  for (const data of dataArray) {
    const result = await importEndpointData(data);
    results.push({
      endpointId: data.endpoint.id,
      ...result,
    });
  }
  
  const successCount = results.filter(r => r.success).length;
  const failCount = results.filter(r => !r.success).length;
  
  console.log(`📊 Import complete: ${successCount} succeeded, ${failCount} failed`);
  
  return {
    total: results.length,
    succeeded: successCount,
    failed: failCount,
    results,
  };
}

/**
 * JSON 파일에서 데이터를 읽어서 임포트
 */
export async function importFromJSON(jsonString: string) {
  try {
    const data = JSON.parse(jsonString);
    
    if (Array.isArray(data)) {
      return importMultipleEndpoints(data);
    } else {
      return importEndpointData(data);
    }
  } catch (error) {
    console.error('❌ JSON parse error:', error);
    return {
      success: false,
      error: error instanceof Error ? error.message : 'Invalid JSON'
    };
  }
}

/**
 * 파일 업로드를 통한 임포트
 */
export async function importFromFile(file: File) {
  return new Promise((resolve, reject) => {
    const reader = new FileReader();
    
    reader.onload = async (e) => {
      try {
        const content = e.target?.result as string;
        const result = await importFromJSON(content);
        resolve(result);
      } catch (error) {
        reject(error);
      }
    };
    
    reader.onerror = () => reject(reader.error);
    reader.readAsText(file);
  });
}

/**
 * 샘플 데이터 생성 (테스트/개발용)
 * 
 * ⚠️ 이 함수는 개발 및 테스트 목적으로만 사용됩니다.
 * 프로덕션에서는 Import 기능을 통해 실제 데이터를 추가하세요.
 */
export function createSampleData(): ImportData {
  console.warn('⚠️ createSampleData() is for testing only. Use Import feature for production data.');
  
  return {
    endpoint: {
      id: 'test/sample',
      name: 'Sample Test API',
      method: 'POST',
      path: '/test/sample',
      product: 'test-product',
      group_name: 'Test',
      description: '[TEST ONLY] Sample endpoint for development',
      status: 'active',
    },
    version: {
      version: '0.0.1-dev',
      author: 'Developer',
      changeLog: 'Test data - DO NOT USE IN PRODUCTION',
    },
    schema: {
      jsonSchema: {
        type: 'object',
        properties: {
          testField: { type: 'string', description: 'Test field' },
          testValue: { type: 'number', description: 'Test value' },
        },
        required: ['testField'],
      },
    },
    manual: {
      title: '[TEST] Sample API Manual',
      category: 'Test',
      htmlContent: `
        <div class="manual">
          <h1>⚠️ Test Data Only</h1>
          <p>This is test data for development purposes.</p>
          <p><strong>Do not use in production!</strong></p>
        </div>
      `,
      specifications: 'Test data for development and testing.',
    },
    examples: [
      {
        name: 'Test Example',
        description: 'Simple test example',
        request: {
          testField: 'test',
          testValue: 123,
        },
        response: {
          success: true,
          message: 'Test response',
        },
      },
    ],
  };
}









