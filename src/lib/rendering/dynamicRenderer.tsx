/**
 * Dynamic Schema Renderer
 * YAML ?뺤쓽???곕씪 ?숈쟻?쇰줈 UI瑜??뚮뜑留곹빀?덈떎.
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
  fieldRuntimeStates?: FieldRuntimeStateMap; // ?렞 NEW: Runtime States
}

function resolveChildFieldKey(parentFieldName: string, childFieldName: string): string {
  if (!childFieldName) return childFieldName;
  if (childFieldName === parentFieldName || childFieldName.startsWith(`${parentFieldName}.`)) {
    return childFieldName;
  }
  return `${parentFieldName}.${childFieldName}`;
}

/**
 * YAML ?뺤쓽 湲곕컲 ?숈쟻 ???뚮뜑??
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
  // 而⑦뀒?대꼫 ?ㅽ????곸슜
  const containerClassName = definition.formLayout?.fieldContainer?.className || 'space-y-4';

  // ?뵦 ?뱀뀡 ?ㅻ뜑??visibility 怨꾩궛: ?대떦 ?뱀뀡???ㅼ쓬 ?꾨뱶??以?visible??寃껋씠 ?덈뒗吏 ?뺤씤
  const isSectionVisible = (sectionIndex: number, _sectionName: string): boolean => {
    // ?뱀뀡 ?댄썑???꾨뱶?ㅼ쓣 ?뺤씤 (?ㅼ쓬 ?뱀뀡 ?ㅻ뜑 ?꾧퉴吏)
    for (let i = sectionIndex + 1; i < schemaFields.length; i++) {
      const field = schemaFields[i];

      // ?ㅼ쓬 ?뱀뀡 ?ㅻ뜑瑜?留뚮굹硫?以묐떒
      if (field.name.startsWith(definition.sectionHeaders?.detectBy || '__section_')) {
        break;
      }

      // ?꾨뱶媛 visible?몄? ?뺤씤
      if (fieldRuntimeStates && fieldRuntimeStates[field.name]) {
        if (fieldRuntimeStates[field.name].visible) {
          return true; // ?섎굹?쇰룄 visible?대㈃ ?뱀뀡 ?쒖떆
        }
      } else if (field.visible !== false) {
        return true; // fallback: visible??false媛 ?꾨땲硫??쒖떆
      }
    }

    return false; // 紐⑤뱺 ?꾨뱶媛 hidden?대㈃ ?뱀뀡???④?
  };

  return (
    <div className={containerClassName}>
      {schemaFields
        .filter((field, index) => {
          // ?뵦 ?뱀뀡 ?ㅻ뜑??寃쎌슦: ?대떦 ?뱀뀡???꾨뱶 以?visible??寃껋씠 ?덈뒗吏 ?뺤씤
          if (definition.sectionHeaders?.enabled && field.name.startsWith(definition.sectionHeaders.detectBy || '__section_')) {
            return isSectionVisible(index, field.description || field.name);
          }

          // ?렞 Runtime State 湲곕컲 visible ?먮떒 (Single Source of Truth)
          if (fieldRuntimeStates && fieldRuntimeStates[field.name]) {
            return fieldRuntimeStates[field.name].visible;
          }

          // ?뵦 Fallback: visible??false???꾨뱶???뚮뜑留곹븯吏 ?딆쓬
          // visible??undefined?대㈃ true濡?媛꾩＜ (?뱀뀡 ?ㅻ뜑 ??
          return field.visible !== false;
        })
        .map((field) => {
          // ?뱀뀡 ?ㅻ뜑 媛먯?
          if (definition.sectionHeaders?.enabled && field.name.startsWith(definition.sectionHeaders.detectBy || '__section_')) {
            return renderSectionHeader(field, definition);
          }

          // ?쇰컲 ?꾨뱶 ?뚮뜑留?
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
 * ?뱀뀡 ?ㅻ뜑 ?뚮뜑留?
 */
