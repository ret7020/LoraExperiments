#include <SPI.h>
#include <LoRa.h>
#include <WiFi.h>

int counter = 0;

#define LORA_SCK 14
#define LORA_MISO 12
#define LORA_MOSI 13

#define LORA_SS 15
#define LORA_RST 33
#define LORA_DIO0 32

#define START_FREQ 433E6

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

    while (!Serial);

    Serial.println("LoRa Sender");
    LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);

    if (!LoRa.begin(START_FREQ))
    {
        Serial.println("Starting LoRa failed!");
        while (1);
    }
    LoRa.setSpreadingFactor(12);
    LoRa.setSignalBandwidth(125E3);
    Serial.println("Lora Ready");
}

void loop()
{
    //  Serial.print("Sending packet: ");
    //  Serial.println(counter);
    //
    //  // send packet
    //  LoRa.beginPacket();
    //  LoRa.print(" hello ");
    //  LoRa.print(counter);
    //  LoRa.endPacket();
    //
    //  counter++;
    //
    //  delay(1000);
}