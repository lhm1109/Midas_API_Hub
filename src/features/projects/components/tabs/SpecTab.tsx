import { ScrollArea } from '@/components/ui/scroll-area';
import { ResizableHandle, ResizablePanel, ResizablePanelGroup } from '@/components/ui/resizable';
import { useState, useEffect, useMemo } from 'react';
import { ChevronDown, Send, Save, AlertCircle } from 'lucide-react';
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
import type { ManualData, Settings } from '@/types';
import { toast } from 'sonner';
import { 
  resolveActiveSchema, 
  isEnhancedSchemaActive, 
  compileSchema,
} from '@/lib/schema';
import { 
  compileEnhancedSchema,
  type EnhancedSchema 
} from '@/lib/schema/schemaCompiler';
import { generateHTMLDocument } from '@/lib/schema/tableGenerator';
import { DynamicTableRenderer } from '@/lib/rendering/dynamicTableRenderer';
import { loadCachedDefinition, type TableDefinition, type DefinitionType } from '@/lib/rendering/definitionLoader';
import { generateHTMLTable, type TableParameter } from '@/lib/rendering/tableToHTML';
import { useEndpoints } from '@/hooks/useEndpoints';
import { getPSDForProduct } from '@/config/psdMapping';

interface SpecTabProps {
  endpoint: {
    id: string;
    name: string;
    method: string;
    path: string;
  };
  settings?: Settings;
}

