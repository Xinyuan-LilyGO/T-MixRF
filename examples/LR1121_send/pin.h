#pragma once

// Frequency define 
#define LR1121_FREQ 868.0       // Allowed values are in range from 150.0 to 960.0 MHz, 1900 - 2200 MHz and 2400 - 2500 MHz. Will also perform calibrations.
#define CC1101_FREQ 433.0       // 315 MHz、 433 MHz、868 MHz、915 MHz
#define NRF24L01_FREQ 2400.0     // Allowed values range from 2400 MHz to 2525 MHz.

#ifdef ESP32S2
// SPI
#define BOARD_SPI_SCK   17
#define BOARD_SPI_MOSI  15
#define BOARD_SPI_MISO  16

// LR1121
#define LR1121_CS       33
#define LR1121_DIO9     9
#define LR1121_RST      37
#define LR1121_BUSY     6
#define LR1121_SW2      7
#define LR1121_SCK      BOARD_SPI_SCK
#define LR1121_MOSI     BOARD_SPI_MOSI
#define LR1121_MISO     BOARD_SPI_MISO

// CC1101
#define CC1101_CS       11
#define CC1101_GDO0     14 //irq
#define CC1101_GDO2     12
#define CC1101_SW0      13
#define CC1101_SW1      10
#define CC1101_SCK      BOARD_SPI_SCK
#define CC1101_MOSI     BOARD_SPI_MOSI
#define CC1101_MISO     BOARD_SPI_MISO

// NRF24L01
#define NRF24L01_CS     34  // SPI Chip Select
#define NRF24L01_CE     4   // Chip Enable Activates RX or TX(High) mode
#define NRF24L01_IQR    5   // Maskable interrupt pin. Active low
#define NRF24L01_SCK    BOARD_SPI_SCK
#define NRF24L01_MOSI   BOARD_SPI_MOSI
#define NRF24L01_MISO   BOARD_SPI_MISO

// ST25R3916
#define ST25R3916_BSS   36
#define ST25R3916_CLK
#define ST25R3916_EN    35
#define ST25R3916_NFC1
#define ST25R3916_NFC2
#define ST25R3916_SCK   BOARD_SPI_SCK
#define ST25R3916_MOSI  BOARD_SPI_MOSI
#define ST25R3916_MISO  BOARD_SPI_MISO

#elif ESP32

// SPI
#define BOARD_SPI_SCK   17
#define BOARD_SPI_MOSI  5
#define BOARD_SPI_MISO  16

// LR1121
#define LR1121_CS       18
#define LR1121_DIO9     25
#define LR1121_RST      26
#define LR1121_BUSY     27
#define LR1121_SW2      7
#define LR1121_SCK      BOARD_SPI_SCK
#define LR1121_MOSI     BOARD_SPI_MOSI
#define LR1121_MISO     BOARD_SPI_MISO

// CC1101
#define CC1101_CS       12
#define CC1101_GDO0     14 //irq
#define CC1101_GDO2     12
#define CC1101_SW0      13
#define CC1101_SW1      10
#define CC1101_SCK      BOARD_SPI_SCK
#define CC1101_MOSI     BOARD_SPI_MOSI
#define CC1101_MISO     BOARD_SPI_MISO

// NRF24L01
#define NRF24L01_CS     14  // SPI Chip Select
#define NRF24L01_CE     4   // Chip Enable Activates RX or TX(High) mode
#define NRF24L01_IQR    5   // Maskable interrupt pin. Active low
#define NRF24L01_SCK    BOARD_SPI_SCK
#define NRF24L01_MOSI   BOARD_SPI_MOSI
#define NRF24L01_MISO   BOARD_SPI_MISO

// ST25R3916
#define ST25R3916_BSS   0
#define ST25R3916_CLK
#define ST25R3916_EN    35
#define ST25R3916_NFC1
#define ST25R3916_NFC2
#define ST25R3916_SCK   BOARD_SPI_SCK
#define ST25R3916_MOSI  BOARD_SPI_MOSI
#define ST25R3916_MISO  BOARD_SPI_MISO

#endif



// void radio_config(LR1121 &radio, float freq, float bw, uint8_t sf, uint8_t cr,
//                   uint8_t sysn, int8_t outputPower, uint16_t perlen)
// {
//     radio.standby();
//     radio.setRfSwitchTable(rfswitch_dio_pins, rfswitch_table);

//     if (radio.setFrequency(freq) == RADIOLIB_ERR_INVALID_FREQUENCY)
//     {
//         Serial.println(F("Selected frequency is invalid for this module!"));
//         while (true)
//             ;
//     }

