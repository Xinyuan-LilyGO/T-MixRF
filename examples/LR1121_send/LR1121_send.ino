/**
 * @file      T3S3Factory.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2023-05-13
 *
 */

#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <SD.h>
#include <esp_adc_cal.h>
#include <RadioLib.h>
#include "pin.h"

void lr1121_init(void);
void lr1121_loop(void);

void setup()
{   
    Serial.begin(115200);

    // pinMode(LR1121_CS, OUTPUT);
    // pinMode(LR1121_RST, OUTPUT);
    pinMode(CC1101_CS, OUTPUT);
    pinMode(NRF24L01_CE, OUTPUT);
    pinMode(NRF24L01_CS, OUTPUT);
    pinMode(ST25R3916_BSS, OUTPUT);
    // pinMode(ST25R3916_EN, OUTPUT);

    // digitalWrite(LR1121_CS, HIGH);
    // digitalWrite(LR1121_RST, LOW);
    digitalWrite(CC1101_CS, HIGH);
    digitalWrite(NRF24L01_CS, HIGH);
    digitalWrite(NRF24L01_CE, HIGH);
    digitalWrite(ST25R3916_BSS, HIGH);
    // digitalWrite(ST25R3916_EN, LOW);

    // SPI.setFrequency(100000);
    SPI.begin(BOARD_SPI_SCK, BOARD_SPI_MISO, BOARD_SPI_MOSI, LR1121_CS);

    // pinMode(LR1121_SW2, OUTPUT);
    // digitalWrite(LR1121_SW2, HIGH);

    lr1121_init();
}

static int count = 0;

void loop()
{
    lr1121_loop();
    // printf("hello\n");
}
