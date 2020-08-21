/*
Author: William Redenbaugh
Last Edit Date: 7/20/2020
*/

#ifndef _KEYBOARD_RUNTIME_THREAD_HPP
#define _KEYBOARD_RUNTIME_THREAD_HPP

// Arduino and ChibiOS Libraries 
#include "Arduino.h"
#include "OS/OSThreadKernel.h"
#include "OS/OSMutexKernel.h"

// Keyboard gpio reading module. 
#include "keyboard_gpio.hpp"
// Macro keys that we are using 
#include "kb_macros_define.h"
// USB Keyboard Handler from Teensy. 
#include "Keyboard.h"
//#include "led_matrix_runtime.hpp"
#include "program_keybindings.pb.h"
//#include "spi_display_runtime.hpp"
#include "EEPROM.h"

void keyboard_runtime_func(); 
extern void start_keyboard_runtime_thread(void); 
extern void reprogram_key(uint16_t map[], size_t map_size);

#endif 