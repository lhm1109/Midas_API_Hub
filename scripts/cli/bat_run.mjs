#!/usr/bin/env node

import {
  fail,
  hasInteractiveWrapperOptions,
  parseArgs,
  printHelp,
  runInteractiveSession,
  startWrapperShell,
} from "./core.mjs";
import { commandApi } from "./commands/api.mjs";
import { commandAuth } from "./commands/auth.mjs";
import { commandExec } from "./commands/exec.mjs";
import { commandMcp } from "./commands/mcp.mjs";
import { commandPaste } from "./commands/paste.mjs";
import { commandSchemaCreate } from "./schema/create.mjs";

function main() {
  const args = process.argv.slice(2);

  if (args.length === 0) {
    startWrapperShell({ _: [] }).catch((err) => fail(String(err?.message ?? err)));
    return;
  }

  const [command, ...rest] = args;
  if (command === "help" || command === "--help" || command === "-h") {
    printHelp();
    process.exit(0);
  }

  if (command === "mcp") {
    commandMcp(rest).catch((err) => fail(String(err?.message ?? err)));
    return;
  }

  if (command === "api") {
    commandApi(rest).catch((err) => fail(String(err?.message ?? err)));
    return;
  }

  if (command === "exec") {
    commandExec(rest);
    return;
  }

  if (command === "paste") {
    commandPaste(rest);
    return;
  }

  if (command === "auth") {
    commandAuth(rest);
    return;
  }

  if (command === "shell") {
    const opts = parseArgs(rest);
    startWrapperShell(opts).catch((err) => fail(String(err?.message ?? err)));
    return;
  }

  if (command === "native") {
    const opts = parseArgs(rest);
    runInteractiveSession(opts);
    return;
  }

  if (command === "schema" && rest[0] === "create") {
    commandSchemaCreate(rest.slice(1));
    return;
  }

  if (hasInteractiveWrapperOptions(args)) {
    const opts = parseArgs(args);
    startWrapperShell(opts).catch((err) => fail(String(err?.message ?? err)));
    return;
  }

  const opts = parseArgs(args);
  startWrapperShell(opts).catch((err) => fail(String(err?.message ?? err)));
}

main();
