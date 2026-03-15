import { spawn, spawnSync } from "node:child_process";
import fs from "node:fs";
import os from "node:os";
import path from "node:path";

function fail(message, code = 1) {
  process.stderr.write(`[bat_run] ${message}\n`);
  process.exit(code);
}

function parseProvider(providerRaw) {
  const provider = String(providerRaw ?? "codex").trim().toLowerCase();
  if (provider === "codex") return "codex";
  if (provider === "claude" || provider === "claude-code") return "claude-code";
  throw new Error(`Unsupported provider: ${providerRaw}. Use codex or claude-code.`);
}

export function normalizeProvider(providerRaw) {
  try {
    return parseProvider(providerRaw);
  } catch (err) {
    fail(String(err?.message ?? err));
  }
}

function tryResolveNodeShim(command, args) {
  if (process.platform !== "win32") return null;
  const commandText = String(command);
  const ext = path.extname(commandText).toLowerCase();
  if ((ext !== ".cmd" && ext !== ".bat") || !fs.existsSync(commandText)) {
    return null;
  }

  try {
    const shimText = fs.readFileSync(commandText, "utf8");
    const match = shimText.match(/"%dp0%\\([^\"]+\.(?:[cm]?js))"\s+%\*/i);
    if (!match) return null;

    const scriptPath = path.join(
      path.dirname(commandText),
      match[1].replace(/\\/g, path.sep)
    );
    if (!fs.existsSync(scriptPath)) return null;

    return {
      command: process.execPath,
      args: [scriptPath, ...args],
      windowsVerbatimArguments: false,
    };
  } catch {
    return null;
  }
}

