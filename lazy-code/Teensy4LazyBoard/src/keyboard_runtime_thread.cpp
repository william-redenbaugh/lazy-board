#include "keyboard_runtime_thread.hpp"
#include "kb_macros_define.h"

extern void start_keyboard_runtime_thread(void);

/**************************************************************************/
/*!
    @brief Thread function and stack space for dealing the keyboard runtime stuff. 
*/
/**************************************************************************/
static THD_WORKING_AREA(keyboard_runtime_thread_wa, 4096);
static THD_FUNCTION(keyboard_runtime_thread, arg){
    (void)arg; 

    start_keyboard_gpio();
    // Creates pointer to the array that has the keyboard values. 
    KeyState *key_state_ptr = get_keyboard_values();
    Keyboard.begin();

    systime_t kb_thread_begin_tick;
    systime_t kb_thread_end_tick;  

    while(1){
        // Reads in the keyboard data from the matrix. 
        //read_keybord_gpio();

        // Get current tick
        kb_thread_begin_tick = chVTGetSystemTimeX();

        // Run through 2D array, check which keys are pressed and which arent. 

        // BEGIN OF KEYSTROKE KEYBOARD OUTPUT // 
        for(uint8_t row = 0; row < NUM_ROWS; row++){
            for(uint8_t col = 0; col < NUM_COLS; col++){
                uint16_t current_enum = row * NUM_COLS + col;
                if(*key_state_ptr[row][col]){
                    switch(current_enum){
                    case(KB_MACRO_0_POS):
                        Keyboard.press(KB_MACRO_0);
                    break;
                    case(KB_MACRO_1_POS):
                        Keyboard.press(KB_MACRO_1);
                    break;
                    case(KB_MACRO_2_POS):
                        Keyboard.press(KB_MACRO_2);
                    break;
                    case(KB_MACRO_3_POS):
                        Keyboard.press(KB_MACRO_3);
                    break;
                    case(KB_MACRO_4_POS):
                        Keyboard.press(KB_MACRO_4);
                    break;
                    case(KB_MACRO_5_POS):
                        Keyboard.press(KB_MACRO_5);
                    break;
                    case(KB_MACRO_6_POS):
                        Keyboard.press(KB_MACRO_6);
                    break;
                    case(KB_MACRO_7_POS):
                        Keyboard.press(KB_MACRO_7);
                    break;
                    case(KB_MACRO_8_POS):
                        Keyboard.press(KB_MACRO_8);
                    break;
                    case(KB_MACRO_9_POS):
                        Keyboard.press(KB_MACRO_9);
                    break;
                    case (KB_MACRO_10_POS):
                        Keyboard.press(KB_MACRO_10);
                    break;
                    case(KB_MACRO_11_POS):
                        Keyboard.press(KB_MACRO_11);
                    break;
                    
                    // ** NOTE: CURRENTLY UNUSED PINS BEGIN ** // 
                    case(KB_MACRO_12_POS):
                        Keyboard.press(KB_MACRO_12);
                    break;
                    case(KB_MACRO_13_POS):
                        Keyboard.press(KB_MACRO_13);
                    break;
                    case(KB_MACRO_14_POS):
                        Keyboard.press(KB_MACRO_14);
                    break;
                    case(KB_MACRO_15_POS):
                        Keyboard.press(KB_MACRO_15);
                    break;
                    // ** NOTE: CURRENTLY UNUSED PINS END ** // 
                    default:
                        // HOW TF DID WE GET HERE ?!?!!!??  :0 // 
                    break;
                    }
                }
                // If key isn't currently pressed, then we make sure it's been released. 
                else{
                    switch(current_enum){
                    case(KB_MACRO_0_POS):
                        Keyboard.release(KB_MACRO_0);
                    break;
                    case(KB_MACRO_1_POS):
                        Keyboard.release(KB_MACRO_1);
                    break;
                    case(KB_MACRO_2_POS):
                        Keyboard.release(KB_MACRO_2);
                    break;
                    case(KB_MACRO_3_POS):
                        Keyboard.release(KB_MACRO_3);
                    break;
                    case(KB_MACRO_4_POS):
                        Keyboard.release(KB_MACRO_4);
                    break;
                    case(KB_MACRO_5_POS):
                        Keyboard.release(KB_MACRO_5);
                    break;
                    case(KB_MACRO_6_POS):
                        Keyboard.release(KB_MACRO_6);
                    break;
                    case(KB_MACRO_7_POS):
                        Keyboard.release(KB_MACRO_7);
                    break;
                    case(KB_MACRO_8_POS):
                        Keyboard.release(KB_MACRO_8);
                    break;
                    case(KB_MACRO_9_POS):
                        Keyboard.release(KB_MACRO_9);
                    break;
                    case (KB_MACRO_10_POS):
                        Keyboard.release(KB_MACRO_10);
                    break;
                    case(KB_MACRO_11_POS):
                        Keyboard.release(KB_MACRO_11);
                    break;
                    
                    // ** NOTE: CURRENTLY UNUSED PINS BEGIN ** // 
                    case(KB_MACRO_12_POS):
                        Keyboard.release(KB_MACRO_12);
                    break;
                    case(KB_MACRO_13_POS):
                        Keyboard.release(KB_MACRO_13);
                    break;
                    case(KB_MACRO_14_POS):
                        Keyboard.release(KB_MACRO_14);
                    break;
                    case(KB_MACRO_15_POS):
                        Keyboard.release(KB_MACRO_15);
                    break;
                    // ** NOTE: CURRENTLY UNUSED PINS END ** // 
                    default:
                        // HOW TF DID WE GET HERE ?!?!!!??  :0 // 
                    break;
                    }
                }
            }
        }
        // END OF KEYSTROKE KEYBOARD OUTPUT // 

        // we sleep the remainder of the time for the keyboard. 
        kb_thread_end_tick = kb_thread_begin_tick + TIME_I2MS(33);
        if(kb_thread_end_tick > chVTGetSystemTimeX())
            chThdSleepUntil(kb_thread_end_tick);
    }   
}

/**************************************************************************/
/*!
    @brief Allows us to start up our keyboard runtime thread
*/
/**************************************************************************/
extern void start_keyboard_runtime_thread(void){
    chThdCreateStatic(keyboard_runtime_thread_wa, 
                      sizeof(keyboard_runtime_thread_wa), 
                      NORMALPRIO + 3, 
                      keyboard_runtime_thread, 
                      NULL);
}