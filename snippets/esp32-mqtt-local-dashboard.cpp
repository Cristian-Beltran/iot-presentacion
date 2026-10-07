#include <WiFi.h>
#include <PubSubClient.h>

// 1) Cambia estos cuatro valores antes de subir el programa.
const char* WIFI_SSID = "TU_WIFI";
const char* WIFI_PASSWORD = "TU_CLAVE";
const char* MQTT_HOST = "192.168.1.10"; // IP de la PC donde corre Mosquitto.
const int MQTT_PORT = 1883;

const char* TOPIC_TELEMETRY = "clase/esp32-01/telemetry";
const char* TOPIC_CONTROL = "clase/esp32-01/control";
constexpr uint8_t LED_PIN = 2;

WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);
bool ledOn = false;
unsigned long lastTelemetry = 0;

void publishTelemetry() {
  // Datos simulados: no se necesita sensor para esta práctica.
  const float temperatureC = random(180, 321) / 10.0;
  const float humidityPct = random(400, 801) / 10.0;
  char json[128];
  snprintf(json, sizeof(json),
    "{\"temperatureC\":%.1f,\"humidityPct\":%.1f,\"ledOn\":%s}",
    temperatureC, humidityPct, ledOn ? "true" : "false");
  mqtt.publish(TOPIC_TELEMETRY, json);
  Serial.println(json);
}

void onMqttMessage(char* topic, byte* payload, unsigned int length) {
  String message;
  for (unsigned int i = 0; i < length; i++) message += (char)payload[i];
  Serial.printf("Mensaje en %s: %s\n", topic, message.c_str());

  if (message == "ON") ledOn = true;
  if (message == "OFF") ledOn = false;
  digitalWrite(LED_PIN, ledOn ? HIGH : LOW);
  publishTelemetry(); // La página ve el cambio sin recargar.
}

void connectWifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print('.');
  }
  Serial.printf("\nWi-Fi listo. IP ESP32: %s\n", WiFi.localIP().toString().c_str());
}

void connectMqtt() {
  while (!mqtt.connected()) {
    Serial.print("Conectando a Mosquitto...");
    if (mqtt.connect("esp32-alumno-01")) {
      Serial.println(" listo");
      mqtt.subscribe(TOPIC_CONTROL);
      publishTelemetry();
    } else {
      Serial.printf(" error %d; reintento en 2 s\n", mqtt.state());
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  randomSeed(micros());
  connectWifi();
  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  mqtt.setCallback(onMqttMessage);
}

void loop() {
  if (!mqtt.connected()) connectMqtt();
  mqtt.loop();
  if (millis() - lastTelemetry >= 5000) {
    lastTelemetry = millis();
    publishTelemetry();
  }
}
