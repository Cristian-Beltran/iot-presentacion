---
layout: cover
class: cover
routeAlias: inicio
---

<div class="eyebrow">Clase 1 · 120 minutos</div>

# IoT: del mundo físico a la <span class="accent">web</span>

<p class="subtitle">Cómo conectar sensores, actuadores y microcontroladores con aplicaciones y servicios de software.</p>

<div class="mt-10 flex gap-3">
  <span class="chip">ESP32</span><span class="chip">ESP8266</span><span class="chip green">HTTP</span><span class="chip amber">Sensores</span><span class="chip">PlatformIO</span>
</div>

<!--
Tiempo: 3 min. Abra con una pregunta: “¿Qué objeto cotidiano deja de ser solo electrónico cuando puede enviar datos o recibir órdenes?”. Explique que hoy construiremos el puente completo, no solo un circuito.
-->

---

# Al terminar esta clase podrás…

<div class="iot-grid cols-2 mt-8">
  <div class="iot-card cyan"><h3>Leer el sistema completo</h3><p>Identificar dispositivo, red, protocolo, servicio, datos e interfaz.</p></div>
  <div class="iot-card blue"><h3>Elegir hardware</h3><p>Distinguir cuándo conviene ESP8266, ESP32 o Raspberry Pi.</p></div>
  <div class="iot-card green"><h3>Diseñar conexiones</h3><p>Separar buses eléctricos, redes y protocolos de aplicación.</p></div>
  <div class="iot-card amber"><h3>Construir con HTTP</h3><p>Usar el ESP32 como cliente y como servidor web local.</p></div>
</div>

<div class="callout mt-7"><p><strong>Producto de hoy:</strong> navegador ↔ ESP32 ↔ LED, más telemetría hacia un servidor local.</p></div>

<!--
Tiempo: 4 min. Presente los cuatro objetivos y el producto final. Aclare que el código completo queda en examples/ y que en las diapositivas se muestran solo las partes importantes.
-->

---

# Antes de hablar de IoT

<div class="iot-grid cols-3 mt-8">
  <div class="iot-card"><div class="metric">1</div><div class="metric-label">Entrada</div><p class="mt-4">Algo mide o detecta: temperatura, tarjeta, huella, movimiento.</p></div>
  <div class="iot-card"><div class="metric">2</div><div class="metric-label">Decisión</div><p class="mt-4">Un programa interpreta el dato y aplica reglas.</p></div>
  <div class="iot-card"><div class="metric">3</div><div class="metric-label">Salida</div><p class="mt-4">Algo cambia: LED, relé, cerradura, alerta o registro.</p></div>
</div>

<p class="lead mt-8">IoT añade una cuarta capacidad: <strong>compartir ese ciclo mediante una red</strong>.</p>

<!--
Tiempo: 4 min. Use un interruptor como ejemplo no conectado. Luego añada una app remota y pregunte qué nuevas piezas aparecen. Respuesta esperada: red, identidad, protocolo, servidor y seguridad.
-->

---

# La cadena IoT completa

<div class="flow">
  <div class="flow-step"><span class="icon">S</span><b>Sensor</b><small>mundo físico</small></div><div class="flow-arrow">→</div>
  <div class="flow-step"><span class="icon">µC</span><b>ESP32</b><small>firmware</small></div><div class="flow-arrow">→</div>
  <div class="flow-step"><span class="icon">Wi‑Fi</span><b>Red</b><small>IP + transporte</small></div><div class="flow-arrow">→</div>
  <div class="flow-step"><span class="icon">API</span><b>Servicio</b><small>reglas + datos</small></div><div class="flow-arrow">→</div>
  <div class="flow-step"><span class="icon">UI</span><b>Aplicación</b><small>personas</small></div>
</div>

<div class="iot-grid cols-3 mt-8">
  <div class="iot-card"><h3>Telemetría</h3><p>El dispositivo informa lo que está pasando.</p></div>
  <div class="iot-card"><h3>Comando</h3><p>El software solicita una acción verificable.</p></div>
  <div class="iot-card"><h3>Evento</h3><p>Algo relevante queda registrado y auditable.</p></div>
</div>

<!--
Tiempo: 5 min. Recorra la cadena de izquierda a derecha con el ejemplo de una estación meteorológica. Después recórrala en sentido inverso con “encender un ventilador”.
-->

---

# Hardware y software hablan distinto

<div class="iot-grid cols-2 mt-7">
  <div class="iot-card cyan">
    <h3>Mundo físico</h3>
    <p>Voltaje, corriente, ruido, tiempos, pines, buses, alimentación y fallos materiales.</p>
    <div class="rule"></div><span class="chip">GPIO</span> <span class="chip">I²C</span> <span class="chip">SPI</span>
  </div>
  <div class="iot-card blue">
    <h3>Mundo del software</h3>
    <p>JSON, rutas, usuarios, bases de datos, permisos, procesos y experiencia de usuario.</p>
    <div class="rule"></div><span class="chip">HTTP</span> <span class="chip">MQTT</span> <span class="chip">Web</span>
  </div>
</div>

