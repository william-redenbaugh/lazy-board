#include "spi_display_runtime.hpp"

Adafruit_ST7735 key_ips = Adafruit_ST7735(LCD_SPI_PIN_CS, LCD_SPI_PIN_DC, LCD_SPI_PIN_RST);

extern void start_spi_display_thread(void);

/**************************************************************************/
/*!
    @brief Thread function and stack space for our spi display. 
*/
/**************************************************************************/
static THD_WORKING_AREA(spi_display_thread_wa, 4096);
static THD_FUNCTION(spi_display_thread, arg){
    (void)arg; 

    key_ips.initR(INITR_MINI160x80);  // Init ST7735S mini display

    // Looping runtime. 
    for(;;){
        chThdSleepSeconds(1);
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