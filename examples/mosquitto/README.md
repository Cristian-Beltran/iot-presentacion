# Mosquitto local en Windows

Esta práctica crea un broker MQTT local para la clase. El ESP32 usa el puerto `1883`; una página web puede usar WebSockets en el puerto `9001`.

## Instalar

1. Descarga el instalador de Windows desde [mosquitto.org/download](https://mosquitto.org/download/).
2. Instálalo y abre PowerShell dentro de la carpeta de instalación, o agrega Mosquitto al `PATH`.
3. Comprueba que funciona:

```powershell
mosquitto -v
```

## Prueba de mensajes

Abre tres terminales:

```powershell
# 1. Broker
mosquitto -v

# 2. Suscriptor
mosquitto_sub -h localhost -t "clase/#" -v

# 3. Publicador
mosquitto_pub -h localhost -t "clase/led" -m "hola"
```

## WebSockets para páginas web

Ejecuta Mosquitto con la configuración incluida:

```powershell
mosquitto -c mosquitto.conf -v
```

La página o MQTT.js debe usar `ws://IP_DE_LA_PC:9001`. Para una práctica de aula, usa la IP local de la computadora que ejecuta Mosquitto; no uses esta configuración abierta en Internet.
