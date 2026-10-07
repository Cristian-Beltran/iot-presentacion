# IoT: conexión entre hardware y software

Presentación Slidev en español para dos clases de dos horas sobre conexión inalámbrica entre hardware y software. Cubre ESP32, Wi‑Fi, HTTP, MQTT, Mosquitto y WebSockets; el brazo robótico educativo aparece como proyecto final.

## Ejecutar la presentación

```bash
npm install
npm run dev
```

Slidev abrirá normalmente `http://localhost:3030`. Rutas útiles:

- `/#/inicio`: clase 1.
- `/#/clase-2`: clase 2.
- `/#/anexo`: material técnico.
- `http://localhost:3030/presenter`: vista del presentador con notas.

Para validar o generar una versión desplegable:

```bash
npm run build
npm run export
```

## Demostración MQTT

El panel de la clase 2 monitorea y controla el ESP32 mediante MQTT.js sobre WSS; también dispone de un modo simulado. La configuración está centralizada en `config/iot-demo.ts`.

La cuenta incluida es didáctica: debe estar restringida por ACL a `cristian/device/esp32-01/#`, ser revocable y no reutilizarse en producción. Un sitio público expone cualquier credencial incorporada en JavaScript.

## Prácticas

Consulta `examples/README.md` para ejecutar:

- ESP32 como servidor y cliente HTTP.
- ESP32 con MQTT para telemetría y control.
- Brazo robótico como proyecto final.
- Mosquitto local con TCP y WebSocket.

Las imágenes son opcionales. Si se agregan, sus nombres esperados aparecen al final del anexo de la presentación.

## Nota de compilación

`vite.config.ts` desactiva únicamente la minificación CSS porque la combinación actual de Vite 8, Lightning CSS y el CSS generado por UnoCSS produce un selector inválido durante esa optimización. El resto del build de producción permanece activo.
