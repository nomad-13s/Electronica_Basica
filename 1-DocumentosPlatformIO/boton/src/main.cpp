#include <Arduino.h>

// Definimos el pin del interruptor
const int switchPin = 5; // D5

void setup() {
  // Iniciamos la comunicación serial
  Serial.begin(115200);
  // Configuramos el pin del interruptor como entrada
  pinMode(switchPin, INPUT);
}

void loop() {
  // Leemos el estado del interruptor
  int switchState = digitalRead(switchPin);
  // Imprimimos el estado en el monitor serial
  if (switchState == HIGH) {
    Serial.println("Interruptor ON");
  } else {
    Serial.println("Interruptor OFF");
  }
  // Esperamos un poco antes de la siguiente lectura
  delay(500);
}
