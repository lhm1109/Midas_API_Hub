// Global data store for the application

export type AttributeType = 'text' | 'number' | 'boolean' | 'date' | 'select';

export interface Attribute {
  id: string;
  name: string;
  type: AttributeType;
  required: boolean;
  options?: string[]; // For select type
}

export interface ObjectType {
  id: string;
  name: string;
  description: string;
  icon: string;
  color: string;
  attributes: Attribute[];
  createdAt: string;
}

export interface DataRecord {
  id: string;
  objectTypeId: string;
  values: Record<string, any>;
  createdAt: string;
}

export interface Relationship {
  id: string;
  name: string;
  fromObjectTypeId: string;
  toObjectTypeId: string;
  type: 'one-to-one' | 'one-to-many' | 'many-to-many';
  description: string;
  createdAt: string;
}

export interface RelationshipLink {
  id: string;
  relationshipId: string;
  fromRecordId: string;
  toRecordId: string;
  createdAt: string;
}

// Mock initial data
const initialObjectTypes: ObjectType[] = [
  {
    id: 'obj-1',
    name: 'Design Parameter',
    description: 'Engineering design parameters',
    icon: 'IconAdjustments',
    color: 'blue',
    attributes: [
      { id: 'attr-1', name: 'name', type: 'text', required: true },
      { id: 'attr-2', name: 'type', type: 'select', required: true, options: ['General', 'Load', 'Member', 'Strength', 'Section'] },
      { id: 'attr-3', name: 'enabled', type: 'boolean', required: true },
    ],
    createdAt: new Date().toISOString(),
  },
  {
    id: 'obj-2',
    name: 'Steel Code',
    description: 'Steel design codes and standards',
    icon: 'IconCode',
    color: 'green',
    attributes: [
      { id: 'attr-4', name: 'code', type: 'text', required: true },
      { id: 'attr-5', name: 'country', type: 'text', required: true },
      { id: 'attr-6', name: 'type', type: 'text', required: true },
      { id: 'attr-7', name: 'year', type: 'number', required: false },
    ],
    createdAt: new Date().toISOString(),
  },
  {
    id: 'obj-3',
    name: 'RcDesign',
    description: 'Concrete design codes by panel',
    icon: 'IconSettings',
    color: 'orange',
    attributes: [
      { id: 'attr-8', name: '패널', type: 'text', required: true },
      { id: 'attr-9', name: '지원 코드', type: 'text', required: true },
      { id: 'attr-10', name: '코드명', type: 'text', required: true },
      { id: 'attr-11', name: '비고', type: 'text', required: false },
    ],
    createdAt: new Date().toISOString(),
  },
  {
    id: 'obj-4',
    name: 'RcDesignParameter',
    description: 'RC Design parameters by panel and section',
    icon: 'IconAdjustments',
    color: 'violet',
    attributes: [
      { id: 'attr-12', name: '패널', type: 'text', required: true },
      { id: 'attr-13', name: '패널 섹션', type: 'text', required: true },
      { id: 'attr-14', name: '파라미터명', type: 'text', required: true },
      { id: 'attr-15', name: '비고', type: 'text', required: false },
    ],
    createdAt: new Date().toISOString(),
  },
];