export function SpecTab({ endpoint, settings }: SpecTabProps) {
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
  
  // 🔥 제품 ID로 PSD 설정 가져오기 (로컬 매핑)
  const { endpoints: products } = useEndpoints();
  const currentProduct = products.find(p => p.id === (endpoint as any).product);
  const productId = (endpoint as any).product || currentProduct?.id;
  
  // PSD 매핑 (로컬 관리)
  const { psdSet, schemaType: defaultSchemaType } = useMemo(() => {
    return getPSDForProduct(productId);
  }, [productId]);
  const schemaType = defaultSchemaType as 'original' | 'enhanced';
  
  // 🔥 YAML Definition 로드
  const [tableDefinition, setTableDefinition] = useState<TableDefinition | null>(null);
  const [isLoadingDefinition, setIsLoadingDefinition] = useState(true);
  const [initializedSchemaTypes, setInitializedSchemaTypes] = useState<Set<string>>(new Set());
  
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
  
  // 🔥 NEW Enhanced Schema 감지: x-ui, x-transport, x-enum-by-type 등의 필드가 있는지 확인
  const isNewEnhancedSchema = useMemo(() => {
    const schemaStr = JSON.stringify(activeSchema);
    return schemaStr.includes('x-ui') || 
           schemaStr.includes('x-transport') || 
           schemaStr.includes('x-enum-by-type') ||
           schemaStr.includes('x-node-count-by-type');
  }, [activeSchema]);
  
  // 🔥 Schema Definition 결정 (Settings 우선, 없으면 자동 감지)
  const effectiveDefinitionType: DefinitionType = useMemo(() => {
    if (settings?.schemaDefinition === 'original') return 'original';
    if (settings?.schemaDefinition === 'enhanced') return 'enhanced';
    // Auto: 자동 감지
    return isNewEnhancedSchema ? 'enhanced' : 'original';
  }, [settings?.schemaDefinition, isNewEnhancedSchema]);
  
  // 🎯 Schema View Toggle: 'original' | 'enhanced' (⚠️ tableParameters보다 먼저 선언)
  const [schemaView, setSchemaView] = useState<'original' | 'enhanced'>(() => {
    return hasEnhancedSchema ? 'enhanced' : 'original';
  });
  
  // 🔥 endpoint 변경 시 schemaView 재설정 (Enhanced 우선)
  useEffect(() => {
    // 1. Enhanced 스키마가 명시적으로 있으면 Enhanced 우선
    if (hasEnhancedSchema) {
      setSchemaView('enhanced');
    }
    // 2. Original 스키마에 x-ui 등이 있으면 (New Enhanced Schema) Enhanced로 전환
    else if (isNewEnhancedSchema) {
      console.log('🔄 Auto-switching to Enhanced view (x-ui detected in schema)');
      setSchemaView('enhanced');
    }
    // 3. 순수 Original 스키마만 있으면 Original
    else {
      setSchemaView('original');
    }
  }, [endpoint.id, hasEnhancedSchema, isNewEnhancedSchema]);
  
  // 🔥 YAML Definition 로드 (effectiveDefinitionType 또는 schemaView 변경 시)
  useEffect(() => {
    setIsLoadingDefinition(true);
    
    // 🔥 현재 뷰에 맞는 schemaType 결정
    const currentSchemaType = schemaView === 'original' ? 'original' : schemaType;
    const key = `${psdSet}/${currentSchemaType}`;
    
    // 🔥 Schema Logic Rules 초기화 (먼저!)
    import('@/lib/schema/schemaLogicEngine').then(({ initSchemaLogicRules }) => {
      return initSchemaLogicRules(psdSet, currentSchemaType);
    }).then(() => {
      console.log(`✅ Initialized schema logic rules for ${key}`);
      
      // 🔥 초기화 완료 추적
      setInitializedSchemaTypes(prev => new Set(prev).add(key));
      
      // 🔥 제품의 PSD 설정 사용
      return loadCachedDefinition(
        effectiveDefinitionType, 
        'table',
        undefined, // schemaSet (deprecated)
        psdSet, // psdSet (Level 1)
        currentSchemaType // schemaType (Level 2) - 현재 뷰 기준
      );
    })
      .then((def) => {
        console.log(`✅ Loaded table definition from ${key}`);
        setTableDefinition(def as TableDefinition);
        setIsLoadingDefinition(false);
      })
      .catch((error) => {
        console.error('Failed to load table definition:', error);
        setIsLoadingDefinition(false);
      });
  }, [effectiveDefinitionType, psdSet, schemaType, schemaView]);
  
  // 🔥 NEW: Schema Compiler로 정규화된 AST 생성
  const canonicalFields = useMemo(() => {
    // 🔥 현재 schemaView에 맞는 YAML 규칙이 초기화되었는지 확인
    const currentSchemaType = schemaView === 'original' ? 'original' : schemaType;
    const key = `${psdSet}/${currentSchemaType}`;
    
    if (!initializedSchemaTypes.has(key)) {
      console.log(`⏳ Waiting for ${key} to be initialized...`);
      return [];
    }
    
    if (isNewEnhancedSchema) {
      // New Enhanced Schema: 무시하고 빈 배열 반환 (새 컴파일러 사용)
      return [];
    }
    return compileSchema(activeSchema, psdSet, schemaType);
  }, [activeSchema, isNewEnhancedSchema, initializedSchemaTypes, psdSet, schemaType, schemaView]);
  
  // 🔥 NEW: UI Schema Adapter로 테이블 스키마 생성
  const tableParameters = useMemo(() => {
    // 🔥 현재 schemaView에 맞는 schemaType 결정
    const currentSchemaType = schemaView === 'original' ? 'original' : schemaType;
    const key = `${psdSet}/${currentSchemaType}`;
    
    // 🔥 YAML 규칙이 초기화되었는지 확인
    console.log('🔍 Checking initialization for key:', key);
    console.log('🔍 initializedSchemaTypes:', Array.from(initializedSchemaTypes));
    if (!initializedSchemaTypes.has(key)) {
      console.log(`⏳ Waiting for ${key} schema logic rules to be initialized...`);
      return [];
    }
    console.log('✅ Schema logic rules initialized for:', key);
    
    // 🔥 schemaView에 따라 사용할 스키마 결정
    const schemaToUse = schemaView === 'enhanced' 
      ? (combinedSpecData.jsonSchemaEnhanced || activeSchema)
      : (combinedSpecData.jsonSchemaOriginal || combinedSpecData.jsonSchema);
    
    console.log('🔍 SpecTab - schemaView:', schemaView);
    console.log('🔍 SpecTab - schemaToUse:', schemaToUse);
    console.log('🔍 SpecTab - isNewEnhancedSchema:', isNewEnhancedSchema);
    
    // 🔥 Enhanced 스키마 구조 감지 (현재 뷰 기준)
    // Original 뷰일 때는 무조건 Original 컴파일러 사용!
    const isEnhancedStructure = schemaView === 'enhanced';
    
    console.log('🔍 SpecTab - isEnhancedStructure:', isEnhancedStructure);
    console.log('🔍 SpecTab - Object.keys(schemaToUse).length:', schemaToUse ? Object.keys(schemaToUse).length : 0);
    console.log('🔍 SpecTab - psdSet:', psdSet, 'currentSchemaType:', currentSchemaType);
    
    if (isEnhancedStructure && schemaToUse && Object.keys(schemaToUse).length > 0) {
      // New Enhanced Schema: 새 컴파일러로 섹션 생성
      try {
        console.log('🔄 Calling compileEnhancedSchema with:', { psdSet, schemaType: currentSchemaType });
        const sections = compileEnhancedSchema(schemaToUse as EnhancedSchema, psdSet, currentSchemaType);
        console.log('✅ compileEnhancedSchema returned sections:', sections.length);
        
        // Convert sections to table parameters format
        const params: any[] = [];
        let rowNumber = 1;
        
        for (const section of sections) {
          // Add section header
          params.push({
            no: '',
            section: section.name,
            name: '',
            type: '',
            default: '',
            required: '',
            description: '',
          });
          
          // Add fields
          for (const field of section.fields) {
            const param: any = {
              no: rowNumber++,
              name: field.key,
              type: field.type === 'array' ? `Array[${field.items?.type || 'any'}]` : field.type,
              default: field.default !== undefined ? String(field.default) : '-',
              description: field.ui?.label || field.key,
              required: 'Optional', // Default to optional
            };
            
            // 🔥 중첩 필드 (children) 추가
            if (field.children && field.children.length > 0) {
              param.children = field.children.map((child, idx) => {
                // 🔥 oneOf 섹션 헤더 처리
                if (child.type === 'section-header') {
                  return {
                    no: '',
                    name: '',
                    type: 'section-header',
                    section: child.section || child.ui?.label || '',
                    default: '',
                    description: '',
                    required: '',
                  };
                }
                
                // 일반 중첩 필드
                return {
                  no: `${rowNumber - 1}.${idx + 1}`, // 예: 4.1, 4.2, 4.3
                  name: child.key.split('.').pop() || child.key, // UNIT.FORCE → FORCE
                  type: child.type === 'array' ? `Array[${child.items?.type || 'any'}]` : child.type,
                  default: child.default !== undefined ? String(child.default) : '-',
                  description: child.ui?.label || child.key.split('.').pop() || child.key,
                  required: child.required?.['*'] === 'required' ? 'Required' : 'Optional',
                };
              });
            }
            
            // Build description with enum/constraints
            const descParts: string[] = [];
            if (field.ui?.label) {
              descParts.push(`**${field.ui.label}**`);
            }
            
            // Standard enum
            if (field.enum && field.enum.length > 0) {
              descParts.push('**Enum Values:**');
              field.enum.forEach(val => {
                const label = field.enumLabels?.[String(val)] || val;
                descParts.push(`• ${val} - ${label}`);
              });
            }
            
            // Enum by type
            if (field.enumByType) {
              descParts.push('**Enum Values by Type:**');
              for (const [type, values] of Object.entries(field.enumByType)) {
                descParts.push(`*${type}:*`);
                values.forEach(val => {
                  const label = field.enumLabelsByType?.[type]?.[String(val)] || val;
                  descParts.push(`• ${val} - ${label}`);
                });
              }
            }
            
            // Value constraints
            if (field.valueConstraint) {
              descParts.push('**Value Constraints:**');
              for (const [type, constraint] of Object.entries(field.valueConstraint)) {
                descParts.push(`• ${type}: ${constraint}`);
              }
            }
            
            // Node count by type
            if (field.nodeCountByType) {
              descParts.push('**Node Count by Type:**');
              for (const [type, count] of Object.entries(field.nodeCountByType)) {
                const countStr = Array.isArray(count) ? count.join(' or ') : count;
                descParts.push(`• ${type}: ${countStr} nodes`);
              }
            }
            
            // Hint
            if (field.ui?.hint) {
              descParts.push(`*${field.ui.hint}*`);
            }
            
            param.description = descParts.join('\n');
            
            // Required status (check all types)
            const requiredStatuses = Object.values(field.required);
            const hasRequired = requiredStatuses.some(s => s === 'required');
            const hasOptional = requiredStatuses.some(s => s === 'optional');
            
            if (hasRequired && hasOptional) {
              // Mixed: show detail
              const grouped: Record<string, string[]> = { required: [], optional: [] };
              for (const [type, status] of Object.entries(field.required)) {
                if (status === 'required') grouped.required.push(type);
                if (status === 'optional') grouped.optional.push(type);
              }
              
              const reqParts: string[] = [];
              if (grouped.required.length > 0) {
                reqParts.push(`**Required:** ${grouped.required.join(', ')}`);
              }
              if (grouped.optional.length > 0) {
                reqParts.push(`**Optional:** ${grouped.optional.join(', ')}`);
              }
              param.required = reqParts.join('\n');
            } else if (hasRequired) {
              param.required = 'Required';
            } else {
              param.required = 'Optional';
            }
            
            params.push(param);
          }
        }
        
        console.log('✅ Enhanced path - returning params:', params.length);
        return params;
      } catch (error) {
        console.error('❌ Failed to compile enhanced schema:', error);
        console.error('Error stack:', error);
        return [];
      }
    }
    
    console.log('⚠️ Enhanced condition not met, falling through to Original path');
    
    // Original Schema: compileSchema 결과를 직접 사용
    // schemaView가 'original'이면 Original 스키마로 컴파일
    // currentSchemaType은 위에서 이미 선언됨
    const sections = schemaView === 'original'
      ? compileSchema(schemaToUse, psdSet, currentSchemaType)
      : canonicalFields;
    
    console.log('🔍 SpecTab - Original path - sections:', sections);
    console.log('🔍 SpecTab - Original path - sections.length:', sections.length);
    
    // Convert sections to table parameters format (same as Enhanced)
    const params: any[] = [];
    let rowNumber = 1;
    
    for (const section of sections) {
      // Add section header
      params.push({
        no: '',
        section: section.name,
        name: '',
        type: '',
        default: '',
        required: '',
        description: '',
      });
      
      // Add fields
      for (const field of section.fields) {
        params.push({
          no: rowNumber++,
          name: field.key,
          type: field.type === 'array' ? `Array[${field.items?.type || 'any'}]` : field.type,
          default: field.default !== undefined ? String(field.default) : '-',
          description: field.ui?.label || field.key,
          required: field.required?.['*'] === 'required' ? 'Required' : 'Optional',
        });
      }
    }
    
    console.log('🔍 SpecTab - tableSchema (params):', params);
    console.log('🔍 SpecTab - tableSchema.length:', params.length);
    
    return params;
  }, [initializedSchemaTypes, canonicalFields, isNewEnhancedSchema, schemaView, combinedSpecData.jsonSchemaOriginal, combinedSpecData.jsonSchemaEnhanced, combinedSpecData.jsonSchema, psdSet, schemaType]);
  
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
  
  // 🔥 초기 로드 시 모든 아코디언을 열린 상태로 설정
  useEffect(() => {
    if (tableParameters && tableParameters.length > 0) {
      const allParamsWithChildren = new Set<number>();
      tableParameters.forEach((param: any) => {
        if (param.children && param.children.length > 0) {
          allParamsWithChildren.add(param.no);
        }
      });
      setExpandedParams(allParamsWithChildren);
    }
  }, [tableParameters]);
  
  // 🎯 Editable Schema State
  const [editableSchema, setEditableSchema] = useState<string>('');
  const [isSchemaModified, setIsSchemaModified] = useState(false);
  
  // 🎯 저장된 스키마를 추적하는 state (리렌더링 트리거용)
  const [, setSavedSchema] = useState<any>(null);
  
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
    // 🔥 NEW: Enhanced Schema인 경우 완전한 HTML 문서 생성
    if (isNewEnhancedSchema) {
      try {
        const htmlDocument = generateHTMLDocument(activeSchema as EnhancedSchema);
        
        const newManualData: ManualData = {
          title: spec.title || endpoint.name,
          category: endpoint.method,
          inputUri: endpoint.path,
          activeMethods: endpoint.method,
          jsonSchema: manualData?.jsonSchema || JSON.stringify(spec.jsonSchema, null, 2),
          jsonSchemaOriginal: JSON.stringify(spec.jsonSchema, null, 2),
          jsonSchemaEnhanced: spec.jsonSchemaEnhanced ? JSON.stringify(spec.jsonSchemaEnhanced, null, 2) : undefined,
          examples: manualData?.examples || [],
          requestExamples: manualData?.requestExamples || [],
          responseExamples: manualData?.responseExamples || [],
          specifications: htmlDocument, // 완전한 HTML 문서
        };

        setManualData(newManualData);
        toast.success('✅ Enhanced Schema table sent to Manual tab!');
        return;
      } catch (error) {
        console.error('Failed to generate enhanced HTML:', error);
        toast.error('❌ Failed to generate enhanced schema table');
            return;
          }
        }

    // 🔥 YAML 기반: 테이블 정의를 사용하여 HTML 생성
    if (!tableDefinition) {
      toast.error('❌ Table definition not loaded!');
              return;
            }
            
    const specificationsHTML = generateHTMLTable(tableParameters as TableParameter[], tableDefinition);

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
      specifications: specificationsHTML,
    };

    setManualData(newManualData);
    toast.success('✅ Table sent to Manual tab!');
  };

  return (
    <div className="h-full w-full flex flex-col">
      {/* 🔥 중앙 토글 헤더 */}
      <div className="flex-shrink-0 bg-zinc-900 border-b border-zinc-800 px-4 py-3">
        <div className="relative flex items-center justify-center">
          {/* 왼쪽: Schema View 레이블 */}
          <div className="absolute left-0 flex items-center gap-2">
            <h3 className="text-sm text-zinc-400">Schema View:</h3>
          </div>
          
          {/* 중앙: 토글 버튼 (절대 위치) */}
          <div className="flex items-center gap-1 bg-zinc-800 rounded-lg p-1 w-[240px]">
            <button
              onClick={() => setSchemaView('original')}
              className={`flex-1 py-1.5 text-xs rounded transition-colors font-medium ${
                schemaView === 'original'
                  ? 'bg-blue-600 text-white'
                  : 'text-zinc-400 hover:text-zinc-200'
              }`}
            >
              Original
            </button>
            <button
              onClick={() => setSchemaView('enhanced')}
              className={`flex-1 py-1.5 text-xs rounded transition-colors font-medium ${
                schemaView === 'enhanced'
                  ? 'bg-green-600 text-white'
                  : 'text-zinc-400 hover:text-zinc-200'
              }`}
              disabled={!hasEnhancedSchema && !isNewEnhancedSchema}
            >
              Enhanced
            </button>
          </div>
          
          {/* 오른쪽: 설명 텍스트 */}
          <div className="absolute right-0">
            <span className="text-xs text-zinc-500">
              {schemaView === 'original' 
                ? '(Original schema definition)' 
                : '(Enhanced with x-ui, x-transport, conditions)'}
            </span>
          </div>
        </div>
      </div>
      
      {/* 🔥 경고: Fallback 스키마 사용 중 */}
      {isUsingFallback && (
        <div className="flex-shrink-0 bg-yellow-900/30 border-b border-yellow-700/50 px-4 py-2 flex items-center gap-2">
          <AlertCircle className="w-4 h-4 text-yellow-400" />
          <span className="text-xs text-yellow-200">
            Using default schema template. Load a version to see saved schema.
          </span>
        </div>
      )}
      
      {/* Resizable Panel Group */}
      <ResizablePanelGroup direction="horizontal" className="flex-1">
        {/* Left Pane - JSON Schema Editor */}
        <ResizablePanel defaultSize={50} minSize={30}>
          <div className="h-full flex flex-col bg-zinc-950 overflow-hidden">
            <div className="p-4 border-b border-zinc-800 bg-zinc-900 flex-shrink-0">
              <div className="flex items-center justify-between">
                <h3 className="text-sm font-medium">JSON Schema Editor</h3>
              </div>
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
                      toast.warning('⚠️ No version selected! Please go to Version tab and create or load a version first.');
                      return;
                    }
                    
                    // 🎯 현재 버전 가져오기
                    const currentVersion = getCurrentVersion();
                    if (!currentVersion) {
                      toast.error('❌ Failed to get current version!');
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
                    toast.success(`✅ Schema saved successfully!\n💾 Database: ${currentVersion.version}`);
                  } catch (error) {
                    console.error('Save failed:', error);
                    toast.error(`❌ Save failed: ${error instanceof Error ? error.message : 'Unknown error'}`);
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
          <div className="p-4 border-b border-zinc-800 bg-zinc-900 flex-shrink-0">
            <h3 className="text-sm font-medium">Visual Schema Table</h3>
          </div>

          <ScrollArea className="flex-1 h-0">
            <div className="p-6">
              {isLoadingDefinition ? (
                <div className="border border-zinc-800 rounded-lg p-8 text-center text-zinc-500">
                  Loading table definition...
                </div>
              ) : tableDefinition ? (
                <DynamicTableRenderer
                  definition={tableDefinition}
                  parameters={displayParameters}
                  expandedParams={expandedParams}
                  toggleParam={toggleParam}
                />
              ) : (
                <div className="border border-zinc-800 rounded-lg p-8 text-center text-red-500">
                  Failed to load table definition
                </div>
              )}
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
    </div>
  );
}
