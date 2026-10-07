# Ejemplos del curso

Los ejemplos siguen el orden de la presentación: primero conexión Wi‑Fi y HTTP, luego MQTT/WebSockets y por último robótica.

- `esp32-http-server`: ESP32 como servidor web; entrega una página y una API local para un LED y un sensor.
- `esp32-http-client`: ESP32 como cliente HTTP; envía telemetría a Node o Python.
- `esp32-http-random-sensor`: práctica sin sensor físico; manda números aleatorios a Node y consulta el botón de LED.
- `http-node` y `http-python`: servidores sencillos que reciben telemetría.
- `esp32-mqtt-led-dht`: ESP32 que publica datos y recibe control por MQTT.
- `esp32-mqtt-hivemq`: práctica con un cluster gratuito de HiveMQ Cloud y TLS.
- `mosquitto`: broker local con MQTT TCP y MQTT sobre WebSockets.
- `esp32-servo-web`: brazo robótico de cuatro servos controlado desde una web local.
- `esp32-robot-arm-mqtt`: brazo robótico controlado por MQTT; es el punto de partida del proyecto final.

Antes de compilar, reemplaza SSID, claves, IP/host y GPIO por los de tu montaje.

## Librerías Arduino

- PubSubClient
- ArduinoJson
- ESP32Servo
- DHT sensor library (para ejemplos de telemetría)

## Flujo recomendado

1. Comprueba que el ESP32 entra a Wi‑Fi e imprime una IP.
2. Abre la página del ESP32 y controla el LED por HTTP.
3. Envía telemetría al servidor Node o Python.
4. Conecta el ESP32 y el panel web al broker MQTT.
5. Reutiliza ese flujo para controlar el brazo final.
# Prácticas de la presentación

## Práctica 1 · HTTP

1. Instala Arduino IDE, la placa **esp32 by Espressif Systems** y Node.js LTS.
2. En `http-node`, ejecuta `npm install` y luego `npm start`.
3. Abre `esp32-http-random-sensor.ino`, cambia Wi‑Fi e `IP_DEL_PC` y súbelo.
4. Visita `http://IP_DEL_PC:3000`. La página está en `http-node/public/index.html`.

## Práctica 2 · MQTT

1. Instala Mosquitto y la biblioteca **PubSubClient** en Arduino IDE.
2. Inicia `mosquitto -c mosquitto.conf -v`.
3. Abre `esp32-mqtt-local-dashboard.ino`, cambia Wi‑Fi y `MQTT_HOST`, y súbelo.
4. Abre `mqtt-web-dashboard/index.html` y escribe la IP de la PC broker.
