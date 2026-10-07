---
layout: cover
class: cover
routeAlias: clase-2
---

<div class="eyebrow">Clase 2 · 120 minutos</div>

# Muchos equipos, un <span class="accent">mensajero</span>

<p class="subtitle">MQTT, Mosquitto, WebSockets y páginas para monitorear o controlar hardware.</p>

<div class="mt-10 flex gap-3"><span class="chip">MQTT</span><span class="chip green">Mosquitto</span><span class="chip amber">WebSockets</span></div>

---

# Antes: conexión directa con HTTP

<div class="network-diagram"><div class="device laptop"><i>⌘</i><b>Página web</b><small>pregunta</small></div><div class="diagram-link bidirectional">GET ↔ JSON<br><em>HTTP</em></div><div class="device board"><i>ESP</i><b>ESP32</b><small>responde</small></div></div>

<p class="lead mt-8">Funciona muy bien para una página local. Pero ¿qué hacemos con varias páginas, sensores y programas?</p>

---

# MQTT añade un punto de encuentro

<div class="network-diagram"><div class="device board"><i>ESP</i><b>ESP32</b><small>publica</small></div><div class="diagram-link">mensaje<br><em>MQTT</em></div><div class="device server"><i>▣</i><b>Broker</b><small>recibe y reparte</small></div><div class="diagram-link">mensaje<br><em>MQTT</em></div><div class="device laptop"><i>⌘</i><b>Web / app</b><small>se suscribe</small></div></div>

<p class="lead mt-8">El dispositivo y la página no necesitan conocerse directamente.</p>

---

# MQTT en un diagrama

<div class="iot-grid cols-2 mt-4"><div class="media-panel"><img src="https://phuongnamvina.com/img_data/images/co-che-hoat-dong-mqtt.jpg" alt="Diagrama MQTT publisher broker subscriber"><span class="media-label">Publicar → broker → suscribirse</span></div><div class="v-center"><p class="lead">El sensor publica una vez. El broker entrega el dato a cada pantalla o programa que está suscrito.</p><div class="callout mt-6"><p>Los equipos no se llaman entre sí: usan el broker como punto de encuentro.</p></div><p class="tiny mt-4">Referencia visual externa: Phuong Nam Vina · mecanismo MQTT.</p></div></div>

---
layout: center
class: section-slide
---

<div class="eyebrow">Bloque 3</div>

# MQTT en palabras simples

<p>Publicar es dejar un mensaje. Suscribirse es esperar mensajes.</p>

<div class="section-number">03</div>

---

# Los tres personajes

<div class="iot-grid cols-3 mt-7"><div class="iot-card cyan"><div class="metric">1</div><h3>Publicador</h3><p>Envía un dato: temperatura, botón o estado.</p></div><div class="iot-card green"><div class="metric">2</div><h3>Broker</h3><p>Es el mensajero que recibe y entrega.</p></div><div class="iot-card blue"><div class="metric">3</div><h3>Suscriptor</h3><p>Recibe los datos que le interesan.</p></div></div>

---

# Un tópico es el nombre del buzón

<div class="big-code">

```text
cristian/device/esp32-01/telemetry
cristian/device/esp32-01/control
```

</div>

<div class="iot-grid cols-2 mt-6"><div class="iot-card cyan"><h3>telemetry</h3><p>El ESP32 cuenta lo que está pasando.</p></div><div class="iot-card green"><h3>control</h3><p>La web envía una orden al ESP32.</p></div></div>

---

# Un mensaje pequeño y claro

<div class="iot-grid cols-2 mt-5"><div class="big-code">

```json
{ "temperatureC": 24.6, "ledOn": true }
```

</div><div class="big-code">

```json
{ "command": "SET_LED", "value": true }
```

</div></div>

<p class="lead mt-7">JSON es solo una forma ordenada de enviar datos con nombre y valor.</p>

---

# Mosquitto: nuestro broker

<div class="network-diagram"><div class="device board"><i>ESP</i><b>ESP32</b><small>Wi‑Fi · MQTT</small></div><div class="diagram-link bidirectional">puerto 1883<br><em>MQTT</em></div><div class="device server"><i>▣</i><b>Mosquitto</b><small>broker propio</small></div><div class="diagram-link bidirectional">puerto 9001<br><em>WebSocket</em></div><div class="device laptop"><i>⌘</i><b>Página web</b><small>control y monitor</small></div></div>

