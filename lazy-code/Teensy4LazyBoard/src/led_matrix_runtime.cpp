/*
Author: William Redenbaugh
Last Edit Date: 7/3/2020
*/

#include "led_matrix_runtime.hpp"

MUTEX_DECL(led_matrix_mutex);
thread_t *led_runtime_handler;

#define CHANGE_MATRIX_BITMASK 0 
eventmask_t event_mask;  
volatile bool interrupt_animation = false; 

LEDMatrixConfiguration current_matrix_config; 
RgbColor static_all_col = {100, 100, 100};
RgbColor static_individual_col[NUM_MATRIX_LEDS];

// Map of keys that are pressed for key triggered animations
uint8_t latest_key_press_map[16];

bool matrix_cycle_individual(void);
bool matrix_trigger_ripple(void);
bool matrix_static_all(void);
bool matrix_static_individual(void);
bool matrix_keytrigger_reactive(void);

extern void start_led_strip_thread(void);
extern void change_led_config(LEDMatrixConfiguration next_matrix_config);
extern void trigger_keymap(volatile uint8_t key_pressed_map[]);

volatile bool keypress_trigger = false; 

/**************************************************************************/
/*!
    @brief wipes through the rgb array through the numpad. 
    @return animation was interrupted. 
*/
/**************************************************************************/
bool matrix_cycle_individual(void){
    // Looping runtime. 
    for(uint8_t h = 0; h < 255; h++){
        chMtxLock(&led_matrix_mutex);
        for(uint8_t y = 0; y < 4; y++){
            for(uint8_t x = 0; x < 4; x++){
                uint8_t hue_val = (uint8_t)((y* 9 * x) + h ) % 255; 
                _set_ws2812b_led_hsv(x, y, hue_val, 255, 20);
            }
        }
        chMtxUnlock(&led_matrix_mutex);
        _update_ws2812b_matrix();
        
        event_mask = chEvtWaitAnyTimeout(EVENT_MASK(CHANGE_MATRIX_BITMASK), 10 * current_matrix_config.interval_speed);
        if(event_mask & EVENT_MASK(CHANGE_MATRIX_BITMASK))
            return 1; 
        if(interrupt_animation)
            return 1; 
 
    }
    return 0; 
}

/**************************************************************************/
/*!
    @brief reactive animations on the matrix, similar to most rgb keyboards. 
    @return animation was interrupted. 
*/
/**************************************************************************/
bool matrix_trigger_ripple(void){
    for(;;){
        if(interrupt_animation)
            return 1; 
    }
    return 0; 
}

/**************************************************************************/
/*!
    @brief Reactive LED pattern. 
    @return animation was interrupted. 
*/
/**************************************************************************/
bool matrix_keytrigger_reactive(void){
    HsvColor current_hsv[NUM_MATRIX_LEDS];
    memset(current_hsv, 0, sizeof(current_hsv));
    memset(latest_key_press_map, 0, sizeof(latest_key_press_map));

    while(1){
        chThdSleepMilliseconds(10);
        if(keypress_trigger){
            keypress_trigger = false; 
            // Run through all of the matrix leds. 
            for(uint8_t y = 0; y < 12; y++){
                // If said key has been triggered
                if(latest_key_press_map[y] == 0){
                    uint8_t x = 0; 
                    switch(y){
                    case(0):
                        x = 0;
                    break;
                    case(3):
                        x = 1;  
                    break; 
                    case(1):
                        x = 2;  
                    break; 
                    case(2): 
                        x = 3;
                    break; 
                    case(4): 
                        x = 4; 
                    break;
                    case(5): 
                        x = 5; 
                    break; 
                    case(7): 
                        x = 6; 
                    break; 
                    case(8): 
                        x = 7; 
                    break;
                    case(11): 
                        x = 11;
                    break; 
                    case(9): 
                        x = 8; 
                    break; 
                    case(10): 
                        x = 10; 
                    break; 
                    case(6): 
                        x = 9; 
                    break;
                    default:
                        continue; 
                    break;
                    }
                    // We update the current hsv with latest keypress values. 
                    current_hsv[x].h = 194;
                    current_hsv[x].s = 170;
                    current_hsv[x].v = 200;
                    _set_ws2812b_macro_hsv((led_macro_t)x, current_hsv[x].h, current_hsv[x].s, current_hsv[x].v);
                }
            }
        }

        // Slowly decrement each LED. 
        for(uint8_t x = 0; x < NUM_MATRIX_LEDS; x++){
            if(!(current_hsv[x].v == 0)){
                current_hsv[x].v = current_hsv[x].v - 1; 
                _set_ws2812b_macro_hsv((led_macro_t)x, current_hsv[x].h, current_hsv[x].s, current_hsv[x].v);
            }
        }

        // Update after each run. 
        _update_ws2812b_matrix();

        if(interrupt_animation)
            return 1; 
    }

    return 0; 
}


