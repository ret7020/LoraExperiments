#include "Arduino.h"
#include <SoftwareSerial.h>

uint8_t packetId = 0;

SoftwareSerial Lora(2, 3);
void setup(){
    Serial.begin(9600);
    pinMode(7, OUTPUT);
    digitalWrite(7, 1);
    
    Lora.begin(9600);

    delay(2000);
    //Lora.write(byte(0xC1));
    //Lora.write(byte(0x00));
    //Lora.write(byte(0x06));
    Lora.write(byte(0xC0));
    Lora.write(byte(0x02));
    Lora.write(byte(0x01));
    Lora.write(byte(0x62));
    delay(2000);
    Serial.println("Config regs: ");
    while (Lora.available())
    {
        Serial.print(Lora.read(), HEX);
        Serial.print(" ");
    }
    Serial.println();
    digitalWrite(7, 0);

    
}
void loop(){
    Lora.write(byte (0xFF));
    Lora.write(byte (0xFF));
    Lora.write(byte (0x17));
    Lora.write(byte (0xA));
    Lora.write(byte (0xB));
    Lora.write(byte (0xC));
    Lora.write((byte)strtol(String(packetId, HEX).c_str(), NULL, 16));
    packetId++;
    if (packetId == 201) packetId = 0;
    delay(2000);
}
