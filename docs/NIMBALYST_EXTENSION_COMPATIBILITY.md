# Nimbalyst Extension Compatibility Layer

Bat Rack can load a practical subset of Nimbalyst extensions from the local `extensions/` folder.

The first global tab is now a Nimbalyst workbench instead of the earlier AI-only assistance view.
It exposes Nimbalyst-style modes (`Files`, `Agent`, `Tracker`, `Shared Docs`, `Extensions`, `Settings`) and connects to the compatibility host below.

## Porting Direction

Use `docs/REFERENCE/nimbalyst` as the source reference, but do not embed or launch the whole reference app as the production tab. Bat Rack should copy or adapt the specific runtime pieces it needs into this compatibility layer so future features can share Bat Rack routing, Supabase data, workspace permissions, Electron IPC, and settings.

## Supported Now

- `manifest.json` discovery
- Manifest-only extension discovery and activation for metadata-only contributions such as `claudePlugin` and `agentWorkflows`
- Reference marketplace listing, local install flow, source-extension build/install flow, and one-click build/install-all flow for bundled extensions under `docs/REFERENCE/nimbalyst/packages/extensions`
- `main` ESM module import
- `activate(context)` / `deactivate()`
- `aiTools` export
- `context.services.ai.registerTool(tool)`
- `context.services.ai.registerContextProvider(provider)`
- `context.services.ai.sendPrompt()`
- `context.services.ai.chatCompletion()`
- `context.services.ai.chatCompletionStream()`
- `context.services.filesystem.readFile/writeFile/fileExists/findFiles`
- `context.services.configuration.get/update/getAll`
- Settings UI and REST persistence for `contributions.configuration`
- Settings UI selector and workbench CSS variable application for `contributions.themes`
- Backend module declaration validation, workspace/global consent persistence, and worker-thread execution for `contributions.backendModules`
- `context.services.ui.showInfo/showWarning/showError`
- Runtime shim for server-hosted imports from `@nimbalyst/extension-sdk`
- Runtime shim for server-hosted imports from `@nimbalyst/runtime`
- Manifest contribution registry for panels, custom editors, new file items, commands, themes, file icons, configuration, and workflow metadata
- Executable command registry for active extensions using `contributions.commands` + `module.commandHandlers[commandId]`
- Executable slash command registry for active extensions using `contributions.slashCommands` + `module.slashCommandHandlers`
- Agent tab surfacing for `aiTools`, slash commands, `contributions.agentWorkflows`, and `contributions.claudePlugin`
- Tracker tab with workspace-local Nimbalyst-style tracker records stored in `.nimbalyst-tracker.json`
- Tracker schema loading from workspace `.nimbalyst/trackers/*.yaml` files with type-specific fields, roles, status options, and priority options
- Tracker frontmatter indexing for Markdown files with canonical `trackerStatus.type` or legacy `planStatus` / `decisionStatus` / `bugStatus` / `taskStatus` / `ideaStatus` / `automationStatus`
- Electron workspace watcher bridge for Tracker store, tracker schema, Markdown frontmatter, and Shared Docs refresh
- Shared Docs tab with workspace-local shared document records stored in `.nimbalyst-shared-docs.json`
- Panel toggle commands using Nimbalyst's `${extensionId}.${panelId}.toggle` convention
- Manifest keybinding listener for commands registered in the first tab
- Browser iframe host for panel contributions that export `panels[id].component`
- Browser iframe host for settings panel contributions that export `settingsPanel[component]`
- Browser iframe host for app-level host components that export `hostComponents[component]`
- Browser iframe host for document header contributions that export `components[component]`
- Browser iframe host for custom editor contributions that export `components[component]`
- File list and New File menu icons from `contributions.fileIcons` / `contributions.newFileMenu[].icon`
- Browser ESM shims for `react`, `react/jsx-runtime`, `react-dom/client`, `@nimbalyst/extension-sdk`, and `@nimbalyst/runtime`
- Extension theme selection persists locally as the first-tab active theme
- Iframe Electron IPC compatibility for the reference Git extension's status, branch list, log, working changes, file diff, commit detail, stage, unstage, commit, checkout, cherry-pick, fetch, pull, and push operations through Bat Rack's REST host
- Iframe Electron IPC compatibility for extension file reads, binary writes, and `file://` fetches used by image-generation custom editors

Filesystem paths are restricted to the Bat Rack workspace root by default.

## Not Supported Yet

