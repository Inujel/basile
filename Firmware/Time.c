#include "Time.h"

static volatile uint32_t time;
void HAL_SYSTICK_Callback(void)
{
   time++;
}

uint32_t Time_Get()
{
   return time;
}

uint32_t Time_ElapsedSince(uint32_t start)
{
   const uint32_t now = time;
   if (now >= start) return now - start;
   return 0xFFFFFFFF - start + now;
}

void Time_Wait(uint32_t amount)
{
   const uint32_t start = time;
   while (Time_Get() < start + amount) { }
}
