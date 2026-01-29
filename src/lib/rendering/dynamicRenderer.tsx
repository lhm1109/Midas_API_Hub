/**
 * Dynamic Schema Renderer
 * YAML 정의에 따라 동적으로 UI를 렌더링합니다.
 */

import React from 'react';
import { Input } from '@/components/ui/input';
import { Label } from '@/components/ui/label';
import { Select, SelectContent, SelectItem, SelectTrigger, SelectValue } from '@/components/ui/select';
import { Textarea } from '@/components/ui/textarea';
import { ChevronDown, ChevronRight } from 'lucide-react';
import type { UIBuilderField } from '@/lib/schema';
import type { BuilderDefinition } from './definitionLoader';
import type { FieldRuntimeStateMap } from '@/lib/schema/fieldRuntimeState';

interface DynamicRendererProps {
  definition: BuilderDefinition;
  schemaFields: UIBuilderField[];
  dynamicFormData: Record<string, any>;
  updateDynamicField: (key: string, value: any) => void;
  expandedObjects: Set<string>;
  toggleObject: (fieldName: string) => void;
  fieldRuntimeStates?: FieldRuntimeStateMap; // 🎯 NEW: Runtime States
}

/**
 * YAML 정의 기반 동적 폼 렌더러
 */
export function DynamicSchemaRenderer({
  definition,
  schemaFields,
  dynamicFormData,
  updateDynamicField,
  expandedObjects,
  toggleObject,
  fieldRuntimeStates
}: DynamicRendererProps) {
  // 컨테이너 스타일 적용
  const containerClassName = definition.formLayout?.fieldContainer?.className || 'space-y-4';

  // 🔥 섹션 헤더의 visibility 계산: 해당 섹션의 다음 필드들 중 visible한 것이 있는지 확인
  const isSectionVisible = (sectionIndex: number, _sectionName: string): boolean => {
    // 섹션 이후의 필드들을 확인 (다음 섹션 헤더 전까지)
    for (let i = sectionIndex + 1; i < schemaFields.length; i++) {
      const field = schemaFields[i];

      // 다음 섹션 헤더를 만나면 중단
      if (field.name.startsWith(definition.sectionHeaders?.detectBy || '__section_')) {
        break;
      }

      // 필드가 visible인지 확인
      if (fieldRuntimeStates && fieldRuntimeStates[field.name]) {
        if (fieldRuntimeStates[field.name].visible) {
          return true; // 하나라도 visible이면 섹션 표시
        }
      } else if (field.visible !== false) {
        return true; // fallback: visible이 false가 아니면 표시
      }
    }

    return false; // 모든 필드가 hidden이면 섹션도 숨김
  };

  return (
    <div className={containerClassName}>
      {schemaFields
        .filter((field, index) => {
          // 🔥 섹션 헤더의 경우: 해당 섹션의 필드 중 visible한 것이 있는지 확인
          if (definition.sectionHeaders?.enabled && field.name.startsWith(definition.sectionHeaders.detectBy || '__section_')) {
            return isSectionVisible(index, field.description || field.name);
          }

          // 🎯 Runtime State 기반 visible 판단 (Single Source of Truth)
          if (fieldRuntimeStates && fieldRuntimeStates[field.name]) {
            return fieldRuntimeStates[field.name].visible;
          }

          // 🔥 Fallback: visible이 false인 필드는 렌더링하지 않음
          // visible이 undefined이면 true로 간주 (섹션 헤더 등)
          return field.visible !== false;
        })
        .map((field) => {
          // 섹션 헤더 감지
          if (definition.sectionHeaders?.enabled && field.name.startsWith(definition.sectionHeaders.detectBy || '__section_')) {
            return renderSectionHeader(field, definition);
          }

          // 일반 필드 렌더링
          return (
            <div key={field.name} className="space-y-2">
              {renderField(field, definition, dynamicFormData, updateDynamicField, expandedObjects, toggleObject, fieldRuntimeStates)}
            </div>
          );
        })}
    </div>
  );
}

/**
 * 섹션 헤더 렌더링
 */
function renderSectionHeader(field: UIBuilderField, definition: BuilderDefinition) {
  const sectionName = field.description || field.name.replace('__section_', '').replace(/__$/, '');
  const style = definition.sectionHeaders?.style || {};

  const containerClass = style.container || 'pt-4 pb-2 border-t-2 border-cyan-800/50 first:pt-0 first:border-t-0';
  const titleClass = style.title || 'text-sm font-semibold text-cyan-400 flex items-center gap-2';
  const icon = style.icon || '📋';

  return (
    <div key={field.name} className={containerClass}>
      <h4 className={titleClass}>
        <span className="text-base">{icon}</span>
        {sectionName}
      </h4>
    </div>
  );
}

