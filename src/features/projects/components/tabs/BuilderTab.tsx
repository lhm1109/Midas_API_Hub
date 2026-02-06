import { useState, useEffect, useMemo } from 'react';
import { ScrollArea } from '@/components/ui/scroll-area';
import { Button } from '@/components/ui/button';
import { Trash2, Save, FileText, Clock, AlertCircle, Plus, Edit, RefreshCw } from 'lucide-react';
import { Input } from '@/components/ui/input';
import { Label } from '@/components/ui/label';
import { Textarea } from '@/components/ui/textarea';
import {
  Dialog,
  DialogContent,
  DialogDescription,
  DialogFooter,
  DialogHeader,
  DialogTitle,
} from '@/components/ui/dialog';
import { CodeEditor } from '@/components/common';
import { useAppStore } from '@/store/useAppStore';
import type { ApiEndpoint } from '@/types';
import { toast } from 'sonner';
import {
  resolveActiveSchema,
  isEnhancedSchemaActive,
  type UIBuilderField
} from '@/lib/schema';
import {
  extractTriggerFields,
  type EnhancedSchema
} from '@/lib/schema/builderAdapter';
import { DynamicSchemaRenderer } from '@/lib/rendering/dynamicRenderer';
import { loadCachedDefinition, loadBuilderRules, type DefinitionType } from '@/lib/rendering/definitionLoader';
import { useEndpoints } from '@/hooks/useEndpoints';
import { getPSDForProduct } from '@/config/psdMapping';
import {
  calculateFieldRuntimeStates,
  shouldIncludeInJSON,
  type FieldRuntimeStateMap
} from '@/lib/schema/fieldRuntimeState';
import { compileSchemaWithContext } from '@/lib/schema/schemaCompiler';
// 🔥 PR#1: 순수 함수 추출 - builder.logic.ts에서 import
import {
  getDefaultValue,
  buildInitialDynamicFormData,
} from './builder.logic';

interface BuilderTabProps {
  endpoint: ApiEndpoint;
  settings: {
    baseUrl: string;
    mapiKey: string;
    commonHeaders: string;
    useAssignWrapper?: boolean;
    schemaDefinition?: DefinitionType;  // 🔥 NEW: YAML 정의 타입
  };
}

