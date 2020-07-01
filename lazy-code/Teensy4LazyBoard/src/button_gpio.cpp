#include "button_gpio.hpp"

struct {
    uint8_t dip_one_val = 0; 
    uint8_t dip_two_val = 0; 
    uint8_t dip_three_val = 0; 
}dip_val; 

void dip_read_task(void *args){
    (void)args; 
    dip_val.dip_one_val = digitalReadFast(DIP1);
    dip_val.dip_two_val = digitalReadFast(DIP2);
    dip_val.dip_three_val = digitalReadFast(DIP3);
}

extern void setup_dip_button(void){    
    pinMode(DIP1, INPUT);
    pinMode(DIP2, INPUT);
    pinMode(DIP3, INPUT);

    // Every second we read the dip switches
    void *args = NULL;
    add_lwip_task(&dip_read_task, args, chTimeS2I(1));
}