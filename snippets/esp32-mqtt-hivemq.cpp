#include <WiFi.h>
#include <PubSubClient.h>

// Broker público de pruebas: no requiere cuenta ni contraseña.
const char* WIFI_SSID = "TU_WIFI";
const char* WIFI_PASSWORD = "TU_CLAVE";
const char* MQTT_HOST = "broker.hivemq.com";
const int MQTT_PORT = 1883;
const char* CONTROL_TOPIC = "univalle/iot/grupo-01/led/control";
const char* TELEMETRY_TOPIC = "univalle/iot/grupo-01/led/telemetry";

WiFiClient network;
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
    String clientId = String("esp32-clase-") + String((uint32_t)ESP.getEfuseMac(), HEX);
    if (mqtt.connect(clientId.c_str())) mqtt.subscribe(CONTROL_TOPIC);
    else delay(2000);
  }
}

void setup() {
  Serial.begin(115200); pinMode(2, OUTPUT);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) delay(300);
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
