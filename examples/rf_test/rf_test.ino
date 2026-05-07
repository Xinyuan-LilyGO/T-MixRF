#include <Arduino.h>
#include <RadioLib.h>
#include "pin.h"

// #define LR1121_TEST
// #define CC1101_TEST
#define NRF24_TEST

extern void lr1121_init(void);
extern void lr1121_loop(void);

extern void cc1101_init(void);
extern void cc1101_loop(void);

extern void nfr24_init(void);
extern void nrf24_loop(void);

void setup()
{
    Serial.begin(115200);

#if defined(LR1121_TEST)

    // pinMode(LR1121_CS, OUTPUT);
    // pinMode(LR1121_RST, OUTPUT);
    pinMode(CC1101_CS, OUTPUT);
    pinMode(NRF24L01_CE, OUTPUT);
    pinMode(NRF24L01_CS, OUTPUT);
    pinMode(ST25R3916_BSS, OUTPUT);
    pinMode(ST25R3916_EN, OUTPUT);

    // digitalWrite(LR1121_CS, HIGH);
    // digitalWrite(LR1121_RST, LOW);
    digitalWrite(CC1101_CS, HIGH);
    digitalWrite(NRF24L01_CS, HIGH);
    digitalWrite(NRF24L01_CE, HIGH);
    digitalWrite(ST25R3916_BSS, HIGH);
    digitalWrite(ST25R3916_EN, LOW);

    // SPI.setFrequency(100000);
    SPI.begin(BOARD_SPI_SCK, BOARD_SPI_MISO, BOARD_SPI_MOSI, LR1121_CS);

    pinMode(LR1121_SW2, OUTPUT);
    digitalWrite(LR1121_SW2, HIGH);

    lr1121_init();
#elif defined(CC1101_TEST)

    pinMode(LR1121_CS, OUTPUT);
    pinMode(LR1121_RST, OUTPUT);
    // pinMode(CC1101_CS, OUTPUT);
    pinMode(NRF24L01_CE, OUTPUT);
    pinMode(NRF24L01_CS, OUTPUT);
    pinMode(ST25R3916_BSS, OUTPUT);
    pinMode(ST25R3916_EN, OUTPUT);

    digitalWrite(LR1121_CS, HIGH);
    digitalWrite(LR1121_RST, LOW);
    // digitalWrite(CC1101_CS, HIGH);
    digitalWrite(NRF24L01_CS, HIGH);
    digitalWrite(NRF24L01_CE, HIGH);
    digitalWrite(ST25R3916_BSS, HIGH);
    digitalWrite(ST25R3916_EN, LOW);

    // SPI.setFrequency(100000);
    SPI.begin(BOARD_SPI_SCK, BOARD_SPI_MISO, BOARD_SPI_MOSI, CC1101_CS);

    cc1101_init();
#elif defined(NRF24_TEST)
    pinMode(LR1121_CS, OUTPUT);
    pinMode(LR1121_RST, OUTPUT);
    pinMode(CC1101_CS, OUTPUT);
    // pinMode(NRF24L01_CE, OUTPUT);
    // pinMode(NRF24L01_CS, OUTPUT);
    pinMode(ST25R3916_BSS, OUTPUT);
    pinMode(ST25R3916_EN, OUTPUT);

    digitalWrite(LR1121_CS, HIGH);
    digitalWrite(LR1121_RST, LOW);
    digitalWrite(CC1101_CS, HIGH);
    // digitalWrite(NRF24L01_CS, HIGH);
    // digitalWrite(NRF24L01_CE, HIGH);
    digitalWrite(ST25R3916_BSS, HIGH);
    digitalWrite(ST25R3916_EN, LOW);

    // SPI.setFrequency(100000);
    SPI.begin(BOARD_SPI_SCK, BOARD_SPI_MISO, BOARD_SPI_MOSI, NRF24L01_CS);

    nfr24_init();
#endif
}

void loop()
{
#if defined(LR1121_TEST)
    lr1121_loop();
#elif defined(CC1101_TEST)
    cc1101_loop();
#elif defined(NRF24_TEST)
    nrf24_loop();
#endif
}
