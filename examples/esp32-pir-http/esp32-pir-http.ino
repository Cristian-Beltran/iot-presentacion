#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* WIFI_SSID = "TU_RED";
const char* WIFI_PASS = "TU_CLAVE";
const char* API_URL   = "http://192.168.1.50:3000/api/events";

const int PIR_PIN = 27;

unsigned long lastMotionSent = 0;
const unsigned long DEBOUNCE_MS = 3000;
bool lastState = LOW;

void setup() {
  Serial.begin(115200);
  pinMode(PIR_PIN, INPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Conectando WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.printf("\nWiFi conectado. IP: %s\n", WiFi.localIP().toString().c_str());
}

void loop() {
  bool motion = digitalRead(PIR_PIN);

  if (motion == HIGH && lastState == LOW && (millis() - lastMotionSent > DEBOUNCE_MS)) {
    lastMotionSent = millis();
    sendMotionEvent();
  }

  lastState = motion;
  delay(100);
}

void sendMotionEvent() {
  Serial.println("Movimiento detectado - enviando evento");

  HTTPClient http;
  http.begin(API_URL);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(5000);

  JsonDocument doc;
  doc["type"] = "motion";
  doc["deviceId"] = "esp32-pir-01";
  doc["sensorPin"] = PIR_PIN;
  doc["timestamp"] = millis();
  doc["rssi"] = WiFi.RSSI();

  String payload;
  serializeJson(doc, payload);

  int httpCode = http.POST(payload);
  Serial.printf("HTTP %d\n", httpCode);
  http.end();
}
