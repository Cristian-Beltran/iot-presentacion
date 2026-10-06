---
layout: cover
class: cover
routeAlias: clase-2
---

<div class="eyebrow">Clase 2 · 120 minutos</div>

# IoT en tiempo real con <span class="accent">MQTT</span>

<p class="subtitle">Broker, publicación/suscripción, WebSocket, seguridad y arquitecturas aplicadas.</p>

<div class="mt-10 flex gap-3"><span class="chip">MQTT</span><span class="chip green">WebSocket</span><span class="chip amber">Broker</span><span class="chip">ESP32</span></div>

<!--
Tiempo: 2 min. Presente el objetivo: convertir el prototipo HTTP en un sistema desacoplado y observable. La demo final controla un LED y recibe DHT desde esta misma presentación.
-->

---

# Activación · ¿qué recuerdas?

<div class="iot-grid cols-2 mt-6">
  <div class="iot-card"><h3>¿Wi‑Fi es HTTP?</h3><p v-click>No. Wi‑Fi conecta a la red; HTTP define mensajes de aplicación.</p></div>
  <div class="iot-card"><h3>¿Quién inicia HTTP?</h3><p v-click>El cliente envía una petición; el servidor responde.</p></div>
  <div class="iot-card"><h3>¿ESP32 puede ser ambos?</h3><p v-click>Sí: cliente de una API y servidor dentro de la LAN.</p></div>
  <div class="iot-card"><h3>¿Qué falta para escalar?</h3><p v-click>Desacoplar, enrutar eventos y manejar desconexiones.</p></div>
</div>

<!--
Tiempo: 5 min. No muestre las respuestas inmediatamente. Haga que distintos estudiantes argumenten. Corrija términos sin penalizar intuiciones correctas.
-->

---
layout: center
class: section-slide
---

<div class="eyebrow">Bloque 4</div>

# MQTT cambia la conversación

<p>Los clientes no necesitan conocerse; comparten mensajes mediante un broker.</p>

<div class="section-number">04</div>

<!-- Tiempo: 1 min. -->

---

# Publicar y suscribirse

<div class="flow">
  <div class="flow-step"><b>ESP32</b><small>publica telemetría</small></div><div class="flow-arrow">→</div>
  <div class="flow-step"><b>Broker</b><small>recibe y distribuye</small></div><div class="flow-arrow">→</div>
  <div class="flow-step"><b>Web · API · BD</b><small>se suscriben</small></div>
</div>

<div class="iot-grid cols-3 mt-8">
  <div class="iot-card cyan"><h3>Publisher</h3><p>Envía un mensaje a un tópico, sin elegir destinatarios.</p></div>
  <div class="iot-card green"><h3>Broker</h3><p>Autentica, recibe y enruta hacia suscriptores.</p></div>
  <div class="iot-card blue"><h3>Subscriber</h3><p>Declara qué tópicos le interesan y reacciona.</p></div>
</div>

<!--
Tiempo: 5 min. Use la analogía de una estación de radio con cuidado: MQTT también es bidireccional y los clientes pueden publicar y suscribirse simultáneamente.
-->

---

# El tópico es una dirección semántica

```text
sebastian/device/esp32-01/telemetry
sebastian/device/esp32-01/status
sebastian/device/esp32-01/control
sebastian/device/esp32-01/cmd/ack
sebastian/device/esp32-01/alerts
```

<div class="iot-grid cols-2 mt-5">
  <div class="iot-card"><h3>+ · un nivel</h3><p><code>sebastian/device/+/telemetry</code></p></div>
  <div class="iot-card"><h3># · todo lo restante</h3><p><code>sebastian/device/#</code></p></div>
</div>

<div class="callout warn mt-5"><p>El tópico enruta. El payload describe. Evita esconder datos dinámicos importantes dentro de nombres imposibles de autorizar.</p></div>

<!--
Tiempo: 5 min. Construya la jerarquía desde dominio → tipo → identificador → evento. Explique que los comodines son para suscripciones, no para publicar.
-->

---

# Un contrato de mensajes pequeño y claro

<div class="iot-grid cols-2 mt-4">
  <div>

```json
{
  "temperatureC": 24.6,
  "humidityPct": 51.2,
  "ledOn": true,
  "rssi": -58,
  "uptimeMs": 182033
}
```

  </div>
  <div>

```json
{
  "command": "SET_LED",
  "value": false,
  "requestId": "b4f1...",
  "timestamp": "2026-10-05T...Z"
}
```

  </div>
</div>

<p class="small muted mt-4">Incluye unidad en el nombre, versión cuando cambie el contrato y un identificador para correlacionar comando y ACK.</p>

<!--
Tiempo: 4 min. Compare temperature: 24 con temperatureC: 24. Explique que los consumidores deben poder distinguir versión, unidad y origen sin adivinar.
-->

---

# QoS: entrega, no magia

<div class="iot-grid cols-3 mt-6">
  <div class="iot-card"><div class="metric">0</div><div class="metric-label">como máximo una vez</div><p class="mt-4">Rápido. Puede perderse. Telemetría frecuente tolerante.</p></div>
  <div class="iot-card green"><div class="metric">1</div><div class="metric-label">al menos una vez</div><p class="mt-4">Puede duplicarse. Comandos y eventos idempotentes.</p></div>
  <div class="iot-card amber"><div class="metric">2</div><div class="metric-label">exactamente una vez*</div><p class="mt-4">Más intercambio y costo. Úsalo solo si se justifica.</p></div>
</div>

<p class="tiny mt-5">* En la entrega MQTT entre dos clientes y el broker; tu lógica de negocio aún debe manejar reintentos, reinicios y efectos duplicados.</p>

<!--
Tiempo: 5 min. Destaque que QoS 1 exige idempotencia. Encender el LED dos veces es seguro; cobrar dos veces o abrir dos accesos puede no serlo.
-->

---

# Retain, Last Will y presencia