/**
 * 필드 렌더링
 */
function renderField(
  field: UIBuilderField,
  definition: BuilderDefinition,
  dynamicFormData: Record<string, any>,
  updateDynamicField: (key: string, value: any) => void,
  expandedObjects: Set<string>,
  toggleObject: (fieldName: string) => void,
  fieldRuntimeStates?: Record<string, any>
): React.ReactNode {
  // Object with children
  if (field.type === 'object' && field.children && field.children.length > 0) {
    return renderObjectField(field, definition, dynamicFormData, updateDynamicField, expandedObjects, toggleObject);
  }

  // 일반 필드
  return renderStandardField(field, definition, dynamicFormData, updateDynamicField, fieldRuntimeStates);
}

/**
 * Object 필드 렌더링 (중첩 필드)
 */
function renderObjectField(
  field: UIBuilderField,
  definition: BuilderDefinition,
  dynamicFormData: Record<string, any>,
  updateDynamicField: (key: string, value: any) => void,
  expandedObjects: Set<string>,
  toggleObject: (fieldName: string) => void
): React.ReactNode {
  const objectStyle = definition.fieldRendering?.object?.style || {};

  // 🔥 oneOf 패턴: 여러 옵션 중 하나 선택
  const isOneOf = field.oneOfOptions && field.oneOfOptions.length > 0;

  return (
    <div className={objectStyle.border || 'border border-zinc-700 rounded-md bg-zinc-900/50'}>
      {/* 헤더 */}
      <div className={objectStyle.header || 'flex items-center gap-2 p-3 bg-zinc-800/50'}>
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
            {field.description || field.name}
            {field.required && <span className="text-red-400">*</span>}
          </Label>
        </button>
        <span className="text-[10px] text-zinc-600 font-mono">{field.type}</span>
      </div>

      {/* 🔥 oneOf 선택 라디오 버튼 */}
      {isOneOf && expandedObjects.has(field.name) && (
        <div className="px-4 pt-4 pb-2 bg-blue-950/20 border-b border-blue-800/30">
          <div className="text-xs text-blue-400 mb-2">Choose one method:</div>
          <div className="space-y-2">
            {field.oneOfOptions!.map((option, idx) => {
              const selectedOption = dynamicFormData[`${field.name}.__selectedOption`] || 0;
              return (
                <label key={idx} className="flex items-center gap-2 cursor-pointer hover:bg-zinc-800/30 p-2 rounded">
                  <input
                    type="radio"
                    name={`${field.name}.__oneOf`}
                    checked={selectedOption === idx}
                    onChange={() => updateDynamicField(`${field.name}.__selectedOption`, idx)}
                    className="w-4 h-4"
                  />
                  <span className="text-sm text-zinc-300">{option}</span>
                </label>
              );
            })}
          </div>
        </div>
      )}

      {/* 자식 필드들 */}
      {expandedObjects.has(field.name) && (
        <div className={objectStyle.content || 'p-4 space-y-3 bg-zinc-900/30'}>
          {field.children!.map((child: any, _idx) => {
            // 🔥 oneOf인 경우: 선택된 옵션의 필드만 표시
            if (isOneOf && child.optionIndex !== undefined) {
              const selectedOption = dynamicFormData[`${field.name}.__selectedOption`] || 0;

              if (child.optionIndex !== selectedOption) {
                return null; // 선택되지 않은 옵션의 필드는 숨김
              }
            }

            return (
              <div key={child.name} className={objectStyle.childBorder || 'space-y-2 pl-4 border-l-2 border-zinc-700'}>
                <Label className="text-xs flex items-center gap-2">
                  {child.description || child.name}
                  {child.required && <span className="text-red-400">*</span>}
                  <span className="text-[10px] text-zinc-600 font-mono ml-auto">{child.type}</span>
                </Label>

                {/* 🔥 child.name이 이미 전체 경로를 포함 (NODE_ELEMS.KEYS) */}
                {renderFieldInput(
                  child,
                  child.name,
                  dynamicFormData[child.name],
                  (value) => updateDynamicField(child.name, value),
                  definition,
                  !dynamicFormData[`${field.name}._enabled`]
                )}
              </div>
            );
          })}
        </div>
      )}
    </div>
  );
}