<p class="lead mt-9">El firmware es el <strong>traductor</strong>; la arquitectura define cómo viaja y se protege la información.</p>

<!--
Tiempo: 3 min. Destaque que muchos errores de IoT nacen al ignorar uno de los dos mundos: un backend perfecto no corrige una alimentación inestable; un circuito perfecto no resuelve usuarios o auditoría.
-->

---
layout: center
class: section-slide
---

<div class="eyebrow">Bloque 1</div>

# ¿Por qué empezar con ESP32?

<p>Una plataforma pequeña, económica y suficientemente potente para aprender el ciclo IoT completo.</p>

<div class="section-number">01</div>

<!-- Tiempo: 1 min. Use esta diapositiva como cambio de ritmo. -->

---

# ESP32 en una sola mirada

<div class="iot-grid cols-2 mt-5">
  <div>
    <div class="iot-grid cols-2">
      <div class="iot-card"><div class="metric">Wi‑Fi</div><div class="metric-label">integrado</div></div>
      <div class="iot-card"><div class="metric">BLE</div><div class="metric-label">según familia</div></div>
      <div class="iot-card"><div class="metric">32 bit</div><div class="metric-label">MCU</div></div>
      <div class="iot-card"><div class="metric">GPIO</div><div class="metric-label">periféricos</div></div>
    </div>
    <div class="callout mt-5"><p>Puede leer sensores, controlar actuadores, cifrar conexiones y ejecutar un servidor web pequeño.</p></div>
  </div>
  <div class="image-placeholder">
    <div><strong>Fotografía opcional</strong><br><span class="tiny">public/images/esp32-board.webp</span><br><br>Mientras no exista, este marco evita dependencias externas.</div>
  </div>
</div>

<!--
Tiempo: 4 min. Muestre físicamente la placa. Señale USB, regulador, antena, módulo, pines y botones. No prometa que todos los modelos tienen exactamente los mismos periféricos.
-->

---

# ESP8266, ESP32 o Raspberry Pi

<table class="compare mt-5">
  <thead><tr><th></th><th>ESP8266</th><th>ESP32</th><th>Raspberry Pi</th></tr></thead>
  <tbody>
    <tr><td><strong>Tipo</strong></td><td>Microcontrolador</td><td>Microcontrolador</td><td>Computador Linux</td></tr>
    <tr><td><strong>Conectividad</strong></td><td>Wi‑Fi</td><td>Wi‑Fi + BLE*</td><td>Wi‑Fi, Ethernet*, USB</td></tr>
    <tr><td><strong>Tiempo de arranque</strong></td><td class="yes">Muy rápido</td><td class="yes">Muy rápido</td><td>Segundos</td></tr>
    <tr><td><strong>Consumo</strong></td><td class="yes">Bajo</td><td class="yes">Bajo</td><td class="warn">Mayor</td></tr>
    <tr><td><strong>Ideal para</strong></td><td>Nodos simples</td><td>Nodos IoT completos</td><td>Gateway, broker, BD, visión</td></tr>
    <tr><td><strong>Sistema operativo</strong></td><td>No</td><td>No</td><td>Linux</td></tr>
  </tbody>
</table>

<p class="tiny mt-4">* Depende del modelo o familia. Los precios deben verificarse localmente y mostrarse como rangos fechados.</p>

<!--
Tiempo: 6 min. Pregunte cuál usarían para una estación a batería y cuál para reconocimiento de imágenes. Explique microcontrolador versus computador de placa única.
-->

---

# Familias ESP32: no todas son iguales

<div class="iot-grid cols-4 mt-7">
  <div class="iot-card"><h3>ESP32 clásico</h3><p>Excelente base educativa; Wi‑Fi y Bluetooth clásico/BLE.</p></div>
  <div class="iot-card"><h3>ESP32‑C3</h3><p>RISC‑V, Wi‑Fi + BLE; eficiente para productos sencillos.</p></div>
  <div class="iot-card"><h3>ESP32‑S3</h3><p>Más GPIO, USB y capacidades útiles para interfaces/IA ligera.</p></div>
  <div class="iot-card"><h3>ESP32‑C6</h3><p>Wi‑Fi 6, BLE y 802.15.4 para Thread/Zigbee.</p></div>
</div>

<div class="callout warn mt-7"><p>Antes de copiar un tutorial, verifica <strong>modelo exacto, pinout, voltaje y periféricos</strong>.</p></div>

<!--
Tiempo: 4 min. El objetivo no es memorizar familias, sino aprender a leer la ficha técnica. Señale que DAC, USB, Bluetooth clásico o Zigbee cambian según variante.
-->

---

# Lenguajes y entornos

<div class="iot-grid cols-3 mt-6">
  <div class="iot-card cyan"><h3>Arduino C++</h3><p>Ruta principal del curso. Gran ecosistema, ejemplos abundantes y acceso directo al hardware.</p></div>
  <div class="iot-card blue"><h3>MicroPython</h3><p>Iteración rápida y sintaxis amigable; recursos y compatibilidad más limitados.</p></div>
  <div class="iot-card green"><h3>ESP‑IDF</h3><p>SDK oficial, FreeRTOS y control profundo para productos profesionales.</p></div>
