#include <Arduino.h>
#include <TinyGPS++.h>

// Definimos los pines para el GPS
const int RXPin = 16, TXPin = 17;
const uint32_t GPSBaud = 9600;

// Inicializamos el objeto TinyGPS++
TinyGPSPlus gps;

// Inicializamos el puerto serial para el GPS
HardwareSerial gpsSerial(2);

void setup() {
  // Iniciamos la comunicación serial
  Serial.begin(115200);
  // Iniciamos la comunicación serial con el GPS
  gpsSerial.begin(GPSBaud, SERIAL_8N1, RXPin, TXPin);

  Serial.println("GPS Iniciado");
}

void loop() {
  // Procesamos los datos del GPS
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }

  // Si hay datos válidos del GPS, los mostramos
  if (gps.location.isValid()) {
    Serial.print("Latitud: ");
    Serial.println(gps.location.lat(), 6);
    Serial.print("Longitud: ");
    Serial.println(gps.location.lng(), 6);
    Serial.print("Altitud: ");
    Serial.println(gps.altitude.meters());
    Serial.print("Velocidad: ");
    Serial.println(gps.speed.kmph());
    Serial.print("Satélites: ");
    Serial.println(gps.satellites.value());
  } else {
    Serial.println("No se han encontrado datos válidos del GPS");
  }

  // Esperamos un poco antes de la siguiente lectura
  delay(1000);
}
