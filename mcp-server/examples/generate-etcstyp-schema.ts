/**
 * Test script to generate schema for CEtcStypDlg using save_schema
 */
import { saveSchema } from '../src/tools/save.js';

async function main() {
    console.log('🔨 Generating schema for CEtcStypDlg...\n');
    
    const result = await saveSchema({
        schemaName: 'CEtcStypDlg',
        schema: {
            "type": "object",
            "title": "Structure Type",
            "description": "Structure type and mass control parameters",
            "required": ["iSTRUC_TYPE", "iMASS_TYPE", "iSTRUC_MASS", "dGRAVITY", "dINIT_TEMP"],
            "properties": {
                "iSTRUC_TYPE": {
                    "type": "integer",
                    "description": "Structure type definition",
                    "enum": [0, 1, 2, 3, 4],
                    "x-enum-labels": {
                        "0": "3-D",
                        "1": "X-Z Plane",
                        "2": "Y-Z Plane",
                        "3": "X-Y Plane",
                        "4": "Constraint RZ"
                    },
                    "x-ui": {
                        "label": "Structure Type",
                        "group": "Structure Type",
                        "controlType": "radio"
                    }
                },
                "iMASS_TYPE": {
                    "type": "integer",
                    "description": "Mass type (Lumped or Consistent)",
                    "enum": [1, 2],
                    "x-enum-labels": {
                        "1": "Lumped Mass",
                        "2": "Consistent Mass"
                    },
                    "x-ui": {
                        "label": "Mass Type",
                        "group": "Mass Control Parameter",
                        "controlType": "radio"
                    }
                },
                "iSTRUC_MASS": {
                    "type": "integer",
                    "description": "Structure mass conversion type",
                    "enum": [1, 2, 3],
                    "x-enum-labels": {
                        "1": "Convert to X, Y, Z",
                        "2": "Convert to X, Y",
                        "3": "Convert to Z"
                    },
                    "x-ui": {
                        "label": "Structure Mass",
                        "group": "Mass Control Parameter",
                        "controlType": "radio",
                        "visibleWhen": {
                            "bCONV_SELFWT": true
                        }
                    }
                },
                "bCONSIDER_OFF": {
                    "type": "boolean",
                    "description": "Consider off-diagonal masses in lumped mass matrix",
                    "default": false,
                    "x-ui": {
                        "label": "Consider Off-diagonal Masses",
                        "group": "Mass Control Parameter",
                        "visibleWhen": {
                            "iMASS_TYPE": 1
                        }
                    }
                },
                "bCONV_SELFWT": {
                    "type": "boolean",
                    "description": "Convert self weight to masses",
                    "default": false,
                    "x-ui": {
                        "label": "Convert Self Weight to Masses",
                        "group": "Mass Control Parameter"
                    }
                },
                "bALIGN_BEAM": {
                    "type": "boolean",
                    "description": "Align beam to floor level",
                    "default": false,
                    "x-ui": {
                        "label": "Align Beam (Floor)",
                        "group": "Mass Control Parameter"
                    }
                },
                "bALIGN_SLAB": {
                    "type": "boolean",
                    "description": "Align slab to floor level",
                    "default": false,
                    "x-ui": {
                        "label": "Align Slab (Floor)",
                        "group": "Mass Control Parameter"
                    }
                },
                "bCONSIDER_ROT_MPF": {
                    "type": "boolean",
                    "description": "Consider rotational rigid body mode for modal participation factor",
                    "default": false,
                    "x-ui": {
                        "label": "Consider Rotational Rigid Body Mode for MPF",
                        "group": "Mass Control Parameter"
                    }
                },
                "dGRAVITY": {
                    "type": "number",
                    "description": "Gravity acceleration",
                    "x-ui": {
                        "label": "Gravity Acceleration",
                        "group": "Mass Control Parameter",
                        "controlType": "number",
                        "unit": "Acceleration"
                    }
                },
                "dINIT_TEMP": {
                    "type": "number",
                    "description": "Initial temperature",
                    "x-ui": {
                        "label": "Initial Temperature",
                        "group": "Mass Control Parameter",
                        "controlType": "number",
                        "unit": "Temperature"
                    }
                }
            },
            "allOf": [
                {
                    "description": "If mass type is Consistent Mass, disable structure mass options",
                    "if": {
                        "properties": {
                            "iMASS_TYPE": { "const": 2 }
                        }
                    },
                    "then": {
                        "properties": {
                            "iSTRUC_MASS": { "readOnly": true },
                            "bCONSIDER_OFF": { "readOnly": true }
                        }
                    }
                },
                {
                    "description": "Enable Consider Rotational MPF only if Lumped Mass and not considering offset",
                    "if": {
                        "allOf": [
                            {
                                "properties": {
                                    "iMASS_TYPE": { "const": 1 }
                                }
                            },
                            {
                                "properties": {
                                    "bCONSIDER_OFF": { "const": false }
                                }
                            }
                        ]
                    },
                    "then": {
                        "properties": {
                            "bCONSIDER_ROT_MPF": { "readOnly": false }
                        }
                    },
                    "else": {
                        "properties": {
                            "bCONSIDER_ROT_MPF": { "readOnly": true }
                        }
                    }
                },
                {
                    "description": "Enable structure mass options only if Convert Self Weight is enabled",
                    "if": {
                        "properties": {
                            "bCONV_SELFWT": { "const": true }
                        }
                    },
                    "then": {
                        "properties": {
                            "iSTRUC_MASS": { "readOnly": false }
                        }
                    },
                    "else": {
                        "properties": {
                            "iSTRUC_MASS": { "readOnly": true }
                        }
                    }
                }
            ]
        },
        outputDir: 'default',
        entityType: 'collection',  // ✅ patternProperties 구조 사용
        bodyRoot: 'Assign'
    });
    
    console.log(JSON.stringify(result, null, 2));
    
    if (result.ok) {
        console.log('\n✅ Schema generated successfully!');
        console.log(`📄 Output: ${result.outputPath}`);
    } else {
        console.error('\n❌ Schema generation failed:', result.error);
    }
}

main().catch(console.error);
