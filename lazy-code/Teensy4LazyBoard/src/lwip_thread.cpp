#include "lwip_thread.hpp"

/**************************************************************************/
/*!
    @brief Thread pointer struct that holds everything we need to run a mini-thread
*/
/**************************************************************************/
struct LwipThread{
    // Pass in function as arguement
    void (*func)(void *ptr);
    // Pointer to function arguements
    void *ptr; 
    systime_t next_exec_time; 
    systime_t interval_time;
};

// Number of low priority threads available
uint16_t num_lwip_threads = 0; 
systime_t lowest_interval = 0; 

// List of low priority functions and parameters, with exectution time. 
LwipThread thread_list[NUM_MAX_LPWTHREADS];

extern LPThreadInitStatus_t add_lwip_task(void (*func)(void *ptr),  void *args, systime_t interval);
extern void setup_lwip_thread(void);

/**************************************************************************/
/*!
    @brief Thread function and stack space for our lwip thread, where all low priority work will sit. 
*/
/**************************************************************************/
static THD_WORKING_AREA(lwip_thread_wa, 2048);
static THD_FUNCTION(lwip_thread, arg){
    for(;;){
        if(num_lwip_threads >= 1){
            // Minimum ticks until we need to circle back around and get to all the lwip threads
            systime_t min_tick = thread_list[0].next_exec_time; 

            // run through entire list of functions and execute
            for(uint16_t num = 0; num < num_lwip_threads; num++){
                // If it's about time to run the function. 
                if(thread_list[num].next_exec_time <= chVTGetSystemTimeX()){
                    // We run the function
                    thread_list[num].func(thread_list[num].ptr);

                    // Set the next time we want the function to run
                    thread_list[num].next_exec_time = thread_list[num].interval_time + chVTGetSystemTimeX();
                    // If there is a thread that must run in less time than currently specified for low
                
                }
                // priority thread to work in, decreate time for next thread sleep. 
                if(min_tick > thread_list[num].next_exec_time)
                    min_tick = thread_list[num].next_exec_time; 
            }
            
            // Just in case timer restarted or other issue that hangs a lwip thread, then we don't wait for a sleep command just in case things take longer than we want them to. 
            // We opt out of sleeping. 
            if(min_tick > chVTGetSystemTimeX())
                // Sleep for lowest remaining tick cycle. 
                chThdSleepUntil(min_tick);
        }
        // If there are no threads, then we just chill. 
        else{
            chThdSleepSeconds(1);
        }
    }
}

/**************************************************************************/
/*!
    @brief Starts up our low priority work thread. 
*/
/**************************************************************************/
extern void setup_lwip_thread(void){
    void *args = NULL;
    Serial.begin(115200);

    chThdCreateStatic(lwip_thread_wa, 
                      sizeof(lwip_thread_wa), 
                      NORMALPRIO - 2, 
                      lwip_thread, 
                      NULL);
                      
}

/**************************************************************************/
/*!
    @brief Allows us to add "sub-threads" to our low priority work thread
    @param void (*func)(void *ptr) pointer to the void ptr function
    @param void *args ptr arguements to pass into function
    @param system_t (time expected between each task interval)
*/
/**************************************************************************/
extern LPThreadInitStatus_t add_lwip_task(void (*func)(void *ptr),  void *args, systime_t interval){
    if(num_lwip_threads >= NUM_MAX_LPWTHREADS){
        return LP_THREAD_INIT_FAILIURE_MAX_THREAD;
    }
    else{        
        
        // Pass in function arguments and interval speed
        thread_list[num_lwip_threads].func = func; 
        thread_list[num_lwip_threads].ptr = args; 
        thread_list[num_lwip_threads].interval_time = interval; 

        // Next time we want to run this function
        thread_list[num_lwip_threads].next_exec_time = interval + chVTGetSystemTimeX();
        
        // Increment number of threads currently
        num_lwip_threads++;
        return LP_THREAD_INIT_SUCCESS; 
    }
}