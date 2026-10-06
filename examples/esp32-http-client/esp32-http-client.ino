#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

const char* WIFI_SSID = "TU_WIFI";
const char* WIFI_PASSWORD = "TU_CLAVE";
const char* API_URL = "http://192.168.1.50:3000/api/telemetry";
constexpr uint8_t DHT_PIN = 4;
constexpr uint8_t DHT_TYPE = DHT22;
constexpr unsigned long SEND_EVERY_MS = 5000;

DHT dht(DHT_PIN, DHT_TYPE);
unsigned long lastSend = 0;

void connectWifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) delay(300);
  Serial.printf("Wi-Fi listo: %s\n", WiFi.localIP().toString().c_str());
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  connectWifi();
}

void loop() {
  if (millis() - lastSend < SEND_EVERY_MS) return;
  lastSend = millis();
  if (WiFi.status() != WL_CONNECTED) connectWifi();

  const float t = dht.readTemperature();
  const float h = dht.readHumidity();
  if (isnan(t) || isnan(h)) {
    Serial.println("Lectura DHT inválida");
    return;
  }

  WiFiClient network;
  HTTPClient http;
  http.setTimeout(4000);
  if (!http.begin(network, API_URL)) return;
  http.addHeader("Content-Type", "application/json");
  const String body = "{\"deviceId\":\"esp32-01\",\"temperatureC\":" + String(t, 1) +
    ",\"humidityPct\":" + String(h, 1) + ",\"rssi\":" + String(WiFi.RSSI()) + "}";
  const int status = http.POST(body);
  Serial.printf("POST → %d · %s\n", status, http.getString().c_str());
  http.end();
}
