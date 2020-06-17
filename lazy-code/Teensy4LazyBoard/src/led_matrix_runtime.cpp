#include "led_matrix_runtime.hpp"

MUTEX_DECL(led_matrix_mutex);
thread_t *led_runtime_handler;

#define CHANGE_MATRIX_BITMASK 0 
#define TRIGGER_KEYPRESS_MATRIX_BITMASK 1
eventmask_t event_mask;  

LEDMatrixConfiguration current_matrix_config; 

RgbColor static_all_col = {100, 100, 100};
RgbColor static_individual_col[NUM_MATRIX_LEDS];

// Map of keys that are pressed for key triggered animations
uint8_t latest_key_press_map[NUM_MATRIX_LEDS];

bool matrix_cycle_individual(void);
bool matrix_static_all(void);
bool matrix_static_individual(void);
bool matrix_keytrigger_reactive(void);

extern void start_led_strip_thread(void);
extern void change_led_config(LEDMatrixConfiguration next_matrix_config);
extern void trigger_keymap(volatile uint8_t key_pressed_map[]);

/**************************************************************************/
/*!
    @brief wipes through the rgb array through the numpad. 
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
 
    }

    return 0; 
}


/**************************************************************************/
/*!
    @brief Reactive LED pattern. 
*/
/**************************************************************************/
bool matrix_keytrigger_reactive(void){
    
    current_matrix_config.interval_speed = 20;
    HsvColor current_hsv[NUM_MATRIX_LEDS];
    
    while(1){
        event_mask = chEvtWaitAnyTimeout(EVENT_MASK(CHANGE_MATRIX_BITMASK), 10 * current_matrix_config.interval_speed);
        // If we got a trigger event
        if(event_mask & EVENT_MASK(TRIGGER_KEYPRESS_MATRIX_BITMASK)){
            // Run through all of the matrix leds. 
            for(uint8_t x = 0; x < NUM_MATRIX_LEDS; x++){
                // IF said key has been triggered
                if(latest_key_press_map[x] == 0){
                    // We update the current hsv with latest keypress values. 
                    current_hsv[x].h = 100;
                    current_hsv[x].s = 100;
                    current_hsv[x].v = 100;
                }
            }
        }

        // Slowly decrement each LED. 
        for(uint8_t x = 0; x < NUM_MATRIX_LEDS; x++){
            if(current_hsv[x].v >= 10)
                current_hsv[x].v = current_hsv[x].v - 10; 
            _set_ws2812b_macro_hsv((led_macro_t)x, current_hsv[x].h, current_hsv[x].s, current_hsv[x].v);
        }
        // Update after each run. 
        _update_ws2812b_matrix();
    }
}


/**************************************************************************/
/*!
    @brief Allows us to statically set all LEDs perodically. 
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

    return 0;  
}


/**************************************************************************/
/*!
    @brief Allows us to statically set all LEDs perodically, but with each LED having it's own color. 
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
}

/**************************************************************************/
/*!
    @brief Sends a map of the triggered keys to the LED matrix thread. 
*/
/**************************************************************************/
extern void trigger_keymap(volatile uint8_t key_pressed_map[]){
    // Run through the key press map
    for(uint8_t i = 0; i < NUM_MATRIX_LEDS; i++) 
        latest_key_press_map[i] = key_pressed_map[i];
    
    // Trigger keypress matrix. 
    chEvtSignal(led_runtime_handler, EVENT_MASK(TRIGGER_KEYPRESS_MATRIX_BITMASK));
}