// Initial RcDesign data
const rcDesignData = [
  { panel: 'RcDgn_A', code: 'KSCE-LSD15', name: '한국 최신 코드', remark: '' },
  { panel: 'RcDgn_A', code: 'KCI-USD12', name: '한국 최신 코드', remark: '' },
  { panel: 'RcDgn_A', code: 'KSCE-USD10', name: '한국 최신 코드', remark: '' },
  { panel: 'RcDgn_A', code: 'KSCE-RAIL-USD11', name: '한국 최신 코드', remark: '' },
  { panel: 'RcDgn_A', code: 'KDS 24 14 21 : 2021', name: '한국 최신 코드', remark: '' },
  { panel: 'RcDgn_A', code: 'KDS 14 20 00 : 2022', name: '한국 최신 코드', remark: '' },
  { panel: 'RcDgn_B', code: 'KCI-USD07', name: '한국 중간 버전', remark: '' },
  { panel: 'RcDgn_B', code: 'KSCE-USD05', name: '한국 중간 버전', remark: '' },
  { panel: 'RcDgn_B', code: 'KSCE-RAIL-USD04', name: '한국 중간 버전', remark: '' },
  { panel: 'RcDgn_C', code: 'KCI-USD99', name: '한국 구버전 대만', remark: '' },
  { panel: 'RcDgn_C', code: 'KSCE-USD96', name: '한국 구버전 대만', remark: '' },
  { panel: 'RcDgn_C', code: 'TWN-BRG-LSD90', name: '한국 구버전 대만', remark: '' },
  { panel: 'RcDgn_D', code: 'Eurocode2-2:05', name: '유럽 남아공', remark: '' },
  { panel: 'RcDgn_D', code: 'BS5400-4:1990', name: '유럽 남아공', remark: '' },
  { panel: 'RcDgn_D', code: 'TMH07-3:1989', name: '유럽 남아공', remark: '' },
  { panel: 'RcDgn_E', code: 'ACI318-02', name: '미국 캐나다 인도', remark: '' },
  { panel: 'RcDgn_E', code: 'JTJ023-85', name: '미국 캐나다 인도', remark: '' },
  { panel: 'RcDgn_E', code: 'CSA-S6-00', name: '미국 캐나다 인도', remark: '' },
  { panel: 'RcDgn_E', code: 'CSA-S6-19', name: '미국 캐나다 인도', remark: '' },
  { panel: 'RcDgn_E', code: 'CSA-S6-14', name: '미국 캐나다 인도', remark: '' },
  { panel: 'RcDgn_E', code: 'IRC:21-2000', name: '미국 캐나다 인도', remark: '' },
  { panel: 'RcDgn_E', code: 'IRS', name: '미국 캐나다 인도', remark: '' },
  { panel: 'RcDgn_F', code: 'SNiP 2.05.03-84*', name: '러시아', remark: '' },
  { panel: 'RcDgn_F', code: 'SP 35.13330.2011', name: '러시아', remark: '' },
  { panel: 'RcDgn_F', code: 'SNiP 2.05.03-84*(MKS)', name: '러시아', remark: '' },
  { panel: 'RcDgn_F', code: 'SP 35.13330.2011(MKS)', name: '러시아', remark: '' },
  { panel: 'RcDgn_G', code: 'IRC:112-2011', name: '인도', remark: '' },
  { panel: 'RcDgn_G', code: 'IS456:2000', name: '인도', remark: '' },
  { panel: 'RcDgn_G', code: 'IRC:112-2020', name: '인도', remark: '' },
  { panel: 'RcDgn_H', code: 'AASHTO-LRFD17', name: 'AASHTO 최신 버전', remark: '' },
  { panel: 'RcDgn_H', code: 'AASHTO-LRFD16', name: 'AASHTO 최신 버전', remark: '' },
  { panel: 'RcDgn_H', code: 'AASHTO-LRFD12', name: 'AASHTO 최신 버전', remark: '' },
  { panel: 'RcDgn_H', code: 'AASHTO-LRFD20', name: 'AASHTO 최신 버전', remark: '' },
  { panel: 'RcDgn_H', code: 'AASHTO-LRFD24', name: 'AASHTO 최신 버전', remark: '' },
  { panel: 'RcDgn_I', code: 'AASHTO-LRFD07', name: 'AASHTO 구버전', remark: '' },
  { panel: 'RcDgn_I', code: 'AASHTO-LRFD02', name: 'AASHTO 구버전', remark: '' },
  { panel: 'RcDgn_I', code: 'AASHTO-LFD96', name: 'AASHTO 구버전', remark: '' },
  { panel: 'RcDgn_J', code: 'AREMA-2023', name: 'AREMA', remark: '' },
  { panel: 'RcDgn_K', code: 'AS 5100.5:17', name: '호주', remark: '' },
];

