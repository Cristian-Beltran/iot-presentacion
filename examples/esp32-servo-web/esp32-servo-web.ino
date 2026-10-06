#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

const char* WIFI_SSID = "TU_RED";
const char* WIFI_PASS = "TU_CLAVE";

const int SERVO_PIN = 13;
Servo servo;
int currentAngle = 90;

WebServer server(80);

void handleRoot() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Control Servo ESP32</title>
  <style>
    body { font-family: system-ui; background: #0f172a; color: #e2e8f0; display: flex;
           justify-content: center; align-items: center; min-height: 100vh; margin: 0; }
    .card { background: #1e293b; padding: 2rem; border-radius: 1rem; text-align: center;
            border: 1px solid #334155; max-width: 400px; width: 90%; }
    h1 { margin: 0 0 1rem; font-size: 1.4rem; }
    .angle { font-size: 3rem; font-weight: bold; color: #38bdf8; margin: 1rem 0; }
    input[type=range] { width: 100%; accent-color: #38bdf8; }
    .buttons { display: flex; gap: 0.5rem; margin-top: 1rem; justify-content: center; }
    button { background: #334155; color: #e2e8f0; border: 1px solid #475569;
             padding: 0.5rem 1rem; border-radius: 0.5rem; cursor: pointer; font-size: 0.9rem; }
    button:hover { background: #475569; }
    .status { margin-top: 1rem; color: #94a3b8; font-size: 0.8rem; }
  </style>
</head>
<body>
  <div class="card">
    <h1>Servo ESP32</h1>
    <div class="angle" id="angleVal">)rawliteral" + String(currentAngle) + R"rawliteral(°</div>
    <input type="range" min="0" max="180" value=")rawliteral" + String(currentAngle) + R"rawliteral("
           oninput="setAngle(this.value)">
    <div class="buttons">
      <button onclick="setAngle(0)">0°</button>
      <button onclick="setAngle(90)">90°</button>
      <button onclick="setAngle(180)">180°</button>
    </div>
    <div class="status" id="status">Listo</div>
  </div>
  <script>
    function setAngle(a) {
      fetch('/api/servo?angle=' + a)
        .then(r => r.json())
        .then(d => {
          document.getElementById('angleVal').textContent = d.angle + '°';
          document.querySelector('input').value = d.angle;
          document.getElementById('status').textContent = 'Ángulo: ' + d.angle + '°';
        });
    }
  </script>
</body>
</html>
)rawliteral";
  server.send(200, "text/html", html);
}

void handleServo() {
  if (!server.hasArg("angle")) {
    server.send(400, "application/json", "{\"error\":\"Falta angulo\"}");
    return;
  }
  int angle = server.arg("angle").toInt();
  angle = constrain(angle, 0, 180);
  currentAngle = angle;
  servo.write(currentAngle);
  Serial.printf("Servo -> %d°\n", currentAngle);

  String json = "{\"angle\":" + String(currentAngle) + "}";
  server.send(200, "application/json", json);
}

void setup() {
  Serial.begin(115200);
  servo.attach(SERVO_PIN);
  servo.write(currentAngle);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Conectando WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.printf("\nIP: %s\n", WiFi.localIP().toString().c_str());

  server.on("/", handleRoot);
  server.on("/api/servo", handleServo);
  server.begin();
  Serial.println("Servidor HTTP iniciado");
}

void loop() {
  server.handleClient();
}