function renderSectionHeader(field: UIBuilderField, definition: BuilderDefinition) {
  const sectionName = field.description || field.name.replace('__section_', '').replace(/__$/, '');
  const style = definition.sectionHeaders?.style || {};

  const containerClass = style.container || 'pt-4 pb-2 border-t-2 border-cyan-800/50 first:pt-0 first:border-t-0';
  const titleClass = style.title || 'text-sm font-semibold text-cyan-400 flex items-center gap-2';
  const icon = style.icon || '?뱥';

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
 * ?꾨뱶 ?뚮뜑留?
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
  const isGeneralObjectPlaceholder = (() => {
    if (field.type !== 'object') return false;
    if (field.children && field.children.length > 0) return false;

    const normalizedName = (field.name || '').trim().toLowerCase();
    const normalizedDesc = (field.description || '').trim().toLowerCase();
    const isGeneralField = normalizedName === 'general' || normalizedDesc === 'general';
    if (!isGeneralField) return false;

    const value = dynamicFormData[field.name];
    if (value === undefined || value === null || value === '' || value === '{}') return true;
    if (typeof value === 'object' && !Array.isArray(value) && Object.keys(value).length === 0) return true;
    return false;
  })();

  if (isGeneralObjectPlaceholder) {
    return null;
  }

  // Object with children
  if (field.type === 'object' && field.children && field.children.length > 0) {
    return renderObjectField(field, definition, dynamicFormData, updateDynamicField, expandedObjects, toggleObject, fieldRuntimeStates);
  }

  // ?뵦 Array with children (items.type = object with properties)
  if (field.type === 'array' && field.children && field.children.length > 0) {
    return renderArrayField(field, definition, dynamicFormData, updateDynamicField, expandedObjects, toggleObject);
  }

  // ?쇰컲 ?꾨뱶
  return renderStandardField(field, definition, dynamicFormData, updateDynamicField, fieldRuntimeStates);
}

/**
 * Object ?꾨뱶 ?뚮뜑留?(以묒꺽 ?꾨뱶)
 */
