<script setup lang="ts">
import { computed, onBeforeUnmount, ref } from 'vue'
import mqtt, { type MqttClient } from 'mqtt'
import { IOT_DEMO_CONFIG, IOT_DEMO_TOPICS } from '../config/iot-demo'

type ConnectionState = 'idle' | 'connecting' | 'connected' | 'simulated' | 'error'
type EventItem = { at: string; kind: string; message: string }

const state = ref<ConnectionState>('idle')
const temperature = ref<number | null>(null)
const humidity = ref<number | null>(null)
const ledOn = ref(false)
const deviceOnline = ref(false)
const events = ref<EventItem[]>([])
let client: MqttClient | null = null

const stateLabel = computed(() => ({ idle: 'Desconectado', connecting: 'Conectando…', connected: 'Broker conectado', simulated: 'Modo simulado', error: 'Error de conexión' }[state.value]))
const stateClass = computed(() => ({ idle: '', connecting: 'waiting', connected: 'online', simulated: 'simulated', error: 'error' }[state.value]))

function log(kind: string, message: string) { events.value.unshift({ at: new Date().toLocaleTimeString(), kind, message }); events.value = events.value.slice(0, 4) }
function parse(topic: string, raw: Uint8Array) {
  try {
    const data = JSON.parse(new TextDecoder().decode(raw)) as Record<string, unknown>
    if (topic === IOT_DEMO_TOPICS.telemetry) {
      if (typeof data.temperatureC === 'number') temperature.value = data.temperatureC
      if (typeof data.humidityPct === 'number') humidity.value = data.humidityPct
      if (typeof data.ledOn === 'boolean') ledOn.value = data.ledOn
      deviceOnline.value = true; log('RX', 'Datos del ESP32 recibidos')
    } else if (topic === IOT_DEMO_TOPICS.status) { deviceOnline.value = data.online === true; log('STATUS', deviceOnline.value ? 'ESP32 disponible' : 'ESP32 desconectado')
    } else if (topic === IOT_DEMO_TOPICS.ack) { if (typeof data.ledOn === 'boolean') ledOn.value = data.ledOn; log('ACK', 'Orden confirmada') }
  } catch { log('ERROR', 'Mensaje no válido') }
}
function disconnect(setIdle = true) { if (client) { client.removeAllListeners(); client.end(true); client = null }; if (setIdle) { state.value = 'idle'; deviceOnline.value = false; log('NET', 'Desconectado') } }
function connect() {
  disconnect(false); state.value = 'connecting'; log('NET', 'Conectando al broker…')
  client = mqtt.connect(IOT_DEMO_CONFIG.brokerUrl, { username: IOT_DEMO_CONFIG.username, password: IOT_DEMO_CONFIG.password, clientId: `iot-class-${Math.random().toString(16).slice(2, 9)}`, connectTimeout: 10_000, reconnectPeriod: 2_000, clean: true })
  client.on('connect', () => { state.value = 'connected'; client?.subscribe([IOT_DEMO_TOPICS.telemetry, IOT_DEMO_TOPICS.status, IOT_DEMO_TOPICS.ack], { qos: 1 }); log('NET', 'Suscrito al ESP32') })
  client.on('message', parse); client.on('error', error => { state.value = 'error'; log('ERROR', error.message) })
}
function simulate() { disconnect(false); state.value = 'simulated'; deviceOnline.value = true; temperature.value = 24.6; humidity.value = 52; ledOn.value = false; log('DEMO', 'ESP32 simulado listo') }
function toggleLed() {
  const next = !ledOn.value; const payload = { command: 'SET_LED', value: next, requestId: `class-${Date.now()}` }
  if (state.value === 'simulated') { ledOn.value = next; log('DEMO', `LED ${next ? 'encendido' : 'apagado'}`); return }
  if (!client?.connected) { log('ERROR', 'Conecta el broker o usa Simulación'); return }
  client.publish(IOT_DEMO_TOPICS.control, JSON.stringify(payload), { qos: 1 }, error => log(error ? 'ERROR' : 'TX', error?.message ?? `SET_LED=${next}`))
}
onBeforeUnmount(() => disconnect(false))
</script>

