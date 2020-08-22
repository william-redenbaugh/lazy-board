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

/*
*   @brief Thread handler ID for our thread
*/
static os_thread_id_t led_runtime_thread_handler; 

/*
*   @brief Mutex lock instance for the LED runtime so that all shared resources are threadsafe. 
*/
static MutexLock led_runtime_mutex; 

/*
*   @brief Signal that helps trigger an interrupt of an animation. 
*/
static OSSignal led_runtime_trigger; 

static void led_strip_message_callback(MessageReq *msg); 
extern void start_led_strip_thread(void); 
static void led_strip_thread(void *parameters); 
bool matrix_cycle_individual(void); 
__attribute__((always_inline))void matrix_cycle_individual_fillframe(uint8_t h); 

/*
*   @brief Whenever we get a message regarding our LED strip, it's handled here. 
*/
static void led_strip_message_callback(MessageReq *msg){

}

/*
*   @brief Startup our led strip thread so we can send and trigger animations for our LED stuff. 
*/
extern void start_led_strip_thread(void){
    // Callback handler for dealing with changing the RGB matrix animation
    add_message_callback(MessageData_MessageType_CHANGE_RGB_MATRIX_ANIMATION, led_strip_message_callback); 
    
    // Actual LED strip runtime thread. 
    led_runtime_thread_handler = os_add_thread(&led_strip_thread, NULL, sizeof(led_matrix_runtime_stack), led_matrix_runtime_stack); 
}

/*
*   @brief Thread space for our led strip thread. 
*/
static void led_strip_thread(void *parameters){
    // Setup our LED strips 
    _start_ws2812b_matrix(); 
    _start_ws2812b_underglow(); 

    for(;;){
        matrix_cycle_individual(); 
        os_thread_delay_ms(10); 
    }
}

/**************************************************************************/
/*!
    @brief wipes through the rgb array through the numpad. 
    @return animation was interrupted. 
*/
/**************************************************************************/
bool matrix_cycle_individual(void){
    // Looping runtime. 
    for(uint8_t h = 0; h < 255; h++){
        // Fills in each individual keyframe 
        matrix_cycle_individual_fillframe(h);  

        // Checking to see if there was a signal trigger.     
        if(led_runtime_trigger.wait(THREAD_SIGNAL_0, 10))
            return true; 
    
        _update_ws2812b_matrix(); 
    }

    // There was no need to interrupt 
    return false; 
}

/**************************************************************************/
/*!
    @brief Fills a single frame for the matrix_cycle_individual function. Set's each RGB LED value.  
*/
/**************************************************************************/
__attribute__((always_inline)) void matrix_cycle_individual_fillframe(uint8_t h){
    for(uint8_t y = 0; y < 4; y++){
        for(uint8_t x = 0; x < 4; x++){
            uint8_t hue_val = (uint8_t)(((y + 2) * (2 + x)* 12) + (h * 5)) % 255;
            _set_ws2812b_led_hsv(x, y, hue_val, 255, 160);
        }
    }
}