#include "lwip_thread.hpp"
#include <vector>

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
    bool thread_en = true; 
    uint32_t handle = 0; 
};

// Prevent thread manipulation errors during  or using threads. 
MUTEX_DECL(lwip_mutex);

// Number of low priority threads available
volatile uint16_t num_lwip_threads = 0;
volatile uint32_t handle_increment = 0;  
volatile systime_t lowest_interval = 0; 

// List of low priority functions and parameters, with exectution time.
std::vector<LwipThread> thread_list;  
//LwipThread thread_list[NUM_MAX_LPWTHREADS];

extern LPThreadInitReturn add_lwip_task(void (*func)(void *ptr),  void *args, systime_t interval);
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

            chMtxLock(&lwip_mutex);
            // run through entire list of functions and execute
            for(uint16_t num = 0; num < num_lwip_threads; num++){
                // If it's about time to run the function. 
                if(thread_list[num].next_exec_time <= chVTGetSystemTimeX()){
                    // If the function is enabled. 
                    if(thread_list[num].thread_en){
                        // We run the function
                        thread_list[num].func(thread_list[num].ptr);

                        // Set the next time we want the function to run
                        thread_list[num].next_exec_time = thread_list[num].interval_time + chVTGetSystemTimeX();
                        // If there is a thread that must run in less time than currently specified for low
                }
                
                }
                // priority thread to work in, decreate time for next thread sleep. 
                if(min_tick > thread_list[num].next_exec_time)
                    min_tick = thread_list[num].next_exec_time; 
                
            }
            chMtxUnlock(&lwip_mutex);
            
            // Just in case timer restarted or other issue that hangs a lwip thread, then we don't wait for a sleep command just in case things take longer than we want them to. 
            // We opt out of sleeping. 
            if(min_tick > chVTGetSystemTimeX())
                // Sleep for shortest remaining tick cycle until the next command 
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
    @returns LPThreadInitReturn(return struct with a couple of things thread management inside. )
*/
/**************************************************************************/
extern LPThreadInitReturn add_lwip_task(void (*func)(void *ptr),  void *args, systime_t interval){
    LPThreadInitReturn lp_return; 
    if(num_lwip_threads >= NUM_MAX_LPWTHREADS){
        lp_return.init_status = LP_THREAD_INIT_FAILIURE_MAX_THREAD;
        return lp_return; 
    }
    else{        
        chMtxLock(&lwip_mutex);
        thread_list.push_back(LwipThread());

        // Pass in function arguments and interval speed
        thread_list[num_lwip_threads].func = func; 
        thread_list[num_lwip_threads].ptr = args; 
        thread_list[num_lwip_threads].interval_time = interval; 
        thread_list[num_lwip_threads].thread_en = true; 
        
        thread_list[num_lwip_threads].handle = handle_increment; 
        handle_increment++; 

        // Next time we want to run this function
        thread_list[num_lwip_threads].next_exec_time = interval + chVTGetSystemTimeX();
        
        // Save LP return data
        lp_return.init_status = LP_THREAD_INIT_SUCCESS; 
        lp_return.thread_handle = thread_list[num_lwip_threads].handle; 

        // Increment number of threads currently
        num_lwip_threads++;
        chMtxUnlock(&lwip_mutex);
        return lp_return; 
    }
}

/**************************************************************************/
/*!
    @brief disables a specifc low priority task
    @param uint32_t (handle of task) 
*/
/**************************************************************************/
extern LPThreadInitStatus_t disable_lwip_task(uint32_t thread_handle){
    if(thread_handle < num_lwip_threads){
        for(uint16_t n = 0; n < num_lwip_threads; n++){
            // find and search for the specific thread handle
            if(thread_list[thread_handle].handle == thread_handle){
                thread_list[thread_handle].thread_en = false; 
                return LP_THREAD_EN_SUCCESS; 
            }
        }
    }
    return LP_THREAD_EN_UNDEFINED;
}

/**************************************************************************/
/*!
    @brief enables a specifc low priority task
    @param uint32_t (handle of task) 
*/
/**************************************************************************/
extern LPThreadInitStatus_t enable_lwip_task(uint32_t thread_handle){
    if(thread_handle < num_lwip_threads){
        for(uint16_t n = 0; n < num_lwip_threads; n++){
            if(thread_list[n].handle == thread_handle){
                thread_list[n].thread_en = true; 
                return LP_THREAD_EN_SUCCESS; 
            }
        }
    }
    return LP_THREAD_EN_UNDEFINED;
}

/**************************************************************************/
/*!
    @brief deletes a specifc low priority task
    @param uint32+t (handle of task) 
*/
/**************************************************************************/
extern LPThreadInitStatus_t del_lwip_task(uint32_t thread_handle){
    if(thread_handle < num_lwip_threads){
        for(uint16_t n = 0; n < num_lwip_threads; n++){
            if(thread_list[n].handle == thread_handle){
                thread_list.erase(thread_list.begin() + n);
                return LP_THREAD_DELETE_SUCCESS; 
            }
        }
    }
    return LP_THREAD_DELETE_UNDEFINED; 
}