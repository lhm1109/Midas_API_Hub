import { parentPort, workerData } from 'worker_threads';

if (!parentPort) {
  throw new Error('nimbalyst backend module worker requires parentPort.');
}

function serializeError(error) {
  if (error instanceof Error) {
    return {
      name: error.name,
      message: error.message,
      stack: error.stack,
    };
  }
  return {
    name: 'Error',
    message: String(error),
  };
}

function createServices(runtimeContext) {
  const grantedPermissions = new Set(runtimeContext.grantedPermissions || []);
  return {
    workspacePath: runtimeContext.workspacePath || '',
    extensionPath: runtimeContext.extensionPath || '',
    log(level, message, data) {
      parentPort.postMessage({
        type: 'log',
        level,
        message,
        data,
      });
    },
    assertPermission(permissionId) {
      if (!grantedPermissions.has(permissionId)) {
        throw new Error(`Permission not granted: ${permissionId}`);
      }
    },
    hasPermission(permissionId) {
      return grantedPermissions.has(permissionId);
    },
  };
}

async function loadBackendApi() {
  const runtimeContext = workerData.runtimeContext || {};
  const services = createServices(runtimeContext);
  const imported = await import(workerData.moduleUrl);
  const activate = imported.activate || imported.default;

  if (typeof activate === 'function') {
    const api = (await activate({ runtimeContext, services })) || {};
    return {
      methods: api.methods || {},
      deactivate: api.deactivate,
      services,
    };
  }

  const methods = {};
  for (const [key, value] of Object.entries(imported)) {
    if (typeof value === 'function') {
      methods[key] = value;
    }
  }

  return {
    methods,
    deactivate: undefined,
    services,
  };
}

let apiPromise = loadBackendApi();

parentPort.on('message', async (message) => {
  if (!message || message.type !== 'execute') return;

  try {
    const api = await apiPromise;
    const method = api.methods[message.method];
    if (typeof method !== 'function') {
      throw new Error(`Backend module method not found: ${message.method}`);
    }

    const result = await method(message.params ?? {}, {
      services: api.services,
      signal: new AbortController().signal,
    });
    parentPort.postMessage({
      type: 'result',
      id: message.id,
      success: true,
      data: result,
    });
  } catch (error) {
    parentPort.postMessage({
      type: 'result',
      id: message.id,
      success: false,
      error: serializeError(error),
    });
  }
});
