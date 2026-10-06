#include <WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <MFRC522.h>
#include <ArduinoJson.h>

const char* WIFI_SSID = "TU_RED";
const char* WIFI_PASS = "TU_CLAVE";
const char* MQTT_HOST = "server-local.tail9af6ac.ts.net";
const int   MQTT_PORT = 1883;
const char* MQTT_USER = "device";
const char* MQTT_PASS = "esp32";

const int SS_PIN = 5;
const int RST_PIN = 22;
const char* DEVICE_ID = "esp32-rfid-01";
const char* TOPIC_ROOT = "cristian/device/esp32-rfid-01";

MFRC522 mfrc522(SS_PIN, RST_PIN);
WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);

char accessTopic[80];
char statusTopic[80];

unsigned long lastCardSent = 0;
const unsigned long DEBOUNCE_MS = 2000;

void connectWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Conectando WiFi");
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
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
      Serial.printf("fallo rc=%d\n", mqtt.state());
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();

  snprintf(accessTopic, sizeof(accessTopic), "%s/access", TOPIC_ROOT);
  snprintf(statusTopic, sizeof(statusTopic), "%s/status", TOPIC_ROOT);

  connectWiFi();
  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  connectMQTT();

  Serial.println("RFID listo - acerca una tarjeta");
}

void loop() {
  if (!mqtt.connected()) connectMQTT();
  mqtt.loop();

  if (!mfrc522.PICC_IsNewCardAvailable() || !mfrc522.PICC_ReadCardSerial()) return;

  if (millis() - lastCardSent < DEBOUNCE_MS) {
    mfrc522.PICC_HaltA();
    return;
  }
  lastCardSent = millis();

  String uid = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uid += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
    uid += String(mfrc522.uid.uidByte[i], HEX);
    if (i < mfrc522.uid.size - 1) uid += ":";
  }
  uid.toUpperCase();

  Serial.printf("Tarjeta leída: %s\n", uid.c_str());

  JsonDocument doc;
  doc["deviceId"] = DEVICE_ID;
  doc["uid"] = uid;
  doc["eventType"] = "card_read";
  doc["timestamp"] = millis();
  doc["rssi"] = WiFi.RSSI();

  String payload;
  serializeJson(doc, payload);
  mqtt.publish(accessTopic, payload.c_str(), true);

  mfrc522.PICC_HaltA();
}
