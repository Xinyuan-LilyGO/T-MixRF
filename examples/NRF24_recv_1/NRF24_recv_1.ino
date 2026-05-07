/*
  RadioLib nRF24 Receive Example

  This example listens for FSK transmissions using nRF24 2.4 GHz radio module.
  Once a packet is received, an interrupt is triggered.
  To successfully receive data, the following settings have to be the same
  on both transmitter and receiver:
  - carrier frequency
  - data rate
  - transmit pipe on transmitter must match receive pipe
    on receiver

  For default module settings, see the wiki page
  https://github.com/jgromes/RadioLib/wiki/Default-configuration#nrf24

  For full API reference, see the GitHub Pages
  https://jgromes.github.io/RadioLib/
*/

// include the library
#include <RadioLib.h>
#include "pin.h"
// nRF24 has the following connections:
// CS pin:    10
// IRQ pin:   2
// CE pin:    3
nRF24 radio = new Module(NRF24L01_CS, NRF24L01_IQR, NRF24L01_CE);

std::string info_head = "Hello World! #";

// or detect the pinout automatically using RadioBoards
// https://github.com/radiolib-org/RadioBoards
/*
#define RADIO_BOARD_AUTO
#include <RadioBoards.h>
Radio radio = new RadioModule();
*/

// flag to indicate that a packet was received
volatile bool receivedFlag = false;

// this function is called when a complete packet
// is received by the module
// IMPORTANT: this function MUST be 'void' type
//            and MUST NOT have any arguments!
#if defined(ESP8266) || defined(ESP32)
ICACHE_RAM_ATTR
#endif
void setFlag(void)
{
    // we got a packet, set the flag
    receivedFlag = true;
}

void setup()
{
    Serial.begin(115200);

    pinMode(LR1121_CS, OUTPUT);
    pinMode(LR1121_RST, OUTPUT);
    pinMode(CC1101_CS, OUTPUT);
    // pinMode(NRF24L01_CE, OUTPUT);
    // pinMode(NRF24L01_CS, OUTPUT);
    pinMode(ST25R3916_BSS, OUTPUT);
    // pinMode(ST25R3916_EN, OUTPUT);

    digitalWrite(LR1121_CS, HIGH);
    digitalWrite(LR1121_RST, LOW);
    digitalWrite(CC1101_CS, HIGH);
    // digitalWrite(NRF24L01_CS, HIGH);
    // digitalWrite(NRF24L01_CE, HIGH);
    digitalWrite(ST25R3916_BSS, HIGH);
    // digitalWrite(ST25R3916_EN, LOW);

    // SPI.end();
    SPI.begin(BOARD_SPI_SCK, BOARD_SPI_MISO, BOARD_SPI_MOSI, NRF24L01_CS);

    // initialize nRF24 with default settings
    Serial.print(F("[nRF24] Initializing ... "));
    int state = radio.begin();
    if (state == RADIOLIB_ERR_NONE)
    {
        Serial.println(F("success!"));
    }
    else
    {
        Serial.print(F("failed, code "));
        Serial.println(state);
        while (true)
        {
            delay(10);
        }
    }

    radio.setOutputPower(0);

    // set receive pipe 0 address
    // NOTE: address width in bytes MUST be equal to the
    //       width set in begin() or setAddressWidth()
    //       methods (5 by default)
    Serial.print(F("[nRF24] Setting address for receive pipe 0 ... "));
    byte addr[] = {0x01, 0x23, 0x45, 0x67, 0x89};
    state = radio.setReceivePipe(0, addr);
    if (state == RADIOLIB_ERR_NONE)
    {
        Serial.println(F("success!"));
    }
    else
    {
        Serial.print(F("failed, code "));
        Serial.println(state);
        while (true)
        {
            delay(10);
        }
    }

    // set the function that will be called
    // when new packet is received
    radio.setPacketReceivedAction(setFlag);

    // start listening
    Serial.print(F("[nRF24] Starting to listen ... "));
    state = radio.startReceive();
    if (state == RADIOLIB_ERR_NONE)
    {
        Serial.println(F("success!"));
    }
    else
    {
        Serial.print(F("failed, code "));
        Serial.println(state);
        while (true)
        {
            delay(10);
        }
    }

    // if needed, 'listen' mode can be disabled by calling
    // any of the following methods:
    //
    // radio.standby()
    // radio.sleep()
    // radio.transmit();
    // radio.receive();
    // radio.readData();
}
uint32_t curr_tick = 0;

bool containsSubstring(const std::string &mainStr, const std::string &subStr)
{
    // 如果子串为空，通常认为包含关系成立
    if (subStr.empty())
        return true;

    // 在 mainStr 中查找 subStr
    return mainStr.find(subStr) != std::string::npos;
}

void loop()
{
    // check if the flag is set
    // check if the flag is set
    if (millis() - curr_tick > 100)
    {
        // reset flag
        receivedFlag = false;

        // you can read received data as an Arduino String
        String str;
        int state = radio.readData(str);

        // you can also read received data as byte array
        /*
          byte byteArr[8];
          int numBytes = radio.getPacketLength();
          int state = radio.readData(byteArr, numBytes);
        */

        if (state == RADIOLIB_ERR_NONE)
        {
            if (containsSubstring(str.c_str(), info_head))
            {
                // packet was successfully received
                Serial.println(F("[nRF24] Received packet!"));
                // print data of the packet
                Serial.print(F("[nRF24] Data:\t\t"));
                Serial.println(str);

                // ws2812_demo1();
            }
        }
        else
        {
            // some other error occurred
            Serial.print(F("[nRF24] Failed, code "));
            Serial.println(state);
        }

        // put module back to listen mode
        radio.startReceive();
    }
    delay(1);
}