/**
 * 일반 필드 렌더링
 */
function renderStandardField(
  field: UIBuilderField,
  definition: BuilderDefinition,
  dynamicFormData: Record<string, any>,
  updateDynamicField: (key: string, value: any) => void,
  fieldRuntimeStates?: Record<string, any>
): React.ReactNode {
  const labelStyle = definition.fieldRendering?.standard?.label || {};

  // 🎯 Runtime State에서 requiredNow 확인 (조건부 required 지원)
  const runtimeState = fieldRuntimeStates?.[field.name];
  const isRequired = runtimeState?.requiredNow ?? field.required;

  return (
    <>
      <Label className={labelStyle.className || 'text-sm flex items-center gap-2'}>
        {field.description || field.name}
        {isRequired && labelStyle.showRequired && <span className="text-red-400">*</span>}
        {labelStyle.showType && (
          <span className="text-[10px] text-zinc-600 font-mono ml-auto">{field.type}</span>
        )}
      </Label>

      {/* Hint 표시 */}
      {definition.hintsDisplay?.enabled && field.placeholder && (
        <p className="text-[10px] text-amber-400 italic">
          💡 {field.placeholder}
        </p>
      )}

      {renderFieldInput(field, field.name, dynamicFormData[field.name], (value) => updateDynamicField(field.name, value), definition)}
    </>
  );
}

/**
 * 필드 입력 컴포넌트 렌더링
 */
function renderFieldInput(
  field: UIBuilderField,
  _fieldKey: string,
  value: any,
  onChange: (value: any) => void,
  definition: BuilderDefinition,
  disabled: boolean = false
): React.ReactNode {
  const inputClassName = definition.fieldRendering?.standard?.input?.className || 'bg-zinc-800 border-zinc-700';

  // Enum
  if (field.enum) {
    return (
      <Select
        value={String(value || '')}
        onValueChange={onChange}
        disabled={disabled}
      >
        <SelectTrigger className={inputClassName}>
          <SelectValue placeholder={`Select ${field.description || field.name}`} />
        </SelectTrigger>
        <SelectContent>
          {field.enum.map((option: any) => (
            <SelectItem key={String(option)} value={String(option)}>
              {String(option)}
            </SelectItem>
          ))}
        </SelectContent>
      </Select>
    );
  }

  // Array
  if (field.type === 'array') {
    return (
      <Textarea
        placeholder={field.placeholder || `Enter as JSON array, e.g., [1, 2, 3]`}
        value={typeof value === 'string' ? value : JSON.stringify(value || [])}
        onChange={(e) => {
          try {
            const parsed = JSON.parse(e.target.value);
            onChange(parsed);
          } catch {
            onChange(e.target.value);
          }
        }}
        className={`${inputClassName} font-mono text-xs`}
        disabled={disabled}
      />
    );
  }

  // Object
  if (field.type === 'object') {
    return (
      <Textarea
        placeholder={`Enter as JSON object, e.g., {"key": "value"}`}
        value={typeof value === 'string' ? value : JSON.stringify(value || {}, null, 2)}
        onChange={(e) => {
          try {
            const parsed = JSON.parse(e.target.value);
            onChange(parsed);
          } catch {
            onChange(e.target.value);
          }
        }}
        className={`${inputClassName} font-mono text-xs min-h-[100px]`}
        disabled={disabled}
      />
    );
  }

  // Number
  if (field.type === 'number' || field.type === 'integer') {
    return (
      <Input
        type="number"
        value={value || ''}
        onChange={(e) => onChange(parseFloat(e.target.value) || 0)}
        className={inputClassName}
        placeholder={field.default?.toString() || '0'}
        min={(field as any).minimum}
        max={(field as any).maximum}
        disabled={disabled}
      />
    );
  }

  // Boolean
  if (field.type === 'boolean') {
    return (
      <div className="flex items-center gap-2">
        <input
          type="checkbox"
          checked={value || false}
          onChange={(e) => onChange(e.target.checked)}
          className="w-4 h-4"
          disabled={disabled}
        />
        <span className="text-sm text-zinc-400">{value ? 'true' : 'false'}</span>
      </div>
    );
  }

  // String (default)
  return (
    <Input
      value={value || ''}
      onChange={(e) => onChange(e.target.value)}
      className={inputClassName}
      placeholder={field.default || `Enter ${field.description || field.name}`}
      disabled={disabled}
    />
  );
}

