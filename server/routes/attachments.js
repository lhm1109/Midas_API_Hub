import express from 'express';
import multer from 'multer';
import db from '../database.js';
import { readFileSync, existsSync, mkdirSync, unlinkSync } from 'fs';
import { join, dirname, extname } from 'path';
import { fileURLToPath } from 'url';

const __filename = fileURLToPath(import.meta.url);
const __dirname = dirname(__filename);
const router = express.Router();

// uploads 디렉토리 경로
const uploadsDir = join(__dirname, '..', 'uploads');

// uploads 디렉토리가 없으면 생성
if (!existsSync(uploadsDir)) {
  mkdirSync(uploadsDir, { recursive: true });
}

// 🎯 Multer 설정 - 스트리밍 방식으로 파일 저장
const storage = multer.diskStorage({
  destination: (req, file, cb) => {
    cb(null, uploadsDir);
  },
  filename: (req, file, cb) => {
    // 고유한 파일명 생성
    const uniqueId = `att_${Date.now()}_${Math.random().toString(36).substr(2, 9)}`;
    const ext = extname(file.originalname);
    const baseName = file.originalname.replace(ext, '');
    cb(null, `${uniqueId}_${baseName}${ext}`);
  }
});

const upload = multer({
  storage: storage,
  limits: {
    fileSize: 100 * 1024 * 1024 // 100MB 제한 (필요시 조정 가능)
  }
});

// 🎯 파일 업로드 - Multipart/form-data 방식 (스트리밍)
router.post('/upload', upload.single('file'), (req, res) => {
  try {
    const { versionId } = req.body;
    const file = req.file;
    
    console.log('📥 Upload request received:', {
      versionId,
      file: file ? {
        originalname: file.originalname,
        mimetype: file.mimetype,
        size: file.size,
        path: file.path,
        extension: file.originalname.split('.').pop()
      } : 'NO FILE'
    });
    
    if (!versionId || !file) {
      console.error('❌ Missing fields:', { versionId: !!versionId, file: !!file });
      return res.status(400).json({ error: 'Missing required fields' });
    }
    
    // 파일 ID는 multer의 filename에서 추출
    const attachmentId = file.filename.split('_').slice(0, 3).join('_');
    
    // 데이터베이스에 메타데이터 저장
    const now = new Date().toISOString();
    db.prepare(`
      INSERT INTO attachments (id, version_id, file_name, file_size, file_path, mime_type, uploaded_at)
      VALUES (?, ?, ?, ?, ?, ?, ?)
    `).run(
      attachmentId,
      versionId,
      file.originalname,
      file.size,
      file.path,
      file.mimetype || 'application/octet-stream',
      now
    );
    
    console.log('✅ File uploaded successfully:', attachmentId);
    
    res.json({
      id: attachmentId,
      versionId,
      fileName: file.originalname,
      fileSize: file.size,
      mimeType: file.mimetype || 'application/octet-stream',
      uploadedAt: now
    });
  } catch (error) {
    console.error('❌ File upload error:', error);
    res.status(500).json({ error: 'Failed to upload file' });
  }
});

// 🎯 버전의 첨부파일 목록 조회
router.get('/version/:versionId', (req, res) => {
  try {
    const { versionId } = req.params;
    
    const attachments = db.prepare(`
      SELECT id, version_id as versionId, file_name as fileName, 
             file_size as fileSize, mime_type as mimeType, uploaded_at as uploadedAt
      FROM attachments
      WHERE version_id = ?
      ORDER BY uploaded_at DESC
    `).all(versionId);
    
    res.json(attachments);
  } catch (error) {
    console.error('Error fetching attachments:', error);
    res.status(500).json({ error: 'Failed to fetch attachments' });
  }
});

// 🎯 파일 다운로드
router.get('/download/:attachmentId', (req, res) => {
  try {
    const { attachmentId } = req.params;
    
    const attachment = db.prepare(`
      SELECT file_name, file_path, mime_type
      FROM attachments
      WHERE id = ?
    `).get(attachmentId);
    
    if (!attachment) {
      return res.status(404).json({ error: 'Attachment not found' });
    }
    
    if (!existsSync(attachment.file_path)) {
      return res.status(404).json({ error: 'File not found on disk' });
    }
    
    const fileData = readFileSync(attachment.file_path);
    
    res.setHeader('Content-Type', attachment.mime_type || 'application/octet-stream');
    res.setHeader('Content-Disposition', `attachment; filename="${attachment.file_name}"`);
    res.send(fileData);
  } catch (error) {
    console.error('File download error:', error);
    res.status(500).json({ error: 'Failed to download file' });
  }
});

// 🎯 파일 삭제
router.delete('/:attachmentId', (req, res) => {
  try {
    const { attachmentId } = req.params;
    
    const attachment = db.prepare(`
      SELECT file_path
      FROM attachments
      WHERE id = ?
    `).get(attachmentId);
    
    if (!attachment) {
      return res.status(404).json({ error: 'Attachment not found' });
    }
    
    // 파일 삭제
    if (existsSync(attachment.file_path)) {
      unlinkSync(attachment.file_path);
    }
    
    // DB에서 삭제
    db.prepare('DELETE FROM attachments WHERE id = ?').run(attachmentId);
    
    res.json({ success: true });
  } catch (error) {
    console.error('File delete error:', error);
    res.status(500).json({ error: 'Failed to delete file' });
  }
});

export default router;

