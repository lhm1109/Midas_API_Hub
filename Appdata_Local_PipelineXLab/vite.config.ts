import { defineConfig } from 'vite';
import react from '@vitejs/plugin-react';

// https://vitejs.dev/config/
export default defineConfig({
    plugins: [react()],
    server: {
        port: 5180, // 런처 전용 포트 (flow-editor는 5173 사용)
        strictPort: true, // 포트가 사용 중이면 에러 발생
    },
});
