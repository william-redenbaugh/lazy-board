#ifndef _KEYBOARD_GPIO_HPP
#define _KEYBOARD_GPIO_HPP

// Arduino and ChibiOS Libraries 
#include "Arduino.h"
#include "ChRt.h"

// Keyboard matrix pins.
#define KB_MATRIX_PIN_0 2
#define KB_MATRIX_PIN_1 3
#define KB_MATRIX_PIN_2 4
#define KB_MATRIX_PIN_3 5
#define KB_MATRIX_PIN_4 6
#define KB_MATRIX_PIN_5 7
#define KB_MATRIX_PIN_6 8
#define KB_MATRIX_PIN_7 9

/**************************************************************************/
/*!
    @brief Size of our matrix array. 
*/
/**************************************************************************/
enum matrix_size_t {NUM_ROWS = 4, 
                    NUM_COLS = 4
};                      

/**************************************************************************/
/*!
    @brief The position of all of our keyboard macros
*/
/**************************************************************************/
enum kb_macro_pos_t{
    KB_MACRO_0_POS = 0,   
    KB_MACRO_1_POS = 1,   
    KB_MACRO_2_POS = 2,  
    KB_MACRO_3_POS = 3,   
    KB_MACRO_4_POS = 4,   
    KB_MACRO_5_POS = 5,   
    KB_MACRO_6_POS = 6,   
    KB_MACRO_7_POS = 7,   
    KB_MACRO_8_POS = 8,   
    KB_MACRO_9_POS = 9,   
    KB_MACRO_10_POS = 10,   
    KB_MACRO_11_POS = 11,   
    KB_MACRO_12_POS = 12,   
    KB_MACRO_13_POS = 13,   
    KB_MACRO_14_POS = 14,   
    KB_MACRO_15_POS = 15
};

/**************************************************************************/
/*!
    @brief redefined array that has our row and colum space. 
*/
/**************************************************************************/
typedef volatile uint8_t KeyState[NUM_ROWS][NUM_COLS]; 

extern void start_keyboard_gpio(void);
extern void read_keybord_gpio(void);
extern KeyState* get_keyboard_values();

#endif