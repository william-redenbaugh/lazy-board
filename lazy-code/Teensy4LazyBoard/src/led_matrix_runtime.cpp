#include "led_matrix_runtime.hpp"
#include "OctoWS2811.h"

// OCTOWS2811 PIN SETUP BEGIN // 

// Number of gpio pins aka strips we are attaching
const int num_pins = 1; 

// the list of pins we are using. 
byte pin_list[num_pins] = {14};

// number of leds per strip
const int leds_per_strip = 16;

// dma memory space for reading the led strips. 
DMAMEM int display_memory[leds_per_strip*num_pins*(3/4)];
int drawing_memory[leds_per_strip * num_pins * 3 / 4];
const int config = WS2811_GRB | WS2811_800kHz;

// OctoWS2811 object for dealing with large arrays of LEDs. 
OctoWS2811 leds(leds_per_strip, display_memory, drawing_memory, config, num_pins, pin_list);

// OCTOWS2811 PIN SETUP END // 

extern void start_led_strip_thread(void);

static THD_WORKING_AREA(led_matrix_thread_wa, 4096);
static THD_FUNCTION(led_matrix_thread, arg){
    (void)arg; 
    
    leds.begin();
    leds.show();
    
    // Looping runtime. 
    for(;;){
        chThdSleepSeconds(1);
    }   
}

extern void start_led_strip_thread(void){
    chThdCreateStatic(led_matrix_thread_wa, 
                      sizeof(led_matrix_thread_wa), 
                      NORMALPRIO + 3, 
                      led_matrix_thread, 
                      NULL);
}