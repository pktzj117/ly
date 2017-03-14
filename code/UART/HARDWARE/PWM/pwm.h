#ifndef __PWM_H
#define __PWM_H
#include "sys.h"



void TIM3_PWM_Init(u16 arr,u16 psc);
void TIM_SetTIM3Compare1(uint32_t compare);
void TIM_SetTIM3Compare2(uint32_t compare);
void TIM_SetTIM3Compare3(uint32_t compare);
void TIM_SetTIM3Compare4(uint32_t compare);
#endif
