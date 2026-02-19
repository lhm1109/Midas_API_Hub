/**
 * Remove Hungarian notation prefix from field name
 * iSTRUC_TYPE → STRUC_TYPE
 * bTORSION_REDUCTION_FACTOR_FOR_BEAM → TORSION_REDUCTION_FACTOR_FOR_BEAM
 * dGRAVITY → GRAVITY
 * 
 * Strategy: 
 * 1. Remove prefix (i, b, d, s, n, arr) only
 * 2. Keep original field name intact (no truncation/abbreviation)
 * 3. LLM should use abbreviated names from promptRules.yaml when creating schemas
 */
function removeHungarianPrefix(key: string): string {
    // Hungarian notation prefixes: i, b, d, s, n, arr
    const prefixMatch = key.match(/^(i|b|d|s|n|arr)([A-Z_].*)$/);
    if (!prefixMatch) return key;
    
    // Simply return the part after prefix - no truncation
    return prefixMatch[2];
}

/**
 * Remove internal markers from schema (x-evidence etc.)
 * AND remove Hungarian notation from keys
 * These are for validation only and should not appear in final output
 */
export function removeInternalMarkers(schema: Record<string, unknown>): Record<string, unknown> {
    const cleanSchema = { ...schema };

    // Remove x-evidence from root
    if (cleanSchema['x-evidence']) {
        delete cleanSchema['x-evidence'];
    }

    // Recursively remove from properties AND rename keys
    if (cleanSchema.properties && typeof cleanSchema.properties === 'object') {
        const props = cleanSchema.properties as Record<string, any>;
        const newProps: Record<string, any> = {};
        for (const key of Object.keys(props)) {
            const cleanKey = removeHungarianPrefix(key);
            newProps[cleanKey] = removeInternalMarkersFromField(props[key]);
        }
        cleanSchema.properties = newProps;
    }

    // Handle wrapper structures (Assign, Argument)
    for (const wrapperKey of ['Assign', 'Argument']) {
        if (cleanSchema.properties && (cleanSchema.properties as any)[wrapperKey]) {
            const wrapper = (cleanSchema.properties as any)[wrapperKey];
            
            // Handle direct properties
            if (wrapper.properties) {
                const newProps: Record<string, any> = {};
                for (const key of Object.keys(wrapper.properties)) {
                    const cleanKey = removeHungarianPrefix(key);
                    newProps[cleanKey] = removeInternalMarkersFromField(wrapper.properties[key]);
                }
                wrapper.properties = newProps;
            }
            
            // Handle patternProperties (Entity Collection)
            if (wrapper.patternProperties && typeof wrapper.patternProperties === 'object') {
                const patterns = wrapper.patternProperties as Record<string, any>;
                for (const pattern of Object.keys(patterns)) {
                    const entitySchema = patterns[pattern];
                    
                    // Clean properties inside pattern
                    if (entitySchema.properties) {
                        const newProps: Record<string, any> = {};
                        for (const key of Object.keys(entitySchema.properties)) {
                            const cleanKey = removeHungarianPrefix(key);
                            newProps[cleanKey] = removeInternalMarkersFromField(entitySchema.properties[key]);
                        }
                        entitySchema.properties = newProps;
                    }
                    
                    // Clean required array inside pattern
                    if (entitySchema.required && Array.isArray(entitySchema.required)) {
                        entitySchema.required = entitySchema.required.map((key: string) => removeHungarianPrefix(key));
                    }
                    
                    // Clean allOf inside pattern
                    if (entitySchema.allOf && Array.isArray(entitySchema.allOf)) {
                        entitySchema.allOf = entitySchema.allOf.map((condition: any) => 
                            cleanAllOfCondition(condition)
                        );
                    }
                }
            }
            
            // Handle additionalProperties
            if (wrapper.additionalProperties && typeof wrapper.additionalProperties === 'object') {
                wrapper.additionalProperties = removeInternalMarkersFromField(wrapper.additionalProperties);
            }
        }
    }

    // Update required array to use clean keys
    if (cleanSchema.required && Array.isArray(cleanSchema.required)) {
        cleanSchema.required = cleanSchema.required.map((key: string) => removeHungarianPrefix(key));
    }
    
    // Update required in wrapper
    for (const wrapperKey of ['Assign', 'Argument']) {
        if (cleanSchema.properties && (cleanSchema.properties as any)[wrapperKey]) {
            const wrapper = (cleanSchema.properties as any)[wrapperKey];
            if (wrapper.required && Array.isArray(wrapper.required)) {
                wrapper.required = wrapper.required.map((key: string) => removeHungarianPrefix(key));
            }
            
            // Update allOf conditions to use clean keys
            if (wrapper.allOf && Array.isArray(wrapper.allOf)) {
                wrapper.allOf = wrapper.allOf.map((condition: any) => 
                    cleanAllOfCondition(condition)
                );
            }
        }
    }

    return cleanSchema;
}

/**
 * Clean allOf condition by removing Hungarian notation from property references
 */
