import * as crypto from 'crypto';

/**
 * SHA256 해시 생성
 */
export function sha256(content: string): string {
    return 'sha256:' + crypto.createHash('sha256').update(content, 'utf8').digest('hex');
}
