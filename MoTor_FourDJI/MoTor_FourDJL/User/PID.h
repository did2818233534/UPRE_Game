#ifndef __PID_Speed_H__
#define __PID_Speed_H__


#include "main.h"
#include "can.h"
#include "usart.h"
#include "gpio.h"

//struct PID_Current_HandleTypeDef
//{
//	float err;
//	float err_last;
//	float integral;
//	float Current_set;
//	float pout;
//	float iout;
//	float dout;
//};


struct PID_Speed_HandleTypeDef
{
	float err;
	float err_last;
	float integral;
	float Current_set;
	float FilterSpeed;
	float FilterSpeed_last;
	float pout;
	float iout;
	float dout;
};


struct PID_Angle_HandleTypeDef
{
	float err;
	float err_last;
	float integral;
	float speed_set;
	float Current_set;
	float pout;
	float iout;
	float dout;
};


struct PID_Angle_and_Speed_HandleTypeDef
{
	float err;
	float err_last;
	float integral;
	float speed_set;
	float Current_set;
	float pout;
	float iout;
	float dout;
};


//struct PID_Current_HandleTypeDef* pid_Current(uint8_t ID,float Torque_expect,
//																int16_t current_now,
//																float kpC, float kiC, float kdC);

void pid_Speed(uint8_t ID,struct PID_Speed_HandleTypeDef* PID_Speed,
							float speed_expect,
							int16_t speed_now,
							float kpV, float kiV, float kdV);

void pid_Angle(uint8_t ID,
							struct PID_Angle_HandleTypeDef* PID_Angle,struct PID_Speed_HandleTypeDef* PID_Speed,
							float angle_expect,
							float angle_now, int16_t speed_now,
							float kpA, float kiA, float kdA,
							float kpV, float kiV, float kdV);

														
void pid_Angle_and_Speed(uint8_t ID,
												struct PID_Angle_and_Speed_HandleTypeDef* PID_Angle_and_Speed,struct PID_Speed_HandleTypeDef* PID_Speed,
												float angle_expect,float speed_expect,
												int16_t angle_now, int16_t speed_now,int16_t Current_now,
												float kpA, float kiA, float kdA,
												float kpV, float kiV, float kdV);											

#endif
