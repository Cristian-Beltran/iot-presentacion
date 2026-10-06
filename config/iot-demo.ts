/**
 * Credencial exclusivamente didáctica.
 * Debe estar limitada por ACL a sebastian/device/esp32-01/# y revocarse
 * cuando termine el curso. Nunca reutilizarla como credencial de producción.
 */
export const IOT_DEMO_CONFIG = {
  brokerUrl: 'wss://server-local.tail9af6ac.ts.net',
  username: 'device',
  password: 'esp32',
  topicRoot: 'sebastian',
  deviceId: 'esp32-01',
} as const

export const IOT_DEMO_TOPICS = {
  telemetry: `${IOT_DEMO_CONFIG.topicRoot}/device/${IOT_DEMO_CONFIG.deviceId}/telemetry`,
  status: `${IOT_DEMO_CONFIG.topicRoot}/device/${IOT_DEMO_CONFIG.deviceId}/status`,
  control: `${IOT_DEMO_CONFIG.topicRoot}/device/${IOT_DEMO_CONFIG.deviceId}/control`,
  ack: `${IOT_DEMO_CONFIG.topicRoot}/device/${IOT_DEMO_CONFIG.deviceId}/cmd/ack`,
  alerts: `${IOT_DEMO_CONFIG.topicRoot}/device/${IOT_DEMO_CONFIG.deviceId}/alerts`,
} as const
