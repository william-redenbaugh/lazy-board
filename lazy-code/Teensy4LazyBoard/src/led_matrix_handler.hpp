#ifndef _LED_MATRIX_HANDLER_HPP
#define _LED_MATRIX_HANDLER_HPP

#include "ChRt.h"
#include "WS2812Serial.h"
#include "Arduino.h"

// Usable pins:
// Teensy 4.0:  1, 8, 14, 17, 20, 24, 29, 39

// GPIO pin that we will put our led strip on. 
#define LED_STRIP_GPIO 14

// For us on our prototyping board. 
#define NUM_MATRIX_LEDS 12

enum led_rows_t{
    LED_ROW_0 = 0, 
    LED_ROW_1 = 1, 
    LED_ROW_2 = 2, 
    LED_ROW_3 = 3
};

enum led_cols_t{
    LED_COL_0 = 0, 
    LED_COL_1 = 1, 
    LED_COL_2 = 2,
    LED_COL_3 = 3
};

extern void _start_ws2812b_matrix(void);
extern void _update_ws2812b_matrix(void);

#endif 