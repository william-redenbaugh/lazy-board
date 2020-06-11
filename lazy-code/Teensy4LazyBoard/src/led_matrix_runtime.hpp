#ifndef _LED_STRIP_RUNTIME_HPP
#define _LED_STRIP_RUNTIME_HPP

// Arduino and ChibiOS Libraries 
#include "Arduino.h"
#include "ChRt.h"

// WS2812Serial DMA Bitbang library
#include "WS2812Serial.h"

// Our LED matrix handler library. 
#include "led_matrix_handler.hpp"

extern void start_led_strip_thread(void);

#endif