- Lexical extension graph integration
- Full app-wide replacement of every hard-coded Bat Rack color with Nimbalyst `--nim-*` variables
- Full source/diff/collaboration behavior for Nimbalyst custom editors
- Utility-process backend module runtime
- Full Nimbalyst backend RPC streaming/cancellation contract
- Collab document adapters beyond no-op registration
- Full PGLite-backed tracker indexing and tracker room sync
- Encrypted collaborative document rooms and remote shared-document sync
- Electron IPC-only Nimbalyst plugins unless their IPC channels are adapted to the REST/server host

The SDK/runtime shims let AI-tool extensions import the Nimbalyst SDK package without bundling it.
Panel, document header, and custom editor components can now render through the iframe host.
App-level host components render through the floating host-component layer in the first tab.
The browser SDK shim includes a lightweight `useEditorLifecycle()` for local file editing.
Advanced renderer hooks such as `useCollaborativeEditor()` are compatibility no-ops because Bat Rack
does not host Nimbalyst's encrypted collaboration services yet.

## API

```http
GET  /api/nimbalyst/extensions
GET  /api/nimbalyst/marketplace/extensions
POST /api/nimbalyst/marketplace/extensions/:id/install
POST /api/nimbalyst/marketplace/extensions/:id/build-install
POST /api/nimbalyst/marketplace/extensions/build-install-all
POST /api/nimbalyst/extensions/:id/activate
POST /api/nimbalyst/extensions/:id/deactivate
POST /api/nimbalyst/extensions/:id/reload
GET  /api/nimbalyst/tools
GET  /api/nimbalyst/commands
GET  /api/nimbalyst/slash-commands
GET  /api/nimbalyst/contributions
GET  /api/nimbalyst/extensions/:id/assets/*
GET  /api/nimbalyst/extensions/:id/module
GET  /api/nimbalyst/extensions/:id/configuration
PUT  /api/nimbalyst/extensions/:id/configuration
GET  /api/nimbalyst/extensions/:id/backend-modules
PUT  /api/nimbalyst/extensions/:id/backend-modules/:moduleId/grant
POST /api/nimbalyst/extensions/:id/backend-modules/:moduleId/execute
GET  /api/nimbalyst/browser-shims/:name
POST /api/nimbalyst/tools/:name/execute
POST /api/nimbalyst/commands/:id/execute
POST /api/nimbalyst/slash-commands/:id/execute
POST /api/nimbalyst/host/exec
POST /api/nimbalyst/host/git
POST /api/nimbalyst/host/file/read
POST /api/nimbalyst/host/file/write-binary
GET  /api/nimbalyst/reference/status
POST /api/nimbalyst/reference/install
POST /api/nimbalyst/reference/launch
POST /api/nimbalyst/reference/stop
```

Tool execute payload:

```json
{
  "args": {},
  "workspacePath": "C:/MIDAS/code/apiverification",
  "activeFilePath": "optional/current/file.py"
}
```

Backend module execute payload:

```json
{
  "method": "describe",
  "params": {},
  "workspacePath": "C:/MIDAS/code/apiverification"
}
```

## Sample Extension

The sample extension is in:

```text
extensions/api-schema-agent
```

Manual check:

```powershell
Invoke-RestMethod -Method Post `
  -Uri http://localhost:9527/api/nimbalyst/extensions/com.batrack.api-schema-agent/activate

Invoke-RestMethod `
  -Uri http://localhost:9527/api/nimbalyst/tools

Invoke-RestMethod -Method Post `
  -Uri http://localhost:9527/api/nimbalyst/tools/batrack.ping/execute `
  -ContentType application/json `
  -Body '{"args":{}}'
```

AI services use `OPENAI_API_KEY` and `OPENAI_MODEL` from the server environment.

## Reference Source Host

The source reference is expected at:

```text
docs/REFERENCE/nimbalyst
```

`/api/nimbalyst/reference/status` reports whether the reference repo exists, whether dependencies are installed, whether the current Node runtime satisfies the reference toolchain, and whether the Windows/macOS/Linux dev scripts are present.

`/api/nimbalyst/reference/install` runs `npm ci` in the reference repository and exposes recent install logs in the first tab. `/api/nimbalyst/reference/launch` starts the actual reference Electron dev app when dependencies are installed and the Node runtime is compatible.

Bundled source extensions require the reference dependencies before they can be built. In the first tab, run `Reference Source > Install` before using `Build & Install All Reference Extensions`.

Current limitation: the reference Electron main process is not embedded into Bat Rack yet. The workbench can launch and coordinate with it, but full Nimbalyst parity still requires porting or hosting the reference main-process IPC services inside Bat Rack.
