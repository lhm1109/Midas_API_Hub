import * as path from 'path';
import * as fs from 'fs';

// PROJECT_ROOT: 환경변수 우선, 없으면 cwd 기반 (mcp-server 또는 mcp-server/dist에서도 동작)
const PROJECT_ROOT = process.env.APIVERIFICATION_ROOT
    || process.cwd().replace(/[\\/]mcp-server([\\/]dist)?$/, '');

export const SCHEMA_DEFS_DIR = path.join(PROJECT_ROOT, 'schema_definitions');
export const GENERATED_DIR = path.join(PROJECT_ROOT, 'generated_schemas');

export interface WritePaths {
    dir: string;
    schemaPath: string;
    metaPath: string;
}

/**
 * 세그먼트 기반 traversal 검사 (a..b.yaml 같은 정상 파일명 허용)
 */
function hasTraversalSegment(relativePath: string): boolean {
    const normalized = path.normalize(relativePath);
    const segments = normalized.split(path.sep);
    return segments.some(seg => seg === '..');
}

/**
 * 심볼릭 링크 여부 검사
 */
function isSymlink(targetPath: string): boolean {
    try {
        const stat = fs.lstatSync(targetPath);
        return stat.isSymbolicLink();
    } catch {
        return false;
    }
}

/**
 * 경계 검증 (접두사 취약점 방지)
 */
function isWithinBase(targetReal: string, baseReal: string): boolean {
    return targetReal === baseReal || targetReal.startsWith(baseReal + path.sep);
}

/**
 * 읽기 경로 검증
 * - 절대 경로 차단
 * - traversal 차단
 * - 심볼릭 링크 차단
 * - 경계 검증
 * - 확장자 검증
 */
export function validateReadPath(relativePath: string): string {
    // 1. 절대 경로 차단
    if (path.isAbsolute(relativePath)) {
        throw new Error('Absolute paths not allowed');
    }

    // 2. 세그먼트 기반 traversal 검사
    if (hasTraversalSegment(relativePath)) {
        throw new Error('Path traversal not allowed');
    }

    // 3. 절대경로 resolve
    const absolutePath = path.resolve(SCHEMA_DEFS_DIR, relativePath);

    // 4. 파일 존재 확인
    if (!fs.existsSync(absolutePath)) {
        throw new Error(`File not found: ${relativePath}`);
    }

    // 5. 심볼릭 링크 명시적 거부
    if (isSymlink(absolutePath)) {
        throw new Error('Symbolic links not allowed');
    }

    // 6. realpath 후 경계 검증
    const realPath = fs.realpathSync(absolutePath);
    const baseReal = fs.realpathSync(SCHEMA_DEFS_DIR);

    if (!isWithinBase(realPath, baseReal)) {
        throw new Error('Path escapes allowed directory');
    }

    // 7. 확장자 검증
    const ext = path.extname(realPath).toLowerCase();
    if (!['.yaml', '.yml'].includes(ext)) {
        throw new Error('Only .yaml/.yml files allowed');
    }

    return realPath;
}

/**
 * 쓰기 경로 빌드
 * - traversal 차단
 * - 디렉토리 생성 후 realpath 검증
 * - 심볼릭 링크 차단
 */
export function buildWritePath(outputDir: string, schemaName: string): WritePaths {
    // 1. 세그먼트 기반 traversal 검증
    if (hasTraversalSegment(outputDir) || hasTraversalSegment(schemaName)) {
        throw new Error('Path traversal not allowed');
    }
    if (schemaName.includes('/') || schemaName.includes('\\')) {
        throw new Error('Slashes not allowed in schemaName');
    }

    // 2. 경로 정규화
    const normalizedDir = path.normalize(outputDir);

    // 3. 경로 조합
    const dir = path.join(GENERATED_DIR, normalizedDir);
    const schemaPath = path.join(dir, `${schemaName}.json`);
    const metaPath = path.join(dir, `${schemaName}.meta.json`);

    // 4. GENERATED_DIR 존재 확인 및 생성
    if (!fs.existsSync(GENERATED_DIR)) {
        fs.mkdirSync(GENERATED_DIR, { recursive: true });
    }

    // 5. 디렉토리 생성 (먼저 생성해야 realpath 가능)
    fs.mkdirSync(dir, { recursive: true });

    // 6. 생성된 디렉토리가 심볼릭 링크인지 확인
    if (isSymlink(dir)) {
        throw new Error('Symbolic links not allowed in output path');
    }

    // 7. realpath 후 경계 검증
    const dirReal = fs.realpathSync(dir);
    const baseReal = fs.realpathSync(GENERATED_DIR);

    if (!isWithinBase(dirReal, baseReal)) {
        throw new Error('Path escapes allowed directory');
    }

    return { dir: dirReal, schemaPath, metaPath };
}