//     /*
//      *   Sets LoRa link bandwidth.
//      *   SX1278/SX1276 : Allowed values are 10.4, 15.6, 20.8, 31.25, 41.7, 62.5, 125, 250 and 500 kHz. Only available in %LoRa mode.
//      *   SX1268/SX1262 : Allowed values are 7.8, 10.4, 15.6, 20.8, 31.25, 41.7, 62.5, 125.0, 250.0 and 500.0 kHz.
//      *   SX1280        : Allowed values are 203.125, 406.25, 812.5 and 1625.0 kHz.
//      *   LR1121        : Allowed values are 62.5, 125.0, 250.0 and 500.0 kHz.
//      * * * */
//     if (radio.setBandwidth(bw, freq > 1000.0) == RADIOLIB_ERR_INVALID_BANDWIDTH)
//     {
//         Serial.println(F("Selected bandwidth is invalid for this module!"));
//         while (true)
//             ;
//     }

//     /*
//      * Sets LoRa link spreading factor.
//      * SX1278/SX1276 :  Allowed values range from 6 to 12. Only available in LoRa mode.
//      * SX1262        :  Allowed values range from 5 to 12.
//      * SX1280        :  Allowed values range from 5 to 12.
//      * LR1121        :  Allowed values range from 5 to 12.
//      * * * */
//     if (radio.setSpreadingFactor(sf) == RADIOLIB_ERR_INVALID_SPREADING_FACTOR)
//     {
//         Serial.println(F("Selected spreading factor is invalid for this module!"));
//         while (true)
//             ;
//     }

//     /*
//      * Sets LoRa coding rate denominator.
//      * SX1278/SX1276/SX1268/SX1262 : Allowed values range from 5 to 8. Only available in LoRa mode.
//      * SX1280        :  Allowed values range from 5 to 8.
//      * LR1121        :  Allowed values range from 5 to 8.
//      * * * */
//     if (radio.setCodingRate(cr) == RADIOLIB_ERR_INVALID_CODING_RATE)
//     {
//         Serial.println(F("Selected coding rate is invalid for this module!"));
//         while (true)
//             ;
//     }

//     /*
//      * Sets transmission output power.
//      * SX1278/SX1276 :  Allowed values range from -3 to 15 dBm (RFO pin) or +2 to +17 dBm (PA_BOOST pin). High power +20 dBm operation is also supported, on the PA_BOOST pin. Defaults to PA_BOOST.
//      * SX1262        :  Allowed values are in range from -9 to LF_CONFIG_RADIO_OUTPUT_POWER; dBm. This method is virtual to allow override from the SX1261 class.
//      * SX1268        :  Allowed values are in range from -9 to LF_CONFIG_RADIO_OUTPUT_POWER; dBm.
//      * SX1280        :  Allowed values are in range from -18 to HF_CONFIG_RADIO_OUTPUT_POWER; dBm. PA Version range : -18 ~ 3dBm
//      * LR1121        :  Allowed values are in range from -17 to LF_CONFIG_RADIO_OUTPUT_POWER; dBm (high-power PA) or -18 to HF_CONFIG_RADIO_OUTPUT_POWER; dBm (High-frequency PA)
//      * * * */
//     if (radio.setOutputPower(outputPower) == RADIOLIB_ERR_INVALID_OUTPUT_POWER)
//     {
//         Serial.println(F("Selected output power is invalid for this module!"));
//         while (true)
//             ;
//     }

//     /*
//      * Sets preamble length for LoRa or FSK modem.
//      * SX1278/SX1276 : Allowed values range from 6 to 65535 in %LoRa mode or 0 to 65535 in FSK mode.
//      * SX1262/SX1268 : Allowed values range from 1 to 65535.
//      * SX1280        : Allowed values range from 1 to 65535.
//      * LR1121        : Allowed values range from 1 to 65535.
//      * * */
//     if (radio.setPreambleLength(perlen) == RADIOLIB_ERR_INVALID_PREAMBLE_LENGTH)
//     {
//         Serial.println(F("Selected preamble length is invalid for this module!"));
//         while (true)
//             ;
//     }

//     if (radio.setSyncWord(sysn) == RADIOLIB_ERR_INVALID_SYNC_WORD)
//     {
//         Serial.println(F("Selected sync word is invalid for this module!"));
//         while (true)
//             ;
//     }

//     if (radio.setCRC(false) != RADIOLIB_ERR_NONE)
//     {
//         Serial.println(F("Failed to set CRC!"));
//     }

//     radio.invertIQ(false);

//     radio.setTCXO(3.3); // BAYCKRC board has no TCXO
// }