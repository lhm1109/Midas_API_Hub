/**
 * Dynamic Schema Renderer
 * YAML ?Î∫§ÏìΩ???Í≥ïÏî™ ??àÏüª??∞Ï§à UI?????úëÔßçÍ≥πÎπÄ??àÎñé.
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
  fieldRuntimeStates?: FieldRuntimeStateMap; // ???NEW: Runtime States
}

function resolveChildFieldKey(parentFieldName: string, childFieldName: string): string {
  if (!childFieldName) return childFieldName;
  if (childFieldName === parentFieldName || childFieldName.startsWith(`${parentFieldName}.`)) {
    return childFieldName;
  }
  return `${parentFieldName}.${childFieldName}`;
}

function getFieldLeafName(fieldName: string): string {
  return fieldName.split('.').filter(Boolean).pop() || fieldName;
}

function getKeyedObjectChildDefault(field: UIBuilderField): any {
  if (field.default !== undefined && field.default !== null) {
    return field.default;
  }

  if (field.enum && field.enum.length > 0) {
    return field.enum[0];
  }

  if (field.type === 'array') return [];
  if (field.type === 'boolean') return false;
  if (field.type === 'object') return {};

  return '';
}

function shouldRenderConditionalChild(
  field: UIBuilderField,
  dynamicFormData: Record<string, any>,
  scopedFormData?: Record<string, any>
): boolean {
  const condition = (field as any)['x-required-when'] || (field as any)['x-optional-when'];
  if (!condition) return true;

  const sources = scopedFormData ? [scopedFormData, dynamicFormData] : [dynamicFormData];

  return Object.entries(condition).every(([key, expectedValue]) => {
    const leafKey = getFieldLeafName(key);

    for (const source of sources) {
      const directValue = getValueByPath(source, key);
      if (directValue !== undefined) {
        return matchesConditionalValue(directValue, expectedValue);
      }

      const leafValue = getValueByPath(source, leafKey);
      if (leafValue !== undefined) {
        return matchesConditionalValue(leafValue, expectedValue);
      }
    }

    return false;
  });
}

function getValueByPath(source: Record<string, any> | undefined, path: string): any {
  if (!source || typeof source !== 'object' || !path) return undefined;

  if (Object.prototype.hasOwnProperty.call(source, path)) {
    return source[path];
  }

  const normalizedPath = path
    .replace(/\[\]/g, '')
    .replace(/\[(\d+)\]/g, '.$1');
  const parts = normalizedPath.split('.').filter(Boolean);

  let current: any = source;
  for (const part of parts) {
    if (!current || typeof current !== 'object') {
      return undefined;
    }
    if (!(part in current)) {
      return undefined;
    }
    current = current[part];
  }

  return current;
}

function matchesConditionalValue(actualValue: any, expectedValue: any): boolean {
  if (Array.isArray(expectedValue)) {
    return expectedValue.some((candidate) => matchesConditionalValue(actualValue, candidate));
  }

  if (typeof expectedValue === 'number') {
    return Number(actualValue) === expectedValue;
  }

  if (typeof expectedValue === 'boolean') {
    if (typeof actualValue === 'string') {
      return actualValue.toLowerCase() === String(expectedValue);
    }
    return actualValue === expectedValue;
  }

  return actualValue === expectedValue || String(actualValue) === String(expectedValue);
}

function resolveScopedConditionValue(
  conditionKey: string,
  scopeStack: Array<Record<string, any> | undefined>,
  rootFormData: Record<string, any>
): any {
  const leafKey = getFieldLeafName(conditionKey);

  for (const scope of scopeStack) {
    if (!scope || typeof scope !== 'object' || Array.isArray(scope)) continue;

    const directValue = getValueByPath(scope, conditionKey);
    if (directValue !== undefined) return directValue;

    const leafValue = getValueByPath(scope, leafKey);
    if (leafValue !== undefined) return leafValue;

    if (Object.prototype.hasOwnProperty.call(scope, conditionKey)) {
      return scope[conditionKey];
    }
    if (Object.prototype.hasOwnProperty.call(scope, leafKey)) {
      return scope[leafKey];
    }
  }

  const rootDirectValue = getValueByPath(rootFormData, conditionKey);
  if (rootDirectValue !== undefined) return rootDirectValue;

  const rootLeafValue = getValueByPath(rootFormData, leafKey);
  if (rootLeafValue !== undefined) return rootLeafValue;

  return rootFormData[conditionKey] ?? rootFormData[leafKey];
}

function evaluateConditionalRule(
  condition: Record<string, any> | undefined,
  scopeStack: Array<Record<string, any> | undefined>,
  rootFormData: Record<string, any>
): boolean {
  if (!condition) return true;

  return Object.entries(condition).every(([key, expectedValue]) => {
    const actualValue = resolveScopedConditionValue(key, scopeStack, rootFormData);
    return matchesConditionalValue(actualValue, expectedValue);
  });
}

function shouldRenderScopedConditionalChild(
  field: UIBuilderField,
  scopeStack: Array<Record<string, any> | undefined>,
  rootFormData: Record<string, any>
): boolean {
  const condition = (field as any)['x-required-when'] || (field as any)['x-optional-when'];
  return evaluateConditionalRule(condition, scopeStack, rootFormData);
}

function isFieldRequiredInScope(
  field: UIBuilderField,
  scopeStack: Array<Record<string, any> | undefined>,
  rootFormData: Record<string, any>
): boolean {
  const requiredWhen = (field as any)['x-required-when'];
  if (requiredWhen) {
    return evaluateConditionalRule(requiredWhen, scopeStack, rootFormData);
  }

  return field.required;
}

function getNestedFieldDefaultValue(field: UIBuilderField): any {
  if (field.default !== undefined && field.default !== null) {
    return field.default;
  }

  if (field.enum && field.enum.length > 0) {
    return field.enum[0];
  }

  if (field.type === 'array') return [];
  if (field.type === 'boolean') return false;
  if (field.type === 'object') return {};
  if (field.type === 'number' || field.type === 'integer') return 0;

  return '';
}

function buildScopedFormData(
  rootFormData: Record<string, any>,
  scopeStack: Array<Record<string, any> | undefined>
): Record<string, any> {
  const merged = { ...rootFormData };

  for (let index = scopeStack.length - 1; index >= 0; index -= 1) {
    const scope = scopeStack[index];
    if (!scope || typeof scope !== 'object' || Array.isArray(scope)) continue;
    Object.assign(merged, scope);
  }

  return merged;
}

function createArrayItemDefaultValue(field: UIBuilderField): Record<string, any> {
  const nextItem: Record<string, any> = {};

  field.children?.forEach((child) => {
    if ((child as any).type === 'section-header') return;
    const condition = (child as any)['x-required-when'] || (child as any)['x-optional-when'];
    if (condition) return;

    nextItem[getFieldLeafName(child.name)] = getNestedFieldDefaultValue(child);
  });

  return nextItem;
}

interface NestedArrayFieldRenderContext {
  definition: BuilderDefinition;
  expandedObjects: Set<string>;
  toggleObject: (fieldName: string) => void;
  rootFormData: Record<string, any>;
  scopeStack: Array<Record<string, any> | undefined>;
}

function renderArrayItemField(
  field: UIBuilderField,
  value: any,
  onChange: (value: any) => void,
  fieldPath: string,
  context: NestedArrayFieldRenderContext
): React.ReactNode {
  if ((field as any).type === 'section-header') {
    return null;
  }

  if (!shouldRenderScopedConditionalChild(field, context.scopeStack, context.rootFormData)) {
    return null;
  }

  const fieldLabel = field.description || getFieldLeafName(field.name);
  const isRequired = isFieldRequiredInScope(field, context.scopeStack, context.rootFormData);
  const scopedFormData = buildScopedFormData(context.rootFormData, context.scopeStack);

  if (field.type === 'object' && field.children && field.children.length > 0) {
    const objectValue =
      value && typeof value === 'object' && !Array.isArray(value)
        ? value
        : {};
    const nextScopeStack = [objectValue, ...context.scopeStack];

    return (
      <div className="space-y-2 rounded-md border border-zinc-700 bg-zinc-900/40 p-3">
        <div className="flex items-center gap-2">
          <Label className="text-[10px] text-zinc-300 flex items-center gap-1">
            {fieldLabel}
            {isRequired && <span className="text-red-400">*</span>}
          </Label>
          <span className="text-[10px] text-zinc-600 font-mono ml-auto">{field.type}</span>
        </div>
        <div className="space-y-3">
          {field.children!.map((child) => {
            const childLeafName = getFieldLeafName(child.name);
            return (
              <div key={`${fieldPath}.${childLeafName}`} className="space-y-2 pl-4 border-l-2 border-zinc-700">
                {renderArrayItemField(
                  child,
                  objectValue[childLeafName],
                  (nextValue) => onChange({
                    ...objectValue,
                    [childLeafName]: nextValue,
                  }),
                  `${fieldPath}.${childLeafName}`,
                  {
                    ...context,
                    scopeStack: nextScopeStack,
                  }
                )}
              </div>
            );
          })}
        </div>
      </div>
    );
  }

  if (field.type === 'array' && field.children && field.children.length > 0) {
    const arrayValue = Array.isArray(value) ? value : [];

    const addItem = () => {
      onChange([...arrayValue, createArrayItemDefaultValue(field)]);
    };

    const removeItem = (index: number) => {
      const nextArrayValue = [...arrayValue];
      nextArrayValue.splice(index, 1);
      onChange(nextArrayValue);
    };

    const updateItemField = (index: number, childKey: string, nextValue: any) => {
      const nextArrayValue = [...arrayValue];
      const currentItem =
        nextArrayValue[index] && typeof nextArrayValue[index] === 'object' && !Array.isArray(nextArrayValue[index])
          ? nextArrayValue[index]
          : {};

      nextArrayValue[index] = {
        ...currentItem,
        [childKey]: nextValue,
      };

      onChange(nextArrayValue);
    };

    return (
      <div className="space-y-3 rounded-md border border-green-800/60 bg-zinc-900/40 p-3">
        <div className="flex items-center justify-between gap-2">
          <Label className="text-[10px] text-zinc-300 flex items-center gap-1">
            {fieldLabel}
            {isRequired && <span className="text-red-400">*</span>}
            <span className="text-[10px] text-green-400 font-mono ml-2">
              [{arrayValue.length} items]
            </span>
          </Label>
          <button
            type="button"
            onClick={addItem}
            className="px-2 py-1 text-xs bg-green-600 hover:bg-green-500 text-white rounded"
          >
            + Add
          </button>
        </div>

        {arrayValue.length === 0 ? (
          <div className="rounded border border-dashed border-zinc-700 px-3 py-4 text-xs text-zinc-500">
            No items yet. Click "+ Add" to add an item.
          </div>
        ) : (
          <div className="space-y-4">
            {arrayValue.map((item: any, index: number) => {
              const itemValue =
                item && typeof item === 'object' && !Array.isArray(item)
                  ? item
                  : {};
              const itemScopeStack = [itemValue, ...context.scopeStack];

              return (
                <div key={`${fieldPath}[${index}]`} className="border border-zinc-700 rounded-md bg-zinc-800/50 p-3">
                  <div className="flex items-center justify-between mb-3">
                    <span className="text-xs font-semibold text-zinc-400">
                      Item #{index + 1}
                    </span>
                    <button
                      type="button"
                      onClick={() => removeItem(index)}
                      className="text-xs text-red-400 hover:text-red-300"
                    >
                      Remove
                    </button>
                  </div>

                  <div className="grid grid-cols-2 gap-3">
                    {field.children!.map((child) => {
                      const childLeafName = getFieldLeafName(child.name);
                      const childNode = renderArrayItemField(
                        child,
                        itemValue[childLeafName],
                        (nextValue) => updateItemField(index, childLeafName, nextValue),
                        `${fieldPath}[${index}].${childLeafName}`,
                        {
                          ...context,
                          scopeStack: itemScopeStack,
                        }
                      );

                      if (!childNode) {
                        return null;
                      }

                      const isStructuredChild = child.type === 'object' || child.type === 'array';

                      return (
                        <div
                          key={`${fieldPath}[${index}].${childLeafName}`}
                          className={isStructuredChild ? 'space-y-1 col-span-2' : 'space-y-1'}
                        >
                          {childNode}
                        </div>
                      );
                    })}
                  </div>
                </div>
              );
            })}
          </div>
        )}
      </div>
    );
  }

  return (
    <div className="space-y-1">
      <Label className="text-[10px] text-zinc-400 flex items-center gap-1">
        {fieldLabel}
        {isRequired && <span className="text-red-400">*</span>}
      </Label>
      {renderFieldInput(
        field,
        fieldPath,
        value,
        onChange,
        context.definition,
        false,
        scopedFormData
      )}
    </div>
  );
}

/**
 * Dynamic schema renderer
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
  // ?å‚ë¶???ÄÍº???????Í≥∏Ïäú
  const containerClassName = definition.formLayout?.fieldContainer?.className || 'space-y-4';

  // ?Îµ??Î±Ä????ªÎúë??visibility ?®Íæ©Í∂? ??Ä???Î±Ä?????ºÏì¨ ?Íæ®Î±∂??‰ª?visible??ÂØÉÍªã????àÎíóÔßû¬Ä ?Î∫§Ïî§
  const isSectionVisible = (sectionIndex: number, _sectionName: string): boolean => {
    // ?Î±Ä????ÑÏçë???Íæ®Î±∂??ºÏì£ ?Î∫§Ïî§ (??ºÏì¨ ?Î±Ä????ªÎúë ?ÍæßÌâ¥Ôßû¬Ä)
    for (let i = sectionIndex + 1; i < schemaFields.length; i++) {
      const field = schemaFields[i];

      // Stop when the next section header starts
      if (field.name.startsWith(definition.sectionHeaders?.detectBy || '__section_')) {
        break;
      }

      // ?Íæ®Î±∂Â™õ¬Ä visible?Î™? ?Î∫§Ïî§
      if (fieldRuntimeStates && fieldRuntimeStates[field.name]) {
        if (fieldRuntimeStates[field.name].visible) {
          return true; // ??éÍµπ??∞Î£Ñ visible??Ä???Î±Ä????ñÎñÜ
        }
      } else if (field.visible !== false) {
        return true; // fallback: visible??falseÂ™õ¬Ä ?Íæ®Îï≤Ôß???ñÎñÜ
      }
    }

    return false; // Ôßè‚ë§Î±??Íæ®Î±∂Â™õ¬Ä hidden??Ä???Î±Ä??????
  };

  return (
    <div className={containerClassName}>
      {schemaFields
        .filter((field, index) => {
          // ?Îµ??Î±Ä????ªÎúë??ÂØÉÏéå?? ??Ä???Î±Ä????Íæ®Î±∂ ‰ª?visible??ÂØÉÍªã????àÎíóÔßû¬Ä ?Î∫§Ïî§
          if (definition.sectionHeaders?.enabled && field.name.startsWith(definition.sectionHeaders.detectBy || '__section_')) {
            return isSectionVisible(index, field.description || field.name);
          }

          // ???Runtime State Êπ≤Í≥ïÏª?visible ?Î®?ñí (Single Source of Truth)
          if (fieldRuntimeStates && fieldRuntimeStates[field.name]) {
            return fieldRuntimeStates[field.name].visible;
          }

          // ?Îµ?Fallback: visible??false???Íæ®Î±∂?????úëÔßçÍ≥πÎ∏?ßû? ??ÜÏì¨
          // visible??undefined??Ä??trueÊø?Â™õÍæ©Ôº?(?Î±Ä????ªÎúë ??
          return field.visible !== false;
        })
        .map((field) => {
          // ?Î±Ä????ªÎúë Â™õÎ®Ø?
          if (definition.sectionHeaders?.enabled && field.name.startsWith(definition.sectionHeaders.detectBy || '__section_')) {
            return renderSectionHeader(field, definition);
          }

          // ??∞Ïª≤ ?Íæ®Î±∂ ???úëÔß?
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
 * ?Î±Ä????ªÎúë ???úëÔß?
 */
