/**
 * Designer Store (Zustand)
 * 
 * Visual Schema Designer 상태 관리
 * Row-based Grid Layout System
 */

import { create } from 'zustand';
import { persist } from 'zustand/middleware';
import type { DesignedField, DesignerConfig, DesignedSection, DesignerRow } from '../types/designer';
import { loadDesignerConfig } from '../lib/yamlRulesLoader';

// ============================================================================
// Store State
// ============================================================================

interface DesignerStore {
    // State
    fields: DesignedField[];
    rows: DesignerRow[];  // 행 배열 (순서대로)
    selectedFieldId: string | null;
    config: DesignerConfig | null;
    isDirty: boolean;
    isLoading: boolean;

    // Transport 설정
    transport: {
        uri?: string;
        methods?: string[];
        bodyRoot?: string;
    };

    // Actions - Config
    loadConfig: (psdSet?: string) => Promise<void>;

    // Actions - Fields
    addField: (field: DesignedField, parentId?: string) => void;
    updateField: (id: string, updates: Partial<DesignedField>) => void;
    removeField: (id: string) => void;
    reorderFields: (sourceIndex: number, destinationIndex: number, parentId?: string) => void;
    moveField: (fieldId: string, newParentId: string | null, newIndex: number) => void;

    // Actions - Row Layout
    createRowWithField: (fieldId: string, afterRowId?: string) => void;
    mergeToRow: (fieldId: string, targetRowId: string, targetColumnIndex: number) => void;
    separateToNewRow: (fieldId: string, afterRowId?: string) => void;
    reorderRows: (sourceRowIndex: number, destinationRowIndex: number) => void;
    reorderColumnsInRow: (rowId: string, sourceColIndex: number, destColIndex: number) => void;
    moveFieldToRow: (fieldId: string, targetRowId: string, columnIndex: number) => void;

    // Actions - Selection
    selectField: (id: string | null) => void;

    // Actions - Sections (derived from x-ui.group)
    getSections: () => DesignedSection[];

    // Actions - Transport
    updateTransport: (transport: Partial<DesignerStore['transport']>) => void;

    // Actions - State Management
    setFields: (fields: DesignedField[], rows?: DesignerRow[]) => void;
    setDirty: (dirty: boolean) => void;
    reset: () => void;
}

// ============================================================================
// Helper Functions
// ============================================================================

function generateId(): string {
    return `field_${Date.now()}_${Math.random().toString(36).substr(2, 9)}`;
}

function generateRowId(): string {
    return `row_${Date.now()}_${Math.random().toString(36).substr(2, 9)}`;
}

function findFieldById(fields: DesignedField[], id: string): DesignedField | null {
    for (const field of fields) {
        if (field.id === id) return field;
        if (field.children) {
            const found = findFieldById(field.children, id);
            if (found) return found;
        }
    }
    return null;
}

function updateFieldInTree(
    fields: DesignedField[],
    id: string,
    updates: Partial<DesignedField>
): DesignedField[] {
    return fields.map(field => {
        if (field.id === id) {
            return { ...field, ...updates };
        }
        if (field.children) {
            return {
                ...field,
                children: updateFieldInTree(field.children, id, updates),
            };
        }
        return field;
    });
}

function removeFieldFromTree(fields: DesignedField[], id: string): DesignedField[] {
    return fields
        .filter(field => field.id !== id)
        .map(field => {
            if (field.children) {
                return {
                    ...field,
                    children: removeFieldFromTree(field.children, id),
                };
            }
            return field;
        });
}

function addFieldToParent(
    fields: DesignedField[],
    newField: DesignedField,
    parentId: string
): DesignedField[] {
    return fields.map(field => {
        if (field.id === parentId) {
            return {
                ...field,
                children: [...(field.children || []), newField],
            };
        }
        if (field.children) {
            return {
                ...field,
                children: addFieldToParent(field.children, newField, parentId),
            };
        }
        return field;
    });
}

// Remove field from its current row
function removeFieldFromRows(rows: DesignerRow[], fieldId: string): DesignerRow[] {
    return rows
        .map(row => ({
            ...row,
            fieldIds: row.fieldIds.filter(id => id !== fieldId),
        }))
        .filter(row => row.fieldIds.length > 0); // Remove empty rows
}

// ============================================================================
// Store
// ============================================================================

