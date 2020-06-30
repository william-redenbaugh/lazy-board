#include "lwip_thread.hpp"

/**************************************************************************/
/*!
    @brief Thread function and stack space for our lwip thread, where all low priority work will sit. 
*/
/**************************************************************************/
static THD_WORKING_AREA(lwip_thread_wa, 2048);
static THD_FUNCTION(lwip_thread, arg){
    for(;;){
        chThdSleepSeconds(1);
    }
}

/**************************************************************************/
/*!
    @brief Starts up our low priority work thread. 
*/
/**************************************************************************/
extern void setup_lwip_thread(void){
    chThdCreateStatic(lwip_thread_wa, 
                      sizeof(lwip_thread_wa), 
                      NORMALPRIO - 2, 
                      lwip_thread, 
                      NULL);
}
