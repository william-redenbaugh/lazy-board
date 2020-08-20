/*
Author: William Redenbaugh
Last Edit Date: 7/3/2020
*/

#include "keyboard_gpio.hpp"

// Pinmap of all the gpio pins for reading the keyboard matrix, rows and columns
uint8_t matrix_col_gpio[] = {KB_MATRIX_PIN_0, 
                             KB_MATRIX_PIN_1, 
                             KB_MATRIX_PIN_2};

uint8_t matrix_row_gpio[] = {KB_MATRIX_PIN_3, 
                             KB_MATRIX_PIN_4, 
                             KB_MATRIX_PIN_5, 
                             KB_MATRIX_PIN_6};  

KeyState key_state;

extern void start_keyboard_gpio(void);
extern void read_keyboard_gpio(void);
extern void get_keyboard_values(KeyState key_state_ptr);

/**************************************************************************/
/*!
    @brief allows us to get a pointer reference to the latest keyboard values
*/
/**************************************************************************/
extern void get_keyboard_values(KeyState key_state_ptr){
    for(uint8_t row = 0; row < NUM_ROWS * NUM_COLS; row++){
        key_state_ptr[row] = key_state[row];
    }
}

/**************************************************************************/
/*!
    @brief allows us to read in the latest keyboard gpio values
*/
/**************************************************************************/
extern void read_keyboard_gpio(void){
    uint8_t x = 0;
    for(uint8_t row = 0; row < NUM_ROWS; row++){
        pinMode(matrix_row_gpio[row], OUTPUT);
        digitalWriteFast(matrix_row_gpio[row], LOW);
        
        for(uint8_t col = 0; col < NUM_COLS; col++){
            pinMode(matrix_col_gpio[col], INPUT_PULLUP);
            chThdSleepMilliseconds(1);
            key_state[x] = (uint8_t)digitalReadFast(matrix_col_gpio[col]);
            x++;
            pinMode(matrix_col_gpio[col], INPUT);
        }
        pinMode(matrix_row_gpio[row], INPUT);
    }
}

/**************************************************************************/
/*!
    @brief Allows us to setup our keyboard gpio
*/
/**************************************************************************/
extern void start_keyboard_gpio(void){
    // Clearing key state values
    for(uint8_t row = 0; row < NUM_ROWS * NUM_COLS; row++){
        key_state[row] = 1; 
    } 
}