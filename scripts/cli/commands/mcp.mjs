import {
  fail,
  normalizeMcpDisplayValue,
  parseArgs,
  parseEqualsEntries,
  printMcpHelp,
  printMcpListResult,
  printMcpValue,
  readJsonInputOption,
  withMcpClient,
} from "../core.mjs";
import { createProgressReporter } from "../output/progress.mjs";

export async function commandMcp(rawArgs) {
  const [sub = "help", ...rest] = rawArgs;
  const opts = parseArgs(rest);
  const jsonOutput = Boolean(opts.json);
  const progress = createProgressReporter("mcp");

  if (sub === "help" || sub === "--help" || sub === "-h") {
    printMcpHelp();
    return;
  }

  if (sub === "list") {
    const kind = String(opts._[0] ?? "tools").trim().toLowerCase();
    progress.log(`listing ${kind}`);
    const result = await withMcpClient(async (client) => {
      if (kind === "tools") return await client.listTools();
      if (kind === "resources") return await client.listResources();
      if (kind === "prompts") return await client.listPrompts();
      if (kind === "resource-templates" || kind === "templates") {
        return await client.listResourceTemplates();
      }
      throw new Error(`Unknown MCP list target: ${kind}`);
    }, progress);
    printMcpListResult(kind === "templates" ? "resource-templates" : kind, result, jsonOutput);
    return;
  }

  if (sub === "call") {
    const toolName = String(opts._[0] ?? "").trim();
    if (!toolName) {
      fail("mcp call requires a tool name.");
    }
    const input = readJsonInputOption(opts, "input-json", "input-file", {});
    progress.log(`calling tool ${toolName}`);
    const result = await withMcpClient(
      async (client) => await client.callTool({ name: toolName, arguments: input }),
      progress
    );
    if (jsonOutput) {
      process.stdout.write(`${JSON.stringify(result, null, 2)}\n`);
    } else {
      printMcpValue(normalizeMcpDisplayValue(result));
    }
    if (result?.isError) {
      process.exitCode = 1;
    }
    return;
  }

  if (sub === "read-resource") {
    const uri = String(opts._[0] ?? "").trim();
    if (!uri) {
      fail("mcp read-resource requires a resource URI.");
    }
    progress.log(`reading resource ${uri}`);
    const result = await withMcpClient(
      async (client) => await client.readResource({ uri }),
      progress
    );
    if (jsonOutput) {
      process.stdout.write(`${JSON.stringify(result, null, 2)}\n`);
    } else {
      printMcpValue(normalizeMcpDisplayValue(result));
    }
    return;
  }

  if (sub === "get-prompt") {
    const name = String(opts._[0] ?? "").trim();
    if (!name) {
      fail("mcp get-prompt requires a prompt name.");
    }
    const promptArgs = parseEqualsEntries(opts.arg, "prompt arg");
    progress.log(`getting prompt ${name}`);
    const result = await withMcpClient(
      async (client) => await client.getPrompt({ name, arguments: promptArgs }),
      progress
    );
    if (jsonOutput) {
      process.stdout.write(`${JSON.stringify(result, null, 2)}\n`);
    } else {
      printMcpValue(normalizeMcpDisplayValue(result));
    }
    return;
  }

  fail(`Unknown mcp subcommand: ${sub}`);
}
