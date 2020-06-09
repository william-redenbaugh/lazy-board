// Arduino and ChibiOS Libraries 
#include "Arduino.h"
#include "ChRt.h"

// Runtime files.
#include "keyboard_runtime_thread.hpp"
#include "keyboard_gpio.hpp"
#include "spi_display_runtime.hpp"
#include "led_matrix_runtime.hpp"

/**************************************************************************/
/*!
    @brief Where we call our chibiOS initialization functions. d 
*/
/**************************************************************************/
void chibiSetup(void){
  
  // Starting our different threads for dealing with communication. 
  start_keyboard_runtime_thread();
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

/**************************************************************************/
/*!
    @brief Main loop that will eventually have stuff in it. 
*/
/**************************************************************************/
void loop() {
  chThdSleepSeconds(1);
}