export const useDesignerStore = create<DesignerStore>()(
    persist(
        (set, get) => ({
            // Initial State
            fields: [],
            rows: [],
            selectedFieldId: null,
            config: null,
            isDirty: false,
            isLoading: false,
            transport: {},

            // Load Config
            loadConfig: async (psdSet?: string) => {
                set({ isLoading: true });
                try {
                    const config = await loadDesignerConfig(psdSet);
                    set({ config, isLoading: false });
                } catch (error) {
                    console.error('Failed to load designer config:', error);
                    set({ isLoading: false });
                }
            },

            // Add Field (creates new row by default)
            addField: (field: DesignedField, parentId?: string) => {
                const newField = { ...field, id: field.id || generateId() };
                const newRowId = generateRowId();

                // Set layout info
                newField.layout = {
                    rowId: newRowId,
                    columnIndex: 0,
                };

                set(state => {
                    if (parentId) {
                        // Add to parent (object/array children) - no row needed
                        return {
                            fields: addFieldToParent(state.fields, newField, parentId),
                            isDirty: true,
                        };
                    }

                    // Add as new row
                    const newRow: DesignerRow = {
                        id: newRowId,
                        fieldIds: [newField.id],
                    };

                    return {
                        fields: [...state.fields, newField],
                        rows: [...state.rows, newRow],
                        isDirty: true,
                    };
                });
            },

            // Update Field
            updateField: (id: string, updates: Partial<DesignedField>) => {
                set(state => ({
                    fields: updateFieldInTree(state.fields, id, updates),
                    isDirty: true,
                }));
            },

            // Remove Field (also removes from row)
            removeField: (id: string) => {
                set(state => {
                    const newRows = removeFieldFromRows(state.rows, id);
                    return {
                        fields: removeFieldFromTree(state.fields, id),
                        rows: newRows,
                        selectedFieldId: state.selectedFieldId === id ? null : state.selectedFieldId,
                        isDirty: true,
                    };
                });
            },

            // Reorder Fields (legacy - for compatibility)
            reorderFields: (sourceIndex: number, destinationIndex: number, parentId?: string) => {
                set(state => {
                    const reorder = (fields: DesignedField[]): DesignedField[] => {
                        const result = [...fields];
                        const [removed] = result.splice(sourceIndex, 1);
                        result.splice(destinationIndex, 0, removed);
                        return result;
                    };

                    if (parentId) {
                        return {
                            fields: state.fields.map(field => {
                                if (field.id === parentId && field.children) {
                                    return { ...field, children: reorder(field.children) };
                                }
                                return field;
                            }),
                            isDirty: true,
                        };
                    }

                    return {
                        fields: reorder(state.fields),
                        isDirty: true,
                    };
                });
            },

            // Move Field (for drag between containers - legacy)
            moveField: (fieldId: string, newParentId: string | null, newIndex: number) => {
                set(state => {
                    const field = findFieldById(state.fields, fieldId);
                    if (!field) return state;

                    // Remove from current position
                    let newFields = removeFieldFromTree(state.fields, fieldId);

                    // Add to new position
                    if (newParentId) {
                        newFields = addFieldToParent(newFields, field, newParentId);
                    } else {
                        newFields.splice(newIndex, 0, field);
                    }

                    return { fields: newFields, isDirty: true };
                });
            },

            // ================================================================
            // Row Layout Actions
            // ================================================================

            // Create a new row with the given field
            createRowWithField: (fieldId: string, afterRowId?: string) => {
                set(state => {
                    const field = findFieldById(state.fields, fieldId);
                    if (!field) return state;

                    // Remove from current row first
                    let newRows = removeFieldFromRows(state.rows, fieldId);

                    // Create new row
                    const newRowId = generateRowId();
                    const newRow: DesignerRow = {
                        id: newRowId,
                        fieldIds: [fieldId],
                    };

                    // Find insertion index
                    let insertIndex = newRows.length; // Default: end
                    if (afterRowId) {
                        const afterIndex = newRows.findIndex(r => r.id === afterRowId);
                        if (afterIndex !== -1) {
                            insertIndex = afterIndex + 1;
                        }
                    }

                    // Insert new row
                    newRows.splice(insertIndex, 0, newRow);

                    // Update field layout
                    const newFields = updateFieldInTree(state.fields, fieldId, {
                        layout: { rowId: newRowId, columnIndex: 0 },
                    });

                    return { fields: newFields, rows: newRows, isDirty: true };
                });
            },

            // Merge field to an existing row at the specified column position
            mergeToRow: (fieldId: string, targetRowId: string, targetColumnIndex: number) => {
                set(state => {
                    const field = findFieldById(state.fields, fieldId);
                    if (!field) {
                        console.warn('mergeToRow: field not found', fieldId);
                        return state;
                    }

                    // Check if target row exists before we start
                    const targetRowExists = state.rows.some(r => r.id === targetRowId);
                    if (!targetRowExists) {
                        console.warn('mergeToRow: target row not found', targetRowId);
                        return state;
                    }

                    // Get the current row of the field (before removing)
                    const currentRow = state.rows.find(r => r.fieldIds.includes(fieldId));
                    const isSameRow = currentRow?.id === targetRowId;

                    // If same row, this is a reorder operation, handle differently
                    if (isSameRow && currentRow) {
                        const currentIndex = currentRow.fieldIds.indexOf(fieldId);
                        // Skip if dropping in same position
                        if (currentIndex === targetColumnIndex || currentIndex === targetColumnIndex - 1) {
                            return state;
                        }

                        // Reorder within same row
                        const newFieldIds = [...currentRow.fieldIds];
                        newFieldIds.splice(currentIndex, 1);
                        const adjustedIndex = currentIndex < targetColumnIndex ? targetColumnIndex - 1 : targetColumnIndex;
                        newFieldIds.splice(adjustedIndex, 0, fieldId);

                        const newRows = state.rows.map(row =>
                            row.id === targetRowId ? { ...row, fieldIds: newFieldIds } : row
                        );

                        // Update columnIndex for all fields in this row
                        let newFields = state.fields;
                        newFieldIds.forEach((fId, index) => {
                            newFields = updateFieldInTree(newFields, fId, {
                                layout: { rowId: targetRowId, columnIndex: index },
                            });
                        });

                        return { fields: newFields, rows: newRows, isDirty: true };
                    }

                    // Different row - remove from current row first
                    let newRows = state.rows.map(row => {
                        if (row.fieldIds.includes(fieldId)) {
                            return {
                                ...row,
                                fieldIds: row.fieldIds.filter(id => id !== fieldId),
                            };
                        }
                        return row;
                    }).filter(row => row.fieldIds.length > 0); // Remove empty rows

                    // Add to target row
                    newRows = newRows.map(row => {
                        if (row.id === targetRowId) {
                            const newFieldIds = [...row.fieldIds];
                            // Clamp index to valid range
                            const insertIndex = Math.min(targetColumnIndex, newFieldIds.length);
                            newFieldIds.splice(insertIndex, 0, fieldId);
                            return { ...row, fieldIds: newFieldIds };
                        }
                        return row;
                    });

                    // Update field layout
                    let newFields = updateFieldInTree(state.fields, fieldId, {
                        layout: { rowId: targetRowId, columnIndex: targetColumnIndex },
                    });

                    // Update all columnIndex values in the target row
                    const targetRow = newRows.find(r => r.id === targetRowId);
                    if (targetRow) {
                        targetRow.fieldIds.forEach((fId, index) => {
                            newFields = updateFieldInTree(newFields, fId, {
                                layout: { rowId: targetRowId, columnIndex: index },
                            });
                        });
                    }

                    console.log('mergeToRow completed:', { fieldId, targetRowId, targetColumnIndex, newRows });
                    return { fields: newFields, rows: newRows, isDirty: true };
                });
            },

            // Separate field to a new row (after specified row or at end)
            separateToNewRow: (fieldId: string, afterRowId?: string) => {
                get().createRowWithField(fieldId, afterRowId);
            },

            // Reorder rows
            reorderRows: (sourceRowIndex: number, destinationRowIndex: number) => {
                set(state => {
                    const newRows = [...state.rows];
                    const [removed] = newRows.splice(sourceRowIndex, 1);
                    newRows.splice(destinationRowIndex, 0, removed);
                    return { rows: newRows, isDirty: true };
                });
            },

            // Reorder columns within a row
            reorderColumnsInRow: (rowId: string, sourceColIndex: number, destColIndex: number) => {
                set(state => {
                    const newRows = state.rows.map(row => {
                        if (row.id === rowId) {
                            const newFieldIds = [...row.fieldIds];
                            const [removed] = newFieldIds.splice(sourceColIndex, 1);
                            newFieldIds.splice(destColIndex, 0, removed);
                            return { ...row, fieldIds: newFieldIds };
                        }
                        return row;
                    });

                    // Update columnIndex for all fields in this row
                    const targetRow = newRows.find(r => r.id === rowId);
                    let newFields = state.fields;
                    if (targetRow) {
                        targetRow.fieldIds.forEach((fId, index) => {
                            newFields = updateFieldInTree(newFields, fId, {
                                layout: { rowId, columnIndex: index },
                            });
                        });
                    }

                    return { fields: newFields, rows: newRows, isDirty: true };
                });
            },

            // Move field to a specific row at a specific column index
            moveFieldToRow: (fieldId: string, targetRowId: string, columnIndex: number) => {
                set(state => {
                    const field = findFieldById(state.fields, fieldId);
                    if (!field) return state;

                    // Get current row of the field
                    const currentRow = state.rows.find(r => r.fieldIds.includes(fieldId));

                    // If already in the same row, just reorder within row
                    if (currentRow?.id === targetRowId) {
                        const currentIndex = currentRow.fieldIds.indexOf(fieldId);
                        if (currentIndex !== columnIndex) {
                            get().reorderColumnsInRow(targetRowId, currentIndex, columnIndex);
                        }
                        return state;
                    }

                    // Otherwise, merge to the target row
                    get().mergeToRow(fieldId, targetRowId, columnIndex);
                    return get();
                });
            },

            // Select Field
            selectField: (id: string | null) => {
                set({ selectedFieldId: id });
            },

            // Get Sections (derived from x-ui.group)
            getSections: () => {
                const { fields } = get();
                const sectionMap = new Map<string, DesignedField[]>();
                const noGroupFields: DesignedField[] = [];

                fields.forEach(field => {
                    const group = field.xui?.group;
                    if (group) {
                        if (!sectionMap.has(group)) {
                            sectionMap.set(group, []);
                        }
                        sectionMap.get(group)!.push(field);
                    } else {
                        noGroupFields.push(field);
                    }
                });

                const sections: DesignedSection[] = [];
                let order = 0;

                // Fields without group first
                if (noGroupFields.length > 0) {
                    sections.push({
                        id: 'default',
                        name: 'General',
                        fields: noGroupFields,
                        order: order++,
                    });
                }

                // Grouped fields
                sectionMap.forEach((sectionFields, name) => {
                    sections.push({
                        id: `section_${name}`,
                        name,
                        fields: sectionFields,
                        order: order++,
                    });
                });

                return sections;
            },

            // Update Transport
            updateTransport: (transport) => {
                set(state => ({
                    transport: { ...state.transport, ...transport },
                    isDirty: true,
                }));
            },

            // Set Fields (for loading from schema)
            setFields: (fields: DesignedField[], rows?: DesignerRow[]) => {
                // If rows not provided, create one row per field
                const actualRows = rows || fields.map((field, index) => ({
                    id: field.layout?.rowId || generateRowId(),
                    fieldIds: [field.id],
                }));

                // Update field layouts if not set
                const updatedFields = fields.map((field, index) => {
                    if (!field.layout) {
                        return {
                            ...field,
                            layout: {
                                rowId: actualRows[index]?.id || generateRowId(),
                                columnIndex: 0,
                            },
                        };
                    }
                    return field;
                });

                set({ fields: updatedFields, rows: actualRows, isDirty: false });
            },

            // Set Dirty
            setDirty: (dirty: boolean) => {
                set({ isDirty: dirty });
            },

            // Reset
            reset: () => {
                set({
                    fields: [],
                    rows: [],
                    selectedFieldId: null,
                    isDirty: false,
                    transport: {},
                });
            },
        }),
        {
            name: 'schema-builder-storage-v2', // Changed key to clear old data
            partialize: (state) => ({
                fields: state.fields,
                rows: state.rows,
                transport: state.transport,
                _version: 2, // Version for future migrations
            }),
            // Migration: if rows is missing or empty but fields exist, recreate rows
            onRehydrateStorage: () => (state) => {
                if (state) {
                    // If we have fields but no rows, create rows from fields
                    if (state.fields.length > 0 && state.rows.length === 0) {
                        const rows: DesignerRow[] = state.fields.map((field, index) => ({
                            id: field.layout?.rowId || `row_migrated_${index}_${Date.now()}`,
                            fieldIds: [field.id],
                        }));
                        state.rows = rows;

                        // Update field layouts
                        state.fields = state.fields.map((field, index) => ({
                            ...field,
                            layout: {
                                rowId: rows[index].id,
                                columnIndex: 0,
                            },
                        }));
                    }
                }
            },
        }
    )
);

// ============================================================================
// Selectors
// ============================================================================

export const selectSelectedField = (state: DesignerStore) => {
    if (!state.selectedFieldId) return null;
    return findFieldById(state.fields, state.selectedFieldId);
};

export const selectFieldById = (id: string) => (state: DesignerStore) => {
    return findFieldById(state.fields, id);
};

export const selectFieldsByRowId = (rowId: string) => (state: DesignerStore) => {
    const row = state.rows.find(r => r.id === rowId);
    if (!row) return [];
    return row.fieldIds
        .map(id => findFieldById(state.fields, id))
        .filter((f): f is DesignedField => f !== null);
};
