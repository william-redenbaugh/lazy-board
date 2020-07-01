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

/**************************************************************************/
/*!
    @brief Where we call our chibiOS initialization functions. d 
*/
/**************************************************************************/
void chibiSetup(void){
  // Setting up message managment with the computer
  start_message_management();
  
  // Starting up our runtime threads
  //start_keyboard_runtime_thread();
  //start_led_strip_thread();
  //start_spi_display_thread();

  // This is technically a runtime thread but it holds a bunch of subthreads to do work 
  setup_lwip_thread();

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