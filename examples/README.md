# Ejemplos del curso IoT

Los ejemplos acompañan la presentación y están deliberadamente separados por objetivo:

- `esp32-http-server`: el ESP32 sirve una página y una API local.
- `esp32-http-client`: el ESP32 envía telemetría a Node o Python.
- `esp32-mqtt-led-dht`: telemetría, estado, control y ACK por MQTT.
- `esp8266-mqtt-led-dht`: adaptación equivalente para ESP8266.
- `http-node` y `http-python`: receptores HTTP locales.
- `mosquitto`: broker local con MQTT TCP y MQTT sobre WebSocket.

Antes de compilar, reemplaza SSID, claves, IP/host y pines. Las credenciales incluidas son únicamente didácticas. No expongas el broker del ejemplo a Internet.

## Librerías Arduino

- PubSubClient
- ArduinoJson
- DHT sensor library
- Adafruit Unified Sensor

El ejemplo usa PubSubClient por claridad. Esta biblioteca publica con QoS 0; el panel envía comandos con QoS 1 y el `requestId` permite confirmar el efecto mediante ACK. Para requerir QoS 1 también desde el dispositivo, usa un cliente que lo soporte, como ESP-MQTT en ESP-IDF.
