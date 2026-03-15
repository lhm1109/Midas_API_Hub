import path from "node:path";

import {
  asArray,
  createSchemaSaveTracker,
  defaultSchemaName,
  describeImageFile,
  ensureImageFiles,
  fail,
  gatherCodeContext,
  makeSchemaCreatePrompt,
  normalizeSchemaOutputDir,
  parseArgs,
  printSchemaSaveResult,
  readPrompt,
  resolveSchemaOutputPaths,
  saveClipboardImageToPng,
  toProjectRelative,
} from "../core.mjs";
import { createProgressReporter } from "../output/progress.mjs";
import { normalizeProvider, runAiCommand } from "../providers/runner.mjs";

export function commandSchemaCreate(rawArgs) {
  const opts = parseArgs(rawArgs);
  const provider = normalizeProvider(opts.provider);
  const progress = createProgressReporter("schema");
  const userPrompt = readPrompt(opts);

  if (!userPrompt) {
    fail("schema create requires --prompt or --prompt-file.");
  }

  progress.log("preparing schema request");

  const images = asArray(opts.image);
  if (opts["from-clipboard"]) {
    const clipboardImage = saveClipboardImageToPng();
    images.push(clipboardImage);
    process.stdout.write(`[bat_run] Clipboard image saved: ${describeImageFile(clipboardImage)}\n`);
  }
  ensureImageFiles(images);
  if (images.length > 0) {
    progress.log(`attachments ready: ${images.length} image(s)`);
  }

  const codeQueries = asArray(opts["code-search"]).map((query) => String(query).trim()).filter(Boolean);
  const schemaName =
    (typeof opts["schema-name"] === "string" && opts["schema-name"].trim()) ||
    defaultSchemaName();
  const outputDir = normalizeSchemaOutputDir(opts["output-dir"]);
  const paths = resolveSchemaOutputPaths(schemaName, outputDir);
  const schemaPathRelative = toProjectRelative(paths.schemaPath);
  const dialogClass =
    typeof opts["dialog-class"] === "string" && opts["dialog-class"].trim().length > 0
      ? opts["dialog-class"].trim()
      : "";
  if (dialogClass) {
    progress.log(`dialog class: ${dialogClass}`);
  }
  const codeContext = gatherCodeContext(codeQueries, progress);

  const prompt = makeSchemaCreatePrompt({
    userPrompt,
    schemaName,
    outputDir,
    schemaPathRelative,
    dialogClass,
    codeContext,
  });
  progress.log(`prompt ready for ${provider}${opts.model ? ` using ${opts.model}` : ""}`);

  if (opts["dry-run"]) {
    const info = {
      provider,
      schemaName,
      outputDir,
      schemaPath: schemaPathRelative,
      metaPath: toProjectRelative(paths.metaPath),
      dialogClass: dialogClass || null,
      images: images.map((imagePath) => path.resolve(process.cwd(), imagePath)),
      codeQueries,
      prompt,
    };
    process.stdout.write(`${JSON.stringify(info, null, 2)}\n`);
    process.exit(0);
  }

  const schemaSaveTracker = createSchemaSaveTracker(paths);
  process.stdout.write(`[bat_run] schema target: ${schemaPathRelative}\n`);
  progress.log(`running ${provider} CLI`);
  const result = runAiCommand(provider, prompt, images, opts.model, { exitOnComplete: false });
  progress.log("checking generated_schemas for saved schema");
  printSchemaSaveResult(schemaSaveTracker);
  progress.log(result?.status === 0 ? "schema command complete" : `provider exited with code ${result?.status ?? 1}`);
  process.exit(result?.status ?? 1);
}