</div>

<div class="iot-grid cols-2 mt-5">
  <div class="iot-card"><h3>Arduino IDE</h3><p>Inicio rápido y sencillo para el aula.</p></div>
  <div class="iot-card"><h3>PlatformIO</h3><p>Dependencias, entornos, pruebas y proyectos más estructurados.</p></div>
</div>

<!--
Tiempo: 4 min. Explique que elegir un lenguaje también elige librerías, depuración, memoria y flujo de trabajo. En este curso el código completo será Arduino C++.
-->

---

# Electricidad mínima para no quemar nada

<div class="iot-grid cols-3 mt-6">
  <div class="iot-card red"><h3>3.3 V lógico</h3><p>Los GPIO del ESP32 no deben recibir 5 V directamente.</p></div>
  <div class="iot-card amber"><h3>Corriente limitada</h3><p>Motores, relés y cerraduras necesitan etapa de potencia y fuente adecuada.</p></div>
  <div class="iot-card blue"><h3>Tierra común</h3><p>Los módulos deben compartir referencia GND salvo aislamiento diseñado.</p></div>
</div>

<div class="callout danger mt-7"><p><strong>Nunca</strong> conectes una cerradura, motor o carga de red eléctrica directamente a un GPIO.</p></div>

<!--
Tiempo: 5 min. Enseñe resistencia del LED y explique transistor/MOSFET, diodo flyback y relé sin profundizar en cálculo. Verifique polaridad antes de energizar.
-->

---
layout: center
class: section-slide
---

<div class="eyebrow">Bloque 1.5</div>

# Manos a la obra con el hardware

<p>Del diagrama al prototipo: sensores, actuadores y el primer firmware.</p>

<div class="section-number">01.5</div>

<!-- Tiempo: 1 min. Saquen los componentes. Vamos a reconocer cada pieza antes de programar. -->

---

# Kit básico recomendado

<div class="iot-grid cols-4 mt-5">
  <div class="iot-card cyan"><h3>Microcontrolador</h3><p>ESP32 DevKit o ESP8266 NodeMCU.</p></div>
  <div class="iot-card green"><h3>Sensores</h3><p>DHT22, PIR HC‑SR501, LDR, ultrasónico HC‑SR04.</p></div>
  <div class="iot-card amber"><h3>Actuadores</h3><p>LED, servo SG90, relé 5 V, buzzer.</p></div>
  <div class="iot-card blue"><h3>Infraestructura</h3><p>Protoboard, jumpers, resistencias, cable USB, fuente.</p></div>
</div>

<div class="image-placeholder mt-7">
  <div><strong>Fotografía opcional</strong><br><span class="tiny">public/images/kit-basico.webp</span></div>
</div>

<!--
Tiempo: 5 min. Muestre cada componente físicamente. Explique qué mide o qué controla cada uno. El objetivo es familiarizarse con el hardware antes de conectarlo.
-->

---

# Arduino IDE vs PlatformIO

<div class="iot-grid cols-2 mt-5">
  <div class="iot-card cyan">
    <h3>Arduino IDE</h3>
    <p>Inicio rápido, todo en uno, ideal para el primer blink.</p>
    <div class="rule"></div>
    <p class="tiny"><strong>Limitación:</strong> gestión manual de librerías, sin entornos múltiples, depuración limitada.</p>
  </div>
  <div class="iot-card blue">
    <h3>PlatformIO (VS Code)</h3>
    <p>Dependencias declarativas, múltiples placas, monitor serie integrado, depuración y OTA.</p>
    <div class="rule"></div>
    <p class="tiny"><strong>Recomendado:</strong> cuando el proyecto crece más allá de un sketch.</p>
  </div>
</div>

<div class="image-placeholder mt-5">
  <div><strong>Captura opcional</strong><br><span class="tiny">public/images/platformio-window.webp</span></div>
</div>

<!--
Tiempo: 4 min. Abra VS Code y muestre la estructura platformio.ini, src/main.cpp y la gestión de librerías. No configure todo en vivo; tenga un proyecto listo.
-->

---

# Estructura PlatformIO

```ini
; platformio.ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 115200
lib_deps =
    knolleary/PubSubClient@^2.8
    bblanchon/ArduinoJson@^7.0.0
    adafruit/DHT sensor library@^1.4.6
build_flags =
    -DWIFI_SSID=\"TU_RED\"
    -DWIFI_PASS=\"TU_CLAVE\"
```

<div class="iot-grid cols-3 mt-5">
  <div class="iot-card"><h3>platformio.ini</h3><p>Placa, librerías y flags.</p></div>
  <div class="iot-card"><h3>src/main.cpp</h3><p>El firmware completo.</p></div>
  <div class="iot-card"><h3>lib/</h3><p>Librerías locales opcionales.</p></div>
</div>

<!--
Tiempo: 3 min. Explique cómo las credenciales se separan del código con build_flags. Esto facilita trabajar con distintos entornos sin tocar el código.
-->

---

# Sensor PIR · detectar movimiento

