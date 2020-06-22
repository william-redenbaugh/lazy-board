#include "spi_display_runtime.hpp"
#include "Adafruit_SSD1351.h"
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


extern void start_spi_display_thread(void);

/**************************************************************************/
/*!
    @brief Thread function and stack space for our spi display. 
*/
/**************************************************************************/
static THD_WORKING_AREA(spi_display_thread_wa, 4096);
static THD_FUNCTION(spi_display_thread, arg){
    (void)arg; 

    Serial.begin(115200);
    oled.begin();
    oled.fill_screen(OLED_Color_Green);    

    for(;;){
        chThdSleepSeconds(1);
        for(uint8_t x = 0; x < 128; x++){
            oled.queue_pixel(x, 0, OLED_Color_Yellow);
            if(x%3 == 0)
            oled.draw_queue();
        }

        for(uint8_t x = 0; x < 128; x++){
            oled.queue_pixel(x, 127, OLED_Color_Yellow);
            if(x%3 == 0)
            oled.draw_queue();
        }

        for(uint8_t x = 0; x < 128; x++){
            oled.queue_pixel(127, x, OLED_Color_Yellow);
            if(x%3 == 0)
            oled.draw_queue();
        }

        for(uint8_t x = 0; x < 128; x++){
            oled.queue_pixel(0, x, OLED_Color_Yellow);
            if(x%3 == 0)
            oled.draw_queue();
        }

        for(uint8_t x = 0; x < 128; x++){
            for(uint8_t y = 0; y < x; y++){     
                oled.queue_pixel(x, y, OLED_Color_Yellow);
            }       
            if(x%2 == 0)
            oled.draw_queue();
        }
        
        chThdSleepSeconds(1);   
        oled.fill_screen(OLED_Color_Black);
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