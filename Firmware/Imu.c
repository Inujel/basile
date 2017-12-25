#include "Imu.h"
#include <stm32l4xx_hal.h>

#define ADDR   0x68

extern I2C_HandleTypeDef hi2c1;
#define I &hi2c1

uint8_t Imu_ReadRegister(uint8_t addr)
{
   uint8_t result;
   Imu_Read(addr, &result, 1);
   return result;
}

void Imu_WriteRegister(uint8_t addr, uint8_t value)
{
   Imu_Write(addr, &value, 1);
}

void Imu_Read(uint8_t addr, uint8_t* buffer, uint32_t size)
{
   HAL_I2C_Master_Sequential_Transmit_IT(I, (ADDR << 1) + 1, buffer,);
}

void Imu_Write(uint8_t addr, const uint8_t* buffer, uint32_t size)
{
   HAL_I2C_Master_Sequential_Transmit_IT(I, (ADDR << 1), &addr, 1, )
}
