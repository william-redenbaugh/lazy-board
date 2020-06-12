// Arduino and ChibiOS Libraries 
#include "Arduino.h"
#include "ChRt.h"

// Runtime files.
#include "keyboard_runtime_thread.hpp"
#include "keyboard_gpio.hpp"
#include "spi_display_runtime.hpp"
#include "led_matrix_runtime.hpp"
#include "message_management_thread.hpp"

/**************************************************************************/
/*!
    @brief Where we call our chibiOS initialization functions. d 
*/
/**************************************************************************/
void chibiSetup(void){
  
  // Starting our different threads for dealing with communication. 
  //start_message_management();
  //start_keyboard_runtime_thread();
  start_led_strip_thread();
  start_spi_display_thread();
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