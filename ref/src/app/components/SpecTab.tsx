import { ScrollArea } from './ui/scroll-area';
import { ResizableHandle, ResizablePanel, ResizablePanelGroup } from './ui/resizable';
import React, { useState, useEffect } from 'react';
import { ChevronRight, ChevronDown, Send, Save } from 'lucide-react';
import { Button } from './ui/button';
import { apiSpecs } from '../data/apiSpecs';
import { useAppStore } from '../store/useAppStore';
import type { ManualData } from '../store/useAppStore';

interface SpecTabProps {
  endpoint: {
    id: string;
    name: string;
    method: string;
    path: string;
  };
}

export function SpecTab({ endpoint }: SpecTabProps) {
  const { setManualData, manualData } = useAppStore();
  const spec = apiSpecs[endpoint.id] || {
    title: endpoint.name,
    description: 'API endpoint specification',
    parameters: [],
    jsonSchema: {},
  };

  // Track which parameters are expanded
  const [expandedParams, setExpandedParams] = useState<Set<number>>(new Set());
  
  // 🎯 Schema View Toggle: 'original' | 'enhanced'
  const [schemaView, setSchemaView] = useState<'original' | 'enhanced'>('original');
  
  // 🎯 Editable Schema State
  const [editableSchema, setEditableSchema] = useState<string>('');
  const [isSchemaModified, setIsSchemaModified] = useState(false);
  
  // Initialize editable schema
  useEffect(() => {
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
  }, [schemaView, spec]);
  
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
  
  // 🎯 Enhanced Schema에서 Parameters를 자동으로 파싱하는 함수
  const parseEnhancedSchemaToParameters = (enhancedSchema: any): any[] => {
    if (!enhancedSchema) return [];
    
    const params: any[] = [];
    let paramNo = 1;
    let currentSection = '';
    
    // Get the root schema object (e.g., "NLCT")
    const rootKey = Object.keys(enhancedSchema).find(key => key !== '$schema');
    if (!rootKey) return [];
    
    const schema = enhancedSchema[rootKey];
    const properties = schema.properties || {};
    const required = schema.required || [];
    
    // 🎯 조건부 필드를 모두 표시하되, 섹션으로 구분
    const processedFields = new Set<string>();
    
    // 1. 공통 필드 먼저 처리 (x-condition이 없는 필드)
    Object.keys(properties).forEach(key => {
      const prop = properties[key];
      if (!prop['x-condition'] && !processedFields.has(key)) {
        processedFields.add(key);
        
        const section = prop['x-section'] || '';
        if (section && section !== currentSection) {
          params.push({ section });
          currentSection = section;
        }
        
        params.push(createParameter(key, prop, required, paramNo++));
      }
    });
    
    // 2. 조건부 필드를 섹션별로 처리
    const sections = ['Iteration Method: Newton-Raphson', 'Iteration Method: Arc-Length', 'Iteration Method: Displacement-Control'];
    sections.forEach(sectionName => {
      const sectionFields = Object.keys(properties).filter(key => {
        const prop = properties[key];
        return prop['x-section'] === sectionName && !processedFields.has(key);
      });
      
      if (sectionFields.length > 0) {
        params.push({ section: sectionName });
        currentSection = sectionName;
        
        sectionFields.forEach(key => {
          processedFields.add(key);
          params.push(createParameter(key, properties[key], required, paramNo++));
        });
      }
    });
    
    return params;
    
    // Helper function to create parameter
    function createParameter(key: string, prop: any, required: string[], no: number) {
      // Determine type
      let type = prop.type || 'string';
      if (type === 'integer') type = 'Integer';
      else if (type === 'number') type = 'Number';
      else if (type === 'string') type = 'String';
      else if (type === 'boolean') type = 'Boolean';
      else if (type === 'array') {
        const itemType = prop.items?.type || 'object';
        type = itemType === 'object' ? 'Array[Object]' : `Array[${itemType}]`;
      }
      
      // Determine required/optional
      const isRequired = required.includes(key) ? 'Required' : 'Optional';
      
      // Get default value
      let defaultValue = '-';
      if (prop.default !== undefined) {
        if (typeof prop.default === 'string') {
          defaultValue = `"${prop.default}"`;
        } else if (typeof prop.default === 'boolean') {
          defaultValue = prop.default.toString();
        } else if (Array.isArray(prop.default)) {
          defaultValue = JSON.stringify(prop.default);
        } else {
          defaultValue = String(prop.default);
        }
      }
      
      // Get options from x-options or enum
      const options = prop['x-options'] || prop.enum || [];
      
      // Build parameter object
      let description = prop.description || key;
      if (prop['x-note']) {
        description += `\n • ${prop['x-note']}`;
      }
      
      const param: any = {
        no,
        name: key,
        description,
        type,
        default: defaultValue,
        required: isRequired,
      };
      
      if (options.length > 0) {
        param.options = options.map((opt: any) => ` • ${opt}`);
      }
      
      // Handle array items (children)
      if (prop.type === 'array' && prop.items?.type === 'object') {
        const itemProps = prop.items.properties || {};
        const itemRequired = prop.items.required || [];
        const children: any[] = [];
        
        Object.keys(itemProps).forEach((childKey) => {
          const childProp = itemProps[childKey];
          let childType = childProp.type || 'string';
          if (childType === 'integer') childType = 'Integer';
          else if (childType === 'number') childType = 'Number';
          else if (childType === 'string') childType = 'String';
          else if (childType === 'array') childType = 'Array[Number]';
          
          const childRequired = itemRequired.includes(childKey) ? 'Required' : 'Optional';
          let childDefault = '-';
          if (childProp.default !== undefined) {
            if (typeof childProp.default === 'string') {
              childDefault = `"${childProp.default}"`;
            } else if (Array.isArray(childProp.default)) {
              childDefault = JSON.stringify(childProp.default);
            } else {
              childDefault = String(childProp.default);
            }
          }
          
          let childDesc = childProp.description || childKey;
          if (childProp['x-note']) {
            childDesc += `\n • ${childProp['x-note']}`;
          }
          
          const childOptions = childProp['x-options'] || childProp.enum || [];
          
          const child: any = {
            name: childKey,
            description: childDesc,
            type: childType,
            default: childDefault,
            required: childRequired,
          };
          
          if (childOptions.length > 0) {
            child.options = childOptions.map((opt: any) => ` • ${opt}`);
          }
          
          children.push(child);
        });
        
        if (children.length > 0) {
          param.children = children;
        }
      }
      
      return param;
    }
  };
  
  // 🎯 Enhanced가 있으면 파싱, 없으면 기존 parameters 사용
  const displayParameters = spec.jsonSchemaEnhanced 
    ? parseEnhancedSchemaToParameters(spec.jsonSchemaEnhanced)
    : spec.parameters;

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

  // 🎯 Send to Manual 함수
  const handleSendToManual = () => {
    // JSON Schema를 색상 있는 HTML로 변환
    const formatJsonSchemaToHTML = (schema: any): string => {
      const jsonStr = JSON.stringify(schema, null, 2);
      return jsonStr
        .split('\\n')
        .map(line => {
          // 키-값 분리
          const keyMatch = line.match(/"([^"]+)":\s*(.+)/);
          if (keyMatch) {
            const key = keyMatch[1];
            let value = keyMatch[2].replace(/,$/, ''); // 콤마 제거
            
            // 값의 타입에 따라 색상 변경
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

    // Specifications 테이블을 HTML로 변환
    const generateSpecificationsTable = (): string => {
      // 🎯 Enhanced가 있으면 파싱된 parameters 사용
      const params = spec.jsonSchemaEnhanced 
        ? parseEnhancedSchemaToParameters(spec.jsonSchemaEnhanced)
        : spec.parameters;
        
      if (!params || params.length === 0) {
        return '<p>No parameters available</p>';
      }

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
        // 섹션 행
        if (param.section) {
          tableHTML += `
<tr>
<td style="background-color: #e6fcff; padding: 10px 5px 10px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;" colspan="7">
<p><span style="color: #4c9aff;">${param.section}</span></p>
</td>
</tr>`;
        }

        // 파라미터 행
        const rowspan = param.children ? param.children.length + 1 : 1;
        tableHTML += `
<tr>
<td style="padding: 10px 5px 10px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;" ${param.children ? `rowspan="${rowspan}"` : ''}>
<p style="text-align: center;">${param.no}</p>
</td>
<td style="padding: 10px 5px 10px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;" colspan="2">
<p>${param.description || param.name}</p>
${param.options ? param.options.map((opt: string) => `<p> • ${opt}</p>`).join('') : ''}
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

        // 자식 파라미터
        if (param.children) {
          param.children.forEach((child: any, idx: number) => {
            tableHTML += `
<tr>
<td style="padding: 10px 5px 10px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;">
<p style="text-align: center;">(${idx + 1})</p>
</td>
<td style="padding: 10px 5px 10px 5px; word-wrap: break-word; overflow-wrap: break-word; word-break: break-word;">
<p>${child.description || child.name}</p>
${child.options ? child.options.map((opt: string) => `<p> • ${opt}</p>`).join('') : ''}
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

    // 🎯 Enhanced Schema가 있으면 우선 사용, 없으면 Original 사용
    const prioritySchema = spec.jsonSchemaEnhanced || spec.jsonSchema;
    const originalSchemaHTML = formatJsonSchemaToHTML(spec.jsonSchema);
    const enhancedSchemaHTML = spec.jsonSchemaEnhanced 
      ? formatJsonSchemaToHTML(spec.jsonSchemaEnhanced) 
      : null;

    const newManualData: ManualData = {
      title: spec.title,
      category: endpoint.method,
      inputUri: spec.uri || endpoint.path,
      activeMethods: spec.methods ? spec.methods.join(', ') : endpoint.method,
      jsonSchema: enhancedSchemaHTML || originalSchemaHTML, // Enhanced 우선
      jsonSchemaOriginal: originalSchemaHTML,
      jsonSchemaEnhanced: enhancedSchemaHTML || undefined,
      examples: manualData?.examples || [],
      specifications: generateSpecificationsTable(),
    };

    setManualData(newManualData);
    
    const schemaType = spec.jsonSchemaEnhanced ? 'Enhanced' : 'Original';
    alert(`✅ Spec data sent to Manual tab!\n📋 Schema Type: ${schemaType}`);
  };

  return (
    <ResizablePanelGroup direction="horizontal" className="h-full w-full">
      {/* Left Pane - Visual Schema Grid */}
      <ResizablePanel defaultSize={50} minSize={30}>
        <div className="h-full flex flex-col bg-zinc-950 overflow-hidden">
          <div className="p-4 border-b border-zinc-800 bg-zinc-900 flex-shrink-0 flex items-center justify-between">
            <div>
              <h3 className="text-sm mb-1">Visual Schema Grid</h3>
              <p className="text-xs text-zinc-500">API specification and parameters</p>
            </div>
            <Button
              onClick={handleSendToManual}
              size="sm"
              className="bg-blue-600 hover:bg-blue-500 h-8"
            >
              <Send className="w-3 h-3 mr-2" />
              Send to Manual
            </Button>
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
                          <th className="text-left p-3 border-b border-zinc-800">Field Name</th>
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
                                <td colSpan={5} className="p-2 text-cyan-400 font-semibold text-xs">
                                  {param.section}
                                </td>
                              </tr>
                            );
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
                                  <div className="font-mono text-blue-400">{param.name}</div>
                                  {param.description && (
                                    <div className="text-xs text-zinc-500 mt-0.5">{param.description}</div>
                                  )}
                                </div>
                                {param.options && (
                                  <div className="text-xs text-zinc-600 mt-1">
                                    {param.options.map((opt: string, optIdx: number) => (
                                      <div key={`${param.name}-opt-${optIdx}`}>• {opt}</div>
                                    ))}
                                  </div>
                                )}
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
                              rows.push(
                                <tr key={`child-${param.no}-${childIdx}`} className="border-b border-zinc-800 bg-zinc-900/50">
                                  <td className="p-3"></td>
                                  <td className="p-3 pl-8">
                                    <div className="font-mono text-amber-400">└─ {child.name}</div>
                                    {child.options && (
                                      <div className="text-xs text-zinc-600 mt-1">
                                        {child.options.map((opt: string, optIdx: number) => (
                                          <div key={`${child.name}-opt-${optIdx}`}>• {opt}</div>
                                        ))}
                                      </div>
                                    )}
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
        </div>
      </ResizablePanel>

      <ResizableHandle />

      {/* Right Pane - Raw Definition */}
      <ResizablePanel defaultSize={50} minSize={30}>
        <div className="h-full flex flex-col bg-zinc-950 overflow-hidden">
          <div className="p-4 border-b border-zinc-800 bg-zinc-900 flex-shrink-0">
            <div className="flex items-center justify-between mb-2">
              <h3 className="text-sm">JSON Schema</h3>
              
              {/* 🎯 Schema Toggle */}
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

          <ScrollArea className="flex-1 h-0">
            <div className="relative h-full">
              {/* 🎯 Line numbers */}
              <div className="absolute left-0 top-0 bottom-0 w-12 bg-zinc-900 border-r border-zinc-800 text-right text-xs text-zinc-600 select-none overflow-hidden">
                {editableSchema.split('\n').map((_, idx) => (
                  <div key={idx} className="px-2 leading-6">
                    {idx + 1}
                  </div>
                ))}
              </div>
              
              {/* 🎯 Editable Code Area */}
              <textarea
                className="w-full h-full bg-zinc-950 text-zinc-100 font-mono text-sm leading-6 pl-14 pr-4 py-4 resize-none border-none outline-none focus:ring-2 focus:ring-blue-500/30 rounded-none"
                style={{
                  minHeight: '800px',
                  fontFamily: 'ui-monospace, SFMono-Regular, "SF Mono", Menlo, Consolas, "Liberation Mono", monospace',
                  tabSize: 2,
                }}
                value={editableSchema}
                onChange={(e) => handleSchemaChange(e.target.value)}
                spellCheck={false}
                placeholder="Enter JSON schema..."
              />
              
              {/* 🎯 Modified Indicator */}
              {isSchemaModified && (
                <div className="absolute top-2 right-2 px-2 py-1 bg-orange-600/20 border border-orange-600/50 rounded text-xs text-orange-400">
                  Modified
                </div>
              )}
            </div>
          </ScrollArea>
          
          {/* 🎯 Footer with Save Button */}
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
                onClick={() => {
                  try {
                    // Validate JSON
                    const parsedSchema = JSON.parse(editableSchema);
                    
                    // Update the spec in memory
                    if (schemaView === 'original') {
                      spec.jsonSchema = parsedSchema;
                    } else {
                      spec.jsonSchemaEnhanced = parsedSchema;
                    }
                    
                    setIsSchemaModified(false);
                    alert(`✅ ${schemaView === 'original' ? 'Original' : 'Enhanced'} Schema saved successfully!\n\n⚠️ Note: Changes are saved in memory only.\nTo persist changes, update apiSpecs.ts file.`);
                  } catch (error) {
                    alert(`❌ Invalid JSON!\n\nPlease fix the syntax errors before saving.\n\nError: ${error instanceof Error ? error.message : 'Unknown error'}`);
                  }
                }}
                size="sm"
                disabled={!isSchemaModified}
                className="h-8 text-xs bg-blue-600 hover:bg-blue-500 disabled:opacity-50 disabled:cursor-not-allowed"
              >
                <Save className="w-3 h-3 mr-2" />
                Save Schema
              </Button>
            </div>
          </div>
        </div>
      </ResizablePanel>
    </ResizablePanelGroup>
  );
}