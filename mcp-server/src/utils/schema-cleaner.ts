/**
 * Remove internal markers from schema (x-evidence etc.)
 * These are for validation only and should not appear in final output
 */
export function removeInternalMarkers(schema: Record<string, unknown>): Record<string, unknown> {
    const cleanSchema = { ...schema };

    // Remove x-evidence from root
    if (cleanSchema['x-evidence']) {
        delete cleanSchema['x-evidence'];
    }

    // Recursively remove from properties
    if (cleanSchema.properties && typeof cleanSchema.properties === 'object') {
        const props = cleanSchema.properties as Record<string, any>;
        for (const key of Object.keys(props)) {
            props[key] = removeInternalMarkersFromField(props[key]);
        }
    }

    // Handle wrapper structures (Assign, Argument)
    for (const wrapperKey of ['Assign', 'Argument']) {
        if (cleanSchema.properties && (cleanSchema.properties as any)[wrapperKey]) {
            const wrapper = (cleanSchema.properties as any)[wrapperKey];
            if (wrapper.properties) {
                for (const key of Object.keys(wrapper.properties)) {
                    wrapper.properties[key] = removeInternalMarkersFromField(wrapper.properties[key]);
                }
            }
            // Handle additionalProperties
            if (wrapper.additionalProperties && typeof wrapper.additionalProperties === 'object') {
                wrapper.additionalProperties = removeInternalMarkersFromField(wrapper.additionalProperties);
            }
        }
    }

    return cleanSchema;
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

    // Recurse into nested properties
    if (cleaned.properties) {
        for (const key of Object.keys(cleaned.properties)) {
            cleaned.properties[key] = removeInternalMarkersFromField(cleaned.properties[key]);
        }
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