<div class="iot-grid cols-2 mt-5">
  <div class="iot-card cyan">
    <h3>HC‑SR501</h3>
    <p>Digital: HIGH cuando detecta infrarrojo de cuerpo humano. Rango ajustable ~3–7 m.</p>
    <div class="rule"></div>
    <p class="tiny"><strong>Pines:</strong> VCC (5–20 V), GND, OUT (3.3 V). Ajustar potenciómetros: sensibilidad y tiempo.</p>
  </div>
  <div>

```cpp
const int PIR_PIN = 27;
bool lastState = LOW;

void setup() {
  pinMode(PIR_PIN, INPUT);
}

void loop() {
  bool motion = digitalRead(PIR_PIN);
  if (motion && !lastState) {
    Serial.println("¡Movimiento!");
  }
  lastState = motion;
  delay(100);
}
```

  </div>
</div>

<div class="image-placeholder mt-4">
  <div><strong>Fotografía opcional</strong><br><span class="tiny">public/images/pir-sensor.webp</span></div>
</div>

<!--
Tiempo: 4 min. Conecte el PIR y pruebe con el monitor serie. Explique el debounce: el sensor puede disparar varios HIGH seguidos.
-->

---

# Sensor ultrasónico · medir distancia

<div class="iot-grid cols-2 mt-5">
  <div class="iot-card green">
    <h3>HC‑SR04</h3>
    <p>Emite ultrasonido y mide el eco. Rango: 2–400 cm, precisión ~3 mm.</p>
    <div class="rule"></div>
    <p class="tiny"><strong>Pines:</strong> VCC (5 V), TRIG → GPIO, ECHO → GPIO (con divisor de voltaje 5 V→3.3 V), GND.</p>
  </div>
  <div>

```cpp
float medirDistancia() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duracion = pulseIn(ECHO, HIGH, 30000);
  if (duracion == 0) return -1;
  return duracion * 0.0343 / 2.0;
}
```

  </div>
</div>

<div class="image-placeholder mt-4">
  <div><strong>Fotografía opcional</strong><br><span class="tiny">public/images/ultrasonic-sensor.webp</span></div>
</div>

<!--
Tiempo: 4 min. Explique la fórmula: velocidad del sonido / 2. El divisor de voltaje en ECHO es obligatorio porque el HC-SR04 devuelve 5 V y el ESP32 es 3.3 V.
-->

---

# LDR · medir luz ambiente

<div class="iot-grid cols-2 mt-5">
  <div class="iot-card amber">
    <h3>Divisor de voltaje</h3>
    <p>LDR + resistencia fija (10 kΩ). El GPIO lee el voltaje proporcional a la luz.</p>
    <div class="rule"></div>
    <p class="tiny"><strong>Fórmula:</strong> V<sub>out</sub> = V<sub>CC</sub> × R<sub>ldr</sub> / (R<sub>ldr</sub> + R<sub>fija</sub>). Más luz → menos resistencia.</p>
  </div>
  <div>

```cpp
const int LDR_PIN = 34;

void setup() {
  Serial.begin(115200);
}

void loop() {
  int raw = analogRead(LDR_PIN);
  float voltaje = raw * (3.3 / 4095.0);
  float brillo = (voltaje / 3.3) * 100.0;
  Serial.printf("LDR: raw=%d %.1f%%\n",
                raw, brillo);
  delay(1000);
}
```

  </div>
</div>

<div class="image-placeholder mt-4">
  <div><strong>Fotografía opcional</strong><br><span class="tiny">public/images/ldr-resistor.webp</span></div>
</div>

<!--
Tiempo: 3 min. Muestre cómo la lectura cambia tapando el sensor con la mano. El ADC del ESP32 no es lineal en los extremos; para medición precisa usar un multímetro como referencia.
-->

---

# Servo motor · movimiento angular

<div class="iot-grid cols-2 mt-5">
  <div class="iot-card blue">
    <h3>SG90 / MG90S</h3>
    <p>Control por PWM, rango 0°–180°. Ideal para cerraduras, válvulas y brazos.</p>
    <div class="rule"></div>
    <p class="tiny"><strong>Alimentación:</strong> si usa más de un servo, fuente externa de 5 V. El GPIO solo alimenta la señal.</p>
  </div>
  <div>

```cpp
#include <ESP32Servo.h>

Servo servo;
const int SERVO_PIN = 13;

void setup() {
  servo.attach(SERVO_PIN);
  servo.write(90);
}

void loop() {
  servo.write(0);   delay(1000);
  servo.write(90);  delay(1000);
  servo.write(180); delay(1000);
}
```

  </div>
</div>

<div class="image-placeholder mt-4">
  <div><strong>Fotografía opcional</strong><br><span class="tiny">public/images/servo-sg90.webp</span></div>
</div>

<!--
Tiempo: 3 min. Conecte un servo y muévalo. Explique la diferencia entre servo (posición) y motor DC (velocidad). La librería ESP32Servo adapta la API de Arduino.
-->

---

# Relé · controlar cargas externas

