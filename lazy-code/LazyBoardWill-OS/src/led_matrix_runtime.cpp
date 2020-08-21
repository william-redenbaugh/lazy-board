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

extern void start_led_strip_thread(void); 
static void led_strip_thread(void *parameters); 
bool matrix_cycle_individual(void); 

/*
*   @brief Startup our led strip thread so we can send and trigger animations for our LED stuff. 
*/
extern void start_led_strip_thread(void){
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
        for(uint8_t y = 0; y < 4; y++){
            for(uint8_t x = 0; x < 4; x++){
                uint8_t hue_val = (uint8_t)(((y + 2) * (2 + x)* 12) + (h * 5)) % 255;
                _set_ws2812b_led_hsv(x, y, hue_val, 255, 160);
            }
        }
        os_thread_delay_ms(30);
        _update_ws2812b_matrix(); 
    }
    return 0; 
}
