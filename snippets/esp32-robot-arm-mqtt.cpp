#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <ESP32Servo.h>

// Cambia Wi-Fi y el ID: cada grupo debe usar un ID diferente.
const char* WIFI_SSID = "TU_RED";
const char* WIFI_PASS = "TU_CLAVE";
const char* DEVICE_ID = "grupo-01";
const char* MQTT_HOST = "broker.hivemq.com";
const int MQTT_PORT = 1883;

char controlTopic[120];
char positionTopic[120];
char gripperTopic[120];

Servo base, hombro, codo, garra;
int baseAngle = 90, hombroAngle = 90, codoAngle = 90, garraAngle = 40;
WiFiClient network;
PubSubClient mqtt(network);

void moveArm(int b, int h, int c, int g) {
  baseAngle = constrain(b, 0, 180); hombroAngle = constrain(h, 0, 180);
  codoAngle = constrain(c, 0, 180); garraAngle = constrain(g, 0, 180);
  base.write(baseAngle); hombro.write(hombroAngle); codo.write(codoAngle); garra.write(garraAngle);
}

void publishState() {
  JsonDocument position;
  position["base"] = baseAngle; position["hombro"] = hombroAngle; position["codo"] = codoAngle;
  char positionJson[128]; serializeJson(position, positionJson);
  mqtt.publish(positionTopic, positionJson, true); // retained: la web recibe el último estado al conectar.

  JsonDocument gripper;
  gripper["angle"] = garraAngle; gripper["closed"] = garraAngle <= 40;
  char gripperJson[80]; serializeJson(gripper, gripperJson);
  mqtt.publish(gripperTopic, gripperJson, true);
}

void mqttCallback(char*, byte* raw, unsigned int length) {
  JsonDocument json; if (deserializeJson(json, raw, length)) return;
  if ((json["action"] | "") == "ping") { publishState(); return; }

  String pose = json["pose"] | "";
  if (pose == "inicio") moveArm(90, 90, 90, 40);
  else if (pose == "bajar") moveArm(90, 120, 55, 75);
  else if (pose == "tomar") moveArm(90, 120, 55, 20);
  else if (pose == "soltar") moveArm(130, 80, 105, 75);
  else {
    String joint = json["joint"] | ""; int angle = json["angle"] | 90;
    if (joint == "base") base.write(baseAngle = constrain(angle, 0, 180));
    if (joint == "hombro") hombro.write(hombroAngle = constrain(angle, 0, 180));
    if (joint == "codo") codo.write(codoAngle = constrain(angle, 0, 180));
    if (joint == "garra") garra.write(garraAngle = constrain(angle, 0, 180));
  }
  publishState();
}

void ensureMqtt() {
  while (!mqtt.connected()) {
    String clientId = String("univalle-arm-") + String((uint32_t)ESP.getEfuseMac(), HEX);
    if (mqtt.connect(clientId.c_str())) { mqtt.subscribe(controlTopic); publishState(); Serial.println("[MQTT] listo"); }
    else { Serial.printf("[MQTT] error %d; reintento en 2 s\n", mqtt.state()); delay(2000); }
  }
}

void setup() {
  Serial.begin(115200);
  snprintf(controlTopic, sizeof(controlTopic), "univalle/iot/%s/robot-arm/control", DEVICE_ID);
  snprintf(positionTopic, sizeof(positionTopic), "univalle/iot/%s/robot-arm/status/position", DEVICE_ID);
  snprintf(gripperTopic, sizeof(gripperTopic), "univalle/iot/%s/robot-arm/status/gripper", DEVICE_ID);
  base.attach(13); hombro.attach(14); codo.attach(25); garra.attach(26); moveArm(90, 90, 90, 40);
  WiFi.begin(WIFI_SSID, WIFI_PASS); while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print('.'); }
  mqtt.setServer(MQTT_HOST, MQTT_PORT); mqtt.setCallback(mqttCallback); mqtt.setBufferSize(512); mqtt.setKeepAlive(30);
}

void loop() { if (!mqtt.connected()) ensureMqtt(); mqtt.loop(); }
