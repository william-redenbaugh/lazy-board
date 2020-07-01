#ifndef _LWIP_THREAD_HPP
#define _LWIP_THREAD_HPP

// Default inclusion libraries. 
#include "Arduino.h"
#include "ChRt.h"
#include "lwip_thread.hpp"

// Maximum number of low priority work threads available
#define NUM_MAX_LPWTHREADS 256

enum LPThreadInitStatus_t{
    LP_THREAD_INIT_SUCCESS, 
    LP_THREAD_INIT_FAILIURE_UNKOWN,
    LP_THREAD_REPLACE_UNDEFINED, 
    LP_THREAD_DELETE_UNDEFINED, 
    LP_THREAD_DELETE_SUCCESS,
    LP_THREAD_EN_UNDEFINED,
    LP_THREAD_EN_SUCCESS, 
    LP_THREAD_INIT_FAILIURE_MAX_THREAD
};

struct LPThreadInitReturn{
    uint32_t thread_handle = 0; 
    LPThreadInitStatus_t init_status; 
};

// Starting up our low priority work thread. 
extern void setup_lwip_thread(void);
extern LPThreadInitReturn add_lwip_task(void (*func)(void *ptr),  void *args, systime_t interval);
extern LPThreadInitStatus_t disable_lwip_task(uint32_t thread_handle);
extern LPThreadInitStatus_t enable_lwip_task(uint32_t thread_handle);
extern LPThreadInitStatus_t del_lwip_task(uint32_t thread_handle);
#endif