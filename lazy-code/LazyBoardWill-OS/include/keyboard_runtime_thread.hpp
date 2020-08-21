/*
Author: William Redenbaugh
Last Edit Date: 7/20/2020
*/

#ifndef _KEYBOARD_RUNTIME_THREAD_HPP
#define _KEYBOARD_RUNTIME_THREAD_HPP

// Arduino and ChibiOS Libraries 
#include "Arduino.h"
#include "OS/OSThreadKernel.h"

// Keyboard gpio reading module. 
#include "keyboard_gpio.hpp"

void keyboard_runtime_func(); 
extern void start_keyboard_runtime_thread(void); 
extern void reprogram_key(uint16_t map[], size_t map_size);

#endif 