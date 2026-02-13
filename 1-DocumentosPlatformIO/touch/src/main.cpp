#include <Arduino.h>

// Definir el pin conectado al puerto SIG del HW-139
const int sensorPin = 4;

void setup() {
  // Inicializar la comunicación serie
  Serial.begin(115200);
  
  // Configurar el pin del sensor como entrada
  pinMode(sensorPin, INPUT);
  
  Serial.println("Inicializando el sensor HW-139...");
  // Código adicional de inicialización si es necesario
  Serial.println("Sensor inicializado.");
}

void loop() {
  // Leer el valor del sensor
  int sensorValue = digitalRead(sensorPin);
  Serial.print("Valor del sensor: ");
  Serial.println(sensorValue);
  
  // Esperar 1 segundo antes de la siguiente lectura
  delay(1000);
}
