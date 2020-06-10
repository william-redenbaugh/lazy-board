#ifndef _KEYBOARD_RUNTIME_THREAD_HPP
#define _KEYBOARD_RUNTIME_THREAD_HPP

// Arduino and ChibiOS Libraries 
#include "Arduino.h"
#include "ChRt.h"

// Keyboard gpio reading module. 
#include "keyboard_gpio.hpp"

extern void start_keyboard_runtime_thread(void); 

#endif 