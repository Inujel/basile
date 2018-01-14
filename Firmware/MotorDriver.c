#include "MotorDriver.h"
#include <stm32l4xx_hal.h>
#include "main.h"

#define STEPS_PER_ROUND    200
#define PERIODS_PER_STEP   16
#define TIMER_FREQ         1e6f

static float speed, acceleration;
extern TIM_HandleTypeDef htim1;

void Motor_Initialize(void)
{
}

void Motor_Enable(void)
{
   HAL_TIM_Base_Start(&htim1);
   HAL_TIMEx_OCN_Start(&htim1, TIM_CHANNEL_1);
   MOT_EN_GPIO_Port->ODR &= ~MOT_EN_Pin;
   GPIOA->ODR |= 0xE;
}

void Motor_Disable(void)
{
   MOT_EN_GPIO_Port->ODR |= MOT_EN_Pin;
}

float Motor_GetSpeed(void)
{
   return speed;
}

void Motor_SetAcceleration(float a)
{
   acceleration = a;
}

static void UpdateTimer(void)
{
   const float steps_per_sec = speed * STEPS_PER_ROUND;
   const float periods_per_sec = steps_per_sec * PERIODS_PER_STEP;
   const uint32_t reload = (uint32_t)(TIMER_FREQ / periods_per_sec);
   htim1.Instance->ARR = reload;
   htim1.Instance->CCR1 = reload / 2;
}

void Motor_Process(float dt)
{
   speed += acceleration * dt;
   UpdateTimer();
}


