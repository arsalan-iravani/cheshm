#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <TFT_eSPI.h>


// ==================================================
// TFT OBJECT
// ==================================================

extern TFT_eSPI tft;


// ==================================================
// SCREEN
// ==================================================

#define SCREEN_WIDTH   240
#define SCREEN_HEIGHT  240

#define SCREEN_CENTER_X 120
#define SCREEN_CENTER_Y 120


// ==================================================
// DISPLAY FUNCTIONS
// ==================================================

void displayBegin();

void showImage(const char *filename);

void showUploadMessage();


// ==================================================
// IMAGE
// ==================================================

bool imageExists(const char *filename);

bool isImageFile(String filename);

String getFirstImage();


#endif