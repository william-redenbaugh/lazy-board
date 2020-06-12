#include "led_matrix_runtime.hpp"

LEDMatrixConfiguration current_matrix_config; 

RgbColor static_all_col = {100, 100, 100};
RgbColor static_individual_col[NUM_MATRIX_LEDS];

void matrix_cycle_individual(void);
void matrix_static_all(void);
void matrix_static_individual(void);

extern void start_led_strip_thread(void);

/**************************************************************************/
/*!
    @brief wipes through the rgb array through the numpad. 
*/
/**************************************************************************/
void matrix_cycle_individual(void){
    // Looping runtime. 
    for(uint8_t h = 0; h < 255; h++){
        for(uint8_t y = 0; y < 4; y++){
            for(uint8_t x = 0; x < 4; x++){
                uint8_t hue_val = (uint8_t)((y* 9 * x) + h ) % 255; 
                _set_ws2812b_led_hsv(x, y, hue_val, 255, 20);
            }
        }
        _update_ws2812b_matrix();
        chThdSleepMilliseconds(current_matrix_config.interval_speed);
    } 
}

/**************************************************************************/
/*!
    @brief Allows us to statically set all LEDs perodically. 
*/
/**************************************************************************/
void matrix_static_all(void){
    _set_ws2812b_led_all(static_all_col.r, static_all_col.g, static_all_col.b);
    _update_ws2812b_matrix();

    chThdSleepMilliseconds(current_matrix_config.interval_speed);
}

void matrix_static_individual(void){
    for(uint8_t i = 0; i < NUM_MATRIX_LEDS; i++)
        _set_ws2812b_macro((led_macro_t)i, static_individual_col[i].r, static_individual_col[i].g, static_individual_col[i].b);
    _update_ws2812b_matrix();
    
    chThdSleepMilliseconds(current_matrix_config.interval_speed);
}

/**************************************************************************/
/*!
    @brief Thread function and stack space for our led matrix stuff. 
*/
/**************************************************************************/
static THD_WORKING_AREA(led_matrix_thread_wa, 4096);
static THD_FUNCTION(led_matrix_thread, arg){
    (void)arg; 
    
    _start_ws2812b_matrix();
    
    for(;;){
        switch(current_matrix_config.matrix_animation_enum){
        case(MATRIX_CYCLE_INDIVIDUAL):
            matrix_cycle_individual();
        break;
        case(MATRIX_CYCLE_ALL):
        
        break;
        case(MATRIX_REACTIVE):
        
        break;
        case(MATRIX_RIPPLE):
        
        break;
        case(MATRIX_STATIC_INDIVIDUAL):
        matrix_static_individual();
        break;
        case(MATRIX_STATIC_ALL):
        matrix_static_all();
        break;
        default:
        break;
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