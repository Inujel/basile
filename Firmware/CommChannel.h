#pragma once

#include "Basile.h"

void CommChannel_Initialize(void);
void CommChannel_SetBaudRate(uint32_t rate);
uint32_t CommChannel_Write(const void* data, uint32_t size);
uint32_t CommChannel_Read(void* data, uint32_t size);
