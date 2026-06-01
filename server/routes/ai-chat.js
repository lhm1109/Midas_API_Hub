import express from 'express';

const router = express.Router();

router.post('/stream', async (req, res) => {
  const { messages, systemPrompt } = req.body;

  const apiKey = process.env.OPENAI_API_KEY;
  if (!apiKey) {
    return res.status(500).json({ error: 'OPENAI_API_KEY가 설정되지 않았습니다.' });
  }

  const body = {
    model: process.env.OPENAI_MODEL || 'gpt-4o',
    stream: true,
    messages: [
      { role: 'system', content: systemPrompt || 'You are a helpful assistant.' },
      ...(messages || []),
    ],
  };

  try {
    const upstream = await fetch('https://api.openai.com/v1/chat/completions', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
        Authorization: `Bearer ${apiKey}`,
      },
      body: JSON.stringify(body),
    });

    if (!upstream.ok) {
      const err = await upstream.json().catch(() => ({ error: { message: upstream.statusText } }));
      return res.status(upstream.status).json({ error: err?.error?.message ?? 'OpenAI 오류' });
    }

    res.setHeader('Content-Type', 'text/event-stream');
    res.setHeader('Cache-Control', 'no-cache');
    res.setHeader('Connection', 'keep-alive');
    res.flushHeaders();

    const reader = upstream.body.getReader();
    const decoder = new TextDecoder();

    const cleanup = () => { try { reader.cancel(); } catch { /* ignore */ } };
    req.on('close', cleanup);

    while (true) {
      const { done, value } = await reader.read();
      if (done) break;
      const chunk = decoder.decode(value, { stream: true });
      res.write(chunk);
    }

    res.end();
  } catch (err) {
    if (!res.headersSent) {
      res.status(500).json({ error: err instanceof Error ? err.message : String(err) });
    }
  }
});

export default router;
