#include "led_matrix_handler.hpp"

// SERIAL DMA FOR LED MATRIX BEGIN // 

// default number of LEDs. 
const int numled = NUM_MATRIX_LEDS;
// Default GPIO value. 
uint8_t gpio_pin = LED_STRIP_GPIO;

byte drawingMemory[numled*3];         //  3 bytes per LED for RGB
DMAMEM byte displayMemory[numled*12]; // 12 bytes per LED for RGB

WS2812Serial leds = WS2812Serial(numled, displayMemory, drawingMemory, gpio_pin, WS2812_GRB);

// SERIAL DMA FOR LED MATRIX END // 

// Will allow us to start up our matrix from the runtime. 
extern void _start_ws2812b_matrix(void);
extern void _update_ws2812b_matrix(void);

extern void _set_ws2812b_led(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b);
extern void _set_ws2812b_macro(led_macro_t led_macro,  uint8_t r, uint8_t g, uint8_t b);

extern void _set_ws2812b_led_hsv(uint8_t x, uint8_t y, uint8_t h, uint8_t s, uint8_t v);
extern void _set_ws2812b_macro_hsv(led_macro_t led_macro, uint8_t h, uint8_t s, uint8_t v);

extern void _start_ws2812b_matrix(void){
    leds.begin();
    leds.show();
}

/**************************************************************************/
/*!
    @brief LED matrix function that converts x and y corridinates into LED strip values since matrix is actually a strip
    @param uint8_t x, uint8_t y(positional values), uint8_t r, uint8_t g, uint8_t b(color values)
*/
/**************************************************************************/
extern void _set_ws2812b_led(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b){
    switch(x){
    case(LED_ROW_0):
        switch(y){
        case(LED_COL_0):
        leds.setPixelColor(0, r, g, b);
        break;
        
        case(LED_COL_1):
        leds.setPixelColor(1, r, g, b);
        break;
        
        case(LED_COL_2):
        leds.setPixelColor(2, r, g, b);
        break;
        
        case(LED_COL_3):
        leds.setPixelColor(3, r, g, b);
        break;
        };
    break;
    
    case(LED_ROW_1):
        switch(y){
        
        case(LED_COL_2):
        leds.setPixelColor(5, r, g, b);
        break;
        
        case(LED_COL_3):
        leds.setPixelColor(4, r, g, b);
        break;
        
        default:
        break;
        };
    break;

    case(LED_ROW_2):
        switch(y){
        
        case(LED_COL_2):
        leds.setPixelColor(6, r, g, b);
        break;
        
        case(LED_COL_3):
        leds.setPixelColor(7, r, g, b);
        break;
        };
    break;

    case(LED_ROW_3):
        switch(y){
        
        case(LED_COL_0):
        leds.setPixelColor(11, r, g, b);
        break;
        
        case(LED_COL_1):
        leds.setPixelColor(10, r, g, b);
        break;
        
        case(LED_COL_2):
        leds.setPixelColor(9, r, g, b);
        break;
        
        case(LED_COL_3):
        leds.setPixelColor(8, r, g, b);
        break;
        };
    break;
    default:
    break;
    }
}

extern void _set_ws2812b_macro(led_macro_t led_macro,  uint8_t r, uint8_t g, uint8_t b){
    switch (led_macro){
    case(LED_MACRO_0_POS):
        leds.setPixelColor(0, r, g, b);
    break;
    case(LED_MACRO_1_POS):
        leds.setPixelColor(1, r, g, b);
    break;
    case(LED_MACRO_2_POS):
        leds.setPixelColor(2, r, g, b);
    break;
    case(LED_MACRO_3_POS):
        leds.setPixelColor(3, r, g, b);
    break;
    case(LED_MACRO_4_POS):
        leds.setPixelColor(5, r, g, b);
    break;
    case(LED_MACRO_5_POS):
        leds.setPixelColor(4, r, g, b);
    break;
    case(LED_MACRO_7_POS):
        leds.setPixelColor(7, r, g, b);
    break;
    case(LED_MACRO_6_POS):
        leds.setPixelColor(6, r, g, b);
    break;
    case(LED_MACRO_8_POS):
        leds.setPixelColor(11, r, g, b);
    break;
    case(LED_MACRO_9_POS):
        leds.setPixelColor(10, r, g, b);
    break;
    case(LED_MACRO_10_POS):
        leds.setPixelColor(9, r, g, b);
    break;
    case(LED_MACRO_11_POS):
        leds.setPixelColor(8, r, g, b);
    break;
    default:
    break;
    }
}

extern void _set_ws2812b_led_hsv(uint8_t x, uint8_t y, uint8_t h, uint8_t s, uint8_t v){
    HsvColor hsv; 
    hsv.h = h; 
    hsv.s = s; 
    hsv.v = v; 
    RgbColor rgb = HsvToRgb(hsv);

    _set_ws2812b_led(x, y, rgb.r, rgb.g, rgb.g);
}

extern void _set_ws2812b_macro_hsv(led_macro_t led_macro, uint8_t h, uint8_t s, uint8_t v){
    HsvColor hsv; 
    hsv.h = h; 
    hsv.s = s; 
    hsv.v = v; 
    RgbColor rgb = HsvToRgb(hsv);

    _set_ws2812b_macro(led_macro, rgb.r, rgb.g, rgb.g);
}

/**************************************************************************/
/*!
    @brief Since our object is generated in this file, we update it here. 
*/
/**************************************************************************/
extern void _update_ws2812b_matrix(void){
    leds.show();
}