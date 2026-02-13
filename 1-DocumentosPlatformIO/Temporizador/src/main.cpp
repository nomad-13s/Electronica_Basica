#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define SWITCH_PIN 23
#define TOUCH_PIN 32
#define BUZZER_PIN 25
#define SDA_PIN 21
#define SCL_PIN 22

bool switchOn = false;
bool touchPressed = false;
bool lastTouchState = false;
int contador = 0;
unsigned long previousMillis = 0;
unsigned long stopMillis = 0;
unsigned long buzzerMillis = 0;
unsigned long countdownInterval = 1000;

enum EstadoSistema {
  APAGADO,
  ESPERANDO_PULSO,
  CUENTA_ATRAS,
  MOSTRANDO_STOP,
  MOSTRANDO_RESET,
  BUZZER_FINAL
};

EstadoSistema estadoActual = APAGADO;

void setup() {
  Serial.begin(115200);
  
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  pinMode(TOUCH_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  
  Wire.begin(SDA_PIN, SCL_PIN);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Error SSD1306!"));
    for(;;);
  }
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Iniciando...");
  display.display();
  delay(1500);
  
  Serial.println("=== COUNTDOWN ESP32 ===");
}

void loop() {
  unsigned long currentMillis = millis();
  
  bool switchEstadoActual = !digitalRead(SWITCH_PIN);  // LOW = OFF (funciona), HIGH = ON (sistema off)
  
  if (switchEstadoActual != switchOn) {
    switchOn = switchEstadoActual;
    Serial.printf("Switch: %s\n", switchOn ? "ON (Sistema OFF)" : "OFF (Sistema ACTIVO)");
    
    if (switchOn) {  // Switch físico ON = sistema APAGADO
      estadoActual = APAGADO;
      contador = 0;
      digitalWrite(BUZZER_PIN, LOW);
    }
  }
  
  bool touchEstadoActual = digitalRead(TOUCH_PIN);
  if (touchEstadoActual && !lastTouchState) {
    touchPressed = true;
    Serial.println("Touch!");
  }
  lastTouchState = touchEstadoActual;
  
  switch (estadoActual) {
    case APAGADO:
      // Solo cambia si el switch está físicamente OFF (switchOn = false)
      if (!switchOn) {
        estadoActual = ESPERANDO_PULSO;
      }
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(5, 20);
      display.println("Sistema OFF");
      display.display();
      break;
      
    case ESPERANDO_PULSO:
      // Verificar que switch siga OFF para continuar
      if (switchOn) {
        estadoActual = APAGADO;
        break;
      }
      
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(5, 20);
      display.println("Pulse para activar");
      display.display();
      
      if (touchPressed && !switchOn) {  // Solo si switch sigue OFF
        contador = 10;
        touchPressed = false;
        estadoActual = CUENTA_ATRAS;
        previousMillis = currentMillis;
        Serial.printf("Cuenta: %d\n", contador);
      }
      break;
      
    case CUENTA_ATRAS:
      // Parar si switch se pone ON
      if (switchOn) {
        estadoActual = APAGADO;
        digitalWrite(BUZZER_PIN, LOW);
        break;
      }
      
      if (currentMillis - previousMillis >= countdownInterval) {
        previousMillis = currentMillis;
        if (contador > 0) {
          contador--;
          Serial.printf("Cuenta: %d\n", contador);
        }
        
        if (contador <= 0) {
          estadoActual = BUZZER_FINAL;
          buzzerMillis = currentMillis;
          digitalWrite(BUZZER_PIN, HIGH);
          Serial.println("TIEMPO!!!");
        }
      }
      
      display.clearDisplay();
      display.setTextSize(4);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(45, 10);
      char buffer[8];
      sprintf(buffer, "%d", contador);
      display.println(buffer);
      display.setTextSize(1);
      display.setCursor(20, 50);
      display.println("segundos");
      display.display();
      
      if (touchPressed) {
        touchPressed = false;
        estadoActual = MOSTRANDO_STOP;
        stopMillis = currentMillis;
      }
      break;
      
    case MOSTRANDO_STOP:
      if (switchOn) {
        estadoActual = APAGADO;
        digitalWrite(BUZZER_PIN, LOW);
        break;
      }
      
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(15, 20);
      display.println("STOP");
      display.display();
      
      if (currentMillis - stopMillis >= 3000) {
        estadoActual = CUENTA_ATRAS;
        previousMillis = currentMillis;
      }
      break;
      
    case BUZZER_FINAL:
      if (switchOn) {
        digitalWrite(BUZZER_PIN, LOW);
        estadoActual = APAGADO;
        break;
      }
      
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(5, 20);
      display.println("TIEMPO!!!");
      display.display();
      
      if (currentMillis - buzzerMillis >= 3000) {
        digitalWrite(BUZZER_PIN, LOW);
        estadoActual = MOSTRANDO_RESET;
      }
      break;
      
    case MOSTRANDO_RESET:
      if (switchOn) {
        estadoActual = APAGADO;
        digitalWrite(BUZZER_PIN, LOW);
        break;
      }
      
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(5, 20);
      display.println("Resetear?");
      display.display();
      
      if (touchPressed) {
        contador = 10;
        estadoActual = CUENTA_ATRAS;
        previousMillis = currentMillis;
        touchPressed = false;
        Serial.println("Reiniciado");
      }
      break;
  }
  
  delay(50);
}