// Initial RcDesignParameter data
const rcDesignParameterData = [
  { panel: 'RcDgn_A', section: 'Design Input Data', param: 'Design Code Option', remark: '' },
  { panel: 'RcDgn_A', section: 'Design Input Data', param: 'Common Design Parameters (공통)', remark: '' },
  { panel: 'RcDgn_A', section: 'Design Input Data', param: 'Load Combination Type', remark: '' },
  { panel: 'RcDgn_A', section: 'Design Input Data', param: 'Member Assignment (Plate)', remark: '' },
  { panel: 'RcDgn_A', section: 'Design Input Data', param: 'Modify RC Material', remark: '' },
  { panel: 'RcDgn_A', section: 'Rebar 패널', param: 'Rebar Input - Beam Section', remark: '' },
  { panel: 'RcDgn_A', section: 'Rebar 패널', param: 'Rebar Input - Column Section', remark: '' },
  { panel: 'RcDgn_A', section: 'Rebar 패널', param: 'Rebar Input - General Section', remark: '' },
  { panel: 'RcDgn_A', section: 'Table 패널', param: 'Common Parameter Tables', remark: '' },
  { panel: 'RcDgn_A', section: 'Perform 패널', param: 'Perform Design', remark: '' },
  { panel: 'RcDgn_A', section: 'Design Section 패널', param: 'Section for Design', remark: '' },
  { panel: 'RcDgn_A', section: 'Batch Processing 패널', param: 'Perform Batch Design', remark: '' },
  { panel: 'RcDgn_B', section: 'Design Input Data', param: 'Design Code Option', remark: '' },
  { panel: 'RcDgn_B', section: 'Design Input Data', param: 'Common Design Parameters (공통)', remark: '' },
  { panel: 'RcDgn_B', section: 'Design Input Data', param: 'Strength Reduction Factor', remark: '' },
  { panel: 'RcDgn_B', section: 'Design Input Data', param: 'Modify RC Material', remark: '' },
  { panel: 'RcDgn_B', section: 'Rebar 패널', param: 'Rebar Input - Beam Section', remark: '' },
  { panel: 'RcDgn_B', section: 'Rebar 패널', param: 'Rebar Input - Column Section', remark: '' },
  { panel: 'RcDgn_B', section: 'Rebar 패널', param: 'Rebar Input - General Section', remark: '' },
  { panel: 'RcDgn_B', section: 'Table 패널', param: 'Common Parameter Tables', remark: '' },
  { panel: 'RcDgn_B', section: 'Perform 패널', param: 'Perform Design - RC Design Parameter', remark: '' },
  { panel: 'RcDgn_B', section: 'Perform 패널', param: 'Perform Design - RC Plate Parameter', remark: '' },
  { panel: 'RcDgn_B', section: 'Design Section 패널', param: 'Section for Design', remark: '' },
  { panel: 'RcDgn_B', section: 'Batch Processing 패널', param: 'Perform Batch Design', remark: '' },
  { panel: 'RcDgn_C', section: 'Design Input Data', param: 'Design Code Option', remark: '' },
  { panel: 'RcDgn_C', section: 'Design Input Data', param: 'Common Design Parameters (공통)', remark: '' },
  { panel: 'RcDgn_C', section: 'Design Input Data', param: 'Strength Reduction Factor', remark: '' },
  { panel: 'RcDgn_C', section: 'Design Input Data', param: 'Maximum Rebar Ratio', remark: '' },
  { panel: 'RcDgn_C', section: 'Design Input Data', param: 'Limiting Minimum Section Size', remark: '' },
  { panel: 'RcDgn_C', section: 'Design Input Data', param: 'Column Scale Up Factor', remark: '' },
  { panel: 'RcDgn_C', section: 'Design Input Data', param: 'Serviceability Parameters', remark: '' },
  { panel: 'RcDgn_C', section: 'Design Input Data', param: 'Modify RC Material', remark: '' },
  { panel: 'RcDgn_C', section: 'Rebar 패널', param: 'Rebar Input for Design - Beam Section', remark: '' },
  { panel: 'RcDgn_C', section: 'Rebar 패널', param: 'Rebar Input for Design - Column Section', remark: '' },
  { panel: 'RcDgn_C', section: 'Rebar 패널', param: 'Rebar Input for Checking - Beam Section', remark: '' },
  { panel: 'RcDgn_C', section: 'Rebar 패널', param: 'Rebar Input for Checking - Column Section', remark: '' },
  { panel: 'RcDgn_C', section: 'Rebar 패널', param: 'Rebar Input for Checking - General Section', remark: 'TWN-BRG-LSD90 제외' },
  { panel: 'RcDgn_C', section: 'Table 패널', param: 'Common Parameter Tables', remark: '' },
  { panel: 'RcDgn_C', section: 'Table 패널', param: 'Column Scale Up Factor Table', remark: '' },
  { panel: 'RcDgn_C', section: 'Perform 패널', param: 'Concrete Code Design - Beam Design', remark: '' },
  { panel: 'RcDgn_C', section: 'Perform 패널', param: 'Concrete Code Design - Column Design', remark: '' },
  { panel: 'RcDgn_C', section: 'Perform 패널', param: 'Concrete Code Check - Beam Check', remark: '' },
  { panel: 'RcDgn_C', section: 'Perform 패널', param: 'Concrete Code Check - Column Check', remark: '' },
  { panel: 'RcDgn_C', section: 'Design Section 패널', param: 'Section for Design', remark: '' },
  { panel: 'RcDgn_C', section: 'Batch Processing 패널', param: 'Perform Batch Design', remark: '' },
  // RcDgn_D has many parameters, adding them all
  { panel: 'RcDgn_D', section: 'Design Input Data', param: 'Design Code Option', remark: '' },
  { panel: 'RcDgn_D', section: 'Design Input Data', param: 'Common Design Parameters (공통)', remark: '' },
  { panel: 'RcDgn_D', section: 'Design Input Data', param: 'Strength Reduction Factor', remark: '' },
  { panel: 'RcDgn_D', section: 'Design Input Data', param: 'Maximum Rebar Ratio', remark: '' },
  { panel: 'RcDgn_D', section: 'Design Input Data', param: 'Limiting Minimum Section Size', remark: '' },
  { panel: 'RcDgn_D', section: 'Design Input Data', param: 'Column Scale Up Factor', remark: '' },
  { panel: 'RcDgn_D', section: 'Design Input Data', param: 'Serviceability Parameters', remark: '' },
  { panel: 'RcDgn_D', section: 'Design Input Data', param: 'Design Criteria for Rebars', remark: '' },
  { panel: 'RcDgn_D', section: 'Design Input Data', param: 'Serviceability Load Combination Type', remark: '' },
  { panel: 'RcDgn_D', section: 'Design Input Data', param: 'Modify RC Material', remark: '' },
  { panel: 'RcDgn_D', section: 'Rebar 패널', param: 'Rebar Input for Design - Beam Section', remark: '' },
  { panel: 'RcDgn_D', section: 'Rebar 패널', param: 'Rebar Input for Design - Column Section', remark: '' },
  { panel: 'RcDgn_D', section: 'Rebar 패널', param: 'Rebar Input for Checking - Beam Section', remark: '' },
  { panel: 'RcDgn_D', section: 'Rebar 패널', param: 'Rebar Input for Checking - Column Section', remark: '' },
  { panel: 'RcDgn_D', section: 'Rebar 패널', param: 'Rebar Input for Checking - General Section', remark: '' },
  { panel: 'RcDgn_D', section: 'Rebar 패널', param: 'Rebar Input for Plate - Plate Beam Design', remark: '' },
  { panel: 'RcDgn_D', section: 'Rebar 패널', param: 'Rebar Input for Plate - Plate Beam Rebar', remark: '' },
  { panel: 'RcDgn_D', section: 'Rebar 패널', param: 'Rebar Input for Plate - Plate Beam Check', remark: '' },
  { panel: 'RcDgn_D', section: 'Rebar 패널', param: 'Rebar Input for Plate - Plate Column Design', remark: '' },
  { panel: 'RcDgn_D', section: 'Rebar 패널', param: 'Rebar Input for Plate - Plate Column Rebar', remark: '' },
  { panel: 'RcDgn_D', section: 'Rebar 패널', param: 'Rebar Input for Plate - Plate Column Check', remark: '' },
  { panel: 'RcDgn_D', section: 'Table 패널', param: 'Common Parameter Tables', remark: '' },
  { panel: 'RcDgn_D', section: 'Table 패널', param: 'Column Scale Up Factor Table', remark: '' },
  { panel: 'RcDgn_D', section: 'Perform 패널', param: 'Concrete Code Design - Beam Design', remark: '' },
  { panel: 'RcDgn_D', section: 'Perform 패널', param: 'Concrete Code Design - Column Design', remark: '' },
  { panel: 'RcDgn_D', section: 'Perform 패널', param: 'Concrete Code Design - Plate Beam Design', remark: '' },
  { panel: 'RcDgn_D', section: 'Perform 패널', param: 'Concrete Code Design - Plate Column Design', remark: '' },
  { panel: 'RcDgn_D', section: 'Perform 패널', param: 'Concrete Code Check - Beam Check', remark: '' },
  { panel: 'RcDgn_D', section: 'Perform 패널', param: 'Concrete Code Check - Column Check', remark: '' },
  { panel: 'RcDgn_D', section: 'Perform 패널', param: 'Concrete Code Check - Plate Beam Check', remark: '' },
  { panel: 'RcDgn_D', section: 'Perform 패널', param: 'Concrete Code Check - Plate Column Check', remark: '' },
  { panel: 'RcDgn_D', section: 'Mesh 패널', param: 'Meshed Design - Design Criteria for Rebars', remark: 'Eurocode2-2:05 Shell 국가' },
  { panel: 'RcDgn_D', section: 'Mesh 패널', param: 'Meshed Design - Slab/Shell Rebars for Checking', remark: 'Eurocode2-2:05 Shell 국가' },
  { panel: 'RcDgn_D', section: 'Mesh 패널', param: 'Meshed Design - Shell Design', remark: 'Eurocode2-2:05 Shell 국가' },
  { panel: 'RcDgn_D', section: 'Mesh 패널', param: 'Meshed Design - Shell Checking', remark: 'Eurocode2-2:05 Shell 국가' },
  { panel: 'RcDgn_D', section: 'Mesh 패널', param: 'Meshed Design - Shell Shear Checking', remark: 'Eurocode2-2:05 Shell 국가' },
  { panel: 'RcDgn_D', section: 'Design Section 패널', param: 'Section for Design', remark: '' },
  { panel: 'RcDgn_D', section: 'Batch Processing 패널', param: 'Perform Batch Design', remark: '' },
  // Continue with remaining panels...
  { panel: 'RcDgn_E', section: 'Design Input Data', param: 'Design Code Option', remark: '' },
  { panel: 'RcDgn_E', section: 'Design Input Data', param: 'Common Design Parameters (공통)', remark: '' },
  { panel: 'RcDgn_E', section: 'Design Input Data', param: 'Strength Reduction Factor', remark: '' },
  { panel: 'RcDgn_E', section: 'Design Input Data', param: 'Maximum Rebar Ratio', remark: '' },
  { panel: 'RcDgn_E', section: 'Design Input Data', param: 'Limiting Minimum Section Size', remark: '' },
  { panel: 'RcDgn_E', section: 'Design Input Data', param: 'Column Scale Up Factor', remark: '' },
  { panel: 'RcDgn_E', section: 'Design Input Data', param: 'Serviceability Parameters', remark: '' },
  { panel: 'RcDgn_E', section: 'Design Input Data', param: 'Modify RC Material', remark: '' },
  { panel: 'RcDgn_E', section: 'Rebar 패널', param: 'Rebar Input for Design - Beam Section', remark: '' },
  { panel: 'RcDgn_E', section: 'Rebar 패널', param: 'Rebar Input for Design - Column Section', remark: '' },
  { panel: 'RcDgn_E', section: 'Rebar 패널', param: 'Rebar Input for Checking - Beam Section', remark: '' },
  { panel: 'RcDgn_E', section: 'Rebar 패널', param: 'Rebar Input for Checking - Column Section', remark: '' },
  { panel: 'RcDgn_E', section: 'Table 패널', param: 'Common Parameter Tables', remark: '' },
  { panel: 'RcDgn_E', section: 'Table 패널', param: 'Column Scale Up Factor Table', remark: '' },
  { panel: 'RcDgn_E', section: 'Perform 패널', param: 'Concrete Code Design - Beam Design', remark: '' },
  { panel: 'RcDgn_E', section: 'Perform 패널', param: 'Concrete Code Design - Column Design', remark: '' },
  { panel: 'RcDgn_E', section: 'Perform 패널', param: 'Concrete Code Check - Beam Check', remark: '' },
  { panel: 'RcDgn_E', section: 'Perform 패널', param: 'Concrete Code Check - Column Check', remark: '' },
  { panel: 'RcDgn_E', section: 'Design Section 패널', param: 'Section for Design', remark: '' },
  { panel: 'RcDgn_E', section: 'Batch Processing 패널', param: 'Perform Batch Design', remark: '' },
  // RcDgn_F
  { panel: 'RcDgn_F', section: 'Design Input Data', param: 'Design Code Option', remark: '' },
  { panel: 'RcDgn_F', section: 'Design Input Data', param: 'Common Design Parameters (공통)', remark: '' },
  { panel: 'RcDgn_F', section: 'Design Input Data', param: 'Load Combination Type', remark: '' },
  { panel: 'RcDgn_F', section: 'Design Input Data', param: 'Maximum Rebar Ratio', remark: '' },
  { panel: 'RcDgn_F', section: 'Design Input Data', param: 'Limiting Minimum Section Size', remark: '' },
  { panel: 'RcDgn_F', section: 'Design Input Data', param: 'Strength Reduction Factor (RUS)', remark: '' },
  { panel: 'RcDgn_F', section: 'Design Input Data', param: 'Limit State Group 2 Parameters', remark: '' },
  { panel: 'RcDgn_F', section: 'Design Input Data', param: 'Operation Condition Factor', remark: '' },
  { panel: 'RcDgn_F', section: 'Design Input Data', param: 'Modify RC Material', remark: '' },
  { panel: 'RcDgn_F', section: 'Rebar 패널', param: 'Rebar Input for Design - Beam Section', remark: '' },
  { panel: 'RcDgn_F', section: 'Rebar 패널', param: 'Rebar Input for Design - Column Section', remark: '' },
  { panel: 'RcDgn_F', section: 'Rebar 패널', param: 'Rebar Input for Checking - Beam Section', remark: '' },
  { panel: 'RcDgn_F', section: 'Rebar 패널', param: 'Rebar Input for Checking - Column Section', remark: '' },
  { panel: 'RcDgn_F', section: 'Rebar 패널', param: 'Rebar Input for Checking - General Section', remark: '' },
  { panel: 'RcDgn_F', section: 'Table 패널', param: 'Common Parameter Tables', remark: '' },
  { panel: 'RcDgn_F', section: 'Perform 패널', param: 'Concrete Code Design - Beam Design', remark: '' },
  { panel: 'RcDgn_F', section: 'Perform 패널', param: 'Concrete Code Design - Column Design', remark: '' },
  { panel: 'RcDgn_F', section: 'Perform 패널', param: 'Concrete Code Check - Beam Check', remark: '' },
  { panel: 'RcDgn_F', section: 'Perform 패널', param: 'Concrete Code Check - Column Check', remark: '' },
  { panel: 'RcDgn_F', section: 'Design Section 패널', param: 'Section for Design', remark: '' },
  { panel: 'RcDgn_F', section: 'Batch Processing 패널', param: 'Perform Batch Design', remark: '' },
  // RcDgn_G through K follow similar pattern
  { panel: 'RcDgn_G', section: 'Design Input Data', param: 'Design Code Option', remark: '' },
  { panel: 'RcDgn_G', section: 'Design Input Data', param: 'Common Design Parameters (공통)', remark: '' },
  { panel: 'RcDgn_G', section: 'Design Input Data', param: 'Strength Reduction Factor', remark: '' },
  { panel: 'RcDgn_G', section: 'Design Input Data', param: 'Maximum Rebar Ratio', remark: '' },
  { panel: 'RcDgn_G', section: 'Design Input Data', param: 'Limiting Minimum Section Size', remark: '' },
  { panel: 'RcDgn_G', section: 'Design Input Data', param: 'Serviceability Parameters', remark: '' },
  { panel: 'RcDgn_G', section: 'Design Input Data', param: 'Modify RC Material', remark: '' },
  { panel: 'RcDgn_G', section: 'Rebar 패널', param: 'Rebar Input for Design - Beam Section', remark: '' },
  { panel: 'RcDgn_G', section: 'Rebar 패널', param: 'Rebar Input for Design - Column Section', remark: '' },
  { panel: 'RcDgn_G', section: 'Rebar 패널', param: 'Rebar Input for Checking - Beam Section', remark: '' },
  { panel: 'RcDgn_G', section: 'Rebar 패널', param: 'Rebar Input for Checking - Column Section', remark: '' },
  { panel: 'RcDgn_G', section: 'Rebar 패널', param: 'Rebar Input for Checking - General Section', remark: '' },
  { panel: 'RcDgn_G', section: 'Table 패널', param: 'Common Parameter Tables', remark: '' },
  { panel: 'RcDgn_G', section: 'Perform 패널', param: 'Concrete Code Design - Beam Design', remark: '' },
  { panel: 'RcDgn_G', section: 'Perform 패널', param: 'Concrete Code Design - Column Design', remark: '' },
  { panel: 'RcDgn_G', section: 'Perform 패널', param: 'Concrete Code Check - Beam Check', remark: '' },
  { panel: 'RcDgn_G', section: 'Perform 패널', param: 'Concrete Code Check - Column Check', remark: '' },
  { panel: 'RcDgn_G', section: 'Design Section 패널', param: 'Section for Design', remark: '' },
  { panel: 'RcDgn_G', section: 'Batch Processing 패널', param: 'Perform Batch Design', remark: '' },
  // RcDgn_H, I, J, K - similar structure
  { panel: 'RcDgn_H', section: 'Design Input Data', param: 'Design Code Option', remark: '' },
  { panel: 'RcDgn_H', section: 'Design Input Data', param: 'Common Design Parameters (공통)', remark: '' },
  { panel: 'RcDgn_H', section: 'Design Input Data', param: 'Strength Reduction Factor', remark: '' },
  { panel: 'RcDgn_H', section: 'Design Input Data', param: 'Maximum Rebar Ratio', remark: '' },
  { panel: 'RcDgn_H', section: 'Design Input Data', param: 'Limiting Minimum Section Size', remark: '' },
  { panel: 'RcDgn_H', section: 'Design Input Data', param: 'Serviceability Parameters', remark: '' },
  { panel: 'RcDgn_H', section: 'Design Input Data', param: 'Modify RC Material', remark: '' },
  { panel: 'RcDgn_H', section: 'Rebar 패널', param: 'Rebar Input for Design - Beam Section', remark: '' },
  { panel: 'RcDgn_H', section: 'Rebar 패널', param: 'Rebar Input for Design - Column Section', remark: '' },
  { panel: 'RcDgn_H', section: 'Rebar 패널', param: 'Rebar Input for Checking - Beam Section', remark: '' },
  { panel: 'RcDgn_H', section: 'Rebar 패널', param: 'Rebar Input for Checking - Column Section', remark: '' },
  { panel: 'RcDgn_H', section: 'Rebar 패널', param: 'Rebar Input for Checking - General Section', remark: '' },
  { panel: 'RcDgn_H', section: 'Table 패널', param: 'Common Parameter Tables', remark: '' },
  { panel: 'RcDgn_H', section: 'Perform 패널', param: 'Concrete Code Design - Beam Design', remark: '' },
  { panel: 'RcDgn_H', section: 'Perform 패널', param: 'Concrete Code Design - Column Design', remark: '' },
  { panel: 'RcDgn_H', section: 'Perform 패널', param: 'Concrete Code Check - Beam Check', remark: '' },
  { panel: 'RcDgn_H', section: 'Perform 패널', param: 'Concrete Code Check - Column Check', remark: '' },
  { panel: 'RcDgn_H', section: 'Design Section 패널', param: 'Section for Design', remark: '' },
  { panel: 'RcDgn_H', section: 'Batch Processing 패널', param: 'Perform Batch Design', remark: '' },
  { panel: 'RcDgn_I', section: 'Design Input Data', param: 'Design Code Option', remark: '' },
  { panel: 'RcDgn_I', section: 'Design Input Data', param: 'Common Design Parameters (공통)', remark: '' },
  { panel: 'RcDgn_I', section: 'Design Input Data', param: 'Strength Reduction Factor', remark: '' },
  { panel: 'RcDgn_I', section: 'Design Input Data', param: 'Maximum Rebar Ratio', remark: '' },
  { panel: 'RcDgn_I', section: 'Design Input Data', param: 'Limiting Minimum Section Size', remark: '' },
  { panel: 'RcDgn_I', section: 'Design Input Data', param: 'Serviceability Parameters', remark: '' },
  { panel: 'RcDgn_I', section: 'Design Input Data', param: 'Modify RC Material', remark: '' },
  { panel: 'RcDgn_I', section: 'Rebar 패널', param: 'Rebar Input for Design - Beam Section', remark: '' },
  { panel: 'RcDgn_I', section: 'Rebar 패널', param: 'Rebar Input for Design - Column Section', remark: '' },
  { panel: 'RcDgn_I', section: 'Rebar 패널', param: 'Rebar Input for Checking - Beam Section', remark: '' },
  { panel: 'RcDgn_I', section: 'Rebar 패널', param: 'Rebar Input for Checking - Column Section', remark: '' },
  { panel: 'RcDgn_I', section: 'Rebar 패널', param: 'Rebar Input for Checking - General Section', remark: '' },
  { panel: 'RcDgn_I', section: 'Table 패널', param: 'Common Parameter Tables', remark: '' },
  { panel: 'RcDgn_I', section: 'Perform 패널', param: 'Concrete Code Design - Beam Design', remark: '' },
  { panel: 'RcDgn_I', section: 'Perform 패널', param: 'Concrete Code Design - Column Design', remark: '' },
  { panel: 'RcDgn_I', section: 'Perform 패널', param: 'Concrete Code Check - Beam Check', remark: '' },
  { panel: 'RcDgn_I', section: 'Perform 패널', param: 'Concrete Code Check - Column Check', remark: '' },
  { panel: 'RcDgn_I', section: 'Design Section 패널', param: 'Section for Design', remark: '' },
  { panel: 'RcDgn_I', section: 'Batch Processing 패널', param: 'Perform Batch Design', remark: '' },
  { panel: 'RcDgn_J', section: 'Design Input Data', param: 'Design Code Option', remark: '' },
  { panel: 'RcDgn_J', section: 'Design Input Data', param: 'Common Design Parameters (공통)', remark: '' },
  { panel: 'RcDgn_J', section: 'Design Input Data', param: 'Strength Reduction Factor', remark: '' },
  { panel: 'RcDgn_J', section: 'Design Input Data', param: 'Maximum Rebar Ratio', remark: '' },
  { panel: 'RcDgn_J', section: 'Design Input Data', param: 'Limiting Minimum Section Size', remark: '' },
  { panel: 'RcDgn_J', section: 'Design Input Data', param: 'Serviceability Parameters', remark: '' },
  { panel: 'RcDgn_J', section: 'Design Input Data', param: 'Modify RC Material', remark: '' },
  { panel: 'RcDgn_J', section: 'Rebar 패널', param: 'Rebar Input for Design - Beam Section', remark: '' },
  { panel: 'RcDgn_J', section: 'Rebar 패널', param: 'Rebar Input for Design - Column Section', remark: '' },
  { panel: 'RcDgn_J', section: 'Rebar 패널', param: 'Rebar Input for Checking - Beam Section', remark: '' },
  { panel: 'RcDgn_J', section: 'Rebar 패널', param: 'Rebar Input for Checking - Column Section', remark: '' },
  { panel: 'RcDgn_J', section: 'Rebar 패널', param: 'Rebar Input for Checking - General Section', remark: '' },
  { panel: 'RcDgn_J', section: 'Table 패널', param: 'Common Parameter Tables', remark: '' },
  { panel: 'RcDgn_J', section: 'Perform 패널', param: 'Concrete Code Design - Beam Design', remark: '' },
  { panel: 'RcDgn_J', section: 'Perform 패널', param: 'Concrete Code Design - Column Design', remark: '' },
  { panel: 'RcDgn_J', section: 'Perform 패널', param: 'Concrete Code Check - Beam Check', remark: '' },
  { panel: 'RcDgn_J', section: 'Perform 패널', param: 'Concrete Code Check - Column Check', remark: '' },
  { panel: 'RcDgn_J', section: 'Design Section 패널', param: 'Section for Design', remark: '' },
  { panel: 'RcDgn_J', section: 'Batch Processing 패널', param: 'Perform Batch Design', remark: '' },
  { panel: 'RcDgn_K', section: 'Design Input Data', param: 'Design Code Option', remark: '' },
  { panel: 'RcDgn_K', section: 'Design Input Data', param: 'Common Design Parameters (공통)', remark: '' },
  { panel: 'RcDgn_K', section: 'Design Input Data', param: 'Strength Reduction Factor', remark: '' },
  { panel: 'RcDgn_K', section: 'Design Input Data', param: 'Maximum Rebar Ratio', remark: '' },
  { panel: 'RcDgn_K', section: 'Design Input Data', param: 'Limiting Minimum Section Size', remark: '' },
  { panel: 'RcDgn_K', section: 'Design Input Data', param: 'Serviceability Parameters', remark: '' },
  { panel: 'RcDgn_K', section: 'Design Input Data', param: 'Modify RC Material', remark: 'AS 5100.5:17에서는 Moment Factor와 Moment Magnifier의 표시명이 다름' },
  { panel: 'RcDgn_K', section: 'Rebar 패널', param: 'Rebar Input for Design - Beam Section', remark: '' },
  { panel: 'RcDgn_K', section: 'Rebar 패널', param: 'Rebar Input for Design - Column Section', remark: '' },
  { panel: 'RcDgn_K', section: 'Rebar 패널', param: 'Rebar Input for Checking - Beam Section', remark: '' },
  { panel: 'RcDgn_K', section: 'Rebar 패널', param: 'Rebar Input for Checking - Column Section', remark: '' },
  { panel: 'RcDgn_K', section: 'Rebar 패널', param: 'Rebar Input for Checking - General Section', remark: '' },
  { panel: 'RcDgn_K', section: 'Table 패널', param: 'Common Parameter Tables', remark: '' },
  { panel: 'RcDgn_K', section: 'Perform 패널', param: 'Concrete Code Design - Beam Design', remark: '' },
  { panel: 'RcDgn_K', section: 'Perform 패널', param: 'Concrete Code Design - Column Design', remark: '' },
  { panel: 'RcDgn_K', section: 'Perform 패널', param: 'Concrete Code Check - Beam Check', remark: '' },
  { panel: 'RcDgn_K', section: 'Perform 패널', param: 'Concrete Code Check - Column Check', remark: '' },
  { panel: 'RcDgn_K', section: 'Design Section 패널', param: 'Section for Design', remark: '' },
  { panel: 'RcDgn_K', section: 'Batch Processing 패널', param: 'Perform Batch Design', remark: '' },
];