<div class="callout mt-8"><p>Un broker puede vivir en una Raspberry Pi, una computadora del aula o un servidor de Internet.</p></div>

---

# Instalar Mosquitto en Windows

<div class="iot-grid cols-2 mt-4"><div class="media-panel"><img src="https://images.unsplash.com/photo-1516321318423-f06f85e504b3?auto=format&fit=crop&w=1000&q=80" alt="Computadora Windows"><span class="media-label">PC del aula = broker local</span></div><div class="timeline"><div class="time">1</div><div class="event">Descarga el instalador oficial de Mosquitto para Windows.</div><div class="time">2</div><div class="event">Instala y abre PowerShell.</div><div class="time">3</div><div class="event">Ejecuta <code>mosquitto -v</code>.</div><div class="time">4</div><div class="event">Tus compañeros usan la IP de esa PC como broker.</div></div></div>

---

# Ejecutar y probar Mosquitto local

<div class="big-code">

```powershell
# Terminal 1: iniciar broker
mosquitto -v

# Terminal 2: escuchar mensajes
mosquitto_sub -h localhost -t "clase/#" -v

# Terminal 3: publicar una prueba
mosquitto_pub -h localhost -t "clase/led" -m "hola ESP32"
```

</div>

<p class="lead mt-6">Si los tres comandos funcionan, ya tienes un laboratorio MQTT local.</p>

---

# Configurar WebSockets en Mosquitto

<div class="big-code">

```text
listener 1883
protocol mqtt

listener 9001
protocol websockets
allow_anonymous true
```

</div>

<p class="lead mt-6">El puerto 1883 recibe ESP32. El puerto 9001 permite que una página web se conecte por WebSocket.</p>

---

# WebSocket permite que el navegador participe

<div class="network-diagram"><div class="device laptop"><i>⌘</i><b>Navegador</b><small>abre WebSocket</small></div><div class="diagram-link bidirectional">canal abierto<br><em>WebSocket</em></div><div class="device server"><i>▣</i><b>Mosquitto</b><small>habla MQTT</small></div><div class="diagram-link bidirectional">control<br><em>MQTT</em></div><div class="device board"><i>ESP</i><b>ESP32</b><small>recibe órdenes</small></div></div>

<p class="lead mt-8">WebSocket mantiene un canal abierto: por eso el panel puede actualizarse sin recargar.</p>

---

# Práctica 3 · Mosquitto local + datos simulados + LED

<div class="iot-grid cols-2 mt-5"><div class="terminal"><span class="prompt">En Arduino IDE</span><br>1. Gestor de placas: <b>esp32 by Espressif Systems</b><br>2. Biblioteca: instala <b>PubSubClient</b> de Nick O'Leary<br>3. Abre <code>esp32-mqtt-local-dashboard.ino</code><br>4. Cambia Wi‑Fi y <b>MQTT_HOST = IP_DE_LA_PC</b></div><div class="terminal"><span class="prompt">PC = broker Mosquitto local</span><br>1. Guarda <code>mosquitto.conf</code><br>2. Ejecuta:<br><b>mosquitto -c mosquitto.conf -v</b><br>3. ESP32 usa <b>IP_PC:1883</b><br>4. Página usa <b>ws://IP_PC:9001</b></div></div>

<p class="lead mt-6">En esta práctica el ESP32 publica temperatura, humedad y LED simulados cada 5 segundos; la web puede encender y apagar el LED.</p>

---
layout: default
class: full-source
---

# Copia y pega · ESP32 MQTT completo para la práctica 3

<<< @/snippets/esp32-mqtt-local-dashboard.cpp

---
layout: default
class: full-source
---

# Copia y pega · página WebSocket MQTT completa

<<< @/examples/mqtt-web-dashboard/index.html

---

# Así se ve la práctica 3 con Mosquitto local

