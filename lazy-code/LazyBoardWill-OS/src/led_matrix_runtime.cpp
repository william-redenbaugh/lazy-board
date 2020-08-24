#include "led_matrix_runtime.hpp"

/*
*   @brief Stack space for our led matrix runtime thread 
*/
#define LED_MATRIX_RUNTIME_STACK_SIZE 2048

/*
*   @brief Instance of our led matrix runtime stack 
*/
static uint32_t led_matrix_runtime_stack[LED_MATRIX_RUNTIME_STACK_SIZE]; 
#undef LED_MATRIX_RUNTIME_STACK_SIZE 

/*!
*   @brief Thread handler ID for our thread
*/
static os_thread_id_t led_runtime_thread_handler; 

/*!
*   @brief Mutex lock instance for the LED runtime so that all shared resources are threadsafe. 
*/
static MutexLock led_runtime_mutex; 

/*!
*   @brief Signal that helps trigger an interrupt of an animation. 
*/
static OSSignal led_runtime_trigger; 

/*!
* @brief Map of keys that are pressed for key triggered animations
*/
static uint8_t latest_key_press_map[16];

/*!
* @brief Whenever a key or keys have been pressed, a boolean flags. 
*/
static volatile bool keypress_trigger = false; 

/*!
*   @brief Current animation that the system is taking. 
*   @defaults to a simple cycle program.
*/
static LEDMatrixAnimation current_animation = MATRIX_REACTIVE; 

static void led_strip_message_callback(MessageReq *msg); 
extern void start_led_strip_thread(void); 
__attribute__((always_inline))void select_animation(void); 
static void led_strip_thread(void *parameters); 
static bool matrix_cycle_individual(void); 
__attribute__((always_inline))static void matrix_cycle_individual_fillframe(uint8_t h); 
__attribute__((always_inline)) static uint8_t convert_keymap_led(uint8_t y); 
 bool matrix_keytrigger_reactive(void); 
__attribute__((always_inline)) void clear_hsv(void); 
__attribute__((always_inline)) static void check_reactive_keytrigger(void); 
__attribute__((always_inline)) static void decrement_reactive_keytrigger(void); 

/*!
*   @brief Whenever we get a message regarding our LED strip, it's handled here. 
*/
static void led_strip_message_callback(MessageReq *msg){

}

/*!
*   @brief Startup our led strip thread so we can send and trigger animations for our LED stuff. 
*/
extern void start_led_strip_thread(void){
    // Callback handler for dealing with changing the RGB matrix animation
    add_message_callback(MessageData_MessageType_CHANGE_RGB_MATRIX_ANIMATION, led_strip_message_callback); 
    
    // Actual LED strip runtime thread. 
    led_runtime_thread_handler = os_add_thread(&led_strip_thread, NULL, sizeof(led_matrix_runtime_stack), led_matrix_runtime_stack); 
}

/*!
*  @brief Thread space for our led strip thread. 
*/
static void led_strip_thread(void *parameters){
    // Setup our LED strips 
    _start_ws2812b_matrix(); 
    _start_ws2812b_underglow(); 

    for(;;){
        select_animation();
        os_thread_delay_ms(10); 
    }
}


/*!
*   @brief Which animation are we going to select next?
*   @note Keeps switch/case statements in it's own inline function
*/
__attribute__((always_inline))void select_animation(void){
    switch(current_animation){
    case(MATRIX_CYCLE_INDIVIDUAL):
        matrix_cycle_individual(); 
    break; 

    case(MATRIX_REACTIVE):
        matrix_keytrigger_reactive();
    break; 
    default: 
    break; 
    }
}

/*!
    @brief wipes through the rgb array through the numpad. 
    @return animation was interrupted. 
*/
static bool matrix_cycle_individual(void){

    // Looping runtime. 
    for(uint8_t h = 0; h < 255; h++){
        // Fills in each individual keyframe 
        matrix_cycle_individual_fillframe(h);  

        // Checking to see if there was a signal trigger.     
        //os_thread_delay_ms(30);
        if(led_runtime_trigger.wait(THREAD_SIGNAL_0, 30))
            return true; 
        
        _update_ws2812b_matrix(); 
    }    

    // There was no need to interrupt 
    return false; 
}

