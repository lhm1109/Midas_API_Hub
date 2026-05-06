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
import {
  Select,
  SelectContent,
  SelectItem,
  SelectTrigger,
  SelectValue,
} from '@/components/ui/select';
import { CodeEditor } from '@/components/common';
import { useAppStore } from '@/store/useAppStore';
import type { ApiEndpoint, ApiGroup, ApiProduct } from '@/types';
import { toast } from 'sonner';
import {
  resolveActiveSchema,
  isEnhancedSchemaActive,
  type UIBuilderField
} from '@/lib/schema';
import {
  extractTriggerFields,
  extractImplicitOneOfGroups,
  type EnhancedSchema
} from '@/lib/schema/builderAdapter';
import { DynamicSchemaRenderer } from '@/lib/rendering/dynamicRenderer';
import { loadCachedDefinition, loadBuilderRules, type DefinitionType } from '@/lib/rendering/definitionLoader';
import { getPSDForProduct } from '@/config/psdMapping';
import {
  calculateFieldRuntimeStates,
  shouldIncludeInJSON,
  type FieldRuntimeStateMap
} from '@/lib/schema/fieldRuntimeState';
import { compileSchemaWithContext } from '@/lib/schema/schemaCompiler';
import {
  getDefaultValue,
  buildNormalizedFieldLookup,
  buildNormalizedRuntimeStateLookup,
  buildInitialDynamicFormData,
  flattenObjectToDotNotationWithSchema,
  hasMeaningfulFieldValue,
  normalizeJsonPreviewFieldPath,
  resolveBuilderFieldByPath,
  applyEnabledObjectDefaults,
  coerceValueForBuilderField,
  buildRootValidationOneOfOptionLabels,
  inferRootValidationOneOfSelection,
  getRootValidationOneOfOptionIndexForFieldKey,
  applyRootValidationOneOfSelection,
  isRootValidationOneOfFieldVisible,
  shouldIncludeFieldForRootValidationOneOf,
} from './builder.logic';
import { extractValidationOneOfInfo } from '@/lib/schema/validationOneOf';

interface BuilderTabProps {
  endpoint: ApiEndpoint;
  products: ApiProduct[];
  settings: {
    baseUrl: string;
    mapiKey: string;
    commonHeaders: string;
    useAssignWrapper?: boolean;
    schemaDefinition?: DefinitionType;
  };
}

function resolveNestedFieldKey(parentFieldName: string, childFieldName: string): string {
  if (!childFieldName) return childFieldName;
  if (childFieldName === parentFieldName || childFieldName.startsWith(`${parentFieldName}.`)) {
    return childFieldName;
  }
  return `${parentFieldName}.${childFieldName}`;
}

function buildConditionFormValues(source: Record<string, any>): Record<string, any> {
  const nextValues: Record<string, any> = {};

  for (const [key, value] of Object.entries(source)) {
    if (key.endsWith('._enabled') || key.startsWith('__section_')) {
      continue;
    }

    nextValues[key] = value;
  }

  return nextValues;
}

function splitFieldPath(path: string): string[] {
  return path
    .split('.')
    .map((part) => part.replace(/\[\]/g, ''))
    .filter(Boolean);
}

function getCommonPrefixLength(pathA: string, pathB: string): number {
  const segA = splitFieldPath(pathA);
  const segB = splitFieldPath(pathB);
  const max = Math.min(segA.length, segB.length);

  let count = 0;
  for (let i = 0; i < max; i++) {
    if (segA[i] !== segB[i]) break;
    count++;
  }
  return count;
}

function serializeConditionValue(value: unknown): string {
  if (typeof value === 'string') return value;
  return JSON.stringify(value);
}

function resolveConditionValueForField(
  conditionKey: string,
  formValues: Record<string, any>,
  currentFieldKey?: string
): any {
  const suffix = `.${conditionKey}`;
  const candidates = Object.keys(formValues).filter((key) =>
    !key.endsWith('._enabled') &&
    (key === conditionKey || key.endsWith(suffix))
  );

  if (candidates.length === 0) return undefined;
  if (!currentFieldKey) return formValues[candidates[0]];

  let bestKey = candidates[0];
  let bestScore = getCommonPrefixLength(bestKey, currentFieldKey);
  let bestDepth = splitFieldPath(bestKey).length;

  for (let i = 1; i < candidates.length; i++) {
    const candidate = candidates[i];
    const score = getCommonPrefixLength(candidate, currentFieldKey);
    const depth = splitFieldPath(candidate).length;
    if (score > bestScore || (score === bestScore && depth < bestDepth)) {
      bestKey = candidate;
      bestScore = score;
      bestDepth = depth;
    }
  }

  return formValues[bestKey];
}

function applyConditionalEnumOverride(
  field: UIBuilderField,
  formValues: Record<string, any>
): UIBuilderField {
  const overrides = (field as any)._conditionalPropertyOverrides as
    | Record<string, Record<string, any>>
    | undefined;

  let nextField: UIBuilderField = { ...field };
  let enumApplied = false;

  if (overrides && typeof overrides === 'object') {
    for (const [axisField, axisValueMap] of Object.entries(overrides)) {
      if (!axisValueMap || typeof axisValueMap !== 'object') continue;

      const axisValue = resolveConditionValueForField(axisField, formValues, field.name);
      const override = axisValueMap[serializeConditionValue(axisValue)];
      if (!override || typeof override !== 'object') continue;

      if (Array.isArray((override as any).enum)) {
        nextField.enum = (override as any).enum;
        enumApplied = true;
      }
      if ((override as any)['x-enum-labels']) {
        nextField.enumLabels = (override as any)['x-enum-labels'];
      }
      if ((override as any).enumLabels) {
        nextField.enumLabels = (override as any).enumLabels;
      }
      if ((override as any)['x-ui']?.component && !nextField.uiComponent) {
        nextField.uiComponent = (override as any)['x-ui'].component;
      }
    }
  }

  if (enumApplied && nextField.type !== 'enum') {
    nextField.type = 'enum';
  }

  if (field.children && field.children.length > 0) {
    nextField.children = field.children.map((child) => applyConditionalEnumOverride(child, formValues));
  }

  return nextField;
}

type WrapperShape = 'map' | 'single';

interface SchemaWrapperInfo {
  key: string;
  shape: WrapperShape;
}

function normalizeWrapperKey(value: unknown): string | null {
  if (typeof value !== 'string') return null;
  const trimmed = value.trim();
  if (!trimmed) return null;
  const lowered = trimmed.toLowerCase();
  if (lowered === 'null' || lowered === 'undefined') return null;
  return trimmed;
}

function getFieldLeafName(fieldPath: string): string {
  return fieldPath.replace(/\[\]/g, '').split('.').filter(Boolean).pop() || fieldPath;
}

function buildImplicitOneOfOptionLabels(
  children: UIBuilderField[] | undefined,
  groups: string[][]
): string[] {
  if (!children?.length) {
    return groups.map((group, index) => group[0] || `Option ${index + 1}`);
  }

  return groups.map((group, index) => {
    const matchedChildren = children.filter((child) => group.includes(getFieldLeafName(child.name)));
    if (matchedChildren.length === 0) {
      return group[0] || `Option ${index + 1}`;
    }

    const labels = matchedChildren.map((child) => child.description || getFieldLeafName(child.name));
    return labels.join(' + ');
  });
}

function getImplicitOneOfSiblingKeys(key: string, schemaFields: UIBuilderField[]): string[] {
  const parentField = schemaFields
    .filter((field) => field.implicitOneOfGroups && key.startsWith(`${field.name}.`))
    .sort((a, b) => b.name.length - a.name.length)[0];

  if (!parentField?.implicitOneOfGroups?.length) {
    return [];
  }

  const childLeafName = getFieldLeafName(key);
  const activeGroup = parentField.implicitOneOfGroups.find((group) => group.includes(childLeafName));
  if (!activeGroup) {
    return [];
  }

  const siblingLeafNames = new Set(
    parentField.implicitOneOfGroups.flatMap((group) => (
      group === activeGroup ? [] : group.filter((leafName) => !activeGroup.includes(leafName))
    ))
  );

  if (siblingLeafNames.size === 0) {
    return [];
  }

  return Array.from(siblingLeafNames).map((leafName) => {
    const matchedChild = parentField.children?.find((child) => getFieldLeafName(child.name) === leafName);
    return resolveNestedFieldKey(parentField.name, matchedChild?.name ?? leafName);
  });
}

function inferSelectedOptionIndex(field: UIBuilderField, sourceData: Record<string, any>): number {
  if (!field.oneOfOptions?.length || !field.children?.length) {
    return 0;
  }

  const selectedByValue = field.children
    .filter((child: any) => child.optionIndex !== undefined)
    .map((child: any) => {
      const childKey = resolveNestedFieldKey(field.name, child.name);
      const childValue = sourceData[childKey] ?? sourceData[child.name];
      return {
        optionIndex: child.optionIndex as number,
        hasValue: hasMeaningfulFieldValue(childValue),
      };
    })
    .find((entry) => entry.hasValue);

  return selectedByValue?.optionIndex ?? 0;
}

function applyImplicitOneOfSelection(
  previousState: Record<string, any>,
  key: string,
  value: any,
  schemaFields: UIBuilderField[]
): Record<string, any> {
  const updatedState = { ...previousState, [key]: value };

  if (!hasMeaningfulFieldValue(value)) {
    return updatedState;
  }

  const siblingKeys = getImplicitOneOfSiblingKeys(key, schemaFields);
  siblingKeys.forEach((siblingKey) => {
    delete updatedState[siblingKey];
  });

  const parentField = schemaFields
    .filter((field) => field.implicitOneOfGroups && key.startsWith(`${field.name}.`))
    .sort((a, b) => b.name.length - a.name.length)[0];

  if (parentField?.oneOfOptions?.length) {
    updatedState[`${parentField.name}.__selectedOption`] = inferSelectedOptionIndex(parentField, updatedState);
  }

  return updatedState;
}

