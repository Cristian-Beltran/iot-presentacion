import express from 'express'

const app = express()
const port = Number(process.env.PORT ?? 3000)
let latest = null

app.use(express.json({ limit: '8kb' }))
app.use((_, res, next) => {
  res.setHeader('Access-Control-Allow-Origin', '*') // Solo laboratorio local.
  next()
})

app.get('/', (_, res) => res.type('html').send(`<!doctype html><html lang="es"><meta http-equiv="refresh" content="3"><style>body{font:18px system-ui;max-width:700px;margin:50px auto;background:#071018;color:#e6f2f7}pre{padding:20px;background:#102532;border-radius:14px}</style><h1>Telemetría IoT</h1><pre>${JSON.stringify(latest, null, 2)}</pre></html>`))
app.get('/api/telemetry/latest', (_, res) => latest ? res.json(latest) : res.status(404).json({ error: 'no_data' }))
app.post('/api/telemetry', (req, res) => {
  const { deviceId, temperatureC, humidityPct, rssi } = req.body ?? {}
  if (typeof deviceId !== 'string' || typeof temperatureC !== 'number' || typeof humidityPct !== 'number') {
    return res.status(400).json({ error: 'invalid_payload' })
  }
  latest = { deviceId, temperatureC, humidityPct, rssi, receivedAt: new Date().toISOString() }
  console.log('telemetry', latest)
  res.status(201).json({ accepted: true, latest })
})

app.listen(port, '0.0.0.0', () => console.log(`API lista en http://0.0.0.0:${port}`))