<div class="iot-grid cols-2 mt-5">
  <div class="iot-card red">
    <h3>Módulo de relé</h3>
    <p>Permite que un GPIO controle cargas de mayor potencia: lámparas, cerraduras, motores.</p>
    <div class="rule"></div>
    <p class="tiny"><strong>Seguridad:</strong> los módulos con optoacoplador aíslan el ESP32 de la carga. Para cargas AC, use relés con clasificación adecuada.</p>
  </div>
  <div>

```cpp
const int RELAY_PIN = 26;

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
}

void loop() {
  digitalWrite(RELAY_PIN, HIGH);
  Serial.println("Relé ON");
  delay(3000);
  digitalWrite(RELAY_PIN, LOW);
  Serial.println("Relé OFF");
  delay(3000);
}
```

  </div>
</div>

<div class="callout danger mt-4"><p><strong>Nunca</strong> conectes el ESP32 directamente a una carga AC. Siempre usa un módulo de relé con aislamiento.</p></div>

<div class="image-placeholder mt-4">
  <div><strong>Fotografía opcional</strong><br><span class="tiny">public/images/relay-module.webp</span></div>
</div>

<!--
Tiempo: 3 min. Encienda y apague un relé. Explique NO/NC (normalmente abierto/cerrado). Para cargas AC se requiere cuidado especial; en el aula usaremos solo cargas DC.
-->

---

# Pantalla OLED · mostrar datos localmente

<div class="iot-grid cols-2 mt-5">
  <div class="iot-card">
    <h3>SSD1306 · I²C</h3>
    <p>128×64 px, 0.96". Ideal para mostrar lecturas, estado o menús sin depender del navegador.</p>
    <div class="rule"></div>
    <p class="tiny"><strong>I²C:</strong> SDA → GPIO 21, SCL → GPIO 22. Dirección: 0x3C.</p>
  </div>
  <div>

```cpp
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 oled(128, 64, &Wire, -1);

void setup() {
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  oled.clearDisplay();
  oled.setTextSize(2);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(0, 20);
  oled.print("24.5 C");
  oled.display();
}
```

  </div>
</div>

<div class="image-placeholder mt-4">
  <div><strong>Fotografía opcional</strong><br><span class="tiny">public/images/oled-display.webp</span></div>
</div>

<!--
Tiempo: 4 min. Muestre texto, números y una barra. Explique que el OLED es excelente para feedback local sin necesidad de app o navegador.
-->

---

# Ejercicio guiado 1 · estación ambiental local

<div class="timeline mt-4">
  <div class="time">0–5 min</div><div class="event"><strong>Cablear.</strong> DHT22 + LDR + OLED en la misma protoboard. Compartir 3.3 V y GND.</div>
  <div class="time">5–10 min</div><div class="event"><strong>Programar.</strong> Leer ambos sensores y mostrar temperatura, humedad y brillo en el OLED.</div>
  <div class="time">10–15 min</div><div class="event"><strong>Expandir.</strong> Añadir un LED que se encienda cuando la luminosidad baja de un umbral.</div>
  <div class="time">15–20 min</div><div class="event"><strong>Validar.</strong> Tapar el LDR, soplar el DHT, verificar que todo responde.</div>
</div>

<div class="callout mt-5"><p><strong>Criterio de éxito:</strong> la OLED muestra 3 valores en tiempo real y el LED reacciona a la luz sin reiniciar.</p></div>

<div class="callout warn mt-3"><p>Pistas: usa <code>millis()</code> en vez de <code>delay()</code> para no bloquear. Reserva los pines ADC solo para el LDR (GPIO 34 no tiene pull-up).</p></div>

<!--
Tiempo: 20 min. Circule por el aula. Si algún equipo no tiene OLED, que lo simulen con Serial. El objetivo es integrar múltiples componentes en un mismo firmware.
-->

---
layout: center
class: section-slide
---

<div class="eyebrow">Bloque 2</div>

# Tres capas de conexión

<p>No mezcles el cable del sensor, la red Wi‑Fi y el protocolo de la aplicación.</p>

<div class="section-number">02</div>

<!-- Tiempo: 1 min. -->

---

# Capa 1 · Señales y buses locales

<div class="iot-grid cols-4 mt-5">
  <div class="iot-card"><h3>GPIO</h3><p>Entradas/salidas digitales: botones, LED, relés.</p></div>
  <div class="iot-card"><h3>ADC / PWM</h3><p>Medición analógica y control proporcional.</p></div>
  <div class="iot-card"><h3>I²C</h3><p>Dos hilos, varios dispositivos, sensores y pantallas.</p></div>
  <div class="iot-card"><h3>SPI</h3><p>Mayor velocidad: RFID, SD, pantallas.</p></div>
  <div class="iot-card"><h3>UART</h3><p>Serie punto a punto: GPS, módems, depuración.</p></div>
  <div class="iot-card"><h3>1‑Wire</h3><p>Sensores simples identificables en un bus.</p></div>
  <div class="iot-card"><h3>CAN / TWAI</h3><p>Bus robusto para vehículos y control.</p></div>
  <div class="iot-card"><h3>RS‑485</h3><p>Distancia y ruido; requiere transceptor externo.</p></div>
</div>

