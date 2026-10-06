---
layout: cover
class: cover
routeAlias: anexo
---

<div class="eyebrow">Material de consulta</div>

# Anexo técnico

<p class="subtitle">Cableado, contratos, comandos, fallos frecuentes y siguientes pasos.</p>

<!-- El anexo no forma parte de los 120 minutos de cada clase; úselo para preguntas o ampliación. -->

---

# Montaje de la práctica

<div class="iot-grid cols-2 mt-5">
  <div class="iot-card cyan">
    <h3>LED</h3>
    <p>GPIO 2 → resistencia 220–330 Ω → ánodo LED; cátodo → GND. Ajusta el pin a tu placa.</p>
  </div>
  <div class="iot-card green">
    <h3>DHT11 / DHT22</h3>
    <p>VCC → 3.3 V, GND → GND, DATA → GPIO 4. Algunos módulos ya integran pull-up.</p>
  </div>
</div>

<div class="callout danger mt-7"><p>Verifica el pinout del módulo específico. No alimentes un GPIO con 5 V.</p></div>

---

# Librerías Arduino

<div class="terminal"><span class="prompt">Arduino IDE → Library Manager</span><br>PubSubClient · Nick O'Leary<br>ArduinoJson · Benoit Blanchon<br>DHT sensor library · Adafruit<br>Adafruit Unified Sensor</div>

<div class="iot-grid cols-2 mt-7">
  <div class="iot-card"><h3>ESP32</h3><p><code>#include &lt;WiFi.h&gt;</code></p></div>
  <div class="iot-card"><h3>ESP8266</h3><p><code>#include &lt;ESP8266WiFi.h&gt;</code></p></div>
</div>

---

# Contrato MQTT de la demo

<table class="compare mt-4">
  <thead><tr><th>Sufijo</th><th>Dirección</th><th>QoS</th><th>Retain</th><th>Contenido</th></tr></thead>
  <tbody>
    <tr><td>telemetry</td><td>ESP → consumidores</td><td>0</td><td>No</td><td>Mediciones y salud</td></tr>
    <tr><td>status</td><td>ESP → consumidores</td><td>1</td><td>Sí</td><td>online/offline y versión</td></tr>
    <tr><td>control</td><td>app → ESP</td><td>1</td><td>No</td><td>SET_LED + requestId</td></tr>
    <tr><td>cmd/ack</td><td>ESP → app</td><td>1</td><td>No</td><td>resultado correlacionado</td></tr>
    <tr><td>alerts</td><td>ESP → consumidores</td><td>1</td><td>No</td><td>evento excepcional</td></tr>
  </tbody>
</table>

---

# Comandos de diagnóstico

```bash
# Ver todos los mensajes del dispositivo
mosquitto_sub -h BROKER -u USER -P PASS \
  -t 'cristian/device/esp32-01/#' -v

# Encender LED
mosquitto_pub -h BROKER -u USER -P PASS -q 1 \
  -t 'cristian/device/esp32-01/control' \
  -m '{"command":"SET_LED","value":true,"requestId":"cli-1"}'
```

<p class="small muted">Para un broker TLS añade CA/puerto correspondiente. No copies secretos reales en historial de shell o capturas.</p>

---

# Fallos frecuentes

<table class="compare mt-4">
  <thead><tr><th>Síntoma</th><th>Comprueba primero</th></tr></thead>
  <tbody>
    <tr><td>Wi‑Fi no conecta</td><td>SSID 2.4 GHz, clave, señal y modo de red</td></tr>
    <tr><td>MQTT se reconecta siempre</td><td>URL/puerto, clientId único, credencial, reloj/TLS</td></tr>
    <tr><td>Web conecta; ESP32 no</td><td>WSS ≠ MQTT TCP; usa puerto y esquema correctos</td></tr>
    <tr><td>No llega un comando</td><td>Tópico exacto, suscripción tras reconnect, ACL</td></tr>
    <tr><td>DHT devuelve NaN</td><td>Tipo, cableado, intervalo de lectura y pull-up</td></tr>
    <tr><td>ESP32 se reinicia</td><td>Fuente, brownout, watchdog, memoria y carga</td></tr>
  </tbody>
</table>

---

# Capacidad y resiliencia

