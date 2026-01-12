const fs = require('fs');
const path = require('path');

// API 데이터셋 폴더에서 실제 API 목록 생성
const categories = ['DB', 'OPE', 'DOC', 'VIEW', 'POST'];
const baseDataPath = path.join(__dirname, '..', 'api_data_set');

const apis = [];

categories.forEach(category => {
  const categoryPath = path.join(baseDataPath, category);
  
  if (fs.existsSync(categoryPath)) {
    const folders = fs.readdirSync(categoryPath, { withFileTypes: true })
      .filter(dirent => dirent.isDirectory());
    
    folders.forEach(folder => {
      const match = folder.name.match(/^(\d+)_(.+)$/);
      if (match) {
        const [, number, name] = match;
        const folderPath = path.join(categoryPath, folder.name);
        
        // Zendesk 파일 확인
        const zendeskPath = path.join(folderPath, 'zendesk');
        let zendeskArticleId = null;
        
        if (fs.existsSync(zendeskPath)) {
          const zendeskFiles = fs.readdirSync(zendeskPath);
          // Zendesk HTML에서 Article ID 추출 시도 (여기서는 일단 null)
          // 실제로는 HTML 파일을 파싱해서 추출 가능
        }
        
        // API 엔드포인트 결정 (카테고리와 이름 기반)
        // 문서에서는 /api prefix 없이 직접 카테고리/API명 형식 사용
        const endpoint = `/${category.toLowerCase()}/${name.toUpperCase().replace(/_/g, '')}`;
        
        // HTTP 메서드 결정 (기본값, 나중에 스키마에서 읽기)
        const methods = ['GET', 'POST', 'PUT', 'DELETE'];
        
        apis.push({
          id: `${category.toLowerCase()}-${folder.name}`,
          name: name.replace(/_/g, ' '),
          category: category,
          number: number,
          endpoint: endpoint,
          methods: methods,
          description: `${name.replace(/_/g, ' ')} API`,
          zendeskArticleId: zendeskArticleId,
          dataPath: `api_data_set/${category}/${folder.name}`
        });
      }
    });
  }
});

// TypeScript 파일 생성
const tsContent = `import { ApiEndpoint } from '@/types/api';

// 자동 생성된 API 목록 (${new Date().toISOString()})
// 생성 명령: node scripts/generate_api_list.js

export const apiEndpoints: ApiEndpoint[] = ${JSON.stringify(apis, null, 2)};

export const getApisByCategory = (category: string) => {
  return apiEndpoints.filter(api => api.category === category);
};

export const getApiById = (id: string) => {
  return apiEndpoints.find(api => api.id === id);
};

export const getAllCategories = () => {
  return [...new Set(apiEndpoints.map(api => api.category))];
};

export const getApiCount = () => {
  return {
    total: apiEndpoints.length,
    byCategory: getAllCategories().reduce((acc, cat) => {
      acc[cat] = getApisByCategory(cat).length;
      return acc;
    }, {} as Record<string, number>)
  };
};
`;

const outputPath = path.join(__dirname, '..', 'src', 'data', 'apiEndpoints.ts');
fs.writeFileSync(outputPath, tsContent);

console.log(`✅ API 목록 생성 완료: ${apis.length}개`);
console.log('\n📊 카테고리별 통계:');
const stats = {};
apis.forEach(api => {
  stats[api.category] = (stats[api.category] || 0) + 1;
});
Object.entries(stats).forEach(([cat, count]) => {
  console.log(`  ${cat}: ${count}개`);
});
console.log(`\n📄 파일 생성: src/data/apiEndpoints.ts`);

