#include "button_gpio.hpp"


void dip_read_task(void *args){
    (void)args; 
}

extern void setup_dip_button(void){
    void *args = NULL;
    // Every second we read the dip switches
    add_lwip_task(&dip_read_task, args, chTimeS2I(1));
}