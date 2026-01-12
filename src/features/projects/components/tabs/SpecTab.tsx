import { ScrollArea } from '@/components/ui/scroll-area';
import { ResizableHandle, ResizablePanel, ResizablePanelGroup } from '@/components/ui/resizable';
import { useState, useEffect, useMemo } from 'react';
import { ChevronRight, ChevronDown, Send, Save, AlertCircle } from 'lucide-react';
import { Button } from '@/components/ui/button';
import {
  DropdownMenu,
  DropdownMenuContent,
  DropdownMenuItem,
  DropdownMenuTrigger,
} from '@/components/ui/dropdown-menu';
import { CodeEditor } from '@/components/common';
import { apiSpecs } from '@/data/apiSpecs';
import { useAppStore } from '@/store/useAppStore';
import { apiClient } from '@/lib/api-client';
import type { ManualData } from '@/types';
import { toast } from 'sonner';
import { 
  resolveActiveSchema, 
  isEnhancedSchemaActive, 
  compileSchema,
  canonicalToTableSchema,
} from '@/lib/schema';

interface SpecTabProps {
  endpoint: {
    id: string;
    name: string;
    method: string;
    path: string;
  };
}

export function SpecTab({ endpoint }: SpecTabProps) {
  const { 
    setManualData, 
    manualData, 
    currentVersionId, 
    getCurrentVersion, 
    updateSpecData,
    fetchVersions,
    loadVersion,
    specData,
  } = useAppStore();
  
  // 🔥 Schema Registry로 활성 스키마 결정 (우선순위 정책 분리)
  const fallbackSpec = apiSpecs[endpoint.id] || {
    title: endpoint.name,
    description: 'API endpoint specification',
    parameters: [],
    jsonSchema: {},
  };
  
  // 🎯 specData 상태 확인
  const hasSpecData = !!(specData?.jsonSchema);
  const isUsingFallback = !hasSpecData;
  
  // specData와 fallback을 결합
  const combinedSpecData = {
    jsonSchema: specData?.jsonSchema || specData?.jsonSchemaOriginal || fallbackSpec.jsonSchema,
    jsonSchemaOriginal: specData?.jsonSchemaOriginal || specData?.jsonSchema || fallbackSpec.jsonSchema,
    jsonSchemaEnhanced: specData?.jsonSchemaEnhanced || fallbackSpec.jsonSchemaEnhanced,
    savedSchema: (specData as any)?.savedSchema,
  };
  
  // 🎯 활성 스키마 (우선순위: savedSchema > enhanced > original)
  const activeSchema = resolveActiveSchema(combinedSpecData);
  const hasEnhancedSchema = isEnhancedSchemaActive(combinedSpecData);
  
  // 🔥 NEW: Schema Compiler로 정규화된 AST 생성
  const canonicalFields = useMemo(() => {
    return compileSchema(activeSchema);
  }, [activeSchema]);
  
  // 🔥 NEW: UI Schema Adapter로 테이블 스키마 생성
  const tableParameters = useMemo(() => {
    return canonicalToTableSchema(canonicalFields);
  }, [canonicalFields]);
  
  const spec = {
    title: fallbackSpec.title,
    description: fallbackSpec.description,
    jsonSchema: combinedSpecData.jsonSchemaOriginal || {},
    jsonSchemaEnhanced: combinedSpecData.jsonSchemaEnhanced,
    uri: fallbackSpec?.uri,
    methods: fallbackSpec?.methods,
  };

  // Track which parameters are expanded
  const [expandedParams, setExpandedParams] = useState<Set<number>>(new Set());
  
  // 🎯 Schema View Toggle: 'original' | 'enhanced'
  const [schemaView, setSchemaView] = useState<'original' | 'enhanced'>(() => {
    return hasEnhancedSchema ? 'enhanced' : 'original';
  });
  
  // 🎯 Editable Schema State
  const [editableSchema, setEditableSchema] = useState<string>('');
  const [isSchemaModified, setIsSchemaModified] = useState(false);
  
  // 🎯 저장된 스키마를 추적하는 state (리렌더링 트리거용)
  const [savedSchema, setSavedSchema] = useState<any>(null);
  
  // 🔥 endpoint 변경 시 schemaView 재설정 (Enhanced 우선)
  useEffect(() => {
    if (hasEnhancedSchema) {
      setSchemaView('enhanced');
    } else {
      setSchemaView('original');
    }
  }, [endpoint.id, hasEnhancedSchema]);
  
  // Initialize editable schema
  useEffect(() => {
    // 🔥 이미 수정 중인 경우 초기화하지 않음 (탭 전환 대응)
    if (isSchemaModified && editableSchema) {
      return;
    }
    
    if (schemaView === 'original') {
      setEditableSchema(JSON.stringify(spec.jsonSchema, null, 2));
    } else {
      setEditableSchema(
        spec.jsonSchemaEnhanced 
          ? JSON.stringify(spec.jsonSchemaEnhanced, null, 2)
          : '// Enhanced schema not available yet\n// Add enhanced schema to apiSpecs data'
      );
    }
    setIsSchemaModified(false);
  }, [schemaView, endpoint.id]); // endpoint.id로 변경
  
  // Handle schema changes
  const handleSchemaChange = (value: string) => {
    setEditableSchema(value);
    setIsSchemaModified(true);
  };
  
  // Parse and validate JSON
  const getSchemaForDisplay = () => {
    try {
      if (isSchemaModified) {
        return JSON.parse(editableSchema);
      }
    } catch (e) {
      // If invalid JSON, return the original
    }
    return schemaView === 'original' ? spec.jsonSchema : spec.jsonSchemaEnhanced;
  };
  
  // 🎯 Display parameters - 이제 tableParameters 직접 사용
  const displayParameters = tableParameters;
    
  const toggleParam = (paramNo: number) => {
    setExpandedParams((prev) => {
      const next = new Set(prev);
      if (next.has(paramNo)) {
        next.delete(paramNo);
      } else {
        next.add(paramNo);
      }
      return next;
    });
  };
    
  // 🎯 Schema를 Manual로 전송
  const handleSendSchemaToManual = (schemaType: 'original' | 'enhanced') => {
    const formatJsonSchemaToHTML = (schema: any): string => {
      const jsonStr = JSON.stringify(schema, null, 2);
      return jsonStr
        .split('\n')
        .map(line => {
          const leadingSpaces = line.match(/^(\s*)/)?.[1] || '';
          const indent = leadingSpaces.replace(/ /g, '&nbsp;&nbsp;');
          const trimmedLine = line.trim();
          
          const keyMatch = trimmedLine.match(/^"([^"]+)":\s*(.+)$/);
          if (keyMatch) {
            const key = keyMatch[1];
            let value = keyMatch[2];
            const hasComma = value.endsWith(',');
            if (hasComma) {
              value = value.slice(0, -1);
            }
            
            let styledValue = value;
            if (value === 'true' || value === 'false') {
              styledValue = `<span style="color: #055bcc; font-weight: bold;">${value}</span>`;
            } else if (value.match(/^"[^"]*"$/)) {
              styledValue = `<span style="color: #055bcc;">${value}</span>`;
            } else if (value.match(/^-?\d+(\.\d+)?$/)) {
              styledValue = `<span style="color: #0ab66c;">${value}</span>`;
            } else if (value === '{' || value === '[') {
              styledValue = value;
      }
      
            const styledLine = `${indent}<span style="color: #c31b1b;">"${key}"</span>: ${styledValue}${hasComma ? ',' : ''}`;
            return styledLine;
          }
          
          if (trimmedLine.match(/^[{\[\}\]],?$/)) {
            return indent + trimmedLine;
          }
          
          return indent + trimmedLine;
        })
        .join('<br>');
    };

    // 🔥 Enhanced 스키마인지 확인하는 함수
    const isEnhancedSchemaStructure = (schema: any): boolean => {
      if (!schema || typeof schema !== 'object') return false;
      
      const schemaStr = JSON.stringify(schema);
      // Enhanced 스키마의 특징: oneOf, x-section-header 등
      return schemaStr.includes('oneOf') || 
             schemaStr.includes('x-section-header') || 
             schemaStr.includes('allOf');
    };

    // 🔥 Original: fallbackSpec 또는 DB에서 가져오되, Enhanced 구조가 아닌 것만
    // Enhanced: DB의 jsonSchemaEnhanced 또는 Enhanced 구조를 가진 jsonSchemaOriginal
    let originalSchema: any;
    let enhancedSchema: any;
    
    if (fallbackSpec.jsonSchema && Object.keys(fallbackSpec.jsonSchema).length > 0) {
      // fallbackSpec이 있으면 그것을 Original로 사용
      originalSchema = fallbackSpec.jsonSchema;
      enhancedSchema = specData?.jsonSchemaEnhanced;
            } else {
      // fallbackSpec이 없으면 DB 데이터 사용
      const dbOriginal = specData?.jsonSchemaOriginal || specData?.jsonSchema;
      const dbEnhanced = specData?.jsonSchemaEnhanced;
      
      if (isEnhancedSchemaStructure(dbOriginal) && (!dbEnhanced || Object.keys(dbEnhanced).length === 0)) {
        // DB에 Enhanced가 Original에 저장되어 있는 경우
        console.warn('⚠️ Enhanced schema found in jsonSchemaOriginal! Using it as Enhanced.');
        originalSchema = {}; // Original이 없음
        enhancedSchema = dbOriginal; // Enhanced로 사용
            } else {
        // 정상적인 경우
        originalSchema = dbOriginal || {};
        enhancedSchema = dbEnhanced;
            }
          }
          
    console.log('🔍 specData:', {
      jsonSchema: specData?.jsonSchema,
      jsonSchemaOriginal: specData?.jsonSchemaOriginal,
      jsonSchemaEnhanced: specData?.jsonSchemaEnhanced
    });
    console.log('🔍 fallbackSpec.jsonSchema:', fallbackSpec.jsonSchema);
    console.log('🔍 Final Original Schema:', originalSchema);
    console.log('🔍 Final Enhanced Schema:', enhancedSchema);
    
    // 🎯 스키마 타입에 따라 레이블 설정
    let schemaLabel: string;
    
    if (schemaType === 'original') {
      schemaLabel = 'Original';
    } else {
      if (!enhancedSchema || Object.keys(enhancedSchema).length === 0) {
        toast.error('❌ Enhanced schema is not available!');
        return;
      }
      schemaLabel = 'Enhanced';
    }

    // 🔥 완전히 새로운 ManualData 객체 생성 (기존 값 무시)
    // 🎯 JSON으로 저장 (HTML이 아닌 실제 JSON 문자열)
    const selectedSchema = schemaType === 'original' ? originalSchema : enhancedSchema;
    const newManualData: ManualData = {
      title: spec.title || endpoint.name,
      category: endpoint.method,
      inputUri: endpoint.path,
      activeMethods: endpoint.method,
      jsonSchema: JSON.stringify(selectedSchema, null, 2),  // 🔥 선택한 스키마 (JSON)
      jsonSchemaOriginal: JSON.stringify(originalSchema, null, 2),  // 🔥 항상 Original (JSON)
      jsonSchemaEnhanced: enhancedSchema ? JSON.stringify(enhancedSchema, null, 2) : undefined,  // 🔥 항상 Enhanced (JSON)
      examples: [],  // 🔥 deprecated
      requestExamples: [],  // 🔥 Request Examples 초기화
      responseExamples: [],  // 🔥 Response Examples 초기화
      specifications: '',  // 🔥 Specifications도 초기화
    };

    console.log('📤 Sending to Manual:', { 
      schemaType, 
      schemaLabel, 
      selectedLength: JSON.stringify(selectedSchema).length,
      originalLength: JSON.stringify(originalSchema).length,
      enhancedLength: enhancedSchema ? JSON.stringify(enhancedSchema).length : 0
    });
    
    setManualData(newManualData);
    toast.success(`✅ ${schemaLabel} Schema sent to Manual tab!`);
  };

  // 🎯 Table을 Manual로 전송
  const handleSendTableToManual = () => {
    const formatJsonSchemaToHTML = (schema: any): string => {
      const jsonStr = JSON.stringify(schema, null, 2);
      return jsonStr
        .split('\n')
        .map(line => {
          const leadingSpaces = line.match(/^(\s*)/)?.[1] || '';
          const indent = leadingSpaces.replace(/ /g, '&nbsp;&nbsp;');
          const trimmedLine = line.trim();
          
          const keyMatch = trimmedLine.match(/^"([^"]+)":\s*(.+)$/);
          if (keyMatch) {
            const key = keyMatch[1];
            let value = keyMatch[2];
            const hasComma = value.endsWith(',');
            if (hasComma) {
              value = value.slice(0, -1);
            }
            
            let styledValue = value;
            if (value === 'true' || value === 'false') {
              styledValue = `<span style="color: #055bcc; font-weight: bold;">${value}</span>`;
            } else if (value.match(/^"[^"]*"$/)) {
              styledValue = `<span style="color: #055bcc;">${value}</span>`;
            } else if (value.match(/^-?\d+(\.\d+)?$/)) {
              styledValue = `<span style="color: #0ab66c;">${value}</span>`;
            } else if (value === '{' || value === '[') {
              styledValue = value;
            }
            
            const styledLine = `${indent}<span style="color: #c31b1b;">"${key}"</span>: ${styledValue}${hasComma ? ',' : ''}`;
            return styledLine;
          }
          
          if (trimmedLine.match(/^[{\[\}\]],?$/)) {
            return indent + trimmedLine;
          }
          
          return indent + trimmedLine;
        })
        .join('<br>');
    };

    // Specifications 테이블을 HTML로 변환
    const generateSpecificationsTable = (): string => {
      const params = tableParameters;
        
      if (!params || params.length === 0) {
        return '<p>No parameters available</p>';
      }

      // 🎯 Markdown을 HTML로 변환하는 헬퍼 함수
      const markdownToHtml = (text: string): string => {
        return text
          .replace(/\*\*(.+?)\*\*/g, '<strong>$1</strong>')  // **bold** → <strong>bold</strong>
          .replace(/\*(.+?)\*/g, '<em>$1</em>')              // *italic* → <em>italic</em>
          .replace(/`(.+?)`/g, '<code>$1</code>')            // `code` → <code>code</code>
          .replace(/\n/g, '<br>');                           // 줄바꿈 → <br>
      };

      let tableHTML = `
<div class="table-wrap">
<table style="border-collapse: collapse; width: 100%;" border="1">
<colgroup> 
  <col style="width: 6.00%;"> 
  <col style="width: 6.00%;"> 
  <col style="width: 44.00%;"> 
  <col style="width: 14.00%;"> 
  <col style="width: 10.00%;"> 
  <col style="width: 10.00%;"> 
  <col style="width: 10.00%;"> 
</colgroup>
<tbody>
<tr>
<th style="padding: 15px 5px 15px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;">No.</th>
<th style="padding: 15px 5px 15px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;" colspan="2">Description</th>
<th style="padding: 15px 5px 15px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;">Key</th>
<th style="padding: 15px 5px 15px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;">Value Type</th>
<th style="padding: 15px 5px 15px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;">Default</th>
<th style="padding: 15px 5px 15px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;">Required</th>
</tr>`;

      params.forEach((param: any) => {
        if (param.section) {
          tableHTML += `
<tr>
<td style="background-color: #e6fcff; padding: 10px 5px 10px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;" colspan="7">
<p><span style="color: #4c9aff;">${param.section}</span></p>
</td>
</tr>`;
          // 🔥 섹션 헤더만 있는 행이면 실제 필드 행 추가 안 함
          if (!param.name || !param.type) {
            return;
          }
        }

        const rowspan = param.children ? param.children.length + 1 : 1;
        tableHTML += `
<tr>
<td style="padding: 10px 5px 10px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;" ${param.children ? `rowspan="${rowspan}"` : ''}>
<p style="text-align: center;">${param.no}</p>
</td>
<td style="padding: 10px 5px 10px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;" colspan="2">
<p>${markdownToHtml(param.description || param.name)}</p>
${param.options ? param.options.map((opt: string) => `<p>${markdownToHtml(opt)}</p>`).join('') : ''}
</td>
<td style="padding: 10px 5px 10px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;">
<p style="text-align: center;">"${param.name}"</p>
</td>
<td style="padding: 10px 5px 10px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;">
<p style="text-align: center;">${param.type}</p>
</td>
<td style="padding: 10px 5px 10px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;">
<p style="text-align: center;">${param.default || '-'}</p>
</td>
<td style="padding: 10px 5px 10px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;">
<p style="text-align: center;">${param.required}</p>
</td>
</tr>`;

        if (param.children) {
          param.children.forEach((child: any) => {
            // 🔥 자식 요소 중 섹션 헤더 처리
            if (child.section) {
              tableHTML += `
<tr>
<td style="background-color: #e3f2fd; padding: 10px 5px 10px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;" colspan="7">
<p><span style="color: #1976d2;">${child.section}</span></p>
</td>
</tr>`;
              return; // 🔥 섹션 헤더만 렌더링하고 종료
            }
            
            // 🔥 빈 행 스킵 (name이나 type이 없으면)
            if (!child.name || !child.type || child.name === '""') {
              return;
            }
            
            tableHTML += `
<tr>
<td style="padding: 10px 5px 10px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;">
<p style="text-align: center;">${child.no}</p>
</td>
<td style="padding: 10px 5px 10px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;">
<p>${markdownToHtml(child.description || child.name)}</p>
${child.options ? child.options.map((opt: string) => `<p>${markdownToHtml(opt)}</p>`).join('') : ''}
</td>
<td style="padding: 10px 5px 10px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;">
<p style="text-align: center;">"${child.name}"</p>
</td>
<td style="padding: 10px 5px 10px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;">
<p style="text-align: center;">${child.type}</p>
</td>
<td style="padding: 10px 5px 10px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;">
<p style="text-align: center;">${child.default || '-'}</p>
</td>
<td style="padding: 10px 5px 10px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;">
<p style="text-align: center;">${child.required}</p>
</td>
</tr>`;
          });
        }
      });

      tableHTML += `
</tbody>
</table>
</div>`;
      
      return tableHTML;
    };

    // 🎯 JSON으로 저장 (HTML이 아닌 실제 JSON 문자열)
    const newManualData: ManualData = {
      title: spec.title || endpoint.name,
      category: endpoint.method,
      inputUri: endpoint.path,
      activeMethods: endpoint.method,
      jsonSchema: manualData?.jsonSchema || JSON.stringify(spec.jsonSchema, null, 2),
      jsonSchemaOriginal: JSON.stringify(spec.jsonSchema, null, 2),
      jsonSchemaEnhanced: spec.jsonSchemaEnhanced ? JSON.stringify(spec.jsonSchemaEnhanced, null, 2) : undefined,
      examples: manualData?.examples || [],  // deprecated
      requestExamples: manualData?.requestExamples || [],
      responseExamples: manualData?.responseExamples || [],
      specifications: generateSpecificationsTable(),
    };

    setManualData(newManualData);
    toast.success('✅ Table sent to Manual tab!');
  };

  return (
    <ResizablePanelGroup direction="horizontal" className="h-full w-full">
      {/* Left Pane - JSON Schema Editor */}
      <ResizablePanel defaultSize={50} minSize={30}>
        <div className="h-full flex flex-col bg-zinc-950 overflow-hidden">
          {/* 🔥 경고: Fallback 스키마 사용 중 */}
          {isUsingFallback && (
            <div className="bg-yellow-900/30 border-b border-yellow-700/50 px-4 py-2 flex items-center gap-2">
              <AlertCircle className="w-4 h-4 text-yellow-400" />
              <span className="text-xs text-yellow-200">
                Using default schema template. Load a version to see saved schema.
              </span>
            </div>
          )}
          
          <div className="p-4 border-b border-zinc-800 bg-zinc-900 flex-shrink-0">
            <div className="flex items-center justify-between mb-2">
              <h3 className="text-sm">JSON Schema</h3>
              
              {/* Schema Toggle */}
              <div className="flex items-center gap-1 bg-zinc-800 rounded-lg p-1">
                <button
                  onClick={() => setSchemaView('original')}
                  className={`px-3 py-1 text-xs rounded transition-colors ${
                    schemaView === 'original'
                      ? 'bg-blue-600 text-white'
                      : 'text-zinc-400 hover:text-zinc-200'
                  }`}
                >
                  Original
                </button>
                <button
                  onClick={() => setSchemaView('enhanced')}
                  className={`px-3 py-1 text-xs rounded transition-colors ${
                    schemaView === 'enhanced'
                      ? 'bg-green-600 text-white'
                      : 'text-zinc-400 hover:text-zinc-200'
                  }`}
                >
                  Enhanced
                </button>
              </div>
            </div>
            <p className="text-xs text-zinc-500">
              {schemaView === 'original' 
                ? 'Original schema definition (editable)' 
                : 'Enhanced with conditions, required, and optional fields (editable)'}
            </p>
          </div>

          {/* Monaco Editor - Full Height */}
          <div className="flex-1 relative">
            <CodeEditor
              value={editableSchema}
              onChange={(value) => handleSchemaChange(value || '')}
              language="json"
              minimap={true}
            />
            
            {/* Modified Indicator */}
            {isSchemaModified && (
              <div className="absolute top-4 right-4 px-2 py-1 bg-orange-600/20 border border-orange-600/50 rounded text-xs text-orange-400 z-10">
                Modified
              </div>
            )}
          </div>
          
          {/* Footer with Save Button */}
          <div className="border-t border-zinc-800 bg-zinc-900 p-4 flex items-center justify-between flex-shrink-0">
            <div className="flex items-center gap-2 text-xs text-zinc-500">
              {isSchemaModified ? (
                <>
                  <div className="w-2 h-2 bg-orange-500 rounded-full animate-pulse"></div>
                  <span>Unsaved changes</span>
                </>
              ) : (
                <>
                  <div className="w-2 h-2 bg-green-500 rounded-full"></div>
                  <span>All changes saved</span>
                </>
              )}
            </div>
            
            <div className="flex items-center gap-2">
              <Button
                onClick={() => {
                  // Reset to original
                  if (schemaView === 'original') {
                    setEditableSchema(JSON.stringify(spec.jsonSchema, null, 2));
                  } else {
                    setEditableSchema(
                      spec.jsonSchemaEnhanced 
                        ? JSON.stringify(spec.jsonSchemaEnhanced, null, 2)
                        : '// Enhanced schema not available yet\n// Add enhanced schema to apiSpecs data'
                    );
                  }
                  setIsSchemaModified(false);
                }}
                variant="outline"
                size="sm"
                disabled={!isSchemaModified}
                className="h-8 text-xs"
              >
                Reset
              </Button>
              
              <Button
                onClick={async () => {
                  try {
                    // Validate JSON
                    const parsedSchema = JSON.parse(editableSchema);
                    
                    if (!currentVersionId) {
                      alert(`⚠️ No version selected!\n\n📝 To save:\n1. Go to Version tab\n2. Create or load a version\n3. Come back and save again`);
                      return;
                    }
                    
                    // 🎯 현재 버전 가져오기
                    const currentVersion = getCurrentVersion();
                    if (!currentVersion) {
                      alert('❌ Failed to get current version!');
                      return;
                    }
                    
                    const currentSpecData = currentVersion.specData || {
                      jsonSchema: {},
                      jsonSchemaOriginal: null,
                      jsonSchemaEnhanced: null,
                      specifications: '',
                    };
                    
                    // 🔥 업데이트할 specData 준비 (객체 형태)
                    // 🎯 Original과 Enhanced를 각각 별도로 관리
                    const updatedSpecData = {
                      // 🔥 jsonSchema는 fallback.jsonSchema 또는 Original 유지
                      jsonSchema: currentSpecData.jsonSchema || fallbackSpec.jsonSchema || {},
                      
                      // 🔥 Original 스키마: Original 탭에서 저장 시에만 업데이트
                      // 처음 저장 시 자동으로 Original로 설정
                      jsonSchemaOriginal: schemaView === 'original'
                        ? parsedSchema
                        : (currentSpecData.jsonSchemaOriginal || currentSpecData.jsonSchema || fallbackSpec.jsonSchema),
                      
                      // 🔥 Enhanced 스키마: Enhanced 탭에서 저장 시에만 업데이트
                      jsonSchemaEnhanced: schemaView === 'enhanced'
                        ? parsedSchema
                        : currentSpecData.jsonSchemaEnhanced,
                      
                      specifications: currentSpecData.specifications || '',
                    };
                    
                    console.log('💾 Saving Schema:', {
                      schemaView,
                      hasOriginal: !!updatedSpecData.jsonSchemaOriginal,
                      hasEnhanced: !!updatedSpecData.jsonSchemaEnhanced,
                      originalKeys: updatedSpecData.jsonSchemaOriginal ? Object.keys(updatedSpecData.jsonSchemaOriginal).length : 0,
                      enhancedKeys: updatedSpecData.jsonSchemaEnhanced ? Object.keys(updatedSpecData.jsonSchemaEnhanced).length : 0
                    });
                    
                    // 🔥 1. Zustand store 업데이트 (메모리)
                    updateSpecData(updatedSpecData);
                    
                    // 🔥 2. 백엔드 API 호출 (영구 저장) - 문자열로 변환
                    const response = await apiClient.updateVersion(currentVersionId, {
                      specData: {
                        jsonSchema: JSON.stringify(updatedSpecData.jsonSchema),
                        jsonSchemaOriginal: updatedSpecData.jsonSchemaOriginal 
                          ? JSON.stringify(updatedSpecData.jsonSchemaOriginal) 
                          : undefined,
                        jsonSchemaEnhanced: updatedSpecData.jsonSchemaEnhanced 
                          ? JSON.stringify(updatedSpecData.jsonSchemaEnhanced) 
                          : undefined,
                        specifications: updatedSpecData.specifications,
                      },
                    });
                    
                    if (!response.data) {
                      throw new Error('Backend save failed');
                    }
                    
                    // 🔥 3. 저장된 스키마 state 업데이트 (테이블 리렌더링 트리거)
                    setSavedSchema(parsedSchema);
                    
                    // 🔥 4. 버전 목록도 다시 fetch (프론트 재시작 시 최신 데이터 로드)
                    if (endpoint?.id) {
                      await fetchVersions(endpoint.id);
                      // 버전 목록을 다시 불러온 후, 같은 버전을 다시 로드
                      await loadVersion(currentVersionId);
                    }
                    
                    setIsSchemaModified(false);
                    alert(`✅ Schema saved!\n\n💾 Database: ${currentVersion.version}\n✨ Restart safe!`);
                  } catch (error) {
                    console.error('Save failed:', error);
                    alert(`❌ Save failed!\n\n${error instanceof Error ? error.message : 'Unknown error'}`);
                  }
                }}
                size="sm"
                disabled={!isSchemaModified}
                className="h-8 text-xs bg-blue-600 hover:bg-blue-500 disabled:opacity-50 disabled:cursor-not-allowed"
              >
                <Save className="w-3 h-3 mr-2" />
                Save Schema
              </Button>
              
              <DropdownMenu>
                <DropdownMenuTrigger className="inline-flex items-center justify-center rounded-md text-sm font-medium transition-colors focus-visible:outline-none h-8 px-3 bg-green-600 hover:bg-green-500 text-white text-xs">
                  <Send className="w-3 h-3 mr-2" />
                  Send Schema
                  <ChevronDown className="w-3 h-3 ml-2" />
                </DropdownMenuTrigger>
                <DropdownMenuContent align="end" className="bg-zinc-900 border-zinc-700 w-56">
                  <DropdownMenuItem 
                    onClick={() => handleSendSchemaToManual('original')}
                    className="cursor-pointer hover:bg-zinc-800 focus:bg-zinc-800"
                  >
                    <div className="flex flex-col gap-0.5">
                      <span className="font-medium text-zinc-200">Original</span>
                      <span className="text-xs text-zinc-500">Send original schema</span>
            </div>
                  </DropdownMenuItem>
                  <DropdownMenuItem 
                    onClick={() => handleSendSchemaToManual('enhanced')}
                    className="cursor-pointer hover:bg-zinc-800 focus:bg-zinc-800"
                    disabled={!spec.jsonSchemaEnhanced}
                  >
                    <div className="flex flex-col gap-0.5">
                      <span className="font-medium text-zinc-200">Enhanced</span>
                      <span className="text-xs text-zinc-500">
                        {spec.jsonSchemaEnhanced ? 'Send enhanced schema' : 'Not available'}
                      </span>
                    </div>
                  </DropdownMenuItem>
                </DropdownMenuContent>
              </DropdownMenu>
            </div>
          </div>
        </div>
      </ResizablePanel>

      <ResizableHandle />

      {/* Right Pane - Visual Schema Grid (Table) */}
      <ResizablePanel defaultSize={50} minSize={30}>
        <div className="h-full flex flex-col bg-zinc-950 overflow-hidden">
          <div className="p-4 border-b border-zinc-800 bg-zinc-900 flex-shrink-0 flex items-center justify-between">
            <div>
              <h3 className="text-sm mb-1">Visual Schema Grid</h3>
              <p className="text-xs text-zinc-500">API specification and parameters</p>
            </div>
          </div>

          <ScrollArea className="flex-1 h-0">
            <div className="p-6 space-y-6">
              <div>
                <h2 className="text-2xl mb-2">{spec.title}</h2>
                <div className="flex items-center gap-3 mb-2">
                  <span className="px-3 py-1 bg-blue-600 text-white text-sm rounded">
                    {endpoint.method}
                  </span>
                  <code className="text-sm text-zinc-400 font-mono">{spec.uri || endpoint.path}</code>
                </div>
                {spec.methods && (
                  <div className="text-xs text-zinc-500">
                    Active Methods: {spec.methods.join(', ')}
                  </div>
                )}
              </div>

              <div className="space-y-4">
                <section>
                  <h3 className="text-lg mb-3">Description</h3>
                  <p className="text-zinc-400 text-sm leading-relaxed">{spec.description}</p>
                </section>

                <section>
                  <h3 className="text-lg mb-3">Parameters</h3>
                  <div className="border border-zinc-800 rounded-lg overflow-hidden">
                    <table className="w-full text-sm">
                      <thead className="bg-zinc-900">
                        <tr>
                          <th className="text-left p-3 border-b border-zinc-800">No.</th>
                          <th className="text-left p-3 border-b border-zinc-800">Description</th>
                          <th className="text-left p-3 border-b border-zinc-800">Key</th>
                          <th className="text-left p-3 border-b border-zinc-800">Type</th>
                          <th className="text-left p-3 border-b border-zinc-800">Default</th>
                          <th className="text-left p-3 border-b border-zinc-800">Required</th>
                        </tr>
                      </thead>
                      <tbody>
                        {displayParameters.map((param: any, idx: number) => {
                          const rows = [];
                          
                          // Add section row if exists
                          if (param.section) {
                            rows.push(
                              <tr key={`section-${param.no}-${idx}`} className="bg-cyan-950/30 border-b border-zinc-800">
                                <td colSpan={6} className="p-2 text-cyan-400 font-semibold text-xs">
                                  {param.section}
                                </td>
                              </tr>
                            );
                            // 🔥 섹션 헤더만 있는 행이면 실제 필드 행 추가 안 함
                            if (!param.name || !param.type) {
                              return rows;
                            }
                          }
                          
                          // Add main parameter row
                          rows.push(
                            <tr key={`param-${param.no}-${idx}`} className="border-b border-zinc-800">
                              <td className="p-3 text-zinc-400">{param.no}</td>
                              <td className="p-3 pl-9 relative">
                                {param.children && (
                                  <button
                                    onClick={() => toggleParam(param.no)}
                                    className="absolute left-3 top-3.5 w-4 h-4 flex items-center justify-center hover:text-blue-400 transition-colors"
                                  >
                                    {expandedParams.has(param.no) ? (
                                      <ChevronDown className="w-4 h-4" />
                                    ) : (
                                      <ChevronRight className="w-4 h-4" />
                                    )}
                                  </button>
                                )}
                                <div>
                                  {param.description && (
                                    <div 
                                      className="text-zinc-300 [&_span]:text-zinc-400 [&_strong]:text-zinc-300 [&_strong]:font-semibold"
                                      dangerouslySetInnerHTML={{ 
                                        __html: param.description
                                          .replace(/\n/g, '<br>')
                                          .replace(/• /g, '<span>• </span>')
                                          .replace(/- /g, '<span>- </span>')
                                          .replace(/\*\*([^*]+)\*\*/g, '<strong>$1</strong>')
                                      }}
                                    />
                                  )}
                                  {param.options && param.options.length > 0 && (
                                    <div className="mt-1 space-y-0.5">
                                    {param.options.map((opt: string, optIdx: number) => (
                                        <div 
                                          key={`${param.name}-opt-${optIdx}`}
                                          className="text-zinc-300 [&_span]:text-zinc-400"
                                          dangerouslySetInnerHTML={{
                                            __html: opt.replace(/• /g, '<span>• </span>')
                                          }}
                                        />
                                    ))}
                                  </div>
                                )}
                                </div>
                              </td>
                              <td className="p-3">
                                <code className="font-mono text-blue-400">"{param.name}"</code>
                              </td>
                              <td className="p-3 text-zinc-400">{param.type}</td>
                              <td className="p-3 text-zinc-500 font-mono text-xs">{param.default}</td>
                              <td className="p-3">
                                <span
                                  className={`px-2 py-0.5 text-xs rounded ${
                                    param.required === 'Required'
                                      ? 'bg-red-600/20 text-red-400'
                                      : 'bg-zinc-700/50 text-zinc-400'
                                  }`}
                                >
                                  {param.required}
                                </span>
                              </td>
                            </tr>
                          );
                          
                          // Add child rows if expanded
                          if (param.children && expandedParams.has(param.no)) {
                            param.children.forEach((child: any, childIdx: number) => {
                              // 🔥 자식 요소 중 섹션 헤더 처리
                              if (child.section) {
                                rows.push(
                                  <tr key={`child-section-${param.no}-${childIdx}`} className="bg-blue-950/30 border-b border-zinc-800">
                                    <td colSpan={6} className="p-2 text-blue-400 font-semibold text-xs pl-8">
                                      {child.section}
                                    </td>
                                  </tr>
                                );
                                return;
                              }
                              
                              rows.push(
                                <tr key={`child-${param.no}-${childIdx}`} className="border-b border-zinc-800 bg-zinc-900/50">
                                  <td className="p-3 text-zinc-500 text-center">{child.no}</td>
                                  <td className="p-3 pl-8">
                                    {child.description && (
                                      <div 
                                        className="text-zinc-300 [&_span]:text-zinc-400 [&_strong]:text-zinc-300 [&_strong]:font-semibold"
                                        dangerouslySetInnerHTML={{ 
                                          __html: child.description
                                            .replace(/\n/g, '<br>')
                                            .replace(/• /g, '<span>• </span>')
                                            .replace(/- /g, '<span>- </span>')
                                            .replace(/\*\*([^*]+)\*\*/g, '<strong>$1</strong>')
                                        }}
                                      />
                                    )}
                                    {child.options && child.options.length > 0 && (
                                      <div className="mt-1 space-y-0.5">
                                        {child.options.map((opt: string, optIdx: number) => (
                                          <div 
                                            key={`${child.name}-opt-${optIdx}`}
                                            className="text-zinc-300 [&_span]:text-zinc-400"
                                            dangerouslySetInnerHTML={{
                                              __html: opt.replace(/• /g, '<span>• </span>')
                                            }}
                                          />
                                        ))}
                                      </div>
                                    )}
                                  </td>
                                  <td className="p-3">
                                    <code className="font-mono text-amber-400">"{child.name}"</code>
                                  </td>
                                  <td className="p-3 text-zinc-400">{child.type}</td>
                                  <td className="p-3 text-zinc-500 font-mono text-xs">{child.default}</td>
                                  <td className="p-3">
                                    <span
                                      className={`px-2 py-0.5 text-xs rounded ${
                                        child.required === 'Required'
                                          ? 'bg-red-600/20 text-red-400'
                                          : 'bg-zinc-700/50 text-zinc-400'
                                      }`}
                                    >
                                      {child.required}
                                    </span>
                                  </td>
                                </tr>
                              );
                            });
                          }
                          
                          return rows;
                        })}
                      </tbody>
                    </table>
                  </div>
                </section>
              </div>
            </div>
          </ScrollArea>
          
          {/* Footer with Send Table Button */}
          <div className="border-t border-zinc-800 bg-zinc-900 p-4 flex items-center justify-end flex-shrink-0">
            <Button
              onClick={handleSendTableToManual}
              size="sm"
              className="h-8 text-xs bg-green-600 hover:bg-green-500"
            >
              <Send className="w-3 h-3 mr-2" />
              Send Table to Manual
            </Button>
          </div>
        </div>
      </ResizablePanel>
    </ResizablePanelGroup>
  );
}
