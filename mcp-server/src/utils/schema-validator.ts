/**
 * 스키마 검증기
 * YAML 규칙 기반으로 스키마 검증 및 자동 변환
 */

import { loadValidationRules, loadGenerationRules, loadSharedRules } from '../utils/rules-loader.js';

export interface ValidationResult {
    valid: boolean;
    errors: ValidationError[];
    warnings: ValidationWarning[];
    transformed: Record<string, unknown>;  // 변환된 스키마
}

export interface ValidationError {
    field: string;
    message: string;
    fixable: boolean;
    fixApplied?: boolean;
}

export interface ValidationWarning {
    field: string;
    message: string;
}

/**
 * 스키마 검증 및 자동 변환
 */
export function validateAndTransform(
    schema: Record<string, unknown>
): ValidationResult {
    const validationRules = loadValidationRules();
    const generationRules = loadGenerationRules();

    const errors: ValidationError[] = [];
    const warnings: ValidationWarning[] = [];

    // 깊은 복사로 변환용 스키마 생성
    const transformed = JSON.parse(JSON.stringify(schema)) as Record<string, unknown>;

    const properties = transformed['properties'] as Record<string, unknown> | undefined;

    if (properties) {
        for (const [fieldName, fieldDef] of Object.entries(properties)) {
            const field = fieldDef as Record<string, unknown>;

            // 1. 정수 enum → oneOf 자동 변환
            if (validationRules?.enumValidation?.integerEnumMustUseOneOf?.enabled) {
                const enumValues = field['enum'] as unknown[] | undefined;
                const fieldType = field['type'] as string | undefined;

                if (enumValues && (fieldType === 'integer' || fieldType === 'number') && !field['oneOf']) {
                    const fallbackLabel = validationRules.enumValidation.integerEnumMustUseOneOf.fix.fallbackLabel;

                    // x-ui.options에서 라벨 가져오기
                    const xui = field['x-ui'] as Record<string, unknown> | undefined;
                    const options = xui?.['options'] as Array<{ value: number; label: string }> | undefined;

                    // oneOf 변환
                    const oneOf = enumValues.map(value => {
                        const option = options?.find(o => o.value === value);
                        const title = option?.label || fallbackLabel.replace('{value}', String(value));
                        return { const: value, title };
                    });

                    field['oneOf'] = oneOf;
                    delete field['enum'];

                    // x-ui.options 제거 (oneOf에 통합됨)
                    if (xui?.['options']) {
                        delete xui['options'];
                    }

                    errors.push({
                        field: fieldName,
                        message: validationRules.enumValidation.integerEnumMustUseOneOf.message,
                        fixable: true,
                        fixApplied: true,
                    });
                }
            }

            // 2. x-ui.label 필수 체크
            if (validationRules?.requiredProperties?.xuiLabel?.enabled) {
                const xui = field['x-ui'] as Record<string, unknown> | undefined;
                if (!xui?.['label']) {
                    // 필드명에서 자동 생성
                    const label = fieldName
                        .replace(/^[ibds]/, '')  // 접두사 제거
                        .replace(/_/g, ' ')      // _ → 공백
                        .replace(/([A-Z])/g, ' $1')  // camelCase 분리
                        .trim();

                    if (!xui) {
                        field['x-ui'] = { label };
                    } else {
                        xui['label'] = label;
                    }

                    warnings.push({
                        field: fieldName,
                        message: `x-ui.label 자동 생성: "${label}"`,
                    });
                }
            }

            // 3. x-ui.sectionId 필수 체크 (🔥 v1.5 SSOT: group → sectionId)
            if (validationRules?.requiredProperties?.xuiGroup?.enabled) {
                const xui = field['x-ui'] as Record<string, unknown> | undefined;
                // sectionId가 없고 group도 없으면 기본 sectionId 설정
                if (!xui?.['sectionId'] && !xui?.['group']) {
                    // 🔥 v1.5: sectionRegistry에서 isDefault=true인 섹션 찾기
                    const sharedRules = loadSharedRules();
                    const defaultSection = sharedRules?.sectionRegistry?.find((s: { isDefault?: boolean }) => s.isDefault);
                    const defaultSectionId = defaultSection?.id || 'SECTION_GENERAL';

                    if (!xui) {
                        field['x-ui'] = { sectionId: defaultSectionId };
                    } else {
                        xui['sectionId'] = defaultSectionId;
                    }

                    warnings.push({
                        field: fieldName,
                        message: `x-ui.sectionId 기본값 설정: "${defaultSectionId}" (sectionRegistry SSOT)`,
                    });
                }
                // 🔥 v1.5: group → sectionId 마이그레이션
                if (xui?.['group'] && !xui?.['sectionId']) {
                    const groupValue = xui['group'] as string;
                    // group 문자열을 sectionId 패턴으로 변환
                    const sectionId = `SECTION_${groupValue.toUpperCase().replace(/\s+/g, '_')}`;
                    xui['sectionId'] = sectionId;
                    delete xui['group'];

                    warnings.push({
                        field: fieldName,
                        message: `x-ui.group → x-ui.sectionId 마이그레이션: "${groupValue}" → "${sectionId}"`,
                    });
                }
            }

            // 4. 접두사-타입 일치 확인
            if (validationRules?.typeValidation?.prefixTypeMatch?.enabled) {
                for (const rule of validationRules.typeValidation.prefixTypeMatch.rules) {
                    if (fieldName.startsWith(rule.prefix)) {
                        const fieldType = field['type'] as string | undefined;
                        if (fieldType && fieldType !== rule.expectedType) {
                            warnings.push({
                                field: fieldName,
                                message: rule.message,
                            });
                        }
                        break;
                    }
                }
            }

            // 🔥 5. sectionId 무결성 검증 (v1.5 SSOT)
            const sharedRules = loadSharedRules();
            if (sharedRules?.integrityRules?.requireXuiSectionIdInRegistry) {
                const xui = field['x-ui'] as Record<string, unknown> | undefined;
                const sectionId = xui?.['sectionId'] as string | undefined;
                if (sectionId) {
                    const validSectionIds = sharedRules.sectionRegistry?.map(s => s.id) || [];
                    if (!validSectionIds.includes(sectionId)) {
                        errors.push({
                            field: fieldName,
                            message: `x-ui.sectionId "${sectionId}"가 sectionRegistry에 없음. 유효값: ${validSectionIds.join(', ')}`,
                            fixable: true,
                            fixApplied: false,
                        });
                    }
                }
            }
        }
    }

    // 6. $schema 추가
    if (validationRules?.structureValidation?.requireSchema?.enabled) {
        if (!transformed['$schema']) {
            transformed['$schema'] = validationRules.structureValidation.requireSchema.value;
        }
    }

    return {
        valid: errors.filter(e => !e.fixApplied).length === 0,
        errors,
        warnings,
        transformed,
    };
}