<!--
Tiempo: 6 min. Para cada bus dé un ejemplo físico, no una definición extensa. Pregunte por qué un MFRC522 suele usar SPI y un BME280 puede usar I²C.
-->

---

# Capa 2 · Cómo llega a la red

<table class="compare mt-5">
  <thead><tr><th>Tecnología</th><th>Fortaleza</th><th>Límite</th><th>Uso típico</th></tr></thead>
  <tbody>
    <tr><td>Wi‑Fi STA</td><td>Internet y LAN</td><td>Consumo/cobertura</td><td>Hogar, aula, industria</td></tr>
    <tr><td>Wi‑Fi AP</td><td>Sin router</td><td>Acceso local</td><td>Configuración inicial</td></tr>
    <tr><td>BLE</td><td>Bajo consumo</td><td>Alcance y ancho de banda</td><td>Móvil cercano, sensores</td></tr>
    <tr><td>ESP‑NOW</td><td>Rápido, sin router</td><td>Ecosistema Espressif</td><td>Nodos y controles directos</td></tr>
    <tr><td>Ethernet</td><td>Estable y cableado</td><td>Hardware adicional</td><td>Instalación fija</td></tr>
    <tr><td>LoRa/celular</td><td>Distancia o independencia</td><td>Módulo, costo, latencia</td><td>Campo y activos remotos</td></tr>
  </tbody>
</table>

<!--
Tiempo: 6 min. Destaque que LoRa no significa automáticamente Internet: normalmente requiere gateway o servicio. Use el criterio energía-distancia-datos-costo.
-->

---

# Capa 3 · El idioma de la aplicación

<div class="iot-grid cols-3 mt-6">
  <div class="iot-card cyan"><h3>HTTP / HTTPS</h3><p>Petición y respuesta. APIs, configuración y operaciones puntuales.</p></div>
  <div class="iot-card green"><h3>MQTT / MQTTS</h3><p>Publicación y suscripción. Telemetría y comandos desacoplados.</p></div>
  <div class="iot-card blue"><h3>WebSocket / WSS</h3><p>Canal persistente bidireccional, especialmente útil en navegadores.</p></div>
  <div class="iot-card"><h3>CoAP</h3><p>Modelo REST ligero sobre UDP para dispositivos restringidos.</p></div>
  <div class="iot-card"><h3>Modbus TCP</h3><p>Integración industrial basada en registros.</p></div>
  <div class="iot-card"><h3>Cloud SDK</h3><p>Arduino Cloud, AWS IoT, Azure IoT y plataformas especializadas.</p></div>
</div>

<!--
Tiempo: 5 min. Aclare que Wi‑Fi no es HTTP y que WebSocket no es MQTT. Adelante que en la segunda clase separaremos transporte y protocolo.
-->

---

# ¿Qué camino elegir?

<div class="iot-grid cols-2 mt-6">
  <div class="iot-card"><h3>Panel local sin Internet</h3><p><strong>ESP32 AP/STA + servidor HTTP</strong><br>Simple, directo y limitado a la red.</p></div>
  <div class="iot-card"><h3>API empresarial existente</h3><p><strong>ESP32 como cliente HTTPS</strong><br>Integra autenticación y reglas del backend.</p></div>
  <div class="iot-card"><h3>Muchos dispositivos y tiempo real</h3><p><strong>MQTT + broker</strong><br>Desacopla productores y consumidores.</p></div>
  <div class="iot-card"><h3>Internet inestable</h3><p><strong>Decisión local + sincronización</strong><br>El sistema seguro no puede depender siempre de la nube.</p></div>
</div>

<!--
Tiempo: 4 min. Actividad rápida: lea cuatro escenarios y pida al grupo que levante uno, dos, tres o cuatro dedos según la arquitectura elegida.
-->

---
layout: center
class: section-slide
---

<div class="eyebrow">Bloque 3</div>

# HTTP: el primer puente

<p>Una conversación explícita entre cliente y servidor.</p>

<div class="section-number">03</div>

<!-- Tiempo: 1 min. -->

---

# Anatomía de una petición HTTP

<div class="iot-grid cols-2 mt-4">
  <div>

```http
POST /api/telemetry HTTP/1.1
Host: 192.168.1.50:3000
Content-Type: application/json

{"temperatureC":24.6,"humidityPct":51}
```

  </div>
  <div>

```http
HTTP/1.1 201 Created
Content-Type: application/json

{"accepted":true,"receivedAt":"..."}
```

  </div>
</div>

<div class="flow mt-6"><div class="flow-step"><b>Cliente</b><small>inicia</small></div><div class="flow-arrow">→</div><div class="flow-step"><b>Ruta + método</b><small>intención</small></div><div class="flow-arrow">→</div><div class="flow-step"><b>Servidor</b><small>responde</small></div></div>

<!--
Tiempo: 7 min. Identifique método, ruta, host, cabecera, cuerpo y respuesta. Aclare que HTTP no es solo páginas: también transporta datos entre máquinas.
-->

---

# Métodos y códigos que sí importan

