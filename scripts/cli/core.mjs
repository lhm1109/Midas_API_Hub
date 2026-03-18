
import { spawn, spawnSync } from "node:child_process";
import fs from "node:fs";
import os from "node:os";
import path from "node:path";
import readline from "node:readline";
import { fileURLToPath, pathToFileURL } from "node:url";
import {
  normalizeProvider,
  resolveToolBinary,
  runAiCommand,
  spawnCapture,
  spawnCaptureAsync,
  spawnInherit,
} from "./providers/runner.mjs";
import { colorize, createProgressReporter, formatElapsedTime } from "./output/progress.mjs";

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);
const PROJECT_ROOT = path.resolve(__dirname, "..", "..");
const GENERATED_SCHEMAS_ROOT = path.join(PROJECT_ROOT, "generated_schemas");
const BAT_RUN_SETTINGS_DIR = process.env.APPDATA
  ? path.join(process.env.APPDATA, "bat_run")
  : path.join(os.homedir(), ".bat_run");
const BAT_RUN_SETTINGS_PATH = path.join(BAT_RUN_SETTINGS_DIR, "settings.json");
const BAT_RUN_LOGS_DIR = path.join(BAT_RUN_SETTINGS_DIR, "logs");

const SEARCH_PATHS = [
  "ui_src",
  "src",
  "server",
  "mcp-server/src",
  "schema_definitions",
  "generated_schemas",
];
const STARTUP_BANNER_LINES = [
  " ____    _  _____    ____  _   _ _   _",
  "| __ )  / \\|_   _|  |  _ \\| | | | \\ | |",
  "|  _ \\ / _ \\ | |    | |_) | | | |  \\| |",
  "| |_) / ___ \\| |    |  _ <| |_| | |\\  |",
  "|____/_/   \\_\\_|    |_| \\_\\___/|_| \\_|",
];
const SPINNER_FRAMES = ["-", "\\", "|", "/"];
const ANSI_ENABLED = Boolean(process.stdout.isTTY) && process.env.NO_COLOR !== "1";
const ANSI = {
  reset: "\u001b[0m",
  dim: "\u001b[2m",
  bold: "\u001b[1m",
  gray: "\u001b[90m",
  red: "\u001b[31m",
  green: "\u001b[32m",
  yellow: "\u001b[33m",
  blue: "\u001b[34m",
  magenta: "\u001b[35m",
  cyan: "\u001b[36m",
};
const SCOPE_COLOR_BY_NAME = {
  chat: ANSI.cyan,
  schema: ANSI.magenta,
  system: ANSI.blue,
};
const PROGRESS_COLOR_BY_KIND = {
  info: ANSI.cyan,
  wait: ANSI.blue,
  success: ANSI.green,
  warn: ANSI.yellow,
  error: ANSI.red,
  detail: ANSI.gray,
};
const SHELL_DEFAULT_PROMPT = "bat_run> ";
const SHELL_COMPOSE_PROMPT = "... ";
const SHELL_PASTE_BURST_DEBOUNCE_MS = 90;
let ACTIVE_SESSION_LOG_FILE = null;
let OUTPUT_HOOKS_INSTALLED = false;
const ORIGINAL_STDOUT_WRITE = process.stdout.write.bind(process.stdout);
const ORIGINAL_STDERR_WRITE = process.stderr.write.bind(process.stderr);
const SHELL_COMMAND_PALETTE_ITEMS = [
  { command: "/help", description: "Show shell help", action: "execute", value: "/help" },
  { command: "/settings", description: "Show current settings", action: "execute", value: "/settings" },
  { command: "/log-file", description: "Show current session log file", action: "execute", value: "/log-file" },
  { command: "/tail-log 40", description: "Show recent session log lines", action: "execute", value: "/tail-log 40" },
  { command: "/paste", description: "Attach clipboard image", action: "execute", value: "/paste" },
  { command: "/compose", description: "Open multiline compose mode", action: "execute", value: "/compose" },
  { command: "/images", description: "Show pending images", action: "execute", value: "/images" },
  { command: "/new", description: "Start a new conversation", action: "execute", value: "/new" },
  { command: "/clear", description: "Clear pending images", action: "execute", value: "/clear" },
  { command: "/mode schema", description: "Switch to developer schema mode", action: "execute", value: "/mode schema" },
  { command: "/mode chat", description: "Switch to normal chat mode", action: "execute", value: "/mode chat" },
  { command: "/provider codex", description: "Use Codex provider", action: "execute", value: "/provider codex" },
  { command: "/provider claude-code", description: "Use Claude Code provider", action: "execute", value: "/provider claude-code" },
  { command: "/verbose on", description: "Turn verbose event logs on", action: "execute", value: "/verbose on" },
  { command: "/verbose off", description: "Turn verbose event logs off", action: "execute", value: "/verbose off" },
  { command: "/command-output summary", description: "Show summarized command and MCP output", action: "execute", value: "/command-output summary" },
  { command: "/command-output full", description: "Expand command and MCP output", action: "execute", value: "/command-output full" },
  { command: "/schema ", description: "Insert schema create command", action: "insert", value: "/schema " },
  { command: "/send ", description: "Insert normal prompt command", action: "insert", value: "/send " },
  { command: "/output-dir ", description: "Insert schema output dir command", action: "insert", value: "/output-dir " },
  { command: "/dialog-class ", description: "Insert dialog class command", action: "insert", value: "/dialog-class " },
  { command: "/code-search add ", description: "Insert code search command", action: "insert", value: "/code-search add " },
  { command: "/schema-name ", description: "Insert fixed schema name command", action: "insert", value: "/schema-name " },
  { command: "/model ", description: "Insert model override command", action: "insert", value: "/model " },
];
function stripAnsi(text) {
  return String(text ?? "").replace(/\u001b\[[0-9;]*m/g, "");
}

function ensureBatRunLogsDir() {
  fs.mkdirSync(BAT_RUN_LOGS_DIR, { recursive: true });
  return BAT_RUN_LOGS_DIR;
}

function formatLogTimestamp(date = new Date()) {
  const yyyy = String(date.getFullYear());
  const mm = String(date.getMonth() + 1).padStart(2, "0");
  const dd = String(date.getDate()).padStart(2, "0");
  const hh = String(date.getHours()).padStart(2, "0");
  const mi = String(date.getMinutes()).padStart(2, "0");
  const ss = String(date.getSeconds()).padStart(2, "0");
  return `${yyyy}${mm}${dd}_${hh}${mi}${ss}`;
}

function coerceOutputChunk(chunk, encoding) {
  if (typeof chunk === "string") {
    return chunk;
  }
  if (Buffer.isBuffer(chunk)) {
    return chunk.toString(typeof encoding === "string" ? encoding : "utf8");
  }
  return String(chunk ?? "");
}

function appendActiveSessionLog(chunk, encoding) {
  if (!ACTIVE_SESSION_LOG_FILE) {
    return;
  }

  const text = stripAnsi(coerceOutputChunk(chunk, encoding));
  if (!text || text === "bat_run> ") {
    return;
  }

  try {
    fs.appendFileSync(ACTIVE_SESSION_LOG_FILE, text, "utf8");
  } catch {
    // Ignore log write failures; console output remains source of truth.
  }
}

function appendActiveSessionLogLine(text) {
  if (!ACTIVE_SESSION_LOG_FILE) {
    return;
  }
  appendActiveSessionLog(`${text}\n`, "utf8");
}

function setActiveSessionLogFile(filePath) {
  ACTIVE_SESSION_LOG_FILE = filePath || null;
}

function createShellSessionLogFile() {
  const logsDir = ensureBatRunLogsDir();
  const filePath = path.join(logsDir, `session_${formatLogTimestamp()}.log`);
  fs.writeFileSync(filePath, `[bat_run] session log created ${new Date().toISOString()}\n`, "utf8");
  return filePath;
}

function installOutputHooks() {
  if (OUTPUT_HOOKS_INSTALLED) {
    return;
  }

  const stdoutWrite = process.stdout.write.bind(process.stdout);
  const stderrWrite = process.stderr.write.bind(process.stderr);

  process.stdout.write = function patchedStdoutWrite(chunk, encoding, callback) {
    appendActiveSessionLog(chunk, encoding);
    return stdoutWrite(chunk, encoding, callback);
  };

  process.stderr.write = function patchedStderrWrite(chunk, encoding, callback) {
    appendActiveSessionLog(chunk, encoding);
    return stderrWrite(chunk, encoding, callback);
  };

  OUTPUT_HOOKS_INSTALLED = true;
}

function printRecentLogLines(filePath, count = 40) {
  if (!filePath || !fs.existsSync(filePath)) {
    ORIGINAL_STDOUT_WRITE("[bat_run] no active session log file\n");
    return;
  }

  const raw = fs.readFileSync(filePath, "utf8");
  const lines = raw.split(/\r?\n/g).filter((line) => line.length > 0);
  const safeCount = Math.max(1, Math.min(200, Number.isFinite(count) ? Math.floor(count) : 40));
  const recent = lines.slice(-safeCount);
  ORIGINAL_STDOUT_WRITE(`[bat_run] recent log lines (${recent.length}/${lines.length}): ${filePath}\n`);
  for (const line of recent) {
    ORIGINAL_STDOUT_WRITE(`${line}\n`);
  }
}

installOutputHooks();
const MCP_SERVER_ROOT = path.join(PROJECT_ROOT, "mcp-server");
const MCP_SERVER_DIST_ENTRY = path.join(MCP_SERVER_ROOT, "dist", "index.js");
const MCP_SERVER_SRC_ENTRY = path.join(MCP_SERVER_ROOT, "src", "index.ts");
const MCP_SERVER_TSX_BIN = path.join(
  MCP_SERVER_ROOT,
  "node_modules",
  ".bin",
  process.platform === "win32" ? "tsx.cmd" : "tsx"
);
let MCP_SDK_CLIENT_CACHE = null;

function readTextFile(filePath, label = "file") {
  const absolutePath = path.resolve(process.cwd(), filePath);
  if (!fs.existsSync(absolutePath)) {
    throw new Error(`${label} not found: ${absolutePath}`);
  }
  return fs.readFileSync(absolutePath, "utf8");
}

function parseJsonText(text, label = "JSON") {
  try {
    return JSON.parse(text);
  } catch (error) {
    throw new Error(`${label} parse failed: ${String(error?.message ?? error)}`);
  }
}

function maybeParseJsonText(text, contentType = "") {
  const raw = String(text ?? "").trim();
  if (!raw) {
    return null;
  }

  const shouldTryJson =
    /json/i.test(String(contentType ?? "")) || raw.startsWith("{") || raw.startsWith("[");
  if (!shouldTryJson) {
    return null;
  }

  try {
    return JSON.parse(raw);
  } catch {
    return null;
  }
}

function readJsonInputOption(opts, inlineKey, fileKey, defaultValue = {}) {
  const hasInline = Object.prototype.hasOwnProperty.call(opts, inlineKey);
  const hasFile = Object.prototype.hasOwnProperty.call(opts, fileKey);
  if (hasInline && hasFile) {
    throw new Error(`Use only one of --${inlineKey} or --${fileKey}.`);
  }
  if (hasInline) {
    return parseJsonText(String(opts[inlineKey]), `--${inlineKey}`);
  }
  if (hasFile) {
    return parseJsonText(readTextFile(String(opts[fileKey]), `--${fileKey}`), `--${fileKey}`);
  }
  return defaultValue;
}

function parseEqualsEntries(values, label = "entry") {
  const out = {};
  for (const rawValue of asArray(values)) {
    const text = String(rawValue ?? "").trim();
    if (!text) continue;
    const idx = text.indexOf("=");
    if (idx <= 0) {
      throw new Error(`Invalid ${label}: ${text}. Use key=value.`);
    }
    const key = text.slice(0, idx).trim();
    const value = text.slice(idx + 1).trim();
    out[key] = value;
  }
  return out;
}

function parseHeaderEntries(values) {
  const out = {};
  for (const rawValue of asArray(values)) {
    const text = String(rawValue ?? "").trim();
    if (!text) continue;
    const idx = text.indexOf(":");
    if (idx <= 0) {
      throw new Error(`Invalid header: ${text}. Use \"Name: Value\".`);
    }
    const key = text.slice(0, idx).trim();
    const value = text.slice(idx + 1).trim();
    out[key] = value;
  }
  return out;
}

function mergeHeaderMaps(...maps) {
  return Object.assign({}, ...maps.filter((value) => value && typeof value === "object"));
}

function deepContainsSubset(actual, expected, pathLabel = "$", issues = []) {
  if (expected === null || typeof expected !== "object") {
    if (!Object.is(actual, expected)) {
      issues.push(`${pathLabel}: expected ${JSON.stringify(expected)}, got ${JSON.stringify(actual)}`);
    }
    return issues;
  }

  if (Array.isArray(expected)) {
    if (!Array.isArray(actual)) {
      issues.push(`${pathLabel}: expected array, got ${typeof actual}`);
      return issues;
    }
    expected.forEach((item, index) => {
      if (index >= actual.length) {
        issues.push(`${pathLabel}[${index}]: missing array item`);
        return;
      }
      deepContainsSubset(actual[index], item, `${pathLabel}[${index}]`, issues);
    });
    return issues;
  }

  if (!actual || typeof actual !== "object" || Array.isArray(actual)) {
    issues.push(`${pathLabel}: expected object, got ${typeof actual}`);
    return issues;
  }

  for (const [key, expectedValue] of Object.entries(expected)) {
    if (!Object.prototype.hasOwnProperty.call(actual, key)) {
      issues.push(`${pathLabel}.${key}: missing key`);
      continue;
    }
    deepContainsSubset(actual[key], expectedValue, `${pathLabel}.${key}`, issues);
  }
  return issues;
}

function buildRequestUrl(urlValue, baseUrl = null, query = {}) {
  if (!urlValue) {
    throw new Error("Request URL is required.");
  }

  let url;
  if (baseUrl) {
    url = new URL(String(urlValue), String(baseUrl));
  } else {
    url = new URL(String(urlValue));
  }

  for (const [key, value] of Object.entries(query)) {
    url.searchParams.set(key, String(value));
  }
  return url.toString();
}

function resolveMcpSdkImportPath(...segments) {
  return pathToFileURL(
    path.join(MCP_SERVER_ROOT, "node_modules", "@modelcontextprotocol", "sdk", "dist", "esm", ...segments)
  ).href;
}

async function loadMcpSdkClient() {
  if (!MCP_SDK_CLIENT_CACHE) {
    MCP_SDK_CLIENT_CACHE = Promise.all([
      import(resolveMcpSdkImportPath("client", "index.js")),
      import(resolveMcpSdkImportPath("client", "stdio.js")),
    ]).then(([clientModule, stdioModule]) => ({
      Client: clientModule.Client,
      StdioClientTransport: stdioModule.StdioClientTransport,
    }));
  }
  return await MCP_SDK_CLIENT_CACHE;
}

function resolveMcpServerLaunchConfig() {
  if (fs.existsSync(MCP_SERVER_DIST_ENTRY)) {
    return {
      command: process.execPath,
      args: [MCP_SERVER_DIST_ENTRY],
      cwd: MCP_SERVER_ROOT,
      env: {
        ...process.env,
        APIVERIFICATION_ROOT: PROJECT_ROOT,
      },
    };
  }

  if (fs.existsSync(MCP_SERVER_SRC_ENTRY) && fs.existsSync(MCP_SERVER_TSX_BIN)) {
    return {
      command: MCP_SERVER_TSX_BIN,
      args: [MCP_SERVER_SRC_ENTRY],
      cwd: MCP_SERVER_ROOT,
      env: {
        ...process.env,
        APIVERIFICATION_ROOT: PROJECT_ROOT,
      },
    };
  }

  throw new Error("MCP server entry not found. Build mcp-server first.");
}

async function withMcpClient(handler, progress = null) {
  const { Client, StdioClientTransport } = await loadMcpSdkClient();
  const transport = new StdioClientTransport({
    ...resolveMcpServerLaunchConfig(),
    stderr: "pipe",
  });

  if (transport.stderr) {
    transport.stderr.setEncoding("utf8");
    transport.stderr.on("data", (chunk) => {
      const text = String(chunk ?? "");
      for (const line of text.split(/\r?\n/g)) {
        const trimmed = line.trim();
        if (!trimmed) continue;
        if (progress) progress.log(`server: ${trimmed}`, "detail");
        else process.stderr.write(`[mcp-server] ${trimmed}\n`);
      }
    });
  }

  const client = new Client({ name: "bat_run", version: "1.0.0" }, { capabilities: {} });
  await client.connect(transport);
  try {
    return await handler(client);
  } finally {
    await transport.close();
  }
}

function normalizeMcpResult(result) {
  if (!result || typeof result !== "object") {
    return result;
  }

  if (Array.isArray(result.content) && result.content.length === 1 && result.content[0]?.type === "text") {
    const text = String(result.content[0].text ?? "").trim();
    return maybeParseJsonText(text, "application/json") ?? text;
  }

  return result;
}

function normalizeApiOutputMode(value) {
  const normalized = String(value ?? "pretty").trim().toLowerCase();
  if (["pretty", "json", "text"].includes(normalized)) {
    return normalized;
  }
  throw new Error(`Invalid output mode: ${value}. Use pretty, json, or text.`);
}

function buildApiRequestFromOptions(opts) {
  const method = String(opts.method ?? "GET").trim().toUpperCase();
  const headers = parseHeaderEntries(opts.header);
  const query = parseEqualsEntries(opts.query, "query");
  const timeoutMs = Number(opts.timeout ?? 30000);
  if (!Number.isFinite(timeoutMs) || timeoutMs <= 0) {
    throw new Error("--timeout must be a positive number in milliseconds.");
  }

  let body = null;
  let bodyKind = null;
  if (Object.prototype.hasOwnProperty.call(opts, "json-body") || Object.prototype.hasOwnProperty.call(opts, "json-body-file")) {
    const jsonBody = readJsonInputOption(opts, "json-body", "json-body-file", null);
    body = jsonBody === null ? null : JSON.stringify(jsonBody);
    bodyKind = "json";
    if (!Object.keys(headers).some((key) => key.toLowerCase() === "content-type")) {
      headers["Content-Type"] = "application/json";
    }
  } else if (Object.prototype.hasOwnProperty.call(opts, "body") || Object.prototype.hasOwnProperty.call(opts, "body-file")) {
    if (Object.prototype.hasOwnProperty.call(opts, "body") && Object.prototype.hasOwnProperty.call(opts, "body-file")) {
      throw new Error("Use only one of --body or --body-file.");
    }
    body = Object.prototype.hasOwnProperty.call(opts, "body")
      ? String(opts.body)
      : readTextFile(String(opts["body-file"]), "body file");
    bodyKind = "text";
  }

  return {
    method,
    url: String(opts.url ?? "").trim(),
    baseUrl: typeof opts["base-url"] === "string" ? String(opts["base-url"]).trim() : null,
    headers,
    query,
    timeoutMs,
    body,
    bodyKind,
  };
}

async function performApiRequest(request, progress = null) {
  const finalUrl = buildRequestUrl(request.url, request.baseUrl, request.query);
  const reporter = progress ?? createProgressReporter("api");
  reporter.log(`requesting ${request.method} ${finalUrl}`);

  const controller = new AbortController();
  const timeoutHandle = setTimeout(() => controller.abort(new Error("Request timeout")), request.timeoutMs);
  const startedAt = Date.now();

  try {
    const response = await fetch(finalUrl, {
      method: request.method,
      headers: request.headers,
      body: request.body,
      signal: controller.signal,
    });
    const elapsedMs = Date.now() - startedAt;
    const bodyText = await response.text();
    const contentType = response.headers.get("content-type") ?? "";
    const bodyJson = maybeParseJsonText(bodyText, contentType);
    reporter.log(`response ${response.status} ${response.statusText || ""} (${elapsedMs} ms)`, response.ok ? "success" : "warn");

    return {
      ok: response.ok,
      method: request.method,
      url: finalUrl,
      status: response.status,
      statusText: response.statusText,
      elapsedMs,
      headers: Object.fromEntries(response.headers.entries()),
      bodyText,
      bodyJson,
    };
  } finally {
    clearTimeout(timeoutHandle);
  }
}

function printApiResult(result, outputMode = "pretty") {
  const mode = normalizeApiOutputMode(outputMode);
  const payload = {
    ok: result.ok,
    method: result.method,
    url: result.url,
    status: result.status,
    statusText: result.statusText,
    elapsedMs: result.elapsedMs,
    headers: result.headers,
    body: result.bodyJson ?? result.bodyText,
  };

  if (mode === "json") {
    process.stdout.write(`${JSON.stringify(payload, null, 2)}\n`);
    return;
  }

  if (mode === "text") {
    process.stdout.write(`${result.bodyText}\n`);
    return;
  }

  process.stdout.write(`[bat_run] ${result.method} ${result.url}\n`);
  process.stdout.write(`[bat_run] status: ${result.status} ${result.statusText} (${result.elapsedMs} ms)\n`);
  if (result.bodyJson !== null) {
    process.stdout.write(`${JSON.stringify(result.bodyJson, null, 2)}\n`);
  } else if (result.bodyText.trim()) {
    process.stdout.write(`${result.bodyText}\n`);
  }
}

function evaluateApiExpectations(result, expect = {}) {
  const issues = [];

  if (Object.prototype.hasOwnProperty.call(expect, "status")) {
    const expectedStatuses = Array.isArray(expect.status) ? expect.status : [expect.status];
    if (!expectedStatuses.includes(result.status)) {
      issues.push(`status: expected ${expectedStatuses.join(", ")}, got ${result.status}`);
    }
  }

  if (Object.prototype.hasOwnProperty.call(expect, "ok") && Boolean(expect.ok) !== result.ok) {
    issues.push(`ok: expected ${Boolean(expect.ok)}, got ${result.ok}`);
  }

  if (Object.prototype.hasOwnProperty.call(expect, "textIncludes")) {
    for (const token of asArray(expect.textIncludes)) {
      if (!result.bodyText.includes(String(token))) {
        issues.push(`textIncludes: missing \"${token}\"`);
      }
    }
  }

  if (Object.prototype.hasOwnProperty.call(expect, "jsonContains")) {
    if (result.bodyJson === null) {
      issues.push("jsonContains: response body is not JSON");
    } else {
      deepContainsSubset(result.bodyJson, expect.jsonContains, "$", issues);
    }
  }

  if (Object.prototype.hasOwnProperty.call(expect, "headers")) {
    const actualHeaders = Object.fromEntries(
      Object.entries(result.headers).map(([key, value]) => [key.toLowerCase(), String(value)])
    );
    for (const [key, expectedValue] of Object.entries(expect.headers ?? {})) {
      const actualValue = actualHeaders[String(key).toLowerCase()];
      if (actualValue === undefined) {
        issues.push(`headers.${key}: missing header`);
      } else if (!actualValue.includes(String(expectedValue))) {
        issues.push(`headers.${key}: expected to include ${JSON.stringify(expectedValue)}, got ${JSON.stringify(actualValue)}`);
      }
    }
  }

  return issues;
}

function saveApiResponseIfRequested(result, filePath) {
  if (!filePath) {
    return;
  }
  const absolutePath = path.resolve(process.cwd(), String(filePath));
  fs.mkdirSync(path.dirname(absolutePath), { recursive: true });
  fs.writeFileSync(absolutePath, result.bodyJson !== null ? `${JSON.stringify(result.bodyJson, null, 2)}\n` : result.bodyText, "utf8");
  process.stdout.write(`[bat_run] saved response body: ${absolutePath}\n`);
}

function normalizeApiTestSuite(rawSuite) {
  if (!rawSuite || typeof rawSuite !== "object") {
    throw new Error("API test file must be a JSON object.");
  }

  const requests = Array.isArray(rawSuite.requests)
    ? rawSuite.requests
    : Array.isArray(rawSuite.tests)
      ? rawSuite.tests
      : null;
  if (!requests) {
    throw new Error("API test file must include a requests or tests array.");
  }

  return {
    baseUrl: rawSuite.baseUrl ?? null,
    headers: rawSuite.headers && typeof rawSuite.headers === "object" ? rawSuite.headers : {},
    timeoutMs: Number(rawSuite.timeoutMs ?? 30000),
    requests,
  };
}
function printHelp() {
  const text = `
bat_run - AI wrapper CLI for apiverification

Usage:
  bat_run
  bat_run shell [--provider codex|claude-code] [--mode schema|chat] [--model <name>]
                [--verbose <on|off>] [--command-output off|summary|full]
                [--output-dir <dir>] [--dialog-class <ClassName>] [--code-search "<query>"]
                [--schema-name <name>] [--image <path>] [--from-clipboard] [--prompt "<text>"]
  bat_run native [--provider codex|claude-code] [--image <path>] [--from-clipboard] [--prompt "<text>"]
  bat_run [--provider codex|claude-code] [--mode schema|chat] [--model <name>]
          [--verbose <on|off>] [--command-output off|summary|full]
          [--output-dir <dir>] [--dialog-class <ClassName>] [--code-search "<query>"]
          [--schema-name <name>] [--image <path>] [--from-clipboard] [--prompt "<text>"]
  bat_run help
  bat_run exec "<prompt>" [--provider codex|claude-code] [--image <path>] [--from-clipboard]
  bat_run paste --prompt "<text>" [--provider codex|claude-code] [--dry-run]
  bat_run schema create --prompt "<text>" [--provider codex|claude-code]
                        [--image <path>] [--from-clipboard]
                        [--code-search "<query>"] [--dialog-class <ClassName>]
                        [--schema-name <name>] [--output-dir <dir>] [--dry-run]
  bat_run mcp list [tools|resources|prompts|resource-templates] [--json]
  bat_run mcp call <tool> [--input-json "<json>" | --input-file <file>] [--json]
  bat_run mcp read-resource <uri> [--json]
  bat_run mcp get-prompt <name> [--arg key=value] [--json]
  bat_run api request --url <url> [--method GET|POST|PUT|PATCH|DELETE]
                      [--base-url <url>] [--query key=value] [--header "Name: Value"]
                      [--json-body "<json>" | --json-body-file <file> | --body <text> | --body-file <file>]
                      [--output pretty|json|text] [--save <file>] [--timeout <ms>]
  bat_run api test --file <suite.json> [--json] [--stop-on-fail]

Notes:
  - No args: enters bat_run shell mode (wrapper prompt).
  - shell mode prints a startup banner and starts in schema mode by default.
  - Press / on an empty prompt to open the command palette.
  - Command palette supports Arrow keys + Enter, and Esc closes it.
  - shell mode supports /settings /log-file /tail-log /mode /verbose /command-output /output-dir /dialog-class /code-search and keeps codex session with resume.
  - In shell mode, Ctrl+Shift+V is the official clipboard image shortcut.
  - Alt+V is kept as a fallback when Ctrl+Shift+V is intercepted by the terminal.
  - native mode starts raw provider interactive session.
  - Interactive native mode can pre-attach images with --image / --from-clipboard.
  - --from-clipboard saves clipboard image to a temp PNG file.
  - paste command is a shortcut for clipboard-image + prompt.
  - schema create defaults to generated_schemas/<schemaName>.json.
  - --output-dir is relative to generated_schemas (use "." for the root).
  - shell plain text uses the current mode; default is schema (developer mode).
  - schema create builds a prompt that asks AI to use MCP parse_dialog/save_schema.
  - mcp/api commands are direct-mode utilities and do not require the AI wrapper turn.
`;
  process.stdout.write(text.trimStart() + "\n");
}
function fail(message, code = 1) {
  process.stderr.write(`[bat_run] ${message}\n`);
  process.exit(code);
}

function normalizeBooleanLike(value, label = "value") {
  if (typeof value === "boolean") {
    return value;
  }
  const normalized = String(value ?? "").trim().toLowerCase();
  if (["1", "true", "on", "yes"].includes(normalized)) {
    return true;
  }
  if (["0", "false", "off", "no"].includes(normalized)) {
    return false;
  }
  throw new Error(`Invalid ${label}: ${value}. Use on or off.`);
}

function normalizeCommandOutputMode(value) {
  const normalized = String(value ?? "summary").trim().toLowerCase();
  if (["off", "summary", "full"].includes(normalized)) {
    return normalized;
  }
  throw new Error(`Invalid command output mode: ${value}. Use off, summary, or full.`);
}
function shortenForLog(value, maxLength = 160) {
  const normalized = String(value ?? "").replace(/\s+/g, " ").trim();
  if (normalized.length <= maxLength) {
    return normalized;
  }
  return `${normalized.slice(0, Math.max(0, maxLength - 3))}...`;
}

function summarizeOutputText(value, maxLength = 180) {
  const text = String(value ?? "").replace(/\r/g, "").trim();
  if (!text) {
    return "";
  }
  const lineCount = text.split(/\n/g).length;
  const preview = shortenForLog(text, maxLength);
  return lineCount > 1 ? `${preview} (${lineCount} lines)` : preview;
}

function extractTextPayload(value, seen = new Set()) {
  if (value === null || value === undefined) {
    return "";
  }

  if (typeof value === "string") {
    const text = value.trim();
    if (!text) {
      return "";
    }
    const parsed = parseJsonLine(text);
    if (parsed && typeof parsed === "object" && parsed !== null) {
      const nested = extractTextPayload(parsed, seen);
      if (nested) {
        return nested;
      }
    }
    return text;
  }

  if (typeof value === "number" || typeof value === "boolean") {
    return String(value);
  }

  if (seen.has(value)) {
    return "";
  }
  seen.add(value);

  if (Array.isArray(value)) {
    for (const item of value) {
      const nested = extractTextPayload(item, seen);
      if (nested) {
        return nested;
      }
    }
    return "";
  }

  if (typeof value === "object") {
    const priorityKeys = ["message", "text", "error", "summary", "path", "uri", "content", "structured_content", "result", "data"];
    for (const key of priorityKeys) {
      if (Object.prototype.hasOwnProperty.call(value, key)) {
        const nested = extractTextPayload(value[key], seen);
        if (nested) {
          return nested;
        }
      }
    }

    try {
      return JSON.stringify(value, null, 2);
    } catch {
      return "";
    }
  }

  return "";
}

function formatMcpResultSummary(result) {
  const extracted = extractTextPayload(result);
  return extracted ? shortenForLog(extracted, 200) : "";
}

function trimDetailBlock(text, maxChars = 5000, maxLines = 80) {
  const normalized = String(text ?? "").replace(/\r/g, "").trimEnd();
  if (!normalized) {
    return { text: "", truncated: false };
  }

  let truncated = false;
  let value = normalized;
  if (value.length > maxChars) {
    value = value.slice(0, maxChars);
    truncated = true;
  }

  const lines = value.split(/\n/g);
  if (lines.length > maxLines) {
    value = lines.slice(0, maxLines).join("\n");
    truncated = true;
  }

  return { text: value, truncated };
}

function printDetailBlock(title, text) {
  const block = trimDetailBlock(text);
  if (!block.text) {
    return;
  }
  const titleLine = colorize(`[bat_run] ${title}:`, ANSI.dim);
  const prefix = colorize("  | ", ANSI.dim);
  process.stdout.write(`${titleLine}\n`);
  for (const line of block.text.split(/\n/g)) {
    process.stdout.write(`${prefix}${line}\n`);
  }
  if (block.truncated) {
    process.stdout.write(`${prefix}... (truncated)\n`);
  }
}

function formatMcpArgumentSummary(argumentsValue) {
  if (!argumentsValue || typeof argumentsValue !== "object") {
    return "";
  }

  const entries = Object.entries(argumentsValue)
    .filter(([, value]) => value !== undefined)
    .slice(0, 3)
    .map(([key, value]) => {
      if (typeof value === "string") {
        return `${key}=${shortenForLog(value, 40)}`;
      }
      if (typeof value === "number" || typeof value === "boolean") {
        return `${key}=${value}`;
      }
      try {
        return `${key}=${shortenForLog(JSON.stringify(value), 40)}`;
      } catch {
        return `${key}=[complex]`;
      }
    });

  return entries.length > 0 ? ` (${entries.join(", ")})` : "";
}

function formatUsageSummary(usage) {
  if (!usage || typeof usage !== "object") {
    return "";
  }

  const parts = [];
  if (Number.isFinite(usage.input_tokens)) {
    parts.push(`input=${usage.input_tokens}`);
  }
  if (Number.isFinite(usage.cached_input_tokens) && usage.cached_input_tokens > 0) {
    parts.push(`cached=${usage.cached_input_tokens}`);
  }
  if (Number.isFinite(usage.output_tokens)) {
    parts.push(`output=${usage.output_tokens}`);
  }
  return parts.join(", ");
}

function handleCodexItemEvent(eventType, item, parsed, progress, ui = {}) {
  if (!item || typeof item !== "object") {
    return;
  }

  const started = eventType === "item.started";
  const verbose = Boolean(ui.verbose);
  const commandOutput = ui.commandOutput ?? "summary";

  if (item.type === "agent_message") {
    if (!started) {
      const text = String(item.text ?? "").trim();
      if (text) {
        parsed.messages.push(text);
        process.stdout.write(`assistant> ${text}\n`);
      }
    }
    return;
  }

  if (item.type === "command_execution") {
    const commandText = shortenForLog(item.command ?? "", 180);
    if (started) {
      progress.log(`command started: ${commandText}`);
    } else {
      const exitText = item.exit_code !== null && item.exit_code !== undefined ? ` (exit ${item.exit_code})` : "";
      progress.log(`command completed${exitText}: ${commandText}`, item.exit_code === 0 ? "success" : "warn");
      const outputSummary = summarizeOutputText(item.aggregated_output ?? "");
      if (outputSummary && commandOutput === "summary") {
        progress.log(`command output: ${outputSummary}`, "detail");
      } else if (commandOutput === "full") {
        printDetailBlock("command output", item.aggregated_output ?? "");
      }
    }
    return;
  }

  if (item.type === "mcp_tool_call") {
    const server = item.server || "mcp";
    const tool = item.tool || "tool";
    const argumentSummary = formatMcpArgumentSummary(item.arguments);
    if (started) {
      progress.log(`mcp started: ${server}.${tool}${argumentSummary}`);
    } else if (item.error) {
      progress.log(`mcp failed: ${server}.${tool}${argumentSummary}`, "warn");
      const errorText = extractTextPayload(item.error);
      if (errorText && commandOutput === "summary") {
        progress.log(`mcp error: ${shortenForLog(errorText, 200)}`, "detail");
      } else if (commandOutput === "full") {
        printDetailBlock(`mcp error ${server}.${tool}`, errorText);
      }
    } else {
      progress.log(`mcp completed: ${server}.${tool}${argumentSummary}`, "success");
      const resultSummary = formatMcpResultSummary(item.result);
      if (resultSummary && commandOutput === "summary") {
        progress.log(`mcp result: ${resultSummary}`, "detail");
      } else if (commandOutput === "full") {
        const resultText = extractTextPayload(item.result);
        printDetailBlock(`mcp result ${server}.${tool}`, resultText);
      }
    }
    return;
  }

  const message = `${started ? "item started" : "item completed"}: ${shortenForLog(item.type ?? "event", 80)}`;
  progress.log(message, verbose ? "detail" : "info");
}

function handleCodexJsonLine(rawLine, parsed, progress, ui = {}) {
  const line = String(rawLine ?? "").trim();
  if (!line) {
    return;
  }

  const json = parseJsonLine(line);
  if (!json) {
    parsed.nonJsonLines.push(rawLine);
    const stream = /warn|error/i.test(rawLine) ? process.stderr : process.stdout;
    stream.write(`[codex] ${rawLine}\n`);
    return;
  }

  if (ui.verbose) {
    progress.log(`event: ${shortenForLog(rawLine, 240)}`, "detail");
  }

  if (json.type === "thread.started" && typeof json.thread_id === "string") {
    parsed.threadId = json.thread_id;
    progress.log(`thread started: ${parsed.threadId}`);
    return;
  }

  if (json.type === "turn.started") {
    progress.log("provider turn started");
    return;
  }

  if (json.type === "turn.completed") {
    const usageSummary = formatUsageSummary(json.usage);
    progress.log(
      usageSummary ? `provider turn completed (${usageSummary})` : "provider turn completed",
      "success"
    );
    return;
  }

  if (json.type === "item.started" || json.type === "item.completed") {
    handleCodexItemEvent(json.type, json.item, parsed, progress, ui);
    return;
  }

  parsed.nonJsonLines.push(rawLine);
}

function parseArgs(args) {
  const out = { _: [] };
  let i = 0;
  while (i < args.length) {
    const token = args[i];
    if (!token.startsWith("--")) {
      out._.push(token);
      i += 1;
      continue;
    }

    const eq = token.indexOf("=");
    if (eq > -1) {
      const key = token.slice(2, eq);
      const value = token.slice(eq + 1);
      assignOption(out, key, value);
      i += 1;
      continue;
    }

    const key = token.slice(2);
    const next = args[i + 1];
    if (!next || next.startsWith("--")) {
      assignOption(out, key, true);
      i += 1;
      continue;
    }

    assignOption(out, key, next);
    i += 2;
  }
  return out;
}

function assignOption(target, key, value) {
  if (Object.prototype.hasOwnProperty.call(target, key)) {
    if (Array.isArray(target[key])) {
      target[key].push(value);
    } else {
      target[key] = [target[key], value];
    }
    return;
  }
  target[key] = value;
}

function asArray(v) {
  if (v === undefined) return [];
  return Array.isArray(v) ? v : [v];
}

function readPrompt(opts) {
  const parts = [];
  if (typeof opts.prompt === "string" && opts.prompt.trim().length > 0) {
    parts.push(opts.prompt.trim());
  }
  if (typeof opts["prompt-file"] === "string") {
    const filePath = path.resolve(process.cwd(), opts["prompt-file"]);
    if (!fs.existsSync(filePath)) {
      fail(`Prompt file not found: ${filePath}`);
    }
    parts.push(fs.readFileSync(filePath, "utf8").trim());
  }
  if (opts._.length > 0) {
    parts.push(opts._.join(" ").trim());
  }
  return parts.filter(Boolean).join("\n\n").trim();
}

function buildShellSchemaDefaults(opts = {}) {
  return {
    outputDir: parseSchemaOutputDir(opts["output-dir"]),
    dialogClass:
      typeof opts["dialog-class"] === "string" && opts["dialog-class"].trim().length > 0
        ? opts["dialog-class"].trim()
        : "",
    codeQueries: asArray(opts["code-search"]).map((q) => String(q).trim()).filter(Boolean),
    schemaName:
      typeof opts["schema-name"] === "string" && opts["schema-name"].trim().length > 0
        ? opts["schema-name"].trim()
        : null,
  };
}

function createDefaultShellSettings() {
  return {
    provider: "codex",
    model: null,
    mode: "schema",
    verbose: false,
    commandOutput: "summary",
    schemaDefaults: {
      outputDir: ".",
      dialogClass: "",
      codeQueries: [],
      schemaName: null,
    },
  };
}

function cloneShellSettings(settings) {
  return {
    provider: settings.provider,
    model: settings.model,
    mode: settings.mode,
    verbose: Boolean(settings.verbose),
    commandOutput: normalizeCommandOutputMode(settings.commandOutput ?? "summary"),
    schemaDefaults: {
      outputDir: settings.schemaDefaults.outputDir,
      dialogClass: settings.schemaDefaults.dialogClass,
      codeQueries: [...settings.schemaDefaults.codeQueries],
      schemaName: settings.schemaDefaults.schemaName,
    },
  };
}

function normalizePersistedShellSettings(rawSettings = {}) {
  const next = cloneShellSettings(createDefaultShellSettings());
  if (!rawSettings || typeof rawSettings !== "object") return next;

  try {
    if (Object.prototype.hasOwnProperty.call(rawSettings, "provider")) {
      next.provider = parseProvider(rawSettings.provider);
    }
  } catch {}

  if (typeof rawSettings.model === "string" && rawSettings.model.trim().length > 0) {
    next.model = rawSettings.model.trim();
  } else if (rawSettings.model === null) {
    next.model = null;
  }

  try {
    if (Object.prototype.hasOwnProperty.call(rawSettings, "mode")) {
      next.mode = normalizeShellMode(rawSettings.mode);
    }
  } catch {}

  try {
    if (Object.prototype.hasOwnProperty.call(rawSettings, "verbose")) {
      next.verbose = normalizeBooleanLike(rawSettings.verbose, "verbose");
    }
  } catch {}

  try {
    if (Object.prototype.hasOwnProperty.call(rawSettings, "commandOutput")) {
      next.commandOutput = normalizeCommandOutputMode(rawSettings.commandOutput);
    }
  } catch {}

  const schemaDefaults =
    rawSettings.schemaDefaults && typeof rawSettings.schemaDefaults === "object"
      ? rawSettings.schemaDefaults
      : {};

  try {
    if (Object.prototype.hasOwnProperty.call(schemaDefaults, "outputDir")) {
      next.schemaDefaults.outputDir = parseSchemaOutputDir(schemaDefaults.outputDir);
    }
  } catch {}

  if (typeof schemaDefaults.dialogClass === "string" && schemaDefaults.dialogClass.trim().length > 0) {
    next.schemaDefaults.dialogClass = schemaDefaults.dialogClass.trim();
  }

  if (Object.prototype.hasOwnProperty.call(schemaDefaults, "codeQueries")) {
    next.schemaDefaults.codeQueries = asArray(schemaDefaults.codeQueries)
      .map((q) => String(q).trim())
      .filter(Boolean);
  }

  if (typeof schemaDefaults.schemaName === "string" && schemaDefaults.schemaName.trim().length > 0) {
    next.schemaDefaults.schemaName = schemaDefaults.schemaName.trim();
  } else if (schemaDefaults.schemaName === null) {
    next.schemaDefaults.schemaName = null;
  }

  return next;
}

function serializeShellSettings(settings) {
  return {
    provider: settings.provider,
    model: settings.model,
    mode: settings.mode,
    verbose: Boolean(settings.verbose),
    commandOutput: normalizeCommandOutputMode(settings.commandOutput ?? "summary"),
    schemaDefaults: {
      outputDir: settings.schemaDefaults.outputDir,
      dialogClass: settings.schemaDefaults.dialogClass,
      codeQueries: [...settings.schemaDefaults.codeQueries],
      schemaName: settings.schemaDefaults.schemaName,
    },
  };
}

function writeShellSettingsFile(settings, settingsPath = BAT_RUN_SETTINGS_PATH) {
  fs.mkdirSync(path.dirname(settingsPath), { recursive: true });
  fs.writeFileSync(settingsPath, `${JSON.stringify(serializeShellSettings(settings), null, 2)}\n`, "utf8");
}

function loadShellSettingsFile(settingsPath = BAT_RUN_SETTINGS_PATH) {
  const fallback = createDefaultShellSettings();
  if (!fs.existsSync(settingsPath)) {
    writeShellSettingsFile(fallback, settingsPath);
    return fallback;
  }

  try {
    const rawText = fs.readFileSync(settingsPath, "utf8");
    const parsed = JSON.parse(rawText);
    const normalized = normalizePersistedShellSettings(parsed);
    const normalizedText = `${JSON.stringify(serializeShellSettings(normalized), null, 2)}\n`;
    if (rawText !== normalizedText) {
      fs.writeFileSync(settingsPath, normalizedText, "utf8");
    }
    return normalized;
  } catch (err) {
    process.stderr.write(`[bat_run] failed to read settings file, recreating defaults: ${String(err?.message ?? err)}\n`);
    writeShellSettingsFile(fallback, settingsPath);
    return fallback;
  }
}

function applyShellSettingsOverrides(settings, opts = {}) {
  const next = cloneShellSettings(settings);

  if (Object.prototype.hasOwnProperty.call(opts, "provider")) {
    next.provider = parseProvider(opts.provider);
  }

  if (Object.prototype.hasOwnProperty.call(opts, "model")) {
    next.model =
      typeof opts.model === "string" && opts.model.trim().length > 0 ? opts.model.trim() : null;
  }

  if (Object.prototype.hasOwnProperty.call(opts, "mode")) {
    next.mode = normalizeShellMode(opts.mode);
  }

  if (Object.prototype.hasOwnProperty.call(opts, "verbose")) {
    next.verbose = normalizeBooleanLike(opts.verbose, "verbose");
  }

  if (Object.prototype.hasOwnProperty.call(opts, "command-output")) {
    next.commandOutput = normalizeCommandOutputMode(opts["command-output"]);
  }

  if (Object.prototype.hasOwnProperty.call(opts, "output-dir")) {
    next.schemaDefaults.outputDir = parseSchemaOutputDir(opts["output-dir"]);
  }

  if (Object.prototype.hasOwnProperty.call(opts, "dialog-class")) {
    const dialogClass = String(opts["dialog-class"] ?? "").trim();
    next.schemaDefaults.dialogClass =
      !dialogClass || dialogClass.toLowerCase() === "off" || dialogClass === "-" ? "" : dialogClass;
  }

  if (Object.prototype.hasOwnProperty.call(opts, "code-search")) {
    next.schemaDefaults.codeQueries = asArray(opts["code-search"])
      .map((q) => String(q).trim())
      .filter(Boolean);
  }

  if (Object.prototype.hasOwnProperty.call(opts, "schema-name")) {
    const schemaName = String(opts["schema-name"] ?? "").trim();
    next.schemaDefaults.schemaName =
      !schemaName || schemaName.toLowerCase() === "auto" || schemaName === "-" ? null : schemaName;
  }

  return next;
}

function persistShellSettings(state) {
  writeShellSettingsFile(
    {
      provider: state.provider,
      model: state.model,
      mode: state.mode,
      verbose: Boolean(state.verbose),
      commandOutput: normalizeCommandOutputMode(state.commandOutput ?? "summary"),
      schemaDefaults: {
        outputDir: state.schemaDefaults.outputDir,
        dialogClass: state.schemaDefaults.dialogClass,
        codeQueries: [...state.schemaDefaults.codeQueries],
        schemaName: state.schemaDefaults.schemaName,
      },
    },
    state.settingsPath
  );
}

function tryPersistShellSettings(state, action = "save settings") {
  try {
    persistShellSettings(state);
    return true;
  } catch (err) {
    process.stderr.write(`[bat_run] failed to ${action}: ${String(err?.message ?? err)}\n`);
    return false;
  }
}
function ensureImageFiles(imagePaths) {
  for (const p of imagePaths) {
    const absolute = path.resolve(process.cwd(), p);
    if (!fs.existsSync(absolute)) {
      fail(`Image not found: ${absolute}`);
    }
  }
}

function createTempDir() {
  const dir = path.join(os.tmpdir(), "bat_run");
  fs.mkdirSync(dir, { recursive: true });
  return dir;
}

function saveClipboardImageToPng(options = {}) {
  const fatal = options.fatal !== false;
  const tempDir = createTempDir();
  const fileName = `clipboard_${Date.now()}.png`;
  const outPath = path.join(tempDir, fileName);
  const escaped = outPath.replace(/'/g, "''");

  const psScript = `
Add-Type -AssemblyName System.Windows.Forms
Add-Type -AssemblyName System.Drawing
$outputPath = '${escaped}'
try {
  if ([Windows.Forms.Clipboard]::ContainsImage()) {
    $img = [Windows.Forms.Clipboard]::GetImage()
    if ($img -ne $null) {
      $img.Save($outputPath, [System.Drawing.Imaging.ImageFormat]::Png)
      Write-Output $outputPath
      exit 0
    }
  }
} catch {}

try {
  Add-Type -AssemblyName WindowsBase
  Add-Type -AssemblyName PresentationCore
  Add-Type -AssemblyName PresentationFramework
  if ([System.Windows.Clipboard]::ContainsImage()) {
    $bmp = [System.Windows.Clipboard]::GetImage()
    if ($bmp -ne $null) {
      $encoder = New-Object System.Windows.Media.Imaging.PngBitmapEncoder
      $encoder.Frames.Add([System.Windows.Media.Imaging.BitmapFrame]::Create($bmp))
      $stream = [System.IO.File]::Open($outputPath, [System.IO.FileMode]::Create)
      try {
        $encoder.Save($stream)
      } finally {
        $stream.Dispose()
      }
      Write-Output $outputPath
      exit 0
    }
  }
} catch {}

Write-Output '__NO_CLIPBOARD_IMAGE__'
exit 3
`;

  const result = spawnCapture("powershell.exe", ["-NoProfile", "-STA", "-Command", psScript], {
    cwd: PROJECT_ROOT,
  });

  if (result.error) {
    const msg = `Clipboard read failed: ${result.error.message}`;
    if (fatal) fail(msg);
    throw new Error(`[bat_run] ${msg}`);
  }
  if (result.status !== 0) {
    const noImageToken = "__NO_CLIPBOARD_IMAGE__";
    if (result.stdout.includes(noImageToken)) {
      const msg = "No image found in clipboard. Use --image <path> instead.";
      if (fatal) fail(msg, 3);
      throw new Error(`[bat_run] ${msg}`);
    }
    const msg = (result.stderr || result.stdout || "Clipboard read failed.").trim();
    const full = `${msg} Use --image <path> instead.`;
    if (fatal) fail(full, 3);
    throw new Error(`[bat_run] ${full}`);
  }
  if (!fs.existsSync(outPath)) {
    const msg = "Clipboard command completed but PNG file was not created.";
    if (fatal) fail(msg);
    throw new Error(`[bat_run] ${msg}`);
  }

  return outPath;
}

const PNG_SIGNATURE = Buffer.from([0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a]);

function readPngMetadata(filePath) {
  try {
    const fd = fs.openSync(filePath, "r");
    try {
      const header = Buffer.alloc(24);
      const bytesRead = fs.readSync(fd, header, 0, header.length, 0);
      if (bytesRead < 24) return null;
      if (!header.subarray(0, 8).equals(PNG_SIGNATURE)) return null;
      return {
        width: header.readUInt32BE(16),
        height: header.readUInt32BE(20),
      };
    } finally {
      fs.closeSync(fd);
    }
  } catch {
    return null;
  }
}

function formatByteSize(bytes) {
  if (!Number.isFinite(bytes) || bytes < 0) return "unknown size";
  if (bytes < 1024) return `${bytes} B`;
  const units = ["KB", "MB", "GB", "TB"];
  let value = bytes / 1024;
  let unitIndex = 0;
  while (value >= 1024 && unitIndex < units.length - 1) {
    value /= 1024;
    unitIndex += 1;
  }
  const digits = value >= 100 ? 0 : value >= 10 ? 1 : 2;
  return `${value.toFixed(digits)} ${units[unitIndex]}`;
}

function describeImageFile(imagePath) {
  const abs = path.resolve(process.cwd(), imagePath);
  try {
    const stat = fs.statSync(abs);
    const sizeText = formatByteSize(stat.size);
    const png = readPngMetadata(abs);
    if (!png) {
      return `${abs} (${sizeText})`;
    }
    const suspicious = stat.size < 1024 || png.width <= 2 || png.height <= 2;
    const suffix = suspicious ? ", suspiciously small" : "";
    return `${abs} (PNG ${png.width}x${png.height}, ${sizeText}${suffix})`;
  } catch {
    return abs;
  }
}

function parseSchemaOutputDir(outputDirRaw) {
  if (typeof outputDirRaw !== "string" || outputDirRaw.trim().length === 0) return ".";
  const normalized = path.normalize(outputDirRaw.trim());
  if (path.isAbsolute(normalized)) {
    throw new Error(`--output-dir must be relative to generated_schemas. Received absolute path: ${normalized}`);
  }
  if (normalized === ".." || normalized.startsWith(`..${path.sep}`)) {
    throw new Error(`--output-dir must stay inside generated_schemas. Received: ${normalized}`);
  }
  return normalized;
}

function normalizeSchemaOutputDir(outputDirRaw) {
  try {
    return parseSchemaOutputDir(outputDirRaw);
  } catch (err) {
    fail(String(err?.message ?? err));
  }
}

function normalizeShellMode(modeRaw) {
  const mode = String(modeRaw ?? "schema").trim().toLowerCase();
  if (!mode || mode === "schema" || mode === "dev" || mode === "developer") return "schema";
  if (mode === "chat" || mode === "user" || mode === "prompt") return "chat";
  throw new Error(`Unsupported shell mode: ${modeRaw}. Use schema or chat.`);
}
function defaultSchemaName() {
  return `schema_${new Date().toISOString().replace(/\D/g, "").slice(0, 14)}`;
}

function resolveSchemaOutputPaths(schemaName, outputDir = ".") {
  const relativeDir = !outputDir || outputDir === "." ? "" : outputDir;
  return {
    schemaPath: path.join(GENERATED_SCHEMAS_ROOT, relativeDir, `${schemaName}.json`),
    metaPath: path.join(GENERATED_SCHEMAS_ROOT, relativeDir, `${schemaName}.meta.json`),
  };
}

function toProjectRelative(filePath) {
  return path.relative(PROJECT_ROOT, filePath).split(path.sep).join("/");
}

function snapshotFile(filePath) {
  try {
    const stat = fs.statSync(filePath);
    return { mtimeMs: stat.mtimeMs, size: stat.size };
  } catch {
    return null;
  }
}

function snapshotGeneratedSchemas(rootDir = GENERATED_SCHEMAS_ROOT) {
  const files = new Map();
  if (!fs.existsSync(rootDir)) return files;
  const stack = [rootDir];
  while (stack.length > 0) {
    const currentDir = stack.pop();
    const entries = fs.readdirSync(currentDir, { withFileTypes: true });
    for (const entry of entries) {
      const abs = path.join(currentDir, entry.name);
      if (entry.isDirectory()) {
        stack.push(abs);
        continue;
      }
      if (!entry.isFile() || !entry.name.endsWith(".json")) continue;
      const stat = fs.statSync(abs);
      const rel = path.relative(PROJECT_ROOT, abs).split(path.sep).join("/");
      files.set(rel, { mtimeMs: stat.mtimeMs, size: stat.size });
    }
  }
  return files;
}

function diffGeneratedSchemas(beforeMap, afterMap) {
  const changed = [];
  for (const [rel, stat] of afterMap.entries()) {
    const prev = beforeMap.get(rel);
    if (!prev || prev.mtimeMs !== stat.mtimeMs || prev.size !== stat.size) {
      changed.push(rel);
    }
  }
  changed.sort();
  return changed;
}

function didFileChange(beforeStat, afterStat) {
  if (!afterStat) return false;
  if (!beforeStat) return true;
  return beforeStat.mtimeMs !== afterStat.mtimeMs || beforeStat.size !== afterStat.size;
}

function createSchemaSaveTracker(paths) {
  return {
    schemaPath: paths.schemaPath,
    metaPath: paths.metaPath,
    schemaPathRelative: toProjectRelative(paths.schemaPath),
    metaPathRelative: toProjectRelative(paths.metaPath),
    beforeSchema: snapshotFile(paths.schemaPath),
    beforeMeta: snapshotFile(paths.metaPath),
    beforeGenerated: snapshotGeneratedSchemas(),
  };
}

function printSchemaSaveResult(tracker) {
  if (!tracker) return false;
  const afterSchema = snapshotFile(tracker.schemaPath);
  const afterMeta = snapshotFile(tracker.metaPath);
  const afterGenerated = snapshotGeneratedSchemas();
  const changed = diffGeneratedSchemas(tracker.beforeGenerated, afterGenerated);
  const expectedSchemaChanged = didFileChange(tracker.beforeSchema, afterSchema);
  const expectedMetaChanged = didFileChange(tracker.beforeMeta, afterMeta);

  if (afterSchema && (expectedSchemaChanged || expectedMetaChanged || changed.includes(tracker.schemaPathRelative))) {
    process.stdout.write(`[bat_run] saved schema: ${tracker.schemaPathRelative}\n`);
    if (afterMeta) {
      process.stdout.write(`[bat_run] schema meta: ${tracker.metaPathRelative}\n`);
    }
    return true;
  }

  const changedSchemaFiles = changed.filter((rel) => rel.endsWith(".json") && !rel.endsWith(".meta.json"));
  if (changedSchemaFiles.length === 1) {
    process.stdout.write(`[bat_run] saved schema: ${changedSchemaFiles[0]}\n`);
    const metaCandidate = changed.find((rel) => rel === changedSchemaFiles[0].replace(/\.json$/, ".meta.json"));
    if (metaCandidate) {
      process.stdout.write(`[bat_run] schema meta: ${metaCandidate}\n`);
    }
    return true;
  }

  if (changedSchemaFiles.length > 1) {
    process.stdout.write("[bat_run] multiple schema files updated:\n");
    for (const rel of changedSchemaFiles) {
      process.stdout.write(`- ${rel}\n`);
    }
    return true;
  }

  if (afterSchema) {
    process.stdout.write(`[bat_run] schema file present: ${tracker.schemaPathRelative}\n`);
    if (afterMeta) {
      process.stdout.write(`[bat_run] schema meta: ${tracker.metaPathRelative}\n`);
    }
    return true;
  }

  process.stderr.write(`[bat_run] schema file not detected under generated_schemas. Expected: ${tracker.schemaPathRelative}\n`);
  return false;
}

function gatherCodeContext(queries, progress = null) {
  if (queries.length === 0) return "";

  if (progress) {
    progress.log(`collecting code context from ${queries.length} quer${queries.length === 1 ? "y" : "ies"}`);
  }

  const chunks = [];
  for (const [index, query] of queries.entries()) {
    if (progress) {
      progress.log(`code search ${index + 1}/${queries.length}: ${query}`);
    }

    const args = [
      "-n",
      "--no-heading",
      "--color",
      "never",
      "--max-count",
      "120",
      query,
      ...SEARCH_PATHS,
    ];
    const result = spawnCapture("rg", args, { cwd: PROJECT_ROOT });
    if (result.error) {
      chunks.push(`# query: ${query}\n[rg error] ${result.error.message}`);
      continue;
    }
    if (result.status === 0) {
      chunks.push(`# query: ${query}\n${result.stdout.trim()}`);
    } else {
      chunks.push(`# query: ${query}\n(no matches)`);
    }
  }

  if (progress) {
    progress.log("code context ready");
  }

  return chunks.join("\n\n").trim();
}

function makeSchemaCreatePrompt({
  userPrompt,
  schemaName,
  outputDir,
  schemaPathRelative,
  dialogClass,
  codeContext,
}) {
  const lines = [
    "You are working in the apiverification repository.",
    "Create a JSON schema using the provided prompt, image context, and optional code context.",
    "",
    "Mandatory MCP workflow:",
    "1) If dialogClass is provided, call mcp__apiverification__parse_dialog first.",
    "2) Read MCP rules resources before saving schema:",
    "   - mcp://rules/shared.yaml",
    "   - mcp://rules/promptRules.yaml",
    "3) Call mcp__apiverification__save_schema with these exact path values:",
    `   - schemaName: "${schemaName}"`,
    `   - outputDir: "${outputDir}"`,
    "   - outputDir is relative to the repo generated_schemas directory",
    "   - do not replace it with default unless the user explicitly asks",
    `   - expected schema path: "${schemaPathRelative}"`,
    "   - schema: <generated schema object>",
    "",
    "If information is uncertain, include questions in save_schema call instead of guessing.",
    "",
    "User prompt:",
    "```text",
    userPrompt,
    "```",
  ];

  if (dialogClass) {
    lines.push("", "Dialog class:", "```text", dialogClass, "```");
  }

  if (codeContext && codeContext.length > 0) {
    lines.push("", "Code context (search results):", "```text", codeContext, "```");
  }

  lines.push(
    "",
    "Final response should include:",
    "- saved schema path",
    "- summary of key fields/conditions",
    "- unresolved questions (if any)"
  );

  return lines.join("\n");
}
function runInteractiveSession(opts) {
  const provider = normalizeProvider(opts.provider);
  const images = asArray(opts.image);

  if (opts["from-clipboard"]) {
    const clipboardImage = saveClipboardImageToPng();
    images.push(clipboardImage);
    process.stdout.write(`[bat_run] Clipboard image saved: ${describeImageFile(clipboardImage)}\n`);
  }
  ensureImageFiles(images);

  let prompt = readPrompt(opts);
  if (images.length > 0) {
    const imagePathNote = images
      .map((p) => `- ${path.resolve(process.cwd(), p)}`)
      .join("\n");
    const header = `Attached image path(s):\n${imagePathNote}`;
    prompt = prompt ? `${header}\n\n${prompt}` : header;
  }

  if (opts["dry-run"]) {
    const info = {
      mode: "interactive",
      provider,
      images: images.map((p) => path.resolve(process.cwd(), p)),
      prompt: prompt || null,
    };
    process.stdout.write(`${JSON.stringify(info, null, 2)}\n`);
    process.exit(0);
  }

  if (provider === "codex") {
    const codexBin = resolveToolBinary("codex");
    const args = [];
    if (opts.model) {
      args.push("--model", String(opts.model));
    }
    if (prompt) {
      args.push(prompt);
    }
    for (const image of images) {
      args.push("-i", path.resolve(process.cwd(), image));
    }
    spawnInherit(codexBin, args, { cwd: process.cwd() });
    return;
  }

  if (images.length > 0) {
    fail("claude-code provider currently does not support --image in interactive wrapper. Use codex provider.");
  }

  const claudeBin = resolveToolBinary("claude");
  const args = [];
  if (opts.model) {
    args.push("--model", String(opts.model));
  }
  if (prompt) {
    args.push(prompt);
  }
  spawnInherit(claudeBin, args, { cwd: process.cwd() });
}

function parseJsonLine(line) {
  try {
    return JSON.parse(line);
  } catch {
    return null;
  }
}

function parseCodexJsonOutput(stdout) {
  const out = {
    threadId: null,
    messages: [],
    nonJsonLines: [],
  };

  const lines = stdout.split(/\r?\n/g);
  for (const rawLine of lines) {
    const line = rawLine.trim();
    if (!line) continue;
    const json = parseJsonLine(line);
    if (!json) {
      out.nonJsonLines.push(rawLine);
      continue;
    }

    if (json.type === "thread.started" && typeof json.thread_id === "string") {
      out.threadId = json.thread_id;
      continue;
    }

    if (json.type === "item.completed" && json.item?.type === "agent_message") {
      const text = String(json.item?.text ?? "").trim();
      if (text) out.messages.push(text);
    }
  }

  return out;
}

function appendPendingImage(state, imagePath) {
  const abs = path.resolve(process.cwd(), imagePath);
  if (!fs.existsSync(abs)) {
    fail(`Image not found: ${abs}`);
  }
  state.pendingImages.push(abs);
}

async function runShellCodexTurn(state, prompt, schemaSaveTracker = null, progress = null) {
  const reporter = progress ?? createProgressReporter("chat");
  const codexBin = resolveToolBinary("codex");
  const args = [];
  const isResume = Boolean(state.sessionId);

  if (isResume) {
    args.push("exec", "resume", "--json", state.sessionId);
  } else {
    args.push("exec", "--json", "-C", process.cwd(), "--skip-git-repo-check");
  }

  if (state.model) {
    args.push("--model", state.model);
  }

  args.push(prompt);

  for (const image of state.pendingImages) {
    args.push("-i", image);
  }

  reporter.log(isResume ? `resuming codex turn (${state.sessionId})` : "starting codex turn");
  if (state.model) {
    reporter.log(`model: ${state.model}`);
  }
  if (state.pendingImages.length > 0) {
    reporter.log(`attachments: ${state.pendingImages.length} image(s)`);
  }

  const parsed = {
    threadId: state.sessionId,
    messages: [],
    nonJsonLines: [],
  };

  const result = await spawnCaptureAsync(codexBin, args, {
    cwd: process.cwd(),
    heartbeatIntervalMs: 5000,
    onHeartbeat: (elapsedMs) => {
      reporter.pulse(`waiting for codex response (${formatElapsedTime(elapsedMs)})`);
    },
    onStdoutLine: (line) => {
      handleCodexJsonLine(line, parsed, reporter, {
        verbose: state.verbose,
        commandOutput: state.commandOutput,
      });
    },
    onStderrLine: (line) => {
      const text = String(line ?? "").trim();
      if (text) {
        process.stderr.write(`[codex] ${text}\n`);
      }
    },
  });

  if (result.error) {
    process.stderr.write(`[bat_run] codex exec failed: ${result.error.message}\n`);
    return;
  }

  if (parsed.threadId) {
    state.sessionId = parsed.threadId;
  }

  if (result.status !== 0) {
    process.stderr.write(`[bat_run] codex turn failed with code ${result.status}\n`);
  }

  if (schemaSaveTracker) {
    reporter.log("checking generated_schemas for saved schema");
    printSchemaSaveResult(schemaSaveTracker);
  }

  state.pendingImages = [];
  reporter.log("turn complete");
}

async function runShellClaudeTurn(state, prompt, schemaSaveTracker = null, progress = null) {
  const reporter = progress ?? createProgressReporter("chat");

  if (state.pendingImages.length > 0) {
    process.stderr.write(
      "[bat_run] claude-code in shell mode does not support image attach in this wrapper yet.\n"
    );
    return;
  }

  const claudeBin = resolveToolBinary("claude");
  const args = ["--print"];
  if (state.model) {
    args.push("--model", state.model);
  }
  args.push(prompt);

  reporter.log("starting claude turn");
  if (state.model) {
    reporter.log(`model: ${state.model}`);
  }

  const result = await spawnCaptureAsync(claudeBin, args, {
    cwd: process.cwd(),
    heartbeatIntervalMs: 5000,
    onHeartbeat: (elapsedMs) => {
      reporter.pulse(`waiting for claude response (${formatElapsedTime(elapsedMs)})`);
    },
    onStderrLine: (line) => {
      const text = String(line ?? "").trim();
      if (text) {
        process.stderr.write(`[claude] ${text}\n`);
      }
    },
  });

  if (result.error) {
    process.stderr.write(`[bat_run] claude run failed: ${result.error.message}\n`);
    return;
  }

  const out = result.stdout.trim();
  if (out) {
    reporter.log("assistant response received");
    process.stdout.write(`assistant> ${out}\n`);
  }

  if (result.status !== 0) {
    process.stderr.write(`[bat_run] claude turn failed with code ${result.status}\n`);
  }

  if (schemaSaveTracker) {
    reporter.log("checking generated_schemas for saved schema");
    printSchemaSaveResult(schemaSaveTracker);
  }

  reporter.log("turn complete");
}

function printShellHelp() {
  const text = `
bat_run shell commands:
  /help                    Show this help
  /settings                Show current shell settings
  /status                  Alias of /settings
  /log-file                Show current shell log file
  /tail-log [n]            Show recent shell log lines
  /mode <schema|chat>      Set plain-text mode (default: schema)
  /provider <name>         Set provider (codex|claude-code)
  /model <name|off>        Set or clear model
  /verbose <on|off>        Toggle raw Codex event lines
  /command-output <mode>   Set command/MCP output mode (off|summary|full)
  /output-dir <rel|.>      Set schema output dir under generated_schemas
  /dialog-class <name|off> Set default dialog class for schema mode
  /code-search add <q>     Add code-search query for schema mode
  /code-search list        Show code-search queries
  /code-search clear       Clear code-search queries
  /schema-name <name|auto> Set fixed schema name or revert to auto
  /paste                   Add clipboard image to pending attachments
  /compose                 Open multiline compose mode
  /submit                  Send compose buffer (only in compose mode)
  /cancel-compose          Discard compose buffer (only in compose mode)
  /image <path>            Add image file to pending attachments
  /images                  Show pending image attachments
  /schema <text>           Force schema create with current schema settings
  /send <text>             Force a normal AI prompt (bypass schema mode)
  /clear                   Clear pending attachments
  /new                     Start a new codex conversation (clear session)
  /exit                    Exit shell

Tip:
  - Press / on an empty prompt to open the command palette.
  - Use Arrow keys + Enter inside the palette. Esc closes it.
  - Plain text follows the current mode. Default is schema (developer mode).
  - /schema uses current schema settings and saves to generated_schemas/<schemaName>.json.
  - /send is the escape hatch for normal chat while staying in schema mode.
  - /verbose on shows raw provider event lines for debugging.
  - /command-output full expands command/MCP output blocks.
  - /log-file prints the session log path under %APPDATA%\bat_run\logs.
  - /tail-log 80 shows the most recent session log lines.
  - Ctrl+Shift+V is the official image paste shortcut.
  - Alt+V is the fallback shortcut.
  - If the terminal steals paste shortcuts, use /paste.
  - /compose opens multiline input. Each Enter adds a line until /submit.
  - Multiline paste is automatically merged into one prompt before execution.
`;
  process.stdout.write(text.trimStart() + "\n");
}
function handleShellPaste(state, options = {}) {
  try {
    const clip = saveClipboardImageToPng({ fatal: false });
    appendPendingImage(state, clip);
    const prefix = options.inline ? "\n" : "";
    process.stdout.write(`${prefix}[bat_run] pasted image: ${describeImageFile(clip)}\n`);
    return true;
  } catch (err) {
    const prefix = options.inline ? "\n" : "";
    process.stderr.write(prefix + String(err.message ?? err) + "\n");
    return false;
  }
}

async function handleShellLine(line, state, rl) {
  if (line === "/exit" || line === "/quit") {
    rl.close();
    process.exit(0);
  }


  appendActiveSessionLogLine(`[user] ${line}`);
  if (line === "/help") {
    printShellHelp();
    return;
  }

  if (line === "/settings" || line === "/status") {
    printShellStatus(state);
    return;
  }

  if (line === "/log-file") {
    process.stdout.write(`[bat_run] session log: ${state.logFilePath}\n`);
    return;
  }

  if (line.startsWith("/tail-log")) {
    const parts = line.split(/\s+/g).filter(Boolean);
    const count = parts.length > 1 ? Number(parts[1]) : 40;
    if (!Number.isFinite(count) || count <= 0) {
      process.stderr.write("[bat_run] usage: /tail-log [positive-count]\n");
      return;
    }
    printRecentLogLines(state.logFilePath, count);
    return;
  }

  if (line === "/mode") {
    process.stderr.write("[bat_run] usage: /mode <schema|chat>\n");
    return;
  }

  if (line === "/provider") {
    process.stderr.write("[bat_run] usage: /provider <codex|claude-code>\n");
    return;
  }

  if (line === "/model") {
    process.stderr.write("[bat_run] usage: /model <name|off>\n");
    return;
  }

  if (line === "/verbose") {
    process.stderr.write("[bat_run] usage: /verbose <on|off>\\n");
    return;
  }

  if (line === "/command-output") {
    process.stderr.write("[bat_run] usage: /command-output <off|summary|full>\\n");
    return;
  }

  if (line === "/output-dir") {
    process.stderr.write("[bat_run] usage: /output-dir <relative-dir|.>\n");
    return;
  }

  if (line === "/dialog-class") {
    process.stderr.write("[bat_run] usage: /dialog-class <name|off>\n");
    return;
  }

  if (line === "/code-search") {
    process.stderr.write("[bat_run] usage: /code-search <add <query>|list|clear>\n");
    return;
  }

  if (line === "/schema-name") {
    process.stderr.write("[bat_run] usage: /schema-name <name|auto>\n");
    return;
  }

  if (line === "/schema") {
    process.stderr.write("[bat_run] usage: /schema <text>\n");
    return;
  }

  if (line === "/send") {
    process.stderr.write("[bat_run] usage: /send <text>\n");
    return;
  }

  if (line === "/compose") {
    process.stderr.write("[bat_run] /compose opens multiline input from the shell prompt.\n");
    return;
  }

  if (line === "/submit") {
    process.stderr.write("[bat_run] /submit is only available while compose mode is active.\n");
    return;
  }

  if (line === "/cancel-compose") {
    process.stderr.write("[bat_run] /cancel-compose is only available while compose mode is active.\n");
    return;
  }

  if (line === "/images") {
    if (state.pendingImages.length === 0) {
      process.stdout.write("[bat_run] no pending images\n");
    } else {
      state.pendingImages.forEach((img, idx) => process.stdout.write(`${idx + 1}. ${describeImageFile(img)}\n`));
    }
    return;
  }

  if (line === "/clear") {
    state.pendingImages = [];
    process.stdout.write("[bat_run] cleared pending images\n");
    return;
  }

  if (line === "/new") {
    state.sessionId = null;
    process.stdout.write("[bat_run] started new conversation context\n");
    return;
  }

  if (line === "/paste") {
    handleShellPaste(state);
    return;
  }

  if (line.startsWith("/image ")) {
    const imgPath = line.slice("/image ".length).trim();
    if (!imgPath) {
      process.stderr.write("[bat_run] usage: /image <path>\n");
      return;
    }
    try {
      appendPendingImage(state, imgPath);
      process.stdout.write(`[bat_run] image queued: ${describeImageFile(imgPath)}\n`);
    } catch (err) {
      process.stderr.write(String(err?.message ?? err) + "\n");
    }
    return;
  }

  if (line.startsWith("/verbose ")) {
    const rawValue = line.slice("/verbose ".length).trim();
    try {
      state.verbose = normalizeBooleanLike(rawValue, "verbose");
      tryPersistShellSettings(state);
      process.stdout.write(`[bat_run] verbose events ${state.verbose ? "enabled" : "disabled"}\n`);
    } catch (err) {
      process.stderr.write(`[bat_run] ${String(err?.message ?? err)}\n`);
    }
    return;
  }

  if (line.startsWith("/command-output ")) {
    const rawValue = line.slice("/command-output ".length).trim();
    try {
      state.commandOutput = normalizeCommandOutputMode(rawValue);
      tryPersistShellSettings(state);
      process.stdout.write(`[bat_run] command/MCP output mode: ${state.commandOutput}\n`);
    } catch (err) {
      process.stderr.write(`[bat_run] ${String(err?.message ?? err)}\n`);
    }
    return;
  }

  if (line.startsWith("/mode ")) {
    const rawMode = line.slice("/mode ".length).trim();
    try {
      state.mode = normalizeShellMode(rawMode);
      tryPersistShellSettings(state);
      process.stdout.write(`[bat_run] mode set to ${state.mode}\n`);
    } catch (err) {
      process.stderr.write(String(err?.message ?? err) + "\n");
    }
    return;
  }

  if (line.startsWith("/provider ")) {
    const nextProvider = line.slice("/provider ".length).trim();
    try {
      state.provider = parseProvider(nextProvider);
      state.sessionId = null;
      tryPersistShellSettings(state);
      process.stdout.write(`[bat_run] provider set to ${state.provider} (session reset)\n`);
    } catch (err) {
      process.stderr.write(String(err?.message ?? err) + "\n");
    }
    return;
  }

  if (line.startsWith("/model ")) {
    const modelName = line.slice("/model ".length).trim();
    if (!modelName || modelName.toLowerCase() === "off" || modelName === "-") {
      state.model = null;
      tryPersistShellSettings(state);
      process.stdout.write("[bat_run] model override cleared\n");
    } else {
      state.model = modelName;
      tryPersistShellSettings(state);
      process.stdout.write(`[bat_run] model override set: ${state.model}\n`);
    }
    return;
  }

  if (line.startsWith("/output-dir ")) {
    const outputDirRaw = line.slice("/output-dir ".length).trim();
    try {
      state.schemaDefaults.outputDir = parseSchemaOutputDir(outputDirRaw);
      tryPersistShellSettings(state);
      process.stdout.write(`[bat_run] schema output dir set: ${state.schemaDefaults.outputDir}\n`);
    } catch (err) {
      process.stderr.write(String(err?.message ?? err) + "\n");
    }
    return;
  }

  if (line.startsWith("/dialog-class ")) {
    const dialogClassRaw = line.slice("/dialog-class ".length).trim();
    if (!dialogClassRaw || dialogClassRaw.toLowerCase() === "off" || dialogClassRaw === "-") {
      state.schemaDefaults.dialogClass = "";
      tryPersistShellSettings(state);
      process.stdout.write("[bat_run] schema dialog class cleared\n");
    } else {
      state.schemaDefaults.dialogClass = dialogClassRaw;
      tryPersistShellSettings(state);
      process.stdout.write(`[bat_run] schema dialog class set: ${state.schemaDefaults.dialogClass}\n`);
    }
    return;
  }

  if (line === "/code-search list") {
    if (state.schemaDefaults.codeQueries.length === 0) {
      process.stdout.write("[bat_run] no code-search queries configured\n");
    } else {
      state.schemaDefaults.codeQueries.forEach((query, idx) => process.stdout.write(`${idx + 1}. ${query}\n`));
    }
    return;
  }

  if (line === "/code-search clear") {
    state.schemaDefaults.codeQueries = [];
    tryPersistShellSettings(state);
    process.stdout.write("[bat_run] cleared schema code-search queries\n");
    return;
  }

  if (line.startsWith("/code-search add ")) {
    const query = line.slice("/code-search add ".length).trim();
    if (!query) {
      process.stderr.write("[bat_run] usage: /code-search add <query>\n");
      return;
    }
    if (!state.schemaDefaults.codeQueries.includes(query)) {
      state.schemaDefaults.codeQueries.push(query);
      tryPersistShellSettings(state);
    }
    process.stdout.write(`[bat_run] schema code-search added: ${query}\n`);
    return;
  }

  if (line.startsWith("/schema-name ")) {
    const schemaNameRaw = line.slice("/schema-name ".length).trim();
    if (!schemaNameRaw || schemaNameRaw.toLowerCase() === "auto" || schemaNameRaw === "-") {
      state.schemaDefaults.schemaName = null;
      tryPersistShellSettings(state);
      process.stdout.write("[bat_run] schema name reset to auto timestamp\n");
    } else {
      state.schemaDefaults.schemaName = schemaNameRaw;
      tryPersistShellSettings(state);
      process.stdout.write(`[bat_run] fixed schema name set: ${state.schemaDefaults.schemaName}\n`);
    }
    return;
  }

  if (line.startsWith("/schema ")) {
    const userPrompt = line.slice("/schema ".length).trim();
    if (!userPrompt) {
      process.stderr.write("[bat_run] usage: /schema <text>\n");
      return;
    }
    await runShellSchemaCreate(state, userPrompt);
    return;
  }

  let prompt = line;
  let forceSend = false;
  if (line.startsWith("/send ")) {
    prompt = line.slice("/send ".length).trim();
    if (!prompt) {
      process.stderr.write("[bat_run] usage: /send <text>\n");
      return;
    }
    forceSend = true;
  }

  if (!forceSend && state.mode === "schema") {
    await runShellSchemaCreate(state, prompt);
    return;
  }

  if (state.provider === "codex") await runShellCodexTurn(state, prompt);
  else await runShellClaudeTurn(state, prompt);
}
async function runShellSchemaCreate(state, userPrompt) {
  const progress = createProgressReporter("schema");
  progress.log("preparing schema request");

  const schemaName = state.schemaDefaults.schemaName || defaultSchemaName();
  const outputDir = state.schemaDefaults.outputDir;
  const dialogClass = state.schemaDefaults.dialogClass;
  const codeQueries = [...state.schemaDefaults.codeQueries];

  if (state.pendingImages.length > 0) {
    progress.log(`attachments ready: ${state.pendingImages.length} image(s)`);
  }
  if (dialogClass) {
    progress.log(`dialog class: ${dialogClass}`);
  }

  const codeContext = gatherCodeContext(codeQueries, progress);
  const paths = resolveSchemaOutputPaths(schemaName, outputDir);
  const schemaPathRelative = toProjectRelative(paths.schemaPath);
  const schemaSaveTracker = createSchemaSaveTracker(paths);
  const prompt = makeSchemaCreatePrompt({
    userPrompt,
    schemaName,
    outputDir,
    schemaPathRelative,
    dialogClass,
    codeContext,
  });

  progress.log(`prompt ready for ${state.provider}${state.model ? ` using ${state.model}` : ""}`);
  process.stdout.write(`[bat_run] schema target: ${schemaPathRelative}\n`);

  if (state.provider === "codex") {
    await runShellCodexTurn(state, prompt, schemaSaveTracker, progress);
  } else {
    await runShellClaudeTurn(state, prompt, schemaSaveTracker, progress);
  }
}

function printShellStatus(state) {
  const status = {
    settingsFile: state.settingsPath,
    logFile: state.logFilePath,
    provider: state.provider,
    model: state.model,
    mode: state.mode,
    verbose: Boolean(state.verbose),
    commandOutput: state.commandOutput,
    plainTextTarget: state.mode === "schema" ? "schema-create" : "chat",
    sessionId: state.sessionId,
    pendingImages: state.pendingImages.map((img) => describeImageFile(img)),
    schemaDefaults: {
      outputDir: state.schemaDefaults.outputDir,
      dialogClass: state.schemaDefaults.dialogClass || null,
      schemaName: state.schemaDefaults.schemaName || "auto",
      codeQueries: [...state.schemaDefaults.codeQueries],
    },
  };
  process.stdout.write(`${JSON.stringify(status, null, 2)}\n`);
}
function printStartupBanner(state) {
  const banner = colorize(STARTUP_BANNER_LINES.join("\n"), ANSI.cyan);
  process.stdout.write(`${banner}\n`);
  process.stdout.write(`[bat_run] project: ${path.basename(PROJECT_ROOT)}\n`);
  process.stdout.write(`[bat_run] settings: ${state.settingsPath}\n`);
  process.stdout.write(`[bat_run] log: ${state.logFilePath}\n`);
  process.stdout.write(`[bat_run] ui: verbose=${state.verbose ? "on" : "off"}, command-output=${state.commandOutput}\n`);
}

function createCommandPaletteState() {
  return {
    open: false,
    items: [...SHELL_COMMAND_PALETTE_ITEMS],
    selectedIndex: 0,
    renderedLines: 0,
    suppressNextEmptyLine: false,
  };
}

function createMultilineComposerState() {
  return {
    active: false,
    lines: [],
    submitOnBlankLine: false,
  };
}

function resetReadlineBuffer(rl) {
  rl.line = "";
  rl.cursor = 0;
}

function clearCommandPalette(rl, palette) {
  if (!palette.renderedLines) return;
  const output = rl.output ?? process.stdout;
  readline.cursorTo(output, 0);
  for (let i = 0; i < palette.renderedLines; i += 1) {
    readline.clearLine(output, 0);
    if (i < palette.renderedLines - 1) {
      readline.moveCursor(output, 0, -1);
    }
  }
  readline.cursorTo(output, 0);
  palette.renderedLines = 0;
}

function renderCommandPalette(rl, palette) {
  clearCommandPalette(rl, palette);
  const output = rl.output ?? process.stdout;
  const lines = [
    "[bat_run] command palette (Arrow keys, Enter to select, Esc to cancel)",
    ...palette.items.map((item, idx) => {
      const marker = idx === palette.selectedIndex ? ">" : " ";
      return `${marker} ${item.command.padEnd(24)} ${item.description}`;
    }),
  ];
  output.write(lines.join("\n"));
  palette.renderedLines = lines.length;
}

function openCommandPalette(rl, palette) {
  if (palette.open) return;
  palette.open = true;
  palette.selectedIndex = 0;
  palette.suppressNextEmptyLine = false;
  resetReadlineBuffer(rl);
  const output = rl.output ?? process.stdout;
  readline.clearLine(output, 0);
  readline.cursorTo(output, 0);
  renderCommandPalette(rl, palette);
}

function closeCommandPalette(rl, palette, insertText = "") {
  if (!palette.open) return;
  clearCommandPalette(rl, palette);
  palette.open = false;
  palette.selectedIndex = 0;
  resetReadlineBuffer(rl);
  rl.prompt();
  if (insertText) {
    rl.write(insertText);
  }
}

function handleCommandPaletteKeypress(rl, palette, key, runShellCommand) {
  if (!palette.open) return false;

  if (key.name === "up") {
    palette.selectedIndex =
      (palette.selectedIndex - 1 + palette.items.length) % palette.items.length;
    renderCommandPalette(rl, palette);
    return true;
  }

  if (key.name === "down" || key.name === "tab") {
    palette.selectedIndex = (palette.selectedIndex + 1) % palette.items.length;
    renderCommandPalette(rl, palette);
    return true;
  }

  if (key.name === "escape") {
    closeCommandPalette(rl, palette);
    return true;
  }

  if (key.name === "return" || key.name === "enter") {
    const item = palette.items[palette.selectedIndex];
    palette.suppressNextEmptyLine = true;
    closeCommandPalette(rl, palette, item?.action === "insert" ? item.value : "");
    if (item?.action === "execute") {
      void runShellCommand(item.value);
    }
    return true;
  }

  return true;
}
function buildShellIo() {
  let input = process.stdin;
  let output = process.stdout;
  let rawCapable = Boolean(process.stdin.isTTY && typeof process.stdin.setRawMode === "function");
  let cleanup = () => {};

  if (process.platform === "win32" && (!process.stdin.isTTY || !process.stdout.isTTY)) {
    try {
      const inputFd = fs.openSync("CONIN$", "r");
      const outputFd = fs.openSync("CONOUT$", "w");
      const consoleInput = fs.createReadStream(null, { fd: inputFd, autoClose: true });
      const consoleOutput = fs.createWriteStream(null, { fd: outputFd, autoClose: true });
      input = consoleInput;
      output = consoleOutput;
      rawCapable = Boolean(typeof consoleInput.setRawMode === "function");
      cleanup = () => {
        consoleInput.destroy();
        consoleOutput.end();
      };
    } catch {
      // Keep default stdio when console handles are not available.
    }
  }

  return {
    input,
    output,
    rawCapable,
    terminal: input === process.stdin ? Boolean(process.stdin.isTTY) : true,
    cleanup,
  };
}

async function startWrapperShell(initialOpts = {}) {
  const logFilePath = createShellSessionLogFile();
  setActiveSessionLogFile(logFilePath);
  const persistedSettings = loadShellSettingsFile();
  const effectiveSettings = applyShellSettingsOverrides(persistedSettings, initialOpts);
  const state = {
    settingsPath: BAT_RUN_SETTINGS_PATH,
    logFilePath,
    provider: effectiveSettings.provider,
    model: effectiveSettings.model,
    mode: effectiveSettings.mode,
    verbose: Boolean(effectiveSettings.verbose),
    commandOutput: normalizeCommandOutputMode(effectiveSettings.commandOutput ?? "summary"),
    sessionId: null,
    pendingImages: [],
    schemaDefaults: {
      outputDir: effectiveSettings.schemaDefaults.outputDir,
      dialogClass: effectiveSettings.schemaDefaults.dialogClass,
      codeQueries: [...effectiveSettings.schemaDefaults.codeQueries],
      schemaName: effectiveSettings.schemaDefaults.schemaName,
    },
  };

  tryPersistShellSettings(state, "initialize settings file");

  for (const img of asArray(initialOpts.image)) {
    appendPendingImage(state, img);
  }
  if (initialOpts["from-clipboard"]) {
    try {
      const clip = saveClipboardImageToPng({ fatal: false });
      appendPendingImage(state, clip);
      process.stdout.write(`[bat_run] Clipboard image saved: ${describeImageFile(clip)}\n`);
    } catch (err) {
      process.stderr.write(`${String(err.message ?? err)}\n`);
    }
  }

  printStartupBanner(state);
  process.stdout.write(
    `[bat_run] shell started (provider=${state.provider}, mode=${state.mode}). Press / for command palette or type /help.\n`
  );

  const initialPrompt = readPrompt(initialOpts);
  if (initialPrompt) {
    if (state.mode === "schema") {
      await runShellSchemaCreate(state, initialPrompt);
    } else if (state.provider === "codex") {
      await runShellCodexTurn(state, initialPrompt);
    } else {
      await runShellClaudeTurn(state, initialPrompt);
    }
  }

  const shellIo = buildShellIo();

  const rl = readline.createInterface({
    input: shellIo.input,
    output: shellIo.output,
    terminal: shellIo.terminal,
    historySize: 1000,
  });

  const commandPalette = createCommandPaletteState();
  const composer = createMultilineComposerState();
  const pendingBurst = {
    lines: [],
    timer: null,
  };
  let handlingLine = false;
  let shellClosed = false;

  const updatePrompt = () => {
    rl.setPrompt(composer.active ? SHELL_COMPOSE_PROMPT : SHELL_DEFAULT_PROMPT);
  };

  const promptShell = (preserveCursor = false) => {
    updatePrompt();
    rl.prompt(preserveCursor);
  };

  const clearPendingBurst = () => {
    if (pendingBurst.timer) {
      clearTimeout(pendingBurst.timer);
      pendingBurst.timer = null;
    }
    pendingBurst.lines = [];
  };

  const exitComposeMode = ({ message = null } = {}) => {
    composer.active = false;
    composer.lines = [];
    composer.submitOnBlankLine = false;
    if (message) {
      process.stdout.write(`${message}\n`);
    }
  };

  const enterComposeMode = ({ initialLines = [], submitOnBlankLine = false, message = null } = {}) => {
    if (!composer.active) {
      composer.active = true;
      composer.lines = [];
    }
    composer.submitOnBlankLine = submitOnBlankLine;
    if (initialLines.length > 0) {
      composer.lines.push(...initialLines);
    }
    process.stdout.write(
      `${message ?? "[bat_run] compose mode enabled. Enter multiple lines, then type /submit to send or /cancel-compose to discard."}\n`
    );
    promptShell();
  };

  const runShellCommand = async (line) => {
    const trimmedLine = String(line ?? "").trim();
    if (!trimmedLine) {
      if (!shellClosed) {
        promptShell();
      }
      return;
    }
    if (trimmedLine === "/compose") {
      enterComposeMode();
      return;
    }

    handlingLine = true;
    try {
      await handleShellLine(line, state, rl);
    } finally {
      handlingLine = false;
      if (!shellClosed) {
        promptShell();
      }
    }
  };

  const flushPendingBurst = async () => {
    if (pendingBurst.lines.length === 0) return;

    const rawBurstLines = pendingBurst.lines.map((line) => String(line ?? ""));
    const looksLikePaste = rawBurstLines.length > 1 || rawBurstLines.some((line) => /\u001b\[\?2004[hl]|\u001b\[200~|\u001b\[201~/.test(line));
    const burstLines = rawBurstLines.map((line) =>
      line.replace(/\u001b\[\?2004[hl]/g, "").replace(/\u001b\[200~|\u001b\[201~/g, "")
    );
    clearPendingBurst();

    const merged = burstLines.join("\n");
    if (!merged.trim()) {
      if (!shellClosed) {
        promptShell();
      }
      return;
    }

    if (looksLikePaste) {
      if (composer.active) {
        composer.lines.push(...burstLines);
        if (!shellClosed) {
          promptShell();
        }
        return;
      }

      enterComposeMode({
        initialLines: burstLines,
        submitOnBlankLine: true,
        message:
          "[bat_run] multiline paste loaded. Review or add more text, then press Enter on an empty prompt to send. Use /cancel-compose to discard.",
      });
      return;
    }

    await runShellCommand(merged);
  };

  const queuePendingBurst = (lineRaw) => {
    pendingBurst.lines.push(lineRaw);
    if (pendingBurst.timer) {
      clearTimeout(pendingBurst.timer);
    }
    pendingBurst.timer = setTimeout(() => {
      void flushPendingBurst();
    }, SHELL_PASTE_BURST_DEBOUNCE_MS);
  };

  readline.emitKeypressEvents(shellIo.input, rl);
  if (shellIo.rawCapable) {
    shellIo.input.setRawMode(true);
  }
  if (shellIo.terminal) {
    shellIo.output.write("\u001b[?2004h");
  }

  const keypressHandler = (_str, key = {}) => {
    if (handlingLine) return;

    if (commandPalette.open) {
      if (handleCommandPaletteKeypress(rl, commandPalette, key, runShellCommand)) {
        return;
      }
    }

    const wantsPalette =
      !key.ctrl &&
      !key.meta &&
      !key.alt &&
      key.sequence === "/" &&
      ((rl.line ?? "") === "" || (rl.line ?? "") === "/");
    if (wantsPalette) {
      openCommandPalette(rl, commandPalette);
      return;
    }

    const wantsPaste =
      key.name === "v" && ((key.ctrl && key.shift) || key.alt || key.meta);
    if (wantsPaste) {
      const pasted = handleShellPaste(state, { inline: true });
      if (pasted) {
        promptShell(true);
      }
    }
  };

  shellIo.input.on("keypress", keypressHandler);

  promptShell();

  await new Promise((resolve) => {
    rl.on("line", async (lineRaw) => {
      if (commandPalette.suppressNextEmptyLine && !lineRaw.trim()) {
        commandPalette.suppressNextEmptyLine = false;
        if (!shellClosed) {
          promptShell();
        }
        return;
      }

      if (composer.active) {
        const line = lineRaw.trim();
        if (line === "/submit") {
          const prompt = composer.lines.join("\n");
          exitComposeMode();
          if (!prompt.trim()) {
            process.stderr.write("[bat_run] compose buffer is empty\n");
            if (!shellClosed) {
              promptShell();
            }
            return;
          }
          await runShellCommand(prompt);
          return;
        }
        if (!lineRaw.length && composer.submitOnBlankLine) {
          const prompt = composer.lines.join("\n");
          exitComposeMode();
          if (!prompt.trim()) {
            if (!shellClosed) {
              promptShell();
            }
            return;
          }
          await runShellCommand(prompt);
          return;
        }
        if (line === "/cancel-compose") {
          exitComposeMode({ message: "[bat_run] compose buffer discarded" });
          if (!shellClosed) {
            promptShell();
          }
          return;
        }
        composer.lines.push(lineRaw);
        if (!shellClosed) {
          promptShell();
        }
        return;
      }

      if (lineRaw.trim() === "/compose") {
        enterComposeMode();
        return;
      }

      queuePendingBurst(lineRaw);
    });

    rl.on("close", () => {
      shellClosed = true;
      clearPendingBurst();
      clearCommandPalette(rl, commandPalette);
      shellIo.input.off("keypress", keypressHandler);
      if (shellIo.rawCapable) {
        shellIo.input.setRawMode(false);
      }
      if (shellIo.terminal) {
        shellIo.output.write("\u001b[?2004l");
      }
      shellIo.cleanup();
      setActiveSessionLogFile(null);
      resolve();
    });
  });
}
function hasInteractiveWrapperOptions(args) {
  const flags = new Set([
    "--provider",
    "--image",
    "--from-clipboard",
    "--prompt",
    "--prompt-file",
    "--model",
    "--mode",
    "--verbose",
    "--command-output",
    "--output-dir",
    "--dialog-class",
    "--code-search",
    "--schema-name",
    "--dry-run",
  ]);
  return args.some(
    (a) =>
      flags.has(a) ||
      a.startsWith("--image=") ||
      a.startsWith("--prompt=") ||
      a.startsWith("--mode=") ||
      a.startsWith("--verbose=") ||
      a.startsWith("--command-output=") ||
      a.startsWith("--output-dir=") ||
      a.startsWith("--dialog-class=") ||
      a.startsWith("--code-search=") ||
      a.startsWith("--schema-name=")
  );
}

function normalizeMcpDisplayValue(result) {
  if (!result || typeof result !== "object") {
    return result;
  }

  if (Array.isArray(result.content) && result.content.length === 1 && result.content[0]?.type === "text") {
    const text = String(result.content[0].text ?? "").trim();
    return maybeParseJsonText(text, "application/json") ?? text;
  }

  if (Array.isArray(result.contents) && result.contents.length === 1 && typeof result.contents[0]?.text === "string") {
    const text = String(result.contents[0].text ?? "").trim();
    return maybeParseJsonText(text, String(result.contents[0].mimeType ?? "")) ?? text;
  }

  if (Array.isArray(result.messages) && result.messages.length > 0) {
    const lines = [];
    for (const message of result.messages) {
      const role = String(message?.role ?? "message");
      if (message?.content?.type === "text") {
        lines.push(`[${role}]`);
        lines.push(String(message.content.text ?? "").trim());
      } else {
        lines.push(`[${role}] ${JSON.stringify(message?.content ?? null, null, 2)}`);
      }
    }
    return lines.join("\n\n").trim();
  }

  return normalizeMcpResult(result);
}

function printMcpValue(value, jsonOutput = false) {
  if (jsonOutput || (value && typeof value === "object")) {
    process.stdout.write(`${JSON.stringify(value, null, 2)}\n`);
    return;
  }
  process.stdout.write(`${String(value ?? "")}\n`);
}

function printMcpListResult(kind, result, jsonOutput = false) {
  if (jsonOutput) {
    process.stdout.write(`${JSON.stringify(result, null, 2)}\n`);
    return;
  }

  let items = [];
  let keyField = "name";
  if (kind === "tools") {
    items = result?.tools ?? [];
    keyField = "name";
  } else if (kind === "resources") {
    items = result?.resources ?? [];
    keyField = "uri";
  } else if (kind === "prompts") {
    items = result?.prompts ?? [];
    keyField = "name";
  } else {
    items = result?.resourceTemplates ?? [];
    keyField = "uriTemplate";
  }

  if (!Array.isArray(items) || items.length === 0) {
    process.stdout.write(`[bat_run] no ${kind} available\n`);
    return;
  }

  items.forEach((item, index) => {
    const id = String(item?.[keyField] ?? item?.name ?? item?.uri ?? `item-${index + 1}`);
    process.stdout.write(`${index + 1}. ${id}\n`);
    if (item?.description) {
      const summary = String(item.description).trim().split(/\r?\n/g)[0];
      process.stdout.write(`   ${summary}\n`);
    }
    if (kind === "resources" && item?.mimeType) {
      process.stdout.write(`   mime: ${String(item.mimeType)}\n`);
    }
  });
}

function printMcpHelp() {
  const text = `Usage:\n  bat_run mcp list [tools|resources|prompts|resource-templates] [--json]\n  bat_run mcp call <tool> [--input-json "<json>" | --input-file <file>] [--json]\n  bat_run mcp read-resource <uri> [--json]\n  bat_run mcp get-prompt <name> [--arg key=value] [--json]\n\nExamples:\n  bat_run mcp list tools\n  bat_run mcp call parse_dialog --input-json "{\"dialogClass\":\"CNodeCreateDlg\"}"\n  bat_run mcp read-resource mcp://rules/shared.yaml\n  bat_run mcp get-prompt schema_rules --arg topic=table\n`;
  process.stdout.write(text);
}

function buildApiRequestFromDefinition(requestDef, suiteDefaults = {}) {
  if (!requestDef || typeof requestDef !== "object") {
    throw new Error("API test request must be an object.");
  }

  const source =
    requestDef.request && typeof requestDef.request === "object"
      ? {
          ...requestDef.request,
          name: requestDef.name ?? requestDef.request.name,
          expect: requestDef.expect ?? requestDef.assert ?? requestDef.request.expect ?? requestDef.request.assert,
          save: requestDef.save ?? requestDef.request.save,
        }
      : requestDef;

  const method = String(source.method ?? suiteDefaults.method ?? "GET").trim().toUpperCase();
  const url = String(source.url ?? source.path ?? "").trim();
  const baseUrlRaw = source.baseUrl ?? suiteDefaults.baseUrl ?? null;
  const headers = mergeHeaderMaps(
    suiteDefaults.headers && typeof suiteDefaults.headers === "object" ? suiteDefaults.headers : {},
    source.headers && typeof source.headers === "object" ? source.headers : {}
  );
  const query = Object.assign(
    {},
    suiteDefaults.query && typeof suiteDefaults.query === "object" ? suiteDefaults.query : {},
    source.query && typeof source.query === "object" ? source.query : {}
  );
  const timeoutMs = Number(source.timeoutMs ?? suiteDefaults.timeoutMs ?? 30000);
  if (!Number.isFinite(timeoutMs) || timeoutMs <= 0) {
    throw new Error("API test timeoutMs must be a positive number.");
  }

  let body = null;
  let bodyKind = null;
  if (Object.prototype.hasOwnProperty.call(source, "jsonBody")) {
    body = JSON.stringify(source.jsonBody);
    bodyKind = "json";
  } else if (typeof source.jsonBodyFile === "string" && source.jsonBodyFile.trim()) {
    body = JSON.stringify(parseJsonText(readTextFile(source.jsonBodyFile, "json body file"), "json body file"));
    bodyKind = "json";
  } else if (Object.prototype.hasOwnProperty.call(source, "body")) {
    if (typeof source.body === "string") {
      body = source.body;
      bodyKind = "text";
    } else {
      body = JSON.stringify(source.body);
      bodyKind = "json";
    }
  } else if (typeof source.bodyFile === "string" && source.bodyFile.trim()) {
    body = readTextFile(source.bodyFile, "body file");
    bodyKind = "text";
  }

  if (bodyKind === "json" && !Object.keys(headers).some((key) => key.toLowerCase() === "content-type")) {
    headers["Content-Type"] = "application/json";
  }
  if ((method === "GET" || method === "HEAD") && body !== null) {
    throw new Error(`${method} requests cannot include a body.`);
  }

  return {
    method,
    url,
    baseUrl: typeof baseUrlRaw === "string" ? baseUrlRaw.trim() : baseUrlRaw,
    headers,
    query,
    timeoutMs,
    body,
    bodyKind,
    meta: {
      name: String(source.name ?? `${method} ${url || "request"}`),
      expect: source.expect ?? source.assert ?? {},
      save: source.save ?? null,
    },
  };
}

function printApiHelp() {
  const text = `Usage:\n  bat_run api request --url <url> [--method GET|POST|PUT|PATCH|DELETE]\n                      [--base-url <url>] [--query key=value] [--header "Name: Value"]\n                      [--json-body "<json>" | --json-body-file <file> | --body <text> | --body-file <file>]\n                      [--output pretty|json|text] [--save <file>] [--timeout <ms>]\n  bat_run api test --file <suite.json> [--json] [--stop-on-fail]\n\nExamples:\n  bat_run api request --url https://httpbin.org/get --query hello=world --output json\n  bat_run api test --file api_tests/smoke.json\n`;
  process.stdout.write(text);
}











































export {
  asArray,
  buildApiRequestFromDefinition,
  buildApiRequestFromOptions,
  createProgressReporter,
  createSchemaSaveTracker,
  defaultSchemaName,
  describeImageFile,
  ensureImageFiles,
  evaluateApiExpectations,
  fail,
  gatherCodeContext,
  hasInteractiveWrapperOptions,
  makeSchemaCreatePrompt,
  normalizeApiTestSuite,
  normalizeMcpDisplayValue,
  normalizeProvider,
  normalizeSchemaOutputDir,
  parseArgs,
  parseEqualsEntries,
  parseJsonText,
  performApiRequest,
  printApiHelp,
  printApiResult,
  printHelp,
  printMcpHelp,
  printMcpListResult,
  printMcpValue,
  printSchemaSaveResult,
  readJsonInputOption,
  readPrompt,
  readTextFile,
  resolveSchemaOutputPaths,
  resolveToolBinary,
  runAiCommand,
  runInteractiveSession,
  saveApiResponseIfRequested,
  saveClipboardImageToPng,
  spawnInherit,
  startWrapperShell,
  toProjectRelative,
  withMcpClient,
};