<div class="iot-grid cols-3 mt-6">
  <div class="iot-card cyan"><h3>Retained</h3><p>El nuevo suscriptor recibe el último estado conocido inmediatamente.</p></div>
  <div class="iot-card red"><h3>Last Will</h3><p>El broker publica “offline” si el dispositivo desaparece sin cerrar bien.</p></div>
  <div class="iot-card green"><h3>Birth message</h3><p>Al conectar, el dispositivo publica “online” y sus metadatos.</p></div>
</div>

<div class="callout mt-7"><p>Patrón recomendado: <code>status</code> retained + LWT offline + mensaje online al conectar.</p></div>

<!--
Tiempo: 5 min. Pregunte qué vería un dashboard que abre después que el dispositivo ya publicó. Retain responde a esa necesidad; no reemplaza el historial.
-->

---

# Ciclo robusto del dispositivo

```text
arranque → Wi‑Fi → hora válida → broker → suscripciones → online
    ↓          ↓          ↓          ↓
 timeout     reintento    TLS      backoff + jitter
```

<div class="iot-grid cols-3 mt-6">
  <div class="iot-card"><h3>No bloquear</h3><p>El sensor y la lógica local deben seguir funcionando.</p></div>
  <div class="iot-card"><h3>Reintentar con calma</h3><p>Backoff evita saturar red y broker.</p></div>
  <div class="iot-card"><h3>Observar</h3><p>Estado, RSSI, uptime, versión y razón de reinicio.</p></div>
</div>

<!--
Tiempo: 4 min. Explique por qué un while(!connected) infinito puede inutilizar una cerradura o estación. El equipo debe degradarse de forma segura.
-->

---

# Código mínimo en ESP32

```cpp
client.setServer(MQTT_HOST, MQTT_PORT);
client.setCallback(onMessage);

client.subscribe("sebastian/device/esp32-01/control", 1);
client.publish("sebastian/device/esp32-01/status",
               "{\"online\":true}", true);
```

```cpp
if (millis() - lastTelemetry >= 5000) {
  publishTelemetry();
  lastTelemetry = millis();
}
client.loop();
```

<div class="callout warn mt-4"><p>No uses <code>delay(5000)</code> para planificar todo: impide atender mensajes y otras tareas a tiempo.</p></div>

<!--
Tiempo: 5 min. Muestre la diferencia entre setup y loop. El ejemplo completo incluye reconexión, DHT, JSON, ACK y control del LED.
-->

---
layout: center
class: section-slide
---

<div class="eyebrow">Bloque 4.5</div>

# MicroPython · iteración rápida

<p>Python en el microcontrolador: depura en vivo, sin compilar.</p>

<div class="section-number">04.5</div>

<!-- Tiempo: 1 min. Cambie de ritmo: de C++ a Python. Muestre que el mismo hardware puede programarse de otra manera. -->

---

# ¿Qué es MicroPython?

<div class="iot-grid cols-2 mt-5">
  <div class="iot-card cyan">
    <h3>Python 3 en el MCU</h3>
    <p>Subconjunto de Python 3 optimizado para microcontroladores. REPL interactivo, import en caliente.</p>
  </div>
  <div class="iot-card green">
    <h3>Soporte ESP32/ESP8266</h3>
    <p>Firmware oficial de Espressif. Acceso a GPIO, WiFi, I²C, SPI, ADC, PWM, BLE.</p>
  </div>
</div>

<div class="iot-grid cols-3 mt-6">
  <div class="iot-card"><h3>Ventajas</h3><p>Iteración rápida, sin compilar, REPL, fácil de leer.</p></div>
  <div class="iot-card"><h3>Limitaciones</h3><p>Mayor uso de RAM, menos librerías, rendimiento inferior a C++.</p></div>
  <div class="iot-card"><h3>Ideal para</h3><p>Prototipos rápidos, educación, scripts de automatización.</p></div>
</div>

<!--
Tiempo: 3 min. No es reemplazo de Arduino C++, es complemento. Para producción con restricciones de memoria o rendimiento, C++ sigue siendo mejor opción.
-->

---

# Instalar MicroPython en ESP32

<div class="timeline mt-4">
  <div class="time">1</div><div class="event"><strong>Descargar firmware.</strong> <code>micropython.org</code> → ESP32 → archivo <code>.bin</code>.</div>
  <div class="time">2</div><div class="event"><strong>Instalar esptool.</strong> <code>pip install esptool</code></div>
  <div class="time">3</div><div class="event"><strong>Borrar flash.</strong> <code>esptool.py --chip esp32 --port /dev/ttyUSB0 erase_flash</code></div>
  <div class="time">4</div><div class="event"><strong>Flashear firmware.</strong> <code>esptool.py --chip esp32 --port /dev/ttyUSB0 --baud 460800 write_flash -z 0x1000 esp32-*.bin</code></div>
</div>

<div class="callout warn mt-5"><p>En Windows, el puerto es <code>COM3</code> o similar. En Linux, <code>/dev/ttyUSB0</code>. Verifica permisos del puerto con <code>ls -la /dev/ttyUSB*</code>.</p></div>

<!--
Tiempo: 5 min. Haga el proceso en vivo o tenga un ESP32 ya flasheado. Una vez flasheado, el ESP32 arranca en el REPL de MicroPython al conectarlo.
-->

---

# Thonny IDE

<div class="iot-grid cols-2 mt-5">
  <div class="iot-card">
    <h3>Entorno amigable</h3>
    <p>Editor con resaltado, REPL integrado, explorador de archivos del MCU y subida directa de scripts.</p>
    <div class="rule"></div>
    <p class="tiny"><strong>Descargar:</strong> <code>thonny.org</code>. Soporta MicroPython nativamente.</p>
  </div>
  <div class="image-placeholder">
    <div><strong>Captura opcional</strong><br><span class="tiny">public/images/thonny-window.webp</span></div>
  </div>
</div>

