import express from 'express';
import path from 'path';
import fs from 'fs/promises';

const router = express.Router();

function asTrimmedString(value) {
  return typeof value === 'string' ? value.trim() : '';
}

/**
 * GET /api/workspace/file?path=C:\...\file.html
 * - 워크스페이스(process.cwd()) 이하의 파일만 읽습니다 (dev:all 브라우저용).
 */
router.get('/file', async (req, res) => {
  const raw = asTrimmedString(req.query.path);
  if (!raw) {
    return res.status(400).json({ success: false, error: 'path query is required' });
  }

  let resolved;
  try {
    resolved = path.resolve(raw);
  } catch {
    return res.status(400).json({ success: false, error: 'Invalid path' });
  }

  const cwd = process.cwd();
  const rel = path.relative(cwd, resolved);
  if (rel.startsWith('..') || path.isAbsolute(rel)) {
    return res.status(403).json({ success: false, error: 'Path must be under workspace root' });
  }

  try {
    const content = await fs.readFile(resolved, 'utf-8');
    res.setHeader('Content-Type', 'text/html; charset=utf-8');
    return res.send(content);
  } catch (e) {
    return res.status(404).json({
      success: false,
      error: e instanceof Error ? e.message : 'read failed',
    });
  }
});

export default router;
