// Arduino and ChibiOS Libraries 
#include "Arduino.h"
#include "ChRt.h"

// Runtime files.
#include "keyboard_runtime_thread.hpp"
#include "keyboard_gpio.hpp"
#include "spi_display_runtime.hpp"
#include "led_matrix_runtime.hpp"

void chibiSetup(void){
  
  // Starting our different threads for dealing with communication. 
  start_keyboard_runtime_thread();
  start_led_strip_thread();
  start_spi_display_thread();

}

void setup() {
  // We do all of our setup in here. 
  chBegin(chibiSetup);
}

// 
void loop() {
  chThdSleepSeconds(1);
}