<div class="iot-grid cols-2 mt-5">
  <div class="iot-card"><h3>boot.py</h3><p>Se ejecuta al arrancar. Configuración WiFi y conexión inicial.</p></div>
  <div class="iot-card"><h3>main.py</h3><p>Lógica principal. Se ejecuta después de boot.py.</p></div>
</div>

<!--
Tiempo: 4 min. Abra Thonny, conecte al ESP32 y muestre el REPL. Escriba <code>import machine</code> y pruebe comandos en vivo.
-->

---

# Primer script en MicroPython

```python
import machine
import time

led = machine.Pin(2, machine.Pin.OUT)

while True:
    led.value(1)
    time.sleep(0.5)
    led.value(0)
    time.sleep(0.5)
```

```python
from machine import Pin, ADC

ldr = ADC(Pin(34))
ldr.atten(ADC.ATTN_11DB)

while True:
    print("LDR:", ldr.read())
    time.sleep(1)
```

<div class="callout mt-4"><p>Guárdelo como <code>main.py</code> en el ESP32 desde Thonny. Se ejecutará automáticamente al reiniciar.</p></div>

<!--
Tiempo: 5 min. Suba ambos scripts. Muestre que no hay compilación: el script se interpreta directamente. Esto acelera la iteración pero sacrifica rendimiento.
-->

---

# MQTT en MicroPython

```python
from umqtt.simple import MQTTClient
import json, machine, time

client = MQTTClient("esp32-mp-01", "broker.local",
                    port=1883, user="device", password="esp32")

def on_message(topic, msg):
    data = json.loads(msg)
    print("CMD:", data)

client.set_callback(on_message)
client.connect()
client.subscribe(b"sebastian/device/esp32-mp-01/control")

while True:
    client.check_msg()
    payload = json.dumps({"temperatureC": 24.5, "rssi": -60})
    client.publish(b"sebastian/device/esp32-mp-01/telemetry",
                   payload.encode())
    time.sleep(5)
```

<div class="callout mt-4"><p><strong>Código completo:</strong> <code>examples/esp32-micropython-mqtt/</code>. Incluye WiFi, LED, LDR y ACK.</p></div>

<!--
Tiempo: 6 min. Compare línea por línea con el código Arduino C++. Mismos conceptos (connect, subscribe, publish, callback), sintaxis diferente. Suba y pruebe con el broker del curso.
-->

---

# Arduino C++ vs MicroPython

<div class="iot-grid cols-2 mt-4">
  <div>

**Arduino C++**
```cpp
void setup() {
  WiFi.begin(SSID, PASS);
  while (WiFi.status() != WL_CONNECTED)
    delay(500);
  mqtt.setServer(HOST, 1883);
  mqtt.connect("esp32-01");
}
void loop() {
  mqtt.loop();
  if (millis() - last >= 5000) {
    mqtt.publish(topic, payload);
    last = millis();
  }
}
```

  </div>
  <div>

**MicroPython**
```python
wlan.connect(SSID, PASS)
while not wlan.isconnected():
    time.sleep(0.5)

client = MQTTClient("id", HOST, port=1883)
client.connect()

while True:
    client.check_msg()
    client.publish(topic, payload)
    time.sleep(5)
```

  </div>
</div>

<table class="compare mt-5">
  <thead><tr><th></th><th>Arduino C++</th><th>MicroPython</th></tr></thead>
  <tbody>
    <tr><td><strong>Compilación</strong></td><td>Sí</td><td>No (interpretado)</td></tr>
    <tr><td><strong>RAM</strong></td><td class="yes">Menor</td><td class="warn">Mayor</td></tr>
    <tr><td><strong>Velocidad</strong></td><td class="yes">Nativa</td><td class="warn">~10× más lento</td></tr>
    <tr><td><strong>Iteración</strong></td><td>Lenta</td><td class="yes">Rápida (REPL)</td></tr>
    <tr><td><strong>Librerías</strong></td><td class="yes">Amplio ecosistema</td><td>Más limitado</td></tr>
  </tbody>
</table>

<!--
Tiempo: 4 min. No es "cuál es mejor": depende del caso. Prototipo rápido → MicroPython. Producción con restricciones → C++. Muchos proyectos usan ambos: prototipo en MicroPython y migración a C++ para producto final.
-->

---
layout: center
class: section-slide
---

<div class="eyebrow">Bloque 5</div>

# ¿Dónde vive el broker?

<p>Local, en una Raspberry Pi, en tu servidor o como servicio administrado.</p>

<div class="section-number">05</div>

<!-- Tiempo: 1 min. -->

---

# Mosquitto local en minutos

<div class="terminal"><span class="prompt">$</span> cd examples/mosquitto<br><span class="prompt">$</span> docker compose up -d<br><span class="prompt">$</span> mosquitto_sub -h localhost -t 'course/#' -v<br><span class="prompt">$</span> mosquitto_pub -h localhost -t 'course/test' -m '{"ok":true}'</div>

<div class="iot-grid cols-3 mt-6">
  <div class="iot-card"><div class="metric">1883</div><div class="metric-label">MQTT TCP</div></div>
  <div class="iot-card"><div class="metric">9001</div><div class="metric-label">MQTT WebSocket</div></div>
  <div class="iot-card"><div class="metric">8883</div><div class="metric-label">MQTTS habitual</div></div>
</div>

<!--
Tiempo: 6 min. Levante el compose y pruebe dos terminales. Aclare que la configuración didáctica no es producción: producción requiere credenciales, ACL, TLS, persistencia y monitoreo.
-->

---

# Local, autogestionado o administrado

