/*
Author: William Redenbaugh
Last Edit Date: 7/3/2020
*/

#ifndef _LED_STRIP_RUNTIME_HPP
#define _LED_STRIP_RUNTIME_HPP

// Arduino and ChibiOS Libraries 
#include "Arduino.h"
#include "OS/OSMutexKernel.h"
#include "OS/OSSignalKernel.h"
#include "OS/OSThreadKernel.h"

// Our LED matrix handler library. 
#include "led_matrix_handler.hpp"

// Our teensy messaging callbacks
#include "MODULES/PROTOCALLBACKS/teensy_coms.h"

extern void start_led_strip_thread(void);
extern void trigger_keymap(volatile uint8_t key_pressed_map[]);

#endif