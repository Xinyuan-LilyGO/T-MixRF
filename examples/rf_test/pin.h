#pragma once

// Frequency define 
#define LR1121_FREQ 868.0       // Allowed values are in range from 150.0 to 960.0 MHz, 1900 - 2200 MHz and 2400 - 2500 MHz. Will also perform calibrations.
#define CC1101_FREQ 433.0       // 315 MHz、 433 MHz、868 MHz、915 MHz
#define NRF24L01_FREQ 2400.0     // Allowed values range from 2400 MHz to 2525 MHz.

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