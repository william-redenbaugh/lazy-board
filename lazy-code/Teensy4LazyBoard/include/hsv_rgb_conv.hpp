#ifndef _HSV_RGB_CONV_HPP
#define _HSV_RGB_CONV_HPP

#include "Arduino.h"
#include "ChRt.h"

typedef struct RgbColor{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} RgbColor;

typedef struct HsvColor{
    uint8_t h;
    uint8_t s;
    uint8_t v;
} HsvColor;

HsvColor RgbToHsv(RgbColor rgb);
RgbColor HsvToRgb(HsvColor hsv);

#endif 
