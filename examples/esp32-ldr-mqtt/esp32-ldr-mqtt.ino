#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* WIFI_SSID = "TU_RED";
const char* WIFI_PASS = "TU_CLAVE";
const char* MQTT_HOST = "server-local.tail9af6ac.ts.net";
const int   MQTT_PORT = 1883;
const char* MQTT_USER = "device";
const char* MQTT_PASS = "esp32";

const int LDR_PIN = 34;
const char* DEVICE_ID = "esp32-ldr-01";
const char* TOPIC_ROOT = "sebastian/device/esp32-ldr-01";

WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);

char telemetryTopic[80];
char statusTopic[80];

unsigned long lastPublish = 0;
const unsigned long PUBLISH_INTERVAL = 5000;

void connectWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Conectando WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.printf("\nIP: %s\n", WiFi.localIP().toString().c_str());
}

void connectMQTT() {
  while (!mqtt.connected()) {
    Serial.print("Conectando MQTT...");
    String clientId = String(DEVICE_ID) + "-" + String(millis());

    if (mqtt.connect(clientId.c_str(), MQTT_USER, MQTT_PASS,
                     statusTopic, 1, true, "{\"online\":false}")) {
      Serial.println("ok");
      mqtt.publish(statusTopic, "{\"online\":true,\"version\":\"1.0.0\"}", true);
    } else {
      Serial.printf("fallo rc=%d, reintentando en 5s\n", mqtt.state());
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  snprintf(telemetryTopic, sizeof(telemetryTopic), "%s/telemetry", TOPIC_ROOT);
  snprintf(statusTopic, sizeof(statusTopic), "%s/status", TOPIC_ROOT);

  connectWiFi();
  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  connectMQTT();
}

void loop() {
  if (!mqtt.connected()) connectMQTT();
  mqtt.loop();

  if (millis() - lastPublish >= PUBLISH_INTERVAL) {
    lastPublish = millis();
    publishTelemetry();
  }
}

void publishTelemetry() {
  int raw = analogRead(LDR_PIN);
  float voltage = raw * (3.3 / 4095.0);
  float brightnessPct = (voltage / 3.3) * 100.0;

  JsonDocument doc;
  doc["lightRaw"] = raw;
  doc["voltage"] = round(voltage * 100.0) / 100.0;
  doc["brightnessPct"] = round(brightnessPct * 10.0) / 10.0;
  doc["rssi"] = WiFi.RSSI();
  doc["uptimeMs"] = millis();

  String payload;
  serializeJson(doc, payload);

  mqtt.publish(telemetryTopic, payload.c_str());
  Serial.printf("LDR: raw=%d  volt=%.2f  bright=%.1f%%\n", raw, voltage, brightnessPct);
}
