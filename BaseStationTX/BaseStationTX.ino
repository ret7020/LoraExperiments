#include "config.h"

extern "C" {
  #include "user_interface.h"
}


void setup(){
    Serial.begin(115200);
    pinMode(D4, OUTPUT);
    
    #ifdef UART_DEBUG
        radio.printDetails();
        Serial.println("NRF Ready");
        Serial.println("Lora setup...");
    #endif
//
//    // Lora Setup
    pinMode(LORA_CONTROL, OUTPUT);
//    #ifdef UART_DEBUG
//      digitalWrite(LORA_CONTROL, 1);
//      delay(2000);
//      Serial.write(byte(0xC0));
//      Serial.write(byte(0x02));
//      Serial.write(byte(0x01));
//      Serial.write(byte(0xE5));
//      delay(2000);
//      while (loraSerial.available()){
//        Serial.print(loraSerial.read(), HEX);
//        Serial.print(" ");
//      }
//    #endif
    digitalWrite(LORA_CONTROL, 0);
}
void loop(){
    // Header
    Serial.write(byte (0xFF));
    Serial.write(byte (0xFF));
    Serial.write(byte (0x1E));

    // Data
    Serial.write(byte (0xA));
    Serial.write(byte (0xB));
    Serial.write(byte (0xC));
    Serial.write(byte (0xD));
    delay(1000);
}