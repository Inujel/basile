#pragma once

#include "Basile.h"

typedef enum
{
   HC06_BaudRate_Unknown,
   HC06_BaudRate_1200,
   HC06_BaudRate_2400,
   HC06_BaudRate_4800,
   HC06_BaudRate_9600,
   HC06_BaudRate_19200,
   HC06_BaudRate_38400,
   HC06_BaudRate_57600,
   HC06_BaudRate_115200,
} HC06_BaudRate;

uint32_t HC06_GetBaudRateValue(HC06_BaudRate rate);
bool HC06_CheckCommunication(void);
HC06_BaudRate HC06_GuessBaudRate(void);
bool HC06_SetBaudRate(HC06_BaudRate rate);
void HC06_SetName(const char* name);
void HC06_GetVersion(char* buffer);

