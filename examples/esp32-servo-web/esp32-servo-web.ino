#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

const char* WIFI_SSID = "TU_RED";
const char* WIFI_PASS = "TU_CLAVE";

// Cambia estos GPIO según tu montaje.
const int BASE_PIN = 13;
const int HOMBRO_PIN = 14;
const int CODO_PIN = 25;
const int GARRA_PIN = 26;

Servo base, hombro, codo, garra;
int baseAngle = 90, hombroAngle = 90, codoAngle = 90, garraAngle = 40;
WebServer server(80);

String armJson() {
  return "{\"base\":" + String(baseAngle) + ",\"hombro\":" + String(hombroAngle) +
    ",\"codo\":" + String(codoAngle) + ",\"garra\":" + String(garraAngle) + "}";
}

void moveJoint(const String& joint, int angle) {
  angle = constrain(angle, 0, 180);
  if (joint == "base") { baseAngle = angle; base.write(angle); }
  else if (joint == "hombro") { hombroAngle = angle; hombro.write(angle); }
  else if (joint == "codo") { codoAngle = angle; codo.write(angle); }
  else if (joint == "garra") { garraAngle = angle; garra.write(angle); }
}

void movePose(int b, int h, int c, int g) {
  moveJoint("base", b); moveJoint("hombro", h); moveJoint("codo", c); moveJoint("garra", g);
}

void handleRoot() {
  const char* html = R"HTML(
<!doctype html><html lang="es"><meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1">
<title>Brazo robótico ESP32</title><style>body{font-family:system-ui;background:#071018;color:#e6f2f7;max-width:680px;margin:auto;padding:24px}h1{color:#5eead4}.card{background:#102734;border:1px solid #245466;border-radius:14px;padding:14px;margin:12px 0}label{display:block;text-transform:capitalize;font-weight:bold}input{width:100%;accent-color:#5eead4}button{padding:10px;margin:4px;background:#164e50;color:white;border:1px solid #2dd4bf;border-radius:8px}#state{color:#a3e635}</style>
<h1>🦾 Brazo robótico</h1><p>Mueve una articulación o usa una pose.</p><div id="controls"></div>
<button onclick="pose('inicio')">Inicio</button><button onclick="pose('bajar')">Bajar</button><button onclick="pose('tomar')">Tomar</button><button onclick="pose('soltar')">Soltar</button><p id="state">Listo</p>
<script>const joints=['base','hombro','codo','garra'];const controls=document.querySelector('#controls');joints.forEach(j=>controls.innerHTML+=`<div class=card><label>${j} <b id=${j}v>90</b>°</label><input type=range min=0 max=180 value=90 oninput="move('${j}',this.value)"></div>`);function move(j,a){document.querySelector('#'+j+'v').textContent=a;fetch(`/api/arm?joint=${j}&angle=${a}`).then(r=>r.json()).then(()=>state.textContent=`${j}: ${a}°`)}function pose(name){fetch('/api/pose?name='+name).then(r=>r.json()).then(d=>{joints.forEach(j=>{document.querySelector('#'+j+'v').textContent=d[j]}),state.textContent='Pose: '+name})}</script>
)HTML";
  server.send(200, "text/html", html);
}

void handleArm() {
  if (!server.hasArg("joint") || !server.hasArg("angle")) { server.send(400, "application/json", "{\"error\":\"Falta joint o angle\"}"); return; }
  String joint = server.arg("joint");
  if (joint != "base" && joint != "hombro" && joint != "codo" && joint != "garra") { server.send(400, "application/json", "{\"error\":\"Articulacion desconocida\"}"); return; }
  moveJoint(joint, server.arg("angle").toInt());
  Serial.println(joint + " -> " + String(server.arg("angle").toInt()));
  server.send(200, "application/json", armJson());
}

void handlePose() {
  String name = server.arg("name");
  if (name == "inicio") movePose(90, 90, 90, 40);
  else if (name == "bajar") movePose(90, 120, 55, 75);
  else if (name == "tomar") movePose(90, 120, 55, 20);
  else if (name == "soltar") movePose(130, 80, 105, 75);
  else { server.send(400, "application/json", "{\"error\":\"Pose desconocida\"}"); return; }
  server.send(200, "application/json", armJson());
}

void setup() {
  Serial.begin(115200);
  base.attach(BASE_PIN); hombro.attach(HOMBRO_PIN); codo.attach(CODO_PIN); garra.attach(GARRA_PIN);
  movePose(baseAngle, hombroAngle, codoAngle, garraAngle);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print('.'); }
  Serial.printf("\nAbre http://%s\n", WiFi.localIP().toString().c_str());
  server.on("/", handleRoot); server.on("/api/arm", handleArm); server.on("/api/pose", handlePose);
  server.begin();
}

void loop() { server.handleClient(); }
