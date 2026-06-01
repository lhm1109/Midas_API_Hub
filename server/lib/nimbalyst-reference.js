import fs from 'fs/promises';
import path from 'path';
import { spawn } from 'child_process';

const DEFAULT_REFERENCE_PATH = path.resolve(process.cwd(), 'docs/REFERENCE/nimbalyst');

let launchedProcess = null;
let launchedAt = null;
let recentLog = [];
let installProcess = null;
let installStartedAt = null;
let installFinishedAt = null;
let installExit = null;
let installLog = [];

function appendBoundedLog(target, chunk, limit = 80) {
  const text = String(chunk || '');
  if (!text.trim()) return target;
  return [...target, ...text.split(/\r?\n/).filter(Boolean).slice(-20)].slice(-limit);
}

function appendLog(chunk) {
  recentLog = appendBoundedLog(recentLog, chunk);
}

function appendInstallLog(chunk) {
  installLog = appendBoundedLog(installLog, chunk, 140);
}

function parseNodeVersion(value) {
  const match = String(value || '').match(/v?(\d+)\.(\d+)\.(\d+)/);
  if (!match) return null;
  return {
    major: Number(match[1]),
    minor: Number(match[2]),
    patch: Number(match[3]),
  };
}

function isNodeVersionSupported(value = process.version) {
  const version = parseNodeVersion(value);
  if (!version) return false;
  if (version.major > 22) return true;
  if (version.major === 22) return version.minor > 12 || (version.minor === 12 && version.patch >= 0);
  if (version.major === 20) return version.minor > 19 || (version.minor === 19 && version.patch >= 0);
  return false;
}

async function exists(targetPath) {
  try {
    await fs.access(targetPath);
    return true;
  } catch {
    return false;
  }
}

async function readJson(targetPath) {
  try {
    return JSON.parse(await fs.readFile(targetPath, 'utf8'));
  } catch {
    return null;
  }
}

async function hasReferenceDependencies(referencePath, electronPath) {
  const candidates = [
    path.join(referencePath, 'node_modules/electron-vite'),
    path.join(electronPath, 'node_modules/electron-vite'),
    path.join(referencePath, 'node_modules/@nimbalyst/runtime'),
    path.join(referencePath, 'node_modules/react'),
    path.join(referencePath, 'node_modules/electron'),
    path.join(electronPath, 'node_modules/electron'),
  ];
  const checks = await Promise.all(candidates.map((candidate) => exists(candidate)));
  return {
    installed: checks.some(Boolean) && (checks[0] || checks[1]) && (checks[4] || checks[5]),
    checks: {
      electronVite: checks[0] || checks[1],
      runtime: checks[2],
      react: checks[3],
      electron: checks[4] || checks[5],
    },
  };
}

export async function getNimbalystReferenceStatus() {
  const referencePath = process.env.NIMBALYST_REFERENCE_PATH
    ? path.resolve(process.env.NIMBALYST_REFERENCE_PATH)
    : DEFAULT_REFERENCE_PATH;
  const electronPath = path.join(referencePath, 'packages/electron');

  const rootPackage = await readJson(path.join(referencePath, 'package.json'));
  const electronPackage = await readJson(path.join(electronPath, 'package.json'));
  const dependencyStatus = await hasReferenceDependencies(referencePath, electronPath);

  return {
    referencePath,
    electronPath,
    exists: await exists(referencePath),
    packageJson: rootPackage
      ? {
          name: rootPackage.name,
          version: rootPackage.version,
          workspaces: rootPackage.workspaces || [],
        }
      : null,
    electronPackageJson: electronPackage
      ? {
          name: electronPackage.name,
          version: electronPackage.version,
          main: electronPackage.main,
        }
      : null,
    dependenciesInstalled: dependencyStatus.installed,
    dependencyChecks: dependencyStatus.checks,
    packageLockExists: await exists(path.join(referencePath, 'package-lock.json')),
    node: {
      version: process.version,
      supported: isNodeVersionSupported(process.version),
      required: '^20.19.0 || >=22.12.0',
    },
    install: {
      running: Boolean(installProcess && !installProcess.killed),
      startedAt: installStartedAt,
      finishedAt: installFinishedAt,
      exit: installExit,
      recentLog: installLog,
    },
    runtimeDistExists: await exists(path.join(referencePath, 'packages/runtime/dist')),
    electronOutExists: await exists(path.join(electronPath, 'out')),
    windowsDevScriptExists: await exists(path.join(electronPath, 'scripts/dev.ps1')),
    shellDevScriptExists: await exists(path.join(electronPath, 'scripts/dev.sh')),
    running: Boolean(launchedProcess && !launchedProcess.killed),
    pid: launchedProcess && !launchedProcess.killed ? launchedProcess.pid : null,
    launchedAt,
    recentLog,
    suggestedCommands: [
      `cd ${referencePath}`,
      'npm ci',
      'powershell -ExecutionPolicy Bypass -File packages/electron/scripts/dev.ps1',
    ],
  };
}