<table class="compare mt-5">
  <thead><tr><th>Opción</th><th>Ventaja</th><th>Responsabilidad</th><th>Cuándo elegir</th></tr></thead>
  <tbody>
    <tr><td>Mosquitto local</td><td>Simple y gratuito</td><td>Tu red y configuración</td><td>Aprender, prototipos, LAN</td></tr>
    <tr><td>Raspberry Pi/VPS</td><td>Control total</td><td>Actualizaciones, TLS, copias</td><td>Edge o proyecto propio</td></tr>
    <tr><td>EMQX/HiveMQ Cloud</td><td>Escala y operación</td><td>Costo y dependencia</td><td>Producción y equipos</td></tr>
    <tr><td>Arduino Cloud</td><td>Provisionamiento y dashboard</td><td>Modelo de la plataforma</td><td>Inicio rápido y educación</td></tr>
  </tbody>
</table>

<p class="tiny mt-4">Comprueba siempre límites, regiones, precios y planes gratuitos actuales en la documentación oficial antes de elegir.</p>

<!--
Tiempo: 6 min. No convierta la tabla en ranking universal. Compare costo total: tiempo de operación, seguridad, disponibilidad y salida de la plataforma.
-->

---

# Arduino Cloud: integración rápida

<div class="iot-grid cols-2 mt-6">
  <div class="iot-card cyan"><h3>Incluye</h3><p>Devices, Things, variables sincronizadas, dashboards, triggers, OTA y aplicaciones móviles.</p></div>
  <div class="iot-card green"><h3>Conecta</h3><p>ESP32/ESP8266, Arduino, Node‑RED y APIs para JavaScript y Python.</p></div>
</div>

<div class="callout mt-7"><p>Excelente para avanzar rápido. MQTT propio ofrece más libertad sobre tópicos, broker, datos y arquitectura.</p></div>

<p class="small mt-5"><a href="https://docs.arduino.cc/arduino-cloud">docs.arduino.cc/arduino-cloud</a></p>

<!--
Tiempo: 4 min. Muestre la idea Thing → variable → dashboard. Evite crear una cuenta en vivo si consume tiempo; deje el tutorial como práctica opcional.
-->

---

# Seguridad mínima viable

<div class="iot-grid cols-3 mt-5">
  <div class="iot-card"><h3>TLS</h3><p>MQTTS/WSS para confidencialidad e identidad del servidor.</p></div>
  <div class="iot-card"><h3>Identidad por dispositivo</h3><p>No compartir una clave global en producción.</p></div>
  <div class="iot-card"><h3>ACL por tópico</h3><p>Cada cliente publica y lee solo lo necesario.</p></div>
  <div class="iot-card"><h3>Validación</h3><p>Esquema, rangos, tamaño, versión y comandos permitidos.</p></div>
  <div class="iot-card"><h3>Actualizaciones</h3><p>Firmware firmado, OTA controlada y capacidad de revocar.</p></div>
  <div class="iot-card"><h3>Auditoría</h3><p>Quién ordenó qué, cuándo, a qué dispositivo y resultado.</p></div>
</div>

<!--
Tiempo: 6 min. Remarque que ocultar una contraseña dentro del firmware no la vuelve secreta. Las credenciales del curso son temporales y limitadas por ACL.
-->

---
layout: center
class: section-slide
---

<div class="eyebrow">Bloque 6</div>

# WebSocket no es MQTT

<p>Pero MQTT puede viajar sobre WebSocket para entrar al navegador.</p>

<div class="section-number">06</div>

<!-- Tiempo: 1 min. -->

---

# HTTP, WebSocket y MQTT

<table class="compare mt-5">
  <thead><tr><th></th><th>HTTP</th><th>WebSocket</th><th>MQTT</th></tr></thead>
  <tbody>
    <tr><td>Modelo</td><td>Petición/respuesta</td><td>Canal bidireccional</td><td>Publish/subscribe</td></tr>
    <tr><td>Intermediario</td><td>Servidor</td><td>Servidor</td><td>Broker</td></tr>
    <tr><td>Enrutamiento</td><td>Rutas</td><td>Lo diseña la app</td><td>Tópicos</td></tr>
    <tr><td>Entrega/QoS</td><td>Respuesta</td><td>Lo diseña la app</td><td>Parte del protocolo</td></tr>
    <tr><td>Navegador</td><td class="yes">Nativo</td><td class="yes">Nativo</td><td>Mediante WebSocket</td></tr>
  </tbody>
</table>

<div class="callout mt-6"><p><strong>MQTT sobre WSS</strong> conserva tópicos, QoS y broker; solo cambia el transporte usado por el cliente web.</p></div>

<!--
Tiempo: 6 min. Dibuje capas: MQTT → WebSocket → TLS → TCP. Socket.IO tampoco es WebSocket puro: agrega su propio protocolo y funciones.
-->

---

# Una web conectada al broker

<div class="flow">
  <div class="flow-step"><b>ESP32</b><small>MQTT/TLS</small></div><div class="flow-arrow">↔</div>
  <div class="flow-step"><b>Broker</b><small>TCP + WSS</small></div><div class="flow-arrow">↔</div>
  <div class="flow-step"><b>Navegador</b><small>MQTT.js sobre WSS</small></div>
</div>

```ts
const client = mqtt.connect('wss://broker.example', options)
client.subscribe('sebastian/device/esp32-01/telemetry')
client.publish(controlTopic, JSON.stringify(command), { qos: 1 })
```

<div class="callout warn mt-4"><p>Una app pública no debe llevar credenciales privilegiadas. La cuenta didáctica solo accede al namespace del laboratorio.</p></div>

<!--
Tiempo: 5 min. Explique por qué el navegador no usa normalmente MQTT TCP directo. Señale que el backend puede usar TCP mientras la web usa WSS contra el mismo broker.
-->

---

# Demo en vivo · panel MQTT

<MqttDashboard />

<!--
Tiempo: 12 min. Pulse Conectar; si no hay acceso al broker active Simulación. Observe telemetría, cambie el LED y revise el log. Desconecte temporalmente el dispositivo para enseñar Last Will/reconexión. Criterio: comando y ACK comparten requestId.
-->

---

# Arquitectura reutilizada del proyecto Sebastian

