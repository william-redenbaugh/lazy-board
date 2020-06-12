#include "led_matrix_runtime.hpp"

extern void start_led_strip_thread(void);

#define RED    0x160000
#define GREEN  0x001600
#define BLUE   0x000016
#define YELLOW 0x101400
#define PINK   0x120009
#define ORANGE 0x100400
#define WHITE  0x101010

/**************************************************************************/
/*!
    @brief Thread function and stack space for our led matrix stuff. 
*/
/**************************************************************************/
static THD_WORKING_AREA(led_matrix_thread_wa, 4096);
static THD_FUNCTION(led_matrix_thread, arg){
    (void)arg; 
    
    _start_ws2812b_matrix();
    
    // Looping runtime. 
    for(;;){
        for(uint8_t h = 0; h < 255; h++){
            for(uint8_t y = 0; y < 4; y++){
                for(uint8_t x = 0; x < 4; x++){
                    uint8_t hue_val = (uint8_t)((y* 9 * x) + h ) % 255; 
                    _set_ws2812b_led_hsv(x, y, hue_val, 255, 20);
                }
            }
            _update_ws2812b_matrix();
            chThdSleepMilliseconds(20);
        }
    }   
}

/**************************************************************************/
/*!
    @brief Allows us to setup our led strip/matrix thread.  
*/
/**************************************************************************/
extern void start_led_strip_thread(void){
    chThdCreateStatic(led_matrix_thread_wa, 
                      sizeof(led_matrix_thread_wa), 
                      NORMALPRIO + 3, 
                      led_matrix_thread, 
                      NULL);
}