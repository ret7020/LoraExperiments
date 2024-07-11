#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "config.h"
#include <Arduino_JSON.h>

byte prev = 0;
byte loraData[8];

AsyncWebSocket ws("/ws");
AsyncWebServer server(HTTP_PORT);
JSONVar clientPacket;


void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    switch (type)
    {
    case WS_EVT_CONNECT:
        Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
        break;
    case WS_EVT_DISCONNECT:
        Serial.printf("WebSocket client #%u disconnected\n", client->id());
        break;
    case WS_EVT_DATA:
        Serial.println("New data");
        // handleWebSocketMessage(arg, data, len);
        break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
        break;
    }
}

void setup()
{
    Serial.begin(115200);
    pinMode(LORA_433_MODE_PIN, OUTPUT);

    // 433 Mhz
    Serial.println("[INFO] 433 Mhz link started");
    Serial1.begin(9600, SERIAL_8N1, LORA_433_USART1_TX, LORA_433_USART1_RX);
    // printf("Writing 1 to pin %d", LORA_433_MODE_PIN, Serial);
    digitalWrite(LORA_433_MODE_PIN, 1); // Config mode

    delay(1000);

    // Request to read 6 config regs

    // Serial1.write(byte(0xC1));
    // Serial1.write(byte(0x00));
    // Serial1.write(byte(0x06));

    // Enable RSSI 
    Serial1.write(byte(0xC0));
    Serial1.write(byte(0x02));
    Serial1.write(byte(0x01));
    Serial1.write(byte(0x62));

    delay(1000);
    Serial.println("Config regs: ");
    while (Serial1.available())
    {
        Serial.print(Serial1.read(), HEX);
        Serial.print(" ");
    }
    Serial.println();
    Serial1.flush();
    delay(500);
    Serial1.end();
    delay(500);
    Serial1.begin(9600, SERIAL_8N1, LORA_433_USART1_TX, LORA_433_USART1_RX);
    // Serial1.setTimeout(100);
    Serial.println("LoRa Ready to recieve");
    delay(1000);
    digitalWrite(LORA_433_MODE_PIN, 0); // Normal mode
    delay(500);
    Serial1.flush();
    Serial.println("[INFO] 433 Mhz ready");

    // UI stack init

    WiFi.softAP(WIFI_SSID, WIFI_PSK);
    IPAddress IP = WiFi.softAPIP();

    Serial.print("AP IP address: ");
    Serial.println(IP);
    Serial.println("[INFO] WiFi Ready");

    ws.onEvent(onEvent);
    server.addHandler(&ws);

    server.begin();
}

void loop()
{
    if (Serial1.available()){
        Serial1.readBytes(loraData, 8);
        clientPacket["RSSI"] = loraData[7];
        Serial.println(loraData[6]);
        if (loraData[3] == byte(0xA) && loraData[4] == byte(0xB) 
        && loraData[5] == byte(0xC) && loraData[6] == byte(0xD)) 
            clientPacket["PacketCheck"] = 1;
        ws.textAll(JSON.stringify(clientPacket));
    }
    
    // if (Serial1.available())
    // {
    //     byte incomingByte = Serial1.read();
        
    //     if (prev == 255 && incomingByte == 23)
    //     {
    //         Serial.println("Packet start OK");
    //         Serial.println(Serial1.available());
    //         loraData[0] = Serial1.read();
    //         loraData[1] = Serial1.read();
    //         loraData[2] = Serial1.read();
    //         loraData[3] = Serial1.read();
    //         loraData[4] = Serial1.read();
    //         Serial.println(Serial1.available());
    //         clientPacket["RSSI"] = loraData[4];
    //         clientPacket["byte0"] = loraData[0];
    //         clientPacket["byte1"] = loraData[1];
    //         clientPacket["byte2"] = loraData[2];
    //         clientPacket["byte3"] = loraData[3];
    //         
    //         else clientPacket["PacketCheck"] = 0;
    //         // Serial.println(loraData[4]);
    //         ws.textAll(JSON.stringify(clientPacket));
    //         prev = loraData[4];
    //     }
    //     else
    //     {
    //         prev = incomingByte;
    //     }
    // }
    ws.cleanupClients();

}