function quoteCmdArg(arg) {
  const value = String(arg);
  if (value.length === 0) return '""';
  const escaped = value.replace(/(["%])/g, '"$1"');
  return /[\s&|<>()^%!"]/u.test(value) ? `"${escaped}"` : escaped;
}

export function prepareSpawn(command, args) {
  const shim = tryResolveNodeShim(command, args);
  if (shim) return shim;

  if (process.platform === "win32") {
    const ext = path.extname(String(command)).toLowerCase();
    if (ext === ".cmd" || ext === ".bat") {
      const commandLine = [`"${String(command)}"`, ...args.map(quoteCmdArg)].join(" ");
      return {
        command: process.env.ComSpec || "cmd.exe",
        args: ["/d", "/s", "/c", `"${commandLine}"`],
        windowsVerbatimArguments: true,
      };
    }
  }

  return { command, args, windowsVerbatimArguments: false };
}

export function spawnInherit(command, args, opts = {}) {
  const prepared = prepareSpawn(command, args);
  const result = spawnSync(prepared.command, prepared.args, {
    stdio: "inherit",
    cwd: opts.cwd ?? process.cwd(),
    env: process.env,
    shell: false,
    windowsVerbatimArguments: prepared.windowsVerbatimArguments,
  });

  if (result.error) {
    throw result.error;
  }

  const status = Number.isInteger(result.status) ? result.status : 1;
  if (opts.exitOnComplete !== false) {
    process.exit(status);
  }

  return { status };
}

export function spawnCapture(command, args, opts = {}) {
  const prepared = prepareSpawn(command, args);
  const result = spawnSync(prepared.command, prepared.args, {
    encoding: "utf8",
    stdio: ["ignore", "pipe", "pipe"],
    cwd: opts.cwd ?? process.cwd(),
    env: process.env,
    shell: false,
    windowsVerbatimArguments: prepared.windowsVerbatimArguments,
  });

  return {
    status: Number.isInteger(result.status) ? result.status : 1,
    stdout: String(result.stdout ?? ""),
    stderr: String(result.stderr ?? ""),
    error: result.error ?? null,
  };
}

function drainStreamLines(buffer, onLine, flushRemainder = false) {
  let remaining = buffer;
  while (true) {
    const newlineIndex = remaining.indexOf("\n");
    if (newlineIndex === -1) break;
    const line = remaining.slice(0, newlineIndex).replace(/\r$/, "");
    if (typeof onLine === "function") {
      onLine(line);
    }
    remaining = remaining.slice(newlineIndex + 1);
  }

  if (flushRemainder && remaining.length > 0) {
    const line = remaining.replace(/\r$/, "");
    if (typeof onLine === "function") {
      onLine(line);
    }
    return "";
  }

  return remaining;
}

export async function spawnCaptureAsync(command, args, opts = {}) {
  const prepared = prepareSpawn(command, args);
  return await new Promise((resolve) => {
    let stdout = "";
    let stderr = "";
    let stdoutBuffer = "";
    let stderrBuffer = "";
    let settled = false;

    const child = spawn(prepared.command, prepared.args, {
      stdio: ["ignore", "pipe", "pipe"],
      cwd: opts.cwd ?? process.cwd(),
      env: process.env,
      shell: false,
      windowsVerbatimArguments: prepared.windowsVerbatimArguments,
    });

    const heartbeatStartedAt = Date.now();
    const heartbeatTimer =
      typeof opts.onHeartbeat === "function"
        ? setInterval(() => {
            opts.onHeartbeat(Date.now() - heartbeatStartedAt);
          }, opts.heartbeatIntervalMs ?? 4000)
        : null;

    const finish = (payload) => {
      if (settled) return;
      settled = true;
      if (heartbeatTimer) {
        clearInterval(heartbeatTimer);
      }
      resolve(payload);
    };

    if (child.stdout) {
      child.stdout.setEncoding("utf8");
      child.stdout.on("data", (chunk) => {
        stdout += chunk;
        stdoutBuffer += chunk;
        stdoutBuffer = drainStreamLines(stdoutBuffer, opts.onStdoutLine);
      });
    }

    if (child.stderr) {
      child.stderr.setEncoding("utf8");
      child.stderr.on("data", (chunk) => {
        stderr += chunk;
        stderrBuffer += chunk;
        stderrBuffer = drainStreamLines(stderrBuffer, opts.onStderrLine);
      });
    }

    child.on("error", (error) => {
      finish({ status: 1, stdout, stderr, error });
    });

    child.on("close", (code) => {
      stdoutBuffer = drainStreamLines(stdoutBuffer, opts.onStdoutLine, true);
      stderrBuffer = drainStreamLines(stderrBuffer, opts.onStderrLine, true);
      finish({ status: code ?? 1, stdout, stderr, error: null });
    });
  });
}

function firstExistingPath(paths) {
  for (const candidate of paths) {
    if (!candidate) continue;
    if (fs.existsSync(candidate)) return candidate;
  }
  return null;
}

function findCodexFromVscodeExtensions() {
  const extRoot = path.join(os.homedir(), ".vscode", "extensions");
  if (!fs.existsSync(extRoot)) return null;

  let candidates = [];
  try {
    candidates = fs
      .readdirSync(extRoot, { withFileTypes: true })
      .filter((entry) => entry.isDirectory() && entry.name.startsWith("openai.chatgpt-"))
      .map((entry) => path.join(extRoot, entry.name, "bin", "windows-x86_64", "codex.exe"));
  } catch {
    return null;
  }

  const existing = candidates.filter((candidate) => fs.existsSync(candidate));
  if (existing.length === 0) return null;
  existing.sort().reverse();
  return existing[0];
}

export function resolveToolBinary(tool) {
  if (tool === "codex") {
    const envPath = process.env.BAT_RUN_CODEX_BIN;
    const fallbackPaths = [
      envPath,
      path.join(process.env.APPDATA ?? "", "npm", "codex.cmd"),
      path.join(process.env.APPDATA ?? "", "npm", "codex"),
      path.join(process.env.LOCALAPPDATA ?? "", "Programs", "codex", "codex.exe"),
      findCodexFromVscodeExtensions(),
    ];
    const direct = firstExistingPath(fallbackPaths);
    if (direct) return direct;

    const where = spawnCapture("where.exe", ["codex"], { cwd: process.cwd() });
    if (where.status === 0) {
      const hit = where.stdout
        .split(/\r?\n/g)
        .map((value) => value.trim())
        .find((value) => value.length > 0 && fs.existsSync(value));
      if (hit) return hit;
    }

    fail("codex executable not found. Set BAT_RUN_CODEX_BIN or add codex to PATH.");
  }

  if (tool === "claude") {
    const envPath = process.env.BAT_RUN_CLAUDE_BIN;
    const fallbackPaths = [
      envPath,
      path.join(process.env.APPDATA ?? "", "npm", "claude.cmd"),
      path.join(process.env.APPDATA ?? "", "npm", "claude"),
    ];
    const direct = firstExistingPath(fallbackPaths);
    if (direct) return direct;

    const where = spawnCapture("where.exe", ["claude"], { cwd: process.cwd() });
    if (where.status === 0) {
      const hit = where.stdout
        .split(/\r?\n/g)
        .map((value) => value.trim())
        .find((value) => value.length > 0 && fs.existsSync(value));
      if (hit) return hit;
    }

    fail("claude executable not found. Set BAT_RUN_CLAUDE_BIN or add claude to PATH.");
  }

  return tool;
}

export function runAiCommand(provider, prompt, images, model, opts = {}) {
  if (provider === "codex") {
    const codexBin = resolveToolBinary("codex");
    const args = ["exec", "-C", process.cwd(), "--skip-git-repo-check"];
    if (model) {
      args.push("--model", model);
    }
    if (prompt) {
      args.push(prompt);
    }
    for (const image of images) {
      args.push("-i", path.resolve(process.cwd(), image));
    }
    return spawnInherit(codexBin, args, {
      cwd: process.cwd(),
      exitOnComplete: opts.exitOnComplete,
    });
  }

  if (images.length > 0) {
    fail("claude-code provider currently does not support --image in this wrapper. Use codex provider.");
  }

  const args = ["--print"];
  if (model) {
    args.push("--model", model);
  }
  args.push(prompt);
  const claudeBin = resolveToolBinary("claude");
  return spawnInherit(claudeBin, args, {
    cwd: process.cwd(),
    exitOnComplete: opts.exitOnComplete,
  });
}
