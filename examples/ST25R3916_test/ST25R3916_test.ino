
#include <Arduino.h>
#include "rfal_rfst25r3916.h"
#include "ndef_class.h"
#include "pin.h"

/*
******************************************************************************
* GLOBAL DEFINES
******************************************************************************
*/
/* Definition of possible states the demo state machine could have */
#define DEMO_ST_NOTINIT 0         /*!< Demo State:  Not initialized */
#define DEMO_ST_START_DISCOVERY 1 /*!< Demo State:  Start Discovery */
#define DEMO_ST_DISCOVERY 2       /*!< Demo State:  Discovery       */

#define NDEF_DEMO_READ              0U   /*!< NDEF menu read               */
#define NDEF_DEMO_WRITE_MSG1        1U   /*!< NDEF menu write 1 record     */
#define NDEF_DEMO_WRITE_MSG2        2U   /*!< NDEF menu write 2 records    */
#define NDEF_DEMO_FORMAT_TAG        3U   /*!< NDEF menu format tag         */

#define NDEF_DEMO_MAX_FEATURES      4U   /*!< Number of menu items         */


#define NDEF_LED_BLINK_DURATION       250U /*!< Led blink duration         */

/*
 ******************************************************************************
 * LOCAL VARIABLES
 ******************************************************************************
 */
/* P2P communication data */
static uint8_t NFCID3[] = {0x01, 0xFE, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A};
static uint8_t GB[] = {0x46, 0x66, 0x6d, 0x01, 0x01, 0x11, 0x02, 0x02, 0x07, 0x80, 0x03, 0x02, 0x00, 0x03, 0x04, 0x01, 0x32, 0x07, 0x01, 0x03};


static const uint8_t *ndefDemoFeatureDescription[NDEF_DEMO_MAX_FEATURES] = {
  (uint8_t *)"1. Tap a tag to read its content",
  (uint8_t *)"2. Present a tag to write a Text record",
  (uint8_t *)"3. Present a tag to write a URI record and an Android Application record",
  (uint8_t *)"4. Present an ST tag to format",
};

/*
 ******************************************************************************
 * LOCAL VARIABLES
 ******************************************************************************
 */
RfalRfST25R3916Class rfst25r3916(&SPI, ST25R3916_BSS, ST25R3916_IQR);
RfalNfcClass rfal_nfc(&rfst25r3916);
NdefClass ndef(&rfal_nfc);

static rfalNfcDiscoverParam discParam;
static uint8_t state = DEMO_ST_NOTINIT;

static uint8_t              ndefDemoFeature     = NDEF_DEMO_READ;
static uint8_t              ndefDemoPrevFeature = 0xFF;


static uint32_t             timer;
static uint32_t             timerLed;
static bool                 ledOn;


#define MAX_HEX_STR         4
#define MAX_HEX_STR_LENGTH  128
char hexStr[MAX_HEX_STR][MAX_HEX_STR_LENGTH];
uint8_t hexStrIdx = 0;

/*
******************************************************************************
* LOCAL FUNCTION PROTOTYPES
******************************************************************************
*/

static void ledsOn(void);
static void ledsOff(void);

static char *hex2Str(unsigned char *data, size_t dataLen);

void setup(void)
{
    Serial.begin(115200);

    pinMode(LR1121_CS, OUTPUT);
    pinMode(LR1121_RST, OUTPUT);
    pinMode(CC1101_CS, OUTPUT);
    pinMode(NRF24L01_CE, OUTPUT);
    pinMode(NRF24L01_CS, OUTPUT);
    // pinMode(ST25R3916_BSS, OUTPUT);
    pinMode(ST25R3916_I2C_EN, OUTPUT);

    digitalWrite(LR1121_CS, HIGH);
    digitalWrite(LR1121_RST, LOW);
    digitalWrite(CC1101_CS, HIGH);
    digitalWrite(NRF24L01_CS, HIGH);
    digitalWrite(NRF24L01_CE, HIGH);
    // digitalWrite(ST25R3916_BSS, HIGH);
    digitalWrite(ST25R3916_I2C_EN, LOW); // Connect to GND (SPI) or VDD_D (I2C)

    // SPI.setFrequency(100000);
    SPI.begin(BOARD_SPI_SCK, BOARD_SPI_MISO, BOARD_SPI_MOSI, ST25R3916_BSS);

    ReturnCode status = rfal_nfc.rfalNfcInitialize();
    if (status == ERR_NONE)
    {
        Serial.println("[ST25R3916] Init success!");

        discParam.compMode = RFAL_COMPLIANCE_MODE_NFC;
        discParam.compMode = RFAL_COMPLIANCE_MODE_NFC;
        discParam.devLimit = 1U;
        discParam.nfcfBR = RFAL_BR_212;
        discParam.ap2pBR = RFAL_BR_424;

        ST_MEMCPY(&discParam.nfcid3, NFCID3, sizeof(NFCID3));
        ST_MEMCPY(&discParam.GB, GB, sizeof(GB));
        discParam.GBLen = sizeof(GB);

        discParam.notifyCb = NULL;
        discParam.totalDuration = 1000U;
        discParam.wakeupEnabled = false;
        discParam.wakeupConfigDefault = true;
        discParam.techs2Find = (RFAL_NFC_POLL_TECH_A | RFAL_NFC_POLL_TECH_B | RFAL_NFC_POLL_TECH_F | RFAL_NFC_POLL_TECH_V | RFAL_NFC_POLL_TECH_ST25TB);
        discParam.techs2Find |= RFAL_NFC_POLL_TECH_AP2P;

        state = DEMO_ST_START_DISCOVERY;
    }
    else
    {
        Serial.printf("[ST25R3916] Init error, code : %d!\n", status);
    }
}

