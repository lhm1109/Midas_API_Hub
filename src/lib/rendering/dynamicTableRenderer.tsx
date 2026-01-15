/**
 * Dynamic Table Renderer (Spec Tab)
 * YAML 정의에 따라 동적으로 테이블을 렌더링합니다.
 */

import { ChevronRight, ChevronDown } from 'lucide-react';
import type { TableDefinition } from './definitionLoader';

interface DynamicTableRendererProps {
  definition: TableDefinition;
  parameters: any[];
  expandedParams: Set<number>;
  toggleParam: (no: number) => void;
}

/**
 * YAML 정의 기반 동적 테이블 렌더러
 */
export function DynamicTableRenderer({
  definition,
  parameters,
  expandedParams,
  toggleParam
}: DynamicTableRendererProps) {
  const tableStyle = definition.styling?.table || {};
  const headerStyle = definition.styling?.header || {};
  const bodyStyle = definition.styling?.body || {};
  
  return (
    <div className={`border rounded-lg overflow-hidden ${tableStyle.borderColor || 'border-zinc-800'}`}>
      <table className={`w-full text-sm ${tableStyle.borderCollapse ? 'border-collapse' : ''}`}>
        {/* 테이블 헤더 */}
        <thead className={headerStyle.background || 'bg-zinc-900'}>
          <tr>
            {definition.tableStructure?.columns?.map((col: any) => (
              <th
                key={col.id}
                className={`text-left p-3 border-b ${tableStyle.borderColor || 'border-zinc-800'}`}
                style={{ width: col.width }}
              >
                {col.header}
              </th>
            ))}
          </tr>
        </thead>
        
        {/* 테이블 바디 */}
        <tbody className={bodyStyle.fontSize || 'text-sm'}>
          {parameters.map((param: any) => {
            const rows = [];
            
            // 섹션 헤더
            if (param.section) {
              rows.push(renderSectionHeader(param, definition));
              
              // 섹션만 있는 행이면 필드 행 추가 안함
              if (!param.name || !param.type) {
                return rows;
              }
            }
            
            // 메인 파라미터 행
            rows.push(renderParameterRow(param, definition, expandedParams, toggleParam));
            
            // 🔥 자식 행들 - expandedParams 확인 후 표시 (아코디언)
            if (param.children && param.children.length > 0 && expandedParams.has(param.no)) {
              param.children.forEach((child: any, childIdx: number) => {
                // 🔥 oneOf 섹션 헤더 (type === 'section-header')
                if (child.type === 'section-header' || child.section) {
                  rows.push(renderNestedSectionHeader(child, definition, param.no, childIdx));
                  return;
                }
                
                // 자식 행
                rows.push(renderChildRow(child, definition, param.no, childIdx));
              });
            }
            
            return rows;
          })}
        </tbody>
      </table>
    </div>
  );
}

/**
 * 섹션 헤더 렌더링
 */
function renderSectionHeader(param: any, definition: TableDefinition) {
  const sectionStyle = definition.sectionHeaders?.style || {};
  
  return (
    <tr key={`section-${param.no}`} className={sectionStyle.background || 'bg-cyan-950/30 border-b border-zinc-800'}>
      <td colSpan={definition.sectionHeaders?.colspan || 6} className={`p-2 ${sectionStyle.textColor || 'text-cyan-400'} ${sectionStyle.fontWeight || 'font-semibold'} ${sectionStyle.fontSize || 'text-xs'}`}>
        {param.section}
      </td>
    </tr>
  );
}

/**
 * 중첩 섹션 헤더 렌더링
 */
function renderNestedSectionHeader(child: any, definition: TableDefinition, parentNo: number, childIdx: number) {
  const nestedStyle = definition.nestedFields?.nestedSectionHeader?.style || {};
  
  // 고유한 key 생성: 부모 no + 자식 인덱스 + 섹션명
  const uniqueKey = `child-section-${parentNo}-${childIdx}-${child.section || child.name || ''}`;
  
  return (
    <tr key={uniqueKey} className={nestedStyle.background || 'bg-blue-950/30 border-b border-zinc-800'}>
      <td colSpan={6} className={`p-2 ${nestedStyle.textColor || 'text-blue-400'} font-semibold text-xs pl-8`}>
        {child.section}
      </td>
    </tr>
  );
}

/**
 * 파라미터 행 렌더링
 */
