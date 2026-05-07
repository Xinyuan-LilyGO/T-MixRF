#if 0

// include the library
#include <RadioLib.h>


#define NRF24L01_CS     6  // SPI Chip Select
#define NRF24L01_CE     1   // Chip Enable Activates RX or TX(High) mode
#define NRF24L01_IQR    5   // Maskable interrupt pin. Active low
#define NRF24L01_MOSI   3
#define NRF24L01_MISO   2
#define NRF24L01_SCK    4

// nRF24 has the following connections:
// CS pin:    10
// IRQ pin:   2
// CE pin:    3

nRF24 radio = new Module(NRF24L01_CS, NRF24L01_IQR, NRF24L01_CE);

// or detect the pinout automatically using RadioBoards
// https://github.com/radiolib-org/RadioBoards
/*
#define RADIO_BOARD_AUTO
#include <RadioBoards.h>
Radio radio = new RadioModule();
*/

// save transmission state between loops
int transmissionState = RADIOLIB_ERR_NONE;

// flag to indicate that a packet was sent
volatile bool transmittedFlag = false;

// this function is called when a complete packet
// is transmitted by the module
// IMPORTANT: this function MUST be 'void' type
//            and MUST NOT have any arguments!
#if defined(ESP8266) || defined(ESP32)
ICACHE_RAM_ATTR
#endif
void setFlag(void)
{
    // we sent a packet, set the flag
    transmittedFlag = true;
}

void setup()
{
    Serial.begin(115200);

    pinMode(40, OUTPUT);
    digitalWrite(40, HIGH);

    // SPI.end();
    SPI.begin(NRF24L01_SCK, NRF24L01_MISO, NRF24L01_MOSI);

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
            // delay(10);
        }
    }

    radio.setOutputPower(-18);

    // while(1){
    //     radio.transmitDirect();
    // }

    // set transmit address
    // NOTE: address width in bytes MUST be equal to the
    //       width set in begin() or setAddressWidth()
    //       methods (5 by default)
    byte addr[] = {0x01, 0x23, 0x45, 0x67, 0x89};
    Serial.print(F("[nRF24] Setting transmit pipe ... "));
    state = radio.setTransmitPipe(addr);
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
    // when packet transmission is finished
    radio.setPacketSentAction(setFlag);

    // start transmitting the first packet
    Serial.print(F("[nRF24] Sending first packet ... "));

    // you can transmit C-string or Arduino string up to
    // 256 characters long
    transmissionState = radio.startTransmit("Hello World!");

    // you can also transmit byte array up to 256 bytes long
    /*
      byte byteArr[] = {0x01, 0x23, 0x45, 0x67,
                        0x89, 0xAB, 0xCD, 0xEF};
      state = radio.startTransmit(byteArr, 8);
    */
}

// counter to keep track of transmitted packets
int count = 0;

uint32_t curr_tick = 0;

void loop()
{
    // curr_tick = millis(); //
    if(millis() - curr_tick > 100)
    {
        curr_tick = millis();
        if (transmissionState == RADIOLIB_ERR_NONE)
        {
            // packet was successfully sent
            Serial.println(F("transmission finished!"));

            // NOTE: when using interrupt-driven transmit method,
            //       it is not possible to automatically measure
            //       transmission data rate using getDataRate()
        }
        else
        {
            Serial.print(F("failed, code "));
            Serial.println(transmissionState);
        }

        // clean up after transmission is finished
        // this will ensure transmitter is disabled,
        // RF switch is powered down etc.
        radio.finishTransmit();

        // wait a second before transmitting again
        // delay(1000);

        // send another one
        Serial.print(F("[nRF24] Sending another packet ... "));

        // you can transmit C-string or Arduino string up to
        // 32 characters long
        String str = "Hello World! #" + String(count++);
        transmissionState = radio.startTransmit(str);

        // you can also transmit byte array up to 256 bytes long
        /*
          byte byteArr[] = {0x01, 0x23, 0x45, 0x67,
                            0x89, 0xAB, 0xCD, 0xEF};
          int state = radio.startTransmit(byteArr, 8);
        */
    }
}


#else
#include "SPI.h"
#include "RF24.h"
#include "nRF24L01.h"
#include "MinRf.h"

#define CE_PIN NRF24L01_CE
#define CSN_PIN NRF24L01_CS
#define INTERVAL_MS_TRANSMISSION 250
RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";
// NRF24L01 buffer limit is 32 bytes (max struct size)
struct payload
{
    byte data1;
    char data2;
};
payload payload;

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

    // SPI.setFrequency(100000);
    SPI.begin(BOARD_SPI_SCK, BOARD_SPI_MISO, BOARD_SPI_MOSI, NRF24L01_CS);

    Serial.printf("begin = %d\n", radio.begin()) ;
    
    radio.setChannel(0);
    // Append ACK packet from the receiving radio back to the transmitting radio
    radio.setAutoAck(false); //(true|false)
    // Set the transmission datarate
    radio.setDataRate(RF24_250KBPS); //(RF24_250KBPS|RF24_1MBPS|RF24_2MBPS)
    // Greater level = more consumption = longer distance
    radio.setPALevel(RF24_PA_MAX); //(RF24_PA_MIN|RF24_PA_LOW|RF24_PA_HIGH|RF24_PA_MAX)
    // Default value is the maximum 32 bytes
    radio.setPayloadSize(sizeof(payload));
    // Act as transmitter
    radio.openWritingPipe(address);
    radio.stopListening();
}
void loop()
{
    payload.data1 = 123;
    payload.data2 = 'x';
    radio.write(&payload, sizeof(payload));
    Serial.print("Data1:");
    Serial.println(payload.data1);
    Serial.print("Data2:");
    Serial.println(payload.data2);
    Serial.println("Sent");
    delay(INTERVAL_MS_TRANSMISSION);
}

#endif