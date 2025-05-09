#ifndef _BUTTON_GPIO_H
#define _BUTTON_GPIO_H

// Arduino and Will-OS Libraries 
#include "Arduino.h"
#include "OS/OSThreadKernel.h"
#include "OS/OSMutexKernel.h"

// We are throwing the button gpio stuff on our lpriority work thread. 
#include "MODULES/LPWORK/lp_work_thread.h"

// The runtime of the different hardware interaction hardware. 
#include "led_matrix_runtime.hpp"
#include "OLED_display_runtime.hpp"
#include "keyboard_runtime_thread.hpp"

void start_button_gpio(void); 

#endif 