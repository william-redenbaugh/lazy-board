/*
Author: William Redenbaugh
Last Edit Date: 7/24/2020
*/

#ifndef _OLED_DISPLAY_RUNTIME_HPP
#define _OLED_DISPLAY_RUNTIME_HPP

// Arduino Linker file. 
#include <Arduino.h>

// Our OS Linker files. 
#include "OS/OSThreadKernel.h"
#include "OS/OSMutexKernel.h"
#include "MODULES/PROTOCALLBACKS/teensy_coms.h"

// Our OLED drivers
#include "MatrixOLED.hpp"

void start_display_thread(void); 

#endif 