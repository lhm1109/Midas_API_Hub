/**
 * Schema Validation Engine
 * YAML 기반 스키마 검증 엔진
 */

import yaml from 'js-yaml';

interface ValidationRule {
  id: string;
  name: string;
  description: string;
  severity: 'error' | 'warning' | 'info';
  category: string;
  checks: ValidationCheck[];
}

interface ValidationCheck {
  type: string;
  message: string;
  explanation: string;
  detect: DetectCondition;
  fix?: FixAction;
}

interface DetectCondition {
  conditions: Condition[];
}

interface Condition {
  path: string;
  count?: number;
  childType?: string;
  childName?: string[];
  exists?: boolean;
  missing?: string;
  exclude?: any;
  pattern?: string;
  value?: any;
  valueInKeys?: string;
  hasAnyKey?: string[];
}

interface FixAction {
  action: string;
  steps?: string[];
}

interface ValidationResult {
  valid: boolean;
  errors: ValidationIssue[];
  warnings: ValidationIssue[];
  infos: ValidationIssue[];
  fixable: boolean;
}

interface ValidationIssue {
  ruleId: string;
  ruleName: string;
  severity: 'error' | 'warning' | 'info';
  message: string;
  explanation: string;
  path: string;
  fix?: FixAction;
}

let validationRules: any = null;

/**
 * 검증 규칙 로드
 */
export async function initSchemaValidationRules(): Promise<void> {
  try {
    const response = await fetch('/schema_definitions/schema-validation-rules.yaml');
    const text = await response.text();
    validationRules = yaml.load(text);
    console.log('✅ Schema validation rules loaded');
  } catch (error) {
    console.error('❌ Failed to load schema validation rules:', error);
    throw error;
  }
}

/**
 * 검증 규칙 가져오기
 */
export function getValidationRules(): any {
  if (!validationRules) {
    throw new Error('Validation rules not loaded. Call initSchemaValidationRules() first.');
  }
  return validationRules;
}

/**
 * 스키마 검증
 */
export function validateSchema(
  schema: any,
  level: 'strict' | 'standard' | 'minimal' = 'standard'
): ValidationResult {
  const rules = getValidationRules();
  const result: ValidationResult = {
    valid: true,
    errors: [],
    warnings: [],
    infos: [],
    fixable: false,
  };

  // 검증 수준에 따라 규칙 선택
  const ruleCategories = rules.validationConfig.levels[level];
  const rulesToCheck: ValidationRule[] = [];

  for (const category of ruleCategories) {
    if (rules[category]) {
      rulesToCheck.push(...rules[category]);
    }
  }

  // 각 규칙 실행
  for (const rule of rulesToCheck) {
    for (const check of rule.checks) {
      const issues = executeCheck(schema, rule, check);
      
      for (const issue of issues) {
        if (issue.severity === 'error') {
          result.errors.push(issue);
          result.valid = false;
        } else if (issue.severity === 'warning') {
          result.warnings.push(issue);
        } else {
          result.infos.push(issue);
        }

        if (issue.fix) {
          result.fixable = true;
        }
      }
    }
  }

  return result;
}

/**
 * 검증 실행
 */
function executeCheck(schema: any, rule: ValidationRule, check: ValidationCheck): ValidationIssue[] {
  const issues: ValidationIssue[] = [];

  for (const condition of check.detect.conditions) {
    const violations = checkCondition(schema, condition);

    for (const violation of violations) {
      issues.push({
        ruleId: rule.id,
        ruleName: rule.name,
        severity: rule.severity,
        message: check.message,
        explanation: check.explanation,
        path: violation.path,
        fix: check.fix,
      });
    }
  }

  return issues;
}

/**
 * 조건 체크
 */
function checkCondition(schema: any, condition: Condition): Array<{ path: string }> {
  const violations: Array<{ path: string }> = [];

  // 1. 래퍼 검사: properties에 단일 객체만 있고, 알려진 래퍼인 경우
  if (condition.count !== undefined && condition.childType && condition.childName) {
    const properties = schema.properties;
    if (properties && Object.keys(properties).length === condition.count) {
      const propKeys = Object.keys(properties);
      const firstKey = propKeys[0];
      const firstProp = properties[firstKey];
      
      if (firstProp.type === condition.childType && condition.childName.includes(firstKey)) {
        violations.push({ path: `properties.${firstKey}` });
      }
    }
  }

  // 2. 중첩된 required 검사
  if (condition.path.includes('properties.*.required') && condition.exists) {
    const properties = schema.properties || {};
    for (const [key, value] of Object.entries(properties)) {
      if (value && typeof value === 'object' && (value as any).required) {
        violations.push({ path: `properties.${key}.required` });
      }
    }
  }

  // 3. allOf에서 점 표기법 검사
  if (condition.path.includes('allOf') && condition.pattern) {
    const allOf = schema.allOf || [];
    const regex = new RegExp(condition.pattern);
    
    allOf.forEach((item: any, index: number) => {
      if (item.then && item.then.required) {
        item.then.required.forEach((req: string, reqIndex: number) => {
          if (regex.test(req)) {
            violations.push({ path: `allOf[${index}].then.required[${reqIndex}]` });
          }
        });
      }
    });
  }

  // 4. x-transport.body-root와 properties 중복 검사
  if (condition.path === 'x-transport.body-root' && condition.valueInKeys) {
    const bodyRoot = schema['x-transport']?.['body-root'];
    if (bodyRoot && schema.properties && schema.properties[bodyRoot]) {
      violations.push({ path: `properties.${bodyRoot}` });
    }
  }

  // 5. 최상위 type이 array인지 검사
  if (condition.path === 'type' && condition.value === 'array') {
    if (schema.type === 'array') {
      violations.push({ path: 'type' });
    }
  }

  // 6. x-ui에 검증 키워드 검사
  if (condition.path === 'properties.*.x-ui' && condition.hasAnyKey) {
    const properties = schema.properties || {};
    for (const [key, value] of Object.entries(properties)) {
      const xUi = (value as any)?.['x-ui'];
      if (xUi) {
        for (const forbiddenKey of condition.hasAnyKey) {
          if (xUi[forbiddenKey] !== undefined) {
            violations.push({ path: `properties.${key}.x-ui.${forbiddenKey}` });
          }
        }
      }
    }
  }

  return violations;
}