<div class="iot-grid cols-3 mt-5">
  <div class="iot-card"><h3>Frecuencia</h3><p>No publiques cada loop. Muestrea y agrega según el fenómeno.</p></div>
  <div class="iot-card"><h3>Buffer</h3><p>Conserva eventos críticos durante cortes con límites definidos.</p></div>
  <div class="iot-card"><h3>Backpressure</h3><p>Decide qué descartar cuando se producen datos más rápido de lo que se consumen.</p></div>
  <div class="iot-card"><h3>Watchdog</h3><p>Recupera bloqueos, pero registra la causa del reinicio.</p></div>
  <div class="iot-card"><h3>Idempotencia</h3><p>Un mismo requestId no debe repetir un efecto peligroso.</p></div>
  <div class="iot-card"><h3>Observabilidad</h3><p>Logs, métricas, disponibilidad y versión de firmware.</p></div>
</div>

---

# De prototipo a producción

<div class="timeline mt-4">
  <div class="time">01</div><div class="event">Separar credenciales por entorno y dispositivo.</div>
  <div class="time">02</div><div class="event">TLS, ACL mínima, rotación y revocación.</div>
  <div class="time">03</div><div class="event">Versionar firmware y contrato de mensajes.</div>
  <div class="time">04</div><div class="event">OTA firmada, rollback y despliegue gradual.</div>
  <div class="time">05</div><div class="event">Pruebas de corte, duplicado, reloj incorrecto y datos inválidos.</div>
  <div class="time">06</div><div class="event">Monitorear flota, no solo una placa en el escritorio.</div>
</div>

---

# Ruta de aprendizaje recomendada

<div class="flow">
  <div class="flow-step"><b>1</b><small>GPIO + sensor</small></div><div class="flow-arrow">→</div>
  <div class="flow-step"><b>2</b><small>Wi‑Fi + HTTP</small></div><div class="flow-arrow">→</div>
  <div class="flow-step"><b>3</b><small>MQTT + dashboard</small></div><div class="flow-arrow">→</div>
  <div class="flow-step"><b>4</b><small>seguridad + OTA</small></div><div class="flow-arrow">→</div>
  <div class="flow-step"><b>5</b><small>gateway + datos</small></div>
</div>

<div class="callout mt-8"><p>Construye una versión pequeña de extremo a extremo antes de añadir más sensores, nubes o frameworks.</p></div>

---

# Referencias oficiales

<div class="iot-grid cols-2 mt-5 small">
  <div class="iot-card"><h3>Espressif</h3><p><a href="https://docs.espressif.com/">ESP-IDF, HTTP server, ESP-NOW y seguridad</a></p></div>
  <div class="iot-card"><h3>Arduino</h3><p><a href="https://docs.arduino.cc/arduino-cloud">Arduino Cloud y ESP32/ESP8266</a></p></div>
  <div class="iot-card"><h3>MQTT</h3><p><a href="https://mqtt.org/">Especificación, software y recursos</a></p></div>
  <div class="iot-card"><h3>Mosquitto</h3><p><a href="https://mosquitto.org/documentation/">Broker, clientes y configuración</a></p></div>
  <div class="iot-card"><h3>EMQX Cloud</h3><p><a href="https://docs.emqx.com/en/cloud/latest/">Planes y operación administrada</a></p></div>
  <div class="iot-card"><h3>Raspberry Pi</h3><p><a href="https://www.raspberrypi.com/documentation/">Hardware, Linux y GPIO</a></p></div>
</div>

<p class="tiny mt-5">Consulta precios y límites justo antes de impartir la clase: cambian con el tiempo y la región.</p>

---

# Guía de sensores