void loop()
{
  static rfalNfcDevice *nfcDevice;

  rfalNfcaSensRes       sensRes;
  rfalNfcaSelRes        selRes;

  rfalNfcbSensbRes      sensbRes;
  uint8_t               sensbResLen;

  uint8_t               devCnt = 0;
  rfalFeliCaPollRes     cardList[1];
  uint8_t               collisions = 0U;
  rfalNfcfSensfRes     *sensfRes;

  rfalNfcvInventoryRes  invRes;
  uint16_t              rcvdLen;

  rfal_nfc.rfalNfcWorker();                                    /* Run RFAL worker periodically */

  if ((ndefDemoFeature != NDEF_DEMO_READ) && (rfst25r3916.timerIsExpired(timer))) {
    Serial.print("Timer expired, back to Read mode...\r\n");
    ledsOff();
    ndefDemoFeature = NDEF_DEMO_READ;
  }

  if (ndefDemoFeature != ndefDemoPrevFeature) {
    ndefDemoPrevFeature = ndefDemoFeature;
    Serial.print((char *)ndefDemoFeatureDescription[ndefDemoFeature]);
    Serial.print("\r\n");
  }

  if (ndefDemoFeature != NDEF_DEMO_READ) {
    if (rfst25r3916.timerIsExpired(timerLed)) {
      timerLed = rfst25r3916.timerCalculateTimer(NDEF_LED_BLINK_DURATION);
      ledOn = !ledOn;
    }
    if (ledOn) {
      ledsOn();
    } else {
      ledsOff();
    }
  }

  switch (state) {
    case DEMO_ST_START_DISCOVERY:
        Serial.printf("1.rfalNfcGetState = %d\n", rfal_nfc.rfalNfcGetState());

        rfal_nfc.rfalNfcDeactivate(false);

        Serial.printf("2.rfalNfcGetState = %d\n", rfal_nfc.rfalNfcGetState());

        rfal_nfc.rfalNfcDiscover(&discParam);

        state = DEMO_ST_DISCOVERY;
    break;

    case DEMO_ST_DISCOVERY:

        Serial.printf("3.rfalNfcGetState = %d\n", rfal_nfc.rfalNfcGetState());

        if(rfal_nfc.rfalNfcGetActiveDevice(&nfcDevice) == ERR_NONE)
        {
            Serial.printf("nfcDevice->type = %d\n", nfcDevice->type);


            rfal_nfc.rfalNfcDeactivate(false);
            delay(500);
            state = DEMO_ST_START_DISCOVERY;
        }

        Serial.printf("4.rfalNfcGetState = %d\n", rfal_nfc.rfalNfcGetState());
    break;

  }
    
    delay(1000);

//   checkUserButton();
#if 0
  switch (state) {
    /*******************************************************************************/
    case DEMO_ST_START_DISCOVERY:
      ledsOff();

      rfal_nfc.rfalNfcDeactivate(false);
      rfal_nfc.rfalNfcDiscover(&discParam);

      state = DEMO_ST_DISCOVERY;
      break;

    /*******************************************************************************/
    case DEMO_ST_DISCOVERY:
      if (rfalNfcIsDevActivated(rfal_nfc.rfalNfcGetState())) {
        rfal_nfc.rfalNfcGetActiveDevice(&nfcDevice);

        ledsOff();
        delay(50);
        ndefDemoPrevFeature = 0xFF; /* Force the display of the prompt */
        
        switch (nfcDevice->type) {
          /*******************************************************************************/
          case RFAL_NFC_LISTEN_TYPE_NFCA:

            // digitalWrite(LED_A_PIN, HIGH);
            switch (nfcDevice->dev.nfca.type) {
              case RFAL_NFCA_T1T:
                Serial.print("ISO14443A/Topaz (NFC-A T1T) TAG found. UID: ");
                Serial.print(hex2Str(nfcDevice->nfcid, nfcDevice->nfcidLen));
                Serial.print("\r\n");
                rfal_nfc.rfalNfcaPollerSleep();
                break;

              case RFAL_NFCA_T4T:
                Serial.print("NFCA Passive ISO-DEP device found. UID: ");
                Serial.print(hex2Str(nfcDevice->nfcid, nfcDevice->nfcidLen));
                Serial.print("\r\n");
                // demoNdef(nfcDevice);
                rfal_nfc.rfalIsoDepDeselect();
                break;

              case RFAL_NFCA_T4T_NFCDEP:
              case RFAL_NFCA_NFCDEP:
                Serial.print("NFCA Passive P2P device found. NFCID: ");
                Serial.print(hex2Str(nfcDevice->nfcid, nfcDevice->nfcidLen));
                Serial.print("\r\n");
                // demoP2P();
                break;

              default:
                Serial.print("ISO14443A/NFC-A card found. UID: ");
                Serial.print(hex2Str(nfcDevice->nfcid, nfcDevice->nfcidLen));
                Serial.print("\r\n");
                // demoNdef(nfcDevice);
                rfal_nfc.rfalNfcaPollerSleep();
                break;
            }
            /* Loop until tag is removed from the field */
            Serial.print("Operation completed\r\nTag can be removed from the field\r\n");
            rfal_nfc.rfalNfcaPollerInitialize();
            while (rfal_nfc.rfalNfcaPollerCheckPresence(RFAL_14443A_SHORTFRAME_CMD_WUPA, &sensRes) == ERR_NONE) {
              if (((nfcDevice->dev.nfca.type == RFAL_NFCA_T1T) && (!rfalNfcaIsSensResT1T(&sensRes))) ||
                  ((nfcDevice->dev.nfca.type != RFAL_NFCA_T1T) && (rfal_nfc.rfalNfcaPollerSelect(nfcDevice->dev.nfca.nfcId1, nfcDevice->dev.nfca.nfcId1Len, &selRes) != ERR_NONE))) {
                break;
              }
              rfal_nfc.rfalNfcaPollerSleep();
              delay(130);
            }
            break;

          /*******************************************************************************/
          case RFAL_NFC_LISTEN_TYPE_NFCB:

            Serial.print("ISO14443B/NFC-B card found. UID: ");
            Serial.print(hex2Str(nfcDevice->nfcid, nfcDevice->nfcidLen));
            Serial.print("\r\n");
            // digitalWrite(LED_B_PIN, HIGH);

            if (rfalNfcbIsIsoDepSupported(&nfcDevice->dev.nfcb)) {
            //   demoNdef(nfcDevice);
              rfal_nfc.rfalIsoDepDeselect();
            } else {
              rfal_nfc.rfalNfcbPollerSleep(nfcDevice->dev.nfcb.sensbRes.nfcid0);
            }
            /* Loop until tag is removed from the field */
            Serial.print("Operation completed\r\nTag can be removed from the field\r\n");
            rfal_nfc.rfalNfcbPollerInitialize();
            while (rfal_nfc.rfalNfcbPollerCheckPresence(RFAL_NFCB_SENS_CMD_ALLB_REQ, RFAL_NFCB_SLOT_NUM_1, &sensbRes, &sensbResLen) == ERR_NONE) {
              if (ST_BYTECMP(sensbRes.nfcid0, nfcDevice->dev.nfcb.sensbRes.nfcid0, RFAL_NFCB_NFCID0_LEN) != 0) {
                break;
              }
              rfal_nfc.rfalNfcbPollerSleep(nfcDevice->dev.nfcb.sensbRes.nfcid0);
              delay(130);
            }
            break;

          /*******************************************************************************/
          case RFAL_NFC_LISTEN_TYPE_NFCF:

            if (rfalNfcfIsNfcDepSupported(&nfcDevice->dev.nfcf)) {
              Serial.print("NFCF Passive P2P device found. NFCID: ");
              Serial.print(hex2Str(nfcDevice->nfcid, nfcDevice->nfcidLen));
              Serial.print("\r\n");
            //   demoP2P();
            } else {
              Serial.print("Felica/NFC-F card found. UID: ");
              Serial.print(hex2Str(nfcDevice->nfcid, nfcDevice->nfcidLen));
              Serial.print("\r\n");
            //   demoNdef(nfcDevice);
            }

            // digitalWrite(LED_F_PIN, HIGH);
            /* Loop until tag is removed from the field */
            Serial.print("Operation completed\r\nTag can be removed from the field\r\n");
            devCnt = 1;
            rfal_nfc.rfalNfcfPollerInitialize(RFAL_BR_212);
            while (rfal_nfc.rfalNfcfPollerPoll(RFAL_FELICA_1_SLOT, RFAL_NFCF_SYSTEMCODE, RFAL_FELICA_POLL_RC_NO_REQUEST, cardList, &devCnt, &collisions) == ERR_NONE) {
              /* Skip the length field byte */
              sensfRes = (rfalNfcfSensfRes *) & ((uint8_t *)cardList)[1];
              if (ST_BYTECMP(sensfRes->NFCID2, nfcDevice->dev.nfcf.sensfRes.NFCID2, RFAL_NFCF_NFCID2_LEN) != 0) {
                break;
              }
              delay(130);
            }
            break;

          /*******************************************************************************/
          case RFAL_NFC_LISTEN_TYPE_NFCV: {
              uint8_t devUID[RFAL_NFCV_UID_LEN];

              ST_MEMCPY(devUID, nfcDevice->nfcid, nfcDevice->nfcidLen);     /* Copy the UID into local var */
              REVERSE_BYTES(devUID, RFAL_NFCV_UID_LEN);                   /* Reverse the UID for display purposes */
              Serial.print("ISO15693/NFC-V card found. UID: ");
              Serial.print(hex2Str(devUID, RFAL_NFCV_UID_LEN));
              Serial.print("\r\n");

            //   digitalWrite(LED_V_PIN, HIGH);

            //   demoNdef(nfcDevice);

              /* Loop until tag is removed from the field */
              Serial.print("Operation completed\r\nTag can be removed from the field\r\n");
              rfal_nfc.rfalNfcvPollerInitialize();
              while (rfal_nfc.rfalNfcvPollerInventory(RFAL_NFCV_NUM_SLOTS_1, RFAL_NFCV_UID_LEN * 8U, nfcDevice->dev.nfcv.InvRes.UID, &invRes, &rcvdLen) == ERR_NONE) {
                delay(130);
              }
            }
            break;

          /*******************************************************************************/
          case RFAL_NFC_LISTEN_TYPE_ST25TB:

            Serial.print("ST25TB card found. UID: ");
            Serial.print(hex2Str(nfcDevice->nfcid, nfcDevice->nfcidLen));
            Serial.print("\r\n");
            // digitalWrite(LED_B_PIN, HIGH);
            break;

          /*******************************************************************************/
          case RFAL_NFC_LISTEN_TYPE_AP2P:

            Serial.print("NFC Active P2P device found. NFCID3: ");
            Serial.print(hex2Str(nfcDevice->nfcid, nfcDevice->nfcidLen));
            Serial.print("\r\n");
            // digitalWrite(LED_AP2P_PIN, HIGH);

            // demoP2P();
            break;

          /*******************************************************************************/
          default:
            break;
        }

        rfal_nfc.rfalNfcDeactivate(false);
        delay(500);
        state = DEMO_ST_START_DISCOVERY;
      }
      break;

    /*******************************************************************************/
    case DEMO_ST_NOTINIT:
    default:
      break;
  }

#endif
}




