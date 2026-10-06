#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <DHT.h>

#ifndef WIFI_SSID
  #define WIFI_SSID "TU_RED"
#endif
#ifndef WIFI_PASS
  #define WIFI_PASS "TU_CLAVE"
#endif
#ifndef MQTT_HOST
  #define MQTT_HOST "server-local.tail9af6ac.ts.net"
#endif
#ifndef MQTT_PORT
  #define MQTT_PORT 1883
#endif
#ifndef MQTT_USER
  #define MQTT_USER "device"
#endif
#ifndef MQTT_PASS
  #define MQTT_PASS "esp32"
#endif
#ifndef DEVICE_ID
  #define DEVICE_ID "esp32-pio-01"
#endif

#define DHT_PIN   4
#define LED_PIN   2
#define DHT_TYPE  DHT22

DHT dht(DHT_PIN, DHT_TYPE);
WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);

char topicRoot[64];
char telemetryTopic[100];
char statusTopic[100];
char controlTopic[100];
char ackTopic[100];

unsigned long lastTelemetry = 0;
const unsigned long TELEMETRY_INTERVAL = 5000;

void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.printf(" OK %s\n", WiFi.localIP().toString().c_str());
}

void onMqttMessage(char* topic, byte* payload, unsigned int length) {
  String msg;
  for (unsigned int i = 0; i < length; i++) msg += (char)payload[i];
  Serial.printf("[%s] %s\n", topic, msg.c_str());

  JsonDocument doc;
  if (deserializeJson(doc, msg)) return;

  const char* cmd = doc["command"];
  const char* reqId = doc["requestId"] | "";

  if (strcmp(cmd, "SET_LED") == 0) {
    bool val = doc["value"];
    digitalWrite(LED_PIN, val ? HIGH : LOW);

    JsonDocument ack;
    ack["command"] = "SET_LED";
    ack["value"] = val;
    ack["requestId"] = reqId;
    ack["ok"] = true;
    String out;
    serializeJson(ack, out);
    mqtt.publish(ackTopic, out.c_str());
  }
}

void connectMQTT() {
  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  mqtt.setCallback(onMqttMessage);

  while (!mqtt.connected()) {
    Serial.print("MQTT");
    String cid = String(DEVICE_ID) + "-" + String(millis());
    if (mqtt.connect(cid.c_str(), MQTT_USER, MQTT_PASS,
                     statusTopic, 1, true, "{\"online\":false}")) {
      Serial.println(" OK");
      mqtt.subscribe(controlTopic, 1);
      mqtt.publish(statusTopic, "{\"online\":true,\"fw\":\"pio-demo-1.0\"}", true);
    } else {
      Serial.printf(" rc=%d\n", mqtt.state());
      delay(5000);
    }
  }
}

void publishTelemetry() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if (isnan(temp) || isnan(hum)) {
    Serial.println("DHT error");
    return;
  }

  JsonDocument doc;
  doc["temperatureC"] = round(temp * 10.0) / 10.0;
  doc["humidityPct"] = round(hum * 10.0) / 10.0;
  doc["ledOn"] = digitalRead(LED_PIN) == HIGH;
  doc["rssi"] = WiFi.RSSI();
  doc["freeHeap"] = ESP.getFreeHeap();
  doc["uptimeMs"] = millis();

  String payload;
  serializeJson(doc, payload);
  mqtt.publish(telemetryTopic, payload.c_str());
  Serial.printf("T=%.1f H=%.1f\n", temp, hum);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  dht.begin();

  snprintf(topicRoot, sizeof(topicRoot), "sebastian/device/%s", DEVICE_ID);
  snprintf(telemetryTopic, sizeof(telemetryTopic), "%s/telemetry", topicRoot);
  snprintf(statusTopic, sizeof(statusTopic), "%s/status", topicRoot);
  snprintf(controlTopic, sizeof(controlTopic), "%s/control", topicRoot);
  snprintf(ackTopic, sizeof(ackTopic), "%s/cmd/ack", topicRoot);

  connectWiFi();
  connectMQTT();
}

void loop() {
  if (!mqtt.connected()) connectMQTT();
  mqtt.loop();

  if (millis() - lastTelemetry >= TELEMETRY_INTERVAL) {
    lastTelemetry = millis();
    publishTelemetry();
  }
}
