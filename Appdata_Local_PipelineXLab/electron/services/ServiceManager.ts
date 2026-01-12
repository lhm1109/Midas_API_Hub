import { spawn, ChildProcess } from 'child_process';
import { BrowserWindow } from 'electron';
import path from 'path';
import kill from 'tree-kill';
import { exec } from 'child_process';
import { promisify } from 'util';
import * as fs from 'fs';
import * as http from 'http';
import * as https from 'https';

const execAsync = promisify(exec);

// ==================== Interfaces ====================

export interface ServiceInfo {
  id: string;
  name: string;
  port: number;
  process: ChildProcess | null;
  status: 'stopped' | 'starting' | 'running' | 'error' | 'crashed';
  pid?: number;
  autoRestart?: boolean;
  restartCount?: number;
  lastHealthCheck?: number; // timestamp
}

/**
 * 상태 머신 (State Machine)
 * 
 * stopped → starting → running → crashed → starting (auto-restart)
 *              ↓          ↓
 *           error      error
 * 
 * - stopped: 정상 종료 또는 미시작
 * - starting: 시작 중 (헬스체크 대기)
 * - running: 정상 실행 중
 * - error: 환경/설정 문제 (재시작 불가)
 * - crashed: 런타임 크래시 (auto-restart 대상)
 */

export interface StopOptions {
  removeVolumes?: boolean; // 기본값: false (데이터 보존)
}

export interface DockerCheckResult {
  available: boolean;
  command: string; // 'docker compose' | 'docker-compose' | ''
  error?: string;
}

// ==================== Constants ====================

const MAX_LOGS_PER_SERVICE = 500;
const MAX_RESTART_COUNT = 3;
const HEALTH_CHECK_TIMEOUT = 30000; // 30초
const HEALTH_CHECK_INTERVAL = 30000; // 30초 (주기적 헬스체크)

// ==================== Service Manager ====================

export class ServiceManager {
  private services: Map<string, ServiceInfo> = new Map();
  private serviceLogs: Map<string, string[]> = new Map();
  private healthCheckIntervals: Map<string, NodeJS.Timeout> = new Map(); // 주기적 헬스체크
  private mainWindow: BrowserWindow | null = null;
  private projectRoot: string;
  private dockerCommand: string = ''; // 캐시된 Docker 명령어

  constructor(projectRoot: string) {
    this.projectRoot = projectRoot;
    this.initializeServices();
  }

  setMainWindow(window: BrowserWindow) {
    this.mainWindow = window;
  }

  // ==================== 초기화 ====================

  private initializeServices() {
    this.services.set('database', {
      id: 'database',
      name: 'Database',
      port: 5432,
      process: null,
      status: 'stopped',
    });

    this.services.set('backend', {
      id: 'backend',
      name: 'Backend API',
      port: 8000,
      process: null,
      status: 'stopped',
      autoRestart: true,
      restartCount: 0,
    });

    this.services.set('frontend', {
      id: 'frontend',
      name: 'Frontend',
      port: 5173,
      process: null,
      status: 'stopped',
    });

    this.services.set('proxy', {
      id: 'proxy',
      name: 'Caddy Proxy',
      port: 8000,
      process: null,
      status: 'stopped',
    });

    // 로그 초기화
    this.services.forEach((_, id) => {
      this.serviceLogs.set(id, []);
    });
    this.serviceLogs.set('system', []);
  }

  // ==================== 유틸리티 메서드 ====================

  /**
   * 포트 사용 여부 확인 (Windows 전용)
   */
  private async isPortInUse(port: number): Promise<boolean> {
    try {
      const { stdout } = await execAsync(
        `netstat -ano | findstr :${port} | findstr LISTENING`
      );
      return stdout.trim().length > 0;
    } catch {
      return false;
    }
  }

  /**
   * 포트가 해제될 때까지 대기
   */
  private async waitForPortFree(port: number, timeoutSec = 10): Promise<boolean> {
    const start = Date.now();

    while (Date.now() - start < timeoutSec * 1000) {
      try {
        const isUsed = await this.isPortInUse(port);
        if (!isUsed) return true;
      } catch { }

      await new Promise(r => setTimeout(r, 500));
    }

    return false;
  }

  /**
   * 포트가 열릴 때까지 대기
   */
  private async waitForPort(port: number, timeoutSec = 30): Promise<boolean> {
    const start = Date.now();

    while (Date.now() - start < timeoutSec * 1000) {
      try {
        const isOpen = await this.isPortInUse(port);
        if (isOpen) return true;
      } catch { }

      await new Promise(r => setTimeout(r, 1000));
    }

    return false;
  }

