/*
Author: William Redenbaugh
Last Edit Date: 7/3/2020
*/

#ifndef _BUTTON_GPIO_HPP
#define _BUTTON_GPIO_HPP

#include "Arduino.h"
#include "ChRt.h"
#include "lwip_thread.hpp"

#define DIP1 17
#define DIP2 16
#define DIP3 15
#define BUTTON 14

extern void setup_dip_button(void);
#endif 