function renderParameterRow(
  param: any,
  definition: TableDefinition,
  expandedParams: Set<number>,
  toggleParam: (no: number) => void
) {
  const rowStyle = definition.rowRendering?.standard || {};
  const hoverClass = rowStyle.hover ? (rowStyle.hoverStyle || 'hover:bg-zinc-800/30') : '';
  
  return (
    <tr key={`param-${param.no}`} className={`border-b border-zinc-800 ${hoverClass}`}>
      {/* No. */}
      <td className="p-3 text-zinc-400">{param.no}</td>
      
      {/* Description */}
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
          {param.description && renderDescription(param.description)}
          {param.options && param.options.length > 0 && (
            <div className="mt-1 space-y-0.5">
              {param.options.map((opt: string, optIdx: number) => (
                <div
                  key={`${param.name}-opt-${optIdx}`}
                  className="text-zinc-300"
                  dangerouslySetInnerHTML={{ __html: opt.replace(/• /g, '<span class="text-zinc-400">• </span>') }}
                />
              ))}
            </div>
          )}
        </div>
      </td>
      
      {/* Key */}
      <td className="p-3">
        <code className="font-mono text-blue-400">"{param.name}"</code>
      </td>
      
      {/* Type */}
      <td className="p-3 text-zinc-400">{param.type}</td>
      
      {/* Default */}
      <td className="p-3 text-zinc-500 font-mono text-xs">{param.default}</td>
      
      {/* Required */}
      <td className="p-3">
        {renderRequired(param.required, definition)}
      </td>
    </tr>
  );
}

/**
 * 자식 행 렌더링
 */
function renderChildRow(child: any, definition: TableDefinition, parentNo: number, childIdx: number) {
  const nestedStyle = definition.nestedFields?.style || {};
  
  // 고유한 key 생성: 부모 no + 자식 인덱스 + 자식 이름
  const uniqueKey = `child-${parentNo}-${childIdx}-${child.name || child.no || ''}`;
  
  return (
    <tr key={uniqueKey} className="border-b border-zinc-800 bg-zinc-900/50">
      <td className="p-3 text-zinc-500 text-center">{child.no}</td>
      <td className={`p-3 ${nestedStyle.indentation || 'pl-8'}`}>
        {child.description && renderDescription(child.description)}
        {child.options && child.options.length > 0 && (
          <div className="mt-1 space-y-0.5">
            {child.options.map((opt: string, optIdx: number) => (
              <div
                key={`${child.name}-opt-${optIdx}`}
                className="text-zinc-300"
                dangerouslySetInnerHTML={{ __html: opt.replace(/• /g, '<span class="text-zinc-400">• </span>') }}
              />
            ))}
          </div>
        )}
      </td>
      <td className="p-3">
        <code className={`font-mono ${nestedStyle.keyColor || 'text-amber-400'}`}>"{child.name}"</code>
      </td>
      <td className="p-3 text-zinc-400">{child.type}</td>
      <td className="p-3 text-zinc-500 font-mono text-xs">{child.default}</td>
      <td className="p-3">
        {renderRequired(child.required, definition)}
      </td>
    </tr>
  );
}

/**
 * Description 렌더링 (마크다운 지원)
 */
function renderDescription(description: string) {
  return (
    <div
      className="text-zinc-300 [&_span]:text-zinc-400 [&_strong]:text-zinc-300 [&_strong]:font-semibold"
      dangerouslySetInnerHTML={{
        __html: description
          .replace(/\n/g, '<br>')
          .replace(/• /g, '<span>• </span>')
          .replace(/- /g, '<span>- </span>')
          .replace(/\*\*([^*]+)\*\*/g, '<strong>$1</strong>')
      }}
    />
  );
}

/**
 * Required 컬럼 렌더링
 */
function renderRequired(required: string, definition: TableDefinition) {
  const requiredConfig = definition.rowRendering?.required || {};
  
  if (requiredConfig.simpleFormat) {
    // Simple format: "Required" | "Optional"
    if (required === 'Required') {
      return (
        <span className={requiredConfig.badge?.required?.className || 'px-2 py-0.5 text-xs rounded bg-red-600/20 text-red-400'}>
          {requiredConfig.badge?.required?.text || 'Required'}
        </span>
      );
    } else {
      return (
        <span className={requiredConfig.badge?.optional?.className || 'px-2 py-0.5 text-xs rounded bg-zinc-700/50 text-zinc-400'}>
          {requiredConfig.badge?.optional?.text || 'Optional'}
        </span>
      );
    }
  }
  
  // Complex format (TYPE별 다른 경우)
  return <div dangerouslySetInnerHTML={{ __html: required }} />;
}

