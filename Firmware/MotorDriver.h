#pragma once

#include "Basile.h"

void Motor_Initialize(void);
void Motor_Enable(void);
void Motor_Disable(void);
float Motor_GetSpeed(void);   // rps
void Motor_SetAcceleration(float a);
void Motor_Process(float dt);