<div class="browser-preview"><div class="browser-bar"><span></span><span></span><span></span><b>Panel MQTT del aula · ws://IP_PC:9001</b></div><div class="browser-content"><div><h2>Temperatura: 24.6 °C</h2><p>Humedad: <b>52 %</b><br>LED: <b style="color:#0f766e">ENCENDIDO</b></p></div><div><button>Conectar broker local</button><br><button>Encender LED</button><button>Apagar LED</button><p class="small">El ESP32 publica cada 5 segundos en <code>clase/esp32-01/telemetry</code>.</p></div></div></div>

<p class="small muted mt-3">Abre <code>examples/mqtt-web-dashboard/index.html</code>, escribe la IP de la PC que ejecuta Mosquitto y presiona Conectar.</p>

---

# El viaje de un clic

<div class="network-diagram"><div class="device laptop"><i>⌘</i><b>1. Botón web</b><small>usuario hace clic</small></div><div class="diagram-link">publica<br><em>/control</em></div><div class="device server"><i>▣</i><b>2. Broker</b><small>entrega al ESP32</small></div><div class="diagram-link">recibe<br><em>/telemetry</em></div><div class="device board"><i>ESP</i><b>3. ESP32</b><small>LED cambia y reporta</small></div></div>

---

# ¿Por qué HTTP no basta para tiempo real?

<div class="network-diagram"><div class="device laptop"><i>⌘</i><b>Página</b><small>«¿hay datos?»</small></div><div class="diagram-link">pregunta<br><em>HTTP</em></div><div class="device board"><i>ESP</i><b>ESP32 / servidor</b><small>responde una vez</small></div></div>

<div class="iot-grid cols-2 mt-7"><div class="iot-card amber"><h3>HTTP funciona muy bien para…</h3><p>Abrir una página, consultar estado o enviar un dato puntual.</p></div><div class="iot-card red"><h3>Pero para controlar siempre…</h3><p>La página debe preguntar repetidamente; el ESP32 no puede avisar por sí solo con HTTP normal.</p></div></div>

---

# El problema de preguntar una y otra vez

<div class="network-diagram"><div class="device laptop"><i>⌘</i><b>Página</b><small>¿cambió?</small></div><div class="diagram-link">consulta<br><em>cada 3 s</em></div><div class="device server"><i>▣</i><b>Servidor</b><small>todavía no</small></div><div class="diagram-link">repite<br><em>cada 3 s</em></div><div class="device laptop"><i>⌘</i><b>Página</b><small>¿cambió ahora?</small></div></div>

<p class="lead mt-8">Esto se llama <strong>preguntar repetidamente</strong>. Funciona, pero desperdicia mensajes y puede sentirse lento.</p>

---

# MQTT resuelve el aviso inmediato

<div class="network-diagram"><div class="device board"><i>ESP</i><b>ESP32</b><small>publica al cambiar</small></div><div class="diagram-link">aviso<br><em>MQTT</em></div><div class="device server"><i>▣</i><b>Broker</b><small>reparte de inmediato</small></div><div class="diagram-link">aviso<br><em>MQTT</em></div><div class="device laptop"><i>⌘</i><b>Página</b><small>actualiza sola</small></div></div>

<div class="callout mt-8"><p>MQTT es bidireccional: la web puede enviar una orden y el ESP32 puede publicar un cambio en cualquier momento.</p></div>

---

# HTTP o MQTT: elige según el problema

<div class="iot-grid cols-2 mt-6"><div class="iot-card cyan"><h3>HTTP</h3><p><strong>Pregunta y respuesta.</strong><br>Configuración local, formularios, APIs y acciones puntuales.</p></div><div class="iot-card green"><h3>MQTT</h3><p><strong>Mensajes que llegan solos.</strong><br>Telemetría, alertas, varias pantallas y control en tiempo real.</p></div></div>

<p class="lead mt-8">En muchos proyectos se usan los dos: HTTP para configurar; MQTT para comunicar en vivo.</p>

---

# Servicios que puedes encontrar en Internet

<div class="iot-grid cols-3 mt-6"><div class="iot-card cyan"><h3>Broker propio</h3><p>Mosquitto en tu Raspberry Pi o servidor.</p></div><div class="iot-card green"><h3>Broker de prueba</h3><p>Útil para experimentar, no para datos importantes.</p></div><div class="iot-card blue"><h3>IoT administrado</h3><p>Paneles, usuarios, reglas y almacenamiento listos.</p></div></div>

