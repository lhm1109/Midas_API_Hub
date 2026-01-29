#!/usr/bin/env node
/**
 * CI Validation Script: shared.yaml → meta.schema.json
 * 
 * 사용법:
 *   node scripts/validate-shared-yaml.js [schemaType]
 *   
 * 예시:
 *   node scripts/validate-shared-yaml.js enhanced
 *   node scripts/validate-shared-yaml.js original
 *   node scripts/validate-shared-yaml.js  # 기본: enhanced
 * 
 * CI에서:
 *   npm run validate:shared
 */

const fs = require('fs');
const path = require('path');
const yaml = require('js-yaml');
const Ajv = require('ajv');

// CLI 인자 파싱
const schemaType = process.argv[2] || 'enhanced';
const validTypes = ['enhanced', 'original'];

if (!validTypes.includes(schemaType)) {
    console.error(`❌ Invalid schema type: ${schemaType}`);
    console.error(`   Valid types: ${validTypes.join(', ')}`);
    process.exit(1);
}

// 경로 설정
const basePath = path.join(__dirname, '..', 'schema_definitions', 'civil_gen_definition', schemaType);
const sharedYamlPath = path.join(basePath, 'shared.yaml');
const metaSchemaPath = path.join(basePath, 'shared.meta.schema.json');

console.log(`🔍 Validating: ${schemaType}/shared.yaml`);
console.log(`   Using meta-schema: ${schemaType}/shared.meta.schema.json`);
console.log('');

// 파일 존재 확인
if (!fs.existsSync(sharedYamlPath)) {
    console.error(`❌ File not found: ${sharedYamlPath}`);
    process.exit(1);
}

if (!fs.existsSync(metaSchemaPath)) {
    console.error(`❌ Meta-schema not found: ${metaSchemaPath}`);
    console.error('   Create shared.meta.schema.json first.');
    process.exit(1);
}

try {
    // 1. YAML 파싱
    console.log('📖 Parsing YAML...');
    const yamlContent = fs.readFileSync(sharedYamlPath, 'utf8');
    const sharedJson = yaml.load(yamlContent);
    console.log('   ✅ YAML parsed successfully');

    // 2. 메타스키마 로드
    console.log('📖 Loading meta-schema...');
    const metaSchema = JSON.parse(fs.readFileSync(metaSchemaPath, 'utf8'));
    console.log('   ✅ Meta-schema loaded');

    // 3. Ajv 검증
    console.log('🔬 Validating against meta-schema...');
    const ajv = new Ajv({
        allErrors: true,  // 모든 에러 수집
        strict: false     // strict mode 비활성화 (유연한 검증)
    });

    const validate = ajv.compile(metaSchema);
    const valid = validate(sharedJson);

    if (valid) {
        console.log('');
        console.log('═══════════════════════════════════════════════════');
        console.log('✅ VALIDATION PASSED');
        console.log(`   ${schemaType}/shared.yaml is valid!`);
        console.log('═══════════════════════════════════════════════════');
        process.exit(0);
    } else {
        console.error('');
        console.error('═══════════════════════════════════════════════════');
        console.error('❌ VALIDATION FAILED');
        console.error('═══════════════════════════════════════════════════');
        console.error('');

        // 에러 상세 출력
        validate.errors.forEach((error, index) => {
            console.error(`[${index + 1}] ${error.instancePath || '(root)'}`);
            console.error(`    Message: ${error.message}`);
            if (error.params) {
                console.error(`    Params: ${JSON.stringify(error.params)}`);
            }
            console.error('');
        });

        console.error(`Total errors: ${validate.errors.length}`);
        process.exit(1);
    }
} catch (error) {
    console.error('');
    console.error('═══════════════════════════════════════════════════');
    console.error('❌ SCRIPT ERROR');
    console.error('═══════════════════════════════════════════════════');
    console.error(error.message);
    process.exit(1);
}
