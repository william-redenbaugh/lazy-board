/*
Author: William Redenbaugh
Last Edit Date: 7/3/2020
*/

#ifndef _MESSAGE_MANAGEMENT_THREAD_HPP
#define _MESSAGE_MANAGEMENT_THREAD_HPP

#include "Arduino.h"
#include "ChRt.h"
#include "MessageManagement.hpp"
#include "keyboard_runtime_thread.hpp"
#include "led_matrix_runtime.hpp"
#include "spi_display_runtime.hpp"

extern void start_message_management(void);
extern void loop(void);

#endif 