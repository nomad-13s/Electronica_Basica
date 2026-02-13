#include <Wire.h>
#include <Adafruit_PN532.h>

// Pines I2C ESP32 (SDA=21, SCL=22)
#define SDA_PIN 21
#define SCL_PIN 22

Adafruit_PN532 nfc(SDA_PIN, SCL_PIN);

bool detectMagicCard();

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);
  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.println("No se encontró el PN532!");
    while (1);
  }

  Serial.print("PN532 encontrado. Firmware versión: ");
  Serial.print((versiondata >> 24) & 0xFF, HEX); Serial.print('.');
  Serial.print((versiondata >> 16) & 0xFF, HEX); Serial.print('.');
  Serial.println((versiondata >> 8) & 0xFF, HEX);

  nfc.SAMConfig();
  Serial.println("Acerca una tarjeta NFC para detectar si es MAGIC o estándar...");
}

void loop() {
  uint8_t uid[7];
  uint8_t uidLength;

  if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength)) {
    Serial.print("UID de la tarjeta: ");
    for (uint8_t i = 0; i < uidLength; i++) {
      Serial.print(uid[i], HEX);
      Serial.print(" ");
    }
    Serial.println();

    if (detectMagicCard()) {
      Serial.println(">>> Tarjeta MAGIC detectada!");
    } else {
      Serial.println(">>> Tarjeta estándar detectada.");
    }
    delay(2000); // Esperar 2 segundos para la siguiente lectura
  }
}

bool detectMagicCard() {
  // Comando Backdoor para activar cambio de UID en Magic 1A Cards
  uint8_t backdoorCmd[] = {0x40};
  uint8_t response[1];
  uint8_t responseLength = 1;

  uint8_t status = nfc.inDataExchange(backdoorCmd, sizeof(backdoorCmd), response, &responseLength);

  // Respuesta válida para Magic 1A es 0x0A
  if (status == 1 && responseLength == 1 && response[0] == 0x0A) {
    return true;
  }
  return false;
}
