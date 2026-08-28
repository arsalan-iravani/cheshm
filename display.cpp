#include "display.h"

#include <SPI.h>
#include <SD.h>
#include <TJpg_Decoder.h>


// ==================================================
// TFT
// ==================================================

TFT_eSPI tft = TFT_eSPI();


// ==================================================
// JPEG CALLBACK
// ==================================================

bool jpegOutput(
    int16_t x,
    int16_t y,
    uint16_t w,
    uint16_t h,
    uint16_t *bitmap
)
{
    if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT)
        return true;


    // جلوگیری از بیرون رفتن از صفحه

    if (x + w > SCREEN_WIDTH)
        w = SCREEN_WIDTH - x;

    if (y + h > SCREEN_HEIGHT)
        h = SCREEN_HEIGHT - y;


    // انتقال بلوک JPEG به TFT

    tft.pushImage(
        x,
        y,
        w,
        h,
        bitmap
    );

    return true;
}


// ==================================================
// DISPLAY BEGIN
// ==================================================

void displayBegin()
{
    tft.init();

    tft.setRotation(0);

    tft.fillScreen(TFT_BLACK);

    tft.setSwapBytes(true);


    // JPEG decoder

    TJpgDec.setJpgScale(1);

    TJpgDec.setCallback(jpegOutput);


    Serial.println("Display initialized");
}


// ==================================================
// SHOW IMAGE
// ==================================================

void showImage(const char *filename)
{
    Serial.println();
    Serial.println("------------------------------");
    Serial.print("Showing: ");
    Serial.println(filename);


    if (!SD.exists(filename))
    {
        Serial.println("Image does not exist!");

        showUploadMessage();

        return;
    }


    // پاک کردن صفحه

    tft.fillScreen(TFT_BLACK);


    // ------------------------------------------------
    // دریافت اندازه JPEG
    // ------------------------------------------------

    uint16_t w = 0;
    uint16_t h = 0;

    JRESULT result =
        TJpgDec.getSdJpgSize(
            &w,
            &h,
            filename
        );


    if (result != JDR_OK)
    {
        Serial.print("JPEG size error: ");
        Serial.println(result);

        tft.fillScreen(TFT_BLACK);

        tft.setTextColor(TFT_RED);
        tft.setTextSize(2);
        tft.setCursor(25, 105);
        tft.println("JPEG ERROR");

        return;
    }


    Serial.print("JPEG Width: ");
    Serial.println(w);

    Serial.print("JPEG Height: ");
    Serial.println(h);


    // ------------------------------------------------
    // محاسبه محل قرارگیری
    // ------------------------------------------------

    int x = 0;
    int y = 0;


    // اگر عکس 240x240 باشد
    // دقیقاً تمام صفحه را پر می‌کند.

    if (w < 240)
        x = (240 - w) / 2;

    if (h < 240)
        y = (240 - h) / 2;


    // ------------------------------------------------
    // نمایش
    // ------------------------------------------------

    result =
        TJpgDec.drawSdJpg(
            x,
            y,
            filename
        );


    if (result != JDR_OK)
    {
        Serial.print("JPEG draw error: ");
        Serial.println(result);

        tft.fillScreen(TFT_BLACK);

        tft.setTextColor(TFT_RED);
        tft.setTextSize(2);
        tft.setCursor(25, 105);
        tft.println("DRAW ERROR");

        return;
    }


    Serial.println("JPEG displayed.");
    Serial.println("------------------------------");
}


// ==================================================
// UPLOAD MESSAGE
// ==================================================

void showUploadMessage()
{
    tft.fillScreen(TFT_BLACK);

    tft.setTextColor(TFT_WHITE);

    tft.setTextSize(2);

    tft.setCursor(42, 80);
    tft.println("PARAX");

    tft.setCursor(25, 110);
    tft.println("UPLOAD");

    tft.setCursor(48, 140);
    tft.println("IMAGE");
}


// ==================================================
// IMAGE EXISTS
// ==================================================

bool imageExists(const char *filename)
{
    return SD.exists(filename);
}


// ==================================================
// CHECK IMAGE FILE
// ==================================================

bool isImageFile(String filename)
{
    filename.toLowerCase();


    if (filename.endsWith(".jpg"))
        return true;

    if (filename.endsWith(".jpeg"))
        return true;


    return false;
}


// ==================================================
// FIND FIRST IMAGE
// ==================================================

String getFirstImage()
{
    File root = SD.open("/");

    if (!root)
        return "";


    File file;


    while (true)
    {
        file = root.openNextFile();

        if (!file)
            break;


        if (!file.isDirectory())
        {
            String name = file.name();

            file.close();


            if (isImageFile(name))
            {
                root.close();

                return name;
            }
        }


        file.close();
    }


    root.close();

    return "";
}