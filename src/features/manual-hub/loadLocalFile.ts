import { API_CONFIG } from '@/config/constants';

type ElectronReadResult = { success: boolean; content?: string; error?: string };

function getServerOrigin(): string {
  return API_CONFIG.BASE_URL.replace(/\/api\/?$/, '');
}

/**
 * Manual Hub에서 로컬 HTML을 읽습니다.
 * 1) Electron이면 `electronAPI.readFile`
 * 2) 그렇지 않으면 API 서버 `GET /api/workspace/file` (dev:all 등 브라우저 + localhost:9527)
 */
export async function readTextFileForManualHub(filePath: string): Promise<
  | { ok: true; content: string; source: 'electron' | 'http' }
  | { ok: false; message: string }
> {
  const trimmed = filePath.trim();
  if (!trimmed) {
    return { ok: false, message: '경로가 비어 있습니다.' };
  }

  const readFile = (
    window as Window & {
      electronAPI?: { readFile: (p: string) => Promise<ElectronReadResult> };
    }
  ).electronAPI?.readFile;

  if (typeof readFile === 'function') {
    try {
      const result = await readFile(trimmed);
      if (result?.success && typeof result.content === 'string') {
        return { ok: true, content: result.content, source: 'electron' };
      }
    } catch {
      // API 폴백 시도
    }
  }

  const origin = getServerOrigin();
  try {
    const res = await fetch(
      `${origin}/api/workspace/file?path=${encodeURIComponent(trimmed)}`
    );
    if (res.ok) {
      const content = await res.text();
      return { ok: true, content, source: 'http' };
    }
    const j = (await res.json().catch(() => null)) as { error?: string } | null;
    return { ok: false, message: j?.error || `HTTP ${res.status}` };
  } catch (e) {
    return {
      ok: false,
      message: e instanceof Error ? e.message : String(e),
    };
  }
}
