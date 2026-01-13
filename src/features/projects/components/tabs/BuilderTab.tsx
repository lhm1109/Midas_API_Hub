import { useState, useEffect, useMemo } from 'react';
import { ScrollArea } from '@/components/ui/scroll-area';
import { Button } from '@/components/ui/button';
import { Trash2, Save, FileText, Clock, AlertCircle, ChevronDown, ChevronRight } from 'lucide-react';
import { Input } from '@/components/ui/input';
import { Label } from '@/components/ui/label';
import {
  Select,
  SelectContent,
  SelectItem,
  SelectTrigger,
  SelectValue,
} from '@/components/ui/select';
import {
  Dialog,
  DialogContent,
  DialogDescription,
  DialogFooter,
  DialogHeader,
  DialogTitle,
} from '@/components/ui/dialog';
import { Textarea } from '@/components/ui/textarea';
import { CodeEditor } from '@/components/common';
import { useAppStore } from '@/store/useAppStore';
import type { ApiEndpoint } from '@/types';
import { toast } from 'sonner';
import { 
  resolveActiveSchema, 
  isEnhancedSchemaActive,
  compileSchema,
  canonicalToBuilderSchema,
  type UIBuilderField
} from '@/lib/schema';

interface BuilderTabProps {
  endpoint: ApiEndpoint;
}

