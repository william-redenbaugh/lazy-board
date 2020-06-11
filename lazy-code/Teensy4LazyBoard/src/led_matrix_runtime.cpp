#include "led_matrix_runtime.hpp"
#include "WS2812Serial.h"

// OCTOWS2811 PIN SETUP BEGIN // 

const int numled = NUM_MATRIX_LEDS;
const int pin = LED_STRIP_GPIO;

// Usable pins:
//   Teensy LC:   1, 4, 5, 24
//   Teensy 3.2:  1, 5, 8, 10, 31   (overclock to 120 MHz for pin 8)
//   Teensy 3.5:  1, 5, 8, 10, 26, 32, 33, 48
//   Teensy 3.6:  1, 5, 8, 10, 26, 32, 33
//   Teensy 4.0:  1, 8, 14, 17, 20, 24, 29, 39

byte drawingMemory[numled*3];         //  3 bytes per LED for RGB
DMAMEM byte displayMemory[numled*12]; // 12 bytes per LED for RGB

WS2812Serial leds(numled, displayMemory, drawingMemory, pin, WS2812_GRB);

// OCTOWS2811 PIN SETUP END // 

extern void start_led_strip_thread(void);

#define RED    0x160000
#define GREEN  0x001600
#define BLUE   0x000016
#define YELLOW 0x101400
#define PINK   0x120009
#define ORANGE 0x100400
#define WHITE  0x101010

/**************************************************************************/
/*!
    @brief Thread function and stack space for our led matrix stuff. 
*/
/**************************************************************************/
static THD_WORKING_AREA(led_matrix_thread_wa, 4096);
static THD_FUNCTION(led_matrix_thread, arg){
    (void)arg; 
    
    leds.begin();
    leds.show();
    
    // Looping runtime. 
    for(;;){

        for(uint8_t i = 0; i < NUM_MATRIX_LEDS; i++)
            leds.setPixel(i, BLUE);
        leds.show(); 
        
        chThdSleepSeconds(1);
        
        for(uint8_t i = 0; i < NUM_MATRIX_LEDS; i++)
            leds.setPixel(i, GREEN);
        leds.show();

        chThdSleepSeconds(1);

        for(uint8_t i = 0; i < NUM_MATRIX_LEDS; i++)
            leds.setPixel(i, RED);
        leds.show(); 

        chThdSleepSeconds(1);

        for(uint8_t i = 0; i < NUM_MATRIX_LEDS; i++)
            leds.setPixel(i, ORANGE);
        leds.show(); 

        chThdSleepSeconds(1);

        for(uint8_t i = 0; i < NUM_MATRIX_LEDS; i++)
            leds.setPixel(i, WHITE);
        leds.show(); 

        chThdSleepSeconds(1);

        for(uint8_t i = 0; i < NUM_MATRIX_LEDS; i++)
            leds.setPixel(i, PINK);
        leds.show(); 

        chThdSleepSeconds(1);

        for(uint8_t i = 0; i < NUM_MATRIX_LEDS; i++)
            leds.setPixel(i, YELLOW);
        leds.show(); 

        chThdSleepSeconds(1);
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