function cleanAllOfCondition(condition: any): any {
    if (!condition || typeof condition !== 'object') return condition;
    
    const cleaned = { ...condition };
    
    // Clean if.properties
    if (cleaned.if?.properties) {
        const newProps: Record<string, any> = {};
        for (const key of Object.keys(cleaned.if.properties)) {
            const cleanKey = removeHungarianPrefix(key);
            newProps[cleanKey] = cleaned.if.properties[key];
        }
        cleaned.if.properties = newProps;
    }
    
    // Clean then.properties
    if (cleaned.then?.properties) {
        const newProps: Record<string, any> = {};
        for (const key of Object.keys(cleaned.then.properties)) {
            const cleanKey = removeHungarianPrefix(key);
            newProps[cleanKey] = cleaned.then.properties[key];
        }
        cleaned.then.properties = newProps;
    }
    
    // Clean then.required
    if (cleaned.then?.required && Array.isArray(cleaned.then.required)) {
        cleaned.then.required = cleaned.then.required.map((key: string) => removeHungarianPrefix(key));
    }
    
    // Clean else.properties
    if (cleaned.else?.properties) {
        const newProps: Record<string, any> = {};
        for (const key of Object.keys(cleaned.else.properties)) {
            const cleanKey = removeHungarianPrefix(key);
            newProps[cleanKey] = cleaned.else.properties[key];
        }
        cleaned.else.properties = newProps;
    }
    
    // Recurse into nested allOf (in if clause)
    if (cleaned.if?.allOf && Array.isArray(cleaned.if.allOf)) {
        cleaned.if.allOf = cleaned.if.allOf.map((nestedCondition: any) => {
            // Each nested condition might have properties too
            if (nestedCondition.properties) {
                const newProps: Record<string, any> = {};
                for (const key of Object.keys(nestedCondition.properties)) {
                    const cleanKey = removeHungarianPrefix(key);
                    newProps[cleanKey] = nestedCondition.properties[key];
                }
                return { ...nestedCondition, properties: newProps };
            }
            return nestedCondition;
        });
    }
    
    return cleaned;
}

/**
 * Remove internal markers from a single field
 */
function removeInternalMarkersFromField(field: any): any {
    if (!field || typeof field !== 'object') return field;

    const cleaned = { ...field };

    // Remove x-evidence
    if (cleaned['x-evidence']) {
        delete cleaned['x-evidence'];
    }
    
    // Update x-optional-when to use clean keys
    if (cleaned['x-optional-when']) {
        if (typeof cleaned['x-optional-when'] === 'object' && !Array.isArray(cleaned['x-optional-when'])) {
            const newCondition: Record<string, any> = {};
            for (const key of Object.keys(cleaned['x-optional-when'])) {
                const cleanKey = removeHungarianPrefix(key);
                newCondition[cleanKey] = cleaned['x-optional-when'][key];
            }
            cleaned['x-optional-when'] = newCondition;
        } else if (Array.isArray(cleaned['x-optional-when'])) {
            cleaned['x-optional-when'] = cleaned['x-optional-when'].map((item: any) => {
                if (item.condition && typeof item.condition === 'object') {
                    const newCondition: Record<string, any> = {};
                    for (const key of Object.keys(item.condition)) {
                        const cleanKey = removeHungarianPrefix(key);
                        newCondition[cleanKey] = item.condition[key];
                    }
                    return { ...item, condition: newCondition };
                }
                return item;
            });
        }
    }
    
    // Update x-required-when to use clean keys
    if (cleaned['x-required-when']) {
        if (typeof cleaned['x-required-when'] === 'object' && !Array.isArray(cleaned['x-required-when'])) {
            const newCondition: Record<string, any> = {};
            for (const key of Object.keys(cleaned['x-required-when'])) {
                const cleanKey = removeHungarianPrefix(key);
                newCondition[cleanKey] = cleaned['x-required-when'][key];
            }
            cleaned['x-required-when'] = newCondition;
        }
    }

    // Recurse into nested properties AND rename keys
    if (cleaned.properties) {
        const newProps: Record<string, any> = {};
        for (const key of Object.keys(cleaned.properties)) {
            const cleanKey = removeHungarianPrefix(key);
            newProps[cleanKey] = removeInternalMarkersFromField(cleaned.properties[key]);
        }
        cleaned.properties = newProps;
    }

    // Update required array to use clean keys
    if (cleaned.required && Array.isArray(cleaned.required)) {
        cleaned.required = cleaned.required.map((key: string) => removeHungarianPrefix(key));
    }

    // Recurse into items (for arrays)
    if (cleaned.items) {
        cleaned.items = removeInternalMarkersFromField(cleaned.items);
    }

    // Recurse into oneOf/allOf/anyOf
    for (const combiner of ['oneOf', 'allOf', 'anyOf']) {
        if (Array.isArray(cleaned[combiner])) {
            cleaned[combiner] = cleaned[combiner].map((item: any) =>
                removeInternalMarkersFromField(item)
            );
        }
    }

    return cleaned;
}