<div class="iot-grid cols-2 mt-5 small">
  <div class="iot-card">
    <h3>PIR HC‑SR501</h3>
    <p><strong>VCC:</strong> 5–20 V. <strong>OUT:</strong> 3.3 V (HIGH al detectar). <strong>Ajustes:</strong> sensibilidad (20 cm – 7 m) y tiempo (0.3 s – 200 s).</p>
    <p class="mt-2"><strong>Debounce:</strong> el sensor puede dar varios HIGH seguidos. Usa un flag <code>lastState</code> y un temporizador mínimo.</p>
  </div>
  <div class="iot-card">
    <h3>Ultrasónico HC‑SR04</h3>
    <p><strong>TRIG:</strong> pulso de 10 µs. <strong>ECHO:</strong> HIGH durante el tiempo de ida y vuelta. <strong>Fórmula:</strong> distancia = duración × 0.0343 / 2.</p>
    <p class="mt-2"><strong>Divisor:</strong> ECHO devuelve 5 V. Usa R1=1 kΩ y R2=2 kΩ para bajar a 3.3 V.</p>
  </div>
  <div class="iot-card">
    <h3>LDR (fotorresistencia)</h3>
    <p>Resistencia variable con la luz. Usa divisor: LDR + 10 kΩ. Más luz → menos resistencia → mayor voltaje en el GPIO.</p>
    <p class="mt-2"><strong>ADC:</strong> GPIO 34–39 en ESP32. El valor <code>analogRead()</code> va de 0 a 4095.</p>
  </div>
  <div class="iot-card">
    <h3>Gas MQ‑2 / MQ‑135</h3>
    <p><strong>Precalentamiento:</strong> 24 h continuas antes de dar lecturas estables. <strong>Salida:</strong> analógica (ADC) y digital (comparador con potenciómetro).</p>
    <p class="mt-2"><strong>Calibración:</strong> el valor ADC es relativo. Para ppm, usa la curva del datasheet: Rs/R0 vs ppm.</p>
  </div>
  <div class="iot-card">
    <h3>RFID MFRC522</h3>
    <p><strong>Interfaz:</strong> SPI a 3.3 V. <strong>Frecuencia:</strong> 13.56 MHz (MIFARE). Lee UID de 4 bytes.</p>
    <p class="mt-2"><strong>Librería:</strong> <code>miguelbalboa/MFRC522</code>. Inicializa con <code>mfrc.PCD_Init()</code> y lee con <code>PICC_ReadCardSerial()</code>.</p>
  </div>
  <div class="iot-card">
    <h3>BME280</h3>
    <p>Mide temperatura, humedad y presión. Interfaz I²C (dirección 0x76 o 0x77). Mejor precisión que DHT22.</p>
    <p class="mt-2"><strong>Librería:</strong> <code>adafruit/Adafruit BME280 Library</code>. Ideal para estaciones meteorológicas.</p>
  </div>
</div>

---

# Guía de actuadores

<div class="iot-grid cols-2 mt-5 small">
  <div class="iot-card">
    <h3>Servo SG90 / MG90S</h3>
    <p>PWM: pulso de 1–2 ms cada 20 ms. Ángulo 0°–180°. <strong>Alimentación:</strong> si usa más de uno, fuente externa 5 V.</p>
    <p class="mt-2"><strong>Librería:</strong> <code>ESP32Servo</code> (adapta la API de Arduino a ESP32).</p>
  </div>
  <div class="iot-card">
    <h3>Relé 5 V</h3>
    <p>Controla cargas AC/DC. Módulo con optoacoplador aísla el ESP32. <strong>Pines:</strong> VCC, GND, IN (HIGH activa).</p>
    <p class="mt-2"><strong>NO/NC:</strong> normalmente abierto (cierra al activar) o normalmente cerrado (abre al activar).</p>
  </div>
  <div class="iot-card">
    <h3>Buzzer pasivo</h3>
    <p>Genera tonos con PWM. Frecuencia 200–5000 Hz para sonidos audibles. <strong>No use buzzer activo</strong> (solo hace "beep").</p>
    <p class="mt-2"><strong>Librería:</strong> <code>ledcWriteTone()</code> en ESP32 para generar frecuencia variable.</p>
  </div>
  <div class="iot-card">
    <h3>OLED SSD1306</h3>
    <p>128×64 px, I²C (0x3C). Muestra texto, gráficos, barras. <strong>Librería:</strong> <code>Adafruit_SSD1306</code> + <code>Adafruit_GFX</code>.</p>
    <p class="mt-2"><strong>Uso:</strong> <code>oled.clearDisplay()</code>, <code>oled.setCursor(x,y)</code>, <code>oled.print()</code>, <code>oled.display()</code>.</p>
  </div>
</div>

---

# PlatformIO paso a paso

<div class="timeline mt-4">
  <div class="time">1</div><div class="event"><strong>Instalar VS Code.</strong> Descarga desde <code>code.visualstudio.com</code>.</div>
  <div class="time">2</div><div class="event"><strong>Extensión PlatformIO.</strong> Busca "PlatformIO IDE" en el marketplace de VS Code. Reinicia VS Code.</div>
  <div class="time">3</div><div class="event"><strong>Nuevo proyecto.</strong> PlatformIO Home → New Project. Board: "ESP32 Dev Module". Framework: "Arduino".</div>
  <div class="time">4</div><div class="event"><strong>Estructura.</strong> <code>platformio.ini</code> (configuración), <code>src/main.cpp</code> (firmware), <code>lib/</code> (librerías locales).</div>
  <div class="time">5</div><div class="event"><strong>Compilar.</strong> Clic en "✓" (Build) o <code>Ctrl+Alt+B</code>. PlatformIO descarga dependencias automáticamente.</div>
  <div class="time">6</div><div class="event"><strong>Subir.</strong> Clic en "→" (Upload) o <code>Ctrl+Alt+U</code>. Conecta el ESP32 por USB.</div>
  <div class="time">7</div><div class="event"><strong>Monitor serie.</strong> Clic en el enchufe (Serial Monitor) o <code>Ctrl+Alt+M</code>. Configura 115200 baud.</div>