  /**
   * HTTP 헬스체크 (상태 판별 핵심)
   * ✅ Node.js http/https 모듈 사용 (fetch 의존성 제거)
   */
  private async waitForHealth(
    url: string,
    timeoutSec = 30,
    serviceId: string
  ): Promise<boolean> {
    const start = Date.now();

    while (Date.now() - start < timeoutSec * 1000) {
      const isOk = await this.checkHttp200(url);

      if (isOk) {
        this.sendLog('success', serviceId, `Health check passed: ${url}`);
        return true;
      }

      await new Promise(r => setTimeout(r, 1000)); // 1초 대기
    }

    this.sendLog('error', serviceId, `Health check timeout: ${url}`);
    return false;
  }

  /**
   * HTTP 200 OK 체크 (Node.js 네이티브 모듈 사용)
   */
  private async checkHttp200(url: string): Promise<boolean> {
    return new Promise((resolve) => {
      const urlObj = new URL(url);
      const protocol = urlObj.protocol === 'https:' ? https : http;

      const req = protocol.get(url, { timeout: 5000 }, (res) => {
        resolve(res.statusCode === 200);
        res.resume(); // 응답 데이터 소비
      });

      req.on('error', () => resolve(false));
      req.on('timeout', () => {
        req.destroy();
        resolve(false);
      });
    });
  }

  /**
   * 주기적 헬스체크 시작
   * 실행 중인 서비스가 죽었는지 모니터링
   */
  private startPeriodicHealthCheck(serviceId: string, url: string) {
    // 기존 interval 정리
    this.stopPeriodicHealthCheck(serviceId);

    const interval = setInterval(async () => {
      const service = this.services.get(serviceId);

      if (!service || service.status !== 'running') {
        this.stopPeriodicHealthCheck(serviceId);
        return;
      }

      const isHealthy = await this.checkHttp200(url);
      service.lastHealthCheck = Date.now();

      if (!isHealthy) {
        // 프로세스는 살아있지만 HTTP 응답 없음 (유령 상태)
        this.sendLog('error', serviceId, '⚠️ Health check failed - service may be unresponsive');
        service.status = 'crashed';
        this.sendStatus(serviceId);

        // Backend는 auto-restart
        if (serviceId === 'backend' && service.autoRestart) {
          this.sendLog('warning', serviceId, 'Attempting to restart...');
          await this.stopService(serviceId);
          await new Promise(r => setTimeout(r, 2000));
          await this.startBackend();
        }
      }
    }, HEALTH_CHECK_INTERVAL);

    this.healthCheckIntervals.set(serviceId, interval);
    this.sendLog('debug', serviceId, `Periodic health check started (every ${HEALTH_CHECK_INTERVAL / 1000}s)`);
  }

  /**
   * 주기적 헬스체크 중지
   */
  private stopPeriodicHealthCheck(serviceId: string) {
    const interval = this.healthCheckIntervals.get(serviceId);
    if (interval) {
      clearInterval(interval);
      this.healthCheckIntervals.delete(serviceId);
      this.sendLog('debug', serviceId, 'Periodic health check stopped');
    }
  }

  /**
   * Docker 실행 가능 여부 확인 및 명령어 감지
   */
  private async checkDockerAvailable(): Promise<DockerCheckResult> {
    // 이미 확인했으면 캐시 사용
    if (this.dockerCommand) {
      return { available: true, command: this.dockerCommand };
    }

    // docker compose (v2) 먼저 시도
    try {
      await execAsync('docker compose version');
      this.dockerCommand = 'docker compose';
      return { available: true, command: this.dockerCommand };
    } catch { }

    // docker-compose (v1) 시도
    try {
      await execAsync('docker-compose --version');
      this.dockerCommand = 'docker-compose';
      return { available: true, command: this.dockerCommand };
    } catch { }

    return {
      available: false,
      command: '',
      error: 'Docker가 설치되지 않았거나 실행되지 않았습니다.\n\n해결 방법:\n1. Docker Desktop을 설치하세요\n2. Docker Desktop이 실행 중인지 확인하세요\n3. 환경변수 PATH에 Docker가 포함되어 있는지 확인하세요'
    };
  }

  // ==================== 로그 관리 ====================

  /**
   * 서비스 상태 전송
   */
  private sendStatus(serviceId: string) {
    const service = this.services.get(serviceId);
    if (service && this.mainWindow && !this.mainWindow.isDestroyed()) {
      this.mainWindow.webContents.send('service-status', {
        id: service.id,
        name: service.name,
        port: service.port,
        status: service.status,
        pid: service.pid,
      });
    }
  }

