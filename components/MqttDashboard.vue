<script setup lang="ts">
import { computed, onBeforeUnmount, ref } from 'vue'
import mqtt, { type MqttClient } from 'mqtt'
import { IOT_DEMO_CONFIG, IOT_DEMO_TOPICS } from '../config/iot-demo'

type ConnectionState = 'idle' | 'connecting' | 'connected' | 'simulated' | 'error'
type EventItem = { at: string; kind: string; message: string }

const state = ref<ConnectionState>('idle')
const temperature = ref<number | null>(null)
const humidity = ref<number | null>(null)
const rssi = ref<number | null>(null)
const ledOn = ref(false)
const deviceOnline = ref(false)
const lastSeen = ref<string>('—')
const events = ref<EventItem[]>([])
const simulationRequested = ref(false)

let client: MqttClient | null = null
let simulationTimer: ReturnType<typeof setInterval> | null = null

const stateLabel = computed(() => ({
  idle: 'Desconectado',
  connecting: 'Conectando…',
  connected: 'Broker conectado',
  simulated: 'Modo simulado',
  error: 'Error de conexión',
}[state.value]))

const stateClass = computed(() => ({
  idle: '', connecting: 'waiting', connected: 'online', simulated: 'simulated', error: 'error',
}[state.value]))

function log(kind: string, message: string) {
  events.value.unshift({ at: new Date().toLocaleTimeString(), kind, message })
  events.value = events.value.slice(0, 5)
}

function parseMessage(topic: string, raw: Uint8Array) {
  try {
    const payload = JSON.parse(new TextDecoder().decode(raw)) as Record<string, unknown>
    lastSeen.value = new Date().toLocaleTimeString()
    if (topic === IOT_DEMO_TOPICS.telemetry) {
      if (typeof payload.temperatureC === 'number') temperature.value = payload.temperatureC
      if (typeof payload.humidityPct === 'number') humidity.value = payload.humidityPct
      if (typeof payload.rssi === 'number') rssi.value = payload.rssi
      if (typeof payload.ledOn === 'boolean') ledOn.value = payload.ledOn
      deviceOnline.value = true
      log('RX', `Telemetría · ${temperature.value ?? '—'} °C`)
    } else if (topic === IOT_DEMO_TOPICS.status) {
      deviceOnline.value = payload.online === true
      log('STATUS', deviceOnline.value ? 'Dispositivo online' : 'Dispositivo offline')
    } else if (topic === IOT_DEMO_TOPICS.ack) {
      if (typeof payload.ledOn === 'boolean') ledOn.value = payload.ledOn
      log('ACK', `${payload.ok === false ? 'Rechazado' : 'Confirmado'} · ${String(payload.requestId ?? '')}`)
    } else {
      log('ALERT', String(payload.message ?? 'Alerta recibida'))
    }
  } catch {
    log('ERROR', `JSON inválido en ${topic.split('/').at(-1)}`)
  }
}

function stopSimulation() {
  if (simulationTimer) clearInterval(simulationTimer)
  simulationTimer = null
}

function startSimulation() {
  disconnect(false)
  simulationRequested.value = true
  state.value = 'simulated'
  deviceOnline.value = true
  temperature.value = 24.4
  humidity.value = 52.1
  rssi.value = -57
  lastSeen.value = new Date().toLocaleTimeString()
  log('DEMO', 'Simulación iniciada')
  simulationTimer = setInterval(() => {
    temperature.value = Number((24 + Math.random() * 1.4).toFixed(1))
    humidity.value = Number((50 + Math.random() * 4).toFixed(1))
    rssi.value = -54 - Math.floor(Math.random() * 8)
    lastSeen.value = new Date().toLocaleTimeString()
    log('RX', `Telemetría simulada · ${temperature.value} °C`)
  }, 3500)
}

