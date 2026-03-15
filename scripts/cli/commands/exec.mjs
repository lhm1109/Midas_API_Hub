import path from "node:path";

import {
  asArray,
  describeImageFile,
  ensureImageFiles,
  fail,
  parseArgs,
  readPrompt,
  saveClipboardImageToPng,
} from "../core.mjs";
import { normalizeProvider, runAiCommand } from "../providers/runner.mjs";

export function commandExec(rawArgs) {
  const opts = parseArgs(rawArgs);
  const provider = normalizeProvider(opts.provider);
  const prompt = readPrompt(opts);
  if (!prompt) {
    fail('exec requires a prompt. Example: bat_run exec "summarize this repo"');
  }

  const images = asArray(opts.image);
  if (opts["from-clipboard"]) {
    const clipboardImage = saveClipboardImageToPng();
    images.push(clipboardImage);
    process.stdout.write(`[bat_run] Clipboard image saved: ${describeImageFile(clipboardImage)}\n`);
  }
  ensureImageFiles(images);

  if (opts["dry-run"]) {
    const info = {
      provider,
      images: images.map((imagePath) => path.resolve(process.cwd(), imagePath)),
      prompt,
    };
    process.stdout.write(`${JSON.stringify(info, null, 2)}\n`);
    process.exit(0);
  }

  runAiCommand(provider, prompt, images, opts.model);
}
