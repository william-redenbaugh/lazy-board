#include "keyboard_gpio.hpp"

// Pinmap of all the gpio pins for reading the keyboard matrix, rows and columns
uint8_t matrix_row_gpio[] = {KB_MATRIX_PIN_0, 
                             KB_MATRIX_PIN_1, 
                             KB_MATRIX_PIN_2, 
                             KB_MATRIX_PIN_3};

uint8_t matrix_col_gpio[] = {KB_MATRIX_PIN_4, 
                             KB_MATRIX_PIN_5, 
                             KB_MATRIX_PIN_6, 
                             KB_MATRIX_PIN_7};  

KeyState key_state;

extern void start_keyboard_gpio(void);
extern void read_keyboard_gpio(void);
extern KeyState* get_keyboard_values(void);

/**************************************************************************/
/*!
    @brief allows us to get a pointer reference to the latest keyboard values
*/
/**************************************************************************/
extern KeyState* get_keyboard_values(void){
    return &key_state;
}

/**************************************************************************/
/*!
    @brief allows us to read in the latest keyboard gpio values
*/
/**************************************************************************/
extern void read_keyboard_gpio(void){
    for(uint8_t row = 0; row < NUM_ROWS; row++){
        pinMode(matrix_row_gpio[row], OUTPUT);
        digitalWriteFast(matrix_row_gpio[row], LOW);
        for(uint8_t col = 0; col < NUM_COLS; col++){
            pinMode(matrix_col_gpio[col], INPUT_PULLUP);
            key_state[row][col] = (uint8_t)digitalReadFast(matrix_col_gpio[col]);
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
    for(uint8_t row = 0; row < NUM_ROWS; row++){
        for(uint8_t col = 0; col < NUM_COLS; col++)
            key_state[row][col] = 0; 
    } 
}