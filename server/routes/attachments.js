import express from 'express';
import multer from 'multer';
import supabase from '../database.js';
import { extname } from 'path';

const router = express.Router();

// 🎯 Multer 설정 - 메모리에 임시 저장 후 Supabase Storage로 업로드
const storage = multer.memoryStorage();

const upload = multer({
  storage: storage,
  limits: {
    fileSize: 100 * 1024 * 1024 // 100MB 제한
  }
});

// 🎯 파일 업로드 - Supabase Storage 사용
router.post('/upload', upload.single('file'), async (req, res) => {
  try {
    const { versionId } = req.body;
    const file = req.file;
    
    if (!versionId || !file) {
      console.error('❌ Missing fields:', { versionId: !!versionId, file: !!file });
      return res.status(400).json({ error: 'Missing required fields' });
    }
    
    // Multer는 Latin1로 인코딩하므로 UTF-8로 재변환
    const originalFileName = Buffer.from(file.originalname, 'latin1').toString('utf8');
    
    console.log('📥 Upload request received:', {
      versionId,
      file: {
        originalname: originalFileName,
        mimetype: file.mimetype,
        size: file.size
      }
    });
    
    // 고유한 파일 경로 생성
    const attachmentId = `att_${Date.now()}_${Math.random().toString(36).substr(2, 9)}`;
    const ext = extname(originalFileName);
    const storagePath = `attachments/${versionId}/${attachmentId}${ext}`;
    
    // Supabase Storage에 파일 업로드
    const { data: uploadData, error: uploadError } = await supabase.storage
      .from('api-verification')
      .upload(storagePath, file.buffer, {
        contentType: file.mimetype || 'application/octet-stream',
        upsert: false
      });
    
    if (uploadError) {
      console.error('❌ Supabase Storage upload error:', uploadError);
      throw uploadError;
    }
    
    console.log('✅ File uploaded to Supabase Storage:', storagePath);
    
    // 데이터베이스에 메타데이터 저장
    const now = new Date().toISOString();
    const { error: dbError } = await supabase
      .from('attachments')
      .insert({
        id: attachmentId,
        version_id: versionId,
        file_name: originalFileName,
        file_size: file.size,
        file_path: storagePath,
        mime_type: file.mimetype || 'application/octet-stream',
        uploaded_at: now
      });
    
    if (dbError) throw dbError;
    
    console.log('✅ Attachment metadata saved:', attachmentId);
    
    res.json({
      id: attachmentId,
      versionId,
      fileName: originalFileName,
      fileSize: file.size,
      mimeType: file.mimetype || 'application/octet-stream',
      uploadedAt: now
    });
  } catch (error) {
    console.error('❌ File upload error:', error);
    res.status(500).json({ error: 'Failed to upload file', details: error.message });
  }
});

// 🎯 버전의 첨부파일 목록 조회
router.get('/version/:versionId', async (req, res) => {
  try {
    const { versionId } = req.params;
    
    const { data: attachments, error } = await supabase
      .from('attachments')
      .select('id, version_id, file_name, file_size, mime_type, uploaded_at')
      .eq('version_id', versionId)
      .order('uploaded_at', { ascending: false });
    
    if (error) throw error;
    
    // 필드명 변환 (snake_case -> camelCase)
    const formattedAttachments = (attachments || []).map(att => ({
      id: att.id,
      versionId: att.version_id,
      fileName: att.file_name,
      fileSize: att.file_size,
      mimeType: att.mime_type,
      uploadedAt: att.uploaded_at
    }));
    
    res.json(formattedAttachments);
  } catch (error) {
    console.error('Error fetching attachments:', error);
    res.status(500).json({ error: 'Failed to fetch attachments', details: error.message });
  }
});

// 🎯 파일 다운로드
router.get('/download/:attachmentId', async (req, res) => {
  try {
    const { attachmentId } = req.params;
    
    // DB에서 메타데이터 조회
    const { data: attachment, error: dbError } = await supabase
      .from('attachments')
      .select('file_name, file_path, mime_type')
      .eq('id', attachmentId)
      .single();
    
    if (dbError) {
      if (dbError.code === 'PGRST116') {
        return res.status(404).json({ error: 'Attachment not found' });
      }
      throw dbError;
    }
    
    // Supabase Storage에서 파일 다운로드
    const { data: fileData, error: downloadError } = await supabase.storage
      .from('api-verification')
      .download(attachment.file_path);
    
    if (downloadError) {
      console.error('❌ Supabase Storage download error:', downloadError);
      throw downloadError;
    }
    
    // ArrayBuffer를 Buffer로 변환
    const buffer = Buffer.from(await fileData.arrayBuffer());
    
    // 한글 파일명을 올바르게 인코딩 (RFC 5987)
    const encodedFilename = encodeURIComponent(attachment.file_name);
    
    res.setHeader('Content-Type', attachment.mime_type || 'application/octet-stream');
    res.setHeader('Content-Disposition', `attachment; filename*=UTF-8''${encodedFilename}`);
    res.setHeader('Content-Length', buffer.length);
    res.send(buffer);
  } catch (error) {
    console.error('File download error:', error);
    res.status(500).json({ error: 'Failed to download file', details: error.message });
  }
});

// 🎯 파일 삭제
router.delete('/:attachmentId', async (req, res) => {
  try {
    const { attachmentId } = req.params;
    
    // DB에서 메타데이터 조회
    const { data: attachment, error: dbError } = await supabase
      .from('attachments')
      .select('file_path')
      .eq('id', attachmentId)
      .single();
    
    if (dbError) {
      if (dbError.code === 'PGRST116') {
        return res.status(404).json({ error: 'Attachment not found' });
      }
      throw dbError;
    }
    
    // Supabase Storage에서 파일 삭제
    const { error: storageError } = await supabase.storage
      .from('api-verification')
      .remove([attachment.file_path]);
    
    if (storageError) {
      console.warn('⚠️ Storage deletion failed (file may not exist):', storageError);
      // Storage 삭제 실패해도 계속 진행 (파일이 이미 없을 수 있음)
    }
    
    // DB에서 삭제
    const { error: deleteError } = await supabase
      .from('attachments')
      .delete()
      .eq('id', attachmentId);
    
    if (deleteError) throw deleteError;
    
    res.json({ success: true });
  } catch (error) {
    console.error('File delete error:', error);
    res.status(500).json({ error: 'Failed to delete file', details: error.message });
  }
});

export default router;
