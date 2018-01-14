#include "Imu.h"
#include "I2CDev/MPU6050/MPU6050.h"
#include "Log.h"
#include "Time.h"
#include <stm32l4xx_hal.h>

static int datacount;

extern I2C_HandleTypeDef hi2c1;

void Imu_Initialize()
{
   bool mpu6050ok = HAL_I2C_IsDeviceReady(&hi2c1, 0x68 << 1, 10, 100) == HAL_OK;
   if (mpu6050ok)
   {
      MPU6050(0x68);
      MPU6050_initialize();
      if (MPU6050_testConnection())
         Log_Info("MPU6050 OK");
      else
         Log_Error("Connection to MPU6050 failed");
      MPU6050_setDLPFMode(MPU6050_DLPF_BW_98);
      MPU6050_setRate(9);
   }
   else
      Log_Error("MPU6050 isn't connected");
}

void Imu_Process()
{
   if (MPU6050_getIntDataReadyStatus())
   {
      int16_t ax, ay, az, gx, gy, gz;
      MPU6050_getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
      float axf = ax / 32678.0f, ayf = ay / 32678.0f, azf = az / 32678.0f, gxf = gx / 32678.0f, gyf = gy / 32678.0f, gzf = gz / 32678.0f;
      //Log_Info("IMU: %1.3f %1.3f %1.3f %1.3f %1.3f %1.3f", axf, ayf, azf, gxf, gyf, gzf);
      datacount++;

      if (datacount % 100 == 0)
      {
         Log_Info("%d data in %d ms => %4.2f Hz", datacount, Time_Get(), (float)datacount * 1000.0f / Time_Get());
      }
   }
}
