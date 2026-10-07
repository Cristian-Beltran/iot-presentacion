from datetime import datetime, timezone
from flask import Flask, jsonify, request

app = Flask(__name__)
latest = None


@app.after_request
def allow_lab_origin(response):
    response.headers["Access-Control-Allow-Origin"] = "*"  # Solo laboratorio local.
    return response


@app.get("/")
def index():
    return f"""<!doctype html><html lang='es'><meta http-equiv='refresh' content='3'>
    <style>body{{font:18px system-ui;max-width:700px;margin:50px auto;background:#071018;color:#e6f2f7}}pre{{padding:20px;background:#102532;border-radius:14px}}</style>
    <h1>Telemetría IoT</h1><pre>{latest}</pre></html>"""


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
