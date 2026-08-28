// ==================================================
// ParaxFace2 - Procedural Eye Animation System
// ESP8266 + GC9A01 240x240 TFT
// 
// Complete rewrite for real-time procedural eye
// rendering with WiFi control
// ==================================================

#include <Arduino.h>
#include <SPI.h>

#include "display.h"
#include "wifi.h"
#include "web.h"
#include "eye.h"
#include "animation.h"
#include "config.h"

// ==================================================
// ANIMATION TIMING
// ==================================================

#define FRAME_TIME_MS 16  // ~60 FPS (1000/60 = 16.67ms)

unsigned long lastFrameTime = 0;
unsigned long lastWebHandleTime = 0;

// ==================================================
// SETUP
// ==================================================

void setup()
{
    // Serial
    Serial.begin(115200);
    delay(500);

    Serial.println();
    Serial.println("==============================");
    Serial.println("    ParaxFace2");
    Serial.println("  Procedural Eye Animation");
    Serial.println("==============================");

    // TFT Display
    Serial.println("\n[INIT] TFT Display...");
    displayBegin();
    Serial.println("[OK] TFT Display initialized");

    // Eye System
    Serial.println("[INIT] Eye System...");
    eyeInit();
    animationInit();
    Serial.println("[OK] Eye System initialized");

    // WiFi
    Serial.println("\n[INIT] WiFi...");
    wifiBegin();

    // Web Server
    Serial.println("[INIT] Web Server...");
    webBegin();

    Serial.println("\n==============================");
    Serial.println("       Ready to animate!");
    Serial.println("==============================\n");

    lastFrameTime = millis();
    lastWebHandleTime = millis();
}

// ==================================================
// LOOP - NON-BLOCKING ANIMATION + WEB SERVER
// ==================================================

void loop()
{
    unsigned long now = millis();

    // ------------------------------------------------
    // Animation Loop (60 FPS target)
    // ------------------------------------------------
    if (now - lastFrameTime >= FRAME_TIME_MS)
    {
        // Update animation state
        animationUpdate();

        // Render eye to TFT
        eyeRender();

        lastFrameTime = now;
    }

    // ------------------------------------------------
    // Web Server (non-blocking)
    // ------------------------------------------------
    if (now - lastWebHandleTime >= 10)
    {
        server.handleClient();
        lastWebHandleTime = now;
    }

    // ------------------------------------------------
    // Prevent Watchdog Timeout
    // ------------------------------------------------
    yield();
}
