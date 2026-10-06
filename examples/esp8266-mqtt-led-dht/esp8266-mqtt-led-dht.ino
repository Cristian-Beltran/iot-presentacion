#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <DHT.h>

const char* WIFI_SSID = "TU_WIFI";
const char* WIFI_PASSWORD = "TU_CLAVE";
const char* MQTT_HOST = "IP_O_HOST_DEL_BROKER";
constexpr uint16_t MQTT_PORT = 1883;
const char* MQTT_USER = "device";
const char* MQTT_PASSWORD = "esp32";

constexpr uint8_t LED_PIN = LED_BUILTIN; // Activo en LOW en muchas placas ESP8266.
constexpr uint8_t DHT_PIN = D2;
DHT dht(DHT_PIN, DHT22);
WiFiClient network;
PubSubClient mqtt(network);
bool ledOn = false;
unsigned long lastTelemetry = 0;

const char* TELEMETRY_TOPIC = "cristian/device/esp8266-01/telemetry";
const char* STATUS_TOPIC = "cristian/device/esp8266-01/status";
const char* CONTROL_TOPIC = "cristian/device/esp8266-01/control";
const char* ACK_TOPIC = "cristian/device/esp8266-01/cmd/ack";

void onMessage(char*, byte* bytes, unsigned int length) {
  JsonDocument doc;
  if (length > 384 || deserializeJson(doc, bytes, length)) return;
  const char* requestId = doc["requestId"] | "missing";
  if (String(doc["command"] | "") != "SET_LED" || !doc["value"].is<bool>()) return;
  ledOn = doc["value"].as<bool>();
  digitalWrite(LED_PIN, ledOn ? LOW : HIGH);
  JsonDocument ack;
  ack["requestId"] = requestId; ack["ok"] = true; ack["ledOn"] = ledOn;
  char payload[128]; serializeJson(ack, payload);
  mqtt.publish(ACK_TOPIC, payload);
}

void connectAll() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) delay(250);
  }
  if (!mqtt.connected()) {
    const String id = "esp8266-course-" + String(ESP.getChipId(), HEX);
    if (mqtt.connect(id.c_str(), MQTT_USER, MQTT_PASSWORD, STATUS_TOPIC, 1, true, "{\"online\":false}")) {
      mqtt.subscribe(CONTROL_TOPIC, 1);
      mqtt.publish(STATUS_TOPIC, "{\"online\":true,\"firmware\":\"course-1.0.0\"}", true);
    }
  }
}

void setup() {
  Serial.begin(115200); pinMode(LED_PIN, OUTPUT); digitalWrite(LED_PIN, HIGH); dht.begin();
  mqtt.setServer(MQTT_HOST, MQTT_PORT); mqtt.setCallback(onMessage); connectAll();
}

void loop() {
  connectAll(); mqtt.loop();
  if (mqtt.connected() && millis() - lastTelemetry >= 5000) {
    lastTelemetry = millis();
    JsonDocument doc;
    doc["temperatureC"] = dht.readTemperature(); doc["humidityPct"] = dht.readHumidity();
    doc["ledOn"] = ledOn; doc["rssi"] = WiFi.RSSI(); doc["uptimeMs"] = millis();
    char payload[224]; serializeJson(doc, payload); mqtt.publish(TELEMETRY_TOPIC, payload);
  }
}
