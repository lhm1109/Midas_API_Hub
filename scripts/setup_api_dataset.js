const fs = require('fs');
const path = require('path');

// api_docs_zendesk 폴더에서 API 정보를 읽어 api_data_set 폴더 구조 생성
const categories = ['DB', 'OPE', 'DOC', 'VIEW', 'POST'];
const baseSourcePath = path.join(__dirname, '..', 'api_docs_zendesk');
const baseTargetPath = path.join(__dirname, '..', 'api_data_set');

// 각 카테고리 처리
categories.forEach(category => {
  const categorySourcePath = path.join(baseSourcePath, category);
  const categoryTargetPath = path.join(baseTargetPath, category);

  // 카테고리 폴더 생성
  if (!fs.existsSync(categoryTargetPath)) {
    fs.mkdirSync(categoryTargetPath, { recursive: true });
  }

  // 해당 카테고리의 HTML 파일 목록 읽기
  if (fs.existsSync(categorySourcePath)) {
    const files = fs.readdirSync(categorySourcePath);
    
    files.forEach(file => {
      if (file.endsWith('.html') && !file.startsWith('_')) {
        // 파일명에서 번호와 이름 추출
        const match = file.match(/^(\d+)_(.+)\.html$/);
        if (match) {
          const [, number, name] = match;
          const endpointName = `${number}_${name}`;
          const endpointPath = path.join(categoryTargetPath, endpointName);

          // 엔드포인트 폴더 생성
          if (!fs.existsSync(endpointPath)) {
            fs.mkdirSync(endpointPath, { recursive: true });

            // 하위 폴더 생성
            const subFolders = [
              'product_samples',    // 제품 샘플 파일
              'zendesk',           // Zendesk 문서
              'api_schema',        // API 스키마
              'test_results'       // 테스트 결과
            ];

            subFolders.forEach(subFolder => {
              const subFolderPath = path.join(endpointPath, subFolder);
              fs.mkdirSync(subFolderPath, { recursive: true });
            });

            // Zendesk 문서 복사
            const sourceFilePath = path.join(categorySourcePath, file);
            const targetFilePath = path.join(endpointPath, 'zendesk', file);
            fs.copyFileSync(sourceFilePath, targetFilePath);

            // README 생성
            const readmeContent = `# ${name.replace(/_/g, ' ')}

## 카테고리: ${category}
## 번호: ${number}

### 폴더 구조

- **product_samples/**: MIDAS 제품에서 추출한 실제 파일 데이터
- **zendesk/**: Zendesk 공식 문서 및 링크
- **api_schema/**: API 스키마 정의 (JSON Schema, OpenAPI 등)
- **test_results/**: API 테스트 결과 및 검증 데이터

### 사용 방법

1. 제품 샘플 파일을 \`product_samples/\` 폴더에 추가
2. Zendesk 문서를 확인하여 스키마 정보 추출
3. \`api_schema/\` 폴더에 스키마 파일 생성
4. 테스트 실행 후 결과를 \`test_results/\` 폴더에 저장
`;

            fs.writeFileSync(path.join(endpointPath, 'README.md'), readmeContent);

            console.log(`✅ Created: ${category}/${endpointName}`);
          }
        }
      }
    });
  }
});

console.log('\n🎉 API 데이터셋 폴더 구조 생성 완료!');
console.log('\n📁 생성된 구조:');
console.log('api_data_set/');
console.log('├── DB/');
console.log('│   ├── 001_Project_Information/');
console.log('│   │   ├── product_samples/');
console.log('│   │   ├── zendesk/');
console.log('│   │   ├── api_schema/');
console.log('│   │   └── test_results/');
console.log('│   └── ...');
console.log('├── OPE/');
console.log('├── DOC/');
console.log('├── VIEW/');
console.log('└── POST/');