<div class="iot-grid cols-2 mt-5">
  <div class="iot-card"><h3>GET</h3><p>Leer estado. Debe evitar efectos secundarios.</p></div>
  <div class="iot-card"><h3>POST</h3><p>Crear evento o solicitar una acción.</p></div>
  <div class="iot-card"><h3>PUT / PATCH</h3><p>Reemplazar o modificar una configuración.</p></div>
  <div class="iot-card"><h3>DELETE</h3><p>Eliminar o revocar un recurso.</p></div>
</div>

<p class="mt-6"><span class="chip green">200 OK</span> <span class="chip green">201 Created</span> <span class="chip amber">400 Bad Request</span> <span class="chip amber">401/403</span> <span class="chip">404</span> <span class="chip">500</span></p>

<!--
Tiempo: 5 min. Use /api/led como ejemplo. Pregunte por qué “GET /encender” funciona pero representa mal la intención y es fácil de activar accidentalmente.
-->

---

# ESP32 como cliente HTTP

<div class="flow">
  <div class="flow-step"><b>DHT</b><small>lectura</small></div><div class="flow-arrow">→</div>
  <div class="flow-step"><b>ESP32</b><small>JSON + POST</small></div><div class="flow-arrow">→</div>
  <div class="flow-step"><b>API local</b><small>valida</small></div><div class="flow-arrow">→</div>
  <div class="flow-step"><b>Aplicación</b><small>muestra/guarda</small></div>
</div>

```cpp
HTTPClient http;
http.begin("http://192.168.1.50:3000/api/telemetry");
http.addHeader("Content-Type", "application/json");
int status = http.POST(jsonPayload);
http.end();
```

<div class="callout warn mt-4"><p>Siempre define timeout, comprueba el código y cierra la conexión. En Internet usa HTTPS y valida el certificado.</p></div>

<!--
Tiempo: 6 min. Explique que localhost desde el ESP32 sería el propio ESP32, no el computador. Use la IP LAN del docente y compruebe que firewall y red permiten acceso.
-->

---

# ESP32 como servidor HTTP

<div class="iot-grid cols-2 mt-5">
  <div class="iot-card cyan"><h3>GET /</h3><p>Entrega una página HTML pequeña para el navegador.</p></div>
  <div class="iot-card blue"><h3>GET /api/status</h3><p>Devuelve LED, temperatura, humedad, RSSI y uptime.</p></div>
  <div class="iot-card green"><h3>POST /api/led</h3><p>Valida JSON y cambia el actuador.</p></div>
  <div class="iot-card amber"><h3>Red local</h3><p>Puede operar en modo estación o crear su propio punto de acceso.</p></div>
</div>

<p class="lead mt-7">Es ideal para configuración y control local; no reemplaza un backend cuando hay usuarios, historial o muchos dispositivos.</p>

<!--
Tiempo: 5 min. Dibuje navegador → IP del ESP32. Explique límites: RAM, concurrencia, persistencia, seguridad y exposición a Internet.
-->

---

# Servidor local con Node

<div class="terminal"><span class="prompt">$</span> cd examples/http-node<br><span class="prompt">$</span> npm install<br><span class="prompt">$</span> npm start<br>API lista en http://0.0.0.0:3000</div>

```js
app.post('/api/telemetry', (req, res) => {
  latest = { ...req.body, receivedAt: new Date().toISOString() }
  res.status(201).json({ accepted: true, latest })
})
```

<p class="small muted mt-4">Node resulta natural cuando frontend y backend comparten JavaScript/TypeScript.</p>

<!--
Tiempo: 4 min. Ejecute el servidor y pruebe primero con curl. Luego cambie la IP del sketch ESP32. Señale 0.0.0.0 versus localhost.
-->

---

# El mismo servicio con Python

<div class="terminal"><span class="prompt">$</span> cd examples/http-python<br><span class="prompt">$</span> python -m venv .venv<br><span class="prompt">$</span> pip install -r requirements.txt<br><span class="prompt">$</span> flask --app app run --host 0.0.0.0 --port 5000</div>

```python
@app.post("/api/telemetry")
def telemetry():
    payload = request.get_json(silent=True) or {}
    return {"accepted": True, "data": payload}, 201
```

<p class="small muted mt-4">Python es una ruta muy cómoda para análisis, automatización, ciencia de datos y Raspberry Pi.</p>

<!--
Tiempo: 3 min. No repita toda la explicación de HTTP. Compare únicamente ecosistema y sintaxis. El alumnado puede escoger uno de los dos servidores.
-->

---

# Práctica · navegador ↔ ESP32 ↔ LED

<div class="timeline mt-4">
  <div class="time">0–3 min</div><div class="event"><strong>Cablear.</strong> LED con resistencia y DHT; confirmar 3.3 V y GND.</div>
  <div class="time">3–7 min</div><div class="event"><strong>Configurar.</strong> SSID, clave, pin y tipo de sensor.</div>
  <div class="time">7–11 min</div><div class="event"><strong>Probar.</strong> Abrir la IP del monitor serie y cambiar el LED.</div>
  <div class="time">11–15 min</div><div class="event"><strong>Integrar.</strong> Enviar telemetría al servidor Node o Python.</div>
</div>

