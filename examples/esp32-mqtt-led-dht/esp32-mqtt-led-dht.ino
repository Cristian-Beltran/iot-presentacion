#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <DHT.h>

const char* WIFI_SSID = "TU_WIFI";
const char* WIFI_PASSWORD = "TU_CLAVE";
const char* MQTT_HOST = "IP_O_HOST_DEL_BROKER";
constexpr uint16_t MQTT_PORT = 1883;
const char* MQTT_USER = "device";      // Credencial didáctica y limitada.
const char* MQTT_PASSWORD = "esp32";   // Revocar al terminar el curso.

constexpr uint8_t LED_PIN = 2;
constexpr uint8_t DHT_PIN = 4;
constexpr uint8_t DHT_TYPE = DHT22;
constexpr unsigned long TELEMETRY_EVERY_MS = 5000;

const char* STATUS_TOPIC = "sebastian/device/esp32-01/status";
const char* TELEMETRY_TOPIC = "sebastian/device/esp32-01/telemetry";
const char* CONTROL_TOPIC = "sebastian/device/esp32-01/control";
const char* ACK_TOPIC = "sebastian/device/esp32-01/cmd/ack";

WiFiClient network;
PubSubClient mqtt(network);
DHT dht(DHT_PIN, DHT_TYPE);
bool ledOn = false;
unsigned long lastTelemetry = 0;
unsigned long lastReconnectAttempt = 0;

void publishStatus(bool online) {
  JsonDocument doc;
  doc["online"] = online;
  doc["ip"] = WiFi.localIP().toString();
  doc["firmware"] = "course-1.0.0";
  char payload[192];
  serializeJson(doc, payload);
  mqtt.publish(STATUS_TOPIC, payload, true);
}

void publishAck(const char* requestId, bool ok, const char* message) {
  JsonDocument doc;
  doc["requestId"] = requestId;
  doc["ok"] = ok;
  doc["message"] = message;
  doc["ledOn"] = ledOn;
  char payload[224];
  serializeJson(doc, payload);
  mqtt.publish(ACK_TOPIC, payload, false);
}

void onMessage(char* topic, byte* bytes, unsigned int length) {
  if (strcmp(topic, CONTROL_TOPIC) != 0 || length > 512) return;
  JsonDocument doc;
  const DeserializationError error = deserializeJson(doc, bytes, length);
  if (error) return;

  const char* command = doc["command"] | "";
  const char* requestId = doc["requestId"] | "missing";
  if (strcmp(command, "SET_LED") != 0 || !doc["value"].is<bool>()) {
    publishAck(requestId, false, "invalid_command");
    return;
  }
  ledOn = doc["value"].as<bool>();
  digitalWrite(LED_PIN, ledOn ? HIGH : LOW);
  publishAck(requestId, true, "applied");
}

void connectWifi() {
  if (WiFi.status() == WL_CONNECTED) return;
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  const unsigned long deadline = millis() + 12000;
  while (WiFi.status() != WL_CONNECTED && millis() < deadline) delay(250);
}

void connectMqtt() {
  if (mqtt.connected() || millis() - lastReconnectAttempt < 3000) return;
  lastReconnectAttempt = millis();
  const String clientId = "esp32-course-" + String((uint32_t)ESP.getEfuseMac(), HEX);
  const char* offline = "{\"online\":false}";
  if (mqtt.connect(clientId.c_str(), MQTT_USER, MQTT_PASSWORD, STATUS_TOPIC, 1, true, offline)) {
    mqtt.subscribe(CONTROL_TOPIC, 1);
    publishStatus(true);
  }
}

void publishTelemetry() {
  const float t = dht.readTemperature();
  const float h = dht.readHumidity();
  if (isnan(t) || isnan(h)) return;
  JsonDocument doc;
  doc["temperatureC"] = serialized(String(t, 1));
  doc["humidityPct"] = serialized(String(h, 1));
  doc["ledOn"] = ledOn;
  doc["rssi"] = WiFi.RSSI();
  doc["uptimeMs"] = millis();
  char payload[256];
  serializeJson(doc, payload);
  mqtt.publish(TELEMETRY_TOPIC, payload, false);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  dht.begin();
  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  mqtt.setCallback(onMessage);
  mqtt.setBufferSize(768);
  connectWifi();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) connectWifi();
  connectMqtt();
  mqtt.loop();
  if (mqtt.connected() && millis() - lastTelemetry >= TELEMETRY_EVERY_MS) {
    lastTelemetry = millis();
    publishTelemetry();
  }
}