/*!
    @brief Fills a single frame for the matrix_cycle_individual function. Set's each RGB LED value.  
*/
__attribute__((always_inline)) static void matrix_cycle_individual_fillframe(uint8_t h){
    for(uint8_t y = 0; y < 4; y++){
        for(uint8_t x = 0; x < 4; x++){
            uint8_t hue_val = (uint8_t)(((y + 2) * (2 + x)* 12) + (h * 5)) % 255;
            _set_ws2812b_led_hsv(x, y, hue_val, 255, 60);
        }
    }
}

/*!
    @brief Sends a map of the triggered keys to the LED matrix thread. 
*/
extern void trigger_keymap(volatile uint8_t key_pressed_map[]){
    // Run through the key press map
    for(uint8_t i = 0; i < 12; i++) 
        latest_key_press_map[i] = key_pressed_map[i];
    
    keypress_trigger = true; 
}

/*
*   @brief Converts incoming keymap location into a LED location. 
*   @notes Essentially a very basic hashmap for dealing with keys and LED matrix. 
*/
__attribute__((always_inline)) static uint8_t convert_keymap_led(uint8_t y){
    uint8_t x; 
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

    // Generally shouldn't get here, but we also don't have 255 LEDS on our matrix so that works out aswell
    default: 
        x = 255; 
    break;
    }
    return x; 
}

/*
*   @brief The current HSV values of of our matrix keytrigger reaction program. 
*/
static uint8_t current_hue[NUM_MATRIX_LEDS]; 
static uint8_t current_saturation[NUM_MATRIX_LEDS]; 
static uint8_t current_value[NUM_MATRIX_LEDS]; 

/*!
    @brief Reactive LED pattern. 
    @return animation was interrupted. 
*/
static bool matrix_keytrigger_reactive(void){
    clear_hsv(); 
    memset(latest_key_press_map, 0, sizeof(latest_key_press_map));

    while(1){
        // Waiting for a thread interrupt 
        if(led_runtime_trigger.wait(THREAD_SIGNAL_0, 10))
            return true; 

        check_reactive_keytrigger(); 
        decrement_reactive_keytrigger(); 

        // Update after each run. 
        _update_ws2812b_matrix();
    }
    return 0; 
}

/*!
*   @brief Allows us to instantly clear all of our hue, saturation and values for use later on oin the function
*/
__attribute__((always_inline)) static void clear_hsv(void){
    memset(current_hue, 0, sizeof(current_hue)); 
    memset(current_saturation, 0, sizeof(current_saturation)); 
    memset(current_value, 0, sizeof(current_value)); 
}

/*!
*   @brief Checking if there are any keypress updates, and if so, for us to process said key updates. 
*/
__attribute__((always_inline)) static void check_reactive_keytrigger(void){
    if(!keypress_trigger)
        return; 

    keypress_trigger = false; 
    // Run through all of the matrix leds. 
    for(uint8_t y = 0; y < 12; y++){
        // If said key has been triggered
        if(latest_key_press_map[y] == 0){
            uint8_t x = convert_keymap_led(y);         
            // We update the current hsv with latest keypress values. 
            current_hue[x] = random(255);
            current_saturation[x] = 255;
            current_value[x] = 80;
            _set_ws2812b_macro_hsv((led_macro_t)x, current_hue[x], current_saturation[x], current_value[x]);
        }
    }
}

/*!
*   @brief Goes through each reactive led, and decrements unless otherwise specified. 
*/
__attribute__((always_inline)) static void decrement_reactive_keytrigger(void){
    for(uint8_t x = 0; x < NUM_MATRIX_LEDS; x++){
        if(!(current_value[x] == 0)){
            current_value[x] = current_value[x] - 1; 
            _set_ws2812b_macro_hsv((led_macro_t)x, current_hue[x], current_saturation[x], current_value[x]);
        }
    }
}