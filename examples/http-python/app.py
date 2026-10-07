from datetime import datetime, timezone
from pathlib import Path

from flask import Flask, jsonify, request, send_from_directory

# La misma página HTML que entrega Node también se utiliza con Flask.
SHARED_WEB_DIR = Path(__file__).resolve().parents[1] / "http-node" / "public"
app = Flask(__name__)
latest = None
led_on = False


@app.after_request
def allow_lab_origin(response):
    response.headers["Access-Control-Allow-Origin"] = "*"  # Solo laboratorio local.
    return response


@app.get("/")
def index():
    return send_from_directory(SHARED_WEB_DIR, "index.html")


@app.get("/api/telemetry/latest")
def get_latest():
    return jsonify(latest) if latest else (jsonify(error="no_data"), 404)


@app.post("/api/telemetry")
def telemetry():
    global latest
    payload = request.get_json(silent=True) or {}
    required = ("deviceId", "temperatureC", "humidityPct")
    if not all(key in payload for key in required):
        return jsonify(error="invalid_payload"), 400
    latest = {**payload, "receivedAt": datetime.now(timezone.utc).isoformat()}
    print("telemetry", latest, flush=True)
    return jsonify(accepted=True, latest=latest), 201


@app.post("/api/control/toggle")
def toggle_led():
    global led_on
    led_on = not led_on
    return jsonify(ledOn=led_on)


@app.get("/api/control")
def get_control():
    return jsonify(ledOn=led_on)
