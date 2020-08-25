#include "OLED_display_runtime.hpp"

/*!
*   @brief The size of our display thread stack. 
*/
#define DISPLAY_THREAD_STACK_SIZE 2048

/*!
*   @brief Array stack for our display thread. 
*/
static uint32_t display_thread_stack[DISPLAY_THREAD_STACK_SIZE]; 

/*!
*   @brief Display "Object" function and data module. 
*/  
MatrixOLED display; 

// SSD1331 color definitions
const uint16_t  OLED_Color_Black        = 0x0000;
const uint16_t  OLED_Color_Blue         = 0x001F;
const uint16_t  OLED_Color_Red          = 0xF800;
const uint16_t  OLED_Color_Green        = 0x07E0;
const uint16_t  OLED_Color_Cyan         = 0x07FF;
const uint16_t  OLED_Color_Magenta      = 0xF81F;
const uint16_t  OLED_Color_Yellow       = 0xFFE0;
const uint16_t  OLED_Color_White        = 0xFFFF;

void start_display_thread(void); 
static void display_thread(void *parameters); 

/*!
*   @brief Initializes the Display runtime thread. 
*/
void start_display_thread(void){
    os_add_thread(&display_thread, NULL, sizeof(display_thread_stack), display_thread_stack); 
}

/*!
*   @brief The display thread runtime function. 
*/
static void display_thread(void *parameters){
    display.begin(); 
    display.set_rotation(2); 
    display.draw_queue(); 
    for(;;){
        
        os_thread_delay_ms(10); 
    }
}