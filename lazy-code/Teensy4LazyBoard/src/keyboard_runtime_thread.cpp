#include "keyboard_runtime_thread.hpp"

extern void start_keyboard_runtime_thread(void);

static THD_WORKING_AREA(keyboard_runtime_thread_wa, 4096);
static THD_FUNCTION(keyboard_runtime_thread, arg){
    (void)arg; 

    // Looping runtime. 
    for(;;){
        chThdSleepSeconds(1);
    }   
}

extern void start_keyboard_runtime_thread(void){
    chThdCreateStatic(keyboard_runtime_thread_wa, 
                      sizeof(keyboard_runtime_thread_wa), 
                      NORMALPRIO + 3, 
                      keyboard_runtime_thread, 
                      NULL);
}