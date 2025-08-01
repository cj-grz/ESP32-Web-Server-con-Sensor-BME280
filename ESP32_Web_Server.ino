/****************************************************************
 * Práctica 5 Justin Valdez
 * ======================
 * Servidor WEB con ESP32
 * Parte 2. Se envía la información del sensor BME280 por el
 * servidor WEB de la ESP32 para su consulta en cualquier
 * navegador que tenga la dirección IP de la ESP32
 * Program: P5b.ino
 * Date : Agosto, 2025
 **************************************************************/

#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // Sensor BME280 por I2C

// Credenciales del hotspot o red Wi-Fi
const char* ssid = "****";
const char* password = "******";

// Servidor web en puerto 80
WiFiServer server(80);
String header;

unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);

  // Inicia I2C en GPIO 22 (SDA), GPIO 21 (SCL)
  Wire.begin(22, 21);

  bool status = bme.begin(0x76);  // Dirección I2C común
  if (!status) {
    Serial.println("No se detecta sensor BME280 válido. Verifica el cableado.");
    while (1);
  }

  // Conecta al WiFi
  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado.");
  Serial.println("Dirección IP: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("Nuevo cliente.");
    String currentLine = "";

    while (client.connected() && currentTime - previousTime <= timeoutTime) {
      currentTime = millis();

      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        header += c;

        if (c == '\n') {
          if (currentLine.length() == 0) {
            // Cabecera HTTP
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // HTML y tabla de sensor
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>body{text-align:center;font-family:\"Trebuchet MS\",Arial;}");
            client.println("table{border-collapse:collapse;width:35%;margin-left:auto;margin-right:auto;}");
            client.println("th{padding:12px;background-color:#0043af;color:white;}");
            client.println("tr{border:1px solid #ddd;padding:12px;}");
            client.println("tr:hover{background-color:#bcbcbc;}");
            client.println("td{border:none;padding:12px;}");
            client.println(".sensor{color:white;font-weight:bold;background-color:#bcbcbc;padding:1px;}");
            client.println("</style></head><body><h1>ESP32 con BME280</h1>");

            client.println("<table><tr><th>Parámetro</th><th>Valor</th></tr>");

            client.print("<tr><td>Temp. Celsius</td><td><span class=\"sensor\">");
            client.print(bme.readTemperature());
            client.println(" *C</span></td></tr>");

            client.print("<tr><td>Temp. Fahrenheit</td><td><span class=\"sensor\">");
            client.print(1.8 * bme.readTemperature() + 32);
            client.println(" *F</span></td></tr>");

            client.print("<tr><td>Presión</td><td><span class=\"sensor\">");
            client.print(bme.readPressure() / 100.0F);
            client.println(" hPa</span></td></tr>");

            client.print("<tr><td>Altitud Aproximada</td><td><span class=\"sensor\">");
            client.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
            client.println(" m</span></td></tr>");

            client.print("<tr><td>Humedad</td><td><span class=\"sensor\">");
            client.print(bme.readHumidity());
            client.println(" %</span></td></tr>");

            client.println("</table></body></html>");
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }

    header = "";
    client.stop();
    Serial.println("Cliente desconectado.");
    Serial.println("");
  }
}