</div>

<div class="callout mt-5"><p><strong>Ventajas sobre Arduino IDE:</strong> autocompletado, depuración, múltiples placas, control de versiones con Git, CI/CD.</p></div>

<div class="callout warn mt-3"><p><strong>Espacio en disco:</strong> PlatformIO descarga toolchains y librerías en <code>~/.platformio</code>. Puede ocupar varios GB.</p></div>

---

# MicroPython paso a paso

<div class="timeline mt-4">
  <div class="time">1</div><div class="event"><strong>Descargar firmware.</strong> Ve a <code>micropython.org/downloads/esp32</code>. Descarga el archivo <code>.bin</code> más reciente para ESP32.</div>
  <div class="time">2</div><div class="event"><strong>Instalar esptool.</strong> <code>pip install esptool</code>. En Windows, usa <code>python -m pip install esptool</code>.</div>
  <div class="time">3</div><div class="event"><strong>Borrar flash.</strong> <code>esptool.py --chip esp32 --port /dev/ttyUSB0 erase_flash</code>. En Windows: <code>COM3</code>.</div>
  <div class="time">4</div><div class="event"><strong>Flashear.</strong> <code>esptool.py --chip esp32 --port /dev/ttyUSB0 --baud 460800 write_flash -z 0x1000 esp32-*.bin</code></div>
  <div class="time">5</div><div class="event"><strong>Thonny IDE.</strong> Descarga desde <code>thonny.org</code>. En Tools → Options → Interpreter: selecciona "MicroPython (ESP32)" y el puerto.</div>
  <div class="time">6</div><div class="event"><strong>REPL.</strong> Al conectar, verás el prompt <code>&gt;&gt;&gt;</code>. Escribe <code>import machine</code> y prueba comandos.</div>
  <div class="time">7</div><div class="event"><strong>Subir scripts.</strong> Crea <code>boot.py</code> (WiFi) y <code>main.py</code> (lógica). Guarda en el ESP32 desde Thonny.</div>
</div>

<div class="callout mt-5"><p><strong>Sistema de archivos:</strong> el ESP32 con MicroPython tiene <code>/flash</code> y opcionalmente <code>/sd</code>. Los scripts <code>boot.py</code> y <code>main.py</code> se ejecutan automáticamente.</p></div>

---

# Ejercicios graduados

<div class="iot-grid cols-2 mt-5 small">
  <div class="iot-card">
    <h3>Nivel 1 · Básico</h3>
    <ol class="mt-2">
      <li>Blink: encender/apagar LED cada segundo.</li>
      <li>Leer botón y mostrar en monitor serie.</li>
      <li>Leer DHT22 y mostrar en Serial.</li>
      <li>Leer LDR y mostrar porcentaje de brillo.</li>
    </ol>
  </div>
  <div class="iot-card">
    <h3>Nivel 2 · Conectividad</h3>
    <ol class="mt-2">
      <li>Conectar WiFi y mostrar IP.</li>
      <li>HTTP client: POST telemetría a servidor local.</li>
      <li>HTTP server: servir página HTML con estado.</li>
      <li>MQTT publish: enviar telemetría cada 5 s.</li>
    </ol>
  </div>
  <div class="iot-card">
    <h3>Nivel 3 · Integración</h3>
    <ol class="mt-2">
      <li>MQTT subscribe: recibir comando SET_LED.</li>
      <li>Servo controlado por HTTP (slider web).</li>
      <li>PIR + HTTP POST (alarma de movimiento).</li>
      <li>OLED: mostrar telemetría local.</li>
    </ol>
  </div>
  <div class="iot-card">
    <h3>Nivel 4 · Avanzado</h3>
    <ol class="mt-2">
      <li>RFID + MQTT (publicar UID de tarjeta).</li>
      <li>Relé controlado por MQTT (cerradura).</li>
      <li>ACK con requestId (comando + confirmación).</li>
      <li>Sistema completo: sensores + actuadores + dashboard.</li>
    </ol>
  </div>
</div>

<div class="callout mt-5"><p><strong>Sugerencia:</strong> complete los 4 niveles en orden. Cada nivel construye sobre el anterior. No salte al nivel 4 sin dominar el 2.</p></div>