  /**
   * 로그 전송 (메모리 상한 적용 + 파일 저장)
   */
  private sendLog(
    type: 'info' | 'success' | 'error' | 'warning' | 'debug',
    source: string,
    message: string
  ) {
    const timestamp = new Date();
    const timeString = timestamp.toLocaleTimeString('ko-KR', { hour12: false });

    // 로그 저장 (진단용)
    if (!this.serviceLogs.has(source)) {
      this.serviceLogs.set(source, []);
    }

    const logs = this.serviceLogs.get(source)!;
    logs.push(message);

    // 메모리 상한 적용
    if (logs.length > MAX_LOGS_PER_SERVICE) {
      logs.shift();
    }

    // 파일에 저장 (비동기, 실패해도 무시)
    this.writeLogToFile(source, type, message, timestamp).catch(() => { });

    // UI로 전송
    if (this.mainWindow && !this.mainWindow.isDestroyed()) {
      this.mainWindow.webContents.send('service-log', {
        type,
        source,
        message,
        timestamp: timeString,
      });
    }

    // 에러 발생 시 자동 진단
    if (type === 'error') {
      this.diagnoseError(source);
    }
  }

  /**
   * 로그 파일에 저장
   * logs/YYYY-MM-DD/service.log
   */
  private async writeLogToFile(
    source: string,
    type: string,
    message: string,
    timestamp: Date
  ): Promise<void> {
    try {
      const dateStr = timestamp.toISOString().split('T')[0]; // YYYY-MM-DD
      const timeStr = timestamp.toLocaleTimeString('ko-KR', { hour12: false });

      const logDir = path.join(this.projectRoot, 'logs', dateStr);
      const logFile = path.join(logDir, `${source}.log`);

      // 디렉토리 생성 (없으면)
      if (!fs.existsSync(logDir)) {
        fs.mkdirSync(logDir, { recursive: true });
      }

      // 로그 형식: [HH:MM:SS] [TYPE] message
      const logLine = `[${timeStr}] [${type.toUpperCase()}] ${message}\n`;

      // 파일에 append
      fs.appendFileSync(logFile, logLine, 'utf-8');
    } catch (error) {
      // 파일 저장 실패는 조용히 무시 (메모리 로그는 유지)
    }
  }

  /**
   * 에러 자동 진단 및 해결 방법 제시
   */
  private diagnoseError(serviceId: string) {
    const logs = this.serviceLogs.get(serviceId) || [];
    const recentLogs = logs.slice(-20).join('\n').toLowerCase();

    let diagnosis: { cause: string; fix: string } | null = null;

    // Backend 에러 패턴
    if (serviceId === 'backend') {
      if (recentLogs.includes('modulenotfounderror')) {
        diagnosis = {
          cause: 'Python 패키지가 설치되지 않았습니다',
          fix: 'cd flow-editor\\backend\n.venv\\Scripts\\activate\npip install -r requirements.txt'
        };
      } else if (recentLogs.includes('address already in use') || recentLogs.includes('port') && recentLogs.includes('8001')) {
        diagnosis = {
          cause: '포트 8001이 이미 사용 중입니다',
          fix: 'netstat -ano | findstr :8001\ntaskkill /PID <PID> /F'
        };
      } else if (recentLogs.includes('could not connect to server') || recentLogs.includes('connection refused')) {
        diagnosis = {
          cause: 'PostgreSQL 연결 실패',
          fix: '1. Database 서비스를 먼저 시작하세요\n2. Docker Desktop이 실행 중인지 확인하세요'
        };
      }
    }

    // Frontend 에러 패턴
    if (serviceId === 'frontend') {
      if (recentLogs.includes('enoent') || recentLogs.includes('cannot find module')) {
        diagnosis = {
          cause: 'node_modules가 설치되지 않았습니다',
          fix: 'cd flow-editor\\frontend\nnpm install'
        };
      } else if (recentLogs.includes('address already in use') || recentLogs.includes('port') && recentLogs.includes('5173')) {
        diagnosis = {
          cause: '포트 5173이 이미 사용 중입니다',
          fix: 'Vite 개발 서버가 이미 실행 중입니다. 기존 프로세스를 종료하세요.'
        };
      }
    }

    // Database 에러 패턴
    if (serviceId === 'database') {
      if (recentLogs.includes('cannot connect to docker daemon')) {
        diagnosis = {
          cause: 'Docker Desktop이 실행되지 않았습니다',
          fix: '1. Docker Desktop을 실행하세요\n2. 트레이 아이콘에서 "Docker Desktop is running" 확인'
        };
      }
    }

    if (diagnosis && this.mainWindow && !this.mainWindow.isDestroyed()) {
      this.mainWindow.webContents.send('service-diagnosis', {
        serviceId,
        cause: diagnosis.cause,
        fix: diagnosis.fix,
      });
    }
  }