/**************************************************************************/
/*!
    @brief Allows us to statically set all LEDs perodically. 
    @return animation was interrupted. 
*/
/**************************************************************************/
bool matrix_static_all(void){
    chMtxLock(&led_matrix_mutex);
    _set_ws2812b_led_all(static_all_col.r, static_all_col.g, static_all_col.b);
    chMtxUnlock(&led_matrix_mutex);
    
    _update_ws2812b_matrix();

    event_mask = chEvtWaitAnyTimeout(EVENT_MASK(CHANGE_MATRIX_BITMASK), 10 * current_matrix_config.interval_speed);
    if(event_mask & EVENT_MASK(CHANGE_MATRIX_BITMASK))
        return 1;
    
    if(interrupt_animation)
        return 1; 

    return 0;  
}


/**************************************************************************/
/*!
    @brief Allows us to statically set all LEDs perodically, but with each LED having it's own color. 
    @return animation was interrupted. 
*/
/**************************************************************************/
bool matrix_static_individual(void){
    
    chMtxLock(&led_matrix_mutex);
    for(uint8_t i = 0; i < NUM_MATRIX_LEDS; i++)
        _set_ws2812b_macro((led_macro_t)i, static_individual_col[i].r, static_individual_col[i].g, static_individual_col[i].b);
    chMtxUnlock(&led_matrix_mutex);

    _update_ws2812b_matrix();
    
    event_mask = chEvtWaitAnyTimeout(EVENT_MASK(CHANGE_MATRIX_BITMASK), 10 * current_matrix_config.interval_speed);
    if(event_mask & EVENT_MASK(CHANGE_MATRIX_BITMASK))
        return 1;
    
    if(interrupt_animation)
        return 1; 

    return 0;  
}

/**************************************************************************/
/*!
    @brief Thread function and stack space for our led matrix stuff. 
*/
/**************************************************************************/
static THD_WORKING_AREA(led_matrix_thread_wa, 4096);
static THD_FUNCTION(led_matrix_thread, arg){
    (void)arg; 
    // So we can access the thread handlers. 
    led_runtime_handler = chThdGetSelfX();

    _start_ws2812b_matrix();
    _start_ws2812b_underglow();
    
    bool animation_changed = 0; 
    for(;;){
        chMtxLock(&led_matrix_mutex);
        LEDMatrixAnimation ani_enum = current_matrix_config.matrix_animation_enum;
        chMtxUnlock(&led_matrix_mutex);
        
        switch(ani_enum){
        case(MATRIX_CYCLE_INDIVIDUAL):
            animation_changed = matrix_cycle_individual();
        break;
        case(MATRIX_CYCLE_ALL):
        
        break;
        case(MATRIX_REACTIVE):
            animation_changed = matrix_keytrigger_reactive();    
        break;
        case(MATRIX_RIPPLE):
        
        break;
        case(MATRIX_STATIC_INDIVIDUAL):
            animation_changed = matrix_static_individual();
        break;
        case(MATRIX_STATIC_ALL):
            animation_changed = matrix_static_all();
        break;
        default:
        break;
        }

        if(animation_changed){
            // TODO but dynamically fades to the next animation. 
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

/**************************************************************************/
/*!
    @brief Remote function that let's us change what's the configutation of our LED matrix
    @param LEDMatrixConfiguration Struct of the next matrix configuration information
*/
/**************************************************************************/
extern void change_led_config(LEDMatrixConfiguration next_matrix_config){
    chMtxLock(&led_matrix_mutex);
    current_matrix_config = next_matrix_config;     
    chMtxUnlock(&led_matrix_mutex);

    // Signal to the thread to change the animations. 
    chEvtSignal(led_runtime_handler, EVENT_MASK(0));
    interrupt_animation = true; 
}

/**************************************************************************/
/*!
    @brief Remote function that let's us change what's the configutation of our LED matrix
    @param LEDMatrixAnimation Struct of the next matrix configuration information
*/
/**************************************************************************/
extern void change_led_animation(LEDMatrixAnimation animation){
    chMtxLock(&led_matrix_mutex);
    current_matrix_config.matrix_animation_enum = animation;
    chMtxUnlock(&led_matrix_mutex);

    // Signal to the thread to change the animations. 
    chEvtSignal(led_runtime_handler, EVENT_MASK(0));
    interrupt_animation = true; 
}

/**************************************************************************/
/*!
    @brief Sends a map of the triggered keys to the LED matrix thread. 
*/
/**************************************************************************/
extern void trigger_keymap(volatile uint8_t key_pressed_map[]){
    // Run through the key press map
    for(uint8_t i = 0; i < 16; i++) 
        latest_key_press_map[i] = key_pressed_map[i];
    
    keypress_trigger = true; 
}