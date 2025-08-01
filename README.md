# ESP32 Web Server con Sensor BME280

Este proyecto consiste en el desarrollo de un sistema embebido basado en ESP32 que actúa como servidor web, mostrando en tiempo real los valores obtenidos de un sensor BME280 (temperatura, humedad, presión y altitud) a través de una interfaz HTML accesible desde cualquier navegador conectado a la misma red WiFi.

## Tecnologías utilizadas
- Microcontrolador ESP32
- Sensor ambiental BME280
- Lenguaje C++ (Arduino)
- Comunicación I2C
- Librerías:
  - Adafruit_BME280
  - WiFi.h
  - Wire.h

## Descripción del funcionamiento
- Lee constantemente datos del sensor BME280.
- Crea una interfaz web para visualizar los datos en una tabla HTML.
- Se conecta a una red WiFi definida por el usuario y muestra su dirección IP en el monitor serial.
- Permite consultar el estado ambiental desde cualquier navegador local conectado a la misma red.

## Interfaz web
La información se despliega en una tabla con estilo CSS que muestra:
- Temperatura en grados Celsius y Fahrenheit
- Presión en hPa
- Altitud aproximada en metros
- Humedad relativa en porcentaje

## Conexiones principales
| Componente   | Pin ESP32 |
|--------------|-----------|
| SDA (BME280) | GPIO 22   |
| SCL (BME280) | GPIO 21   |
| VCC          | 3.3V      |
| GND          | GND       |


