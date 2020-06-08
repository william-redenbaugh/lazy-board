#include "led_matrix_runtime.hpp"

extern void start_led_strip_thread(void);

static THD_WORKING_AREA(led_matrix_thread_wa, 4096);
static THD_FUNCTION(led_matrix_thread, arg){
    (void)arg; 

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