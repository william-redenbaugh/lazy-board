#ifndef _HSV_RGB_CONV_HPP
#define _HSV_RGB_CONV_HPP

typedef struct RgbColor{
    unsigned char r;
    unsigned char g;
    unsigned char b;
} RgbColor;

typedef struct HsvColor{
    unsigned char h;
    unsigned char s;
    unsigned char v;
} HsvColor;

HsvColor RgbToHsv(RgbColor rgb);
RgbColor HsvToRgb(HsvColor hsv);

#endif 
