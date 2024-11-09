#include <SPI.h>
#include <LoRa.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>

uint8_t rxData[10];
int packetSize = 0;
bool newPacket = 0;
int newPacketRSSI = 0;
float newPacketSNR = 0.0;
int D2Status = 0;

// LoRa wiring
#define LORA_SCK 14
#define LORA_MISO 12
#define LORA_MOSI 13

#define LORA_SS 15
#define LORA_RST 32
#define LORA_DIO0 33

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void onReceive(int packetSize)
{
    for (int i = 0; i < packetSize; i++)
    {
        rxData[i] = LoRa.read();
    }
    if (rxData[0] == 100 && rxData[1] == 200 && rxData[2] == 250)
    {
        newPacketRSSI = LoRa.packetRssi();
        newPacketSNR = LoRa.packetSnr();
        newPacket = 1;
    }
}
void setup()
{
    Serial.begin(115200);
    while (!Serial)
        ;

    Serial.println("LoRa RX");
    pinMode(2, OUTPUT);

    SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI);
    LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);
    if (!LoRa.begin(433E6))
    {
        Serial.println("Starting LoRa failed!");
        while (1)
            ;
    }

    LoRa.setSignalBandwidth(125E3);
    LoRa.setSpreadingFactor(12);

    LoRa.receive();
    LoRa.onReceive(onReceive);

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.println("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED)
    {
        digitalWrite(2, D2Status = !D2Status);
        Serial.print(".");
        delay(100);
    }
    digitalWrite(2, D2Status = 0);

    Serial.println("\nConnected to the WiFi network");
    Serial.println(WiFi.localIP());
    server.addHandler(&ws);
    server.begin();
}

void loop()
{
    if (newPacket)
    {
        char sendPkg[100];
        sprintf(sendPkg, "New packet: %d with RSSI: %d and SNR: %f\n", rxData[3], newPacketRSSI, newPacketSNR);
        ws.textAll(sendPkg);
        newPacket = 0;
        digitalWrite(2, D2Status = !D2Status);
    }
    ws.cleanupClients();
}