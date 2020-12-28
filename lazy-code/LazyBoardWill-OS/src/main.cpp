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

// RGB Matrix stuff 
#include "led_matrix_runtime.hpp"

// The display runtime for our system 
#include "OLED_display_runtime.hpp"

//test formatting images for oled
#include "MatrixOLED.hpp"
#include "example1.hpp" //images stored here
MatrixOLED oled;
void setup() {
  // Starting up our RTOS. 
  os_init(); 

  //test formatting images for oled
  oled.begin();
  for(int i = 0; i < 128;++i){
    for(int j = 0; j < 128; ++j){
      oled.queue_pixel(i,j, img[0][i][j]);
    }
  }
  oled.set_rotation(2);  
  oled.draw_queue();


  // Setup our Low priority work thread. 
  setup_lwip_thread(); 

  // Sets up our message callback thread. 
  message_callbacks_begin(); 

  // Starts up our LED strip thread. 
  start_led_strip_thread();   

  // Starts up our display thread
  //start_display_thread();
}

/*
* @brief This is just playing into the Arduino's functions.
* @notes The keyboard runtime function never returns, so it's not really a loop
*/
void loop() {
  keyboard_runtime_func();
}