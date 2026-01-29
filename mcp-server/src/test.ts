// 테스트 스크립트
import { saveSchema } from './tools/save.js';

async function test() {
    // 테스트 1: questions가 있으면 pending_questions 반환
    const result1 = await saveSchema({
        schemaName: 'TEST',
        schema: { type: 'object' },
        questions: [
            { field: 'test', question: '테스트 질문입니다' }
        ]
    });
    console.log('=== Test 1: With questions ===');
    console.log(JSON.stringify(result1, null, 2));

    // 테스트 2: questions가 없으면 저장
    const result2 = await saveSchema({
        schemaName: 'TEST2',
        outputDir: 'test',
        schema: { type: 'object', properties: { name: { type: 'string' } } }
    });
    console.log('\n=== Test 2: Without questions ===');
    console.log(JSON.stringify(result2, null, 2));
}

test();
