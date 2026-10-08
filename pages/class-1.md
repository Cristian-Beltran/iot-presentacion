---
layout: cover
class: cover
routeAlias: inicio
---

<div class="eyebrow">Clase 1 · 120 minutos</div>

# Del circuito al <span class="accent">software</span>

<p class="subtitle">Cómo un ESP32 se conecta sin cables a páginas, servidores y personas.</p>

<div class="mt-10 flex gap-3"><span class="chip">Wi‑Fi</span><span class="chip green">HTTP</span><span class="chip amber">ESP32</span></div>

---

# ¿Por qué conectar un circuito?

<div class="iot-grid cols-3 mt-7"><div class="iot-card cyan"><div class="metric">👀</div><h3>Ver</h3><p>Mirar datos desde una página o celular.</p></div><div class="iot-card green"><div class="metric">👆</div><h3>Controlar</h3><p>Enviar una orden sin tocar el circuito.</p></div><div class="iot-card amber"><div class="metric">🧾</div><h3>Guardar</h3><p>Crear un historial para entender lo que pasó.</p></div></div>

<p class="lead mt-8">Un circuito conectado deja de trabajar solo: puede <strong>informar y recibir órdenes</strong>.</p>

---

# Ejemplos que ya conoces

<div class="iot-grid cols-2 mt-5"><div class="media-panel"><img src="https://images.unsplash.com/photo-1558002038-1055907df827?auto=format&fit=crop&w=1000&q=80" alt="Casa inteligente"><span class="media-label">Casa: luces y sensores desde el celular</span></div><div class="media-panel"><img src="https://images.unsplash.com/photo-1581092160607-ee22621dd758?auto=format&fit=crop&w=1000&q=80" alt="Industria"><span class="media-label">Fábrica: datos y máquinas en una pantalla</span></div></div>

---

# La idea completa en un dibujo

<div class="network-diagram four-way"><div class="device sensor"><i>◌</i><b>Sensor / LED</b><small>mide o actúa</small></div><div class="diagram-link">datos<br><em>GPIO</em></div><div class="device board"><i>ESP</i><b>ESP32</b><small>lee y decide</small></div><div class="diagram-link bidirectional">Wi‑Fi<br><em>sin cables</em></div><div class="device laptop"><i>⌘</i><b>Software / web</b><small>muestra y controla</small></div></div>

<p class="lead mt-8">Hoy aprenderemos a construir ese puente paso a paso.</p>

---
layout: center
class: section-slide
---

<div class="eyebrow">Bloque 1</div>

# Conectar sin cables

<p>El ESP32 trae Wi‑Fi integrado: puede entrar a una red o crear la suya.</p>

<div class="section-number">01</div>

---

# ESP32: pequeño, programable y con Wi‑Fi

<div class="iot-grid cols-2 mt-5"><div class="media-panel"><img src="https://arduino-projekte.info/cdn/shop/files/NodeMCU-ESP32-38Pin-Front.webp?v=1734034634&width=1600" alt="Placa ESP32"><span class="media-label">ESP32: pines + programa + Wi‑Fi</span></div><div class="iot-grid cols-2"><div class="iot-card cyan"><h3>Pines</h3><p>Conecta sensores y actuadores.</p></div><div class="iot-card green"><h3>Programa</h3><p>Lee, decide y responde.</p></div><div class="iot-card blue"><h3>Wi‑Fi</h3><p>Habla con software.</p></div><div class="iot-card amber"><h3>Precio</h3><p>Ideal para aprender y prototipar.</p></div></div></div>

---

# Tres formas simples de comunicar

<div class="diagram-panel"><img src="/images/esp32-three-wireless-options.png?v=2" alt="ESP32 comunicado por Wi-Fi, Bluetooth y ESP-NOW"><span>Wi‑Fi: router y laptop · Bluetooth: celular cercano · ESP‑NOW: otro ESP32 directo.</span></div>

<p class="lead mt-8">Nos enfocaremos en <strong>Wi‑Fi</strong> porque permite unir hardware con aplicaciones web.</p>

---

# ESP32 como visitante de una red

<div class="network-diagram"><div class="device board"><i>ESP</i><b>ESP32</b><small>se une al Wi‑Fi</small></div><div class="diagram-link">SSID + clave<br><em>Wi‑Fi</em></div><div class="device router"><i>⌁</i><b>Router</b><small>entrega una IP</small></div><div class="diagram-link">red local<br><em>192.168.x.x</em></div><div class="device laptop"><i>⌘</i><b>Laptop / celular</b><small>abre la IP</small></div></div>