<div class="flow">
  <div class="flow-step"><b>Dispositivo</b><small>telemetry · status · alerts</small></div><div class="flow-arrow">→</div>
  <div class="flow-step"><b>Broker</b><small>WSS/TCP</small></div><div class="flow-arrow">→</div>
  <div class="flow-step"><b>Backend</b><small>validación · logs · BD</small></div><div class="flow-arrow">→</div>
  <div class="flow-step"><b>Frontend</b><small>monitoreo y control</small></div>
</div>

<div class="iot-grid cols-3 mt-7">
  <div class="iot-card"><h3>Comando</h3><p>Backend publica con QoS 1 y requestId.</p></div>
  <div class="iot-card"><h3>ACK</h3><p>ESP32 confirma ejecución o explica el rechazo.</p></div>
  <div class="iot-card"><h3>Salud</h3><p>Silencio, cierre y mensajes inválidos quedan registrados.</p></div>
</div>

<!--
Tiempo: 4 min. Explique que la demo simplificada respeta la misma familia de tópicos. En un sistema real, el backend autoriza acciones y conserva auditoría.
-->

---
layout: center
class: section-slide
---

<div class="eyebrow">Bloque 7</div>

# Del laboratorio a casos reales

<p>El patrón se mantiene; cambian sensores, reglas, riesgos y experiencia de usuario.</p>

<div class="section-number">07</div>

<!-- Tiempo: 1 min. -->

---

# Caso 1 · Molinete con RFID

<div class="flow">
  <div class="flow-step"><b>Tarjeta</b><small>UID/credencial</small></div><div class="flow-arrow">→</div>
  <div class="flow-step"><b>ESP32</b><small>lee + valida formato</small></div><div class="flow-arrow">→</div>
  <div class="flow-step"><b>Backend</b><small>autoriza + registra</small></div><div class="flow-arrow">→</div>
  <div class="flow-step"><b>Molinete</b><small>abre + sensor confirma</small></div>
</div>

<div class="iot-grid cols-3 mt-7">
  <div class="iot-card"><h3>Entrada/salida</h3><p>El sentido debe venir del molinete o del punto de acceso.</p></div>
  <div class="iot-card"><h3>Antipassback</h3><p>Evita dos entradas consecutivas con la misma credencial.</p></div>
  <div class="iot-card"><h3>Offline</h3><p>Lista local limitada y sincronización posterior.</p></div>
</div>

<!--
Tiempo: 4 min. Aclare que UID de tarjeta no siempre equivale a credencial segura. El evento debe incluir dispositivo, dirección, hora confiable, decisión y correlación.
-->

---

# Caso 2 · Acceso residencial con huella

<div class="iot-grid cols-2 mt-5">
  <div class="iot-card cyan"><h3>Enrolamiento</h3><p>Proceso administrativo autenticado; asigna un ID local al residente.</p></div>
  <div class="iot-card green"><h3>Verificación local</h3><p>El sensor compara plantilla y devuelve identificador/confianza.</p></div>
  <div class="iot-card blue"><h3>Autorización</h3><p>Reglas por puerta, horario, vigencia y estado del sistema.</p></div>
  <div class="iot-card amber"><h3>Accionamiento seguro</h3><p>Relé/controlador, sensor de puerta, timeout y salida de emergencia.</p></div>
</div>

<div class="callout danger mt-5"><p>No publiques plantillas biométricas por MQTT. Minimiza datos, cifra, controla enrolamiento y define revocación.</p></div>

<!--
Tiempo: 4 min. Diferencie identificación biométrica de autorización de acceso. Explique comportamiento seguro ante caída de Internet y normativa aplicable a datos biométricos.
-->

---

# Caso 3 · Hogar inteligente

<div class="iot-grid cols-3 mt-5">
  <div class="iot-card"><h3>Nodos ESP32</h3><p>Movimiento, temperatura, consumo, luces, persianas y riego.</p></div>
  <div class="iot-card"><h3>Broker local</h3><p>Continúa operando aunque Internet se caiga.</p></div>
  <div class="iot-card"><h3>Automatización</h3><p>Home Assistant o Node‑RED convierte eventos en reglas.</p></div>
</div>

<div class="flow mt-7"><div class="flow-step"><b>Sensor</b><small>ocupación</small></div><div class="flow-arrow">→</div><div class="flow-step"><b>Regla local</b><small>hora + luz</small></div><div class="flow-arrow">→</div><div class="flow-step"><b>Actuador</b><small>lámpara</small></div><div class="flow-arrow">→</div><div class="flow-step"><b>App</b><small>estado y override</small></div></div>

<!--
Tiempo: 3 min. Destaque “local first”: las funciones esenciales del hogar no deberían depender de una nube remota. La nube añade acceso y análisis, no necesariamente la decisión básica.
-->

---

# Caso 4 · Estación meteorológica

<div class="iot-grid cols-4 mt-5">
  <div class="iot-card"><h3>Temperatura/humedad</h3><p>DHT para aula; BME280 para mejor calidad.</p></div>
  <div class="iot-card"><h3>Lluvia y viento</h3><p>Pulsos, rebote, calibración y exposición.</p></div>
  <div class="iot-card"><h3>Energía</h3><p>Deep sleep, batería, solar y presupuesto energético.</p></div>
  <div class="iot-card"><h3>Datos</h3><p>Marca temporal, calidad, buffer offline e histórico.</p></div>
</div>

<p class="lead mt-7">Publicar más datos no siempre es mejor: define frecuencia según cuánto cambia la variable y cuánto cuesta transmitir.</p>

<!--
Tiempo: 3 min. Compare DHT11 de aula con un sensor calibrado. Introduzca la diferencia entre demostración, prototipo y sistema de medición confiable.
-->

---

# Caso 5 · Invernadero automatizado

