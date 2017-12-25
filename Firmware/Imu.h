#pragma once

#include "Basile.h"

uint8_t Imu_ReadRegister(uint8_t addr);
void Imu_WriteRegister(uint8_t addr, uint8_t value);
void Imu_Read(uint8_t addr, uint8_t* buffer, uint32_t size);
void Imu_Write(uint8_t addr, const uint8_t* buffer, uint32_t size);