<div class="callout mt-8"><p><strong>Modo estación:</strong> el ESP32 entra al Wi‑Fi existente, igual que tu celular.</p></div>

---

# ESP32 como su propia red

<div class="network-diagram"><div class="device laptop"><i>⌘</i><b>Celular / laptop</b><small>elige red ESP32-Setup</small></div><div class="diagram-link bidirectional">Wi‑Fi propio<br><em>sin router</em></div><div class="device board"><i>ESP</i><b>ESP32</b><small>crea el punto de acceso</small></div><div class="diagram-link">GPIO</div><div class="device sensor"><i>◉</i><b>Circuito</b><small>LED o sensor</small></div></div>

<p class="lead mt-8"><strong>Modo punto de acceso:</strong> útil cuando no hay router o para configurar el dispositivo por primera vez.</p>

---

# Conectarse es obtener una dirección

<div class="network-diagram"><div class="device board"><i>ESP</i><b>1. Nombre y clave</b><small>elige la red</small></div><div class="diagram-link">se une<br><em>Wi‑Fi</em></div><div class="device router"><i>⌁</i><b>2. Router</b><small>entrega una IP</small></div><div class="diagram-link">ejemplo<br><em>192.168.1.45</em></div><div class="device laptop"><i>⌘</i><b>3. Tu navegador</b><small>visita esa IP</small></div></div>

<p class="lead mt-7">El programa completo que hace esto aparece en la práctica HTTP: cambia solo <code>TU_WIFI</code> y <code>TU_CLAVE</code>.</p>

---
layout: center
class: section-slide
---

<div class="eyebrow">Bloque 2</div>

# HTTP: hablar directo con una web

<p>Una aplicación pide algo; el otro equipo responde.</p>

<div class="section-number">02</div>

---

# HTTP en una imagen

<div class="network-diagram"><div class="device laptop"><i>⌘</i><b>Cliente: navegador</b><small>GET /api/status</small></div><div class="diagram-link bidirectional">petición ↔ respuesta<br><em>HTTP</em></div><div class="device board"><i>ESP</i><b>Servidor: ESP32</b><small>envía datos o página</small></div></div>

<div class="iot-grid cols-2 mt-7"><div class="iot-card cyan"><h3>GET</h3><p>Leer: ver temperatura o estado.</p></div><div class="iot-card green"><h3>POST</h3><p>Actuar: encender, apagar o enviar datos.</p></div></div>

---

# Tú eres el cliente

<div class="iot-grid cols-2 mt-5"><div class="media-panel"><img src="/images/http-client-server.png" alt="Diagrama local de petición y respuesta HTTP"><span class="media-label">Cliente pide → servidor responde</span></div><div class="v-center"><p class="lead">Cuando escribes una URL o presionas un botón, tu navegador actúa como <strong>cliente</strong>.</p><div class="callout mt-6"><p>El servidor espera solicitudes; el cliente siempre inicia cada conversación HTTP.</p></div></div></div>

---

# ESP32 como servidor web

<div class="network-diagram"><div class="device laptop"><i>⌘</i><b>1. Abres la IP</b><small>192.168.1.45</small></div><div class="diagram-link">GET /<br><em>HTTP</em></div><div class="device board"><i>ESP</i><b>2. ESP32</b><small>entrega la página</small></div><div class="diagram-link">POST /api/led<br><em>orden</em></div><div class="device sensor"><i>◉</i><b>3. LED</b><small>enciende / apaga</small></div></div>

<div class="big-code mt-6">

```text
http://192.168.1.45/api/status
```

</div>

---

# Qué hace el programa del ESP32

<div class="big-code">

```cpp
server.on("/api/status", []() {
  server.send(200, "application/json",
    "{\"ledOn\":false}");
});

server.begin();
```

</div>

<p class="lead mt-6">La ruta es una puerta: cuando alguien la visita, el ESP32 prepara una respuesta.</p>

<p class="small muted">No es código para copiar aislado: el programa entero está en la siguiente diapositiva y en la descarga.</p>

---

# Copia y pega · ESP32 servidor web completo

<div class="full-source">

<<< @/snippets/esp32-http-server.cpp

</div>

---

# Práctica 1 · página para un circuito

<div class="iot-grid cols-2 mt-5"><div class="terminal"><span class="prompt">Instala una vez</span><br>1. Arduino IDE<br>2. Gestor de placas: <b>esp32 by Espressif Systems</b><br><br><span class="prompt">No instales Node, npm ni bibliotecas de sensor.</span><br>El ESP32 es el servidor web y simula los datos.</div><div class="iot-card cyan"><h3>Conexión y prueba</h3><p><b>Sin DHT:</b> el programa genera temperatura y humedad aleatorias para practicar.<br><br><b>LED:</b> usa el LED integrado del GPIO 2.<br><br><b>Navegador:</b> abre <code>http://IP_DEL_ESP32</code> que aparece en el monitor serie.</p></div></div>

