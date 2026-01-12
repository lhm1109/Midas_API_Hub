import express from 'express';
import cors from 'cors';
import bodyParser from 'body-parser';
import versionsRouter from './routes/versions.js';
import dataRouter from './routes/data.js';
import debugRouter from './routes/debug.js';
import endpointsRouter from './routes/endpoints.js';
import apiRouter from './routes/api.js';
import attachmentsRouter from './routes/attachments.js';
import { syncRoutesToDatabase, printRouteMap } from './routeRegistry.js';
import { initDb } from './database.js';

const app = express();
const PORT = 9527; // 일반적인 포트를 피한 포트

// 미들웨어
app.use(cors());
app.use(bodyParser.json({ limit: '50mb' }));
app.use(bodyParser.urlencoded({ extended: true, limit: '50mb' }));

// 로깅 미들웨어
app.use((req, res, next) => {
  console.log(`${new Date().toISOString()} - ${req.method} ${req.path}`);
  next();
});

// API 라우트
app.use('/api/endpoints', endpointsRouter);
app.use('/api/versions', versionsRouter);
app.use('/api/data', dataRouter);
app.use('/api/debug', debugRouter);
app.use('/api/attachments', attachmentsRouter);

// 실제 비즈니스 API (자동 문서화)
app.use('/api/civil/db', apiRouter);
app.use('/api/civil/gen', apiRouter);
app.use('/api/gen/analysis', apiRouter);

// Health check
app.get('/health', (req, res) => {
  res.json({ status: 'OK', timestamp: new Date().toISOString() });
});

// 404 핸들러
app.use((req, res) => {
  res.status(404).json({ error: 'Not Found' });
});

// 에러 핸들러
app.use((err, req, res, next) => {
  console.error('Error:', err);
  res.status(500).json({ error: err.message });
});

// 데이터베이스 초기화 및 라우트 동기화
initDb();
syncRoutesToDatabase();
printRouteMap();

// 서버 시작
app.listen(PORT, () => {
  console.log(`
╔════════════════════════════════════════════════╗
║   🚀 API Verification Backend Server         ║
║                                                ║
║   Port: ${PORT}                                  ║
║   URL:  http://localhost:${PORT}                ║
║   Health: http://localhost:${PORT}/health      ║
║                                                ║
║   📖 Routes auto-synced to database           ║
╚════════════════════════════════════════════════╝
  `);
});

export default app;