function connect() {
  stopSimulation()
  simulationRequested.value = false
  if (client) client.end(true)
  state.value = 'connecting'
  log('NET', `Conectando a ${new URL(IOT_DEMO_CONFIG.brokerUrl).host}`)

  client = mqtt.connect(IOT_DEMO_CONFIG.brokerUrl, {
    username: IOT_DEMO_CONFIG.username,
    password: IOT_DEMO_CONFIG.password,
    clientId: `iot-slide-${Math.random().toString(16).slice(2, 10)}`,
    connectTimeout: 10_000,
    reconnectPeriod: 2_000,
    clean: true,
  })

  client.on('connect', () => {
    state.value = 'connected'
    log('NET', 'Sesión MQTT establecida')
    client?.subscribe(Object.values(IOT_DEMO_TOPICS).filter(topic => topic !== IOT_DEMO_TOPICS.control), { qos: 1 })
  })
  client.on('reconnect', () => {
    state.value = 'connecting'
    log('NET', 'Reconectando…')
  })
  client.on('message', (topic, payload) => parseMessage(topic, payload))
  client.on('error', (error) => {
    state.value = 'error'
    log('ERROR', error.message)
  })
  client.on('offline', () => {
    if (!simulationRequested.value) state.value = 'connecting'
  })
}

function disconnect(setIdle = true) {
  stopSimulation()
  simulationRequested.value = false
  if (client) {
    client.removeAllListeners()
    client.end(true)
    client = null
  }
  if (setIdle) {
    state.value = 'idle'
    deviceOnline.value = false
    log('NET', 'Desconectado')
  }
}

function toggleLed() {
  const next = !ledOn.value
  const requestId = globalThis.crypto?.randomUUID?.() ?? `slide-${Date.now()}`
  const payload = {
    command: 'SET_LED',
    value: next,
    requestId,
    timestamp: new Date().toISOString(),
  }
  if (state.value === 'simulated') {
    ledOn.value = next
    log('ACK', `${next ? 'Encendido' : 'Apagado'} · ${requestId.slice(0, 8)}`)
    return
  }
  if (!client?.connected) {
    log('ERROR', 'Conecta el broker antes de enviar')
    return
  }
  client.publish(IOT_DEMO_TOPICS.control, JSON.stringify(payload), { qos: 1 }, (error) => {
    log(error ? 'ERROR' : 'TX', error?.message ?? `SET_LED=${next} · ${requestId.slice(0, 8)}`)
  })
}

onBeforeUnmount(() => disconnect(false))
</script>

<template>
  <div class="dashboard">
    <div class="dash-head">
      <div>
        <div class="device-title">ESP32 · {{ IOT_DEMO_CONFIG.deviceId }}</div>
        <div class="broker">{{ new URL(IOT_DEMO_CONFIG.brokerUrl).host }}</div>
      </div>
      <div class="connection" :class="stateClass"><span />{{ stateLabel }}</div>
    </div>

    <div class="metrics">
      <div class="dash-card"><small>Temperatura</small><strong>{{ temperature ?? '—' }}<em> °C</em></strong></div>
      <div class="dash-card"><small>Humedad</small><strong>{{ humidity ?? '—' }}<em> %</em></strong></div>
      <div class="dash-card"><small>RSSI</small><strong>{{ rssi ?? '—' }}<em> dBm</em></strong></div>
      <div class="dash-card"><small>Último dato</small><strong class="clock">{{ lastSeen }}</strong></div>
    </div>

    <div class="bottom">
      <div class="control-card">
        <div class="led" :class="{ active: ledOn }"><span /></div>
        <div><small>Actuador</small><b>LED {{ ledOn ? 'encendido' : 'apagado' }}</b></div>
        <button class="primary" @click="toggleLed">{{ ledOn ? 'Apagar' : 'Encender' }}</button>
      </div>
      <div class="events">
        <div v-if="!events.length" class="empty">Sin eventos todavía</div>
        <div v-for="event in events" :key="`${event.at}-${event.message}`" class="event-row">
          <span>{{ event.at }}</span><b>{{ event.kind }}</b><p>{{ event.message }}</p>
        </div>
      </div>
    </div>

    <div class="actions">
      <button @click="connect">Conectar broker</button>
      <button @click="startSimulation">Simulación</button>
      <button @click="disconnect()">Desconectar</button>
      <span class="device-state" :class="{ online: deviceOnline }">Dispositivo {{ deviceOnline ? 'online' : 'sin señal' }}</span>
    </div>
  </div>
