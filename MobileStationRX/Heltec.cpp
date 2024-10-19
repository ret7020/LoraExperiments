#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <RadioLib.h>
#include <U8g2lib.h>

#define LoRa_MOSI 10
#define LoRa_MISO 11
#define LoRa_SCK 9

#define LoRa_nss 8
#define LoRa_dio1 14
#define LoRa_nrst 12
#define LoRa_busy 13

#define oled_scl 18
#define oled_sda 17
#define oled_rst 21

SX1262 radio = new Module(LoRa_nss, LoRa_dio1, LoRa_nrst, LoRa_busy);
U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, /* clock=*/oled_scl, /* data=*/oled_sda, /* reset=*/oled_rst);
int packetId = 0;

void setup()
{
  Serial.begin(115200);
  SPI.begin(LoRa_SCK, LoRa_MISO, LoRa_MOSI, LoRa_nss);

  // initialize SX1262 with default settings
  Serial.print(F("[SX1262] Initializing ... "));
  
  int state = radio.begin();
  radio.setOutputPower(22);
  
  if (state == RADIOLIB_ERR_NONE)
  {
    Serial.println(F("success!"));
  }
  else
  {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true)
      ;
  }
  u8g2.begin();
  
}

void loop()
{
  char dispMessage[30];
  char dispMessage2[30];

  int state = radio.transmit("Hello World!");

  if (state == RADIOLIB_ERR_NONE)
  {
    Serial.println(F("success!"));
    sprintf(dispMessage, "Datarate: %.1f bps", radio.getDataRate());
    sprintf(dispMessage2, "Packet id: %d", packetId);
  }
  else if (state == RADIOLIB_ERR_TX_TIMEOUT)
  {
    // timeout occured while transmitting packet
    Serial.println(F("timeout!"));
  }
  else
  {
    // some other error occurred
    Serial.print(F("failed, code "));
    Serial.println(state);
  }

  u8g2.firstPage();
  do
  {
    u8g2.setFont(u8g2_font_helvR08_tr);
    u8g2.drawStr(0, 24, dispMessage);
    u8g2.drawStr(0, 48, dispMessage2);
  } while (u8g2.nextPage());

  packetId++;
 
//  delay(1000);
}
