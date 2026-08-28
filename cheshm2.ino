// ==================================================
// ParaxFace2
// ESP8266 + GC9A01 + SD + WiFi
// ==================================================

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

#include "display.h"
#include "wifi.h"
#include "web.h"


// ==================================================
// SETUP
// ==================================================

void setup()
{
    Serial.begin(115200);

    delay(500);

    Serial.println();
    Serial.println("==============================");
    Serial.println("       ParaxFace2");
    Serial.println("==============================");


    // ------------------------------------------------
    // TFT
    // ------------------------------------------------

    displayBegin();

    Serial.println("TFT OK");


    // ------------------------------------------------
    // SD
    // ------------------------------------------------

    if (!SD.begin(D2))
    {
        Serial.println("SD CARD ERROR!");

        tft.fillScreen(TFT_BLACK);

        tft.setTextColor(TFT_RED);
        tft.setTextSize(2);
        tft.setCursor(30, 105);
        tft.println("SD ERROR");

        return;
    }

    Serial.println("SD CARD OK");


    // ------------------------------------------------
    // WiFi
    // ------------------------------------------------

    wifiBegin();


    // ------------------------------------------------
    // WEB SERVER
    // ------------------------------------------------

    webBegin();


    // ------------------------------------------------
    // اولین عکس
    // ------------------------------------------------

    if (findFirstImage())
    {
        Serial.println("Image found!");

        showImage(firstImageName.c_str());
    }
    else
    {
        Serial.println("No image found.");

        showUploadMessage();
    }
}


// ==================================================
// LOOP
// ==================================================

void loop()
{
    webHandle();
}