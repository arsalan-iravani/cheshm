#ifndef WEB_H
#define WEB_H

#include <Arduino.h>


// ==================================================
// WEB
// ==================================================

void webBegin();

void webHandle();


// ==================================================
// IMAGE
// ==================================================

bool findFirstImage();

extern String firstImageName;


// ==================================================
// IMAGE FUNCTIONS
// ==================================================

void deleteImage(String filename);

void showSelectedImage(String filename);


#endif