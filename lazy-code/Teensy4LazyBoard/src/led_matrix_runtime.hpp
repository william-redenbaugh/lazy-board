#ifndef _LED_STRIP_RUNTIME_HPP
#define _LED_STRIP_RUNTIME_HPP

// Arduino and ChibiOS Libraries 
#include "Arduino.h"
#include "ChRt.h"

// GPIO pin that we will put our led strip on. 
#define LED_STRIP_GPIO 14

// For us on our prototyping board. 
#define NUM_MATRIX_LEDS 12

extern void start_led_strip_thread(void);

#endif