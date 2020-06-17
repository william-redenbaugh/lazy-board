#include "keyboard_runtime_thread.hpp"
#include "kb_macros_define.h"
#include "led_matrix_runtime.hpp"

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
    Keyboard.begin();

    systime_t kb_thread_begin_tick;
    systime_t kb_thread_end_tick;  

    Serial.begin(115200);
    KeyState key_state; 

    // Sets and clears all the previous key state information information
    KeyState prev_key_state;
    for(uint8_t i = 0; i < NUM_ROWS * NUM_COLS; i++)
        prev_key_state[i] = 1; 
    
    while(1){
        // Get current tick  aneiobfk
        kb_thread_begin_tick = chVTGetSystemTimeX();

        // Reads in the keyboard data from the matrix. 
        read_keyboard_gpio();
        get_keyboard_values(key_state);    
        // Run through 2D array, check which keys are pressed and which arent. 
        for(uint8_t x = 0; x < NUM_ROWS * NUM_COLS; x++){
            
            if((key_state[x] == 0) && !(key_state[x] == prev_key_state[x])){
                switch(x){
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
            if((key_state[x] == 1) && !(key_state[x] == prev_key_state[x])){
                switch(x){
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
            // Setting the previous key state to the next key_state
            prev_key_state[x] = key_state[x];
        }
        // END OF KEYSTROKE KEYBOARD OUTPUT // 

        // Send keystroke information to the LED strip thread, casts to volatile unsigned 8 bit integer. 
        trigger_keymap((volatile uint8_t*)key_state);

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

