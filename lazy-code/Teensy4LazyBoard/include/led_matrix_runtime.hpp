#ifndef _LED_STRIP_RUNTIME_HPP
#define _LED_STRIP_RUNTIME_HPP

// Arduino and ChibiOS Libraries 
#include "Arduino.h"
#include "ChRt.h"

// WS2812Serial DMA Bitbang library
#include "WS2812Serial.h"

// Our LED matrix handler library. 
#include "led_matrix_handler.hpp"

enum LEDMatrixAnimation{
    MATRIX_CYCLE_INDIVIDUAL, 
    MATRIX_CYCLE_ALL, 
    MATRIX_REACTIVE, 
    MATRIX_RIPPLE,
    MATRIX_STATIC_ALL, 
    MATRIX_STATIC_INDIVIDUAL
};

struct LEDMatrixConfiguration{
    uint32_t interval_speed = 17; 
    LEDMatrixAnimation matrix_animation_enum = MATRIX_REACTIVE;  
};

extern void start_led_strip_thread(void);
extern void change_led_config(LEDMatrixConfiguration config);
extern void change_led_animation(LEDMatrixAnimation animation);
extern void trigger_keymap(volatile uint8_t key_pressed_map[]);

#endif