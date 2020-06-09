// Arduino header file
#include <Arduino.h>

// ChibiOS header file
#include "ChRt.h"

/**************************************************************************/
/*!
    @brief Where we call our chibiOS initialization functions. d 
*/
/**************************************************************************/
void chibiSetup(void){
<<<<<<< HEAD
  
  // Starting our different threads for dealing with communication. 
  start_keyboard_runtime_thread();
  start_led_strip_thread();
  start_spi_display_thread();
=======

>>>>>>> parent of 5cdcd64... set pin denominations and created thread and stack space for all of our runtime threads
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

<<<<<<< HEAD
/**************************************************************************/
/*!
    @brief Main loop that will eventually have stuff in it. 
*/
/**************************************************************************/
=======
>>>>>>> parent of 5cdcd64... set pin denominations and created thread and stack space for all of our runtime threads
void loop() {
  chThdSleepSeconds(1);
}