<div class="iot-grid cols-4 mt-5">
  <div class="iot-card cyan"><h3>Suelo</h3><p>Sensor de humedad capacitivo. Umbral de riego configurable.</p></div>
  <div class="iot-card green"><h3>Ambiente</h3><p>BME280: temperatura, humedad, presión. Ventilación automática.</p></div>
  <div class="iot-card amber"><h3>Actuadores</h3><p>Relé para bomba de agua, servos para ventanas, LED grow light.</p></div>
  <div class="iot-card blue"><h3>Reglas</h3><p>Node‑RED o firmware local: horario, umbrales, prioridad.</p></div>
</div>

<div class="flow mt-6">
  <div class="flow-step"><b>Sensores</b><small>suelo + aire</small></div><div class="flow-arrow">→</div>
  <div class="flow-step"><b>ESP32</b><small>decide + actúa</small></div><div class="flow-arrow">→</div>
  <div class="flow-step"><b>Broker</b><small>telemetría</small></div><div class="flow-arrow">→</div>
  <div class="flow-step"><b>Dashboard</b><small>histórico</small></div>
</div>

<div class="callout mt-5"><p><strong>Clave:</strong> las decisiones críticas (regar, ventilar) deben poder ejecutarse localmente aunque Internet se caiga.</p></div>

<!--
Tiempo: 3 min. Compare con la estación meteorológica: aquí hay actuadores que afectan el ambiente. El sistema debe tomar decisiones autónomas y reportar lo que hizo.
-->

---

# Caso 6 · Control de acceso RFID

<div class="iot-grid cols-2 mt-5">
  <div class="iot-card">
    <h3>MFRC522 · SPI</h3>
    <p>Lee tarjetas RFID 13.56 MHz (MIFARE). UID de 4 bytes. Comunicación SPI a 3.3 V.</p>
    <div class="rule"></div>
    <p class="tiny"><strong>Pines:</strong> SDA→GPIO 5, SCK→GPIO 18, MOSI→GPIO 23, MISO→GPIO 19, RST→GPIO 22, GND.</p>
  </div>
  <div>

```cpp
#include <MFRC522.h>
MFRC522 mfrc(SS_PIN, RST_PIN);

// Leer UID
for (byte i = 0; i < mfrc.uid.size; i++) {
  uid += String(mfrc.uid.uidByte[i], HEX);
}
mqtt.publish(accessTopic, uid);
```

  </div>
</div>

<div class="callout warn mt-4"><p>El UID no es suficiente para seguridad real. Para acceso real, combine con encriptación MIFARE DESFire o un backend que autorice la credencial.</p></div>

<div class="callout mt-3"><p><strong>Código completo:</strong> <code>examples/esp32-rfid-mqtt/</code></p></div>

<div class="image-placeholder mt-4">
  <div><strong>Fotografía opcional</strong><br><span class="tiny">public/images/rfid-mfrc522.webp</span></div>
</div>

<!--
Tiempo: 4 min. Muestre el módulo RFID y una tarjeta. Lea el UID en vivo. Explique que el ESP32 envía el UID al broker y el backend decide si autorizar. El firmware no debe contener la lista de UIDs autorizados si hay muchos usuarios.
-->

---

# Caso 7 · Monitor de calidad de aire

<div class="iot-grid cols-3 mt-5">
  <div class="iot-card red"><h3>MQ‑2 / MQ‑135</h3><p>Gas combustible, humo, CO₂. Analógico. Precalentamiento 24 h.</p></div>
  <div class="iot-card green"><h3>BME280</h3><p>Temperatura, humedad, presión. I²C. Calidad de medición superior al DHT.</p></div>
  <div class="iot-card blue"><h3>PMS5003</h3><p>Partículas PM2.5/PM10. UART. Estación profesional.</p></div>
</div>

<div class="flow mt-7">
  <div class="flow-step"><b>MQ‑2</b><small>gas/smoke</small></div><div class="flow-arrow">→</div>
  <div class="flow-step"><b>ESP32</b><small>ADC + UART</small></div><div class="flow-arrow">→</div>
  <div class="flow-step"><b>MQTT</b><small>telemetría</small></div><div class="flow-arrow">→</div>
  <div class="flow-step"><b>Alerta</b><small>si > umbral</small></div>
</div>

<div class="callout danger mt-5"><p>Los sensores MQ se calibran en aire limpio. El valor ADC es relativo. Para ppm reales se necesita curva de calibración del datasheet.</p></div>

<!--
Tiempo: 3 min. Explique que el MQ-2 necesita 24 horas de precalentamiento continuo antes de dar lecturas estables. Es barato pero no preciso. Para calidad de aire profesional usar PMS5003 o Sensirion.
-->

---

# Ejercicio final · sistema de seguridad básico

<div class="timeline mt-4">
  <div class="time">0–5 min</div><div class="event"><strong>Diseñar.</strong> Diagrama: PIR + RFID + buzzer + LED + MQTT.</div>
  <div class="time">5–10 min</div><div class="event"><strong>Cablear.</strong> PIR (GPIO 27), RFID (SPI), buzzer (GPIO 25), LED (GPIO 2).</div>
  <div class="time">10–20 min</div><div class="event"><strong>Programar.</strong> PIR detecta → alerta MQTT. RFID autoriza → buzzer silencio.</div>
  <div class="time">20–25 min</div><div class="event"><strong>Integrar.</strong> Dashboard web que muestra estado y recibe comandos.</div>
</div>

<div class="iot-grid cols-3 mt-5">
  <div class="iot-card"><h3>PIR</h3><p>Movimiento → <code>alerts/motion</code> QoS 1</p></div>
  <div class="iot-card"><h3>RFID</h3><p>Tarjeta → <code>access/card</code> QoS 1</p></div>
  <div class="iot-card"><h3>Buzzer/LED</h3><p>Control remoto vía <code>control</code></p></div>
</div>

<div class="callout mt-5"><p><strong>Criterio:</strong> el sistema funciona aunque el WiFi se caiga (buzzer local), y reporta todo por MQTT cuando hay red.</p></div>

