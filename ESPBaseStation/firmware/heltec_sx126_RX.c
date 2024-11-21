#include <RadioLib.h>
#include <SPI.h>
#include <U8g2lib.h>

#define LORA_NSS 8
#define LORA_SCK 9
#define LORA_MOSI 10
#define LORA_MISO 11
#define LORA_RST 12
#define LORA_BUSY 13
#define LORA_DIO1 14

#define OLED_SCL 18
#define OLED_SDA 17
#define OLED_RST 21

byte packet[5];
char dispMessage[30];
char dispMessage2[30];
char dispMessage3[30];

long long lastVoltageRead = 0;

// NSS DIO1 NRST BUSY
SX1262 radio = new Module(LORA_NSS, LORA_DIO1, LORA_RST, LORA_BUSY);
U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, OLED_SCL, OLED_SDA, OLED_RST);
volatile bool receivedFlag = false;

void setFlag(void)
{
  receivedFlag = true;
}

void setup()
{
  Serial.begin(115200);
  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_NSS);
  Serial.print(F("[SX1262] Initializing ... "));
  int state = radio.begin(868.0, 500.0, 12, 5, 0x12, 20, 10);
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

  radio.setPacketReceivedAction(setFlag);
  Serial.print(F("[SX1262] Going into RX mode ... "));
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
  u8g2.begin();
  pinMode(37, OUTPUT);

  analogReadResolution(12);
}

void loop()
{
  if (receivedFlag)
  {
    receivedFlag = false;
    int state = radio.readData(packet, radio.getPacketLength());
    if (state == RADIOLIB_ERR_NONE)
    {
      if (packet[0] == 255 && packet[1] == 200 && packet[2] == 145 && packet[3] == 90)
      {
        sprintf(dispMessage, "PacketNum: %d", packet[4]);
        sprintf(dispMessage2, "RSSI: %.1f dBm; SNR: %.1f", radio.getRSSI(), radio.getSNR());
      }
    }
  }

  if (millis() - lastVoltageRead >= 5000)
  {
    digitalWrite(37, LOW);
    float analogVolts = 0.004 * analogRead(1);
    digitalWrite(37, HIGH);
    sprintf(dispMessage3, "VBAT: %.2f", analogVolts);
  }

  u8g2.firstPage();
  do
  {
    u8g2.setFont(u8g2_font_helvR08_tr);
    u8g2.drawStr(0, 24, dispMessage);
    u8g2.drawStr(0, 48, dispMessage2);
    u8g2.drawStr(0, 57, dispMessage3);
  } while (u8g2.nextPage());
}