export function BuilderTab({ endpoint }: BuilderTabProps) {
  const { 
    updateRunnerData, 
    addTestCase,
    deleteTestCase,
    runnerData,
    specData,
    saveCurrentVersion,
  } = useAppStore();
  
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
  
  // 🔥 NEW: Schema Compiler로 정규화된 AST 생성
  const canonicalFields = useMemo(() => {
    return compileSchema(activeSchema);
  }, [activeSchema]);
  
  // 🔥 NEW: UI Schema Adapter로 빌더 필드 생성
  const schemaFields: UIBuilderField[] = useMemo(() => {
    return canonicalToBuilderSchema(canonicalFields);
  }, [canonicalFields]);
  
  // 🎯 스키마 기반 동적 상태 (기존 하드코딩 대체)
  const [dynamicFormData, setDynamicFormData] = useState<any>(() => {
    const initialData: any = {};
    schemaFields.forEach(field => {
      // 중첩 필드는 점(.) 표기법으로 저장
      if (field.type === 'array' && field.items) {
        initialData[field.name] = [];
      } else if (field.type === 'object' && field.children) {
        // 🔥 Object with children: 각 자식 필드를 dot notation으로 초기화
        initialData[`${field.name}._enabled`] = false; // 체크박스 상태
        field.children.forEach(child => {
          initialData[`${field.name}.${child.name}`] = child.default !== undefined ? child.default : '';
        });
      } else {
        initialData[field.name] = field.default !== undefined ? field.default : '';
      }
    });
    return initialData;
  });
  
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
  
  // 🎯 스키마 변경 시 동적 폼 데이터 재초기화
  useEffect(() => {
    const initialData: any = {};
    schemaFields.forEach(field => {
      const existingValue = dynamicFormData[field.name];
      if (field.type === 'array' && field.items) {
        initialData[field.name] = existingValue || [];
      } else if (field.type === 'object' && field.children) {
        // 🔥 Object with children: 각 자식 필드를 dot notation으로 유지
        const enabledKey = `${field.name}._enabled`;
        initialData[enabledKey] = dynamicFormData[enabledKey] !== undefined ? dynamicFormData[enabledKey] : false;
        
        field.children.forEach(child => {
          const childKey = `${field.name}.${child.name}`;
          const existingChildValue = dynamicFormData[childKey];
          initialData[childKey] = existingChildValue !== undefined ? existingChildValue : (child.default !== undefined ? child.default : '');
        });
      } else {
        initialData[field.name] = existingValue !== undefined ? existingValue : (field.default !== undefined ? field.default : '');
      }
    });
    setDynamicFormData(initialData);
  }, [JSON.stringify(activeSchema)]);
  
  const updateDynamicField = (key: string, value: any) => {
    setDynamicFormData((prev: any) => ({ ...prev, [key]: value }));
  };
  
  // 🎯 Test Case 저장 다이얼로그 상태
  const [showSaveDialog, setShowSaveDialog] = useState(false);
  const [caseName, setCaseName] = useState('');
  const [caseDescription, setCaseDescription] = useState('');
  
  // 🎯 선택된 Test Case 상태
  const [selectedTestCaseId, setSelectedTestCaseId] = useState<string | null>(null);
  
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
    return JSON.stringify({ [rootKey]: {} }, null, 2);
  });
  
  // 🎨 JSON 필드 메타데이터 정의 (스키마 기반)
  const getFieldMetadata = (fieldPath: string): { type: 'required' | 'optional'; color: string; label: string } => {
    // 스키마 필드에서 required 정보 찾기
    const field = canonicalFields.find(f => f.path === fieldPath || f.name === fieldPath);
    
    if (field) {
      return field.required
        ? { type: 'required', color: 'text-red-400', label: 'Required' }
        : { type: 'optional', color: 'text-blue-400', label: 'Optional' };
    }
    
    // 중첩 필드 체크 (예: UNIT.FORCE)
    const parentField = canonicalFields.find(f => fieldPath.startsWith(f.name + '.'));
    if (parentField && parentField.children) {
      const childName = fieldPath.split('.').pop();
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
    const renderValue = (value: any, key?: string): JSX.Element => {
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
                {renderValue(item)}
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
              
              return (
                <div key={k} className="pl-4 group hover:bg-zinc-800/30 transition-colors rounded py-0.5">
                  <span className={`${metadata.color} font-semibold`}>"{k}"</span>
                  <span className="text-zinc-500">: </span>
                  {/* 🏷️ Inline Badge */}
                  <span className={`text-[9px] px-1.5 py-0.5 rounded mr-2 ${
                    metadata.type === 'required' 
                      ? 'bg-red-900/50 text-red-300 border border-red-700/50' 
                      : 'bg-blue-900/50 text-blue-300 border border-blue-700/50'
                  }`}>
                    {metadata.label}
                  </span>
                  {renderValue(v, fieldPath)}
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
    
    return <div className="font-mono text-xs leading-relaxed">{renderValue(data)}</div>;
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
      
      // 🔥 1단계: 스키마 기반으로 초기 formData 생성 (모든 필드 초기화)
      const initialData: any = {};
      schemaFields.forEach(field => {
        if (field.type === 'array' && field.items) {
          initialData[field.name] = [];
        } else if (field.type === 'object' && field.children) {
          // Object with children: 각 자식 필드를 dot notation으로 초기화
          initialData[`${field.name}._enabled`] = false;
          field.children.forEach(child => {
            initialData[`${field.name}.${child.name}`] = child.default !== undefined ? child.default : '';
          });
        } else {
          initialData[field.name] = field.default !== undefined ? field.default : '';
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
    // 🔥 동적 스키마 필드를 중첩 구조로 변환 (_enabled 체크박스 반영)
    const convertDotNotationToNested = (flatData: any) => {
      const nested: any = {};
      
      Object.keys(flatData).forEach(key => {
        // _enabled 키는 제외
        if (key.endsWith('._enabled')) {
          return;
        }
        
        if (key.includes('.')) {
          // dot notation을 중첩 객체로 변환 (예: "UNIT.FORCE" -> nested.UNIT.FORCE)
          const parts = key.split('.');
          const parentKey = parts[0];
          
          // 🔥 부모 객체가 체크되어 있지 않으면 스킵
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
          
          current[parts[parts.length - 1]] = flatData[key];
        } else {
          // dot notation이 아닌 필드는 그대로 추가
          nested[key] = flatData[key];
        }
      });
      
      return nested;
    };
    
    const nestedDynamicData = convertDotNotationToNested(dynamicFormData);
    
    const cleaned: any = {
      // 🔥 동적 스키마 필드 (중첩 구조로 변환됨, 체크박스 상태 반영)
      ...nestedDynamicData,
    };
    
    return cleaned;
  };

  // formData 변경 시 JSON 업데이트 (Store에 직접 저장)
  useEffect(() => {
    const rootKey = endpoint.name.toUpperCase();
    const cleanData = buildCleanJSON();
    const result = { [rootKey]: cleanData };
    const requestBody = JSON.stringify(result, null, 2);
    
    // Store의 Runner 데이터 업데이트
    updateRunnerData({ requestBody });
    
    // 🎯 편집 가능한 JSON도 업데이트
    setEditableJson(requestBody);
  }, [dynamicFormData, endpoint.name]);
  
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

  // 🎯 Test Case 저장 핸들러
  const handleSaveTestCase = async () => {
    if (!caseName.trim()) {
      toast.error('❌ Please enter a test case name');
      return;
    }
    
    // 🎯 현재 Builder의 폼 데이터 수집
    const currentFormData = {
      dynamicFormData,
    };
    
    // 🎯 JSON으로 변환하여 requestBody로 저장
    const requestBody = JSON.stringify(currentFormData, null, 2);
    
    // 🎯 Test Case 저장 (requestBody에 폼 데이터 포함)
    updateRunnerData({ requestBody }); // 먼저 runnerData 업데이트
    addTestCase(caseName.trim(), caseDescription.trim() || undefined);
    
    // 🔥 글로벌 저장 (DB에 영구 저장)
    try {
      await saveCurrentVersion();
      toast.success(`✅ Test Case "${caseName}" saved successfully!`);
    } catch (error) {
      console.error('Failed to save version:', error);
      toast.error('❌ Failed to save test case');
    }
    
    // Reset dialog
    setCaseName('');
    setCaseDescription('');
    setShowSaveDialog(false);
  };
  
  // 🎯 Test Case 선택 시 폼에 로드
  const handleLoadTestCase = (testCaseId: string) => {
    const testCase = testCases.find(tc => tc.id === testCaseId);
    if (!testCase) return;
    
    try {
      const formData = JSON.parse(testCase.requestBody);
      
      // 폼 데이터 복원
      if (formData.dynamicFormData) setDynamicFormData(formData.dynamicFormData);
      
      setSelectedTestCaseId(testCaseId);
      toast.success(`Test Case "${testCase.name}"를 로드했습니다`);
    } catch (error) {
      console.error('Failed to load test case:', error);
      toast.error('Test Case 로드에 실패했습니다');
    }
  };
  
  const handleDeleteTestCase = async (caseId: string, e: React.MouseEvent) => {
    e.stopPropagation();
    if (confirm('이 Test Case를 삭제하시겠습니까?')) {
      deleteTestCase(caseId);
      if (selectedTestCaseId === caseId) {
        setSelectedTestCaseId(null);
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
                      <h4 className="text-sm font-semibold text-zinc-100 truncate">
                        {testCase.name}
                      </h4>
                      {testCase.description && (
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
          <h3 className="text-sm mb-1 flex items-center gap-2">
            🏗️ Context-Aware Builder
            {hasEnhancedSchema && (
              <span className="px-2 py-0.5 bg-green-600/20 text-green-400 text-[10px] rounded border border-green-600/50">
                Enhanced Schema Active
              </span>
            )}
          </h3>
          <p className="text-xs text-zinc-500">
            Spec 기반 지능형 조립기 {hasEnhancedSchema && '(조건부 필드 지원)'}
          </p>
        </div>

        <ScrollArea className="flex-1 h-0">
          <div className="p-6 space-y-6">
            {/* 🎯 Dynamic Schema-Based Form */}
            {schemaFields.length > 0 && (
              <section className="bg-gradient-to-br from-purple-950/50 to-zinc-900 border-2 border-purple-800/50 rounded-lg p-6">
                <h3 className="text-lg font-semibold mb-4 flex items-center gap-2">
                  <span className="text-2xl">📝</span>
                  Schema-Based Fields
                  {hasEnhancedSchema && (
                    <span className="px-2 py-0.5 bg-green-600/20 text-green-400 text-[10px] rounded border border-green-600/50">
                      From Spec Tab
                    </span>
                  )}
                </h3>
                <p className="text-xs text-zinc-400 mb-4">
                  ✨ 이 필드들은 Spec 탭에서 저장한 JSON Schema로부터 자동 생성되었습니다
                </p>
                
                <div className="space-y-4">
                  {schemaFields.map((field) => (
                    <div key={field.name} className="space-y-2">
                      {/* 🔥 Object with Children: 아코디언 + 체크박스 */}
                      {field.type === 'object' && field.children && field.children.length > 0 ? (
                        <div className="border border-zinc-700 rounded-md bg-zinc-900/50">
                          {/* 헤더: 체크박스 + 이름 + 아코디언 버튼 */}
                          <div className="flex items-center gap-2 p-3 bg-zinc-800/50">
                            <input
                              type="checkbox"
                              checked={dynamicFormData[`${field.name}._enabled`] || false}
                              onChange={(e) => updateDynamicField(`${field.name}._enabled`, e.target.checked)}
                              className="w-4 h-4"
                            />
                            <button
                              onClick={() => toggleObject(field.name)}
                              className="flex-1 flex items-center gap-2 text-left hover:text-white transition-colors"
                            >
                              {expandedObjects.has(field.name) ? (
                                <ChevronDown className="w-4 h-4 text-zinc-400" />
                              ) : (
                                <ChevronRight className="w-4 h-4 text-zinc-400" />
                              )}
                              <Label className="text-sm flex items-center gap-2 cursor-pointer">
                                {field.name}
                                {field.required && <span className="text-red-400">*</span>}
                                {field.description && (
                                  <span className="text-xs text-zinc-500 font-normal">
                                    ({field.description})
                                  </span>
                                )}
                              </Label>
                            </button>
                            <span className="text-[10px] text-zinc-600 font-mono">
                              {field.type}
                            </span>
                          </div>
                          
                          {/* 확장된 내용: 자식 필드들 */}
                          {expandedObjects.has(field.name) && (
                            <div className="p-4 space-y-3 bg-zinc-900/30">
                              {field.children.map((child) => (
                                <div key={child.name} className="space-y-2 pl-4 border-l-2 border-zinc-700">
                                  <Label className="text-xs flex items-center gap-2">
                                    {child.name}
                                    {child.required && <span className="text-red-400">*</span>}
                                    {child.description && (
                                      <span className="text-xs text-zinc-500 font-normal">
                                        ({child.description})
                                      </span>
                                    )}
                                    <span className="text-[10px] text-zinc-600 font-mono ml-auto">
                                      {child.type}
                                    </span>
                                  </Label>
                                  
                                  {/* Child input field */}
                                  {child.enum ? (
                                    <Select
                                      value={String(dynamicFormData[`${field.name}.${child.name}`] || '')}
                                      onValueChange={(value) => updateDynamicField(`${field.name}.${child.name}`, value)}
                                      disabled={!dynamicFormData[`${field.name}._enabled`]}
                                    >
                                      <SelectTrigger className="bg-zinc-800 border-zinc-700 h-8 text-xs">
                                        <SelectValue placeholder={`Select ${child.name}`} />
                                      </SelectTrigger>
                                      <SelectContent>
                                        {child.enum.map((option: any) => (
                                          <SelectItem key={String(option)} value={String(option)}>
                                            {String(option)}
                                          </SelectItem>
                                        ))}
                                      </SelectContent>
                                    </Select>
                                  ) : child.type === 'number' || child.type === 'integer' ? (
                                    <Input
                                      type="number"
                                      value={dynamicFormData[`${field.name}.${child.name}`] || ''}
                                      onChange={(e) => updateDynamicField(`${field.name}.${child.name}`, parseFloat(e.target.value) || 0)}
                                      className="bg-zinc-800 border-zinc-700 h-8 text-xs"
                                      placeholder={child.default?.toString() || '0'}
                                      disabled={!dynamicFormData[`${field.name}._enabled`]}
                                    />
                                  ) : child.type === 'boolean' ? (
                                    <div className="flex items-center gap-2">
                                      <input
                                        type="checkbox"
                                        checked={dynamicFormData[`${field.name}.${child.name}`] || false}
                                        onChange={(e) => updateDynamicField(`${field.name}.${child.name}`, e.target.checked)}
                                        className="w-3 h-3"
                                        disabled={!dynamicFormData[`${field.name}._enabled`]}
                                      />
                                      <span className="text-xs text-zinc-400">
                                        {dynamicFormData[`${field.name}.${child.name}`] ? 'true' : 'false'}
                                      </span>
                                    </div>
                                  ) : (
                                    <Input
                                      value={dynamicFormData[`${field.name}.${child.name}`] || ''}
                                      onChange={(e) => updateDynamicField(`${field.name}.${child.name}`, e.target.value)}
                                      className="bg-zinc-800 border-zinc-700 h-8 text-xs"
                                      placeholder={child.default || `Enter ${child.name}`}
                                      disabled={!dynamicFormData[`${field.name}._enabled`]}
                                    />
                                  )}
                                </div>
                              ))}
                            </div>
                          )}
                        </div>
                      ) : (
                        /* 일반 필드 렌더링 */
                        <>
                          <Label className="text-sm flex items-center gap-2">
                            {field.name}
                            {field.required && <span className="text-red-400">*</span>}
                            {field.description && (
                              <span className="text-xs text-zinc-500 font-normal">
                                ({field.description})
                              </span>
                            )}
                            <span className="text-[10px] text-zinc-600 font-mono ml-auto">
                              {field.type}
                            </span>
                          </Label>
                          
                          {field.enum ? (
                            <Select
                              value={String(dynamicFormData[field.name] || '')}
                              onValueChange={(value) => updateDynamicField(field.name, value)}
                            >
                              <SelectTrigger className="bg-zinc-800 border-zinc-700">
                                <SelectValue placeholder={`Select ${field.name}`} />
                              </SelectTrigger>
                              <SelectContent>
                                {field.enum.map((option: any) => (
                                  <SelectItem key={String(option)} value={String(option)}>
                                    {String(option)}
                                  </SelectItem>
                                ))}
                              </SelectContent>
                            </Select>
                          ) : field.type === 'array' ? (
                            <Textarea
                              placeholder={`Enter ${field.name} as JSON array, e.g., ["item1", "item2"]`}
                              value={typeof dynamicFormData[field.name] === 'string' 
                                ? dynamicFormData[field.name]
                                : JSON.stringify(dynamicFormData[field.name] || [])}
                              onChange={(e) => {
                                try {
                                  const parsed = JSON.parse(e.target.value);
                                  updateDynamicField(field.name, parsed);
                                } catch {
                                  updateDynamicField(field.name, e.target.value);
                                }
                              }}
                              className="bg-zinc-800 border-zinc-700 font-mono text-xs"
                            />
                          ) : field.type === 'object' ? (
                            <Textarea
                              placeholder={`Enter ${field.name} as JSON object, e.g., {"key": "value"}`}
                              value={typeof dynamicFormData[field.name] === 'string' 
                                ? dynamicFormData[field.name]
                                : JSON.stringify(dynamicFormData[field.name] || {}, null, 2)}
                              onChange={(e) => {
                                try {
                                  const parsed = JSON.parse(e.target.value);
                                  updateDynamicField(field.name, parsed);
                                } catch {
                                  updateDynamicField(field.name, e.target.value);
                                }
                              }}
                              className="bg-zinc-800 border-zinc-700 font-mono text-xs min-h-[100px]"
                            />
                          ) : field.type === 'number' || field.type === 'integer' ? (
                            <Input
                              type="number"
                              value={dynamicFormData[field.name] || ''}
                              onChange={(e) => updateDynamicField(field.name, parseFloat(e.target.value) || 0)}
                              className="bg-zinc-800 border-zinc-700"
                              placeholder={field.default?.toString() || '0'}
                              min={field.minimum}
                              max={field.maximum}
                            />
                          ) : field.type === 'boolean' ? (
                            <div className="flex items-center gap-2">
                              <input
                                type="checkbox"
                                checked={dynamicFormData[field.name] || false}
                                onChange={(e) => updateDynamicField(field.name, e.target.checked)}
                                className="w-4 h-4"
                              />
                              <span className="text-sm text-zinc-400">
                                {dynamicFormData[field.name] ? 'true' : 'false'}
                              </span>
                            </div>
                          ) : (
                            <Input
                              value={dynamicFormData[field.name] || ''}
                              onChange={(e) => updateDynamicField(field.name, e.target.value)}
                              className="bg-zinc-800 border-zinc-700"
                              placeholder={field.default || `Enter ${field.name}`}
                            />
                          )}
                        </>
                      )}
                    </div>
                  ))}
                </div>
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
                    <JSONRenderer data={{ [endpoint.name.toUpperCase()]: buildCleanJSON() }} />
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
                // Reset to initial state
                const initialData: any = {};
                schemaFields.forEach(field => {
                  if (field.type === 'array' && field.items) {
                    initialData[field.name] = [];
                  } else if (field.type === 'object' && field.children) {
                    initialData[`${field.name}._enabled`] = false;
                    field.children.forEach(child => {
                      initialData[`${field.name}.${child.name}`] = child.default !== undefined ? child.default : '';
                    });
                  } else {
                    initialData[field.name] = field.default !== undefined ? field.default : '';
                  }
                });
                setDynamicFormData(initialData);
                setIsModified(false);
                setInitialState('');
              }}
              variant="outline"
              size="sm"
              disabled={!isModified}
              className="h-8 text-xs"
            >
              Reset
            </Button>
            
            <Button
              onClick={() => setShowSaveDialog(true)}
              size="sm"
              disabled={!isModified}
              className="h-8 text-xs bg-blue-600 hover:bg-blue-500 disabled:opacity-50 disabled:cursor-not-allowed"
            >
              <Save className="w-3 h-3 mr-2" />
              Save as Test Case
            </Button>
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