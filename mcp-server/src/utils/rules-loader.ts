/**
 * YAML 규칙 리더
 * MCP가 생성/검증 시 사용할 규칙을 YAML에서 읽음
 */

import * as fs from 'fs';
import * as path from 'path';
import yaml from 'js-yaml';

// 기본 규칙 경로
const RULES_DIR = path.resolve(
    process.env.APIVERIFICATION_ROOT || process.cwd().replace(/[\\/]mcp-server([\\/]dist)?$/, ''),
    'schema_definitions/civil_gen_definition/enhanced'
);

export interface GenerationRules {
    version: string;
    enumRules: {
        integerEnumFormat: {
            type: string;
            structure: unknown;
        };
        stringEnumFormat: {
            type: string;
            condition: string;
        };
    };
    fieldNaming: {
        prefixInference: {
            enabled: boolean;
            rules: Array<{ prefix: string; type: string; example: string }>;
        };
    };
    xuiRules: {
        required: string[];
        optional: string[];
        componentInference: Array<{ condition: string; component: string }>;
    };
}

export interface ValidationRules {
    version: string;
    enumValidation: {
        integerEnumMustUseOneOf: {
            enabled: boolean;
            message: string;
            fix: {
                type: string;
                fallbackLabel: string;
            };
        };
    };
    requiredProperties: Record<string, {
        enabled: boolean;
        path: string;
        message: string;
        fix?: Record<string, unknown>;
    }>;
    typeValidation: {
        prefixTypeMatch: {
            enabled: boolean;
            rules: Array<{ prefix: string; expectedType: string; message: string }>;
        };
    };
    structureValidation?: {
        requireSchema?: {
            enabled: boolean;
            value: string;
        };
        recommendTitle?: {
            enabled: boolean;
            level: string;
        };
        requiredFieldsExist?: {
            enabled: boolean;
            message: string;
        };
    };
}

/**
 * 생성 규칙 로드
 */
export function loadGenerationRules(): GenerationRules | null {
    const filePath = path.join(RULES_DIR, 'mcp-generation-rules.yaml');
    try {
        const content = fs.readFileSync(filePath, 'utf-8');
        return yaml.load(content) as GenerationRules;
    } catch (error) {
        console.error(`Failed to load generation rules: ${filePath}`, error);
        return null;
    }
}

/**
 * 검증 규칙 로드
 */
export function loadValidationRules(): ValidationRules | null {
    const filePath = path.join(RULES_DIR, 'mcp-validation-rules.yaml');
    try {
        const content = fs.readFileSync(filePath, 'utf-8');
        return yaml.load(content) as ValidationRules;
    } catch (error) {
        console.error(`Failed to load validation rules: ${filePath}`, error);
        return null;
    }
}

/**
 * 규칙 디렉토리 경로 반환
 */
export function getRulesDir(): string {
    return RULES_DIR;
}

/**
 * 🔥 v1.5: shared.yaml에서 SSOT 규칙 로드
 */
export interface SharedRules {
    versioning: { rulesSpecVersion: string };
    sectionRegistry: Array<{
        id: string;
        name: string;
        description?: string;
        isDefault?: boolean;
    }>;
    conditionRegistry: Array<{
        type: string;
        requiredParams: string[];
        description?: string;
    }>;
    integrityRules: {
        requireMarkerIdInRegistry: boolean;
        requireSectionIdInRegistry: boolean;
        requireConditionTypeInRegistry: boolean;
        requireXuiSectionIdInRegistry: boolean;
    };
    wrapperRegistryPolicy: {
        sort: string;
        match: string;
    };
}

export function loadSharedRules(): SharedRules | null {
    const filePath = path.join(RULES_DIR, 'shared.yaml');
    try {
        const content = fs.readFileSync(filePath, 'utf-8');
        return yaml.load(content) as SharedRules;
    } catch (error) {
        console.error(`Failed to load shared rules: ${filePath}`, error);
        return null;
    }
}

