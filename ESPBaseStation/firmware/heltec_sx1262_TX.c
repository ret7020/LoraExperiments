#include <RadioLib.h>
#include <SPI.h>

#define LORA_NSS 8
#define LORA_SCK 9
#define LORA_MOSI 10
#define LORA_MISO 11
#define LORA_RST 12
#define LORA_BUSY 13
#define LORA_DIO1 14


byte packet[5] = {255, 200, 145, 90, 35};
byte packetIndex = 0;
// NSS DIO1 NRST BUSY
SX1262 radio = new Module(LORA_NSS, LORA_DIO1, LORA_RST, LORA_BUSY);

void setup() {
  Serial.begin(115200);
  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_NSS);
  Serial.print(F("[SX1262] Initializing ... "));
  int state = radio.begin(868.0, 500.0, 12, 5, 0x12, 20, 10);
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true) { delay(10); }
  }

//  state = radio.setOutputPower(20);
//  Serial.println(state);
  
  

}

void loop(){
  packet[4] = packetIndex;
  packetIndex++;
  int state = radio.transmit(packet, 5);
  if (state == RADIOLIB_ERR_NONE){
    Serial.println("[SX1262] TX OK");
  }
  if (packetIndex >= 255) packetIndex = 0;
  delay(1000);

}