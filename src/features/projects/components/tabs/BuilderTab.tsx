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
  compileSchema,
  type UIBuilderField
} from '@/lib/schema';
import {
  enhancedSchemaToBuilderFields,
  schemaToBuilderFields,
  type EnhancedSchema
} from '@/lib/schema/builderAdapter';
import { DynamicSchemaRenderer } from '@/lib/rendering/dynamicRenderer';
import { loadCachedDefinition, loadBuilderRules, type DefinitionType } from '@/lib/rendering/definitionLoader';
import { useEndpoints } from '@/hooks/useEndpoints';
import { getPSDForProduct } from '@/config/psdMapping';

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
  // ⚠️ specData가 없으면 빈 스키마 사용 (fallback 제거하여 정확성 보장)
  if (!specData?.jsonSchema) {
    return (
      <div className="flex-1 flex items-center justify-center bg-zinc-950 text-zinc-600">
        <div className="text-center">
          <AlertCircle className="w-12 h-12 mx-auto mb-3 opacity-50" />
          <p className="text-sm">No schema available for this endpoint</p>
          <p className="text-xs text-zinc-700 mt-2">Please load a version with schema data</p>
        </div>
      </div>
    );
  }
  
  const combinedSpecData = {
    jsonSchema: specData.jsonSchema || specData.jsonSchemaOriginal,
    jsonSchemaOriginal: specData.jsonSchemaOriginal || specData.jsonSchema,
    jsonSchemaEnhanced: specData.jsonSchemaEnhanced,
    savedSchema: (specData as any)?.savedSchema,
  };
  
  const activeSchema = resolveActiveSchema(combinedSpecData);
  const hasEnhancedSchema = isEnhancedSchemaActive(combinedSpecData);
  
  // 🔥 Builder Rules에서 wrapper rules 및 enhanced schema markers 로드
  const [wrapperRules, setWrapperRules] = useState<Array<{ pattern: string; wrapper: string }>>([]);
  const [enhancedSchemaMarkers, setEnhancedSchemaMarkers] = useState<string[]>([]);
  
  useEffect(() => {
    const loadBuilderConfig = async () => {
      try {
        // 🔥 제품의 PSD 설정 사용
        const builderDef = await loadBuilderRules(psdSet, schemaType);
        
        if (builderDef.wrapperRules) {
          console.log('✅ Loaded wrapper rules from', `${psdSet}/${schemaType}:`, builderDef.wrapperRules);
          setWrapperRules(builderDef.wrapperRules as Array<{ pattern: string; wrapper: string }>);
        }
        
        if (builderDef.enhancedSchemaMarkers) {
          console.log('✅ Loaded enhanced schema markers:', builderDef.enhancedSchemaMarkers);
          setEnhancedSchemaMarkers(builderDef.enhancedSchemaMarkers);
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
  
  // 🔥 NEW: Schema Compiler로 정규화된 AST 생성
  const canonicalFields = useMemo(() => {
    if (isNewEnhancedSchema) {
      return [];
    }
    return compileSchema(activeSchema, psdSet, schemaType);
  }, [activeSchema, isNewEnhancedSchema, psdSet, schemaType]);
  
  // 🔥 Temporary state to track form values for enhanced schema
  const [tempFormValuesForSchema, setTempFormValuesForSchema] = useState<Record<string, any>>({});
  
  // 🔥 NEW: UI Schema Adapter로 빌더 필드 생성
  const schemaFields: UIBuilderField[] = useMemo(() => {
    if (isNewEnhancedSchema) {
      // Enhanced Schema: 새 어댑터 사용 (현재 폼 값 전달하여 동적 업데이트)
      return enhancedSchemaToBuilderFields(activeSchema as EnhancedSchema, tempFormValuesForSchema, psdSet, schemaType);
    }
    // Original Schema: compileSchema로 SectionGroup[] 생성 후 schemaToBuilderFields로 변환
    return schemaToBuilderFields(activeSchema as EnhancedSchema, tempFormValuesForSchema, psdSet, schemaType);
  }, [canonicalFields, isNewEnhancedSchema, activeSchema, tempFormValuesForSchema, psdSet, schemaType]);
  
  // 🔥 기본값 적용 헬퍼 함수 (공통)
  const getDefaultValue = (field: UIBuilderField): any => {
    // 1. 명시적 default 값이 있으면 사용
    if (field.default !== undefined && field.default !== null) {
      return field.default;
    }
    
    // 2. 타입별 최소 초기값 (JSON Preview에 불필요한 값이 안 들어가도록)
    // Required 필드는 사용자가 직접 입력해야 하므로 빈 값으로 초기화
    if (field.type === 'array') return [];  // 배열은 빈 배열
    if (field.type === 'boolean') return false;  // boolean은 false
    if (field.type === 'enum' && field.enum && field.enum.length > 0) {
      // enum은 첫 번째 값 (선택이 필요하므로)
      return field.enum[0];
    }
    
    // 3. number, integer, string은 빈 문자열 (사용자가 입력하도록)
    // 이렇게 하면 JSON Preview에 불필요한 0이 표시되지 않음
    return '';
  };
  
  // 🎯 스키마 기반 동적 상태 (기존 하드코딩 대체)
  const [dynamicFormData, setDynamicFormData] = useState<any>(() => {
    const initialData: any = {};
    schemaFields.forEach(field => {
      // 중첩 필드는 점(.) 표기법으로 저장
      if (field.type === 'array' && field.items) {
        initialData[field.name] = getDefaultValue(field);
      } else if (field.type === 'object' && field.children) {
        // 🔥 Object with children: 각 자식 필드를 dot notation으로 초기화
        initialData[`${field.name}._enabled`] = false; // 체크박스 상태
        field.children.forEach(child => {
          initialData[`${field.name}.${child.name}`] = getDefaultValue(child);
        });
      } else {
        initialData[field.name] = getDefaultValue(field);
      }
    });
    return initialData;
  });
  
  // 🔥 Assign 인스턴스 관리 (여러 노드를 위한 상태)
  const [assignInstances, setAssignInstances] = useState<{ [key: string]: any }>(() => {
    // 기본적으로 "1" 인스턴스 하나 생성 (공통 getDefaultValue 함수 사용)
    const initialData: any = {};
    schemaFields.forEach(field => {
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
    });
    
    return {
      "1": initialData
    };
  });
  
  // Assign 인스턴스 추가
  const addAssignInstance = () => {
    const keys = Object.keys(assignInstances);
    const nextKey = String(Math.max(...keys.map(k => parseInt(k) || 0)) + 1);
    
    // 🔥 새 인스턴스는 기본값으로 초기화
    const newInstanceData: any = {};
    schemaFields.forEach(field => {
      if (field.type === 'array' && field.items) {
        newInstanceData[field.name] = getDefaultValue(field);
      } else if (field.type === 'object' && field.children) {
        newInstanceData[`${field.name}._enabled`] = false;
        field.children.forEach(child => {
          newInstanceData[`${field.name}.${child.name}`] = getDefaultValue(child);
        });
      } else {
        newInstanceData[field.name] = getDefaultValue(field);
      }
    });
    
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
  useEffect(() => {
    if (isNewEnhancedSchema) {
      // Flatten dot notation to nested object for schema evaluation
      const flatValues: Record<string, any> = {};
      for (const [key, value] of Object.entries(dynamicFormData)) {
        if (!key.includes('.')) {
          flatValues[key] = value;
        }
      }
      setTempFormValuesForSchema(flatValues);
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
    const definitionType: DefinitionType = isNewEnhancedSchema ? 'enhanced' : 'original';
    const overrideType = settings.schemaDefinition || definitionType;
    
    // 🔥 제품의 PSD 설정 사용
    loadCachedDefinition(
      overrideType, 
      'builder',
      undefined, // schemaSet (deprecated)
      psdSet, // psdSet (Level 1)
      overrideType // schemaType (Level 2)
    )
      .then(def => setBuilderDefinition(def))
      .catch(err => console.error('Failed to load builder definition:', err));
  }, [isNewEnhancedSchema, settings.schemaDefinition, psdSet]);
  
  // 🎯 스키마 변경 시 동적 폼 데이터 재초기화
  // 🔥 schemaFields가 변경되면 (조건부 필드 포함) 동적으로 업데이트
  useEffect(() => {
    const initialData: any = {};
    schemaFields.forEach(field => {
      const existingValue = dynamicFormData[field.name];
      if (field.type === 'array' && field.items) {
        initialData[field.name] = existingValue !== undefined ? existingValue : getDefaultValue(field);
      } else if (field.type === 'object' && field.children) {
        // 🔥 Object with children: 각 자식 필드를 dot notation으로 유지
        const enabledKey = `${field.name}._enabled`;
        initialData[enabledKey] = dynamicFormData[enabledKey] !== undefined ? dynamicFormData[enabledKey] : false;
        
        field.children.forEach(child => {
          // 🔥 child.name이 이미 전체 경로를 포함 (NODE_ELEMS.KEYS)
          const childKey = child.name;
          const existingChildValue = dynamicFormData[childKey];
          initialData[childKey] = existingChildValue !== undefined ? existingChildValue : getDefaultValue(child);
        });
      } else {
        initialData[field.name] = existingValue !== undefined ? existingValue : getDefaultValue(field);
      }
    });
    setDynamicFormData(initialData);
    
    // 🔥 Assign 인스턴스도 동일하게 업데이트 (현재 선택된 인스턴스만)
    if (settings.useAssignWrapper && currentInstanceKey) {
      setAssignInstances(prev => {
        const currentInstanceData = prev[currentInstanceKey] || {};
        const updatedInstanceData: any = {};
        
        schemaFields.forEach(field => {
          const existingValue = currentInstanceData[field.name];
          if (field.type === 'array' && field.items) {
            updatedInstanceData[field.name] = existingValue !== undefined ? existingValue : getDefaultValue(field);
          } else if (field.type === 'object' && field.children) {
            const enabledKey = `${field.name}._enabled`;
            updatedInstanceData[enabledKey] = currentInstanceData[enabledKey] !== undefined ? currentInstanceData[enabledKey] : false;
            
            field.children.forEach(child => {
              // 🔥 child.name이 이미 전체 경로를 포함 (NODE_ELEMS.KEYS)
              const childKey = child.name;
              const existingChildValue = currentInstanceData[childKey];
              updatedInstanceData[childKey] = existingChildValue !== undefined ? existingChildValue : getDefaultValue(child);
            });
          } else {
            updatedInstanceData[field.name] = existingValue !== undefined ? existingValue : getDefaultValue(field);
          }
        });
        
        return {
          ...prev,
          [currentInstanceKey]: updatedInstanceData
        };
      });
    }
  }, [schemaFields.length, JSON.stringify(schemaFields.map(f => f.name))]);
  
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
  
  // 🎯 Resizable Panel 상태 - 초기값을 화면의 50%로 설정
  const [rightPanelWidth, setRightPanelWidth] = useState(() => {
    if (typeof window !== 'undefined') {
      return Math.min(window.innerWidth / 2, 800);
    }
    return 600; // fallback for SSR
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
    // 🔥 중첩 경로 정규화: "Assign.1.TYPE" → "TYPE", "Assign.1.__section_Common Keys and Solid__.TYPE" → "TYPE"
    const normalizeFieldPath = (path: string): string => {
      const parts = path.split('.');
      // 🔥 래퍼 키, 숫자, __section__ 제거 (wrapper keys는 builder.yaml에서 동적으로 가져옴)
      const wrapperKeys = [...new Set(wrapperRules.map(rule => rule.wrapper).filter(Boolean))];
      const filtered = parts.filter(p => 
        !wrapperKeys.includes(p) && 
        !/^\d+$/.test(p) && 
        !p.startsWith('__section_')
      );
      return filtered.join('.');
    };
    
    const normalizedPath = normalizeFieldPath(fieldPath);
    
    // 🔥 Enhanced Schema 사용 시: schemaFields에서 required 정보 확인
    if (isNewEnhancedSchema) {
      const field = schemaFields.find(f => f.name === normalizedPath);
      if (field && field.required !== undefined) {
        return field.required
          ? { type: 'required', color: 'text-red-400', label: 'Required' }
          : { type: 'optional', color: 'text-blue-400', label: 'Optional' };
      }
      
      // 중첩 필드 체크 (예: UNIT.FORCE)
      const parts = normalizedPath.split('.');
      if (parts.length > 1) {
        const parentName = parts[0];
        const childName = parts[parts.length - 1];
        const parentField = schemaFields.find(f => f.name === parentName);
        if (parentField && parentField.children) {
          const childField = parentField.children.find(c => c.name === childName);
          if (childField && childField.required !== undefined) {
            return childField.required
              ? { type: 'required', color: 'text-red-400', label: 'Required' }
              : { type: 'optional', color: 'text-blue-400', label: 'Optional' };
          }
        }
      }
      
      // Default for Enhanced Schema
      return { type: 'optional', color: 'text-zinc-400', label: 'Optional' };
    }
    
    // 🔥 Original Schema: canonicalFields에서 required 정보 확인
    const field = canonicalFields.find(f => f.path === normalizedPath || f.name === normalizedPath);
    
    if (field) {
      return field.required
        ? { type: 'required', color: 'text-red-400', label: 'Required' }
        : { type: 'optional', color: 'text-blue-400', label: 'Optional' };
    }
    
    // 중첩 필드 체크 (예: UNIT.FORCE)
    const parentField = canonicalFields.find(f => normalizedPath.startsWith(f.name + '.'));
    if (parentField && parentField.children) {
      const childName = normalizedPath.split('.').pop();
      const childField = parentField.children.find(c => c.name === childName);
      if (childField) {
        return childField.required
          ? { type: 'required', color: 'text-red-400', label: 'Required' }
          : { type: 'optional', color: 'text-blue-400', label: 'Optional' };
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
            {value.map((item, idx) => (
              <div key={idx} className="pl-4">
                {renderValue(item, undefined, depth + 1)}
                {idx < value.length - 1 && <span className="text-zinc-500">,</span>}
              </div>
            ))}
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
                    <span className={`text-[9px] px-1.5 py-0.5 rounded mr-2 ${
                      metadata.type === 'required' 
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
  const buildCleanJSON = () => {
    console.log('🔍 dynamicFormData (start of buildCleanJSON):', dynamicFormData);
    
    // 🔥 동적 스키마 필드를 중첩 구조로 변환 (_enabled 체크박스 반영)
    const convertDotNotationToNested = (flatData: any) => {
      const nested: any = {};
      
      // 🔥 oneOf 필드 수집: 부모 필드명 -> 선택된 옵션 인덱스
      const oneOfSelections: Map<string, number> = new Map();
      Object.keys(flatData).forEach(key => {
        const match = key.match(/^(.+)\.__selectedOption$/);
        if (match) {
          const parentField = match[1];
          const selectedOption = flatData[key] || 0;
          oneOfSelections.set(parentField, selectedOption);
        }
      });
      
      // 🔥 oneOf 필드의 옵션별 필드 매핑 (schemaFields에서 추출)
      const oneOfFieldsByOption: Map<string, Map<number, Set<string>>> = new Map();
      schemaFields.forEach(field => {
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
          console.log(`🔍 oneOf field detected: ${field.name}`, {
            options: field.oneOfOptions,
            fieldMap: Array.from(fieldMap.entries()).map(([idx, fields]) => 
              ({ optionIndex: idx, fields: Array.from(fields) }))
          });
        }
      });
      
      console.log('🔍 oneOf selections:', Array.from(oneOfSelections.entries()));
      console.log('🔍 oneOf field mappings:', Array.from(oneOfFieldsByOption.entries()));
      
      Object.keys(flatData).forEach(key => {
        // 🔥 섹션 헤더 키 제외 (UI 전용)
        if (key.startsWith('__section_') || key.includes('.__section_')) {
          console.log('🔥 Filtering out section:', key);
          return;
        }
        
        // 🔥 oneOf 선택 상태 키 제외 (UI 전용)
        if (key.endsWith('.__selectedOption') || key.includes('.__oneOf')) {
          console.log('🔥 Filtering out selectedOption:', key);
          return;
        }
        
        // _enabled 키는 제외
        if (key.endsWith('._enabled')) {
          return;
        }
        
        // 🔥 빈 값 제외
        const value = flatData[key];
        if (value === '' || value === null || value === undefined) {
          console.log(`❌ Skipped (empty value): ${key}=${value}`);
          return;
        }
        
        console.log('✅ Processing key:', key, 'value:', value);
        
        if (key.includes('.')) {
          // dot notation을 중첩 객체로 변환
          const parts = key.split('.');
          const parentKey = parts[0];
          const childKey = parts[parts.length - 1];
          
          // 🔥 부모 객체가 체크되어 있지 않으면 스킵
          if (flatData[`${parentKey}._enabled`] === false) {
            console.log(`❌ Skipped (parent disabled): ${key}, _enabled=${flatData[`${parentKey}._enabled`]}`);
            return;
          }
          
          // 🔥 oneOf 필드인 경우, 선택되지 않은 옵션의 필드는 제외
          if (oneOfSelections.has(parentKey) && oneOfFieldsByOption.has(parentKey)) {
            const selectedOption = oneOfSelections.get(parentKey)!;
            const fieldMap = oneOfFieldsByOption.get(parentKey)!;
            const selectedFields = fieldMap.get(selectedOption);
            
            console.log(`🔍 oneOf check: ${parentKey}.${childKey}`, {
              selectedOption,
              selectedFields: selectedFields ? Array.from(selectedFields) : 'none',
              childKey,
              isIncluded: selectedFields ? selectedFields.has(childKey) : 'no mapping'
            });
            
            // 🔥 selectedFields가 있으면 선택된 필드만 포함, 없으면 모두 포함 (oneOf가 아닌 경우)
            if (selectedFields) {
              if (!selectedFields.has(childKey)) {
                console.log(`❌ Filtered out (unselected option): ${key}`);
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
    
    const nestedDynamicData = convertDotNotationToNested(dynamicFormData);
    
    const cleaned: any = {
      // 🔥 동적 스키마 필드 (중첩 구조로 변환됨, 체크박스 상태 반영)
      ...nestedDynamicData,
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
  const getWrapperKey = (): string | null => {
    const path = endpoint.path || '';
    
    console.log('🔍 getWrapperKey called:', { path, wrapperRules });
    
    // wrapperRules를 순회하며 매칭되는 패턴 찾기
    for (const rule of wrapperRules) {
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
        const convertDotNotationToNested = (flatData: any) => {
          const nested: any = {};
          
          Object.keys(flatData).forEach(fieldKey => {
            // 🔥 섹션 헤더 키 제외 (UI 전용)
            if (fieldKey.startsWith('__section_')) {
              return;
            }
            
            if (fieldKey.endsWith('._enabled')) {
              return;
            }
            
            // 🔥 빈 값 제외 (default가 없는 필드는 JSON에 포함하지 않음)
            const value = flatData[fieldKey];
            if (value === '' || value === null || value === undefined) {
              return;
            }
            
            if (fieldKey.includes('.')) {
              const parts = fieldKey.split('.');
              const parentKey = parts[0];
              
              if (flatData[`${parentKey}._enabled`] === false) {
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
        allInstances[key] = cleanUIKeys(convertDotNotationToNested(instanceData));
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

  // formData 변경 시 JSON 업데이트 (Store에 직접 저장)
  useEffect(() => {
    console.log('🔄 useEffect triggered - updating JSON');
    const cleanData = buildCleanJSON();
    const rawRequestBody = JSON.stringify(cleanData, null, 2);
    
    console.log('📦 Raw request body:', rawRequestBody);
    
    // 🔥 래퍼 적용 (URI 패턴에 따라)
    const requestBody = wrapWithAssign(rawRequestBody);
    
    console.log('🎁 Final request body:', requestBody);
    
    // Store의 Runner 데이터 업데이트
    updateRunnerData({ requestBody });
    
    // 🎯 편집 가능한 JSON도 업데이트
    setEditableJson(requestBody);
  }, [JSON.stringify(assignInstances), endpoint.name, endpoint.method, endpoint.path, settings.useAssignWrapper, JSON.stringify(wrapperRules)]);
  
  // Update modified state whenever data changes
  useEffect(() => {
    const currentState = JSON.stringify(buildCleanJSON());
    if (initialState === '') {
      setInitialState(currentState);
    } else if (currentState !== initialState) {
      setIsModified(true);
    } else {
      setIsModified(false);
    }
  }, [dynamicFormData]);

  // 🎯 Test Case 저장 핸들러 (신규)
  const handleSaveTestCase = async () => {
    if (!caseName.trim()) {
      toast.error('❌ Please enter a test case name');
      return;
    }
    
    // 🔥 실제 API 요청에 사용될 JSON 생성
    const cleanData = buildCleanJSON();
    const rawRequestBody = JSON.stringify(cleanData, null, 2);
    const requestBody = wrapWithAssign(rawRequestBody);
    
    console.log('💾 Saving New Test Case:', {
      name: caseName,
      assignInstances,
      requestBody: requestBody.substring(0, 200)
    });
    
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

    // 🔥 실제 API 요청에 사용될 JSON 생성
    const cleanData = buildCleanJSON();
    const rawRequestBody = JSON.stringify(cleanData, null, 2);
    const requestBody = wrapWithAssign(rawRequestBody);

    console.log('🔄 Updating Test Case:', {
      id: selectedTestCaseId,
      name: selectedTestCase.name,
      requestBody: requestBody.substring(0, 200)
    });

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
    
    // 🔥 폼 기본값으로 초기화
    const initialData: any = {};
    schemaFields.forEach(field => {
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
                  className={`group relative p-3 rounded-lg border cursor-pointer transition-all ${
                    selectedTestCaseId === testCase.id
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
                    <div className={`w-2 h-2 rounded-full mt-1.5 flex-shrink-0 ${
                      selectedTestCaseId === testCase.id ? 'bg-blue-500' : 'bg-zinc-600'
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
                        className={`px-3 py-1 rounded text-sm transition-colors ${
                          currentInstanceKey === key
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
                className={`px-3 py-1 text-xs rounded transition-colors ${
                  jsonPreviewMode === 'annotated'
                    ? 'bg-blue-600 text-white'
                    : 'bg-zinc-800 text-zinc-400 hover:bg-zinc-700'
                }`}
              >
                🏷️ View
              </button>
              <button
                onClick={() => setJsonPreviewMode('monaco')}
                className={`px-3 py-1 text-xs rounded transition-colors ${
                  jsonPreviewMode === 'monaco'
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
                        // 파싱 실패 시 폴백
                        const cleanData = buildCleanJSON();
                        const finalData = cleanUIKeys(cleanData);
                        return finalData;
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
                // 🔥 Reset to default values
                const initialData: any = {};
                schemaFields.forEach(field => {
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