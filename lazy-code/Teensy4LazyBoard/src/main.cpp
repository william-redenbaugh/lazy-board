// Arduino header file
#include <Arduino.h>

// ChibiOS header file
#include "ChRt.h"

void chibiSetup(void){

}

void setup() {
  // We do all of our setup in here. 
  chBegin(chibiSetup);
}

void loop() {
  chThdSleepSeconds(1);
}