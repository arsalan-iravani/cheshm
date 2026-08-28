#include "wifi.h"

#include <ESP8266WiFi.h>


// ==================================================
// WIFI SETTINGS
// ==================================================

const char *WIFI_SSID =
    "ParaxFace2";

const char *WIFI_PASSWORD =
    "12345678";


// ==================================================
// WIFI BEGIN
// ==================================================

void wifiBegin()
{
    Serial.println();
    Serial.println("==============================");
    Serial.println("       WIFI START");
    Serial.println("==============================");


    WiFi.mode(WIFI_AP);


    bool result =
        WiFi.softAP(
            WIFI_SSID,
            WIFI_PASSWORD
        );


    if (!result)
    {
        Serial.println("WiFi AP ERROR!");

        return;
    }


    delay(500);


    Serial.print("SSID: ");
    Serial.println(WIFI_SSID);


    Serial.print("IP: ");
    Serial.println(WiFi.softAPIP());


    Serial.println("==============================");
}


// ==================================================
// WIFI IP
// ==================================================

String wifiIP()
{
    return WiFi.softAPIP().toString();
}