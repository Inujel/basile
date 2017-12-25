#pragma once

#include "Basile.h"

/* Everything is in ms */
uint32_t Time_Get(void);
uint32_t Time_ElapsedSince(uint32_t start);
void Time_Wait(uint32_t amount);