function renderObjectField(
  field: UIBuilderField,
  definition: BuilderDefinition,
  dynamicFormData: Record<string, any>,
  updateDynamicField: (key: string, value: any) => void,
  expandedObjects: Set<string>,
  toggleObject: (fieldName: string) => void,
  fieldRuntimeStates?: Record<string, any>
): React.ReactNode {
  const objectStyle = definition.fieldRendering?.object?.style || {};
  const isEnabled = Boolean(dynamicFormData[`${field.name}._enabled`]);

  // ?뵦 oneOf ?⑦꽩: ?щ윭 ?듭뀡 以??섎굹 ?좏깮
  const isOneOf = field.oneOfOptions && field.oneOfOptions.length > 0;

  return (
    <div className={objectStyle.border || 'border border-zinc-700 rounded-md bg-zinc-900/50'}>
      {/* ?ㅻ뜑 */}
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

      {/* ?뵦 oneOf ?좏깮 ?쇰뵒??踰꾪듉 */}
      {isEnabled && isOneOf && expandedObjects.has(field.name) && (
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

      {/* ?먯떇 ?꾨뱶??*/}
      {isEnabled && expandedObjects.has(field.name) && (
        <div className={objectStyle.content || 'p-4 space-y-3 bg-zinc-900/30'}>
          {field.children!.map((child: any, _idx) => {
            const childKey = resolveChildFieldKey(field.name, child.name);
            const childRuntime = fieldRuntimeStates?.[childKey] ?? fieldRuntimeStates?.[child.name];
            if (childRuntime && childRuntime.visible === false) return null;
            // oneOf option filter
            if (isOneOf && child.optionIndex !== undefined) {
              const selectedOption = dynamicFormData[`${field.name}.__selectedOption`] || 0;

              if (child.optionIndex !== selectedOption) {
                return null;
              }
            }

            if ((child.type === 'object' || child.type === 'array') && child.children && child.children.length > 0) {
              return (
                <div key={child.name} className={objectStyle.childBorder || 'space-y-2 pl-4 border-l-2 border-zinc-700'}>
                  {renderField(
                    child,
                    definition,
                    dynamicFormData,
                    updateDynamicField,
                    expandedObjects,
                    toggleObject,
                    fieldRuntimeStates
                  )}
                </div>
              );
            }

            return (
              <div key={child.name} className={objectStyle.childBorder || 'space-y-2 pl-4 border-l-2 border-zinc-700'}>
                <Label className="text-xs flex items-center gap-2">
                  {child.description || child.name}
                  {(childRuntime?.requiredNow ?? child.required) && <span className="text-red-400">*</span>}
                  <span className="text-[10px] text-zinc-600 font-mono ml-auto">{child.type}</span>
                </Label>

                {/* ?뵦 child.name???대? ?꾩껜 寃쎈줈瑜??ы븿 (NODE_ELEMS.KEYS) */}
                {renderFieldInput(
                  child,
                  childKey,
                  dynamicFormData[childKey] ?? dynamicFormData[child.name],
                  (value) => updateDynamicField(childKey, value),
                  definition,
                  false,
                  dynamicFormData
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
 * ?뵦 Array ?꾨뱶 ?뚮뜑留?(items.type = object)
 * REDUCTION_DATA泥섎읆 諛곗뿴 ?대???媛앹껜媛 ?덈뒗 寃쎌슦 泥섎━
 */
function renderArrayField(
  field: UIBuilderField,
  definition: BuilderDefinition,
  dynamicFormData: Record<string, any>,
  updateDynamicField: (key: string, value: any) => void,
  expandedObjects: Set<string>,
  toggleObject: (fieldName: string) => void
): React.ReactNode {
  const objectStyle = definition.fieldRendering?.object?.style || {};

  // ?꾩옱 諛곗뿴 ?곗씠??媛?몄삤湲?
  const arrayData = dynamicFormData[field.name] || [];
  const isExpanded = expandedObjects.has(field.name);

  // ???꾩씠??異붽?
  const addItem = () => {
    const newItem: Record<string, any> = {};
    // ?먯떇 ?꾨뱶?ㅼ쓽 湲곕낯媛믪쑝濡?珥덇린??
    field.children!.forEach(child => {
      if ((child as any).type === 'section-header') return;

      // ?뵦 x-required-when ?먮뒗 x-optional-when 議곌굔 泥댄겕
      const condition = (child as any)['x-required-when'] || (child as any)['x-optional-when'];
      if (condition) {
        // ?뵦 FIX: ???鍮꾧탳 臾몄젣 ?닿껐
        const shouldInclude = Object.entries(condition).every(([key, expectedValue]) => {
          const actualValue = dynamicFormData[key];
          if (typeof expectedValue === 'number') {
            return Number(actualValue) === expectedValue;
          }
          return actualValue === expectedValue;
        });
        if (!shouldInclude) return; // 議곌굔 遺덉땐議????꾨뱶 異붽? ?덊븿
      }

      const childName = child.name.split('.').pop() || child.name;
      newItem[childName] = child.default ?? (child.type === 'number' || child.type === 'integer' ? 0 : '');
    });
    updateDynamicField(field.name, [...arrayData, newItem]);
  };

  // ?꾩씠????젣
  const removeItem = (index: number) => {
    const newArray = [...arrayData];
    newArray.splice(index, 1);
    updateDynamicField(field.name, newArray);
  };

  // ?꾩씠???꾨뱶 ?낅뜲?댄듃
  const updateItemField = (index: number, childKey: string, value: any) => {
    const newArray = [...arrayData];
    if (!newArray[index]) {
      newArray[index] = {};
    }
    newArray[index][childKey] = value;
    updateDynamicField(field.name, newArray);
  };

  return (
    <div className={objectStyle.border || 'border border-green-700 rounded-md bg-zinc-900/50'}>
      {/* ?ㅻ뜑 */}
      <div className={objectStyle.header || 'flex items-center gap-2 p-3 bg-green-900/30'}>
        <button
          onClick={() => toggleObject(field.name)}
          className="flex-1 flex items-center gap-2 text-left hover:text-white transition-colors"
        >
          {isExpanded ? (
            <ChevronDown className="w-4 h-4 text-green-400" />
          ) : (
            <ChevronRight className="w-4 h-4 text-green-400" />
          )}
          <Label className="text-sm flex items-center gap-2 cursor-pointer">
            {field.description || field.name}
            {field.required && <span className="text-red-400">*</span>}
            <span className="text-[10px] text-green-400 font-mono ml-2">
              [{arrayData.length} items]
            </span>
          </Label>
        </button>
        <button
          onClick={addItem}
          className="px-2 py-1 text-xs bg-green-600 hover:bg-green-500 text-white rounded"
        >
          + Add
        </button>
        <span className="text-[10px] text-zinc-600 font-mono">array[object]</span>
      </div>

      {/* 諛곗뿴 ?꾩씠?쒕뱾 */}
      {isExpanded && (
        <div className="p-4 space-y-4 bg-zinc-900/30">
          {arrayData.length === 0 ? (
            <div className="text-center py-4 text-zinc-500 text-sm">
              No items yet. Click "+ Add" to add an item.
            </div>
          ) : (
            arrayData.map((item: any, index: number) => (
              <div key={index} className="border border-zinc-700 rounded-md bg-zinc-800/50 p-3">
                <div className="flex items-center justify-between mb-3">
                  <span className="text-xs font-semibold text-zinc-400">
                    Item #{index + 1}
                  </span>
                  <button
                    onClick={() => removeItem(index)}
                    className="text-xs text-red-400 hover:text-red-300"
                  >
                    Remove
                  </button>
                </div>
                <div className="grid grid-cols-2 gap-3">
                  {field.children!.map((child: any) => {
                    // section-header???ㅽ궢
                    if (child.type === 'section-header') return null;

                    // ?뵦 x-required-when ?먮뒗 x-optional-when 議곌굔 泥댄겕
                    const condition = child['x-required-when'] || child['x-optional-when'];
                    if (condition) {
                      // 議곌굔 ?됯? - dynamicFormData?먯꽌 ?곸쐞 ?쇱쓽 媛??뺤씤
                      // ?뵦 FIX: ???鍮꾧탳 臾몄젣 ?닿껐 (臾몄옄??"1" vs ?レ옄 0)
                      const shouldShow = Object.entries(condition).every(([key, expectedValue]) => {
                        const actualValue = dynamicFormData[key];
                        // ?뵦 ?レ옄 鍮꾧탳: ?????レ옄濡?蹂?섑빐??鍮꾧탳
                        if (typeof expectedValue === 'number') {
                          return Number(actualValue) === expectedValue;
                        }
                        return actualValue === expectedValue;
                      });
                      console.log('?뵇 Condition check:', { condition, dynamicFormData, shouldShow });
                      if (!shouldShow) return null; // 議곌굔 遺덉땐議????④?
                    }

                    const childKey = child.name.split('.').pop() || child.name;
                    const childValue = item[childKey];

                    return (
                      <div key={child.name} className="space-y-1">
                        <Label className="text-[10px] text-zinc-400 flex items-center gap-1">
                          {child.description || childKey}
                          {/* ?뵦 x-required-when 議곌굔 異⑹” ??Required 蹂?*) ?쒖떆 */}
                          {child['x-required-when'] && <span className="text-red-400">*</span>}
                          {child.required && !child['x-required-when'] && <span className="text-red-400">*</span>}
                        </Label>
                        {child.enum ? (
                          <Select
                            value={childValue !== undefined ? String(childValue) : ''}
                            onValueChange={(val) => {
                              const matchedOption = child.enum?.find((opt: any) => String(opt) === val);
                              if (matchedOption !== undefined) {
                                updateItemField(index, childKey, matchedOption);
                                return;
                              }

                              if (child.type === 'integer') {
                                const parsed = Number.parseInt(val, 10);
                                updateItemField(index, childKey, Number.isNaN(parsed) ? val : parsed);
                                return;
                              }

                              if (child.type === 'number') {
                                const parsed = Number.parseFloat(val);
                                updateItemField(index, childKey, Number.isNaN(parsed) ? val : parsed);
                                return;
                              }

                              if (child.type === 'boolean' && (val === 'true' || val === 'false')) {
                                updateItemField(index, childKey, val === 'true');
                                return;
                              }

                              updateItemField(index, childKey, val);
                            }}
                          >
                            <SelectTrigger className="h-8 text-xs bg-zinc-800 border-zinc-700">
                              <SelectValue placeholder="Select..." />
                            </SelectTrigger>
                            <SelectContent>
                              {child.enum.map((opt: any) => (
                                <SelectItem key={String(opt)} value={String(opt)}>
                                  {String(opt)}
                                </SelectItem>
                              ))}
                            </SelectContent>
                          </Select>
                        ) : child.type === 'number' || child.type === 'integer' ? (
                          <Input
                            type="number"
                            value={childValue ?? ''}
                            onChange={(e) => {
                              const val = e.target.value;
                              const parsed = val === '' ? null :
                                child.type === 'integer' ? parseInt(val, 10) : parseFloat(val);
                              updateItemField(index, childKey, parsed);
                            }}
                            className="h-8 text-xs bg-zinc-800 border-zinc-700"
                          />
                        ) : (
                          <Input
                            value={childValue || ''}
                            onChange={(e) => updateItemField(index, childKey, e.target.value)}
                            className="h-8 text-xs bg-zinc-800 border-zinc-700"
                          />
                        )}
                      </div>
                    );
                  })}
                </div>
              </div>
            ))
          )}
        </div>
      )}
    </div>
  );
}

/**
 * ?쇰컲 ?꾨뱶 ?뚮뜑留?
 */
function renderStandardField(
  field: UIBuilderField,
  definition: BuilderDefinition,
  dynamicFormData: Record<string, any>,
  updateDynamicField: (key: string, value: any) => void,
  fieldRuntimeStates?: Record<string, any>
): React.ReactNode {
  const labelStyle = definition.fieldRendering?.standard?.label || {};

  // ?렞 Runtime State?먯꽌 requiredNow ?뺤씤 (議곌굔遺 required 吏??
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

      {/* Hint ?쒖떆 */}
      {definition.hintsDisplay?.enabled && field.placeholder && (
        <p className="text-[10px] text-amber-400 italic">
          ?뮕 {field.placeholder}
        </p>
      )}

      {renderFieldInput(
        field,
        field.name,
        dynamicFormData[field.name],
        (value) => updateDynamicField(field.name, value),
        definition,
        false,
        dynamicFormData
      )}
    </>
  );
}

/**
 * ?꾨뱶 ?낅젰 而댄룷?뚰듃 ?뚮뜑留?
 */
function resolveEnumLabelMap(
  field: UIBuilderField,
  formData?: Record<string, any>
): Record<string, string> | undefined {
  const enumLabelsByType = field.enumLabelsByType || (field as any)['x-enum-labels-by-type'];
  if (!enumLabelsByType || typeof enumLabelsByType !== 'object') {
    return undefined;
  }

  const typeKeys = Object.keys(enumLabelsByType);
  if (typeKeys.length === 0) {
    return undefined;
  }

  if (formData && typeof formData === 'object') {
    const preferredTypeFields = ['TABLE_TYPE', 'TYPE', 'STYPE', 'LOAD_TYPE'];
    for (const typeField of preferredTypeFields) {
      const typeValue = formData[typeField];
      if (typeValue !== undefined && typeValue !== null) {
        const matched = enumLabelsByType[String(typeValue)];
        if (matched) {
          return matched;
        }
      }
    }

    for (const candidate of Object.values(formData)) {
      if (candidate === undefined || candidate === null) {
        continue;
      }
      const matched = enumLabelsByType[String(candidate)];
      if (matched) {
        return matched;
      }
    }
  }

  return enumLabelsByType[typeKeys[0]];
}

function renderFieldInput(
  field: UIBuilderField,
  _fieldKey: string,
  value: any,
  onChange: (value: any) => void,
  definition: BuilderDefinition,
  disabled: boolean = false,
  formData?: Record<string, any>
): React.ReactNode {
  const inputClassName = definition.fieldRendering?.standard?.input?.className || 'bg-zinc-800 border-zinc-700';
  const itemEnumOptions = Array.isArray((field.items as any)?.enum) ? ((field.items as any).enum as any[]) : undefined;
  const isArrayEnumField = field.type === 'array' && itemEnumOptions && itemEnumOptions.length > 0;
  const enumLabelMap = resolveEnumLabelMap(field, formData);
  const directEnumLabels = field.enumLabels || (field as any)['x-enum-labels'];
  const getOptionLabel = (option: any) =>
    enumLabelMap?.[String(option)] ||
    directEnumLabels?.[String(option)] ||
    String(option);
  const parseEnumSelection = (rawValue: string): any => {
    const matchedOption = field.enum?.find((option: any) => String(option) === rawValue);
    if (matchedOption !== undefined) {
      return matchedOption;
    }

    if (field.type === 'integer') {
      const parsed = Number.parseInt(rawValue, 10);
      return Number.isNaN(parsed) ? rawValue : parsed;
    }

    if (field.type === 'number') {
      const parsed = Number.parseFloat(rawValue);
      return Number.isNaN(parsed) ? rawValue : parsed;
    }

    if (field.type === 'boolean') {
      if (rawValue === 'true') return true;
      if (rawValue === 'false') return false;
    }

    return rawValue;
  };

  // Enum
  if (field.enum) {
    if (field.uiComponent === 'RadioGroup') {
      const selectedValue = value !== undefined && value !== null ? String(value) : '';
      return (
        <div className="space-y-2">
          <div className="grid grid-cols-1 gap-2 rounded border border-zinc-700 bg-zinc-900/40 p-3">
            {field.enum.map((option: any) => (
              <label key={String(option)} className="flex items-center gap-2 text-sm text-zinc-200 cursor-pointer">
                <input
                  type="radio"
                  name={field.name}
                  value={String(option)}
                  checked={selectedValue === String(option)}
                  onChange={() => onChange(parseEnumSelection(String(option)))}
                  disabled={disabled}
                  className="w-4 h-4"
                />
                <span>{getOptionLabel(option)}</span>
              </label>
            ))}
          </div>
        </div>
      );
    }

    return (
      <Select
        value={value !== undefined && value !== null ? String(value) : ''}
        onValueChange={(selected) => onChange(parseEnumSelection(selected))}
        disabled={disabled}
      >
        <SelectTrigger className={inputClassName}>
          <SelectValue placeholder={`Select ${field.description || field.name}`} />
        </SelectTrigger>
        <SelectContent>
          {field.enum.map((option: any) => (
            <SelectItem key={String(option)} value={String(option)}>
              {getOptionLabel(option)}
            </SelectItem>
          ))}
        </SelectContent>
      </Select>
    );
  }

  // Array of enum values (e.g., COMPONENTS, PARTS)
  if (isArrayEnumField) {
    const selectedValues = Array.isArray(value) ? value : [];

    const toggleValue = (option: any) => {
      const exists = selectedValues.includes(option);
      const nextValues = exists
        ? selectedValues.filter((item) => item !== option)
        : [...selectedValues, option];
      onChange(nextValues);
    };

    return (
      <div className="space-y-2">
        <div className="flex items-center gap-2">
          <button
            type="button"
            onClick={() => onChange([...itemEnumOptions])}
            disabled={disabled}
            className="px-2 py-1 text-[10px] rounded bg-zinc-800 hover:bg-zinc-700 text-zinc-300 disabled:opacity-50"
          >
            Select All
          </button>
          <button
            type="button"
            onClick={() => onChange([])}
            disabled={disabled}
            className="px-2 py-1 text-[10px] rounded bg-zinc-800 hover:bg-zinc-700 text-zinc-300 disabled:opacity-50"
          >
            Clear
          </button>
        </div>
        <div className="grid grid-cols-2 gap-2 rounded border border-zinc-700 bg-zinc-900/40 p-3">
          {itemEnumOptions.map((option) => {
            const checked = selectedValues.includes(option);
            return (
              <label key={String(option)} className="flex items-center gap-2 text-sm text-zinc-200 cursor-pointer">
                <input
                  type="checkbox"
                  checked={checked}
                  onChange={() => toggleValue(option)}
                  disabled={disabled}
                  className="w-4 h-4"
                />
                <span>{getOptionLabel(option)}</span>
              </label>
            );
          })}
        </div>
      </div>
    );
  }

  // Array
  if (field.type === 'array') {
    return (
      <Textarea
        placeholder={field.placeholder || `Enter as JSON array, e.g., [1, 2, 3]`}
        value={typeof value === 'string' ? value : JSON.stringify(value || [])}
        onChange={(e) => onChange(e.target.value)}
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
        value={value !== undefined && value !== null ? value : ''}
        onChange={(e) => {
          const val = e.target.value;
          if (val === '') {
            onChange(null);
          } else {
            const parsed = field.type === 'integer' ? parseInt(val, 10) : parseFloat(val);
            onChange(isNaN(parsed) ? 0 : parsed);
          }
        }}
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



