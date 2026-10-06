#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* WIFI_SSID = "TU_RED";
const char* WIFI_PASS = "TU_CLAVE";
const char* MQTT_HOST = "server-local.tail9af6ac.ts.net";
const int   MQTT_PORT = 1883;
const char* MQTT_USER = "device";
const char* MQTT_PASS = "esp32";

const int RELAY_PIN = 26;
const char* DEVICE_ID = "esp32-relay-01";
const char* TOPIC_ROOT = "cristian/device/esp32-relay-01";

bool relayState = false;

WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);

char controlTopic[80];
char telemetryTopic[80];
char statusTopic[80];
char ackTopic[80];

void connectWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Conectando WiFi");
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.printf("\nIP: %s\n", WiFi.localIP().toString().c_str());
}

void onMessage(char* topic, byte* payload, unsigned int length) {
  String msg;
  for (unsigned int i = 0; i < length; i++) msg += (char)payload[i];

  Serial.printf("MQTT [%s]: %s\n", topic, msg.c_str());

  JsonDocument doc;
  DeserializationError err = deserializeJson(doc, msg);
  if (err) {
    Serial.printf("JSON invalido: %s\n", err.c_str());
    return;
  }

  const char* command = doc["command"];
  const char* requestId = doc["requestId"] | "";

  if (strcmp(command, "SET_RELAY") == 0) {
    relayState = doc["value"].as<bool>();
    digitalWrite(RELAY_PIN, relayState ? HIGH : LOW);

    JsonDocument ack;
    ack["command"] = "SET_RELAY";
    ack["value"] = relayState;
    ack["requestId"] = requestId;
    ack["ok"] = true;
    ack["timestamp"] = millis();

    String ackPayload;
    serializeJson(ack, ackPayload);
    mqtt.publish(ackTopic, ackPayload.c_str());

    Serial.printf("Relé -> %s (req: %s)\n", relayState ? "ON" : "OFF", requestId);
  }
}

void connectMQTT() {
  while (!mqtt.connected()) {
    Serial.print("Conectando MQTT...");
    String clientId = String(DEVICE_ID) + "-" + String(millis());

    if (mqtt.connect(clientId.c_str(), MQTT_USER, MQTT_PASS,
                     statusTopic, 1, true, "{\"online\":false}")) {
      Serial.println("ok");
      mqtt.subscribe(controlTopic, 1);
      mqtt.publish(statusTopic, "{\"online\":true,\"relay\":false}", true);
    } else {
      Serial.printf("fallo rc=%d\n", mqtt.state());
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  snprintf(controlTopic, sizeof(controlTopic), "%s/control", TOPIC_ROOT);
  snprintf(telemetryTopic, sizeof(telemetryTopic), "%s/telemetry", TOPIC_ROOT);
  snprintf(statusTopic, sizeof(statusTopic), "%s/status", TOPIC_ROOT);
  snprintf(ackTopic, sizeof(ackTopic), "%s/cmd/ack", TOPIC_ROOT);

  connectWiFi();
  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  mqtt.setCallback(onMessage);
  connectMQTT();
}

void loop() {
  if (!mqtt.connected()) connectMQTT();
  mqtt.loop();
}