const initialDataRecords: DataRecord[] = [
  ...rcDesignData.map((data, index) => ({
    id: `rec-rc-${index + 1}`,
    objectTypeId: 'obj-3',
    values: {
      '패널': data.panel,
      '지원 코드': data.code,
      '코드명': data.name,
      '비고': data.remark,
    },
    createdAt: new Date().toISOString(),
  })),
  ...rcDesignParameterData.map((data, index) => ({
    id: `rec-param-${index + 1}`,
    objectTypeId: 'obj-4',
    values: {
      '패널': data.panel,
      '패널 섹션': data.section,
      '파라미터명': data.param,
      '비고': data.remark,
    },
    createdAt: new Date().toISOString(),
  })),
];

const initialRelationships: Relationship[] = [
  {
    id: 'rel-1',
    name: 'supports',
    fromObjectTypeId: 'obj-2',
    toObjectTypeId: 'obj-1',
    type: 'many-to-many',
    description: 'Steel Code supports Design Parameters',
    createdAt: new Date().toISOString(),
  },
  {
    id: 'rel-2',
    name: 'has_parameters',
    fromObjectTypeId: 'obj-3',
    toObjectTypeId: 'obj-4',
    type: 'one-to-many',
    description: 'RcDesign panel has RcDesignParameters',
    createdAt: new Date().toISOString(),
  },
];

