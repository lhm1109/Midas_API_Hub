const fs = require('fs');
const path = require('path');

// Path to the apiEndpoints.ts file
const apiEndpointsPath = path.join(__dirname, '..', 'src', 'data', 'apiEndpoints.ts');
const zendeskDocsPath = path.join(__dirname, '..', 'api_docs_zendesk');

/**
 * Extract Article ID from HTML file
 */
function extractArticleId(htmlFilePath) {
  try {
    const content = fs.readFileSync(htmlFilePath, 'utf8');
    
    // Look for: <div class="info-item"><span class="label">Article ID:</span> XXXXXXXXX</div>
    const match = content.match(/<span class="label">Article ID:<\/span>\s*(\d+)/);
    
    if (match && match[1]) {
      return match[1];
    }
    
    return null;
  } catch (error) {
    console.error(`Failed to read ${htmlFilePath}:`, error.message);
    return null;
  }
}

/**
 * Get all HTML files from api_docs_zendesk folder
 */
function getHtmlFiles() {
  const categories = ['DB', 'DOC', 'OPE', 'POST', 'VIEW'];
  const htmlFiles = [];
  
  for (const category of categories) {
    const categoryPath = path.join(zendeskDocsPath, category);
    
    if (!fs.existsSync(categoryPath)) {
      continue;
    }
    
    const files = fs.readdirSync(categoryPath);
    
    for (const file of files) {
      if (file.endsWith('.html')) {
        const filePath = path.join(categoryPath, file);
        const articleId = extractArticleId(filePath);
        
        if (articleId) {
          // Extract number from filename (e.g., "015_Node.html" -> "015")
          const numberMatch = file.match(/^(\d+)_/);
          if (numberMatch) {
            htmlFiles.push({
              category: category,
              number: numberMatch[1],
              articleId: articleId,
              filename: file
            });
          }
        }
      }
    }
  }
  
  return htmlFiles;
}

/**
 * Update apiEndpoints.ts with Article IDs
 */
function updateApiEndpoints() {
  console.log('🔍 Extracting Article IDs from HTML files...\n');
  
  const htmlFiles = getHtmlFiles();
  
  console.log(`Found ${htmlFiles.length} HTML files with Article IDs:\n`);
  
  // Create a map for quick lookup
  const articleIdMap = new Map();
  htmlFiles.forEach(file => {
    const key = `${file.category}-${file.number}`;
    articleIdMap.set(key, file.articleId);
    console.log(`  ${key}: ${file.articleId} (${file.filename})`);
  });
  
  console.log('\n📝 Updating apiEndpoints.ts...\n');
  
  // Read apiEndpoints.ts
  let content = fs.readFileSync(apiEndpointsPath, 'utf8');
  
  let updateCount = 0;
  
  // Update each entry
  // Pattern: "id": "db-015_Node", ... "zendeskArticleId": null,
  const apiPattern = /"id":\s*"([^"]+)"[\s\S]*?"zendeskArticleId":\s*(null|"[^"]*")/g;
  
  content = content.replace(apiPattern, (match, id, currentValue) => {
    // Parse ID (e.g., "db-015_Node" -> category: "DB", number: "015")
    const idParts = id.split('-');
    if (idParts.length >= 2) {
      const category = idParts[0].toUpperCase();
      const numberPart = idParts[1].split('_')[0];
      const key = `${category}-${numberPart}`;
      
      const articleId = articleIdMap.get(key);
      
      if (articleId) {
        updateCount++;
        // Replace the zendeskArticleId value
        const newMatch = match.replace(
          /"zendeskArticleId":\s*(null|"[^"]*")/,
          `"zendeskArticleId": "${articleId}"`
        );
        console.log(`  ✅ Updated ${id}: ${articleId}`);
        return newMatch;
      }
    }
    
    return match;
  });
  
  // Write back
  fs.writeFileSync(apiEndpointsPath, content, 'utf8');
  
  console.log(`\n✅ Updated ${updateCount} API endpoints with Zendesk Article IDs!`);
  console.log(`📁 File: ${apiEndpointsPath}`);
}

// Run the update
try {
  updateApiEndpoints();
} catch (error) {
  console.error('❌ Error:', error);
  process.exit(1);
}