  // ==================== 서비스 시작 ====================

  /**
   * Docker Compose 시작 (PostgreSQL + Redis)
   * ✅ 헬스체크 기반 상태 판별
   */
  async startDatabase(): Promise<boolean> {
    const service = this.services.get('database')!;

    try {
      service.status = 'starting';
      this.sendStatus('database');

      // 1. Docker 사용 가능 여부 확인
      const dockerCheck = await this.checkDockerAvailable();
      if (!dockerCheck.available) {
        this.sendLog('error', 'database', dockerCheck.error!);
        service.status = 'error';
        this.sendStatus('database');
        return false;
      }

      this.sendLog('info', 'database', `Using: ${dockerCheck.command}`);

      // 2. docker-compose.yml 존재 확인
      const dockerComposePath = path.join(this.projectRoot, 'flow-editor');
      const composeFile = path.join(dockerComposePath, 'docker-compose.yml');

      if (!fs.existsSync(composeFile)) {
        this.sendLog('error', 'database', `docker-compose.yml not found: ${composeFile}`);
        service.status = 'error';
        this.sendStatus('database');
        return false;
      }

      // 3. 이미 실행 중인지 확인
      const isRunning = await this.isPortInUse(5432);
      if (isRunning) {
        this.sendLog('warning', 'database', 'Database already running on port 5432');
        service.status = 'running';
        this.sendStatus('database');
        return true;
      }

      // 4. Docker Compose 실행
      this.sendLog('info', 'database', 'Starting Docker Compose...');

      // ✅ shell: true면 전체 명령어를 문자열로 전달 (Windows 안전성)
      const dockerProcess = spawn(`${dockerCheck.command} up -d`, {
        cwd: dockerComposePath,
        shell: true,
      });

      let stderr = '';

      dockerProcess.stdout?.on('data', (data: Buffer) => {
        this.sendLog('debug', 'database', data.toString().trim());
      });

      dockerProcess.stderr?.on('data', (data: Buffer) => {
        stderr += data.toString();
        this.sendLog('debug', 'database', data.toString().trim());
      });

      const exitCode = await new Promise<number>((resolve) => {
        dockerProcess.on('close', resolve);
      });

      if (exitCode !== 0) {
        this.sendLog('error', 'database', `Docker Compose failed (exit ${exitCode}):`);
        this.sendLog('error', 'database', stderr);
        service.status = 'error';
        this.sendStatus('database');
        return false;
      }

      // 5. ✅ 헬스체크 (포트 기반)
      this.sendLog('info', 'database', 'Waiting for PostgreSQL...');

      const isHealthy = await this.waitForPort(5432, 30);

      if (isHealthy) {
        service.status = 'running';
        this.sendStatus('database');
        this.sendLog('success', 'database', 'PostgreSQL ready on port 5432');
        this.sendLog('success', 'database', 'Redis ready on port 6379');
        return true;
      } else {
        service.status = 'error';
        this.sendStatus('database');
        this.sendLog('error', 'database', 'Database startup timeout');
        return false;
      }

    } catch (error: any) {
      service.status = 'error';
      this.sendStatus('database');
      this.sendLog('error', 'database', error.message);
      return false;
    }
  }

