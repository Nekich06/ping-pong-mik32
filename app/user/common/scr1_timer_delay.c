#include "scr1_timer_delay.h"

void SCR1_Timer_Delay(uint32_t us)
{
  uint64_t end_mtimer = SCR1_TIMER_GET_TIME() + us * (SYSTEM_FREQ_HZ / 1000000);
  while (SCR1_TIMER_GET_TIME() < end_mtimer)
    ;
}