export function BuilderTab({ endpoint, settings }: BuilderTabProps) {
  const {
    updateRunnerData,
    addTestCase,
    deleteTestCase,
    runnerData,
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

  const testCases = runnerData?.testCases || [];

  // 🔥 Schema Registry로 활성 스키마 결정
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

  const combinedSpecData = useMemo(() => {
    return {
      jsonSchema: parseIfString(specData?.jsonSchema) || parseIfString(specData?.jsonSchemaOriginal),
      jsonSchemaOriginal: parseIfString(specData?.jsonSchemaOriginal) || parseIfString(specData?.jsonSchema),
      jsonSchemaEnhanced: parseIfString(specData?.jsonSchemaEnhanced),
      savedSchema: (specData as any)?.savedSchema,
    };
  }, [specData]);

  const activeSchema = resolveActiveSchema(combinedSpecData);
  const hasEnhancedSchema = isEnhancedSchemaActive(combinedSpecData);

  // ⚠️ specData가 없거나 activeSchema가 비어있으면 안내 메시지 표시
  if (!specData || !activeSchema || (typeof activeSchema === 'object' && Object.keys(activeSchema).length === 0)) {
    return (
      <div className="flex-1 flex items-center justify-center bg-zinc-950 text-zinc-600">
        <div className="text-center">
          <AlertCircle className="w-12 h-12 mx-auto mb-3 opacity-50" />
          <p className="text-sm">No schema available for this endpoint</p>
          <p className="text-xs text-zinc-700 mt-2">Please add a schema in the Spec tab and save it</p>
        </div>
      </div>
    );
  }

  // 🔥 Builder Rules에서 wrapper rules 및 enhanced schema markers 로드
  const [wrapperRules, setWrapperRules] = useState<Array<{ pattern: string; wrapper: string; priority?: number }>>([]);
  const [wrapperPriorityDefault, setWrapperPriorityDefault] = useState<number>(0);  // 🔥 shared.yaml에서 로드
  const [enhancedSchemaMarkers, setEnhancedSchemaMarkers] = useState<string[]>([]);

  useEffect(() => {
    const loadBuilderConfig = async () => {
      try {
        // 🔥 제품의 PSD 설정 사용
        const builderDef = await loadBuilderRules(psdSet, schemaType);

        if (builderDef.wrapperRules) {
          console.log('✅ Loaded wrapper rules from', `${psdSet}/${schemaType}:`, builderDef.wrapperRules);
          setWrapperRules(builderDef.wrapperRules as Array<{ pattern: string; wrapper: string; priority?: number }>);
        }

        if (builderDef.enhancedSchemaMarkers) {
          console.log('✅ Loaded enhanced schema markers:', builderDef.enhancedSchemaMarkers);
          setEnhancedSchemaMarkers(builderDef.enhancedSchemaMarkers);
        }

        // 🔥 NEW: wrapperPriorityDefault 로드 (shared.yaml에서)
        if (builderDef.wrapperPriorityDefault !== undefined) {
          console.log('✅ Loaded wrapperPriorityDefault:', builderDef.wrapperPriorityDefault);
          setWrapperPriorityDefault(builderDef.wrapperPriorityDefault);
        }
      } catch (error) {
        console.error('❌ Failed to load builder config:', error);
      }
    };
    loadBuilderConfig();
  }, [psdSet, schemaType]);

  // 🔥 NEW Enhanced Schema 감지: builder.yaml의 enhancedSchemaMarkers 사용
  const isNewEnhancedSchema = useMemo(() => {
    if (enhancedSchemaMarkers.length === 0) {
      // 마커가 로드되지 않았으면 기본값 사용 (폴백)
      return false;
    }

    const schemaStr = JSON.stringify(activeSchema);
    return enhancedSchemaMarkers.some(marker => schemaStr.includes(marker));
  }, [activeSchema, enhancedSchemaMarkers]);

  // 🎯 스키마 기반 동적 상태 (기존 하드코딩 대체)
  // 초기값은 빈 객체로 설정하고, schemaFields가 준비되면 useEffect에서 초기화
  const [dynamicFormData, setDynamicFormData] = useState<any>({});

  // 🔥 Temporary state to track form values for enhanced schema
  // 초기값은 빈 객체로 설정하고, schemaFields가 준비되면 useEffect에서 초기화
  const [tempFormValuesForSchema, setTempFormValuesForSchema] = useState<Record<string, any>>({});

  // 🎯 NEW: Field Runtime States 계산 (Single Source of Truth)
  // ⚠️ useMemo로 계산 (useState 아님!) → 무한 루프 방지
  const compiledSchemaContext = useMemo(() => {
    if (!activeSchema || typeof activeSchema !== 'object' || Object.keys(activeSchema).length === 0) {
      return { sections: [], variantAxes: [] };
    }
    try {
      return compileSchemaWithContext(activeSchema, psdSet, schemaType);
    } catch (error) {
      console.error('❌ Failed to compile schema:', error);
      return { sections: [], variantAxes: [] };
    }
  }, [activeSchema, psdSet, schemaType]);

  const compiledSchemaSections = compiledSchemaContext.sections;
  const variantAxes = compiledSchemaContext.variantAxes;

  // 🎯 VariantAxes 디버그 로그
  useEffect(() => {
    if (variantAxes.length > 0) {
      console.log('🎯 VariantAxes detected:', variantAxes.map(axis => ({
        field: axis.field,
        type: axis.type,
        values: axis.values,
        labels: axis.labels
      })));
    }
  }, [variantAxes]);

  // 🔥 NEW: UI Schema Adapter로 빌더 필드 생성
  // ⚠️ 주의: compiledSchemaSections 기반으로 필드 생성 (unwrap 완료된 상태)
  const schemaFields: UIBuilderField[] = useMemo(() => {
    if (compiledSchemaSections.length === 0) {
      return [];
    }

    // 🔥 compiledSchemaSections에서 필드를 직접 추출하여 UIBuilderField로 변환
    const fields: UIBuilderField[] = [];

    for (const section of compiledSchemaSections) {
      // 섹션 헤더 추가
      if (section.name) {
        fields.push({
          name: `__section_${section.name}__`,
          type: 'object',
          description: section.name,
          required: false,
        });
      }

      // 섹션의 필드들을 UIBuilderField로 변환
      for (const field of section.fields) {
        const uiField: UIBuilderField = {
          name: field.key,
          type: field.type === 'integer' || field.type === 'number' ? field.type :
            field.enum || field.enumByType ? 'enum' :
              field.type === 'boolean' ? 'boolean' :
                field.type === 'array' ? 'array' :
                  field.type === 'object' ? 'object' : 'string',
          description: field.ui?.label || field.description || field.key,
          required: typeof field.required === 'boolean' ? field.required :
            typeof field.required === 'object' && field.required['*'] === 'required',
          default: field.default,
          enum: field.enum || (field.enumByType ? Object.values(field.enumByType)[0] as any[] : undefined),
          items: field.items,
        };

        // 중첩 필드 처리 - 🔥 section-header 타입 필터링
        if (field.children && field.children.length > 0) {
          uiField.children = field.children
            .filter(child => child.type !== 'section-header') // 🔥 섹션 헤더는 Builder 탭에서 제외
            .map(child => ({
              name: child.key,
              type: child.type === 'integer' || child.type === 'number' ? child.type :
                child.enum ? 'enum' : child.type === 'boolean' ? 'boolean' : 'string',
              description: child.ui?.label || child.description || child.key,
              required: typeof child.required === 'boolean'
                ? child.required
                : (typeof child.required === 'object' && child.required?.['*'] === 'required'),
              default: child.default,
              enum: child.enum,
              // 🔥 조건부 필드 정보 유지
              'x-required-when': (child as any)['x-required-when'],
              'x-optional-when': (child as any)['x-optional-when'],
            }));
        }

        fields.push(uiField);
      }
    }

    return fields;
  }, [compiledSchemaSections]);

  // 🔥 schemaFields가 준비되면 트리거 필드 및 기본값이 있는 필드를 자동으로 초기화
  // visibleWhen 조건에 사용되는 필드(iMETHOD, TYPE 등)를 스키마에서 자동 감지
  useEffect(() => {
    if (schemaFields.length > 0 && Object.keys(tempFormValuesForSchema).length === 0) {
      const initialValues: Record<string, any> = {};

      // 🎯 Step 1: 스키마에서 자동으로 트리거 필드 추출
      let triggerFieldNames: string[] = [];
      if (activeSchema && typeof activeSchema === 'object') {
        try {
          triggerFieldNames = extractTriggerFields(activeSchema as EnhancedSchema, psdSet, schemaType);
          console.log('🎯 Auto-detected trigger fields from schema:', triggerFieldNames);
        } catch (error) {
          console.warn('⚠️ Failed to extract trigger fields:', error);
        }
      }

      // 🎯 Step 2: 트리거 필드는 반드시 초기화 (기본값 또는 enum 첫 번째 값)
      for (const triggerFieldName of triggerFieldNames) {
        const field = schemaFields.find(f => f.name === triggerFieldName);
        if (field) {
          if (field.default !== undefined && field.default !== null) {
            initialValues[field.name] = field.default;
            console.log(`✅ Trigger field "${field.name}" initialized with default:`, field.default);
          } else if (field.type === 'enum' && field.enum && field.enum.length > 0) {
            initialValues[field.name] = field.enum[0];
            console.log(`✅ Trigger field "${field.name}" initialized with first enum:`, field.enum[0]);
          }
        }
      }

      // 🔥 Step 3 제거: Optional 필드는 초기화하지 않음
      // ⚠️ 중요: Optional/Conditional 필드는 사용자가 입력할 때 state에 추가됨
      //         초기에 key를 만들면 JSON에 불필요하게 포함됨

      if (Object.keys(initialValues).length > 0) {
        console.log('🎯 Initializing tempFormValuesForSchema (Trigger only):', initialValues);
        setTempFormValuesForSchema(initialValues);
      }
    }
  }, [schemaFields, activeSchema, psdSet, schemaType]);

  // 🔥 PR#1: getDefaultValue는 builder.logic.ts에서 import됨

  const fieldRuntimeStates: FieldRuntimeStateMap = useMemo(() => {
    if (compiledSchemaSections.length === 0) {
      return {};
    }

    try {
      // 🔥 현재 폼 값(tempFormValuesForSchema + dynamicFormData)과 함께 런타임 상태 계산
      const combinedFormValues = { ...tempFormValuesForSchema, ...dynamicFormData };
      const runtimeStates = calculateFieldRuntimeStates(compiledSchemaSections, combinedFormValues, variantAxes);

      // 🔍 디버깅: Required 필드 상태 출력
      const requiredFields = Object.entries(runtimeStates).filter(([_, state]) => state.requiredNow);
      if (requiredFields.length > 0) {
        console.log('🎯 Required fields (requiredNow=true):',
          requiredFields.map(([name, state]) => `${name} (visible: ${state.visible})`).join(', ')
        );
      }

      return runtimeStates;
    } catch (error) {
      console.error('❌ Failed to calculate field runtime states:', error);
      return {};
    }
  }, [compiledSchemaSections, tempFormValuesForSchema, dynamicFormData, variantAxes]);

  // 🎯 schemaFields가 준비되면 dynamicFormData 초기화 (Trigger + Required 필드만)
  // 🔥 PR#1: buildInitialDynamicFormData로 대체
  useEffect(() => {
    if (schemaFields.length > 0 && Object.keys(dynamicFormData).length === 0) {
      const initialData = buildInitialDynamicFormData(schemaFields, {});
      setDynamicFormData(initialData);
      console.log('🎯 Initialized dynamicFormData (Trigger + Required only):', initialData);
    }
  }, [schemaFields]);

  // 🔥 Assign 인스턴스 관리 (여러 노드를 위한 상태)
  // 🔥 PR#1: buildInitialDynamicFormData로 대체
  const [assignInstances, setAssignInstances] = useState<{ [key: string]: any }>(() => {
    const initialData = buildInitialDynamicFormData(schemaFields, {});
    return { "1": initialData };
  });

  // Assign 인스턴스 추가
  // 🔥 PR#1: buildInitialDynamicFormData로 대체
  const addAssignInstance = () => {
    const keys = Object.keys(assignInstances);
    const nextKey = String(Math.max(...keys.map(k => parseInt(k) || 0)) + 1);
    const newInstanceData = buildInitialDynamicFormData(schemaFields, {});

    setAssignInstances(prev => ({
      ...prev,
      [nextKey]: newInstanceData
    }));
    setCurrentInstanceKey(nextKey);
  };

  // Assign 인스턴스 삭제
  const removeAssignInstance = (key: string) => {
    if (Object.keys(assignInstances).length <= 1) {
      toast.error('❌ At least one instance is required');
      return;
    }

    setAssignInstances(prev => {
      const next = { ...prev };
      delete next[key];
      return next;
    });

    // 삭제된 인스턴스가 현재 선택된 것이면 다른 인스턴스로 변경
    if (currentInstanceKey === key) {
      const remaining = Object.keys(assignInstances).filter(k => k !== key);
      setCurrentInstanceKey(remaining[0]);
    }
  };

  // 현재 선택된 인스턴스
  const [currentInstanceKey, setCurrentInstanceKey] = useState<string>("1");

  // 현재 인스턴스의 데이터를 dynamicFormData에 반영
  useEffect(() => {
    if (assignInstances[currentInstanceKey]) {
      setDynamicFormData(assignInstances[currentInstanceKey]);
    }
  }, [currentInstanceKey]);

  // dynamicFormData 변경 시 현재 인스턴스에 저장
  useEffect(() => {
    setAssignInstances(prev => ({
      ...prev,
      [currentInstanceKey]: dynamicFormData
    }));
  }, [dynamicFormData, currentInstanceKey]);

  // 🔥 Enhanced Schema: dynamicFormData 변경 시 tempFormValuesForSchema 업데이트 (visibleWhen 재평가용)
  // ⚠️ 주의: 무한 루프 방지를 위해 JSON.stringify로 실제 값 변경 확인
  useEffect(() => {
    if (isNewEnhancedSchema && Object.keys(dynamicFormData).length > 0) {
      // Flatten dot notation to nested object for schema evaluation
      const flatValues: Record<string, any> = {};
      for (const [key, value] of Object.entries(dynamicFormData)) {
        if (!key.includes('.') && !key.endsWith('._enabled') && !key.startsWith('__section_')) {
          flatValues[key] = value;
        }
      }

      // 🔥 실제 값이 변경된 경우에만 업데이트
      const currentStringified = JSON.stringify(tempFormValuesForSchema);
      const newStringified = JSON.stringify(flatValues);
      if (currentStringified !== newStringified) {
        setTempFormValuesForSchema(flatValues);
      }
    }
  }, [dynamicFormData, isNewEnhancedSchema]);

  // 🎯 아코디언 상태 관리
  const [expandedObjects, setExpandedObjects] = useState<Set<string>>(new Set());

  const toggleObject = (fieldName: string) => {
    setExpandedObjects(prev => {
      const next = new Set(prev);
      if (next.has(fieldName)) {
        next.delete(fieldName);
      } else {
        next.add(fieldName);
      }
      return next;
    });
  };

  // 🔥 NEW: YAML 정의 로드
  const [builderDefinition, setBuilderDefinition] = useState<any>(null);

  useEffect(() => {
    // 🔥 Enhanced 스키마 감지: jsonSchemaEnhanced가 있거나, 마커가 있으면 Enhanced
    const hasEnhancedData = !!combinedSpecData.jsonSchemaEnhanced;
    const definitionType: DefinitionType = (hasEnhancedData || isNewEnhancedSchema) ? 'enhanced' : 'original';

    console.log(`🔄 BuilderTab: Loading YAML ${definitionType} for ${psdSet} (hasEnhancedData: ${hasEnhancedData}, isNewEnhancedSchema: ${isNewEnhancedSchema})`);

    // 🔥 제품의 PSD 설정 사용
    loadCachedDefinition(
      definitionType,
      'builder',
      undefined, // schemaSet (deprecated)
      psdSet, // psdSet (Level 1)
      definitionType // schemaType (Level 2)
    )
      .then(def => {
        console.log(`✅ BuilderTab: Loaded ${definitionType} builder definition`);
        setBuilderDefinition(def);
      })
      .catch(err => console.error('Failed to load builder definition:', err));
  }, [isNewEnhancedSchema, psdSet]);

  // 🎯 스키마 필드 목록을 안정적으로 추적하기 위한 memoized string
  const schemaFieldsKey = useMemo(() => {
    return schemaFields.map(f => f.name).join(',');
  }, [schemaFields]);

  // 🎯 스키마 변경 시 동적 폼 데이터 재초기화
  // 🔥 schemaFields가 변경되면 (조건부 필드 포함) 동적으로 업데이트
  // ⚠️ 주의: 무한 루프 방지를 위해 schemaFieldsKey로 실제 변경만 감지
  useEffect(() => {
    // 🔥 초기화되지 않은 상태이거나, 스키마가 비어있으면 스킵
    if (schemaFields.length === 0) return;

    setDynamicFormData((prev: any) => {
      const initialData: any = { ...prev };

      // 🔥 새로운 필드 추가 (Trigger + Required만), 기존 필드는 값 유지
      schemaFields.forEach(field => {
        const existingValue = prev[field.name];
        if (existingValue === undefined) {
          // ✅ Trigger 필드 (enum이 있는 필드는 VariantAxis일 가능성이 높음)
          const isTriggerField = field.enum && Array.isArray(field.enum) && field.enum.length > 0;

          // ✅ Required 필드 (boolean 또는 모든 타입에서 required)
          const isAlwaysRequired =
            field.required === true ||
            (typeof field.required === 'object' &&
              (field.required as any)['*'] === 'required');

          // 🎯 Trigger 또는 Always Required만 초기화
          if (isTriggerField || isAlwaysRequired) {
            if (field.type === 'array' && field.items) {
              initialData[field.name] = getDefaultValue(field);
            } else if (field.type === 'object' && field.children) {
              const enabledKey = `${field.name}._enabled`;
              initialData[enabledKey] = false;
              field.children.forEach(child => {
                initialData[child.name] = getDefaultValue(child);
              });
            } else {
              initialData[field.name] = getDefaultValue(field);
            }
          }
          // 🔥 Optional 필드는 key 자체를 만들지 않음 (사용자가 입력할 때 추가됨)
        }
      });

      // 🔥 스키마에 없는 필드 제거 (조건부 필드가 사라진 경우)
      const validFieldNames = new Set(schemaFields.map(f => f.name));
      schemaFields.forEach(f => {
        if (f.type === 'object' && f.children) {
          f.children.forEach(child => validFieldNames.add(child.name));
          validFieldNames.add(`${f.name}._enabled`);
        }
      });

      for (const key of Object.keys(initialData)) {
        if (key.startsWith('__section_')) continue; // 섹션 헤더는 유지
        if (!validFieldNames.has(key) && !key.includes('.')) {
          delete initialData[key];
        }
      }

      return initialData;
    });
  }, [schemaFieldsKey]);

  // 🔥 NEW: fieldRuntimeStates 변경 시 Required+Visible 필드 자동 추가
  // TYPE/iMETHOD 변경 시 새로운 필드가 required가 되면 dynamicFormData에 추가
  useEffect(() => {
    if (Object.keys(fieldRuntimeStates).length === 0) return;

    const requiredVisibleFields = Object.entries(fieldRuntimeStates)
      .filter(([_, state]) => state.requiredNow && state.visible);

    if (requiredVisibleFields.length === 0) return;

    setDynamicFormData((prev: any) => {
      const updated = { ...prev };
      let addedCount = 0;

      requiredVisibleFields.forEach(([fieldName, _state]) => {
        // 🔥 FIX: 키가 이미 존재하면 스킵 (null이든 뭐든 상관없이)
        // 이렇게 해야 무한 루프가 방지됨
        if (fieldName in updated) {
          return; // 키가 이미 있으면 스킵
        }

        // schemaFields에서 필드 정보 찾기
        const field = schemaFields.find(f => f.name === fieldName);
        if (field) {
          // Required 필드는 null로 초기화 (enum이 있으면 첫 번째 값)
          updated[fieldName] = getDefaultValue(field, true);
          addedCount++;
          console.log(`🔥 Auto-added Required field "${fieldName}":`, updated[fieldName]);
        }
      });

      if (addedCount > 0) {
        console.log(`🎯 Total ${addedCount} Required fields auto-added`);
        return updated;
      }

      return prev; // 변경 없으면 이전 상태 반환 (불필요한 리렌더링 방지)
    });
  }, [fieldRuntimeStates, schemaFields]);

  const updateDynamicField = (key: string, value: any) => {
    // 🔥 __selectedOption 변경 시, oneOf 필드 정리 및 초기화
    if (key.endsWith('.__selectedOption')) {
      const parentFieldName = key.replace('.__selectedOption', '');
      const parentField = schemaFields.find(f => f.name === parentFieldName);

      console.log('🎯 oneOf selection changed:', { key, value, parentFieldName, parentField });

      if (parentField && parentField.oneOfOptions && parentField.children) {
        const children = parentField.children; // 타입 가드

        console.log('🔍 oneOf children:', children.map((c: any) => ({
          name: c.name,
          optionIndex: c.optionIndex,
          type: c.type,
          defaultValue: getDefaultValue(c)
        })));

        setDynamicFormData((prev: any) => {
          const updated = { ...prev, [key]: value };

          // 1. 모든 oneOf 자식 필드를 삭제
          children.forEach((child: any) => {
            if (child.optionIndex !== undefined) {
              console.log('🗑️ Deleting:', child.name);
              delete updated[child.name];
            }
          });

          // 2. 선택된 옵션의 필드만 초기화
          children.forEach((child: any) => {
            if (child.optionIndex === value) {
              const defaultVal = getDefaultValue(child);
              console.log('✨ Initializing:', child.name, '=', defaultVal);
              updated[child.name] = defaultVal;
            }
          });

          console.log('📦 Updated dynamicFormData:', updated);
          return updated;
        });

        // 🔥 tempFormValuesForSchema도 업데이트 (visibleWhen 조건 재평가를 위해)
        setTempFormValuesForSchema((prev: any) => {
          const updated = { ...prev, [key]: value };

          // 1. 모든 oneOf 자식 필드를 삭제
          children.forEach((child: any) => {
            if (child.optionIndex !== undefined) {
              delete updated[child.name];
            }
          });

          // 2. 선택된 옵션의 필드만 초기화
          children.forEach((child: any) => {
            if (child.optionIndex === value) {
              updated[child.name] = getDefaultValue(child);
            }
          });

          return updated;
        });

        if (settings.useAssignWrapper && currentInstanceKey) {
          setAssignInstances(prev => {
            const currentInstance = { ...prev[currentInstanceKey], [key]: value };

            // assignInstances에서도 동일하게 처리
            children.forEach((child: any) => {
              if (child.optionIndex !== undefined) {
                delete currentInstance[child.name];
              }
            });

            children.forEach((child: any) => {
              if (child.optionIndex === value) {
                currentInstance[child.name] = getDefaultValue(child);
              }
            });

            return {
              ...prev,
              [currentInstanceKey]: currentInstance
            };
          });
        }
        return;
      }
    }

    // 일반 필드 업데이트
    setDynamicFormData((prev: any) => ({ ...prev, [key]: value }));

    // 🔥 tempFormValuesForSchema도 업데이트 (visibleWhen 조건 재평가를 위해)
    setTempFormValuesForSchema((prev: any) => ({ ...prev, [key]: value }));

    // 🔥 Assign 래퍼가 활성화되어 있으면 현재 인스턴스도 업데이트
    if (settings.useAssignWrapper && currentInstanceKey) {
      setAssignInstances(prev => ({
        ...prev,
        [currentInstanceKey]: {
          ...prev[currentInstanceKey],
          [key]: value
        }
      }));
    }
  };

  // 🎯 Test Case 저장 다이얼로그 상태
  const [showSaveDialog, setShowSaveDialog] = useState(false);
  const [caseName, setCaseName] = useState('');
  const [caseDescription, setCaseDescription] = useState('');

  // 🎯 선택된 Test Case 상태
  const [selectedTestCaseId, setSelectedTestCaseId] = useState<string | null>(null);

  // 🎯 인라인 편집 상태
  const [editingTestCaseId, setEditingTestCaseId] = useState<string | null>(null);
  const [editingTestCaseName, setEditingTestCaseName] = useState<string>('');

  // 🎯 Resizable Panel 상태 - 초기값을 화면의 35%로 설정 (빌더 컬럼 확장)
  const [rightPanelWidth, setRightPanelWidth] = useState(() => {
    if (typeof window !== 'undefined') {
      return Math.min(window.innerWidth * 0.30, 1200);  // 🔥 50% → 35%, max 800 → 500
    }
    return 400; // fallback for SSR (600 → 400)
  });

  // 🎯 Modified state tracking
  const [isModified, setIsModified] = useState(false);

  // 🎯 JSON Preview Mode
  const [jsonPreviewMode, setJsonPreviewMode] = useState<'monaco' | 'annotated'>('annotated');

  // Track initial state for comparison
  const [initialState, setInitialState] = useState<string>('');

  // 🎯 JSON 에디터용 임시 상태 (편집 중인 JSON)
  const [editableJson, setEditableJson] = useState<string>(() => {
    const rootKey = endpoint.name.toUpperCase();
    const initialData = { [rootKey]: {} };
    const rawJson = JSON.stringify(initialData, null, 2);

    // 🔥 초기값도 Assign 래퍼 적용
    if (settings.useAssignWrapper) {
      try {
        const parsed = JSON.parse(rawJson);
        let dataToWrap = parsed;

        if (parsed && typeof parsed === 'object' && rootKey in parsed) {
          dataToWrap = parsed[rootKey];
        }

        const wrapped = {
          Assign: {
            "1": dataToWrap
          }
        };

        return JSON.stringify(wrapped, null, 2);
      } catch (error) {
        console.warn('Failed to apply Assign wrapper to initial JSON:', error);
      }
    }

    return rawJson;
  });

  // 🎨 JSON 필드 메타데이터 정의 (스키마 기반)
  const getFieldMetadata = (fieldPath: string): { type: 'required' | 'optional'; color: string; label: string } => {
    // 🔥 숫자 인스턴스 키 특별 처리 (Assign.1, Assign.2 등)
    // patternProperties의 minProperties: 1 규칙에 따라 최소 1개 인스턴스는 Required
    const parts = fieldPath.split('.');
    if (parts.length === 2) {
      const wrapperKeys = [...new Set(wrapperRules.map(rule => rule.wrapper).filter(Boolean))];
      if (wrapperKeys.includes(parts[0]) && /^\d+$/.test(parts[1])) {
        // 래퍼 키 아래의 숫자 키는 Required (Entity Instance)
        return { type: 'required', color: 'text-red-400', label: 'Required' };
      }
    }

    // 🔥 중첩 경로 정규화: "Assign.1.TYPE" → "TYPE", "Assign.1.__section_Common Keys and Solid__.TYPE" → "TYPE"
    const normalizeFieldPath = (path: string): string => {
      const pathParts = path.split('.');
      // 🔥 래퍼 키, 숫자, __section__ 제거 (wrapper keys는 builder.yaml에서 동적으로 가져옴)
      const wrapperKeys = [...new Set(wrapperRules.map(rule => rule.wrapper).filter(Boolean))];
      const filtered = pathParts.filter(p =>
        !wrapperKeys.includes(p) &&
        !/^\d+$/.test(p) &&
        !p.startsWith('__section_')
      );
      return filtered.join('.');
    };

    const normalizedPath = normalizeFieldPath(fieldPath);

    // 🔥 배열 아이템 내부 필드 체크 (예: REDUCTION_DATA.0.dRANGE_MAX → REDUCTION_DATA.dRANGE_MAX)
    // 정규화된 경로가 "PARENT.CHILD" 형태이면 배열의 자식 필드일 수 있음
    const arrayChildParts = normalizedPath.split('.');

    // 🔥 DEBUG: 배열 아이템 필드 경로 확인
    if (fieldPath.includes('REDUCTION_DATA')) {
      console.log('🔍 [getFieldMetadata] Array item path check:', {
        originalPath: fieldPath,
        normalizedPath,
        arrayChildParts,
        partsLength: arrayChildParts.length
      });
    }

    if (arrayChildParts.length === 2) {
      const [parentName, childFieldName] = arrayChildParts;
      const parentField = schemaFields.find(f => f.name === parentName);

      // 배열 필드의 children에서 자식 찾기
      if (parentField && parentField.type === 'array' && parentField.children) {
        const childField = parentField.children.find((c: any) => {
          // name이 전체 경로이거나 짧은 이름일 수 있음
          const childShortName = c.name?.includes('.') ? c.name.split('.').pop() : c.name;
          return childShortName === childFieldName || c.name === childFieldName;
        });

        if (childField) {
          // 🔥 x-required-when 조건 평가
          const condition = (childField as any)['x-required-when'];
          if (condition) {
            const conditionMet = Object.entries(condition).every(([key, expectedValue]) => {
              const actualValue = tempFormValuesForSchema[key];
              if (typeof expectedValue === 'number') {
                return Number(actualValue) === expectedValue;
              }
              return actualValue === expectedValue;
            });

            return conditionMet
              ? { type: 'required' as const, color: 'text-red-400', label: 'Required' }
              : { type: 'optional' as const, color: 'text-yellow-400', label: 'Conditional' };
          }

          // 🔥 일반 required 속성 체크 (x-required-when이 없는 경우)
          // required가 boolean true이거나 { '*': 'required' } 객체 형태일 수 있음
          const isRequired = childField.required === true ||
            (typeof childField.required === 'object' && (childField.required as any)?.['*'] === 'required');

          console.log('🔍 [getFieldMetadata] childField required check:', {
            childFieldName: childField.name,
            required: childField.required,
            isRequired
          });

          if (isRequired) {
            return { type: 'required' as const, color: 'text-red-400', label: 'Required' };
          }
        }
      }
    }

    // 🎯 Runtime State 우선 확인 (Single Source of Truth)
    const runtimeState = fieldRuntimeStates[normalizedPath];
    if (runtimeState) {
      // requiredNow: 현재 조건 하에서 required인지 (조건부 required 지원)
      return runtimeState.requiredNow
        ? { type: 'required', color: 'text-red-400', label: 'Required' }
        : { type: 'optional', color: 'text-blue-400', label: 'Optional' };
    }

    // 🔥 Enhanced Schema 우선 사용 (jsonSchemaEnhanced가 있으면 그것 기반으로 판단)
    const useEnhancedForRequired = hasEnhancedSchema;

    // 🔥 Enhanced Schema 사용 시: schemaFields에서 required 정보 확인
    if (useEnhancedForRequired) {
      const field = schemaFields.find(f => f.name === normalizedPath);
      if (field && field.required !== undefined) {
        // 🔥 field.required는 boolean이 아닐 수 있음 (Record<string, string> 또는 { '*': 'conditional' })
        // boolean인 경우만 직접 사용
        if (typeof field.required === 'boolean') {
          return field.required
            ? { type: 'required', color: 'text-red-400', label: 'Required' }
            : { type: 'optional', color: 'text-blue-400', label: 'Optional' };
        }
        // 그 외는 Optional로 표시 (Runtime State에서 계산해야 함)
        return { type: 'optional', color: 'text-blue-400', label: 'Optional' };
      }

      // 중첩 필드 체크 (예: UNIT.FORCE, REDUCTION_DATA.dRANGE_MAX)
      const parts = normalizedPath.split('.');
      if (parts.length > 1) {
        const parentName = parts[0];
        const childName = parts[parts.length - 1];
        const parentField = schemaFields.find(f => f.name === parentName);
        if (parentField && parentField.children) {
          const childField = parentField.children.find(c => c.name === childName);
          if (childField) {
            // 🔥 x-required-when 조건 체크 추가
            const condition = (childField as any)['x-required-when'];
            if (condition) {
              // 조건 평가 - tempFormValuesForSchema에서 상위 폼의 값 확인
              const conditionMet = Object.entries(condition).every(([key, expectedValue]) => {
                const actualValue = tempFormValuesForSchema[key];
                // 숫자 비교: 둘 다 숫자로 변환
                if (typeof expectedValue === 'number') {
                  return Number(actualValue) === expectedValue;
                }
                return actualValue === expectedValue;
              });

              return conditionMet
                ? { type: 'required' as const, color: 'text-red-400', label: 'Required' }
                : { type: 'optional' as const, color: 'text-yellow-400', label: 'Conditional' };
            }

            if (childField.required !== undefined) {
              if (typeof childField.required === 'boolean') {
                return childField.required
                  ? { type: 'required', color: 'text-red-400', label: 'Required' }
                  : { type: 'optional', color: 'text-blue-400', label: 'Optional' };
              }
              return { type: 'optional', color: 'text-blue-400', label: 'Optional' };
            }
          }
        }
      }

      // Default for Enhanced Schema
      return { type: 'optional', color: 'text-zinc-400', label: 'Optional' };
    }

    // 🔥 Original Schema: schemaFields에서 required 정보 확인 (fallback)
    const field = schemaFields.find(f => f.name === normalizedPath);

    if (field && field.required !== undefined) {
      return field.required
        ? { type: 'required', color: 'text-red-400', label: 'Required' }
        : { type: 'optional', color: 'text-blue-400', label: 'Optional' };
    }

    // 중첩 필드 체크 (예: UNIT.FORCE)
    const normalizedParts = normalizedPath.split('.');
    if (normalizedParts.length > 1) {
      const parentName = normalizedParts[0];
      const childName = normalizedParts[normalizedParts.length - 1];
      const parentField = schemaFields.find(f => f.name === parentName);
      if (parentField && parentField.children) {
        const childField = parentField.children.find((c: any) => c.name === childName);
        if (childField && childField.required !== undefined) {
          return childField.required
            ? { type: 'required', color: 'text-red-400', label: 'Required' }
            : { type: 'optional', color: 'text-blue-400', label: 'Optional' };
        }
      }
    }

    // Default
    return { type: 'optional', color: 'text-zinc-400', label: 'Optional' };
  };

  // 🎨 커스텀 JSON 렌더러 컴포넌트
  const JSONRenderer = ({ data }: { data: any }) => {
    // 🔥 래퍼 키 목록 (builder.yaml의 wrapperRules에서 동적으로 추출)
    const WRAPPER_KEYS = [...new Set(wrapperRules.map(rule => rule.wrapper))];

    const renderValue = (value: any, key?: string, depth: number = 0): JSX.Element => {
      if (value === null) {
        return <span className="text-purple-400">null</span>;
      }

      if (typeof value === 'boolean') {
        return <span className="text-orange-400">{value.toString()}</span>;
      }

      if (typeof value === 'number') {
        return <span className="text-green-400">{value}</span>;
      }

      if (typeof value === 'string') {
        return <span className="text-emerald-400">"{value}"</span>;
      }

      if (Array.isArray(value)) {
        if (value.length === 0) {
          return <span className="text-zinc-500">[]</span>;
        }

        return (
          <>
            <span className="text-zinc-500">[</span>
            {value.map((item, idx) => {
              // 🔥 배열 아이템의 경로 생성 (예: REDUCTION_DATA.0)
              const itemPath = key ? `${key}.${idx}` : String(idx);
              return (
                <div key={idx} className="pl-4">
                  {renderValue(item, itemPath, depth + 1)}
                  {idx < value.length - 1 && <span className="text-zinc-500">,</span>}
                </div>
              );
            })}
            <div>
              <span className="text-zinc-500">]</span>
            </div>
          </>
        );
      }

      if (typeof value === 'object') {
        const entries = Object.entries(value);
        if (entries.length === 0) {
          return <span className="text-zinc-500">{'{}'}</span>;
        }

        return (
          <>
            <span className="text-zinc-500">{'{'}</span>
            {entries.map(([k, v], idx) => {
              const fieldPath = key ? `${key}.${k}` : k;
              const metadata = getFieldMetadata(fieldPath);

              // 🔥 래퍼 키(Argument, Assign)는 depth 0에서만 체크하고 배지 표시 안함
              const isWrapperKey = depth === 0 && WRAPPER_KEYS.includes(k);

              return (
                <div key={k} className="pl-4 group hover:bg-zinc-800/30 transition-colors rounded py-0.5">
                  <span className={`${isWrapperKey ? 'text-purple-400' : metadata.color} font-semibold`}>"{k}"</span>
                  <span className="text-zinc-500">: </span>
                  {/* 🏷️ Inline Badge - 래퍼 키는 배지 표시 안함 */}
                  {!isWrapperKey && (
                    <span className={`text-[9px] px-1.5 py-0.5 rounded mr-2 ${metadata.type === 'required'
                      ? 'bg-red-900/50 text-red-300 border border-red-700/50'
                      : 'bg-blue-900/50 text-blue-300 border border-blue-700/50'
                      }`}>
                      {metadata.label}
                    </span>
                  )}
                  {renderValue(v, fieldPath, depth + 1)}
                  {idx < entries.length - 1 && <span className="text-zinc-500">,</span>}
                </div>
              );
            })}
            <div>
              <span className="text-zinc-500">{'}'}</span>
            </div>
          </>
        );
      }

      return <span>{String(value)}</span>;
    };

    return <div className="font-mono text-xs leading-relaxed">{renderValue(data, undefined, 0)}</div>;
  };

  // 🎯 Resize 이벤트 핸들러
  const handleResizeStart = (e: React.MouseEvent) => {
    e.preventDefault();

    const startX = e.clientX;
    const startWidth = rightPanelWidth;

    const handleMouseMove = (moveEvent: MouseEvent) => {
      const deltaX = startX - moveEvent.clientX;
      const newWidth = startWidth + deltaX;
      setRightPanelWidth(Math.max(280, Math.min(window.innerWidth - 400, newWidth)));
    };

    const handleMouseUp = () => {
      document.removeEventListener('mousemove', handleMouseMove);
      document.removeEventListener('mouseup', handleMouseUp);
      document.body.style.cursor = '';
      document.body.style.userSelect = '';
    };

    document.addEventListener('mousemove', handleMouseMove);
    document.addEventListener('mouseup', handleMouseUp);
    document.body.style.cursor = 'ew-resize';
    document.body.style.userSelect = 'none';
  };

  // 🔄 JSON to Form 역변환: Nested JSON을 Flat Form Data로 변환
  const convertJsonToFormData = (json: string) => {
    try {
      const parsed = JSON.parse(json);
      const rootKey = endpoint.name.toUpperCase();
      let nestedData = parsed[rootKey] || parsed;

      // 🔥 Argument 래퍼가 있으면 벗겨내기
      if (nestedData && typeof nestedData === 'object' && 'Argument' in nestedData && Object.keys(nestedData).length === 1) {
        nestedData = nestedData.Argument;
        console.log('📦 Unwrapped "Argument" wrapper');
      }

      console.log('🔍 Starting conversion:', { rootKey, parsed, nestedData });

      // 🔥 1단계: 스키마 기반으로 초기 formData 생성 (모든 필드 기본값으로 초기화)
      const initialData: any = {};
      schemaFields.forEach(field => {
        if (field.type === 'array' && field.items) {
          initialData[field.name] = getDefaultValue(field);
        } else if (field.type === 'object' && field.children) {
          // Object with children: 각 자식 필드를 dot notation으로 초기화
          initialData[`${field.name}._enabled`] = false;
          field.children.forEach(child => {
            initialData[`${field.name}.${child.name}`] = getDefaultValue(child);
          });
        } else {
          initialData[field.name] = getDefaultValue(field);
        }
      });

      console.log('📋 Initial form data:', initialData);

      // 🔥 2단계: JSON 데이터를 flat structure로 변환
      const flatData: any = {};

      Object.keys(nestedData).forEach(key => {
        const value = nestedData[key];

        // 🔍 각 필드가 스키마에 있는지 확인
        const schemaField = schemaFields.find(f => f.name === key);

        if (!schemaField) {
          // 스키마에 없는 필드는 그대로 저장
          flatData[key] = value;
          console.log(`⚠️ Field not in schema: ${key}`);
          return;
        }

        // Object with children인 경우
        if (schemaField.type === 'object' && schemaField.children &&
          value !== null && typeof value === 'object' && !Array.isArray(value)) {

          // 부모 객체 활성화
          flatData[`${key}._enabled`] = true;

          // 각 자식 필드를 dot notation으로 저장
          Object.keys(value).forEach(childKey => {
            flatData[`${key}.${childKey}`] = value[childKey];
          });

          console.log(`✅ Processed object field: ${key}`, value);
        }
        // 배열인 경우
        else if (Array.isArray(value)) {
          flatData[key] = value;
          console.log(`✅ Processed array field: ${key}`, value);
        }
        // 단순 값인 경우
        else {
          flatData[key] = value;
          console.log(`✅ Processed simple field: ${key} =`, value);
        }
      });

      // 🔥 3단계: 초기 데이터와 병합
      const mergedData = { ...initialData, ...flatData };

      console.log('🔄 JSON to Form conversion complete:', {
        initialData,
        flatData,
        mergedData,
        schemaFields: schemaFields.map(f => ({ name: f.name, type: f.type, hasChildren: !!f.children }))
      });

      setDynamicFormData(mergedData);
      toast.success('✅ JSON이 폼으로 로드되었습니다');
    } catch (error) {
      console.error('❌ Failed to parse JSON:', error);
      toast.error('JSON 파싱에 실패했습니다. 올바른 형식인지 확인해주세요.');
    }
  };

  // 🧹 JSON Pruning: 현재 선택된 메소드에 맞는 데이터만 추출
  // ✅ 완전 순수 함수: 모든 입력을 인자로 받음 (외부 state 참조 금지)
  const buildCleanJSON = (
    flatData: Record<string, any>,
    runtimeStates: FieldRuntimeStateMap,
    fields: UIBuilderField[]
  ): any => {
    // 🔥 동적 스키마 필드를 중첩 구조로 변환 (_enabled 체크박스 반영)
    const convertDotNotationToNested = (data: any): any => {
      const nested: any = {};

      // 🎯 Step 1: Required + Visible 필드를 data 복사본에 추가
      const enrichedData = { ...data };
      let addedRequiredCount = 0;
      fields.forEach(field => {
        const runtimeState = runtimeStates[field.name];
        // 🔥 Rule 3: Required는 값이 없거나 빈 값이어도 key를 생성
        if (runtimeState && runtimeState.requiredNow && runtimeState.visible) {
          // ✅ key가 없거나 빈 값('')이면 null로 초기화
          if (!(field.name in enrichedData) || enrichedData[field.name] === '') {
            // enum 필드는 첫 번째 옵션, 그 외는 null
            if (field.enum && field.enum.length > 0) {
              enrichedData[field.name] = field.enum[0];
              console.log(`✅ Added Required field "${field.name}": ${field.enum[0]} (enum)`);
            } else {
              enrichedData[field.name] = null;
              console.log(`✅ Added Required field "${field.name}": null`);
            }
            addedRequiredCount++;
          }
        } else if (runtimeState && runtimeState.requiredNow) {
          console.log(`⚠️ Required field "${field.name}" NOT added (visible: ${runtimeState.visible})`);
        }
      });
      if (addedRequiredCount > 0) {
        console.log(`🎯 Total Required fields added: ${addedRequiredCount}`);
      }

      // 🔥 oneOf 필드 수집: 부모 필드명 -> 선택된 옵션 인덱스
      const oneOfSelections: Map<string, number> = new Map();
      Object.keys(enrichedData).forEach(key => {
        const match = key.match(/^(.+)\.__selectedOption$/);
        if (match) {
          const parentField = match[1];
          const selectedOption = enrichedData[key] || 0;
          oneOfSelections.set(parentField, selectedOption);
        }
      });

      // 🔥 oneOf 필드의 옵션별 필드 매핑 (schemaFields에서 추출)
      const oneOfFieldsByOption: Map<string, Map<number, Set<string>>> = new Map();
      fields.forEach(field => {
        if (field.oneOfOptions && field.children) {
          const fieldMap = new Map<number, Set<string>>();
          field.children.forEach((child: any) => {
            if (child.optionIndex !== undefined) {
              if (!fieldMap.has(child.optionIndex)) {
                fieldMap.set(child.optionIndex, new Set());
              }
              const childKeyName = child.name.split('.').pop() || child.name;
              fieldMap.get(child.optionIndex)!.add(childKeyName);
            }
          });
          oneOfFieldsByOption.set(field.name, fieldMap);
        }
      });

      // 🎯 Step 2: enrichedData를 순회하여 중첩 구조로 변환
      Object.keys(enrichedData).forEach(key => {
        // 🔥 섹션 헤더 키 제외 (UI 전용)
        if (key.startsWith('__section_') || key.includes('.__section_')) {
          return;
        }

        // 🔥 oneOf 선택 상태 키 제외 (UI 전용)
        if (key.endsWith('.__selectedOption') || key.includes('.__oneOf')) {
          return;
        }

        // _enabled 키는 제외
        if (key.endsWith('._enabled')) {
          return;
        }

        // 🔥 Field Runtime State 기반 필터링 (Single Source of Truth)
        const value = enrichedData[key];
        const runtimeState = runtimeStates[key];

        const shouldInclude = shouldIncludeInJSON(key, value, runtimeState);
        if (!shouldInclude) {
          console.log(`❌ Excluded field "${key}": value=${JSON.stringify(value)}, visible=${runtimeState?.visible}, requiredNow=${runtimeState?.requiredNow}`);
          return;
        } else if (runtimeState?.requiredNow) {
          console.log(`✅ Included Required field "${key}": value=${JSON.stringify(value)}`);
        }

        if (key.includes('.')) {
          // dot notation을 중첩 객체로 변환
          const parts = key.split('.');
          const parentKey = parts[0];
          const childKey = parts[parts.length - 1];

          // 🔥 부모 객체가 체크되어 있지 않으면 스킵
          if (enrichedData[`${parentKey}._enabled`] === false) {
            return;
          }

          // 🔥 oneOf 필드인 경우, 선택되지 않은 옵션의 필드는 제외
          if (oneOfSelections.has(parentKey) && oneOfFieldsByOption.has(parentKey)) {
            const selectedOption = oneOfSelections.get(parentKey)!;
            const fieldMap = oneOfFieldsByOption.get(parentKey)!;
            const selectedFields = fieldMap.get(selectedOption);

            // 🔥 selectedFields가 있으면 선택된 필드만 포함, 없으면 모두 포함 (oneOf가 아닌 경우)
            if (selectedFields) {
              if (!selectedFields.has(childKey)) {
                return; // 선택되지 않은 옵션의 필드는 스킵
              }
            }
            // selectedFields가 없으면 oneOf가 아니므로 그대로 포함
          }

          let current = nested;

          for (let i = 0; i < parts.length - 1; i++) {
            if (!current[parts[i]]) {
              current[parts[i]] = {};
            }
            current = current[parts[i]];
          }

          current[parts[parts.length - 1]] = value;
        } else {
          // dot notation이 아닌 필드는 그대로 추가
          nested[key] = value;
        }
      });

      return nested;
    };

    // 🔥 배열 아이템 내부의 조건부 필드 필터링
    const filterConditionalArrayFields = (obj: any): any => {
      if (obj === null || obj === undefined) return obj;

      if (Array.isArray(obj)) {
        return obj.map(item => filterConditionalArrayFields(item));
      }

      if (typeof obj === 'object') {
        const filtered: any = {};

        for (const [key, value] of Object.entries(obj)) {
          // 배열 필드인지 확인
          const schemaField = fields.find(f => f.name === key);

          if (schemaField && schemaField.type === 'array' && schemaField.children && Array.isArray(value)) {
            // 🔥 DEBUG: 배열 필드 + children 정보 출력
            console.log('🔥 [filterConditionalArrayFields] Found array field:', key, {
              childrenCount: schemaField.children.length,
              childrenNames: schemaField.children.map((c: any) => c.name),
              childrenConditions: schemaField.children.map((c: any) => ({
                name: c.name,
                'x-required-when': (c as any)['x-required-when']
              }))
            });

            // 배열 아이템에서 조건부 필드 필터링
            filtered[key] = (value as any[]).map(item => {
              if (typeof item !== 'object' || item === null) return item;

              const filteredItem: any = {};
              for (const [itemKey, itemValue] of Object.entries(item)) {
                // 자식 필드의 x-required-when 조건 체크
                const childField = schemaField.children!.find((c: any) => {
                  const shortName = c.name?.includes('.') ? c.name.split('.').pop() : c.name;
                  return shortName === itemKey || c.name === itemKey;
                });

                // 🔥 DEBUG: 매칭 결과 확인 (dRANGE_MAX/MIN 필드만)
                if (itemKey === 'dRANGE_MAX' || itemKey === 'dRANGE_MIN') {
                  console.log('🔍 [filterConditionalArrayFields] Child matching:', {
                    itemKey,
                    childFieldFound: !!childField,
                    childFieldName: childField?.name,
                    'x-required-when': (childField as any)?.['x-required-when'],
                    'x-optional-when': (childField as any)?.['x-optional-when'],
                    flatDataKeys: Object.keys(flatData),
                    iCALC_RULE_value: flatData['iCALC_RULE']
                  });
                }

                if (childField) {
                  const condition = (childField as any)['x-required-when'];
                  if (condition) {
                    // 조건 평가 - flatData에서 상위 폼의 값 확인
                    const conditionMet = Object.entries(condition).every(([condKey, expectedValue]) => {
                      const actualValue = flatData[condKey];
                      if (typeof expectedValue === 'number') {
                        return Number(actualValue) === expectedValue;
                      }
                      return actualValue === expectedValue;
                    });

                    console.log('🔍 [filterConditionalArrayFields] Condition check:', {
                      itemKey,
                      condition,
                      actualValue: flatData['iCALC_RULE'],
                      conditionMet
                    });

                    if (!conditionMet) {
                      console.log(`🔥 Filtered out conditional field "${key}.${itemKey}" (condition not met: ${JSON.stringify(condition)})`);
                      continue; // 조건 불충족 시 필드 제외
                    }
                  }
                }

                filteredItem[itemKey] = filterConditionalArrayFields(itemValue);
              }
              return filteredItem;
            });
          } else {
            filtered[key] = filterConditionalArrayFields(value);
          }
        }

        return filtered;
      }

      return obj;
    };

    const nestedDynamicData = convertDotNotationToNested(flatData);

    // 🔥 조건부 배열 필드 필터링 적용
    const filteredData = filterConditionalArrayFields(nestedDynamicData);

    const cleaned: any = {
      // 🔥 동적 스키마 필드 (중첩 구조로 변환됨, 체크박스 상태 반영, 조건부 필터링됨)
      ...filteredData,
    };

    // 🔥 UI 전용 키 제거 (__selectedOption 등)
    return cleanUIKeys(cleaned);
  };

  // 🔥 JSON에서 UI 전용 키 제거 (후처리)
  const cleanUIKeys = (obj: any): any => {
    if (obj === null || obj === undefined) return obj;

    if (Array.isArray(obj)) {
      return obj.map(item => cleanUIKeys(item));
    }

    if (typeof obj === 'object') {
      const cleaned: any = {};

      for (const [key, value] of Object.entries(obj)) {
        // UI 전용 키 필터링
        if (key.startsWith('__') || key.startsWith('_') && key !== '_id') {
          continue; // __selectedOption, __section_0, _enabled 등 제외
        }

        cleaned[key] = cleanUIKeys(value);
      }

      return cleaned;
    }

    return obj;
  };

  // 🔥 URI 패턴에 따라 래퍼 키 결정
  // ⚠️ 매칭 알고리즘 (shared.yaml과 일치):
  //   1. priority DESC 정렬 (undefined는 defaultHandlers.wrapperPriorityDefault 적용)
  //   2. 동률이면 리스트 순서 유지 (stable sort)
  //   3. 첫 매칭 rule 반환 (short-circuit)
  const getWrapperKey = (): string | null => {
    const path = endpoint.path || '';

    console.log('🔍 getWrapperKey called:', { path, wrapperRules });

    // 🔥 priority 기반 정렬 (stable sort - 동률은 원래 순서 유지)
    // priority 없으면 wrapperPriorityDefault 적용 (shared.yaml SSOT)
    const sortedRules = [...wrapperRules]
      .map((rule, index) => ({ ...rule, _originalIndex: index }))
      .sort((a, b) => {
        const priorityA = a.priority ?? wrapperPriorityDefault;
        const priorityB = b.priority ?? wrapperPriorityDefault;
        if (priorityB !== priorityA) {
          return priorityB - priorityA;  // DESC
        }
        return (a as any)._originalIndex - (b as any)._originalIndex;  // stable
      });

    // 첫 매칭 반환 (short-circuit)
    for (const rule of sortedRules) {
      const regex = new RegExp(rule.pattern);
      if (regex.test(path)) {
        console.log('✅ Matched rule:', rule);
        return rule.wrapper;
      }
    }

    console.log('❌ No matching wrapper rule for path:', path);
    return null; // 래퍼 없음
  };

  // 🔥 Request Body를 래퍼로 변환하는 함수
  const wrapWithAssign = (body: string): string => {
    const wrapperKey = getWrapperKey();

    // 🔥 useAssignWrapper가 명시적으로 false가 아닌 이상 래퍼 적용 (undefined도 true로 간주)
    const shouldUseWrapper = settings.useAssignWrapper !== false;

    console.log('🔍 wrapWithAssign called:', {
      wrapperKey,
      useAssignWrapper: settings.useAssignWrapper,
      shouldUseWrapper,
      endpointPath: endpoint.path,
      bodyLength: body.length
    });

    // 래퍼가 필요 없으면 원본 반환
    if (!wrapperKey || !shouldUseWrapper) {
      console.log('❌ No wrapper needed, wrapperKey:', wrapperKey, 'shouldUseWrapper:', shouldUseWrapper);
      return body;
    }

    try {
      const parsed = JSON.parse(body);

      // 이미 래퍼가 있으면 그대로 반환
      if (parsed && typeof parsed === 'object' && (wrapperKey in parsed)) {
        console.log('✅ Already wrapped with', wrapperKey);
        return body;
      }

      // 🔥 모든 인스턴스를 래퍼로 감싸기
      const allInstances: any = {};
      Object.keys(assignInstances).forEach(key => {
        const instanceData = assignInstances[key];

        // 🔥 FIX: buildCleanJSON과 동일한 로직 적용
        // Required+Visible 필드는 null이어도 포함되어야 함
        const convertDotNotationToNestedWithRequired = (flatData: any) => {
          const nested: any = {};

          // 🎯 Step 1: Required + Visible 필드를 data 복사본에 추가
          const enrichedData = { ...flatData };
          schemaFields.forEach(field => {
            const runtimeState = fieldRuntimeStates[field.name];
            // 🔥 Rule: Required+Visible는 값이 없거나 빈 값이어도 key를 생성
            if (runtimeState && runtimeState.requiredNow && runtimeState.visible) {
              if (!(field.name in enrichedData) || enrichedData[field.name] === '') {
                // enum 필드는 첫 번째 옵션, 그 외는 null
                if (field.enum && field.enum.length > 0) {
                  enrichedData[field.name] = field.enum[0];
                } else {
                  enrichedData[field.name] = null;
                }
              }
            }
          });

          Object.keys(enrichedData).forEach(fieldKey => {
            // 🔥 섹션 헤더 키 제외 (UI 전용)
            if (fieldKey.startsWith('__section_')) {
              return;
            }

            if (fieldKey.endsWith('._enabled')) {
              return;
            }

            const value = enrichedData[fieldKey];

            // 🔥 FIX: Required 필드는 null도 포함
            const runtimeState = fieldRuntimeStates[fieldKey];
            const isRequired = runtimeState?.requiredNow && runtimeState?.visible;

            // 🔥 빈 값 제외 (단, Required 필드는 예외)
            if (!isRequired && (value === '' || value === undefined)) {
              return;
            }
            // null은 Required 필드만 허용
            if (value === null && !isRequired) {
              return;
            }

            if (fieldKey.includes('.')) {
              const parts = fieldKey.split('.');
              const parentKey = parts[0];

              if (enrichedData[`${parentKey}._enabled`] === false) {
                return;
              }

              let current = nested;
              for (let i = 0; i < parts.length - 1; i++) {
                if (!current[parts[i]]) {
                  current[parts[i]] = {};
                }
                current = current[parts[i]];
              }

              current[parts[parts.length - 1]] = value;
            } else {
              nested[fieldKey] = value;
            }
          });

          return nested;
        };

        // 🔥 UI 전용 키 제거 후 저장
        allInstances[key] = cleanUIKeys(convertDotNotationToNestedWithRequired(instanceData));
      });

      const wrapped = {
        [wrapperKey]: allInstances
      };

      console.log('✅ Wrapped with', wrapperKey, wrapped);
      return JSON.stringify(wrapped, null, 2);
    } catch (error) {
      // JSON 파싱 실패 시 원본 반환
      console.warn('Failed to parse request body for Assign wrapper:', error);
      return body;
    }
  };

  // 🔥 assignInstances를 안정적으로 추적하기 위한 memoized string
  const assignInstancesKey = useMemo(() => {
    return JSON.stringify(assignInstances);
  }, [assignInstances]);

  const wrapperRulesKey = useMemo(() => {
    return JSON.stringify(wrapperRules);
  }, [wrapperRules]);

  // 🔥 dynamicFormData를 안정적으로 추적하기 위한 memoized string
  const dynamicFormDataKey = useMemo(() => {
    return JSON.stringify(dynamicFormData);
  }, [dynamicFormData]);

  // 🔥 fieldRuntimeStates를 안정적으로 추적하기 위한 memoized string
  const fieldRuntimeStatesKey = useMemo(() => {
    return JSON.stringify(fieldRuntimeStates);
  }, [fieldRuntimeStates]);

  // 🔥 invisible 필드를 dynamicFormData에서 제거 (VariantAxis 변경 시)
  // ✅ 이게 핵심: UI는 바뀌는데 JSON이 안 바뀌는 이유는 이전 값이 남아있기 때문
  useEffect(() => {
    if (Object.keys(fieldRuntimeStates).length === 0) return;

    const invisibleFields: string[] = [];
    Object.entries(fieldRuntimeStates).forEach(([key, state]) => {
      if (!state.visible && dynamicFormData[key] !== undefined) {
        invisibleFields.push(key);
      }
    });

    if (invisibleFields.length > 0) {
      setDynamicFormData((prev: any) => {
        const updated = { ...prev };
        invisibleFields.forEach(key => {
          delete updated[key];
        });
        return updated;
      });

      // Assign 인스턴스에서도 제거
      if (settings.useAssignWrapper) {
        setAssignInstances((prev: any) => {
          const updated = { ...prev };
          Object.keys(updated).forEach(instanceKey => {
            invisibleFields.forEach(fieldKey => {
              if (updated[instanceKey][fieldKey] !== undefined) {
                delete updated[instanceKey][fieldKey];
              }
            });
          });
          return updated;
        });
      }
    }
  }, [fieldRuntimeStatesKey, settings.useAssignWrapper]); // ✅ runtimeState 변경 시에만 실행

  // formData 변경 시 JSON 업데이트 (Store에 직접 저장)
  // ✅ 순수 함수 사용: 외부 state 참조 금지, 인자로 명시적 전달
  useEffect(() => {
    // 🔥 스키마가 비어있으면 스킵 (dynamicFormData가 비어도 Required 필드는 포함해야 함)
    if (schemaFields.length === 0) return;

    // ✅ buildCleanJSON에 모든 입력을 명시적으로 전달
    const cleanData = buildCleanJSON(dynamicFormData, fieldRuntimeStates, schemaFields);
    const rawRequestBody = JSON.stringify(cleanData, null, 2);

    // 🔥 래퍼 적용 (URI 패턴에 따라)
    const requestBody = wrapWithAssign(rawRequestBody);

    // Store의 Runner 데이터 업데이트
    updateRunnerData({ requestBody });

    // 🎯 편집 가능한 JSON도 업데이트
    setEditableJson(requestBody);
  }, [
    dynamicFormDataKey,
    fieldRuntimeStatesKey, // ✅ memoized key 사용 (무한 루프 방지)
    assignInstancesKey,
    settings.useAssignWrapper,
    wrapperRulesKey
    // ❌ endpoint.name, endpoint.method, endpoint.path 제거 (불필요, identity 변경 위험)
  ]);

  // Update modified state whenever data changes
  useEffect(() => {
    // ✅ buildCleanJSON에 인자 명시적 전달
    const currentState = JSON.stringify(buildCleanJSON(dynamicFormData, fieldRuntimeStates, schemaFields));
    if (initialState === '') {
      setInitialState(currentState);
    } else if (currentState !== initialState) {
      setIsModified(true);
    } else {
      setIsModified(false);
    }
  }, [dynamicFormDataKey, fieldRuntimeStatesKey]);

  // 🎯 Test Case 저장 핸들러 (신규)
  const handleSaveTestCase = async () => {
    if (!caseName.trim()) {
      toast.error('❌ Please enter a test case name');
      return;
    }

    // ✅ 실제 API 요청에 사용될 JSON 생성 (순수 함수)
    const cleanData = buildCleanJSON(dynamicFormData, fieldRuntimeStates, schemaFields);
    const rawRequestBody = JSON.stringify(cleanData, null, 2);
    const requestBody = wrapWithAssign(rawRequestBody);

    // 🎯 Test Case 저장 (실제 JSON requestBody 저장)
    updateRunnerData({ requestBody }); // Runner에서 사용할 JSON
    addTestCase(caseName.trim(), caseDescription.trim() || undefined);

    // 🔥 글로벌 저장 (DB에 영구 저장)
    try {
      await saveCurrentVersion();
      toast.success(`✅ Test Case "${caseName}" saved successfully!`);

      // 새로 저장한 케이스를 선택 상태로 설정
      const newTestCase = testCases[testCases.length]; // 가장 최근 추가된 케이스
      if (newTestCase) {
        setSelectedTestCaseId(newTestCase.id);
      }
    } catch (error) {
      console.error('Failed to save version:', error);
      toast.error('❌ Failed to save test case');
    }

    // Reset dialog
    setCaseName('');
    setCaseDescription('');
    setShowSaveDialog(false);
  };

  // 🎯 Test Case 업데이트 핸들러 (기존 케이스 수정)
  const handleUpdateTestCase = async () => {
    if (!selectedTestCaseId) {
      toast.error('❌ No test case selected');
      return;
    }

    const selectedTestCase = testCases.find(tc => tc.id === selectedTestCaseId);
    if (!selectedTestCase) {
      toast.error('❌ Test case not found');
      return;
    }

    // ✅ 실제 API 요청에 사용될 JSON 생성 (순수 함수)
    const cleanData = buildCleanJSON(dynamicFormData, fieldRuntimeStates, schemaFields);
    const rawRequestBody = JSON.stringify(cleanData, null, 2);
    const requestBody = wrapWithAssign(rawRequestBody);

    // 🎯 Test Case 업데이트
    const { updateTestCase } = useAppStore.getState();
    updateTestCase(selectedTestCaseId, { requestBody });
    updateRunnerData({ requestBody });

    // 🔥 글로벌 저장 (DB에 영구 저장)
    try {
      await saveCurrentVersion();
      toast.success(`✅ Test Case "${selectedTestCase.name}" updated successfully!`);
    } catch (error) {
      console.error('Failed to save version:', error);
      toast.error('❌ Failed to update test case');
    }
  };

  // 🎯 현재 편집 중인 테스트케이스 초기화 (새로 시작)
  const handleClearTestCase = () => {
    setSelectedTestCaseId(null);

    // 🎯 폼 기본값으로 초기화 (Trigger + Required 필드만)
    const initialData: any = {};
    schemaFields.forEach(field => {
      const isTriggerField = field.enum && Array.isArray(field.enum) && field.enum.length > 0;
      const isAlwaysRequired =
        field.required === true ||
        (typeof field.required === 'object' && field.required['*'] === 'required');

      if (isTriggerField || isAlwaysRequired) {
        if (field.type === 'array' && field.items) {
          initialData[field.name] = getDefaultValue(field);
        } else if (field.type === 'object' && field.children) {
          initialData[`${field.name}._enabled`] = false;
          field.children.forEach(child => {
            initialData[`${field.name}.${child.name}`] = getDefaultValue(child);
          });
        } else {
          initialData[field.name] = getDefaultValue(field);
        }
      }
    });
    setDynamicFormData(initialData);

    // 🔥 Assign 인스턴스 기본값으로 초기화
    setAssignInstances({ '1': initialData });
    setCurrentInstanceKey('1');

    toast.info('📝 Ready to create new test case');
  };

  // 🎯 Test Case 선택 시 폼에 로드
  const handleLoadTestCase = (testCaseId: string) => {
    const testCase = testCases.find(tc => tc.id === testCaseId);
    if (!testCase) return;

    try {
      console.log('📥 Loading Test Case:', testCase.requestBody.substring(0, 200));

      // requestBody는 실제 JSON 형식
      const parsed = JSON.parse(testCase.requestBody);

      // 🔥 Assign 래퍼가 있으면 벗겨내고 인스턴스별로 로드
      if (parsed && typeof parsed === 'object' && 'Assign' in parsed) {
        const assignData = parsed.Assign;
        const loadedInstances: any = {};

        // Assign 내부의 각 인스턴스를 assignInstances로 변환
        Object.keys(assignData).forEach(key => {
          const instanceData = assignData[key];

          // 중첩 구조를 flat structure로 변환
          const flatData: any = {};

          const flattenObject = (obj: any, prefix = '') => {
            Object.keys(obj).forEach(key => {
              const value = obj[key];
              const newKey = prefix ? `${prefix}.${key}` : key;

              if (value !== null && typeof value === 'object' && !Array.isArray(value)) {
                // Object인 경우: _enabled를 true로 설정하고 자식들을 펼침
                flatData[`${newKey}._enabled`] = true;
                flattenObject(value, newKey);
              } else {
                flatData[newKey] = value;
              }
            });
          };

          flattenObject(instanceData);
          loadedInstances[key] = flatData;
        });

        console.log('✅ Loaded instances:', loadedInstances);
        setAssignInstances(loadedInstances);

        // 첫 번째 인스턴스를 현재 선택
        const firstKey = Object.keys(loadedInstances)[0];
        setCurrentInstanceKey(firstKey);
        setDynamicFormData(loadedInstances[firstKey]);
      }
      // rootKey 형식인 경우 (이전 버전 호환)
      else {
        const rootKey = endpoint.name.toUpperCase();
        if (parsed && typeof parsed === 'object' && rootKey in parsed) {
          const data = parsed[rootKey];

          // 중첩 구조를 flat structure로 변환
          const flatData: any = {};

          const flattenObject = (obj: any, prefix = '') => {
            Object.keys(obj).forEach(key => {
              const value = obj[key];
              const newKey = prefix ? `${prefix}.${key}` : key;

              if (value !== null && typeof value === 'object' && !Array.isArray(value)) {
                flatData[`${newKey}._enabled`] = true;
                flattenObject(value, newKey);
              } else {
                flatData[newKey] = value;
              }
            });
          };

          flattenObject(data);

          // "1" 인스턴스로 로드
          setAssignInstances({ "1": flatData });
          setCurrentInstanceKey("1");
          setDynamicFormData(flatData);
        }
      }

      setSelectedTestCaseId(testCaseId);
      toast.success(`✅ Test Case "${testCase.name}" loaded successfully!`);
    } catch (error) {
      console.error('Failed to load test case:', error);
      toast.error('❌ Failed to load test case');
    }
  };

  const handleDeleteTestCase = async (caseId: string, e: React.MouseEvent) => {
    e.stopPropagation();
    if (confirm('이 Test Case를 삭제하시겠습니까?')) {
      deleteTestCase(caseId);
      if (selectedTestCaseId === caseId) {
        setSelectedTestCaseId(null);
      }
      if (editingTestCaseId === caseId) {
        setEditingTestCaseId(null);
      }

      // 🔥 글로벌 저장 (DB에 영구 저장)
      try {
        await saveCurrentVersion();
        toast.success('Test Case가 삭제되었습니다');
      } catch (error) {
        console.error('Failed to save after delete:', error);
        toast.error('삭제 후 저장에 실패했습니다');
      }
    }
  };

  // 🎯 테스트케이스 이름 편집 시작
  const handleStartEditName = (testCaseId: string, currentName: string, e: React.MouseEvent) => {
    e.stopPropagation();
    setEditingTestCaseId(testCaseId);
    setEditingTestCaseName(currentName);
  };

  // 🎯 테스트케이스 이름 편집 취소
  const handleCancelEditName = () => {
    setEditingTestCaseId(null);
    setEditingTestCaseName('');
  };

  // 🎯 테스트케이스 이름 저장
  const handleSaveEditName = async (testCaseId: string) => {
    const trimmedName = editingTestCaseName.trim();

    if (!trimmedName) {
      toast.error('❌ Test Case 이름은 비어있을 수 없습니다');
      return;
    }

    const testCase = testCases.find(tc => tc.id === testCaseId);
    if (testCase && trimmedName === testCase.name) {
      // 변경사항 없음
      handleCancelEditName();
      return;
    }

    // 이름 중복 체크
    const isDuplicate = testCases.some(
      tc => tc.id !== testCaseId && tc.name.toLowerCase() === trimmedName.toLowerCase()
    );

    if (isDuplicate) {
      toast.error('❌ 같은 이름의 Test Case가 이미 존재합니다');
      return;
    }

    // 업데이트
    const { updateTestCase } = useAppStore.getState();
    updateTestCase(testCaseId, { name: trimmedName });

    // 🔥 글로벌 저장 (DB에 영구 저장)
    try {
      await saveCurrentVersion();
      toast.success(`✅ Test Case 이름이 "${trimmedName}"로 변경되었습니다`);
      setEditingTestCaseId(null);
      setEditingTestCaseName('');
    } catch (error) {
      console.error('Failed to save after rename:', error);
      toast.error('❌ 이름 변경 후 저장에 실패했습니다');
    }
  };


  return (
    <div className="flex h-full w-full relative">
      {/* Left Sidebar - Test Case List */}
      <div className="w-80 flex flex-col border-r border-zinc-800 bg-zinc-950 flex-shrink-0">
        <div className="p-4 border-b border-zinc-800 bg-zinc-900 flex-shrink-0">
          <h3 className="text-sm font-semibold mb-1">🧪 Test Cases</h3>
          <p className="text-xs text-zinc-500">Select a case to load</p>
        </div>

        <ScrollArea className="flex-1 h-0">
          <div className="p-4 space-y-2">
            {testCases.length === 0 ? (
              <div className="text-center py-8">
                <FileText className="w-12 h-12 mx-auto mb-3 text-zinc-700" />
                <p className="text-sm text-zinc-500 mb-1">No test cases yet</p>
                <p className="text-xs text-zinc-600">
                  Click "Save as Test Case" to create one
                </p>
              </div>
            ) : (
              testCases.map((testCase) => (
                <div
                  key={testCase.id}
                  onClick={() => handleLoadTestCase(testCase.id)}
                  className={`group relative p-3 rounded-lg border cursor-pointer transition-all ${selectedTestCaseId === testCase.id
                    ? 'bg-blue-900/20 border-blue-700'
                    : 'bg-zinc-900 border-zinc-800 hover:border-zinc-700 hover:bg-zinc-800'
                    }`}
                >
                  {/* Delete Button */}
                  <Button
                    size="sm"
                    variant="ghost"
                    onClick={(e) => handleDeleteTestCase(testCase.id, e)}
                    className="absolute top-2 right-2 opacity-0 group-hover:opacity-100 transition-opacity h-6 w-6 p-0 text-red-400 hover:text-red-300 hover:bg-red-900/20"
                  >
                    <Trash2 className="w-3 h-3" />
                  </Button>

                  <div className="flex items-start gap-2 mb-2">
                    <div className={`w-2 h-2 rounded-full mt-1.5 flex-shrink-0 ${selectedTestCaseId === testCase.id ? 'bg-blue-500' : 'bg-zinc-600'
                      }`} />
                    <div className="flex-1 min-w-0">
                      {editingTestCaseId === testCase.id ? (
                        // 🔥 편집 모드
                        <div className="flex items-center gap-1">
                          <Input
                            value={editingTestCaseName}
                            onChange={(e) => setEditingTestCaseName(e.target.value)}
                            onKeyDown={(e) => {
                              if (e.key === 'Enter') {
                                e.stopPropagation();
                                handleSaveEditName(testCase.id);
                              } else if (e.key === 'Escape') {
                                e.stopPropagation();
                                handleCancelEditName();
                              }
                            }}
                            onClick={(e) => e.stopPropagation()}
                            onBlur={() => handleSaveEditName(testCase.id)}
                            autoFocus
                            className="h-7 text-sm font-semibold bg-zinc-800 border-blue-500 text-white focus:ring-2 focus:ring-blue-500"
                          />
                        </div>
                      ) : (
                        // 🔥 일반 모드
                        <h4
                          className="text-sm font-semibold text-zinc-100 truncate cursor-text hover:text-blue-300 transition-colors"
                          onDoubleClick={(e) => handleStartEditName(testCase.id, testCase.name, e)}
                          title="더블클릭하여 이름 변경"
                        >
                          {testCase.name}
                        </h4>
                      )}
                      {testCase.description && !editingTestCaseId && (
                        <p className="text-xs text-zinc-400 mt-1 line-clamp-2">
                          {testCase.description}
                        </p>
                      )}
                    </div>
                  </div>

                  <div className="flex items-center gap-2 text-xs text-zinc-500 mt-2">
                    <Clock className="w-3 h-3" />
                    <span>{new Date(testCase.createdAt).toLocaleDateString()}</span>
                  </div>
                </div>
              ))
            )}
          </div>
        </ScrollArea>
      </div>

      {/* Center: Context-Aware Form Builder */}
      <div className="flex-1 flex flex-col border-r border-zinc-800 bg-zinc-950 overflow-hidden">
        <div className="p-4 border-b border-zinc-800 bg-zinc-900 flex-shrink-0">
          <div className="flex items-center justify-between mb-2">
            <h3 className="text-sm flex items-center gap-2">
              🏗️ Context-Aware Builder
              {hasEnhancedSchema && (
                <span className="px-2 py-0.5 bg-green-600/20 text-green-400 text-[10px] rounded border border-green-600/50">
                  Enhanced Schema Active
                </span>
              )}
            </h3>
            {selectedTestCaseId && (
              <Button
                onClick={handleClearTestCase}
                size="sm"
                variant="outline"
                className="h-7 text-xs border-zinc-700 hover:bg-zinc-800"
              >
                <Plus className="w-3 h-3 mr-1" />
                New Test Case
              </Button>
            )}
          </div>

          {/* 🔥 현재 상태 표시 배너 */}
          {selectedTestCaseId ? (
            <div className="flex items-center gap-2 px-3 py-2 bg-blue-900/20 border border-blue-700/50 rounded-lg">
              <Edit className="w-4 h-4 text-blue-400" />
              <div className="flex-1">
                <p className="text-xs font-semibold text-blue-300">
                  Editing: {testCases.find(tc => tc.id === selectedTestCaseId)?.name || 'Unknown'}
                </p>
                <p className="text-[10px] text-blue-400/70">
                  수정 후 "Update Test Case" 버튼을 눌러 저장하세요
                </p>
              </div>
            </div>
          ) : (
            <div className="flex items-center gap-2 px-3 py-2 bg-green-900/20 border border-green-700/50 rounded-lg">
              <Plus className="w-4 h-4 text-green-400" />
              <div className="flex-1">
                <p className="text-xs font-semibold text-green-300">
                  Creating New Test Case
                </p>
                <p className="text-[10px] text-green-400/70">
                  구성 완료 후 "Save as New Test Case" 버튼을 눌러 저장하세요
                </p>
              </div>
            </div>
          )}
        </div>

        <ScrollArea className="flex-1 h-0">
          <div className="p-6 space-y-6">
            {/* 🔥 Assign Instance Selector */}
            {settings.useAssignWrapper && (
              <section className="bg-gradient-to-br from-blue-950/50 to-zinc-900 border-2 border-blue-800/50 rounded-lg p-4">
                <div className="flex items-center justify-between mb-3">
                  <h3 className="text-sm font-semibold flex items-center gap-2">
                    <span className="text-xl">🔢</span>
                    Assign Instances
                  </h3>
                  <Button
                    onClick={addAssignInstance}
                    size="sm"
                    className="h-7 text-xs bg-blue-600 hover:bg-blue-500"
                  >
                    + Add Instance
                  </Button>
                </div>
                <div className="flex flex-wrap gap-2">
                  {Object.keys(assignInstances).sort((a, b) => parseInt(a) - parseInt(b)).map((key) => (
                    <div key={key} className="flex items-center gap-1">
                      <button
                        onClick={() => setCurrentInstanceKey(key)}
                        className={`px-3 py-1 rounded text-sm transition-colors ${currentInstanceKey === key
                          ? 'bg-blue-600 text-white'
                          : 'bg-zinc-800 text-zinc-400 hover:bg-zinc-700'
                          }`}
                      >
                        {key}
                      </button>
                      {Object.keys(assignInstances).length > 1 && (
                        <button
                          onClick={() => removeAssignInstance(key)}
                          className="p-1 rounded text-red-400 hover:bg-red-900/20"
                          title="Remove instance"
                        >
                          <Trash2 className="w-3 h-3" />
                        </button>
                      )}
                    </div>
                  ))}
                </div>
                <p className="text-xs text-zinc-500 mt-2">
                  Select an instance to edit. Each instance represents a separate item in the Assign wrapper.
                </p>
              </section>
            )}

            {/* 🎯 Dynamic Schema-Based Form */}
            {schemaFields.length > 0 && (
              <section className="bg-gradient-to-br from-purple-950/50 to-zinc-900 border-2 border-purple-800/50 rounded-lg p-6">
                <h3 className="text-lg font-semibold mb-4 flex items-center gap-2">
                  <span className="text-xl">📝</span>
                  Schema-Based Fields
                  {settings.useAssignWrapper && (
                    <span className="px-2 py-0.5 bg-blue-600/20 text-blue-400 text-[10px] rounded border border-blue-600/50">
                      Instance: {currentInstanceKey}
                    </span>
                  )}
                  {hasEnhancedSchema && (
                    <span className="px-2 py-0.5 bg-green-600/20 text-green-400 text-[10px] rounded border border-green-600/50">
                      From Spec Tab
                    </span>
                  )}
                </h3>
                {/* 🔥 YAML 정의 기반 동적 렌더러 */}
                {builderDefinition ? (
                  <DynamicSchemaRenderer
                    definition={builderDefinition}
                    schemaFields={schemaFields}
                    dynamicFormData={dynamicFormData}
                    updateDynamicField={updateDynamicField}
                    expandedObjects={expandedObjects}
                    toggleObject={toggleObject}
                    fieldRuntimeStates={fieldRuntimeStates}
                  />
                ) : (
                  <div className="text-center py-8 text-zinc-500">
                    Loading schema definition...
                  </div>
                )}
              </section>
            )}
          </div>
        </ScrollArea>
      </div>

      {/* Right: Clean JSON Preview */}
      <div className="relative flex flex-col bg-zinc-950 overflow-hidden flex-shrink-0" style={{ width: `${rightPanelWidth}px` }}>
        {/* 🎯 Resizable Handle - 패널의 왼쪽 경계 */}
        <div
          className="absolute left-0 top-0 bottom-0 w-1 hover:w-2 bg-zinc-700 hover:bg-blue-500 cursor-ew-resize transition-all z-10"
          onMouseDown={handleResizeStart}
          title="Drag to resize"
        />

        <div className="p-4 border-b border-zinc-800 bg-zinc-900 flex-shrink-0">
          <div className="flex items-center justify-between">
            <div>
              <h3 className="text-sm mb-1 flex items-center gap-2">
                {jsonPreviewMode === 'monaco' ? '📝' : '✨'} JSON Preview
                {jsonPreviewMode === 'monaco' && (
                  <span className="px-2 py-0.5 bg-green-600/20 text-green-400 text-[10px] rounded border border-green-600/50">
                    Editable
                  </span>
                )}
              </h3>
              <p className="text-xs text-zinc-500">
                {jsonPreviewMode === 'monaco'
                  ? '⚡ 코드를 수정하고 "Apply" 버튼을 클릭하세요'
                  : '정제된 Request Body (스키마 기반)'}
              </p>
            </div>
            <div className="flex items-center gap-2">
              <button
                onClick={() => setJsonPreviewMode('annotated')}
                className={`px-3 py-1 text-xs rounded transition-colors ${jsonPreviewMode === 'annotated'
                  ? 'bg-blue-600 text-white'
                  : 'bg-zinc-800 text-zinc-400 hover:bg-zinc-700'
                  }`}
              >
                🏷️ View
              </button>
              <button
                onClick={() => setJsonPreviewMode('monaco')}
                className={`px-3 py-1 text-xs rounded transition-colors ${jsonPreviewMode === 'monaco'
                  ? 'bg-blue-600 text-white'
                  : 'bg-zinc-800 text-zinc-400 hover:bg-zinc-700'
                  }`}
              >
                📝 Edit
              </button>
            </div>
          </div>
        </div>

        <div className="flex-1 flex flex-col overflow-hidden">
          {/* JSON Preview - Switchable */}
          <div className="flex-1 overflow-auto">
            {jsonPreviewMode === 'monaco' ? (
              <div className="h-full flex flex-col">
                <div className="flex-1 px-4 pt-4 min-h-0">
                  <CodeEditor
                    value={editableJson}
                    onChange={(value) => setEditableJson(value || '')}
                    language="json"
                    readOnly={false}
                    minimap={true}
                  />
                </div>
                <div className="px-4 py-3 border-t border-zinc-800 bg-zinc-900/50 flex items-center justify-between gap-3">
                  <p className="text-xs text-zinc-400 flex items-center gap-2">
                    <span className="text-yellow-400">💡</span>
                    JSON을 수정한 후 버튼을 클릭하면 왼쪽 폼에 반영됩니다
                  </p>
                  <Button
                    onClick={() => convertJsonToFormData(editableJson)}
                    size="sm"
                    className="bg-green-600 hover:bg-green-500 text-white shadow-lg shadow-green-600/20"
                  >
                    ✨ Apply to Form
                  </Button>
                </div>
              </div>
            ) : (
              <ScrollArea className="h-full">
                <div className="p-4">
                  <div className="p-4 bg-zinc-900/50 rounded-lg border border-zinc-800">
                    <JSONRenderer data={(() => {
                      // 🔥 editableJson 상태를 사용 (이미 래퍼가 적용된 상태)
                      try {
                        return JSON.parse(editableJson);
                      } catch (error) {
                        // ✅ 파싱 실패 시 빈 객체 반환 (buildCleanJSON 호출 금지)
                        return {};
                      }
                    })()} />
                  </div>
                </div>
              </ScrollArea>
            )}
          </div>
        </div>

        {/* 🎯 Footer with Save Button - Spec Tab Style */}
        <div className="border-t border-zinc-800 bg-zinc-900 p-4 flex items-center justify-between flex-shrink-0">
          <div className="flex items-center gap-2 text-xs text-zinc-500">
            {isModified ? (
              <>
                <div className="w-2 h-2 bg-orange-500 rounded-full animate-pulse"></div>
                <span>Unsaved configuration</span>
              </>
            ) : (
              <>
                <div className="w-2 h-2 bg-green-500 rounded-full"></div>
                <span>Ready to save</span>
              </>
            )}
          </div>

          <div className="flex items-center gap-2">
            <Button
              onClick={() => {
                // 🎯 Reset to default values (Trigger + Required 필드만)
                const initialData: any = {};
                schemaFields.forEach(field => {
                  const isTriggerField = field.enum && Array.isArray(field.enum) && field.enum.length > 0;
                  const isAlwaysRequired =
                    field.required === true ||
                    (typeof field.required === 'object' && field.required['*'] === 'required');

                  if (isTriggerField || isAlwaysRequired) {
                    if (field.type === 'array' && field.items) {
                      initialData[field.name] = getDefaultValue(field);
                    } else if (field.type === 'object' && field.children) {
                      initialData[`${field.name}._enabled`] = false;
                      field.children.forEach(child => {
                        initialData[`${field.name}.${child.name}`] = getDefaultValue(child);
                      });
                    } else {
                      initialData[field.name] = getDefaultValue(field);
                    }
                  }
                });
                setDynamicFormData(initialData);
                setIsModified(false);
                setInitialState('');
              }}
              variant="outline"
              size="sm"
              disabled={!isModified}
              className="h-8 text-xs border-zinc-700 hover:bg-zinc-800"
            >
              <RefreshCw className="w-3 h-3 mr-2" />
              Reset
            </Button>

            {selectedTestCaseId ? (
              // 🔥 수정 모드: Update 버튼
              <Button
                onClick={handleUpdateTestCase}
                size="sm"
                disabled={!isModified}
                className="h-8 text-xs bg-orange-600 hover:bg-orange-500 disabled:opacity-50 disabled:cursor-not-allowed"
              >
                <RefreshCw className="w-3 h-3 mr-2" />
                Update Test Case
              </Button>
            ) : (
              // 🔥 신규 모드: Save as New 버튼
              <Button
                onClick={() => setShowSaveDialog(true)}
                size="sm"
                disabled={!isModified}
                className="h-8 text-xs bg-blue-600 hover:bg-blue-500 disabled:opacity-50 disabled:cursor-not-allowed"
              >
                <Save className="w-3 h-3 mr-2" />
                Save as New Test Case
              </Button>
            )}
          </div>
        </div>
      </div>

      {/* 🎯 Save Test Case Dialog */}
      <Dialog open={showSaveDialog} onOpenChange={setShowSaveDialog}>
        <DialogContent className="bg-zinc-900 border-zinc-700 max-w-md">
          <DialogHeader>
            <DialogTitle className="text-xl text-white">💾 Save Test Case</DialogTitle>
            <DialogDescription className="text-zinc-400">
              현재 구성을 Test Case로 저장합니다. Runner 탭에서 선택하여 실행할 수 있습니다.
            </DialogDescription>
          </DialogHeader>

          <div className="space-y-4 py-4">
            <div className="space-y-2">
              <Label htmlFor="case-name" className="text-sm font-semibold text-zinc-200">
                Test Case Name *
              </Label>
              <Input
                id="case-name"
                value={caseName}
                onChange={(e) => setCaseName(e.target.value)}
                placeholder="e.g., Newton_GEOM_10Steps"
                className="bg-zinc-800 border-zinc-700 text-white"
              />
            </div>

            <div className="space-y-2">
              <Label htmlFor="case-description" className="text-sm font-semibold text-zinc-200">
                Description <span className="text-zinc-500 font-normal">(optional)</span>
              </Label>
              <Textarea
                id="case-description"
                value={caseDescription}
                onChange={(e) => setCaseDescription(e.target.value)}
                placeholder="e.g., Newton-Raphson method with GEOM nonlinearity"
                className="bg-zinc-800 border-zinc-700 text-white min-h-[80px]"
              />
            </div>

            <div className="p-3 bg-zinc-800/50 border border-zinc-700 rounded-lg">
              <div className="text-xs text-zinc-400 mb-1">Current Configuration:</div>
              <div className="text-sm text-zinc-200">
                🔧 Schema-based form with {schemaFields.length} field(s)
              </div>
            </div>
          </div>

          <DialogFooter>
            <Button
              variant="outline"
              onClick={() => setShowSaveDialog(false)}
              className="border-zinc-700 hover:bg-zinc-800 text-zinc-200"
            >
              Cancel
            </Button>
            <Button
              onClick={handleSaveTestCase}
              className="bg-blue-600 hover:bg-blue-500"
            >
              <Save className="w-4 h-4 mr-2" />
              Save
            </Button>
          </DialogFooter>
        </DialogContent>
      </Dialog>
    </div>
  );
}