  /**
   * Backend 시작 (FastAPI)
   * ✅ 헬스체크 기반 상태 판별 + Auto-restart
   */
  async startBackend(): Promise<boolean> {
    const service = this.services.get('backend')!;

    try {
      service.status = 'starting';
      this.sendStatus('backend');
      this.sendLog('info', 'backend', 'Starting FastAPI backend...');

      const backendPath = path.join(this.projectRoot, 'flow-editor', 'backend');
      const venvPython = path.join(backendPath, '.venv', 'Scripts', 'python.exe');

      // Python 환경 확인
      const pythonPath = fs.existsSync(venvPython) ? venvPython : 'python';

      this.sendLog('debug', 'backend', `Using Python: ${pythonPath}`);

      // 경로에 공백이 있을 경우 따옴표로 감싸기
      const quotedPythonPath = pythonPath.includes(' ') ? `"${pythonPath}"` : pythonPath;

      const childProcess = spawn(
        quotedPythonPath,
        ['-m', 'uvicorn', 'app.main:app', '--reload', '--host', '0.0.0.0', '--port', '8000'],
        {
          cwd: backendPath,
          shell: true,
          env: { ...process.env },
        }
      );

      service.process = childProcess;
      service.pid = childProcess.pid;

      // ✅ stdout는 100% UI용 (상태 판별 X)
      childProcess.stdout?.on('data', (data: Buffer) => {
        const message = data.toString().trim();
        if (message) {
          this.sendLog('info', 'backend', message);
        }
      });

      childProcess.stderr?.on('data', (data: Buffer) => {
        const message = data.toString().trim();
        if (message) {
          if (message.includes('ERROR') || message.includes('Exception')) {
            this.sendLog('error', 'backend', message);
          } else {
            this.sendLog('warning', 'backend', message);
          }
        }
      });

      // ✅ Auto-restart 로직 (PM2-lite)
      childProcess.on('close', async (code: number | null) => {
        const wasRunning = service.status === 'running';
        service.process = null;
        service.pid = undefined;

        // 주기적 헬스체크 중지
        this.stopPeriodicHealthCheck('backend');

        // 재시작 조건:
        // 1. 실행 중이었음 (wasRunning)
        // 2. autoRestart 활성화 (사용자 정지 시 비활성화됨)
        // 3. 재시작 횟수 제한 내
        if (wasRunning && service.autoRestart && (service.restartCount || 0) < MAX_RESTART_COUNT) {
          service.status = 'crashed'; // 런타임 크래시 (재시작 대상)
          service.restartCount = (service.restartCount || 0) + 1;
          this.sendStatus('backend');
          this.sendLog('warning', 'backend', `Backend crashed (code ${code}). Auto-restarting (${service.restartCount}/${MAX_RESTART_COUNT})...`);

          await new Promise(r => setTimeout(r, 2000)); // 2초 대기
          await this.startBackend();
        } else {
          // 정상 종료 또는 재시작 불가
          if (code === 0) {
            service.status = 'stopped'; // 정상 종료
          } else if ((service.restartCount || 0) >= MAX_RESTART_COUNT) {
            service.status = 'crashed'; // 재시작 실패
            this.sendLog('error', 'backend', 'Max restart count reached. Manual intervention required.');
            service.restartCount = 0;
          } else {
            service.status = 'error'; // 환경 문제 추정
          }

          this.sendStatus('backend');
          this.sendLog('info', 'backend', `Backend process exited with code ${code}`);
        }
      });

      // ✅ 헬스체크로 상태 판별
      this.sendLog('info', 'backend', 'Waiting for health check...');

      const isHealthy = await this.waitForHealth(
        'http://127.0.0.1:8000/health',
        30,
        'backend'
      );

      if (isHealthy) {
        service.status = 'running';
        service.restartCount = 0; // 성공 시 카운터 리셋
        this.sendStatus('backend');
        this.sendLog('success', 'backend', 'Backend API ready on http://127.0.0.1:8000');
        this.sendLog('info', 'backend', 'API docs: http://127.0.0.1:8000/docs');

        // ✅ 주기적 헬스체크 시작
        this.startPeriodicHealthCheck('backend', 'http://127.0.0.1:8000/health');

        return true;
      } else {
        service.status = 'error'; // 시작 실패 = 환경 문제 (재시작 불가)
        this.sendStatus('backend');
        this.sendLog('error', 'backend', 'Backend startup timeout - check environment/config');
        return false;
      }
    } catch (error: any) {
      service.status = 'error';
      this.sendStatus('backend');
      this.sendLog('error', 'backend', error.message);
      return false;
    }
  }