static void ledsOn(void)
{
//   digitalWrite(LED_A_PIN, HIGH);
//   digitalWrite(LED_B_PIN, HIGH);
//   digitalWrite(LED_F_PIN, HIGH);
//   digitalWrite(LED_V_PIN, HIGH);
//   digitalWrite(LED_AP2P_PIN, HIGH);
//   digitalWrite(LED_FIELD_PIN, HIGH);
}

static void ledsOff(void)
{
//   digitalWrite(LED_A_PIN, LOW);
//   digitalWrite(LED_B_PIN, LOW);
//   digitalWrite(LED_F_PIN, LOW);
//   digitalWrite(LED_V_PIN, LOW);
//   digitalWrite(LED_AP2P_PIN, LOW);
//   digitalWrite(LED_FIELD_PIN, LOW);
}


char *hex2Str(unsigned char *data, size_t dataLen)
{
  unsigned char *pin = data;
  const char *hex = "0123456789ABCDEF";
  char *pout = hexStr[hexStrIdx];
  uint8_t i = 0;
  uint8_t idx = hexStrIdx;
  size_t len;

  if (dataLen == 0) {
    pout[0] = 0;
  } else {
    /* Trim data that doesn't fit in buffer */
    len = MIN(dataLen, (MAX_HEX_STR_LENGTH / 2));

    for (; i < (len - 1); ++i) {
      *pout++ = hex[(*pin >> 4) & 0xF];
      *pout++ = hex[(*pin++) & 0xF];
    }
    *pout++ = hex[(*pin >> 4) & 0xF];
    *pout++ = hex[(*pin) & 0xF];
    *pout = 0;
  }

  hexStrIdx++;
  hexStrIdx %= MAX_HEX_STR;

  return hexStr[idx];
}