<div class="callout mt-5"><p><strong>Criterio de éxito:</strong> el navegador muestra estado, el botón cambia el LED y el servidor recibe JSON.</p></div>

<!--
Tiempo: 15 min. Trabaje en parejas. Si el hardware falla, pruebe primero rutas con curl y use el modo simulado. Errores frecuentes: pin incorrecto, DHT equivocado, IP equivocada, redes aisladas y firewall.
-->

---

# Depuración por capas

<div class="iot-grid cols-3 mt-6">
  <div class="iot-card"><h3>1 · Hardware</h3><p>¿Hay alimentación? ¿GND? ¿Pin correcto? ¿Lectura local?</p></div>
  <div class="iot-card"><h3>2 · Red</h3><p>¿Obtuvo IP? ¿Misma red? ¿Ping/ruta? ¿Puerto abierto?</p></div>
  <div class="iot-card"><h3>3 · Aplicación</h3><p>¿Método y ruta? ¿JSON válido? ¿Código HTTP? ¿Logs?</p></div>
</div>

<div class="terminal mt-7"><span class="prompt">$</span> curl http://IP_DEL_ESP32/api/status<br><span class="prompt">$</span> curl -X POST http://IP_DEL_ESP32/api/led -H "Content-Type: application/json" -d '{"on":true}'</div>

<!--
Tiempo: 4 min. Enseñe a no cambiar cinco cosas a la vez. La regla es encontrar la última capa que funciona y probar la siguiente con la herramienta más simple.
-->

---

# Ejercicio guiado 2 · servo desde el navegador

<div class="iot-grid cols-2 mt-4">
  <div>
    <p class="lead">El ESP32 sirve una página con un slider. Mueve el servo en tiempo real.</p>
    <div class="timeline mt-4">
      <div class="time">1</div><div class="event"><strong>Hardware:</strong> servo SG90 → GPIO 13, fuente 5 V.</div>
      <div class="time">2</div><div class="event"><strong>Código:</strong> <code>WebServer</code> + <code>ESP32Servo</code>.</div>
      <div class="time">3</div><div class="event"><strong>GET /</strong> → HTML con slider <code>range</code>.</div>
      <div class="time">4</div><div class="event"><strong>GET /api/servo?angle=90</strong> → mueve el servo + JSON.</div>
    </div>
  </div>
  <div>

```cpp
server.on("/api/servo", []() {
  int a = server.arg("angle").toInt();
  a = constrain(a, 0, 180);
  servo.write(a);
  server.send(200, "application/json",
    "{\"angle\":" + String(a) + "}");
});
```

  </div>
</div>

<div class="callout mt-4"><p><strong>Código completo:</strong> <code>examples/esp32-servo-web/</code></p></div>

<!--
Tiempo: 12 min. Ejecute el ejemplo. Abra la IP del ESP32 en el navegador. Explique que este patrón (HTML + API JSON en el mismo servidor) es la base del control local.
-->

---

# Ejercicio guiado 3 · alarma PIR vía HTTP

<div class="flow">
  <div class="flow-step"><b>PIR</b><small>detecta</small></div><div class="flow-arrow">→</div>
  <div class="flow-step"><b>ESP32</b><small>POST JSON</small></div><div class="flow-arrow">→</div>
  <div class="flow-step"><b>Servidor</b><small>registra</small></div><div class="flow-arrow">→</div>
  <div class="flow-step"><b>App</b><small>notifica</small></div>
</div>

```cpp
void sendMotionEvent() {
  HTTPClient http;
  http.begin("http://192.168.1.50:3000/api/events");
  http.addHeader("Content-Type", "application/json");

  JsonDocument doc;
  doc["type"] = "motion";
  doc["deviceId"] = "esp32-pir-01";
  doc["timestamp"] = millis();

  String payload;
  serializeJson(doc, payload);
  int code = http.POST(payload);
  Serial.printf("HTTP %d\n", code);
  http.end();
}
```

<div class="callout warn mt-4"><p>Usa debounce: evita enviar múltiples eventos por la misma detección. <code>examples/esp32-pir-http/</code></p></div>

<!--
Tiempo: 10 min. Combine el PIR con el servidor Node o Python. Muestre que el servidor recibe el evento y puede disparar acciones.
-->

---

# Cierre de la clase 1

<div class="iot-grid cols-2 mt-6">
  <div class="iot-card cyan"><h3>Ya conectamos</h3><p>Sensores, actuadores, ESP32, Wi‑Fi, HTTP, servidor local, OLED y páginas web.</p></div>
  <div class="iot-card green"><h3>La limitación</h3><p>HTTP requiere que alguien conozca al servidor y comience cada conversación.</p></div>
</div>

<p class="lead mt-9">¿Qué ocurre si hay 500 dispositivos, tres aplicaciones y necesitamos datos en tiempo real?</p>

<div class="callout mt-7"><p>En la siguiente clase insertaremos un intermediario especializado: <strong>el broker MQTT</strong>.</p></div>

<!--
Tiempo: 5 min. Pida tres aprendizajes y una duda. Deje como salida que dibujen la arquitectura HTTP de un control de acceso e identifiquen qué pasa si el servidor no está disponible.
-->
