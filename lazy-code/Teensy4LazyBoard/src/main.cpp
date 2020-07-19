/*
Author: William Redenbaugh
Last Edit Date: 7/3/2020
*/

// Arduino and ChibiOS Libraries 
#include "Arduino.h"
#include "ChRt.h"

// Runtime files.
#include "keyboard_runtime_thread.hpp"
#include "keyboard_gpio.hpp"
#include "spi_display_runtime.hpp"
#include "led_matrix_runtime.hpp"
#include "message_management_thread.hpp"
#include "lwip_thread.hpp"
#include "button_gpio.hpp"

void setup_application_threads(void);
void setup_lp_threads(void);

/**************************************************************************/
/*!
    @brief Where we call our chibiOS initialization functions.
*/
/**************************************************************************/
void chibiSetup(void){
  // This is technically a runtime thread but it holds a bunch of subthreads to do work 
  //setup_lwip_thread();

  // Setting up message managment with the computer
  start_message_management();
  setup_application_threads();
  setup_lp_threads();
}

/**************************************************************************/
/*!
    @brief Where we call the initialization of our chibiOS application level threads. 
*/
/**************************************************************************/
void setup_application_threads(void){
  // Starting up our runtime threads
  start_keyboard_runtime_thread();
  start_led_strip_thread();
  start_spi_display_thread();
}

/**************************************************************************/
/*!
    @brief Where we call the initialization of our low priority threads
*/
/**************************************************************************/
void setup_lp_threads(void){
  // Starting up our low priority work threads:
  setup_dip_button();
}

/**************************************************************************/
/*!
    @brief All of our setup happens in here 
*/
/**************************************************************************/
void setup() {
  // We do all of our setup in here. 
  chBegin(chibiSetup);  
}