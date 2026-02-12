/**
 * Test script to parse CEtcStypDlg and generate schema
 */
import { parseDialog } from './src/tools/parse-dialog.js';

async function main() {
    console.log('🔍 Parsing CEtcStypDlg...\n');
    
    const result = await parseDialog({
        dialogClass: 'CEtcStypDlg'
    });
    
    console.log(JSON.stringify(result, null, 2));
    
    if (result.ok && result.info) {
        console.log('\n✅ Parse successful!');
        console.log(`📄 Resource JSON: ${result.resourceJsonPath}`);
        console.log(`\n📋 Controls found: ${result.info.controls.length}`);
        console.log(`🔀 Conditionals found: ${result.info.conditionals.length}`);
    } else {
        console.error('\n❌ Parse failed:', result.error);
    }
}

main().catch(console.error);
