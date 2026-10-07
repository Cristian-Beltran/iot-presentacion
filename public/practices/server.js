import express from 'express'

const app = express()
const port = Number(process.env.PORT ?? 3000)
let latest = null
let ledOn = false

app.use(express.json({ limit: '8kb' }))
app.use(express.static('public'))
app.use((_, res, next) => {
  res.setHeader('Access-Control-Allow-Origin', '*') // Solo laboratorio local.
  next()
})

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

// El navegador cambia la orden; el ESP32 la consulta periódicamente por HTTP.
app.post('/api/control/toggle', (_, res) => {
  ledOn = !ledOn
  res.json({ ledOn })
})
app.get('/api/control', (_, res) => res.json({ ledOn }))

app.listen(port, '0.0.0.0', () => console.log(`API lista en http://0.0.0.0:${port}`))
