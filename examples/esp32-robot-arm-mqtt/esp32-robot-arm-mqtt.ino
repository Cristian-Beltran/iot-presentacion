#include <WiFi.h>
#include <WebSocketsClient.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <ESP32Servo.h>

// Reemplaza solo la red Wi-Fi. El broker es el mismo que usa el panel web de la clase.
const char* WIFI_SSID = "TU_RED";
const char* WIFI_PASS = "TU_CLAVE";
const char* MQTT_HOST = "server-local.tail9af6ac.ts.net";
const char* MQTT_USER = "device";
const char* MQTT_PASS = "esp32";
const char* CONTROL_TOPIC = "cristian/robot-arm/robot-arm-01/control";
const char* STATUS_TOPIC = "cristian/robot-arm/robot-arm-01/status";

class WebSocketBridge : public Client {
public:
  void setSocket(WebSocketsClient* value) { socket = value; }
  void setConnected(bool value) { connectedToSocket = value; }
  void add(const uint8_t* data, size_t size) { for (size_t i = 0; i < size; i++) if ((head + 1) % sizeof(buffer) != tail) { buffer[head] = data[i]; head = (head + 1) % sizeof(buffer); } }
  int connect(const char*, uint16_t) override { return 1; }
  int connect(IPAddress, uint16_t) override { return 1; }
  size_t write(uint8_t value) override { return write(&value, 1); }
  size_t write(const uint8_t* data, size_t size) override { return connectedToSocket && socket ? (socket->sendBIN(data, size), size) : 0; }
  int available() override { return head >= tail ? head - tail : sizeof(buffer) - tail + head; }
  int read() override { if (!available()) return -1; uint8_t value = buffer[tail]; tail = (tail + 1) % sizeof(buffer); return value; }
  int read(uint8_t* data, size_t size) override { size_t count = 0; while (count < size && available()) data[count++] = read(); return count; }
  int peek() override { return available() ? buffer[tail] : -1; }
  void flush() override {} void stop() override { connectedToSocket = false; }
  uint8_t connected() override { return connectedToSocket; }
  operator bool() override { return connectedToSocket; }
private:
  WebSocketsClient* socket = nullptr; uint8_t buffer[2048]; size_t head = 0, tail = 0; bool connectedToSocket = false;
};

Servo base, hombro, codo, garra;
int baseAngle = 90, hombroAngle = 90, codoAngle = 90, garraAngle = 40;
WebSocketsClient webSocket; WebSocketBridge bridge; PubSubClient mqtt(bridge);
unsigned long lastMqttTry = 0;

void moveArm(int b, int h, int c, int g) { baseAngle = constrain(b, 0, 180); hombroAngle = constrain(h, 0, 180); codoAngle = constrain(c, 0, 180); garraAngle = constrain(g, 0, 180); base.write(baseAngle); hombro.write(hombroAngle); codo.write(codoAngle); garra.write(garraAngle); }
void publishStatus() { JsonDocument json; json["online"] = true; json["base"] = baseAngle; json["hombro"] = hombroAngle; json["codo"] = codoAngle; json["garra"] = garraAngle; char output[180]; serializeJson(json, output); mqtt.publish(STATUS_TOPIC, output); }

void mqttCallback(char*, byte* raw, unsigned int length) {
  JsonDocument json; if (deserializeJson(json, raw, length)) return;
  String pose = json["pose"] | "";
  if (pose == "inicio") moveArm(90, 90, 90, 40);
  else if (pose == "bajar") moveArm(90, 120, 55, 75);
  else if (pose == "tomar") moveArm(90, 120, 55, 20);
  else if (pose == "soltar") moveArm(130, 80, 105, 75);
  else {
    String joint = json["joint"] | ""; int angle = json["angle"] | 90;
    if (joint == "base") base.write(baseAngle = constrain(angle, 0, 180));
    if (joint == "hombro") hombro.write(hombroAngle = constrain(angle, 0, 180));
    if (joint == "codo") codo.write(codoAngle = constrain(angle, 0, 180));
    if (joint == "garra") garra.write(garraAngle = constrain(angle, 0, 180));
  }
  publishStatus();
}

void socketEvent(WStype_t type, uint8_t* payload, size_t length) {
  if (type == WStype_CONNECTED) { bridge.setConnected(true); lastMqttTry = 0; Serial.println("[WSS] conectado"); }
  else if (type == WStype_DISCONNECTED) { bridge.setConnected(false); if (mqtt.connected()) mqtt.disconnect(); }
  else if (type == WStype_BIN) bridge.add(payload, length);
}

void ensureMqtt() {
  if (!bridge.connected() || mqtt.connected() || millis() - lastMqttTry < 5000) return;
  lastMqttTry = millis();
  if (mqtt.connect("robot-arm-01", MQTT_USER, MQTT_PASS)) { mqtt.subscribe(CONTROL_TOPIC); publishStatus(); Serial.println("[MQTT] listo"); }
}

void setup() {
  Serial.begin(115200);
  base.attach(13); hombro.attach(14); codo.attach(25); garra.attach(26); moveArm(90, 90, 90, 40);
  WiFi.begin(WIFI_SSID, WIFI_PASS); while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print('.'); }
  bridge.setSocket(&webSocket); webSocket.beginSSL(MQTT_HOST, 443, "/", nullptr, "mqtt"); webSocket.onEvent(socketEvent); webSocket.setReconnectInterval(5000);
  mqtt.setCallback(mqttCallback); mqtt.setBufferSize(512); mqtt.setKeepAlive(30);
}

void loop() { webSocket.loop(); ensureMqtt(); if (mqtt.connected()) mqtt.loop(); }