<p class="lead mt-8">La pregunta no es “cuál es mejor”, sino: <strong>¿qué necesita mi proyecto?</strong></p>

---

# HiveMQ Cloud · broker gratuito administrado

<div class="iot-grid cols-2 mt-4"><div class="media-panel"><img src="https://images.unsplash.com/photo-1451187580459-43490279c0fa?auto=format&fit=crop&w=1000&q=80" alt="Nube y conexiones"><span class="media-label">El broker vive en Internet</span></div><div class="v-center"><p class="lead">HiveMQ Cloud ofrece un plan gratuito para crear un broker sin instalar un servidor.</p><div class="timeline mt-5"><div class="time">1</div><div class="event">Crear cuenta y cluster gratuito.</div><div class="time">2</div><div class="event">Crear credenciales MQTT.</div><div class="time">3</div><div class="event">Copiar host, puerto TLS, usuario y clave.</div></div></div></div>

---

# Código ESP32 para HiveMQ Cloud

<div class="big-code">

```cpp
WiFiClientSecure network;
network.setInsecure(); // Solo para la práctica.
PubSubClient mqtt(network);

mqtt.setServer("TU_CLUSTER.s1.eu.hivemq.cloud", 8883);
mqtt.connect("esp32-alumno", "USUARIO", "CLAVE");
mqtt.subscribe("clase/control");
```

</div>

<p class="lead mt-6">El ejemplo completo queda en <code>examples/esp32-mqtt-hivemq/</code>.</p>

---
layout: default
class: full-source
---

# Copia y pega · ESP32 + HiveMQ Cloud completo

<<< @/snippets/esp32-mqtt-hivemq.cpp

---

# Alternativas fáciles para proyectos IoT

<div class="iot-grid cols-2 mt-4"><div class="media-panel"><img src="https://smarthomescene.com/wp-content/uploads/2022/03/themes-waves-1-1024x576.jpg.webp" alt="Panel Home Assistant"><span class="media-label">Home Assistant: domótica local</span></div><div class="iot-grid cols-2"><div class="iot-card cyan"><h3>Arduino Cloud</h3><p>Programar, conectar y crear paneles en una misma plataforma.</p></div><div class="iot-card green"><h3>Home Assistant</h3><p>Automatizar una casa con paneles y dispositivos locales.</p></div><div class="iot-card blue"><h3>Node‑RED</h3><p>Unir sensores, reglas y paneles con bloques visuales.</p></div><div class="iot-card amber"><h3>HiveMQ Cloud</h3><p>Broker MQTT administrado para practicar sin servidor propio.</p></div></div></div>

---

# Del dato a un panel de monitoreo

<div class="network-diagram"><div class="device sensor"><i>◌</i><b>Sensor</b><small>mide</small></div><div class="diagram-link">GPIO</div><div class="device board"><i>ESP</i><b>ESP32</b><small>publica</small></div><div class="diagram-link">MQTT</div><div class="device server"><i>▣</i><b>Broker</b><small>reparte</small></div><div class="diagram-link">MQTT</div><div class="device laptop"><i>⌘</i><b>Panel</b><small>muestra</small></div></div>

---

# Aplicaciones reales

<div class="iot-grid cols-2 mt-5"><div class="media-panel"><img src="https://images.unsplash.com/photo-1558002038-1055907df827?auto=format&fit=crop&w=1000&q=80" alt="Automatización del hogar"><span class="media-label">Hogar: luces, alarmas y sensores</span></div><div class="media-panel"><img src="https://images.unsplash.com/photo-1581092160607-ee22621dd758?auto=format&fit=crop&w=1000&q=80" alt="Fábrica"><span class="media-label">Industria: máquinas y mantenimiento</span></div></div>

---

# También sirve para agricultura y ciudades

<div class="iot-grid cols-3 mt-7"><div class="iot-card cyan"><div class="metric">🌱</div><h3>Invernadero</h3><p>Humedad, riego y ventilación.</p></div><div class="iot-card green"><div class="metric">🚗</div><h3>Movilidad</h3><p>Ubicación y estado de vehículos.</p></div><div class="iot-card amber"><div class="metric">🏭</div><h3>Fábrica</h3><p>Alertas y control de procesos.</p></div></div>