function renderSectionHeader(field: UIBuilderField, definition: BuilderDefinition) {
  const sectionName = field.description || field.name.replace('__section_', '').replace(/__$/, '');
  const style = definition.sectionHeaders?.style || {};

  const containerClass = style.container || 'pt-4 pb-2 border-t-2 border-cyan-800/50 first:pt-0 first:border-t-0';
  const titleClass = style.title || 'text-sm font-semibold text-cyan-400 flex items-center gap-2';
  const icon = style.icon || '*';

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
 * ?Íæ®Î±∂ ???úëÔß?
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

  if (field.type === 'object' && field.isKeyedObject) {
    return renderKeyedObjectField(field, definition, dynamicFormData, updateDynamicField, fieldRuntimeStates);
  }

  // Object with children
  if (field.type === 'object' && field.children && field.children.length > 0) {
    return renderObjectField(field, definition, dynamicFormData, updateDynamicField, expandedObjects, toggleObject, fieldRuntimeStates);
  }

  // ?Îµ?Array with children (items.type = object with properties)
  if (field.type === 'array' && field.children && field.children.length > 0) {
    return renderArrayField(field, definition, dynamicFormData, updateDynamicField, expandedObjects, toggleObject);
  }

  // ??∞Ïª≤ ?Íæ®Î±∂
  return renderStandardField(field, definition, dynamicFormData, updateDynamicField, fieldRuntimeStates);
}

