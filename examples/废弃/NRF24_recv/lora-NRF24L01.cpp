#include <Arduino.h>
#include <RadioLib.h>
#include "pin.h"

// nRF24 has the following connections:
// CS pin:    34
// IRQ pin:   5
// CE pin:    4
static nRF24 radio = new Module(NRF24L01_CS, NRF24L01_IQR, NRF24L01_CE);

byte addr[] = {0x00, 0x23, 0x45, 0x67, 0x89};

// save transmission state between loops
static int transmissionState = RADIOLIB_ERR_NONE;

// flag to indicate that a packet was sent
static volatile bool receivedFlag = false;

void nrf24_transmit(void);

static void nrf24_setFlag(void)
{
    // we sent a packet, set the flag
    receivedFlag = true;
    Serial.println(F("nrf24_setFlag!"));
}

void nfr24_init(void)
{
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


    Serial.print(F("[nRF24] Setting address for receive pipe 0 ... "));
    // byte addr[] = {0x01, 0x23, 0x45, 0x67, 0x89};
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
}

void nrf24_loop(void)
{
    Serial.print(F("[nRF24] Waiting for incoming transmission ... "));

    // you can receive data as an Arduino String
    // NOTE: receive() is a blocking method!
    //       See example ReceiveInterrupt for details
    //       on non-blocking reception method.
    String str;
    int state = radio.receive(str);

    // you can also receive data as byte array
    /*
      byte byteArr[8];
      int state = radio.receive(byteArr, 8);
    */

    if (state == RADIOLIB_ERR_NONE)
    {
        // packet was successfully received
        Serial.println(F("success!"));

        // print the data of the packet
        Serial.print(F("[nRF24] Data:\t\t"));
        Serial.println(str);
    }
    else if (state == RADIOLIB_ERR_RX_TIMEOUT)
    {
        // timeout occurred while waiting for a packet
        Serial.println(F("timeout!"));
    }
    else
    {
        // some other error occurred
        Serial.print(F("failed, code "));
        Serial.println(state);
    }
}