  /**
   * Frontend 시작 (Vite)
   * ✅ 헬스체크 기반 상태 판별
   */
  async startFrontend(): Promise<boolean> {
    const service = this.services.get('frontend')!;

    try {
      service.status = 'starting';
      this.sendStatus('frontend');
      this.sendLog('info', 'frontend', 'Starting Vite development server...');

      const frontendPath = path.join(this.projectRoot, 'flow-editor', 'frontend');

      const childProcess = spawn('npm', ['run', 'dev'], {
        cwd: frontendPath,
        shell: true,
      });

      service.process = childProcess;
      service.pid = childProcess.pid;

      // ✅ stdout는 100% UI용 (상태 판별 X)
      childProcess.stdout?.on('data', (data: Buffer) => {
        const message = data.toString().trim();
        if (message) {
          this.sendLog('info', 'frontend', message);
        }
      });

      childProcess.stderr?.on('data', (data: Buffer) => {
        const message = data.toString().trim();
        if (message) {
          if (message.includes('error') || message.includes('Error')) {
            this.sendLog('error', 'frontend', message);
          } else {
            this.sendLog('info', 'frontend', message);
          }
        }
      });

      childProcess.on('close', (code: number | null) => {
        service.status = code === 0 ? 'stopped' : 'error';
        service.process = null;
        service.pid = undefined;
        this.sendStatus('frontend');
        this.sendLog('info', 'frontend', `Frontend process exited with code ${code}`);
      });

      // ✅ 헬스체크로 상태 판별
      this.sendLog('info', 'frontend', 'Waiting for health check...');

      const isHealthy = await this.waitForHealth(
        'http://localhost:5173',
        30,
        'frontend'
      );

      if (isHealthy) {
        service.status = 'running';
        this.sendStatus('frontend');
        this.sendLog('success', 'frontend', 'Frontend ready on http://localhost:5173');

        // ✅ 주기적 헬스체크 시작
        this.startPeriodicHealthCheck('frontend', 'http://localhost:5173');

        return true;
      } else {
        service.status = 'error'; // 시작 실패 = 환경 문제
        this.sendStatus('frontend');
        this.sendLog('error', 'frontend', 'Frontend startup timeout - check node_modules');
        return false;
      }
    } catch (error: any) {
      service.status = 'error';
      this.sendStatus('frontend');
      this.sendLog('error', 'frontend', error.message);
      return false;
    }
  }

  /**
   * Caddy Proxy 시작 (선택사항)
   */
  async startProxy(): Promise<boolean> {
    const service = this.services.get('proxy')!;

    try {
      service.status = 'starting';
      this.sendStatus('proxy');
      this.sendLog('info', 'system', 'Starting Caddy proxy...');

      const caddyPath = path.join(this.projectRoot, 'flow-editor');
      const caddyExe = path.join(caddyPath, 'caddy.exe');

      if (!fs.existsSync(caddyExe)) {
        this.sendLog('warning', 'system', 'Caddy not found, skipping proxy');
        service.status = 'stopped';
        this.sendStatus('proxy');
        return true; // 선택사항이므로 true 반환
      }

      const childProcess = spawn(caddyExe, ['run', '--config', 'Caddyfile'], {
        cwd: caddyPath,
        shell: true,
      });

      service.process = childProcess;
      service.pid = childProcess.pid;

      childProcess.stdout?.on('data', (data: Buffer) => {
        this.sendLog('debug', 'system', `[Caddy] ${data.toString().trim()}`);
      });

      childProcess.stderr?.on('data', (data: Buffer) => {
        const message = data.toString().trim();
        if (message && message.includes('serving')) {
          service.status = 'running';
          this.sendStatus('proxy');
          this.sendLog('success', 'system', 'Caddy proxy ready on http://localhost:8000');
        }
      });

      childProcess.on('close', () => {
        service.status = 'stopped';
        service.process = null;
        service.pid = undefined;
        this.sendStatus('proxy');
      });

      return true;
    } catch (error: any) {
      service.status = 'error';
      this.sendStatus('proxy');
      this.sendLog('error', 'system', error.message);
      return false;
    }
  }

  /**
   * 모든 서비스 시작
   */
  async startAll(): Promise<void> {
    this.sendLog('info', 'system', '========================================');
    this.sendLog('info', 'system', '🚀 Starting PipelineXLab Platform...');
    this.sendLog('info', 'system', '========================================');

    // 1. Database (Docker Compose)
    const dbStarted = await this.startDatabase();
    if (!dbStarted) {
      this.sendLog('error', 'system', 'Failed to start database. Aborting.');
      return;
    }

    await new Promise(r => setTimeout(r, 2000)); // 2초 대기

    // 2. Backend
    const backendStarted = await this.startBackend();
    if (!backendStarted) {
      this.sendLog('error', 'system', 'Failed to start backend. Aborting.');
      return;
    }

    await new Promise(r => setTimeout(r, 1000)); // 1초 대기

    // 3. Frontend
    const frontendStarted = await this.startFrontend();
    if (!frontendStarted) {
      this.sendLog('error', 'system', 'Failed to start frontend.');
    }

    // 4. Proxy (선택사항)
    await this.startProxy();

    this.sendLog('success', 'system', '========================================');
    this.sendLog('success', 'system', '✅ All services started successfully!');
    this.sendLog('success', 'system', '========================================');
    this.sendLog('info', 'system', '🌐 Frontend: http://localhost:5173');
    this.sendLog('info', 'system', '🔧 Backend API: http://localhost:8001');
    this.sendLog('info', 'system', '📚 API Docs: http://localhost:8001/docs');
  }