---

# Node‑RED en 5 minutos

<div class="iot-grid cols-2 mt-5">
  <div class="iot-card">
    <h3>Instalación con Docker</h3>
    <div class="terminal mt-2"><span class="prompt">$</span> cd examples/node-red<br><span class="prompt">$</span> docker compose up -d<br>Node-RED listo en http://localhost:1880</div>
  </div>
  <div class="iot-card">
    <h3>Qué hace</h3>
    <p>Herramienta visual para automatizar flujos MQTT/HTTP. Arrastra nodos, conecta, despliega.</p>
    <p class="mt-2"><strong>Uso:</strong> suscribirse a tópicos, filtrar, transformar JSON, publicar alertas, integrar con APIs.</p>
  </div>
</div>

<div class="callout mt-5"><p><strong>Flow de ejemplo:</strong> <code>examples/node-red-flow.json</code>. Importe en Node-RED (Import → Clipboard). Suscribe a telemetría, alerta si temp > 30 °C, recibe comandos HTTP.</p></div>

<div class="callout warn mt-3"><p>Node‑RED no reemplaza un backend completo para producción. Es ideal para prototipos rápidos, automatización visual y pruebas.</p></div>

---

# Home Assistant como hub local

<div class="iot-grid cols-2 mt-5">
  <div class="iot-card cyan">
    <h3>Instalación</h3>
    <p>En Raspberry Pi: <code>pip3 install homeassistant</code> o Docker. Interfaz web en <code>http://raspberrypi.local:8123</code>.</p>
  </div>
  <div class="iot-card green">
    <h3>Integración MQTT</h3>
    <p>Configuración → Devices & Services → Add Integration → MQTT. Apunta a tu broker (localhost:1883).</p>
  </div>
</div>

<div class="flow mt-6">
  <div class="flow-step"><b>ESP32</b><small>MQTT</small></div><div class="flow-arrow">→</div>
  <div class="flow-step"><b>Broker</b><small>local</small></div><div class="flow-arrow">→</div>
  <div class="flow-step"><b>Home Assistant</b><small>automatizaciones</small></div><div class="flow-arrow">→</div>
  <div class="flow-step"><b>Dashboard</b><small>app móvil/web</small></div>
</div>

<div class="callout mt-5"><p><strong>Ventaja:</strong> dashboards profesionales, automatizaciones visuales, integración con Google Home/Alexa, todo local.</p></div>

<div class="callout warn mt-3"><p>Home Assistant puede ser pesado para una Raspberry Pi 3. Recomendado: Raspberry Pi 4 con SSD externa.</p></div>

---

# Deep sleep en ESP32

```cpp
#define uS_TO_S_FACTOR 1000000ULL
#define TIME_TO_SLEEP  10

void setup() {
  Serial.begin(115200);
  Serial.println("Antes de dormir...");
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  esp_deep_sleep_start();
}

void loop() {
  // Nunca llega aquí
}
```

<div class="iot-grid cols-3 mt-5">
  <div class="iot-card"><h3>Consumo</h3><p>~10 µA en deep sleep (sin WiFi).</p></div>
  <div class="iot-card"><h3>Despertar</h3><p>Timer, GPIO externo, touch pad, ULP.</p></div>
  <div class="iot-card"><h3>Uso típico</h3><p>Estaciones a batería: mide, publica, duerme.</p></div>
</div>

<div class="callout warn mt-5"><p><strong>Limitación:</strong> pierde el estado de RAM. El firmware arranca desde cero. Úselo solo si la aplicación lo permite.</p></div>

---
layout: center
---

# Archivos de imagen opcionales

<p class="lead">Si deseas sustituir los marcos visuales por fotografías, agrega estos archivos en <code>public/images/</code>:</p>

```text
esp32-board.webp
esp8266-board.webp
raspberry-pi.webp
kit-basico.webp
platformio-window.webp
thonny-window.webp
pir-sensor.webp
ultrasonic-sensor.webp
ldr-resistor.webp
mq2-sensor.webp
rfid-mfrc522.webp
servo-sg90.webp
relay-module.webp
oled-display.webp
buzzer.webp
wiring-diagram-1.webp
wiring-diagram-2.webp
mqtt-broker-diagram.webp
iot-system-diagram.webp
rfid-turnstile.webp
fingerprint-access.webp
```

<p class="small muted mt-6">El deck no depende de ellos para compilar o presentarse. Usa formato WebP para optimizar tamaño. Resolución recomendada: 1280×720 o superior.</p>
