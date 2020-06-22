#include "spi_display_runtime.hpp"
#include "MatrixOLED.hpp"

// SSD1331 color definitions
const uint16_t  OLED_Color_Black        = 0x0000;
const uint16_t  OLED_Color_Blue         = 0x001F;
const uint16_t  OLED_Color_Red          = 0xF800;
const uint16_t  OLED_Color_Green        = 0x07E0;
const uint16_t  OLED_Color_Cyan         = 0x07FF;
const uint16_t  OLED_Color_Magenta      = 0xF81F;
const uint16_t  OLED_Color_Yellow       = 0xFFE0;
const uint16_t  OLED_Color_White        = 0xFFFF;

MatrixOLED oled; 

struct {
    // Key stuff .
    volatile bool new_char_available = false; 
    volatile bool char_release = false; 
    volatile char new_char = 'c'; 
}key_animations;

struct {
    volatile bool mode = false; 
}sleep;

// Counter for dealing with periodic stuff. 
uint32_t counter = 0;    


extern void start_spi_display_thread(void);
void reset_sleep_mode(void);
void enable_sleep_mode(void);

/**************************************************************************/
/*!
    @brief Thread function and stack space for our spi display. 
*/
/**************************************************************************/
static THD_WORKING_AREA(spi_display_thread_wa, 4096);
static THD_FUNCTION(spi_display_thread, arg){
    (void)arg; 

    //Setting up our oled display
    oled.begin();
    oled.set_rotation(2);
    oled.queue_rect(0, 0, 127, 127, OLED_Color_Magenta);    
    oled.draw_queue();
    
    for(;;){
        if(key_animations.new_char_available){
            if(sleep.mode == true)
                reset_sleep_mode();
            oled.draw_queue();
        }

        if(key_animations.char_release){
            if(sleep.mode == true)
                reset_sleep_mode();
            oled.draw_queue();
        }

        // Where we can do periodic stuff!
        counter++; 
        switch(counter){
            case(900):
            enable_sleep_mode();
            break;

            // Reset counter after 20 seconds. 
            case(4000):
                counter = 0; 
            break;

            default: 
                break;
        }
        chThdSleepMilliseconds(5);
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

/**************************************************************************/
/*!
    @brief Allows us to trigger a new char onto the OLED display. 
    @param char c (character that we are pushing to the screen. )
*/
/**************************************************************************/
extern void trigger_new_char(char c){
    // If sleep mode is enabled, gotta turn it off!    
    counter = 0; 

    key_animations.char_release= false; 
    oled.queue_rect_fill(1, 1, 126, 126, OLED_Color_Magenta);
    oled.queue_rect(0, 0, 127, 127, OLED_Color_Magenta);    
    oled.draw_char(24, 6, key_animations.new_char, OLED_Color_Black, OLED_Color_Magenta, 16, 16);
    
    key_animations.new_char = c; 
    key_animations.new_char_available = true;
}

/**************************************************************************/
/*!
    @brief Allows us to trigger a new char that was released onto the OLED display. 
    @param char c (character that we are pushing to the screen. )
*/
/**************************************************************************/
extern void trigger_char_release(char c){
    // If sleep mode is enabled, gotta turn it off!
    if(sleep.mode == true)
        reset_sleep_mode();

    key_animations.new_char_available = false; 
    oled.queue_rect_fill(1, 1, 126, 126, OLED_Color_Black);
    oled.queue_rect(0, 0, 127, 127, OLED_Color_Magenta);    
    oled.draw_char(24, 6, key_animations.new_char, OLED_Color_Magenta, OLED_Color_Black, 16, 16);
    
    key_animations.new_char = c; 
    key_animations.char_release = true;
}

/**************************************************************************/
/*!
    @brief Allows us to wake the device. 
*/
/**************************************************************************/
void reset_sleep_mode(void){
    sleep.mode = false; 
    oled.enable_display(true);
}

/**************************************************************************/
/*!
    @brief allows us to sleep the device
*/
/**************************************************************************/
void enable_sleep_mode(void){
    sleep.mode = true; 
    oled.fill_screen(OLED_Color_Black);
    oled.enable_display(false);
}