// In-memory store
class DataStore {
  objectTypes: ObjectType[] = initialObjectTypes;
  dataRecords: DataRecord[] = initialDataRecords;
  relationships: Relationship[] = initialRelationships;
  relationshipLinks: RelationshipLink[] = [];

  // Object Types
  addObjectType(objectType: Omit<ObjectType, 'id' | 'createdAt'>) {
    const newObjectType: ObjectType = {
      ...objectType,
      id: `obj-${Date.now()}`,
      createdAt: new Date().toISOString(),
    };
    this.objectTypes.push(newObjectType);
    return newObjectType;
  }

  updateObjectType(id: string, updates: Partial<ObjectType>) {
    const index = this.objectTypes.findIndex(o => o.id === id);
    if (index !== -1) {
      this.objectTypes[index] = { ...this.objectTypes[index], ...updates };
    }
  }

  deleteObjectType(id: string) {
    this.objectTypes = this.objectTypes.filter(o => o.id !== id);
    this.dataRecords = this.dataRecords.filter(r => r.objectTypeId !== id);
  }

  // Data Records
  addDataRecord(record: Omit<DataRecord, 'id' | 'createdAt'>) {
    const newRecord: DataRecord = {
      ...record,
      id: `rec-${Date.now()}-${Math.random()}`,
      createdAt: new Date().toISOString(),
    };
    this.dataRecords.push(newRecord);
    return newRecord;
  }

