#include <Wire.h>
#include <Adafruit_PN532.h>

// Pines I2C por defecto en ESP32: SDA=21, SCL=22
#define SDA_PIN 21
#define SCL_PIN 22

Adafruit_PN532 nfc(SDA_PIN, SCL_PIN);

void setup() {
    Serial.begin(115200);
    Wire.begin(SDA_PIN, SCL_PIN);

    nfc.begin();

    uint32_t versiondata = nfc.getFirmwareVersion();
    if (!versiondata) {
        Serial.println("No se encontró el PN532!");
        while (1); // Detener ejecución
    }
    Serial.print("Encontrado PN5"); Serial.println((versiondata>>24) & 0xFF, HEX);

    nfc.SAMConfig();
    Serial.println("Esperando una tarjeta NFC...");
}

void loop() {
    uint8_t uid[7]; // UID puede ser hasta 7 bytes
    uint8_t uidLength;

    if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength)) {
        Serial.print("UID de la tarjeta: ");
        for (uint8_t i = 0; i < uidLength; i++) {
            Serial.print(uid[i], HEX);
            Serial.print(" ");
        }
        Serial.println();
        delay(1000);
    }
}
