#include <WiFi.h>
#include <HTTPClient.h>

// Práctica: el ESP32 inventa una lectura de sensor y la envía a Node por HTTP.
const char* WIFI_SSID = "TU_WIFI";
const char* WIFI_PASSWORD = "TU_CLAVE";
const char* SERVER = "http://IP_DEL_PC:3000";
const int LED_PIN = 2;
unsigned long lastSend = 0;

void connectWifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) { delay(300); Serial.print('.'); }
  Serial.printf("\nWi-Fi listo: %s\n", WiFi.localIP().toString().c_str());
}

void sendFakeSensor() {
  int temperature = random(18, 32);
  int humidity = random(40, 71);
  HTTPClient http;
  http.begin(String(SERVER) + "/api/telemetry");
  http.addHeader("Content-Type", "application/json");
  String json = "{\"deviceId\":\"esp32-class\",\"temperatureC\":" + String(temperature) +
    ",\"humidityPct\":" + String(humidity) + "}";
  Serial.printf("Enviando sensor: %s\n", json.c_str());
  http.POST(json);
  http.end();
}

void askForLedState() {
  HTTPClient http;
  http.begin(String(SERVER) + "/api/control");
  if (http.GET() == 200) {
    String response = http.getString();
    bool shouldTurnOn = response.indexOf("true") >= 0;
    digitalWrite(LED_PIN, shouldTurnOn ? HIGH : LOW);
    Serial.printf("LED solicitado: %s\n", shouldTurnOn ? "ON" : "OFF");
  }
  http.end();
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  randomSeed(analogRead(34));
  connectWifi();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) connectWifi();
  if (millis() - lastSend >= 5000) {
    lastSend = millis();
    sendFakeSensor();
    askForLedState(); // El ESP32 tiene que preguntar por la orden del navegador.
  }
}
