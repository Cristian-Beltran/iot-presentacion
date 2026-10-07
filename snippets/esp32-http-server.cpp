#include <WiFi.h>
#include <WebServer.h>

const char* WIFI_SSID = "TU_WIFI";
const char* WIFI_PASSWORD = "TU_CLAVE";
constexpr uint8_t LED_PIN = 2;

WebServer server(80);
bool ledOn = false;

// Simulación para practicar sin un DHT conectado.
// Si después instalas un DHT, estos valores se reemplazan por las lecturas del sensor.
float simulatedTemperatureC() { return random(180, 351) / 10.0f; }
float simulatedHumidityPct() { return random(350, 851) / 10.0f; }

const char PAGE[] PROGMEM = R"HTML(
<!doctype html><html lang="es"><meta name="viewport" content="width=device-width">
<style>body{font:16px system-ui;max-width:560px;margin:50px auto;background:#071018;color:#e6f2f7}button{padding:12px 20px;border:0;border-radius:10px;background:#5eead4;color:#05202a;font-weight:700}</style>
<h1>ESP32 local</h1><p id="data">Cargando…</p><button onclick="toggle()">Cambiar LED</button>
<script>
let on=false;
async function refresh(){const r=await fetch('/api/status');const d=await r.json();on=d.ledOn;data.textContent=`${d.temperatureC} °C · ${d.humidityPct} % · LED ${on?'ON':'OFF'}`}
async function toggle(){await fetch('/api/led',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify({on:!on})});refresh()}
refresh();setInterval(refresh,3000);
</script></html>)HTML";

void sendJson(int code, const String& body) {
  server.sendHeader("Access-Control-Allow-Origin", "*"); // Solo laboratorio local.
  server.send(code, "application/json", body);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  randomSeed(micros()); // Semilla para que cada lectura simulada cambie.

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print('.');
  }
  Serial.printf("\nAbre http://%s\n", WiFi.localIP().toString().c_str());

  server.on("/", HTTP_GET, [] { server.send_P(200, "text/html", PAGE); });
  server.on("/api/status", HTTP_GET, [] {
    const float temperatureC = simulatedTemperatureC(); // Antes: dht.readTemperature().
    const float humidityPct = simulatedHumidityPct();   // Antes: dht.readHumidity().
    String json = "{\"ledOn\":" + String(ledOn ? "true" : "false") +
      ",\"temperatureC\":" + String(temperatureC, 1) +
      ",\"humidityPct\":" + String(humidityPct, 1) +
      ",\"rssi\":" + String(WiFi.RSSI()) +
      ",\"uptimeMs\":" + String(millis()) + "}";
    sendJson(200, json);
  });
  server.on("/api/led", HTTP_POST, [] {
    if (!server.hasArg("plain")) return sendJson(400, "{\"error\":\"body_required\"}");
    const String body = server.arg("plain");
    if (body.indexOf("\"on\":true") >= 0) ledOn = true;
    else if (body.indexOf("\"on\":false") >= 0) ledOn = false;
    else return sendJson(400, "{\"error\":\"invalid_on\"}");
    digitalWrite(LED_PIN, ledOn ? HIGH : LOW);
    sendJson(200, String("{\"ok\":true,\"ledOn\":") + (ledOn ? "true" : "false") + "}");
  });
  server.onNotFound([] { sendJson(404, "{\"error\":\"not_found\"}"); });
  server.begin();
}

void loop() {
  server.handleClient();
}
