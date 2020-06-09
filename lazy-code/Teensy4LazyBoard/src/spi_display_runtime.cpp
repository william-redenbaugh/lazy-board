#include "spi_display_runtime.hpp"

extern void start_spi_display_thread(void);

/**************************************************************************/
/*!
    @brief Thread function and stack space for our spi display. 
*/
/**************************************************************************/
static THD_WORKING_AREA(spi_display_thread_wa, 4096);
static THD_FUNCTION(spi_display_thread, arg){
    (void)arg; 

    // Looping runtime. 
    for(;;){
        chThdSleepSeconds(1);
    }   
}

/**************************************************************************/
/*!
    @brief Allows us to setup our spi display thread
*/
/**************************************************************************/
extern void start_spi_display_thread(void){
    chThdCreateStatic(spi_display_thread_wa, 
                      sizeof(spi_display_thread_wa), 
                      NORMALPRIO + 3, 
                      spi_display_thread, 
                      NULL);
}