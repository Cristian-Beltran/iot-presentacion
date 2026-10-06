import json
import time
import machine
from machine import Pin, ADC
from umqtt.simple import MQTTClient

WIFI_SSID = "TU_RED"
WIFI_PASS = "TU_CLAVE"

MQTT_BROKER = "server-local.tail9af6ac.ts.net"
MQTT_PORT = 1883
MQTT_USER = "device"
MQTT_PASS = "esp32"
CLIENT_ID = "esp32-micropython-01"

TOPIC_ROOT = b"cristian/device/esp32-micropython-01"
TOPIC_TELEMETRY = TOPIC_ROOT + b"/telemetry"
TOPIC_STATUS = TOPIC_ROOT + b"/status"
TOPIC_CONTROL = TOPIC_ROOT + b"/control"

LED_PIN = 2
DHT_PIN = 4
LDR_PIN = 34

led = Pin(LED_PIN, Pin.OUT)
ldr = ADC(Pin(LDR_PIN))
ldr.atten(ADC.ATTN_11DB)

def connect_wifi():
    import network
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)
    if not wlan.isconnected():
        print("Conectando WiFi...")
        wlan.connect(WIFI_SSID, WIFI_PASS)
        while not wlan.isconnected():
            time.sleep(0.5)
    print("WiFi conectado:", wlan.ifconfig())

def on_message(topic, msg):
    print("MQTT [%s]: %s" % (topic.decode(), msg.decode()))
    try:
        data = json.loads(msg)
    except Exception:
        return

    command = data.get("command")
    if command == "SET_LED":
        led.value(1 if data.get("value") else 0)
        ack = json.dumps({
            "command": "SET_LED",
            "value": data.get("value"),
            "requestId": data.get("requestId", ""),
            "ok": True
        })
        client.publish(TOPIC_ROOT + b"/cmd/ack", ack.encode(), qos=1)

def connect_mqtt():
    client = MQTTClient(CLIENT_ID, MQTT_BROKER, port=MQTT_PORT,
                        user=MQTT_USER, password=MQTT_PASS)
    client.set_callback(on_message)
    client.connect()
    client.subscribe(TOPIC_CONTROL, qos=1)
    status = json.dumps({"online": True, "runtime": "micropython"})
    client.publish(TOPIC_STATUS, status.encode(), retain=True)
    print("MQTT conectado")
    return client

def publish_telemetry():
    ldr_raw = ldr.read()
    brightness = round(ldr_raw / 4095 * 100, 1)
    payload = json.dumps({
        "lightRaw": ldr_raw,
        "brightnessPct": brightness,
        "ledOn": bool(led.value()),
        "uptimeMs": time.ticks_ms()
    })
    client.publish(TOPIC_TELEMETRY, payload.encode())
    print("Telemetría:", payload)

connect_wifi()
client = connect_mqtt()

last_publish = 0
while True:
    try:
        client.check_msg()
    except Exception as e:
        print("Error check_msg:", e)

    if time.ticks_ms() - last_publish >= 5000:
        last_publish = time.ticks_ms()
        try:
            publish_telemetry()
        except Exception as e:
            print("Error publicando:", e)

    time.sleep(0.1)
