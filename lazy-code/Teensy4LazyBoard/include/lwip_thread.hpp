#ifndef _LWIP_THREAD_HPP
#define _LWIP_THREAD_HPP

// Default inclusion libraries. 
#include "Arduino.h"
#include "ChRt.h"
#include "lwip_thread.hpp"

// Maximum number of low priority work threads available
#define NUM_MAX_LPWTHREADS 64

enum LPThreadInitStatus_t{
    LP_THREAD_INIT_SUCCESS, 
    LP_THREAD_INIT_FAILIURE_UNKOWN, 
    LP_THREAD_INIT_FAILIURE_MAX_THREAD
};

// Starting up our low priority work thread. 
extern void setup_lwip_thread(void);
extern LPThreadInitStatus_t add_lwip_task(void (*func)(void *ptr),  void *args, systime_t interval);

#endif