  updateDataRecord(id: string, updates: Partial<DataRecord>) {
    const index = this.dataRecords.findIndex(r => r.id === id);
    if (index !== -1) {
      this.dataRecords[index] = { ...this.dataRecords[index], ...updates };
    }
  }

  deleteDataRecord(id: string) {
    this.dataRecords = this.dataRecords.filter(r => r.id !== id);
  }

  getRecordsByObjectType(objectTypeId: string) {
    return this.dataRecords.filter(r => r.objectTypeId === objectTypeId);
  }

  // Relationships
  addRelationship(relationship: Omit<Relationship, 'id' | 'createdAt'>) {
    const newRelationship: Relationship = {
      ...relationship,
      id: `rel-${Date.now()}`,
      createdAt: new Date().toISOString(),
    };
    this.relationships.push(newRelationship);
    return newRelationship;
  }

  deleteRelationship(id: string) {
    this.relationships = this.relationships.filter(r => r.id !== id);
    this.relationshipLinks = this.relationshipLinks.filter(l => l.relationshipId !== id);
  }

  // Relationship Links
  addRelationshipLink(link: Omit<RelationshipLink, 'id' | 'createdAt'>) {
    const newLink: RelationshipLink = {
      ...link,
      id: `link-${Date.now()}-${Math.random()}`,
      createdAt: new Date().toISOString(),
    };
    this.relationshipLinks.push(newLink);
    return newLink;
  }

  deleteRelationshipLink(id: string) {
    this.relationshipLinks = this.relationshipLinks.filter(l => l.id !== id);
  }

  getLinksForRelationship(relationshipId: string) {
    return this.relationshipLinks.filter(l => l.relationshipId === relationshipId);
  }
}

export const dataStore = new DataStore();
