import path from "node:path";

import {
  describeImageFile,
  fail,
  parseArgs,
  readPrompt,
  saveClipboardImageToPng,
} from "../core.mjs";
import { normalizeProvider, runAiCommand } from "../providers/runner.mjs";

export function commandPaste(rawArgs) {
  const opts = parseArgs(rawArgs);
  const provider = normalizeProvider(opts.provider);
  const prompt = readPrompt(opts);

  if (!prompt) {
    fail('paste requires --prompt or prompt text. Example: bat_run paste --prompt "generate schema from UI image"');
  }

  const clipboardImage = saveClipboardImageToPng();
  const images = [clipboardImage];
  process.stdout.write(`[bat_run] Clipboard image saved: ${describeImageFile(clipboardImage)}\n`);

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