<p class="lead mt-6">Copia el programa del ESP32, cambia Wi‑Fi, súbelo y abre la IP del ESP32. La página ya viaja dentro del programa.</p>

---

# Práctica 2 · ESP32 cliente + servidor en PC

<div class="network-diagram"><div class="device sensor"><i>◌</i><b>Sensor</b><small>mide un valor</small></div><div class="diagram-link">GPIO<br><em>dato</em></div><div class="device board"><i>ESP</i><b>ESP32 cliente</b><small>prepara JSON</small></div><div class="diagram-link">POST<br><em>HTTP</em></div><div class="device server"><i>▣</i><b>Servidor</b><small>guarda y responde</small></div></div>

<p class="lead mt-7">Ahora el ESP32 inicia la conversación para enviar datos a un sistema.</p>

---

# Qué hace el cliente ESP32

<div class="big-code">

```cpp
HTTPClient http;
http.begin("http://IP_DEL_PC:3000/api/telemetry");
http.addHeader("Content-Type", "application/json");
http.POST("{\"temperatureC\":24.6}");
http.end();
```

</div>

<p class="lead mt-6">En este caso el ESP32 es el <strong>cliente</strong>; el programa Node o Python es el servidor.</p>

<p class="small muted">El código completo de esta práctica se muestra después de las instrucciones de instalación.</p>

---

# Una orden y una respuesta pequeñas

<div class="iot-grid cols-2 mt-5"><div class="big-code">

```http
POST /api/telemetry
{"temperatureC":24.6}
```

</div><div class="big-code">

```json
{"accepted": true}
```

</div></div>

---

# Servidores para la práctica

<div class="network-diagram"><div class="device laptop"><i>JS</i><b>Node + Express</b><small>JavaScript en la PC</small></div><div class="diagram-link">o<br><em>elige uno</em></div><div class="device server"><i>Py</i><b>Python + Flask</b><small>Python en la PC</small></div></div>

<p class="lead mt-8">No importa el lenguaje: ambos reciben el dato y devuelven una respuesta.</p>

---

# Objetivo de la práctica 2 · telemetría HTTP + botón LED

<div class="practice-diagram"><div class="device board"><i>ESP</i><b>ESP32</b><small>número aleatorio<br>cada 5 segundos</small></div><div class="diagram-link">POST<br><em>telemetría</em></div><div class="device server"><i>▣</i><b>Servidor</b><small>Node o Python</small></div><div class="diagram-link bidirectional">página web<br><em>botón + datos</em></div><div class="device laptop"><i>⌘</i><b>Navegador</b><small>ve el número<br>enciende el LED</small></div></div>

---

# Código de la práctica · datos aleatorios

<div class="big-code">

```cpp
int fakeTemperature = random(18, 32);
String json = "{\"temperatureC\":" + String(fakeTemperature) + "}";
// Enviar json con HTTP POST cada 5 segundos.
```

</div>

<div class="callout mt-6"><p><strong>Meta:</strong> ver números nuevos en el servidor y encender/apagar el LED desde la página local.</p></div>

---

# Copia y pega · ESP32 completo

<div class="full-source">

<<< @/snippets/esp32-http-random-sensor.cpp

</div>

---

# Opción A · instalar y levantar Node + Express

<div class="iot-grid cols-2 mt-4"><div class="terminal"><span class="prompt">1.</span> Instala Node.js LTS<br><span class="prompt">2.</span> cd examples/http-node<br><span class="prompt">3.</span> npm init<br><span class="prompt">4.</span> npm pkg set type=module<br><span class="prompt">5.</span> npm install express<br><span class="prompt">6.</span> node server.js<br><br>Servidor: http://IP_DEL_PC:3000</div><div class="big-code">

```js
app.post('/api/telemetry', (req, res) => {
  console.log(req.body)
  res.json({ accepted: true })
})
```

</div></div>

<p class="small muted mt-4"><code>npm install express</code> descarga exactamente el paquete <strong>Express</strong>, que recibe los datos del ESP32 y entrega la página web.</p>

---

# Copia y pega · servidor Node completo

<div class="full-source">

