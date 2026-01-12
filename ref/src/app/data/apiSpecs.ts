// API specifications data
export const apiSpecs: Record<string, any> = {
  nlct: {
    title: 'Nonlinear Analysis Control Data',
    category: 'DB',
    uri: '/db/NLCT',
    methods: ['POST', 'GET', 'PUT', 'DELETE'],
    description: 'Configure nonlinear analysis control settings for structural analysis in MIDAS Civil. Supports Newton-Raphson, Arc-Length, and Displacement-Control iteration methods.',
    
    // 🎯 Enhanced JSON Schema with conditional logic
    jsonSchemaEnhanced: {
      "NLCT": {
        "$schema": "http://json-schema.org/draft-07/schema#",
        "type": "object",
        "required": ["ENERGY_NORM", "DISPLACEMENT_NORM", "FORCE_NORM"],
        "properties": {
          "NONLINEAR_TYPE": {
            "description": "Nonlinear Type",
            "type": "string",
            "enum": ["GEOM", "MATL", "GEOM+MATL"],
            "default": "GEOM",
            "x-options": [
              "Geometry Nonlinear: \"GEOM\"",
              "Material Nonlinear: \"MATL\"",
              "Geometry/Material Nonlinear: \"GEOM+MATL\""
            ]
          },
          "ITERATION_METHOD": {
            "description": "Iteration Method",
            "type": "string",
            "enum": ["NEWTON", "ARC", "DISP"],
            "default": "NEWTON",
            "x-options": [
              "Newton-Raphson: \"NEWTON\"",
              "Arc-Length: \"ARC\"",
              "Displacement-Control: \"DISP\""
            ]
          },
          "OPT_ENERGY_NORM": {
            "description": "Energy Norm",
            "type": "boolean",
            "default": false,
            "x-options": ["Active/Inactive"]
          },
          "ENERGY_NORM": {
            "description": "Energy Norm",
            "type": "number"
          },
          "OPT_DISPLACEMENT_NORM": {
            "description": "Displacement Norm",
            "type": "boolean",
            "default": false,
            "x-options": ["Active/Inactive"]
          },
          "DISPLACEMENT_NORM": {
            "description": "Displacement Norm",
            "type": "number"
          },
          "OPT_FORCE_NORM": {
            "description": "Force Norm",
            "type": "boolean",
            "default": false,
            "x-options": ["Active/Inactive"]
          },
          "FORCE_NORM": {
            "description": "Force Norm",
            "type": "number"
          },
          
          // 🎯 Newton-Raphson specific fields
          "NUMBER_STEPS": {
            "description": "Number of Load Steps",
            "type": "integer",
            "minimum": 1,
            "x-condition": "ITERATION_METHOD === 'NEWTON'",
            "x-section": "Iteration Method: Newton-Raphson"
          },
          "MAX_ITERATIONS": {
            "description": "Maximum Number of Iterations/Load Step",
            "type": "integer",
            "minimum": 1,
            "x-condition": "ITERATION_METHOD === 'NEWTON'",
            "x-section": "Iteration Method: Newton-Raphson"
          },
          "NEWTON_ITEMS": {
            "description": "Load Case Specific Nonlinear Analysis Control Data - Newton-Raphson",
            "type": "array",
            "x-condition": "ITERATION_METHOD === 'NEWTON' || ITERATION_METHOD === 'DISP'",
            "x-section": "Iteration Method: Newton-Raphson",
            "x-note": "Insert the data as an object",
            "items": {
              "type": "object",
              "required": ["LCNAME", "NUMBER_STEPS", "MAX_ITERATIONS"],
              "properties": {
                "ITERATION_METHOD": {
                  "description": "Iteration Method",
                  "type": "string",
                  "enum": ["NEWTON"],
                  "default": "NEWTON",
                  "x-options": ["Newton-Raphson: \"NEWTON\""]
                },
                "LCNAME": {
                  "description": "Load Case Name",
                  "type": "string"
                },
                "NUMBER_STEPS": {
                  "description": "Number of Load Steps",
                  "type": "number",
                  "minimum": 1
                },
                "MAX_ITERATIONS": {
                  "description": "Maximum Number of Iterations/Load Step",
                  "type": "integer",
                  "minimum": 1
                },
                "LOAD_FACTORS": {
                  "description": "Load Factor",
                  "type": "array",
                  "default": [1],
                  "x-note": "Index: Step",
                  "items": {
                    "type": "number"
                  }
                }
              }
            }
          },
          
          // 🎯 Arc-Length specific fields
          "INITIAL_FORCE_RATIO_ARC_LEN": {
            "description": "Initial Force Ratio for Unit Arc-Length",
            "type": "number",
            "x-condition": "ITERATION_METHOD === 'ARC'",
            "x-section": "Iteration Method: Arc-Length"
          },
          "MAXIMUM_DISPLACEMENT": {
            "description": "Maximum Displacement Boundary",
            "type": "number",
            "default": 0,
            "x-condition": "ITERATION_METHOD === 'ARC' || ITERATION_METHOD === 'DISP'",
            "x-section": "Iteration Method: Arc-Length"
          },
          "ARCLEN_ITEMS": {
            "description": "Load Case Specific Nonlinear Analysis Control Data - Arc-Length",
            "type": "array",
            "x-condition": "ITERATION_METHOD === 'ARC'",
            "x-section": "Iteration Method: Arc-Length",
            "x-note": "Insert the data as an object",
            "items": {
              "type": "object",
              "required": ["LCNAME", "NUMBER_STEPS", "MAX_ITERATIONS"],
              "properties": {
                "ITERATION_METHOD": {
                  "description": "Iteration Method",
                  "type": "string",
                  "enum": ["ARC"],
                  "default": "ARC",
                  "x-options": ["Arc-Length: \"ARC\""]
                },
                "LCNAME": {
                  "description": "Load Case Name",
                  "type": "string"
                },
                "INITIAL_FORCE_RATIO_ARC_LEN": {
                  "description": "Initial Force Ratio for Unit Arc-Length",
                  "type": "number",
                  "default": 0
                },
                "NUMBER_STEPS": {
                  "description": "Number of Load Steps",
                  "type": "number",
                  "minimum": 1
                },
                "MAX_ITERATIONS": {
                  "description": "Maximum Number of Iterations/Load Step",
                  "type": "integer",
                  "minimum": 1
                },
                "MAXIMUM_DISPLACEMENT": {
                  "description": "Maximum Displacement Boundary",
                  "type": "number",
                  "default": 0
                }
              }
            }
          },
          
          // 🎯 Displacement-Control specific fields
          "MASTER_NODE": {
            "description": "Master Node ID Number",
            "type": "integer",
            "x-condition": "ITERATION_METHOD === 'DISP'",
            "x-section": "Iteration Method: Displacement-Control"
          },
          "DIRECTION": {
            "description": "Direction",
            "type": "integer",
            "default": 0,
            "x-condition": "ITERATION_METHOD === 'DISP'",
            "x-section": "Iteration Method: Displacement-Control",
            "x-options": [
              "Dx: 0",
              "Dy: 1",
              "Dz: 2"
            ]
          },
          "DISPCT_ITEMS": {
            "description": "Load Case Specific Nonlinear Analysis Control Data - Displacement-Control",
            "type": "array",
            "x-condition": "ITERATION_METHOD === 'DISP'",
            "x-section": "Iteration Method: Displacement-Control",
            "x-note": "Insert the data as an object",
            "items": {
              "type": "object",
              "required": ["LCNAME", "NUMBER_STEPS", "MAX_ITERATIONS", "MASTER_NODE", "MAXIMUM_DISPLACEMENT"],
              "properties": {
                "ITERATION_METHOD": {
                  "description": "Iteration Method",
                  "type": "string",
                  "enum": ["DISP"],
                  "default": "DISP",
                  "x-options": ["Newton-Raphson: \"DISP\""]
                },
                "LCNAME": {
                  "description": "Load Case Name",
                  "type": "string"
                },
                "NUMBER_STEPS": {
                  "description": "Number of Displacement Steps",
                  "type": "integer",
                  "minimum": 1
                },
                "MAX_ITERATIONS": {
                  "description": "Maximum Number of Iterations/Load Step",
                  "type": "number",
                  "minimum": 1
                },
                "MASTER_NODE": {
                  "description": "Master Node ID Number",
                  "type": "integer"
                },
                "DIRECTION": {
                  "description": "Direction",
                  "type": "integer",
                  "default": 0,
                  "x-options": [
                    "Dx: 0",
                    "Dy: 1",
                    "Dz: 2"
                  ]
                },
                "MAXIMUM_DISPLACEMENT": {
                  "description": "Maximum Displacement",
                  "type": "number"
                },
                "LOAD_FACTORS": {
                  "description": "Master Node Displacement",
                  "type": "array",
                  "default": [1],
                  "x-note": "Index: Step",
                  "items": {
                    "type": "number"
                  }
                }
              }
            }
          }
        }
      }
    },
    
    jsonSchema: {
      NLCT: {
        $schema: 'http://json-schema.org/draft-07/schema#',
        type: 'object',
        properties: {
          NONLINEAR_TYPE: { type: 'string', description: 'Nonlinear Type' },
          ITERATION_METHOD: { type: 'string', description: 'Iteration Method' },
          NUMBER_STEPS: { type: 'integer', description: 'Number of Load/Displacement Steps' },
          MAX_ITERATIONS: { type: 'integer', description: 'Maximum Number of Iterations/Load Step' },
          NEWTON_ITEMS: {
            type: 'array',
            items: {
              type: 'object',
              properties: {
                ITERATION_METHOD: { type: 'string' },
                LCNAME: { type: 'string' },
                NUMBER_STEPS: { type: 'number' },
                MAX_ITERATIONS: { type: 'integer' },
                LOAD_FACTORS: { type: 'array', items: { type: 'number' } }
              }
            }
          }
        }
      }
    },
    
    parameters: [
      // This is now auto-generated from jsonSchemaEnhanced
    ],
  },
  
  mvct: {
    title: 'Moving Load Control Data',
    category: 'DB',
    uri: '/db/MVCT',
    methods: ['POST', 'GET', 'PUT', 'DELETE'],
    description: 'Configure moving load settings for bridge analysis.',
    parameters: [
      {
        no: 1,
        name: 'CODE',
        description: 'Design Code',
        type: 'String',
        default: '-',
        required: 'Required',
        options: ['AASHTO', 'BS5400', 'EUROCODE', 'IRC', 'KBC', 'etc.'],
      },
      {
        no: 2,
        name: 'NUMLN',
        description: 'Number of Lanes',
        type: 'Integer',
        default: '-',
        required: 'Required',
      },
      {
        no: 3,
        name: 'LNSPAC',
        description: 'Lane Spacing',
        type: 'Number',
        default: '-',
        required: 'Required',
      },
    ],
    jsonSchema: {
      MVCT: {
        CODE: 'string',
        NUMLN: 'integer',
        LNSPAC: 'number',
        TRKLNWID: 'number',
        SNGMOV: 'string',
      },
    },
  },
};