function detectSchemaWrapperInfo(schema: any): SchemaWrapperInfo | null {
  if (!schema || typeof schema !== 'object') return null;

  const props = schema?.properties;
  if (!props || typeof props !== 'object') return null;

  const preferredKeys = ['Assign', 'Argument', 'MCD'];
  const preferredKey = preferredKeys.find((key) => props[key]);

  let wrapperKey: string | null = preferredKey ?? null;
  if (!wrapperKey) {
    const keys = Object.keys(props);
    if (keys.length === 1) {
      wrapperKey = keys[0];
    }
  }
  if (!wrapperKey) return null;

  const wrapperSchema = props[wrapperKey];
  if (!wrapperSchema || typeof wrapperSchema !== 'object') return null;

  const isMapWrapper = Boolean(wrapperSchema.additionalProperties || wrapperSchema.patternProperties);
  const isSingleWrapper = Boolean(
    wrapperSchema.properties ||
    wrapperSchema.required ||
    wrapperSchema.allOf ||
    wrapperSchema.oneOf ||
    wrapperSchema.anyOf
  );

  if (!isMapWrapper && !isSingleWrapper && !preferredKey) {
    return null;
  }

  return {
    key: wrapperKey,
    shape: isMapWrapper ? 'map' : 'single',
  };
}

function unwrapSchemaForBuilder(schema: any, wrapperInfo: SchemaWrapperInfo | null): any {
  if (!schema || typeof schema !== 'object' || !wrapperInfo || wrapperInfo.shape !== 'single') {
    return schema;
  }

  const wrapperSchema = schema?.properties?.[wrapperInfo.key];
  if (!wrapperSchema || typeof wrapperSchema !== 'object' || Array.isArray(wrapperSchema)) {
    return schema;
  }

  return {
    ...wrapperSchema,
    title: wrapperSchema.title ?? schema.title,
    description: wrapperSchema.description ?? schema.description,
  };
}

