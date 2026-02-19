import { ResizableHandle, ResizablePanel, ResizablePanelGroup } from '@/components/ui/resizable';
import { useState, useEffect, useMemo } from 'react';
import { Save, AlertCircle, Sparkles, ArrowRight } from 'lucide-react';
import { convertWithDetails } from '@/lib/schema/schemaConverter';
import { Button } from '@/components/ui/button';
import { CodeEditor } from '@/components/common';
import { apiSpecs } from '@/data/apiSpecs';
import { useAppStore } from '@/store/useAppStore';
// import { apiClient } from '@/lib/api-client'; // Unused
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
import {
  collectFieldConditionInfo,
  groupFieldsByCondition
} from '@/lib/schema/conditionExtractor';
import { buildFieldDescription } from '@/lib/schema/descriptionBuilder';
import { schemaCompileCache } from '@/lib/cache/schemaCache';
import { SchemaDesigner } from '@/features/schema-designer/components/SchemaDesigner';

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
    // getCurrentVersion,
    updateSpecData,
    // fetchVersions,
    // loadVersion,
    specData,
    saveCurrentVersion,
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
  // 🔥 fallbackSpec을 useMemo로 감싸서 매 렌더링마다 새 객체 생성 방지
  const fallbackSpec = useMemo(() => {
    return apiSpecs[endpoint.id] || {
      title: endpoint.name,
      description: 'API endpoint specification',
      parameters: [],
      jsonSchema: {},
    };
  }, [endpoint.id, endpoint.name]);

  // 🎯 specData 상태 확인
  const hasSpecData = !!(specData?.jsonSchema);
  const isUsingFallback = !hasSpecData;

  // specData와 fallback을 결합 (specData 변경 시 재계산)
  const combinedSpecData = useMemo(() => {
    // 문자열인 경우 파싱, 이미 객체인 경우 그대로 사용
    const parseIfString = (value: any) => {
      if (!value) return value;
      if (typeof value === 'string') {
        try {
          return JSON.parse(value);
        } catch {
          return value;
        }
      }
      return value;
    };

    const result = {
      jsonSchema: parseIfString(specData?.jsonSchema) || parseIfString(specData?.jsonSchemaOriginal) || fallbackSpec.jsonSchema,
      jsonSchemaOriginal: parseIfString(specData?.jsonSchemaOriginal) || parseIfString(specData?.jsonSchema) || fallbackSpec.jsonSchema,
      jsonSchemaEnhanced: parseIfString(specData?.jsonSchemaEnhanced) || fallbackSpec.jsonSchemaEnhanced,
      savedSchema: (specData as any)?.savedSchema,
    };

    return result;
  }, [specData, fallbackSpec]);

  // 🎯 활성 스키마 (우선순위: savedSchema > enhanced > original)
  const activeSchema = resolveActiveSchema(combinedSpecData);
  const hasEnhancedSchema = isEnhancedSchemaActive(combinedSpecData);

  // 🔥 NEW Enhanced Schema 감지: x-ui, x-transport, x-enum-by-type 등의 필드가 있는지 확인
  // ⚡ 최적화: JSON.stringify 대신 객체 직접 탐색 (10-100배 빠름)
  const isNewEnhancedSchema = useMemo(() => {
    if (!activeSchema || typeof activeSchema !== 'object') return false;

    // 재귀적으로 x-* 필드 검색
    const hasEnhancedFields = (obj: any, depth: number = 0): boolean => {
      if (depth > 10 || !obj || typeof obj !== 'object') return false;

      for (const key in obj) {
        if (key === 'x-ui' || key === 'x-transport' ||
          key === 'x-enum-by-type' || key === 'x-node-count-by-type') {
          return true;
        }
        if (typeof obj[key] === 'object' && hasEnhancedFields(obj[key], depth + 1)) {
          return true;
        }
      }
      return false;
    };

    return hasEnhancedFields(activeSchema);
  }, [activeSchema]);

  // 🔥 Schema Definition 결정 (Settings 우선, 없으면 자동 감지)
  const effectiveDefinitionType: DefinitionType = useMemo(() => {
    if (settings?.schemaDefinition === 'original') return 'original';
    if (settings?.schemaDefinition === 'enhanced') return 'enhanced';
    // Auto: 자동 감지
    return isNewEnhancedSchema ? 'enhanced' : 'original';
  }, [settings?.schemaDefinition, isNewEnhancedSchema]);

  // 🎯 Schema View Toggle: 'original' | 'enhanced' (⚠️ tableParameters보다 먼저 선언)
  // 🔥 일반 모드에서는 schemaView 고정 (탭 숨김)
  const [schemaView, setSchemaView] = useState<'original' | 'enhanced'>(() => {
    if (settings?.schemaMode === 'normal') {
      return 'original'; // 일반 모드는 항상 original
    }
    return hasEnhancedSchema ? 'enhanced' : 'original';
  });

  // 🎨 Designer Mode Toggle: 'code' | 'visual'
  const [designerMode, _setDesignerMode] = useState<'code' | 'visual'>('code');

  // 🔥 endpoint 변경 시 schemaView 재설정 (Enhanced 우선)
  useEffect(() => {
    // 일반 모드에서는 항상 original
    if (settings?.schemaMode === 'normal') {
      setSchemaView('original');
      return;
    }

    // 개선 모드에서만 자동 전환
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
  }, [endpoint.id, hasEnhancedSchema, isNewEnhancedSchema, settings?.schemaMode]);

  // 🔥 YAML Definition 로드 (schemaView 변경 시)
  useEffect(() => {
    setIsLoadingDefinition(true);

    // 🔥 일반 모드에서는 definition 하나만 로드
    // 🔥 개선 모드에서는 original 또는 enhanced 로드
    const currentSchemaType = settings?.schemaMode === 'normal'
      ? 'definition'  // 일반 모드
      : schemaView === 'original' ? 'original' : schemaType; // 개선 모드

    const key = `${psdSet}/${currentSchemaType}`;

    console.log(`🔄 Loading YAML definition: ${key} (mode: ${settings?.schemaMode || 'enhanced'})`);

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
        console.log(`🔍 tableDefinition object:`, def);
        console.log(`🔍 tableDefinition.schemaExtensions:`, (def as any)?.schemaExtensions);
        setTableDefinition(def as TableDefinition);
        setIsLoadingDefinition(false);
      })
      .catch((error) => {
        console.error('Failed to load table definition:', error);
        setIsLoadingDefinition(false);
      });
  }, [effectiveDefinitionType, psdSet, schemaType, schemaView, settings?.schemaMode]);

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

  // 🎯 Helper: Convert required status to display string
  const formatRequiredStatus = (requiredStatus: Record<string, string> | undefined): string => {
    if (!requiredStatus || !requiredStatus['*']) {
      return 'Optional';
    }

    const status = requiredStatus['*'];
    if (status === 'required') return 'Required';
    if (status === 'conditional') return 'Conditional';
    if (status === 'optional') return 'Optional';
    if (status === 'n/a') return 'N/A';

    return 'Optional'; // Default fallback
  };

  // 🔥 NEW: UI Schema Adapter로 테이블 스키마 생성
  const tableParameters = useMemo(() => {
    // 🔥 현재 schemaView에 맞는 schemaType 결정
    const currentSchemaType = schemaView === 'original' ? 'original' : schemaType;
    const key = `${psdSet}/${currentSchemaType}`;

    // 🔥 YAML 규칙이 초기화되었는지 확인
    if (!initializedSchemaTypes.has(key)) {
      return [];
    }

    // 🔥 schemaView에 따라 사용할 스키마 결정
    // Enhanced 탭: jsonSchemaEnhanced가 있으면 사용, 없으면 jsonSchemaOriginal을 Enhanced로 처리
    // Original 탭: jsonSchemaOriginal 사용
    const schemaToUse = schemaView === 'enhanced'
      ? (combinedSpecData.jsonSchemaEnhanced || combinedSpecData.jsonSchemaOriginal || combinedSpecData.jsonSchema)
      : (combinedSpecData.jsonSchemaOriginal || combinedSpecData.jsonSchema);

    // 🔥 Enhanced 스키마 구조 감지 (현재 뷰 기준)
    // schemaView === 'enhanced'이면 무조건 Enhanced 컴파일러 사용 (사용자 선택 우선)
    const isEnhancedStructure = schemaView === 'enhanced';

    // 🔥 스키마가 비어있거나 유효하지 않으면 빈 배열 반환
    if (!schemaToUse || typeof schemaToUse !== 'object' || Object.keys(schemaToUse).length === 0) {
      return [];
    }

    if (isEnhancedStructure) {
      // New Enhanced Schema: 새 컴파일러로 섹션 생성
      try {
        const sections = compileEnhancedSchema(schemaToUse as EnhancedSchema, psdSet, currentSchemaType);

        // Convert sections to table parameters format
        const params: any[] = [];
        let rowNumber = 1;

        for (const section of sections) {
          // 🔥 조건 추출 및 그룹핑 (모듈화된 함수 사용)
          const fieldInfoMap = collectFieldConditionInfo(
            section.fields,
            tableDefinition?.schemaExtensions?.conditional || []
          );

          const { fieldGroups, noConditionFields } = groupFieldsByCondition(
            section.fields,
            fieldInfoMap
          );

          // 🔥 조건 없는 필드들이 있으면 일반 섹션 헤더 추가
          if (noConditionFields.length > 0) {
            params.push({
              no: '',
              section: section.name,
              name: '',
              type: '',
              default: '',
              required: '',
              description: '',
            });
          }

          // 🔥 조건 없는 필드들 렌더링
          for (const { field } of noConditionFields) {
            const param: any = {
              no: rowNumber++,
              name: field.key,
              type: field.type === 'array' ? `Array[${field.items?.type || 'any'}]` : field.type,
              default: field.default !== undefined ? String(field.default) : '-',
              description: field.ui?.label || field.description || field.key,
              required: formatRequiredStatus(field.required),
            };

            // 중첩 필드 처리 - 조건별 그룹화 지원
            if (field.children && field.children.length > 0) {
              // 🔥 3-depth 필드들을 조건별로 그룹화
              const childrenToProcess = field.children.filter((c: any) => c.type !== 'section-header');
              
              const childFieldInfoMap = collectFieldConditionInfo(
                childrenToProcess,
                tableDefinition?.schemaExtensions?.conditional || []
              );

              const { fieldGroups: childGroups, noConditionFields: childrenWithoutCondition } = groupFieldsByCondition(
                childrenToProcess,
                childFieldInfoMap
              );

              param.children = [];
              let childNo = 1;

              // 🔥 조건 없는 children 먼저 렌더링
              for (const { field: child } of childrenWithoutCondition) {
                const currentNo = childNo++;
                const mappedChild: any = {
                  no: `${rowNumber - 1}.${currentNo}`,
                  name: child.key.split('.').pop() || child.key,
                  type: child.type === 'array' ? `Array[${child.items?.type || 'any'}]` : child.type,
                  default: child.default !== undefined ? String(child.default) : '-',
                  description: buildFieldDescription(child, tableDefinition),
                  required: child['x-required-when'] ? 'Conditional' :
                    child['x-optional-when'] ? 'Optional' :
                      formatRequiredStatus(child.required),
                };

                // 🔥 3-depth: Grandchildren mapping
                if (child.children && child.children.length > 0) {
                  let grandchildNo = 1;
                  mappedChild.children = child.children.map((grandchild: any) => ({
                    no: `${rowNumber - 1}.${currentNo}.${grandchildNo++}`,
                    name: grandchild.key.split('.').pop() || grandchild.key,
                    type: grandchild.type === 'array' ? `Array[${grandchild.items?.type || 'any'}]` : grandchild.type,
                    default: grandchild.default !== undefined ? String(grandchild.default) : '-',
                    description: buildFieldDescription(grandchild, tableDefinition),
                    required: grandchild['x-required-when'] ? 'Conditional' :
                      grandchild['x-optional-when'] ? 'Optional' :
                        formatRequiredStatus(grandchild.required),
                  }));
                }

                param.children.push(mappedChild);
              }

              // 🔥 조건별 children 렌더링 - section-header 추가
              for (const [conditionKey, childrenWithCondition] of childGroups) {
                // 조건 정보 가져오기
                const { conditionInfo } = childrenWithCondition[0];
                const isRequired = conditionInfo?.type === 'x-required-when';
                
                // 조건 키에서 이름과 값 추출
                const parts = conditionKey.split(':');
                const conditionName = parts[0];
                const conditionValue = parts.slice(1).join(':');
                
                const conditionText = isRequired
                  ? `Required (When "${conditionName}" is ${conditionValue})`
                  : `Optional (When "${conditionName}" is ${conditionValue})`;

                // section-header 추가
                param.children.push({
                  no: '', name: '', type: 'section-header',
                  section: conditionText,
                  default: '', description: '', required: '',
                });

                // 조건에 맞는 children 추가
                for (const { field: child } of childrenWithCondition) {
                  const currentNo = childNo++;
                  const mappedChild: any = {
                    no: `${rowNumber - 1}.${currentNo}`,
                    name: child.key.split('.').pop() || child.key,
                    type: child.type === 'array' ? `Array[${child.items?.type || 'any'}]` : child.type,
                    default: child.default !== undefined ? String(child.default) : '-',
                    description: buildFieldDescription(child, tableDefinition),
                    required: child['x-required-when'] ? 'Conditional' :
                      child['x-optional-when'] ? 'Optional' :
                        formatRequiredStatus(child.required),
                  };

                  // 🔥 3-depth: Grandchildren mapping
                  if (child.children && child.children.length > 0) {
                    let grandchildNo = 1;
                    mappedChild.children = child.children.map((grandchild: any) => ({
                      no: `${rowNumber - 1}.${currentNo}.${grandchildNo++}`,
                      name: grandchild.key.split('.').pop() || grandchild.key,
                      type: grandchild.type === 'array' ? `Array[${grandchild.items?.type || 'any'}]` : grandchild.type,
                      default: grandchild.default !== undefined ? String(grandchild.default) : '-',
                      description: buildFieldDescription(grandchild, tableDefinition),
                      required: grandchild['x-required-when'] ? 'Conditional' :
                        grandchild['x-optional-when'] ? 'Optional' :
                          formatRequiredStatus(grandchild.required),
                    }));
                  }

                  param.children.push(mappedChild);
                }
              }
            }

            // 🔥 Description 빌드 (모듈화된 함수 사용)
            param.description = buildFieldDescription(field, tableDefinition);

            // 🔥 Required 상태 재계산 (TYPE별 다른 상태가 있는 경우)
            const requiredStatuses = Object.values(field.required);
            const hasRequired = requiredStatuses.some(s => s === 'required');
            const hasOptional = requiredStatuses.some(s => s === 'optional');
            const hasConditional = requiredStatuses.some(s => s === 'conditional');

            // 조건부 required 또는 TYPE별로 다른 경우 Conditional로 표시
            if (hasConditional || (hasRequired && hasOptional)) {
              param.required = 'Conditional';
            } else if (hasRequired) {
              param.required = 'Required';
            }

            params.push(param);
          }

          // 🔥 조건별 그룹 렌더링 (조건 헤더 + 필드들)
          for (const [conditionKey, fieldsWithCondition] of fieldGroups) {
            // 조건 정보 가져오기
            const conditionInfo = fieldsWithCondition[0].conditionInfo;
            const isRequired = conditionInfo?.type === 'x-required-when';
            
            // 조건 헤더 추가
            const conditionText = isRequired
              ? `Required (When ${conditionInfo?.conditionText || conditionKey})`
              : `Optional (When ${conditionInfo?.conditionText || conditionKey})`;
            
            params.push({
              no: '',
              section: conditionText,
              name: '',
              type: '',
              default: '',
              required: '',
              description: '',
            });

            // 해당 조건의 필드들 렌더링
            for (const { field } of fieldsWithCondition) {
              const param: any = {
                no: rowNumber++,
                name: field.key,
                type: field.type === 'array' ? `Array[${field.items?.type || 'any'}]` : field.type,
                default: field.default !== undefined ? String(field.default) : '-',
                description: field.ui?.label || field.description || field.key,
                required: formatRequiredStatus(field.required),
              };

              // 중첩 필드 처리 - section-header를 건너뛰는 번호 계산
              if (field.children && field.children.length > 0) {
                let childNo = 1;
                param.children = field.children.map((child: any) => {
                  if (child.type === 'section-header') {
                    return {
                      no: '', name: '', type: 'section-header',
                      section: child.section || child.ui?.label || '',
                      default: '', description: '', required: '',
                    };
                  }
                  const currentNo = childNo++;
                  const mappedChild: any = {
                    no: `${rowNumber - 1}.${currentNo}`,
                    name: child.key.split('.').pop() || child.key,
                    type: child.type === 'array' ? `Array[${child.items?.type || 'any'}]` : child.type,
                    default: child.default !== undefined ? String(child.default) : '-',
                    // 🔥 Use buildFieldDescription for child fields to show x-optional-when conditions
                    description: buildFieldDescription(child, tableDefinition),
                    // 🔥 x-required-when이면 Conditional, x-optional-when이면 Optional
                    required: child['x-required-when'] ? 'Conditional' :
                      child['x-optional-when'] ? 'Optional' :
                        child.required?.['*'] === 'required' ? 'Required' : 'Optional',
                  };

                  // 🔥 3-depth: Grandchildren mapping
                  if (child.children && child.children.length > 0) {
                    let grandchildNo = 1;
                    mappedChild.children = child.children.map((grandchild: any) => ({
                      no: `${rowNumber - 1}.${currentNo}.${grandchildNo++}`,
                      name: grandchild.key.split('.').pop() || grandchild.key,
                      type: grandchild.type === 'array' ? `Array[${grandchild.items?.type || 'any'}]` : grandchild.type,
                      default: grandchild.default !== undefined ? String(grandchild.default) : '-',
                      description: buildFieldDescription(grandchild, tableDefinition),
                      required: grandchild['x-required-when'] ? 'Conditional' :
                        grandchild['x-optional-when'] ? 'Optional' :
                          grandchild.required?.['*'] === 'required' ? 'Required' : 'Optional',
                    }));
                  }

                  return mappedChild;
                });
              }

              // 🔥 Description 빌드 (모듈화된 함수 사용)
              param.description = buildFieldDescription(field, tableDefinition);

              // 🔥 Required 상태 재계산 (TYPE별 다른 상태가 있는 경우)
              const requiredStatuses = Object.values(field.required);
              const hasRequired = requiredStatuses.some(s => s === 'required');
              const hasOptional = requiredStatuses.some(s => s === 'optional');
              const hasConditional = requiredStatuses.some(s => s === 'conditional');

              // 조건부 required 또는 TYPE별로 다른 경우 Conditional로 표시
              if (hasConditional || (hasRequired && hasOptional)) {
                param.required = 'Conditional';
              } else if (hasRequired) {
                param.required = 'Required';
              }

              params.push(param);
            }
          }
        }

        return params;
      } catch (error) {
        console.error('❌ Failed to compile enhanced schema for table:', error);
        return [];
      }
    }

    // Original Schema: compileSchema 결과를 직접 사용
    // schemaView가 'original'이면 Original 스키마로 컴파일
    // currentSchemaType은 위에서 이미 선언됨
    const sections = schemaView === 'original'
      ? compileSchema(schemaToUse, psdSet, currentSchemaType)
      : canonicalFields;

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
          description: field.ui?.label || field.description || field.key,
          required: formatRequiredStatus(field.required),
        });
      }
    }

    return params;
  }, [initializedSchemaTypes, canonicalFields, isNewEnhancedSchema, schemaView, combinedSpecData.jsonSchemaOriginal, combinedSpecData.jsonSchemaEnhanced, combinedSpecData.jsonSchema, psdSet, schemaType, tableDefinition]);

  const spec = useMemo(() => ({
    title: fallbackSpec.title,
    description: fallbackSpec.description,
    jsonSchema: combinedSpecData.jsonSchemaOriginal || {},
    jsonSchemaEnhanced: combinedSpecData.jsonSchemaEnhanced,
    uri: fallbackSpec?.uri,
    methods: fallbackSpec?.methods,
  }), [combinedSpecData, fallbackSpec]);

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

  // 🔥 FIX: 이전 schemaView를 추적하여 탭 전환 감지
  const [prevSchemaView, setPrevSchemaView] = useState<'original' | 'enhanced'>(schemaView);

  // Initialize editable schema
  useEffect(() => {
    // 🔥 FIX: schemaView가 변경되면 무조건 리셋 (탭 전환)
    const isTabSwitch = prevSchemaView !== schemaView;

    if (isTabSwitch) {
      setPrevSchemaView(schemaView);
      // 탭 전환 시 수정 상태도 리셋
      setIsSchemaModified(false);
    } else {
      // 🔥 같은 탭에서 수정 중인 경우에만 초기화 방지
      if (isSchemaModified && editableSchema) {
        return;
      }
    }

    // 스키마가 문자열인 경우 파싱, 객체인 경우 그대로 사용
    const getSchemaForView = () => {
      if (schemaView === 'original') {
        return spec.jsonSchema;
      } else {
        return spec.jsonSchemaEnhanced;
      }
    };

    const schemaForView = getSchemaForView();

    if (!schemaForView || (typeof schemaForView === 'object' && Object.keys(schemaForView).length === 0)) {
      setEditableSchema('{}');
    } else if (typeof schemaForView === 'string') {
      // 이미 문자열인 경우 파싱 시도
      try {
        const parsed = JSON.parse(schemaForView);
        setEditableSchema(JSON.stringify(parsed, null, 2));
      } catch {
        setEditableSchema(schemaForView);
      }
    } else {
      setEditableSchema(JSON.stringify(schemaForView, null, 2));
    }
    setIsSchemaModified(false);
  }, [schemaView, endpoint.id, spec.jsonSchema, spec.jsonSchemaEnhanced, prevSchemaView]); // spec 의존성 추가

  // Handle schema changes
  const handleSchemaChange = (value: string) => {
    setEditableSchema(value);
    setIsSchemaModified(true);
  };

  // 🎯 Save Schema - 스키마 저장 및 테이블 반영
  const handleSaveSchema = async () => {
    try {
      const parsedSchema = JSON.parse(editableSchema);

      console.log('💾 handleSaveSchema - parsedSchema:', parsedSchema);
      console.log('💾 handleSaveSchema - schemaView:', schemaView);

      // 현재 스키마 뷰에 따라 적절한 스키마 업데이트
      // 항상 JSON 문자열로 저장 (서버와 호환성을 위해)
      const updates: any = {};
      if (schemaView === 'original') {
        updates.jsonSchemaOriginal = JSON.stringify(parsedSchema);
        updates.jsonSchema = updates.jsonSchemaOriginal; // 호환성을 위해
      } else {
        updates.jsonSchemaEnhanced = JSON.stringify(parsedSchema);
      }

      console.log('💾 handleSaveSchema - updates:', updates);

      // 로컬 상태 업데이트
      updateSpecData(updates);
      setSavedSchema(parsedSchema); // 리렌더링 트리거
      setIsSchemaModified(false);

      // 저장된 스키마로 editableSchema도 업데이트 (포맷팅 유지)
      setEditableSchema(JSON.stringify(parsedSchema, null, 2));

      // 🔥 스키마 캐시 클리어 (변경사항을 즉시 반영하기 위해)
      schemaCompileCache.clear();
      console.log('🗑️ Schema compilation cache cleared');

      // 서버에 저장 (버전이 있는 경우)
      if (currentVersionId) {
        try {
          console.log('💾 handleSaveSchema - Before saveCurrentVersion, specData:', specData);
          await saveCurrentVersion();
          console.log('💾 handleSaveSchema - After saveCurrentVersion, specData:', useAppStore.getState().specData);
          toast.success('✅ Schema saved to server!\n\nThe visual table has been updated with your changes.');
        } catch (error) {
          console.error('Failed to save to server:', error);
          toast.warning('⚠️ Schema saved locally, but failed to save to server.\n\nPlease create a version first.');
        }
      } else {
        toast.success('✅ Schema saved locally!\n\nPlease create a version to save to server.');
      }

      // 🔥 강제 리렌더링을 위해 specData 재설정
      const currentSpecData = useAppStore.getState().specData;
      console.log('💾 handleSaveSchema - Force re-render, currentSpecData:', currentSpecData);
    } catch (error) {
      toast.error('❌ Invalid JSON!\n\nPlease fix the syntax errors before saving.');
    }
  };

  // 🎯 Reset Schema - 원래 스키마로 되돌리기
  const handleResetSchema = () => {
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
  };

  // 🎯 Prettify Schema - 배열을 한 줄로 정렬
  const handlePrettifySchema = () => {
    try {
      const parsed = JSON.parse(editableSchema);

      // Custom stringify with inline arrays
      const stringifyWithInlineArrays = (obj: any, indent = 0): string => {
        const spaces = '  '.repeat(indent);
        const nextSpaces = '  '.repeat(indent + 1);

        if (Array.isArray(obj)) {
          // Check if array contains only primitives (numbers, strings, booleans)
          const isPrimitiveArray = obj.every(item =>
            typeof item === 'number' ||
            typeof item === 'string' ||
            typeof item === 'boolean' ||
            item === null
          );

          if (isPrimitiveArray) {
            // Format primitive arrays inline
            const items = obj.map(item =>
              typeof item === 'string' ? `"${item}"` : JSON.stringify(item)
            ).join(', ');
            return `[${items}]`;
          } else {
            // Complex arrays stay multi-line
            const items = obj.map(item =>
              nextSpaces + stringifyWithInlineArrays(item, indent + 1)
            ).join(',\n');
            return `[\n${items}\n${spaces}]`;
          }
        } else if (obj !== null && typeof obj === 'object') {
          const keys = Object.keys(obj);
          if (keys.length === 0) return '{}';

          const items = keys.map(key => {
            const value = obj[key];
            const valueStr = stringifyWithInlineArrays(value, indent + 1);
            return `${nextSpaces}"${key}": ${valueStr}`;
          }).join(',\n');

          return `{\n${items}\n${spaces}}`;
        } else {
          return JSON.stringify(obj);
        }
      };

      const prettified = stringifyWithInlineArrays(parsed, 0);
      setEditableSchema(prettified);
      setIsSchemaModified(true);
      toast.success('✅ Schema prettified successfully');
    } catch (error) {
      toast.error('❌ Invalid JSON!\n\nCannot prettify invalid JSON.');
    }
  };

  // 🎯 Enhanced → Original 변환 (Original 탭에 저장)
  const handleConvertToOriginal = () => {
    try {
      const parsedSchema = JSON.parse(editableSchema);

      // Enhanced → Original 변환 (x-* 마커 제거)
      const result = convertWithDetails(parsedSchema);

      if (result.removedMarkersCount === 0) {
        toast.info('ℹ️ No x-* markers found. Schema is already in Original format.');
        return;
      }

      // 🔥 변환된 스키마를 Original 탭에 저장 (Enhanced 탭은 유지)
      const originalSchemaStr = JSON.stringify(result.schema, null, 2);
      updateSpecData({
        jsonSchemaOriginal: originalSchemaStr,
        jsonSchema: originalSchemaStr,  // 호환성을 위해
      });

      // 🔥 FIX: 직접 editableSchema 설정 + isSchemaModified 리셋
      // useEffect가 isSchemaModified=true일 때 스킵하므로 직접 설정 필요
      setEditableSchema(originalSchemaStr);
      setIsSchemaModified(false);

      toast.success(
        `✅ Converted to Original tab!\n\nRemoved ${result.removedMarkersCount} x-* markers:\n${result.removedMarkerKeys.slice(0, 5).join(', ')}${result.removedMarkerKeys.length > 5 ? '...' : ''}`
      );

      console.log('🔄 Converted Enhanced → Original (saved to Original tab):', {
        removedCount: result.removedMarkersCount,
        removedKeys: result.removedMarkerKeys,
      });

      // 🔥 Original 탭으로 전환
      setSchemaView('original');
    } catch (error) {
      toast.error('❌ Invalid JSON!\n\nPlease fix the syntax errors before converting.');
    }
  };




  // 🎯 Display parameters - 이제 tableParameters 직접 사용
  // const displayParameters = tableParameters; // Unused - using tableParameters directly

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

    // 🔥 스키마 타입에 따라 레이블 설정
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

    // 🔥 FIX: Ensure schemas are objects for prettify
    // DB에서 가져온 데이터가 문자열일 수 있으므로 파싱하여 객체로 변환
    const safeParse = (val: any) => {
      if (!val) return {};
      if (typeof val === 'string') {
        try {
          return JSON.parse(val);
        } catch {
          return {};
        }
      }
      return val;
    };

    const parsedOriginal = safeParse(originalSchema);
    const parsedEnhanced = enhancedSchema ? safeParse(enhancedSchema) : undefined;
    const selectedSchema = schemaType === 'original' ? parsedOriginal : parsedEnhanced;

    // 🔥 기존 ManualData를 유지하면서 업데이트 (누적 방식)
    // 🎯 JSON으로 저장 (HTML이 아닌 실제 JSON 문자열)
    const newManualData: ManualData = {
      title: spec.title || endpoint.name,
      category: endpoint.method,
      inputUri: endpoint.path,
      activeMethods: endpoint.method,
      jsonSchema: JSON.stringify(selectedSchema, null, 2),  // 🔥 선택한 스키마 (JSON)
      jsonSchemaOriginal: JSON.stringify(parsedOriginal, null, 2),  // 🔥 항상 Original (JSON)
      jsonSchemaEnhanced: parsedEnhanced ? JSON.stringify(parsedEnhanced, null, 2) : undefined,  // 🔥 항상 Enhanced (JSON)
      examples: manualData?.examples || [],  // 🔥 기존 examples 유지
      requestExamples: manualData?.requestExamples || [],  // 🔥 기존 requestExamples 유지
      responseExamples: manualData?.responseExamples || [],  // 🔥 기존 responseExamples 유지
      specifications: manualData?.specifications || '',  // 🔥 기존 specifications 유지
    };

    setManualData(newManualData);
    toast.success(`✅ ${schemaLabel} Schema sent to Manual tab!`);
  };

  // 🎯 Table을 Manual로 전송
  const handleSendTableToManual = () => {
    let specificationsHTML = '';

    // 🔥 NEW: Enhanced Schema 또는 Enhanced View인 경우 HTML 생성
    if (isNewEnhancedSchema || (schemaView === 'enhanced' && activeSchema)) {
      try {
        const htmlDocument = generateHTMLDocument(activeSchema as EnhancedSchema, psdSet, schemaType);

        // 🔥 Extract body content only (remove <!DOCTYPE>, <html>, <head>, <body> tags)
        const bodyMatch = htmlDocument.match(/<body[^>]*>([\s\S]*)<\/body>/i);
        if (bodyMatch && bodyMatch[1]) {
          specificationsHTML = bodyMatch[1].trim();
        } else {
          specificationsHTML = htmlDocument;
        }
      } catch (error) {
        console.error('❌ Failed to generate HTML:', error);
        toast.error('❌ Failed to generate schema table');
        return;
      }
    } else {
      // 🔥 YAML 기반: 테이블 정의를 사용하여 HTML 생성
      if (!tableDefinition) {
        toast.error('❌ Table definition not loaded!');
        return;
      }
      specificationsHTML = generateHTMLTable(tableParameters as TableParameter[], tableDefinition);
    }

    // 🎯 기존 데이터 유지하면서 업데이트
    const newManualData: ManualData = {
      title: spec.title || endpoint.name,
      category: endpoint.method,
      inputUri: endpoint.path,
      activeMethods: endpoint.method,
      jsonSchema: manualData?.jsonSchema || JSON.stringify(spec.jsonSchema, null, 2),
      jsonSchemaOriginal: manualData?.jsonSchemaOriginal || JSON.stringify(spec.jsonSchema, null, 2),
      jsonSchemaEnhanced: manualData?.jsonSchemaEnhanced || (spec.jsonSchemaEnhanced ? JSON.stringify(spec.jsonSchemaEnhanced, null, 2) : undefined),
      examples: manualData?.examples || [],  // deprecated
      requestExamples: manualData?.requestExamples || [],
      responseExamples: manualData?.responseExamples || [],
      specifications: specificationsHTML,  // 🔥 테이블 HTML 조각만 저장
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

          {/* 중앙: 토글 버튼 (절대 위치) - 개선 모드에서만 표시 */}
          {settings?.schemaMode !== 'normal' && (
            <div className="flex items-center gap-1 bg-zinc-800 rounded-lg p-1 w-[240px]">
              <button
                onClick={() => setSchemaView('original')}
                className={`flex-1 py-1.5 text-xs rounded transition-colors font-medium ${schemaView === 'original'
                  ? 'bg-blue-600 text-white'
                  : 'text-zinc-400 hover:text-zinc-200'
                  }`}
              >
                Original
              </button>
              <button
                onClick={() => setSchemaView('enhanced')}
                className={`flex-1 py-1.5 text-xs rounded transition-colors font-medium ${schemaView === 'enhanced'
                  ? 'bg-green-600 text-white'
                  : 'text-zinc-400 hover:text-zinc-200'
                  }`}
              >
                Enhanced
              </button>
            </div>
          )}

          {/* 일반 모드에서는 제목만 표시 */}
          {settings?.schemaMode === 'normal' && (
            <div className="text-sm font-medium text-zinc-300">
              Schema Editor
            </div>
          )}

          {/* 오른쪽: 설명 텍스트 - 개선 모드에서만 */}
          {settings?.schemaMode !== 'normal' && (
            <div className="absolute right-0">
              <span className="text-xs text-zinc-500">
                {schemaView === 'original'
                  ? '(Original schema definition)'
                  : hasEnhancedSchema || isNewEnhancedSchema
                    ? '(Enhanced with x-ui, x-transport, conditions)'
                    : '(No enhanced schema - showing original)'}
              </span>
            </div>
          )}
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

      {/* 🎨 Visual Designer Mode */}
      {designerMode === 'visual' ? (
        <div className="flex-1 overflow-hidden">
          <SchemaDesigner
            initialSchema={activeSchema}
            psdSet={psdSet}
            onSchemaChange={(schemaJson) => {
              setEditableSchema(schemaJson);
              setIsSchemaModified(true);
            }}
          />
        </div>
      ) : (
        /* Resizable Panel Group - Code Mode */
        <ResizablePanelGroup direction="horizontal" className="flex-1">
          {/* Left Pane - JSON Schema Editor */}
          <ResizablePanel defaultSize={50} minSize={30}>
            <div className="h-full flex flex-col bg-zinc-950 overflow-hidden">
              <div className="p-4 border-b border-zinc-800 bg-zinc-900 flex-shrink-0">
                <div className="flex items-center justify-between">
                  <h3 className="text-sm font-medium">JSON Schema Editor</h3>

                  {/* Prettify Button */}
                  <Button
                    onClick={handlePrettifySchema}
                    variant="outline"
                    size="sm"
                    className="h-7 px-2 text-xs"
                  >
                    <Sparkles className="w-3 h-3 mr-1" />
                    Prettify
                  </Button>
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
                    onClick={handleResetSchema}
                    variant="outline"
                    size="sm"
                    disabled={!isSchemaModified}
                    className="h-7 px-2 text-xs"
                  >
                    Reset
                  </Button>

                  <Button
                    onClick={handleSaveSchema}
                    size="sm"
                    disabled={!isSchemaModified}
                    className="h-7 px-2 text-xs bg-blue-600 hover:bg-blue-500 disabled:opacity-50 disabled:cursor-not-allowed"
                  >
                    <Save className="w-3 h-3 mr-1" />
                    Save
                  </Button>

                  {/* Enhanced → Original 변환 버튼 */}
                  {schemaView === 'enhanced' && (
                    <Button
                      onClick={handleConvertToOriginal}
                      variant="outline"
                      size="sm"
                      className="h-7 px-2 text-xs border-orange-600/50 text-orange-400 hover:bg-orange-600/20"
                    >
                      <ArrowRight className="w-3 h-3 mr-1" />
                      Convert to Original
                    </Button>
                  )}

                  <Button
                    variant="outline"
                    size="sm"
                    onClick={() => handleSendSchemaToManual('original')}
                    disabled={!spec.jsonSchema}
                    className="text-xs"
                  >
                    📤 Send Original to Manual
                  </Button>

                  {(hasEnhancedSchema || isNewEnhancedSchema) && (
                    <Button
                      variant="outline"
                      size="sm"
                      onClick={() => handleSendSchemaToManual('enhanced')}
                      className="text-xs"
                    >
                      📤 Send Enhanced to Manual
                    </Button>
                  )}
                </div>
              </div>
            </div>
          </ResizablePanel>

          {/* Resize Handle */}
          <ResizableHandle className="w-1 bg-zinc-800 hover:bg-blue-500" />

          {/* Right Pane - Visual Table Renderer */}
          <ResizablePanel defaultSize={50} minSize={30}>
            <div className="h-full flex flex-col bg-zinc-950 overflow-hidden">
              <div className="p-4 border-b border-zinc-800 bg-zinc-900 flex-shrink-0">
                <div className="flex items-center justify-between">
                  <h3 className="text-sm font-medium">Visual Schema Table</h3>
                </div>
              </div>

              {/* Table Content - Scrollable */}
              <div className="flex-1 overflow-auto p-4 space-y-6">
                {tableParameters.length > 0 && tableDefinition ? (
                  <>
                    {/* 🔥 Table 1: Keyed Object Entry (Map Key Description) */}
                    {(() => {
                      // 원본 스키마에서 wrapper 정보 확인 (schemaView에 따라)
                      const rawSchema = schemaView === 'enhanced'
                        ? combinedSpecData.jsonSchemaEnhanced
                        : combinedSpecData.jsonSchemaOriginal;
                      const wrapperKey = rawSchema?.properties?.Assign ? 'Assign'
                        : rawSchema?.properties?.Argument ? 'Argument' : null;
                      const wrapperInfo = wrapperKey ? rawSchema?.properties?.[wrapperKey] : null;

                      // 🔥 Assign은 additionalProperties/patternProperties 사용, Argument는 properties 사용
                      const isAssignStyle = wrapperInfo?.additionalProperties || wrapperInfo?.patternProperties;
                      const isArgumentStyle = wrapperKey === 'Argument' && wrapperInfo?.properties;

                      if (!wrapperKey || (!isAssignStyle && !isArgumentStyle)) return null;

                      // Argument 스타일도 'Keyed Object Entry'로 표시 (Assign과 동일)
                      const sectionTitle = 'Keyed Object Entry';
                      const descriptionText = isArgumentStyle
                        ? 'Request body wrapper object for Table API.'
                        : wrapperInfo.description || 'Map of keyed objects where each key is a string identifier.';

                      return (
                        <div>
                          <h4 className="text-sm font-semibold text-cyan-400 mb-2">{sectionTitle}</h4>
                          <p className="text-xs text-zinc-400 mb-3">
                          </p>
                          <div className="border rounded-lg overflow-hidden border-zinc-800">
                            <table className="w-full text-sm">
                              {/* 동일한 컬럼 너비 사용 (ui.yaml과 일치) */}
                              <thead className="bg-zinc-900">
                                <tr>
                                  <th className="text-left p-3 border-b border-zinc-800" style={{ width: '6%' }}>No.</th>
                                  <th className="text-left p-3 border-b border-zinc-800" style={{ width: '35%' }}>Description</th>
                                  <th className="text-left p-3 border-b border-zinc-800" style={{ width: '14%' }}>Key</th>
                                  <th className="text-left p-3 border-b border-zinc-800" style={{ width: '10%' }}>Value Type</th>
                                  <th className="text-left p-3 border-b border-zinc-800" style={{ width: '10%' }}>Default</th>
                                  <th className="text-left p-3 border-b border-zinc-800" style={{ width: '25%' }}>Required</th>
                                </tr>
                              </thead>
                              <tbody className="text-sm">
                                {/* Section Header */}
                                <tr className="bg-cyan-950/30 border-b border-zinc-800">
                                  <td colSpan={6} className="p-2 text-cyan-400 font-semibold text-xs">
                                    Root Object
                                  </td>
                                </tr>
                                {/* Data Row */}
                                <tr className="border-b border-zinc-800 hover:bg-zinc-800/30">
                                  <td className="p-3 text-zinc-400">1</td>
                                  <td className="p-3">
                                    <div className="text-zinc-300">
                                      {descriptionText}
                                    </div>
                                  </td>
                                  <td className="p-3">
                                    <code className="font-mono text-blue-400">"{wrapperKey}"</code>
                                  </td>
                                  <td className="p-3 text-zinc-400">object</td>
                                  <td className="p-3 text-zinc-500 font-mono text-xs">-</td>
                                  <td className="p-3">
                                    <span className="px-2 py-0.5 text-xs rounded bg-red-600/20 text-red-400">Required</span>
                                  </td>
                                </tr>
                              </tbody>
                            </table>
                          </div>
                        </div>
                      );
                    })()}

                    {/* 🔥 Table 2: Item (Value Object Schema) */}
                    <div>
                      <h4 className="text-sm font-semibold text-cyan-400 mb-2">Item (Value Object Schema)</h4>
                      <p className="text-xs text-zinc-400 mb-3">
                      </p>
                      <DynamicTableRenderer
                        definition={tableDefinition}
                        parameters={tableParameters}
                        expandedParams={expandedParams}
                        toggleParam={toggleParam}
                      />
                    </div>
                  </>
                ) : (
                  <div className="flex items-center justify-center h-full text-zinc-500">
                    {isLoadingDefinition ? 'Loading table definition...' : 'No schema loaded'}
                  </div>
                )}
              </div>

              {/* Footer with Send to Manual Button */}
              <div className="border-t border-zinc-800 bg-zinc-900 p-4 flex items-center justify-end flex-shrink-0">
                <Button
                  variant="outline"
                  size="sm"
                  onClick={handleSendTableToManual}
                  disabled={tableParameters.length === 0}
                  className="text-xs"
                >
                  📤 Send Table to Manual
                </Button>
              </div>
            </div>
          </ResizablePanel>
        </ResizablePanelGroup>
      )}
    </div>
  );
}