```js
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

app.post('/api/control/toggle', (_, res) => {
  ledOn = !ledOn
  res.json({ ledOn })
})
app.get('/api/control', (_, res) => res.json({ ledOn }))

app.listen(port, '0.0.0.0', () => console.log(`API lista en http://0.0.0.0:${port}`))
```

</div>

---

# Opción B · instalar y levantar Python + Flask

<div class="iot-grid cols-2 mt-4"><div class="terminal"><span class="prompt">1.</span> Instala Python 3<br><span class="prompt">2.</span> cd examples/http-python<br><span class="prompt">3.</span> pip install flask<br><span class="prompt">4.</span> flask --app app run --host 0.0.0.0<br><br>Servidor: http://IP_DEL_PC:5000</div><div class="big-code">

```python
@app.post("/api/telemetry")
def telemetry():
    print(request.get_json())
    return {"accepted": True}
```

</div></div>

<p class="small muted mt-4">Flask entrega la página de telemetría y control en <code>http://IP_DEL_PC:5000</code>.</p>

---

# Copia y pega · servidor Flask completo

<div class="full-source">

<<< @/examples/http-python/app.py

</div>

---

# Página HTML · telemetría y control

<p class="small muted">Con Node o Flask activo, abre su puerto: <code>:3000</code> para Node o <code>:5000</code> para Flask.</p>

<div class="full-source">

```html
<!doctype html>
<html lang="es"><meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1">
<title>Telemetría HTTP</title>
<style>body{font:18px system-ui;max-width:700px;margin:40px auto;padding:0 18px;background:#071018;color:#e6f2f7}pre{padding:20px;background:#102532;border-radius:14px}button{padding:12px 18px;border-radius:9px;border:1px solid #5eead4;background:#164e50;color:white;font-size:18px;cursor:pointer}</style>
<h1>Telemetría HTTP</h1><p>Estado LED: <b id="led">--</b></p><button onclick="toggle()">Cambiar LED</button><h2>Último dato del ESP32</h2><pre id="data">Esperando datos…</pre>
<script>
async function refresh(){try{const r=await fetch('/api/telemetry/latest');data.textContent=JSON.stringify(await r.json(),null,2);const c=await fetch('/api/control');led.textContent=(await c.json()).ledOn?'ENCENDIDO':'APAGADO'}catch{data.textContent='El ESP32 aún no envió datos'}}
async function toggle(){await fetch('/api/control/toggle',{method:'POST'});refresh()}
refresh();setInterval(refresh,3000)
</script></html>
```

</div>

---

# Así se ve la página de la práctica 2

<div class="browser-preview"><div class="browser-bar"><span></span><span></span><span></span><b>Telemetría HTTP</b></div><div class="browser-content"><div><h2>Telemetría HTTP</h2><p>Estado LED: <b style="color:#0f766e">ENCENDIDO</b></p><button>Cambiar LED</button></div><div class="arm-values"><b>Temperatura</b><strong>24.6 °C</strong><b>Humedad</b><strong>51 %</strong><b>ESP32</b><strong>esp32-01</strong></div></div></div>

<p class="lead mt-5">La página consulta el último JSON que mandó el ESP32 y envía una orden HTTP al presionar el botón.</p>

---

# Si falla, prueba por capas

<div class="visual-flow"><div class="visual-node">1 · Circuito<span>¿funciona local?</span></div><div class="visual-arrow">→</div><div class="visual-node">2 · Wi‑Fi<span>¿hay IP?</span></div><div class="visual-arrow">→</div><div class="visual-node">3 · Ruta<span>¿abre en navegador?</span></div><div class="visual-arrow">→</div><div class="visual-node">4 · Datos<span>¿llega JSON?</span></div></div>

---

# Cierre de la clase 1

<div class="iot-grid cols-3 mt-7"><div class="iot-card cyan"><h3>Wi‑Fi</h3><p>Conecta el ESP32 a otros equipos.</p></div><div class="iot-card green"><h3>Servidor</h3><p>El ESP32 puede mostrar una página.</p></div><div class="iot-card amber"><h3>Cliente</h3><p>También puede enviar datos a un sistema.</p></div></div>

<p class="lead mt-8">En la clase 2 usaremos MQTT cuando más de una aplicación necesita hablar con el dispositivo.</p>

---

# Tarea 1 · ESP32 servidor con DHT real

<div class="iot-grid cols-2 mt-6"><div class="iot-card cyan"><h3>Repite la práctica 1</h3><p>Conserva el ESP32 como servidor web: debe entregar su propia página al abrir la IP desde el navegador.</p></div><div class="iot-card green"><h3>Usa un DHT real</h3><p>Reemplaza los valores simulados por la temperatura y humedad medidas por tu sensor DHT.</p></div></div>

<div class="callout mt-8"><p><strong>Entrega:</strong> evidencia de la página del ESP32 mostrando datos reales del DHT y el programa que lee el sensor y sirve la página web.</p></div>