<template>
  <div class="dashboard">
    <div class="dash-head"><div><div class="device-title">ESP32 · panel de clase</div><div class="broker">{{ IOT_DEMO_TOPICS.control }}</div></div><div class="connection" :class="stateClass"><span />{{ stateLabel }}</div></div>
    <div class="metrics"><div class="metric-card"><small>Temperatura</small><b>{{ temperature ?? '—' }}<em> °C</em></b></div><div class="metric-card"><small>Humedad</small><b>{{ humidity ?? '—' }}<em> %</em></b></div><div class="metric-card"><small>Actuador</small><b>{{ ledOn ? 'LED ON' : 'LED OFF' }}</b></div></div>
    <div class="control"><div class="lamp" :class="{ active: ledOn }">💡</div><div><b>Control remoto</b><p>La orden viaja por MQTT.</p></div><button class="primary" @click="toggleLed">{{ ledOn ? 'Apagar LED' : 'Encender LED' }}</button></div>
    <div class="events"><div v-if="!events.length" class="empty">Conecta el broker o inicia la simulación.</div><div v-for="event in events" :key="`${event.at}-${event.message}`" class="event"><span>{{ event.at }}</span><b>{{ event.kind }}</b><p>{{ event.message }}</p></div></div>
    <div class="actions"><button @click="connect">Conectar broker</button><button @click="simulate">Simulación</button><button @click="disconnect()">Desconectar</button><span class="device-state" :class="{ online: deviceOnline }">ESP32 {{ deviceOnline ? 'en línea' : 'sin señal' }}</span></div>
  </div>
</template>

<style scoped>
.dashboard{margin-top:.4rem;padding:1rem;border:1px solid rgba(94,234,212,.22);border-radius:18px;background:rgba(4,14,21,.92);color:#e6f2f7;font-size:.78rem}.dash-head,.actions,.control{display:flex;align-items:center}.dash-head{justify-content:space-between;margin-bottom:.8rem}.device-title{font-size:1.05rem;font-weight:850;color:#fff}.broker{color:#7895a5;font-family:ui-monospace,monospace;font-size:.58rem;margin-top:.2rem}.connection{display:flex;gap:.4rem;align-items:center;border:1px solid #263e4b;border-radius:99px;padding:.3rem .6rem;color:#91a8b5}.connection span,.device-state:before{content:'';width:7px;height:7px;border-radius:50%;background:#64748b;display:inline-block}.connection.online span,.device-state.online:before{background:#a3e635}.connection.waiting span{background:#fbbf24}.connection.simulated span{background:#38bdf8}.connection.error span{background:#fb7185}.metrics{display:grid;grid-template-columns:repeat(3,1fr);gap:.6rem}.metric-card{border:1px solid rgba(56,189,248,.18);background:#0c1d29;border-radius:12px;padding:.55rem .7rem}.metric-card small{display:block;color:#7895a5;text-transform:uppercase;font-size:.55rem}.metric-card b{display:block;color:#fff;font-size:1.2rem;margin-top:.15rem}.metric-card em{font-size:.65rem;color:#7895a5;font-style:normal}.control{gap:.7rem;margin-top:.6rem;padding:.65rem;border:1px solid rgba(163,230,53,.18);border-radius:12px;background:#0c1d29}.lamp{font-size:1.8rem;filter:grayscale(1)}.lamp.active{filter:none;text-shadow:0 0 18px #fbbf24}.control p{margin:.12rem 0 0;color:#7895a5;font-size:.7rem}.control button{margin-left:auto}.events{height:64px;overflow:hidden;border:1px solid rgba(94,234,212,.12);background:#07131b;border-radius:10px;padding:.25rem .5rem;margin-top:.6rem;font-family:ui-monospace,monospace}.event{display:grid;grid-template-columns:58px 44px 1fr;gap:.35rem;line-height:1.55}.event span{color:#607d8b}.event b{color:#5eead4}.event p{margin:0;white-space:nowrap;overflow:hidden;text-overflow:ellipsis;color:#aec3cd}.empty{color:#607d8b;padding:.8rem;text-align:center}.actions{gap:.45rem;margin-top:.6rem}button{border:1px solid #294653;color:#c7dbe4;border-radius:8px;padding:.35rem .62rem;background:#102734;cursor:pointer;font-size:.65rem}button.primary{background:#164e50;border-color:#2dd4bf}.device-state{margin-left:auto;color:#7895a5;display:flex;align-items:center;gap:.35rem}.device-state.online{color:#cceaa4}
</style>
