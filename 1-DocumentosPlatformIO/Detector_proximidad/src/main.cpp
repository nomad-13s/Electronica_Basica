#include <Arduino.h>
#define Buzzer 19
#define Trigger 13
#define Echo 12 //Define el pin de eco del sensor HC-SR04.

const int distWarn = 120;//Define la distancia de advertencia en cm.
const int distAlarma = 60;// Define la distancia de alarma en cm.
float d;//distancia media
float t; //tiempo pulsado

void setup() {
  Serial.begin(9600);//: Inicializa la comunicación serial.
  pinMode(Buzzer, OUTPUT);//Configura el pin del buzzer como salida.
  pinMode(Trigger, OUTPUT); //Configura el pin de disparo como salida
  pinMode(Echo, INPUT);  // Configura el pin de eco como entrada.
  digitalWrite(Trigger, LOW); //  Inicializa el pin de disparo en bajo con 0
  noTone(Buzzer); // Asegura que el buzzer esté apagado al inicio.
}

void loop() {
  digitalWrite(Trigger, HIGH);//Enciende el pin de disparo.
  delayMicroseconds(10); // Mantiene el pin de disparo encendido por 10 microsegundos.
  digitalWrite(Trigger, LOW);//Apaga el pin de disparo.

  t = pulseIn(Echo, HIGH); //  Mide el tiempo del pulso de eco.
  d = t / 59; // Calcula la distancia en cm.
  Serial.println(d);//Imprime la distancia medida en el monitor serial.

  //Condiciones para activar el buzzer según la distancia medida:
  if (d <= distAlarma) {
    tone(Buzzer, 1000); //Activa el buzzer con un tono continuo si la distancia es menor o igual a distAlarma.
  } else if (d <= distWarn) {
    tone(Buzzer, 1000, 10);//Activa el buzzer con un tono intermitente si la distancia es menor o igual a distWarn.
  } else {
    noTone(Buzzer);//Apaga el buzzer si la distancia es mayor a distWarn.
  }

  delay(100);
}
/*Conexiones
    1. ESP32:
        ◦ Conecta el pin 19 al positivo del buzzer.
        ◦ Conecta el pin 13 al pin Trigger del HC-SR04.
        ◦ Conecta el pin 12 al pin Echo del HC-SR04.
    2. HC-SR04:
        ◦ Conecta VCC del HC-SR04 al 5V del ESP32.
        ◦ Conecta GND del HC-SR04 al GND del ESP32.
    3. Buzzer:
        ◦ Conecta el positivo del buzzer al pin 19 del ESP32.
        ◦ Conecta el negativo del buzzer al GND del ESP32.
Ejecución
    1. Subir el código:
        ◦ Asegúrate de que el puerto correcto esté seleccionado en PlatformIO.
        ◦ Haz clic en "Upload" para compilar y cargar el programa en el ESP32.
    2. Monitor de serie:
        ◦ Abre el monitor de serie en PlatformIO para ver las lecturas de distancia.*/