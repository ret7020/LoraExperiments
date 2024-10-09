
#define ESP32
#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include <SPI.h>
#include <LoRa.h>
#include <WiFi.h>
#include <Arduino_JSON.h>

int counter = 0;

// LoRa wiring
#define LORA_SCK 14
#define LORA_MISO 12
#define LORA_MOSI 13

#define LORA_SS 15
#define LORA_RST 33
#define LORA_DIO0 32

// HTTP
#define HTTP_PORT 80

// LoRa modulation params
#define START_FREQ 433E6

AsyncWebServer server(HTTP_PORT);
AsyncWebSocket ws("/ws");
JSONVar packetFromSocket;

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
    packetFromSocket = JSON.parse((char*)data);
    // Packet struct
    /*
    // action == 0 - configure LoRa options
    {"action": 0, "bw": 12, "sf": int, "cr", "pl": int, "crc": bool, "txp": int, "iq": bool, "dl": int}
        bw - Bandwidth
        sf - Spreadig Factor
        cr - Coding Rate
        pl - Preamble length
        crc - Checksum
        txp - tx power
        iq - Invert I/Q Signals
        dl - delay between sending packages in ms
    */
}

void websocketEventHandler(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    if (type == WS_EVT_DATA)
        handleWebSocketMessage(arg, data, len);
    // else if (type == WS_EVT_CONNECT) Serial.printf("New client\n");
}




void setup()
{
    Serial.begin(115200);

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.println("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(100);
    }
    Serial.println("\nConnected to the WiFi network");
    Serial.println(WiFi.localIP());

    SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI);

    while (!Serial)
        ;

    Serial.println("LoRa Sender");
    LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);

    if (!LoRa.begin(START_FREQ))
    {
        Serial.println("Starting LoRa failed!");
        while (1)
            ;
    }
    LoRa.setSpreadingFactor(12);
    LoRa.setSignalBandwidth(125E3);
    Serial.println("Lora Ready");

    // Init top-level http stack
    ws.onEvent(websocketEventHandler);
    server.addHandler(&ws);
    server.begin();
}

void loop()
{
    //  // send packet
    // LoRa.beginPacket();
    // LoRa.print(" hello ");
    // LoRa.print(counter);
    // LoRa.endPacket();
    //
    //  counter++;
    //
    delay(1000);
    ws.cleanupClients();
}