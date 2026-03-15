import {
  buildApiRequestFromDefinition,
  buildApiRequestFromOptions,
  evaluateApiExpectations,
  fail,
  normalizeApiTestSuite,
  parseArgs,
  parseJsonText,
  performApiRequest,
  printApiHelp,
  printApiResult,
  readTextFile,
  saveApiResponseIfRequested,
} from "../core.mjs";
import { createProgressReporter } from "../output/progress.mjs";

export async function commandApi(rawArgs) {
  const [sub = "help", ...rest] = rawArgs;
  const opts = parseArgs(rest);
  const jsonOutput = Boolean(opts.json);

  if (sub === "help" || sub === "--help" || sub === "-h") {
    printApiHelp();
    return;
  }

  if (sub === "request") {
    if (!opts.url && opts._.length > 0) {
      opts.url = opts._[0];
    }
    const request = buildApiRequestFromOptions(opts);
    if (!request.url) {
      fail("api request requires --url or a positional URL.");
    }
    const result = await performApiRequest(request, createProgressReporter("api"));
    saveApiResponseIfRequested(result, opts.save);
    printApiResult(result, jsonOutput && !opts.output ? "json" : opts.output ?? "pretty");
    return;
  }

  if (sub === "test") {
    const filePath = String(opts.file ?? opts._[0] ?? "").trim();
    if (!filePath) {
      fail("api test requires --file <suite.json>.");
    }

    const rawSuite = parseJsonText(readTextFile(filePath, "api test file"), "api test file");
    const suite = normalizeApiTestSuite(rawSuite);
    const progress = createProgressReporter("api");
    const stopOnFail = Boolean(opts["stop-on-fail"]);
    const results = [];

    for (let index = 0; index < suite.requests.length; index += 1) {
      const requestDef = suite.requests[index];
      const request = buildApiRequestFromDefinition(requestDef, suite);
      const testName = request.meta.name || `request-${index + 1}`;
      progress.log(`[${index + 1}/${suite.requests.length}] ${testName}`);
      const result = await performApiRequest(request, progress);
      if (request.meta.save) {
        saveApiResponseIfRequested(result, request.meta.save);
      }
      const issues = evaluateApiExpectations(result, request.meta.expect);
      const passed = issues.length === 0;
      const entry = {
        index: index + 1,
        name: testName,
        method: result.method,
        url: result.url,
        status: result.status,
        ok: result.ok,
        passed,
        issues,
      };
      results.push(entry);

      if (!jsonOutput) {
        process.stdout.write(`[bat_run] ${passed ? "PASS" : "FAIL"} ${entry.index}. ${entry.name} -> ${entry.status}\n`);
        for (const issue of issues) {
          process.stdout.write(`  - ${issue}\n`);
        }
      }

      if (!passed && stopOnFail) {
        progress.log(`stopping after failure in ${testName}`, "warn");
        break;
      }
    }

    const passedCount = results.filter((entry) => entry.passed).length;
    const failedCount = results.length - passedCount;
    const payload = {
      ok: failedCount === 0,
      summary: {
        total: results.length,
        passed: passedCount,
        failed: failedCount,
      },
      results,
    };

    if (jsonOutput) {
      process.stdout.write(`${JSON.stringify(payload, null, 2)}\n`);
    } else {
      process.stdout.write(`[bat_run] api test summary: ${passedCount}/${results.length} passed, ${failedCount} failed\n`);
    }

    if (failedCount > 0) {
      process.exitCode = 1;
    }
    return;
  }

  fail(`Unknown api subcommand: ${sub}`);
}
