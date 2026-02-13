#include <Arduino.h>
#include <U8g2lib.h>

// Configura U8g2 para I2C software en los pines 19 (SCL) y 18 (SDA)
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(
    U8G2_R0,        // Rotación normal
    /* clock=*/ 19, // SCL (SCK)
    /* data=*/ 18,  // SDA
    U8X8_PIN_NONE   // Sin pin de reset
);

void setup() {
  Serial.begin(115200);
  delay(100);

  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(0, 20, "Hola Mundo!");
  u8g2.sendBuffer();

  Serial.println("Pantalla inicializada correctamente.");
}

void loop() {
  // No es necesario poner nada aquí para esta prueba
}
