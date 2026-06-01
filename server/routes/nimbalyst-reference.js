import express from 'express';
import {
  getNimbalystReferenceStatus,
  installNimbalystReferenceDependencies,
  launchNimbalystReference,
  stopNimbalystReference,
} from '../lib/nimbalyst-reference.js';

const router = express.Router();

router.get('/reference/status', async (_req, res) => {
  try {
    res.json(await getNimbalystReferenceStatus());
  } catch (error) {
    res.status(500).json({ error: error.message });
  }
});

router.post('/reference/launch', async (_req, res) => {
  try {
    const result = await launchNimbalystReference();
    res.status(result.ok ? 200 : 409).json(result);
  } catch (error) {
    res.status(500).json({ ok: false, error: error.message });
  }
});

router.post('/reference/install', async (_req, res) => {
  try {
    const result = await installNimbalystReferenceDependencies();
    res.status(result.ok ? 200 : 409).json(result);
  } catch (error) {
    res.status(500).json({ ok: false, error: error.message });
  }
});

router.post('/reference/stop', async (_req, res) => {
  try {
    res.json(await stopNimbalystReference());
  } catch (error) {
    res.status(500).json({ ok: false, error: error.message });
  }
});

export default router;
