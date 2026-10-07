#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

// Completa estos datos al crear tu cluster gratuito en HiveMQ Cloud.
const char* WIFI_SSID = "TU_WIFI";
const char* WIFI_PASSWORD = "TU_CLAVE";
const char* MQTT_HOST = "TU_CLUSTER.s1.eu.hivemq.cloud";
const int MQTT_PORT = 8883;
const char* MQTT_USER = "TU_USUARIO";
const char* MQTT_PASS = "TU_CLAVE_MQTT";
const char* CONTROL_TOPIC = "clase/control";
const char* TELEMETRY_TOPIC = "clase/telemetry";

WiFiClientSecure network;
PubSubClient mqtt(network);
unsigned long lastPublish = 0;

void onMessage(char* topic, byte* payload, unsigned int length) {
  String message;
  for (unsigned int i = 0; i < length; i++) message += char(payload[i]);
  Serial.printf("Mensaje en %s: %s\n", topic, message.c_str());
  if (message == "ON") digitalWrite(2, HIGH);
  if (message == "OFF") digitalWrite(2, LOW);
}

void connectMqtt() {
  while (!mqtt.connected()) {
    if (mqtt.connect("esp32-hivemq-class", MQTT_USER, MQTT_PASS)) mqtt.subscribe(CONTROL_TOPIC);
    else delay(2000);
  }
}

void setup() {
  Serial.begin(115200); pinMode(2, OUTPUT);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) delay(300);
  network.setInsecure(); // Para clase: en un proyecto real valida el certificado.
  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  mqtt.setCallback(onMessage);
}

void loop() {
  if (!mqtt.connected()) connectMqtt();
  mqtt.loop();
  if (millis() - lastPublish > 5000) {
    lastPublish = millis();
    String data = String("{\"randomValue\":") + random(0, 100) + "}";
    mqtt.publish(TELEMETRY_TOPIC, data.c_str());
  }
}