function renderKeyedObjectField(
  field: UIBuilderField,
  definition: BuilderDefinition,
  dynamicFormData: Record<string, any>,
  updateDynamicField: (key: string, value: any) => void,
  fieldRuntimeStates?: Record<string, any>
): React.ReactNode {
  const objectStyle = definition.fieldRendering?.object?.style || {};
  const labelStyle = definition.fieldRendering?.standard?.label || {};
  const runtimeState = fieldRuntimeStates?.[field.name];
  const isRequired = runtimeState?.requiredNow ?? field.required;
  const keyedValue =
    dynamicFormData[field.name] && typeof dynamicFormData[field.name] === 'object' && !Array.isArray(dynamicFormData[field.name])
      ? dynamicFormData[field.name]
      : {};

  const entryKeys = Object.keys(keyedValue).sort((a, b) => {
    const aNum = Number.parseInt(a, 10);
    const bNum = Number.parseInt(b, 10);
    const aIsNum = Number.isFinite(aNum);
    const bIsNum = Number.isFinite(bNum);
    if (aIsNum && bIsNum) return aNum - bNum;
    return a.localeCompare(b);
  });

  const createDefaultEntry = () => {
    const nextEntry: Record<string, any> = {};
    field.children?.forEach((child) => {
      if (!shouldRenderConditionalChild(child, dynamicFormData)) {
        return;
      }
      nextEntry[getFieldLeafName(child.name)] = getKeyedObjectChildDefault(child);
    });
    return nextEntry;
  };

  const addEntry = () => {
    let nextKey = '1';
    while (Object.prototype.hasOwnProperty.call(keyedValue, nextKey)) {
      nextKey = String(Number.parseInt(nextKey, 10) + 1);
    }

    updateDynamicField(field.name, {
      ...keyedValue,
      [nextKey]: createDefaultEntry(),
    });
  };

  const removeEntry = (entryKey: string) => {
    const nextValue = { ...keyedValue };
    delete nextValue[entryKey];
    updateDynamicField(field.name, nextValue);
  };

  const renameEntry = (previousKey: string, nextKeyRaw: string) => {
    const nextKey = nextKeyRaw.trim();
    if (!nextKey || nextKey === previousKey || Object.prototype.hasOwnProperty.call(keyedValue, nextKey)) {
      return;
    }

    const reordered: Record<string, any> = {};
    Object.entries(keyedValue).forEach(([entryKey, entryValue]) => {
      reordered[entryKey === previousKey ? nextKey : entryKey] = entryValue;
    });
    updateDynamicField(field.name, reordered);
  };

  const updateEntryField = (entryKey: string, childName: string, value: any) => {
    updateDynamicField(field.name, {
      ...keyedValue,
      [entryKey]: {
        ...(keyedValue[entryKey] || {}),
        [childName]: value,
      },
    });
  };

  return (
    <div className="space-y-3">
      <div className="flex items-center justify-between gap-3">
        <Label className={labelStyle.className || 'text-sm flex items-center gap-2'}>
          {field.description || field.name}
          {isRequired && labelStyle.showRequired && <span className="text-red-400">*</span>}
          {labelStyle.showType && (
            <span className="text-[10px] text-zinc-600 font-mono ml-auto">{field.type}</span>
          )}
        </Label>
        <button
          type="button"
          onClick={addEntry}
          className="px-2 py-1 text-xs rounded bg-blue-600 hover:bg-blue-500 text-white"
        >
          + Add Entry
        </button>
      </div>

      {definition.hintsDisplay?.enabled && field.placeholder && (
        <p className="text-[10px] text-amber-400 italic">
          Hint {field.placeholder}
        </p>
      )}

      <div className={objectStyle.border || 'border border-zinc-700 rounded-md bg-zinc-900/50'}>
        <div className={objectStyle.content || 'p-4 space-y-3 bg-zinc-900/30'}>
          {entryKeys.length === 0 ? (
            <div className="rounded border border-dashed border-zinc-700 px-3 py-4 text-sm text-zinc-500">
              No entries yet. Add a node key, then fill in its fields.
            </div>
          ) : (
            entryKeys.map((entryKey) => {
              const entryValue =
                keyedValue[entryKey] && typeof keyedValue[entryKey] === 'object' && !Array.isArray(keyedValue[entryKey])
                  ? keyedValue[entryKey]
                  : {};

              return (
                <div key={entryKey} className="rounded border border-zinc-700 bg-zinc-800/40 p-3 space-y-3">
                  <div className="flex items-center gap-2">
                    <Label className="text-xs text-zinc-400 min-w-12">Key</Label>
                    <Input
                      value={entryKey}
                      onChange={(e) => renameEntry(entryKey, e.target.value)}
                      className="h-8 bg-zinc-800 border-zinc-700"
                    />
                    <button
                      type="button"
                      onClick={() => removeEntry(entryKey)}
                      className="px-2 py-1 text-xs rounded bg-zinc-800 hover:bg-zinc-700 text-red-300"
                    >
                      Remove
                    </button>
                  </div>

                  {field.children?.filter((child) => shouldRenderConditionalChild(child, dynamicFormData, entryValue)).map((child) => {
                    const childLeafName = getFieldLeafName(child.name);
                    return (
                      <div key={`${entryKey}-${child.name}`} className="space-y-2 pl-4 border-l-2 border-zinc-700">
                        <Label className="text-xs flex items-center gap-2">
                          {child.description || childLeafName}
                          {child.required && <span className="text-red-400">*</span>}
                          <span className="text-[10px] text-zinc-600 font-mono ml-auto">{child.type}</span>
                        </Label>
                        {renderFieldInput(
                          child,
                          `${field.name}.${entryKey}.${childLeafName}`,
                          entryValue[childLeafName],
                          (value) => updateEntryField(entryKey, childLeafName, value),
                          definition,
                          false,
                          dynamicFormData
                        )}
                      </div>
                    );
                  })}
                </div>
              );
            })
          )}
        </div>
      </div>
    </div>
  );
}

