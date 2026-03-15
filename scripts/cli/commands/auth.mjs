import { fail, parseArgs } from "../core.mjs";
import { normalizeProvider, resolveToolBinary, spawnInherit } from "../providers/runner.mjs";

export function commandAuth(rawArgs) {
  const sub = rawArgs[0];
  const opts = parseArgs(rawArgs.slice(1));
  const provider = normalizeProvider(opts.provider);

  if (!sub || sub === "help" || sub === "--help") {
    process.stdout.write(
      "Usage: bat_run auth <login|status|logout> --provider codex|claude-code\n"
    );
    process.exit(0);
  }

  if (provider === "codex") {
    const codexBin = resolveToolBinary("codex");
    if (sub === "login") {
      spawnInherit(codexBin, ["login"], { cwd: process.cwd() });
      return;
    }
    if (sub === "status") {
      spawnInherit(codexBin, ["login", "status"], { cwd: process.cwd() });
      return;
    }
    if (sub === "logout") {
      spawnInherit(codexBin, ["logout"], { cwd: process.cwd() });
      return;
    }
    fail(`Unknown auth subcommand: ${sub}`);
  }

  if (sub === "login") {
    const claudeBin = resolveToolBinary("claude");
    process.stdout.write("[bat_run] Claude Code login is handled by Claude CLI. Running setup-token...\n");
    spawnInherit(claudeBin, ["setup-token"], { cwd: process.cwd() });
    return;
  }
  if (sub === "status") {
    const claudeBin = resolveToolBinary("claude");
    process.stdout.write(
      "[bat_run] Claude Code CLI has no stable auth status command in this environment. Showing version only.\n"
    );
    spawnInherit(claudeBin, ["--version"], { cwd: process.cwd() });
    return;
  }
  if (sub === "logout") {
    fail("Claude Code logout is not standardized in current CLI. Please remove token via Claude settings.");
  }

  fail(`Unknown auth subcommand: ${sub}`);
}
