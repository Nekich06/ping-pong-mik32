#ifndef SCR1_TIMER_DELAY_H
#define SCR1_TIMER_DELAY_H

#include "mik32_memory_map.h"
#include "scr1_timer.h"

#define SYSTEM_FREQ_HZ 32000000UL

#define SCR1_TIMER_GET_TIME()                                                  \
  (((uint64_t)(SCR1_TIMER->MTIMEH) << 32) | (SCR1_TIMER->MTIME))

void SCR1_Timer_Delay(uint32_t us) __attribute__((section(".ram_text")));

#endif