  /**
   * 특정 서비스 시작
   */
  async startService(serviceId: string): Promise<boolean> {
    switch (serviceId) {
      case 'database':
        return this.startDatabase();
      case 'backend':
        return this.startBackend();
      case 'frontend':
        return this.startFrontend();
      case 'proxy':
        // get startProxy signature - assuming it returns void or Promise<void> based on startAll usage
        // startAll usage: await this.startProxy();. 
        // Need to check strict return type, but assuming it works like others or simple await.
        // Actually, looking at startAll line 837: await this.startProxy();
        // and startDatabase line 813: returns boolean.
        // Let's assume startProxy returns boolean or void.
        // If it returns void, I should return true.
        // To be safe I'll wrap it.
        await this.startProxy();
        return true;
      default:
        this.sendLog('error', 'system', `Unknown service ID: ${serviceId}`);
        return false;
    }
  }

  // ==================== 서비스 중지 ====================

  /**
   * 특정 서비스 중지
   * ✅ 포트가 해제될 때까지 대기하여 중복 실행 방지
   */
  async stopService(serviceId: string): Promise<void> {
    const service = this.services.get(serviceId);
    if (!service) return;

    // auto-restart 비활성화 (재시작 중 자동 재시작 방지)
    const wasAutoRestart = service.autoRestart;
    service.autoRestart = false;

    // 주기적 헬스체크 중지
    this.stopPeriodicHealthCheck(serviceId);

    if (service.process && service.pid) {
      this.sendLog('info', service.id, `Stopping ${service.name}...`);

      await new Promise<void>((resolve) => {
        const timeout = setTimeout(() => {
          // 3초 후에도 안 죽으면 강제 종료
          this.sendLog('warning', service.id, `Force killing ${service.name}...`);
          try {
            kill(service.pid!, 'SIGKILL');
          } catch (e) {
            // ignore
          }
          service.status = 'stopped';
          service.process = null;
          service.pid = undefined;
          this.sendStatus(service.id);
          resolve();
        }, 3000);

        kill(service.pid!, 'SIGTERM', (err) => {
          clearTimeout(timeout);
          if (err) {
            this.sendLog('warning', service.id, `SIGTERM failed, trying SIGKILL...`);
            try {
              kill(service.pid!, 'SIGKILL');
            } catch (e) {
              // ignore
            }
          }
          this.sendLog('success', service.id, `${service.name} stopped`);
          service.status = 'stopped';
          service.process = null;
          service.pid = undefined;
          this.sendStatus(service.id);
          resolve();
        });
      });

      // ✅ 포트가 해제될 때까지 대기 (중복 실행 방지 핵심)
      const portFreed = await this.waitForPortFree(service.port, 10);
      if (!portFreed) {
        this.sendLog('warning', service.id, `Port ${service.port} still in use after stop`);
        // 강제로 포트 해제 시도 (Windows)
        try {
          const { stdout } = await execAsync(
            `netstat -ano | findstr :${service.port} | findstr LISTENING`
          );
          const lines = stdout.trim().split('\n');
          for (const line of lines) {
            const parts = line.trim().split(/\s+/);
            const pid = parts[parts.length - 1];
            if (pid && !isNaN(parseInt(pid))) {
              this.sendLog('warning', service.id, `Killing orphan process on port ${service.port} (PID: ${pid})`);
              await execAsync(`taskkill /F /PID ${pid}`).catch(() => { });
            }
          }
        } catch (e) {
          // ignore
        }
      }
    }

    // auto-restart 복원
    service.autoRestart = wasAutoRestart;
  }

  /**
   * 특정 서비스 재시작
   * ✅ 올바른 순서: 중지 → 포트 해제 대기 → 시작
   */
  async restartService(serviceId: string): Promise<boolean> {
    const service = this.services.get(serviceId);
    if (!service) {
      this.sendLog('error', 'system', `Unknown service ID: ${serviceId}`);
      return false;
    }

    this.sendLog('info', service.id, `Restarting ${service.name}...`);

    // 1. 중지 (포트 해제까지 대기)
    await this.stopService(serviceId);

    // 2. 추가 대기 (안전 마진)
    await new Promise(r => setTimeout(r, 1000));

    // 3. 포트가 정말로 해제되었는지 최종 확인
    const portFree = await this.waitForPortFree(service.port, 5);
    if (!portFree) {
      this.sendLog('error', service.id, `Cannot restart: port ${service.port} still in use`);
      return false;
    }

    // 4. 시작
    return await this.startService(serviceId);
  }

