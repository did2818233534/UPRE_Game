#ifndef __AK60_H__
#define __AK60_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "can.h"
#include "gpio.h"
	
enum AK60_Control_Mode
{
	DutyMode = 0,     	//占空比模式
	CurrentLoopMode,		//电流环模式
	BreakMode,					//刹车模式
	SpeedLoopMode,			//速度环模式
	PosationMode,				//位置环模式
	SetOriginMode,			//设置原点模式
	PositionSpeedMode,	//位置速度环双环模式
};


//Warning!!!
//When U Use The PositionMode(Or PositionSpeedMode),U Should Set Origion.
//Don't run so long times exceed Rated speed.


void Ak60_currentLoopMode(uint8_t ID, float Current);
void AK60_breakMode(uint8_t ID, float BreakCurrent);
void AK60_speedLoopMode(uint8_t ID, float Speed);
//The range of speed is -1190 to 1190 
//Rated speed is ±420
//Unit: rpm

void AK60_posationMode(uint8_t ID, float Posation);
//The range of posation is -36000 to 36000
//Uint: Angle

void AK60_positionSpeedMode(uint8_t ID, float Posation, int32_t Speed, int32_t Acceleration);
//This mode can control posation, speed and Acceleration at the same time
//The range of speed is -3900 to 3900
//Rated speed is ±420
//Uint: rpm
//The range of acceleration is 0 to 3900
//Unit:rpm/s

void AK60_setOriginMode(uint8_t ID);



#ifdef __cplusplus
}
#endif

#endif 

