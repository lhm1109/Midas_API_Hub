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

export function formatElapsedTime(ms) {
  const totalSeconds = Math.max(0, Math.floor(ms / 1000));
  const minutes = Math.floor(totalSeconds / 60);
  const seconds = totalSeconds % 60;
  return `${String(minutes).padStart(2, "0")}:${String(seconds).padStart(2, "0")}`;
}

export function colorize(text, color) {
  if (!ANSI_ENABLED || !color) {
    return text;
  }
  return `${color}${text}${ANSI.reset}`;
}

function buildProgressPrefix(scope, elapsedText, kind = "info", spinnerFrame = null) {
  const glyph =
    spinnerFrame ??
    (kind === "success" ? "ok" : kind === "warn" ? "!!" : kind === "error" ? "xx" : kind === "detail" ? ".." : ">>");
  const scopeText = colorize(scope, SCOPE_COLOR_BY_NAME[scope] ?? ANSI.cyan);
  const elapsedLabel = colorize(`[${elapsedText}]`, ANSI.dim);
  const glyphText = colorize(glyph, PROGRESS_COLOR_BY_KIND[kind] ?? ANSI.cyan);
  const prefix = colorize("[bat_run]", ANSI.bold);
  return `${prefix} ${elapsedLabel} ${scopeText} ${glyphText}`;
}

export function createProgressReporter(scope = "task") {
  const startedAt = Date.now();
  let spinnerIndex = 0;
  return {
    log(message, kind = "info") {
      process.stdout.write(`${buildProgressPrefix(scope, formatElapsedTime(Date.now() - startedAt), kind)} ${message}\n`);
    },
    pulse(message, kind = "wait") {
      const frame = SPINNER_FRAMES[spinnerIndex % SPINNER_FRAMES.length];
      spinnerIndex += 1;
      process.stdout.write(`${buildProgressPrefix(scope, formatElapsedTime(Date.now() - startedAt), kind, frame)} ${message}\n`);
    },
  };
}