</template>

<style scoped>
.dashboard { margin-top: .3rem; padding: .8rem; border: 1px solid rgba(94,234,212,.2); border-radius: 18px; background: rgba(4,14,21,.88); color: #e6f2f7; font-size: .72rem; }
.dash-head,.actions,.control-card { display:flex; align-items:center; }
.dash-head { justify-content:space-between; margin-bottom:.65rem; }
.device-title { font-size:.95rem; font-weight:800; color:#fff; }
.broker { color:#7895a5; font-family:ui-monospace,monospace; font-size:.58rem; }
.connection { display:flex; align-items:center; gap:.4rem; color:#91a8b5; border:1px solid #263e4b; border-radius:99px; padding:.25rem .55rem; }
.connection span,.device-state::before { content:''; width:7px; height:7px; border-radius:50%; background:#64748b; display:inline-block; }
.connection.online span,.device-state.online::before { background:#a3e635; box-shadow:0 0 12px rgba(163,230,53,.7); }
.connection.waiting span { background:#fbbf24; }
.connection.simulated span { background:#38bdf8; }
.connection.error span { background:#fb7185; }
.metrics { display:grid; grid-template-columns:repeat(4,1fr); gap:.55rem; }
.dash-card { border:1px solid rgba(56,189,248,.16); background:#0c1d29; border-radius:12px; padding:.55rem .65rem; }
.dash-card small,.control-card small { display:block; color:#7895a5; text-transform:uppercase; letter-spacing:.08em; font-size:.52rem; }
.dash-card strong { display:block; color:#fff; font-size:1.25rem; margin-top:.18rem; }
.dash-card strong.clock { font-size:.95rem; padding-top:.16rem; }
.dash-card em { color:#7895a5; font-size:.62rem; font-style:normal; }
.bottom { display:grid; grid-template-columns:.9fr 1.45fr; gap:.55rem; margin-top:.55rem; }
.control-card { gap:.65rem; border:1px solid rgba(163,230,53,.18); background:#0c1d29; border-radius:12px; padding:.55rem .65rem; }
.control-card b { display:block; color:#fff; margin-top:.12rem; }
.led { width:28px; height:28px; display:grid; place-items:center; border-radius:50%; background:#172733; }
.led span { width:11px; height:11px; border-radius:50%; background:#536773; }
.led.active span { background:#a3e635; box-shadow:0 0 18px rgba(163,230,53,.9); }
button { border:1px solid #294653; color:#c7dbe4; border-radius:8px; padding:.32rem .58rem; background:#102734; cursor:pointer; font-size:.62rem; }
button:hover { border-color:#5eead4; color:#fff; }
button.primary { margin-left:auto; background:#164e50; border-color:#2dd4bf; }
.events { height:92px; overflow:hidden; border:1px solid rgba(94,234,212,.12); background:#07131b; border-radius:12px; padding:.35rem .55rem; font-family:ui-monospace,monospace; }
.event-row { display:grid; grid-template-columns:55px 45px 1fr; gap:.35rem; border-bottom:1px solid rgba(148,163,184,.08); line-height:1.55; }
.event-row span { color:#607d8b; }.event-row b { color:#5eead4; }.event-row p { margin:0; white-space:nowrap; overflow:hidden; text-overflow:ellipsis; color:#aec3cd; }
.empty { color:#607d8b; padding:1.3rem; text-align:center; }
.actions { gap:.45rem; margin-top:.55rem; }
.device-state { margin-left:auto; color:#7895a5; display:flex; align-items:center; gap:.35rem; }
.device-state.online { color:#cceaa4; }
</style>
