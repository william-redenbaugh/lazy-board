#include <Arduino.h>

// Including Protobuffer libraries 
#include "pb.h"
#include "pb_common.h"
#include "pb_decode.h"
#include "pb_encode.h"

// Including our OS stuff. 
#include "OS/OSThreadKernel.h"
#include "OS/OSSignalKernel.h"
#include "OS/OSMutexKernel.h"

// #include our Serial interfacing drivers 
#include "HAL/OSSerial.h"

// Including our Operating system modules 
#include "MODULES/LPWORK/lp_work_thread.h"
#include "MODULES/PROTOCALLBACKS/teensy_coms.h"

// Keyboard GPIO stuff. 
#include "keyboard_runtime_thread.hpp"

void setup() {
  // Starting up our RTOS. 
  os_init(); 
  
  // Setup our Low priority work thread. 
  setup_lwip_thread(); 

  // Sets up our message callback thread. 
  message_callbacks_begin(); 
}

void loop() {
  keyboard_runtime_func();
}