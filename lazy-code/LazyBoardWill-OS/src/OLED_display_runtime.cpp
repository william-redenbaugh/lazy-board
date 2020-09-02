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

/*!
*   @brief Basic color primitives for the SSD1351 to give us. 
*/
const uint16_t  OLED_Color_Black        = 0x0000;
const uint16_t  OLED_Color_Blue         = 0x001F;
const uint16_t  OLED_Color_Red          = 0xF800;
const uint16_t  OLED_Color_Green        = 0x07E0;
const uint16_t  OLED_Color_Cyan         = 0x07FF;
const uint16_t  OLED_Color_Magenta      = 0xF81F;
const uint16_t  OLED_Color_Yellow       = 0xFFE0;
const uint16_t  OLED_Color_White        = 0xFFFF;

/*!
*   @brief Whether or not sleep mode is enabled. 
*/
static volatile bool sleep_mode = false; 

// Whenever we get new frame data, we update the device. 
static volatile bool new_frame = false; 

/*!
* @brief we use this is take care of the sleep counting.   
*/
static volatile uint32_t sleep_counter = 0;

// If the system tick is 15 milliseconds, this should happen roughly every 10 seconds.  
const uint32_t sleep_interval = 500; 

void start_display_thread(void); 
static void sleep_display(void); 
static void display_thread(void *parameters);
__attribute__((always_inline))void display_loop(void); 

static void oled_frame_callback(MessageReq *msg){
    // If there isn't any other frames that we need to deal with(first man wins style)
    if(new_frame == false){
        // Array pointer and size. 
        uint8_t *ptr = display.out_array_ptr(); 
        uint8_t data_len = display.out_array_size(); 

        // Read in data from the serial bus and into display buffer. 
        os_usb_serial_read(ptr, data_len); 
        new_frame = true; 
    }
    else{
        // Get rid of that data buffer. 
        usb_serial_flush_input(); 
    }
}
/*!
*   @brief Initializes the Display runtime thread. 
*/
void start_display_thread(void){
    os_add_thread(&display_thread, NULL, sizeof(display_thread_stack), display_thread_stack); 
    //add_message_callback(MessageData_MessageType_OLED_FRAME_UPDATE, &oled_frame_callback); 
}

/*!
*   @brief Turn's off the display, and enables the sleep mode boolean. 
*/  
static void sleep_display(void){
    display.enable_display(false); 
    sleep_mode = true; 
}

/*!
*   @brief Turn's on the display and enables sleep mode boolean, also resets sleep timer. 
*/
static void wake_display(void){
    display.enable_display(true); 
    sleep_mode = false; 
    sleep_counter = 0; 
}

/*!
*   @brief The display thread runtime function. 
*/
static void display_thread(void *parameters){
    display.begin(); 
    // Display is oriented upside down on the macropad/ 
    display.set_rotation(2); 
    // Sometimes we get digital noise during bootup. 
    display.draw_queue(); 
        
    // Used so we can ensure that data get's as close to 15 milliseconds as possible. 
    uint32_t interval_counter; 
    for(;;){
        interval_counter = millis(); 

        display_loop(); 

        uint32_t interval_difference = millis() - interval_counter; 
        if(interval_difference <= 20)
            os_thread_delay_ms(20 - interval_difference);     
    }
}

/*! 
*   @brief Thread that deals with pushing data up to the display.
*/
__attribute__((always_inline))void display_loop(void){
    sleep_counter++; 
    // Reset sleep counter. 
    switch(sleep_counter){
        case(2000): 
            sleep_counter = 0;
        break;
        // Every 5 seconds we attempt to reset the display. 
        case(sleep_interval): 
            sleep_display(); 
        break; 
        default:
        break; 
    }

    // Update the frame whenever new data is available. 
    if(new_frame){
        wake_display(); 
        display.draw_queue(); 
    }
}