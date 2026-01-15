/**
 * Schema Definition Sets API Routes (2-Level Structure)
 * YAML 스키마 정의 세트 관리
 * 
 * 구조: schema_definitions/[PSD Set]/[Schema Type]/[YAML Files]
 * 예: schema_definitions/default/enhanced/builder.yaml
 */
import express from 'express';
import fs from 'fs';
import path from 'path';
import { fileURLToPath } from 'url';

const router = express.Router();
const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

const SCHEMA_DEFINITIONS_PATH = path.join(__dirname, '../../schema_definitions');

/**
 * GET /api/schema-definitions
 * PSD 세트 목록 조회 (Level 1: default, civil_gen_definition, etc.)
 */
router.get('/', async (req, res) => {
  try {
    console.log('📚 Fetching PSD sets (Level 1)...');
    
    // schema_definitions 폴더 읽기 (PSD 세트들)
    const psdSets = fs.readdirSync(SCHEMA_DEFINITIONS_PATH, { withFileTypes: true })
      .filter(dirent => dirent.isDirectory())
      .filter(dirent => !dirent.name.startsWith('.') && !dirent.name.startsWith('_'))
      .map(dirent => {
        const setPath = path.join(SCHEMA_DEFINITIONS_PATH, dirent.name);
        const stats = fs.statSync(setPath);
        
        // 각 PSD 세트 안의 스키마 타입 개수 (enhanced, manual, original 등)
        const schemaTypes = fs.readdirSync(setPath, { withFileTypes: true })
          .filter(d => d.isDirectory())
          .map(d => d.name);
        
        // 모든 YAML 파일 수집 (모든 스키마 타입에서)
        let allFiles = [];
        schemaTypes.forEach(typeName => {
          const typePath = path.join(setPath, typeName);
          const yamlFiles = fs.readdirSync(typePath)
            .filter(f => f.endsWith('.yaml') || f.endsWith('.yml'))
            .map(f => `${typeName}/${f}`);
          allFiles = allFiles.concat(yamlFiles);
        });
        
        // README.md가 있으면 읽기
        let description = '';
        const readmePath = path.join(setPath, 'README.md');
        if (fs.existsSync(readmePath)) {
          const readmeContent = fs.readFileSync(readmePath, 'utf-8');
          const firstLine = readmeContent.split('\n')[0].replace(/^#\s*/, '');
          description = firstLine.substring(0, 100);
        }
        
        return {
          id: dirent.name,
          name: dirent.name,
          description: description || `PSD Set: ${dirent.name}`,
          schemaTypes: schemaTypes,
          schemaTypeCount: schemaTypes.length,
          files: allFiles,
          fileCount: allFiles.length,
          created: stats.birthtime,
          modified: stats.mtime,
        };
      });
    
    console.log(`✅ Found ${psdSets.length} PSD sets`);
    res.json(psdSets);
  } catch (error) {
    console.error('❌ Error fetching PSD sets:', error);
    res.status(500).json({ error: error.message });
  }
});

/**
 * GET /api/schema-definitions/:setName
 * PSD 세트의 스키마 타입 목록 조회 (Level 2: enhanced, manual, original)
 */
router.get('/:setName', async (req, res) => {
  try {
    const { setName } = req.params;
    const setPath = path.join(SCHEMA_DEFINITIONS_PATH, setName);
    
    console.log(`📖 Fetching PSD set: ${setName}`);
    
    // 폴더 존재 확인
    if (!fs.existsSync(setPath) || !fs.statSync(setPath).isDirectory()) {
      return res.status(404).json({ error: 'PSD set not found' });
    }
    
    // 스키마 타입 목록 (enhanced, manual, original 등)
    const schemaTypes = fs.readdirSync(setPath, { withFileTypes: true })
      .filter(dirent => dirent.isDirectory())
      .map(dirent => {
        const typePath = path.join(setPath, dirent.name);
        const stats = fs.statSync(typePath);
        
        // 각 타입 안의 YAML 파일 목록
        const files = fs.readdirSync(typePath)
          .filter(file => file.endsWith('.yaml') || file.endsWith('.yml'))
          .map(file => {
            const filePath = path.join(typePath, file);
            const fileStats = fs.statSync(filePath);
            return {
              name: file,
              size: fileStats.size,
              modified: fileStats.mtime,
            };
          });
        
        return {
          id: dirent.name,
          name: dirent.name,
          files: files,
          fileCount: files.length,
          modified: stats.mtime,
        };
      });
    
    // README.md 읽기 (PSD 세트 레벨)
    let readme = '';
    const readmePath = path.join(setPath, 'README.md');
    if (fs.existsSync(readmePath)) {
      readme = fs.readFileSync(readmePath, 'utf-8');
    }
    
    // schema-validation-rules.yaml 확인
    const validationRulesPath = path.join(setPath, 'schema-validation-rules.yaml');
    const hasValidationRules = fs.existsSync(validationRulesPath);
    
    const stats = fs.statSync(setPath);
    
    res.json({
      id: setName,
      name: setName,
      schemaTypes: schemaTypes,
      readme: readme,
      hasValidationRules: hasValidationRules,
      created: stats.birthtime,
      modified: stats.mtime,
    });
  } catch (error) {
    console.error(`❌ Error fetching PSD set ${req.params.setName}:`, error);
    res.status(500).json({ error: error.message });
  }
});

/**
 * POST /api/schema-definitions
 * 새 PSD 세트 생성 (기존 세트 복제, 2-level 구조)
 */
router.post('/', async (req, res) => {
  try {
    const { name, basedOn } = req.body;
    
    if (!name) {
      return res.status(400).json({ error: 'Name is required' });
    }
    
    // 이름 검증 (영문자, 숫자, 하이픈, 언더스코어만)
    if (!/^[a-zA-Z0-9_-]+$/.test(name)) {
      return res.status(400).json({ error: 'Invalid name. Use only letters, numbers, hyphens, and underscores.' });
    }
    
    const newSetPath = path.join(SCHEMA_DEFINITIONS_PATH, name);
    
    // 이미 존재하는지 확인
    if (fs.existsSync(newSetPath)) {
      return res.status(400).json({ error: 'PSD set already exists' });
    }
    
    console.log(`📝 Creating new PSD set: ${name} (based on: ${basedOn || 'default'})`);
    
    // 기본값: default 세트 복제
    const baseSet = basedOn || 'default';
    const baseSetPath = path.join(SCHEMA_DEFINITIONS_PATH, baseSet);
    
    if (!fs.existsSync(baseSetPath)) {
      return res.status(400).json({ error: 'Base PSD set not found' });
    }
    
    // 폴더 복사 (recursive) - enhanced, manual, original 등 모두 복제
    fs.cpSync(baseSetPath, newSetPath, { recursive: true });
    
    // README.md 수정
    const readmePath = path.join(newSetPath, 'README.md');
    const readme = `# ${name} PSD Set

> 📦 Cloned from: **${baseSet}**

이 PSD 세트는 \`${baseSet}\`를 기반으로 생성되었습니다.

## 📁 구조

\`\`\`
${name}/
  ├─ enhanced/       ← 고급 스키마 타입
  ├─ manual/         ← 수동 문서 타입
  ├─ original/       ← 원본 스키마 타입
  └─ schema-validation-rules.yaml
\`\`\`

## 🔧 수정 방법

PSD 탭에서 "${name}" 선택 → 스키마 타입 선택 → 파일 편집
`;
    fs.writeFileSync(readmePath, readme, 'utf-8');
    
    console.log(`✅ PSD set created: ${name}`);
    res.status(201).json({ 
      message: 'PSD set created successfully',
      id: name,
      name: name,
      basedOn: baseSet,
    });
  } catch (error) {
    console.error('❌ Error creating PSD set:', error);
    res.status(500).json({ error: error.message });
  }
});

/**
 * GET /api/schema-definitions/:setName/:schemaType/:fileName
 * 특정 YAML 파일 내용 조회 (2-level)
 * 예: /api/schema-definitions/default/enhanced/builder.yaml
 */
router.get('/:setName/:schemaType/:fileName', async (req, res) => {
  try {
    const { setName, schemaType, fileName } = req.params;
    
    // 파일명 검증 (보안)
    if (!fileName.endsWith('.yaml') && !fileName.endsWith('.yml')) {
      return res.status(400).json({ error: 'Only YAML files are allowed' });
    }
    
    // 경로 traversal 방지
    if (setName.includes('..') || schemaType.includes('..') || fileName.includes('..') ||
        setName.includes('/') || schemaType.includes('/') || fileName.includes('/') ||
        setName.includes('\\') || schemaType.includes('\\') || fileName.includes('\\')) {
      return res.status(400).json({ error: 'Invalid path' });
    }
    
    const filePath = path.join(SCHEMA_DEFINITIONS_PATH, setName, schemaType, fileName);
    
    console.log(`📄 Reading YAML: ${setName}/${schemaType}/${fileName}`);
    
    if (!fs.existsSync(filePath)) {
      return res.status(404).json({ error: 'File not found' });
    }
    
    const content = fs.readFileSync(filePath, 'utf-8');
    const stats = fs.statSync(filePath);
    
    res.json({
      name: fileName,
      content: content,
      size: stats.size,
      modified: stats.mtime,
      path: `${setName}/${schemaType}/${fileName}`,
    });
  } catch (error) {
    console.error(`❌ Error reading file ${req.params.setName}/${req.params.schemaType}/${req.params.fileName}:`, error);
    res.status(500).json({ error: error.message });
  }
});

/**
 * PUT /api/schema-definitions/:setName/:schemaType/:fileName
 * YAML 파일 수정 (2-level)
 * 예: PUT /api/schema-definitions/civil_gen_definition/enhanced/builder.yaml
 */
router.put('/:setName/:schemaType/:fileName', async (req, res) => {
  try {
    const { setName, schemaType, fileName } = req.params;
    const { content } = req.body;
    
    // 파일명 검증
    if (!fileName.endsWith('.yaml') && !fileName.endsWith('.yml')) {
      return res.status(400).json({ error: 'Only YAML files are allowed' });
    }
    
    if (setName.includes('..') || schemaType.includes('..') || fileName.includes('..') ||
        setName.includes('/') || schemaType.includes('/') || fileName.includes('/') ||
        setName.includes('\\') || schemaType.includes('\\') || fileName.includes('\\')) {
      return res.status(400).json({ error: 'Invalid path' });
    }
    
    // 기본 세트는 수정 불가
    if (setName === 'default') {
      return res.status(400).json({ error: 'Cannot modify default PSD set. Please create a custom set.' });
    }
    
    if (!content) {
      return res.status(400).json({ error: 'Content is required' });
    }
    
    const filePath = path.join(SCHEMA_DEFINITIONS_PATH, setName, schemaType, fileName);
    
    // 폴더 존재 확인
    const typePath = path.join(SCHEMA_DEFINITIONS_PATH, setName, schemaType);
    if (!fs.existsSync(typePath)) {
      return res.status(404).json({ error: 'Schema type folder not found' });
    }
    
    console.log(`📝 Updating YAML file: ${setName}/${schemaType}/${fileName}`);
    
    // YAML 유효성 검사
    try {
      const yaml = await import('js-yaml');
      yaml.load(content); // 파싱 테스트
    } catch (yamlError) {
      return res.status(400).json({ 
        error: 'Invalid YAML syntax',
        details: yamlError.message 
      });
    }
    
    // 파일 저장
    fs.writeFileSync(filePath, content, 'utf-8');
    
    console.log(`✅ YAML file updated: ${setName}/${schemaType}/${fileName}`);
    res.json({ 
      message: 'File updated successfully',
      name: fileName,
      path: `${setName}/${schemaType}/${fileName}`,
      size: Buffer.byteLength(content, 'utf-8'),
    });
  } catch (error) {
    console.error(`❌ Error updating file ${req.params.setName}/${req.params.schemaType}/${req.params.fileName}:`, error);
    res.status(500).json({ error: error.message });
  }
});

/**
 * DELETE /api/schema-definitions/:setName
 * PSD 세트 삭제 (전체 폴더)
 */
router.delete('/:setName', async (req, res) => {
  try {
    const { setName } = req.params;
    
    // 기본 세트는 삭제 불가
    if (setName === 'default') {
      return res.status(400).json({ error: 'Cannot delete default PSD set' });
    }
    
    const setPath = path.join(SCHEMA_DEFINITIONS_PATH, setName);
    
    if (!fs.existsSync(setPath)) {
      return res.status(404).json({ error: 'PSD set not found' });
    }
    
    console.log(`🗑️ Deleting PSD set: ${setName}`);
    
    // 폴더 삭제 (recursive)
    fs.rmSync(setPath, { recursive: true, force: true });
    
    console.log(`✅ PSD set deleted: ${setName}`);
    res.json({ message: 'PSD set deleted successfully' });
  } catch (error) {
    console.error(`❌ Error deleting PSD set ${req.params.setName}:`, error);
    res.status(500).json({ error: error.message });
  }
});

export default router;