<!--
Tiempo: 25 min. Trabaje en equipos de 3-4 personas. Este ejercicio integra todo: hardware, firmware, MQTT, dashboard. No necesitan terminarlo en clase; lo importante es el diseño y la arquitectura.
-->

---

# Raspberry Pi como gateway IoT

<div class="flow">
  <div class="flow-step"><b>ESP32</b><small>sensores y tiempo real</small></div><div class="flow-arrow">→</div>
  <div class="flow-step"><b>Raspberry Pi</b><small>broker · Node‑RED · BD</small></div><div class="flow-arrow">→</div>
  <div class="flow-step"><b>Nube/App</b><small>acceso y analítica</small></div>
</div>

<div class="iot-grid cols-3 mt-7">
  <div class="iot-card cyan"><h3>Traduce</h3><p>BLE, serial, Modbus o ESP‑NOW hacia MQTT/HTTP.</p></div>
  <div class="iot-card green"><h3>Procesa</h3><p>Reglas, dashboards, base de datos y modelos.</p></div>
  <div class="iot-card blue"><h3>Resiste</h3><p>Opera localmente y sincroniza cuando vuelve Internet.</p></div>
</div>

<!--
Tiempo: 4 min. Cierre la comparación: ESP32 cerca del proceso físico; Raspberry Pi donde se necesita Linux, almacenamiento o servicios. Pueden trabajar juntos.
-->

---

# Checklist antes de construir

<div class="iot-grid cols-2 mt-5 small">
  <div class="iot-card"><h3>Problema</h3><p>¿Qué decisión mejora? ¿Quién usa el resultado?</p></div>
  <div class="iot-card"><h3>Entorno</h3><p>¿Energía, distancia, humedad, ruido, red?</p></div>
  <div class="iot-card"><h3>Datos</h3><p>¿Frecuencia, unidad, precisión, retención, privacidad?</p></div>
  <div class="iot-card"><h3>Fallos</h3><p>¿Qué pasa sin sensor, broker, servidor o Internet?</p></div>
  <div class="iot-card"><h3>Seguridad</h3><p>¿Identidad, TLS, ACL, actualización y auditoría?</p></div>
  <div class="iot-card"><h3>Operación</h3><p>¿Cómo instalar, observar, mantener y reemplazar?</p></div>
</div>

<!--
Tiempo: 3 min. Este checklist es la transición de tutorial a ingeniería. Pida elegir un caso y responder una pregunta de cada tarjeta.
-->

---

# Reto final · diseña un sistema completo

<p class="lead">En equipos, elijan RFID, huella, hogar o clima y entreguen:</p>

<div class="iot-grid cols-4 mt-6">
  <div class="iot-card"><div class="metric">1</div><p class="mt-3">Diagrama por capas</p></div>
  <div class="iot-card"><div class="metric">2</div><p class="mt-3">Tópicos y payloads</p></div>
  <div class="iot-card"><div class="metric">3</div><p class="mt-3">Política offline</p></div>
  <div class="iot-card"><div class="metric">4</div><p class="mt-3">Tres controles de seguridad</p></div>
</div>

<div class="callout mt-7"><p><strong>Evaluación:</strong> coherencia técnica, manejo de fallos, seguridad y claridad; no cantidad de tecnologías.</p></div>

<!--
Tiempo: 4 min para explicar; puede quedar como tarea o evaluación. Una buena solución justifica decisiones y muestra qué componente conserva autoridad sobre cada acción.
-->

---
layout: center
class: section-slide
---

<div class="eyebrow">Evaluación</div>

# ¿Cuánto aprendiste?

<p>Quiz rápido: 10 preguntas para verificar los conceptos clave.</p>

<div class="section-number">EV</div>

<!-- Tiempo: 10 min. Haga el quiz en voz alta o con una herramienta tipo Kahoot. Discuta cada respuesta incorrecta. -->

---

# Quiz de conocimientos

<div class="iot-grid cols-2 mt-5 small">
  <div class="iot-card">
    <h3>1. Wi‑Fi es HTTP</h3>
    <p><span class="chip">Falso</span> Wi‑Fi es la red; HTTP es un protocolo de aplicación.</p>
  </div>
  <div class="iot-card">
    <h3>2. MQTT usa pub/sub</h3>
    <p><span class="chip green">Verdadero</span> Los clientes publican y se suscriben a tópicos.</p>
  </div>
  <div class="iot-card">
    <h3>3. QoS 2 es at-most-once</h3>
    <p><span class="chip">Falso</span> QoS 0 es at-most-once; QoS 2 es exactly-once.</p>
  </div>
  <div class="iot-card">
    <h3>4. WebSocket es MQTT</h3>
    <p><span class="chip">Falso</span> MQTT puede viajar sobre WebSocket, pero son protocolos distintos.</p>
  </div>
  <div class="iot-card">
    <h3>5. ESP32 usa 5 V lógico</h3>
    <p><span class="chip">Falso</span> Los GPIO del ESP32 son de 3.3 V.</p>
  </div>
  <div class="iot-card">
    <h3>6. El broker enruta mensajes</h3>
    <p><span class="chip green">Verdadero</span> Recibe de publishers y envía a subscribers.</p>
  </div>
  <div class="iot-card">
    <h3>7. retain guarda historial</h3>
    <p><span class="chip">Falso</span> retain guarda solo el último mensaje; no es un historial.</p>
  </div>
  <div class="iot-card">
    <h3>8. LWT avisa offline</h3>
    <p><span class="chip green">Verdadero</span> Last Will publica "offline" si el cliente desaparece.</p>
  </div>
  <div class="iot-card">
    <h3>9. MicroPython es más rápido que C++</h3>
    <p><span class="chip">Falso</span> C++ es nativo; MicroPython es interpretado y más lento.</p>
  </div>
  <div class="iot-card">
    <h3>10. requestId correlaciona comandos</h3>
    <p><span class="chip green">Verdadero</span> Permite vincular un comando con su ACK.</p>
  </div>
</div>