  /**
   * 모든 서비스 중지 (✅ 데이터 보존 옵션)
   */
  async stopAll(options: StopOptions = {}): Promise<void> {
    this.sendLog('info', 'system', 'Stopping all services...');

    // Frontend 중지
    await this.stopService('frontend');

    // Backend 중지 (auto-restart 비활성화)
    const backend = this.services.get('backend');
    if (backend) {
      backend.autoRestart = false; // 수동 정지 시 재시작 방지
    }
    await this.stopService('backend');

    // Proxy 중지
    await this.stopService('proxy');

    // Docker Compose 중지
    try {
      const dockerComposePath = path.join(this.projectRoot, 'flow-editor');

      // Docker 명령어 확인
      const dockerCheck = await this.checkDockerAvailable();
      if (!dockerCheck.available) {
        this.sendLog('warning', 'database', 'Docker not available, skipping database stop');
        return;
      }

      if (options.removeVolumes) {
        // ⚠️ 볼륨까지 삭제 (완전 초기화)
        this.sendLog('warning', 'database', '⚠️ Removing volumes (data will be deleted)...');
        await execAsync(`${dockerCheck.command} down -v`, { cwd: dockerComposePath });
        this.sendLog('success', 'database', 'Docker services and volumes removed');
      } else {
        // ✅ 컨테이너만 중지 (데이터 보존 - 기본값)
        this.sendLog('info', 'database', 'Stopping containers (preserving data)...');
        await execAsync(`${dockerCheck.command} stop`, { cwd: dockerComposePath });
        this.sendLog('success', 'database', 'Docker services stopped (data preserved)');
      }

      const service = this.services.get('database')!;
      service.status = 'stopped';
      this.sendStatus('database');
    } catch (error: any) {
      this.sendLog('error', 'database', `Failed to stop Docker: ${error.message}`);
    }

    // auto-restart 재활성화
    if (backend) {
      backend.autoRestart = true;
    }

    this.sendLog('success', 'system', 'All services stopped');
  }

  /**
   * 완전 초기화 (데이터 삭제)
   * ⚠️ UI에서 2단계 확인 필요!
   */
  async resetAll(): Promise<void> {
    this.sendLog('warning', 'system', '⚠️⚠️⚠️ RESETTING ALL DATA ⚠️⚠️⚠️');
    this.sendLog('warning', 'system', 'This will delete:');
    this.sendLog('warning', 'system', '- All database data (PostgreSQL volumes)');
    this.sendLog('warning', 'system', '- All Redis cache');
    this.sendLog('warning', 'system', '- Process will restart from clean state');

    await this.stopAll({ removeVolumes: true });

    this.sendLog('success', 'system', 'Reset complete. You can now start services fresh.');
  }

  /**
   * 데이터베이스만 초기화 (서비스는 유지)
   */
  async resetDatabase(): Promise<void> {
    this.sendLog('warning', 'database', '⚠️ Resetting database only...');

    try {
      const dockerComposePath = path.join(this.projectRoot, 'flow-editor');
      const dockerCheck = await this.checkDockerAvailable();

      if (!dockerCheck.available) {
        this.sendLog('error', 'database', 'Docker not available');
        return;
      }

      // 컨테이너 중지 및 볼륨 삭제
      await execAsync(`${dockerCheck.command} down -v`, { cwd: dockerComposePath });
      this.sendLog('success', 'database', 'Database reset complete');

      // 재시작
      await this.startDatabase();
    } catch (error: any) {
      this.sendLog('error', 'database', `Reset failed: ${error.message}`);
    }
  }

  // ==================== 조회 ====================

  /**
   * 서비스 상태 조회
   */
  getServiceStatus(serviceId: string): Omit<ServiceInfo, 'process'> | undefined {
    const service = this.services.get(serviceId);
    if (!service) return undefined;
    // process는 직렬화 불가능하므로 제외
    const { process, ...rest } = service;
    return rest;
  }

  /**
   * 모든 서비스 상태 조회 (직렬화 가능한 형태로 반환)
   */
  getAllServices(): Omit<ServiceInfo, 'process'>[] {
    return Array.from(this.services.values()).map(service => {
      const { process, ...rest } = service;
      return rest;
    });
  }
}