---
layout: center
class: section-slide
---

<div class="eyebrow">Proyecto final</div>

# Un brazo robótico conectado

<p>Todo lo aprendido se une en un proyecto: mover un brazo desde una página y observar su estado.</p>

<div class="section-number">04</div>

---

# El brazo reutiliza las mismas ideas

<div class="robot-stage"><div class="robot-arm"><div class="robot-base"></div><div class="robot-link base"></div><div class="robot-link shoulder"></div><div class="robot-link elbow"></div><div class="joint base">1</div><div class="joint shoulder">2</div><div class="joint elbow">3</div><div class="joint gripper">4</div><div class="gripper"></div><span class="robot-label base">Base</span><span class="robot-label shoulder">Hombro</span><span class="robot-label elbow">Codo</span><span class="robot-label gripper">Garra</span></div></div>

<div class="network-diagram"><div class="device laptop"><i>⌘</i><b>Página web</b><small>elige una pose</small></div><div class="diagram-link">mensaje<br><em>MQTT</em></div><div class="device board"><i>ESP</i><b>ESP32</b><small>traduce ángulos</small></div><div class="diagram-link">PWM<br><em>señal</em></div><div class="device sensor"><i>4×</i><b>4 servos</b><small>mueven el brazo</small></div></div>

---

# Proyecto final · tomar y soltar

<div class="pose-strip"><div class="pose"><span class="pose-icon">1</span><b>Inicio</b><small>posición inicial</small></div><div class="pose"><span class="pose-icon">2</span><b>Bajar</b><small>llegar al objeto</small></div><div class="pose"><span class="pose-icon">3</span><b>Tomar</b><small>cerrar garra</small></div><div class="pose"><span class="pose-icon">4</span><b>Soltar</b><small>mover y abrir</small></div></div>

<p class="lead mt-7">No cambia la comunicación: solo cambia el actuador final.</p>

---

# Práctica 4 · instalar y conectar el brazo

<div class="iot-grid cols-2 mt-5"><div class="terminal"><span class="prompt">Bibliotecas Arduino IDE</span><br>1. <b>ESP32Servo</b><br>2. <b>PubSubClient</b><br>3. <b>ArduinoJson</b><br>4. <b>WebSockets</b> de Markus Sattler<br><br>Además instala la placa <b>esp32 by Espressif Systems</b>.</div><div class="terminal"><span class="prompt">Señales de servo</span><br>Base → GPIO 13<br>Hombro → GPIO 14<br>Codo → GPIO 25<br>Garra → GPIO 26<br><br><b>Importante:</b> los servos usan fuente externa de 5 V; une GND de fuente y ESP32.</div></div>

---
layout: default
class: full-source
---

# Copia y pega · ESP32 completo para el brazo

<<< @/snippets/esp32-robot-arm-mqtt.cpp

---

# Así se ve la página de control del brazo

<div class="browser-preview"><div class="browser-bar"><span></span><span></span><span></span><b>Panel · Brazo robótico</b></div><div class="browser-content"><div><h2>Elegir pose</h2><button>Inicio</button> <button>Bajar</button> <button>Tomar</button> <button>Soltar</button></div><div class="arm-values"><b>Base</b><input type="range" value="50"><b>Hombro</b><input type="range" value="65"><b>Codo</b><input type="range" value="35"><b>Garra</b><input type="range" value="20"></div></div></div>

<p class="lead mt-5">Cada botón publica una pose JSON al tópico del brazo; el ESP32 mueve los cuatro servos y publica su estado.</p>

---
layout: default
class: full-source
---

# Copia y pega · página web de control del brazo

<<< @/examples/robot-arm-web/index.html

---

# Lo que ya puedes crear

<div class="visual-flow"><div class="visual-node">🔌<span>circuito</span></div><div class="visual-arrow">+</div><div class="visual-node">📶<span>Wi‑Fi</span></div><div class="visual-arrow">+</div><div class="visual-node">🌐<span>software</span></div><div class="visual-arrow">+</div><div class="visual-node">📬<span>MQTT</span></div><div class="visual-arrow">=</div><div class="visual-node">✨<span>proyecto conectado</span></div></div>
