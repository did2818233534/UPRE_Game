#ifndef __DJ_bsp_h__
#define __DJ_bsp_h__

#include "main.h"
#include "can.h"
#include "usart.h"
#include "gpio.h"

#define DJ_Angle ((int16_t)((DATA_R[0] << 8) + DATA_R[1]))*360/8192
#define DJ_Speed (int16_t)((DATA_R[2] << 8) + DATA_R[3])
#define DJ_Current (int16_t)((DATA_R[4] << 8) + DATA_R[5])


void Motor_Set_Current(uint16_t *DATA);

#endif