/**
 * Object ?Íæ®Î±∂ ???úëÔß?(‰ª•Î¨íÍ∫??Íæ®Î±∂)
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

  // ?Îµ?oneOf ???Ω©: ???????Ä?‰ª???éÍµπ ?Ï¢èÍπÆ
  const isOneOf = field.oneOfOptions && field.oneOfOptions.length > 0;

  return (
    <div className={objectStyle.border || 'border border-zinc-700 rounded-md bg-zinc-900/50'}>
      {/* ??ªÎúë */}
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

      {/* ?Îµ?oneOf ?Ï¢èÍπÆ ??∞Îµí??Ë∏∞Íæ™??*/}
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

      {/* ?Î®?ñá ?Íæ®Î±∂??*/}
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

                {/* ?Îµ?child.name????Ä? ?Íæ©Íªú ÂØÉÏéàÏ§àÁëú???Î∏?(NODE_ELEMS.KEYS) */}
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
 * ?Îµ?Array ?Íæ®Î±∂ ???úëÔß?(items.type = object)
 * REDUCTION_DATAÔß£ÏÑé??Ë´õÍ≥óÎø???Ä???Â™õÏïπÍªúÂ™õ? ??àÎíó ÂØÉÏéå??Ôß£ÏÑé?? */
function renderArrayField(
  field: UIBuilderField,
  definition: BuilderDefinition,
  dynamicFormData: Record<string, any>,
  updateDynamicField: (key: string, value: any) => void,
  expandedObjects: Set<string>,
  toggleObject: (fieldName: string) => void
): React.ReactNode {
  const objectStyle = definition.fieldRendering?.object?.style || {};
  const arrayData = dynamicFormData[field.name] || [];
  const isExpanded = expandedObjects.has(field.name);
  const addItem = () => {
    updateDynamicField(field.name, [...arrayData, createArrayItemDefaultValue(field)]);
  };
  const removeItem = (index: number) => {
    const newArray = [...arrayData];
    newArray.splice(index, 1);
    updateDynamicField(field.name, newArray);
  };
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
                    const childKey = getFieldLeafName(child.name);
                    const childNode = renderArrayItemField(
                      child,
                      item?.[childKey],
                      (value) => updateItemField(index, childKey, value),
                      `${field.name}[${index}].${childKey}`,
                      {
                        definition,
                        expandedObjects,
                        toggleObject,
                        rootFormData: dynamicFormData,
                        scopeStack: [item],
                      }
                    );
                    if (!childNode) {
                      return null;
                    }
                    const isStructuredChild = child.type === 'object' || child.type === 'array';
                    return (
                      <div
                        key={`${field.name}[${index}].${childKey}`}
                        className={isStructuredChild ? 'space-y-1 col-span-2' : 'space-y-1'}
                      >
                        {childNode}
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
 * Standard field renderer
 */
function renderStandardField(
  field: UIBuilderField,
  definition: BuilderDefinition,
  dynamicFormData: Record<string, any>,
  updateDynamicField: (key: string, value: any) => void,
  fieldRuntimeStates?: Record<string, any>
): React.ReactNode {
  const labelStyle = definition.fieldRendering?.standard?.label || {};

  // ???Runtime State?Î®?Ωå requiredNow ?Î∫§Ïî§ (Ë≠∞Í≥åÍµîÈÅ∫? required Ôßû¬Ä??
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

      {/* Hint ??ñÎñÜ */}
      {definition.hintsDisplay?.enabled && field.placeholder && (
        <p className="text-[10px] text-amber-400 italic">
          ?ÎÆ?{field.placeholder}
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
 * ?Íæ®Î±∂ ??ÖÏ†∞ ?åÎåÑÎ£??∞ÎìÉ ???úëÔß?
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
  fieldKey: string,
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
                  name={fieldKey}
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