export async function installNimbalystReferenceDependencies() {
  const status = await getNimbalystReferenceStatus();
  if (!status.exists || !status.packageJson || !status.packageLockExists) {
    return {
      ok: false,
      status,
      error: 'Nimbalyst reference package.json/package-lock.json is missing.',
    };
  }

  if (installProcess && !installProcess.killed) {
    return { ok: true, alreadyRunning: true, status };
  }

  installLog = [];
  installExit = null;
  installStartedAt = new Date().toISOString();
  installFinishedAt = null;

  const command = process.platform === 'win32' ? 'npm.cmd' : 'npm';
  installProcess = spawn(command, ['ci'], {
    cwd: status.referencePath,
    env: process.env,
    windowsHide: true,
    shell: false,
    stdio: ['ignore', 'pipe', 'pipe'],
  });

  installProcess.stdout?.on('data', appendInstallLog);
  installProcess.stderr?.on('data', appendInstallLog);
  installProcess.on('exit', (code, signal) => {
    installExit = { code, signal };
    installFinishedAt = new Date().toISOString();
    appendInstallLog(`[nimbalyst-install] exited code=${code ?? ''} signal=${signal ?? ''}`);
    installProcess = null;
  });

  return {
    ok: true,
    pid: installProcess.pid,
    status: await getNimbalystReferenceStatus(),
  };
}

export async function launchNimbalystReference() {
  const status = await getNimbalystReferenceStatus();
  if (!status.exists || !status.packageJson || !status.electronPackageJson) {
    return {
      ok: false,
      status,
      error: 'Nimbalyst reference source is missing or incomplete.',
    };
  }

  if (!status.dependenciesInstalled) {
    return {
      ok: false,
      status,
    error: 'Nimbalyst dependencies are not installed. Run npm ci in the reference repository first.',
    };
  }

  if (!status.node.supported) {
    return {
      ok: false,
      status,
      error: `Nimbalyst requires Node ${status.node.required}; current runtime is ${status.node.version}.`,
    };
  }

  if (launchedProcess && !launchedProcess.killed) {
    return { ok: true, status, alreadyRunning: true };
  }

  const command = process.platform === 'win32' ? 'powershell.exe' : 'bash';
  const args = process.platform === 'win32'
    ? ['-NoProfile', '-ExecutionPolicy', 'Bypass', '-File', path.join(status.electronPath, 'scripts/dev.ps1')]
    : [path.join(status.electronPath, 'scripts/dev.sh')];

  recentLog = [];
  launchedAt = new Date().toISOString();
  launchedProcess = spawn(command, args, {
    cwd: status.electronPath,
    env: {
      ...process.env,
      NIMBALYST_USER_DATA_DIR:
        process.env.NIMBALYST_USER_DATA_DIR ||
        path.join(process.cwd(), '.nimbalyst-reference-user-data'),
    },
    windowsHide: false,
    shell: false,
    stdio: ['ignore', 'pipe', 'pipe'],
  });

  launchedProcess.stdout?.on('data', appendLog);
  launchedProcess.stderr?.on('data', appendLog);
  launchedProcess.on('exit', (code, signal) => {
    appendLog(`[nimbalyst-reference] exited code=${code ?? ''} signal=${signal ?? ''}`);
    launchedProcess = null;
  });

  return {
    ok: true,
    status: await getNimbalystReferenceStatus(),
    pid: launchedProcess.pid,
  };
}

export async function stopNimbalystReference() {
  if (!launchedProcess || launchedProcess.killed) {
    return { ok: true, stopped: false, status: await getNimbalystReferenceStatus() };
  }

  if (process.platform === 'win32' && launchedProcess.pid) {
    spawn('taskkill', ['/PID', String(launchedProcess.pid), '/T', '/F'], {
      stdio: 'ignore',
      windowsHide: true,
    });
  } else {
    launchedProcess.kill('SIGTERM');
  }

  launchedProcess = null;
  return { ok: true, stopped: true, status: await getNimbalystReferenceStatus() };
}

export function getDefaultNimbalystReferencePath() {
  return DEFAULT_REFERENCE_PATH;
}