<!--
Tiempo: 10 min. No lea las respuestas de inmediato. Discuta cada una. Las incorrectas son oportunidades de aprendizaje, no fracasos.
-->

---

# Reto por equipos · diseña un sistema completo

<p class="lead">En equipos de 3-4 personas, elijan un caso y entreguen un diseño completo:</p>

<div class="iot-grid cols-4 mt-6">
  <div class="iot-card"><div class="metric">1</div><p class="mt-3">Diagrama de arquitectura por capas</p></div>
  <div class="iot-card"><div class="metric">2</div><p class="mt-3">Tópicos MQTT y payloads JSON</p></div>
  <div class="iot-card"><div class="metric">3</div><p class="mt-3">Política offline y recuperación</p></div>
  <div class="iot-card"><div class="metric">4</div><p class="mt-3">Tres controles de seguridad</p></div>
</div>

<div class="iot-grid cols-3 mt-5">
  <div class="iot-card cyan"><h3>Opción A</h3><p>Control de acceso con RFID + huella + dashboard</p></div>
  <div class="iot-card green"><h3>Opción B</h3><p>Invernadero con sensores + riego automático</p></div>
  <div class="iot-card amber"><h3>Opción C</h3><p>Sistema de seguridad con PIR + RFID + alertas</p></div>
</div>

<div class="callout mt-5"><p><strong>Entrega:</strong> diagrama en papel o herramienta, documento de 1 página con decisiones técnicas, y 3 minutos de presentación.</p></div>

<!--
Tiempo: 15 min para diseñar + 10 min para presentar. Evalúe coherencia, manejo de fallos y seguridad; no cantidad de tecnologías.
-->

---

# Rúbrica de evaluación

<table class="compare mt-5">
  <thead><tr><th>Criterio</th><th>Excelente (4)</th><th>Bueno (3)</th><th>Básico (2)</th><th>Insuficiente (1)</th></tr></thead>
  <tbody>
    <tr><td><strong>Arquitectura</strong></td><td>Capas claras, flujos correctos</td><td>Capas identificadas con errores menores</td><td>Capas confusas</td><td>No identifica capas</td></tr>
    <tr><td><strong>Manejo de fallos</strong></td><td>Offline, reconnect, buffer</td><td>Algunos fallos contemplados</td><td>Solo menciona fallos</td><td>No considera fallos</td></tr>
    <tr><td><strong>Seguridad</strong></td><td>TLS, ACL, identidad, auditoría</td><td>TLS + identidad básica</td><td>Menciona seguridad</td><td>Sin seguridad</td></tr>
    <tr><td><strong>Contrato MQTT</strong></td><td>Tópicos jerárquicos, payloads claros</td><td>Tópicos correctos, payloads simples</td><td>Tópicos planos</td><td>No define tópicos</td></tr>
    <tr><td><strong>Presentación</strong></td><td>Clara, justifica decisiones</td><td>Explica pero sin justificar</td><td>Confusa o incompleta</td><td>No presenta</td></tr>
  </tbody>
</table>

<div class="callout mt-5"><p><strong>Peso sugerido:</strong> Arquitectura 30%, Manejo de fallos 25%, Seguridad 25%, Contrato 10%, Presentación 10%.</p></div>

<!--
Tiempo: Use esta rúbrica para evaluar los retos. Puede adaptarla según el nivel del grupo. Lo importante es que los criterios sean conocidos antes de empezar.
-->

---

# Recursos para continuar aprendiendo

<div class="iot-grid cols-2 mt-5">
  <div class="iot-card cyan">
    <h3>Documentación oficial</h3>
    <ul class="mt-3">
      <li>Espressif: docs.espressif.com</li>
      <li>Arduino: docs.arduino.cc</li>
      <li>MQTT: mqtt.org</li>
      <li>Mosquitto: mosquitto.org</li>
    </ul>
  </div>
  <div class="iot-card green">
    <h3>Plataformas en la nube</h3>
    <ul class="mt-3">
      <li>Arduino Cloud (gratis limitado)</li>
      <li>EMQX Cloud (plan gratuito)</li>
      <li>HiveMQ Cloud (plan gratuito)</li>
      <li>ThingsBoard (open source + cloud)</li>
    </ul>
  </div>
  <div class="iot-card blue">
    <h3>Herramientas</h3>
    <ul class="mt-3">
      <li>PlatformIO (VS Code)</li>
      <li>Thonny (MicroPython)</li>
      <li>Node‑RED (automatización visual)</li>
      <li>Home Assistant (domótica)</li>
    </ul>
  </div>
  <div class="iot-card amber">
    <h3>Comunidades</h3>
    <ul class="mt-3">
      <li>Reddit r/esp32, r/arduino, r/IOT</li>
      <li>Foros de Espressif y Arduino</li>
      <li>Discord de PlatformIO y Home Assistant</li>
      <li>YouTube: Andreas Spiess, Great Scott</li>
    </ul>
  </div>
</div>

<div class="callout mt-7"><p><strong>Siguiente paso:</strong> elija un proyecto personal pequeño. Construya extremo a extremo antes de añadir más sensores o nubes.</p></div>

<!--
Tiempo: 2 min. No lea la lista completa. Destaque 2-3 recursos y motive a empezar un proyecto propio. La mejor forma de aprender IoT es construyendo.
-->

---
layout: center
class: cover
---

<div class="eyebrow">Cierre</div>

# El puente ya está completo

<p class="subtitle">Señal → firmware → red → protocolo → servicio → datos → experiencia.</p>

<div class="mt-10 flex gap-3 justify-center"><span class="chip">Construye pequeño</span><span class="chip green">Mide todo</span><span class="chip amber">Diseña para fallar</span></div>

<p class="small muted mt-12">Anexo: cableado, comandos, solución de problemas, seguridad y referencias.</p>

<!--
Tiempo: 2 min. Recapitule el puente completo. Invite a comenzar con un sensor y un actuador, pero con contratos, observabilidad y seguridad desde el primer prototipo.
-->