/**
 * 자동 수정 (가능한 경우)
 */
export function autoFixSchema(schema: any, result: ValidationResult): any {
  const rules = getValidationRules();
  
  if (!rules.validationConfig.autoFix.enabled) {
    throw new Error('Auto-fix is disabled in configuration');
  }

  const fixedSchema = JSON.parse(JSON.stringify(schema)); // Deep clone

  for (const error of result.errors) {
    if (error.fix) {
      applyFix(fixedSchema, error);
    }
  }

  return fixedSchema;
}

/**
 * 수정 적용
 */
function applyFix(schema: any, issue: ValidationIssue): void {
  if (!issue.fix) return;

  switch (issue.fix.action) {
    case 'flatten-wrapper':
      flattenWrapper(schema, issue.path);
      break;
    
    case 'move-required-to-root':
      moveRequiredToRoot(schema, issue.path);
      break;
    
    case 'remove-dot-notation':
      removeDotNotation(schema, issue.path);
      break;
    
    case 'remove-wrapper-from-properties':
      removeWrapperFromProperties(schema);
      break;
  }
}

/**
 * 래퍼 평탄화
 */
function flattenWrapper(schema: any, path: string): void {
  const wrapperKey = path.split('.')[1]; // properties.Argument -> Argument
  const wrapper = schema.properties[wrapperKey];
  
  if (wrapper && wrapper.properties) {
    // 래퍼의 properties를 최상위로 이동
    schema.properties = wrapper.properties;
    
    // 래퍼의 required를 최상위로 이동
    if (wrapper.required) {
      schema.required = wrapper.required;
    }
    
    // x-transport.body-root 추가
    if (!schema['x-transport']) {
      schema['x-transport'] = {};
    }
    schema['x-transport']['body-root'] = wrapperKey;
  }
}

/**
 * required를 최상위로 이동
 */
function moveRequiredToRoot(schema: any, path: string): void {
  const parts = path.split('.');
  const propKey = parts[1]; // properties.Argument.required -> Argument
  const nestedProp = schema.properties[propKey];
  
  if (nestedProp && nestedProp.required) {
    if (!schema.required) {
      schema.required = [];
    }
    schema.required.push(...nestedProp.required);
    delete nestedProp.required;
  }
}

/**
 * 점 표기법 제거
 */
function removeDotNotation(schema: any, path: string): void {
  const match = path.match(/allOf\[(\d+)\]\.then\.required\[(\d+)\]/);
  if (match) {
    const allOfIndex = parseInt(match[1]);
    const reqIndex = parseInt(match[2]);
    
    const required = schema.allOf[allOfIndex].then.required[reqIndex];
    const simplified = required.split('.').pop(); // Argument.STAGE_STEP -> STAGE_STEP
    schema.allOf[allOfIndex].then.required[reqIndex] = simplified;
  }
}

/**
 * properties에서 래퍼 제거
 */
function removeWrapperFromProperties(schema: any): void {
  const bodyRoot = schema['x-transport']?.['body-root'];
  if (bodyRoot && schema.properties && schema.properties[bodyRoot]) {
    // 래퍼의 내용을 최상위로 이동
    const wrapper = schema.properties[bodyRoot];
    if (wrapper.properties) {
      schema.properties = wrapper.properties;
    }
    if (wrapper.required && !schema.required) {
      schema.required = wrapper.required;
    }
  }
}

/**
 * 검증 결과 포맷팅
 */
export function formatValidationResult(result: ValidationResult): string {
  const rules = getValidationRules();
  const template = rules.messageTemplates.detailedError;
  
  let output = '';
  
  if (result.errors.length > 0) {
    output += '\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n';
    output += '🚫 스키마 검증 오류\n';
    output += '━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n';
    
    for (const error of result.errors) {
      output += formatIssue(error, template);
      output += '\n';
    }
  }
  
  if (result.warnings.length > 0) {
    output += '\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n';
    output += '⚠️ 경고\n';
    output += '━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n';
    
    for (const warning of result.warnings) {
      output += `⚠️ [${warning.ruleName}] ${warning.message}\n`;
      output += `   위치: ${warning.path}\n\n`;
    }
  }
  
  if (result.fixable) {
    output += '\n✨ 일부 오류는 자동 수정이 가능합니다.\n';
    output += '   autoFixSchema() 함수를 사용하세요.\n';
  }
  
  return output;
}

/**
 * 이슈 포맷팅
 */
function formatIssue(issue: ValidationIssue, template: string): string {
  let output = template
    .replace('{ruleName}', issue.ruleName)
    .replace('{path}', issue.path)
    .replace('{message}', issue.message)
    .replace('{explanation}', issue.explanation);
  
  if (issue.fix && issue.fix.steps) {
    output = output.replace('{fixSteps}', issue.fix.steps.map((s, i) => `${i + 1}. ${s}`).join('\n'));
  } else {
    output = output.replace('{fixSteps}', '(자동 수정 불가)');
  }
  
  return output;
}