export function BuilderTab({ endpoint, products, settings }: BuilderTabProps) {
  const {
    updateRunnerData,
    addTestCase,
    deleteTestCase,
    runnerData,
    specData,
    saveCurrentVersion,
  } = useAppStore();
  const currentProduct = useMemo(() => {
    const findProductByEndpoint = (groups: ApiGroup[]): boolean => {
      return groups.some((group) =>
        group.endpoints.some((candidate) => candidate.id === endpoint.id) ||
        (group.subgroups.length > 0 && findProductByEndpoint(group.subgroups))
      );
    };

    return products.find((product) => findProductByEndpoint(product.groups));
  }, [endpoint.id, products]);

  const productId = (endpoint as any).product || currentProduct?.id;
  const { psdSet, schemaType: defaultSchemaType } = useMemo(() => {
    return getPSDForProduct(productId);
  }, [productId]);
  const schemaType = defaultSchemaType as 'original' | 'enhanced';

  const testCases = runnerData?.testCases || [];
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
  const schemaWrapperInfo = useMemo(() => detectSchemaWrapperInfo(activeSchema), [activeSchema]);
  const builderSchema = useMemo(
    () => unwrapSchemaForBuilder(activeSchema, schemaWrapperInfo),
    [activeSchema, schemaWrapperInfo]
  );
  const rootValidationOneOfInfo = useMemo(
    () => extractValidationOneOfInfo(builderSchema),
    [builderSchema]
  );
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
  const [wrapperRules, setWrapperRules] = useState<Array<{ pattern: string; wrapper: string; priority?: number }>>([]);
  const [wrapperPriorityDefault, setWrapperPriorityDefault] = useState<number>(0);
  const [enhancedSchemaMarkers, setEnhancedSchemaMarkers] = useState<string[]>([]);

  useEffect(() => {
    const loadBuilderConfig = async () => {
      try {
        const builderDef = await loadBuilderRules(psdSet, schemaType);

        if (builderDef.wrapperRules) {
          console.log('[BuilderTab] Loaded wrapper rules from', `${psdSet}/${schemaType}:`, builderDef.wrapperRules);
          setWrapperRules(builderDef.wrapperRules as Array<{ pattern: string; wrapper: string; priority?: number }>);
        }

        if (builderDef.enhancedSchemaMarkers) {
          console.log('[BuilderTab] Loaded enhanced schema markers:', builderDef.enhancedSchemaMarkers);
          setEnhancedSchemaMarkers(builderDef.enhancedSchemaMarkers);
        }
        if (builderDef.wrapperPriorityDefault !== undefined) {
          console.log('[BuilderTab] Loaded wrapperPriorityDefault:', builderDef.wrapperPriorityDefault);
          setWrapperPriorityDefault(builderDef.wrapperPriorityDefault);
        }
      } catch (error) {
        console.error('[BuilderTab] Failed to load builder config:', error);
      }
    };
    loadBuilderConfig();
  }, [psdSet, schemaType]);
  const isNewEnhancedSchema = useMemo(() => {
    if (enhancedSchemaMarkers.length === 0) {
      return false;
    }

    const schemaStr = JSON.stringify(activeSchema);
    return enhancedSchemaMarkers.some(marker => schemaStr.includes(marker));
  }, [activeSchema, enhancedSchemaMarkers]);
  const [dynamicFormData, setDynamicFormData] = useState<any>({});
  const [tempFormValuesForSchema, setTempFormValuesForSchema] = useState<Record<string, any>>({});
  const compiledSchemaContext = useMemo(() => {
    if (!builderSchema || typeof builderSchema !== 'object' || Object.keys(builderSchema).length === 0) {
      return { sections: [], variantAxes: [] };
    }
    try {
      return compileSchemaWithContext(builderSchema, psdSet, schemaType);
    } catch (error) {
      console.error('[BuilderTab] Failed to compile schema:', error);
      return { sections: [], variantAxes: [] };
    }
  }, [builderSchema, psdSet, schemaType]);

  const compiledSchemaSections = compiledSchemaContext.sections;
  const variantAxes = compiledSchemaContext.variantAxes;
  useEffect(() => {
    if (variantAxes.length > 0) {
      console.log('[BuilderTab] Variant axes detected:', variantAxes.map(axis => ({
        field: axis.field,
        type: axis.type,
        values: axis.values,
        labels: axis.labels
      })));
    }
  }, [variantAxes]);
  const schemaFields: UIBuilderField[] = useMemo(() => {
    if (compiledSchemaSections.length === 0) {
      return [];
    }
    const fields: UIBuilderField[] = [];

    const toUIBuilderField = (compiledField: any): UIBuilderField => {
      const uiHint = compiledField.ui?.hint;
      const minHint = compiledField.minimum !== undefined
        ? `min: ${compiledField.minimum}`
        : (typeof compiledField.exclusiveMinimum === 'number' ? `>${compiledField.exclusiveMinimum}` : undefined);
      const maxHint = compiledField.maximum !== undefined
        ? `max: ${compiledField.maximum}`
        : (typeof compiledField.exclusiveMaximum === 'number' ? `<${compiledField.exclusiveMaximum}` : undefined);
      const rangeHint = minHint || maxHint
        ? `Range ${[minHint, maxHint].filter(Boolean).join(', ')}`
        : undefined;
      const placeholder = [uiHint, rangeHint].filter(Boolean).join(' | ') || undefined;

      const mappedType: UIBuilderField['type'] =
        compiledField.type === 'array' ? 'array' :
          compiledField.type === 'object' ? 'object' :
            compiledField.type === 'integer' || compiledField.type === 'number' ? compiledField.type :
              compiledField.type === 'boolean' ? 'boolean' :
                compiledField.enum || compiledField.enumByType ? 'enum' : 'string';

      const uiField: UIBuilderField = {
        name: compiledField.key,
        type: mappedType,
        description: compiledField.ui?.label || compiledField.description || compiledField.key,
        required: typeof compiledField.required === 'boolean'
          ? compiledField.required
          : (typeof compiledField.required === 'object' && compiledField.required?.['*'] === 'required'),
        default: compiledField.default,
        enum: compiledField.enum || (compiledField.enumByType ? Object.values(compiledField.enumByType)[0] as any[] : undefined),
        enumLabels: compiledField['x-enum-labels'] || compiledField.enumLabels,
        placeholder,
        items: compiledField.items
          ? {
              type: compiledField.items.type || 'any',
              enum: Array.isArray(compiledField.items?.enum) ? compiledField.items.enum : undefined,
            }
          : undefined,
        uiComponent: compiledField.ui?.component,
        enumLabelsByType: compiledField['x-enum-labels-by-type'] || compiledField.enumLabelsByType,
      };
      if (compiledField._conditionalPropertyOverrides) {
        (uiField as any)._conditionalPropertyOverrides = compiledField._conditionalPropertyOverrides;
      }

      const isKeyedObject =
        mappedType === 'object' &&
        (
          (compiledField.additionalProperties &&
            typeof compiledField.additionalProperties === 'object' &&
            !Array.isArray(compiledField.additionalProperties)) ||
          (compiledField.patternProperties &&
            typeof compiledField.patternProperties === 'object' &&
            Object.values(compiledField.patternProperties).some(
              (candidate: any) => candidate && typeof candidate === 'object' && !Array.isArray(candidate)
            ))
        );

      if (isKeyedObject) {
        uiField.isKeyedObject = true;
      }

      if (compiledField.children && compiledField.children.length > 0) {
        uiField.children = compiledField.children
          .filter((child: any) => child.type !== 'section-header')
          .map((child: any) => toUIBuilderField(child));
      }

      if (compiledField['x-required-when']) {
        (uiField as any)['x-required-when'] = compiledField['x-required-when'];
      }
      if (compiledField['x-optional-when']) {
        (uiField as any)['x-optional-when'] = compiledField['x-optional-when'];
      }
      if (compiledField.optionIndex !== undefined) {
        (uiField as any).optionIndex = compiledField.optionIndex;
      }
      const implicitOneOfGroups = extractImplicitOneOfGroups(compiledField);
      if (implicitOneOfGroups) {
        uiField.implicitOneOfGroups = implicitOneOfGroups;
        uiField.oneOfOptions = buildImplicitOneOfOptionLabels(uiField.children, implicitOneOfGroups);
        uiField.children?.forEach((child: any) => {
          const optionIndex = implicitOneOfGroups.findIndex((group) => group.includes(getFieldLeafName(child.name)));
          if (optionIndex !== -1) {
            child.optionIndex = optionIndex;
          }
        });
      }

      return uiField;
    };

    for (const section of compiledSchemaSections) {
      if (section.name) {
        fields.push({
          name: `__section_${section.name}__`,
          type: 'object',
          description: section.name,
          required: false,
        });
      }
      for (const field of section.fields) {
        fields.push(toUIBuilderField(field));
      }
    }

    return fields;
  }, [compiledSchemaSections]);
  const rootValidationOneOfOptionLabels = useMemo(
    () => buildRootValidationOneOfOptionLabels(rootValidationOneOfInfo, schemaFields),
    [rootValidationOneOfInfo, schemaFields]
  );
  useEffect(() => {
    if (schemaFields.length > 0 && Object.keys(tempFormValuesForSchema).length === 0) {
      const initialValues: Record<string, any> = {};
      let triggerFieldNames: string[] = [];
      if (builderSchema && typeof builderSchema === 'object') {
        try {
          triggerFieldNames = extractTriggerFields(builderSchema as EnhancedSchema, psdSet, schemaType);
          console.log('[BuilderTab] Auto-detected trigger fields from schema:', triggerFieldNames);
        } catch (error) {
          console.warn('[BuilderTab] Failed to extract trigger fields:', error);
        }
      }
      for (const triggerFieldName of triggerFieldNames) {
        const field = schemaFields.find(f => f.name === triggerFieldName);
        if (field) {
          if (field.default !== undefined && field.default !== null) {
            initialValues[field.name] = field.default;
            console.log(`[BuilderTab] Trigger field "${field.name}" initialized with default:`, field.default);
          } else if (field.type === 'enum' && field.enum && field.enum.length > 0) {
            initialValues[field.name] = field.enum[0];
            console.log(`[BuilderTab] Trigger field "${field.name}" initialized with first enum:`, field.enum[0]);
          }
        }
      }

      if (Object.keys(initialValues).length > 0) {
        console.log('[BuilderTab] Initializing tempFormValuesForSchema (trigger only):', initialValues);
        setTempFormValuesForSchema(initialValues);
      }
    }
  }, [schemaFields, builderSchema, psdSet, schemaType]);

  const fieldRuntimeStates: FieldRuntimeStateMap = useMemo(() => {
    if (compiledSchemaSections.length === 0) {
      return {};
    }

    try {
      const combinedFormValues = buildConditionFormValues({
        ...tempFormValuesForSchema,
        ...dynamicFormData,
      });
      const runtimeStates = calculateFieldRuntimeStates(compiledSchemaSections, combinedFormValues, variantAxes);
      const requiredFields = Object.entries(runtimeStates).filter(([_, state]) => state.requiredNow);
      if (requiredFields.length > 0) {
        console.log('[BuilderTab] Required fields (requiredNow=true):',
          requiredFields.map(([name, state]) => `${name} (visible: ${state.visible})`).join(', ')
        );
      }

      return runtimeStates;
    } catch (error) {
      console.error('[BuilderTab] Failed to calculate field runtime states:', error);
      return {};
    }
  }, [compiledSchemaSections, tempFormValuesForSchema, dynamicFormData, variantAxes]);
  const selectedRootValidationOneOfOption = useMemo(() => {
    if (!rootValidationOneOfInfo) {
      return 0;
    }

    const storedSelection = dynamicFormData['__root__.__selectedOption'];
    if (typeof storedSelection === 'number') {
      return storedSelection;
    }

    return inferRootValidationOneOfSelection(rootValidationOneOfInfo, schemaFields, dynamicFormData);
  }, [rootValidationOneOfInfo, schemaFields, dynamicFormData]);
  const visibleSchemaFields = useMemo(() => {
    if (!rootValidationOneOfInfo) {
      return schemaFields;
    }

    return schemaFields.filter((field) =>
      isRootValidationOneOfFieldVisible(field, rootValidationOneOfInfo, selectedRootValidationOneOfOption)
    );
  }, [schemaFields, rootValidationOneOfInfo, selectedRootValidationOneOfOption]);
  const runtimeSchemaFields = useMemo(() => {
    const combinedFormValues = buildConditionFormValues({
      ...tempFormValuesForSchema,
      ...dynamicFormData,
    });
    return visibleSchemaFields.map((field) => applyConditionalEnumOverride(field, combinedFormValues));
  }, [visibleSchemaFields, tempFormValuesForSchema, dynamicFormData]);
  useEffect(() => {
    if (schemaFields.length > 0 && Object.keys(dynamicFormData).length === 0) {
      const initialData = buildInitialDynamicFormData(schemaFields, {});
      setDynamicFormData(initialData);
      console.log('[BuilderTab] Initialized dynamicFormData (trigger + required only):', initialData);
    }
  }, [schemaFields]);
  useEffect(() => {
    if (!rootValidationOneOfInfo || schemaFields.length === 0) {
      return;
    }

    setDynamicFormData((prev: any) => {
      if (typeof prev['__root__.__selectedOption'] === 'number') {
        return prev;
      }

      return {
        ...prev,
        '__root__.__selectedOption': inferRootValidationOneOfSelection(rootValidationOneOfInfo, schemaFields, prev),
      };
    });
  }, [rootValidationOneOfInfo, schemaFields]);
  const [assignInstances, setAssignInstances] = useState<{ [key: string]: any }>(() => {
    const initialData = buildInitialDynamicFormData(schemaFields, {});
    return { "1": initialData };
  });
  const [currentInstanceKey, setCurrentInstanceKey] = useState<string>("1");
  const [instanceKeyDraft, setInstanceKeyDraft] = useState<string>("1");
  const [selectionSourceType, setSelectionSourceType] = useState<'NODE' | 'ELEM'>('ELEM');
  const [isLoadingSelectionInstances, setIsLoadingSelectionInstances] = useState(false);

  const enableAssignInstances = useMemo(() => {
    if (settings.useAssignWrapper === false) {
      return false;
    }
    if (schemaWrapperInfo?.shape === 'single') {
      return false;
    }
    if (schemaWrapperInfo?.shape === 'map') {
      return true;
    }
    if (settings.useAssignWrapper === true) {
      return true;
    }
    const path = endpoint.path || '';
    if (!path || wrapperRules.length === 0) {
      return false;
    }

    const sortedRules = [...wrapperRules]
      .map((rule, index) => ({ ...rule, _originalIndex: index }))
      .sort((a, b) => {
        const priorityA = a.priority ?? wrapperPriorityDefault;
        const priorityB = b.priority ?? wrapperPriorityDefault;
        if (priorityB !== priorityA) {
          return priorityB - priorityA;
        }
        return (a as any)._originalIndex - (b as any)._originalIndex;
      });

    return sortedRules.some((rule) => {
      const wrapperKey = normalizeWrapperKey((rule as any).wrapper);
      if (!wrapperKey) {
        return false;
      }
      try {
        return new RegExp(rule.pattern).test(path);
      } catch {
        return false;
      }
    });
  }, [
    settings.useAssignWrapper,
    schemaWrapperInfo,
    endpoint.path,
    wrapperRules,
    wrapperPriorityDefault,
  ]);

  const addAssignInstance = () => {
    const keys = Object.keys(assignInstances);
    const numericKeys = keys
      .map((k) => Number.parseInt(k, 10))
      .filter((num) => Number.isFinite(num));
    const nextKey = String((numericKeys.length > 0 ? Math.max(...numericKeys) : 0) + 1);
    const newInstanceData = buildInitialDynamicFormData(schemaFields, {});

    setAssignInstances(prev => ({
      ...prev,
      [nextKey]: newInstanceData
    }));
    setCurrentInstanceKey(nextKey);
    setInstanceKeyDraft(nextKey);
  };
  const removeAssignInstance = (key: string) => {
    if (Object.keys(assignInstances).length <= 1) {
      toast.error('At least one instance is required.');
      return;
    }

    setAssignInstances(prev => {
      const next = { ...prev };
      delete next[key];
      if (currentInstanceKey === key) {
        const remaining = Object.keys(next)
          .sort((a, b) => Number.parseInt(a, 10) - Number.parseInt(b, 10));
        if (remaining.length > 0) {
          setCurrentInstanceKey(remaining[0]);
        }
      }
      return next;
    });
  };

  const renameAssignInstance = (nextKeyRaw: string) => {
    const nextKey = nextKeyRaw.trim();
    if (!nextKey || nextKey === currentInstanceKey) {
      setInstanceKeyDraft(currentInstanceKey);
      return;
    }
    if (!/^\d+$/.test(nextKey)) {
      toast.error('Instance key must be a numeric string.');
      setInstanceKeyDraft(currentInstanceKey);
      return;
    }
    if (Object.prototype.hasOwnProperty.call(assignInstances, nextKey)) {
      toast.error(`Instance key "${nextKey}" already exists.`);
      setInstanceKeyDraft(currentInstanceKey);
      return;
    }

    setAssignInstances((prev) => {
      if (!Object.prototype.hasOwnProperty.call(prev, currentInstanceKey)) {
        return prev;
      }
      const next = { ...prev };
      const currentData = next[currentInstanceKey];
      delete next[currentInstanceKey];
      next[nextKey] = currentData;
      return next;
    });
    setCurrentInstanceKey(nextKey);
    setInstanceKeyDraft(nextKey);
  };

  const loadAssignInstancesFromSelect = async () => {
    const baseUrl = (settings.baseUrl || '').trim().replace(/\/+$/, '');
    if (!baseUrl) {
      toast.error('Base URL is empty. Please configure it in Settings.');
      return;
    }

    const selectUrl = `${baseUrl}/view/SELECT`;
    setIsLoadingSelectionInstances(true);

    try {
      const headers: Record<string, string> = {
        Accept: 'application/json',
      };

      try {
        const commonHeaders = JSON.parse(settings.commonHeaders || '{}');
        if (commonHeaders && typeof commonHeaders === 'object' && !Array.isArray(commonHeaders)) {
          Object.entries(commonHeaders).forEach(([k, v]) => {
            if (typeof v === 'string') {
              headers[k] = v;
            }
          });
        }
      } catch (error) {
        console.warn('[BuilderTab] Failed to parse common headers:', error);
      }

      if (settings.mapiKey) {
        headers['MAPI-Key'] = settings.mapiKey;
      }

      const response = await fetch(selectUrl, {
        method: 'GET',
        headers,
      });

      const responseText = await response.text();
      let responseJson: any = {};
      try {
        responseJson = responseText ? JSON.parse(responseText) : {};
      } catch {
        throw new Error(`Invalid JSON response from ${selectUrl}`);
      }

      if (!response.ok) {
        const message =
          responseJson?.error?.message ||
          responseJson?.message ||
          `Failed to load selection (${response.status} ${response.statusText})`;
        throw new Error(message);
      }

      const selectPayload = responseJson?.SELECT ?? responseJson;
      const listKey = selectionSourceType === 'NODE' ? 'NODE_LIST' : 'ELEM_LIST';
      const rawList = selectPayload?.[listKey];

      if (!Array.isArray(rawList)) {
        throw new Error(`${listKey} is missing in SELECT response.`);
      }

      const instanceKeys = [...new Set(rawList
        .map((value: unknown) => String(value).trim())
        .filter((value: string) => /^\d+$/.test(value)))];

      if (instanceKeys.length === 0) {
        toast.info(`No ${selectionSourceType === 'NODE' ? 'node' : 'element'} IDs found in ${listKey}.`);
        return;
      }

      const templateData =
        Object.keys(dynamicFormData || {}).length > 0
          ? { ...dynamicFormData }
          : buildInitialDynamicFormData(schemaFields, {});

      const nextInstances = instanceKeys.reduce((acc, key) => {
        acc[key] = { ...templateData };
        return acc;
      }, {} as Record<string, any>);

      const firstKey = instanceKeys
        .slice()
        .sort((a, b) => Number.parseInt(a, 10) - Number.parseInt(b, 10))[0];

      setAssignInstances(nextInstances);
      setCurrentInstanceKey(firstKey);
      setInstanceKeyDraft(firstKey);
      setDynamicFormData(nextInstances[firstKey]);

      toast.success(`Loaded ${instanceKeys.length} ${selectionSourceType === 'NODE' ? 'node' : 'element'} IDs from view/SELECT.`);
    } catch (error) {
      console.error('[BuilderTab] Failed to load selection instances:', error);
      toast.error(error instanceof Error ? error.message : 'Failed to load selection from view/SELECT.');
    } finally {
      setIsLoadingSelectionInstances(false);
    }
  };

  useEffect(() => {
    setInstanceKeyDraft(currentInstanceKey);
  }, [currentInstanceKey]);

  useEffect(() => {
    if (!currentInstanceKey) return;
    const nextInstanceData = assignInstances[currentInstanceKey];
    if (!nextInstanceData) return;

    const currentSerialized = JSON.stringify(dynamicFormData ?? {});
    const nextSerialized = JSON.stringify(nextInstanceData ?? {});
    if (currentSerialized !== nextSerialized) {
      setDynamicFormData(nextInstanceData);
    }
  }, [currentInstanceKey]);
  useEffect(() => {
    if (!enableAssignInstances || !currentInstanceKey) return;

    setAssignInstances(prev => {
      const currentInstanceData = prev[currentInstanceKey] ?? {};
      const prevSerialized = JSON.stringify(currentInstanceData ?? {});
      const nextSerialized = JSON.stringify(dynamicFormData ?? {});

      if (prevSerialized === nextSerialized) {
        return prev;
      }

      return {
        ...prev,
        [currentInstanceKey]: dynamicFormData
      };
    });
  }, [dynamicFormData, currentInstanceKey, enableAssignInstances]);

  useEffect(() => {
    if (isNewEnhancedSchema && Object.keys(dynamicFormData).length > 0) {
      const flatValues = buildConditionFormValues(dynamicFormData);

      const currentStringified = JSON.stringify(tempFormValuesForSchema);
      const newStringified = JSON.stringify(flatValues);
      if (currentStringified !== newStringified) {
        setTempFormValuesForSchema(flatValues);
      }
    }
  }, [dynamicFormData, isNewEnhancedSchema]);
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
  const [builderDefinition, setBuilderDefinition] = useState<any>(null);

  useEffect(() => {
    const hasEnhancedData = !!combinedSpecData.jsonSchemaEnhanced;
    const definitionType: DefinitionType = (hasEnhancedData || isNewEnhancedSchema) ? 'enhanced' : 'original';

    console.log(`[BuilderTab] Loading YAML ${definitionType} for ${psdSet} (hasEnhancedData: ${hasEnhancedData}, isNewEnhancedSchema: ${isNewEnhancedSchema})`);
    loadCachedDefinition(
      definitionType,
      'builder',
      undefined, // schemaSet (deprecated)
      psdSet, // psdSet (Level 1)
      definitionType // schemaType (Level 2)
    )
      .then(def => {
        console.log(`[BuilderTab] Loaded ${definitionType} builder definition`);
        setBuilderDefinition(def);
      })
      .catch(err => console.error('Failed to load builder definition:', err));
  }, [isNewEnhancedSchema, psdSet]);
  const schemaFieldsKey = useMemo(() => {
    return schemaFields.map(f => f.name).join(',');
  }, [schemaFields]);
  useEffect(() => {
    if (schemaFields.length === 0) return;

    setDynamicFormData((prev: any) => {
      const initialData: any = { ...prev };
      schemaFields.forEach(field => {
        const existingValue = prev[field.name];
        if (existingValue === undefined) {
          const isTriggerField = field.enum && Array.isArray(field.enum) && field.enum.length > 0;
          const isAlwaysRequired =
            field.required === true ||
            (typeof field.required === 'object' &&
              (field.required as any)['*'] === 'required');
          if (isTriggerField || isAlwaysRequired) {
            if (field.type === 'array' && field.items) {
              initialData[field.name] = getDefaultValue(field);
            } else if (field.type === 'object' && field.children && !field.isKeyedObject) {
              const enabledKey = `${field.name}._enabled`;
              initialData[enabledKey] = false;
              field.children.forEach(child => {
                const childKey = resolveNestedFieldKey(field.name, child.name);
                initialData[childKey] = getDefaultValue(child);
              });
            } else {
              initialData[field.name] = getDefaultValue(field);
            }
          }
        }
      });
      const validFieldNames = new Set(schemaFields.map(f => f.name));
      schemaFields.forEach(f => {
        if (f.type === 'object' && f.children && !f.isKeyedObject) {
          f.children.forEach(child => {
            validFieldNames.add(resolveNestedFieldKey(f.name, child.name));
          });
          validFieldNames.add(`${f.name}._enabled`);
        }
      });

      for (const key of Object.keys(initialData)) {
        if (key.startsWith('__section_')) continue;
        if (!validFieldNames.has(key) && !key.includes('.')) {
          delete initialData[key];
        }
      }

      return initialData;
    });
  }, [schemaFieldsKey]);
  useEffect(() => {
    if (Object.keys(fieldRuntimeStates).length === 0) return;

    const requiredVisibleFields = Object.entries(fieldRuntimeStates)
      .filter(([_, state]) => state.requiredNow && state.visible);

    if (requiredVisibleFields.length === 0) return;

    setDynamicFormData((prev: any) => {
      const updated = { ...prev };
      let addedCount = 0;

      requiredVisibleFields.forEach(([fieldName, _state]) => {
        if (fieldName in updated) {
          return;
        }
        const field = schemaFields.find(f => f.name === fieldName);
        if (field) {
          updated[fieldName] = getDefaultValue(field, true);
          addedCount++;
          console.log(`[BuilderTab] Auto-added required field "${fieldName}":`, updated[fieldName]);
        }
      });

      if (addedCount > 0) {
        console.log(`[BuilderTab] Total ${addedCount} required fields auto-added`);
        return updated;
      }

      return prev;
    });
  }, [fieldRuntimeStates, schemaFields]);
  const applyAllOneOfSelections = (previousState: Record<string, any>, key: string, value: any) => {
    let updatedState = applyImplicitOneOfSelection(previousState, key, value, schemaFields);
    updatedState = applyEnabledObjectDefaults(updatedState, key, value, schemaFields);

    const rootOptionIndex = getRootValidationOneOfOptionIndexForFieldKey(
      key,
      rootValidationOneOfInfo,
      schemaFields
    );

    if (rootOptionIndex !== -1 && hasMeaningfulFieldValue(value)) {
      updatedState = applyRootValidationOneOfSelection(
        updatedState,
        rootOptionIndex,
        schemaFields,
        rootValidationOneOfInfo
      );
    }

    return updatedState;
  };
  const updateRootValidationOneOfSelection = (selectedOptionIndex: number) => {
    setDynamicFormData((prev: any) =>
      applyRootValidationOneOfSelection(prev, selectedOptionIndex, schemaFields, rootValidationOneOfInfo)
    );
    setTempFormValuesForSchema((prev: any) =>
      applyRootValidationOneOfSelection(prev, selectedOptionIndex, schemaFields, rootValidationOneOfInfo)
    );

    if (enableAssignInstances && currentInstanceKey) {
      setAssignInstances((prev) => ({
        ...prev,
        [currentInstanceKey]: applyRootValidationOneOfSelection(
          prev[currentInstanceKey] || {},
          selectedOptionIndex,
          schemaFields,
          rootValidationOneOfInfo
        ),
      }));
    }
  };

  const updateDynamicField = (key: string, value: any) => {
    if (key === '__root__.__selectedOption') {
      updateRootValidationOneOfSelection(value);
      return;
    }

    if (key.endsWith('.__selectedOption')) {
      const parentFieldName = key.replace('.__selectedOption', '');
      const parentField = resolveBuilderFieldByPath(parentFieldName, schemaFields);

      console.log('[BuilderTab] oneOf selection changed:', { key, value, parentFieldName, parentField });

      if (parentField && parentField.oneOfOptions && parentField.children) {
        const children = parentField.children;

        console.log('[BuilderTab] oneOf children:', children.map((c: any) => ({
          name: c.name,
          optionIndex: c.optionIndex,
          type: c.type,
          defaultValue: getDefaultValue(c)
        })));

        setDynamicFormData((prev: any) => {
          const updated = { ...prev, [key]: value };
          children.forEach((child: any) => {
            if (child.optionIndex !== undefined) {
              console.log('[BuilderTab] Removing child field:', child.name);
              delete updated[child.name];
            }
          });
          children.forEach((child: any) => {
            if (child.optionIndex === value) {
              const defaultVal = getDefaultValue(child);
              console.log('[BuilderTab] Initializing child field:', child.name, '=', defaultVal);
              updated[child.name] = defaultVal;
            }
          });

          const rootOptionIndex = getRootValidationOneOfOptionIndexForFieldKey(
            parentFieldName,
            rootValidationOneOfInfo,
            schemaFields
          );
          const nextState = rootOptionIndex !== -1
            ? applyRootValidationOneOfSelection(updated, rootOptionIndex, schemaFields, rootValidationOneOfInfo)
            : updated;

          console.log('[BuilderTab] Updated dynamicFormData:', nextState);
          return nextState;
        });
        setTempFormValuesForSchema((prev: any) => {
          const updated = { ...prev, [key]: value };
          children.forEach((child: any) => {
            if (child.optionIndex !== undefined) {
              delete updated[child.name];
            }
          });
          children.forEach((child: any) => {
            if (child.optionIndex === value) {
              updated[child.name] = getDefaultValue(child);
            }
          });

          const rootOptionIndex = getRootValidationOneOfOptionIndexForFieldKey(
            parentFieldName,
            rootValidationOneOfInfo,
            schemaFields
          );
          return rootOptionIndex !== -1
            ? applyRootValidationOneOfSelection(updated, rootOptionIndex, schemaFields, rootValidationOneOfInfo)
            : updated;
        });

        if (enableAssignInstances && currentInstanceKey) {
          setAssignInstances(prev => {
            const currentInstance = { ...prev[currentInstanceKey], [key]: value };
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

            const rootOptionIndex = getRootValidationOneOfOptionIndexForFieldKey(
              parentFieldName,
              rootValidationOneOfInfo,
              schemaFields
            );
            const nextInstance = rootOptionIndex !== -1
              ? applyRootValidationOneOfSelection(currentInstance, rootOptionIndex, schemaFields, rootValidationOneOfInfo)
              : currentInstance;

            return {
              ...prev,
              [currentInstanceKey]: nextInstance
            };
          });
        }
        return;
      }
    }
    setDynamicFormData((prev: any) => applyAllOneOfSelections(prev, key, value));
    setTempFormValuesForSchema((prev: any) => applyAllOneOfSelections(prev, key, value));
    if (enableAssignInstances && currentInstanceKey) {
      setAssignInstances(prev => ({
        ...prev,
        [currentInstanceKey]: applyAllOneOfSelections(prev[currentInstanceKey] || {}, key, value)
      }));
    }
  };
  const [showSaveDialog, setShowSaveDialog] = useState(false);
  const [caseName, setCaseName] = useState('');
  const [caseDescription, setCaseDescription] = useState('');
  const [selectedTestCaseId, setSelectedTestCaseId] = useState<string | null>(null);
  const [editingTestCaseId, setEditingTestCaseId] = useState<string | null>(null);
  const [editingTestCaseName, setEditingTestCaseName] = useState<string>('');
  const [rightPanelWidth, setRightPanelWidth] = useState(() => {
    if (typeof window !== 'undefined') {
      return Math.min(window.innerWidth * 0.30, 1200);
    }
    return 400; // fallback for SSR
  });
  const [isModified, setIsModified] = useState(false);
  const [jsonPreviewMode, setJsonPreviewMode] = useState<'monaco' | 'annotated'>('annotated');

  // Track initial state for comparison
  const [initialState, setInitialState] = useState<string>('');
  const [editableJson, setEditableJson] = useState<string>(() => {
    if (schemaWrapperInfo) {
      const wrapped = schemaWrapperInfo.shape === 'map'
        ? { [schemaWrapperInfo.key]: { "1": {} } }
        : { [schemaWrapperInfo.key]: {} };
      return JSON.stringify(wrapped, null, 2);
    }

    if (settings.useAssignWrapper) {
      return JSON.stringify({ Assign: { "1": {} } }, null, 2);
    }

    return JSON.stringify({}, null, 2);
  });
  const previewWrapperKeys = useMemo(() => ([
    ...new Set([
      ...wrapperRules
        .map(rule => normalizeWrapperKey(rule.wrapper))
        .filter((key): key is string => Boolean(key)),
      ...(schemaWrapperInfo ? [schemaWrapperInfo.key] : []),
    ]),
  ]), [wrapperRules, schemaWrapperInfo]);
  const normalizedSchemaFieldLookup = useMemo(
    () => buildNormalizedFieldLookup(schemaFields),
    [schemaFields]
  );
  const normalizedRuntimeStateLookup = useMemo(
    () => buildNormalizedRuntimeStateLookup(fieldRuntimeStates),
    [fieldRuntimeStates]
  );
  const getFieldMetadata = (fieldPath: string): { type: 'required' | 'optional'; color: string; label: string } => {
    const parts = fieldPath.split('.');
    if (parts.length === 2) {
      if (previewWrapperKeys.includes(parts[0]) && /^\d+$/.test(parts[1])) {
        return { type: 'required', color: 'text-red-400', label: 'Required' };
      }
    }

    const normalizedPath = normalizeJsonPreviewFieldPath(fieldPath, previewWrapperKeys);
    const runtimeState = normalizedRuntimeStateLookup.get(normalizedPath);
    if (runtimeState) {
      if (runtimeState.required === 'conditional' && !runtimeState.requiredNow) {
        return { type: 'optional', color: 'text-yellow-400', label: 'Conditional' };
      }

      return runtimeState.requiredNow
        ? { type: 'required', color: 'text-red-400', label: 'Required' }
        : { type: 'optional', color: 'text-blue-400', label: 'Optional' };
    }

    const field = normalizedSchemaFieldLookup.get(normalizedPath);
    if (field) {
      const condition = (field as any)['x-required-when'];
      if (condition) {
        const conditionMet = Object.entries(condition).every(([key, expectedValue]) => {
          const actualValue = tempFormValuesForSchema[key];
          if (typeof expectedValue === 'number') {
            return Number(actualValue) === expectedValue;
          }
          return actualValue === expectedValue;
        });

        return conditionMet
          ? { type: 'required', color: 'text-red-400', label: 'Required' }
          : { type: 'optional', color: 'text-yellow-400', label: 'Conditional' };
      }

      const isRequired = field.required === true ||
        (typeof field.required === 'object' && (field.required as any)?.['*'] === 'required');

      return isRequired
        ? { type: 'required', color: 'text-red-400', label: 'Required' }
        : { type: 'optional', color: 'text-blue-400', label: 'Optional' };
    }

    // Default
    return { type: 'optional', color: 'text-zinc-400', label: 'Optional' };
  };
  const JSONRenderer = ({ data }: { data: any }) => {
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
              const isWrapperKey = depth === 0 && previewWrapperKeys.includes(k);

              return (
                <div key={k} className="pl-4 group hover:bg-zinc-800/30 transition-colors rounded py-0.5">
                  <span className={`${isWrapperKey ? 'text-purple-400' : metadata.color} font-semibold`}>"{k}"</span>
                  <span className="text-zinc-500">: </span>{!isWrapperKey && (
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
  const convertJsonToFormData = (
    json: string,
    options?: { successMessage?: string; errorMessage?: string }
  ) => {
    try {
      const parsed = JSON.parse(json);
      const rootKey = endpoint.name.toUpperCase();
      const wrapperInfo = getResolvedWrapperInfo();

      let nestedData = parsed[rootKey] || parsed;
      let loadedInstances: Record<string, any> | null = null;
      let loadedCurrentKey: string | null = null;

      if (
        wrapperInfo &&
        nestedData &&
        typeof nestedData === 'object' &&
        !Array.isArray(nestedData) &&
        wrapperInfo.key in nestedData
      ) {
        const wrappedValue = (nestedData as any)[wrapperInfo.key];

        if (
          wrapperInfo.shape === 'map' &&
          wrappedValue &&
          typeof wrappedValue === 'object' &&
          !Array.isArray(wrappedValue)
        ) {
          const mappedInstances: Record<string, any> = {};
          Object.entries(wrappedValue as Record<string, any>).forEach(([instanceKey, instanceValue]) => {
            if (instanceValue && typeof instanceValue === 'object' && !Array.isArray(instanceValue)) {
              const flatData: Record<string, any> = {};
              flattenObjectToDotNotationWithSchema(instanceValue, flatData, schemaFields);
              mappedInstances[instanceKey] = flatData;
            }
          });

          const keys = Object.keys(mappedInstances);
          if (keys.length > 0) {
            loadedCurrentKey = keys[0];
            loadedInstances = mappedInstances;
            nestedData = (wrappedValue as Record<string, any>)[loadedCurrentKey];
          } else {
            nestedData = {};
          }
        } else if (wrappedValue && typeof wrappedValue === 'object' && !Array.isArray(wrappedValue)) {
          nestedData = wrappedValue;
        }

        console.log('[BuilderTab] Unwrapped schema wrapper:', {
          wrapperInfo,
          loadedInstanceCount: loadedInstances ? Object.keys(loadedInstances).length : 0,
        });
      }

      if (!nestedData || typeof nestedData !== 'object' || Array.isArray(nestedData)) {
        throw new Error('Invalid JSON structure for form conversion: root object expected.');
      }

      console.log('[BuilderTab] Starting JSON-to-form conversion:', { rootKey, parsed, nestedData });
      const initialData: any = {};
      schemaFields.forEach(field => {
        if (field.type === 'array' && field.items) {
          initialData[field.name] = getDefaultValue(field);
        } else if (field.type === 'object' && field.children && !field.isKeyedObject) {
          initialData[`${field.name}._enabled`] = false;
          field.children.forEach(child => {
            initialData[resolveNestedFieldKey(field.name, child.name)] = getDefaultValue(child);
          });
        } else {
          initialData[field.name] = getDefaultValue(field);
        }
      });

      console.log('[BuilderTab] Initial form data:', initialData);
      const flatData: Record<string, any> = {};
      flattenObjectToDotNotationWithSchema(nestedData, flatData, schemaFields);
      console.log('[BuilderTab] Flattened JSON-to-form data:', flatData);

      const mergedData = { ...initialData, ...flatData };

      schemaFields.forEach((field) => {
        if (field.oneOfOptions?.length && field.children?.length) {
          mergedData[`${field.name}.__selectedOption`] = inferSelectedOptionIndex(field, mergedData);
        }
      });
      if (rootValidationOneOfInfo) {
        mergedData['__root__.__selectedOption'] = inferRootValidationOneOfSelection(
          rootValidationOneOfInfo,
          schemaFields,
          mergedData
        );
      }

      console.log('[BuilderTab] JSON-to-form conversion complete:', {
        initialData,
        flatData,
        mergedData,
        schemaFields: schemaFields.map(f => ({ name: f.name, type: f.type, hasChildren: !!f.children }))
      });

      if (loadedInstances && loadedCurrentKey) {
        setAssignInstances(loadedInstances);
        setCurrentInstanceKey(loadedCurrentKey);
      }
      setDynamicFormData(mergedData);
      toast.success(options?.successMessage || 'JSON has been loaded into the form.');
      return true;
    } catch (error) {
      console.error('[BuilderTab] Failed to parse JSON:', error);
      toast.error(options?.errorMessage || 'Failed to parse JSON. Please check JSON format.');
      return false;
    }
  };
  const buildCleanJSON = (
    flatData: Record<string, any>,
    runtimeStates: FieldRuntimeStateMap,
    fields: UIBuilderField[]
  ): any => {
    const convertDotNotationToNested = (data: any): any => {
      const nested: any = {};
      const enrichedData = { ...data };
      let addedRequiredCount = 0;
      fields.forEach(field => {
        const runtimeState = runtimeStates[field.name];
        if (runtimeState && runtimeState.requiredNow && runtimeState.visible) {
          if (!(field.name in enrichedData) || enrichedData[field.name] === '') {
            if (field.enum && field.enum.length > 0) {
              enrichedData[field.name] = field.enum[0];
              console.log(`[BuilderTab] Added required field "${field.name}": ${field.enum[0]} (enum)`);
            } else {
              enrichedData[field.name] = null;
              console.log(`[BuilderTab] Added required field "${field.name}": null`);
            }
            addedRequiredCount++;
          }
        } else if (runtimeState && runtimeState.requiredNow) {
          console.log(`[BuilderTab] Required field "${field.name}" not added (visible: ${runtimeState.visible})`);
        }
      });
      if (addedRequiredCount > 0) {
        console.log(`[BuilderTab] Total required fields added: ${addedRequiredCount}`);
      }
      const oneOfSelections: Map<string, number> = new Map();
      Object.keys(enrichedData).forEach(key => {
        const match = key.match(/^(.+)\.__selectedOption$/);
        if (match) {
          const parentField = match[1];
          const selectedOption = enrichedData[key] || 0;
          oneOfSelections.set(parentField, selectedOption);
        }
      });
      const oneOfFieldsByOption: Map<string, Map<number, Set<string>>> = new Map();
      fields.forEach(field => {
        if (field.oneOfOptions && field.children) {
          if (!oneOfSelections.has(field.name)) {
            oneOfSelections.set(field.name, inferSelectedOptionIndex(field, enrichedData));
          }

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
      const storedRootSelection = enrichedData['__root__.__selectedOption'];
      const selectedRootOneOfOption = rootValidationOneOfInfo
        ? (typeof storedRootSelection === 'number'
          ? storedRootSelection
          : inferRootValidationOneOfSelection(rootValidationOneOfInfo, fields, enrichedData))
        : 0;
      const orderedKeys = Object.keys(enrichedData).sort((a, b) => {
        const depthA = a.split('.').length;
        const depthB = b.split('.').length;
        return depthA - depthB;
      });

      orderedKeys.forEach(key => {
        if (key.startsWith('__section_') || key.includes('.__section_')) {
          return;
        }
        if (key.endsWith('.__selectedOption') || key.includes('.__oneOf')) {
          return;
        }
        if (key.endsWith('._enabled')) {
          return;
        }
        if (!shouldIncludeFieldForRootValidationOneOf(key, rootValidationOneOfInfo, fields, selectedRootOneOfOption)) {
          return;
        }
        const fieldByPath = resolveBuilderFieldByPath(key, fields);
        const value = coerceValueForBuilderField(enrichedData[key], fieldByPath);
        const runtimeState = runtimeStates[key];

        const shouldInclude = shouldIncludeInJSON(key, value, runtimeState);
        if (!shouldInclude) {
          console.log(`[BuilderTab] Excluded field "${key}": value=${JSON.stringify(value)}, visible=${runtimeState?.visible}, requiredNow=${runtimeState?.requiredNow}`);
          return;
        } else if (runtimeState?.requiredNow) {
          console.log(`[BuilderTab] Included required field "${key}": value=${JSON.stringify(value)}`);
        }

        if (key.includes('.')) {
          const parts = key.split('.');
          const parentKey = parts[0];
          const childKey = parts[parts.length - 1];
          if (enrichedData[`${parentKey}._enabled`] === false) {
            return;
          }
          if (oneOfSelections.has(parentKey) && oneOfFieldsByOption.has(parentKey)) {
            const selectedOption = oneOfSelections.get(parentKey)!;
            const fieldMap = oneOfFieldsByOption.get(parentKey)!;
            const selectedFields = fieldMap.get(selectedOption);
            if (selectedFields) {
              if (!selectedFields.has(childKey)) {
                return;
              }
            }
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
          nested[key] = value;
        }
      });

      return nested;
    };
    const filterConditionalArrayFields = (obj: any): any => {
      if (obj === null || obj === undefined) return obj;

      if (Array.isArray(obj)) {
        return obj.map(item => filterConditionalArrayFields(item));
      }

      if (typeof obj === 'object') {
        const filtered: any = {};

        for (const [key, value] of Object.entries(obj)) {
          const schemaField = fields.find(f => f.name === key);

          if (schemaField && schemaField.type === 'array' && schemaField.children && Array.isArray(value)) {
            console.log('[BuilderTab][filterConditionalArrayFields] Found array field:', key, {
              childrenCount: schemaField.children.length,
              childrenNames: schemaField.children.map((c: any) => c.name),
              childrenConditions: schemaField.children.map((c: any) => ({
                name: c.name,
                'x-required-when': (c as any)['x-required-when']
              }))
            });
            filtered[key] = (value as any[]).map(item => {
              if (typeof item !== 'object' || item === null) return item;

              const filteredItem: any = {};
              for (const [itemKey, itemValue] of Object.entries(item)) {
                const childField = schemaField.children!.find((c: any) => {
                  const shortName = c.name?.includes('.') ? c.name.split('.').pop() : c.name;
                  return shortName === itemKey || c.name === itemKey;
                });
                if (itemKey === 'dRANGE_MAX' || itemKey === 'dRANGE_MIN') {
                  console.log('[BuilderTab][filterConditionalArrayFields] Child matching:', {
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
                    const conditionMet = Object.entries(condition).every(([condKey, expectedValue]) => {
                      const actualValue = flatData[condKey];
                      if (typeof expectedValue === 'number') {
                        return Number(actualValue) === expectedValue;
                      }
                      return actualValue === expectedValue;
                    });

                    console.log('[BuilderTab][filterConditionalArrayFields] Condition check:', {
                      itemKey,
                      condition,
                      actualValue: flatData['iCALC_RULE'],
                      conditionMet
                    });

                    if (!conditionMet) {
                      console.log(`[BuilderTab] Filtered out conditional field "${key}.${itemKey}" (condition not met: ${JSON.stringify(condition)})`);
                      continue;
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
    const filteredData = filterConditionalArrayFields(nestedDynamicData);

    const cleaned: any = {
      ...filteredData,
    };
    return cleanUIKeys(cleaned);
  };
  const cleanUIKeys = (obj: any): any => {
    if (obj === null || obj === undefined) return obj;

    if (Array.isArray(obj)) {
      return obj.map(item => cleanUIKeys(item));
    }

    if (typeof obj === 'object') {
      const cleaned: any = {};

      for (const [key, value] of Object.entries(obj)) {
        if (key.startsWith('__') || key.startsWith('_') && key !== '_id') {
          continue;
        }

        cleaned[key] = cleanUIKeys(value);
      }

      return cleaned;
    }

    return obj;
  };
  const getResolvedWrapperInfo = (): { key: string; shape: WrapperShape; source: 'rule' | 'schema' } | null => {
    const path = endpoint.path || '';

    console.log('[BuilderTab] getResolvedWrapperInfo called:', { path, wrapperRules, schemaWrapperInfo });

    // 스키마에 명시된 wrapper가 있으면 endpoint wrapper rule보다 우선 적용한다.
    if (schemaWrapperInfo) {
      const resolved = {
        key: schemaWrapperInfo.key,
        shape: schemaWrapperInfo.shape,
        source: 'schema' as const,
      };
      console.log('[BuilderTab] Using schema wrapper priority:', resolved);
      return resolved;
    }

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
    for (const rule of sortedRules) {
      const normalizedWrapperKey = normalizeWrapperKey((rule as any).wrapper);
      if (!normalizedWrapperKey) {
        continue;
      }
      const regex = new RegExp(rule.pattern);
      if (regex.test(path)) {
        const resolved = {
          key: normalizedWrapperKey,
          shape: 'map' as const,
          source: 'rule' as const,
        };
        console.log('[BuilderTab] Matched wrapper rule:', { rule, resolved });
        return resolved;
      }
    }

    console.log('[BuilderTab] No wrapper resolved for path:', path);
    return null;
  };
  const convertDotNotationToNestedWithRequired = (flatData: any) => {
    const nested: any = {};
    const enrichedData = { ...flatData };
    schemaFields.forEach(field => {
      const runtimeState = fieldRuntimeStates[field.name];
      if (runtimeState && runtimeState.requiredNow && runtimeState.visible) {
        if (!(field.name in enrichedData) || enrichedData[field.name] === '') {
          if (field.enum && field.enum.length > 0) {
            enrichedData[field.name] = field.enum[0];
          } else {
            enrichedData[field.name] = null;
          }
        }
      }
    });

    const orderedKeys = Object.keys(enrichedData).sort((a, b) => {
      const depthA = a.split('.').length;
      const depthB = b.split('.').length;
      return depthA - depthB;
    });
    const storedRootSelection = enrichedData['__root__.__selectedOption'];
    const selectedRootOneOfOption = rootValidationOneOfInfo
      ? (typeof storedRootSelection === 'number'
        ? storedRootSelection
        : inferRootValidationOneOfSelection(rootValidationOneOfInfo, schemaFields, enrichedData))
      : 0;

    orderedKeys.forEach(fieldKey => {
      if (fieldKey.startsWith('__section_')) {
        return;
      }

      if (fieldKey.endsWith('._enabled')) {
        return;
      }
      if (!shouldIncludeFieldForRootValidationOneOf(fieldKey, rootValidationOneOfInfo, schemaFields, selectedRootOneOfOption)) {
        return;
      }

      const fieldByPath = resolveBuilderFieldByPath(fieldKey, schemaFields);
      const value = coerceValueForBuilderField(enrichedData[fieldKey], fieldByPath);
      const runtimeState = fieldRuntimeStates[fieldKey];
      const isRequired = runtimeState?.requiredNow && runtimeState?.visible;
      if (!isRequired && (value === '' || value === undefined)) {
        return;
      }
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
  const wrapWithAssign = (body: string): string => {
    const wrapperInfo = getResolvedWrapperInfo();
    const shouldUseWrapper = settings.useAssignWrapper !== false || Boolean(schemaWrapperInfo);

    console.log('[BuilderTab] wrapWithAssign called:', {
      wrapperInfo,
      useAssignWrapper: settings.useAssignWrapper,
      shouldUseWrapper,
      endpointPath: endpoint.path,
      bodyLength: body.length
    });
    if (!wrapperInfo || !shouldUseWrapper) {
      console.log('[BuilderTab] No wrapper needed:', { wrapperInfo, shouldUseWrapper });
      return body;
    }

    try {
      const parsed = JSON.parse(body);
      const wrapperKey = wrapperInfo.key;
      if (parsed && typeof parsed === 'object' && (wrapperKey in parsed)) {
        console.log('[BuilderTab] Already wrapped with', wrapperKey);
        return body;
      }

      if (wrapperInfo.shape === 'single') {
        const wrapped = {
          [wrapperKey]: cleanUIKeys(parsed),
        };
        console.log('[BuilderTab] Wrapped with single wrapper', wrapperKey, wrapped);
        return JSON.stringify(wrapped, null, 2);
      }

      const allInstances: Record<string, any> = {};
      const instanceKeys = Object.keys(assignInstances);
      if (instanceKeys.length === 0) {
        allInstances['1'] = cleanUIKeys(parsed);
      } else {
        instanceKeys.forEach((key) => {
          const instanceData = assignInstances[key];
          allInstances[key] = cleanUIKeys(convertDotNotationToNestedWithRequired(instanceData));
        });
      }

      const wrapped = {
        [wrapperKey]: allInstances
      };

      console.log('[BuilderTab] Wrapped with map wrapper', wrapperKey, wrapped);
      return JSON.stringify(wrapped, null, 2);
    } catch (error) {
      console.warn('Failed to parse request body for Assign wrapper:', error);
      return body;
    }
  };
  const assignInstancesKey = useMemo(() => {
    return JSON.stringify(assignInstances);
  }, [assignInstances]);

  const wrapperRulesKey = useMemo(() => {
    return JSON.stringify(wrapperRules);
  }, [wrapperRules]);
  const schemaWrapperInfoKey = useMemo(() => {
    return JSON.stringify(schemaWrapperInfo || null);
  }, [schemaWrapperInfo]);
  const dynamicFormDataKey = useMemo(() => {
    return JSON.stringify(dynamicFormData);
  }, [dynamicFormData]);
  const fieldRuntimeStatesKey = useMemo(() => {
    return JSON.stringify(fieldRuntimeStates);
  }, [fieldRuntimeStates]);
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
      if (enableAssignInstances) {
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
  }, [fieldRuntimeStatesKey, enableAssignInstances]);
  useEffect(() => {
    if (schemaFields.length === 0) return;
    const cleanData = buildCleanJSON(dynamicFormData, fieldRuntimeStates, schemaFields);
    const rawRequestBody = JSON.stringify(cleanData, null, 2);
    const requestBody = wrapWithAssign(rawRequestBody);
    updateRunnerData({ requestBody });
    setEditableJson(requestBody);
  }, [
    dynamicFormDataKey,
    fieldRuntimeStatesKey,
    assignInstancesKey,
    settings.useAssignWrapper,
    wrapperRulesKey,
    schemaWrapperInfoKey
  ]);

  // Update modified state whenever data changes
  useEffect(() => {
    const currentState = JSON.stringify(buildCleanJSON(dynamicFormData, fieldRuntimeStates, schemaFields));
    if (initialState === '') {
      setInitialState(currentState);
    } else if (currentState !== initialState) {
      setIsModified(true);
    } else {
      setIsModified(false);
    }
  }, [dynamicFormDataKey, fieldRuntimeStatesKey]);
  const handleSaveTestCase = async () => {
    if (!caseName.trim()) {
      toast.error('Please enter a test case name.');
      return;
    }
    const cleanData = buildCleanJSON(dynamicFormData, fieldRuntimeStates, schemaFields);
    const rawRequestBody = JSON.stringify(cleanData, null, 2);
    const requestBody = wrapWithAssign(rawRequestBody);
    updateRunnerData({ requestBody });
    addTestCase(caseName.trim(), caseDescription.trim() || undefined);
    try {
      await saveCurrentVersion();
      toast.success(`Test case "${caseName}" saved successfully.`);
      const newTestCase = useAppStore.getState().runnerData?.testCases?.at(-1);
      if (newTestCase) {
        setSelectedTestCaseId(newTestCase.id);
      }
    } catch (error) {
      console.error('Failed to save version:', error);
      toast.error('Failed to save test case.');
    }

    // Reset dialog
    setCaseName('');
    setCaseDescription('');
    setShowSaveDialog(false);
  };

  const openSaveAsNewDialog = () => {
    if (selectedTestCaseId) {
      const selected = testCases.find(tc => tc.id === selectedTestCaseId);
      if (selected) {
        setCaseName(`${selected.name} (copy)`);
        setCaseDescription(selected.description || '');
      } else {
        setCaseName('');
        setCaseDescription('');
      }
    } else {
      setCaseName('');
      setCaseDescription('');
    }
    setShowSaveDialog(true);
  };

  const handleUpdateTestCase = async () => {
    if (!selectedTestCaseId) {
      toast.error('No test case selected.');
      return;
    }

    const selectedTestCase = testCases.find(tc => tc.id === selectedTestCaseId);
    if (!selectedTestCase) {
      toast.error('Test case not found.');
      return;
    }
    const cleanData = buildCleanJSON(dynamicFormData, fieldRuntimeStates, schemaFields);
    const rawRequestBody = JSON.stringify(cleanData, null, 2);
    const requestBody = wrapWithAssign(rawRequestBody);
    const { updateTestCase } = useAppStore.getState();
    updateTestCase(selectedTestCaseId, { requestBody });
    updateRunnerData({ requestBody });
    try {
      await saveCurrentVersion();
      toast.success(`Test case "${selectedTestCase.name}" updated successfully.`);
    } catch (error) {
      console.error('Failed to save version:', error);
      toast.error('Failed to update test case.');
    }
  };
  const handleClearTestCase = () => {
    setSelectedTestCaseId(null);
    const initialData: any = {};
    schemaFields.forEach(field => {
      const isTriggerField = field.enum && Array.isArray(field.enum) && field.enum.length > 0;
      const isAlwaysRequired =
        field.required === true ||
        (typeof field.required === 'object' && field.required['*'] === 'required');

        if (isTriggerField || isAlwaysRequired) {
          if (field.type === 'array' && field.items) {
            initialData[field.name] = getDefaultValue(field);
          } else if (field.type === 'object' && field.children && !field.isKeyedObject) {
            initialData[`${field.name}._enabled`] = false;
            field.children.forEach(child => {
              initialData[resolveNestedFieldKey(field.name, child.name)] = getDefaultValue(child);
            });
          } else {
            initialData[field.name] = getDefaultValue(field);
        }
      }
    });
    setDynamicFormData(initialData);
    setAssignInstances({ '1': initialData });
    setCurrentInstanceKey('1');

    toast.info('Ready to create a new test case.');
  };
  const handleLoadTestCase = (testCaseId: string) => {
    const testCase = testCases.find(tc => tc.id === testCaseId);
    if (!testCase) return;

    console.log('[BuilderTab] Loading test case:', testCase.requestBody.substring(0, 200));
    const loaded = convertJsonToFormData(testCase.requestBody, {
      successMessage: `Test case "${testCase.name}" loaded successfully.`,
      errorMessage: 'Failed to load test case.',
    });

    if (loaded) {
      setSelectedTestCaseId(testCaseId);
    }
  };

  const handleDeleteTestCase = async (caseId: string, e: React.MouseEvent) => {
    e.stopPropagation();
    if (confirm('Delete this test case?')) {
      deleteTestCase(caseId);
      if (selectedTestCaseId === caseId) {
        setSelectedTestCaseId(null);
      }
      if (editingTestCaseId === caseId) {
        setEditingTestCaseId(null);
      }
      try {
        await saveCurrentVersion();
        toast.success('Test case deleted.');
      } catch (error) {
        console.error('Failed to save after delete:', error);
        toast.error('Failed to save after delete.');
      }
    }
  };
  const handleStartEditName = (testCaseId: string, currentName: string, e: React.MouseEvent) => {
    e.stopPropagation();
    setEditingTestCaseId(testCaseId);
    setEditingTestCaseName(currentName);
  };
  const handleCancelEditName = () => {
    setEditingTestCaseId(null);
    setEditingTestCaseName('');
  };
  const handleSaveEditName = async (testCaseId: string) => {
    const trimmedName = editingTestCaseName.trim();

    if (!trimmedName) {
      toast.error('Test case name cannot be empty.');
      return;
    }

    const testCase = testCases.find(tc => tc.id === testCaseId);
    if (testCase && trimmedName === testCase.name) {
      handleCancelEditName();
      return;
    }
    const isDuplicate = testCases.some(
      tc => tc.id !== testCaseId && tc.name.toLowerCase() === trimmedName.toLowerCase()
    );

    if (isDuplicate) {
      toast.error('A test case with the same name already exists.');
      return;
    }
    const { updateTestCase } = useAppStore.getState();
    updateTestCase(testCaseId, { name: trimmedName });
    try {
      await saveCurrentVersion();
      toast.success(`Test case name changed to "${trimmedName}".`);
      setEditingTestCaseId(null);
      setEditingTestCaseName('');
    } catch (error) {
      console.error('Failed to save after rename:', error);
      toast.error('Failed to save after rename.');
    }
  };


  return (
    <div className="flex h-full w-full relative">
      {/* Left Sidebar - Test Case List */}
      <div className="w-80 flex flex-col border-r border-zinc-800 bg-zinc-950 flex-shrink-0">
        <div className="p-4 border-b border-zinc-800 bg-zinc-900 flex-shrink-0">
          <h3 className="text-sm font-semibold mb-1">Test Cases</h3>
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
                        <h4
                          className="text-sm font-semibold text-zinc-100 truncate cursor-text hover:text-blue-300 transition-colors"
                          onDoubleClick={(e) => handleStartEditName(testCase.id, testCase.name, e)}
                          title="Double-click to rename"
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
              Context-Aware Builder
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
          </div>{selectedTestCaseId ? (
            <div className="flex items-center gap-2 px-3 py-2 bg-blue-900/20 border border-blue-700/50 rounded-lg">
              <Edit className="w-4 h-4 text-blue-400" />
              <div className="flex-1">
                <p className="text-xs font-semibold text-blue-300">
                  Editing: {testCases.find(tc => tc.id === selectedTestCaseId)?.name || 'Unknown'}
                </p>
                <p className="text-[10px] text-blue-400/70">
                  Click "Update Test Case" to save edits.
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
                  Click "Save as New Test Case" when ready.
                </p>
              </div>
            </div>
          )}
        </div>

        <ScrollArea className="flex-1 h-0">
          <div className="p-6 space-y-6">{enableAssignInstances && (
              <section className="bg-gradient-to-br from-blue-950/50 to-zinc-900 border-2 border-blue-800/50 rounded-lg p-4">
                <div className="flex items-center justify-between mb-3">
                  <h3 className="text-sm font-semibold flex items-center gap-2">
                    <span className="text-xl">+</span>
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
                <div className="mb-3 flex flex-wrap items-center gap-2">
                  <Select
                    value={selectionSourceType}
                    onValueChange={(value: 'NODE' | 'ELEM') => setSelectionSourceType(value)}
                  >
                    <SelectTrigger className="h-8 w-44 bg-zinc-900 border-zinc-700 text-zinc-100">
                      <SelectValue />
                    </SelectTrigger>
                    <SelectContent>
                      <SelectItem value="ELEM">Element Selection</SelectItem>
                      <SelectItem value="NODE">Node Selection</SelectItem>
                    </SelectContent>
                  </Select>
                  <Button
                    type="button"
                    size="sm"
                    onClick={loadAssignInstancesFromSelect}
                    disabled={isLoadingSelectionInstances}
                    className="h-8 bg-emerald-600 hover:bg-emerald-500 text-xs"
                  >
                    {isLoadingSelectionInstances ? 'Loading...' : 'Load from view/SELECT'}
                  </Button>
                  <span className="text-[11px] text-zinc-500">
                    Uses Base URL + `/view/SELECT` with MAPI-Key/Common Headers
                  </span>
                </div>
                <div className="flex flex-wrap gap-2">
                  {Object.keys(assignInstances).sort((a, b) => Number.parseInt(a, 10) - Number.parseInt(b, 10)).map((key) => (
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
                <div className="mt-3 flex items-center gap-2">
                  <Label className="text-xs text-zinc-400">Current Key</Label>
                  <Input
                    value={instanceKeyDraft}
                    onChange={(e) => setInstanceKeyDraft(e.target.value)}
                    onKeyDown={(e) => {
                      if (e.key === 'Enter') {
                        e.preventDefault();
                        renameAssignInstance(instanceKeyDraft);
                      }
                      if (e.key === 'Escape') {
                        e.preventDefault();
                        setInstanceKeyDraft(currentInstanceKey);
                      }
                    }}
                    onBlur={() => renameAssignInstance(instanceKeyDraft)}
                    className="h-8 w-28 bg-zinc-900 border-zinc-700 text-zinc-100"
                    placeholder="e.g., 1"
                  />
                  <Button
                    type="button"
                    size="sm"
                    variant="outline"
                    onClick={() => renameAssignInstance(instanceKeyDraft)}
                    className="h-8 border-zinc-700 hover:bg-zinc-800 text-xs"
                  >
                    Apply
                  </Button>
                </div>
                <p className="text-xs text-zinc-500 mt-2">
                  Select an instance to edit. Each instance represents a separate item in the Assign wrapper.
                </p>
              </section>
            )}{schemaFields.length > 0 && (
              <section className="bg-gradient-to-br from-purple-950/50 to-zinc-900 border-2 border-purple-800/50 rounded-lg p-6">
                <h3 className="text-lg font-semibold mb-4 flex items-center gap-2">
                  <span className="text-xl">*</span>
                  Schema-Based Fields
                  {enableAssignInstances && (
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
                {rootValidationOneOfInfo && rootValidationOneOfOptionLabels.length > 1 && (
                  <div className="mb-4 rounded-lg border border-amber-700/40 bg-amber-950/20 p-4">
                    <div className="text-xs font-semibold uppercase tracking-[0.2em] text-amber-300">
                      oneOf Branch
                    </div>
                    <p className="mt-2 text-sm text-amber-100/90">
                      {rootValidationOneOfInfo.description}
                    </p>
                    <div className="mt-3 space-y-2">
                      {rootValidationOneOfOptionLabels.map((optionLabel, index) => (
                        <label
                          key={`${optionLabel}-${index}`}
                          className="flex cursor-pointer items-center gap-2 rounded-md border border-zinc-800 bg-zinc-900/60 px-3 py-2 hover:border-amber-600/40"
                        >
                          <input
                            type="radio"
                            name="__root__.__oneOf"
                            checked={selectedRootValidationOneOfOption === index}
                            onChange={() => updateRootValidationOneOfSelection(index)}
                            className="h-4 w-4"
                          />
                          <span className="text-sm text-zinc-100">{optionLabel}</span>
                        </label>
                      ))}
                    </div>
                  </div>
                )}
                {builderDefinition ? (
                  <DynamicSchemaRenderer
                    definition={builderDefinition}
                    schemaFields={runtimeSchemaFields}
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
      <div className="relative flex flex-col bg-zinc-950 overflow-hidden flex-shrink-0" style={{ width: `${rightPanelWidth}px` }}><div
          className="absolute left-0 top-0 bottom-0 w-1 hover:w-2 bg-zinc-700 hover:bg-blue-500 cursor-ew-resize transition-all z-10"
          onMouseDown={handleResizeStart}
          title="Drag to resize"
        />

        <div className="p-4 border-b border-zinc-800 bg-zinc-900 flex-shrink-0">
          <div className="flex items-center justify-between">
            <div>
              <h3 className="text-sm mb-1 flex items-center gap-2">
                {jsonPreviewMode === 'monaco' ? 'Edit' : 'View'} JSON Preview
                {jsonPreviewMode === 'monaco' && (
                  <span className="px-2 py-0.5 bg-green-600/20 text-green-400 text-[10px] rounded border border-green-600/50">
                    Editable
                  </span>
                )}
              </h3>
              <p className="text-xs text-zinc-500">
                {jsonPreviewMode === 'monaco'
                  ? 'Edit JSON and click Apply to update the form.'
                  : 'Generated Request Body (schema-based)'}
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
                View
              </button>
              <button
                onClick={() => setJsonPreviewMode('monaco')}
                className={`px-3 py-1 text-xs rounded transition-colors ${jsonPreviewMode === 'monaco'
                  ? 'bg-blue-600 text-white'
                  : 'bg-zinc-800 text-zinc-400 hover:bg-zinc-700'
                  }`}
              >
                Edit
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
                    <span className="text-yellow-400">!</span>
                    Click "Apply to Form" after editing JSON to update the form.
                  </p>
                  <Button
                    onClick={() => convertJsonToFormData(editableJson)}
                    size="sm"
                    className="bg-green-600 hover:bg-green-500 text-white shadow-lg shadow-green-600/20"
                  >
                    Apply to Form
                  </Button>
                </div>
              </div>
            ) : (
              <ScrollArea className="h-full">
                <div className="p-4">
                  <div className="p-4 bg-zinc-900/50 rounded-lg border border-zinc-800">
                    <JSONRenderer data={(() => {
                      try {
                        return JSON.parse(editableJson);
                      } catch (error) {
                        return {};
                      }
                    })()} />
                  </div>
                </div>
              </ScrollArea>
            )}
          </div>
        </div><div className="border-t border-zinc-800 bg-zinc-900 p-4 flex items-center justify-between flex-shrink-0">
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
                const initialData: any = {};
                schemaFields.forEach(field => {
                  const isTriggerField = field.enum && Array.isArray(field.enum) && field.enum.length > 0;
                  const isAlwaysRequired =
                    field.required === true ||
                    (typeof field.required === 'object' && field.required['*'] === 'required');

                  if (isTriggerField || isAlwaysRequired) {
                    if (field.type === 'array' && field.items) {
                      initialData[field.name] = getDefaultValue(field);
                    } else if (field.type === 'object' && field.children && !field.isKeyedObject) {
                      initialData[`${field.name}._enabled`] = false;
                      field.children.forEach(child => {
                        initialData[resolveNestedFieldKey(field.name, child.name)] = getDefaultValue(child);
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
              <>
                <Button
                  onClick={handleUpdateTestCase}
                  size="sm"
                  disabled={!isModified}
                  className="h-8 text-xs bg-orange-600 hover:bg-orange-500 disabled:opacity-50 disabled:cursor-not-allowed"
                >
                  <RefreshCw className="w-3 h-3 mr-2" />
                  Update Test Case
                </Button>
                <Button
                  onClick={openSaveAsNewDialog}
                  size="sm"
                  className="h-8 text-xs bg-blue-600 hover:bg-blue-500"
                >
                  <Save className="w-3 h-3 mr-2" />
                  Save as New
                </Button>
              </>
            ) : (
              <Button
                onClick={openSaveAsNewDialog}
                size="sm"
                className="h-8 text-xs bg-blue-600 hover:bg-blue-500"
              >
                <Save className="w-3 h-3 mr-2" />
                Save as New Test Case
              </Button>
            )}
          </div>
        </div>
      </div><Dialog open={showSaveDialog} onOpenChange={setShowSaveDialog}>
        <DialogContent className="bg-zinc-900 border-zinc-700 max-w-md">
          <DialogHeader>
            <DialogTitle className="text-xl text-white">Save Test Case</DialogTitle>
            <DialogDescription className="text-zinc-400">
              Save the current configuration as a test case. You can select and run it in Runner.
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
                Schema-based form with {schemaFields.length} field(s)
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


