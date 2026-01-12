import { useState, useEffect } from 'react';
import { ScrollArea } from './ui/scroll-area';
import { Button } from './ui/button';
import { Plus, Trash2, AlertCircle, Save, FolderOpen, Send } from 'lucide-react';
import { Input } from './ui/input';
import { Label } from './ui/label';
import {
  Select,
  SelectContent,
  SelectItem,
  SelectTrigger,
  SelectValue,
} from './ui/select';
import {
  Dialog,
  DialogContent,
  DialogDescription,
  DialogFooter,
  DialogHeader,
  DialogTitle,
} from './ui/dialog';
import { Textarea } from './ui/textarea';
import { apiSpecs } from '../data/apiSpecs';
import { useAppStore } from '../store/useAppStore';
import type { ApiEndpoint } from './APIListPanel';

interface BuilderTabProps {
  endpoint: ApiEndpoint;
}

export function BuilderTab({ endpoint }: BuilderTabProps) {
  const spec = apiSpecs[endpoint.id];
  const { getCurrentVersion, updateBuilderData, updateRunnerData, addTestCase } = useAppStore();
  
  // 🎯 Enhanced Schema 우선 사용
  const activeSchema = spec?.jsonSchemaEnhanced || spec?.jsonSchema;
  const hasEnhancedSchema = !!spec?.jsonSchemaEnhanced;
  
  // 🎯 Test Case 저장 다이얼로그 상태
  const [showSaveDialog, setShowSaveDialog] = useState(false);
  const [caseName, setCaseName] = useState('');
  const [caseDescription, setCaseDescription] = useState('');
  
  // 🎯 Resizable Panel 상태 - 초기값을 화면의 50%로 설정
  const [rightPanelWidth, setRightPanelWidth] = useState(() => {
    if (typeof window !== 'undefined') {
      return window.innerWidth / 2;
    }
    return 600; // fallback for SSR
  });
  const [isResizing, setIsResizing] = useState(false);
  
  // 🎯 Driver Field: 이 값이 전체 UI를 제어함
  const [iterationMethod, setIterationMethod] = useState<string>('NEWTON');
  
  // Base settings (루트 레벨 필드들만)
  const [baseSettings, setBaseSettings] = useState<any>({
    NONLINEAR_TYPE: 'GEOM',
    ITERATION_METHOD: 'NEWTON',
    // Optional fields with enabled flags
    enableEnergyNorm: false,
    OPT_ENERGY_NORM: false,
    ENERGY_NORM: 0,
    enableDisplacementNorm: false,
    OPT_DISPLACEMENT_NORM: false,
    DISPLACEMENT_NORM: 0,
    enableForceNorm: false,
    OPT_FORCE_NORM: false,
    FORCE_NORM: 0,
  });

  // Method-specific items (각 메소드별 독립적인 데이터)
  const [newtonItems, setNewtonItems] = useState<any[]>([]);
  const [arclenItems, setArclenItems] = useState<any[]>([]);
  const [dispctItems, setDispctItems] = useState<any[]>([]);
  
  // 🎯 Modified state tracking
  const [isModified, setIsModified] = useState(false);
  
  // Track initial state for comparison
  const [initialState, setInitialState] = useState<string>('');
  
  // 🎯 Resize 이벤트 핸들러
  useEffect(() => {
    const handleMouseMove = (e: MouseEvent) => {
      if (!isResizing) return;
      
      const newWidth = window.innerWidth - e.clientX;
      setRightPanelWidth(Math.max(280, Math.min(800, newWidth)));
    };

    const handleMouseUp = () => {
      setIsResizing(false);
    };

    if (isResizing) {
      document.addEventListener('mousemove', handleMouseMove);
      document.addEventListener('mouseup', handleMouseUp);
    }

    return () => {
      document.removeEventListener('mousemove', handleMouseMove);
      document.removeEventListener('mouseup', handleMouseUp);
    };
  }, [isResizing]);

  // 🧹 JSON Pruning: 현재 선택된 메소드에 맞는 데이터만 추출
  const buildCleanJSON = () => {
    const cleaned: any = {
      NONLINEAR_TYPE: baseSettings.NONLINEAR_TYPE,
      ITERATION_METHOD: baseSettings.ITERATION_METHOD,
    };
    
    // Optional fields - only include if enabled
    if (baseSettings.enableEnergyNorm) {
      cleaned.OPT_ENERGY_NORM = baseSettings.OPT_ENERGY_NORM;
      cleaned.ENERGY_NORM = baseSettings.ENERGY_NORM;
    }
    if (baseSettings.enableDisplacementNorm) {
      cleaned.OPT_DISPLACEMENT_NORM = baseSettings.OPT_DISPLACEMENT_NORM;
      cleaned.DISPLACEMENT_NORM = baseSettings.DISPLACEMENT_NORM;
    }
    if (baseSettings.enableForceNorm) {
      cleaned.OPT_FORCE_NORM = baseSettings.OPT_FORCE_NORM;
      cleaned.FORCE_NORM = baseSettings.FORCE_NORM;
    }
    
    // Driver field에 따라 해당하는 items만 포함
    if (iterationMethod === 'NEWTON' && newtonItems.length > 0) {
      cleaned.NEWTON_ITEMS = newtonItems.map(item => {
        const cleanItem: any = {
          ITERATION_METHOD: item.ITERATION_METHOD,
          LCNAME: item.LCNAME,
          NUMBER_STEPS: item.NUMBER_STEPS,
          MAX_ITERATIONS: item.MAX_ITERATIONS,
        };
        // Optional: LOAD_FACTORS
        if (item.enableLoadFactors && item.LOAD_FACTORS && item.LOAD_FACTORS.length > 0) {
          cleanItem.LOAD_FACTORS = item.LOAD_FACTORS;
        }
        return cleanItem;
      });
    } else if (iterationMethod === 'ARC' && arclenItems.length > 0) {
      cleaned.ARCLEN_ITEMS = arclenItems.map(item => {
        const cleanItem: any = {
          ITERATION_METHOD: item.ITERATION_METHOD,
          LCNAME: item.LCNAME,
          NUMBER_STEPS: item.NUMBER_STEPS,
          MAX_ITERATIONS: item.MAX_ITERATIONS,
        };
        // Optional: INITIAL_FORCE_RATIO_ARC_LEN
        if (item.enableInitialForceRatio) {
          cleanItem.INITIAL_FORCE_RATIO_ARC_LEN = item.INITIAL_FORCE_RATIO_ARC_LEN;
        }
        // Optional: MAXIMUM_DISPLACEMENT
        if (item.enableMaxDisplacement) {
          cleanItem.MAXIMUM_DISPLACEMENT = item.MAXIMUM_DISPLACEMENT;
        }
        return cleanItem;
      });
    } else if (iterationMethod === 'DISP' && dispctItems.length > 0) {
      cleaned.DISPCT_ITEMS = dispctItems.map(item => {
        const cleanItem: any = {
          ITERATION_METHOD: item.ITERATION_METHOD,
          LCNAME: item.LCNAME,
          NUMBER_STEPS: item.NUMBER_STEPS,
          MAX_ITERATIONS: item.MAX_ITERATIONS,
          MASTER_NODE: item.MASTER_NODE,
          MAXIMUM_DISPLACEMENT: item.MAXIMUM_DISPLACEMENT,
        };
        // Optional: DIRECTION
        if (item.enableDirection) {
          cleanItem.DIRECTION = item.DIRECTION;
        }
        // Optional: LOAD_FACTORS
        if (item.enableLoadFactors && item.LOAD_FACTORS && item.LOAD_FACTORS.length > 0) {
          cleanItem.LOAD_FACTORS = item.LOAD_FACTORS;
        }
        return cleanItem;
      });
    }
    
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
  }, [baseSettings, newtonItems, arclenItems, dispctItems, iterationMethod, endpoint.name]);
  
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
  }, [iterationMethod, baseSettings, newtonItems, arclenItems, dispctItems]);

  const updateBaseSetting = (key: string, value: any) => {
    setBaseSettings((prev: any) => ({ ...prev, [key]: value }));
    
    // 🔄 Driver Field 변경 감지
    if (key === 'ITERATION_METHOD') {
      setIterationMethod(value);
    }
  };

  // 🏗️ Template creators (각 메소드별 기본 템플릿)
  const createNewtonTemplate = () => ({
    ITERATION_METHOD: 'NEWTON',
    LCNAME: `LC_${newtonItems.length + 1}`,
    NUMBER_STEPS: 10,
    MAX_ITERATIONS: 30,
    enableLoadFactors: false,
    LOAD_FACTORS: [],
  });

  const createArclenTemplate = () => ({
    ITERATION_METHOD: 'ARC',
    LCNAME: `LC_${arclenItems.length + 1}`,
    INITIAL_FORCE_RATIO_ARC_LEN: 0,
    NUMBER_STEPS: 10,
    MAX_ITERATIONS: 30,
    enableMaxDisplacement: false,
    MAXIMUM_DISPLACEMENT: 0,
    enableInitialForceRatio: false,
  });

  const createDispctTemplate = () => ({
    ITERATION_METHOD: 'DISP',
    LCNAME: `LC_${dispctItems.length + 1}`,
    NUMBER_STEPS: 10,
    MAX_ITERATIONS: 30,
    MASTER_NODE: 1,
    enableDirection: false,
    DIRECTION: 0,
    enableMaxDisplacement: false,
    MAXIMUM_DISPLACEMENT: 0,
    enableLoadFactors: false,
    LOAD_FACTORS: [],
  });

  // 🎯 Test Case 저장 핸들러
  const handleSaveTestCase = () => {
    if (!caseName.trim()) {
      alert('Test Case 이름을 입력해주세요.');
      return;
    }
    
    saveTestCase(endpoint.id, caseName.trim(), caseDescription.trim() || undefined);
    
    // Reset dialog
    setCaseName('');
    setCaseDescription('');
    setShowSaveDialog(false);
    
    alert('✅ Test Case가 저장되었습니다!');
  };

  // 🎨 JSON 필드 메타데이터 정의
  const getFieldMetadata = (key: string, parentKey?: string): { type: 'required' | 'optional' | 'conditional'; color: string; label: string } => {
    // Base level Required fields
    const baseRequired = ['NONLINEAR_TYPE', 'ITERATION_METHOD'];
    if (baseRequired.includes(key)) {
      return { type: 'required', color: 'text-red-400', label: 'Required' };
    }
    
    // Base level Optional fields (Convergence Norms)
    const baseOptional = ['OPT_ENERGY_NORM', 'ENERGY_NORM', 'OPT_DISPLACEMENT_NORM', 'DISPLACEMENT_NORM', 'OPT_FORCE_NORM', 'FORCE_NORM'];
    if (baseOptional.includes(key)) {
      return { type: 'optional', color: 'text-blue-400', label: 'Optional' };
    }
    
    // Items array fields
    const itemsFields = ['NEWTON_ITEMS', 'ARCLEN_ITEMS', 'DISPCT_ITEMS'];
    if (itemsFields.includes(key)) {
      return { type: 'conditional', color: 'text-yellow-400', label: 'Conditional' };
    }
    
    // Inside ITEMS - Required fields
    const itemRequired = ['ITERATION_METHOD', 'LCNAME', 'NUMBER_STEPS', 'MAX_ITERATIONS', 'MASTER_NODE', 'MAXIMUM_DISPLACEMENT'];
    if (itemRequired.includes(key)) {
      return { type: 'required', color: 'text-red-400', label: 'Required' };
    }
    
    // Inside ITEMS - Optional fields
    const itemOptional = ['LOAD_FACTORS', 'INITIAL_FORCE_RATIO_ARC_LEN', 'DIRECTION'];
    if (itemOptional.includes(key)) {
      return { type: 'optional', color: 'text-blue-400', label: 'Optional' };
    }
    
    // Default
    return { type: 'required', color: 'text-zinc-300', label: 'Default' };
  };

  // 🎨 커스텀 JSON 렌더러 컴포넌트
  const JSONRenderer = ({ data, indent = 0 }: { data: any; indent?: number }) => {
    const renderValue = (value: any, key?: string): JSX.Element => {
      const indentStr = '  '.repeat(indent);
      const nextIndent = indent + 1;
      
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
              const metadata = getFieldMetadata(k, key);
              
              return (
                <div key={k} className="pl-4 group hover:bg-zinc-800/30 transition-colors rounded">
                  <span className={`${metadata.color} font-semibold`}>"{k}"</span>
                  <span className="text-zinc-500">: </span>
                  {/* 🏷️ Inline Badge */}
                  <span className={`text-[9px] px-1.5 py-0.5 rounded mr-2 ${
                    metadata.type === 'required' ? 'bg-red-900/50 text-red-300' :
                    metadata.type === 'optional' ? 'bg-blue-900/50 text-blue-300' :
                    'bg-yellow-900/50 text-yellow-300'
                  }`}>
                    {metadata.label}
                  </span>
                  {renderValue(v, k)}
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

  if (!spec) {
    return (
      <div className="h-full w-full flex items-center justify-center bg-zinc-950 text-zinc-500">
        No specification available for this endpoint
      </div>
    );
  }

  return (
    <div className="flex h-full w-full relative">
      {/* Left: Context-Aware Form Builder */}
      <div className="flex-1 flex flex-col border-r border-zinc-800 bg-zinc-950 overflow-hidden">
        <div className="p-4 border-b border-zinc-800 bg-zinc-900 flex-shrink-0 flex items-center justify-between">
          <div>
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
          <Button
            onClick={() => {
              const { manualData, setManualData } = useAppStore.getState();
              const cleanData = buildCleanJSON();
              const rootKey = endpoint.name.toUpperCase();
              const requestData = { [rootKey]: cleanData };
              const currentJSON = JSON.stringify(requestData, null, 2);
              
              // JSON을 HTML 형식으로 변환
              const formatJsonToHTML = (jsonStr: string): string => {
                return jsonStr
                  .split('\n')
                  .map(line => {
                    const keyMatch = line.match(/"([^"]+)":\s*(.+)/);
                    if (keyMatch) {
                      const key = keyMatch[1];
                      let value = keyMatch[2].replace(/,$/, '');
                      
                      if (value === 'true' || value === 'false') {
                        value = `<span style="color: #055bcc; font-weight: bold;">${value}</span>`;
                      } else if (value.match(/^"\w+"/)) {
                        value = `<span style="color: #055bcc;">${value}</span>`;
                      } else if (value.match(/^\d+$/)) {
                        value = `<span style="color: #0ab66c;">${value}</span>`;
                      }
                      
                      return line.replace(/"([^"]+)":\s*(.+)/, `<span style="color: #c31b1b;">"$1"</span>: ${value}`);
                    }
                    return line;
                  })
                  .join('<br>');
              };
              
              const newExample = {
                title: `Example from Builder - ${new Date().toLocaleTimeString()}`,
                code: formatJsonToHTML(currentJSON)
              };
              
              const updatedData = {
                ...manualData!,
                examples: [...(manualData?.examples || []), newExample]
              };
              
              setManualData(updatedData);
              alert('✅ Builder JSON sent to Manual tab!');
            }}
            size="sm"
            className="bg-green-600 hover:bg-green-500 h-8"
          >
            <Send className="w-3 h-3 mr-2" />
            Send to Manual
          </Button>
        </div>

        <ScrollArea className="flex-1 h-0">
          <div className="p-6 space-y-6">
            {/* 🎯 Driver Section: Analysis Configuration */}
            <section className="bg-gradient-to-br from-blue-950/50 to-zinc-900 border-2 border-blue-800/50 rounded-lg p-6">
              <h3 className="text-lg font-semibold mb-4 flex items-center gap-2">
                <span className="text-2xl">⚙️</span>
                Analysis Configuration
              </h3>
              
              <div className="space-y-4">
                <div className="space-y-2">
                  <Label className="text-sm">Nonlinear Type</Label>
                  <Select
                    value={baseSettings.NONLINEAR_TYPE || 'GEOM'}
                    onValueChange={(value) => updateBaseSetting('NONLINEAR_TYPE', value)}
                  >
                    <SelectTrigger className="bg-zinc-800 border-zinc-700">
                      <SelectValue />
                    </SelectTrigger>
                    <SelectContent>
                      <SelectItem value="GEOM">GEOM (Geometry)</SelectItem>
                      <SelectItem value="MATL">MATL (Material)</SelectItem>
                      <SelectItem value="GEOM+MATL">GEOM+MATL</SelectItem>
                    </SelectContent>
                  </Select>
                </div>

                <div className="space-y-2">
                  <Label className="text-sm font-semibold text-yellow-400">
                    🎯 Iteration Method (Driver Field)
                  </Label>
                  <p className="text-xs text-zinc-400 mb-2">
                    ⚠️ 이 설정이 아래 섹션을 자동으로 전환합니다
                  </p>
                  <Select
                    value={iterationMethod}
                    onValueChange={(value) => updateBaseSetting('ITERATION_METHOD', value)}
                  >
                    <SelectTrigger className="bg-zinc-800 border-yellow-600/50 border-2">
                      <SelectValue />
                    </SelectTrigger>
                    <SelectContent>
                      <SelectItem value="NEWTON">NEWTON (Newton-Raphson)</SelectItem>
                      <SelectItem value="ARC">ARC (Arc-Length)</SelectItem>
                      <SelectItem value="DISP">DISP (Displacement-Control)</SelectItem>
                    </SelectContent>
                  </Select>
                </div>

                {/* Convergence Norm Settings */}
                <div className="border-t border-zinc-700 pt-4 mt-4">
                  <h4 className="text-sm font-semibold mb-3 text-zinc-300">Convergence Norms</h4>
                  
                  <div className="space-y-3">
                    <div className="flex items-center gap-3">
                      <label className="flex items-center gap-2 cursor-pointer">
                        <input
                          type="checkbox"
                          checked={baseSettings.enableEnergyNorm || false}
                          onChange={(e) => updateBaseSetting('enableEnergyNorm', e.target.checked)}
                          className="w-4 h-4"
                        />
                        <span className="text-sm text-zinc-400">Energy Norm</span>
                      </label>
                      <Input
                        type="number"
                        step="0.001"
                        value={baseSettings.ENERGY_NORM || 0}
                        onChange={(e) => updateBaseSetting('ENERGY_NORM', parseFloat(e.target.value) || 0)}
                        className="bg-zinc-800 border-zinc-700 h-9 text-sm"
                        disabled={!baseSettings.enableEnergyNorm}
                        placeholder="0.001"
                      />
                    </div>
                    
                    <div className="flex items-center gap-3">
                      <label className="flex items-center gap-2 cursor-pointer">
                        <input
                          type="checkbox"
                          checked={baseSettings.enableDisplacementNorm || false}
                          onChange={(e) => updateBaseSetting('enableDisplacementNorm', e.target.checked)}
                          className="w-4 h-4"
                        />
                        <span className="text-sm text-zinc-400">Displacement Norm</span>
                      </label>
                      <Input
                        type="number"
                        step="0.001"
                        value={baseSettings.DISPLACEMENT_NORM || 0}
                        onChange={(e) => updateBaseSetting('DISPLACEMENT_NORM', parseFloat(e.target.value) || 0)}
                        className="bg-zinc-800 border-zinc-700 h-9 text-sm"
                        disabled={!baseSettings.enableDisplacementNorm}
                        placeholder="0.001"
                      />
                    </div>
                    
                    <div className="flex items-center gap-3">
                      <label className="flex items-center gap-2 cursor-pointer">
                        <input
                          type="checkbox"
                          checked={baseSettings.enableForceNorm || false}
                          onChange={(e) => updateBaseSetting('enableForceNorm', e.target.checked)}
                          className="w-4 h-4"
                        />
                        <span className="text-sm text-zinc-400">Force Norm</span>
                      </label>
                      <Input
                        type="number"
                        step="0.001"
                        value={baseSettings.FORCE_NORM || 0}
                        onChange={(e) => updateBaseSetting('FORCE_NORM', parseFloat(e.target.value) || 0)}
                        className="bg-zinc-800 border-zinc-700 h-9 text-sm"
                        disabled={!baseSettings.enableForceNorm}
                        placeholder="0.001"
                      />
                    </div>
                  </div>
                </div>
              </div>
            </section>

            {/* ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ */}
            <div className="border-t-2 border-dashed border-zinc-700 my-6 relative">
              <div className="absolute -top-3 left-1/2 -translate-x-1/2 bg-zinc-950 px-3 text-xs text-zinc-500">
                Context-Specific Configuration
              </div>
            </div>

            {/* 🔀 Conditional Rendering: 선택된 메소드만 표시 (DOM에서 완전히 제거) */}
            
            {/* ✅ Newton-Raphson Method */}
            {iterationMethod === 'NEWTON' && (
              <section className="bg-green-950/30 border-2 border-green-700/50 rounded-lg p-6">
                <div className="flex items-center justify-between mb-4">
                  <div>
                    <h3 className="text-lg font-semibold flex items-center gap-2">
                      <span className="text-2xl">🟢</span>
                      Newton-Raphson Load Cases
                    </h3>
                    <p className="text-xs text-zinc-400 mt-1">
                      Newton 해석을 위한 하중 케이스 구성
                    </p>
                  </div>
                  <span className="text-sm text-zinc-400">
                    {newtonItems.length} case(s)
                  </span>
                </div>

                {/* Load Case Cards */}
                <div className="space-y-3 mb-4">
                  {newtonItems.map((item, idx) => (
                    <div
                      key={idx}
                      className="bg-zinc-900/70 border border-green-700/30 rounded-lg p-4 hover:border-green-500/50 transition-colors"
                    >
                      <div className="flex items-center justify-between mb-3">
                        <h4 className="font-semibold text-sm flex items-center gap-2">
                          📦 Load Case #{idx + 1}
                        </h4>
                        <Button
                          size="sm"
                          variant="ghost"
                          onClick={() => setNewtonItems(newtonItems.filter((_, i) => i !== idx))}
                          className="text-red-400 hover:text-red-300 hover:bg-red-900/20 h-7"
                        >
                          <Trash2 className="w-3 h-3" />
                        </Button>
                      </div>

                      <div className="grid grid-cols-2 gap-3">
                        <div className="col-span-2">
                          <Label className="text-xs text-zinc-400 flex items-center gap-1">
                            Load Case Name <span className="text-red-400">*</span>
                          </Label>
                          <Input
                            value={item.LCNAME || ''}
                            onChange={(e) => {
                              const updated = [...newtonItems];
                              updated[idx].LCNAME = e.target.value;
                              setNewtonItems(updated);
                            }}
                            className="bg-zinc-800 border-red-700/50 focus:border-red-500 h-9 text-sm mt-1"
                            placeholder="e.g., ST_DL_001"
                          />
                        </div>
                        <div>
                          <Label className="text-xs text-zinc-400 flex items-center gap-1">
                            Number Steps <span className="text-red-400">*</span>
                          </Label>
                          <Input
                            type="number"
                            value={item.NUMBER_STEPS || 10}
                            onChange={(e) => {
                              const updated = [...newtonItems];
                              updated[idx].NUMBER_STEPS = parseInt(e.target.value) || 0;
                              setNewtonItems(updated);
                            }}
                            className="bg-zinc-800 border-red-700/50 focus:border-red-500 h-9 text-sm mt-1"
                          />
                        </div>
                        <div>
                          <Label className="text-xs text-zinc-400 flex items-center gap-1">
                            Max Iterations <span className="text-red-400">*</span>
                          </Label>
                          <Input
                            type="number"
                            value={item.MAX_ITERATIONS || 30}
                            onChange={(e) => {
                              const updated = [...newtonItems];
                              updated[idx].MAX_ITERATIONS = parseInt(e.target.value) || 0;
                              setNewtonItems(updated);
                            }}
                            className="bg-zinc-800 border-red-700/50 focus:border-red-500 h-9 text-sm mt-1"
                          />
                        </div>
                        
                        {/* OPTIONAL: Load Factors */}
                        <div className="col-span-2">
                          <div className="flex items-center gap-2 mb-1">
                            <input
                              type="checkbox"
                              checked={item.enableLoadFactors || false}
                              onChange={(e) => {
                                const updated = [...newtonItems];
                                updated[idx].enableLoadFactors = e.target.checked;
                                setNewtonItems(updated);
                              }}
                              className="w-4 h-4"
                            />
                            <Label className="text-xs text-zinc-500 flex items-center gap-1">
                              Load Factors <span className="px-1.5 py-0.5 bg-zinc-700 rounded text-[10px]">Optional</span>
                            </Label>
                          </div>
                          <Input
                            value={item.LOAD_FACTORS?.join(', ') || ''}
                            onChange={(e) => {
                              const updated = [...newtonItems];
                              const factors = e.target.value
                                .split(',')
                                .map((f) => parseFloat(f.trim()))
                                .filter((f) => !isNaN(f));
                              updated[idx].LOAD_FACTORS = factors.length > 0 ? factors : [];
                              setNewtonItems(updated);
                            }}
                            className="bg-zinc-800 border-zinc-700 h-9 text-sm"
                            placeholder="e.g., 1.0, 1.5, 2.0"
                            disabled={!item.enableLoadFactors}
                          />
                          <p className="text-[10px] text-zinc-600 mt-1">
                            {item.enableLoadFactors ? '✅ Will be included in JSON' : '⚠️ Will be excluded from JSON'}
                          </p>
                        </div>
                      </div>
                    </div>
                  ))}
                </div>

                <Button
                  onClick={() => setNewtonItems([...newtonItems, createNewtonTemplate()])}
                  className="w-full bg-green-700 hover:bg-green-600"
                  size="sm"
                >
                  <Plus className="w-4 h-4 mr-2" />
                  Add Newton Load Case
                </Button>
              </section>
            )}

            {/* ✅ Arc-Length Method */}
            {iterationMethod === 'ARC' && (
              <section className="bg-purple-950/30 border-2 border-purple-700/50 rounded-lg p-6">
                <div className="flex items-center justify-between mb-4">
                  <div>
                    <h3 className="text-lg font-semibold flex items-center gap-2">
                      <span className="text-2xl">🟣</span>
                      Arc-Length Load Cases
                    </h3>
                    <p className="text-xs text-zinc-400 mt-1">
                      Arc-Length 해석을 위한 하중 케이스 구성
                    </p>
                  </div>
                  <span className="text-sm text-zinc-400">
                    {arclenItems.length} case(s)
                  </span>
                </div>

                <div className="space-y-3 mb-4">
                  {arclenItems.map((item, idx) => (
                    <div
                      key={idx}
                      className="bg-zinc-900/70 border border-purple-700/30 rounded-lg p-4 hover:border-purple-500/50 transition-colors"
                    >
                      <div className="flex items-center justify-between mb-3">
                        <h4 className="font-semibold text-sm flex items-center gap-2">
                          📦 Load Case #{idx + 1}
                        </h4>
                        <Button
                          size="sm"
                          variant="ghost"
                          onClick={() => setArclenItems(arclenItems.filter((_, i) => i !== idx))}
                          className="text-red-400 hover:text-red-300 hover:bg-red-900/20 h-7"
                        >
                          <Trash2 className="w-3 h-3" />
                        </Button>
                      </div>

                      <div className="grid grid-cols-2 gap-3">
                        <div className="col-span-2">
                          <Label className="text-xs text-zinc-400 flex items-center gap-1">
                            Load Case Name <span className="text-red-400">*</span>
                          </Label>
                          <Input
                            value={item.LCNAME || ''}
                            onChange={(e) => {
                              const updated = [...arclenItems];
                              updated[idx].LCNAME = e.target.value;
                              setArclenItems(updated);
                            }}
                            className="bg-zinc-800 border-red-700/50 focus:border-red-500 h-9 text-sm mt-1"
                            placeholder="e.g., ARC_CASE_1"
                          />
                        </div>
                        <div>
                          <Label className="text-xs text-zinc-400 flex items-center gap-1">
                            Number Steps <span className="text-red-400">*</span>
                          </Label>
                          <Input
                            type="number"
                            value={item.NUMBER_STEPS || 10}
                            onChange={(e) => {
                              const updated = [...arclenItems];
                              updated[idx].NUMBER_STEPS = parseInt(e.target.value) || 0;
                              setArclenItems(updated);
                            }}
                            className="bg-zinc-800 border-red-700/50 focus:border-red-500 h-9 text-sm mt-1"
                          />
                        </div>
                        <div>
                          <Label className="text-xs text-zinc-400 flex items-center gap-1">
                            Max Iterations <span className="text-red-400">*</span>
                          </Label>
                          <Input
                            type="number"
                            value={item.MAX_ITERATIONS || 30}
                            onChange={(e) => {
                              const updated = [...arclenItems];
                              updated[idx].MAX_ITERATIONS = parseInt(e.target.value) || 0;
                              setArclenItems(updated);
                            }}
                            className="bg-zinc-800 border-red-700/50 focus:border-red-500 h-9 text-sm mt-1"
                          />
                        </div>
                        
                        {/* OPTIONAL: Initial Force Ratio */}
                        <div className="col-span-2">
                          <div className="flex items-center gap-2 mb-1">
                            <input
                              type="checkbox"
                              checked={item.enableInitialForceRatio || false}
                              onChange={(e) => {
                                const updated = [...arclenItems];
                                updated[idx].enableInitialForceRatio = e.target.checked;
                                setArclenItems(updated);
                              }}
                              className="w-4 h-4"
                            />
                            <Label className="text-xs text-zinc-500 flex items-center gap-1">
                              Initial Force Ratio <span className="px-1.5 py-0.5 bg-zinc-700 rounded text-[10px]">Optional</span>
                            </Label>
                          </div>
                          <Input
                            type="number"
                            step="0.01"
                            value={item.INITIAL_FORCE_RATIO_ARC_LEN || 0}
                            onChange={(e) => {
                              const updated = [...arclenItems];
                              updated[idx].INITIAL_FORCE_RATIO_ARC_LEN = parseFloat(e.target.value) || 0;
                              setArclenItems(updated);
                            }}
                            className="bg-zinc-800 border-zinc-700 h-9 text-sm"
                            placeholder="e.g., 0.5"
                            disabled={!item.enableInitialForceRatio}
                          />
                          <p className="text-[10px] text-zinc-600 mt-1">
                            {item.enableInitialForceRatio ? '✅ Will be included in JSON' : '⚠️ Will be excluded from JSON'}
                          </p>
                        </div>
                        
                        {/* OPTIONAL: Maximum Displacement */}
                        <div className="col-span-2">
                          <div className="flex items-center gap-2 mb-1">
                            <input
                              type="checkbox"
                              checked={item.enableMaxDisplacement || false}
                              onChange={(e) => {
                                const updated = [...arclenItems];
                                updated[idx].enableMaxDisplacement = e.target.checked;
                                setArclenItems(updated);
                              }}
                              className="w-4 h-4"
                            />
                            <Label className="text-xs text-zinc-500 flex items-center gap-1">
                              Maximum Displacement <span className="px-1.5 py-0.5 bg-zinc-700 rounded text-[10px]">Optional</span>
                            </Label>
                          </div>
                          <Input
                            type="number"
                            step="0.001"
                            value={item.MAXIMUM_DISPLACEMENT || 0}
                            onChange={(e) => {
                              const updated = [...arclenItems];
                              updated[idx].MAXIMUM_DISPLACEMENT = parseFloat(e.target.value) || 0;
                              setArclenItems(updated);
                            }}
                            className="bg-zinc-800 border-zinc-700 h-9 text-sm"
                            placeholder="e.g., 0.5"
                            disabled={!item.enableMaxDisplacement}
                          />
                          <p className="text-[10px] text-zinc-600 mt-1">
                            {item.enableMaxDisplacement ? '✅ Will be included in JSON' : '⚠️ Will be excluded from JSON'}
                          </p>
                        </div>
                      </div>
                    </div>
                  ))}
                </div>

                <Button
                  onClick={() => setArclenItems([...arclenItems, createArclenTemplate()])}
                  className="w-full bg-purple-700 hover:bg-purple-600"
                  size="sm"
                >
                  <Plus className="w-4 h-4 mr-2" />
                  Add Arc-Length Load Case
                </Button>
              </section>
            )}

            {/* ✅ Displacement-Control Method */}
            {iterationMethod === 'DISP' && (
              <section className="bg-orange-950/30 border-2 border-orange-700/50 rounded-lg p-6">
                <div className="flex items-center justify-between mb-4">
                  <div>
                    <h3 className="text-lg font-semibold flex items-center gap-2">
                      <span className="text-2xl">🟠</span>
                      Displacement-Control Load Cases
                    </h3>
                    <p className="text-xs text-zinc-400 mt-1">
                      변위 제어 해석을 위한 하중 케이스 구성
                    </p>
                  </div>
                  <span className="text-sm text-zinc-400">
                    {dispctItems.length} case(s)
                  </span>
                </div>

                <div className="space-y-3 mb-4">
                  {dispctItems.map((item, idx) => (
                    <div
                      key={idx}
                      className="bg-zinc-900/70 border border-orange-700/30 rounded-lg p-4 hover:border-orange-500/50 transition-colors"
                    >
                      <div className="flex items-center justify-between mb-3">
                        <h4 className="font-semibold text-sm flex items-center gap-2">
                          📦 Load Case #{idx + 1}
                        </h4>
                        <Button
                          size="sm"
                          variant="ghost"
                          onClick={() => setDispctItems(dispctItems.filter((_, i) => i !== idx))}
                          className="text-red-400 hover:text-red-300 hover:bg-red-900/20 h-7"
                        >
                          <Trash2 className="w-3 h-3" />
                        </Button>
                      </div>

                      <div className="grid grid-cols-2 gap-3">
                        {/* REQUIRED: Load Case Name */}
                        <div className="col-span-2">
                          <Label className="text-xs text-zinc-400 flex items-center gap-1">
                            Load Case Name <span className="text-red-400">*</span>
                          </Label>
                          <Input
                            value={item.LCNAME || ''}
                            onChange={(e) => {
                              const updated = [...dispctItems];
                              updated[idx].LCNAME = e.target.value;
                              setDispctItems(updated);
                            }}
                            className="bg-zinc-800 border-red-700/50 focus:border-red-500 h-9 text-sm mt-1"
                            placeholder="e.g., DISP_CASE_1"
                          />
                        </div>
                        
                        {/* REQUIRED: Master Node */}
                        <div>
                          <Label className="text-xs text-zinc-400 flex items-center gap-1">
                            Master Node <span className="text-red-400">*</span>
                          </Label>
                          <Input
                            type="number"
                            value={item.MASTER_NODE || 1}
                            onChange={(e) => {
                              const updated = [...dispctItems];
                              updated[idx].MASTER_NODE = parseInt(e.target.value) || 0;
                              setDispctItems(updated);
                            }}
                            className="bg-zinc-800 border-red-700/50 focus:border-red-500 h-9 text-sm mt-1"
                          />
                        </div>
                        
                        {/* OPTIONAL: Direction */}
                        <div>
                          <div className="flex items-center gap-2 mb-1">
                            <input
                              type="checkbox"
                              checked={item.enableDirection || false}
                              onChange={(e) => {
                                const updated = [...dispctItems];
                                updated[idx].enableDirection = e.target.checked;
                                setDispctItems(updated);
                              }}
                              className="w-4 h-4"
                            />
                            <Label className="text-xs text-zinc-500 flex items-center gap-1">
                              Direction <span className="px-1.5 py-0.5 bg-zinc-700 rounded text-[10px]">Optional</span>
                            </Label>
                          </div>
                          <Select
                            value={item.DIRECTION?.toString() || '0'}
                            onValueChange={(value) => {
                              const updated = [...dispctItems];
                              updated[idx].DIRECTION = parseInt(value);
                              setDispctItems(updated);
                            }}
                            disabled={!item.enableDirection}
                          >
                            <SelectTrigger className="bg-zinc-800 border-zinc-700 h-9">
                              <SelectValue />
                            </SelectTrigger>
                            <SelectContent>
                              <SelectItem value="0">0 (Dx)</SelectItem>
                              <SelectItem value="1">1 (Dy)</SelectItem>
                              <SelectItem value="2">2 (Dz)</SelectItem>
                            </SelectContent>
                          </Select>
                          <p className="text-[10px] text-zinc-600 mt-1">
                            {item.enableDirection ? '✅ Will be included in JSON' : '⚠️ Will be excluded from JSON'}
                          </p>
                        </div>
                        
                        {/* REQUIRED: Number Steps */}
                        <div>
                          <Label className="text-xs text-zinc-400 flex items-center gap-1">
                            Number Steps <span className="text-red-400">*</span>
                          </Label>
                          <Input
                            type="number"
                            value={item.NUMBER_STEPS || 10}
                            onChange={(e) => {
                              const updated = [...dispctItems];
                              updated[idx].NUMBER_STEPS = parseInt(e.target.value) || 0;
                              setDispctItems(updated);
                            }}
                            className="bg-zinc-800 border-red-700/50 focus:border-red-500 h-9 text-sm mt-1"
                          />
                        </div>
                        
                        {/* REQUIRED: Max Iterations */}
                        <div>
                          <Label className="text-xs text-zinc-400 flex items-center gap-1">
                            Max Iterations <span className="text-red-400">*</span>
                          </Label>
                          <Input
                            type="number"
                            value={item.MAX_ITERATIONS || 30}
                            onChange={(e) => {
                              const updated = [...dispctItems];
                              updated[idx].MAX_ITERATIONS = parseInt(e.target.value) || 0;
                              setDispctItems(updated);
                            }}
                            className="bg-zinc-800 border-red-700/50 focus:border-red-500 h-9 text-sm mt-1"
                          />
                        </div>
                        
                        {/* REQUIRED: Maximum Displacement */}
                        <div className="col-span-2">
                          <Label className="text-xs text-zinc-400 flex items-center gap-1">
                            Maximum Displacement <span className="text-red-400">*</span>
                          </Label>
                          <Input
                            type="number"
                            step="0.001"
                            value={item.MAXIMUM_DISPLACEMENT || 0}
                            onChange={(e) => {
                              const updated = [...dispctItems];
                              updated[idx].MAXIMUM_DISPLACEMENT = parseFloat(e.target.value) || 0;
                              setDispctItems(updated);
                            }}
                            className="bg-zinc-800 border-red-700/50 focus:border-red-500 h-9 text-sm mt-1"
                            placeholder="e.g., 0.5"
                          />
                        </div>
                        
                        {/* OPTIONAL: Load Factors */}
                        <div className="col-span-2">
                          <div className="flex items-center gap-2 mb-1">
                            <input
                              type="checkbox"
                              checked={item.enableLoadFactors || false}
                              onChange={(e) => {
                                const updated = [...dispctItems];
                                updated[idx].enableLoadFactors = e.target.checked;
                                setDispctItems(updated);
                              }}
                              className="w-4 h-4"
                            />
                            <Label className="text-xs text-zinc-500 flex items-center gap-1">
                              Load Factors <span className="px-1.5 py-0.5 bg-zinc-700 rounded text-[10px]">Optional</span>
                            </Label>
                          </div>
                          <Input
                            value={item.LOAD_FACTORS?.join(', ') || ''}
                            onChange={(e) => {
                              const updated = [...dispctItems];
                              const factors = e.target.value
                                .split(',')
                                .map((f) => parseFloat(f.trim()))
                                .filter((f) => !isNaN(f));
                              updated[idx].LOAD_FACTORS = factors.length > 0 ? factors : [];
                              setDispctItems(updated);
                            }}
                            className="bg-zinc-800 border-zinc-700 h-9 text-sm"
                            placeholder="e.g., 1.0, 1.5, 2.0"
                            disabled={!item.enableLoadFactors}
                          />
                          <p className="text-[10px] text-zinc-600 mt-1">
                            {item.enableLoadFactors ? '✅ Will be included in JSON' : '⚠️ Will be excluded from JSON'}
                          </p>
                        </div>
                      </div>
                    </div>
                  ))}
                </div>

                <Button
                  onClick={() => setDispctItems([...dispctItems, createDispctTemplate()])}
                  className="w-full bg-orange-700 hover:bg-orange-600"
                  size="sm"
                >
                  <Plus className="w-4 h-4 mr-2" />
                  Add Displacement-Control Load Case
                </Button>
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
          onMouseDown={() => setIsResizing(true)}
          title="Drag to resize"
        />
        
        <div className="p-4 border-b border-zinc-800 bg-zinc-900 flex-shrink-0">
          <h3 className="text-sm mb-1 flex items-center gap-2">
            ✨ Clean JSON Preview
          </h3>
          <p className="text-xs text-zinc-500">정제된 Request Body (노이즈 제거됨)</p>
        </div>

        <ScrollArea className="flex-1 h-0">
          <div className="p-4">
            {/* Context Indicator */}
            <div className="mb-4 p-3 bg-blue-950/50 border border-blue-800/50 rounded-lg">
              <div className="text-xs text-zinc-400 mb-1">Current Context:</div>
              <div className="text-sm font-semibold text-blue-400">
                {iterationMethod === 'NEWTON' && '🟢 Newton-Raphson Method'}
                {iterationMethod === 'ARC' && '🟣 Arc-Length Method'}
                {iterationMethod === 'DISP' && '🟠 Displacement-Control Method'}
              </div>
            </div>

            <JSONRenderer data={{ [endpoint.name.toUpperCase()]: buildCleanJSON() }} />
          </div>
        </ScrollArea>
        
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
                setBaseSettings({
                  NONLINEAR_TYPE: 'GEOM',
                  ITERATION_METHOD: 'NEWTON',
                  enableEnergyNorm: false,
                  OPT_ENERGY_NORM: false,
                  ENERGY_NORM: 0,
                  enableDisplacementNorm: false,
                  OPT_DISPLACEMENT_NORM: false,
                  DISPLACEMENT_NORM: 0,
                  enableForceNorm: false,
                  OPT_FORCE_NORM: false,
                  FORCE_NORM: 0,
                });
                setNewtonItems([]);
                setArclenItems([]);
                setDispctItems([]);
                setIterationMethod('NEWTON');
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
            <DialogTitle className="text-xl">💾 Save Test Case</DialogTitle>
            <DialogDescription className="text-zinc-400">
              현재 구성을 Test Case로 저장합니다. Runner 탭에서 선택하여 실행할 수 있습니다.
            </DialogDescription>
          </DialogHeader>

          <div className="space-y-4 py-4">
            <div className="space-y-2">
              <Label htmlFor="case-name" className="text-sm font-semibold">
                Test Case Name *
              </Label>
              <Input
                id="case-name"
                value={caseName}
                onChange={(e) => setCaseName(e.target.value)}
                placeholder="e.g., Newton_GEOM_10Steps"
                className="bg-zinc-800 border-zinc-700"
              />
            </div>

            <div className="space-y-2">
              <Label htmlFor="case-description" className="text-sm font-semibold">
                Description <span className="text-zinc-500 font-normal">(optional)</span>
              </Label>
              <Textarea
                id="case-description"
                value={caseDescription}
                onChange={(e) => setCaseDescription(e.target.value)}
                placeholder="e.g., Newton-Raphson method with GEOM nonlinearity"
                className="bg-zinc-800 border-zinc-700 min-h-[80px]"
              />
            </div>

            <div className="p-3 bg-zinc-800/50 border border-zinc-700 rounded-lg">
              <div className="text-xs text-zinc-400 mb-1">Current Configuration:</div>
              <div className="text-sm text-zinc-200">
                {iterationMethod === 'NEWTON' && `🟢 Newton (${newtonItems.length} cases)`}
                {iterationMethod === 'ARC' && `🟣 Arc-Length (${arclenItems.length} cases)`}
                {iterationMethod === 'DISP' && `🟠 Displacement-Control (${dispctItems.length} cases)`}
              </div>
            </div>
          </div>

          <DialogFooter>
            <Button
              variant="outline"
              onClick={() => setShowSaveDialog(false)}
              className="border-zinc-700 hover:bg-zinc-800"
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