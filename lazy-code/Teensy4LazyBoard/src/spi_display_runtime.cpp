/*
Author: William Redenbaugh
Last Edit Date: 7/3/2020
*/

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
    volatile bool new_col = false; 

}key_animations;

struct {
    volatile bool mode = false; 
}sleep;

// 8 Ram memory spaces for us to store our images in ram. 
// Which should result in "fast enough memory"
DMAMEM uint16_t ram_image[8][128][128];

// 32 spaces for storing images in flash 
// Which should result in "flash memory"
PROGMEM uint16_t flash_image[50][128][128];

// Counter for dealing with periodic stuff. 
volatile uint32_t counter = 0;    

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
            key_animations.new_char_available = false; 
        }

        if(key_animations.char_release){
            if(sleep.mode == true)
                reset_sleep_mode();
            oled.draw_queue();
            key_animations.char_release = false; 
        }

        if(key_animations.new_col){
            if(sleep.mode == true)
                reset_sleep_mode(); 
            
            oled.draw_queue(); 
            key_animations.new_col = false; 
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
    @param uint16_t color that we want to trigger onto the display. 
*/
/**************************************************************************/
extern void trigger_color(uint16_t col){
    // If sleep mode is enabled, gotta turn it off!
    if(sleep.mode == true)
        reset_sleep_mode();

    oled.queue_rect_fill(0, 0, 127, 127, col);
    key_animations.char_release = true;    
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
inline void reset_sleep_mode(void){
    sleep.mode = false; 
    oled.enable_display(true);
}

/**************************************************************************/
/*!
    @brief allows us to sleep the device
*/
/**************************************************************************/
inline void enable_sleep_mode(void){
    sleep.mode = true; 
    //oled.fill_screen(OLED_Color_Black);
    oled.enable_display(false);
}

/**************************************************************************/
/*!
    @brief Allows us to update flash with latest image. 
    @param ImageFlash struct with image flash configuration data. 
*/
/**************************************************************************/
void process_image_flash(ImageFlash flash_config){
    for(uint8_t x = 0; x < flash_config.x_len; x++){
        for(uint8_t y = 0; y < flash_config.y_len; y++){
            if(flash_config.flash_space < 8){
                ram_image[flash_config.flash_space][x][y] = Serial.read();
            }
            else{
                flash_image[(flash_config.flash_space - 8)][x][y] = Serial.read();
            }
        }
    }

    // If the user wanted us to display the new image now. 
    if(flash_config.display_now && flash_config.flash_space < 8){
        for(uint8_t x = 0; x < flash_config.x_len; x++)
            for(uint8_t y = 0; y < flash_config.y_len; y++)
                oled.queue_pixel(x, y, ram_image[flash_config.flash_space][x][y]);       
        // So the image show up for a short period time
        reset_sleep_mode();
    }
}